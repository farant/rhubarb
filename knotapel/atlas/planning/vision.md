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

5. **Deterministic syllogism parser** — Using lapifex/chorda, build a parser
   that converts "All X are Y" / "Aristotle is a man" forms into structured
   representations with opaque token slots. First rhubarb library in the
   vision pipeline.

6. **Opaque token flow demo** — Take a compiled syllogism, feed it input with
   opaque tokens (e.g., subject=OPAQUE_1, predicate=OPAQUE_2), verify the net
   produces correct logical output with opaque tokens in the right positions,
   then substitute back to get the natural language answer.

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

---

*First recorded: 2026-02-20. Source: conversation during atlas defrag.*
*Updated: 2026-02-20. Source: second vision discussion — opaque tokens,
syllogism-as-training, wired integration, deterministic NL parser.*
