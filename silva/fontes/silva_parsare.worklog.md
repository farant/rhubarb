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
