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
   numbers.
1. **T1:** umbrae, width, depth, competition and retrieval labels;
   Part A checks; six-word hand sentences; plants.
2. **T2:** measures, U2a / U2b / U1 decided, `oracle_d134.py`, rule
   checks; plants.
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
