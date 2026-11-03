> Raw analyst report, batch F of budget B2 (2026-10-31), filed verbatim as evidence for ../budget-fusionis.md.

# Batch F: silva corpus cluster, round 2

Analyst: batch-F agent (Opus 5.5), 2026-10-30/31. Nothing in the repo was changed except this file.
Scratch work is in `scratchpad/b2/F/`:
- `bin/`: copied and rebuilt test binaries
- `mod/<variant>/`: prototype sources
- `prof/*.txt`: 1 ms `sample` profiles
- `pa.py`: inclusive/self/children/callers analyzer
- `struere.sh`: rebuilds a suite from repo sources plus overrides
- `ab.sh`: interleaved A/B runs reporting user time and instructions retired

## Timing basis

Basis runs: gate run `01a18f3f20b8-bb90-1` (45 min, FUSIO RUBRA on briar-fumus) and frigida `01a18f68da2a-81aa-1`.

Per-merge seconds for the members I own:

| member | fusio gate | frigida | per merge |
|---|---|---|---|
| canon_corpus | 57.1 | 40.1 | **97 s** |
| exemplaria_lint | 48.1 | 30.3 | **78 s** |
| exemplaria_obsoleta | 43.3 | 39.7 | **83 s** |
| exemplaria_inutilia | 35.7 | 28.9 | **65 s** |
| arbor_plagula | served (fresh) | 28.8 | **58 s** (+29.6 forced by silva-m3 `-vis arbor_`; +29 more when the gate itself runs it) |
| examen-corpus (`percursus -semantica`) | 106.8 | – | **107 s** (history 103-155) |
| silva-m3 | 97.0 | – | **97 s** (history 70-173) |
| oraculum-silvae | 41.9 | – | 42 s (not re-profiled; A6 landed) |

Load conditions:
- Load average was **6-26** throughout (other worktrees were running gates).
- Every A/B below is interleaved base/variant, run in the same window.
- I quote **instructions retired** as the robust number and user time as the indicative one.
- Profile shares and user-time deltas diverge for pointer-chasing code: F1 cuts 3.4% of instructions but 8-9% of samples, because the walk it removes is cache-miss-bound.

**Byte identity:**
- Every prototype's full suite stdout was `cmp`-identical to its baseline.
- The one exception is percursus: its memory statistics line `praeparationes: 126 (1286.8 MB)` grows by 3-6 MB. Its verdict and diagnostic lines are identical.

## Ranked table

| # | Idea | Where | est. min / merge | Eff | Status |
|---|---|---|---|---|---|
| F1 | Annotation attachment: unit extents computed in ONE walk per unit for all source files, not one full subtree walk per (annotation × unit) | `silva/fontes/silva_annotationes.c` `_affigere` | **0.35-0.45** (measured) | S | CANDIDATUM |
| F3 | Derived-type interning by hash, not a linear scan (the header comment already says "series linearis … percursus mensurabit") | `silva_c89_semantica.c` `_derivatum_internare` | **0.2-0.3** (measured with F1: percursus −20% instructions) | S-M | IUDICANDUM (adds a field to the `SilvaSemantica` struct in the header) |
| F9 | silva-m3 (and nexus) compile `silva/amalgama/silva.c` with raw clang / an absolute-path key. Route them through E7's `_vendicatum_compilare` key (13.3 s per compile) | `silva/m3_probare.sh`, `silva/nexus.sh` | **0.2-0.45** on merges that change silva.c | S | CANDIDATUM (nexus key miss unverified) |
| F5 | Region texturing: per-element extent cache (stack-resident, direct-mapped, no piscina allocations) | `silva/fontes/silva_parsare.c` `_regionem_texere` | **0.15-0.25** | S | CANDIDATUM (silva computus check not yet run) |
| F8 | haruspex: check the 563 generated TUs with ONE `clang -fsyntax-only` call; fall back to the per-TU loop only if it fails | `silva/haruspex.sh` Gradus II | **0.15-0.2** (measured 20 → 11 s CPU) | S | CANDIDATUM |
| F10+F11 | canon: (a) per-call element-lookup memo keyed by (parent-title pointer, title pointer); (b) one title → node index per document for the 5 unicitas/citatio passes | `lib/canon.c` `_elementum_quaerere_vi`, `canon_iudicare` | 0.2-0.3 (profile only) | S / M | CANDIDATUM |
| F6 | STML trivia intern cache also covers "\n" + k spaces (97.6% of the remaining cache misses) | `lib/stml.c` `_trivia_internare` | 0.1-0.2 (measured jointly with I5) | S | CANDIDATUM |
| F12 | silva-m3: read arbor_* numbers from the fresh (content-keyed) verdict instead of `-vis` re-running them | `silva/m3_probare.sh` + arbor tests | 0.5 when arbor_* are fresh | M | IUDICANDUM |
| F-A5 | A5 refreshed: `_relatum_alterum_expandere` is still 22.1% of lint and 17.2% of obsoleta | lint/obsoleta tests | 0.25-0.5 | M | IUDICANDUM (as in B1) |
| **F-X** | **Correctness, not speed: the citation `lexematis-fons` (`super="*"`) never fires in `canon_iudicare`. A dangling `f="77"` gives 0 vitia** | `lib/canon.c` passus II | 0 | S | **finding: gate gap** |

**Total:**
- CANDIDATUM items (F1, F9, F5, F8, F10/11, F6): **≈ 1.3-2.1 min per merge**.
- Adding F3: **≈ 1.5-2.4 min**.
- IUDICANDUM extras (F12, F-A5): another ≈ 0.75-1.

**Duplicate dropped:** `_lineae_excedunt` memchr is already I5. My combined lint A/B below includes it.

---

## F1. Annotation attachment walks every unit's subtree once per annotation — 0.35-0.45 min

**Where:** `silva/fontes/silva_annotationes.c` `_affigere`. It is called from `silva_annotationes_colligere`, reached via `_contractus_colligere` (from `_signata_binarium_probare`) and `_toleras_colligere` in semantics.

**Evidence:**
- The code is `per annotationem { per unitatem { silva_nodus_extensionem(unitas, a->fons_index, …) } }`. Each extent call walks the unit's whole subtree through `silva_token_radix`.
- Shares of the chain:
  - `silva_annotationes_colligere`: 13.5% of `percursus -semantica`, 10.9% of obsoleta, 12.0% of inutilia
  - `silva_valor_extensionem` self: 7.9% of percursus and 6.7% of obsoleta
- A first prototype that cached extents per distinct `fons_index` gave only −2% instructions. Annotations come from many headers, so there are many distinct sources per parsura.
- The fix that works computes every source's extent in a single walk per unit.

**Mechanism:**
- Per unit, walk the subtree once and accumulate `(fons, minimum, maximum)` triples in a small Xar (usually 1-3 entries).
- Use the same accumulator rule as `silva_valor_extensionem`: start at −1 / 0, take min of `a` and max of `b`, and only count tokens whose root token has `byte_offset >= 0`.
- The per-annotation loop then looks up its `fons_index` in the unit's triples. The interior/supra/plagula decision logic is unchanged.
- Prototype: `mod/annot2/silva/fontes/silva_annotationes.c`.

**Measured:**

| run | instructions | user time | output |
|---|---|---|---|
| obsoleta, base → F1 | 373.1 G → 360.4 G (−3.4%) | median 53.4 → 48.9 s over 3 interleaved pairs (load 13-26) | identical |
| obsoleta profile share of `silva_annotationes_colligere` | 10.9% → 2.2% | | |
| percursus, base → F1 only | 851 G → 785 G (−7.8%) | ~125 → 104.7 s | verdicts identical |

**Estimate (multiplier ×1 for examen-corpus; ×2 for obsoleta and inutilia):**
- examen-corpus: 107 × ~0.11 ≈ 12 s
- obsoleta: 83 × 0.085 ≈ 7 s
- inutilia: 65 × 0.10 ≈ 6.5 s
- Total ≈ **25 s ≈ 0.4 min**.
- Not counted: haruspex's semantic pass, the pre-commit examen hook on every commit, and LSP diagnostics. All of them use the same collector.

**API impact:** none (interior function and interior struct). No golds: semantics allocations are not in computus.

**Risk:** low. It is a pure function of the tree, and the order of comparisons is unchanged.

**Verify:** obsoleta/inutilia `divergentes == 0` with identical stdout. Run `percursus -semantica` before and after and diff everything except the `ms`/MB lines. `examen-vectis` fixtures stay green.

**Plant:** lump all sources into one triple (ignore `fons`).
- `probatio_silva_annotationes` asserts `fons_index == fons_princeps` only, so it is single-source and **would probably NOT catch this**.
- Whether obsoleta or examen-corpus catch it is unverified.
- **Add a two-source fixture first** (an annotation in an included header that must attach SUPRA a unit in that header).

## F3. `_derivatum_internare` is a linear scan over every derived type — 0.2-0.3 min

**Where:** `silva/fontes/silva_c89_semantica.c:3592`. Every pointer, array, qualified or function type created by semantics scans `sem->derivati` from the start, calling `_derivata_paria` on each entry. The struct comment in `silva_c89_semantica.h:668` reads "series linearis - complete primum, optima post; percursus mensurabit".

**Evidence:**
- `_derivatum_internare` is 7.4% of percursus, 3.8% of obsoleta and 4.7% of inutilia.
- `xar_obtinere` at line 3601 alone is 2.9% of percursus.

**Mechanism:**
- Add a hash index next to the Xar. The key holds exactly the fields `_derivata_paria` compares: genus, then the inner pointer(s), numerus / quales / flags, and the parameter pointers.
- Types of other genera are never equal under `_derivata_paria`, so they still always append.
- A hit is re-checked with `_derivata_paria`.
- Behaviour is unchanged: interning guarantees at most one equal entry exists, and the Xar keeps its append order.
- Prototype: `mod/deriv/` (struct field plus `tabula_dispersa_creare_chorda`). It allocates a key on every lookup. The production version should probe with a stack key and allocate only on insert; that allocation is what grew the MB statistics line by 3-6 MB.

**Measured (with F1):**

| run | instructions | user time | output |
|---|---|---|---|
| percursus, F1 → F1+F3 | 785 G → 684 G | 104.7 → ~96.5 s | |
| percursus, base → F1+F3 | 851 G → 684 G (−20%) | 120-130 → 96-97 s, two interleaved pairs at load 9 | REICE/FRACTA lines identical (only the MB line changed) |
| obsoleta, base → F1+F3 | 371.5 G → 343.3 G (−7.6%) | 42.6/45.8 → 37.9/38.8 s | identical |

**Estimate:**
- examen-corpus ≈ 8.5 s, obsoleta ≈ 3 s, inutilia ≈ 3 s.
- Total ≈ **15 s ≈ 0.25 min**.
- F1+F3 together ≈ 0.6 min per merge, plus every examen hook run.

**API impact:** a new field in `SilvaSemantica` (public header, mirrored in `silva/amalgama/silva.h` by regeneration). No new function. **Needs Fran's ruling** under the header rule.

**Risk:** low-medium. The key must mirror `_derivata_paria` exactly. The re-check guard makes a key that is too coarse harmless; a key that is too fine only causes duplicates.

**Plant:** remove the re-check AND drop `acies.numerus` from the key, so `int[3]` and `int[4]` unify. probatio_silva_semantica or the examen fixtures should go red. I did not run this.

## F9. The silva amalgam is compiled outside E7's shared compilator key — 0.2-0.45 min (silva-changing merges)

**Where:**
- `silva/m3_probare.sh`: raw `clang "${VEXILLA_C89[@]}" -c $RADIX/silva/amalgama/silva.c -o build/m3/amalgama.o`, re-run whenever `silva.c` is newer.
- `silva/nexus.sh` `compilare amalgama`: goes through `bin/compilator`, but with the absolute source path.

**Evidence:**
- One amalgam compile takes **13.3 s user** (load 7).
- m3's `amalgama.o` was rebuilt in the basis run (mtime 20:20).
- E7 (b3a306e9) made briar and officina share one compilator key: run from the root with a relative path and `VEXILLA_C89`. officina, briar and nexus all set `GCC_FLAGS=("${VEXILLA_C89[@]}")`, so the flags are identical.

**Mechanism:** call `_vendicatum_compilare "$obj" "${VEXILLA_C89[@]}" -c silva/amalgama/silva.c` (cwd = root) in both scripts.

**Estimate:**
- 13 s for m3 + 13 s for nexus per merge where `silva.c` changed (silva-m3 only runs on such merges).
- **0.2-0.45 min**.
- The nexus half assumes its absolute-path key misses officina's. Confirm in compilator's hit log before counting it.

**API:** none. **Risk:** trivial. The object differs only in DWARF path strings.

**Verify:** the second gate prints a compilator hit (0.08 s).

**Plant:** nothing to break functionally. A misspelt flag must show up as a cache miss (slower), not as a wrong object.

## F5. Region texturing re-walks every root element's subtree for each `#if` region — 0.15-0.25 min

**Where:** `silva/fontes/silva_parsare.c` `_regionem_texere` → `_extentum_valoris`.

**Evidence:**
- For every region (include guards of every header in the closure, plus every `#if` block), every top-level element's extent is recomputed by a full walk. Elements from another source are skipped only after the walk.
- `_extentum_valoris` shares: 5.6% of percursus, 4.5% of arbor, 2.6-3.1% of lint/obsoleta/inutilia, 1.6% of canon. It runs in every silva parse (tools too).

**Mechanism:**
- A direct-mapped cache in `TexturaStatus` (2048 slots of {node pointer, extent}, on the stack), zeroed per `_texere`.
- Root nodes are immutable after construction. Congregated elements are appended into NEW branch nodes and never mutated, so a node's extent is fixed.
- No piscina allocation, so silva computus allocation counts should not move.
- Prototype: `mod/regio/`.

**Measured (arbor_plagula):** 225.1 G → 222.1 G instructions (−1.4%), user 29.1/28.4 → 28.1/24.4 s, output identical. Profile share is 4.5%; the walk is miss-bound.

**Estimate:** about the profile share, ×2 multipliers, on 107 + 381 s of parse-bearing runs ≈ **16 s ≈ 0.25 min**. Lower bound 0.15.

**API:** none.

**Unfinished:** run `probatio_silva_computus` and `arbor_parsura` on the variant before claiming computus immotus.

**Plant:** drop the pointer check (`memoria_nodi[h] == nd`), so slots alias. arbor_plagula `OCTETIM EXACTAE` and `regiones_textae` must diverge.

## F8. haruspex compiles 563 generated TUs with 563 clang processes — 0.15-0.2 min

**Where:** `silva/haruspex.sh` Gradus II loop (silva-m3 gate).

**Measured (copies of `silva/build/exta/*.c`, same flags and include roots, load 8):**

| mode | CPU (user+sys) | wall | result |
|---|---|---|---|
| per-TU loop | 20.3 / 19.8 s | 25.5 / 23.4 s | 563 TU, 0 fracta |
| one `clang -fsyntax-only exta/*.c` call | 10.5 / 11.2 s | 11.8 / 13.8 s | rc 0 |

**Mechanism:**
- Run the batch call first.
- If it returns 0, print the same summary line (`haruspex: 563 TU compilatae, 0 dissentientes`).
- If it fails, run today's loop unchanged, so dissenters are named exactly as now.
- TUs stay separate translation units (no concatenation).

**Estimate:** ~11 s per silva-m3 run ≈ **0.18 min**.

**Risk:** low.

**Plant:**
- (a) Make one TU's `offsetof` value wrong. The batch fails, and the fallback names exactly that TU with `[HARUSPEX DISSENTIT]`.
- (b) Force the fallback path. The output must be identical to the batch path.

## F10+F11. canon_iudicare: 49% of canon_corpus — 0.2-0.3 min (profile arithmetic)

**Evidence:**
- canon_corpus judges each document twice (plain plus `canon_iudicare_expansum`; both are asserted).
- `canon_iudicare` is 49% inclusive:
  - `nodum_iudicare` 16.8%
  - `_elementum_quaerere_vi` 8.3% (builds a `"parent/child"` key with `clavis_scopi`, then hashes it; misses; does a second global lookup; called twice per node, in `nodum_iudicare` and in `_augmenta_cusasque_colligere`)
  - the five rule passes (`unicitas` fontium-indices plus 4 `citatio`), which scan ALL document nodes per rule doing `xar_obtinere` + `chorda_aequalis` on each super title: canon_iudicare self 6% + xar 5% + chorda_aequalis 4% + `_subarborem_colligere` 5.7%

**Mechanisms (all interior; no header change):**
- **(F10, S)** A per-call memo `(parent-title ptr, title ptr) → CanonElementum*` in `CanonTransclusio`, also passed to `_augmenta_cusasque_colligere`. A pointer key is sound: equal pointers mean equal content, and distinct pointers just miss. Est. ~4-5% of canon_corpus.
- **(F11, M)** Build `title → ascending node indices` once over `nodi_documenti`. Each rule merges the lists for its super/ad titles in document order. Fall back to the full scan when a list contains `*` or `intra=` scopes. Est. ~10%.

**Estimate:** (0.045 + 0.10) × 97 s ≈ 14 s, plus md_canon and the other canon gates ≈ **0.2-0.3 min**.

**Risk:** F10 low. F11 medium (vitium and claves order must stay in document order).

**Allocation note:** canon has no computus gold that I found, but check md/html computus before landing.

**Plant:**
- F10: key on the title only (drop the parent). Scoped (`intra=`) definitions resolve wrong and canon_corpus/canon.canon tests go red.
- F11: reverse the merge order. `CANON_NOMEN_BIS` reports move to the other node, and the canon tests' vitium expectations go red.

## F6. Trivia intern cache misses the dominant "\n" + spaces shape — 0.1-0.2 min

**Where:** `lib/stml.c` `_trivia_internare`. B8's cache covers "\n" alone and pure spaces only.

**Evidence:**
- Instrumented over a full lint run: **54.6 M hits, 8.23 M misses, of which 8.03 M (97.6%) are "\n" + k spaces**.
- The misses go to `chorda_internare` (fnv + probe + memcmp).
- Share of this call site (stml.c:3341/3345): canon 1.2%, lint 4.0%, obsoleta 2.9%, inutilia 1.8%, arbor 2.4%.

**Mechanism:**
- `trivia_nl_spatia[LXIV]` filled by `chorda_internare` on first sight.
- Returns are pointer-identical to today (the same interned chorda). Allocations happen at the same first occurrence, so computus counts do not move.

**Measured jointly with I5 (memchr in `_lineae_excedunt`), lint:**
- 291.6 G → 284.7 G instructions (−2.4%)
- user 34.4/30.8 → 34.2/25.8 s
- output identical

**Estimate (F6 alone):** 97×1.2% + 78×4% + 83×2.9% + 65×1.8% + 58×2.4% ≈ **10 s ≈ 0.17 min**, plus md_stml, oratio_stml and the other `stml_legere` users.

**API:** none (private parser context).

**Plant:** index the slot with `s.mensura` instead of `s.mensura − 1`. That returns a string one space off, and the STML round-trip suites (probatio_stml, arbor_plagula octetim) go red.

## F12. silva-m3 forces arbor_* to re-run only to parse their numbers — 0.5 min (IUDICANDUM)

**Evidence:**
- In the basis run, porta:silva served arbor_plagula as fresh. silva-m3 then re-ran it with `compile_probationes.sh -vis arbor_` (29.6 s) only to grep `DIRECTA exacta` / `OCTETIM EXACTAE` lines from the output.
- The fabrica area keeps `credo.tsv` and `verdictum.txt`, but not the census lines.

**Mechanism:**
- The arbor tests write their m3 numbers into the area (e.g. as credo notes in `credo.tsv`), so they survive with the content-keyed verdict.
- m3_probare reads them there, and runs `-vis` only when absent.

**Ruling needed:** is m3 a second proof (then keep `-vis`), or a reader of a verdict fabrica already trusts?

## F-A5 refresh (IUDICANDUM, unchanged question)

`_relatum_alterum_expandere` is still 22.1% of lint and 17.2% of obsoleta today (≈ 31 s per merge). Reading the tree text once would save about half of that (≈ 0.25 min); A9 is still ≤ 0.5. The question for Fran is unchanged from B1: is the "textus UNUS" concatenation itself under test?

## F-X. Correctness: the `super="*"` citation is dead (gate gap)

**Where:** `silva/c89.canon:4628` (`citatio lexematis-fons attributum="f" ad="fons/index" super="*"`) and `lib/canon.c` passus II (~4196-4220).

**Cause:** `_super_congruit(n, "*")` has no `/`, so it reduces to `chorda_aequalis(titulus, "*")` and is false for every node. `*` is handled in `_attributum_declaratum` (canon_examen) and in passus I's `ad_elementa`, but not in passus II's `super`.

**Verified** (scratch build of canon_corpus, `mod/stella/`):
- Inserted `f="77"` (no such `<fons index>`) on a `<lex-int>` in a valid document: **0 vitia**, both plain and expanded.
- `f="0"`: also 0.
- No test catches it.

**Fix:**
- Treat `*` in `super` as "any element" in passus II, as `_attributum_declaratum` already does.
- This adds work (every node's `f=` is checked), so measure it.
- First decide whether the corpus is clean under the live rule.

**Plant (once fixed):** the f="77" document must give `CANON_CITATIO_…`.

## Checked, not worth it

- **F1 with a per-source cache (instead of the single walk):** only −2% instructions, because annotations span many headers. Superseded by the single-walk version.
- **`_lineae_excedunt` memchr:** already I5. I measured it jointly with F6 only.
- **hospes.c / haruspex.c relink in silva-m3:** 0.34 s and 0.10 s per run.
- **percursus `printf`/`fseek`** (4% / 2.7% of samples): blocked IO under load (83 KB of output), not CPU.
- **silva writer internals** (`_scribere_lexema` 5.5%, `_folia_macronum_comprimere` 6.3%, `materia_arbor_scriptor_numerare` 2.2%): the cost is spread across many small calls; no hotspot. `materia_arbor_lexema_tag` is now 0.2-0.5% (A4 landed).
- **excludenda_silva** (6.7 s per run after A3): the gyrus loop is already lean. It runs twice in frigida (SANATUM regeneration plus IUDICIUM judge). Skipping the judge right after a same-input regeneration is fabrica's question (J1 territory).
- **`apparatus_clausuras_petere` residual** (~3 s × 7 calls per merge ≈ 0.35 min): only A1's variant 1b (generator file) removes it, and that needs a freshness contract. Unchanged from B1.
- **porta:silva also spends ~66 s healing `bin/briar`, `bin/briar-spectator` and `bin/silex`** ("ingressus mutati post institutionem") before running suites. That is not silva work. It is the same staleness H6 describes.
