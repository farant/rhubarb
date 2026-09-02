# De Perficiendo — Rhubarb Performance Sententiae

Addressable claims on where silva's time and memory actually go, and
which repairs pay for themselves without a greenfield rewrite.

## Status

```
STATUS:   DRAFT — addresses not yet frozen; reorder freely until
          first commit to the repo.
SCOPE:    silva/ parse path and its Tier 0 dependencies (piscina,
          xar). Excludes officina, apps.
BASIS:    Measurements taken 2026-07-29 against commit 162b3a9.
```

## Sigla

```
RP    this document (De Perficiendo)
SC    silva/CLAUDE.md
SPL   silva/phase-log.md
MAP   MAP.txt
AW    tools/aedilis.worklog.md
```

## Grades used

```
[definition]   a term fixed for this document
[observed]     measured directly; number reproducible
[theorem]      follows from an [observed] claim by arithmetic
[conjecture]   projected, not measured
[principle]    a rule governing which repairs to accept
[disputed]     genuinely contested; see the objection slot
```

---

## Part I — Method and ground

```
1. What was measured, and how?
1.1. The amalgam silva/amalgama/silva.c was compiled standalone on
     Linux x86-64 with gcc -O2 -std=gnu89, and driven through the
     public API (silva_c89_parsare, silva_lexare) by purpose-built
     harnesses. [definition]
1.1.1. Memory figures come from two arena counters: handed-out
       bytes (sum of block offsets) and committed bytes (sum of
       block capacities). Their difference is idle memory.
       [definition]
1.1.2. Timing is the mean of 20 runs per file, arena construction
       and destruction excluded from the timed region. [definition]
1.1.3. The corpus is lib/*.c from this repo: three named files for
       detail, and the 25 smallest for aggregates. [definition]
1.2. Every repair claimed as behaviour-preserving in Part II was
     applied to a patched amalgam and the parse output compared
     against baseline across 25 files — token counts, error counts,
     success flags — and found byte-identical. [observed]
1.3. These measurements are Linux/gcc; the project targets
     macOS/clang. LP64 layouts should agree, but every sizeof in
     this document should be re-confirmed on the real toolchain
     before anything is committed. [principle]

2. What is the baseline?
2.1. Committed memory runs roughly 950x-1070x source size.
     [observed]
2.1.1. lib/piscina.c    13,276 B src -> 12.00 MB committed,
                                        9.97 MB handed out
2.1.2. lib/json.c       42,286 B src -> 39.01 MB committed,
                                       33.94 MB handed out
2.1.3. lib/stml.c      112,364 B src -> 120.01 MB committed,
                                       72.67 MB handed out
2.2. Throughput is roughly 1.3-2.2 MB/s. lib/stml.c parses in
     85.29 ms. [observed]
2.3. Lexing is 28-32% of parse time across the corpus. [observed]
2.4. The parse path is allocator-bound, not scan-bound. [theorem]
2.4.1. Evidence: the four constant-level repairs of Part II, which
       touch no parsing logic whatever, cut stml.c from 85.29 ms to
       49.29 ms — a 1.73x speedup. [observed]
2.4.2. Consequence: any repair that reduces allocation count is
       also a speed repair, and should be evaluated on both axes.
       [principle]

3. Where does the memory go?
3.1. Not into tokens. For lib/stml.c the entire token array,
     trivia included, is 3.28 MB of 72.67 MB handed out — 4.5%.
     [observed]
3.2. Into Xar instances. lib/stml.c creates 62,596 Xars to hold
     16,390 tokens — roughly four growable arrays per token.
     [observed]
3.2.1. sizeof(Xar) is 576 bytes, of which segmenta[64] is 512 and
       the debug field titulus[32] is 32. [observed]
3.2.2. XAR_PRIMUS_SEGMENTUM is 16, so a pointer Xar's first
       segment is 128 bytes. [observed]
3.2.3. A one-element Xar of pointers therefore costs 704 bytes to
       store 8 bytes of payload — 88x overhead. [theorem]
3.2.4. 62,596 x 704 = 44.06 MB, or 61% of stml.c's handed-out
       bytes. [theorem]
3.3. And into idle committed memory. See 4. [observed]

4. Why does lib/stml.c commit 120 MB to hand out 73 MB?
4.1. The arena's oversized-request path permanently raises the base
     block size, and never lowers it. [observed]
4.1.1. In piscina_allocare: capacitas_nova = necessaria +
       mensura_alvei_initia, followed by mensura_alvei_initia =
       capacitas_nova. The second assignment is the ratchet.
       [observed]
4.1.2. Observed block sequence for lib/stml.c: 1, 2, 3, 6, 9, 18,
       27, 54 MB. mensura_alvei_initia rose from 1 MB to 27 MB.
       [observed]
4.1.3. The final 54 MB block holds 6.67 MB. 47.34 MB — 39% of
       total footprint — is committed and idle. [observed]
4.2. The arena already ships instrumentation that would have
     surfaced this: piscina_summa_inutilis_allocatus, covered by
     probatio_piscina.c. [observed]
4.2.1. Nothing calls it. It appears in
       instrumenta/principalia/excludenda_generata.h, the harvested
       unused-function list. [observed]
4.2.2. Instrumentation that is built, tested, and never called is
       equivalent to no instrumentation. [principle]
```

---

## Part II — Tier 0 repairs (highest return)

```
5. What governs acceptance of a repair in this part?
5.1. It must not change what any silva API returns, and this must
     be demonstrated by differential run, not by argument.
     [principle]
5.2. It must be small enough to review in one sitting.
     [principle]
5.3. These four repairs together are measured, not projected.
     [observed]

6. Remove the arena block-size ratchet. [RETURN: highest]
6.1. Delete the assignment mensura_alvei_initia = capacitas_nova in
     the oversized-request branch of piscina_allocare. Let the
     oversized request take an exact-fit block; leave the base
     alone.
6.2. Effort: one line, in lib/piscina.c.
6.3. Measured: lib/stml.c 120.01 MB -> 78.03 MB committed, a 35%
     reduction. Idle 47.34 MB -> 5.36 MB. [observed]
6.4. Handed-out bytes unchanged to the byte (124.41 MB before and
     after, corpus aggregate), which is the cleanest available
     proof that only allocator policy moved. [observed]
6.5. Blast radius is the whole monorepo, since piscina is Tier 0.
     The root suite is the gate, not silva's.
6.5.1. This is a benefit, not a hazard: every project gets the
       reduction.

7. Shrink Xar's segment table. [RETURN: highest]
7.1. Reduce XAR_MAXIMUS_SEGMENTORUM from 64 to 24, or to 32 — see
     the objection at 7.obj.1.
7.2. Effort: one constant, its twin in the hand-written region of
     silva.h, and the compile-time assertion that guards their
     agreement.
7.2.1. That assertion fires the moment the two copies disagree.
       This is the drift machinery working as designed; treat it as
       a checklist, not an obstacle. [observed]
7.2.2. Because the constant lives in silva.h, this is an
       amalgamare.sh regeneration, not merely a fontes edit.
7.3. Measured, cumulative with 6: corpus 176.02 MB -> 100.00 MB
     committed; handed out 124.41 MB -> 63.86 MB. [observed]
7.4. Handed-out bytes fell 48.7% from this one constant. [theorem]
7.5. It is also a speed repair: xar_creare zeroes segmenta[] in a
     loop, so 64 pointer writes per Xar became 24, 62,596 times per
     file. [observed]
7.obj.1. Objection: this is not strictly lossless. It lowers Xar's
         maximum capacity from astronomically large to about 134M
         elements at primum 16, or 33.5M at primum 4. That is a
         behaviour change at the extreme, not a representation
         change.
7.obj.1.1. Reply: conceded, and the claim of losslessness is
           withdrawn for this item specifically. The repair should
           carry a named limit and an explicit refusal rather than
           silent truncation.
7.obj.1.2. Reply: if the bound is unwelcome, 32 segments still
           saves 256 bytes per Xar and puts the ceiling back into
           the billions. Take 32 if the argument is not worth
           having.

8. Delete Xar's debug title field. [RETURN: high]
8.1. Remove titulus[32] from Xar, and the strcpy of the literal
     "Xar Anonymus" that fills it in xar_creare.
8.2. Nothing reads the field. [observed]
8.3. Effort: two lines.
8.4. Measured, cumulative with 6 and 7 and 9: corpus 95.00 MB ->
     87.00 MB committed; handed out 59.83 -> 53.21 MB. [observed]
8.5. If the field is wanted for debugging, keep it under a
     PISCINA_DEBUG guard rather than in release builds.

9. Reduce the first-segment size for small arrays. [RETURN: high]
9.1. Reduce XAR_PRIMUS_SEGMENTUM from 16 to 4.
9.2. Justified by the size distribution: of 11,010 trivia Xars in
     lib/stml.c, 10,395 hold exactly one element and 352 hold two.
     [observed]
9.3. Measured, cumulative with 6 and 7: corpus 100.00 MB -> 95.00
     MB committed; handed out 63.86 -> 59.83 MB. [observed]
9.4. Smallest of the four returns. Take it last, and drop it
     without regret if it interacts badly with 7.obj.1.

10. What do 6 through 9 deliver together?
10.1. Corpus of 25 files: 184.01 MB -> 87.00 MB committed, a 52.7%
      reduction; handed out 124.41 MB -> 53.21 MB, 57.2%.
      [observed]
10.2. lib/stml.c: 120.01 MB -> 36.00 MB committed, a 70%
      reduction; handed out 72.67 MB -> 31.52 MB. [observed]
10.3. lib/stml.c parse time 85.29 ms -> 49.29 ms, 1.73x. [observed]
10.4. Parse output byte-identical across 25 files at every
      intermediate step. [observed]
10.5. Total edit surface: four constants and two deletions, all in
      lib/piscina.c, include/xar.h, lib/xar.c, and the silva.h
      mirror. No silva logic touched.
10.6. Landed 2026-09-02 with one design change: instead of lowering
      XAR_MAXIMUS_SEGMENTORUM (7), the segment table is allocated
      after the header and sized at creation — 24 entries by default,
      64 via xar_creare_magnum — with a loud refusal past the default
      (7.obj.1.1 satisfied: named limit, no silent truncation). First
      segment 4, debug name removed, ratchet deleted. Header 576 → 48
      bytes. [observed]
10.7. Re-measured on the house toolchain (clang, suite flags, no -O)
      with the include closure in the arena and the INPUT held fixed:
      lib/stml.c 457 → 150 MB handed out (−67%), 635 → 160 MB
      committed (−75%), idle 178 → 10 MB, parse 177 → 152 ms (−14%);
      lib/json.c 127 → 41 MB, 49 → 39 ms; lib/piscina.c 29 → 9.7 MB,
      18.5 → 9.6 ms. Allocation count rose 3% (first segment 4 splits
      more often). The memory figures match 10.1-10.2; the speedup is
      smaller than 10.3 because this build is unoptimized and the
      closure dominates the token count. [observed]
10.8. Equivalence demonstrated as 5.1 demands: arbor.sh canonical
      STML byte-identical for 154 of 156 lib/*.c, the two exceptions
      being lib/xar.c and lib/piscina.c themselves, whose text
      changed. The differential had to hold the header text fixed:
      the first attempt ran with the edited headers in every closure
      and would have flagged the corpus for the wrong reason.
      [observed]
```

---

## Part III — Scoping repairs

```
11. Build the measurement harness before optimizing further.
    [RETURN: high, and prerequisite]
11.1. Nothing in the repo reports arena statistics, so no
      performance regression can currently be detected. (see 4.2)
11.2. Add a tool — instrumenta/principalia, alongside the existing
      read-bench instruments — that parses a file and reports
      handed-out bytes, committed bytes, idle bytes, block count,
      token count, and Xar count.
11.3. Pin a small corpus with expected figures in probationes/fixa,
      so a regression fails a probatio rather than going unnoticed.
11.4. Effort: hours, given piscina_summa_inutilis_allocatus already
      exists and is tested.
11.5. Do this before Part II lands, so Part II's numbers are the
      project's own and not this document's. [principle]
11.6. Done 2026-09-02: ./silva/computus.sh <fons.c> [-nudum]
      [-machina] (silva/instrumenta/silva_computus.c) reports source
      bytes, tokens (expanded and raw), errors, handed-out, committed,
      idle, peak, block count, allocation count, and the lex-only
      figures, each measurement in its own arena with a 1 MiB initial
      block. piscina gained piscina_numerus_alveorum and
      piscina_numerus_allocationum for it. [observed]
11.7. The pin of 11.3 is probatio_silva_computus over four
      fixa/roundtrip files with -nudum, golden
      silva/probationes/fixa/computus/basis.tsv; deterministic
      figures compared exactly, times printed only. Regeneration is
      COMPUTUS_SCRIBERE=1 and must name its cause. [observed]
11.8. Re-measured on this toolchain, with the include closure:
      lib/stml.c (285,866 B) parses in 176 ms into 436 MB handed out,
      606 MB committed, 1,141,283 allocations, 11 blocks; lib/piscina.c
      (14,215 B) into 27.8 MB handed out, 39.0 MB committed, 71,656
      allocations. The 950-1070x of 2.1 is now roughly 1500-2000x with
      the closure in the arena. [observed]

12. Scope transient parse structures to their own arena region.
    [RETURN: high]
12.1. silva never creates an arena; it takes the caller's and puts
      everything in it. [observed]
12.2. Consequently the lexer output, the GLR graph-structured stack
      and forest, the committed tree, the symbol tables, the CFG,
      and the dataflow events are all live simultaneously until the
      caller destroys the arena. [theorem]
12.3. The GLR working set is dead the instant the commit pass
      finishes, and is never reclaimed. [theorem]
12.4. After the Part II repairs, lib/stml.c uses 11.87 MB at end of
      lex and 31.52 MB at end of parse. Roughly 20 MB is added by
      parsing, much of it transient. [observed]
12.5. piscina already provides PiscinaNotatio — mark and reset.
      silva uses it nowhere. [observed]
12.6. Cheapest form: mark before the GLR run, reset after commit.
      No data structure changes.
12.7. Fuller form: two arenas, permanent and scratch, with the
      scratch one reset per phase.
12.obj.1. Objection: the commit pass produces the tree from the
          forest, so nodes may alias forest storage; resetting
          would free live data.
12.obj.1.1. Reply: this must be checked before the repair, not
            assumed either way. If commissio copies, the reset is
            safe as written; if it aliases, the repair becomes
            "make commissio copy," which is a larger but still
            bounded change.
12.obj.1.2. Reply: SC states the commit pass exists precisely to
            consolidate semantics into one walk, which suggests
            copying rather than aliasing — but the code, not the
            doc, decides.

13. Const-ify the grammar and code tables. [RETURN: high]
13.1. The amalgam has zero mutable BSS globals — no lazily
      initialized caches, no counters, no global allocator state.
      [observed]
13.2. It has 15 initialized-data globals (SILVA_C89_TABULA,
      SILVA_C89_GRAMMATICA, SILVA_C89_REGISTRUM, _codices,
      _tolerabiles, _verba_aliena and others) that appear to be
      read-only in fact but are not declared constans. [observed]
13.3. Declaring them constans is a one-build experiment: if it
      compiles, silva is reentrant and the compiler has proved it.
      [theorem]
13.4. Reentrancy is the precondition for Part V. Do it early even
      if parallelism is deferred. [principle]
```

---

## Part IV — Representation repairs inside silva

```
14. Reorder SilvaToken's fields. [RETURN: moderate, effort trivial]
14.1. sizeof(SilvaToken) is 120 bytes, of which 15 are padding: 4
      after genus, 4 inside chorda, 3 after standard, 4 after
      initium_lineae. [observed]
14.2. Grouping the pointers and origo first, then the four-byte
      scalars, then standard, yields 112 bytes with no semantic
      change of any kind. [observed]
14.3. This is the only item in Part IV with no API consequence.
      Take it whenever convenient.

15. Store trivia as an exact-sized array rather than an Xar.
    [RETURN: moderate]
15.1. Every token's spatia_ante and spatia_post is an Xar*, at 704
      bytes each before the Part II repairs. [observed]
15.2. Trivia lists are built once at attach time and never appended
      to afterward, so the growable array buys nothing. [observed]
15.3. Replace Xar* with an inline pair — a pointer and a count —
      allocated exact-size from the arena.
15.4. Before Part II this was worth about 10% of the arena. After
      Part II it is worth roughly 3 MB of 87 on the corpus.
      [theorem]
15.5. The marginal value collapsed because the overhead was in the
      Xar header, not in the choice of a growable array. This is
      the general lesson of Part II. [principle]

16. Make SilvaOrigo a pointer rather than an inline union.
    [RETURN: low after Part II; effort high] [disputed]
16.1. SilvaOrigo is 40 bytes inline in every token, and its union
      is unused for source-origin tokens. [observed]
16.2. Measured across 56,686 tokens: 100.0% are SILVA_ORIGO_FONS
      with no lexicon loaded, and 89.6% with latina loaded and
      macros genuinely expanding. [observed]
16.3. Changing the field to SilvaOrigo*, with NIHIL meaning FONS,
      would save 32 bytes on roughly nine tokens in ten.
      [conjecture]
16.4. Combined with 14 and 15 and the removal of scissurae, the
      token reaches 72 bytes — and reaches it without touching
      linea or columna. [observed, by compiling the candidate
      layouts]
16.5. Token array on the corpus: 4.03 MB -> about 2.43 MB.
      [conjecture]
16.obj.1. Objection: 1.6 MB out of 87 is under 2%, and the repair
          touches all five constructors and every consumer that
          reads t->origo.genus.
16.obj.1.1. Reply: no answer. The objection stands. Defer this
            item until Part II has landed and been re-measured;
            it may never be worth its churn.

17. Remove redundant token fields. [RETURN: low, effort low]
17.1. longitudo equals valor.mensura in 56,686 of 56,686 tokens
      measured, expansion-origin tokens included. [observed]
17.2. scissurae was non-NULL in 0 of 56,686 tokens; the field's own
      comment concedes "NIHIL fere semper". [observed]
17.3. Removing longitudo alone gains nothing, because trailing
      padding absorbs it: 80 bytes before and after. [observed]
17.4. Therefore these removals are worth taking only as part of
      16, not on their own. [theorem]
17.5. scissurae, if kept, belongs in a side table keyed by token
      index rather than as a per-token pointer.
```

---

## Part V — Parallelism

```
18. Is silva's workload tractable to threads?
18.1. Partly. The division is not parser versus lexer; it is
      across-file versus within-file. [theorem]

19. What is fundamentally linear?
19.1. The preprocessor. The macro table at line N depends on every
      line before it, and conditional inclusion depends on the
      macro table. [theorem]
19.2. The GLR parse of a single translation unit, which is a
      left-to-right state machine by construction. [theorem]
19.3. Name resolution, by design. SC's oracle argument rests on
      ISO declaration-point semantics — a type is visible only if
      already registered when the ambiguity is reached — which is
      order-dependent on purpose. [theorem]
19.4. These are not defects to be engineered around. Attempting to
      parallelize them would forfeit correctness properties the
      project deliberately bought. [principle]

20. What is embarrassingly parallel? [RETURN: highest wall-clock]
20.1. Whole files. Translation units share nothing. [theorem]
20.2. This covers examen.sh over the corpus, examen_vectis.sh, the
      differential sweeps, and the root suite that currently takes
      about 137 s per SPL.
20.3. It is also what the industry does: clang does not parallelize
      within a translation unit; build systems parallelize across
      them. [observed]
20.4. The arena design already suits it. Because the piscina is
      passed in by the caller, arena-per-thread is the natural
      shape and there is no allocator contention at all — which at
      62,596 allocations per file would otherwise dominate.
      [theorem]
20.5. Precondition is 13. Given that, this is a harness change, not
      a silva change.

21. What is the middle case?
21.1. Per-function analysis. Once semantica has built the symbol
      table, CFG construction, dataflow, and most of examen are
      per-function-definition and independent. [conjecture]
21.2. This is the only form of parallelism that helps LSP latency,
      since file-level parallelism does nothing for a single file
      being edited. [theorem]
21.3. It is also the fastest-growing share of runtime, given the
      recent commit history. (cf. SPL, entries from 2026-07-27)
21.4. Shape: fork-join over function definitions, arena per worker,
      diagnostics merged into a shared list at join.
21.5. Defer until 12 has established phase boundaries; the two
      repairs want the same seam.
```

---

## Part VI — What not to do

```
22. Should SIMD be applied to the parser?
22.1. No, not as the next lever, and never to the parser proper.
      [principle]
22.2. Vectorization applies to stage-one scanning — the simdjson
      model, where a SIMD pass builds a structural bitmap and a
      scalar stage two walks it. [observed]
22.3. LR and GLR are data-dependent state machines; every step's
      branch target depends on the previous step's result. Nobody
      has made vectorized LR parsing pay. [observed]
22.4. Therefore the ceiling is the lex share, about 30%. If lexing
      became free, lib/stml.c would go from 49.29 ms to 33.67 ms.
      [theorem]
22.5. A realistic 2-3x scanner buys 10-15% overall — against the
      73% already collected by four constants. [theorem]
22.6. Intrinsics are not C89 and -pedantic -Werror will reject
      them. The honest option for this codebase is SWAR over
      unsigned long, which is portable C89 and worth 2-4x on tight
      scan loops.
22.7. If it is ever done, do it in this order: line-table
      construction first (it makes deriving line and column from
      byte offset cheap), then trivia skipping, then identifier
      runs.
22.8. C phase 2 line splicing means the byte stream is not clean
      the way JSON's is, so a speculate-then-repair scanner is
      required — but the measured scissurae count of 0 in 56,686
      tokens says the repair path essentially never fires, which is
      the best case for that shape. [observed]

23. Should line and column be derived from byte offset?
23.1. Not yet, and not as part of the memory work. [principle]
23.2. Every other item in Part IV is free: the same information in
      fewer bytes. This one trades a field read for a binary search
      against a line table. [theorem]
23.3. For an LSP doing position lookups constantly, that may be the
      wrong trade, and it is not needed to reach 72 bytes. (see
      16.4)

24. Should the representation be rebuilt structure-of-arrays?
24.1. Not now. Out of scope for this document by construction.
      [principle]
24.2. The case for it is real and is recorded elsewhere; the case
      against it is that Part II and Part III cost days and deliver
      a large fraction of the available return.
24.3. Re-open the question only after 6-13 have landed and been
      re-measured against the harness of 11. If the result is near
      200x source size and the LSP feels responsive, the greenfield
      case is materially weaker. [principle]
```

---

## Part VII — Ordering

```
25. In what order should this be done?
25.1. First 11, the harness, so every subsequent claim becomes the
      project's own measurement.
25.2. Then 6, 7, 8, 9 — the four constants — as one reviewable
      change with a root-suite gate.
25.3. Then 13, const-ification, which is a single build and unlocks
      Part V.
25.4. Then 12, phase-scoped arenas, and re-measure.
25.5. Then 20, file-level parallelism in the harnesses, which is
      the largest wall-clock win and needs no C.
25.6. Then 14, the token reorder, whenever convenient.
25.7. Stop there and re-read the numbers before considering 15, 16,
      21, or 24. [principle]

26. When should this document be revisited?
26.1. When the harness of 11 disagrees with any figure in Part I or
      Part II. Amend by appending, never by renumbering.
26.2. When semantica's share of runtime exceeds the parse share, at
      which point Part V article 21 becomes the leading item rather
      than a deferred one.
26.obj.1. Objection: the codebase is in exploratory mode and none
          of this is urgent.
26.obj.1.1. Reply: agreed, and that is the correct posture. The
            value of this document while exploration continues is
            that it records where the costs are, so that the
            decision to pay them is made deliberately rather than
            by default. Nothing here needs doing today.
```
