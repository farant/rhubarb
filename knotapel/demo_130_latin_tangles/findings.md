# Demo 130: Latin Dependency Trees as Tangles — Findings

Proposal N9 (ledger …3X58Q9). Plan: `plan.md` (approved 2026-10-17;
its task notes record each task's decisions). Context:
`knotapel/brainstorm-skeleton-settling.md` sections IV and XI.

The picture under test: a Latin sentence's syntax is a TANGLE. It has a
planar, Temperley–Lieb-like skeleton of nested dependencies. Crossings
appear where word order separates words that belong together
(hyperbaton). Agreement in case, gender and number is a label carried
along a strand. Before building on the picture (N14, the toy Latin
Sudoku), measure whether real Latin looks like it.

1. How often, and how much, do Latin dependency trees cross? Is the
   non-projectivity rare and mild (bounded, well-nested), or pervasive?
2. Are crossings concentrated in agreement relations (adjective and
   determiner hyperbaton), with the crossing arcs agreeing?
3. How does this vary by genre, and against English?

Everything is counted exactly, in integers and exact rationals, over
the treebanks' gold trees. The measures are the house library
`dependentia`; the CoNLL-U reader is the house library `conllu`. The
reader was moved there from oratio in task 0 so that the demo could be
frozen. Both libraries had an independent review before the demo used
them (T1a). The frozen copy is `demo-snapshot.c`: one file that
compiles alone; its banner gives the clang line and commit.

## Definitions

These are from the plan, checked against Kuhlmann and Nivre, "Mildly
Non-Projective Dependency Structures", COLING/ACL 2006 poster sessions,
pp. 507-514 ("K&N").

- **Arcs and crossings.** Words sit at positions 1..n; an artificial
  root sits at 0, with an arc to the sentence's root word. Two arcs
  CROSS when their endpoints interleave strictly. Arcs that share a
  word never cross.
- **Planar (K&N Def. 4):** no two real arcs cross; the root arc is NOT
  added.
- **Projective (Def. 3):** every word's yield (the words it dominates)
  is an interval. Equivalently, no crossing once the root arc is
  added. Planar is strictly weaker.
- **Non-projective arc** (the standard per-arc notion): some word
  strictly between head and dependent is not dominated by the head.
  Equivalently, the arc's edge degree is > 0. A **non-planar arc** is
  an arc in some crossing. The two differ: in a crossing pair usually
  only one arc is non-projective.
- **Gap degree, well-nested, edge degree** are K&N Defs. 6-9. **Mild**
  = gap degree <= 1 and well-nested.
- **The exact null.** Under a uniformly random order of a tree's
  words, two arcs with four distinct endpoints cross with probability
  exactly 1/3. Of the 24 relative orders of four endpoints, 8 nest, 8
  sit side by side and 8 cross. So E[crossings] = (disjoint arc
  pairs) / 3 exactly. **obs/exp** = crossings / E, summed over a
  treebank, so long sentences, which have many pairs, weigh most.
- **Agreement** on an arc: head and dependent both carry the feature
  in FEATS and the values are equal. "all3" = Case, Gender and Number
  all agree.
- **Relations** are the universal DEPREL before ':' (`obl:arg` ->
  `obl`).
- **Headline** = trees without punctuation. UPOS PUNCT words are
  removed and their dependents re-attached to their head; a sentence
  whose root is PUNCT is set aside and counted. The second column is
  the trees as annotated.

## Data

| treebank | text | sentences | words | source |
|---|---|---|---|---|
| CIRCSE | Seneca, *Hercules Furens* (verse) | 893 | 11,503 | repo, CC BY-SA |
| LLCT (dev + test) | charters, Tuscany 774-897 | 1,734 | 48,268 | repo, CC BY-SA |
| EWT (dev + test) | English web text | 4,078 | 50,242 | repo, CC BY-SA |
| PROIEL (test) | Caesar, Cicero, Vulgate, ... | 1,260 | 14,091 | NC, fetched |
| Perseus (test) | classical prose and verse | 939 | 10,964 | NC, fetched |
| ITTB (test) | Aquinas | 2,101 | 29,842 | NC, fetched |
| UDante (test) | Dante's Latin works | 421 | 13,474 | NC, fetched |

- **Repo treebanks** match FONTES.md exactly: sentences, syntactic
  words and multiword ranges, per file.
- **NC treebanks** were fetched by `./oratio/oraculum.sh -petere` from
  UD's master branches on 2026-09-08 and are read from `$D130_UD_NC`.
  Bytes and SHA-256 prefixes:
  - PROIEL 1,289,508, 35be241f1b5b47b6;
  - Perseus 1,153,778, e0e53fdcc8040a2a;
  - ITTB 3,184,535, b25d8f12a7f483ff;
  - UDante 1,319,582, e9eee3bf19b3ff84.

  Master moves, so these numbers belong to those files. By Fran's
  decision only aggregate numbers are reported, never a sentence.
- **Every one of the 11,426 sentences is a sound tree.** EWT has 45
  all-punctuation sentences with a PUNCT root; they are set aside in
  the headline. No Latin treebank has one. Outside those sentences no
  PUNCT word has a non-PUNCT dependent in any treebank, so the
  headline re-attaches nothing ("moved" = 0).

## Result

1. **Latin crosses far less than random word order, and far more than
   English. P1 HOLDS.**
   - obs/exp runs from 0.0065 (LLCT) to 0.1022 (Perseus), all below
     the registered 1/4.
   - English is 0.0010, below every Latin treebank.
   - Even the most tangled Latin keeps about a tenth of the crossings
     a random order would make.
2. **Genre orders the density. P4 HOLDS.**
   - Crossings per word, verse to prose to charters: CIRCSE 0.1885 >
     Perseus 0.1433 > PROIEL 0.0533 > LLCT 0.0502. The PROIEL / LLCT
     step is narrow.
   - ITTB (Aquinas, 0.0466) and UDante (0.0786) were not part of the
     prediction.
3. **Non-projectivity is common but mild.**
   - Projective shares: 40.9% (UDante), 42.0% (CIRCSE), 58.9% (Perseus),
     67.9% (ITTB), 72.0% (LLCT), 72.9% (PROIEL); English 98.5%.
   - Yet 95.96% to 98.57% of trees are MILD in every prose Latin
     treebank: **P3 HOLDS**. Without a prediction, CIRCSE is 88.13% and
     Perseus 94.04%.
   - Gap degree >= 2 is rare: at most 6.7% of trees (CIRCSE), 1.3% to
     3.3% elsewhere.
   - For comparison, K&N's Czech (PDT) is 76.85% projective and Danish
     (DDT) 84.95%, both 99.89% well-nested. Those are counts over
     dependency FORESTS, several roots allowed, so they are yardsticks,
     not like-for-like.
4. **Literary hyperbaton is agreement; the charters' is not. P2
   FAILS, on LLCT alone.**
   - In five of the six Latin treebanks both halves of the rule hold.
     The share of non-projective arcs agreeing in all three features
     is 1.80x (PROIEL, UDante), 1.99x (ITTB), 3.12x (CIRCSE) and 3.29x
     (Perseus) the share among all arcs.
   - amod + det are 18% to 55% of non-projective arcs against 8% to
     15% of all arcs. In Seneca, adjectives alone are 34.8% of the
     non-projective arcs (9.9% of all), and 64.6% of non-projective
     arcs agree in all three.
   - **In the charters (LLCT) agreement is slightly BELOW baseline:**
     21.1% of non-projective arcs against 22.6% of all, x0.93. amod +
     det are rarer among non-projective arcs, 10.7% against 17.3%. LLCT's
     non-projective arcs are oblique (26.7%), adverbial (11.2%),
     object (9.2%) and adverbial-clause (8.2%) arcs.
   - The rule was registered for every Latin treebank, so it fails.
     The genre split is the finding.
5. **The tangles are small.** The crossing graph (nodes = arcs, edges =
   crossings) breaks into components.
   - 37% to 67% of components with two or more arcs are a single
     crossing pair.
   - In Latin a component averages 1.7 (ITTB) to 3.6 (LLCT)
     crossings; in English 1.6.
   - Components of 9 or more arcs are at most 5.0% (Perseus).
   - Components per 100 words: 6.8 in Seneca, 4.8 Perseus, about 2.8
     PROIEL / ITTB / UDante, 1.4 LLCT, 0.13 English.
6. **Crossings are local.** obs/exp falls with sentence length in
   every Latin treebank: CIRCSE 0.32 for 1-10 words, 0.12, 0.06, 0.04,
   0.03 for 51+. LLCT is the exception at the short end (0.012, then
   0.038). Long sentences add many arc pairs that could cross and few
   that do. **[interpretation]** The skeleton is planar at large scale,
   and the crossings live inside phrases.
7. **The picture, measured.** **[interpretation]** For literary Latin
   the tangle picture holds in the form N14 needs: a mostly planar
   skeleton, few and small tangles, and crossing strands that carry
   matching agreement labels. Charter Latin crosses for another reason,
   so a toy Latin Sudoku built on agreement-driven crossings models the
   literary register, not Latin as such.

## The census (Part B, headline)

| bank | trees | proj% | planar% | wn% | cross/tree | nonproj% | nonplan% | obs/exp | root-arc cross/tree | core deficit |
|---|---|---|---|---|---|---|---|---|---|---|
| CIRCSE | 893 | 41.99 | 44.90 | 92.72 | 2.4278 | 12.102 | 25.919 | 0.0614 | 0.2844 | 10.075% |
| LLCT | 1,734 | 71.97 | 72.66 | 99.88 | 1.1995 | 2.998 | 5.467 | 0.0065 | 0.2687 | 2.016% |
| EWT | 4,033 | 98.51 | 98.64 | 100.00 | 0.0228 | 0.167 | 0.367 | 0.0010 | 0.0027 | 0.147% |
| PROIEL | 1,260 | 72.86 | 74.05 | 99.37 | 0.5960 | 4.185 | 8.667 | 0.0262 | 0.0992 | 3.460% |
| Perseus | 939 | 58.89 | 61.77 | 96.38 | 1.3919 | 9.141 | 19.479 | 0.1022 | 0.2535 | 7.564% |
| ITTB | 2,101 | 67.87 | 68.87 | 99.95 | 0.5688 | 3.747 | 7.995 | 0.0231 | 0.0814 | 3.322% |
| UDante | 421 | 40.86 | 40.86 | 99.05 | 2.1378 | 4.123 | 10.457 | 0.0150 | 0.2209 | 3.652% |

Shares of arcs are of real arcs. The core deficit is the share of real
arcs outside a largest crossing-free set.

Exact, in the headline column:

| bank | mean crossings per tree | crossings per word | obs/exp |
|---|---|---|---|
| CIRCSE | 2168/893 | 2168/11503 | 3252/52999 |
| LLCT | 1040/867 | 2080/41463 | 6240/960319 |
| EWT | 92/4033 | 92/44071 | 92/96545 |
| PROIEL | 751/1260 | 751/14091 | 751/28616 |
| Perseus | 1307/939 | 1307/9122 | 3921/38353 |
| ITTB | 1195/2101 | 239/5128 | 1195/51738 |
| UDante | 900/421 | 100/1273 | 1350/89911 |

As annotated, with punctuation, the shares move little. Perseus,
ITTB, UDante and LLCT gain crossings through punctuation arcs; for
example Perseus goes to 1.8136 crossings per tree. CIRCSE and PROIEL
have no punctuation. The demo's output gives the full second table.

obs/exp by sentence length (headline; trees in brackets):

| bank | 1-10 | 11-20 | 21-30 | 31-50 | 51+ |
|---|---|---|---|---|---|
| CIRCSE | 0.3235 (484) | 0.1216 (267) | 0.0611 (77) | 0.0409 (49) | 0.0274 (16) |
| LLCT | 0.0121 (944) | 0.0376 (177) | 0.0259 (166) | 0.0087 (143) | 0.0049 (304) |
| EWT | 0.0008 (2387) | 0.0010 (1063) | 0.0010 (400) | 0.0009 (166) | 0.0008 (17) |
| PROIEL | 0.0769 (748) | 0.0388 (360) | 0.0241 (110) | 0.0116 (38) | 0.0095 (4) |
| Perseus | 0.2060 (599) | 0.1087 (279) | 0.0799 (49) | 0.0577 (12) | - |
| ITTB | 0.0769 (1033) | 0.0330 (783) | 0.0173 (202) | 0.0114 (80) | 0.0010 (3) |
| UDante | 0.1062 (55) | 0.0499 (115) | 0.0221 (113) | 0.0164 (100) | 0.0071 (38) |

## Relations and agreement (Part C, headline)

Share of non-projective arcs / share of all real arcs (%), the
relations that matter most:

| rel | CIRCSE | LLCT | PROIEL | Perseus | ITTB | UDante | EWT |
|---|---|---|---|---|---|---|---|
| amod | 34.8 / 9.9 | 8.0 / 5.6 | 7.3 / 3.4 | 25.3 / 7.2 | 9.6 / 3.9 | 7.5 / 3.8 | 0.0 / 6.4 |
| det | 19.9 / 4.8 | 2.8 / 11.7 | 11.0 / 5.0 | 12.0 / 3.7 | 9.2 / 4.7 | 12.1 / 4.7 | 0.0 / 9.2 |
| nmod | 6.5 / 4.7 | 5.0 / 9.5 | 10.2 / 4.5 | 12.4 / 4.9 | 11.6 / 6.0 | 7.0 / 4.3 | 1.5 / 6.7 |
| acl | 7.7 / 3.1 | 5.0 / 4.5 | 11.9 / 2.9 | 6.6 / 1.9 | 18.0 / 4.1 | 13.2 / 3.2 | 22.4 / 1.9 |
| obl | 5.9 / 11.2 | 26.7 / 12.2 | 8.0 / 11.0 | 5.7 / 10.9 | 6.2 / 9.2 | 15.2 / 9.5 | 14.9 / 5.7 |
| obj | 2.9 / 10.3 | 9.2 / 3.7 | 4.3 / 7.5 | 5.2 / 12.5 | 2.4 / 4.5 | 5.9 / 4.7 | 9.0 / 5.9 |
| advmod | 2.3 / 6.9 | 11.2 / 3.9 | 5.2 / 6.5 | 3.1 / 9.4 | 5.0 / 8.3 | 3.1 / 6.2 | 0.0 / 6.7 |
| nsubj | 2.2 / 9.9 | 0.1 / 6.8 | 6.9 / 10.8 | 5.9 / 11.5 | 6.1 / 10.9 | 7.0 / 7.5 | 1.5 / 10.5 |
| mark | 0.2 / 2.1 | 0.3 / 2.6 | 4.1 / 3.7 | 0.8 / 3.6 | 14.6 / 6.7 | 5.5 / 6.0 | 0.0 / 3.8 |

The demo prints all 21 relation rows, the same table for non-planar
arcs, and agreement per feature. Agreement in all three features (%
of arcs):

| bank | all arcs | non-projective | non-planar | amod+det: all / non-proj |
|---|---|---|---|---|
| CIRCSE | 20.7 | 64.6 | 33.1 | 93.8 / 94.3 |
| LLCT | 22.6 | 21.1 | 12.3 | 75.7 / 96.9 |
| PROIEL | 12.1 | 21.8 | 12.5 | 60.5 / 68.4 |
| Perseus | 15.0 | 49.5 | 24.1 | 93.7 / 96.1 |
| ITTB | 12.9 | 25.7 | 12.5 | 92.7 / 95.8 |
| UDante | 14.7 | 26.4 | 12.5 | 91.4 / 93.3 |
| EWT | 0.0 | 0.0 | 0.0 | 0.0 / 0.0 |

- **English** marks Number but not Case or Gender on nouns, so all3 is
  0 by annotation. P2 concerns only Latin.
- **Non-planar arcs dilute the signal** (CIRCSE 33.1% against 64.6%):
  a crossed projective partner seldom agrees. This is why "non-
  projective arc" was defined before the census (plan.md, after the
  T1a review).
- **In LLCT the few amod + det arcs that are non-projective agree
  almost always** (96.9%). There are only 128 of them.

## Mildness and shape (Part D, headline)

| bank | gd0 | gd1 | gd2 | gd3+ | wn | MILD | ed0 | ed1 | ed2 | ed3+ |
|---|---|---|---|---|---|---|---|---|---|---|
| CIRCSE | 42.0 | 51.3 | 6.5 | 0.2 | 92.72 | 88.13 | 42.0 | 49.3 | 7.3 | 1.5 |
| LLCT | 72.0 | 25.7 | 2.2 | 0.2 | 99.88 | 97.58 | 72.0 | 26.7 | 1.0 | 0.3 |
| EWT | 98.5 | 1.5 | 0.0 | 0.0 | 100.00 | 100.00 | 98.5 | 1.5 | 0.0 | 0.0 |
| PROIEL | 72.9 | 24.9 | 2.1 | 0.2 | 99.37 | 97.38 | 72.9 | 25.3 | 1.4 | 0.4 |
| Perseus | 58.9 | 38.1 | 3.0 | 0.0 | 96.38 | 94.04 | 58.9 | 37.5 | 3.2 | 0.4 |
| ITTB | 67.9 | 30.7 | 1.3 | 0.0 | 99.95 | 98.57 | 67.9 | 31.4 | 0.7 | 0.0 |
| UDante | 40.9 | 55.8 | 3.3 | 0.0 | 99.05 | 95.96 | 40.9 | 57.5 | 1.7 | 0.0 |

Crossing-graph components with two or more arcs:

| bank | components | per 100 words | 2 arcs | 3 | 4 | 5-8 | 9+ | one pair | crossings each |
|---|---|---|---|---|---|---|---|---|---|
| CIRCSE | 782 | 6.798 | 43.4% | 21.7% | 15.6% | 16.1% | 3.2% | 43.4% | 2.772 |
| LLCT | 576 | 1.389 | 44.4% | 17.9% | 10.2% | 22.9% | 4.5% | 44.4% | 3.611 |
| EWT | 56 | 0.127 | 66.1% | 17.9% | 5.4% | 10.7% | 0.0% | 66.1% | 1.643 |
| PROIEL | 394 | 2.796 | 58.1% | 19.8% | 12.2% | 9.6% | 0.3% | 58.1% | 1.906 |
| Perseus | 441 | 4.834 | 39.5% | 23.6% | 15.0% | 17.0% | 5.0% | 39.5% | 2.964 |
| ITTB | 712 | 2.777 | 67.0% | 16.3% | 8.3% | 8.3% | 0.1% | 67.0% | 1.678 |
| UDante | 324 | 2.828 | 37.3% | 22.2% | 16.0% | 22.5% | 1.9% | 37.3% | 2.778 |

A two-arc component is always exactly one crossing (checked), so "one
pair" equals the 2-arc column.

## Registered predictions

All four were registered before any census ran (plan.md). One term,
"non-projective arc", was defined before the census. It was added
after the T1a review found the plan used it undefined; at that point
no agreement or relation figure existed.

| | rule | verdict |
|---|---|---|
| P1 | obs/exp < 1/4 in every Latin treebank; English below every Latin one | **HOLDS** |
| P2 | all3 agreement among non-projective arcs >= 1.5x all arcs, and amod + det over-represented among non-projective arcs, in every Latin treebank | **FAILS** (LLCT only: x0.93; amod + det 10.7% vs 17.3%) |
| P3 | >= 95% mild in every prose Latin treebank (LLCT, PROIEL, ITTB, UDante) | **HOLDS** (95.96% to 98.57%) |
| P4 | crossings per word CIRCSE > {PROIEL, Perseus} > LLCT | **HOLDS** |

**Perseus was not split by genre.** The plan's "Perseus's prose if
separable" is not met here. The only genre classifier in the repo is
oratio's, outside the tiers archive.sh can freeze. So Perseus is
reported against P3's threshold without a prediction, like CIRCSE.

## Checks (42 full, 38 quick)

- **Part A:**
  - Kuhlmann and Nivre's Figures 2a and 3a-c, reconstructed from every
    property their text states.
  - Per repo file: sentences, words and ranges = FONTES.md. Per file:
    every sentence a sound tree.
  - On every analysed tree, in both columns:
    - **the four routes to projectivity agree**: by crossings with the
      root arc, by interval yields, by edge degree 0, by no
      non-projective arc;
    - projective => planar => well-nested;
    - per-arc crossings sum to twice the tree's, and crossed arcs and
      the largest edge degree match;
    - **reversing the word order keeps** crossings, root crossings,
      gap and edge degree, well-nestedness and planar core;
    - crossing components sum to the non-planar arcs and crossings,
      and every two-arc component is one crossing.
  - The planar core (interval DP) equals an independent branching
    search on all 19,698 trees with <= 24 arcs.
  - **The 1/3 law exactly: the sum of crossings over all n! orders =
    n! x pairs / 3**, on all 2,194 repo trees of <= 7 words (quick:
    <= 5).
- **Part B:**
  - Repo totals for both columns pinned from `oracle_partb.py`, an
    independent Python count with its own parsing, PUNCT removal,
    all-pairs crossings, interval yields and head walks. These are
    constants, and the checks say so.
  - Length bins, crossing distribution and obs/exp pinned the same
    way.
  - The rational helpers.
- **Parts C-D:** repo totals pinned from `oracle_partcd.py`, a second
  independent Python count. It has its own FEATS parsing, tests
  well-nestedness over ALL disjoint subtree pairs (no sibling
  reduction), and computes edge degree by union-find with least-depth
  roots.
- **Rules:** each registered rule is a function, applied to the repo
  totals against answers computed by hand from the oracles' numbers.

## Plants

All through `silva.planta`, all red:

- **T1a (libraries, 29):** in `lib/dependentia.worklog.md` and
  `lib/conllu.worklog.md`.
- **T1b (17):**
  - the 1/3 law's relabelling and its factor;
  - the core search's adjacency and its isolated-vertex step;
  - reversal not reflecting;
  - PUNCT detection, the PUNCT-root count;
  - words, pairs and non-projective counts;
  - range counting, both bin boundaries;
  - the rational comparison and obs/exp without the 3;
  - planar counted as projective;
  - the hierarchy test.

  One first draft was an equivalent mutant: the branching search
  branches lowest-first, so it never reads the lower half of its
  adjacency. It was re-planted on the upper half and went red; a
  comment in main.c says why.
- **T2 (16):**
  - relation subtypes, both agreement rules, amod + det;
  - crossed used for non-projective;
  - the gap-degree bin, mild, the component-size bin and crossing
    halving;
  - kept-word alignment, the pair count, agreement read from the
    wrong word;
  - each of the four rule functions.

  Checks were added where a plant would otherwise have passed: the
  length-bin and distribution pins, the pair count, the rule check.

## Not covered

- **A canonical braid or tangle word for a sentence.** Part D
  measures what such an encoding would have to carry; the encoding
  itself is left to N14 or a later demo.
- **Why the charters cross.** Formulaic order is a guess;
  sub-relations and lemmas were not examined.
- **Genre within Perseus and PROIEL** (verse and prose; Vulgate and
  classical).
- **Annotation conventions differ between treebanks**, for example
  whether and where punctuation and coordination attach. Differences
  between treebanks mix language with convention; nothing was
  corrected.
- **Nulls other than uniform random order:** random projective
  orders, or a length-matched English null.
- **Training-split and full-treebank sizes.** Only test, and for LLCT
  and EWT dev + test, splits were used.

## Run

```text
./bin/aedilis knotapel/demo_130_latin_tangles/main.c &&
  bash build/aedilis/main/struere.sh &&
  D130_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
```

- **Full mode:** 0.6 s, 42 checks, 238 lines by wc -l (archive.sh
  reports 239, counting one more). Without `D130_UD_NC`, or with the
  files absent, the NC rows are reported ABSENT and the verdicts are
  marked as not the registered ones.
- **Quick mode:** `D130_CELER=1` (repo treebanks only, 1/3 law up to
  5 words), 38 checks.
- **Snapshot:** gives byte-identical output with the same
  `D130_UD_NC`.
- **Python oracles:** run from the repo root with
  `python3 -I knotapel/demo_130_latin_tangles/oracle_part{b,cd}.py`.
