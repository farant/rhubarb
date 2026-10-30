# Demo 134: Umbra Width of the Toy World — Findings

Proposal N17 (ledger …NBF5ZX). Plan: `plan.md` (approved 2026-10-20
after a DESIGN REVIEW and its re-check, before any data). Context: D131
(KC 58, the world), D133 (KC 60, labels do not buy distance),
`knotapel/brainstorm-skeleton-settling.md` sections XI, XV, XVII.

Reading left to right, a reader carries what the structure has promised
and not yet delivered: a word waiting for its head, a verb waiting for
its object, a clause waiting for its verb. Fran's name for these open
strand ends is UMBRAE (brainstorm XV). D131 measured the outcome on its
toy Latin world (garden paths of a nesting-first reader); this demo
measures the mechanism on the same world, extended to six words: **how
many umbrae are open at once, how deep they sit, and what makes a
displaced word misread - a nearer word its endings also fit
(COMPETITION), or an arriving word that cannot single out its partner
among the open ones (RETRIEVAL)?**

**Competition, yes; "retrieval adds nothing", no; Latin's endings keep a
little under two thirds of the ideal's retrieval.** Registered at six
words: U2a HOLDS (garden paths follow competition; a positive control,
partly implied by the definitions); U2b FAILS on the genitive cell,
where interfered genitives are misread LESS than retrieved ones (73.83%
against 84.13%) - but the T4 review showed that is a configuration
effect, not retrieval: set aside the cross-serial genitives stranded
across the verb, which no reader can misread, and interference RAISES
misreading (99.04%; Result 5); U1 FAILS narrowly (Latin keeps 64.5% of
the ideal's retrieval advantage over case-stripped endings, against two
thirds registered). On the way
the demo found that one of the plan's "theorems" (covered => competed)
is false once d's own subtree is excluded from competition: the
exceptions are reversed dependencies (Result 6).

Everything is counted exactly: every pair (a string with one of its
analyses) up to six words, 816,286,080 pairs, at four morphology levels.
The tree measures are `dependentia`; the registered rules compare exact
rationals (`fractio`). The totals are pinned from an independent Python
oracle, `oracle_d134.py`: exhaustive up to five words, a fixed 1/128
subset at six (partial). The frozen copy is `demo-snapshot.c`; its
banner gives the clang line and commit.

## The design (after the design review)

- **The world:** D131's, copied (the toy grammar is not a general
  algorithm): four nouns, two adjectives, *amat / amant, dat / dant*,
  Bennett's endings checked against Whitaker; levels L0 Latin, L1 ideal
  (every reading its own form), L2 case-stripped, L3 bare. Task 0
  reproduced every D131 pin and measured six words feasible in C (83 s,
  268 MB; 809,533,440 six-word pairs).
- **Umbrae** (an arc whose earlier endpoint promised it): DANGLING
  (dependent first), SLOT (head first, a required relation: nsubj, obj,
  iobj), ROOT (opened at boundary 0, closed at the verb). An optional
  dependent after its head is a BACKWARD ATTACHMENT: it promised
  nothing. WIDTH = the most umbrae open at one boundary. DEPTH of an
  umbra = the umbrae opened strictly inside its span that outlive it.
- **Displaced arcs** are D131's per-arc non-projective arcs (amod and
  nmod only, KC 58.2). On each:
  - **COMPETITION:** COMPETED iff some word w other than the head, and
    outside d's own subtree, is at least as near as the head and fits
    d by its endings (comp(*, d, w): some reading of each satisfies some
    relation); else CLEAR.
  - **RETRIEVAL**, read when the later end arrives: dependent first, the
    open dangling umbrae that could depend on the arriving head, beyond
    the ones it truly closes; head first, the earlier words the
    arriving dependent could depend on, beyond its head. None: RETRIEVED;
    any: INTERFERED.
- **B** = D131's garden-path share: a crossed pair misread by R_P (the
  reader that takes the projective analyses when any exist).
- **Strata:** what is displaced (adjectives only / genitives only /
  both) x direction (all dependent first / all head first / mixed).
- **Population for U2a and U2b:** L0 crossed pairs with |F(s)| >= 2. A
  group is TESTABLE with >= 1,000 pairs and >= 2% of the set it is
  compared within.

## Result

1. **Width and depth.** Up to six words a pair keeps at most 6 umbrae
   open (projective pairs; crossed pairs at most 5); depth reaches 3.
   Six-word pairs: mean width 3.25 projective, 3.41 crossed; 18.39% of
   all umbrae have depth >= 1. Width and depth are the same at every
   level (checked), so they measure structure, not endings.

   | six-word pairs | width 2 | 3 | 4 | 5 | 6 |
   |---|---|---|---|---|---|
   | projective | 36,506,624 | 60,273,664 | 34,678,784 | 15,304,704 | 2,621,440 |
   | crossed | 83,334,144 | 276,943,872 | 248,086,528 | 51,783,680 | 0 |

2. **Head-first displacement costs no depth.** In every head-first
   stratum every umbra has depth 0; in the dependent-first and mixed
   strata 27.6-42.1% of umbrae have depth >= 1. A theorem (T4 review):
   two crossing arcs always include a non-projective one, i.e. a
   displaced amod or nmod arc; a head-first one of those is a backward
   attachment, not an umbra; so every crossing of two umbrae involves a
   DEPENDENT-first displaced arc, and all depth in this world - the
   root's included - comes from dependent-first displacement. Exactly half the displaced arcs are
   dependent first - a theorem: reversing a string keeps the pair, its
   displaced arcs and their kind, and swaps the two directions.
3. **Competition makes the garden paths (U2a HOLDS).** In every
   testable stratum, competed pairs are misread far more often than
   clear ones: +45.19 points for adjectives only (dependent or head
   first), +77.26 for genitives only, +17.56 for both kinds, and +17.72
   and +16.04 for the mixed-direction strata (adjectives, both). Expected; partly implied by
   the definitions (a nearer fitting word is what a projective
   alternative uses); registered so that a failure would be seen, and
   not read as a mechanism.
4. **The endings do the sorting for adjectives, not for genitives.**
   Beside the real label, a MORPHOLOGY-BLIND one (any noun as near as
   the head competes): in the adjectives-only strata almost every pair
   is blind-competed (27.5M of 28.7M, dependent first) and
   blind-competed pairs are misread 12.65%, real-competed 46.22% - the
   endings pick out the dangerous quarter. For displaced genitives the two labels nearly coincide
   (82.53% against 82.38%): a genitive's ending names no head, so every
   noun competes (as KC 58.4.4 found for the outcome).
5. **Retrieval does not "add nothing" (U2b FAILS) - and the genitive
   reversal is a configuration effect.** Dependent-first strata, within
   the competition cells. Adjectives, competed: interfered 49.75% of
   609,988, retrieved 45.89% of 6,436,919 (+3.86, inside the bound).
   Genitives, competed: interfered 73.83% of 27,986,240, retrieved 84.13%
   of 136,687,296 (-10.30), the same sign at the ideal level (62.87
   against 83.02). Up to five words (descriptive): adjectives +10.03,
   both +35.44, genitives -8.05. **Why interfered genitives look
   protected (found by the T4 review, now computed and pinned):** split
   the interfered pairs by their interferers. 7,578,272 have a GENITIVE
   interferer that no projective alternative uses (neither hanging d on
   it nor it on h); they are misread 5.95%, because 94.05% of them have
   NO projective analysis at all - two genitives interleaved
   cross-serially and stranded across the verb from every noun that could
   head them (*donorum amat regis puella donum*), so R_P falls back to
   all analyses and cannot misread. The other 20,407,968 interfered pairs
   are misread 99.04% - MORE than the retrieved ones (84.13%), the
   direction interference predicts; where a projective alternative hangs
   d on an interferer (16,484,368 pairs), always. Up to five words the
   same: 3.30% of 23,296 against 98.35% of 70,016 (retrieved 82.67%). So
   the protective sign is a selection effect: an interfering second
   genitive marks exactly the strings with no projective reading. The
   guess written in T3 (the open noun is the one a nesting reader would
   use, and its being unattached blocks the alternative) was wrong in its
   first clause and right in its second.
6. **A plan theorem fell: covered => competed.** The review's re-check
   M1 said every misread pair whose P(s) attaches a displaced word to a
   word as near as its head is COMPETED. Both the demo and the oracle
   found exceptions: 1,536 pairs up to five words, 560,472 at six. Every
   one is a REVERSED dependency (*domini dominorum amant domini
   dominos*: the truth hangs *domini* on *dominorum*, the alternative
   *dominorum* on *domini*): the nearer word lies in d's own subtree,
   which re-check L1 took out of competition because attaching there in
   the true tree closes a cycle. Two review fixes met. True and checked:
   covered from OUTSIDE d's subtree => COMPETED (0 exceptions). Of the
   386,865,852 misread six-word pairs, 99.81% are covered (0.29% by a
   reversed dependency); the CLEAR misreads are mostly reversed
   dependencies (560,472) and role changes that leave every displaced
   word on its head (423,370).
7. **Latin keeps a little under two thirds of the ideal's retrieval (U1
   FAILS).** Over the 193,351,680 displaced adjective arcs of six-word
   crossed pairs, the share RETRIEVED: L1 94.86%, L0 83.45%, L2 62.72%
   (L3 36.4%). Latin keeps 64.51% of the gap between ideal and
   case-stripped endings (I(L0) - I(L2) = 20.73 points against
   2 (I(L1) - I(L0)) = 22.81); the rule asked for 66.7% (and a 5-point
   floor, which holds). By direction the picture splits: dependent first
   Latin keeps 82.93%, head first 55.45% - when the adjective arrives
   last, perhaps its own syncretism (*bona*, *forti*, *fortis*) fails it
   (an interpretation, not decomposed). Up to five words U1 holds (19.09
   against 17.49) - but only under the reading fixed in T2 (all crossed
   pairs): on the registered population (L0 |F| >= 2, the same arcs
   labelled at each level) it fails at both lengths, 15.72 against 27.56
   up to five words and 19.96 against 25.07 at six (T4 review M2; pinned).
   The six-word verdict does not depend on the reading.
8. **Head-first genitives are almost never retrieved.** 3.21% at L0,
   3.71% even at L1: when a genitive arrives after its head, an earlier
   noun nearly always could also take it. Dependent first, 88.37%.
9. **Retrieval can fail through the subject.** First-declension *-ae* is
   nominative plural and genitive singular, so an open plural subject is
   a genitive candidate for every noun that arrives before the verb
   (*bonum puellae dominum amant*, already at four words; hand sentence
   9 at six). The design review had expected retrieval failure only from
   two same-feature nouns.
10. **The cycle-only reading barely matters.** Retrieval candidates that
    would close a cycle (kept, as registered) make 9,875,520 of the
    1,012,027,392 displaced arcs of six-word pairs INTERFERED that the
    cycle-free reading would call RETRIEVED (L0); hand sentence 12
    (*fortis puellae regis amant dominum*) is one.

## Registered predictions

Registered in plan.md before any data (approved 8be85808); decided at
SIX words on the pairs of exactly six words, L0, |F(s)| >= 2; no rule
changed after registration.

| | rule | verdict |
|---|---|---|
| U2a | every testable stratum B(competed) - B(clear) >= 10 points; >= 3 testable strata | **HOLDS** (8 testable; +16.04 to +77.26) |
| U2b | dependent-first strata, within COMPETED and within CLEAR: \|B(interfered) - B(retrieved)\| <= 5 points in every counting cell (both groups testable, B(retrieved) in [10%, 90%]); >= 2 counting cells | **FAILS** (2 counting: adjectives +3.86, genitives -10.30) |
| U1 | adjective arcs: I(L0) - I(L2) >= 2 (I(L1) - I(L0)) and >= 5 points | **FAILS** (20.73 against 22.81) |

Up to five words (descriptive): U2a HOLDS (7 strata), U2b FAILS (3
counting cells), U1 HOLDS (on all crossed pairs; on the |F| >= 2
population it fails, Result 7).

**Disclosure.**
- Seen before registering: D131's every published five-word number,
  including its adjective split (syncretic displaced word 7.28% against
  1.13%), close to a competition split - U2a's adjective strata were
  partly foreseen at five words, not at six.
- Readings fixed in T2 before any T2 number was printed: "six words" =
  pairs of exactly six words; U1 over all crossed six-word pairs (I(L)
  involves no misreading); retrieval with the candidates that would close
  a cycle kept (head first: words in d's subtree; dependent first: open
  words that are ancestors of h), as registered (Fran left it so in T1),
  the cycle-free variant beside it. The T4 review recomputed every
  verdict under the other readings (pooled up to six words, cycle-free
  retrieval, U1 on |F| >= 2): none flips.
- U2a's strata are not independent: competition and B are
  reversal-invariant, so each dependent-first stratum and its head-first
  mirror carry the same numbers. The 8 testable strata are 5 independent
  groups, still more than the 3 required.
- "Covered => competed" was a check, not a prediction; it failed as
  stated and was restated (Result 6). No verdict depends on it.
- The five-word and six-word results came out of one run.

## The design review, in hindsight

The reviewer (consiliarius-d134) turned a near-tautological U2 into two
tests: U2a, admitted as a positive control, and U2b, the discriminating
one, with a counting window so it could not pass vacuously, and moved
the labels onto displaced arcs. That made U2b decidable here (two
counting cells) and its failure informative: it failed by a cell OUTSIDE
the 5-point bound on the protective side, which no one had considered.
Two of its fixes conflicted (L1's subtree exclusion against M1's
covering theorem); the demo's own checks found that. Lesson for the next
review: when a fix changes a definition, re-derive every theorem the
plan states about it.

## Checks (120 full, 88 quick, 88 five-word)

- D131 reproduced: every pinned five-word total, mean |F| exactly; the
  tree table to n = 6 (728 projective, 5,652 mild, pinned from D131's
  `oracle_trees.py`); six-word pairs 809,533,440 and the largest |F| 144
  (D131 task 0).
- Umbrae + backward attachments = arcs + 1; one root umbra, closed at
  the verb; depth by the sweep == depth by pairwise crossing; projective
  => depth 0; exactly half the displaced arcs dependent first.
- Displaced arcs amod / nmod only, and the tree table's set == a yield
  route's; competition with the subtree excluded two ways; the true
  partner always a candidate; per arc RETRIEVED and CLEAR at L0 => at
  L1.
- The per-level pass (each level's own bags) == task 1's pass (L1 bags)
  for every arc label, level by level and length by length; width and
  depth the same at every level; the 2 x 2 cells hold every crossed pair
  and their misreads are D131's B by length; every misread pair has
  |F| >= 2; covered from outside the subtree => competed.
- Twelve hand sentences (the plan's eight; two at six words where
  retrieval fails with competition clear; one each for the subtree
  exclusion and the cycle-only reading), worked by hand first.
- Pins from `oracle_d134.py`: 1,125 totals up to five words
  (exhaustive), 1,125 up to four (quick mode), 837 on the six-word
  subset (2,250 of 288,000 L1 bags, 6,670,440 pairs; B at L0 and L1) -
  every number agreed on the first comparison.
- The rules on constructed totals at every threshold; the rules' inputs
  on constructed totals; verdicts from the oracle's totals == from the
  demo's (wiring).
- Since T4, pinned from the reviewer's own oracle (`rec.c`): the genitive
  cell by its interferers, and the displaced arcs of the |F| >= 2
  population labelled at every level - up to four, up to five, six
  words.

## Plants

All through `silva.planta`, all red.
- **T0 (7):** `first_order` reversed; the n = 6 tree pins; mean |F|; the
  sweep; the analyser skipping six-word bags; six-word sizes; the subset
  sum.
- **T1 (9):** v1's literal comp; the depth sweep; the root umbra; slots
  for optional arcs; T not set aside; head-first h against itself; the
  yield route; width; the subtree exclusion (green at four words until
  hand sentence 11 was added).
- **T2 (14):** U2a at 9 points; testable at 1%; U2b's 90% bound; U2b's
  gap signed; U1's floor; U2b on head-first strata; U1 on genitives;
  routes strictly nearer; |F| >= 3; misread by M; routes from M; blind
  counting any nominal; the cycle never set aside; directions swapped.
  Three needed five words (`D134_QUINQUE=1`).
- **T4 fixes (3):** the interferer-on-head test reversed; the
  interferer's relation off by one bit; the |F| >= 2 arcs labelled at L0
  only.

## Review (T4, 2026-10-20)

An independent reviewer (recensor-d134) wrote its own oracle in C,
`rec.c`: trees first (every rooted tree, then kinds, relations, lemmas
and numbers assigned top down), F(s) and P(s) by grouping pairs by
their string in hash tables - no method shared with main.c or the
Python oracles - and a separate brute-force parser for the hand
sentences. It reproduced all 1,125 pinned totals up to five words and
all 1,125 up to four (zero mismatches, first comparison), and counted
the SIX-WORD population EXHAUSTIVELY (809,533,440 pairs, not a subset):
every comparable line of the demo's six-word tables (113) and all three
verdicts with their inputs agree. The registration is byte-identical to
the approved plan. **No wrong verdict; no H.** M 2, L 6, N 7, all acted
on:

- **M1:** the genitive reversal (U2b's failing cell) is a configuration
  effect, not retrieval: Result 5 rewritten, the decomposition computed
  in the demo and pinned from the reviewer's oracle; the ledger question
  closed; KC 61.5.1.1 and 61.8 marked [falls] with corrected children.
- **M2:** the five-word U1 "holds" is reading-dependent (fails on the
  |F| >= 2 population at both lengths): Result 7 and KC 61.6.1 say so;
  the |F| >= 2 arc labels computed and pinned.
- **L1** head-first depth 0 is a theorem (proof in Result 2, KC 61.3.2.1);
  **L2** the dependent-first cycle candidates are ancestors of h, not
  d's subtree (disclosure corrected); **L3** the syncretism clause of KC
  61.6.1.1 is interpretation; **L4** Result 3's ranges; **L5** grades and
  status lines; **L6** 22.82 / 17.48 / 83.0% / 55.5% were differences of
  rounded shares: exactly 22.81 / 17.49 / 82.93% / 55.45%, now printed
  by the demo.
- **N1-N7:** the rules implemented exactly as registered; KC 61.3.3 and
  the reversal symmetry of U2a's strata sound; KC 61.7.2 proved; every
  reversed-route CLEAR pair hangs d on a DIRECT child of d (560,472 of
  560,472); the four-word subject example; every other number in the
  findings and KC 61 checked.

## Not covered

- **Retrieval for a reader that cannot fall back:** under R_P a string
  with no projective analysis is never misread, which is what made the
  genitive cell look protected (Result 5); a reader that must choose
  among crossed analyses would separate retrieval from that fallback.
- **Real Latin:** everything here is a toy world of one clause, four
  nouns and two adjectives; D133 found real displacement short.
- **Readers other than R_P:** B is one reader's misreads; umbrae and
  labels are computed on the true analysis, not on what a reader knows.
- **Seven words and up:** six words is exhaustive in C (21 minutes);
  seven would be about 120 times more.

## Run

```text
./bin/aedilis knotapel/demo_134_umbra_width/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
python3 -I knotapel/demo_134_umbra_width/oracle_d134.py 5
python3 -I knotapel/demo_134_umbra_width/oracle_d134.py 6 sub
```

- **Full mode:** about 21 minutes, 307 MB, 120 checks. **Five-word
  mode** (`D134_QUINQUE=1`): about 30 s, 88 checks. **Quick mode**
  (`D134_CELER=1`): four words, under a second, 88 checks. Times print
  with `D134_TEMPORA=1`; the totals in pin order with `D134_VECTOR=1`.
- **Oracle:** about 6 minutes for five words, 7 for the six-word
  subset (470 MB).
