# Demo 130: Latin Dependency Trees as Tangles — Findings

Proposal N9 (ledger …3X58Q9). Plan: `plan.md` (approved 2026-10-17;
its task notes record each task's decisions, including a CHANGE TO A
REGISTERED OPERAND, disclosed below). Context:
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

The demo itself had an independent review (T4, below). It re-derived
every number here with its own oracle, for all seven treebanks, with
zero mismatches. Its findings changed what P2 means and how three
results are read.

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
  added.
- **Non-projective arc: two operands.**
  - **REGISTERED (P2's operand as approved, plan 1c4c1b0c):** an arc
    in at least one crossing, the root arc included among the arcs.
  - **AMENDED (ce8816c8, per-arc, standard):** some word strictly
    between head and dependent is not dominated by the head,
    equivalently edge degree > 0.
  - **Non-planar arcs:** arcs crossing another real arc.
  - In a crossing pair usually only one arc is non-projective in the
    per-arc sense. The amendment's history is under "Registered
    predictions".
- **Gap degree, well-nested, edge degree** are K&N Defs. 6-9. **Mild**
  = gap degree <= 1 and well-nested.
- **The exact null.** Under a uniformly random order of a tree's
  words, two arcs with four distinct endpoints cross with probability
  exactly 1/3. Of the 24 relative orders of four endpoints, 8 nest, 8
  sit side by side and 8 cross. So E[crossings] = (disjoint arc
  pairs) / 3 exactly. **obs/exp** = crossings / E, POOLED over a
  treebank. The pairs grow like n^2, so long sentences weigh most. In
  LLCT the 51+-word bin holds 84% of the pairs.
- **Agreement** on an arc: head and dependent both carry the feature
  in FEATS and the values are equal. "all3" = Case, Gender and Number
  all agree.
- **Relations** are the universal DEPREL before ':' (`obl:arg` ->
  `obl`).
- **Headline** = trees without punctuation. UPOS PUNCT words are
  removed and their dependents re-attached to their head; a sentence
  whose root is PUNCT is set aside and counted. The second column,
  "annotated", is the trees as annotated. Both columns are printed and
  pinned for every part.

## Data

| treebank | text | sentences | words | source |
|---|---|---|---|---|
| CIRCSE | Seneca, *Hercules Furens* (verse) | 893 | 11,503 | repo, CC BY-SA |
| LLCT (dev + test) | charters, Tuscany 774-897 | 1,734 | 48,268 | repo, CC BY-SA |
| EWT (dev + test) | English web text | 4,078 | 50,242 | repo, CC BY-SA |
| PROIEL (test) | Vulgate (733 sentences), Cicero (326), Caesar (118), Palladius (83) | 1,260 | 14,091 | NC, fetched |
| Perseus (test) | Petronius (prose, 547); Ovid, Phaedrus, Vergil (verse, 392) | 939 | 10,964 | NC, fetched |
| ITTB (test) | Aquinas | 2,101 | 29,842 | NC, fetched |
| UDante (test) | Dante, *De vulgari eloquentia* | 421 | 13,474 | NC, fetched |

- **Repo treebanks** match FONTES.md exactly: sentences, syntactic
  words and multiword ranges, per file.
- **NC treebanks** were fetched by `./oratio/oraculum.sh -petere` from
  UD's master branches on 2026-09-08 and are read from `$D130_UD_NC`.
  Bytes and SHA-256 prefixes:
  - PROIEL 1,289,508, 35be241f1b5b47b6;
  - Perseus 1,153,778, e0e53fdcc8040a2a;
  - ITTB 3,184,535, b25d8f12a7f483ff;
  - UDante 1,319,582, e9eee3bf19b3ff84.

  Master moves, so these numbers belong to those files. The work
  composition of PROIEL comes from the T4 review, via its `# source`
  lines. By Fran's decision only aggregate numbers are reported, never
  a sentence.
- **Perseus is split by the work code** that starts each sent_id
  (phi0972 Petronius; phi0959 Ovid, phi0975 Phaedrus, phi0690 Vergil).
- **Every one of the 11,426 sentences is a sound tree.** EWT has 45
  all-punctuation sentences with a PUNCT root; they are set aside in
  the headline. No Latin treebank has one. Outside those sentences no
  PUNCT word has a non-PUNCT dependent in any treebank, so the
  headline re-attaches nothing ("moved" = 0).

## Result

1. **Pooled over each treebank, Latin crosses far less than random
   word order, and far more than English. P1 HOLDS.**
   - Pooled obs/exp runs from 0.0065 (LLCT) to 0.1022 (Perseus), all
     below the registered 1/4. English is 0.0010, below every Latin
     treebank.
   - The pooling matters. The UNWEIGHTED mean of per-tree obs/exp is
     **0.2745 in CIRCSE, above 1/4**, and its 484 sentences of 1-10
     words reach 0.3235. The other means are Perseus 0.1541, PROIEL
     0.0732, ITTB 0.0576, UDante 0.0431, LLCT 0.0148.
   - The rule as registered fixes no aggregation, and pooling is its
     natural reading, so the verdict stands; but "Latin keeps about a
     tenth of random crossings" is a statement about long sentences.
   - The null is weak evidence of crossing avoidance as such. Uniform
     random order ignores the well-known pressure toward short
     dependencies, which alone removes most crossings.
2. **Genre orders the density. P4 HOLDS, but its last step is not
   robust.**
   - Crossings per word: CIRCSE 0.1885 > Perseus 0.1433 > PROIEL
     0.0533 > LLCT 0.0502.
   - The PROIEL > LLCT step fails three tests (T4 review):
     - **sampling:** a sentence bootstrap gives a 95% interval of
       [-0.0052, +0.0116] for the difference, with PROIEL above LLCT
       in 77% of resamples;
     - **punctuation:** it reverses in the annotated column (LLCT
       0.0551 > PROIEL 0.0533);
     - **composition:** PROIEL's test split is 58% Vulgate, which
       alone (0.0276) crosses less than LLCT. The registration called
       PROIEL "classical prose".
   - With root-arc crossings counted too, P4 still holds (0.0622 vs
     0.0614).
   - ITTB (Aquinas, 0.0466) and UDante (0.0786) were not part of the
     prediction.
3. **Non-projectivity is common but mild. P3 HOLDS.**
   - Projective shares: 40.9% (UDante), 42.0% (CIRCSE), 58.9% (Perseus),
     67.9% (ITTB), 72.0% (LLCT), 72.9% (PROIEL); English 98.5%.
   - Yet every prose Latin treebank is at least 95% MILD: LLCT 97.58,
     PROIEL 97.38, ITTB 98.57, UDante 95.96 (by 4 trees: 404 mild, 400
     needed), Perseus's prose (Petronius) 97.99.
   - The verse is less mild, without a prediction: CIRCSE 88.13%,
     Perseus's verse 88.52%.
   - Gap degree >= 2 is at most 6.7% of trees (CIRCSE), 1.4% to 3.3%
     elsewhere in Latin.
   - For comparison, K&N's Czech (PDT) is 76.85% projective and Danish
     (DDT) 84.95%, both 99.89% well-nested. Those are counts over
     dependency FORESTS, several roots allowed, so they are yardsticks,
     not like-for-like.
4. **P2 FAILS under both operands. Only the verse-heavy treebanks
   (CIRCSE, Perseus) pass under both.**
   - **Registered operand** (arcs in a crossing, root arc included):
     the all-three-agreement ratio is CIRCSE x1.613 and Perseus x1.614
     (pass). PROIEL x1.046, ITTB x0.967, UDante x0.851 and LLCT x0.542
     fail. The amod + det half fails in LLCT and UDante.
   - **Amended operand** (per-arc): CIRCSE x3.124, Perseus x3.288,
     ITTB x1.992, PROIEL x1.805, UDante x1.800 pass; LLCT x0.931 fails,
     and its amod + det are rarer among non-projective arcs (10.7%
     against 17.3% of all).
   - **The LLCT failure is robust.** Its bootstrap interval is [0.81,
     1.06]; dev (x0.965) and test (x0.896) each fail alone; with
     punctuation kept it is x1.091, still below 1.5.
   - **The PROIEL and UDante passes under the amended operand are
     marginal**: bootstrap lower bounds 1.52 and 1.55.
   - **What survives:** in Seneca and in Perseus the crossing arcs
     carry agreement under either reading. In Seneca adjectives alone
     are 34.8% of per-arc non-projective arcs (9.9% of all), and 64.6%
     of them agree in all three. Beyond those two, whether crossing
     arcs agree depends on which arcs are counted. A crossing pair
     usually holds one non-projective (often agreeing) arc and one
     projective partner that seldom agrees: crossed-but-projective
     arcs agree in all three only 1.2% (ITTB) to 6.8% (CIRCSE) of the
     time (T4).
5. **The tangles are small.** The crossing graph (nodes = arcs, edges =
   crossings) breaks into components.
   - 37% to 67% of components with two or more arcs are a single
     crossing pair.
   - In Latin a component averages 1.7 (ITTB) to 3.6 (LLCT)
     crossings; in English 1.6.
   - Components of 9 or more arcs are at most 5.0% (Perseus).
   - Components per 100 words: 6.8 in Seneca, 4.8 Perseus, about 2.8
     PROIEL / ITTB / UDante, 1.4 LLCT, 0.13 English.
6. **Crossings per word RISE with sentence length; the falling
   obs/exp is the null's normalisation.**
   - Pooled obs/exp falls with length: CIRCSE 0.32 for 1-10 words to
     0.03 for 51+. But the pairs that could cross grow like n^2 by
     construction; in CIRCSE pairs per tree go from 6.2 to 2,432.
   - The raw rate grows: crossings per word in CIRCSE are 0.117,
     0.192, 0.188, 0.212, 0.310, and Perseus goes 0.080 to 0.296,
     PROIEL 0.029 to 0.101, LLCT 0.007 to 0.059.
   - Locality itself (arc lengths, phrase spans of crossings) was not
     measured. An earlier reading, "the crossings live inside phrases",
     is withdrawn.
7. **The picture, measured. [interpretation]**
   - What holds:
     - the skeleton is mostly planar, with few and small tangles (5);
     - in the verse-heavy treebanks the crossing strands carry
       matching agreement labels under either reading (4).
   - What does not hold: that this holds for "literary Latin" as a
     whole or fails for "charter Latin" as a register.
     - "Literary" was the complement of LLCT, chosen after the run.
     - Under the registered operand, Aquinas, Dante and PROIEL fail
       too.
     - The charter side is one treebank from one annotation team, and
       its relation profile looks convention-driven: det is 11.7% of
       LLCT's arcs, against 3.7% to 5.0% elsewhere.
   - A toy Latin Sudoku (N14) built on agreement-driven crossings
     models Seneca and Ovid-like verse safely; more than that is not
     established.

## The census (Part B, headline)

| bank | trees | proj% | planar% | wn% | cross/tree | nonproj% | nonplan% | obs/exp (pooled) | root-arc cross/tree | core deficit |
|---|---|---|---|---|---|---|---|---|---|---|
| CIRCSE | 893 | 41.99 | 44.90 | 92.72 | 2.4278 | 12.102 | 25.919 | 0.0614 | 0.2844 | 10.075% |
| LLCT | 1,734 | 71.97 | 72.66 | 99.88 | 1.1995 | 2.998 | 5.467 | 0.0065 | 0.2687 | 2.016% |
| EWT | 4,033 | 98.51 | 98.64 | 100.00 | 0.0228 | 0.167 | 0.367 | 0.0010 | 0.0027 | 0.147% |
| PROIEL | 1,260 | 72.86 | 74.05 | 99.37 | 0.5960 | 4.185 | 8.667 | 0.0262 | 0.0992 | 3.460% |
| Perseus | 939 | 58.89 | 61.77 | 96.38 | 1.3919 | 9.141 | 19.479 | 0.1022 | 0.2535 | 7.564% |
| ITTB | 2,101 | 67.87 | 68.87 | 99.95 | 0.5688 | 3.747 | 7.995 | 0.0231 | 0.0814 | 3.322% |
| UDante | 421 | 40.86 | 40.86 | 99.05 | 2.1378 | 4.123 | 10.457 | 0.0150 | 0.2209 | 3.652% |

Shares of arcs are of real arcs; nonproj% is per-arc. The core deficit
is the share of real arcs outside a largest crossing-free set.

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

As annotated, with punctuation: CIRCSE and PROIEL have no punctuation,
so their figures do not change. The others gain crossings through
punctuation arcs: LLCT goes from 1.1995 to 1.5329 crossings per tree,
Perseus to 1.8136, ITTB to 0.6840, UDante to 2.6366, EWT from 0.0228
to 0.0272. That is enough to reverse P4's PROIEL / LLCT step. The
demo prints the full second table.

By sentence length (headline): pooled obs/exp, and crossings per word.

| bank | 1-10 | 11-20 | 21-30 | 31-50 | 51+ |
|---|---|---|---|---|---|
| CIRCSE obs/exp | 0.3235 (484 trees) | 0.1216 (267) | 0.0611 (77) | 0.0409 (49) | 0.0274 (16) |
| CIRCSE cross/word | 0.1172 | 0.1921 | 0.1883 | 0.2118 | 0.3098 |
| LLCT obs/exp | 0.0121 (944) | 0.0376 (177) | 0.0259 (166) | 0.0087 (143) | 0.0049 (304) |
| LLCT cross/word | 0.0069 | 0.0535 | 0.0762 | 0.0496 | 0.0588 |
| PROIEL obs/exp | 0.0769 (748) | 0.0388 (360) | 0.0241 (110) | 0.0116 (38) | 0.0095 (4) |
| PROIEL cross/word | 0.0286 | 0.0581 | 0.0746 | 0.0628 | 0.1007 |
| Perseus obs/exp | 0.2060 (599) | 0.1087 (279) | 0.0799 (49) | 0.0577 (12) | - |
| Perseus cross/word | 0.0804 | 0.1537 | 0.2441 | 0.2955 | - |
| ITTB obs/exp | 0.0769 (1033) | 0.0330 (783) | 0.0173 (202) | 0.0114 (80) | 0.0010 (3) |
| ITTB cross/word | 0.0309 | 0.0500 | 0.0529 | 0.0570 | 0.0100 |
| UDante obs/exp | 0.1062 (55) | 0.0499 (115) | 0.0221 (113) | 0.0164 (100) | 0.0071 (38) |
| UDante cross/word | 0.0511 | 0.0826 | 0.0708 | 0.0877 | 0.0751 |
| EWT obs/exp | 0.0008 (2387) | 0.0010 (1063) | 0.0010 (400) | 0.0009 (166) | 0.0008 (17) |
| EWT cross/word | 0.0003 | 0.0015 | 0.0030 | 0.0046 | 0.0072 |

## Relations and agreement (Part C, headline)

Share of per-arc non-projective arcs / share of all real arcs (%), the
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

In LLCT the largest groups among per-arc non-projective arcs are obl
26.7%, advmod 11.2%, obj 9.2% and advcl 8.2%; together 55.3%. The demo
prints all 21 rows, in both columns, and the non-planar shares.

Agreement in all three features (% of arcs). REG is P2's registered
operand; "per-arc" is the amended one:

| bank | all arcs | REG | per-arc | non-planar | amod+det: all / per-arc |
|---|---|---|---|---|---|
| CIRCSE | 20.7 | 33.3 | 64.6 | 33.1 | 93.8 / 94.3 |
| LLCT | 22.6 | 12.3 | 21.1 | 12.3 | 75.7 / 96.9 |
| PROIEL | 12.1 | 12.6 | 21.8 | 12.5 | 60.5 / 68.4 |
| Perseus | 15.0 | 24.3 | 49.5 | 24.1 | 93.7 / 96.1 |
| ITTB | 12.9 | 12.5 | 25.7 | 12.5 | 92.7 / 95.8 |
| UDante | 14.7 | 12.5 | 26.4 | 12.5 | 91.4 / 93.3 |
| EWT | 0.0 | 0.0 | 0.0 | 0.0 | 0.0 / 0.0 |

- **English** marks Number but not Case or Gender on nouns, so all3 is
  0 by annotation. P2 concerns only Latin.
- **The registered and per-arc operands differ by about 2x** because a
  crossed pair usually holds one per-arc non-projective arc and one
  projective partner.
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

- Perseus by work: prose (Petronius) 97.99% mild (536 of 547); verse
  (Ovid, Phaedrus, Vergil) 88.52% (347 of 392).
- The mild shares are the same as annotated, in every treebank.

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

P1-P4 were registered before any census ran (plan 1c4c1b0c). Their
wording and thresholds never changed. Two operands did, both before
any census:

- **"Crossings" in P1 and P4** were narrowed in task 0 to real arcs
  (the root arc not counted). Counting root-arc crossings too changes
  no verdict (T4).
- **P2's operand.** The approved plan defined "non-projective arcs =
  arcs in at least one crossing", root arc included.
  - Task 0 (fb6e2532), correcting planar against projective, renamed
    that sentence "non-planar arcs" and so left P2's term without a
    definition.
  - The T1a library review reported the term as undefined. Claude
    passed that on to Fran as "the plan never defines it", which was
    false.
  - On that basis Fran approved the per-arc notion (ce8816c8). At that
    point the T1a reviewer had reported only that crossed and per-arc
    counts differ by about 2x; no agreement or relation figure had
    been computed.
  - The T4 review found the history. P2 is now DECIDED on the
    registered operand, and the amended one is reported beside it.
    (An earlier version of this file said the term was "defined before
    the census" because it had been undefined, and that the dilution
    by non-planar arcs was "why" it was defined. Both were wrong: the
    dilution was first measured in T2.)

| | rule | verdict |
|---|---|---|
| P1 | obs/exp < 1/4 in every Latin treebank; English below every Latin one | **HOLDS** (pooled; CIRCSE's per-tree mean is 0.2745) |
| P2 | all3 agreement among non-projective arcs >= 1.5x all arcs, and amod + det over-represented among them, in every Latin treebank | **FAILS**: registered operand in LLCT, PROIEL, ITTB, UDante; amended in LLCT |
| P3 | >= 95% mild in every prose Latin treebank (LLCT, PROIEL, ITTB, UDante, Perseus's prose) | **HOLDS** (95.96% to 98.57%) |
| P4 | crossings per word CIRCSE > {PROIEL, Perseus} > LLCT | **HOLDS** (PROIEL / LLCT step not robust) |

## Checks (50 full, 44 quick)

- **Part A:**
  - Kuhlmann and Nivre's Figures 2a and 3a-c, reconstructed from every
    property their text states.
  - Per repo file: sentences, words and ranges = FONTES.md. Per file:
    every sentence a sound tree.
  - On every analysed tree, in both columns:
    - the four routes to projectivity agree: by crossings with the
      root arc, by interval yields, by edge degree 0, by no
      non-projective arc. The last two share one library call;
    - projective => planar => well-nested;
    - per-arc crossings sum to twice the tree's, and crossed arcs and
      the largest edge degree match;
    - reversal invariance;
    - crossing components sum to the tree's figures, and every
      two-arc component is one crossing.
  - The planar core (interval DP) equals an independent search, done
    per crossing component, on ALL 22,807 tree analyses (both columns;
    none skipped).
  - The 1/3 law exactly: the sum of crossings over all n! orders =
    n! x pairs / 3, on 2,194 repo trees of <= 7 words. 880 of them
    have a disjoint pair; the other 1,314 pass trivially (0 = 0).
- **Parts B-D:** repo totals pinned in both columns from
  `oracle_partb.py` and `oracle_partcd.py`. These are independent
  Python counts with their own parsing, PUNCT removal, all-pairs
  crossings, interval yields, head walks, FEATS, well-nestedness over
  all disjoint pairs, and least-depth edge-degree roots. They now
  include the registered P2 operand, non-planar agreement and words
  per length bin. The checks say these are constants.
- **Perseus by work:** pinned to the T4 review's aggregates (prose 547
  / 536 mild, verse 392 / 347), said so.
- **Rules:**
  - each registered rule is a function, applied to the repo totals
    against answers computed by hand (both P2 operands);
  - each rule is also tested at its threshold on constructed totals.
- **NC rows:** re-derived by the T4 review's own oracle; there are no
  in-demo pins beyond Perseus.

## Plants

All through `silva.planta`, all red:

- **T1a (libraries, 29):** in `lib/dependentia.worklog.md` and
  `lib/conllu.worklog.md`.
- **T1b (17) and T2 (16):** listed in plan.md's task notes.
- **T4 fixes (13):**
  - both registered-P2 rules, and the registered operand without the
    root arc;
  - words per length bin;
  - the component search: upper neighbours, component membership;
  - the informative 1/3-law count;
  - the Perseus work code, Perseus mild;
  - the four rule thresholds (P1 <, the P2 amended >=, P2 registered
    amod+det >, P3 >=).

  One survived at first: swapping the registered P2 rule's 2 and 3
  moves the threshold from 1.5 to 0.67, which no repo treebank lies
  between. That led to the threshold check, and the plant then went
  red.

## Review (T4, 2026-10-17)

An independent reviewer (recensor-d130) built the snapshot in its own
scratch directory and wrote its own oracle with different algorithms:
- per-arc span counting;
- C(m, 2) - sum C(deg, 2) for the pairs;
- Euler-tour domination;
- greedy interleaving;
- BFS components;
- its own interval DP per crossing component, with brute force up to
  18 arcs.

It re-derived every printed table row (106), every pinned constant and
every number in this file and in KC 57, for all seven treebanks and
both columns. **Zero mismatches.** H 1, M 4, L 5, N, all fixed:

- **H1:** P2's operand history, above.
- **M1:** P4's step.
- **M2:** locality withdrawn; crossings per word printed.
- **M3:** P1 pooled.
- **M4:** register vs annotation convention.
- **L1:** Perseus split.
- **L2:** grades cite this review.
- **L3:** both columns in Parts C and D.
- **L4:** wording.
- **L5:** the informative 1/3-law count.

The reviewer did not check the live build, the plants or the ledger.

## Not covered

- **A canonical braid or tangle word for a sentence.** Part D
  measures what such an encoding would have to carry.
- **Why LLCT behaves differently:** formula, register or annotation
  convention. It is one treebank from one team; sub-relations, lemmas
  and document position were not examined (ledger …V3QKWX).
- **Locality:** lengths of crossing arcs, phrase spans of crossing
  components.
- **A null that keeps dependency lengths** (e.g. random projective
  orders, or orders matched for arc length), and genre within PROIEL.
- **Annotation conventions**, for example where punctuation and
  coordination attach. Differences between treebanks mix language
  with convention; nothing was corrected.
- **Training-split and full-treebank sizes.** Only test, and for LLCT
  and EWT dev + test, splits were used.

## Run

```text
./bin/aedilis knotapel/demo_130_latin_tangles/main.c &&
  bash build/aedilis/main/struere.sh &&
  D130_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
```

- **Full mode:** 0.6 s, 50 checks, 339 lines by wc -l (archive.sh
  counts one more). Without `D130_UD_NC`, or with the files absent,
  the NC rows are reported ABSENT and the verdicts are marked as not
  the registered ones.
- **Quick mode:** `D130_CELER=1` (repo treebanks only, 1/3 law up to
  5 words), 44 checks.
- **Snapshot:** gives byte-identical output with the same
  `D130_UD_NC`.
- **Python oracles:** run from the repo root with
  `python3 -I knotapel/demo_130_latin_tangles/oracle_part{b,cd}.py`.
