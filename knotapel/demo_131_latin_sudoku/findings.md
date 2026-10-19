# Demo 131: Toy Latin Sudoku — Does Morphology Pay for Crossing? — Findings

Proposal N14, slice 1 (ledger …9XK1EA). Plan: `plan.md` (approved
2026-10-19; its task notes record each task's decisions). Context:
`knotapel/brainstorm-skeleton-settling.md` sections XI, XIII and XIV;
D130 (KC 57).

D130 found that where real Latin crosses freely (verse), the displaced
word of a crossing usually agrees with its head, and that English,
without case labels, almost never crosses. The hypothesis drawn from
it: **morphology pays for crossing**. Case, gender and number endings
let a reader reconnect a displaced word, so the grammar can afford free
order; without them, order must carry the structure.

That cannot be tested on treebanks: there is one Latin, and it is
confounded with genre and annotation. It can be tested exactly on a
toy world small enough to enumerate, where the endings can be weakened
on purpose. And it must be tested against REAL syncretism: Latin
endings are ambiguous, so they do not always tell the candidates
apart.

1. How much ambiguity does LICENSING crossings cost a reader, with
   Latin's real endings, against ideal endings and against endings
   stripped of case (and of agreement)?
2. When a reader prefers nesting and meets a crossed sentence, how
   often does it settle on a wrong nested reading (a garden path)? Does
   that depend on whether the displaced word's ending is syncretic?

Everything is counted exactly, in integers, over every sentence of the
world. The tree measures are the house library `dependentia` (D130).
The frozen copy is `demo-snapshot.c`: one file that compiles alone; its
banner gives the clang line and commit. The demo had an independent
review (T4, below) that re-derived every number with its own oracle,
zero mismatches; its findings changed how the failures are read.

**Both registered predictions FAIL** (below). Q1 fails by a near-tie
that flips with sentence length. Q2 fails by a Simpson's paradox: in
each kind of displacement the registered direction holds, and only the
pooled split reverses. Descriptively (after the run): endings pay for
ADJECTIVAL hyperbaton, and cannot pay for a displaced GENITIVE.

## The world

Syntax only, no semantics ("donum puellam amat" is grammatical).

- **Lexicon:** nouns *puella* (1st, f), *dominus* (2nd, m), *donum*
  (2nd, n), *rex, regis* (3rd, m); adjectives *bonus* (1st/2nd) and
  *fortis* (3rd); verbs *amat / amant* (subject + object), *dat / dant*
  (subject + object + dative): four verb forms of two lemmas. Cases
  nominative, genitive, dative, accusative, ablative; no vocative or
  locative.
- **Endings:** Charles E. Bennett, *New Latin Grammar* (1895, 1908,
  1918; public domain; Project Gutenberg #15665), sections 20, 23, 32,
  63, 69, 101 and 127, macrons dropped as in the treebanks (which is
  where `-a` / `-ā` collapse). *fortis* takes Bennett's first-listed
  forms (acc. pl. *-is* omitted). The demo's table is checked against
  Whitaker's WORDS (`oratio/vocabularium/la/INFLECTS.LAT`, entries AGE
  X, FREQ A), read as data: equal in all 70 noun and *bonus* cells; for
  *fortis*, the table's form is in Whitaker's set in all 30 cells, and
  Whitaker's 8 other forms are listed in the demo. The table against
  Bennett's text itself was checked mechanically by the T4 review, not
  by the demo.
- **Slot frames:** the verb is the root and takes exactly one
  nominative subject agreeing in number, exactly one accusative object,
  for *dat / dant* exactly one dative, and any number of ablative
  adjuncts; a genitive depends on another noun (at most one per noun);
  an adjective depends on a noun of the same case, gender and number.
  No pro-drop. One clause only.
- **Sentences:** every analysis of up to five words, in every word
  order (task 0 chose five: six words alone would add 809.5M pairs,
  816.3M in all up to six, beyond an exhaustive Python oracle).

A **pair** is a string together with one of its analyses (a head and a
reading per position). Each analysis yields exactly one string at each
level, so the pairs are the SAME 6,752,640 at every level, and every
measure below has one denominator across levels.

### Morphology levels (only the surface forms change)

| level | what the forms carry | nominal forms | strings | max \|F(s)\| | mean \|F(s)\| over pairs |
|---|---|---|---|---|---|
| L1 ideal | every reading its own form (counterfactual) | 100 | 3,176,832 | 6 | 3.08 |
| L0 Latin | the real paradigms | 47 | 2,734,201 | 18 | 3.79 |
| L2 case-stripped | one form per gender and number (the nominative); agreement in gender and number kept | 17 | 296,312 | 156 | 67.22 |
| L3 bare | as L2, adjectives invariant (no agreement); nouns keep number | 10 | 169,760 | 156 | 70.28 |

Verbs agree in number at every level. L2 and L3 are derived from the
L0 table mechanically (a check). (Means rounded; an earlier version
truncated them to 3.07, 3.78 and 67.21.)

## Readers, cells, measures

- **F(s):** every analysis of s (crossings allowed). It does not depend
  on the order of the words (a check, on every string).
- **P(s):** those whose tree is projective under s's order (Kuhlmann
  and Nivre Def. 3, root arc included). **M(s):** those mildly
  non-projective (gap degree <= 1, well-nested).
- **R_P(s)** = P(s) if non-empty, else F(s): the reader that prefers
  nesting and accepts crossing only when nothing nested works. R_M
  likewise with M.
- **Cells** (Sudoku): per word, an ATTACHMENT cell (its head) and a
  READING cell (case, gender, number). A cell is forced in a set of
  analyses if it has one value there, open otherwise.
- **Misread:** a pair is misread by a reader if its analysis is not in
  the reader's set for its string.
- **A (licensing cost)**, over pairs whose tree is projective: the
  attachment cells open under F but forced under P, as a share of all
  their cells (every word's, the verb's included; the denominator is
  shared across levels, so this reading cannot move a verdict).
- **B (garden paths)**, over pairs whose tree is not projective: the
  share misread by R_P. Split by whether some DISPLACED word (the
  dependent of a non-projective arc: a word strictly inside its span is
  not dominated by its head) has a syncretic form (two or more readings
  at that level).

## Result

1. **Q1 FAILS, by a near-tie that flips with length.**

   | | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | A, five words | 6.51% | 6.46% | 7.30% | 7.31% |
   | A, four words | 5.13% | 5.14% | 6.11% | 7.05% |
   | A, up to five (registered) | 6.49% | 6.45% | 7.29% | 7.31% |
   | A numerator over 13,631,616 cells | 884,736 | 878,568 | 993,296 | 996,352 |

   - A(L1) <= A(L0) is false, by 6,168 cells: 0.045 percentage points
     (0.70% relative). Up to four words it is TRUE (8,192 <= 8,208), and
     Q1 holds there on every clause. The rest of the rule holds at five:
     A(L0) < A(L2) <= A(L3), and A(L0) - A(L1) <= A(L2) - A(L0) (Latin
     at least as close to ideal as to case-stripped).
   - **What A is made of.** A is a DIFFERENCE: the cells open under F
     minus those open under P. Syncretism opens cells under both:

     | open attachment cells, projective pairs | L1 | L0 | L2 | L3 |
     |---|---|---|---|---|
     | under F | 1,953,792 | 2,209,476 | 9,736,704 | 10,017,792 |
     | under P | 1,069,056 | 1,330,908 | 8,743,408 | 9,021,440 |

     That makes the difference small and its sign between L0 and L1
     fragile, but it does not decide the sign: up to four words the
     same pattern holds (under F 13,464 against 12,288, under P 5,256
     against 4,096) and A(L0) is the larger. The total ambiguity is
     ordered as the hypothesis says at every length.
   - A is small at every level (6.5-7.3%) and moves little with the
     endings: most of what licensing crossings costs here is not
     morphological (item 4).
2. **Q2 FAILS, by a Simpson's paradox.**

   | | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | B, of 4,018,176 crossed pairs | 57.08% | 60.24% | 86.38% | 95.90% |
   | B, a syncretic displaced word | - | 45.54% of 1,665,120 | 86.38% | 95.90% |
   | B, displaced words unambiguous | 57.08% | 70.63% of 2,353,056 | - | - |

   - Pooled, at L0 a syncretic displaced word gives FEWER garden paths
     (45.54%) than unambiguous ones (70.63%). B(L0) < B(L2), the rule's
     second clause, holds.
   - Within each kind of displacement (item 4) the registered direction
     HOLDS: adjectives only 7.28% against 1.13%, genitives only 77.44%
     against 74.01%, both 19.79% against 12.83%. The pooled split
     reverses because the kinds are unevenly mixed: genitive-only pairs,
     the garden-path-prone kind, are 52.2% of the syncretic side and
     95.2% of the unambiguous side.
   - B itself orders the levels as the hypothesis says, with Latin near
     the ideal (60.24% against 57.08%) and far from case-stripped
     (86.38%) and bare (95.90%).
3. **Every non-projective arc in this world is NP-internal** (an
   adjective's or a genitive's); no crossed pair displaces a verb
   dependent (counted directly). [proved: the root word dominates every
   word, so its arcs are never non-projective.] Crossings themselves are
   not all NP-internal: in *bonum puella dominum amat* the adjective's
   arc crosses the subject's.
4. **What is displaced decides the garden paths (descriptive, NOT
   registered).** The kinds (adjectives, genitives) are forced by item
   3; separating pairs that displace both is a choice, made in the T4
   fixes.

   | crossed pairs that displace | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | adjectives only (663,552) | 0.00% | 6.49% | 35.70% | 78.86% |
   | genitives only (3,108,864) | 72.99% | 74.97% | 100.00% | 100.00% |
   | both (245,760) | 10.00% | 18.98% | 50.92% | 90.00% |

   At L0, by the displaced words' forms: adjectives only, syncretic
   7.28% of 578,784, unambiguous 1.13% of 84,768; genitives only 77.44%
   of 869,376 and 74.01% of 2,239,488; both 19.79% of 216,960 and
   12.83% of 28,800.
   - **Adjectival hyperbaton is what endings pay for.** Ideal endings
     0%, Latin's 6.49%, agreement in gender and number only 35.70%, none
     78.86%. The 0% at L1 is structural to a five-word world: beside an
     adjective there is room for at most one noun besides the verb's
     arguments, so no two nouns share case, gender and number, and an
     ideal ending always names its noun. Six words would allow two such
     nouns.
   - **A displaced genitive is a garden path most of the time at EVERY
     level**, ideal endings included (72.99%), and always without case
     (100%). A genitive can depend on any noun, and its ending agrees
     with nothing, so no ending can say which noun.
5. **B by stratum** (registered as reported, not predicted). Mild
   crossed pairs are misread more often than the others (L0 62.70% of
   3,611,648 against 38.35% of 406,528), and B falls with the number of
   crossings (root arc included): L0 68.89%, 59.20%, 50.53%, 24.27% for
   1, 2, 3, 4+; the same fall at L1 and L2, while at L3 it is nearly
   flat (97.22%, 95.20%, 94.23%, 94.51%). Mild pairs are misread more
   than the others at every level. A reading, not measured: more
   crossings leave fewer nested alternatives.
6. **The mild reader R_M misreads 9.4-10.1% of crossed pairs at every
   level** (L1 9.38%, L0 9.53%, L2 9.88%, L3 10.12%): its misreads come
   from non-mild trees whose strings have a mild alternative, and
   barely depend on the endings. (Up to four words every tree is mild,
   so R_M never misreads there.)
7. **Reading cells.** The share of reading cells open under F but forced
   under P is 0.58% at L0, 0.00% at L1, 0.28% at L2, 0.22% at L3; not
   monotone, and not analysed.
8. **The picture, measured. [interpretation]**
   - A case ending tells a reader what ROLE a word plays (subject,
     object, possessor). Agreement tells it which WORD a word goes
     with. Only the second reconnects a displaced word: an adjective
     carries agreement with its noun; a genitive carries a role and no
     agreement.
   - That fits D130 in verse: there the non-projective arcs usually
     agree (KC 57.5.4: CIRCSE 64.6%), and their crossed projective
     partners seldom do (57.5.4.1). It does not reach D130's prose:
     under the registered operand most Latin treebanks fail P2 (57.5.1,
     57.5.3 [falls]), and the charters' non-projective arcs are mostly
     verb dependents (57.5.2), which a one-clause world cannot displace
     at all.
   - It also says what "morphology pays for crossing" cannot cover: a
     displaced genitive needs something else - order preference
     (adjacency), meaning, or statistics. That is slice 2's settler.

## Registered predictions

Registered in plan.md before any measure ran (plan approved f7b76ffd).
The rules never changed (checked by the T4 review against the git
history).

| | rule | verdict |
|---|---|---|
| Q1 | A(L1) <= A(L0) < A(L2) <= A(L3), and A(L0) - A(L1) <= A(L2) - A(L0) | **FAILS** on A(L1) <= A(L0) (884,736 > 878,568, a near-tie; holds up to four words); the other clauses hold |
| Q2 | at L0, B with a syncretic displaced word > B with unambiguous ones; and B(L0) < B(L2) | **FAILS** on the first clause (45.54% < 70.63%, pooled; the direction holds within each kind of displacement); B(L0) < B(L2) holds |

**Disclosure.** While testing the oracle at four words, its output
(including the four-word A and B) was printed before the five-word run
that decides the verdicts. Up to four words, Q2's first clause already
failed (46.12% against 62.83%), and Q1 HELD on every clause (8,192 /
8,208 / 9,760 / 11,264); Q1 failed only at five words. The rules were
not touched. (An earlier version of this file said the four-word
numbers "pointed to both failures"; that was false - T4 review, H1.)

**What the registration got wrong** (Claude's design, approved as
written):
- Q1 put a DIFFERENCE measure where a level was meant: A measures the
  marginal ambiguity of licensing crossings, which syncretism can
  shrink by making the nested reading ambiguous too, so its sign
  between near levels is fragile. The total ambiguity (open cells under
  F, or mean |F(s)|) is the level the hypothesis is about.
- Q2 pooled two kinds of displacement with different base rates, so
  its split could reverse by composition alone, and did. A split by
  what is displaced was needed first.
- Both would have shown up by working three or four sentences of each
  kind by hand through each measure before registering.

## Checks (48 full, 47 quick)

- **Paradigms:** the table == Whitaker on 70 cells; *fortis* membership
  on 30, Whitaker's 8 other forms exactly the listed ones; nominal
  forms per level 47 / 100 / 17 / 10 (by hand); every form's readings
  of one kind; L1 one reading per form; every L2 / L3 form an L0 form.
- **Tree table:** all 701 trees on n <= 5 positions (Cayley); projective
  by crossings (root arc) == by interval yields == no non-projective
  arc; projective => mild; reversal; projective and mild counts per n
  (1, 2, 7, 30, 143 and 1, 2, 9, 64, 569) pinned from
  `oracle_trees.py`.
- **Generator == analyser** at all four levels: positioned analyses on
  canonical strings, readings included. The generator builds slot by
  slot; the analyser applies the constraints to EVERY multiset of forms
  with one verb (18.4M at L1). The analyser never repeats an analysis.
- **Sizes:** pairs 384 / 55,296 / 6,696,960 by length at every level (=
  `oracle_partb.py`'s pairs by length, = task 0's count).
- **Word order:** F(s) of every string at every level (6.4M) equals its
  bag's F carried to that order.
- **Hand sentences (9):** *bonum puella dominum amat* (displaced,
  unambiguous, read); *puella bonum dominum amat* (nested); *bona puella
  dona amat* (displaced *bona* -> *dona*, syncretic: R_P settles on
  *bona* -> *puella*) and the same analysis at L1 (read); *donum donum
  amat* (attachments forced, readings open); *puella dominus amat* at L2
  and *puella dominum amat* at L0; *amant bona dona dona* (the neuter
  plural pair: |P| = 2, *bona*'s head open under F and forced under P,
  one cell of A); *amant bona bona dona dona* (two adjectives crossing
  to two nouns, not well-nested: R_P and R_M both misread). Each checks
  |F|, |P|, |M|, the truth under R_P and R_M, and forced cells under F
  and under P.
- **Part B:** one denominator per measure across levels; every A, B,
  split and R_M total, the three-way descriptive split with its form
  splits, the open cells under F and P, and B by mildness and by
  crossing count pinned from `oracle_partb.py` at four and five words;
  open F - open P == A's numerator; L1 has no syncretic form; no
  crossed pair displaces a verb dependent (counted directly).
- **Rules:** Q1 and Q2 as functions, tested on constructed totals,
  including each threshold, each isolated (A1 = A0; A0 = A2 as (2, 2,
  2, 3); A2 = A3; A2 > A3 alone as (1, 2, 4, 3); 2 A0 = A1 + A2; equal
  B shares); the verdicts re-derived from the oracle's numbers
  (wiring).

`oracle_partb.py` is independent in algorithm, not in data: it re-types
Bennett's tables, finds analyses tree first (main.c goes readings
first), takes its bags from its own structural enumeration, and tests
projectivity by interval yields, displacement by domination, mildness
by gaps and interleaving. (The plan's registered description, "brute
force over all head arrays and readings", does not match it; see the
plan's T4 note.)

## Plants

All through `silva.planta`, all red.

- **T1 (16):** a table form; Whitaker's AGE and FREQ filters; the
  *fortis* extras list; L2 losing gender; L3 keeping agreement; planar
  for projective; mild as gap degree <= 2; the generator building a
  genitive as an ablative; the analyser dropping subject agreement,
  adjective gender, the cycle test, the one-genitive limit, or looking
  only rightward; the multinomial; R_P reading with M; the order
  check's mapping.
- **T2 (12):** A's sign and denominator; misread judged by M; the
  syncretic threshold; the displaced mask; R_M judged by P; the
  genitive class; Q1's factor and its strict A0 < A2; Q2's strictness;
  two verdict wirings.
- **T4 fixes (8):** Q1's A2 <= A3 clause; genitives-only and both
  swapped; open under P counted as under F; crossings without the root
  arc; the mild stratum inverted; genitives counted as verb dependents;
  the hand check's P cells taken over all analyses; R_M read with P.
- Re-planted (the plant's fault, not the gate's): two did not compile,
  one anchor matched twice; three needed five words (two genitives on
  one noun; R_M misreads, since every tree up to four words is mild;
  a Q1 threshold case that was not isolated); one verdict wiring was an
  equivalent mutant (Q2 already fails on its first clause).

## Review (T4, 2026-10-19)

An independent reviewer (recensor-d131) compiled the snapshot in its
own scratch directory and wrote its own oracle with different
algorithms: position-first generation (lexeme sequences, then heads),
F(s) as the group of generated analyses with that surface string (no
parser), projectivity by crossings with the root arc, displacement by
descendants, mildness by gaps and interleaving. It re-derived every
number in this file, in KC 58 and in the plan's notes, at four and five
words and all four levels, and checked the table against Bennett's text
and Whitaker mechanically. **Zero mismatches** (mean |F| was
truncated, not rounded). No rule changed after registration; both
verdicts follow. H 1, M 8, L 7, N 5, all acted on:

- **H1:** the disclosure was false (Q1 held at four words). Corrected
  here, in the plan and in claims.
- **M1:** Q1's failure is a near-tie that flips with length; the
  mechanism does not decide its sign. A by length added.
- **M2:** the reviewer found the registered split reversing inside the
  genitive class too (65.93% against 73.23%). Separating the pairs that
  displace both (a T4 fix) showed that this, like the pooled reversal,
  is composition: within each of the three kinds the registered
  direction holds (item 2).
- **M3:** the objection's reply said the split was forced; only the
  kinds are, the treatment of mixed pairs was a choice. Three-way split
  added.
- **M4:** D130 was cited wrongly (57.5.4.1 for 57.5.4) and stretched to
  prose. Corrected, with the one-clause limit.
- **M5:** "every crossing is inside a noun phrase" was false; every
  non-projective arc is.
- **M6:** the 0% at L1 is structural to five words; "more lemmas" in
  Not covered corrected to six words.
- **M7:** the open-cell totals behind A rested on a scratch probe; now
  computed and pinned in the demo.
- **M8:** the rule check never isolated A2 > A3; added and planted.
- **L1-L7, N1-N5:** the verb-dependent count made direct and the class
  relabelled; the Whitaker check relabelled (the table, not Bennett's
  text); the tree-count oracle committed (`oracle_trees.py`) and the
  pairs-by-length pin sourced from `oracle_partb.py`; means rounded;
  the six-word size made consistent; an unsupported "same reason"
  removed; hand sentences for cells under P and an R_M misread added
  (with the registered neuter pair); B by mildness and by crossing
  count delivered; the oracle description noted; "four verbs" made
  "four verb forms"; the output line count noted.

The reviewer did not re-run the plants.

## Not covered

- **Statistics and settling** (slice 2): the ordered tier, an order
  preference, the settler at three temperatures (brainstorm XIV). A
  displaced genitive is exactly where they would have to work.
- **Six words**: two nouns of the same case, gender and number beside
  an adjective (so ideal endings no longer always name the noun), and
  richer tangles. Also prepositions, pro-drop, predicate adjectives.
- **More than one clause**: verb dependents displaced across clauses,
  the kind D130 found in prose, cannot occur here.
- **Real frequencies:** every analysis and order weighs the same, so
  60% of pairs fall in crossed orders. Shares are within strata.
- **The registered failure's fix** (a level measure for Q1, a split by
  what is displaced for Q2) is reported descriptively here; testing it
  as a prediction needs a new world (e.g. six words) where it was not
  seen first.

## Run

```text
./bin/aedilis knotapel/demo_131_latin_sudoku/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
python3 -I knotapel/demo_131_latin_sudoku/oracle_partb.py 5
python3 -I knotapel/demo_131_latin_sudoku/oracle_trees.py
```

- **Full mode:** about 21 s, 48 checks. Reads `INFLECTS.LAT` at run
  time (from the repo root).
- **Quick mode** (`D131_CELER=1`): exhaustive passes up to four words,
  0.4 s, 47 checks; the verdicts are not decided there.
- **Oracle:** about 2.5 min at five words, 1.5 s at four.
