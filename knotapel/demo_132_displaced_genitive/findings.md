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

The hypothesis tested here: **writers displace a genitive where the
word order itself leaves no nested alternative**, so that no other
noun could take it with a non-crossing arc. A nesting-first reader
(D131's R_P) is then never misled: the nested reading it would prefer
does not exist.

Everything is counted exactly over the gold trees of six Latin UD
treebanks, headline without punctuation (D130's convention). The tree
measures are the house library `dependentia`; the reader is `conllu`;
exact rationals are `fractio`. The frozen copy is `demo-snapshot.c`:
one file that compiles alone; its banner gives the clang line and
commit.

**Both registered predictions HOLD** (G1 in six of six treebanks, G2 in
five of six). The counts are small - 5 to 80 displaced genitives per
treebank - and every number below should be read with its N.

## Definitions

- **Genitive arc:** relation nmod (any subtype) with `Case=Gen` on the
  dependent. All six treebanks label genitive attributes this way (task
  0, totals only).
- **Displaced:** the arc is per-arc non-projective (a word strictly
  inside its span is not dominated by its head; `dependentia_arcus`).
- **Candidate host:** a NOUN or PROPN other than the true head and not
  in the genitive's own subtree.
- **Nested alternative:** a candidate host whose arc to the genitive
  would be projective, with the genitive (and its subtree) re-hung
  under it.
- **Safe:** displaced, with no nested alternative.
- **Positional null:** the genitive alone (its dependents stay attached)
  is moved through every position of its sentence, the other words
  keeping their order. Among the placements where its true arc is still
  displaced, the share that are safe. E is the sum of these shares over
  the displaced genitives: the null's expected safe count. S is the
  observed safe count, N the number displaced.
- **Adjective contrast** (reported only): amod arcs with an ADJ
  dependent carrying Case, Gender and Number; candidate hosts either the
  nouns that AGREE in all three, or every noun.

The plan walked the measure through hand sentences before registering
(D131's lesson): *regis puella filiam amat* is displaced and UNSAFE
(*puella* can host *regis* with an adjacent arc), *regis amat puella
filiam* is displaced and SAFE (every other host would cross the verb),
each with null share 1/2. These are checks in the demo, with a fifth
tree for the genitive's own dependent moving with it.

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

   The true head is a NOUN or PROPN in 88.6% to 99.5% of genitive arcs
   (the rest: partitives after numerals, pronouns, adjectives).
2. **G1 HOLDS: more displaced genitives are safe than chance placement
   gives, in all six treebanks. G2 HOLDS: most are safe, in five of
   six.**

   | treebank | N | S / N | E / N | P(null >= S) |
   |---|---|---|---|---|
   | CIRCSE | 80 | 45.00% | 35.27% | 0.027751 |
   | LLCT | 5 | 60.00% | 10.94% | 0.008446 |
   | PROIEL | 44 | 65.91% | 54.58% | 0.036797 |
   | Perseus | 42 | 64.29% | 45.28% | 0.003280 |
   | ITTB | 71 | 66.20% | 45.38% | 0.000001 |
   | UDante | 31 | 74.19% | 50.54% | 0.001644 |
   | Perseus prose | 6 | 66.67% | 53.25% | 0.383900 |
   | Perseus verse | 36 | 63.89% | 43.95% | 0.004141 |

   - S exceeds E everywhere, by 9.7 (CIRCSE) to 23.7 (UDante)
     percentage points (LLCT by 49, on five genitives). G2 fails only
     in CIRCSE (45.00%).
   - The last column is descriptive, not registered: under the null
     each displaced genitive is safe independently with its own share,
     so the safe count is Poisson-binomial; the exact probability of a
     count at least S is below 0.05 in every treebank alone (Perseus's
     six prose genitives cannot say anything). The independence of
     genitives within a sentence or text is the null's assumption.
   - The noun-headed subset gives the same direction in every row
     (e.g. ITTB 68.85% against 45.84%, CIRCSE 44.30% against 34.45%).
3. **The unsafe ones mostly have exactly one competitor.** Of the 108
   unsafe displaced genitives, 86 have one nested alternative host, 18
   two, 4 three, none more. CIRCSE contributes 44 of them (35 with
   one).
4. **The adjective contrast (reported only): agreement does most of
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
     the sentence at all, so safety is automatic there (the degeneracy
     found in task 0).
   - Ignoring agreement, the prose treebanks (PROIEL, ITTB, UDante) place
     displaced adjectives positionally safe far above chance too; the
     verse (CIRCSE, Perseus) sits near chance; the charters' 68
     displaced adjectives are never positionally safe, and are safe by
     agreement in 67.
5. **The picture, measured. [interpretation]**
   - D131 and D132 together suggest two resources that make a displaced
     word recoverable: AGREEMENT (a head label) for adjectives, and
     POSITIONAL SAFETY (no nested competitor, as when the verb stands
     between the genitive and every other noun) for genitives, which
     agreement cannot help.
   - Writers seem to use positional safety exactly where morphology
     cannot pay: displaced genitives are safe well above chance. Verse
     takes the most risk (CIRCSE 45% safe, the one G2 failure); the
     charters avoid displacing genitives altogether (0.2%).
   - Prose writers make even displaced adjectives positionally safe,
     beyond what agreement needs; verse leans on agreement alone. That
     contrast was not registered and rests on small N.

## Registered predictions

Registered in plan.md before any measure ran (plan approved 0392a892).
The rules never changed.

| | rule | verdict |
|---|---|---|
| G1 | in at least five of six Latin treebanks, S > E | **HOLDS** in six of six |
| G2 | in at least five of six, S / N >= 1/2 | **HOLDS** in five of six (CIRCSE 45.00% fails) |

**Disclosure.**
- D130's nmod shares (Part C) were seen before registering, and the
  plan says so. The genitive split, every host count, S, E and the
  adjective contrast were not.
- In T1 the quick mode (CIRCSE and LLCT only) was run moments before
  the full run that decided the verdicts, on the same code; nothing
  changed between the two runs.
- The tail probabilities were added in T2, after the verdicts, as a
  description of the small-N question raised in task 0; they decide
  nothing.

## Checks (26 full, 22 quick)

- **Reader:** sentences, words and ranges of the repo files = FONTES.md;
  every sentence a sound tree.
- **The fast arc test** (a per-sentence dominance matrix; moving a word
  changes positions, not dominance; re-hanging the genitive adds its
  subtree to the host's dominance) == `dependentia_arcus` on every arc
  of every headline tree (105,928).
- **The null contains the observation:** every displaced genitive (273),
  placed at its own position, is displaced with its own verdict.
- **Reversal:** reversing a sentence keeps every verdict and every null
  share.
- **Hand trees (6):** the plan's walk-through (a)-(d) and (e), a genitive
  whose own dependent sits between it and a host.
- **Rules:** S > E strict (including fractional E), S / N = 1/2 at its
  threshold, five of six against four of six.
- **Pins from `oracle_d132.py`** (its own reader and PUNCT removal,
  non-projectivity by ancestor walks, alternatives by re-attaching and
  re-testing, the null by REBUILDING each moved sentence,
  `fractions.Fraction`): every count of every row, E as an exact
  fraction (registered, noun-headed, adjectives filtered and
  unfiltered), the tail rounded to 1e-6.

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
- **Not red, and why:** the span endpoints (< vs <=) are equivalent, as
  only the arc's own words sit there; excluding the true head from the
  hosts is logically redundant (a displaced arc is never a nested
  alternative); excluding an adjective's own subtree from the
  degenerate test is idle on THIS data (no agreeing noun sits in a
  displaced adjective's subtree), not logically equivalent.

## Not covered

- **Why a placement is safe.** The hand sentences suggest the verb
  between the genitive and the other nouns; the demo does not measure
  which word blocks each alternative.
- **Meaning and frequency.** "Safe" is relative to a nesting-first
  reader over these trees; the 108 unsafe displaced genitives must be
  recovered by something else, which is not modelled.
- **The null's realism.** Moving one word through every position makes
  some unnatural orders; a null that keeps phrase structure (e.g.
  moving whole subtrees, or only to positions attested for genitives)
  would be stricter. Genitives are treated as independent.
- **Annotation conventions** across treebanks; pronouns as hosts;
  training splits (test splits only, LLCT dev + test).
- **Statistics:** the counts are small; the tails describe them, a
  registered test with a stated alpha would decide.

## Run

```text
./bin/aedilis knotapel/demo_132_displaced_genitive/main.c &&
  bash build/aedilis/main/struere.sh &&
  D132_UD_NC=<dir with the NC .conllu files> ./build/aedilis/main/main
python3 -I knotapel/demo_132_displaced_genitive/oracle_d132.py [--celer] [NC_DIR]
```

- **Full mode:** 0.2 s, 26 checks. Without `D132_UD_NC`, or with the NC
  files absent, their rows are reported ABSENT and the verdicts are not
  decided.
- **Quick mode** (`D132_CELER=1`): CIRCSE and LLCT only, 22 checks.
- **Oracle:** about 2 s.
