# Demo 133: labels buy distance (plan)

Status: approved by Fran (2026-10-19), v2 after a DESIGN REVIEW
(consiliarius-d133, before any data; its findings are summarised in
"Design review" below). "Open for Fran" decided as proposed: (1) L1,
L1u and L3 registered, L2 descriptive; (2) a design review before
approval becomes the STANDARD step for knotapel demos; (3) folder
`knotapel/demo_133_labels_distance/`. Follows D132 (KC 59);
context brainstorm sections XIII-XV (umbrae), D131 (KC 58).

## Question

D131 and D132 suggest two ways a displaced word stays recoverable: a
LABEL (agreement repeats the head's features, so the head can be found
by content: adjectives) or CLOSENESS (a genitive's ending names no head;
displaced genitives are few and short, KC 59.3.1.1).

In umbra terms (brainstorm XV): an open slot that carries a label can
WAIT, because the reader can find its head again by its features; an
unlabelled one must close soon. Hence:

1. Do displaced adjectives (labelled) wait longer - more foreign words
   between them and their head - than displaced genitives (unlabelled)?
2. **Within adjectives** (the discriminating test): do adjectives whose
   label is UNIQUE in the sentence (no other agreeing noun) wait longer
   than adjectives whose label is SHARED with another noun? If labels
   are what buy distance, the label must tell the candidates apart.

A kind-level difference (question 1) is open to cheaper explanations
(construction mix, clause span, direction, particles); the within-kind
dose-response (question 2) is what separates "labels permit" from
"something else demands" (design review, H3).

## What is counted

Same six Latin treebanks, reader and headline trees as D132 (UD, PUNCT
removed, `conllu`, `dependentia`); NC treebanks from main's
`oratio/build/ud` (`D133_UD_NC`), aggregates only.

- **Populations (registered):**
  - genitives: nmod with Case=Gen, head NOUN or PROPN, PronType not Rel
    or Int (relatives are fronted by syntax, not chosen);
  - adjectives: amod with an ADJ dependent carrying Case, Gender and
    Number, head NOUN or PROPN.
  - Excluded arcs are counted and reported per treebank: relative and
    interrogative genitives, pronoun genitives (PronType=Prs, kept but
    reported separately), non-noun heads, feature-incomplete adjectives
    (PROIEL 58 of 421).
- **Displaced:** per-arc non-projective (as D132). A check: over D132's
  full populations (no head or PronType filter), D133's displaced
  counts reproduce D132's (genitives 80, 5, 44, 42, 71, 31; adjectives
  445, 68, 28, 178, 78, 34).
- **Operand F (foreign words):** the words strictly inside the arc's
  span that its head does NOT dominate - exactly the witnesses of its
  non-projectivity, the words an open umbra waits through outside its
  own phrase (F >= 1 for every displaced arc). Raw distance |h - d|
  counts the dependent's own phrase and the head's other dependents, so
  it is reported, not decided on (design review, M5).
  - Statistics use min(F, 10) (outliers; keeps the exact tests small).
- **Direction:** dependent-first (the dependent precedes its head: it
  opens an umbra) or head-first (retrieval, no umbra).
- **Length bin** of the sentence (punct-free words): <= 10, 11-20, > 20.
- **Strata:** treebank x direction x length bin; Perseus split into
  prose and verse (by work code, as D130).
- **Unique / shared label** (adjectives): unique when no NOUN or PROPN in
  the sentence, other than the true head and outside the adjective's
  subtree, agrees with it in Case, Gender and Number; agreement by SET
  INTERSECTION of the feature values (multi-valued Gender such as
  "Fem,Masc" agrees with either).
- **Function-word flag:** an arc whose foreign words are all PART, CCONJ,
  ADV or a second-position particle (a phrase split by *autem*, *enim*:
  not a chosen displacement).

## The exact test

For a group X within a population split into strata: in each stratum s
with n_s arcs, k_s of them in X, the null permutes the group labels
within the stratum (every k_s-subset equally likely). The statistic T
is the total of min(F, 10) over X. Its exact distribution is the
convolution over strata of each stratum's subset-sum distribution
(dynamic programming; counts as exact big integers, `magnus`; values
<= 10 keep the sums small). p = P(T >= t_obs), inclusive, an exact
rational. E_perm[T] = sum over strata of k_s x (mean of min(F, 10) in
s), exact.

## Registered before running

(Decided per the definitions above; exact integers and rationals.)

- **L1 (labels buy distance, kinds):** displaced adjectives against
  displaced genitives, both populations, strata as above.
  - (a) pooled over all treebanks: p <= 1/20 for the adjectives' T;
  - AND (b) in at least four of the five treebanks other than LLCT
    (LLCT's five displaced genitives are about one formula), the
    treebank's T > its E_perm[T].
  - (a) has chance <= 1/20 by construction; (b)'s chance rate under the
    same permutation null is computed in the run and reported.
- **L1u (the umbra version):** the same pooled test (a) on DEPENDENT-
  FIRST arcs only: p <= 1/20. Head-first arcs are reported, not decided.
- **L3 (the discriminating test):** among displaced adjectives, the
  UNIQUE-label group's T against the SHARED-label group, strata
  treebank x length bin, pooled over the five treebanks other than LLCT
  (one unique arc there): p <= 1/20.

**Not registered (descriptive):** the adjectives' excess beyond distance
(D132's distance-limited null), beside the genitives'. Design review H1:
from D132's published numbers alone, a registered comparison would hold
in LLCT always and almost surely in CIRCSE and Perseus, against genitive
values that are themselves noise (tails 0.11-0.29), and the null's
resolution depends on distance (so it is coupled to L1). Reported with
N, tails, points AND log-odds, and the threshold table:

| treebank | adjective every-noun S/N (D132) | genitive excess (D132) | adjective E_near/N would have to exceed | registered E/N (D132) |
|---|---|---|---|---|
| CIRCSE | 35.51 | 2.31 | 33.20 | 32.93 |
| LLCT | 0.00 | 30.48 | -30.48 (always) | 2.17 |
| PROIEL | 67.86 | 5.11 | 62.75 | 44.61 |
| Perseus | 47.19 | 6.54 | 40.65 | 41.80 |
| ITTB | 74.36 | 3.71 | 70.65 | 49.24 |
| UDante | 76.47 | 5.64 | 70.83 | 47.85 |

**Sensitivities (reported, not decided):** one arc per sentence per kind
(the first); function-word-flagged arcs removed; raw distance in place of
F; Perseus as one stratum; the mid-rank percentile of each arc's F among
its own move-alone null's displaced placements (a shape-free operand);
nested arcs' F (all zero by definition) and raw distance by kind as the
baseline.

## Already seen (disclosed before registering)

- D132 printed, for GENITIVES: N, S, E, the distance-limited E and tails,
  the nearest-alternative distances; and established that displaced
  genitives are SHORT (its Result 3). L1's genitive side is
  qualitatively known, and the hypothesis was built on it.
- D132 printed, for ADJECTIVES: N displaced, the safe counts, the
  registered-null E, and the unique/shared group SIZES (70-82% unique;
  LLCT 1.5%) - L3's group sizes.
- The design review computed totals not split by displacement (relative
  genitives, particles at position 2, sentence lengths, feature
  completeness) and synthetic chance rates; nothing the predictions are
  decided on.
- NOT seen: any arc's F or distance, by kind or by label group; the
  adjectives' distance-limited null.

## Hand walk-through (design review; constructed sentences, D132's definitions)

Positions from 1; every-noun hosts NOUN/PROPN. Distance / F; direction;
every-noun verdict; null share registered / distance-limited;
agreement-filtered verdict.

- **A1** *magnam puer in silva vidit ursam* (magnam -> ursam): 5 / 4;
  dependent-first; every-noun UNSAFE (puer); 0/4, 0/4; filtered SAFE.
  Far, label unique: agreement resolves what every-noun safety calls
  unsafe.
- **A2** *magnam habet domum*: 2 / 1; dependent-first; SAFE, no host;
  1/1, 1/1. The degenerate short case.
- **A3** *magna autem pars militum fugit*: 2 / 1 (autem); SAFE; 2/2,
  1/1. A particle split: function-word flag.
- **A4** *reginam amat puella bonam* (bonam -> reginam): 3 / 2;
  HEAD-first; every-noun UNSAFE; filtered SAFE. Retrieval, no umbra.
- **A5** *bonam reginam amat puellam* (bonam -> puellam): 3 / 2;
  dependent-first; UNSAFE both ways (reginam agrees). A SHARED label
  (L3).
- **A6** *magnam parvus amat puer urbem* (golden line abVAB): outer
  magnam -> urbem 4 / 3, SAFE (the verb blocks puer), 2/3, 2/3; inner
  parvus -> puer 2 / 1, SAFE, 2/3 vs limited 1/2 (the limited null can
  fall below the registered one).
- **G1** *regis vidi filiam in urbe*: 2 / 1; SAFE (the verb blocks
  urbe); 1/3, 1/2.
- **G2** *rex cuius heri filiam vidi venit* (cuius -> filiam): 2 / 1;
  UNSAFE (rex); a relative genitive - excluded from the population.
- **G3** *Caesaris in castra venerunt legati* (Caesaris -> legati): 4 /
  3; UNSAFE (castra: "into Caesar's camp"); a long risky genitive, which
  the hypothesis says should be rare.
- **G4** *regis magni amat filiam* (magni -> regis): distance 3, F 1 -
  raw distance counts the genitive's own phrase, F does not.
- **G5** *filiam et filium regis vidi* (regis -> filiam, first conjunct):
  distance 3, NOT displaced - coordination stretches nested distances.

These are T1's hand-tree checks (distance, F, direction, flags, verdicts,
null shares).

## Checks

- D132's reader and tree checks; D133's displaced counts over D132's
  full populations = D132's (above).
- F two ways (the witness count against a direct recount by ancestor
  walks) on every displaced arc; F >= 1 on every displaced arc and 0 on
  every nested one.
- The exact test against brute force (every k-subset enumerated) on
  small constructed strata; E_perm against the mean of the exact
  distribution.
- The hand trees above.
- Totals, p-values and E_perm pinned from an independent Python oracle;
  the rules on constructed totals at each threshold (p = 1/20 holds,
  p just above fails; four of five holds, three fails; T = E fails).
- The L2 thresholds pinned as D132's exact fractions.
- Plants via `silva.planta`.

## Design review (consiliarius-d133, 2026-10-19, before any data)

H 3, M 4, L 2, all acted on: L2 made descriptive (H1); strata, the F
operand and the stricter populations against the cheaper explanations
of L1 - relatives, particles, direction, sentence length, odd heads,
Perseus's two genres, outliers, LLCT (H2); the within-adjective
dose-response L3 added as the discriminating test (H3); the sign rule's
chance (0.12-0.16 by simulation, not 0.109) replaced by an exact pooled
test plus a per-treebank consistency clause, the test fully specified
(M4); F instead of raw distance (M5); excess currencies and the
distance-limited null's coarseness at distance 2 (M6, descriptive only
now); PROIEL's feature-incomplete adjectives and set-intersection
agreement (M7); a wider disclosure (L8); exact thresholds and a
reproduction check (L9). Its forecast of the most likely failure: L1
passing for the wrong reason (clause span, construction mix) - which is
why L3 is the test the findings will lean on.

## Tasks (one per turn, Fran's go-ahead each)

1. **T1:** the demo: populations, F, direction, strata, the exact
   stratified test, L1 / L1u / L3, the descriptive excess and
   sensitivities; checks (hand trees, brute-force test, D132
   reproduction); plants.
2. **T2:** the oracle; pins; rules on constructed totals; plants.
3. **T3:** findings, claims (beside 59), ledger, archive.
4. **T4:** independent review, then fixes.

## Open for Fran

1. The registered tests: L1 (pooled exact test AND four of the five
   non-LLCT treebanks), L1u (dependent-first), L3 (unique vs shared
   label, the discriminating one); L2 descriptive only.
2. Whether a design review before approval should become the standard
   step for knotapel demos.
3. Folder `knotapel/demo_133_labels_distance/`.
