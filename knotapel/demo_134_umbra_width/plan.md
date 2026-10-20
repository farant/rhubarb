# Demo 134: umbra width of the toy world (plan)

Status: approved by Fran (2026-10-20), v2.1 after a DESIGN REVIEW and
its re-check (consiliarius-d134, before any data; summarised in "Design
review" below). "Open for Fran" decided as proposed: (1) SIX words for
the registered verdicts, five descriptive (T0 checks feasibility); (2)
the umbra definitions as written, with the nearness rule; (3) folder
`knotapel/demo_134_umbra_width/`. Proposal N17
(`knotapel/demo-proposals.md`, ledger …NBF5ZX); context brainstorm sections XI (the slot library), XV (umbrae), XVII
(rules into weights); D131 (KC 58: the world), D133 (KC 60: labels do
not buy distance in real Latin).

## Question

Fran's umbrae (brainstorm XV): reading left to right, a reader carries
what the structure has PROMISED and not yet delivered - a word waiting
for its head, a verb waiting for its object, a clause waiting for its
verb. In knot terms they are open strand ends. Projective reading keeps
them in stack order; a crossing closes one out of order.

D131 found that endings pay for adjectival hyperbaton and cannot pay for
a displaced genitive (KC 58.4), but it measured the outcome (garden
paths of a nesting reader), not the mechanism. Here, exactly, on the
same world:

1. **How much memory?** How many umbrae are open at once (WIDTH), and
   how deep in the stack is an umbra when it closes (DEPTH)?
   (Descriptive: a first measurement.)
2. **What makes a displaced word misread?** Two candidate mechanisms:
   - COMPETITION (premature or local attachment): the displaced word's
     endings also fit a NEARER word than its true head, so a reader
     that attaches locally goes wrong (D131's *bona puella dona amat*:
     *bona* fits *puella* first);
   - RETRIEVAL (interference in memory): when the two ends of the arc
     meet, the arriving word's endings do not single out its partner
     among the words it could attach to (cue-based retrieval, Lewis
     and Vasishth, recalled; the attention-head picture of brainstorm
     XV).
   The registered bet: garden paths follow COMPETITION, and RETRIEVAL
   adds nothing once competition is fixed.

## The world

D131's world: lexicon (*puella, dominus, donum, rex*; *bonus, fortis*;
*amat / amant, dat / dant*), Bennett's endings checked against
Whitaker, the slot frames, every analysis in every order, the four
morphology levels L0 Latin, L1 ideal, L2 case-stripped, L3 bare, and
the readers F, P, M, R_P. A PAIR is a string with one of its analyses.
D131's world code is COPIED into this demo (the toy grammar is not a
general algorithm, so not a house library); T0 checks that the copy
reproduces D131's pinned five-word totals.

**Length (Open for Fran 1; the review's recommendation): the registered
verdicts at SIX words, a world nobody has seen** - D131's own findings
say testing its kind split "needs a new world (e.g. six words) where it
was not seen first", and at five words no two nouns of the same case,
gender and number can stand beside an adjective (KC 58.4.3.1), so
retrieval can hardly fail there. Five words: every measure, exhaustive
and oracle-pinned, descriptive. Six words is about 809.5M pairs (D131
task 0), roughly 120 times five; T0 measures whether it is feasible in
C (time, memory) before anything is computed.

## Definitions

Words are read left to right; boundary t lies after word t. Everything
is computed on a pair's TRUE analysis: the memory and the competitors
the right reading faces (dependency-locality "storage cost", Gibson,
recalled), not what a reader knows.

- **Arcs.** Each non-root word d has one arc to its head h, relation r
  in {nsubj, obj, iobj, obl, amod, nmod}. REQUIRED: nsubj, obj, and iobj
  under *dat / dant*; the others OPTIONAL.
- **Umbrae** (an arc whose earlier endpoint PROMISED it):
  - DANGLING: dependent first (d < h); opened at d, closed at h;
  - SLOT: head first (h < d) and r required; opened at h, closed at d;
  - ROOT: the clause promises its verb; opened at boundary 0, closed at
    the verb (projectivity here, as in D131, includes the root arc;
    without this umbra a root-arc crossing would be invisible).
  - A required dependent BEFORE its verb never becomes a slot umbra:
    its own dangling umbra carries the promise (a convention, re-check
    N2).
  - An optional dependent after its head (adjective after its noun,
    genitive after its noun, ablative after the verb) promised nothing:
    a BACKWARD ATTACHMENT, not an umbra (counted).
- **Open umbrae at boundary t:** opened at or before t, closed after t.
  **WIDTH** of a pair: the maximum over its boundaries. Width depends on
  tree, order and relations only: the same at every level (a check).
- **DEPTH** of an umbra opened at a, closed at word j: the umbrae opened
  at a word strictly between a and j and still open after j - exactly
  the umbra arcs that cross it. A projective pair (root arc included)
  has depth 0 everywhere (theorem, check).
- **Compatibility** comp(r, x, y) at a level: some reading of form x and
  some reading of form y satisfy relation r with x dependent, y head:
  amod - x an adjective, y a noun, same case, gender, number; nmod - x a
  NOUN with a genitive reading, y a noun; nsubj - x a NOUN, nominative,
  y a verb of x's number; obj - x a NOUN, accusative, y a verb; iobj -
  x a NOUN, dative, y *dat / dant*; obl - x a NOUN, ablative, y a verb.
  comp(*, x, y) = comp(r, x, y) for SOME r. Local, per pair of words:
  it ignores "at most one genitive per noun" and every other global
  constraint (stated: a competitor may not be a legal alternative).
- **DISPLACED arcs:** D131's - the arc of a dependent that is
  non-projective by the per-arc test (`dependentia_arcus`). In this
  world they are amod and nmod arcs only (KC 58.2). The labels below
  are read on the displaced arcs, NOT on deep umbrae (review H2a: depth
  is charged to the earlier-opened of two crossing arcs, often the
  root, not to the displaced word).
- **COMPETITION label** of a displaced arc (d, h): COMPETED iff some
  word w other than h and outside d's own subtree (a descendant would
  close a cycle; re-check L1), on either side of d, with comp(*, d, w),
  is at least as near to d as h (|w - d| <= |h - d|; ties on opposite
  sides count); otherwise CLEAR. One
  rule for both directions: it covers the dependent-first case (a
  compatible word inside the span, an eager reader's premature closure)
  and the head-first case (*dominum amat regis puella*: *puella* is
  nearer than *dominum*).
- **RETRIEVAL label** of a displaced arc, read when its later endpoint
  arrives:
  - dependent first (h arrives; the umbra d is open): candidates C =
    the dangling umbrae open at boundary h - 1 that could depend on h
    (comp(*, d', h)); T = the dangling umbrae h truly closes.
    RETRIEVED iff C = T (two adjectives both on *puella* are not
    interference, review M3); otherwise INTERFERED.
  - head first (d arrives): C = the earlier words y < d with
    comp(*, d, y). RETRIEVED iff C = {h}.
  - Relation-specific variants (C limited to the arc's own relation)
    reported, not decided.
- **Kinds and directions** (strata): a crossed pair displaces
  adjectives only, genitives only, or both (D131); its displaced arcs
  are all dependent-first, all head-first, or mixed. Nine strata.
- A pair is COMPETED if some displaced arc is competed (else CLEAR);
  INTERFERED if some displaced arc is interfered (else RETRIEVED).

## Hand sentences (worked by hand; L0 unless stated; all re-derived by the code in T1)

Readings: *puella* nom/abl sg f; *dominum* acc sg m; *dona* nom/acc pl
n; *bona* nom sg f, abl sg f, nom/acc pl n; *bonum* acc sg m, nom/acc
sg n; *regis* gen sg m.

1. *puella dominum amat* (projective). Width 3 (root, puella, dominum
   after word 2); every depth 0. No displaced arc.
2. *bonum puella dominum amat* (crossed; R_P reads it). Displaced
   *bonum* -> *dominum*, dependent first, depth 1 (*puella*'s umbra
   crosses it). Competition: *puella* is nearer but not compatible
   (gender): CLEAR. Retrieval at *dominum*: C = {bonum} (*puella*
   cannot depend on *dominum*: no genitive reading) = T: RETRIEVED.
3. *bona puella dona amat* (crossed; D131's garden path). Displaced
   *bona* -> *dona*: COMPETED (*puella*, nom sg f, distance 1 < 2);
   retrieval at *dona*: C = {bona} = T: RETRIEVED. Misread by R_P.
4. *regis puella amat dominum* (crossed via the root arc). Width 3. The
   root umbra closes at *amat* with *regis* open: depth 1; *regis* ->
   *dominum* (dependent first) depth 0. COMPETED (*puella*, any noun
   takes a genitive, distance 1 < 3). Retrieval at *dominum*: open
   dangling {regis} (puella closed at *amat*): RETRIEVED. Misread.
5. *bona bonum amat puella dominum* (the golden line). The root closes
   at *amat* with depth 2; width 4 after *amat* (bona, bonum, nsubj and
   obj slots). Displaced *bona* -> *puella* (depth 2) and *bonum* ->
   *dominum* (depth 0; both arcs are non-projective by the per-arc
   test). Competition: *bona* - nearer *bonum* is an adjective, *amat*
   a verb, neither compatible (comp needs a NOUN for nsubj): CLEAR;
   *bonum* - *puella* nearer, gender: CLEAR. Retrieval at *puella*:
   C = {bona} (bonum: gender) = T; at *dominum*: C = {bonum} = T:
   RETRIEVED. P empty: R_P reads it. (Under the v1 literal comp,
   *bona* would satisfy nsubj with *amat*: a plant.)
6. *bona amat puella dona* (review): *bona* -> *dona* COMPETED
   (*puella* nearer), but every alternative crosses the root arc too,
   so P is empty and R_P reads it: competed, not misread.
7. *puella regis amat dominum* (review): *regis* -> *dominum*; v1's
   inside-the-span closure called it labelled, yet R_P misreads it
   (*regis* -> *puella* is projective). Under the nearness rule:
   COMPETED (*puella* at distance 1 < 2). Retrieval at *dominum*: open
   dangling {regis}: RETRIEVED.
8. *dominum amat regis puella* (review; head first): *regis* ->
   *dominum*: every umbra depth 0 (backward attachments are not
   umbrae), misread. COMPETED (*puella* at 1 < 2); retrieval: earlier
   nouns {dominum} = {h}: RETRIEVED.

Six-word sentences where retrieval can fail (two compatible open
umbrae or two compatible earlier heads, with the competition label
clear) are constructed in T1 and worked by hand before T2.

## Measures

At each length (five: all; six: all, if T0 allows), per level:
- width and depth distributions, by projective / crossed, kind and
  direction; umbrae by type; backward attachments;
- for crossed pairs: B (D131's garden-path share of R_P) in the
  competition x retrieval 2 x 2, per stratum, with group sizes;
- I(L) = the share of displaced arcs that are RETRIEVED; the share
  CLEAR, likewise.

## Registered before running

Decided at SIX words (if Fran keeps five, see the end), on exact
integer totals; gaps in percentage points, compared exactly as
rationals.

- **Population:** L0 crossed pairs with |F(s)| >= 2 (a pair with one
  analysis cannot be misread; review control a).
- **Testable group:** >= 1,000 pairs and >= 2% of the set it is
  compared within - for U2a the kind x direction stratum, for U2b the
  competition cell (COMPETED or CLEAR) of that stratum (re-check L3).
  The same floors for both (N5).
- **U2a (competition makes the garden paths; EXPECTED, a positive
  control):** in every stratum where both COMPETED and CLEAR are
  testable, B(competed) - B(clear) >= 10 points; at least three such
  strata, otherwise UNDECIDED. The review expects it to pass, because a
  competitor is near to a projective alternative, which is what R_P
  falls for; it is registered so that a failure would be seen, and the
  findings may not read its pass as a mechanism. It is partly IMPLIED
  by the definitions (re-check M1): if some analysis in P(s) attaches
  a displaced dependent to a word at least as near as its true head,
  that word satisfies comp(*), so every such misread is COMPETED; B(clear)
  comes only from other routes (a farther reattachment, a role change,
  another arc). "Covered => competed" is a check, and B(clear) is
  reported by misread route.
- **U2b (retrieval adds nothing; the discriminating test):** decided on
  the DEPENDENT-FIRST strata only (re-check M2: in head-first strata an
  earlier compatible word between h and d is both a retrieval candidate
  and a competitor, so the two labels are coupled; head-first U2b is
  descriptive). Within COMPETED pairs and, separately, within CLEAR
  pairs, in every such stratum where both INTERFERED and RETRIEVED are
  testable: |B(interfered) - B(retrieved)| <= 5 points. A cell COUNTS
  only if B(retrieved) lies in [10%, 90%] (re-check H1: near 0% or
  100% the 5-point bound holds whatever retrieval does); cells outside
  are reported but neither pass nor fail U2b. At least two counting
  cells, otherwise UNDECIDED - and "the world gives retrieval no room
  to fail" is then a finding.
- **U1 (Latin's endings keep most of the ideal's retrieval):** over the
  displaced ADJECTIVE arcs of crossed pairs (re-check M3: genitive arcs
  dominate the crossed pairs and are nearly level-insensitive, which
  would make both differences small - D131's Q1 near-tie risk):
  I(L0) - I(L2) >= 2 x (I(L1) - I(L0)) AND I(L0) - I(L2) >= 5 points.
  Genitive arcs and all arcs: descriptive. (I(L1) >= I(L0) is a theorem - each L1 form's readings are a
  subset of the L0 form's, and candidate sets only shrink - and is a
  check, not a prediction.)

If Fran keeps FIVE words: U2a is registered as above; U2b and U1 become
descriptive (the review: retrieval can hardly fail at five words).

**Descriptive, required:** the full 2 x 2 with B and sizes per stratum
and level; a MORPHOLOGY-BLIND competition label (competed iff a word of
the right part of speech - a noun - is as near as h) beside the real
one, so that what the endings add is visible; MECHANISM COVERAGE - the
share of misread pairs where some analysis in P(s) attaches a displaced
dependent to a word at least as near as its true head; the
relation-specific variants; the L1, L2, L3 versions; five-word values
beside six.

## Already seen (disclosed)

D131's every published number at five words: B by level, by kind, by
syncretic split, A, open cells, mean |F|, R_M. D131's adjective split
(syncretic displaced word 7.28% against unambiguous 1.13%) is close to
a COMPETITION split for adjectives (a syncretic adjective is likelier
to fit a nearer noun): U2a's adjective strata are partly foreseen at
five words; at six they are not. Nothing in this plan has been computed
over the population; the review computed only hand sentences.

## Checks (characterizations)

- D131 reproduction (copy of the world): pairs by length, B by level
  and kind, mean |F| at five words.
- Width and depth the same at every level, pair by pair (a CODE check:
  true by construction, re-check N1).
- Projective pair => depth 0 everywhere; depth >= 1 <=> crossed by a
  later-opened umbra arc (two routes).
- Umbrae (root included) + backward attachments = arcs + 1.
- Per arc: RETRIEVED at L0 => RETRIEVED at L1; CLEAR at L0 => CLEAR at
  L1 (reading subsets).
- Covered => competed: every misread pair whose P(s) holds an analysis
  attaching a displaced dependent at least as near as its true head is
  COMPETED (re-check M1).
- No competitor lies in d's own subtree.
- Displaced arcs are amod / nmod only; the per-arc displaced set equals
  D131's.
- Hand sentences 1-8 at L0 (1 also at L3), and the six-word ones.
- Totals pinned from an independent Python oracle: its own umbra and
  label computation (from arc pairs and distances, not a left-to-right
  sweep), world from D131's `oracle_partb.py`; exhaustive at five
  words; at six on a DETERMINISTIC subset of bags (fixed in T0, e.g.
  canonical index = 0 mod k), stated as partial.
- Rule functions (U2a, U2b, U1, testability, UNDECIDED) on constructed
  totals at each threshold; verdict wiring from the oracle's numbers.
- Plants via `silva.planta`, including the v1 literal comp (sentence 5).

## Size and time

Five words: D131's 21 s plus the measures. Six: about 120 times as many
pairs (an estimate of 40 minutes in C by the review, unmeasured): T0
measures it; a long run goes detached (python double fork + `setsid` +
`caffeinate`, watched by Monitor).

## Tasks (one per turn, Fran's go-ahead each)

0. **T0:** copy D131's world; reproduce D131's five-word totals; measure
   six-word feasibility (time and memory, generator vs analyser) and fix
   the oracle's six-word subset - counts only, no umbra, label or B
   numbers. *Done 2026-10-20 (Task 0 findings below).*
1. **T1:** umbrae, width, depth, competition and retrieval labels;
   Part A checks; six-word hand sentences; plants. *Done 2026-10-20 (Task 1 notes below).*
2. **T2:** measures, U2a / U2b / U1 decided, `oracle_d134.py`, rule
   checks; plants. *Done 2026-10-20 (Task 2 notes below).*
3. **T3:** findings.md, claims.md (KC 61), ledger (N17), archive.sh.
4. **T4:** independent review with its own oracle, then fixes.

## Design review (consiliarius-d134, 2026-10-20, before any data)

H 3, M 7, L 3, all acted on (the report's last notes were lost in
transit):
- **H1** U2 (v1) was near-certain: "closure-unlabelled" is close to
  R_P's own definition plus string ambiguity. Now U2a (registered as an
  expected positive control, its pass not to be read as mechanism),
  U2b the discriminating test, |F| >= 2, the morphology-blind label and
  mechanism coverage as controls.
- **H2a** labels were read on DEEP umbrae, which are often the root, not
  the displaced word: labels now on the displaced arcs.
- **H2b** backward attachments were invisible (*dominum amat regis
  puella*): head-first retrieval added; the competition label covers
  both directions by nearness. (The nearness rule is this plan's
  answer, not the review's: it also catches the review's sentence 7,
  which v1's inside-the-span closure missed.)
- **M1** comp lacked "x a noun" (sentence 5 flipped): fixed, planted.
- **M2** asymmetric conventions: one convention per label (comp(*)),
  relation-specific variants reported.
- **M3** two umbrae closed by one word are not interference: C = T.
- **M4** root umbra excluded from labels (it is never displaced).
- **M5** U1 was half theorem: the theorem clause is a check; one clause
  registered, at six words.
- **M6** gap rules without margins, empty groups, Simpson within kinds:
  margins (10 and 5 points), testability (1,000 pairs and 2%), strata
  kind x direction, UNDECIDED rules.
- **M7** D131's adjective split disclosed as a near-proxy.
- **L1** a reader-view retrieval (all earlier words): the head-first
  label is already that; the dependent-first one stays on open umbrae
  (C = T), stated. **L2** comp's locality stated.
- **Length:** six words for the verdicts, five descriptive - adopted as
  the recommendation, Fran to decide.

**Re-check of v2** (same reviewer, 2026-10-20): hand sentences 1-8
correct under v2 (sentence 5's two arcs both per-arc non-projective).
H1 U2b could pass vacuously at B near 0% or 100%: only cells with
B(retrieved) in [10%, 90%] count. M1 U2a partly implied by the
definitions: stated, checked, B(clear) by route. M2 head-first
retrieval coupled to competition: U2b decided on dependent-first
strata. M3 U1 dominated by level-insensitive genitive arcs: decided on
adjective arcs, with a 5-point floor. L1 a competitor in d's own
subtree: excluded. L2 KC 58.3 -> 58.2. L3 "2% of its stratum" made
explicit. N1 width-across-levels labelled a code check; N2 the
pre-verbal-argument convention written down; N3 the two per-arc
theorems confirmed sound; N4 root labels moot (never displaced); N5
one floor for both tests.

## Open for Fran

1. Six words for the registered verdicts (recommended; T0 checks it is
   feasible) or five (then only U2a registered)?
2. The umbra definitions: slot umbrae for required relations only, a
   root umbra, optional right dependents as backward attachments; the
   nearness rule for competition.
3. Folder `knotapel/demo_134_umbra_width/`.

## Task 0 findings (2026-10-20)

- **The copy reproduces D131 exactly.** `main.c` is D131's world (the
  lexicon, Bennett against Whitaker, the four levels, generator against
  analyser, tree table, word order, D131's nine hand sentences, Part B)
  with every D131 pin carried over and green at five words: pairs by
  length (384 / 55,296 / 6,696,960), A, A (reading), B, the
  syncretic / unambiguous split and B (R_M) at every level, B by kind
  (adjectives only / genitives only / both, each by form), by mildness,
  by crossing count, the open cells behind A. New pin: **mean |F|
  exactly**, as the sums of |F(s)| over projective and over crossed
  pairs, from `oracle_partb.py` with one line added (each pair adds
  `len(an)`): L0 3.79, L1 3.08, L2 67.22, L3 70.28 (as D131 printed).
  Quick mode (`D134_CELER=1`, four words) pinned the same way.
- **Changes to D131's code, each checked:** MAXW 6 (an analysis key is
  3 + 10 x 6 = 63 bits, a bag code 45); permutations to 720; the tree
  table to 7^6 codes, Cayley to n = 6 (8,477 trees in all) and the
  projective / mild counts 728 / 5,652 at n = 6 pinned from D131's
  `oracle_trees.py` run to n = 6 (0.2 s); D131's Q1 / Q2 rules and
  verdicts dropped (D134 registers its own in T2); the size table
  factored into `sizes()`. **Distinct orders:** D131 found them with a
  list of the strings seen, n!^2 per bag - fine at five words, about
  500,000 comparisons per bag at six. `first_order` keeps an order iff
  equal tokens keep their canonical order (the lexicographically first
  order of each string), and is checked against D131's list on every
  order of every bag at every level at five words.
- **Six words is feasible, exhaustively, in C.** The whole run (five
  words with every pin, then six words counts only) is **83 s, peak
  268 MB** (`/usr/bin/time -l`; 8 cores, 16 GB, one used).
  Generator: 1,317,056 abstract analyses in 5.2 s, 9.9M placements
  (192 MB) before sorting. Analyser: L0 1.9 s (10.4M bags tried), **L1
  18.3 s (386M bags tried: every multiset of 100 forms)**, L2 2.4 s, L3
  1.7 s. Generated == analysed at every level; the analyser never
  yields an analysis twice.

  | level | bags | strings | pairs 6 | max \|F\| |
  |---|---|---|---|---|
  | L0 | 261,113 | 148,605,241 | 809,533,440 | 144 |
  | L1 | 316,608 | 195,484,032 | 809,533,440 | 24 |
  | L2 | 18,720 | 6,378,488 | 809,533,440 | 1,460 |
  | L3 | 9,404 | 2,329,376 | 809,533,440 | 1,460 |

  (bags and strings up to six words). Pins: six-word pairs 809,533,440
  and the largest L0 |F| 144 (D131 task 0's counts); the population is
  the same at every level, length by length. The largest |F| (1,460)
  is far inside the buffer (4,096).
- **Six-word pairs: 149,385,216 projective, 660,148,224 crossed
  (81.5%; 59.8% at five words)**, the same at every level. The per-pair
  pass (every distinct order, analyses carried to it, the tree table)
  takes 6.5-10.2 s per level over all 809.5M pairs; the same pass at
  five words gives D131 task 0's 2,694,144 / 4,002,816 (a pin). So the
  T1 / T2 measures run on **all** six-word pairs; their cost is this
  pass times the work per pair (readers, umbrae, labels), estimated at
  minutes, measured in T1.
- **The oracle's six-word subset (fixed).** Key: the six-word bags at
  **L1**, where every reading is its own form (checked: form index ==
  reading index), so an L1 bag is a multiset of readings and selects
  the same pairs at every level. Order: `bag_code` ascending (n in bits
  0-2, the sorted reading indices r_i at bits 3 + 7i; reading index =
  noun (lemma x 2 + number) x 5 + case, then adjective ((lemma x 3 +
  gender) x 2 + number) x 5 + case + 40, then the verbs amat, amant,
  dat, dant at 100-103 - the order of `oracle_partb.py`'s READINGS
  list). **The bag of rank r is in the subset iff r mod 128 == 0:
  2,250 of 288,000 bags, 6,670,440 pairs (0.82% of the six-word
  pairs)** - about the size of the five-word population (6,752,640),
  which D131's oracle covers in 138 s and 50 MB. The oracle in T2 must
  reproduce the 2,250 bags and 6,670,440 pairs before any measure (a
  pin). Every registered six-word number is computed by C on all pairs;
  the oracle pins the subset's totals, stated as partial.

## Task 1 notes (2026-10-20)

- **Implemented as defined above**, in `main.c` after Part B:
  `build_umbrae` (dangling, slot, root; backward attachments counted;
  width over the boundaries; depth by a left-to-right sweep),
  `build_comp` (comp(r, x, y) per level from the readings, x a NOUN for
  every relation but amod), `label_arcs` (COMPETED / CLEAR, RETRIEVED /
  INTERFERED, plus the relation-specific and morphology-blind variants
  for T2). One pass meets every pair once, by the L1 bags (multisets of
  readings), so a pair's labels at L0, L1, L2 and L3 sit side by side
  and the per-arc theorem L0 => L1 is checked arc by arc. The pass
  prints checks and counts only: no label share, width or depth
  distribution, and no B (T2).
- **Checks, all green at four, five and six words** (six: 816,286,080
  pairs up to six words, 664,166,400 crossed, 1,016,804,352 displaced
  arcs, 403-464 s for the pass at four levels; whole run 8-10
  minutes, under 290 MB; final run 90 checks, 0 failures):
  umbrae + backward attachments == arcs + 1 and one root umbra closed at
  the verb; depth by the sweep == depth by pairwise crossing of
  later-opened umbra arcs; projective => depth 0; displaced arcs amod /
  nmod only, and the tree table's per-arc set == a yield route's;
  competition with the subtree excluded by walking up == by yields; the
  true partner is always a candidate (T in C, h in C); per arc,
  RETRIEVED at L0 => at L1 and CLEAR at L0 => at L1 (variants too).
  New, a theorem: **exactly half the displaced arcs are dependent
  first** - reversing a string keeps the pair (word order is free), its
  displaced arcs (checked on the tree table) and their kind, and swaps
  the directions. Deferred to T2, where P(s) and the per-level passes
  live: "covered => competed", and width / depth the same at every level
  (here true by construction: `build_umbrae` takes no level).
- **Hand sentences:** 1-8 exactly as worked in this plan, first run.
  Six words, retrieval failing with competition clear: **9** *bonum
  puellae dominum amant rege forti* (dependent first: when *dominum*
  arrives, the open subject *puellae*, nom. pl., has a genitive reading
  that could depend on it; *puellae* is feminine, so no nearer word can
  take *bonum*; one analysis, read) and **10** *rege puella dominum amat
  regis fortis* (head first: *rege*, *puella*, *dominum* all could take
  *regis*; only *dominum* is as near; every analysis crosses the root
  arc, read). **11** *puella dominum amat regis puellarum* (five words)
  pins re-check L1: *puellarum* hangs on *regis* and is nearer than
  *dominum*, but would close a cycle, so CLEAR.
- **Noticed while working 9 (existence, not a count):** retrieval can
  fail by a SUBJECT. The first declension's *-ae* is nom. pl. and gen.
  sg., so an open plural subject is a genitive candidate for every noun
  that arrives before the verb (already at four words: *bonum puellae
  dominum amant*). The review's "retrieval can hardly fail at five
  words" was about two nouns of one case, gender and number beside an
  adjective; this is a second route, through case syncretism of the
  candidate rather than of the displaced word.
- **Open for Fran (a reading of the registered definition, no data
  seen):** re-check L1 took d's own subtree out of COMPETITION (a
  descendant would close a cycle); the RETRIEVAL candidates, as
  registered, keep it - C is a match on endings, made when the later
  word arrives, before its own attachment is known (cue-based retrieval
  matches features, not trees). So a word in the subtree can be a
  retrieval candidate (a genitive of a head-first genitive, placed
  before it). Implemented as registered; proposed for T2: report how many
  INTERFERED labels rest only on a subtree candidate, descriptive,
  without changing U2b.
- **Plants (silva.planta, all red):** v1's literal comp (sentence 5
  turns COMPETED), the sweep counting same-word openings, the root
  umbra never open, optional head-first arcs made slots, T not set
  aside, head-first h counted against itself, the yield route's wrong
  yield, width counting the closing boundary, the subtree exclusion
  dropped (green at four words until sentence 11 was added - a plant
  found the gap).

## Task 2 notes (2026-10-20)

- **Readings fixed before any T2 number was printed:** "six words" =
  the pairs of exactly six words (the world nobody has seen; the
  oracle's subset is six-word bags too); "five words" = every pair up
  to five words (D131's world; descriptive). U1 is read over the
  displaced adjective arcs of ALL crossed six-word pairs (the plan says
  "of crossed pairs"; I(L) involves no misreading); U2a and U2b over the
  registered population (L0 crossed pairs with |F| >= 2). Retrieval as
  registered (the subtree kept in C; Fran left it so), with the
  cycle-free variant counted beside it.
- **Implemented:** one pass per level over that level's own bags, so
  F(s) and P(s) are the string's own (`measure_umbrae`); per pair the
  labels at that level, the stratum (kind x direction), B (R_P misreads
  it), the misread ROUTE, the relation-specific and morphology-blind
  labels; the rules (`u2a_rule`, `u2b_rule`, `u1_rule`, `testable`)
  exact, gaps as rationals (house `fractio`), on constructed totals at
  every threshold, and `rule_inputs` checked on constructed totals (a
  wrong stratum would otherwise pass: reversal makes the dependent- and
  head-first strata the same size, and the wiring check reads both
  sides through the same function). Five-word mode `D134_QUINQUE=1`
  (28 s) for plants that four words cannot see.
- **`oracle_d134.py`** (independent: D131's oracle for the world -
  tree-first analyses, projectivity by yields, displaced arcs by
  domination; its own relations, comp, umbrae by counting spans and
  pairwise crossings, labels, routes, strata, subset key). It prints
  its totals as C arrays in `flatten`'s order, pasted as the pins:
  **1,125 totals up to five words (exhaustive, 382 s), 1,125 up to four
  (quick mode), 837 on the six-word subset (2,250 L1 bags, 6,670,440
  pairs; B at L0 and L1 only - PARTIAL; 397 s, 470 MB). Every number
  agreed on the first comparison**, before and after the route split
  below. The rules applied to the oracle's totals give the demo's
  answers (wiring), at five words and on the subset.
- **Found: re-check M1's "covered => competed" is false as stated.**
  Both C and the oracle count misread pairs whose P(s) puts a displaced
  word on a word as near as its head and are CLEAR: 1,536 up to five
  words. Every one is a REVERSED dependency (*domini dominorum amant
  domini dominos*: the truth hangs *domini* on *dominorum*, the
  alternative *dominorum* on *domini*): the nearer word lies in d's own
  subtree, which re-check L1 took out of competition. The two review
  fixes meet. Restated and checked (0 exceptions at every level, five
  and six words): covered from OUTSIDE d's subtree => COMPETED. The
  route is split: as near outside the subtree / as near inside it
  (reversed) / farther / displaced words kept. Up to five words, L0:
  of 2,420,352 misread pairs, 2,416,088 as near outside (0 CLEAR),
  3,072 reversed (1,536 CLEAR), 728 farther (104 CLEAR), 464 kept (458
  CLEAR); coverage 99.95%. Six-word pairs, L0: of 386,865,852
  misread pairs, 384,998,214 as near outside (0 CLEAR), 1,127,084
  reversed (560,472 CLEAR), 289,938 farther (87,944 CLEAR), 450,616
  kept (423,370 CLEAR); coverage 99.81%. So B(clear) comes mostly from
  reversed dependencies and from role changes that keep every displaced
  word on its head.
- **Registered verdicts, six-word pairs, L0, |F| >= 2:**
  - **U2a HOLDS** (8 testable strata): B(competed) - B(clear) from
    +16.04 (both, mixed) to +77.26 points (genitives only). Expected, a
    positive control, partly implied by the definitions - not read as
    a mechanism. Disclosed: competition and B are reversal-invariant
    (nearness is symmetric, P(s) reverses with s), so each
    dependent-first stratum and its head-first mirror carry the SAME
    numbers; the 8 testable strata are 5 independent groups, still
    more than the 3 required.
  - **U2b FAILS** (2 counting cells). Adjectives, COMPETED:
    interfered 49.75% of 609,988 against retrieved 45.89% of 6,436,919,
    +3.86 points (within 5). Genitives, COMPETED: interfered 73.83% of
    27,986,240 against retrieved 84.13% of 136,687,296, **-10.30
    points: interfered genitive arcs are misread LESS** - retrieval
    adds something, against the direction interference predicts. The
    other four cells: B(retrieved) outside [10%, 90%] (three) or a group
    under 2% (genitives CLEAR: 7,424 interfered).
  - **U1 FAILS** (193,351,680 displaced adjective arcs): I(L1) 94.86%,
    I(L0) 83.45%, I(L2) 62.72%; I(L0) - I(L2) = 20.73 points, under
    2 (I(L1) - I(L0)) = 22.82; the 5-point floor holds. Latin keeps
    64.5% of the ideal's retrieval advantage over case-stripped, not
    the two thirds registered.
  - Up to five words (descriptive): U2a HOLDS (7 strata), U2b FAILS
    (3 counting cells), U1 HOLDS.
- **Seen in the tables (for T3, not decided here):** head-first
  genitive arcs are almost never RETRIEVED (3.21% at L0: an earlier
  noun nearly always also fits), dependent-first ones mostly are
  (88.37%); the cycle-only INTERFERED arcs are few (9,875,520 of
  the 1,012,027,392 displaced arcs of six-word pairs at L0) but not zero, so
  the registered reading (subtree kept) and the cycle-free one differ
  slightly; L1 is not perfect retrieval for adjectives (91.64%
  dependent first): an open genitive noun can still take the arriving
  noun.
- **Checks green** at four, five and six words: the 2 x 2 cells hold
  every crossed pair, their misreads are D131's B by length (to five);
  every misread pair has |F| >= 2; arc labels by the per-level pass ==
  task 1's pass, level by level and length by length; width, depth,
  umbra types and backward attachments the same at every level; the
  restated covered => competed; hand sentence 12 (*fortis puellae regis
  amant dominum*: INTERFERED only through a cycle) pins the cycle-free
  variant.
- **Plants (silva.planta, 14, all red):** U2a at 9 points; testable at
  1%; U2b's 90% bound open; U2b's gap signed; U1's floor strict; U2b
  read from the head-first strata; U1 from genitive arcs; routes
  strictly nearer only; the population |F| >= 3; misread judged by M;
  routes read from M; blind competition counting any nominal; the cycle
  never set aside; directions swapped in the strata. Three needed five
  words (four words has no tie, no M-only route, no nearer adjective).
- **Size and time:** the final full run (every part, five and six
  words) is 1,281 s, peak 245 MB: **118 checks, 0 failures**; the
  six-word measure pass, four levels, about 12 minutes.
