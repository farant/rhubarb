# The skeleton and the settling — knots, ratios, grammar and weights

*Brainstorm, 2026-10-16, after D129. A conversation between Fran and
Claude (Opus 5.5) that started from "what do you think of the last demos"
and ran through handwired, ratio graphs, grammar compilation, LLMs and
the nominalist/realist question. Recorded so a later session can pick
the thread up without the conversation. It continues the 2026-10-15
discussion recorded in `demo-proposals.md` ("keep the depth", N1–N5).
Demos that came out of it are proposals N8 onward in that file.*

**Discipline note.** This is a brainstorm, not a set of claims: nothing
here is graded. Facts recalled from the literature are marked
*(recalled)* and listed in AUDIENDA at the end; a demo that leans on one
checks it first. Results from this repo cite their source (KC node,
demo, commit).

## I. Where we stood

- **D128** (KC 55): a braid word is an exact, untrained ReLU network on
  Dynnikov coordinates, weights in {-1, 0, 1}, no biases.
- **D129** (KC 56): on B_3 (classes up to length 8, m <= 12) the regions
  of w^m grow linearly, never exponentially; the PIECES (the function's
  own linear pieces) stay flat, 8 for every positive-entropy class.
  Regions belong to the word, pieces to the braid: Delta sigma_1 as
  [1 2 1 1] carves 12 regions, as [2 1 2 1] carves 6, same pieces.
  Networks of the same shape with permuted coefficients also grow
  regions linearly, but their pieces grow (46 against 8). What braids
  add is bounded functional complexity, not capacity.
- The lattice method saw 13 of 88 regions: sectors narrow by lambda^2
  per pass. Structure that matters can be invisible to sampling.
- Period 2 on B_4 forces the limit onto a wall; the converse fails.
- chenglou/handwired (`../handwired` @ 4ea7fcb): programs compiled by
  hand into affine + ReGLU + boolean-atom circuits, exact in FP32
  within declared contracts. Same thesis as nervus (a program IS a
  network, no training), engineering-first in FP32 where we are
  mathematics-first in exact integers.

## II. Placement against handwired

- **Same substrate:** affine maps and gated piecewise-linear
  nonlinearities. nervus is a narrower, exact sibling of handwired's
  `Circuit` IR (affine + ReLU only, no products; braid programs need no
  more, and are bias-free, hence cones).
- **D129's lesson for compiled nets:** the region count of a compiled
  network belongs to the COMPILATION (the presentation), not the
  program. The standard capacity measure (Montufar et al.) would mislead
  about handwired programs. Unrolling (handwired unrolls every loop; our
  w^m) bought linear region growth and no growth in pieces.
- **Where they would collide:** braid orbits grow like lambda^m, leaving
  FP32's exact integers after about log_lambda(2^24) passes (~17 at
  lambda = phi^2); structure concentrates near walls (950 of 4,014 E
  braids on B_4 end ON one). Braid networks are a natural stress test
  for handwired-style FP32 contracts.

## III. Ratio graphs and holonomy

Fran's question: a graph of nodes where each edge is a ratio — can knots
or braid words encode such things, or serve as shorthand?

- A graph with a number (or group element) on each edge is studied for
  its CYCLE products, the holonomy. All cycles trivial: the ratios come
  from node potentials (r_uv = x_v / x_u), a "flat" graph. Nontrivial
  cycles carry the content.
- **A knot diagram is a constraint system on edge labels:** label the
  arcs, impose a local rule at each crossing (conjugation for the knot
  group, a quandle operation, a linear rule with weight t for
  Burau/Alexander). Consistent labelings are the knot's representations;
  invariants are what survives the consistency (N3 is this picture).
- **Knots encode ratios, classically** *(recalled)*: Conway's rational
  tangle theorem (rational tangles <-> fractions p/q via continued
  fractions; twist = +1, rotation = -1/x); 2-bridge knots <-> fractions.
- **B_3 <-> continued fractions:** sigma_1, sigma_2 act as
  [[1,1],[0,1]], [[1,0],[-1,1]] in SL(2, Z) (Burau at t = -1, as D129
  uses). A pseudo-Anosov braid's attracting direction is a quadratic
  irrational with a periodic continued fraction; lambda is its unit.
  Continued fractions give provably best rational approximations
  *(recalled)*: an ideal encoding of a ratio in a precise sense.
- **Many numbers: Rauzy–Veech** *(recalled)*: train tracks and
  Rauzy–Veech induction for mapping class groups are multidimensional
  continued-fraction algorithms. Dynnikov coordinates are a projective
  vector of ratios moved piecewise-linearly; each linear region is one
  branch of such an algorithm.
- **ReLU networks are ratio graphs too** *(recalled)*: rescaling a hidden
  neuron's inputs by c and its outputs by 1/c leaves the function
  unchanged, so the gauge-invariant content is products of weights along
  paths ("path-norm"). The holonomy picture again.
- **A testable link to D129:** its exploratory slope (regions added per
  pass = cyclically reduced length, 1,816 of 2,108) may be the number of
  Farey (Stern–Brocot) steps per period of the attracting direction's
  continued fraction: new walls pile up toward the repeller, and on B_3
  approaching it is a Farey walk. (Proposal N8.)

## IV. Recursion: Temperley–Lieb, Dyck and Latin

Fran's wrinkle: the non-linear RECURSIVE structure (grammar rules as in
oratio, corpus statistics) is part of what knots might encode; the
non-linear part would be layers that just do the calculation.

- **Where recursion lives in knot theory:** the Kauffman bracket resolves
  each crossing into two smoothings and sums over states; each state is
  a set of NON-CROSSING nested arcs. Planar matchings are counted by
  Catalan numbers and are in bijection with balanced parentheses, the
  Dyck language — the prototypical context-free language
  (Chomsky–Schützenberger *(recalled)*: every CFL is a homomorphic image
  of Dyck intersected with a regular language). So TL diagrams are
  nesting, and the bracket is a weighted sum over nested structures, as
  a weighted grammar's inside algorithm is a weighted sum over parse
  trees: both are partition functions over nestings.
- **Latin is the right test:** free word order gives CROSSING
  dependencies (hyperbaton). The projective part of a dependency diagram
  is planar (TL-like); hyperbaton arcs cross (braid-like); agreement
  (case, gender, number) is a label carried along a strand that must
  match at its ends, like arc colorings. A Latin sentence's syntax looks
  like a tangle. Treebanks are in the repo
  (`oratio/probationes/fixa/ud/la_llct-*`, `la_circse-*`; `oraculum.sh`
  fetches ITTB/PROIEL/Perseus/UDante; reader `oratio_conllu.h`).
  (Proposal N9.)
- **Compiling would look like:** rules as gates (affine + ReLU, the
  ReLU picks a branch), corpus statistics as weights (exact rational
  probabilities: ratios on the edges of the grammar graph, normalization
  as the consistency condition), recursion as depth (one layer = one
  parser step, iterated like w^m; bounded depth, as handwired unrolls
  loops).

## V. Prior art: compiling grammar into weights *(recalled throughout)*

- **Rules into networks, then train (1990s):** KBANN (Towell and Shavlik,
  ~1994: Horn clauses -> network -> training; better on small data,
  rules extractable); automata programmed into RNN weights (Omlin and
  Giles); Siegelmann and Sontag (~1995: rational-weight RNNs Turing
  complete — unbounded precision).
- **Structure as vectors:** Smolensky's tensor product representations
  (1990); Harmonic Grammar (Smolensky and Legendre, *The Harmonic Mind*,
  2006); Pollack's RAAM (1990). Elegant; size grows with depth; never a
  practical parser.
- **Hierarchical languages by hand (2018–2021):** bounded-depth Dyck by
  hand-built RNN weights (Hewitt et al., ~2020); limits of
  self-attention on unbounded hierarchy (Hahn, ~2020) and hand-built
  attention for bounded Dyck (Yao et al., ~2021); weighted automata =
  linear RNNs (Rabusseau et al., ~2019); "Neural Networks and the
  Chomsky Hierarchy" (Delétang et al., ~2023: plain nets reliably learn
  only the regular level).
- **Program compilers:** RASP (Weiss et al., 2021), Tracr (Lindner et
  al., DeepMind, 2023: RASP -> transformer weights; used as ground truth
  for interpretability); handwired.
- **Statistics learned on fixed structure:** neural PCFGs (Kim, Dyer and
  Rush, ~2019).
- **Robustness:** expressivity solid (finite-state exact; bounded
  recursion costs depth/memory; unbounded needs unbounded precision or
  memory). Compiled-by-construction nets work but are brittle at their
  contracts. As an initialization, modestly positive. Broad coverage of
  real language: the historical failure (the long tail).
- **Apparent gaps (to confirm by a literature search):** knot/TL algebra
  as the ENCODING (structure = nesting + crossing with built-in
  invariants and composition); exact arithmetic end to end;
  free-word-order languages as the test bed.

## VI. Where this meets LLMs

- **Same machine:** a transformer is attention (matmuls + softmax) and
  MLPs (affine, gated nonlinearity — SwiGLU/GeGLU are handwired's ReGLU
  family). Weights are the program, the architecture the CPU. Tracr
  compiles into exactly this.
- **Different programmer:** compiled = sparse, one concept per unit,
  exact within contract, only what was written; trained = dense,
  superposed (more features than dimensions), approximate, covers the
  long tail, must be reverse-engineered.
- **Trained models find compiled-like structure** *(recalled)*: parse-tree
  distances linearly recoverable (Hewitt and Manning, structural probe,
  2019); circuits (induction heads); concepts as directions in
  superposition. An LLM seems to learn approximate, distributed versions
  of structures one could compile exactly.
- **Parallels with our work:** an LLM has fixed depth per forward pass
  and iterates by generating tokens (an unrolling, like w^m; chain of
  thought buys iterations). D129's regions-vs-pieces lesson applies to
  reading capacity from region counts in trained models too.

## VII. Nominal and real: what training converges to

Fran: is training an emergent, somewhat arbitrary structure (categories
arbitrary, nominalist), or does the model converge, like a limit, on an
ideal encoding of a structure that already exists? Knots might give an
angle on compiling the IDEAL compressed representation rather than the
verbose artificial one current compilers produce.

- **Our own data has the split:** regions belong to the word (the
  presentation, conventional, inflatable), pieces to the braid (the
  invariant). Current compilers compile the presentation — one unit per
  rule — and carve walls the function does not need.
- **The evidence suggests a middle position** *(recalled)*: convergence
  (the "Platonic representation hypothesis", Huh et al. 2024: models
  across modalities grow alike with scale; recurring features and
  circuits across models); apparent arbitrariness (features split by
  analysis resolution; many bases for one structure). Reading: the
  COORDINATES are conventional, the RELATIONS are real — what converges
  is the structure modulo the network's symmetries (rescaling,
  rotations of hidden spaces). Close to moderate realism: the form in
  the thing, abstracted; different abstractions as different coordinates
  on one form.
- **Knot theory is the mathematics of separating presentation from
  substance:** diagrams vs knots (Reidemeister moves), braid words vs
  braids, Garside normal forms *(recalled)* and Dynnikov coordinates as
  canonical representatives, invariants. Recipe for "compile the
  ideal": find the invariant structure (normal form, canonical
  coordinates) and compile THAT; two grammars for one language would
  compile to one object.
- **A measurable notion of "ideal":** a compiled network whose region
  complexity approaches its function's piece complexity (D129's two
  counts). Current compilers sit far above that line. (Proposal N10.)
- **A test of the realist hypothesis:** compile bounded Dyck exactly
  through a TL-structured encoding; train small networks on the same
  language; compare representations modulo symmetries. Convergence
  toward the TL encoding = evidence for a real form. (N10, second half.)
- **Limit:** nesting has a knot-theoretic home (TL/Dyck), crossings
  plausibly; lexical and semantic statistics may not be knot-shaped.

## VIII. The skeleton and the settling

Fran's refinement: the knot-shaped invariants are the LOGICAL relational
skeleton (syllogisms, facts); the statistical part is a force-directed
expansion that settles to solve for it. A new logical rule makes the
statistics settle differently.

- **An energy landscape:** logic = hard constraints, statistics = soft
  forces, representation = equilibrium. Harmonic Grammar,
  Hopfield/Boltzmann machines, knowledge-graph embeddings (TransE,
  RotatE, QuatE: facts as constraints, entity vectors settle), and
  attention as a modern Hopfield update *(recalled: Ramsauer et al.,
  "Hopfield networks is all you need")* — an LLM's forward pass settles,
  and a new rule in a prompt is a new constraint to settle around.
- **Rigidity makes "a new rule changes the settling" exact** *(recalled:
  Laman's theorem)*: a graph with springs is a framework; rigidity
  theory says whether the skeleton determines the shape or leaves it
  flexible. One more edge can rigidify a floppy framework or
  over-constrain it (stresses). **Frustration** (spin glasses): a cycle
  of constraints that cannot all be satisfied; contradictory facts as
  frustrated cycles, the holonomy != 1 of section III. (N5's HodgeRank
  curl is the same object; proposal N11.)
- **The deepest link: knot invariants ARE statistics settled on an
  algebraic skeleton** *(recalled)*: the Jones polynomial and the
  Kauffman bracket come from statistical mechanics — Boltzmann weights
  per crossing, a sum over states, a partition function — and are
  invariants only when the weights satisfy the Yang–Baxter equation,
  the braid relation sigma_1 sigma_2 sigma_1 = sigma_2 sigma_1 sigma_2.
  YBE says composing pairwise interactions gives the same total whatever
  the order: a consistency law for multi-party interactions. Skeleton =
  braid/YBE relations; statistics = weights and state sum; invariant =
  independence from how the diagram is drawn; change the skeleton (a
  different YBE solution) and the admissible weights and the invariant
  change. Baxter, Jones, Kauffman, Turaev. Knotapel's bracket work
  already lives inside this.
- **Knots also have the literal geometric version** *(recalled)*: ideal
  knots and knot energies (Möbius energy, ropelength): topology fixed,
  geometry relaxes to a canonical settled shape; change a crossing and
  it settles elsewhere. `laqueus` polygons could host it.
- **Honest note on syllogisms:** plain syllogistic logic is most
  naturally a lattice or category (inclusions; Barbara = transitivity).
  Knots add the ORDER in which interactions pass each other, with a law
  (YBE) making order globally irrelevant. Language's relations interact
  and do not commute (scope, quantifier order, attachment order), so
  the braid/YBE structure is a real candidate there; for plain
  inclusion logic a lattice may be enough.

## IX. Threads, in one line each

1. Farey steps explain D129's slope? (N8)
2. Latin dependency trees as tangles: how often, and how, do they
   cross? (N9)
3. Ideal vs verbose compilation, measured by regions vs pieces; does
   training converge toward the TL encoding? (N10)
4. A relational skeleton settling exactly: rigidity and frustration when
   a rule is added. (N11)
5. Braid networks under FP32 contracts: where patterns flip. (N12)
6. Gauge-invariant path products of braid networks against Burau and
   Alexander: are the network's holonomies the knot's invariants? (N13)
7. Not yet a demo: YBE weights off the Yang–Baxter locus — how fast does
   "invariance under redrawing" break, as an analogy for "conclusions
   independent of proof order"?
8. Not yet a demo: the shortest braid or tangle realizing a target
   ratio graph (knot as shorthand, as a search problem).

## AUDIENDA

Not verified in this repo (all recalled from memory during the
conversation; check before relying on any):
- Conway's rational tangle theorem as stated; continued fractions as
  best approximations; Rauzy–Veech as multidimensional continued
  fractions for mapping class groups.
- Chomsky–Schützenberger; the TL/Catalan/Dyck correspondence as used
  here (the counting is standard; the parse-forest analogy is ours).
- Every citation in section V, the Platonic representation hypothesis,
  Hewitt and Manning's structural probe, Ramsauer et al., Laman's
  theorem, knot energies, the Yang–Baxter/statistical-mechanics origin
  of the Jones polynomial (standard, but cited from memory).
- The "apparent gaps" in section V (needs a literature search; the
  deep-research skill is the tool).
- handwired was READ (README, docs/execution-model.md), not run.
