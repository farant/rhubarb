# Knotapel: proposed demos

Ideas for future demos, with the reasoning behind each, so a later
session can pick one up without the conversation that produced it.
Each proposal has a ledger desideratum in the tabularium, region
"knotapel research", which carries its lifecycle (open, taken, done);
this file carries the argument. Append new proposals at the end, dated;
when a proposal becomes a demo, add its number here and leave the text.

These are proposals, not claims: nothing here is graded. Where a
proposal leans on a mathematical fact recalled from memory rather than
checked in this repo, it says so, and the demo's first task is to
check it.

## 2026-10-15: after D127 — keep the depth

### Where these come from

D127 (audit P1) found that, under one exact activation, the group
catalogs compute XOR no better than non-group points of the same
height, and worse than generic points (KC 47.1.5). A catalog entry is a
braid word multiplied out through a linear representation into ONE
value, and that value becomes one weight in a depth-1 neuron (a signed
sum, then a cell). Any structure in the word that the product does not
keep is gone before the neuron sees it.

Fran's framing in the discussion that followed (2026-10-15):
- the useful computation is irreducible: it needs depth (layers,
  recursion, nonlinearity between steps), not one weighted sum;
- what a model has to capture is a graph of logical relationships, and
  the weights (the "angles" between things) can be solved FROM that
  graph, in a force-directed sense, rather than learned from statistics
  that are themselves shadows of the same relationships;
- this is the same problem as oratio's word-class disambiguation (the
  sudoku doctrine), and it connects to `notes.md` (compile syllogisms
  into DKC).

What already supports this:
- One threshold draws one hyperplane, and XOR is not separable by one
  (KC 10: three-input parity unreachable with the split-sigmoid; D50:
  parity needs more sectors). Parity is the textbook irreducible
  function: constant-depth circuits need exponential size for it
  (Håstad).
- D106 found topological entropy invisible to DKC for every Boolean
  function tested. Read with D127, that is expected: the TL value of a
  braid is a quotient that erases the dynamics, so whether the braid
  is periodic or pseudo-Anosov cannot reach the neuron.
- Linear representations (Burau, TL, quaternion images) compose to a
  single matrix, so depth collapses. To keep it, the layers need a
  nonlinearity between them.

Related work to read before each demo:
- relaxation labeling (Rosenfeld, Hummel and Zucker, 1976), the
  compatibility-driven iteration once used for part-of-speech tagging;
  Constraint Grammar (Karlsson) is its discrete cousin;
- Goemans–Williamson MAX-CUT: unit vectors per variable, constraints as
  angles, a semidefinite relaxation, then a random hyperplane as the
  threshold (within 0.878 of optimal);
- deep unfolding (Gregor and LeCun 2010; Hershey et al. 2014): each
  iteration of a solver becomes one layer, with the solver's
  coefficients as the layer's weights;
- RotatE and QuatE: knowledge-graph embeddings where each relation is a
  rotation (QuatE: a unit quaternion), and composition around a cycle
  should return to the start;
- chenglou/handwired (`../handwired` @ 4ea7fcb): programs compiled into
  affine + ReGLU + boolean-atom layers with exact FP32 contracts,
  weights written, not trained.

Lessons from D119–D127 that every proposal below inherits:
- exact arithmetic only (house libraries), and exhaustive counts where
  the space allows; never mix a sampled scale with an exhaustive one;
- register the verdict rule BEFORE the run, and name in it which nulls
  and which N decide (D127 review M1: an unnamed null made the rule
  unusable);
- control height (D127: height explained more than group did);
- judge distributions, not one listing (D127: D94's BFS prefix of 2I
  stood out; 20 random subsets of 2I did not);
- an independent oracle for every headline number, and plants.

The proposals, in the order I would take them:

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N1 | …AKY6Z7 | braids as exact ReLU networks (Dynnikov coordinates) | — | a depth-keeping, nonlinear, exact action of braids; foundation for N2 |
| N2 | …4291V2 | does the braid's depth compute? | N1 | the question D127 leaves: are knots useful when the depth is kept? bears on P3 |
| N3 | …YRF7DKP | the knot as a constraint system: SU(2) representations, exactly | house algebraic numbers | first demo where the knot itself is the input; an Alexander-polynomial oracle |
| N4 | …ZPRDMSM | compile a logical graph into weights (deep unfolding, exact) | a house circuit library? | Fran's "weights are a function of the logical relationships"; `notes.md` syllogisms |
| N5 | …Z4ASHS (oratio region) | oratio: a vector relaxation against `partitio`'s forced cells | N4 helps; oratio is paused | whether an analog layer can order open cells soundly |

### N1. Braids as exact ReLU networks

**Why.** A braid word is naturally deep: one crossing per step. The
representations knotapel used are linear, so the word collapses to one
matrix. There is a known action of the braid group that keeps the
depth AND is nonlinear: on Dynnikov coordinates (integer coordinates of
a curve system in the punctured disk), each Artin generator acts by
piecewise-linear update rules built from positive and negative parts,
max(0, x) and min(0, x). That is a ReLU layer with integer weights. A
braid word is then a deep, exact, untrained integer ReLU network, the
same four-instruction shape as handwired's programs. It is used to
solve the braid word problem and to compute topological entropy
(Dynnikov 2002; Dehornoy, Dynnikov, Rolfsen and Wiest, "Ordering
Braids"; Thiffeault's braidlab).

**Recalled, to verify first:** the exact update formulas. Take them from
a primary source, not from memory, and record the source in the plan.

**Design.**
- Implement the action on n strands exactly (`magnus` for coordinates
  that grow; integers only).
- Each generator as an explicit layer: affine maps plus max/min, with
  an operation count per generator.

**Checks (oracles independent of the implementation).**
- Braid relations act identically: σ_i σ_{i+1} σ_i = σ_{i+1} σ_i σ_{i+1}
  and far commutation, on many coordinate vectors.
- A word and its inverse compose to the identity.
- Word problem: words known equal act equally; the action of a word
  that is not the identity moves some vector.
- Entropy: the growth rate of coordinates under repeated σ_1 σ_2^-1 on
  3 strands is log((3 + √5)/2). Cross-check exactly against the
  spectral radius of reduced Burau at t = -1 (D106's machinery), which
  bounds entropy from below and is equal for this braid.
- Periodic braids (e.g. the full twist) show no exponential growth.

**Output.** A verified action, its per-generator layer form, and growth
data. No computing claim yet.

### N2. Does the braid's depth compute?

**Why.** This is the question D127 leaves. Knotapel has asked whether
knot-derived VALUES make good weights (answer: no better than their
height). N2 asks whether the braid's STRUCTURE, kept as depth with a
nonlinearity at every crossing, computes Boolean functions that the
collapsed value cannot.

**Design (to settle in the plan).**
- Input encoding: Boolean inputs written into the initial Dynnikov
  coordinates (options: inputs as signed unit offsets on chosen
  coordinates; inputs as the choice of a puncture's curve).
- The network: the braid word, applied exactly.
- Output: a threshold on one coordinate or one sign pattern, fixed in
  advance and the same for every word.
- Exhaustive over braid words up to length L on n strands (small L, n),
  counting which words compute XOR, AND, MAJ, and parity on 3 inputs.

**Nulls (register which decide, and at which L).**
- the SAME words through reduced Burau (linear, collapsed): depth kept
  versus multiplied away, with the words held fixed;
- random integer piecewise-linear networks of the same depth, width and
  coefficient size: is it the braid, or any ReLU net of that shape?
- shuffled words (same letters, random order): does order matter?

**Questions it would answer.**
- Does the crossing structure implement functions a depth-1 neuron
  cannot (3-input parity)?
- Is a verdict constant on conjugacy classes, and under stabilization
  (Markov moves)? If yes, it is a property of the closed braid's link,
  which is P3's question asked of a representation where it can hold.
- Does entropy (pseudo-Anosov against periodic) now matter, where D106
  found it invisible?

### N3. The knot as a constraint system: SU(2) representations, exactly

**Why.** A knot diagram is literally a logical graph: one generator per
arc, one relation per crossing (Wirtinger). Asking for a unit
quaternion per arc that satisfies every crossing's relation is Fran's
"solve the logical graph for the angles". Each relation is local; the
global topology decides whether they can all hold at once. The set of
solutions, the SU(2) representation variety, is a knot invariant (its
signed count is the Casson invariant; Kronheimer and Mrowka used it to
show Khovanov homology detects the unknot). This is the first knotapel
experiment where the knot itself is the input rather than a source of
numbers, and it is the ratio-on-a-graph picture (holonomy around
cycles) in its natural home.

**Recalled, to verify first:** nonabelian representations with
meridians of a fixed angle α branch off the abelian ones where
Δ(e^{2iα}) = 0, Δ the Alexander polynomial (Burde; de Rham). If that
holds as stated, `laqueus`'s Alexander polynomial gives an INDEPENDENT
oracle for where the solutions appear.

**Design.**
- Knots: trefoil, figure-eight, 5_1, 5_2 (small presentations from
  `laqueus` or `tabula_nodorum`).
- Unknowns: one unit quaternion per arc, meridians conjugate (same real
  part cos α).
- Solve exactly: with the angle as a parameter, the relations are
  polynomial equations. Exact solutions live in algebraic extensions,
  so this needs the house extension-field arithmetic (`extensio`), or a
  restriction to points with rational coordinates where they exist
  (state which).
- Output: for each knot, the angles where nonabelian solutions exist,
  their dimension, and the comparison with the roots of Δ on the unit
  circle.

**Bridge to the computing question.** If N2 shows that depth computes,
the solved angles are candidate weights that come FROM the knot's
relations, rather than from a catalog.

### N4. Compile a logical graph into weights

**Why.** Fran's second point: a trained model's statistics are shadows
of the constraints that generated the data, so the weights can be
derived from the constraints directly. Deep unfolding does this for
iterative solvers: K rounds of propagation become K layers, each weight
is a constraint coefficient with a provenance ("weight w = constraint
37"), and the nonlinearity is the solver's own update. `notes.md`
already asks for this ("we should be able to compile syllogisms into
dkc").

**Design.**
- A small constraint system with a plain discrete solver as the oracle:
  a 4x4 sudoku, a set of syllogisms ("all cats are mammals"), or one
  oratio sentence's agreement constraints.
- Compile each propagation round into an exact layer (affine + ReLU on
  exact bits, handwired-style), with the provenance of every weight.
- Verify, layer by layer, that the network's state equals the discrete
  solver's after each round.
- Measure depth: the number of rounds a puzzle needs is its
  irreducible depth; puzzles that need search (backtracking) mark where
  a fixed-depth network stops and recurrence (repeat until a settled
  bit, as handwired's springs do) begins.

**House-library rule.** A compiler from logic to exact layers is general
code. If it is more than demo scaffolding, it belongs in a house library
(knotapel only consumes); ask Fran before writing one.

### N5. Oratio: a vector relaxation against the forced cells

**Why.** The sudoku doctrine (oratio spec, decisions 36–42) trusts only
sound constraints and grows the FORCED share. A continuous relaxation of
the same constraints does not guess: the angles are the solver's
workspace. Rigid directions should coincide with forced cells, flat
directions (several equally good configurations) with open cells, and
frustrated cycles (constraints that cannot all close, the curl part of
a HodgeRank split) with contradictions or wrong rules. If the
correspondence holds, the relaxation is a principled ORDERING for open
cells, which the doctrine keeps in the last tier: ordering only, never
eliminating.

**Design.**
- One sentence at a time: candidate readings as nodes, agreement and
  binding constraints as signed couplings.
- Relaxation labeling in exact rationals (small sentences keep
  denominators manageable), or a vector relaxation with an exact
  certificate.
- Compare against `Oratio.partitio()`'s forced | ordered | open profile
  on the oracle sentences.

**Status.** Oratio is paused (since 2026-09-15). This belongs to oratio
rather than knotapel and is listed here because it came out of the same
discussion; its desideratum sits in the oratio region.
