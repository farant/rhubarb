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
banner gives the clang line and commit.

**Both registered predictions FAIL** (below). What the failures expose
is the result: endings pay for ADJECTIVAL hyperbaton, and cannot pay
for a displaced GENITIVE, because a case ending says what role a word
plays, not which noun it belongs to. That reading is descriptive: it
was found after the run.

## The world

Syntax only, no semantics ("donum puellam amat" is grammatical).

- **Lexicon:** nouns *puella* (1st, f), *dominus* (2nd, m), *donum*
  (2nd, n), *rex, regis* (3rd, m); adjectives *bonus* (1st/2nd) and
  *fortis* (3rd); verbs *amat / amant* (subject + object), *dat / dant*
  (subject + object + dative). Cases nominative, genitive, dative,
  accusative, ablative; no vocative or locative.
- **Endings:** Charles E. Bennett, *New Latin Grammar* (1895, 1908,
  1918; public domain; Project Gutenberg #15665), sections 20, 23, 32,
  63, 69, 101 and 127, macrons dropped as in the treebanks (which is
  where `-a` / `-ā` collapse). *fortis* takes Bennett's first-listed
  forms (acc. pl. *-is* omitted). Cross-checked against Whitaker's
  WORDS (`oratio/vocabularium/la/INFLECTS.LAT`, entries AGE X, FREQ A),
  read as data: equal in all 70 noun and *bonus* cells; for *fortis*,
  Bennett's form is in Whitaker's set in all 30 cells, and Whitaker's 8
  other forms are listed in the demo.
- **Slot frames:** the verb is the root and takes exactly one
  nominative subject agreeing in number, exactly one accusative object,
  for *dat / dant* exactly one dative, and any number of ablative
  adjuncts; a genitive depends on another noun (at most one per noun);
  an adjective depends on a noun of the same case, gender and number.
  No pro-drop.
- **Sentences:** every analysis of up to five words, in every word
  order (task 0 chose five: six words would be 810M pairs, beyond an
  exhaustive Python oracle).

A **pair** is a string together with one of its analyses (a head and a
reading per position). Each analysis yields exactly one string at each
level, so the pairs are the SAME 6,752,640 at every level, and every
measure below has one denominator across levels.

### Morphology levels (only the surface forms change)

| level | what the forms carry | nominal forms | strings | max \|F(s)\| | mean \|F(s)\| over pairs |
|---|---|---|---|---|---|
| L1 ideal | every reading its own form (counterfactual) | 100 | 3,176,832 | 6 | 3.07 |
| L0 Latin | the real paradigms | 47 | 2,734,201 | 18 | 3.78 |
| L2 case-stripped | one form per gender and number (the nominative); agreement in gender and number kept | 17 | 296,312 | 156 | 67.21 |
| L3 bare | as L2, adjectives invariant (no agreement); nouns keep number | 10 | 169,760 | 156 | 70.28 |

Verbs agree in number at every level. L2 and L3 are derived from the
L0 table mechanically (a check).

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
  their cells (every word's, the verb's included).
- **B (garden paths)**, over pairs whose tree is not projective: the
  share misread by R_P. Split by whether some DISPLACED word (the
  dependent of a non-projective arc: a word strictly inside its span is
  not dominated by its head) has a syncretic form (two or more readings
  at that level).

## Result

1. **Q1 FAILS: Latin's licensing cost is not above the ideal's.**

   | | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | A | 6.49% | 6.45% | 7.29% | 7.31% |
   | A, numerator over 13,631,616 cells | 884,736 | 878,568 | 993,296 | 996,352 |

   - A(L1) <= A(L0) is false. The rest of the rule holds: A(L0) <
     A(L2) <= A(L3), and the "real bet" A(L0) - A(L1) <= A(L2) - A(L0)
     (Latin at least as close to ideal as to case-stripped).
   - **Why:** A is a DIFFERENCE, the cells open under F minus those open
     under P. Syncretism opens cells under both. Over the projective
     pairs, open attachment cells under F are 2,209,476 at L0 and
     1,953,792 at L1; under P, 1,330,908 and 1,069,056. Latin's readings
     are more ambiguous overall, and its nested reading already leaves
     more open, so the increment that licensing crossings adds is
     slightly smaller. (Probe in the scratchpad with the oracle's own
     functions; the two A numerators are pinned.)
   - A is small at every level (6.5-7.3%) and moves little with the
     endings: most of what licensing crossings costs here is not
     morphological (item 4).
2. **Q2 FAILS: the registered split goes the wrong way.**

   | | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | B, of 4,018,176 crossed pairs | 57.08% | 60.24% | 86.38% | 95.90% |
   | B, a syncretic displaced word | - | 45.54% of 1,665,120 | 86.38% | 95.90% |
   | B, displaced words unambiguous | 57.08% | 70.63% of 2,353,056 | - | - |

   - At L0 a syncretic displaced word gives FEWER garden paths (45.54%)
     than unambiguous ones (70.63%). B(L0) < B(L2), the rule's second
     clause, holds.
   - B itself orders the levels as the hypothesis says, with Latin near
     the ideal (60.24% against 57.08%) and far from case-stripped
     (86.38%) and bare (95.90%).
3. **Every crossing in this world is inside a noun phrase.** Every
   non-projective arc is an adjective's or a genitive's; no crossed
   pair displaces only verb dependents. [theorem here: the root word
   dominates every word, so its arcs are never non-projective.]
4. **Adjectives and genitives behave oppositely (descriptive, NOT
   registered).**

   | crossed pairs that displace | L1 ideal | L0 Latin | L2 case-stripped | L3 bare |
   |---|---|---|---|---|
   | only adjectives (663,552) | 0.00% | 6.49% | 35.70% | 78.86% |
   | a genitive (3,354,624) | 68.38% | 70.87% | 96.40% | 99.27% |

   - **Adjectival hyperbaton is what endings pay for.** With ideal
     endings a nested reader never misreads it; with Latin's, 6.49%; with
     agreement in gender and number only, 35.70%; with none, 78.86%.
     Within Latin: 7.28% when the displaced adjective's form is
     syncretic (578,784 pairs), 1.13% when it is unambiguous (84,768).
     This is the plan's second question answered on the case the
     hypothesis was pictured from (adjectival hyperbaton).
   - **A displaced genitive is a garden path most of the time at EVERY
     level**, ideal endings included. A genitive can depend on any noun,
     and its ending agrees with nothing, so no ending can say which
     noun. The registered split mixes the two. Of its "unambiguous"
     side at L0 (2,353,056 pairs), only 84,768 displace adjectives
     alone; the other 96.4% displace a genitive, in forms such as
     *regis* or *dominorum* that are unambiguous yet attach anywhere.
     That is why Q2's first clause fails.
5. **The mild reader R_M misreads 9.4-10.1% of crossed pairs at every
   level** (L1 9.38%, L0 9.53%, L2 9.88%, L3 10.12%): its misreads come
   from non-mild trees whose strings have a mild alternative, and
   barely depend on the endings. (Up to four words every tree is mild,
   so R_M never misreads there.)
6. **Reading cells.** The share of reading cells open under F but forced
   under P is 0.58% at L0, 0.00% at L1, 0.28% at L2, 0.22% at L3 - not
   monotone either, for the same reason as A.
7. **The picture, measured. [interpretation]**
   - A case ending tells a reader what ROLE a word plays (subject,
     object, possessor). Agreement tells it which WORD a word goes
     with. Only the second reconnects a displaced word: an adjective
     carries agreement with its noun; a genitive carries a role and no
     agreement.
   - That fits D130 (KC 57.5.4.1): in real Latin the crossing arc is
     usually a displaced, agreeing word, and its crossed partner seldom
     agrees. Latin crosses where morphology can pay.
   - It also says what "morphology pays for crossing" cannot cover: a
     displaced genitive needs something else - order preference
     (adjacency), meaning, or statistics. That is slice 2's settler.

## Registered predictions

Registered in plan.md before any measure ran (plan approved f7b76ffd).
The rules never changed.

| | rule | verdict |
|---|---|---|
| Q1 | A(L1) <= A(L0) < A(L2) <= A(L3), and A(L0) - A(L1) <= A(L2) - A(L0) | **FAILS** on A(L1) <= A(L0) (884,736 > 878,568); the other clauses hold |
| Q2 | at L0, B with a syncretic displaced word > B with unambiguous ones; and B(L0) < B(L2) | **FAILS** on the first clause (45.54% < 70.63%); B(L0) < B(L2) holds |

**Disclosure.** While testing the oracle at four words, its output
(including the four-word A and B) was printed before the five-word run
that decides the verdicts. The four-word numbers already pointed to
both failures. The rules were not touched.

**What the registration got wrong** (Claude's design, approved as
written):
- Q1 put a DIFFERENCE measure where a level was meant: A measures the
  marginal ambiguity of licensing crossings, which syncretism can
  lower by making the nested reading ambiguous too. The total
  ambiguity (open cells under F, or mean |F(s)|) is the level that the
  hypothesis is about.
- Q2's split was by the displaced word's FORM, but the world has two
  kinds of displacement, and only one of them (adjectives) is reached
  by agreement. A split by what is displaced was needed first.
- Both would have shown up by working three or four sentences of each
  kind by hand through each measure before registering.

## Checks (45 full, 44 quick)

- **Paradigms:** Bennett == Whitaker on 70 cells; *fortis* membership
  on 30, Whitaker's 8 other forms exactly the listed ones; nominal
  forms per level 47 / 100 / 17 / 10 (by hand); every form's readings
  of one kind; L1 one reading per form; every L2 / L3 form an L0 form.
- **Tree table:** all 701 trees on n <= 5 positions (Cayley); projective
  by crossings (root arc) == by interval yields == no non-projective
  arc; projective => mild; reversal; projective and mild counts per n
  (1, 2, 7, 30, 143 and 1, 2, 9, 64, 569) pinned from an independent
  Python count.
- **Generator == analyser** at all four levels: positioned analyses on
  canonical strings, readings included. The generator builds slot by
  slot; the analyser applies the constraints to EVERY multiset of forms
  with one verb (18.4M at L1). The analyser never repeats an analysis.
- **Sizes:** pairs 384 / 55,296 / 6,696,960 by length at every level (=
  task 0's independent count).
- **Word order:** F(s) of every string at every level (6.4M) equals its
  bag's F carried to that order.
- **Hand sentences (7):** among them *bonum puella dominum amat*
  (displaced, unambiguous, read), *bona puella dona amat* (displaced
  *bona* -> *dona*, syncretic: R_P settles on *bona* -> *puella*), the
  same analysis at L1 (read), *donum donum amat* (attachments forced,
  readings open), *puella dominus amat* at L2 (subject and object
  trade places).
- **Part B:** one denominator per measure across levels; every A, B,
  split and R_M total at every level pinned from `oracle_partb.py` at
  four and five words; the descriptive split pinned too; L1 has no
  syncretic form; no crossed pair displaces only verb dependents.
- **Rules:** Q1 and Q2 as functions, tested on constructed totals,
  including each threshold (A1 = A0, A0 = A2 isolated as (2, 2, 2, 3),
  A2 = A3, 2 A0 = A1 + A2, equal B shares); the verdicts re-derived
  from the oracle's numbers (wiring).

`oracle_partb.py` is independent in algorithm, not in data: it re-types
Bennett's tables, finds analyses tree first (main.c goes readings
first), takes its bags from its own structural enumeration, and tests
projectivity by interval yields, displacement by domination, mildness
by gaps and interleaving.

## Plants

All through `silva.planta`, all red.

- **T1 (16):** a Bennett form; Whitaker's AGE and FREQ filters; the
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
- Re-planted (the plant's fault, not the gate's): one did not compile,
  one anchor matched twice; three needed five words (two genitives on
  one noun; R_M misreads, since every tree up to four words is mild;
  a Q1 threshold case that was not isolated); one verdict wiring was an
  equivalent mutant (Q2 already fails on its first clause).

## Not covered

- **Statistics and settling** (slice 2): the ordered tier, an order
  preference, the settler at three temperatures (brainstorm XIV). A
  displaced genitive is exactly where they would have to work.
- **Six words** and the richer tangles they allow; prepositions;
  pro-drop; more lemmas (two nouns of the same case, gender and number
  make even agreement ambiguous); predicate adjectives.
- **Real frequencies:** every analysis and order weighs the same, so
  60% of pairs fall in crossed orders. Shares are within strata.
- **The registered failure's fix** (a level measure for Q1, a split by
  what is displaced for Q2) is reported descriptively here; testing it
  as a prediction needs a new world (e.g. other lemmas, or six words)
  where it was not seen first.

## Run

```text
./bin/aedilis knotapel/demo_131_latin_sudoku/main.c &&
  bash build/aedilis/main/struere.sh && ./build/aedilis/main/main
python3 -I knotapel/demo_131_latin_sudoku/oracle_partb.py 5
```

- **Full mode:** about 21 s, 45 checks. Reads `INFLECTS.LAT` at run
  time (from the repo root).
- **Quick mode** (`D131_CELER=1`): exhaustive passes up to four words,
  0.4 s, 44 checks; the verdicts are not decided there.
- **Oracle:** about 2.5 min at five words, 1.5 s at four.
