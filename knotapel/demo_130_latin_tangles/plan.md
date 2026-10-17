# Demo 130: Latin dependency trees as tangles (plan)

Status: approved by Fran (2026-10-17), with "Open for Fran" decided:
(1) option (b) — the NC treebanks are used, aggregate numbers only, no
sentences or excerpts quoted, the files stay fetched (never copied into
the repo); (2) the tree measures go in a small library beside oratio's
CoNLL-U reader (location and name settled in task 0, with the lint),
tested on its own; (3) the registered thresholds as written. Proposal
N9 in
`knotapel/demo-proposals.md` (ledger …3X58Q9); context
`knotapel/brainstorm-skeleton-settling.md` sections IV, XI.

## Question

The brainstorm's picture of a Latin sentence: its syntax is a TANGLE —
a planar (projective, Temperley–Lieb-like) skeleton, crossings where
word order separates words that belong together (hyperbaton), and
agreement (case, gender, number) as labels carried along a strand.
Before building anything on that picture (N14, the toy Latin Sudoku),
measure whether real Latin looks like it.

1. How often, and how much, do Latin dependency trees cross? Is the
   non-projectivity rare and mild (bounded, well-nested), or pervasive?
2. Are crossings concentrated where the picture predicts: in agreement
   relations (adjective/determiner–noun hyperbaton), with the crossing
   arcs carrying matching case, gender and number?
3. How does this vary by genre (verse, classical prose, scholastic
   prose, legal charters), and against English?

## What is counted (definitions)

- **Tree.** Per sentence, the syntactic words (UD IDs; multiword-token
  ranges `a-b` skipped, their words kept; empty nodes `a.b` already
  omitted by the reader), each with its HEAD; an artificial root at
  position 0 with an arc to the sentence's root word. Sentences that are
  not trees (no single root, a head out of range, a cycle) are counted
  and set aside, never repaired.
- **Arc** (h, d) spans the interval between h and d. **Two arcs cross**
  iff their endpoints interleave strictly (a < c < b < e for arcs {a, b},
  {c, e}). Arcs sharing a word never cross.
- **Projective** = no crossing arcs (root arc included). Equivalently,
  every word's yield (its subtree) is a contiguous interval; both
  definitions are computed and must agree (a check).
- **Crossings** = number of crossing pairs (the crossing number of the
  arc diagram drawn with all arcs on one side). **Non-projective arcs**
  = arcs in at least one crossing.
- **Mild non-projectivity** *(recalled: Kuhlmann and Nivre 2006,
  "Mildly non-projective dependency structures"; verify the definitions
  from the source in task 0)*: gap degree (the most gaps in any word's
  yield), well-nestedness (no two disjoint subtrees interleave), edge
  degree. Gap degree <= 1 + well-nested corresponds to the
  tree-adjoining-grammar level of mild context sensitivity, so
  "bounded crossings" gets a formal-language meaning.
- **Planar core** = the largest set of arcs with no crossing pair (a
  maximum independent set of the crossing graph; arcs on a line form a
  circle graph, where this is polynomial by interval dynamic
  programming): how far each sentence is from planar, in arcs.
- **Agreement on an arc:** head and dependent both carry the feature
  (Case, Gender, Number in FEATS) and the values are equal; reported per
  feature and for all three together.
- **Punctuation.** UD attaches punctuation by convention, which can add
  crossings that are not syntax. Every count is reported WITHOUT
  PUNCT arcs as the headline, and with them as a second column.

**The exact null: random word order.** For a fixed tree and a uniformly
random order of its words, two arcs with four distinct endpoints cross
with probability exactly 1/3: the relative order of the four endpoints
is uniform over its 24 possibilities, and the two arcs then form one of
three patterns (nested, side by side, crossing) in 8 of them each.
Arcs that share a word never cross. So the expected number of crossings is EXACTLY
(number of vertex-disjoint arc pairs) / 3, a rational per sentence —
no sampling. Observed / expected measures how strongly real word order
avoids crossings. (The 1/3 law is checked by exhaustive enumeration of
all orders for small trees.)

## Data

| treebank | text | in repo? | licence |
|---|---|---|---|
| la_circse (test) | Seneca, *Hercules Furens* (verse) | yes, `oratio/probationes/fixa/ud/` | CC BY-SA 4.0 |
| la_llct (dev, test) | Late Latin charters, Tuscany, 774–897 (legal) | yes | CC BY-SA 4.0 |
| en_ewt (dev, test) | English Web Treebank (contrast) | yes | CC BY-SA 4.0 |
| la_proiel (test) | Caesar, Cicero, Vulgate, ... | fetched, main's `oratio/build/ud/` (2026-09-08) | NC |
| la_perseus (test) | classical prose and verse | fetched | NC |
| la_ittb (test) | Aquinas (scholastic prose) | fetched | NC |
| la_udante (test) | Dante's Latin works | fetched | NC |

Sizes (in repo): CIRCSE 893 sentences / 11,503 tokens; LLCT dev+test
1,734 / 48,268; EWT dev+test 4,078 / 50,242 (FONTES.md, recounted).
FONTES.md's policy for the NC treebanks: fetched and referenced only,
"nothing derived from them is ever published" (see Open for Fran 1).

## Parts

- **Part A: trees.** Read every treebank (`oratio_conllu_legere`);
  sentence and token counts against FONTES.md; trees validated; the
  two projectivity definitions agree on every tree; hand-built trees
  with known crossings, gap degree and well-nestedness.
- **Part B: the crossing census.** Per treebank: projective share;
  crossings per sentence (distribution, mean as an exact rational);
  non-projective arc share; observed / expected crossings under random
  order (exact), overall and by sentence length; planar core deficit.
- **Part C: which relations cross, and do they agree.** Per relation
  (amod, det, nmod, obj, obl, nsubj, advmod, ...): its share of all arcs
  vs its share of non-projective arcs; for crossing arcs vs all arcs,
  the share with Case / Gender / Number agreement; the same for
  amod/det arcs alone (the hyperbaton case).
- **Part D: mildness and shape.** Gap degree and well-nestedness
  distributions; edge degree; the crossing graph's components (how many
  arcs and crossings per component; how often a component is a single
  crossing pair) — the shape a tangle encoding would have to carry.
  A braid-word encoding of sentences is NOT attempted here: no
  canonical one exists; Part D measures what any encoding must
  represent, and the encoding is left to N14 or a later demo.

## Checks (characterizations, not counts)

- Reader: counts equal FONTES.md for the repo treebanks.
- Projective by crossings == projective by contiguous yields, on every
  tree (two independent definitions).
- The random-order law: for every tree with n <= 7 words in the repo
  treebanks, the expected crossings by the 1/3 formula equal the mean
  over ALL n! orders, exactly.
- Hand trees: crossings, gap degree, well-nestedness, planar core of
  small constructed trees, computed by hand.
- Planar core: the dynamic program against brute force (all arc
  subsets) on every tree with <= 16 non-root arcs.
- Invariance: reversing a sentence's word order leaves crossings, gap
  degree and well-nestedness unchanged (a theorem check).

## Registered before running

(Decided per Latin treebank, headline without punctuation.)

- **P1 (rarity):** in every Latin treebank, observed / expected
  crossings under random order is below 1/4; English is lower than
  every Latin treebank.
- **P2 (agreement):** in every Latin treebank, the share of
  non-projective arcs that agree in Case, Gender and Number is at least
  1.5 times the share among all arcs; and amod + det together make up a
  larger share of non-projective arcs than of all arcs.
- **P3 (mildness):** at least 95% of sentences have gap degree <= 1 and
  are well-nested in every PROSE Latin treebank (LLCT, PROIEL, ITTB,
  UDante, and Perseus's prose if separable); verse (CIRCSE) is reported
  against the same threshold without a prediction.
- **P4 (genre):** crossings per word: CIRCSE (verse) > classical prose
  (PROIEL, Perseus) > LLCT (charters); stated as an ordering of the
  exact means.

How the outcome reaches claims.md: a new part for language (first
non-braid knotapel claims), graded [exact] for counts (independent
oracle in the review), the predictions as registered verdicts.

## Size and time

Seven Latin treebank files plus two English, about 150,000 tokens:
counting is O(n^2) arcs per sentence; the n! check runs only up to
n = 7 (5,040 orders). Seconds.

## Tasks (one per turn, Fran's go-ahead each)

0. **Tooling and decisions:** whether aedilis builds and archive.sh
   freezes a knotapel demo that includes `oratio/fontes/oratio_conllu.h`
   (closure outside include/ and lib/; check conflator's client roots);
   verify the mild non-projectivity definitions from Kuhlmann and
   Nivre; settle Open for Fran 1–3.
1. **T1:** Parts A and B, checks, plants.
2. **T2:** Parts C and D, checks, plants.
3. **T3:** findings.md, claims.md, ledger, archive.sh.
4. **T4:** independent review with its own oracle, then fixes.

## Out of scope

- A canonical braid or tangle word for a sentence (Part D measures what
  one would need).
- Parsing: the trees are the treebanks' gold annotations; annotation
  conventions differ between treebanks (noted, not corrected).
- Compiling anything into weights (N14, N10).

## Open for Fran

1. **The NC treebanks.** FONTES.md says nothing derived from them is
   ever published. Aggregate statistics (crossing rates, shares) in a
   committed findings.md — is that "derived" for our purposes? Options:
   (a) repo treebanks only (Seneca verse, LLCT charters, EWT) — narrow
   but clean; (b) NC treebanks too, aggregate numbers only, no
   sentences or excerpts quoted (proposed: aggregate counts are
   measurements about a corpus, not redistribution of it; the
   treebanks stay fetched, never copied); (c) NC numbers in a local,
   uncommitted report only.
2. **Where the tree measures live.** Crossings, projectivity, gap
   degree, well-nestedness and the planar core are general
   dependency-tree algorithms. By the knotapel rule they belong in a
   house library, not the demo. Proposed: a small library beside
   oratio's reader (oratio is the natural consumer; a header in
   `oratio/fontes/` or a new `lib/` file, name to choose with the
   lint), tested on its own; alternative: demo code now, promoted when
   oratio needs it.
3. **The registered thresholds** (P1 1/4, P2 1.5x, P3 95%) as written,
   or different.
