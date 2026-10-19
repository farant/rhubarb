# Demo 133: Labels Buy Distance — Findings

Proposal N19 (ledger …7V5Z1J). Plan: `plan.md` (approved 2026-10-19
after a DESIGN REVIEW before any data - the first knotapel plan with
one; now the standard step). Context: D131 (KC 58), D132 (KC 59),
`knotapel/brainstorm-skeleton-settling.md` sections XIII-XV.

D131 and D132 suggested two ways a displaced word stays recoverable: a
LABEL (agreement repeats the head's case, gender and number, so the head
can be found by content: adjectives) or CLOSENESS (a genitive's ending
names no head; displaced genitives are few and short). In umbra terms
(brainstorm XV), a labelled open slot could WAIT - the reader finds its
head again by its features - while an unlabelled one must close soon.
This demo tests that: **do labels buy distance?**

**No.** All three registered predictions FAIL, cleanly, under a design
the review had hardened against the cheaper explanations. Displaced
adjectives and displaced genitives wait through about the same number of
foreign words (mostly exactly one), and an adjective whose label is
unique in its sentence waits no longer than one whose label is shared.
Short displacement is how these treebanks displace everything, not a
special constraint on unlabelled words.

Everything is counted exactly over the gold trees of six Latin UD
treebanks, headline without punctuation. The tree measures are
`dependentia`; exact arithmetic is `fractio` and `magnus`. The frozen
copy is `demo-snapshot.c`; its banner gives the clang line and commit.

## The design (after the review)

- **Populations:** genitives - nmod with Case=Gen, head NOUN or PROPN,
  relatives and interrogatives excluded (fronted by syntax, not chosen);
  adjectives - amod with an ADJ dependent carrying Case, Gender and
  Number, head NOUN or PROPN. Exclusions are counted (Part B below).
- **Displaced:** per-arc non-projective (as D132).
- **Operand F (foreign words):** the words strictly inside the arc's span
  that its head does not dominate - exactly the witnesses of its
  non-projectivity, what an open slot waits through outside its own
  phrase. Raw distance would count the dependent's own phrase (the
  review's sentence *regis magni amat filiam*: distance 3, F 1).
  Statistic: the total of min(F, 10) over a group.
- **Strata:** treebank (Perseus split into prose and verse) x direction
  (dependent-first opens an umbra; head-first is retrieval) x sentence
  length (<= 10, 11-20, > 20 words).
- **The exact test:** the group labels are permuted within strata; per
  stratum the count of label assignments by sum (dynamic programming,
  exact big integers), convolved across strata; p = P(T >= t_obs) as an
  exact fraction.
- **Unique / shared label:** an adjective's label is unique when no
  other noun in the sentence (outside its own subtree) agrees with it in
  Case, Gender and Number, by set intersection of the values.

## Result

1. **Populations.**

   | treebank | genitives | relative (excl.) | non-noun head (excl.) | population | displaced | adjectives | incomplete (excl.) | non-noun head (excl.) | population | displaced |
   |---|---|---|---|---|---|---|---|---|---|---|
   | CIRCSE | 477 | 6 | 22 | 449 | 77 | 1,031 | 0 | 19 | 1,012 | 434 |
   | LLCT | 2,382 | 0 | 11 | 2,371 | 5 | 2,179 | 142 | 3 | 2,034 | 68 |
   | PROIEL | 458 | 2 | 50 | 406 | 26 | 421 | 58 | 15 | 348 | 28 |
   | Perseus | 255 | 9 | 27 | 219 | 39 | 510 | 1 | 42 | 467 | 168 |
   | ITTB | 1,098 | 41 | 101 | 956 | 59 | 880 | 0 | 65 | 815 | 72 |
   | UDante | 438 | 9 | 47 | 382 | 28 | 416 | 1 | 27 | 388 | 34 |

   Personal-pronoun genitives (kept) number 10, 125, 6, 11, 142, 36.
   Multi-valued Gender among the adjectives: CIRCSE 8, PROIEL 74.
2. **L1 FAILS: displaced adjectives do not wait longer than displaced
   genitives.** Pooled over 38 strata, the adjectives' total min(F, 10)
   is 1,151 against an expected 1,143.26 under the permutation null;
   p = 0.2086. Per treebank, T exceeds E in CIRCSE, PROIEL, Perseus and
   UDante but not ITTB (four of the five non-LLCT treebanks, which by
   itself has chance 0.183 under the null); the pooled clause fails.
   - The adjectives' mean min(F, 10) is 1,151 / 804 = 1.43, with 69.5%
     at exactly one foreign word; the genitives' 1.33, 78.6% at one (the
     genitive figures from the oracle's items, not pinned).
3. **L1u FAILS:** on dependent-first arcs only (the umbra case), p =
   0.3991 (T 878 against E 875.47).
4. **L3 FAILS: a unique label does not wait longer than a shared one.**
   Among displaced adjectives (five treebanks, LLCT excluded), the unique
   group's total is 787 against an expected 788.80; p = 0.6104.
5. **Every sensitivity agrees** (reported, not decided):

   | variant | L1 p | L3 p |
   |---|---|---|
   | registered | 0.2086 | 0.6104 |
   | one displaced arc per sentence per kind | 0.2142 | 0.1852 |
   | function-word-only arcs removed | 0.2752 | 0.5932 |
   | raw distance instead of F | 0.9715 | 0.4988 |
   | Perseus as one stratum | 0.2083 | 0.5680 |

   On raw distance, displaced adjectives are if anything CLOSER to their
   nouns than displaced genitives (p 0.97).
6. **The excess beyond distance (descriptive; L2 was not registered).**
   With every noun as a possible host, the safe share against the
   distance-limited null's expectation:

   | treebank | adjectives S/N vs E/N (points) | genitives S/N vs E/N (points) |
   |---|---|---|
   | CIRCSE | 35.48 vs 36.33 (-0.85) | 44.16 vs 42.51 (+1.65) |
   | LLCT | 0.00 vs 0.74 (-0.74) | 60.00 vs 29.52 (+30.48) |
   | PROIEL | 67.86 vs 59.05 (+8.81) | 61.54 vs 53.85 (+7.69) |
   | Perseus | 46.43 vs 43.24 (+3.19) | 66.67 vs 58.35 (+8.32) |
   | ITTB | 75.00 vs 69.21 (+5.79) | 67.80 vs 65.83 (+1.97) |
   | UDante | 76.47 vs 67.97 (+8.50) | 75.00 vs 68.75 (+6.25) |

   Small excesses of the same order for both kinds, in both directions;
   no pattern separating labelled from unlabelled words.
7. **Baselines (descriptive).** Nested arcs sit at a mean raw distance of
   1.07 to 1.40 words for adjectives and 1.18 to 1.76 for genitives. The
   mean mid-rank percentile of each displaced arc's distance among its
   own move-alone null's displaced placements is low for both kinds (0.05
   to 0.22): displaced words sit far closer to their heads than chance
   placement would put them.
8. **The picture, measured. [interpretation]**
   - D131: a label tells a reader WHICH head a displaced word belongs to.
     D133: it does not let the word travel FARTHER. Both kinds are
     displaced short - mostly across a single foreign word - and the
     difference between them is within chance, in every variant.
   - So D132's finding ("displaced genitives are short, and short is
     safe") is not about genitives lacking labels; it is the general
     shortness of Latin displacement, which dependency length
     minimisation would predict *(recalled: Futrell, Mahowald and Gibson
     2015)*.
   - The umbra intuition "a labelled slot can wait longer" is not
     supported here. A weaker version - labels permit the same short
     displacement with less risk of misreading (D131, D132) - survives.

## Registered predictions

Registered in plan.md before any measure ran (plan approved 8e78ff60,
after the design review). The rules never changed.

| | rule | verdict |
|---|---|---|
| L1 | pooled exact test p <= 1/20, AND T > E_perm in at least four of the five non-LLCT treebanks | **FAILS** (p = 0.2086; four of five, which alone has chance 0.183) |
| L1u | the pooled test on dependent-first arcs, p <= 1/20 | **FAILS** (p = 0.3991) |
| L3 | among displaced adjectives, unique-label T against shared, p <= 1/20 (LLCT excluded) | **FAILS** (p = 0.6104) |

**Disclosure.**
- Seen before registering (plan "Already seen"): D132's genitive results
  (that displaced genitives are short) and the adjectives' unique/shared
  group sizes. Not seen: any arc's F or distance by kind or label group,
  and the adjectives' distance-limited null.
- The design reviewer computed only totals not split by displacement,
  synthetic chance rates and constructed sentences.
- In T1 the quick mode (CIRCSE and LLCT) ran before the full run that
  decided the verdicts, on the same code.

## The design review, in hindsight

The reviewer (consiliarius-d133) forecast that the most likely failure
was L1 PASSING for the wrong reason (clause span, construction mix), and
built L3 as the discriminating test. Both kind-level and within-kind
tests came out null instead. The hardened design matters for reading a
null too: with relatives excluded, particle splits flagged, strata by
direction and length, and the foreign-word operand, the null is not an
artefact of a confound masking a real effect in the obvious places.

## Checks (34 full, 29 quick)

- Reader and trees as D132; D132's displaced counts and genitive
  distance-limited E reproduced exactly (the L2 thresholds' source).
- F by the dominance matrix == F by ancestor walks on every registered
  arc; F = 0 exactly when nested.
- The design review's 11 hand sentences reproduce exactly (A1-A6,
  G1-G5; their values were computed independently with D132's
  functions).
- The exact test: equal to brute force on three small strata; E_perm
  equal to the distribution's mean; the group's own sum and its
  complement give the same p and mean.
- Pins from `oracle_d133.py` (its own reader, F from ancestor sets, the
  exact test from value multisets with binomials, rebuilt nulls):
  populations, exclusions, baselines; L1, L1u, L3 and the eight
  sensitivities (T, N, strata, exact E_perm, p to 1e-9) in both modes;
  L1 per treebank and the clause's chance; the excess and the
  percentiles.
- The rules on constructed values at each threshold.

## Plants

All through `silva.planta`.
- **T1 (9 red):** F counting dominated words; the subset DP's seed; the
  convolution; the complement's tail; the distance-limited null's
  boundary; the p rule's strictness; L1's four-of-five; E_perm per
  stratum; D132's genitive population. The first round ran against D132
  (a gate script whose path a sed failed to rewrite) and was re-run.
- **T2 (7 red):** relatives kept; agreement on the first value only;
  length bins; direction; F capped at 9; the first-arc flag; Perseus
  verse as prose.
- **Not red:** the particle lemma list in the function-word flag is IDLE
  on this data (those particles always carry UPOS PART, CCONJ or ADV,
  which the flag already catches).

## Not covered

- **More clauses and longer distances:** cross-clause displacement, rare
  here, is where a label might buy real distance; a corpus or genre
  with long-distance hyperbaton (e.g. more verse) would test it.
- **Reading time and processing**: "waiting" is measured as foreign
  words in the gold tree, not as reader behaviour.
- **The exact stratified permutation test** stays in the demo (approved
  plan); a ledger desideratum asks to promote it to a house library when
  a second consumer appears.

## Run

```text
./bin/aedilis knotapel/demo_133_labels_distance/main.c &&
  bash build/aedilis/main/struere.sh &&
  D133_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
python3 -I knotapel/demo_133_labels_distance/oracle_d133.py [--celer] [NC_DIR]
```

- **Full mode:** about 1 s, 34 checks. **Quick mode** (`D133_CELER=1`):
  CIRCSE and LLCT, 29 checks, verdicts not decided. **Oracle:** 4 s.
