# Demo 132: Real Latin and the Displaced Genitive — Findings

Proposal N15 (ledger …PEYPY4). Plan: `plan.md` (approved 2026-10-19;
its task notes record each task's decisions). Context: D131 (KC 58),
D130 (KC 57), `knotapel/brainstorm-skeleton-settling.md` section XV.

D131 found, in a toy world, that endings cannot place a displaced
GENITIVE. A genitive's ending names its role (possessor), not its head;
it agrees with nothing, so any noun could host it. A displaced
ADJECTIVE repeats its head's case, gender and number, so agreement
names the head. Yet real Latin displaces genitives: D130's Part C table
has nmod over-represented among non-projective arcs in five of six
Latin treebanks. How is a displaced genitive recovered?

The hypothesis tested here: **writers displace a genitive where no
other noun could take it with a per-arc projective attachment**, the
rest of the gold tree fixed. (The plan's prose said "non-crossing";
the registered definition, and every count here, is per-arc
projectivity. "Safe" is a property of that one attachment; it does not
mean that D131's nesting-first reader R_P, which judges whole trees, is
never misled - see Result 6.)

Everything is counted exactly over the gold trees of six Latin UD
treebanks, headline without punctuation (D130's convention). The tree
measures are the house library `dependentia`; the reader is `conllu`;
exact rationals are `fractio`. The frozen copy is `demo-snapshot.c`:
one file that compiles alone; its banner gives the clang line and
commit. The demo had an independent review (T4, below) whose own
oracle re-derived every number, zero mismatches.

**Both registered predictions HOLD** (G1 in six of six treebanks, G2 in
five of six). **But the effect is mostly explained by distance**: real
displacements are short, and short placements are safer. Against a
null that keeps the genitive no farther from its head than it is, the
safe share still exceeds the expected one in all six treebanks, but by
2 to 7 points (LLCT 30), and no treebank's tail is below 0.05. What is
shown is "displaced genitives are short, and short is safe", not that
writers choose the safe placement. The counts are small throughout (5
to 80 displaced genitives per treebank).

## Definitions

- **Genitive arc:** relation nmod (any subtype) with `Case=Gen` on the
  dependent. All six treebanks label genitive attributes this way (task
  0, totals only).
- **Displaced:** the arc is per-arc non-projective (a word strictly
  inside its span is not dominated by its head; `dependentia_arcus`).
- **Candidate host:** a NOUN or PROPN other than the true head and not
  in the genitive's own subtree.
- **Nested alternative:** a candidate host whose arc to the genitive
  would be per-arc projective, with the genitive (and its subtree)
  re-hung under it and the rest of the tree fixed.
- **Safe:** displaced, with no nested alternative.
- **Positional null (registered):** the genitive alone (its dependents
  stay attached) is moved through every position of its sentence, the
  other words keeping their order. Among the placements where its true
  arc is still displaced, the share that are safe. E is the sum of these
  shares: the null's expected safe count. S is the observed safe count,
  N the number displaced.
- **Adjective contrast** (reported only): amod arcs with an ADJ
  dependent carrying Case, Gender and Number; candidate hosts either the
  nouns that AGREE in all three, or every noun.

The plan walked the measure through hand sentences before registering
(D131's lesson): *regis puella filiam amat* is displaced and UNSAFE
(*puella* can host *regis* with an adjacent arc), *regis amat puella
filiam* is displaced and SAFE (every other host would cross the verb),
each with null share 1/2. These are checks in the demo, with a fifth
tree for the genitive's own dependent moving with it. The walk-through
tested the MEASURE; it could not show that the null ignored distance.

## Result

1. **Displaced genitives are rare, and in the charters almost absent.**

   | treebank | genitive arcs | displaced (N) | share displaced |
   |---|---|---|---|
   | CIRCSE (Seneca, verse) | 477 | 80 | 16.77% |
   | LLCT (charters) | 2,382 | 5 | 0.21% |
   | PROIEL | 458 | 44 | 9.61% |
   | Perseus | 255 | 42 | 16.47% |
   | ITTB (Aquinas) | 1,098 | 71 | 6.47% |
   | UDante | 438 | 31 | 7.08% |
   | Perseus prose / verse | 117 / 138 | 6 / 36 | 5.13% / 26.09% |

   The true head is a NOUN or PROPN in 88.6% to 99.5% of genitive arcs;
   the rest are ADJ, DET, PRON, NUM, VERB, ADV and one CCONJ (partitives
   after numerals and pronouns among them).
2. **G1 HOLDS (S > E in six of six) and G2 HOLDS (S / N >= 1/2 in five
   of six), under the registered null.**

   | treebank | N | S / N | E / N | P(null >= S) |
   |---|---|---|---|---|
   | CIRCSE | 80 | 45.00% | 35.27% | 0.0278 |
   | LLCT | 5 | 60.00% | 10.94% | 0.0084 |
   | PROIEL | 44 | 65.91% | 54.58% | 0.0368 |
   | Perseus | 42 | 64.29% | 45.28% | 0.0033 |
   | ITTB | 71 | 66.20% | 45.38% | 1.2e-6 |
   | UDante | 31 | 74.19% | 50.54% | 0.0016 |
   | Perseus prose | 6 | 66.67% | 53.25% | 0.3839 |
   | Perseus verse | 36 | 63.89% | 43.95% | 0.0041 |

   - S exceeds E everywhere, by 9.7 (CIRCSE) to 23.7 (UDante) points
     (LLCT by 49, on five genitives). G2 fails only in CIRCSE (45.00%).
   - **How strong the rules are** (computed by the T4 review under the
     registered null, treebanks independent; not pinned): each
     treebank's S > E is close to a coin toss under the null (P 0.42 to
     0.53), so G1 holds by chance with probability 0.075, and six of
     six with 0.009. G2 is far stricter: 0.00018. G1 alone is weak
     evidence; the "more than chance" reading rested on the tails.
   - The tails are descriptive, added after the verdicts, and assume
     displaced genitives independent; several sentences hold two or
     more, and LLCT's are formulas (below).
   - **G2 turns on one LLCT genitive:** S = 3 of N = 5; one fewer safe
     and LLCT is 40%, G2 four of six. LLCT's five displaced genitives
     are about four instances of one charter formula (its two unsafe
     ones are near-identical). Every other passing treebank has a
     margin of at least six genitives.
   - The noun-headed subset gives the same direction in every row
     (e.g. ITTB 68.85% against 45.84%, CIRCSE 44.30% against 34.45%).
3. **Under stricter nulls (descriptive, T4 review; pinned):**

   | treebank | S / N | E / N, no farther than observed | its tail | E / N, whole subtree moved |
   |---|---|---|---|---|
   | CIRCSE | 45.00% | 42.69% | 0.291 | 35.31% |
   | LLCT | 60.00% | 29.52% | 0.136 | 8.82% |
   | PROIEL | 65.91% | 60.80% | 0.136 | 53.13% |
   | Perseus | 64.29% | 57.75% | 0.141 | 45.48% |
   | ITTB | 66.20% | 62.49% | 0.107 | 45.44% |
   | UDante | 74.19% | 68.55% | 0.234 | 49.73% |
   | Perseus prose | 66.67% | 83.33% | 1.000 | 53.25% |
   | Perseus verse | 63.89% | 53.49% | 0.047 | 44.19% |

   - Moving the genitive's whole subtree instead of the genitive alone
     changes almost nothing.
   - Keeping the genitive no farther from its head than observed raises
     E / N most of the way to S / N: S > E survives in all six
     treebanks by 2 to 7 points (LLCT 30), with tails of 0.11 to 0.29.
     Only Perseus's verse stays below 0.05 (0.047).
   - So the registered null's margin is mostly DISTANCE: displaced
     genitives sit close to their heads, and close placements are
     safer. A deliberate choice of the safe placement is not shown.
4. **Some safe genitives are trivially safe.** In 34 of the 165 safe
   displaced genitives no candidate host exists at all (CIRCSE 5, LLCT
   0, PROIEL 9, Perseus 3, ITTB 15, UDante 2): no other noun, so safe
   with null share 1. G1 is unaffected; without them, S / N is CIRCSE
   41.3%, LLCT 60.0%, PROIEL 57.1%, Perseus 61.5%, ITTB 57.1%, UDante
   72.4%, and G2 still holds in five of six.
5. **The unsafe ones mostly have one, near competitor.** Of the 108
   unsafe displaced genitives, 86 have one nested alternative host, 18
   two, 4 three. The nearest alternative is adjacent in 57, two words
   away in 20, three in 10, four in 4, five to nine in 12, ten or more
   in 5 (pinned).
   - Not every alternative is a plausible misreading (T4 review,
     counted by its scripts, not pinned): in 19 of the 108 the
     alternative is the sentence's root, a nominal predicate, which
     dominates everything and so is a nested alternative wherever the
     genitive sits; in 35 only ANCESTORS of the genitive are
     alternatives; a few are 31 to 44 words away. So "unsafe"
     over-counts real misreadings; in the other direction, PRON, ADJ
     and DET hosts are excluded, so "safe" may over-count. S is a count
     under the noun-host model, not reader behaviour.
6. **"Safe" is not "R_P is never misled".** D131's R_P keeps the
   analyses that are projective as WHOLE trees. 84 of the 165 safe
   genitives sit in gold trees with another non-projective arc (all
   five in LLCT), and of the 108 unsafe ones only 33 have an alternative
   that makes the whole re-hung tree projective (T4 review's scripts).
   With "non-crossing" alternatives (the alternative arc crosses no arc
   of the re-hung tree) instead of per-arc projective ones, S would be
   CIRCSE 42, LLCT 3, PROIEL 29, Perseus 25, ITTB 42, UDante 22 (CIRCSE
   would then pass G2 at 52.5%). The verdicts follow the registered,
   per-arc definition.
7. **The adjective contrast (reported only): agreement does most of
   the work, and is often not even needed.**

   | treebank | displaced adjectives | no other agreeing noun | safe, agreeing hosts (S / N vs E / N) | safe, every noun (S / N vs E / N) |
   |---|---|---|---|---|
   | CIRCSE | 445 | 70.11% | 98.43% vs 95.54% | 35.51% vs 32.93% |
   | LLCT | 68 | 1.47% | 98.53% vs 83.84% | 0.00% vs 2.17% |
   | PROIEL | 28 | 82.14% | 100.00% vs 97.06% | 67.86% vs 44.61% |
   | Perseus | 178 | 79.78% | 98.31% vs 96.06% | 47.19% vs 41.80% |
   | ITTB | 78 | 79.49% | 93.59% vs 89.34% | 74.36% vs 49.24% |
   | UDante | 34 | 70.59% | 100.00% vs 97.24% | 76.47% vs 47.85% |

   - With agreement filtering almost every displaced adjective is safe,
     but in 70-82% of them (LLCT 1.5%) no other agreeing noun exists in
     the sentence at all, so safety is automatic there.
   - Ignoring agreement, PROIEL, ITTB and UDante place displaced
     adjectives positionally safe far above chance; CIRCSE and Perseus's
     verse (45.27% vs 41.75%) sit near chance; the charters' 68 are
     never positionally safe, and are safe by agreement in 67. (Not
     registered, small N, and the distance confound of Result 3 was not
     checked for adjectives.)
8. **What blocks the alternatives** (T4 review's scripts, not pinned):
   of the 131 safe genitives that have candidate hosts, a VERB or AUX is
   among the blocking words of every alternative in 93 (CIRCSE 22 of
   31, LLCT 2/3, PROIEL 16/20, Perseus 16/24, ITTB 23/32, UDante
   14/21); the ROOT blocks every alternative in 41. Only 6 of the 273
   displaced genitives are displaced solely because they cover the root
   (their arcs cross nothing); the plan's safe example (c) is of that
   rare kind.
9. **The picture, measured. [interpretation]**
   - D131 and D132 together suggest two resources that make a displaced
     word recoverable: AGREEMENT (a head label) for adjectives; for
     genitives, which agreement cannot help, SHORT displacement, which
     leaves few other nouns in reach of a nested reading - often with a
     verb in between (Result 8).
   - Whether writers go beyond short displacement and choose the safe
     placement is not shown (Result 3).
   - CIRCSE takes the most risk (45% safe, the one G2 failure). Verse
     displaces genitives more often (Perseus verse 26.1% against prose
     5.1%), though Perseus's verse is no less safe than the prose
     treebanks. The charters avoid displacing genitives altogether
     (0.2%).

## Registered predictions

Registered in plan.md before any measure ran (plan approved 0392a892).
The rules never changed (checked by the T4 review against the git
history).

| | rule | verdict |
|---|---|---|
| G1 | in at least five of six Latin treebanks, S > E | **HOLDS** in six of six (a sign rule chance meets with P 0.075) |
| G2 | in at least five of six, S / N >= 1/2 | **HOLDS** in five of six (CIRCSE 45.00% fails; turns on one LLCT genitive) |

**Disclosure.**
- D130's nmod shares (Part C) were seen before registering, and the
  plan says so. The genitive split, every host count, S, E and the
  adjective contrast were not.
- In T1 the quick mode (CIRCSE and LLCT only) was run moments before
  the full run that decided the verdicts, on the same code; nothing
  changed between the two runs.
- The tail probabilities (T2) and the stricter nulls (T4) were added
  after the verdicts; they decide nothing.

**What the registration got wrong** (Claude's design, approved as
written): the null was not matched on the obvious confound, displacement
DISTANCE. The hand walk-through tests a measure, not a null; a null
needs its own question before registering: "what else, besides the
hypothesis, makes the observed placements special?"

## Checks (27 full, 23 quick)

- **Reader:** sentences, words and ranges of the repo files = FONTES.md;
  every sentence a sound tree.
- **The fast arc test** (a per-sentence dominance matrix; moving a word
  changes positions, not dominance; re-hanging the genitive adds its
  subtree to the host's dominance) == `dependentia_arcus` on every arc
  of every headline tree (105,928). It covers identity positions
  without re-hanging; the re-hung path and moved positions are covered
  by hand tree (e) and by the pins.
- **The null contains the observation:** every displaced genitive (273),
  placed at its own position, is displaced with its own verdict.
- **Reversal**, a symmetry sanity check (the code is symmetric by
  construction): reversing a sentence keeps every verdict and null
  share.
- **Hand trees (6):** the plan's walk-through (a)-(d) and (e).
- **Rules:** S > E strict (including fractional E), S / N = 1/2 at its
  threshold, five of six against four of six.
- **Pins from `oracle_d132.py`** (its own reader and PUNCT removal,
  non-projectivity by ancestor walks, alternatives by re-attaching and
  re-testing, every null by REBUILDING the moved sentence,
  `fractions.Fraction`): every count of every row; E as an exact
  fraction (registered, noun-headed, adjectives filtered and
  unfiltered); the tail; and the T4 descriptives (distance-limited E
  and tail, whole-subtree E, genitives with no host, nearest-
  alternative distances). The oracle has the same author and shares
  the definitional choices; the T4 review's oracle, with different
  algorithms, reproduces every pin.

## Plants

All through `silva.planta`.
- **T1 (8 red):** the fast test's dominance; the null's last position;
  the own placement; the re-hung subtree (after hand tree (e) was
  added); S > E strictness; the G2 threshold; G1's count; E's
  denominator.
- **T2 (10 red):** genitive read as accusative; hosts NOUN only; PUNCT
  kept; the tail's p and 1 - p, and its strict bound; adjective gender;
  the Perseus code; the noun head read from the genitive; the
  noun-headed null; the degenerate count.
- **T4 fixes (5 red):** the distance-limited null excluding its own
  distance; the subtree null's last gap; the no-host count; the
  distance buckets; the distance-limited tail reading the registered
  shares.
- **Not red, and why:** the span endpoints (< vs <=) are equivalent, as
  only the arc's own words sit there; excluding the true head from the
  hosts is logically redundant (a displaced arc is never a nested
  alternative); excluding an adjective's own subtree from the
  degenerate test is idle on THIS data (no agreeing noun sits in a
  displaced adjective's subtree), not logically equivalent.

## Review (T4, 2026-10-19)

An independent reviewer (recensor-d132) compiled the snapshot in its
own scratch directory and wrote its own oracle: descendant bitmasks
rebuilt from the head array, an ANALYTIC null (prefix counts per host
answer every slot in O(1)), binomial grouping for the tail; plus
descriptive scripts (stricter nulls, blockers, host kinds). It
re-derived every number in this file, in KC 59 and in the plan's
notes, and the hand trees on paper. **Zero mismatches; no rule changed;
both verdicts follow; no H finding.** M 6, L 7, N 5, all acted on:

- **M1:** G1 is a sign rule that chance meets with P 0.075; the "more
  than chance" reading rested on the tails. Stated in Result 2; KC
  59.3.1 reworded, 59.3.obj.2.1 conceded.
- **M2:** the stricter nulls (Result 3): distance explains most of the
  margin. The headline and the interpretation changed; both nulls are
  now computed and pinned in the demo.
- **M3:** "safe" is not "R_P never misled", and "non-crossing" in the
  prose meant per-arc projective (Result 6, the opening).
- **M4:** G2 turns on one LLCT genitive (Result 2).
- **M5:** "verse takes the most risk" rested on CIRCSE alone, and the
  prose adjective claim ignored the charters (Results 7, 9).
- **M6:** the genitives with no candidate host (Result 4), now counted
  and pinned.
- **L1** the alternative-distance report the plan promised (Result 5,
  pinned); **L2** implausible alternatives (Result 5); **L3** the
  reversal check relabelled; **L4** the non-noun heads listed; **L5**
  the ITTB tail written 1.2e-6; **L6** Perseus's verse row cited;
  **L7** the output line count.
- **N1** what blocks the alternatives (Result 8); **N2** multi-valued
  Gender (CIRCSE 298 and PROIEL 236 tokens; set-intersection agreement
  would lower the adjective degenerate count by one in each, safe
  counts unchanged); **N3** independence (sentences with several
  displaced genitives: CIRCSE 8, PROIEL 3, Perseus 2); **N4** the pins'
  common author (Checks); **N5** the disclosure and history are
  accurate.

## Not covered

- **A registered test of the distance-matched hypothesis:** whether
  writers choose safe placements beyond short displacement needs a new
  registration against the distance-limited null, on data not used here.
- **Meaning and frequency.** The 108 unsafe displaced genitives, and the
  safe ones in trees with other crossings, are recovered by something
  not modelled here.
- **Stricter nulls for the adjective contrast**, and annotation
  conventions across treebanks; pronouns as hosts; training splits
  (test splits only, LLCT dev + test).

## Run

```text
./bin/aedilis knotapel/demo_132_displaced_genitive/main.c &&
  bash build/aedilis/main/struere.sh &&
  D132_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
python3 -I knotapel/demo_132_displaced_genitive/oracle_d132.py [--celer] [NC_DIR]
```

- **Full mode:** 0.2 s, 27 checks. Without `D132_UD_NC`, or with the NC
  files absent, their rows are reported ABSENT and the verdicts are not
  decided.
- **Quick mode** (`D132_CELER=1`): CIRCSE and LLCT only, 23 checks.
- **Oracle:** a few seconds.
