> Raw analyst report, batch C of budget B1 (2026-10-27/28), filed verbatim as evidence for ../budget-fusionis.md. Paths under `scratchpad/b1/` were session scratch (profiles, prototypes) and no longer exist.

# Batch C — core primitives (xar, tabula_dispersa, piscina, chorda, aedificator, internamentum)

All prototypes, benches and test runs are in `scratchpad/b1/C/`. Nothing in the repo was touched.
The machine was loaded the whole time (load avg ~13 on 8 cores, other agents profiling), so read
every number below as a before/after ratio from the same run, not as an absolute.

## How the build compiles these (this settles the "-O level" question)
- `tools/vexilla.sh` is the single source of flags: `-std=c89 … -O2 -g`. `build/clausurae/mandata.tsv`
  shows the same `-O2 -g` for every object. So there's no `-O0` lever: the tests already run optimised.
- There's no LTO. Every `lib/*.c` is its own TU, so `xar_numerus` (a one-line accessor) is a real
  call across TUs. That's why it has 1,000–1,900 self samples per profile.
- The silva **amalgam** (`silva/amalgama/silva.c`, used by aedilis etc.) is a single TU. There,
  `silva_xar_obtinere` has only 17 self samples of 9,983 in prof_aed: clang already inlines it. This
  backs the inline idea below. The `probationes_silva/*` corpus suites link plain `lib/xar.o`
  (their symbols are `xar_obtinere`, with no prefix), so they pay the full call.
- Share of these primitives in busy (non-blocked) samples: lint 54%, canon_corpus 57%, obsoleta 51%,
  inutilia 51%, arbor_plagula 47% (`C/selfsum.py`). Half the CPU of the silva corpus suites is the
  core primitives.

Estimate convention. The **core-heavy pool** is the leaf-minutes of the profiled-style suites from
`briefing_numeri.txt`: canon_corpus 0.78, obsoleta 0.64, lint 0.62, arbor_plagula 0.60+1.33, inutilia
0.51, md_stml 0.68+0.41, md_canon 0.36+0.16, toml_totalitas 0.24 ≈ **6.3 min**, plus porta_aedilis 2.28
(amalgam) where relevant. I take about 85% of it as CPU. The **broad pool** is the frigida:* runs,
radix, briar and so on, roughly 20 min. Their composition is unknown, so I credit them at a quarter of
the pool rate and label that a guess.

---

## 1. Header-inline fast paths for `xar_obtinere`, `xar_numerus`, `xar_addere` — est. **0.5–0.8 min/merge**
- **Where:** `include/xar.h` (new block at the end) and `lib/xar.c` (the real definitions stay;
  `#define XAR_IMPLEMENTATIO` before the include, or `(xar_obtinere)`-style parenthesised definitions).
- **Evidence:** xar_obtinere self 7.9–11.5% of busy and xar_numerus 2.0–5.1% in the silva suites;
  xar_addere about 3%. Line attribution (`C/lineae.py`) puts xar_obtinere's samples at `+0` (function
  entry, xar.c:450) and xar.c:469. xar_numerus's 608 samples sit on its single line. Most of the cost is
  call overhead, plus the fact that a cross-TU call can't be hoisted out of the house loop shape
  `per (i < xar_numerus(x)) xar_obtinere(x, i)`.
- **Mechanism:** under `#if (defined(__clang__)||defined(__GNUC__)) && !defined(XAR_SINE_INLINEA)`,
  define `staticus __inline__` functions `_xar_numerus_inl`, `_xar_obtinere_inl` and `_xar_addere_inl`,
  then `#define xar_obtinere(x,i) _xar_obtinere_inl((x),(i))` and the same for the other two.
  - The inline obtinere does: the null and bounds check; the first-segment path; the power-of-two
    `__builtin_clz` path. Any other first size goes to `(xar_obtinere)(x,i)`.
  - The inline addere handles "segment already allocated" and otherwise calls `(xar_addere)(x)`.
  - These are functions, not text macros, so every argument is evaluated exactly once.
  - Without parentheses after the name, `xar_obtinere` still names the real external function, so
    function pointers keep working. I checked this: `&xar_obtinere` and bare `xar_obtinere` as a
    function pointer both compile under the full -Werror flag set. An unused include also compiles,
    with no `-Wunused-function`.
- **Measured** (`tools/xar_mensura.c` copied to scratch, best of 5, same run):

  | | seriatim | lexemata | temere | addere |
  |---|---|---|---|---|
  | base -O2 | 4.7 ns | 4.9 ns | 3.8 ns | 5.4 ns |
  | header-inline | 2.1 ns | 2.2 ns | 2.0 ns | 3.5 ns |
  | ThinLTO (reference) | 1.7 ns | 1.8 ns | 1.7 ns | 3.4 ns |

  Header-inline gets about 85–90% of LTO's gain with no change to the build.
- **Estimate:** (obtinere+numerus ≈ 13% × 55%) + (addere 3% × 35%) ≈ 8% of busy. Core-heavy pool
  (aedilis excluded, the amalgam already inlines): 6.3 × 0.85 × 8% ≈ 0.43 min. Broad pool:
  20 × 0.85 × 8% / 4 ≈ 0.3 min (guess).
- **API impact:** none at source level: same names, signatures and semantics. It's ABI-neutral (the
  real symbols still exist). Flag it anyway: `xar_obtinere`, `xar_numerus` and `xar_addere` become
  function-like macros as well as functions.
- **Maintainability risk: medium, all of it in tooling.**
  - silva and legati symbol and caller queries (`vocantes xar_obtinere`), `renominare.sh`, and the
    examen rules will see a macro wrapping an inline function. Check that examen doesn't refuse
    `__inline__` or `__builtin_clz` in a header; xar.c already uses `__builtin_clz` under the same guard.
  - The amalgamators rename by prefix at token level. The officina rule (renaming `tabula_`, `xar_` and
    similar prefixes) would rename the macro and helper consistently, but the `_xar_*_inl` helpers must
    not start with a renamed prefix in a way that breaks them. Test this.
  - The hand-kept `SilvaXar` and `OfficinaXar` copies don't need the inline: the amalgams are single
    TUs and already inline. They just must not break.
  - Fallback: `-DXAR_SINE_INLINEA` gives the old path, and any non-GNU compiler gets plain calls.
- **Effort:** S–M (about 60 lines, plus a tooling check).
- **Verify:**
  - `probationes/probatio_xar.c` passes against the prototype (scratch build `C/px_inl`:
    "OMNIA PRAETERIERUNT").
  - **Plant:** an off-by-one in the inline non-first-segment offset (`+ I`) made `C/px_plant` exit 139,
    with FRACTA in "iterator trans segmenta" and in quaerere_binarie. The tests catch it.
  - Also run probatio_xar with `-DXAR_SINE_INLINEA` and `-DXAR_SINE_INTRINSECIS`, so all three paths
    stay tested, and add one section that compares `(xar_obtinere)` with `xar_obtinere` over first
    sizes 1, 3, 4 and 16.

## 2. `piscina` allocation fast path (no 64-bit divide, no 12-register prologue) — est. **0.3–0.45 min/merge**
- **Where:** `lib/piscina.c:_allocare_interna` (190–310) and `_sedem_computare` (66–90).
- **Evidence:** _allocare_interna self is 4.1–7.0% of busy in the silva suites and 879/9983 in aedilis.
  The disassembly shows a `udiv` from `promotus % ordinatio` on every call. The function also saves 12
  registers on every call, because the rare new-alveus path lives in the same function and isn't
  inlined into the four public wrappers.
- **Mechanism:** rename the current body `_allocare_tarde` and mark it `__attribute__((noinline))`
  under a clang/gcc guard. The new small `_allocare_interna` covers the common case: piscina non-null,
  mensura > 0, ordinatio a power of two, and the result fits the current alveus. In that case it does
  `promotus = (offset + o-1) & ~(o-1)` with explicit wrap checks (`promotus >= offset`,
  `necessaria >= promotus`, `necessaria <= capacitas`), the same bookkeeping (usus_currens,
  maximus_usus, numerus_allocationum) and a bump. Everything else goes to the slow path, so semantics
  don't change. clang then inlines the fast path into `piscina_allocare` with the constant VIII
  (checked in the disassembly: about 25 instructions, no udiv, no stack frame).
- **Measured** (`C/pb.c`, 4M allocations of 16–47 B): **10.4 → 4.0 ns/alloc (2.6x)**, stable over 3
  runs. Replacing the modulo with a mask alone, without the split, gave no measurable gain. The
  prologue and the divide together are the cost.
- **Estimate:** about 60% of 4–7% ≈ 3% of busy. Core-heavy pool plus aedilis: 8.6 × 0.85 × 3%
  ≈ 0.22 min. Broad pool (piscina is under everything): about 0.15 min.
- **API:** none. **Risk:** low; the slow path is the old code verbatim. **Effort:** S.
- **Verify:** `probatio_piscina.c` gives 168/168 against the prototype (`C/pp_rap`).
  - Plant A, dropping `numerus_allocationum += I` in the fast path: 3 FRACTA.
  - Plant C, aligning to 4 instead of `ordinatio`: 7 FRACTA (the ordinatum `% XVI` check and the
    summa_usus checks).
  - Note: a plant that only *misaligns downward* is silently routed to the slow path by the wrap
    guard. That's correct behaviour, not a gap.
  - The computus golds (`COMPUTUS_SCRIBERE`) must stay identical. Nothing about the counts changes,
    so they're a good gate for this change.

## 3. `chorda_aequalis_literis`: compare while scanning, no full strlen — est. **0.2–0.3 min/merge**
- **Where:** `lib/chorda.c:chorda_aequalis_literis` (327–358).
- **Evidence:** self is 3.9–6.2% of busy in the exemplaria suites and canon_corpus. **59% of its
  samples are on line 341**, the `len++` strlen loop. The hot callers are if-chains against
  literals, for example `_invariata_numerare` (the `"EXEMPLAR" || "PER" || … || "TRANSPARENTIA"`
  chain in the three exemplaria tests) and `stml_macros.c:_liberum_expandere`. Every literal in the
  chain is strlen'd in full before the usually-first-byte mismatch.
- **Mechanism:** `per (i < s.mensura) { si (cstr[i]=='\0' || s.datum[i]!=cstr[i]) redde FALSUM; }
  redde cstr[s.mensura]=='\0';`. The semantics are identical, including embedded NULs in `s`, an
  empty or NIHIL datum with mensura 0, and `cstr==NIHIL`. Checked by an exhaustive comparison in
  `C/cal.c`.
- **Measured:** **8.0 → 3.1 ns/call (2.5x)** on the 7-literal chain.
- **Estimate:** 60% of about 4.5% ≈ 2.7% of busy, core-heavy 6.3 × 0.85 × 2.7% ≈ 0.15 min, plus
  broad pool about 0.08 min.
- **API:** none. **Risk:** very low. **Effort:** S.
- **Verify:** **`probatio_chorda.c` has zero tests of `chorda_aequalis_literis`.** It passed 271/271
  against both versions, and that proves nothing about this function. Add a section first:
  - exact match;
  - a prefix both ways (`"PER"` against chorda `"PERGE"`, and chorda `"PER"` against `"PERGE"`);
  - an embedded NUL in the chorda;
  - an empty chorda against `""` and against `"a"`;
  - a NIHIL datum with mensura 0;
  - a NIHIL cstr.

  **Plant:** end with `redde VERUM` instead of checking `cstr[s.mensura]`. That's the prefix bug, and
  only the new prefix test catches it.

## 4. Hash-table growth: re-place with the cached hash — est. **0.15–0.25 min/merge**
- **Where:** `lib/tabula_dispersa.c:_tabula_dispersa_crescere` (214–262).
- **Evidence:** growth re-inserts call the full `tabula_dispersa_inserere`. That means the indirect
  `friatio` call, rehashing the key through cross-TU `friatio_fnv1a`, the float load-factor test,
  probing with comparisons, and stats writes. Measured with `C/growth.py`, growth is **37–41% of all
  inserere inclusive time** in every silva profile (lint 1421 of 3467; aedilis 586 of 1585) and
  12–20% of all hashing.
- **Mechanism:** for each occupied old slot, go to `hash_cachatum & mask`, linear-probe to the first
  VACUUM, copy the slot, and bump numerus (keep collisiones and distantia updates if wanted). No
  hashing, no comparisons: keys are distinct and the fresh table has no tombstones. **The slot chosen
  is exactly the one inserere would choose**, so layout and iteration order are byte-identical. The
  hash function is unchanged.
- **Measured:** about 10–15% on a small-table, growth-heavy bench (`C/tb.c`, modus 1). The bench is
  dominated by cache misses, so the real gain is in the profile arithmetic.
- **Estimate:** about 75% of growth (≈ 2.5% of busy in lint, 5.9% of aedilis samples) ≈ 1.7% of busy.
  Core-heavy 6.3 × 0.85 × 1.7% ≈ 0.09 min, aedilis 2.28 × 4% ≈ 0.09 min, broad pool about 0.05 min.
- **API:** none. **Risk:** low. **Effort:** S.
- **Verify:** `probatio_tabula_dispersa` 168/168 and `probatio_internamentum` 47/47 against the
  prototype. **Plant:** flipping a bit of `hash_cachatum` during re-placement gave 28 FRACTA.

## 5. `chorda_aedificator` byte and small-append fast path — est. **~0.05–0.1 min/merge**
- **Where:** `lib/chorda_aedificator.c:_appendere_interna` (74–115) and `appendere_character` (326).
- **Evidence:** _appendere_interna self is 2.2–2.7% of busy; 444/1,940 of its inclusive samples come
  from `appendere_character`. Each one does a 1-byte `memcpy` through `DYLD-STUB$$memcpy` (406
  stub samples in arbor_plagula). `appendere_literis` calls `strlen` and then copies, which is 166
  of 292 `_platform_strlen` samples in inutilia.
- **Mechanism:** `appendere_character`: if `offset < capacitas`, store `buffer[offset++] = c`
  directly, otherwise the old path. In `_appendere_interna`, use a byte loop for `mensura <= XVI`
  and `memcpy` above that.
- **Estimate:** about 1% of busy at best, ≈ 0.05 min core-heavy plus a little elsewhere.
  **API:** none. **Risk:** low. **Effort:** S.
- **Verify:** `probatio_chorda_aedificator.c`, including the "APPENDIX SUI" self-append case (keep the
  `intra` logic on the growth path). **Plant:** drop the `offset++`.

## 6. `tabula_dispersa_invenire_literis`: one strlen, not one per probe hit — est. **~0.03–0.05 min**
- **Where:** `lib/tabula_dispersa.c:_friare_literis` and `_aequalis_literis_chorda` (19–52).
- **Mechanism:** compute `strlen` once in `invenire_literis`, then pass the length down to the slot
  search and use `mensura` equality plus `memcmp`. **API:** none. **Risk:** very low. **Effort:** S.
- **Evidence:** invenire_literis self is 0.8–1.2% of busy, plus its share of `_platform_strlen`.

## 7. `chorda_internare` miss path: hash and probe once, not twice — est. **~0.03 min**
- **Where:** `lib/internamentum.c:chorda_internare` (invenire, then inserere on a miss).
- **Mechanism:** an internal "find or reserve slot" in tabula_dispersa used by internare. This is an
  **API addition**: a new public function such as `tabula_dispersa_invenire_vel_inserere`, or a
  private header shared by the two files. Small gain: inserere is 260 of 1,371 internare samples in
  inutilia. Do it only if a find-or-insert is wanted elsewhere anyway.

## 8. (Larger, unmeasured) ThinLTO for test links — potential **>1 min** of run time, unknown build cost
- `-flto=thin` alone made xar 2.8x faster in the bench, and it would also inline tabula, friatio,
  chorda and piscina across TUs.
- But the link of every test binary does the backend codegen for all of its modules. With hundreds
  of test binaries linking large closures, and the frigida cold builds being a large share of the
  merge, link time could eat the gain unless the ThinLTO cache (`-Wl,-cache_path_lto,…`) hits well.
- Ideas 1–3 capture most of the measurable gain without touching the build. Measure LTO only
  after them, and only on the 5–10 heaviest suites (see open questions).

---

## Checked, not worth it
- **Changing the `-O` level:** already `-O2` everywhere (vexilla.sh, mandata.tsv). No `-O0` lever.
- **Inlining FNV inside tabula_dispersa** to skip the function-pointer call and the cross-TU
  `friatio_fnv1a`: no measurable change in `C/tb.c`. The indirect call is cheap next to the serial
  per-byte multiply chain and the cache misses.
- **A faster word-at-a-time hash in place of FNV-1a:** 13.9 → 12.8 ns at an average key length of 12
  (`C/hb.c`). The variable-length tail and branch mispredictions eat the gain. It would also change
  every table's iteration order, which risks churning generated outputs. Not worth it.
- **Reordering `Slotus` from 40 to 32 bytes** (status after the hash): the bench gain was within noise
  under machine load. It saves 20% of table memory and nothing outside tabula_dispersa.c touches the
  slots (grep), so it's a harmless cleanup, but don't expect run time from it.
- **xar_locare itself:** it already uses clz (worklog 2026-10-15). The remaining cost is the call
  boundary, which is idea 1.
- **The mask-only change in piscina without the slow-path split:** no gain (see idea 2).

## Open questions
1. **Out of batch, but the biggest thing I saw:** `probatio_cellae` (an ASan build; porta:venenum,
   3.55 min, n=33) spends **38% of samples in `__asan_memcpy`**, 26% from
   `magnus_divisor_communis` alone. Copying bignums in a gcd loop under ASan could be worth
   about 1 min. Whoever has the magnus batch should look at it.
2. Does `silva/examen.sh` or the canalis or lexicon lint accept `staticus __inline__` and a
   function-like macro named like a function in an `include/*.h`? That decides idea 1's form.
3. The amalgamators (silva, officina, tessera) vendor these files. They need regenerating after any of
   ideas 1–6 (the lead's job). Idea 1 needs the generator to carry the `XAR_IMPLEMENTATIO` or
   `(name)` definition form through the prefix renaming.
4. For LTO (idea 8), measure `compile_tests.sh` cold-build wall time and the per-binary link time
   with `-flto=thin` plus a cache path on a scratch clone before anyone considers it. `-g` on every
   test object also costs link and dsymutil time; that's batch A territory, maybe.
5. The estimates use the briefing's suite list as the "core-heavy pool". The frigida:* and radix
   runs (about 20 min) weren't profiled here, so their share of these primitives is a guess at a
   quarter of the pool rate. One `sample` of frigida:radix's test phase would tighten every number
   above.
