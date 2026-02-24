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
- **ADE gaps**: ~~E₈ (binary icosahedral, 120 elements) NOT reached.~~
  **RESOLVED by D94**: 2I (E₈) reached via explicit Z[sqrt5] generators
  (s = (1+i+j+k)/2, t = (phi + phi_inv*i + j)/2). Capacity confirmed higher
  than z8 at matched size; solvability bottleneck validated. E₆ (binary
  tetrahedral, 24 elements) is still NOT reached by any natural construction.

**Power-of-two finiteness conjecture (D80):** ζ₂ₖ for k ≥ 1 (i.e., ζ₄,
ζ₈, ζ₁₆, ζ₃₂, ...) may all be finite under this construction. D80 confirmed
the first two; ζ₃₂ is the next test.

Status: Two-regime structure established. D94 resolved the E₈/2I ADE gap and
confirmed solvability as the capacity bottleneck (Barrington). Union catalog,
E₆ gap, and power-of-two conjecture remain active research directions.

### Gap 10: Encoding Design — NEW (D92)

D92 proves that the +/-q input encoding is structurally parity-locked: masks
0...0 and 1...1 always produce identical quaternion sums (both zero), so any
function where f(0...0) != f(1...1) is impossible under ANY activation. This
means AND, OR, MAJ, THRESHOLD — every standard Boolean function except XOR/XNOR
— have exactly zero winners at all depths, all resolutions, all roots of unity.

The encoding is more fundamental than the activation, the catalog, or the root
of unity. It determines which Boolean functions are computable AT ALL. The
design hierarchy is now:

```
Encoding  →  determines which functions are computable
  Activation  →  determines how many solutions exist (resolution)
    Catalog depth  →  determines maximum arity
      Root of unity  →  determines capacity ceiling (finite vs infinite)
```

The 1wpi (one-weight-per-input) encoding from D48/D50 computes all 13 NPN
classes — so DKC is NOT parity-locked, only the +/-q encoding is. But the
1wpi encoding has not been tested for depth-law behavior. Key sub-questions:

- Does max_and or max_maj scale linearly with depth under 1wpi?
- If so, what is the slope? (Parity's slope is ~1 under +/-q.)
- Can hybrid encodings (some inputs +/-q, some 1wpi) balance function breadth
  with depth-law scaling?
- Is the +/-q encoding provably optimal for parity among all 2-bit-per-weight
  encodings?

For the hybrid LLM vision, syllogistic logic requires AND, OR, and implication
— not just XOR. The encoding must be designed to support these functions.
The 1wpi encoding is the current candidate, but its scaling behavior is the
most urgent open question for the compilation pipeline.

Status: PARITY-LOCK PROVED (D92). 1wpi depth-law behavior UNTESTED. This is
the most architecturally consequential gap identified since D50.

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

10. ~~**Indecomposability parameter b calculation** (D85 seed)~~ — **DONE**
    (Demo 85). b = -5/8 confirmed at TL_4 via leading-coefficient extraction
    from delta-parameterized Markov trace on the full regular representation.
    The "controversy" (b = -2 vs b = -5/8) dissolved: different normalization
    conventions. b is a collective property of the full algebra (Demo 86
    established that single P_{0,0} universally diverges).

11. ~~**Dense polymer fusion rules** (D86 seed)~~ — **PARTIALLY ADDRESSED**
    (Demo 86). The delta-parameterized approach on single projective covers
    universally diverges — a novel negative result. The multiplicity from the
    full regular representation is structurally essential. The actual fusion
    rule verification remains open; the method must be different from the
    delta-parameterized Gram matrix approach.

12. **1wpi encoding depth law** (D92 seed) — The most architecturally urgent
    question. The +/-q encoding is parity-locked (D92). The 1wpi encoding
    computes all 13 NPN classes (D48/D50) but its depth-law behavior is
    unknown. Does max_and or max_maj scale linearly with depth under 1wpi?
    If yes, what slope? If no, the entire compilation cost model for
    non-parity functions is open. Single demo, high leverage.

13. **Encoding space exploration** (D92 seed) — Systematic survey of encodings
    between +/-q (paired, parity-only) and 1wpi (independent, all functions).
    How many bits per weight? Which pairing structures? What equivalence
    classes does each encoding produce? Goal: map the encoding design space
    and identify the right encoding for syllogistic logic (AND, OR,
    implication). 1-2 demos.

14. **Balanced exponentials formal proof** (D91 seed) — The linear depth law
    slope arises from BFS branching factor (~2×/round) vs parity demand growth
    (4×/weight). An analytical derivation connecting these two exponentials
    would upgrade the balanced exponentials conjecture to a theorem (P05).
    The BFS branching factor depends on the group structure; the parity demand
    growth is combinatorial. Single demo, clean formal proof candidate.

15. **Regime transition threshold** (D87 seed) — D87 established the two
    regimes (nulls critical at ζ₈, dispensable at ζ₁₂). The exact transition
    point is unknown. Testing at intermediate catalog sizes (truncated ζ₁₂,
    ζ₁₀) would locate the threshold. Is it a specific direction count, null
    fraction, or catalog size? A sharp threshold would give a concrete design
    criterion. Single demo.

16. **Relational activation function** (D90 seed) — Current activations
    (sector, Voronoi, combined) treat entries independently. D90 proves the
    depth law mechanism is relational (axis cancellation between entries). An
    activation function that explicitly uses axis dot products between entries
    in a tuple (rather than classifying entries individually) might break the
    linear depth law and achieve superlinear scaling. Speculative but
    potentially high-impact. 1-2 demos.

17. **D95: RKHS kernel rank test** (D94 seed) — THE quantitative validation
    of the five-pillar synthesis. Compute DKC kernel K(m,m') = quaternion inner
    product of signed sums for 2I vs z8. If rank(K_2I)/rank(K_z8) > 120/24,
    non-solvability contributes above raw catalog size. One number, one demo.
    HIGHEST PRIORITY among new seeds.

18. **Higher k_sec with 2I** (D94 seed) — D94 used k_sec=12 (384 cells).
    2I has 9 half-angles and 31 directions — richer than z8. k_sec=24 doubles
    cells to ~768. Quick test: does N=8 XOR become nonzero? If yes, the
    pigeonhole wall is activation-tunable, not structural. Single parameter
    change.

19. **Depth law under phase_cell** (D93 seed) — D93 showed phase_cell
    recovers all 13 NPN classes and reveals circuit complexity hierarchy. The
    depth law was established under combined_cell. Does the linear relationship
    hold under phase_cell? Different functions may have different slopes —
    a richer depth-law landscape. Single demo.

20. **Quantum dimension direct test at Fibonacci parameter** (D94 seed) —
    Test at q = e^{2*pi*i/5} directly rather than through 2I. [2]_q = phi^{-1}
    ~ 0.618 at this parameter — the "maximally computational" TQC point
    (Mochon 2003). How does a dense catalog here compare to 2I's finite one?
    Bridges TQC and DKC most directly. 1-2 demos.

21. **Cross-depth 2I analysis** (D94 seed) — 2I BFS depth profile is a
    symmetric diamond (5,8,11,12,11,8,4,1). Does the contribution profile
    mirror the symmetry, or are deep entries disproportionately valuable as
    at z12 (D82)? Tests depth law applicability to non-solvable finite groups.
    Single demo.

---

## New Research Axes (from D64-D94)

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

### 8. Resource Decomposition as Architectural Principle — NEW (D83)

D83 established that DKC computational power is not a single monolithic
quantity — it decomposes into three independent, additive axes:

```
DKC capacity = lattice base (XOR6) + 2 × depth_rounds + 2 × framing_present
```

Each axis contributes +2 XOR levels independently:
- **Lattice structure** (the root of unity and its SU(2) image): provides the
  base capacity (XOR6 from the ζ₈ lattice alone).
- **Crossing depth** (generator multiplications, D82): each unit of depth adds
  a fixed increment. The mechanism is angular refinement — deeper entries
  introduce finer angles within the same set of directions.
- **Writhe / framing** (chirality accumulation, D83): the writhe-dependent
  phase factor `(-A³)^{-w}` contributes exactly +2 XOR levels. Jones
  normalization, which removes this factor, costs exactly 2 XOR levels at
  every root tested (ζ₈: bracket XOR8 → Jones XOR6; ζ₁₂: bracket XOR12 →
  Jones XOR10).

The three axes are genuinely independent: depth-writhe correlation r = 0.139
across 4096 ζ₁₂ entries. A deep entry can have low writhe (crossings cancel)
or high writhe (crossings accumulate). Writhe alone can compute XOR6 (32
winners from writhe-only sums) but cannot reach XOR8 — full power requires
the interaction of all three axes.

**Why this matters architecturally:**

For the hybrid LLM compilation pipeline, this means DKC weight selection can
be optimized axis by axis:
- Choose the root of unity (lattice structure) for the base capacity.
- Set the catalog depth for the crossing-depth contribution.
- Decide whether to use bracket (framing-aware, +2) or Jones (framing-
  normalized, -2) weights depending on the target function.

Each axis is a separately tunable hyperparameter. This connects directly to
Gap 5 (dynamic recompilation): recompilation cost can be reduced by holding
two axes fixed and varying only one. For instance, changing a compiled
neuron's capacity by +2 XOR levels can be achieved by deepening the catalog
one round (holding root and writhe strategy constant), rather than
rebuilding from scratch.

**Framing as computation — inverting a 35-year-old assumption:**

Witten (1989) identified the writhe-dependent phase as a "framing anomaly"
— the first term removed when passing from the bracket to the Jones
polynomial to make the invariant topologically well-defined. TQFT's maximally
degenerate point (δ=0, where the quantum dimension vanishes at ζ₈) discards
this term first. Demo 83 demonstrates that the "anomaly" is not bookkeeping
noise but genuine computational content: exactly the component worth +2 XOR
levels. This inverts 35 years of TQFT convention: what TQFT normalized away
as an artifact, DKC uses as a resource. For the DKC research program, this
means we should be suspicious of any normalization that discards information
— the discarded terms may be computationally valuable.

Status: DEMONSTRATED (ζ₈ and ζ₁₂). Predicted universal across all roots but
needs verification at ζ₁₆ and ζ₂₄.

### 9. Null States and the Reservoir Computing Connection — NEW (D84)

D84 established the first concrete bridge between DKC and Reservoir Computing
(RC). The connection runs through null states — bracket-null entries where
Re(q) = 0 (Kauffman trace vanishes).

**The null-state thesis:**

In the ζ₈ catalog (24 entries, binary octahedral group), 9 entries (37.5%)
are bracket-null. These are not dead weight. They maintain 6 unique S²
directions (cube-edge-midpoint axes) that are unavailable to non-null entries.
Removing nulls collapses XOR capacity from XOR8 to XOR6 — worse than
removing a random equal-size subset (random-15 mean = XOR7.8). Nulls are
disproportionately important because they provide directional coverage of S²
regions that the non-null entries do not reach.

This directly maps onto the RC separation property: a reservoir needs its
high-dimensional manifold to remain open in all critical directions for
linear readout to succeed. Null reservoir states (zero output under readout)
are conventionally assumed to be wasted capacity. D84 proves this assumption
wrong for quaternionic DKC: the null states hold the manifold open in exactly
the directions needed for higher-order parity separation.

**Concrete RC↔DKC mapping:**

| RC Concept | DKC Analog |
|---|---|
| Reservoir state | Quaternionic weight (catalog entry) |
| Null reservoir state | Bracket-null entry (Re(q)=0) |
| Separation property | S² directional coverage enabling XOR |
| High-dimensional manifold | 13-direction Voronoi on S² |
| Linear readout | Sector-threshold activation |
| Null state maintains manifold | Null directions hold Voronoi open |

The null fraction dilutes as the group grows (ζ₄: 75%, ζ₈: 37.5%, ζ₁₂: 3%),
consistent with nulls being a finite-group boundary effect. But at ζ₈ — the
most algebraically structured point (ADE type E₇, quantum dimension zero,
maximally non-semisimple TL category) — they are a major structural fraction,
and their role is unmistakable.

**Implication for the hybrid LLM:** When designing compiled DKC neurons, the
weight set should not be pruned to only include "active" (non-null) entries.
Null weights serve a structural role — they maintain the geometric separation
needed for the neuron to compute higher-order functions. This is a concrete
design constraint for compiled weight matrices.

Status: DEMONSTRATED at ζ₈. Role of nulls at ζ₁₂ (where they are 3% of the
catalog but contribute 29 unique directions) is an open question.

### 10. The LCFT Bridge — NEW (D84)

D84 opened a bridge to Logarithmic Conformal Field Theory (LCFT) that was
not in the vision before. The connection:

In LCFT (Gurarie 1993, Gaberdiel-Kausch 1996), null states |N⟩ satisfy
⟨N|N⟩ = 0 but are not zero vectors. They are paired with logarithmic
partners |L⟩ through Jordan-cell (non-diagonalizable) action of the Virasoro
zero-mode L₀:

```
L₀ |N⟩ = h |N⟩
L₀ |L⟩ = h |L⟩ + |N⟩
```

The two-point function of |N⟩ vanishes, but the mixed correlator ⟨N|L⟩ is
nonzero. Computational content lives in the Jordan-cell coupling.

In quaternionic DKC: bracket-null entries (Re(q) = 0, trace vanishes) are
the |N⟩ states. Their "logarithmic partners" are non-null entries sharing
the same S² direction (the 3 coordinate-axis directions where both null and
non-null entries co-exist). The 6 null-only directions (cube-edge-midpoints,
no non-null partner) have no logarithmic partner in the catalog — consistent
with the observation that removing these directions destroys capacity rather
than merely reducing it.

**The dense polymer connection:** The Temperley-Lieb algebra at δ = 0 (the
loop value when [2]_q vanishes at ζ₈) is the algebraic description of dense
polymers — the c = -2 LCFT. The dense polymer model at β = 0 is precisely
the LCFT analog of the ζ₈ bracket. This means:

- ζ₈ DKC lives at the dense polymer / c = -2 LCFT point.
- The Jordan-cell structure of the c = -2 theory (indecomposable but
  reducible representations of the Virasoro algebra) directly describes the
  null / non-null pairing in the DKC catalog.
- The indecomposability parameter b (Gurarie's "b-number") should be
  calculable from the ζ₈ catalog structure. This is a concrete prediction.

**Why this matters for the vision:**

DKC now sits at the intersection of three previously disconnected fields:

```
        LCFT (c = -2, dense polymers)
              ↗
    DKC ← TQC (Temperley-Lieb, Jones, braids)
              ↘
        RC (reservoir computing, separation property)
```

Nobody has mapped this three-way intersection. The existing literature
touches pairs: LCFT ↔ TL (Pearce, Read, Saleur), TL ↔ Jones (Kauffman,
Kuperberg), RC ↔ neural nets (Jaeger, Maass). But the triangle — LCFT null
states as RC separation maintainers computed via TL bracket at δ = 0 — is
novel. This is potentially the deepest structural insight to date: the
non-semisimple part of the TL algebra is simultaneously what makes LCFT
"logarithmic" (Jordan cells instead of diagonal), what makes DKC compute
(null directions holding the manifold open), and what makes the reservoir
work (separation property through geometrically essential null states).

Status: INTERPRETATION, PARTIALLY CONFIRMED. Jordan-cell pairing is consistent
with all data. The indecomposability parameter b = -5/8 has been computed from
first principles at TL_4 (D85), confirming the LCFT correspondence
quantitatively. D86 established that b is a collective property of the full
algebra (single P_{0,0} universally diverges). Dense polymer fusion rules
remain a concrete next verification.

### 11. Non-Semisimplicity as THE Resource — NEW (D50→D82→D84 Arc)

The thread from D50 through D82 to D84 converges on a single deep claim:
the non-semisimple part of the Temperley-Lieb algebra is not an obstacle
or a degenerate edge case — it IS what makes DKC work.

**The convergence:**

- **D50**: The parity wall was in the activation function, not the lattice.
  Increasing sectors from k=2 to k=6 unlocked all 13 NPN classes. The wall
  that looked algebraic was geometric — the activation was too coarse to
  resolve the lattice structure.
- **D65-D67**: The activation progression (sector → generalized sector →
  Voronoi on S¹ → Voronoi on S³ → Voronoi on S²) culminated in the 13-
  direction eigenvector Voronoi. The 13 directions are exactly the
  eigenvector axes of the binary octahedral group — a structure that exists
  only because the ζ₈ representation is non-semisimple (in a semisimple
  representation, eigenvectors would not form discrete clusters on S²).
- **D82**: Crossing depth governs XOR capacity via the depth law
  max_xor ≈ depth + 6. The depth mechanism is algebraic coherence — deep
  entries carry shared intermediate products from generator multiplication
  chains. These chains exist because the algebra is infinite-dimensional
  (at ζ₁₂) or has non-trivial Jordan structure (at ζ₈). In a semisimple
  algebra, all entries would be direct sums of irreducibles with no
  interesting depth structure.
- **D83**: Framing (writhe) contributes +2 XOR levels. The writhe-dependent
  phase factor is precisely the term that TQFT removes at δ = 0 (the
  non-semisimple point). The computational resource IS the non-semisimple
  contribution.
- **D84**: Null states — the most extreme manifestation of non-semisimplicity
  (zero trace, indecomposable but reducible representations) — are
  indispensable for XOR8 capacity. Removing them drops capacity below the
  random baseline. The Jordan-cell structure (non-diagonalizable L₀ action)
  is the algebraic mechanism: the null state anchors the logarithmic partner
  that carries the computation.

**The claim:** At the ζ₈ point (quantum dimension [2]_q = 0, δ = 0, dense
polymer / c = -2 LCFT), the TL category is maximally non-semisimple. Every
feature of DKC that contributes computational power — the discrete direction
spectrum, the depth structure, the writhe contribution, the null-state
manifold maintenance — is a consequence of this non-semisimplicity. A
semisimple TL category (at generic q, δ ≠ 0) would have:
- A continuum of directions (no discrete Voronoi)
- Trivial depth structure (all irreducibles immediately accessible)
- No null states (all representations fully reducible)
- No framing anomaly (Jones and bracket would agree)

**Implication for the vision:** The maximally non-semisimple point is not
a degenerate corner to be avoided — it is the sweet spot for compiled
computation. The entire DKC research program is, in retrospect, an
exploration of what computation looks like when you sit at the most singular
point of the quantum group parameter space. This is the opposite of the
standard quantum computing approach (which avoids non-semisimple points
because they make the Jones polynomial evaluation trivial in the Kuperberg
sense). DKC thrives precisely where quantum computing gives up.

Status: INTERPRETIVE SYNTHESIS. Each individual result is demonstrated;
the overarching claim about non-semisimplicity is a unifying interpretation.

### 12. Encoding as a Design Dimension — NEW (D92)

D92 proves that the encoding — how input bits are mapped to quaternion
multipliers — determines which Boolean functions are computable at all. This
is not a tuning knob; it is a structural selector. The +/-q encoding creates
3^k equivalence classes (per-weight effective states {-q, 0, +q}), and XOR/XNOR
are the ONLY standard Boolean functions constant on all classes.

**Why this is a new research axis, not just a gap:**

The encoding question is not "which encoding is best?" but "what is the space
of possible encodings, and what does each one select for?" This is a design
dimension in the sense that different points in encoding space produce
qualitatively different computational capabilities:

- **+/-q paired encoding**: selects for parity (the hardest function outside
  AC^0). Creates equivalence classes via per-weight pair cancellation. The
  depth law holds with slope ~1.
- **1wpi encoding**: selects for all 13 NPN classes (D48/D50). One weight per
  input bit, no pairing. Depth-law behavior unknown.
- **Hybrid encodings**: some weights paired, some independent. Potentially
  access intermediate function sets. Completely unexplored.

The encoding design dimension is orthogonal to the activation design dimension
(Research Axis 5). Activation determines resolution (how many solutions);
encoding determines reachability (which functions). Together they form a 2D
design space where each (encoding, activation) pair produces a specific
computational profile.

**Connection to the vision:** For the hybrid LLM, different compiled neurons
may need different encodings depending on the logical function they implement.
A syllogism-implementing neuron needs AND/OR/implication (1wpi encoding); a
parity-check neuron needs XOR (+/-q encoding). The encoding is part of the
compilation specification, not a global architectural choice.

The parity-lock theorem (D92) is also the 4th formal proof in the program
(after radical dimension, next-level radical, and Markov RT truncation). It
is the first proof that constrains the design space rather than characterizing
capacity — a qualitative shift in the theoretical program's maturity.

Status: PARITY-LOCK PROVED. Encoding space exploration NOT STARTED. The 1wpi
depth law is the immediate next question.

### 13. Relational Computation as a Design Principle — NEW (D89-D91)

D89-D91 establish that the depth law mechanism is relational: what matters is
how catalog entries RELATE to each other (axis cancellation, cross-depth
algebraic constraints), not where they individually sit on S². The spectral
inversion result (D90) is the cleanest evidence: the S² point cloud bandwidth
DECREASES with depth while computation INCREASES — positional information
degrades monotonically while relational information grows.

**The relational computation thesis:**

```
Positional properties:    entry direction, entry angle, spherical design quality
                          → DECREASE with depth (spectral inversion)
                          → WRONG optimization target

Relational properties:    axis cancellation (anti-aligned pairs, min_dot ≈ -0.75),
                          cross-depth algebraic constraint (73 distinct angles
                          from 50K gen×deep pairs), BFS ancestry structure
                          → INCREASE with depth
                          → CORRECT optimization target
```

This is not just a correction of one hypothesis — it inverts the entire
intuition about what "good" catalog entries look like. Individual entry quality
(positional metrics like angular spacing, direction coverage, spherical design
residual) is fundamentally misleading. Optimizing for positional quality would
move the catalog AWAY from computation (D72: 89% XOR loss under Voronoi
optimization).

**Implications for catalog engineering:**

The correct catalog optimization problem is not: "place N entries to maximize
coverage of S²." It is: "select N entries to maximize pairwise axis
cancellation quality and cross-depth algebraic constraint density." This is a
combinatorial optimization over relationships, not a geometric optimization
over positions.

Specific relational metrics identified by D89-D91:
- **Axis cancellation quality**: XOR winners have mean(min_dot) = -0.75 vs
  -0.65 for non-winners. Deeper entries produce more anti-aligned axis pairs.
- **Cross-depth vocabulary constraint**: Gen × deep-entry sums produce only
  73 distinct angles from 50K pairs. BFS ancestry creates algebraic
  relationships that restrict pairwise sum-angle vocabularies.
- **Balanced exponentials**: BFS vocabulary grows ~2x/round, parity demand
  grows 4x/weight. The ratio determines the depth law slope (~1).

**Connection to the vision:** For the hybrid LLM compilation pipeline, this
means weight selection algorithms should optimize relational properties of the
weight set, not individual weight quality. A compiled neuron's power depends on
how its weights interact, not on where they individually sit in the algebraic
catalog.

Status: MECHANISM DEMONSTRATED (D89-D91). Relational optimization algorithms
NOT STARTED. The balanced exponentials slope derivation is the next clean
formal proof candidate.

### 14. Regime-Dependent Design Principles — NEW (D87-D88)

D87-D88 establish that design principles for DKC neurons are not universal —
they depend qualitatively on whether the underlying group is finite or infinite.

**The two regimes:**

| Property | Finite (ζ₈, 24 entries) | Infinite (ζ₁₂, 4096 entries) |
|----------|------------------------|------------------------------|
| Null fraction | 37.5% | 3.0% |
| Null indispensability | CRITICAL (XOR8→XOR4 without nulls) | DISPENSABLE (XOR12 preserved) |
| Direction density | 13 dirs (sparse) | 2043 dirs (dense) |
| Per-dir residual | 0.171 | 0.021 (8× lower) |
| Dominant pathway | Shadow pairing (100%) | Direction diversity (80-94%) |
| Perturbation sensitivity | Non-null anchors fragile, null scaffolding flexible | Dense dirs absorb perturbation |

The transition mechanism is direction density: when non-null directions are
dense enough on S², losing null-only directions cannot constrain combinatorial
search. At ζ₈ (13 directions, 6 null-only), every direction matters. At ζ₁₂
(2043 directions, 67 null-only), the 1976 non-null directions provide
sufficient combinatorial diversity.

**The two-role direction architecture (D88):**

At ζ₈, the 13 cuboctahedral directions serve two distinct computational roles:
- 4 non-null body diagonals = rigid computational anchors (perturbation-
  sensitive, load-bearing for XOR). Moving them 10° causes 8% XOR loss.
- 6 null-only edge midpoints = flexible topological scaffolding (perturbation-
  tolerant, movable). Moving them 10° causes -2.4% loss (a GAIN).

Constrained optimization (nulls free, non-nulls clamped to 2°) captures 92%
of design improvement with only 2.8% XOR loss. The k-ladder activation
provides 14× better perturbation resilience than Voronoi-only (6.4% vs 89%
loss under the same design-improving gradient).

**Implications for the vision:**

A practical hybrid LLM using compiled DKC neurons would need regime-aware
compilation strategies:
- **Finite-group neurons** (ζ₈): preserve null entries, protect non-null
  anchor positions, use k-ladder activation for robustness.
- **Infinite-group neurons** (ζ₁₂): safe to prune null entries, direction
  diversity is the dominant capacity driver, perturbation resilience is
  inherent.

The exact transition threshold (at what catalog size / direction density /
null fraction does the regime change?) is unknown. Locating it would give a
concrete design criterion: below the threshold, use finite-group design rules;
above, use infinite-group rules.

Status: TWO REGIMES DEMONSTRATED (D87-D88). Transition threshold UNKNOWN.
The regime question may have a sharp answer in terms of direction density or
null fraction.

### 15. Formal Proofs as a Maturing Theoretical Framework — NEW (D92)

The parity-lock theorem (D92) is the 4th formal proof in the program:

| # | Proof | Demo | What it establishes |
|---|-------|------|---------------------|
| P01 | Radical dimension formula | D39 | dim(rad) = f(l) — algebraic structure |
| P02 | Next-level radical structure | D39 | radical propagation — algebraic depth |
| P03 | Markov RT truncation | D51 | trace form properties — algebraic tools |
| P04 | Parity-lock theorem | D92 | +/-q encoding computes only XOR/XNOR — design constraint |

The qualitative shift: P01-P03 characterize the algebraic substrate (what the
algebra looks like). P04 constrains the design space (what the algebra can and
cannot do under a specific encoding). This is the transition from "what is
DKC?" to "what are DKC's structural limits?" — from exploration to theory.

**The proof pipeline is accelerating:**

D89-D91 identified two more proof-ready results:
- **Balanced exponentials**: the linear depth law slope as a theorem (BFS
  branching factor ~2×/round vs parity demand 4×/weight → slope ~1). This
  would be P05.
- **Spectral inversion**: S² bandwidth decreases with depth while computation
  increases. A formal statement and proof would be P06.

D87 identified a potential threshold theorem:
- **Regime transition**: a sharp threshold for null dispensability as a
  function of direction density. This would be P07 if it admits a clean
  statement.

**Why this matters for the vision:**

The hybrid LLM vision requires credibility in academic and engineering
contexts. Moving from "computationally verified" to "formally proved" is the
difference between an empirical observation and a structural guarantee. Four
proofs (and a pipeline of candidates) means the theoretical framework is
approaching the maturity needed for external engagement — publications,
collaborations, and engineering specifications grounded in proven properties
rather than measured heuristics.

Status: 4 FORMAL PROOFS COMPLETE. 3 candidates in pipeline (balanced
exponentials, spectral inversion, regime transition threshold).

### 16. DKC as Discrete Algebraic Reservoir Computer — NEW (D93-D94) — FIVE-PILLAR SYNTHESIS COMPLETE

D93-D94 complete the five-pillar synthesis connecting DKC to five independent
theoretical traditions. This is not a new analogy — it is a precise mathematical
mapping with testable quantitative predictions.

**The five pillars (all connected through D94):**

```
Pillar 1: TL algebra as computation (Abramsky 2009)
    DKC catalog = TL algebra representation
    → connected via bracket values as group elements

Pillar 2: Barrington's theorem (1989) — branching programs over groups
    Non-solvable groups strictly more powerful for NC^1
    → CONFIRMED by D94: 2I (non-solvable) > z8 (solvable) at matched size

Pillar 3: Nazer-Gastpar compute-and-forward (2011)
    DKC's signed sum = linear readout of algebraic reservoir
    → the additive readout mechanism

Pillar 4: MVN neurons (Aizenberg 2008) — k-sector activation
    DKC activation = nonlinear reservoir output function
    → D93: phase_cell vs combined_cell determines function reachability

Pillar 5: Reservoir computing (Maass 2002, Gonon-Ortega 2020)
    Fixed catalog = reservoir, Cayley graph = connectivity,
    signed sum = linear readout, activation = nonlinear output,
    BFS depth = memory depth
    → D94 proposes precise mapping, D95 RKHS kernel test validates it
```

**The precise DKC ↔ Reservoir mapping (D94):**

| Reservoir Computing Concept | DKC Analog |
|---|---|
| Reservoir (fixed recurrent network) | Quaternion catalog (BFS closure of generators) |
| Reservoir state at time t | Signed sum of selected catalog entries |
| Input-to-reservoir coupling | 1wpi encoding: input bits select weights |
| Reservoir topology (connectivity) | Cayley graph of the SU(2) subgroup |
| Linear readout | Quaternion inner product (Nazer-Gastpar) |
| Nonlinear output function | Activation (phase_cell / combined_cell / Voronoi) |
| Memory depth | BFS depth (crossing depth of catalog entries) |
| Separation property | XOR capacity (ability to separate function classes) |
| Echo state property | Group closure (catalog is a fixed point of BFS) |
| Kernel K(m,m') | Quaternion inner product of signed sums |

**Testable prediction (D95):** rank(K_2I)/rank(K_z8) > 120/24 = 5. If
non-solvability contributes above raw catalog size, the kernel rank ratio
exceeds the catalog size ratio. One number validates the entire framework.

**Why this is a milestone:**

Before D94, these five traditions were connected only pairwise at best:
- TL ↔ Jones polynomial (Kauffman, Kuperberg)
- Reservoir ↔ neural nets (Jaeger, Maass)
- Barrington ↔ circuit complexity (textbook)
- MVN ↔ Boolean functions (Aizenberg)
- Nazer-Gastpar ↔ lattice codes (coding theory)

D94 connects ALL FIVE through a single computational object (the DKC neuron
with 1wpi encoding under phase_cell on the 2I catalog). This is the first time
these traditions have been unified. The circuit complexity hierarchy from D93
(AND/XOR ratio = Hastad + LMN + Furst-Saxe-Sipser made concrete) provides
the computational content; the 2I group structure from D94 (Barrington's
theorem validated) provides the algebraic content; the reservoir framing
provides the architectural content.

**Connection to the vision:** The "discrete algebraic reservoir computer" is
a precise name for what DKC IS, not just what it resembles. For the hybrid
LLM, each compiled neuron is literally a discrete algebraic reservoir computer
with a fixed catalog, linear readout, and nonlinear activation. The reservoir
framework provides design principles (echo state property → catalog
construction, separation property → capacity prediction, kernel methods →
weight selection) that translate directly to the compilation pipeline.

Status: FIVE-PILLAR SYNTHESIS COMPLETE (D94). RKHS kernel test (D95) is the
quantitative validation. If confirmed, this is the deepest structural result
in the program — not a new observation but a new framework connecting all
previous observations.

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
*Updated: 2026-02-21. D83-D84 arc: resource decomposition (lattice + depth +
writhe, each +2 XOR independently), framing as computational content inverting
35-year TQFT assumption (Witten 1989), null states as RC separation-property
maintainers, LCFT bridge (Jordan cells, dense polymer c=-2, Gurarie b-number),
non-semisimplicity as THE resource (D50→D82→D84 convergence). Research axes
8-11 added. Near-term explorations 10-11 added (indecomposability parameter b
calculation, dense polymer fusion rules). Three-way LCFT↔TQC↔RC intersection
identified as novel.*
*Updated: 2026-02-24. D85-D92 mechanistic arc: Gap 10 added (encoding design
— D92 parity-lock theorem proves encoding determines computability, more
fundamental than activation or catalog). Research axes 12-15 added: encoding
as design dimension (D92), relational computation thesis (D89-D91, spectral
inversion proves positional optimization is wrong), regime-dependent design
(D87-D88, finite vs infinite group principles differ qualitatively), formal
proofs as maturing framework (P04 parity-lock, pipeline of P05-P07). Near-term
explorations 10-11 marked DONE/partial, 12-16 added (1wpi depth law, encoding
space exploration, balanced exponentials proof, regime transition threshold,
relational activation function).*
*Updated: 2026-02-24. D93-D94 circuit complexity and group structure arc:
Research axis 16 added (DKC as Discrete Algebraic Reservoir Computer — FIVE-
PILLAR SYNTHESIS COMPLETE). D93: complement-blindness theorem, phase_cell
recovery of all 13 NPN classes, circuit complexity hierarchy (AND/XOR ratio
1.01→infinity, N=3-8), pigeonhole mechanism identified. D94: binary icosahedral
group 2I (E₈) reached via Z[sqrt5] exact arithmetic, solvability bottleneck
confirmed (Barrington), circuit complexity hierarchy universal across groups,
crossover hypothesis demonstrated (2I overtakes z12 at N=6-7), "discrete
algebraic reservoir computer" framing proposed with testable kernel-rank
prediction. Gap 9 updated (E₈/2I resolved). Near-term explorations 17-21 added
(D95 RKHS kernel rank, higher k_sec with 2I, depth law under phase_cell,
Fibonacci parameter direct test, cross-depth 2I analysis). Paper 8 path
identified: circuit complexity + five-pillar synthesis.*
