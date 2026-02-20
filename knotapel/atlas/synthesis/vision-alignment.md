# Vision Alignment: Current Research → Long-Term Direction

How the completed work (50 demos) positions us relative to the hybrid LLM vision
described in `planning/vision.md`. Generated 2026-02-20.

---

## Progress Toward Vision

The hybrid LLM vision requires: taxonomy → braids → DKC → compiled weights that
coexist with trained weights in a model that can dynamically update its factual
knowledge.

**What the 47 demos established:**

| Vision Component | Status | Key Evidence |
|-----------------|--------|--------------|
| Exact algebraic computation | PROVEN | Z[ζ₈] arithmetic, zero FP error (D29+) |
| Topology → Boolean logic | PROVEN | Forward DKC, all 13 NPN classes (D29, D50) |
| Finite compilable weight catalog | PROVEN | 100 values at δ=0, exhaustively searchable |
| Provable correctness | PROVEN | Exhaustive verification, analytical proofs (D48, D62) |
| Gate composition / chaining | DEMONSTRATED | Cascade thresholding, 1-bit adder (D19) |
| Taxonomy → braid encoding | NOT STARTED | The critical missing link |
| Multi-layer complex composition | NOT TESTED | Only bit-level cascade (D19) exists |
| Integration with trained weights | NOT STARTED | Architecture question, unexplored |
| Dynamic recompilation | NOT TESTED | Should be fast given finite catalog, unverified |

**Assessment:** The "knots → logic" half of the pipeline is solid. The "taxonomy →
knots" half is the primary gap. The integration architecture is the secondary gap.

---

## Opportunities Opened by Recent Work

### The 11/13 theorem opens activation design space
Demos 61-62 proved the half-plane wall is geometric, not algebraic. This means
the activation function is a design parameter, not a constraint. For the hybrid
LLM vision, this suggests compiled DKC layers could use purpose-built activation
functions (k-sector MVN, or something novel) rather than being forced into
standard sigmoid/ReLU.

### The axiality property simplifies hardware
At δ=0, bracket values are axial — only 1 of 4 Z[ζ₈] coordinates is nonzero.
This means each compiled weight is effectively a single integer × a phase
direction. For hardware implementation, this is a massive simplification:
8 possible directions × integer magnitude = a very compact representation.

### Cross-ℓ universality suggests robustness
The 11/13 wall holds across five different cyclotomic rings (ℓ=2-6), and Demo 60
confirmed all radical structure formulas at ℓ=7 (first cubic number field). This
ring-independence — now tested across quadratic AND cubic extensions — means the
compiled-weight approach isn't fragile. Good for engineering.

### The Fibonacci rank theorem connects to capacity
F(ℓ-1) as the radical's bilinear rank means there's a computable measure of
how much "deductive capacity" each evaluation level provides. Higher ℓ = more
capacity but more complex ring. This could inform choosing the right ℓ for
a given taxonomy size.

---

## Course Corrections

### Parity required non-standard activation
The parity wall (D48) and its resolution (D50) showed that standard activations
(sigmoid, half-plane) cannot access all the computation the lattice contains.
For the hybrid LLM vision, this means compiled DKC layers CANNOT use standard
neural network activation functions. They need k-sector or equivalent. This is
both a constraint and an opportunity — the compiled layers are architecturally
distinct from trained layers, which might actually make integration cleaner
(they're clearly separate subsystems).

### Exhaustive search doesn't scale
The current "compiler" searches |catalog|^(n+1) combinations. At 3 inputs
this is 100M (seconds). At 4 inputs it's 10B (heavy but feasible). At 10
inputs it's intractable. The vision requires either:
- Compositional compilation (build large functions from small compiled units)
- Predictive compilation (theory for which braid → which function)
- Restricting compiled weights to small logical units (syllogisms are 3-term)

The third option is actually fine for the vision — if compiled weights encode
individual syllogisms (3 terms each), and reasoning chains are built by
composition, then the 3-input scale is sufficient per unit.

---

## Shortest Paths to Vision Milestones

### Path A: Prove the syllogism–DKC connection (1-2 demos)
```
Current state → Syllogism→NPN mapping (1 demo) → Toy taxonomy→weight pipeline (1 demo)
```
This would confirm or deny that classical syllogisms naturally fit the 3-input
DKC framework. If yes, the taxonomy→logic half of the vision becomes concrete.
If no, we need to rethink.

### Path B: Prove multi-layer composition works (1 demo)
```
Current state → Multi-layer composition test (1 demo)
```
Feed DKC neuron outputs into other DKC neurons. If exactness survives, the
vision's "reasoning chains" become feasible. If not, cascade thresholding
(D19 style, collapse to bits between layers) is the fallback — still works,
just less elegant.

### Path C: Demonstrate dynamic recompilation (1 demo)
```
Current state → Toy taxonomy with add/remove fact → recompile → verify (1 demo)
```
Even with an ad hoc taxonomy→braid encoding, demonstrating that you can add a
fact, recompile weights, and get correct inference would be a powerful proof
of concept for the dynamic update story.

---

## Recommended Next Investigation

**The syllogism → NPN mapping** is the highest-leverage single demo for the
vision. It requires no new code infrastructure (existing NPN classification +
DKC search), answers the most critical question (do syllogisms fit 3-input
DKC?), and its answer determines whether Path A is viable.

If that succeeds, the toy taxonomy pipeline (Path C) becomes the natural
follow-up, because you'd have: taxonomy → syllogisms → NPN classes → DKC
weights — a complete (if toy) end-to-end pipeline.

---

*Generated 2026-02-20. Reads: narrative.md, novelty.md, connections.md,
four-lenses.md, planning/vision.md, inventory/demo-index.md (via entries
for D19, D29, D48, D50, D61-62).*
