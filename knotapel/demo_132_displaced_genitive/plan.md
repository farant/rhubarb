# Demo 132: real Latin and the displaced genitive (plan)

Status: approved by Fran (2026-10-19), "Open for Fran" decided as
proposed: (1) the hypothesis, G1 and G2 as written; (2) candidate hosts
NOUN and PROPN only; (3) the null moves the genitive alone; (4) folder
`knotapel/demo_132_displaced_genitive/`. Proposal N15
(`knotapel/demo-proposals.md`, ledger …PEYPY4); context
`knotapel/brainstorm-skeleton-settling.md` section XV, D131 (KC 58),
D130 (KC 57).

## Question

D131 found, in a toy world, that endings cannot place a displaced
GENITIVE. A genitive's ending names its role (possessor), not its head:
it agrees with nothing, so any noun could host it. A displaced
ADJECTIVE is different: it repeats its head's case, gender and number,
and only an agreeing noun can host it.

Yet real Latin displaces genitives. D130's printed Part C table has
nmod over-represented among per-arc non-projective arcs in five of six
Latin treebanks (PROIEL 10.2% against 4.5% of all arcs, Perseus 12.4% /
4.9%, ITTB 11.6% / 6.0%; LLCT the exception, 5.0% / 9.5%). So how does
a reader recover a displaced genitive?

The hypothesis here: **writers displace a genitive where the word order
itself leaves no nested alternative**, i.e. where no other noun could
take the genitive with a non-crossing arc. Then a nesting-first reader
(D131's R_P) would never be led astray: if the only nested reading is
missing, the crossed one is the reading.

1. Of the displaced genitives in real Latin, what share are SAFE (no
   other noun could host the genitive with a projective arc)?
2. Is that share higher than chance placement would give?
3. Contrast: displaced adjectives, where agreement filters the hosts.

## What is counted (definitions)

- **Trees:** D130's, read with `conllu`, headline WITHOUT punctuation
  (UPOS PUNCT removed, dependents re-attached, `dependentia_omittere`),
  sentences with a PUNCT root set aside. Same seven treebanks: CIRCSE,
  LLCT (dev + test), PROIEL, Perseus, ITTB, UDante; EWT is not used
  (English has no genitive case feature). The NC treebanks are read
  from main's `oratio/build/ud` (`D132_UD_NC`), aggregates only, never a
  sentence quoted (Fran's decision (b) for D130 carries over).
- **Genitive arc:** an arc whose relation is nmod (any subtype) and
  whose dependent has `Case=Gen` in FEATS. (Task 0 checks how each
  treebank labels genitive attributes, before any measure is computed.)
- **Displaced:** the arc is per-arc non-projective (`dependentia_arcus`:
  a word strictly inside its span is not dominated by its head).
- **Candidate host:** a word with UPOS NOUN or PROPN, other than the
  true head, not in the genitive's own subtree (so re-attaching makes
  no cycle). Task 0 reports what share of true genitive heads are
  NOUN or PROPN.
- **Nested alternative:** a candidate host h' such that, with the
  genitive re-attached to h' and nothing else changed, the arc (h', g)
  is projective (per-arc, `dependentia_arcus` on the modified tree).
- **Safe:** a displaced genitive with NO nested alternative. A
  nesting-first reader cannot be misled about its head by a nested
  reading of the same attachment.
- **Positional null:** for a displaced genitive g, move g ALONE (its
  dependents stay attached to it, everything else keeps its order) to
  each of the n positions of the sentence. Among the placements where
  g's arc to its true head is still displaced, the share that are safe.
  Each displaced genitive contributes this share (an exact rational);
  their sum is the null's expected number of safe genitives.
- **Adjective contrast (not registered):** the same for amod arcs whose
  dependent is ADJ with Case, Gender and Number. Candidate hosts are
  then nouns that AGREE in all three (agreement-filtered), and also
  reported unfiltered.

## Hand walk-through (D131's lesson: every measure through hand sentences of every kind before registering)

Trees use a transitive verb as root; nouns attach to it, the genitive
to its head noun. *regis* = of the king; *filiam* = daughter (acc);
*puella* = girl (nom). Positions from 1.

- **(a) nested:** *filiam regis amat*: regis(2) -> filiam(1), adjacent.
  Not displaced; not counted.
- **(b) displaced, unsafe:** *regis puella filiam amat*: regis(1) ->
  filiam(3) crosses puella(2) -> amat(4). Displaced. Candidate host
  puella(2): arc (1, 2) is adjacent, projective. A nested alternative
  exists: UNSAFE (a nesting-first reader takes "the king's girl").
  - Null: remove regis, leaving *puella filiam amat*; insert at the four
    positions. Position 1 (the original): displaced, unsafe. Position 2
    (*puella regis filiam amat*) and 3 (*puella filiam regis amat*):
    adjacent to filiam, not displaced. Position 4 (*puella filiam amat
    regis*): regis(4) -> filiam(2) spans amat(3), not dominated:
    displaced; puella(1) as host gives arc (1, 4), which spans filiam
    and amat, not dominated by puella: no nested alternative: SAFE.
    Null share 1/2; observed 0.
- **(c) displaced, safe:** *regis amat puella filiam*: regis(1) ->
  filiam(4) spans amat(2), the root, not dominated: displaced. Host
  puella(3): arc (1, 3) spans amat: not projective. No nested
  alternative: SAFE.
  - Null: *amat puella filiam* with regis inserted. Position 1: the
    original, safe. Position 2 (*amat regis puella filiam*): regis(2) ->
    filiam(4) spans puella(3), not dominated: displaced; puella(3) as
    host gives (2, 3), adjacent: UNSAFE. Positions 3 and 4: adjacent,
    not displaced. Null share 1/2; observed 1.
- **(d) adjective contrast:** *bonam puella filiam amat* (bonam, f acc
  sg -> filiam): displaced. Agreement-filtered hosts: puella is
  nominative, does not agree: no nested alternative: SAFE. Unfiltered,
  puella(2) would give an adjacent arc: unsafe. Agreement is what makes
  the adjective safe (D131).
- **What the walk-through shows:** the measure separates (b) from (c);
  the null depends on where the verb and the other nouns sit, so it is
  computed per sentence; the verb's position matters (in (c) the root
  between the genitive and every other noun makes it safe). A pooled
  null across sentences would mix sentence shapes, so each genitive
  carries its own null share and the comparison is per treebank.

## Registered before running

(Decided per Latin treebank, headline without punctuation, on exact
totals: S = displaced genitives that are safe; E = the null's expected
number of safe ones, the sum of per-genitive rationals; N = displaced
genitives.)

- **G1 (writers displace genitives safely):** in at least five of the
  six Latin treebanks, S > E: more displaced genitives are safe than
  chance placement of the same genitives in the same sentences would
  give.
- **G2 (and it matters):** in at least five of the six, S / N >= 1/2:
  most displaced genitives are safe.

G1 is the hypothesis; G2 says it carries most of the cases. If G1
holds and G2 fails, safety is preferred but most displaced genitives
still need something else (meaning, frequency).

Reported beside them, without prediction:
- per treebank: N, S / N, E / N, and the nested genitives' count
  (context);
- verse (CIRCSE, Perseus's verse) and prose separately (Perseus split
  by work code as in D130);
- the adjective contrast, agreement-filtered and unfiltered, with its
  own null;
- the unsafe displaced genitives: how many alternatives they have, and
  the alternative's distance.

How the outcome reaches claims.md: a node beside 58 in Part XI; counts
[exact] after an independent oracle; G1 and G2 as registered verdicts.

## Checks (characterizations, not counts)

- Reader and trees as D130 (sentence counts per repo file = FONTES.md;
  every tree sound).
- The hand walk-through above, as hand-built trees: (b) unsafe with
  null 1/2, (c) safe with null 1/2, (d) safe filtered and unsafe
  unfiltered.
- Re-attachment never makes a cycle (candidate hosts exclude the
  subtree): every modified tree validates (`dependentia_validare`).
- The null's placements: for each genitive, the placement at its own
  position reproduces the original tree's verdict (displaced, and safe
  or not) - the null contains the observation.
- Reversal: reversing a sentence leaves displaced, safe and the null
  share unchanged.
- Totals pinned from an independent Python oracle (its own reader,
  domination by ancestor walk, its own re-attachment), labelled as
  constants; the NC rows re-derived by the T4 review.
- The rules as functions on constructed totals, each clause isolated
  at its threshold (S = E fails; S / N = 1/2 holds; four treebanks of
  six fails).
- Plants via `silva.planta`.

## Size and time

D130's trees (11,426 Latin sentences in the repo and NC files). The
null moves one word through n positions and tests one arc each time:
O(n^2) per position at most, small. Seconds. Exact rationals for E via
`fractio` (denominators up to the sentence length; sums per treebank).

## Tasks (one per turn, Fran's go-ahead each)

0. *Done 2026-10-19 (Task 0 findings below).* **Labels and sources:**
   how each treebank marks genitive attributes
   (relation, subtypes, Case=Gen), counted as TOTALS ONLY (genitive arcs
   per treebank, share with NOUN / PROPN heads), never split by
   displacement; the hand trees run through `dependentia`; `fractio`
   for E.
1. **T1:** Part A (trees, genitive arcs, the hand walk-through, cycle
   and containment checks) and Part B (S, E, N per treebank, G1 and G2,
   reports); checks; plants.
2. **T2:** the Python oracle; pins; rules on constructed totals;
   plants.
3. **T3:** findings.md, claims.md, ledger, archive.sh.
4. **T4:** independent review with its own oracle, then fixes.

## Task 0 findings (2026-10-19)

- **Labels (totals only, annotated trees; no projectivity computed).**
  In all six Latin treebanks genitive attributes are plain `nmod` with
  `Case=Gen`; every `nmod` dependent with a case is mostly genitive,
  and no genitive-specific subtype is used. Genitive nmod arcs: CIRCSE
  477, LLCT 2,382, PROIEL 458, Perseus 255, ITTB 1,098, UDante 438.
  Other relations carry Case=Gen too (amod, det, conj: agreeing words,
  not genitive attributes) and are not counted.
- **Heads.** The true head is NOUN or PROPN in 95.4% (CIRCSE), 99.5%
  (LLCT), 88.6% (PROIEL), 88.6% (Perseus), 89.6% (ITTB), 89.0%
  (UDante) of genitive nmod arcs; the rest are ADJ, DET, PRON, NUM,
  VERB (e.g. partitives after numerals and pronouns). As approved, the
  population is EVERY genitive nmod arc; the noun-headed subset is
  reported beside it, not decided on.
- **Dependents** are mostly NOUN and PROPN, also PRON (*eius*) and some
  ADJ, DET, VERB (substantivised, gerunds); all count.
- **The hand walk-through through `dependentia`** (scratch probe,
  compiled against lib/dependentia.c): (a) not displaced; (b) displaced,
  1 nested alternative, UNSAFE, null 1/2; (c) displaced, SAFE, null
  1/2; (d) agreement-filtered SAFE with null 2/2, unfiltered UNSAFE with
  null 1/2. All as the plan states. New: with agreement filtering, a
  sentence with no other agreeing noun makes every placement safe, so
  the adjective contrast's null is 1 there; the contrast is reported
  only, and this degeneracy will be stated with it.
- **Exact E:** `fractio` (`fractio_ex_s64_s64`, `fractio_adde`,
  `fractio_compara`) covers the per-genitive shares and their sums; G1
  compares S with E exactly.
- **A caution, before any result:** the genitive totals are small in
  CIRCSE (477) and Perseus (255); their displaced subsets will be
  smaller still, so G1's per-treebank comparisons there can turn on a
  few sentences. The rules are fixed; findings will report N beside
  every verdict.

## Disclosed before registering

- D130's nmod shares (above) were seen. The Case=Gen split, every host
  count, S, E and the adjective contrast were not.
- D131's toy result (KC 58) motivated the hypothesis; its numbers are
  not used here.

## Open for Fran

1. The hypothesis and its two rules as written (G1 "at least five of
   six", G2 "most are safe")?
2. Candidate hosts NOUN and PROPN only (pronouns excluded)?
3. The null moves the genitive alone, its dependents staying attached
   to it (simple, and it changes only the arc being tested); moving its
   whole subtree is the alternative.
4. Folder `knotapel/demo_132_displaced_genitive/`.
