# silva_parsare worklog

## 2026-11-03 — conditional weaving: element extents once per pass (budget silvae)

**Found by** the materia DOD audit (a read-only investigation with a scratch prototype; scratchpad materia_dod/report_materia.md).
- Region weaving (`_regiones_texere` -> `_regionem_texere`) re-computed the extent of EVERY top-level element, with a full subtree walk (`_extentum_valoris`), for EVERY conditional region.
- Cost was regions x tree: 9.6% of parse-only percursus, and ~92% of vendor/bearssl_amalgama.c (15.2 G instructions for one file).

**Why caching-free precomputation is exact.**
- An element's extent depends only on its own subtree, and weaving never changes a subtree: it wraps congregated elements in a NEW conditional node.
- So `TexturaStatus.extenta` is a series parallel to `st->radix`, computed once at the start of `_texere` (`_extenta_computare`).
- Each successful weave rebuilds it in lockstep with the new root list: the conditional node's extent is computed ONCE (the node is complete by then), and the others are copied from the old series.
- Failure paths return before the relist, so list and series stay aligned.
- If any allocation fails, `extenta = NIHIL` and `_extentum_elementi` computes per region as before (never a wrong answer).

**Bitset.**
- `congreganda` was a b32 per element, freshly allocated for EVERY region and never freed.
- Now it is one i32-word bitset per pass (`_congreganda_parare`): it grows only when the list does, and is cleared per region.
- `_congregandum_ponere` / `_congregandum_est`. The first name I picked, `_bitum_ponere`, already exists in silva_c89_fluxus_datorum.c, and the amalgam is one TU. Check new static names with `grep -w` over lib/, silva/fontes/, materia/fontes/ before the suite.

**Bench.**
- bearssl alone: 15.24 -> 3.05 G instructions (-80%); peak 297.3 -> 295.0 MB (the bitset).
- Whole repo (silva_mensura): parse-only 262.0 -> 237.5 G instructions (-9.4%), cycles 71.3 -> 61.3 G; -semantica 660.7 -> 636.2 G.

**Equivalence.** aequivalentia -conferre over the usual 483-file corpus PLUS the audit's 76 `#if`-heaviest files (bearssl, stb_image.h, sqlite3.h, knotapel demos, …): 558/559 byte-identical (silva_parsare.c itself changed).

**Plants.**
- Against the existing suites:
  - A (conditional extent never computed): caught by conditionalis.
  - B (conditional extent not appended, so the series is misaligned): caught only by arbor_parsura.
  - C (bitset not cleared between regions): caught only by scribere.
- New test "regiones multas" in probatio_silva_conditionalis:
  - 40 declarations, a region holding 40 more (bitset indices 40..79 cross words), a second region, and a nested pair: four weaves, three relists.
  - A, B, C and E (all bits into word 0) are now red there. The test passes on the old code too.
- D (series length not updated after a relist) is an EQUIVALENT mutant:
  - If the list shrinks, every later index is still below the new length.
  - If it grows (nothing congregated), the last index is past the stale count and falls back to computing.
  - The update stays, for clarity.

**Left (not done):** each woven region still scans and copies the whole root list (O(regions x elements), but cheap per element). Elements are mostly in document order, so a merge or binary search over extents could replace the scan, but headers spliced inline and opaque extents break monotonicity. Unmeasured, and the rest of bearssl's 3.05 G is ordinary parsing.

## 2026-11-07 - budget item 9: parse-side census (measurement only)

Fresh 1 ms `sample` of `percursus -semantica` at a2c765a57 (probe built like
the real tool: separate objects, VEXILLA -O2 -g), 27.7 k samples. Stage
attribution by (function, line) markers in the call graph - scripts in the
session scratchpad s23/ (gradus.py, intra.py, vocantes.py).

- Parse side 64.5%: own-file parse 56.5% + lexicon re-parse 8.0%.
  - GLR + construction 22.7% (silva_glr_parsare self 6.4%, construere 2.7%,
    _nodum_processare 2.4%).
  - Directives 16.7%: lexing INCLUDED HEADERS 7.0% + their trivia attach 2.6%
    (main-file lexing is only 2.1% - headers are re-lexed in every TU, ~4.5x
    the main-file lex), macro-table events (_eventum_scribere:230) 2.0%.
  - Expansion 6.6%: macro lookup _generatio_interna:3117 ~2.5%.
  - _contextum_applicare 4.1%: every TU re-registers every repo header into
    its expansion (silva_includendum_praebere 2.6%).
  - praecommissio 4.2%, commissio 2.9%.
- Semantics 22.2%; percursus own work 9.2% (file reads, _typedefs_numerare +
  _expressiones_numerare report walks 2.8%); round-trip write 3.5%.
- Cross-cutting: xar accessors 20% of all self time (obtinere 12.5%, addere
  6.1%, numerus 1.6%) spread over ~20 sites - call overhead on an O(1) path,
  not an algorithm; clock() (getrusage) 2.0%.

Lexicon re-parse: silva_c89_parsare_cum_contextu parses contextus->lexica
(the composed systema text) into the per-TU oracle for every file, while
percursus has ALREADY filled that oracle from the cached praeparatio
(silva_c89_semantica_oraculum_augere). Possibly redundant for percursus -
unproven; needs a differential before anything changes.

LTO experiment: the same sources built with -flto=thin -> 337.5 -> 285.4 G
instructions (-15.4%), wall 32.1 -> 26.6 s, report identical (2 runs each).
xar_obtinere vanishes from the profile (inlined); xar_addere stays. Cost:
ThinLTO link of percursus ~3 s wall / 10 s CPU vs ~0.2 s. A vexilla.sh flag
touches every build - Fran's decision.
