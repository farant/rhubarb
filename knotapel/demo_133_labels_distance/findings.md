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

**Not measurably.** All three registered predictions FAIL. Displaced
adjectives and displaced genitives wait through about the same number of
foreign words (mostly exactly one), and an adjective whose label is
unique in its sentence waits no longer than one whose label is shared.
The data exclude an effect of about +0.17 foreign words per arc or more
(95%); smaller effects are not excluded (power, below). Short
displacement is how these treebanks displace everything. What labels
visibly do instead: a displaced adjective almost never crosses a noun
with the same label - 3 times in 804, where chance placement would give
92.7 (descriptive, found by the review; Result 7).

Everything is counted exactly over the gold trees of six Latin UD
treebanks, headline without punctuation. The tree measures are
`dependentia`; exact arithmetic is `fractio` and `magnus`. The frozen
copy is `demo-snapshot.c`; its banner gives the clang line and commit.
The demo had an independent review (T4, below); every pinned number was
reproduced by a third exact method.

## The design (after the design review)

- **Populations:** genitives - nmod with Case=Gen, head NOUN or PROPN,
  relatives and interrogatives excluded (fronted by syntax, not chosen);
  adjectives - amod with an ADJ dependent carrying Case, Gender and
  Number, head NOUN or PROPN. Exclusions are counted (Result 1).
- **Displaced:** per-arc non-projective (as D132).
- **Operand F (foreign words):** the words strictly inside the arc's span
  that its head does not dominate - exactly the witnesses of its
  non-projectivity, what an open slot waits through outside its own
  phrase. Raw distance would count the dependent's own phrase (the
  review's sentence *regis magni amat filiam*: distance 3, F 1).
  Statistic: the total of min(F, 10) over a group (the cap is idle here:
  the largest F is 6).
- **Strata:** treebank (Perseus split into prose and verse) x direction
  (dependent-first opens an umbra; head-first is retrieval) x sentence
  length (<= 10, 11-20, > 20 words); L1u and L3 drop direction.
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
2. **L1 FAILS: displaced adjectives do not wait measurably longer than
   displaced genitives.** Pooled over 38 strata, the adjectives' total
   min(F, 10) is 1,151 against an expected 1,143.26; p = 0.2086. Per
   treebank, T exceeds E in CIRCSE, PROIEL, Perseus and UDante but not
   ITTB (four of the five non-LLCT treebanks, a clause chance alone meets
   with P 0.183); the pooled clause fails.
   - Mean min(F, 10): adjectives 1,151 / 804 = 1.43, 69.5% at exactly one
     foreign word; genitives 310 / 234 = 1.32, 78.6% at one (both pinned).
   - **ITTB runs the other way** (within ITTB the genitives wait longer,
     p 0.003 - the review's computation): 115 of its 131 displaced arcs
     (adjectives 68 of 72, genitives 47 of 59) are FUNCTION-WORD-ONLY - a
     phrase split by a particle such as *autem*, which the plan calls
     "not a chosen displacement". Elsewhere such arcs are a minority
     (pinned counts: CIRCSE 38/434 and 15/77, LLCT 1/68 and 2/5, PROIEL
     10/28 and 12/26, Perseus 10/168 and 0/39, UDante 8/34 and 8/28).
3. **L1u FAILS:** on dependent-first arcs only (the umbra case), p =
   0.3991 (T 878 against E 875.47).
4. **L3 FAILS: a globally unique label does not wait longer than a
   shared one.** Among displaced adjectives (five treebanks, LLCT
   excluded), the unique group's total is 787 against an expected
   788.80; p = 0.6104.
   - What L3 actually compared (T4 review): of the 201 "shared"
     adjectives, 190 have every agreeing competitor OUTSIDE the arc's
     span, most of them far away. So L3 tested "an agreeing noun
     somewhere in the sentence" against "none" - valid for its
     registered definition, but not the case the hypothesis turns on, a
     competitor INSIDE the gap, which almost never occurs (Result 7).
5. **Sensitivities** (reported, not decided):

   | variant | L1 p | L3 p |
   |---|---|---|
   | registered | 0.2086 | 0.6104 |
   | one displaced arc per sentence per kind | 0.2142 | 0.1852 |
   | function-word-only arcs removed | 0.2752 | 0.5932 |
   | raw distance instead of F | 0.9715 | 0.4988 |
   | Perseus as one stratum | 0.2083 | 0.5680 |

   - On raw distance, displaced adjectives are if anything CLOSER to
     their nouns than displaced genitives (p 0.97), partly because a
     genitive carries its own phrase.
   - The plan's percentile sensitivity was not run as a test by the demo
     (it prints the mean percentile of each arc's raw DISTANCE among its
     move-alone null's displaced placements, not the plan's F
     percentile). The review ran both by stratified Monte Carlo: L3 gives
     p = 0.022 (distance) and 0.030 (F), in the predicted direction, but
     it is a SENTENCE-LENGTH artefact - percentiles shrink in longer
     sentences, and shared-label adjectives sit in longer ones (48.5
     against 36.4 words in the > 20 bin); with exact sentence-length
     strata, p = 0.71 (distance) and 0.60 (F). (Review's scripts, not
     pinned.)
6. **Power** (T4 review, by simulation on the real strata; not pinned).
   The chance each registered test passes if the adjectives' (or unique
   adjectives') F were higher by delta per arc:

   | delta | L1 | L1 with its clause | L1u | L3 |
   |---|---|---|---|---|
   | +0.1 | 0.44 | 0.38 | 0.29 | 0.36 |
   | +0.2 | 0.93 | 0.87 | 0.65 | 0.89 |
   | +0.3 | 1.00 | 0.98 | 0.93 | 1.00 |

   Test inversion gives 95% intervals for the shift of [-0.065, +0.170]
   (L1), [-0.140, +0.201] (L1u), [-0.144, +0.116] (L3). Under the null
   the tests pass at 0.03-0.05, as they should. So the data exclude a
   shift of about +0.17 foreign words per arc (L1; +0.12 for L3) - about
   12% of the mean - and do not exclude 0.1. L1u is the weakest.
7. **A same-label noun in the gap: almost never (descriptive, found by
   the T4 review; pinned).** A displaced adjective has an agreeing
   competitor noun AMONG ITS FOREIGN WORDS in 3 of 804 cases (CIRCSE 1 of
   434, LLCT 1 of 68, Perseus 1 of 168, none elsewhere), where the
   move-alone null expects 92.7 (CIRCSE 55.80, LLCT 14.88, PROIEL 1.23,
   Perseus 11.26, ITTB 5.36, UDante 4.20). This is the measurable form of
   "a label tells the reader which head": writers almost never put a
   same-label noun in the way. **Caveat:** annotators who attach an
   ambiguous adjective to the nearest agreeing noun would produce the
   same pattern, so it is partly circular. Not registered; the natural
   prediction for a successor demo.
8. **The excess beyond distance (descriptive; L2 was not registered).**
   With every noun as a possible host, the safe share against the
   distance-limited null's expectation (points now from the exact
   difference):

   | treebank | adjectives S/N vs E/N (points) | genitives S/N vs E/N (points) |
   |---|---|---|
   | CIRCSE | 35.48 vs 36.33 (-0.84) | 44.16 vs 42.51 (+1.64) |
   | LLCT | 0.00 vs 0.74 (-0.74) | 60.00 vs 29.52 (+30.48) |
   | PROIEL | 67.86 vs 59.05 (+8.81) | 61.54 vs 53.85 (+7.69) |
   | Perseus | 46.43 vs 43.24 (+3.18) | 66.67 vs 58.35 (+8.32) |
   | ITTB | 75.00 vs 69.21 (+5.79) | 67.80 vs 65.83 (+1.96) |
   | UDante | 76.47 vs 67.97 (+8.50) | 75.00 vs 68.75 (+6.25) |

   Small excesses of the same order for both kinds, in both directions;
   no pattern separating labelled from unlabelled words.
9. **Baselines (descriptive).** Nested arcs sit at a mean raw distance of
   1.07 to 1.40 words for adjectives and 1.18 to 1.76 for genitives. The
   mean DISTANCE percentile of a displaced arc among its move-alone
   null's displaced placements is low for both kinds (0.05 to 0.22):
   displaced words sit far closer to their heads than chance placement
   would put them. The unique-label adjectives' percentiles are higher
   than the shared ones' (CIRCSE 0.16 vs 0.08, PROIEL 0.22 vs 0.06,
   Perseus 0.18 vs 0.13) - the sentence-length effect of Result 5, not
   a shape-free difference.
10. **The picture, measured. [interpretation]**
    - D131: a label tells a reader WHICH head a displaced word belongs
      to - and Result 7 shows writers keeping same-label competitors out
      of the gap. D133: a label does not let the word travel measurably
      FARTHER (no effect above about 0.17 foreign words per arc). Both
      kinds are displaced short, mostly across a single foreign word.
    - D132's short genitives are therefore general shortness, not a
      constraint on unlabelled words: adjectives and genitives have
      similar F (1.43 against 1.32) and similar low percentiles. "Similar",
      not identical: the pooled share at F >= 2 is 30.5% against 21.4%
      (stratified p 0.078, the review's computation).
    - The umbra intuition "a labelled slot can wait longer" is not
      supported at the resolution these data allow. A weaker version -
      labels make the same short displacement safe, and writers avoid
      putting a same-label noun in the way - is what the evidence shows.
    - Dependency length minimisation would predict the shortness
      *(recalled: Futrell, Mahowald and Gibson 2015)*.

## Registered predictions

Registered in plan.md before any measure ran (plan approved 8e78ff60,
after the design review). The rules never changed (the review checked
the git history).

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
- Results 6 and 7, the ITTB reversal and the percentile tests were found
  by the T4 review, after the verdicts.

## The design review, in hindsight

The reviewer (consiliarius-d133) forecast that the most likely failure
was L1 PASSING for the wrong reason (clause span, construction mix), and
proposed L3 as the discriminating test. Both came out null instead. The
hardening (relatives excluded, particle splits flagged, strata by
direction and length, the foreign-word operand) makes the null readable,
but two things it did not do: L3's "shared label" turned out not to
contain the discriminating case (a competitor in the gap, Result 4), and
no power argument was registered (Result 6). Both are lessons for the
next design review: test the case the hypothesis turns on, and register
the effect size the test can see.

## Checks (35 full, 30 quick)

- Reader and trees as D132; D132's displaced counts and genitive
  distance-limited E reproduced exactly (the L2 thresholds' source).
- F by the dominance matrix == F by ancestor walks on every registered
  arc; F = 0 exactly when nested.
- The design review's 11 hand sentences reproduce exactly (A1-A6,
  G1-G5; their values were computed independently with D132's
  functions). They check geometry and null shares only; the population,
  agreement and function-word code is covered by the pins.
- The exact test: equal to brute force on three small strata; E_perm
  equal to the distribution's mean; the group's own sum and its
  complement give the same p and mean.
- Pins from `oracle_d133.py` (its own reader, F from ancestor sets, the
  exact test from value multisets with binomials, rebuilt nulls):
  populations, exclusions, baselines; L1, L1u, L3 and the eight
  sensitivities (T, N, strata, exact E_perm, p to 1e-9) in both modes;
  L1 per treebank and the clause's chance; the excess and the
  percentiles; and the T4 descriptives (the gap count and its exact
  expectation, function-word-only arcs, F sums, arcs at F = 1).
- The rules on constructed values at each threshold.

## Plants

All through `silva.planta`.
- **T1 (9 red):** F counting dominated words; the subset DP's seed; the
  convolution; the complement's tail; the distance-limited null's
  boundary; the p rule's strictness; L1's four-of-five; E_perm per
  stratum; D132's genitive population. The first round ran against D132
  (a gate script whose path a sed failed to rewrite) and was re-run.
- **T2 (7 red):** relatives kept; agreement on the first value only;
  length bins; direction; the cap moved to 9 (red through the RAW-
  DISTANCE sensitivity: two arcs have distance >= 10; on F the cap is
  idle, the largest F being 6); the first-arc flag; Perseus verse as
  prose.
- **T4 fixes (3 red):** the gap null counting every placement; the gap
  observation never set; head-dominated competitors counted.
- **Not red:** the particle lemma list in the function-word flag is IDLE
  on this data (those particles always carry UPOS PART, CCONJ or ADV,
  which the flag already catches).

## Review (T4, 2026-10-19)

An independent reviewer (recensor-d133) built the snapshot and wrote its
own oracle: descendant sets from child lists for F, and the exact test by
KRONECKER SUBSTITUTION (each stratum's generating polynomial packed into
big integers), with Monte Carlo as a sanity bound. It reproduced every
pinned number in both modes, all 12 hand sentences on its own trees, and
the history. **No wrong verdict; no H.** M 3, L 5, N 8, all acted on:

- **M1:** the plan's percentile sensitivity was not run, and run it gives
  L3 p 0.02-0.03 - a sentence-length artefact (Result 5).
- **M2:** L3's "shared" group almost never has a competitor in the gap;
  the gap count (3 against 92.7) is the label's measurable work, now
  computed and pinned (Results 4, 7).
- **M3:** "No." overstated: power and intervals added (Result 6); ITTB's
  reversal and its particle splits reported (Result 2); KC 60's power
  objection answered.
- **L1** the genitives' mean 1.32 (pinned now); **L2** the cap idle on F
  and the plant relabelled; **L3** the hand trees' scope stated; **L4**
  ITTB's cause given; **L5** points from the exact difference.
- **N1-N8:** definitions match the registration; history as described
  (including the sign bug); robustness variants all null (L1 0.08-0.23,
  L3 0.32-0.88); uninformative strata (11 of 38 in L1) are no material
  loss; the checks' strength; Monte Carlo agrees; the citation is
  marked as recalled.

## Not covered

- **The discriminating test:** whether a displaced adjective may carry
  its label past a same-label noun - the gap case, which writers (or
  annotators) almost always avoid. A successor demo can register it,
  ideally on data where annotation was not guided by nearest agreement.
- **More clauses and longer distances:** cross-clause displacement, rare
  here, is where a label might buy real distance.
- **Reading time and processing:** "waiting" is foreign words in the gold
  tree, not reader behaviour.
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

- **Full mode:** about 1 s, 35 checks. **Quick mode** (`D133_CELER=1`):
  CIRCSE and LLCT, 30 checks, verdicts not decided. **Oracle:** a few
  seconds.
