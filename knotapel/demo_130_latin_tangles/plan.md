# Demo 130: Latin dependency trees as tangles (plan)

Status: approved by Fran (2026-10-17), with "Open for Fran" decided:
(1) option (b) — the NC treebanks are used, aggregate numbers only, no
sentences or excerpts quoted, the files stay fetched (never copied into
the repo); (2) the tree measures go in a small library beside oratio's
CoNLL-U reader (location and name settled in task 0, with the lint),
tested on its own; (3) the registered thresholds as written; (4, task
0) option (a): the CoNLL-U reader MOVES into the house tier and the
tree measures go beside it there (decision 2 superseded, since
archive.sh cannot freeze oratio/fontes; API sketch below). Proposal
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
- **Projective** (Kuhlmann and Nivre's Def. 3) = every word's yield
  (its subtree) is an interval. Equivalently, no two arcs cross once the
  artificial root arc (0 -> root word) is added; both are computed and
  must agree (a check).
- **Planar** (their Def. 4) = no two REAL arcs cross when all are drawn
  above the sentence, WITHOUT the root arc. Planar is strictly weaker
  than projective (their Fig. 2a: planar, yet a yield is not an
  interval); they avoid the root arc because it "obscures the
  distinction between projectivity and planarity". Both are reported.
- **Crossings** = number of crossing pairs among real arcs (the crossing
  number of the arc diagram drawn above the sentence); crossings with
  the root arc are reported in their own column. **Non-planar arcs** =
  real arcs in at least one crossing.
- **Non-projective arc** (the standard per-arc notion, e.g. Nivre and
  Nilsson 2005): arc (h, d) is non-projective iff some word strictly
  between h and d is not dominated by h. Equivalently its edge degree
  (Def. 9) is > 0; a tree is projective iff it has no non-projective
  arc. Parts B and C and prediction P2 use THIS notion; non-planar arcs
  are reported beside it. The two differ: in a crossing pair usually
  only one arc is non-projective (the hyperbaton arc), its partner
  projective. (Defined 2026-10-17, after the T1a review, before any
  census ran: the plan used the term without defining it, and the API
  then counted only non-planar arcs. The reviewer reported that the two
  counts differ by about 2x on the repo treebanks; no agreement or
  relation figure has been computed by anyone.)
- **Mild non-projectivity** (Kuhlmann and Nivre, "Mildly Non-Projective
  Dependency Structures", COLING/ACL 2006 poster sessions, pp. 507-514;
  definitions checked against the paper in task 0): a GAP is a pair of
  adjacent nodes in a word's sorted yield that differ by more than 1;
  the gap degree of a word is its number of gaps, of a tree the maximum
  (Defs. 6-7). Two subtrees are disjoint if neither root dominates the
  other; they INTERLEAVE if l1 < l2 < r1 < r2 for l1, r1 in one and l2,
  r2 in the other; a tree is WELL-NESTED if no two disjoint subtrees
  interleave (Def. 8). The degree of an edge = the number of connected
  components of the subgraph induced by its span whose root is not
  dominated by the edge's head; edge degree of a tree = the maximum
  (Def. 9). Hierarchy: projective < planar < well-nested < unrestricted
  (checked on every tree). Gap degree and well-nestedness characterize
  derivations in Tree Adjoining Grammar (the paper, citing Bodirsky et
  al. 2005), so "bounded crossings" gets a formal-language meaning.
  Reference points from the paper (Table 1): Czech PDT 76.85%
  projective, 99.89% well-nested; Danish DDT 84.95% projective, 99.89%
  well-nested (their counts are over dependency FORESTS, several roots
  allowed, so they are yardsticks, not like-for-like with UD trees).
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
  Removing a PUNCT word re-attaches any dependents to its head
  (`dependentia_omittere`, counted). A sentence whose ROOT is PUNCT (45
  all-punctuation sentences in EWT; none in the Latin repo treebanks)
  cannot lose it and stay a tree: it is set aside in the headline and
  the number set aside is reported per treebank.

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

- **Part A: trees.** Read every treebank (`conllu_legere`);
  sentence and token counts against FONTES.md; trees validated; the
  two projectivity definitions agree on every tree; hand-built trees
  with known crossings, gap degree and well-nestedness.
- **Part B: the crossing census.** Per treebank: projective share;
  crossings per sentence (distribution, mean as an exact rational);
  non-projective arc share; observed / expected crossings under random
  order (exact), overall and by sentence length; planar core deficit.
- **Part C: which relations cross, and do they agree.** Per relation
  (amod, det, nmod, obj, obl, nsubj, advmod, ...): its share of all arcs
  vs its share of non-projective arcs; for non-projective arcs vs all
  arcs (and non-planar arcs beside), the share with Case / Gender /
  Number agreement; the same for
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
- Projective by crossings (root arc included) == projective by
  interval yields, on every tree (two independent definitions); and the
  hierarchy projective => planar => well-nested holds on every tree.
- Kuhlmann and Nivre's Figure 3 (gap degrees 0, 1, 2; edge degrees 0,
  1, 1; the third not well-nested) and Figure 2a (planar, not
  projective) reproduced as hand trees.
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
1. **T1a:** the house libraries `conllu` and `dependentia` (API
   sketch below), their suites, plants; oratio switched to `conllu`,
   oratio's copy deleted; owes the oratio gate.
   *T1a done 2026-10-17:* sketch approved by Fran as written (plus
   `dependentia_omittere` refuses an omitted root; counters s64);
   suites dependentia (90 checks: every tree n <= 6 against brute
   oracles, Cayley and projective counts, the 1/3 law over all orders)
   and conllu (109, the five vendored treebanks against FONTES.md);
   23 plants red; worklogs lib/dependentia.worklog.md,
   lib/conllu.worklog.md.
   *T1a review 2026-10-17:* independent oracle, 0 mismatches (all trees
   n <= 7, 20,000 random, nine treebanks); H 0, M 1, L 5, N 4, all
   fixed: "non-projective arc" defined (above) with per-arc measures
   `dependentia_arcus`; `dependentia_omittere` returns a status
   (`DependentiaOmissa`); PUNCT-root rule stated; oracle for edge
   degree made independent; planar and well-nested counts pinned.
   **T1b:** Parts A and B, checks, plants.
2. **T2:** Parts C and D, checks, plants.
3. **T3:** findings.md, claims.md, ledger, archive.sh.
4. **T4:** independent review with its own oracle, then fixes.

## Task 0 findings (2026-10-17)

- **Definitions** checked against Kuhlmann and Nivre 2006 (text above,
  with definition numbers). Correction to the draft: projective and
  planar are different (the draft counted crossings with the root arc,
  which is projectivity, and called it planar); both are now reported.
- **Building:** aedilis builds a knotapel demo that includes
  `oratio/fontes/oratio_conllu.h` (a probe resolved oratio_conllu.c and
  oratio_vocabularium.c, built and read a sentence).
- **Freezing: archive.sh REFUSES** such a demo: its conflator accepts
  closure files only from include/, lib/ and the client roots
  `RADICES_CLIENTIUM = (materia/fontes toml/fontes)`
  (tools/corpus_infixum.sh:47), which must equal silex's
  `SILEX_RADICES_CLIENTIUM` (lib/silex.c; briar uses it). Widening the
  roots is out of proportion for a demo; an archive-only root would
  leave oratio's file-local statics unrenamed (the statics table comes
  from the same roots). So decision 2 ("beside oratio's reader") needs
  revisiting; options in "Open for Fran" 4.

## API sketch (decision 4 (a); awaiting Fran's approval)

Two house libraries. `dependentia` knows nothing of CoNLL-U (a head
array in, measures out); `conllu` is the format reader and builds a
`DependentiaArbor` from a sentence. The name: the Modistae (Thomas of
Erfurt, *Grammatica speculativa*) built every construction from a
*dependens* and a *terminans*, so dependent and head are already
scholastic grammar terms.

`include/dependentia.h`, `lib/dependentia.c`:

    nomen structura {
                       i32  numerus;   /* verba 1..numerus */
        constans       i32* capita;    /* capita[k-1] = caput verbi k;
                                          0 = radix artificialis */
    } DependentiaArbor;

    nomen enumeratio {
        DEPENDENTIA_SANA = ZEPHYRUM,
        DEPENDENTIA_VACUA,          /* numerus 0 */
        DEPENDENTIA_CAPUT_EXTRA,    /* caput extra 0..numerus, aut sui */
        DEPENDENTIA_RADIX_NULLA,
        DEPENDENTIA_RADICES_PLURES,
        DEPENDENTIA_CYCLUS
    } DependentiaStatus;

    DependentiaStatus dependentia_validare (constans DependentiaArbor*);

    /* Arbor sine verbis notatis (omittere[k-1]); reliqua ordine
     * renumerantur, filii verbi omissi capiti eius adhaerent
     * (*contractiones = filii translati). Arbor sana requiritur. */
    b32 dependentia_omittere (Piscina*, constans DependentiaArbor*,
        constans b32* omittere, DependentiaArbor* exitus,
        i32* contractiones);

    nomen structura {
        i64 cruces;           /* paria arcuum realium se secantium */
        i64 cruces_radicis;   /* arcus reales arcum radicis secantes */
        i32 arcus_secti;      /* arcus reales in cruce aliqua */
        i64 paria_disiuncta;  /* paria sine verbo communi: ordine
                                 fortuito E[cruces] = paria / 3 */
        b32 planaris;         /* cruces == 0 (Def. 4) */
        b32 proiectiva;       /* et cruces_radicis == 0 (Def. 3) */
    } DependentiaCruces;

    b32 dependentia_cruces (Piscina*, constans DependentiaArbor*,
        DependentiaCruces* exitus);

    nomen structura {
        i32 gradus_lacunarum;  /* Def. 7 */
        i32 gradus_arcuum;     /* Def. 9 */
        b32 bene_nidificata;   /* Def. 8 */
        b32 proiectiva;        /* proventus omnes intervalla (Def. 3) */
    } DependentiaMensura;

    /* lacunae[k-1] = lacunae proventus verbi k (aut NIHIL) */
    b32 dependentia_mensurare (Piscina*, constans DependentiaArbor*,
        i32* lacunae, DependentiaMensura* exitus);

    /* plurimi arcus reales sine cruce; -1 = memoria */
    s32 dependentia_nucleus_planaris (Piscina*,
        constans DependentiaArbor*);

    /* graphum crucium: componens[k-1] = componens arcus in verbum k
     * (singuli inclusi); redde numerum componentium; -1 = memoria */
    s32 dependentia_componentes (Piscina*, constans DependentiaArbor*,
        i32* componens);

The two projectivity definitions live in different functions on
purpose (crossings with the root arc in `dependentia_cruces`, interval
yields in `dependentia_mensurare`), so the check compares independent
code.

`include/conllu.h`, `lib/conllu.c` (moved from
`oratio/fontes/oratio_conllu.{h,c}`; behaviour unchanged, the oratio
prefix dropped):

    nomen structura {
                       i32  linea;
        constans character* causa;
    } ConlluVitium;           /* loco OratioVocabulariumVitium */

    ConlluLexema              /* campi ut nunc, et: */
        s32 caput;            /* HEAD numerus; -1 si '_' aut non numerus */
    ConlluSententia           /* ut nunc */

    Xar*   conllu_legere (Piscina*, chorda fons, ConlluVitium*);
    chorda conllu_textus (Piscina*, constans ConlluSententia*);
    b32    conllu_spatium_post (chorda misc);

    /* FEATS: valor clavis ("Case" -> "Nom"); vacua si absens */
    chorda conllu_proprietas (chorda feats, constans character* clavis);

    nomen structura {
        DependentiaArbor       arbor;
        constans ConlluLexema** verba;  /* verba[k-1] = lexema verbi k */
    } ConlluArbor;

    /* verba syntactica (rangae omissae); FALSUM si ID non 1..n
     * consecutivi aut memoria; arbor NON validatur */
    b32 conllu_arbor (Piscina*, constans ConlluSententia*,
        ConlluArbor* exitus);

oratio's three consumers (oratio_oraculum.{h,c},
instrumenta/oraculum.c, probatio_oratio_oraculum.c) switch names; they
read only `vitium.linea` and `vitium.causa`, so the new error type is a
drop-in. The reader's own tests move from probatio_oratio_oraculum.c
to `probationes/probatio_conllu.c`; `probationes/probatio_dependentia.c`
holds the hand trees (Kuhlmann and Nivre's Figures 2a and 3), the
planar core against brute force, the 1/3 law by enumerating every
order of small trees, and reversal invariance.

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
4. **(task 0) Where the reader and the measures live, given that
   archive.sh cannot freeze oratio/fontes.**
   (a) Proposed: MOVE the CoNLL-U reader into the house tier
   (`include/` + `lib/`, its own small error type instead of
   OratioVocabulariumVitium, which today drags in oratio_vocabularium.c),
   and put the tree measures beside it in the house tier (input: a head
   array, independent of CoNLL-U). oratio's three consumers
   (oratio_oraculum.c, instrumenta/oraculum.c, the oraculum test) switch
   to it; owes the oratio gate. A data-format reader is house-tier
   material like json and toml, and one home is kept.
   (b) The same, but COPY the reader and leave oratio untouched for now
   (a ledger wish to switch oratio later): quicker, temporary
   duplication.
   (c) Keep decision 2 as made and do not freeze D130 (live only,
   findings cite commits): breaks the knotapel freeze rule.
