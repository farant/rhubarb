# Transparency, Generalization, and Wisdom Circuits

Date: 2026-02-26

---

## The Transparency Requirement

The goal is not "dump fuzzy stuff on a black box ANN and hope for the best." The goal is
to understand ANN structure well enough to build specific kinds of reasoning into it
deliberately — with the same level of transparency that DKC achieves for Boolean computation.

DKC doesn't treat the ANN as a black box. It knows:
- What every weight does (axis-aligned cyclotomic integer)
- What the activation does (k-sector partition)
- Why certain combinations compute certain functions (dual-channel theorem, depth law)

Transparency is total. Eigenflower aims to reach that level of transparency for reasoning
in general — not just parity/XOR, but the full spectrum of how the Summa argues.

Syllogistic deduction is the easy case where mathematical clarity already exists. Analogical
reasoning is harder but not necessarily opaque if you build the architecture right.

---

## Aquinas's Own Taxonomy of Reasoning Modes

Aquinas himself distinguishes modes of reasoning with specific structure:

- **Demonstration** — syllogistic. A is to B, B is to C, therefore A is to C. This is the
  tensor logic layer. Transparent by construction.

- **Analogy** — works by proportionality. A is to B as C is to D. This is a structural
  mapping between domains. It has its own logic (can't conclude identity from analogy, only
  similarity of relation).

- **Argument from authority** — has specific weight depending on the source. Scripture is
  highest, then Church Fathers, then philosophers. This is essentially a weighted citation
  graph.

- **Reductio ad absurdum** — show that denying the conclusion produces a contradiction.
  This is backward chaining through the syllogism graph with negation.

- **Distinguo** — distinguishing senses of a term. "X is true in sense A, false in sense B."
  This is namespace resolution — the same word maps to different proposition nodes depending
  on sense.

Each of these has a different logical structure. In principle each could have its own
architectural component that's transparent in the way DKC is transparent for Boolean
functions. The hybrid architecture doesn't need ONE opaque fuzzy layer — it could have
multiple transparent-ish layers, each handling a specific mode of reasoning.

---

## The "Decompile" Goal Reframed

"Decompile existing models" from the CLAUDE.md makes more sense in this context. The goal
is not decompiling for curiosity. It's decompiling to understand what structural patterns
ANNs use for fuzzy/analogical reasoning, so you can then build those patterns deliberately
into a transparent architecture.

Reverse engineering the mechanism so you can forward-engineer it with clarity.

The GPTQ-as-CVP framework gives this a concrete tool: decompose trained weight matrices on
cyclotomic lattices and see if the lattice structure reveals logical/relational patterns
the model learned. If a trained LLM has internal circuits that implement something like
analogical reasoning, those circuits should leave algebraic signatures in the weights.

---

## Why LLMs Generalize — The Circuit Sharing Thesis

There is a reason why after a certain amount of training data LLMs generalize and exhibit
emergent intelligence. It is because there are algebraic patterns of relationships between
objects in reality, and once you have internal circuits that match those patterns, inputs
from different domains start sharing those circuits. That IS generalization.

Concretely: "parent is to child as cause is to effect as premise is to conclusion" — these
are all instances of the same abstract relational pattern (asymmetric dependency). A model
that develops an internal circuit for "asymmetric dependency" can apply it to family trees,
causal reasoning, and logical inference simultaneously. The circuit is domain-independent.
The inputs are domain-specific. Generalization happens when domain-specific inputs route
through domain-independent circuits.

This is also why scaling works — more data doesn't just give you more facts, it gives you
more instances of the same abstract patterns, which strengthens the shared circuits and
makes them more robust. At some critical mass of examples, the circuit "clicks" and
suddenly works for all instances of that pattern, including ones never seen in training.
That's emergence.

---

## The Hierarchy of Knowledge

This connects to a fundamental idea: there is a hierarchy of knowledge where abstract
patterns are primary and repeat across many domains.

In Thomistic terms this is the distinction between:
- **Scientia** (knowledge of particulars) — "Socrates is mortal"
- **Intellectus** (understanding of principles) — "All men are mortal"
- **Sapientia** (wisdom — knowledge of highest causes) — the abstract patterns that
  organize all other knowledge

The abstract relational patterns that enable generalization are closer to sapientia than
scientia. They are not facts about any particular domain — they are structural principles
that ORGANIZE facts across domains. "Asymmetric dependency," "part-whole composition,"
"cyclic return," "hierarchy of participation" — these are patterns that appear in theology,
philosophy, biology, physics, language, social structure.

A system that has these patterns as explicit, transparent circuits has something like
wisdom in the Thomistic sense — not knowledge of everything, but understanding of the
principles by which everything is organized.

---

## Wisdom Circuits

Call the abstract pattern circuits "wisdom circuits." They are:

- **Domain-independent**: the same circuit handles asymmetric dependency whether the domain
  is family trees, causal chains, or syllogistic inference
- **Reusable**: many domain-specific inputs route through the same wisdom circuit
- **Hierarchical**: some patterns are more abstract than others (asymmetric dependency is
  more abstract than "parent-child" which is more abstract than "Abraham begat Isaac")
- **The source of generalization**: a new domain generalizes immediately if its structure
  maps onto existing wisdom circuits

In the eigenflower architecture, wisdom circuits could live in BOTH layers and in the
interface between them:

### Wisdom circuits in the syllogism layer
The tensor logic equations themselves are wisdom circuits. Barbara (All M are P, All S are
M, therefore All S are P) is not about any particular domain — it's a structural principle.
When compiled as a tensor equation, it IS a reusable circuit. Every domain that has
all-some-therefore structure routes through the same equation.

### Wisdom circuits in the fluency layer
The ANN's learned representations of language structure are wisdom circuits too. If the
model learns that Latin ablative absolute constructions express "simultaneous background
condition," that's a domain-independent pattern applicable to any content expressed in
that grammatical form.

### Wisdom circuits in the interface
The mapping between natural language and logical structure may itself have abstract patterns.
"X est Y" (X is Y) maps to a predication. "Omnis X est Y" (Every X is Y) maps to a
universal affirmative. These mappings are wisdom circuits of the interface — structural
correspondences between linguistic form and logical content.

### Wisdom circuits spanning both layers
The most interesting case: patterns that require BOTH fluency and logic to express. Analogy
is the key example. "Grace is to the soul as light is to the eye" — the fluency layer
parses the four terms, the syllogism layer encodes the proportionality relation A:B::C:D,
and the wisdom circuit IS the proportionality structure itself, which spans both layers.

---

## Channeling Into and Out of Wisdom Circuits

The architecture needs mechanisms for routing domain-specific inputs through the right
abstract circuits and routing the abstract outputs back into domain-specific conclusions.

### Into (abstraction / pattern recognition)
- Fluency layer parses input into structured representation
- Pattern matching identifies which wisdom circuit(s) apply
- Domain-specific terms are bound to the circuit's abstract slots
- Example: "Every man is mortal, Socrates is a man" ->
  recognize Barbara pattern -> bind S=Socrates, M=man, P=mortal

### Through (abstract processing)
- The wisdom circuit operates on the abstract structure
- Syllogism layer: tensor equation computes the conclusion
- Analogy layer: proportionality mapping transfers structure
- The processing is domain-independent — same circuit, different bindings

### Out of (concretization / instantiation)
- Abstract conclusion is bound back to domain-specific terms
- Fluency layer generates natural language expression
- Example: abstract conclusion "All S are P" -> bind S=Socrates, P=mortal ->
  fluency generates "Therefore Socrates is mortal"

The routing problem — which wisdom circuit to activate for a given input — is itself a
pattern recognition task that probably lives in the fluency layer or the interface. It's
analogous to Orchid's data-dependent kernel selection: the input determines which
processing circuit to use.

---

## Connection to DKC

DKC's results are directly relevant here:

- **The depth law** (max_xor ~ depth + 6): recursive application of the same transformation
  builds computational power. Wisdom circuits gain power through recursive application —
  applying the same abstract pattern at multiple levels of a problem.

- **Algebraic coherence beats vocabulary** (D82): shared intermediate structure matters more
  than raw diversity. Wisdom circuits are powerful precisely because they are SHARED across
  domains, not because they are diverse.

- **The parity-lock theorem** (D92): encoding determines which functions are accessible.
  The way you PRESENT information to a wisdom circuit determines what it can compute. This
  is the "channeling in" problem — getting the routing right.

- **The activation determines capability** (D50): the readout mechanism controls what the
  circuit can express. This is the "channeling out" problem — the narration/generation
  step must not bottleneck the circuit's output.

---

## The Research Program

The path from here:

1. **Start with deductive wisdom circuits** — syllogistic forms compiled as tensor equations.
   These are fully transparent by construction. Build and verify.

2. **Study analogical wisdom circuits** — proportionality relations (A:B::C:D). These have
   known mathematical structure (morphisms between relations). Can they be compiled
   transparently? What algebraic structure do they need?

3. **Decompile trained models to find wisdom circuits** — take a trained LLM, decompose
   its weights, look for circuits that correspond to known abstract patterns. The
   GPTQ-as-CVP framework on cyclotomic lattices is the tool.

4. **Build fuzzy reasoning architectures informed by what you find** — once you understand
   how a trained model implements analogical reasoning (for example), build that mechanism
   deliberately into eigenflower's architecture with full transparency.

5. **Never accept a black box** — every component should be understood well enough to
   explain why it works, not just that it works. The DKC standard: know what every weight
   does, what the activation does, and why certain combinations compute certain functions.

The long game: a system where the wisdom circuits are not discovered by gradient descent
and trapped in opaque weights, but are understood, compiled, and composable — like
syllogisms are in the tensor logic framework, but extended to the full range of human
reasoning.
