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

*Became D128 (2026-10-15; KC 55).*

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

## 2026-10-15: after D128 (tentative: D128's review is still running)

### Where these come from

D128 (T6a-T6b, commits 7754f701, eb02efb5) found three things:

- **A braid word is an exact, untrained ReLU network.** On Dynnikov
  coordinates every crossing is a depth-4 block with two ReLU stages
  that touches at most 4 coordinates. The anchor against plexus held on
  4,000 random inputs, including coordinates up to 2^200.
- **The collapsed linear version is blind to much of what the network
  carries.** Burau(-1) is a product of matrices, so the depth is
  multiplied away. It misses the positive entropy of 1,008 of the
  ~4,000 positive-entropy braids on B_4 up to length 6. The shortest is
  s1 s3^-1 s2, at B_4's minimal dilatation 2.29663. The piecewise-
  linear action sees all of them. This is the conversation's "keep the
  depth" point, now measured.
- **D106's entropy classes were Burau's classes.** Its rule decides
  "Burau(-1) radius > 1", so about a quarter of its "periodic" braids
  have positive entropy. D106's comparison "pA braids don't compute
  better" (KC 31.2) rests on those classes.

If D128's review confirms these, the seam to explore is the one between
dynamics (entropy) and computation (what the network can express).

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N6 | …N84R01 | the linear-region census: is entropy expressive capacity? | D128 | the dynamics-computation link D106 tried to test, on the right object; tells N2 which braids could compute parity |
| N7 | …SKVB0QC9 | re-cut KC 31.2 with exact classes | D128 | whether "pA braids don't compute better" survives correct labels |

The exact dilatation (the plexus slice deferred in T4, Hall-Yurttas
via the stabilized activation pattern) is infrastructure that N6
benefits from, not a demo. It has its own ledger entry under plexus
(…VFF141H).
D128 Part D will show whether the pattern stabilizes.

### N6. The linear-region census: is entropy expressive capacity?

*Became D129 (2026-10-16; KC 56).*

**Why.** The standard measure of what a ReLU network CAN compute is the
number of its linear regions: the pieces into which its activation
patterns cut the input space. Deep networks can make exponentially more
regions than shallow ones of the same size (Montufar, Pascanu, Cho and
Bengio, "On the number of linear regions of deep neural networks",
NeurIPS 2014; to read before the plan). Braid networks are exact and
integer, so their regions can be counted exactly, over integer points
or as polyhedra. The question D106 asked ("does chaos help computing?")
then has a precise form: does a braid's dilatation set the rate at
which its network carves the input space? D106 could not answer it: it
measured bracket values (a linear quotient that erases the dynamics,
KC 31.4) under Burau's classes (about a quarter mislabeled, D128).

**Hypothesis to register.** For pseudo-Anosov braids, the number of
distinct activation patterns over a fixed box of loop coordinates,
under w^m, grows exponentially in m, at a rate tied to lambda. For
periodic braids it stays bounded, and for zero-entropy reducible braids
it grows polynomially. A negative result matters too: if entropy does
not buy regions, the ReLU structure of braids is computationally inert,
and N2 should be rethought before any search for parity.

**Design (to settle in the plan).**
- The input space is the integer box [-R, R]^(2n-4) of Dynnikov
  coordinates (every integer vector is a lamination), or a projective
  slice of it.
- The activation pattern of a point is the vector of ReLU signs
  (positive vs zero) along the network, from D128's layer programs.
- Count distinct patterns for w^m, m = 1..M, for every braid of the
  D128 census, by class (P, Z, E) and against the measured rate.
- Exactness: patterns are integer sign data, so counts on a lattice box
  are exact. A count of polyhedral regions (not lattice points) is a
  different object; the plan says which one is reported and why.
- Nulls: the same words through Burau (one linear region, by
  construction); random PL maps of the same shape; and shuffled words.
- Size: the box grows as (2R+1)^(2n-4). B_3 (2 coordinates) and B_4 (4)
  are cheap at moderate R; B_5 needs sampling, marked as SAMPLED.

**What it would settle.** Whether exact entropy predicts expressive
capacity, the precise version of D106's question. It also gives N2 its
candidates: the braids whose networks have enough regions to compute
3-input parity.

### N7. Re-cut KC 31.2 with exact classes

**Why.** KC 31.2 records D106's verdict that pseudo-Anosov braids do not
compute better, which holds under the intended hash (D117). Its classes
were Burau's. D128 shows about a quarter of the "periodic" ones have
positive entropy. Re-splitting D106's bracket-value results by D128's
exact classes (P, Z, E) tells whether the verdict survives correct
labels. This is a small audit node (a re-run of D106's comparison with
the corrected hash from D117 and the classes from plexus), not a new
direction.

**Design.** Rebuild D106's catalog to depth 5 (D128 shows it equals the
exact shortest-length enumeration there), take its bracket-value
computing scores (the D117 intended hash), and compare across P / Z / E.
Report the change against D106's two-class table. Its TL representation
at zeta_8 has to come from a house library or a frozen D106 snapshot;
the plan settles which.

## 2026-10-16: after D129 — the skeleton and the settling

### Where these come from

D129 (KC 56) found that on B_3 the regions of w^m grow linearly while
its pieces stay flat, that regions belong to the word and pieces to the
braid, and that random networks of the same shape grow regions linearly
too but grow pieces as well. The conversation that followed (recorded
in full in `brainstorm-skeleton-settling.md`) placed this against
handwired, ratio graphs and holonomy, B_3 continued fractions,
Temperley–Lieb nesting and the Dyck language, Latin hyperbaton as
crossings, the prior art of compiling grammars into weights, the
nominalist/realist question about what training converges to, and
Fran's picture of a logical skeleton that statistics settle around
(with the Yang–Baxter link: knot invariants are statistics settled on
an algebraic skeleton). Read that file first; each proposal below names
its section.

Inherited from D119–D129: exact arithmetic; the verdict rule registered
before the run, naming its nulls; an independent oracle for every
headline number; plants; and from D129, never read capacity off region
counts alone (count the function's pieces too).

The proposals, in the order I would take them (smallest and most
exact first; each settles something cheaply before the larger ones):

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N8 | …D8JX2V | Farey steps and D129's slope (B_3) | D129 | a law with a reason for KC 56.1.3's post-hoc pattern |
| N9 | …3X58Q9 | Latin dependency trees as tangles | oratio's CoNLL-U reader | how often and how Latin crosses; the tangle picture tested on real data |
| N10 | …HD428V | ideal vs verbose compilation: bounded Dyck | nervus, nervus_regiones_plani (2 inputs) or the d >= 3 counter | a measurable "ideal"; first half exact, second half (training) to plan |
| N11 | …0TNHGT | a relational skeleton settling: rigidity and frustration | matrix (exact) | Fran's "a new rule settles the statistics differently", exactly |
| N12 | …NSR9XG | braid networks under FP32 contracts | nervus, plexus | where float evaluation of a braid network first goes wrong; a bridge to handwired |
| N13 | …KVA8DGY | gauge-invariant path products of braid networks | nervus, plexus | are the network's holonomies the knot's invariants? |
| N14 | …9XK1EA (oratio region) | toy Latin Sudoku: slot library, exact propagation and settling | matrix/fractio (exact); oratio's ideas, not its code | the laboratory version of oratio's sudoku doctrine; precursor of N5 |

### N8. Farey steps and D129's slope (B_3)

**Why.** D129's exploratory pattern (KC 56.1.3): over m = 6..12 the
most-regions word adds 6 x its cyclically reduced length in 1,816 of
2,108 positive-entropy classes (196 fewer, 96 more; 1,804–1,836 over
tie choices). A post-hoc pattern with exceptions wants a reason. On B_3
the braid acts through SL(2, Z); a pseudo-Anosov braid's repelling and
attracting directions are quadratic irrationals with periodic continued
fractions, and new walls of w^m pile up toward the repeller. Approaching
it is a walk in the Stern–Brocot (Farey) tree. Hypothesis: regions
added per pass = the number of Farey steps (the sum of the partial
quotients) per period, and the exceptions are words whose letters and
Farey steps disagree. (Brainstorm III.)

**Recalled, to verify first:** the correspondence between B_3 words and
continued-fraction expansions of the eigen-directions (sign and
orientation conventions), and the Farey interpretation of the walls.

**Design.** For every E class of D129's census (and every geodesic
word): the exact repelling direction (a quadratic irrational from the
SL(2, Z) matrix, via `radices`), its continued-fraction period, the
number of Farey steps per period; compare with the exact regions added
per pass from D129 (`nervus_regiones_plani`, already exact). Register
the rule before the run: "equal for every E word" or "equal except on a
named set", with the exceptions characterized.

**Oracle.** The continued fraction computed two ways (from the matrix's
eigenvector, and by iterating the PL map on a point near the repeller);
D129's frozen counts.

**Output.** A law (or its failure) for the growth rate of regions on
B_3, replacing the post-hoc cyclic-length pattern.

### N9. Latin dependency trees as tangles

*Became D130 (2026-10-17; KC 57): literary Latin crosses as
agreement, the charters do not (P2 falls on LLCT); tangles are small.*
*Corrected after D130's T4 review (15e6df92): that note used the
amended operand. On the REGISTERED operand P2 falls in four treebanks
(LLCT, PROIEL, ITTB, UDante); only the verse-heavy CIRCSE and Perseus
pass. The displaced strand of a crossing carries agreement; its
partner seldom does (brainstorm XIII).*

**Why.** Brainstorm IV: a Latin sentence's syntax looks like a tangle
— a planar (projective, TL-like) skeleton, crossings where hyperbaton
separates words that belong together, agreement as labels carried
along strands. Before compiling anything, measure whether real Latin
looks like that: how many arcs cross, how crossings are distributed,
whether they are bounded (as mildly context-sensitive formalisms
assume), and which relations cross.

**Design.**
- Data: the Latin UD treebanks in the repo
  (`oratio/probationes/fixa/ud/la_llct-*`, `la_circse-*`; `oraculum.sh`
  fetches ITTB, PROIEL, Perseus, UDante), read with oratio's CoNLL-U
  reader (`oratio_conllu.h`). English EWT (also present) as the
  contrast: an order-rigid language.
- Per sentence: the arc diagram of its dependency tree; crossing pairs;
  the crossing number of the arc diagram; whether the tree is
  projective; which relation labels (amod, nmod, obj, ...) take part in
  crossings; whether the crossing arcs carry agreement (shared case,
  gender, number in the features column).
- Map each sentence to a tangle word: the arcs as strands, each crossing
  as a signed generator (the convention fixed in the plan), and report
  the distribution of word lengths and of the planar (TL) part.

**Questions it would answer.** Is Latin's non-projectivity rare and
shallow (a few crossings per sentence) or pervasive? Are crossings
concentrated in agreement relations (adjective–noun hyperbaton), which
is where the "labels along strands" picture predicts them? How does
this compare with English?

**Nulls.** Random trees with the same degree sequence, and the same
trees with words shuffled under a fixed head-direction model: are
Latin's crossings more structured than chance?

**Note.** This sits between knotapel and oratio (paused). It needs no
new library: oratio's reader plus counting. Exact by nature (counts).

### N10. Ideal vs verbose compilation: bounded Dyck

**Why.** Brainstorm VII: current compilers compile the presentation
(one unit per rule) and carve walls the function does not need; D129's
two counts give a measurable notion of "ideal" — a compiled network
whose region complexity approaches its function's piece complexity.
Bounded Dyck (balanced parentheses of depth <= D) is the cleanest test:
the prototypical nesting language, with a TL/Catalan structure.

**Design (first half, exact, buildable now).**
- Two compilations of a bounded-Dyck recognizer (or a depth tracker)
  into nervus networks: a VERBOSE one in the style of the compilers of
  brainstorm V (a counter with one gate per condition), and a
  STRUCTURED one built from the TL/nesting structure (the plan designs
  it; the question is whether one exists that is smaller and carves
  fewer walls).
- For inputs encodable in 2 coordinates, count regions and pieces
  exactly with `nervus_regiones_plani`; beyond 2 inputs this needs the
  d >= 3 counter (ledger …QK7A9) or an exact restriction to 2-dimensional
  slices (the plan says which).
- Measure: size, depth, regions, pieces, and the ratio regions/pieces,
  as the depth bound D grows.

**Second half (to plan separately; it involves training).** Train
small networks on the same language and compare their representations
with the structured compilation modulo the network's symmetries
(rescaling, permutations). Convergence toward the structured encoding
as models improve would be evidence for "converging on a real form";
scattered results would favour the nominalist reading. Training is
floating point: the plan must say where floats enter and how the
comparison is made exact (e.g. exact analysis of the trained network's
activation patterns and pieces).

### N11. A relational skeleton settling: rigidity and frustration

**Why.** Fran's picture (brainstorm VIII): the logical skeleton is
fixed; the statistics settle around it; a new rule makes them settle
differently. In rigidity theory that is exact: a framework's skeleton
either determines its shape or leaves it flexible; one more edge can
rigidify it or over-constrain it (stress); in spin-glass terms a cycle
of constraints that cannot all hold is frustrated. N5 proposed the same
objects for oratio (rigid directions = forced cells, frustration =
contradiction); N11 is the clean, small version first.

**Design.**
- Skeletons: small relational graphs from syllogism-style facts ("all
  A are B", "no B is C", ...), encoded as constraints on vectors
  (inclusion as a signed inner-product constraint, or relations as
  rotations around cycles, RotatE/QuatE style — the plan picks one
  and says why).
- Settling: minimize a quadratic energy (constraints as springs), so
  the equilibrium is a linear system solved EXACTLY over the rationals
  (`matrix`). Rigidity = rank of the rigidity matrix (exact); stresses =
  its left kernel; frustration = the residual around each cycle (the
  holonomy of brainstorm III).
- Experiment: add one rule at a time (consistent, redundant,
  contradictory) and record how the equilibrium moves: rigidification,
  new stress, or frustration localized to a cycle. Compare with a
  discrete logic oracle (which conclusions follow; which rule set is
  inconsistent).

**Questions it would answer.** Do "follows from the facts" and "rigid
in the settled configuration" coincide? Does a contradiction show up as
frustration on exactly the cycles a logician would name? This is the
smallest test of skeleton-plus-settling, and the road to N4 and N5.

### N12. Braid networks under FP32 contracts

**Why.** Brainstorm II: handwired is exact in FP32 within declared
ranges and grids; braid orbits grow like lambda^m, and their structure
concentrates near walls. Where does float evaluation of a braid network
first go wrong, and does handwired-style contract reasoning predict it?
Cheap, practical, and the concrete bridge between the two projects.

**Design.** For the D128/D129 censuses: evaluate w^m in FP32 (round
after every operation, handwired's rule: f(bias + f(f(c) x v)) per
term) and exactly (Magnus), on the loops E_k and on lattice points;
record the first pass where (a) a value differs, (b) an activation
pattern differs, (c) the settled period or the tie verdict of D129 Part
C would change. Compare with the prediction log_lambda(2^24 / |x_0|) for
(a), and with the distance to the nearest wall for (b).

**Output.** A table of failure passes by lambda and starting height,
and a statement of which contract (range, grid) would have made FP32
evaluation exact for how many passes. handwired itself is not run; its
rounding rule is reimplemented in the demo (or a house helper) from its
documentation.

### N13. Gauge-invariant path products of braid networks

**Why.** Brainstorm III: a ReLU network's function is unchanged by
rescaling a hidden neuron's inputs by c and outputs by 1/c, so its
gauge-invariant content is products of weights along paths (the
path-norm literature). A knot's invariants come from labels on arcs
made consistent at crossings. Are a braid network's path products (per
activation pattern, or for the settled linear map of D128 Part D)
related to the braid's Burau matrix, its Alexander polynomial, or the
dilatation? A positive answer would make "the network's holonomies are
the knot's invariants" precise; a negative one would say the ReLU
structure carries something the linear invariants do not.

**Design.** For each braid of the B_3 and B_4 censuses: the network's
path-product generating function per pattern (paths from input to
output weighted by coefficient products, exact via `nervus` and a
small path enumeration), the settled map's characteristic polynomial
(D129 Part C's machinery), and Burau / Alexander from `plexus`.
Register before the run which identities are tested (e.g. "the settled
map's characteristic polynomial divides / equals Burau's at some t").

**Recalled, to verify first:** the rescaling symmetry for bias-free ReLU
networks (standard), and whether any known result relates Dynnikov or
train-track transition matrices to Burau (Bestvina–Handel train tracks
give the dilatation as a Perron–Frobenius eigenvalue; the link to Burau
is through Kolev's bound and is known to be inexact).

### N14. Toy Latin Sudoku: slot library, exact propagation and settling

*Added later the same day (brainstorm XI).*
*Slice 1 = D131 (plan approved 2026-10-19): does morphology pay for
crossing? Real endings at four morphology levels, readers with and
without crossings, exact. Slice 2 (the settler at three temperatures,
brainstorm XIV) still open.*
*D131 done (KC 58): both registered predictions failed (measure
design); descriptively, endings pay for displaced ADJECTIVES (garden
paths 0% / 6.5% / 35.7% / 78.9% from ideal to bare) and not for
displaced GENITIVES (68-99% at every level). Slice 2's settler has
the genitive as its target.*

**Why.** Fran's refinement of the skeleton: not syllogisms or facts but
"a library of things that can take different kinds of slots, and an
iterative process arriving at the most likely classification for a set
of candidates" — a force settling, and literally oratio's task. Latin
makes the slots visible (case endings are slot-type labels on the
word), and the structure is Sudoku: cells = words, candidates = their
analyses, constraints = slot frames (valency, agreement), propagation
eliminates, statistics order what stays open. The brainstorm (XI)
connects this to relaxation labeling, belief propagation, transformer
layers and Mercury-style diffusion as one settling picture, and to
knots through tensor networks and braided monoidal categories
(DisCoCat: planar string diagrams for projective analyses, a braiding
for crossings).

**Design.**
- A toy world with exact ground truth: a few verbs with case-valency
  frames, a few agreeing adjectives, a few nouns with deliberately
  ambiguous endings; EVERY grammatical sentence up to 5–6 words in all
  word orders, with a hyperbaton dial (0 = strictly projective ... free
  order) and an order-preference knob for the statistics.
- Oracle: exact constraint propagation (the forced cells) and full
  enumeration (the solution set per sentence).
- Settler: relaxation labeling or belief propagation in exact rationals
  (small factor graphs keep denominators manageable), producing the
  ordering of open cells; its fixed point compared with the oracle's
  marginals.
- Measured: the forced | ordered | open profile per sentence and per
  dial setting; settling depth (rounds until stable); a left-to-right
  (autoregressive) analyser against parallel settling; the rule-change
  knob (add one valency frame, record which cells flip from open to
  forced, and where frustration appears).

**Register before the run:** which settler, which statistics, and the
verdicts tested (e.g. "settling never contradicts a forced cell", "the
settled ordering ranks the oracle's true analysis first in at least X%
of open cells at dial d").

**Relation to oratio.** This is the laboratory version of oratio's
sudoku doctrine on a world small enough to enumerate. It borrows
oratio's ideas, not its code (oratio is paused); its findings would tell
N5 what a relaxation can and cannot do before it meets real Latin.


## 2026-10-19: after D131 — what endings can and cannot do

### Where these come from

D131 (KC 58) found, descriptively, that endings pay for displaced
ADJECTIVES and cannot pay for displaced GENITIVES: an adjective repeats
its head's features (a head label), a genitive carries only its role
(a type label). Both registered predictions failed, by a near-tie and a
Simpson's paradox. The conversation that followed (brainstorm section
XV) asked what trained weights would look like on this task, what
multi-head attention contributes, and Fran's idea of UMBRAE: slots the
structure implies but the input does not yet (or never) supply,
carried forward through the evaluation.

Inherited: exact arithmetic where possible; verdict rules registered
before the run, each measure walked through three or four hand
sentences of EVERY kind before registering (D131's lesson: a difference
measure and a pooled split both misled); stratify by kind before
pooling; an independent oracle; plants; an independent review.

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N15 | …PEYPY4 | real Latin and the displaced genitive | dependentia, conllu, D130's treebanks | how readers recover what endings cannot place; D131's idea against real data |
| N16 | …JCWDDF | train against the compiled ideal | a new house trainer library; D131's world | size, heads, registers, grokking; does training converge on the ideal? |
| N17 | …NBF5ZX | umbra width of the toy world | D131 | how many open expectations a reader carries, and how many are findable by content |
| N18 | …3AHG53 | umbra nodes: pro-drop in the slot library | D131 | empty slots as candidates; how pro-drop changes ambiguity and garden paths |

### N15. Real Latin and the displaced genitive

*Became D132 (plan approved 2026-10-19): are displaced genitives SAFE
(no other noun could host them with a nested arc) more often than a
per-sentence positional null? G1, G2 registered.*
*D132 done (KC 59): G1 HOLDS (safe above the null in 6 of 6
treebanks), G2 HOLDS (most safe in 5 of 6; CIRCSE 45% fails). Small N
(5-80 displaced genitives per treebank); the charters almost never
displace a genitive (5 of 2,382).*

**Why.** D131 says a displaced genitive cannot be placed by its ending.
Real Latin displaces genitives anyway: D130's Part C table already
shows nmod over-represented among per-arc non-projective arcs in five
of six Latin treebanks (PROIEL 10.2% against 4.5% of all arcs, Perseus
12.4% / 4.9%, ITTB 11.6% / 6.0%; LLCT the exception). So the plain
prediction "writers avoid displacing genitives" is already contradicted
by numbers we have seen, and must not be registered. The live question:
how is a displaced genitive recoverable?

**Design (to sharpen at planning).**
- Restrict to nmod arcs whose dependent has Case=Gen; compare displaced
  (per-arc non-projective) with nested ones, per treebank.
- Candidate hosts: for a genitive, the nouns in its sentence that a
  NESTED attachment could reach (an attachment that would make its arc
  projective). Hypothesis: displaced genitives have FEWER nested
  alternative hosts than nested ones do (writers displace where the
  context leaves one candidate), or their true host agrees with
  something else that disambiguates (e.g. the host carries a displaced
  adjective too).
- Nulls: the same counts over random re-attachment, and over adjectives
  (where agreement should make the host count irrelevant).
- NOT seen before registering: the Case=Gen split and every candidate-
  host count. Seen: D130's nmod shares (above), which must be disclosed.

**Register before the run:** the host-count measure, the comparison,
the strata (treebanks, verse / prose), and the threshold.

### N16. Train against the compiled ideal

**Why.** Brainstorm VII's question (does training converge on a real
form or an arbitrary one?) needs a world whose ideal is KNOWN. D131's
world is one: the grammar compiles to a small exact network, and every
string's true analysis set is enumerated.

**Design.**
- Compile the ideal by hand (handwired-style or on nervus): embedding
  to candidate features, one agreement matcher per word pair, counting
  for the exactly-one constraints. Count its weights and pieces.
- Train small transformers on the same task (soft targets = the
  analysis distribution; or R_P's choice), with a sweep of heads (1, 2,
  4, 8) at fixed parameters, 0 vs k register tokens, and training on
  10% of the world to watch for grokking.
- Measure: exact accuracy against the enumeration; probes for the
  candidate sets; each head against the compiled heads (one relation
  per head, or superposed?); the agreement head's QK (block-diagonal,
  rank ~10?); where genitive uncertainty lives; pieces vs regions
  (D129's lens).
- **Needs a house trainer library** (plain gradient descent in C89,
  deterministic float). Substantial; it also serves N10's second half.
- **Ternary sub-experiment** (added 2026-10-20, brainstorm XVI): also
  train the same model TERNARY from scratch ({-1, 0, +1} weights with a
  scale, BitNet b1.58-style, straight-through gradients). Prediction:
  the agreement head (skeleton: 0/1 feature matching, natively ternary)
  survives almost intact; calibrated uncertainty on displaced genitives
  (settling: graded statistics) degrades first. The compiled ideal is
  known, so which part breaks is measurable.

**Register before the run:** the head-count knee, the agreement-head
rank, and the genitive calibration predictions (brainstorm XV).

### N17. Umbra width of the toy world

**Why.** Fran's umbrae: reading left to right, a reader carries the
slots the structure has promised and not yet filled. In knot terms
they are open strand ends; projective reading keeps them in stack
order, a crossing closes one out of order. D131 says agreement makes an
umbra findable by content; a genitive's is not. How much memory does
each reader need, and how much of it is content-addressable?

**Design.** Over every pair of D131's world (and each morphology
level): for each prefix, the open umbrae of the TRUE analysis (words
whose head has not appeared, and heads whose required dependents have
not), each marked labelled (an agreement label distinguishes it from
the other open umbrae) or unlabelled; the maximum width per pair; the
depth at which an out-of-order umbra is closed. Exact, cheap: D131's
data, a new measure. Relation to working-memory accounts of reading
(Gibson, recalled).

**Register before the run:** e.g. "crossed pairs need a content-
addressable umbra in every case where R_P reads them, and the umbrae
R_P fails on are unlabelled" - to be walked through hand sentences
first.

*Became D134 (2026-10-20, plan reviewed before any data): umbrae
(dangling, slot, root), width and depth over every pair up to SIX words,
and two labels on each displaced arc - COMPETITION (a nearer word fits
it) and RETRIEVAL (the arriving word cannot single out its partner).*
*D134 done (KC 61): U2a HOLDS - garden paths follow competition (a
positive control); U2b FAILS - interfered genitives are misread LESS
(-10.30 points), but the T4 review showed it is R_P's fallback on
strings with no projective analysis (cross-serial genitives stranded
across the verb); elsewhere interference raises misreading; U1 FAILS
narrowly - Latin keeps 64.5% of the ideal's retrieval over case-stripped
endings. The umbra stack is shallow (depth at most 3), and only
dependent-first displacement deepens it (a theorem).*

### N18. Umbra nodes: pro-drop in the slot library

**Why.** D131 required an explicit subject. Real Latin drops it: the
verb's ending carries the subject's person and number. An empty slot
becomes a CANDIDATE ("is this nominative the subject, or is the subject
dropped?"), which is Sudoku with "empty" allowed, and the natural place
to make umbrae explicit.

**Design.** Add an umbra node per verb: an optional subject with
features from the verb's ending (nominative, the verb's number, gender
open), present when no overt subject fills the slot. Re-run D131's
measures at the four levels; count how pro-drop raises ambiguity and
garden paths, and which crossings it newly licenses or blocks. Six
words, if the oracle can follow.

**Register before the run:** the direction of the change in A and B,
per kind of displacement.

### N19. Labels buy distance (D133)

*Added and approved 2026-10-19, from the reading of D130-D132 (KC 57-59):
a labelled umbra can wait, an unlabelled one must close soon. Became
D133 directly: displaced adjectives against genitives in foreign words
(L1, L1u), and within adjectives, unique against shared labels (L3, the
discriminating test). The first plan to have a design review before
approval - now the standard step.*
*D133 done (KC 60): L1, L1u and L3 all FAIL - displaced adjectives and
genitives wait through the same short distance (mostly one foreign
word), and a unique label waits no longer than a shared one. Labels
decide which head, not how far.*

## 2026-10-20: weights - ternary, and dithered

From brainstorm section XVI: is {-1, 0, +1} fundamentally inadequate?
(No proof of a ceiling; the evidence is a cost in memorisation, not in
computation.) And Fran's idea: DITHER the weights - an error-diffused
ternary network, where each synapse carries part of its neighbours'
graded values, as an Atkinson-dithered pixel carries its neighbourhood.

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N20 | …AH87TG | dithered weights: error diffusion into ternary | an exact graded network (no trainer needed for the first slice); possibly a house error-diffusion quantiser | which NEIGHBOURHOOD makes ternary weights faithful; fan-in as viewing distance; Atkinson's partial diffusion as a skeleton / settling knob |

### N20. Dithered weights

**Why.** Error diffusion turns a grey image into black and white whose
local DENSITY carries the grey; a neuron's fan-in sums its inputs as the
eye low-pass filters an image. So a graded layer might be represented by
a ternary one whose weights carry each other's residual error. Prior art
exists (GPTQ: column-wise quantisation with the error pushed onto the
remaining weights by the input covariance; sigma-delta / GPFQ with error
bounds; stochastic rounding as random dither - all recalled). What is
open is the NEIGHBOURHOOD the error should flow along, and whether
partial diffusion (Atkinson's 6/8) trades statistical fidelity for
sparse, skeleton-like structure.

**Design (first slice: exact, no trainer).**
- A graded linear layer with a known exact target: e.g. a readout from
  the toy Latin world's 0/1 features to exact reading probabilities
  (enumerated, D131), or a graded compiled layer; exact rationals.
- Ternarise it six ways: nearest rounding (no dither); stochastic
  rounding (random dither, exact over all draws or many); error
  diffusion along a fixed order (1-D, and 2-D Floyd-Steinberg on the
  weight matrix); Atkinson (6/8 of the error); covariance-guided
  (GPTQ-like, using the exact input distribution).
- Measure exactly over the enumerated inputs: output error, the share of
  zeros (sparsity), and how both scale with FAN-IN (sweep the layer's
  input width).
- Later slices: small ReLU networks on nervus (pieces against regions
  after dithering - does dithering preserve the function's pieces?);
  dither a trained toy Latin model (after N16's trainer); skeleton
  circuits (already ternary) against statistical layers.

**Register before the run** (a design review first): the prediction
that error diffusion beats nearest rounding by a margin that grows with
fan-in; that covariance-guided diffusion beats spatial orders; that
Atkinson's partial diffusion gives more zeros and more error - with the
effect sizes the test can see.

## 2026-10-20: rules into weights - precision, spectrum, hologram

Fran's question (brainstorm XVII): how does a large set of discrete
rules - general rule, exceptions, exceptions to the exceptions - get
"optimised" into a dense classifier, and is a weight like a point of a
hyperdimensional Fourier decomposition, carrying many waves at once?
Three proposals, each exact and needing no trainer. They share one test
world: the toy Latin world (D131) extended with a real Bennett exception
chain. Every one gets a design review before registration.

| | ledger | proposal | depends on | settles or opens |
|---|---|---|---|---|
| N21 | …DXTVZX | priority costs precision: rule lists into threshold units | the shared exception chain; exact rational LP / exhaustive search | whether weight precision tracks nesting depth or tail length; what ternary pays (width or depth) |
| N22 | …XYDKJ1 | the rule spectrum: exact Walsh-Fourier degree profile | N21's classifiers, D131's readers; a house exact Walsh-Hadamard transform | whether degree or support predicts what quantisation kills; a spectral definition of the skeleton |
| N23 | …9ZWZFZ | the slot state as a hologram | D131's world; +-1 vector arithmetic (exact) | capacity of one-vector slot states; whether a hologram can detect an umbra (unfilled role) |

**The shared world: one real exception chain.** Third-declension
endings carry a genuinely NESTED rule list (Bennett, to be checked
against the text and Whitaker before use): genitive plural in -um;
except i-stems in -ium (parisyllabic nouns, nominatives in -ns / -rs,
monosyllables in two consonants, neuters in -e / -al / -ar); except
nouns that look like i-stems but take -um (canis, iuvenis, senex,
pater ...); likewise the ablative singular in -e / -i. Features are 0/1
(class markers, phonological properties, one-hot lemmas for the named
exceptions); the target is a binary decision per noun. A FLAT tail
(many lemma one-hots overriding one rule) is built alongside as the
control: same number of exceptions, nesting depth one.

### N21. Priority costs precision

**Why.** A decision list is one threshold unit with geometrically
shrinking weights (recalled), so the ORDER of rules becomes the SIZE of
weights. If that is how exceptions are stored, a low-precision network
must pay for an exception tail in width or depth - a mechanism for
"ternary is fine for computation, costly for memorisation" (XVI).

**Design.**
- Rule lists of nesting depth k = 1 .. K (the real chain, truncated
  and, past the real depth, extended synthetically) and flat tails of
  the same length.
- Exactly, for each list: the minimal integer weight magnitude (and
  bits) of a single threshold unit realising it (exhaustive or an exact
  rational LP/ILP on fractio for these sizes); against the canonical
  geometric compilation.
- With weights restricted to {-1, 0, +1}: the minimal width of one
  hidden layer, and the minimal depth, that realise it (exhaustive over
  small circuits).
- Later slice: dither the exact graded unit into ternary by N20's
  methods and see which rules of the list flip first.

**Register before the run.** That required precision tracks NESTING
DEPTH, not tail length (flat tails stay cheap; nested chains cost about
a bit or more per level); that ternary pays nesting in width (unary
magnitude) or depth (one layer per level of veto). Effect sizes and the
synthetic extension's construction fixed in the plan.

### N22. The rule spectrum

**Why.** A Boolean classifier has an exact Walsh-Fourier spectrum;
general rules sit at low degree, exceptions are spikes spread over high
degrees, and noise attacks high degrees first (recalled). If so, the
spectrum PREDICTS what quantisation kills, and "the skeleton" gets a
spectral definition: the low-degree part.

**Design.**
- Exact spectra (fractio) of N21's classifiers and of D131's readers'
  decisions over their enumerated feature spaces, under the uniform
  measure AND under the world's own measure (an exact orthogonal basis
  for that measure, by Gram-Schmidt over rationals).
- Report per classifier: the degree profile (weight at degree 1, 2, 3,
  ...) and, separately, each coefficient's SUPPORT (how often it is
  active in the world) - degree and support are different properties
  and the encoding moves an exception between them (one-hot lemma: low
  degree, low support; bit-coded lemma: high degree).
- Perturb: the exact noise operator (coefficient x rho^|S|, exact
  noise stability) and N20's ternarisations of a graded realisation;
  which rules flip, against their degree and their support.
- House library: a fast Walsh-Hadamard transform over exact numbers
  (fractio / magnus) is general; it goes in include/ + lib/ with its
  own suite, not in the demo.

**Register before the run.** Which of degree or support better predicts
the flipped rules (a stated rank correlation and threshold); that the
nested chain of N21 has heavier high-degree mass than the flat tail.

### N23. The slot state as a hologram

**Why.** Hyperdimensional computing and holographic reduced
representations (Kanerva, Plate; recalled) put a whole role-filler
structure into one vector, every component carrying a little of every
binding - Fran's "each weight carries many waves" built on purpose. The
toy world's slot state (XI) and its umbrae (XV, unfilled roles) can be
written this way, exactly.

**Design.**
- +-1 vectors of dimension d; bind role and filler by elementwise
  product (exact for +-1), bundle bindings by sum, read a role by dot
  product with its key.
- Two codebooks: ORTHOGONAL (Hadamard rows: zero crosstalk while the
  bound pairs are distinct rows, then a cliff - note that Sylvester
  rows form a GROUP under elementwise product, so two bindings can land
  on the same row; the plan must handle it) and RANDOM (binomial
  crosstalk, graceful) - the ordered-dither / random-dither contrast of
  XVI, in vector form.
- Over the toy world's sentences, exactly: decoding error per role as
  the number of bindings grows (binomial tails, exact for random codes;
  exact enumeration for Hadamard); and the UMBRA test - reading an
  unfilled role returns pure crosstalk; how well the readout magnitude
  separates filled from unfilled (exact error rates), i.e. can the
  hologram KNOW what is missing?
- Structured fillers (words that share agreement features) give
  correlated crosstalk; measure how that changes capacity.

**Register before the run.** The capacity at which random codes
overtake orthogonal ones (exact, no claim needed beyond the
computation); the prediction that umbra detection degrades with the
number of filled roles at a rate fixed in the plan; that agreement-
sharing fillers cost capacity (a stated effect size).
