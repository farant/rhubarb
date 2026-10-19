# Demo 131: toy Latin Sudoku — does morphology pay for crossing? (plan)

Status: approved by Fran (2026-10-19), "Open for Fran" decided as
proposed: (1) 5 words, 6 if task 0's count allows; (2) the
simplifications as listed; (3) both control levels L1 and L3 kept;
(4) folder `knotapel/demo_131_latin_sudoku/`. Slice 1 of
proposal N14 (`knotapel/demo-proposals.md`, ledger …9XK1EA); context
`knotapel/brainstorm-skeleton-settling.md` sections XI, XIII, XIV
(threads 13–14). Real Latin endings copied into the demo as a fixed,
cited table (Fran, 2026-10-19); oratio's ideas, not its code.

## Question

D130 found that where Latin crosses freely (verse), the DISPLACED word
of a crossing usually carries agreement with its head, and English,
without case labels, almost never crosses. The hypothesis drawn from
it (brainstorm XIII, XIV): **morphology pays for crossing** — case,
gender and number endings let a reader reconnect a displaced word, so
the grammar can afford free order; without the labels, order must
carry the structure, and it stays planar.

That cannot be tested on treebanks (one language, confounded). It can
be tested exactly on a toy world small enough to enumerate, where the
labels can be weakened on purpose. And it must be tested against REAL
syncretism: Latin endings are ambiguous (`-a` nominative or ablative
singular, or neuter plural; `-ae` genitive, dative or nominative
plural; `-is` dative or ablative plural), so the labels do not always
tell the candidates apart.

1. How much ambiguity does LICENSING crossings cost a reader, with
   Latin's real endings, against ideal endings and against endings
   stripped of case (and of agreement)?
2. When a reader prefers nesting and reads a crossed sentence, how
   often does it settle on a wrong nested reading (a garden path)? Does
   that depend on whether the displaced word's ending is syncretic?

## The world (the slot library)

Syntax only, no semantics: "donum puellam amat" is grammatical.

- **Lexicon** (real paradigms, macrons dropped as in the treebanks —
  which is where `-a`/`-ā` and similar collapse):
  - nouns: `puella` (1st, f), `dominus` (2nd, m), `donum` (2nd, n),
    `rex, regis` (3rd, m) — four declension patterns, including
    neuter nominative = accusative and an adjective whose ending does
    not match its 3rd-declension noun (`rex bonus`);
  - adjective: `bonus, bona, bonum` (1st/2nd); possibly also `fortis,
    forte` (3rd) — decided in task 0 by size;
  - verbs: `amat / amant` (subject + accusative), `dat / dant`
    (subject + accusative + dative).
  - Cases: nominative, genitive, dative, accusative, ablative.
    Vocative and locative are left out (no slot; stated).
- **Paradigm table:** every form with ALL its readings (case, gender,
  number), copied into the demo, cited to Allen and Greenough, *New
  Latin Grammar* (1903, public domain; sections checked in task 0)
  [task 0: cited to Bennett's *New Latin Grammar* instead, see Task 0
  findings] and
  checked entry by entry against Whitaker's `INFLECTS.LAT`
  (`oratio/vocabularium/la/`, FREQ A / AGE X entries) as data.
- **Slot frames (constraints):**
  - verb: exactly one nominative subject agreeing in number; exactly
    one accusative object; `dat` also exactly one dative; any number
    of ablative adjuncts (obl). The verb is the root. No pro-drop
    (stated simplification).
  - adjective: head is a noun with the same case, gender and number.
  - noun: subject / object / dative / ablative of the verb, or a
    genitive dependent of another noun (at most one genitive per
    noun); any number of adjectives.
- **Analysis** of a word string = a tree (head per word, UD-like
  arcs: amod, nsubj, obj, iobj, obl, nmod) plus a reading per word,
  satisfying every frame. A string's analyses are exactly the
  grammatical structures that produce it, so ambiguity is real, never
  annotated.
- **Sentences:** every analysis of up to 5 words (6 if task 0's count
  allows), in EVERY word order. The "crossing dial" of N14 becomes a
  stratification: each (analysis, order) pair is classed by its
  tree's crossings under that order (projective / mild / other, and
  crossing count). Pairs are weighted uniformly; real frequencies are
  slice 2's business.

## Morphology levels (the knob)

The same world at four levels; only the surface forms change.

- **L1 ideal:** every reading its own form (no syncretism) — a
  counterfactual bound.
- **L0 Latin:** the real paradigms.
- **L2 case-stripped:** each noun and adjective keeps one form per
  gender and number (its nominative); agreement in gender and number
  stays — roughly the Romance situation.
- **L3 bare:** as L2, and adjectives are invariant (no agreement);
  nouns keep number — roughly English.

Verbs agree in number at every level.

## Readers

For a string s:
- **F(s)**: all analyses (crossings allowed). Independent of word
  order by construction (a check).
- **P(s)**: those whose tree is PROJECTIVE under s's order (Kuhlmann
  and Nivre Def. 3, root arc included; `dependentia_cruces`).
- **M(s)**: those MILDLY non-projective (gap degree <= 1, well-nested;
  `dependentia_mensurare`).
- **R_P(s)** = P(s) if non-empty, else F(s): the reader that prefers
  nesting and accepts crossing only when nothing nested works.
  **R_M(s)** likewise with M.

**Cells** (Sudoku): per word, an ATTACHMENT cell (its head) and a
READING cell (its case, gender, number). A cell is FORCED in a set of
analyses if it has one value across the set, OPEN otherwise. (The
"ordered" tier needs statistics: slice 2.)

**Misread:** the pair (analysis a, order) is misread by a reader if a
is not in the reader's set for the string.

## Measures

- **A (licensing cost)**, over pairs whose tree is projective under
  the order: attachment cells open under F but forced under P, as a
  share of all attachment cells (P(s) ⊆ F(s), so per pair >= 0). The
  cost to a reader that allows crossings of having that option.
- **B (garden paths)**, over pairs whose tree is NOT projective under
  the order: share misread by R_P; split by whether some DISPLACED word
  (the dependent of a per-arc non-projective arc, `dependentia_arcus`)
  has a syncretic form (two or more readings at that level) or all
  displaced words are unambiguous.
- Reported beside them, without prediction: reading cells, R_M's
  misreads (not a subset of R_P's: M(s) can be non-empty where P(s) is
  empty), solution-set sizes, by length and crossing count, per level.

## Registered before running

(Decided on exact rational totals, sentences up to the length fixed in
task 0.)

- **Q1 (morphology pays):** A(L1) <= A(L0) < A(L2) <= A(L3), AND Latin
  keeps most of the ideal's benefit: A(L0) - A(L1) <= A(L2) - A(L0)
  (L0 at least as close to L1 as to L2). The ordering is expected; the
  second half is the real bet.
- **Q2 (syncretism makes the garden paths):** at L0, B among crossed
  pairs with a syncretic displaced word is greater than B among
  crossed pairs whose displaced words are all unambiguous; and B(L0) <
  B(L2) overall.

How the outcome reaches claims.md: a new node in Part XI beside 57
(toy-world claims graded [exact] after an independent oracle), with
the verdicts as registered.

## Checks (characterizations, not counts)

- Paradigm table = Allen and Greenough (sections cited) and = the
  FREQ A entries of INFLECTS.LAT for these declensions, entry by
  entry; L1 has exactly one reading per form; L2 and L3 forms are
  derived mechanically from L0 (no hand-written forms).
- Generator and analyser are separate code: for every string, the set
  of analyses that GENERATE it equals F(s) (soundness and completeness,
  exhaustively).
- F(s) is the same for every order of the same words (theorem).
- P(s) ⊆ M(s) ⊆ F(s); projective by `dependentia_cruces` equals
  projective by `dependentia_mensurare` on every tree (as in D130).
- Reversal: reversing s maps P(s) and M(s) onto those of the reversed
  string.
- Hand-worked sentences: a handful with their analyses, cells and
  misreads computed by hand (e.g. `bonum puella dominum amat`, whose
  displaced `bonum` crosses `puella`'s arc, against `puella bonum
  dominum amat`; a neuter `dona` subject / object pair).
- Totals pinned from an independent Python oracle (brute force over
  all head arrays and readings, its own projectivity test), labelled
  as oracle constants.
- Rule functions checked on hand-computed totals, including totals
  constructed exactly at each threshold (D130 lesson).
- Plants via `silva.planta`.

## Size and time

Estimated in task 0 by counting analyses per length. F(s) is computed
once per word multiset (it ignores order); P and M filter it per order
(n! <= 120 at 5 words, 720 at 6). Expected: seconds.

## Tasks (one per turn, Fran's go-ahead each)

0. *Done 2026-10-19 (Task 0 findings below).* **Sources and size:**
   endings checked against Allen and Greenough
   (section numbers) and INFLECTS.LAT; count analyses at 5 and 6
   words; decide the lexicon (with or without `fortis`) and length;
   confirm `dependentia` covers every tree measure (no new house
   library expected in this slice: the enumeration is specific to the
   toy grammar; slice 2's exact settler is the likely house library).
1. **T1:** the world: paradigm table, levels, generator, analyser,
   readers; Part A checks (table, duality, order-independence,
   inclusions, reversal, hand sentences); plants.
2. **T2:** measures A and B, Q1 and Q2 decided, the Python oracle,
   rule checks; plants.
3. **T3:** findings.md, claims.md, ledger (N14 stays open for slice
   2), archive.sh.
4. **T4:** independent review with its own oracle, then fixes.

## Task 0 findings (2026-10-19)

- **Source: Bennett, not Allen and Greenough.** The local Gutenberg
  mirror has Charles E. Bennett, *New Latin Grammar* (1895, 1908, 1918;
  public domain; Project Gutenberg #15665,
  `../gutenberg-mirror/1/5/6/6/15665/15665-0.txt`), not Allen and
  Greenough; a source the repo can open beats one it cannot. Patterns:
  sec. 20 *porta* (= *puella*), 23 *hortus* and *bellum* (= *dominus*,
  *donum*), 32 *dux* (= *rex, regis*: guttural mute stems), 63
  *bonus*, 69 *fortis*, 101 *amat / amant*, 127 *dat / dant*. Whitaker
  cites Allen and Greenough and Gildersleeve and Lodge, so the cross-
  check still reaches them.
- **Cross-check against INFLECTS.LAT** (AGE X, FREQ A; Whitaker's
  variant 0 = every variant, gender C = masculine or feminine, X =
  any): the four nouns and *bonus*, 80 cells, are EQUAL to Bennett's
  forms with macrons stripped. *fortis*, 20 cells: Bennett's form is
  in Whitaker's set in every cell, and Whitaker adds 8 variants from
  its shared third-declension entries — abl. sg. *-e* and gen. pl.
  *-um* (consonant-stem and comparative patterns; Bennett sec. 67a: an
  adjective like *fortis* has *-i*, *-ium*) and acc. pl. m./f. *-is*
  (Bennett sec. 67a and 69 list *-is* beside *-es* as an alternative).
  **Decision:** Bennett's first-listed forms only; acc. pl. *-is*
  omitted, stated. T1's check: equality on the 80 cells, membership on
  the 20, the 8 extra variants listed in the demo.
- **Lexicon: `fortis` is IN.** A third-declension adjective whose
  endings differ from its nouns' (*puella fortis*) and are syncretic in
  their own way (*fortis* nom. or gen. sg. m./f.; *forti* dat. or abl.
  sg.; *fortes* nom. or acc. pl.; *forte* nom. or acc. sg. n.). It costs
  about 13% more pairs.
- **The unit.** A PAIR = (string, one of its analyses, heads by
  position). Each analysis yields exactly one string at each level, so
  the population of pairs is the SAME at every morphology level: the
  levels share denominators. Count = sum over word bags of (distinct
  orderings) x |F(bag)|. A first count of crossed pairs double-counted
  analyses that differ only by swapping identical tokens (57,504
  against 55,296 at four words); de-duplicating per bag, readings
  included, made the two methods agree. T1's generator/analyser
  duality check must compare positioned analyses WITH readings.
- **Size (with `fortis`):**

  | words | pairs | projective | not projective |
  |---|---|---|---|
  | 3 | 384 | 384 | 0 |
  | 4 | 55,296 | 39,936 | 15,360 |
  | 5 | 6,696,960 | 2,694,144 | 4,002,816 |
  | 6 | 809,533,440 | | |

  (Without `fortis`: 5,966,208 pairs up to five words, 671,299,968 up
  to six. Largest |F| of a string: 18 at five words, 144 at six.)
- **Length: FIVE words.** Five already holds 4.0M crossed pairs,
  including the golden line (adjective, adjective, verb, noun, noun).
  Six (816M pairs) would run in C if projectivity is looked up per head
  array, but the independent Python oracle could not cover it
  exhaustively, and the registered verdicts are to be pinned by one.
  So "6 if the count allows" resolves to no.
- **Uniform weighting puts most pairs in crossed orders** (60% at five
  words), since most orders of five words cross; reported, not a
  defect — the measures are shares within each stratum.
- **`dependentia` covers every tree measure:** projective =
  `dependentia_cruces` (`proiectiva`, root arc included); mild =
  `dependentia_mensurare` (`gradus_lacunarum <= 1` and
  `bene_nidificata`); displaced words = `dependentia_arcus`
  (`non_proiectivus`). At five words there are 5^4 = 625 rooted
  trees (Cayley), so each is measured once and looked up. No new house
  library in this slice.
- Scratch tools (not committed): the Bennett/Whitaker comparison and
  the size counter, both Python.

## Slice 2 (not this demo)

The settler at three temperatures on the same worlds: sum-product,
p^beta for integer beta, max-product, in exact rationals (brainstorm
XIV); the ordered tier; where cells become forced as the temperature
drops; crossed against planar.

## Open for Fran

1. Length: up to 5 words, or 6 if the count allows (task 0 reports
   both).
2. The simplifications: syntax only, no pro-drop, no vocative or
   locative, uniform weighting, macrons dropped.
3. The two invented control levels L1 (ideal) and L3 (bare): derived
   mechanically from the real table, but invented — keep both?
4. Folder `knotapel/demo_131_latin_sudoku/`.
