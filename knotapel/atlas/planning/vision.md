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

**The value proposition:** An LLM where you can add, remove, and correct factual
knowledge in real-time without retraining, without context stuffing, and with
provable correctness for the hard-fact portion.

**Philosophical grounding:** Maps onto the scholastic distinction between
*scientia* (demonstrative knowledge from first principles — compiled) and
*opinio* (probable knowledge from observation — trained). The taxonomy IS
the first principles; the corpus IS the observations.

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
Options include:
- Separate layers (compiled layers + trained layers)
- Mixture of experts (some experts compiled, some trained)
- Attention heads (some algebraic, some statistical)
- Memory-augmented architecture (compiled weights as an external exact memory)

Status: UNEXPLORED. Design question that could be explored theoretically.

### Gap 5: Dynamic Recompilation
Can you update compiled weights fast enough for "dynamic" to be meaningful?
DKC compilation is currently exhaustive search over a finite catalog — fast
for 3 inputs (~100^4 = 100M, seconds), unclear for larger inputs.

Status: DEPENDS ON scaling (Gap 6).

### Gap 6: Scaling Beyond 3 Inputs
222 NPN classes at 4 inputs. Catalog search space grows as |catalog|^(n+1).
Need either: smarter search (predict which braid → which function), larger
catalogs (more braids, higher strand counts), or compositional approaches
(build n-input functions from smaller compiled units).

Status: PARTIALLY EXPLORED (4-input DKC is planned as Demo 63).

---

## Near-Term Explorations (Next Few Demos)

1. **Syllogism → NPN mapping** — Enumerate 24 valid syllogistic forms, express
   as truth tables, classify by NPN, check DKC compilability. Single demo.
   Would confirm or deny the natural fit between syllogisms and 3-input DKC.

2. **Multi-layer composition test** — Feed DKC neuron output into another DKC
   neuron. Test both cascade-threshold (Demo 19 style) and full-complex-value
   passing. Single demo.

3. **4-input DKC** (Demo 63, already planned) — Tests scaling, triskelion
   generalization, and provides the second data point for structural laws.

4. **Toy taxonomy → weight pipeline** — Take a 5-node taxonomy, manually encode
   as braids (even if the encoding is ad hoc), compile to DKC weights, verify
   logical inference works end-to-end. Proof of concept, not general solution.

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

---

*First recorded: 2026-02-20. Source: conversation during atlas defrag.*
