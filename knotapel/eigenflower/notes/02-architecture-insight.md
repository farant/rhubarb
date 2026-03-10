# Eigenflower Architecture Insight — The Hybrid Model

Date: 2026-02-26

---

## The Core Idea

Eigenflower is not a syllogism calculator. It's a hybrid of two coupled systems:

1. **A fluency ANN** — a small neural network that handles language (parsing, understanding,
   generation). Trained on Bible + Summa corpus.

2. **A syllogism ANN** — a structured graph of compiled logical relationships that the
   fluency layer can query and traverse. Functions like an internal graph database the
   fluency layer can walk.

These two systems solve each other's problems.

---

## Why Two Layers

Normal LLMs try to do fluency AND reasoning with one system. The reasoning is unreliable
because it's all compressed into weights trained on token prediction. Eigenflower splits the
architecture at exactly the right seam:

- **Fluency is a pattern matching problem.** ANNs are good at this. Recognizing Latin
  grammar, parsing sentence structure, generating coherent text — this is statistical and
  can tolerate approximation.

- **Reasoning is a structural problem.** Exact algebra is good at this. "Given these
  premises, what follows?" is a graph traversal, not a generation task. This needs to be
  correct, not approximate.

The split means:
- The fluency layer solves the EXTRACTION problem — parsing Latin prose into logical
  structure that the syllogism layer can ingest.
- The fluency layer solves the GENERATION problem — producing coherent text from the
  logical conclusions the syllogism layer outputs.
- The syllogism layer solves the REASONING problem — actual valid inference with exact
  arithmetic, not statistical pattern matching that hallucinates.

---

## The Syllogism Layer

Not a neural network in the traditional sense. More like a tensor logic engine where the
Summa's syllogistic structure has been compiled into exact algebraic operators.

What it is:
- A graph of propositions connected by logical relationships
- Each syllogism is a tensor equation: SP[s,p] = H(SM[s,m] * MP[m,p]) for Barbara, etc.
- Forward chaining computes the deductive closure (all derivable conclusions)
- Backward chaining answers specific queries by recursing through equations
- Kernel operators (Hilbert space framework) provide closed-form reasoning

What it does:
- Stores compiled syllogisms as exact algebraic structure (Z[zeta_8] or similar)
- Answers "what follows from X?" by graph traversal
- Produces logical states, NOT text
- Can be extended incrementally — adding a new syllogism = adding a tensor equation

What it doesn't do:
- Parse natural language
- Generate text
- Handle analogy, metaphor, argument from authority
- Anything fuzzy or approximate

### Connection to DKC

This layer is where the knotapel crossover matters most. DKC provides:
- Exact arithmetic substrate (Cyc8, zero floating point)
- Proof that algebraic structure encodes computation (109 demos)
- The reservoir computing framework (fixed catalog + trained readout)
- Quantitative predictions (depth law, parity-lock, dual-channel theorem)

The syllogism layer's compiled operators could potentially live on cyclotomic lattices,
inheriting DKC's exactness guarantees.

---

## The Fluency Layer

A small trained neural network. Doesn't need to be a frontier model — needs to handle
theological Latin and English at a competent level.

What it is:
- ~50-100M parameters (sweet spot for M2 30-60 min training)
- Ternary weights (BitNet b1.58) for efficiency — integer-only inference
- Possibly recursive (TRM-style) for deeper processing from fewer params
- Trained on Bible (multilingual) + Summa (Latin/English)

What it does:
- Parse input text into propositions the syllogism layer understands
- Generate coherent text from logical states the syllogism layer produces
- Handle the "fuzzy" parts of language — grammar, style, context
- Recognize when a passage contains syllogistic structure vs other argument types

What it doesn't do:
- Reason logically (that's the syllogism layer's job)
- Store theological knowledge in its weights (that's the syllogism graph)
- Need to be "smart" — it's a translator between human language and logical structure

### Design implications

- Vocabulary needs to cover theological Latin + English. Even with byte-level / H-Net
  tokenizer-free approach, the embedding layer is a significant fraction of params.
- The model doesn't need long-range reasoning capability since reasoning is offloaded.
  It needs good LOCAL understanding (parse this sentence) and good generation (express
  this conclusion in Latin/English).
- Free Transformer's latent Z could encode "what mode am I in" — parsing vs generating,
  Latin vs English, premise vs conclusion.

---

## The Interface — The Hard Problem

How does the fluency layer query the syllogism graph? How does it narrate results back?
This is probably the most novel and hardest design problem.

### Fluency -> Syllogism (Parsing / Query)

The fluency layer reads text and needs to produce structured queries:
- "This sentence asserts that all X are Y" -> proposition node
- "This passage is arguing from premises A and B to conclusion C" -> syllogism structure
- "What does the compiled knowledge say about X?" -> graph query

This is essentially a semantic parsing problem. The Summa's structure helps enormously:
- "Videtur quod..." (It seems that...) = objection = premise to be refuted
- "Sed contra..." (On the contrary...) = counter-authority
- "Respondeo dicendum quod..." (I answer that...) = the actual argument
- "Ad primum/secundum/tertium..." = replies to objections

These structural markers are consistent across 3,000+ articles. A parser that exploits
them gets a huge head start.

### Syllogism -> Fluency (Narration)

The syllogism layer produces logical states:
- "From premises P1 and P2, conclusion C follows by Barbara"
- "Query Q has no derivation in the compiled knowledge"
- "Premises P1 and P3 are contradictory"

The fluency layer needs to narrate these into coherent text. This is closer to template
filling than free generation — the logical structure constrains what needs to be said,
the fluency layer handles how to say it.

### The Walk

When the fluency layer encounters a reasoning task, the interaction might look like:

1. Fluency parses input: "Is it true that X?"
2. Fluency formulates query to syllogism layer: find_derivation(X)
3. Syllogism layer walks the graph: X <- P1, P2 by Barbara; P1 <- P3, P4 by Celarent...
4. Syllogism layer returns the derivation chain (or "no derivation found")
5. Fluency narrates the chain: "X follows from P1 and P2. P1 in turn follows from..."

The "walk" is the key metaphor. The fluency layer doesn't reason — it navigates. The
syllogism graph is the territory. The fluency layer is the guide who can read the map
and describe the landscape in natural language.

---

## Dynamic Recompilation

This makes much more sense in the hybrid framing.

When you "record a new syllogism":
- Parse it with the fluency layer (text -> logical structure)
- Compile it into the syllogism layer (new tensor equation / graph node + edges)
- Recompute deductive closure (forward chaining picks up new consequences)

The fluency layer doesn't need retraining for this. The two layers update independently.
The syllogism layer grows by accretion — each new syllogism adds structure. The fluency
layer only needs retraining if the VOCABULARY changes (new terms it hasn't seen before),
not when new logical relationships are added.

This is a massive advantage over monolithic LLMs where "learning something new" requires
fine-tuning the entire model.

---

## Build Order

### Phase 1: Syllogism Layer (Self-Contained, Testable)

1. Tensor logic interpreter in C89
   - Parse syllogisms into tensor equations
   - Execute forward/backward chaining
   - Exact arithmetic throughout (Cyc8 or similar)

2. Manual syllogism compilation
   - Start with a small set (~50-100) of clearly formal syllogisms from the Summa
   - Compile by hand into tensor equations
   - Verify correctness: does forward chaining produce the right conclusions?

3. Kernel operators
   - Implement kernel ridge regression for reasoning operators
   - Closed-form solution: alpha = (K + lambda*I)^{-1} y
   - Test: can it derive conclusions it wasn't explicitly given?

4. Graph infrastructure
   - Proposition nodes, syllogism edges, derivation chains
   - Forward chaining for deductive closure
   - Backward chaining for query answering
   - Incremental update (add syllogism without recomputing everything)

### Phase 2: Fluency Layer (Requires Training Infrastructure)

5. Training infrastructure in C89
   - Matrix multiply (ternary for BitNet)
   - Backpropagation
   - Optimizer (AdamW)
   - Data loading for Bible + Summa corpus

6. Small language model
   - ~50-100M params, ternary weights
   - Train on Bible + Summa
   - Evaluate: can it parse Latin? Generate coherent text?

7. Structural parser
   - Exploit Summa's markers (videtur quod, sed contra, respondeo, ad primum)
   - Extract proposition structure from articles
   - Semi-automated syllogism extraction (fluency layer proposes, human verifies)

### Phase 3: The Interface (Novel Research)

8. Query formulation
   - Fluency layer -> syllogism layer protocol
   - How propositions are represented at the boundary

9. Narration
   - Syllogism layer -> fluency layer protocol
   - Template-constrained generation from logical derivations

10. The walk
    - End-to-end: text in, reasoned text out
    - Fluency parses, syllogism reasons, fluency narrates

---

## Open Questions

- What's the right representation for propositions at the interface? Domingos's Boolean
  tensors? Embedding vectors? Something domain-specific to Thomistic theology?

- How much of the Summa's reasoning is actually syllogistic vs analogical/authoritative/
  other? If only 30% is formal syllogisms, the graph is sparse and the fluency layer is
  doing a lot of heavy lifting for the other 70%.

- Can the syllogism layer handle degrees of certainty? Aquinas distinguishes between
  demonstrations (certain), probable arguments, and arguments from authority. A pure
  Boolean tensor logic only handles the first.

- Where does the Summa's structure of objection-reply fit? The objections are deliberately
  WRONG syllogisms. Does the graph store them as such? Does it store the error type?

- How does the system handle the multilingual corpus? Latin Summa + English Summa + Latin
  Bible + English Bible + Greek Septuagint. Does the fluency layer need to be multilingual,
  or do we train separate fluency models per language?

- Is the DKC cyclotomic lattice the right substrate for the syllogism layer, or is plain
  integer arithmetic sufficient? The DKC connection is beautiful but may be over-engineering
  for propositional logic that doesn't need Z[zeta_8]'s algebraic richness.
