# DKC Research: Next Priorities

Generated 2026-02-20. Informed by synthesis documents (narrative, novelty, connections, four-lenses).

---

## Tier 1: Do Next

### 1.1 Formal proof of the sandwich theorem
**What:** Prove that rad^2(TL_{2k}(0)) is isomorphic to TL_{2k-1}(0) using Graham-Lehrer cellularity plus explicit cell-module bilinear form computation. Write it up proof-quality.
**Why:** This is the strongest novel algebra result (HIGH confidence novelty) but currently only a computational observation at n=4,6,8. A formal proof turns Paper 3 from "computational discovery" to "theorem," which is the difference between a short note and a substantial publication.
**Effort:** SMALL (1 demo — mostly pen-and-paper with verification code).
**Unblocks:** Paper 3 (Sandwich + Radical Anatomy). Also strengthens the TL non-semisimplicity explanation for the parity wall.

### 1.2 4-input parity: test the triskelion generalization
**What:** Extend the forward DKC pipeline to 4-input Boolean functions (222 NPN classes). Search for parity (XOR4) under k-sector activations. Does parity require k=8 sectors as the triskelion generalization (k=2n) predicts?
**Why:** The triskelion conjecture is the most testable open conjecture and a natural "next result" for Paper 1. If k=8 is confirmed, it's a strong structural law. If it fails, we learn something deeper about the sector geometry. Either outcome is publishable.
**Effort:** MEDIUM (2 demos — catalog expansion to 4-strand braids + exhaustive k-sector search at each k).
**Unblocks:** Triskelion generalization conjecture (confirm or refute). Extends the four-tier hierarchy to a richer NPN landscape. Provides the second data point for the k=2n law.

### 1.3 Catalog completeness argument
**What:** Prove or bound that the 100-value Z[zeta_8] catalog at delta=0 contains all distinct bracket values up to braid length L. Alternatively, prove asymptotic saturation (new values per additional crossing approaches zero).
**Why:** Every forward DKC exhaustive search ("0 out of 100M") depends on the catalog being complete enough. Without a completeness argument, a reviewer can ask "what if you missed a value?" This is the single most important gap for Paper 1.
**Effort:** SMALL (1 demo — extend enumeration to length 10-12, track saturation curve, attempt an algebraic bound using axiality + single-loop selection).
**Unblocks:** Strengthens Paper 1 from "exhaustive over our catalog" to "exhaustive over all bracket values up to length L."

### 1.4 Convexity thesis: complete the formal proof
**What:** Formalize the argument that the 11/13 wall is a consequence of activation convexity (not just encoding geometry). D57 and D62 have the ingredients; what's missing is a single clean theorem statement with a self-contained proof.
**Why:** The 11/13 theorem (Paper 2) has clean proofs for the two unreachable classes, but the UNIFYING explanation (convexity) is informal. A formal convexity theorem would elevate Paper 2 from "two separate impossibility results" to "one structural theorem with two corollaries."
**Effort:** SMALL (1 demo — mostly writing, possibly one verification).
**Unblocks:** Paper 2 becomes a stronger, more unified submission.

---

## Tier 2: Do Soon

### ~~2.1 ell=7 verification~~ — DONE (Demo 60)
All predictions confirmed: rad(TL_7) = 11 = 2*7-3, rad(TL_8) = 39 = 49-7-3, rad^2 = 1-dimensional, nilpotency = 3, Fibonacci rank = F(6) = 8. Cross-validated at two independent primes. Universality confirmed beyond quadratic fields. Paper 3 strengthened.

### 2.2 Axiality: determine if it follows from known Galois theory
**What:** Consult the cyclotomic Galois theory literature (or a number theorist) to determine whether the axiality theorem at delta=0 is a trivial consequence of Gal(Q(zeta_8)/Q) acting on bracket values. Write up the answer either way.
**Why:** If axiality is derivable from standard theory, Paper 4 is a short note and the novelty claim must be downgraded. If it is NOT trivially derivable, it is a significant result. Either way, knowing the answer is critical before claiming novelty in print.
**Effort:** SMALL (research task, not a coding demo).
**Unblocks:** Determines whether Paper 4 exists as a standalone paper or is absorbed into Paper 1/3.

### 2.3 Sharpen the relationship to Aizenberg 2008
**What:** Explicitly state how forward DKC relates to Aizenberg's known UBN parity construction (w1=zeta_8, w2=i, w3=-1). Show that these specific weights ARE bracket values in the catalog. Measure how many of the 906 parity solutions at k=6 reduce to Aizenberg's construction under gauge equivalence.
**Why:** A reviewer in the CVNN community will immediately ask this. If all 906 solutions are gauge-equivalent to Aizenberg's, the novelty is "the weights come from topology" (still novel, but narrower). If many are gauge-inequivalent, the catalog provides genuinely new solutions Aizenberg's framework doesn't predict.
**Effort:** SMALL (1 demo — gauge-equivalence classification of the 906 solutions).
**Unblocks:** Precise novelty claim for Paper 1. Prevents a reviewer objection from derailing the submission.

### 2.4 Rate region analysis
**What:** Apply Nazer-Gastpar capacity bounds to the DKC lattice. How many bits per bracket value can DKC extract? Formalize the two-channel (magnitude + phase) coding conjecture with actual achievable rate calculations.
**Why:** The ~4.3-bit magnitude + ~1.5-bit phase = ~5.8 bits/symbol claim is currently informal. A rate region analysis connects DKC to the information-theoretic community and could be a standalone result or a strong section in Paper 1.
**Effort:** MEDIUM (1-2 demos — capacity computation + comparison to Shannon bound).
**Unblocks:** Two-channel coding conjecture (confirm or refine). Potential Paper 5 or strong section in Paper 1.

### 2.5 Multi-layer DKC
**What:** Cascade two bracket-valued neurons: output of layer 1 feeds layer 2. Can a 2-layer DKC network compute functions that a single layer cannot (e.g., parity under split-sigmoid)?
**Why:** Single-layer DKC with split-sigmoid hits the parity wall. If multi-layer circumvents it WITHOUT changing activation, the wall is about depth, not just activation geometry. This would be a significant structural result.
**Effort:** MEDIUM (2 demos — 2-layer architecture + systematic search).
**Unblocks:** Determines whether DKC's computational limits are single-layer phenomena or fundamental to the bracket algebra.

---

## Tier 3: Someday

### 3.1 Abramsky depth: Geometry of Interaction interpretation
**What:** Develop the Geometry of Interaction interpretation to predict which bracket triples compute which functions BEFORE exhaustive search. The goal is a type-theoretic criterion.
**Why:** Currently, "which weights compute which function" is answered by brute force. A GoI interpretation would give a principled theory of DKC computability — the equivalent of a type system for topological neural networks.
**Effort:** LARGE (research program — requires deep engagement with Abramsky's categorical framework).
**Unblocks:** Theoretical prediction of computability without exhaustive enumeration. The "grand theory" of DKC.

### 3.2 5-input parity and beyond
**What:** Extend forward DKC to 5 inputs (616,126 NPN classes). Test triskelion generalization at k=10.
**Why:** Third data point for k=2n law. But 5-strand braid catalogs are computationally expensive and the NPN landscape is vast.
**Effort:** LARGE (3+ demos — catalog generation alone is non-trivial at 5 strands).
**Unblocks:** Third data point for triskelion generalization. Complete NPN hierarchy for 5 inputs.

### 3.3 Physical interpretation via Costello-Francis-Gwilliam
**What:** Costello-Francis-Gwilliam (2026) proved Witten = Reshetikhin-Turaev rigorously. Interpret forward DKC in terms of compiling QFT transition amplitudes into Boolean logic.
**Why:** "DKC compiles quantum field theory into Boolean logic" is an extraordinary claim that the CFG result makes rigorous. But the connection is conceptual, not yet formal.
**Effort:** LARGE (research program — requires engaging with derived algebraic geometry).
**Unblocks:** The deepest possible framing of DKC. Connects to physics community.

### 3.4 Galois symmetry conjecture
**What:** Investigate whether the sandwich duality (top layer isomorphic to bottom layer in the Loewy filtration) corresponds to a Galois automorphism of Gal(Q(zeta_8)/Q).
**Why:** If true, it explains a structural coincidence as an algebraic necessity. But it is currently pure speculation with no clear approach.
**Effort:** MEDIUM (requires number theory expertise — likely needs a collaborator).
**Unblocks:** Deep algebraic explanation for the sandwich theorem. Potential connection to Langlands-type phenomena.

### 3.5 GPTQ/Babai formalization
**What:** Formalize the observation that GPTQ weight quantization (Babai nearest-plane on Z^n) and DKC lattice computation (exact arithmetic on Z[zeta_8]) are two instances of lattice-based neural network theory.
**Why:** Connects DKC to the practical ML quantization community. Could yield a joint framework where training = lattice closest-vector search and DKC = exact lattice computation.
**Effort:** MEDIUM (1-2 demos + writing).
**Unblocks:** Bridge to applied ML community. Potential high-impact framing.

### 3.6 Volume conjecture connection
**What:** Investigate whether the volume conjecture (WRT invariants at large r relate to hyperbolic volume) has DKC implications. Does hyperbolic volume predict computational capacity?
**Why:** Speculative but would connect DKC to the deepest open problem in quantum topology.
**Effort:** LARGE (research program).
**Unblocks:** Connection to hyperbolic geometry and the broader quantum topology program.

---

## Summary: Critical Path

The shortest path to a strong first publication (Paper 1):

```
1.3 (catalog completeness) + 2.3 (Aizenberg relationship)
            |
            v
    1.2 (4-input parity)
            |
            v
      Paper 1 submission
```

The shortest path to Paper 2 (11/13 theorem):

```
1.4 (convexity proof)
      |
      v
Paper 2 submission
```

The shortest path to Paper 3 (radical anatomy):

```
1.1 (sandwich proof) --> [2.1 ell=7 DONE]
                              |
                              v
                       Paper 3 submission
```
