# DKC Vision & Long-Term Directions

Where the research is going, beyond the next few demos. Updated during defrag
or when significant vision discussions happen in conversation.

---

## The Hybrid LLM Thesis

**Core idea:** A language model where some knowledge is *compiled* (from taxonomy
and syllogisms, provably correct, dynamically updatable) and some is *trained*
(from corpus, statistically approximate, fixed after training).

**The pipeline:**
```
Taxonomy / Syllogism Graph  →  Knots / Braids  →  DKC  →  Exact Compiled Weights
                                                              ↓
Small Corpus  →  Statistical Training  →  Trained Weights  →  Hybrid Model
```

**Why this matters:**
- Current LLMs encode ALL knowledge statistically. Facts can hallucinate.
- Context stuffing (RAG) is a bandaid — limited, expensive per-inference.
- Fine-tuning is expensive, slow, and catastrophically forgets.
- Compiled weights would be provably correct, dynamically updatable, and
  don't consume context window.

**The DKC strengths that enable this:**
- No training needed → fast recompilation (add fact → recompile → done)
- Exactness → facts that can't hallucinate
- Finite catalog → bounded cost per fact
- Interpretability → removable facts (trace which weights encode which relations)

**Execution model:**
```
words → tokens → net → tokens → words
```
This is the fundamental loop. Words are both the input format and output format.
The net is the compiled+trained hybrid. One limitation of current LLMs: the
finite token set means you cannot use words outside the vocabulary as input
or output.

**The value proposition:** An LLM where you can add, remove, and correct factual
knowledge in real-time without retraining, without context stuffing, and with
provable correctness for the hard-fact portion.

**Philosophical grounding:** Maps onto the scholastic distinction between
*scientia* (demonstrative knowledge from first principles — compiled) and
*opinio* (probable knowledge from observation — trained). The taxonomy IS
the first principles; the corpus IS the observations.

---

## Opaque Tokens

**Core insight:** Because we compile our net and have granular transparency of
its execution, we can introduce a fundamentally new kind of token — "opaque
tokens" (also: passthrough tokens, argument tokens).

A syllogism (or any logical function) does not need to know the *value* of its
arguments — only their logical nature (term, predicate, subject, etc.). "All X
are Y, all Y are Z, therefore all X are Z" works regardless of what X, Y, Z are.

**Mechanism:** Opaque tokens flow through the compiled portion of the net. The
net operates on logical structure, not content. Execution tracks which input
positions flow to which output positions. At output, opaque tokens are replaced
with the original input values.

This is essentially **type-level computation** — the net reasons about
relationships between slots, not about what fills the slots.

**Implications:**
- Breaks the finite-vocabulary limitation. New proper nouns, technical terms,
  any string can be an opaque token. The net doesn't need to have seen "bears"
  in training to reason about bears.
- Enables **graceful ignorance**: if the net doesn't have a compiled subnet for
  a particular entity (e.g., "are bears mortal?"), it can recognize that it
  lacks the relevant syllogism and use an opaque token in its reply to
  express "I don't know if bears are mortal" rather than hallucinating.
- The compiled portion becomes a **logical skeleton** that the statistical
  portion fills with content.

**Status:** CONCEPTUAL. No implementation yet. Requires: (1) the compiled net
to actually exist, (2) an execution model that tracks token flow through
compiled weights, (3) the substitution mechanism at output.

---

## Syllogism Presentation as Training

**Core idea:** Present syllogisms to the net in natural language ("all cats are
mammals"), and the net compiles that syllogism and dynamically adds it to its
weights.

This means **presenting syllogisms IS a training approach** — but fundamentally
different from gradient-based training:
- No loss function. No backpropagation. No epochs.
- Compilation is deterministic: same syllogism always produces same weights.
- Adding a syllogism doesn't disturb existing knowledge (no catastrophic forgetting).
- Each fact is independently addressable (can remove "all cats are mammals"
  without affecting "all dogs are mammals").

**Example flow:**
```
Input: "all bears are mammals, all mammals are mortal"
  → parse into syllogistic form (Barbara: A-a-A)
  → map to NPN class / Boolean function
  → compile to DKC weights
  → add to compiled weight region
  → net can now answer "are bears mortal?" → yes (via compiled pathway)
```

**Status:** CONCEPTUAL. Depends on: Gap 2 (syllogism → Boolean mapping),
deterministic NL parser (see below), dynamic weight insertion mechanism.

---

## Wired Integration (Not Pipelining)

**Critical architectural point:** The compiled DKC portion and the traditional
trained portion are NOT two separate models with a token-level interface between
them. They are **literally wired together into a single set of weights with
connected pathways.**

This is not:
```
compiled_net(input) → tokens → trained_net(tokens) → output    ← NO, not this
```

This is:
```
single hybrid net where some weight regions are compiled (exact, from DKC)
and some weight regions are trained (statistical, from corpus), with shared
pathways between them
```

**Why this matters:**
- Pipeline approaches lose information at the token bottleneck between stages.
- Wired integration means the compiled weights influence activation patterns
  throughout the network, not just at a handoff point.
- The statistical regions can "lean on" the compiled structure during inference.

**The acceleration thesis:** Having compiled syllogisms as part of the net's
weights will help the "traditional" trained regions become smarter much more
rapidly on an extremely small training corpus. The compiled structure provides
scaffolding — logical relationships that the statistical training doesn't need
to discover from data because they're already in the weights.

**If we can decompile traditional LLMs to knots** (reverse DKC, Demos 26-28),
we should be able to wire compiled and trained portions together — because both
are expressible in the same algebraic framework (bracket values / cyclotomic
integers).

**Status:** HIGHLY SPECULATIVE. Requires: reverse DKC to actually work cleanly
(it doesn't yet — phase decoherence problem from D46), a compatible weight
format, and an integration architecture.

---

## Deterministic Natural Language Parser

**Requirement:** To feed syllogisms into the compiled net, we need a framework
for deterministic natural language parsing that converts natural language into
the right token/opaque-token structure.

**Development path:**
1. Start simple: classic syllogistic grammar. "Aristotle is a man."
   "All men are mortal." Subject-copula-predicate forms.
2. Extend to more complex syllogistic structures: hypothetical syllogisms,
   disjunctive syllogisms, sorites (chains).
3. Eventually: parse a paragraph of natural language text into a data structure
   ready to feed into the compiled net's token/opaque-token format.

**Connection to rhubarb:** We already have a parser generator (lapifex) and
C89 string libraries (chorda). A deterministic syllogism parser is a natural
rhubarb library. This could be one of the first pieces of the pipeline that
actually gets built.

**Status:** NOT STARTED. But the tools exist in rhubarb to build it.

---

## Key Gaps (Ordered by Dependency)

### Gap 1: Taxonomy → Braid Encoding
How do you take a taxonomic/syllogistic structure and produce a braid whose
bracket value encodes the right logical operation? This is the biggest unknown.

Suggestive connections:
- TL diagrams are planar matchings (pairings). Taxonomic categories group things.
- The braid group connects to partial orders and lattice structures. A taxonomy
  IS a partial order.
- Refinement of set partitions (coarser → finer categories) has lattice structure
  that might map to the TL lattice.
- Classical syllogisms are 3-term logical operations. DKC handles 3-input
  Boolean functions. Natural fit in dimensionality.

Status: UNEXPLORED. Needs a dedicated investigation (demo or research arc).

### Gap 2: Syllogism → Boolean Function Mapping
Can the 24 valid syllogistic forms be mapped to specific NPN classes? This
is potentially answerable with existing tools — enumerate the syllogistic
forms, express each as a truth table, classify by NPN equivalence, check
which ones DKC can compile.

Status: APPROACHABLE. Could be a single demo.

### Gap 3: Multi-Layer DKC Composition
Can the output of one DKC neuron feed into another while maintaining exactness?
Demo 19 showed cascade thresholding works (collapse to bit at each stage).
The richer question: can you pass the full complex value through?

Status: PARTIALLY EXPLORED (Demo 19 cascade). Full complex composition UNTESTED.

### Gap 4: Integration Architecture
How do compiled weights coexist with trained weights in the same model?
The answer (per vision discussions): NOT pipelining. Literally wired together
at the weight level. Compiled regions and trained regions share pathways.

Key sub-questions:
- What does "wired together" mean concretely? Shared activation layers?
  Compiled neurons in the same layer as trained neurons?
- How does the opaque token mechanism interact with attention?
- Can reverse DKC (D26-28) provide the bridge format for integration?
- What's the right loss function for the trained portion that respects
  the compiled structure?

Status: CONCEPTUAL. The architectural direction is clearer (wired, not
pipelined) but implementation details are unexplored.

### Gap 5: Dynamic Recompilation
Can you update compiled weights fast enough for "dynamic" to be meaningful?
DKC compilation is currently exhaustive search over a finite catalog — fast
for 3 inputs (~100^4 = 100M, seconds), unclear for larger inputs.

Status: DEPENDS ON scaling (Gap 6).

### Gap 6: Scaling Beyond 3 Inputs — FURTHER RESOLVED (D76-D82)
Demo 63 extended DKC to 4-input (k=8, 96 solutions) and 5-input (k=15, 3020
solutions) parity, establishing the parity ceiling at n=5 for Z[zeta_8]. The
triskelion generalization (k=2n) was FALSIFIED; the true scaling law is
k=2M-1 where M is the ray count.

Demo 65 confirmed Z[zeta_16] does NOT raise the parity ceiling — it stays at
n=5 (tier thresholds identical: XOR3 k=6, XOR4 k=8, XOR5 k=14). The gap-of-2
hypothesis was REFUTED: pigeonhole rises from 7 to 14 but actual ceiling stays
at 5, widening the gap to 9. This supports the Conjecture (Universal Ceiling)
that the n=5 bound is topological, not algebraic.

For the original vision, this meant:
- **Individual DKC neurons handle up to 5-input parity** (the hardest function).
  Simpler functions (AND, OR, MAJ) likely have higher ceilings.
- **3-input syllogisms are well within capacity** — the natural fit between
  syllogisms and DKC is confirmed by the n=5 ceiling.
- **Larger functions require composition** — multi-layer DKC (Gap 3) was
  thought to be the path for anything beyond 5 inputs.
- **The ceiling is algebra-independent** — Z[zeta_16] and Z[zeta_8] agree
  exactly. The wall appears to be topological.

**UPDATED — D76-D82 substantially revised this picture:**

D77 showed that XOR8 IS solvable at ζ₈ with S¹×S² product activation
(Sec(8)×Voronoi, 112 cells, 6 solutions). The n=5 ceiling was
activation-specific: it was the S¹-only sector activation that capped at
XOR5, not the root of unity. D78 then confirmed that XOR10 = 0 at ζ₈ even
with Sec×Vor activation — ζ₈ is a finite group (24 elements, binary octahedral,
E₇) and its combinatorial catalog diversity is genuinely exhausted at XOR8.

D79 broke through the ζ₈ ceiling by switching to ζ₁₂ (infinite group): XOR10
= 124 winners, XOR12 = 50+ winners. The finite-vs-infinite group distinction is
the key variable, not the subscript on ζ.

D82 identified the **fundamental variable governing XOR capacity: crossing
depth**. The linear depth law is:

```
max_xor ≈ depth + 6
```

Each unit of crossing depth (= generator multiplications = braid crossings)
adds a fixed increment to computational capacity. The Demo 81 logarithmic law
(0.62 XOR per doubling of catalog size at ζ₁₂) is a corollary: catalog grows
as ~2^depth, so log₂(catalog) ≈ depth, giving max_xor ≈ 0.62 × log₂(catalog).
The logarithmic law was an artifact of exponential catalog growth — depth is the
real variable.

D82 also demonstrated **algebraic coherence beats raw vocabulary**: a deep
subset of 564 entries (fewer directions, fewer angles) outcomputes a strided
subset of 564 entries with maximum vocabulary coverage, reaching XOR12 vs
XOR10. The determining factor is shared intermediate products from generator
multiplication chains — deep entries carry algebraic coherence, not just
geometric coverage.

**Revised vision implications:**

- **ζ₈ (finite group, 24 elements, E₇)**: ceiling at XOR8 with S¹×S² activation.
  The ADE classification identifies this as the binary octahedral group. Quantum
  dimension [2]_q vanishes at ζ₈ — this is the most singular point of quantum
  group parameter space.
- **ζ₁₂ (infinite group)**: XOR12 reachable with ~1140-entry catalog; XOR14
  predicted at ~38K entries. Capacity grows logarithmically with catalog size.
- **Depth law: max_xor ≈ depth + 6**: the deepest entries (8 generator
  multiplications deep) are 2× more efficient than shallow or strided entries
  at equal count. More complex knots = more computational power.
- **Implication for multi-layer DKC (Gap 3)**: multi-layer composition may
  not be needed if deep infinite-group catalogs can reach high XOR targets
  directly in a single neuron. The architectural question shifts from
  "how do we compose neurons?" to "what is the right depth profile for the
  catalog?"

Status: ζ₈ ceiling confirmed at XOR8. ζ₁₂ breaks the wall with logarithmic/
depth-linear scaling. The fundamental capacity variable is crossing depth, not
catalog vocabulary or root-of-unity subscript.

### Gap 7: The Wall Is Two-Layered — NEW (D65)

The XOR6 wall has two distinct layers that require different tools:

- **Absolute geometric wall (k≤23)**: No binary partition of any number of
  sectors (for any k≤23) can separate the XOR6 truth table using Z[zeta_8]
  weights. PROVEN exhaustively. This is a topological obstruction — the
  self-doubling impossibility theorem (D64) and the matroid minor chain
  structure (D64) explain WHY the wall exists. The minimum Hamming distance
  from any 6-weight candidate to XOR6 is d=4, with no candidate achieving
  d=0,1,2,3.

- **Convention wall (standard labeling fails, custom succeeds)**: At k=24,
  generalized binary labelings (not just standard odd/even) break through.
  Exactly 3 of 2700 candidate pairs admit XOR6 at k=24. The standard
  odd/even convention used since Aizenberg is one specific choice; relaxing it
  reveals that the hard wall is at k<=23, not at all k.

**Implication for the hybrid LLM**: When designing compiled DKC neurons, the
activation function design matters. The standard odd/even convention may not be
optimal; the principled approach is to derive the activation from the underlying
geometry (Voronoi cells on S²) rather than treating it as a fixed convention.

Status: Two-layer structure PROVEN. The convention wall is now understood as
an activation design choice, not a fundamental limit.

### Gap 9: Finite vs. Infinite Group Choice — NEW (D80)

D80 established that under the standard two-generator construction, only ζ₄
and ζ₈ generate finite SU(2) subgroups. All other tested roots (ζ₆, ζ₁₀,
ζ₁₂, ζ₁₆, ζ₂₀) generate infinite groups. The boundary is non-monotonic:
ζ₆ is infinite despite sitting between the two finite cases.

**The two computational regimes:**

- **Finite-group roots (ζ₄, ζ₈)**: structured, small catalog with complete
  algebraic closure; every entry has an exact place in the ADE classification
  (ζ₄ → Binary Dihedral Q₈, ζ₈ → Binary Octahedral E₇); hard capacity
  ceiling because combinatorial diversity is genuinely exhausted. At ζ₈,
  the quantum dimension [2]_q = q + q⁻¹ = i + (-i) = 0: this is the most
  singular point in quantum group parameter space, where the TL category
  becomes maximally non-semisimple. The computation is tractable (Kuperberg:
  lattice roots are outside the #P-hard regime).
- **Infinite-group roots (ζ₁₂, ζ₁₀, ζ₁₆, ...)**: catalog grows without
  bound; capacity scales logarithmically with catalog size (depth-linearly
  with crossing depth); no hard ceiling within feasible search. These roots
  approach dense subgroups of SU(2) — the directions eventually cover S²
  uniformly.

**The architecturally fundamental question:**

Can you get the best of both worlds — the structural guarantees of a finite
group (every entry has an exact algebraic meaning; catalog is fully
characterizable; computation is provably tractable) combined with the scaling
capacity of an infinite group (no ceiling; depth drives power)?

Possible directions:
- **Union catalog** (ζ₈ + ζ₁₂): the two groups are largely non-overlapping
  (only 12% quaternion overlap, 15% direction overlap by D79). A union might
  combine finite-group structural guarantees for the ζ₈ component with
  infinite-group capacity for the ζ₁₂ component. Whether this provides
  superadditive capacity is an open question.
- **Finite substructure extraction**: even within an infinite group, there
  may be finite sub-catalogs with desirable structural properties (e.g., all
  entries up to depth d form a well-characterized set). The depth law
  (D82) already shows these subsets have predictable capacity.
- **ADE gaps**: E₆ (binary tetrahedral, 24 elements) and E₈ (binary
  icosahedral, 120 elements) are NOT reached by the two-perpendicular-axes
  construction. A different generator construction might access them,
  potentially providing finite groups with higher capacity than ζ₈.

**Power-of-two finiteness conjecture (D80):** ζ₂ₖ for k ≥ 1 (i.e., ζ₄,
ζ₈, ζ₁₆, ζ₃₂, ...) may all be finite under this construction. D80 confirmed
the first two; ζ₃₂ is the next test.

Status: Two-regime structure established. Union catalog, ADE gaps, and
power-of-two conjecture are the active research directions.

### Gap 8: Quaternionic DKC and the Bloch Sphere — NEW (D66-D71)

D66-D71 opened a wholly new dimension: the computation lives on S² (the Bloch
sphere / rotation axis direction), not on S¹ (complex circle) or S³ (full
unit quaternion). This is not a minor generalization — it changes the geometry
of DKC entirely.

Key discoveries:
- Kauffman braid representations in SU(2) produce exactly the 24 vertices of
  the 24-cell (binary octahedral group). The bracket and quaternion are
  complementary projections of the same braid group representation.
- The Hopf fibration S³ → S² is respected: the Hopf fiber (phase ξ₁) is
  computationally INERT — it carries zero DKC information at all resolutions
  tested. The computation lives entirely on the S² Hopf base (eigenvector
  directions, i.e., the rotation axis).
- The natural computational object is the 13-direction eigenvector Voronoi on
  S². 14 cells suffice for 36 XOR6 solutions — more than the 25-cell S³
  Voronoi (35 solutions) or the 25-cell S¹ approach.
- The 13=13 theorem (D71): minimum bandwidth l=6 exactly because 2×6+1=13
  equals the number of eigenvector directions. This is the cleanest theoretical
  result yet.

**Implication for the hybrid LLM**: DKC neurons can be understood as computing
on the Bloch sphere S²=CP¹, the state space of a qubit. This connects compiled
DKC weights directly to quantum information theory — the 13-direction Voronoi
may correspond to a specific quantum measurement basis. Whether this connection
is formal or merely structural is an open research question.

Status: ACTIVE RESEARCH FRONTIER. Multiple open questions across D66-D71.

---

## Near-Term Explorations (Next Few Demos)

1. **Syllogism → NPN mapping** — Enumerate 24 valid syllogistic forms, express
   as truth tables, classify by NPN, check DKC compilability. Single demo.
   Would confirm or deny the natural fit between syllogisms and 3-input DKC.

2. **Multi-layer composition test** — Feed DKC neuron output into another DKC
   neuron. Test both cascade-threshold (Demo 19 style) and full-complex-value
   passing. Single demo.

3. ~~**4-input DKC** (Demo 63, already planned)~~ — **DONE** (Demo 63).
   4-input parity at k=8 (96 solutions), 5-input at k=15 (3020 solutions),
   parity ceiling n=5, triskelion FALSIFIED, oriented matroid classification.

   ~~**Parity matroid recursion** (Demo 64)~~ — **DONE**. Funnel-shaped matroid
   minor chain, self-doubling impossibility theorem, two-layer wall anatomy.

   ~~**Clifford staircase / Z[zeta_16]** (Demo 65)~~ — **DONE**. Parity ceiling
   algebra-independent. Two-layer wall structure. Generalized XOR6 at k=24.

   ~~**Quaternionic DKC** (Demos 66-71)~~ — **DONE**. 24-cell emergence, Hopf
   inertness, S² as computational home, 13-direction Voronoi, 13=13 spectral
   theorem, musical mapping, direction nesting theorem, stereographic proof of
   intrinsic curvature.

4. **Toy taxonomy → weight pipeline** — Take a 5-node taxonomy, manually encode
   as braids (even if the encoding is ad hoc), compile to DKC weights, verify
   logical inference works end-to-end. Proof of concept, not general solution.

5. **Deterministic syllogism parser** — Using lapifex/chorda, build a parser
   that converts "All X are Y" / "Aristotle is a man" forms into structured
   representations with opaque token slots. First rhubarb library in the
   vision pipeline.

6. **Opaque token flow demo** — Take a compiled syllogism, feed it input with
   opaque tokens (e.g., subject=OPAQUE_1, predicate=OPAQUE_2), verify the net
   produces correct logical output with opaque tokens in the right positions,
   then substitute back to get the natural language answer.

7. **ζ₁₂ with Sec×Vor activation** — D77 showed Sec(8)×Voronoi unlocks
   XOR8 at ζ₈. D79/D82 showed ζ₁₂ reaches XOR12 with the same activation
   family. The combination has not been rigorously tested: apply D77's
   activation-zoo methodology to ζ₁₂ to find the minimum k for XOR10 and
   XOR12 and confirm the activation determines capacity principle holds at the
   infinite-group scale.

8. **ζ₃₂ finiteness test** — D80's power-of-two conjecture predicts ζ₃₂
   (θ = π/16) generates a finite SU(2) subgroup. A single test_root(32)
   call would confirm or refute. If finite, it would be a new ADE-type member
   between E₇ (ζ₈) and potentially E₈ territory, with its own capacity
   ceiling higher than ζ₈ but tractable.

9. **Direct deep-entry generation at ζ₁₂** — D82's key insight: deep entries
   (depth ≥ 6) are twice as efficient as shallow or strided entries at the same
   count. But the current approach generates them by building the full BFS
   closure through all shallower rounds. D82 asks: can depth-d entries be
   generated directly from their algebraic properties (shared intermediate
   products, angular refinement structure) without computing all shallower
   depths? This would make ζ₁₂'s high-XOR regimes computationally accessible
   without exponential catalog growth.

---

## New Research Axes (from D64-D71)

### 1. Quaternionic DKC as a Research Axis

D66-D71 established that the computation is intrinsically quaternionic and
lives on S². This is a genuinely new dimension of the research, not an
extension of the S¹ (complex) approach:

- **Bloch sphere = qubit state space = DKC computation space.** The S²=CP¹
  identification is not incidental. The rotation axis of a unit quaternion
  IS the Bloch sphere representation of a qubit. DKC with quaternionic weights
  is computing on the same geometric object that quantum information theory uses
  to represent single-qubit states.
- **The 13-direction Voronoi as a quantum measurement basis.** The 13
  eigenvector directions of the binary octahedral group form a specific
  structured arrangement on S². Whether these 13 directions correspond to a
  known quantum measurement basis (e.g., a SIC-POVM or a mutually unbiased
  basis construction) is unexplored.
- **24-cell as the natural computational polytope.** The 24-cell is unique in
  4D: the only regular self-dual polytope with no 3D analogue. Its 24 vertices
  arise naturally from SU(2) braid representations. The F4 symmetry group (576
  elements) decomposes XOR6 solutions into exactly 6 orbits, which may reflect
  the partition structure of XOR arity (6 = 1+2+3).
- **Hopf fibration as a computational factorization.** The fiber phase carries
  nothing; the base carries everything. This means DKC naturally factors through
  the Hopf fibration: braid → SU(2) quaternion → S² direction → computation.

### 2. Spectral Theory of DKC

D71 gave the first frequency-domain characterization of DKC. The 13=13 theorem
is the cleanest theoretical result in the arc:

**Statement**: The minimum spherical harmonic bandwidth for XOR6 DKC on the
13-direction S² eigenvector Voronoi is exactly l=6, because 2l+1 = 13 = the
number of eigenvector directions. The phase transition at l=6 is sharp (0%
recovery at l≤5, 100% at l=6) and all-or-nothing.

**Why this matters for the vision**:
- It explains, from first principles, WHY 14 cells suffice (compressed sensing
  + known support) and WHY the specific number 13 appears (degrees of freedom
  count at minimum bandwidth).
- It opens a spectral approach to DKC that is orthogonal to the algebraic
  (bracket value / cyclotomic integer) approach. Two independent frameworks for
  the same object strengthen the theoretical foundation.
- The spectral universality (all 36 XOR6 winners share the same spectral shape:
  l=6 dominant at ~80%) means the spectrum is a structural invariant of XOR6
  solutions, not a per-solution artifact. This could enable spectral
  classification of what DKC can and cannot compute.
- The trivialization prediction works: for Z[zeta_16] with 3,457 directions,
  the DOF formula predicts l≥1728 minimum bandwidth, explaining the ~90%
  solution rate at Z[zeta_16] resolution. The spectral framework predicted this
  correctly from the direction count alone.

### 3. Music as an Application and Outreach Direction

D70 showed that the Z[zeta_8] algebra produces a natural musical system:
4 eigenvalue angles → 4 notes (C, Eb, E, F#), spanning maximum consonance
(unison) to maximum dissonance (tritone). Braid words become melodies; the
Yang-Baxter relation σ₁σ₂σ₁ = σ₂σ₁σ₂ produces identical melodies step by
step (not just at the final note). Dissonance IS the antipodal map on the
dodecahedron (tritone pairs = antipodal faces).

**Potential directions**:
- Knot invariants as a composition algorithm: the Garside element melody
  (Eb E F# E Eb C) is a complete rising-falling arch visiting all 4 knot notes.
  Different knot types produce structurally distinct melodies.
- Artistic collaborations: composers working with algebraic constraint rather
  than pure taste.
- Educational tools: music as an entry point to braid group theory, the Yang-
  Baxter equation, and knot theory. The Yang-Baxter identity becomes audible.
- Accessibility: DKC results that are otherwise abstract (bracket values,
  cyclotomic integers) become perceivable through pitch and rhythm.

### 4. Matroid Theory Integration

D64 showed the parity hierarchy {XOR2, XOR3, XOR4, XOR5} is a matroid minor
chain with perfect downward deletion-contraction closure. The 44 good / 31
poison value split in the catalog vocabulary is structurally unexplored but
possibly related to Reiner's cyclotomic matroids (mu_8, arXiv:math/0402206):

- Deletion: removing any weight from XOR_n gives XOR_{n-1} (100%).
- Contraction: fixing any input to 1 gives XNOR_{n-1} (100%).
- The 44 good values (extensible upward) vs. 31 poison values (orphan-only)
  may be the matroid's independent sets vs. circuits.
- Poison values avoid octant 4 entirely — a geometric constraint on the
  circuit structure.

**Why this matters**: Matroid theory provides a combinatorial language for
talking about which weight sets are "compatible" for extension. A matroid
characterization of the DKC catalog would:
- Unify the combinatorial (which sets work?) and algebraic (why do those
  values appear in the catalog?) sides of DKC.
- Provide a polynomial-time algorithm for testing extensibility (matroid
  membership tests are poly-time).
- Connect to the geometric (S² Voronoi) side via oriented matroid theory
  (oriented matroids = chirotopes = sign patterns of determinants, which are
  exactly what sector classification computes).

Status: UNEXPLORED. Reiner connection flagged. Needs a dedicated investigation.

### 5. Activation Function Design Theory

The progression across D50-D71 suggests a principled theory of activation
function design grounded in representation theory:

```
Half-plane (binary threshold)
  → Sector (k-sector MVN, Aizenberg)
    → Generalized sector (non-standard binary labeling of k sectors, D65)
      → Voronoi on S¹ (k=24, D65)
        → Voronoi on S³ (24-cell, 25 cells, D66)
          → Voronoi on S² (13-direction eigenvector, 14 cells, D67)
            → Spectral (bandwidth l=6, 13=13 theorem, D71)
```

Each step reduces cell count while maintaining or improving XOR6 solvability.
The endpoint — spectral characterization via spherical harmonics — provides an
analytical framework that transcends the enumeration approach.

**The 13=13 theorem provides the DOF count**: minimum bandwidth = minimum cell
count needed for the computation, set by the number of data-intrinsic
directions. This is a principled design criterion:
- Don't choose activation cell count by convention (Aizenberg's odd/even) or
  by grid resolution (k sectors). Derive it from the data-intrinsic directions.
- The spectral bandwidth l tells you the minimum representation complexity of
  the labeling function. Functions below this bandwidth cannot be expressed;
  functions at or above it can.
- The representation-theoretic source of the directions (binary octahedral
  group, braid group SU(2) image) determines what bandwidth is needed.

This bridges pure math (representation theory, spectral analysis on S²) and
practical ML architecture (how many neurons / activation cells do you need?).

### 6. Depth-Based Catalog Engineering

D82's central insight: algebraic coherence beats raw vocabulary. The depth law
(max_xor ≈ depth + 6) means that what matters is not how many directions or
angles the catalog covers, but how deeply the entries are constructed as
products of generators.

**What this suggests for catalog design:**

The current approach is BFS enumeration — build the full group closure round by
round, accept all resulting entries. This is expensive (exponential in depth)
and produces a catalog with most entries at the deepest rounds (D82 data:
round 8 alone contributes 44% of all 4096 entries). But the shallow rounds are
necessary because:
1. Depth-0 entries (generators + identity) appear in every winner at every XOR
   level — they are the invariant skeleton.
2. The recursive structure (XOR8 winner = XOR6 triple + shadow) means shallower
   entries propagate into deeper selections.

**The engineering question:** Is there a catalog construction that generates
entries with depth ≥ d directly, without the exponential intermediate cost?
Characteristics of deep entries that might be exploitable:
- They share common intermediate algebraic factors (products of 4 generators
  appear as sub-products of products of 8 generators).
- They occupy the "angle refinement" regime (depth 7-8 add new angles while
  directions are saturated) — angular resolution is the bottleneck, not
  directional coverage.
- The two-component winner structure (shallow core + deep extensions) is known:
  winners at XOR(n) have mean depth ~n/6. This is a design spec, not just an
  observation.

**Connection to the vision:** For the hybrid LLM, compiled DKC neurons need to
be generated efficiently at the inference/update time. If deep-entry generation
can be done directly (without full BFS), the compilation cost drops from
exponential to manageable. This is the bottleneck for making Gap 5 (dynamic
recompilation) practical at ζ₁₂.

Status: CONCEPTUAL. D82 identified the phenomenon and posed the direct-
generation question. No implementation yet.

### 7. Finite/Infinite Group Duality

D80 established the two computational regimes. The vision question — getting
the best of both — is more than an engineering optimization; it touches the
deep structure of why DKC works at all.

**Why the finite/infinite distinction matters:**

In the finite case (ζ₈): the binary octahedral group closes at 24 elements.
Every entry has an exact, named place in the ADE/McKay classification. The
quantum dimension [2]_q = 0 at ζ₈ means the TL category is maximally non-
semisimple at this point. The XOR capacity (ceiling at 8 inputs) can in
principle be proven, not just measured. Kuperberg's #P-hardness doesn't apply
because ζ₈ is a lattice root.

In the infinite case (ζ₁₂): the group never closes; it approaches a dense
subgroup of SU(2). Every BFS round produces genuinely new algebraic structure.
The XOR capacity is not bounded by group order — it is bounded only by the
depth you can reach and the computational resources for catalog construction
and search. But: the catalog is no longer fully characterizable. Entries at
depth d are products of d generators, but the algebraic classification of the
full infinite group is open (it's related to Kuperberg's #P-hard territory
for irrational angles).

**Two research directions toward duality:**

1. **Finite subgroups of infinite catalogs**: at any depth d in a ζ₁₂ catalog,
   the entries up to depth d form a finite set (though not closed under
   multiplication). D82's winner architecture (shallow core + deep extensions)
   means the "shallow core" is a small finite structure that provides the
   invariant skeleton, even when the overall catalog is infinite. Can this
   shallow core be characterized as a finite algebraic object with ADE-style
   guarantees?

2. **Union catalogs**: ζ₈ and ζ₁₂ are largely non-overlapping (12-15%
   quaternion/direction overlap, D79). Their union could combine ζ₈'s fully
   characterized structure with ζ₁₂'s depth-scalable capacity. The union
   catalog hypothesis: using ζ₈ entries as the "shallow core" and ζ₁₂ entries
   as the "deep extensions" in a winner construction might give higher capacity
   than ζ₁₂ alone (because the ζ₈ core is algebraically specialized) while
   maintaining ζ₁₂'s scalability.

**Connection to quantum information:** The quantum dimension vanishing at ζ₈
is connected to the TL category being non-semisimple at this root — the same
non-semisimplicity that gives DKC its computational power via the
bracket-blind computation (D74). Infinite-group roots are non-semisimple in a
different, less controlled way. The duality question may have a precise
formulation in terms of monoidal category theory.

Status: CONCEPTUAL. Mathematical framework needed. Potentially a deep
connection between TL non-semisimplicity, the ADE classification, and
computational capacity.

---

## Connection to Broader Rhubarb Project

The hybrid LLM vision connects to rhubarb's larger goals:
- **Long-term thinking**: A model whose factual knowledge can be precisely
  curated and corrected is a model that can serve for decades, not months.
- **No dependencies**: Compiled weights from algebraic structure, not from
  training infrastructure that rots.
- **Latin / Catholic intellectual tradition**: Scholastic logic IS taxonomy +
  syllogism. The Summa is structured as syllogistic arguments. This is
  computation native to the tradition.
- **Smaragda**: Already a taxonomy/entity system. Natural source of structured
  knowledge for the compilation pipeline.

**The depth law as a cost model for compilation (updated, D82):**

The linear depth law (max_xor ≈ depth + 6) gives DKC compilation a concrete
cost structure for the first time. Specifically:
- A compiled neuron capable of expressing an n-input Boolean function requires
  catalog entries of crossing depth ≈ n - 6. For a 3-input syllogism (XOR3),
  depth 0 entries suffice. For a 6-input composition, depth ≈ 0 entries still
  suffice (XOR6 at depth 0). For higher-arity compiled functions, depth scales
  linearly with arity.
- More complex knots = more powerful compiled neurons. This is a direct
  translation: the crossing complexity of a braid used to generate a catalog
  entry determines the computational power that entry contributes.
- Catalog construction cost scales exponentially with depth (D82: ~2× per
  round). But the gain is linear. This means: for the hybrid LLM, there is a
  sweet spot where you build the catalog to the depth you need and stop. You
  don't need the entire infinite group — just the first d rounds for XOR(d+6).
- The two-component winner structure (shallow core + deep extensions) maps
  onto the hybrid LLM's architecture: the "shallow core" of a compiled neuron
  (generators and depth-0 entries) plays the role of the fixed logical skeleton,
  while the "deep extensions" provide the fine-grained capacity for more complex
  computations. This parallels the compiled (skeletal, exact) vs. trained
  (fine-grained, statistical) distinction in the hybrid model itself.

For building a practical hybrid LLM, this means: catalog depth is a
hyperparameter that you set based on the maximum arity of logical functions
you want to compile. For syllogistic logic (arity 3), the catalog is trivially
small. For richer logical operations (arity 8-12), you need ~5-8 rounds of BFS
at ζ₁₂. This is a concrete engineering specification, not just a theoretical
result.

---

## Open Architectural Questions

- What's the right granularity? Is each fact one DKC neuron, a cluster, or
  something else?
- How does the compiled portion influence the statistical portion during
  inference? (Hard facts constraining soft associations?)
- Can compiled weights serve as "guardrails" preventing hallucination in
  specific domains?
- Is there a natural attention mechanism that routes factual queries to
  compiled weights and associative queries to trained weights?
- How does the opaque token substitution mechanism work in practice? Is it
  a simple lookup table, or does it need to handle morphological variation
  (e.g., "bear" → "bears" → "bear's")?
- What happens when two compiled syllogisms share a term? Do their weight
  regions need to be connected? How does the net compose syllogistic chains?
- Does the acceleration thesis (compiled scaffolding → faster statistical
  training) have any theoretical grounding, or is it purely a hypothesis
  to test empirically?
- Can the "graceful ignorance" mechanism (recognizing when you lack a compiled
  subnet) be made reliable? What does the net's output look like when it
  doesn't know something?
- How does the deterministic parser handle ambiguity? "All bank employees
  are mammals" requires disambiguation (bank=financial vs bank=riverbank)
  before syllogistic compilation.

- **Quantum information connection**: DKC computation lives on S²=CP¹ (the
  Bloch sphere). The 13-direction eigenvector Voronoi might correspond to a
  known quantum measurement basis. The l=6 spherical harmonic bandwidth maps
  to hexadecapole operators in quantum optics. Is there a formal connection
  between DKC compiled weights and quantum circuits, beyond the geometric
  analogy?
- **Matroid structure of the catalog**: The 44 good / 31 poison value split
  in the DKC catalog may reflect a matroid structure (Reiner's cyclotomic
  matroids mu_8). If so, matroid membership tests would provide poly-time
  extensibility checking — replacing exhaustive catalog search.

---

*First recorded: 2026-02-20. Source: conversation during atlas defrag.*
*Updated: 2026-02-20. Source: second vision discussion — opaque tokens,
syllogism-as-training, wired integration, deterministic NL parser.*
*Updated: 2026-02-20. D63 resolved Gap 6 scaling (parity ceiling n=5).*
*Updated: 2026-02-21. D64-D71 quaternionic arc: Gap 6 fully resolved, Gap 7
(two-layer wall) and Gap 8 (quaternionic DKC / Bloch sphere) added. New
research axes: quaternionic DKC, spectral theory, music, matroid theory,
activation function design theory.*
*Updated: 2026-02-21. D72-D82 scaling arc: finite/infinite group boundary,
depth law, activation determines capacity. Gap 6 further revised (ζ₈ ceiling
at XOR8 with S¹×S² activation; ζ₁₂ breaks the wall; depth is the fundamental
variable, not catalog size). Gap 9 added (finite vs infinite group choice;
ADE classification; quantum dimension vanishing). Research axes 6 and 7 added
(depth-based catalog engineering; finite/infinite group duality). Near-term
explorations 7-9 added (ζ₁₂ activation zoo; ζ₃₂ finiteness; direct deep-entry
generation). Connection to Broader Rhubarb Project updated with depth law as
compilation cost model.*
