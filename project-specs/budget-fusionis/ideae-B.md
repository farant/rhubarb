> Raw analyst report, batch B of budget B1 (2026-10-27/28), filed verbatim as evidence for ../budget-fusionis.md. Paths under `scratchpad/b1/` were session scratch (profiles, prototypes) and no longer exist.

# Batch B: lib/stml.c internals + silva-tree ↔ STML writers/readers

Analyst: B (Opus 5.5), 2026-10-27. Nothing in the repo was changed.
Analysis tooling (scratch only): `b1/B/pa.py` (fast inclusive/self + "deepest-frame line" histogram + children; caches a .B.pkl next to each profile) and `b1/B/callers.py` (immediate-caller:line histogram). Quick example: `python3 B/pa.py prof_lint.txt _liberos_legere _tok_proximus`.
Line numbers from the profiles match the current quinta sources (I spot-checked stml.c 8253/8265/8968/9004/9199 and silva_arbor.c 2773).

## Basis for the minute estimates

Per-merge minutes per suite (M), from briefing_numeri.txt, times 2 ("each suite runs twice per merge"):

| suite | frigida run | M used |
|---|---|---|
| exemplaria_lint | 0.62 | **1.24** |
| exemplaria_obsoleta | 0.64 | **1.28** |
| exemplaria_inutilia | 0.51 | **1.02** |
| canon_corpus | 0.78 | **1.56** |
| arbor_plagula | 0.60 (+1.33 in the aedilis list) | **1.93** (0.60+1.33, not doubled) |
| **sum** | | **7.0 min** |

The lead's brief puts these suites at "~17 min". If that larger base is right, **multiply every estimate below by about 2.4**. Shares are % of wall-clock samples (1 ms `sample`, so blocked time is counted too). Estimate = Σ(M × share) × expected reduction of that share.

**Out-of-batch flag (bigger than anything below):** `apparatus_clausuras_petere` (silva/probationes/apparatus.c:217) runs `./bin/aedilis --corpus <dir> --partes` through popen and blocks in `fgets`/`__read_nocancel`. That is **20.2% of lint, 17.4% of canon_corpus and 20.7% of arbor_plagula**: 0.25 + 0.27 + 0.40 ≈ **0.9 min/merge** spent waiting on the same corpus query, issued by every suite on every run. Caching its output (keyed by tree state), or computing it once per merge and passing it in, is worth more than the top 3 ideas here combined. That belongs to whoever owns the aedilis/gate batch.

---

## Ranked ideas

### 1. Reverse tag → genus lookup is a linear "mangle every genus" scan, done for every lexeme and trivium read (≈0.28 min)
- **Where:** materia/fontes/materia_arbor.c `materia_arbor_lexema_ex_tag` (L268-298), called from `_trivium_legere` (L2959) and `_lexema_legere` (L3187).
- **Evidence:** arbor_plagula: `materia_arbor_lexema_tag` 15.7% inclusive, 7.8% self, plus strlen/memmove/memcpy stubs. Callers: `_trivia_legere:3043` 10.3% and `_lexema_legere:3187` 4.7%. Nothing else in `silva_arbor_legere_parsuram` (39.4%) comes close.
- **Mechanism:** for each element, the loop over i = 0..numerus_generum mangles genus i: materia_lexicon_titulus, two strlen calls, memcpy, then a per-char lowercase and '_'→'-' pass, then a memcmp. That is O(genera × name length) per lexeme. Trivia genera (whitespace, comment) sit late in the table, so they pay nearly the full scan. Fix: build an index once per `MateriaArborLector` (lazily, on first use): a `tabula_dispersa_creare_chorda` mapping the mangled tag chorda to the genus, built with the same `materia_arbor_lexema_tag` loop. **Keep first-wins semantics:** insert only if the key is absent, so collisions resolve exactly as the linear scan does. A cheaper variant keys on the interned `elementum->titulus` pointer, but a content key is simpler and safe. The writer-side twin, `materia_arbor_lexema_tag` per lexeme in `_scribere_lexema:1305` and `_scribere_trivium:954`, can take a per-scriptor `genus → chorda*` array (see idea 11).
- **Estimate:** 1.93 × 15.0% × 0.95 ≈ **0.28 min**. Every arbor reader gains the same (circuitus, horreum, tools), so this is probably more outside these suites.
- **API:** none (internal to materia_arbor; one new field in the lector struct). Risk: low. **Effort:** S.
- **Verify:** materia/probationes/probatio_materia_arbor.c, silva arbor_plagula, arbor_circuitus, arbor_parsura. **Plant:** make the index insert last-wins, or drop the first genus. If the lexicon has no colliding mangled names, the last-wins plant will not show; then plant "return n (unknown) on index miss without fallback" on one genus. Either should turn arbor_plagula's "genus lexematis lexico ignotum" counter red.

### 2. Test-level: lint and obsoleta parse the same arbor text twice (≈0.26 min)
- **Where:** silva/probationes/probatio_silva_exemplaria_lint.c `_plagulam_probare` (L540 stml_legere) and `_relatum_alterum_expandere` (L398-L460). The same pair exists in probatio_silva_exemplaria_obsoleta.c (L476/L512 and L622).
- **Evidence:** in `_relatum_alterum_expandere`, `stml_legere` costs 10.8% of lint and 11.0% of obsoleta, the same cost as the first parse. The appended lint documents are tiny (conditio_nihilum*.stml are 327–338 B). What is re-read is the whole arbor text.
- **Mechanism:** parse `scriptura.textus` once, then parse each lint document separately with **the same intern**. Some code compares interned pointers (`_titulus_in_copia`), so a shared intern is required. Expansion leaves the original tree untouched ("contractus duarum arborum" in stml_macros.c), so the second variant can reuse the arbor children. Two ways to do it:
  - append the lint1 children to the parsed document root, expand, remove them with `stml_removere`, append the lint2 children, expand again; or
  - add a small library entry point, `stml_legere_continuare(documentum, textus, piscina, intern)`, which parses more text and appends its top-level nodes. That is an API *addition*.

  Trivia at the join differ from true concatenation: the arbor document's tail trivia becomes the next node's `spatia_ante`. The test only compares the two expansions with each other and counts `situs` children, so the outcome is unchanged. Still, confirm that `n_lint` and the CATENA DIVERGIT verdicts are identical before and after.
- **Estimate:** (1.24 × 10.8% + 1.28 × 11.0%) × 0.95 ≈ **0.26 min**.
- **API:** none if done in the test; an addition if `stml_legere_continuare` is added. Risk: low to medium (one test-semantics change Fran should approve). **Effort:** S–M.
- **Verify:** run both suites before and after and diff the census lines exactly (`situs lint`, `catena divergentes`, `invariata violata`). **Plant:** omit lint2's children for the second expansion. "RELATUM CATENAE DEEST" must fire.

### 3. `_processare_captiones` rebuilds every node's child list whenever the document has any capture (≈0.12 min)
- **Where:** lib/stml.c `_processare_captiones` (L4363-4565), called from `stml_legere` L4669.
- **Evidence:** 3.5% of lint, 3.3% obsoleta, 1.9% inutilia, 1.3% canon, 2.0% plagula. Children are xar_addere, xar_creare, self and xar_obtinere. Arbor STML uses capture forms heavily (`<a(>` spine collapse in the pretty printer), so `_habet_captio` is true and the whole tree is rewritten.
- **Mechanism:** for every node with children, the second pass allocates two new Xars (`novi_liberi` and `clean`) and copies every pointer, even when no child captures. Fix: after the recursive first pass, scan the children once. If none is an ELEMENTUM with `captio_directio != STML_CAPTIO_NIHIL`, return and keep the original `liberi`. That result is byte-identical: comments and regular nodes are copied in order, and the parser never stores NULL children, so the NULL-compaction does nothing. Optional: set a parser flag when any capture token is lexed and drop the `_habet_captio` full walk.
- **Estimate:** (1.24×3.5 + 1.28×3.3 + 1.02×1.9 + 1.56×1.3 + 1.93×2.0)% ≈ 0.164 min × 0.75 ≈ **0.12 min**. It also cuts parse memory (two fewer Xars per parent).
- **API:** none. Risk: low. **Effort:** S.
- **Verify:** probatio_stml.c (capture sections), probatio_stml_aurea.c, probatio_stml_pulchrum.c, probatio_stml_sedes.c. **Plant:** skip a node whose only capturing child is RETRO (wrong predicate, e.g. checking only ANTE). Retro and farcimen golden tests must fail.

### 4. Tokenizer: scan text, names and whitespace in bulk instead of `_tok_aspicere` + `_tok_progredi(1)` per byte (≈0.09 min)
- **Where:** lib/stml.c `_tok_legere_textus` (L1827, inlined into `_tok_proximus`, profile L2028), `_tok_legere_nomen` (L403-466, hot L454/L456), `_tok_praeterire_spatium` (L371), `_tok_progredi` (L349).
- **Evidence:** `_tok_proximus` self is 3.0% lint, 3.1% obsoleta, 2.0% inutilia, 1.4% canon, 1.7% plagula. `_tok_legere_nomen` self is 1.5%, 1.6%, 0.9%, 0.7%, 0.8% on the same suites.
- **Mechanism:** each byte does two bounds checks plus a newline/column branch. A name run contains no '\n', so scan with a local index and then do `positus += n; columna += n`. For text, find the next '<' with memchr or a tight loop, then count '\n' in the span (memchr loop), setting `linea += k` and `columna` from the last newline. Same tokens, same lines and columns.
- **Estimate:** Σ M × (tok self + nomen self) ≈ 0.227 min × 0.4 ≈ **0.09 min**. It applies to every STML reader (md, toml, docs, ledger) too.
- **API:** none. Risk: low (line/column bookkeeping is the trap). **Effort:** S–M.
- **Verify:** probatio_stml.c error-position tests (linea_erroris/columna_erroris), probatio_stml_sedes.c (extents), probatio_stml_incolumitas.c (EOF and unclosed edge cases). **Plant:** in bulk text scanning, forget to reset `columna` after the last newline (columna += span). Column assertions must fail.

### 5. `_lineae_contenti` counts every newline of an element's full content, only to compare with 30 (≈0.08 min)
- **Where:** lib/stml.c `_lineae_contenti` (L5062), call at L9004 (`<= STML_CLAUSURA_TACITA_LINEAE`, which is XXX).
- **Evidence:** 1.4% lint, 1.3% obsoleta, 1.7% inutilia, 1.0% canon, 1.3% plagula, almost all self time. It runs for every pretty-printed block element and scans everything from `initium_contenti` to the current end, so the cost is O(content × depth).
- **Mechanism:** replace it with an early-exit form, `_lineae_contenti_excedunt(aed, initium, limes)`, that stops at newline limes+1. Only the comparison result is used, so the output is identical.
- **Estimate:** 0.092 min × 0.9 ≈ **0.08 min**, and every `stml_scribere(…, VERUM)` caller gains.
- **API:** none. Risk: very low. **Effort:** S.
- **Verify:** probatio_stml_pulchrum.c (silent `</>` closure tests near the 30-line threshold; add one at exactly 30 and 31 lines if none exists). **Plant:** off-by-one (`< limes` instead of `<=`). The 30-line golden must flip.

### 6. `_est_perspicuum`: a linear Xar scan per node, done twice per node (≈0.08 min)
- **Where:** lib/stml_macros.c `_titulus_in_copia` (L3302) via `_est_perspicuum`. The hot calls are `_exemplar_petere` L4526 (the node itself) and L4630 (the child filter). The same child is checked again at the top of its own recursive call.
- **Evidence:** L4526 + L4630 = 2.5% lint, 2.7% obsoleta, 2.4% inutilia. They are the main part of `xar_obtinere`'s 1.7% under `_exemplar_petere`. The transparency set has 7–8 interned tags (`expansio pasta stringificatio api extentum ante post [regio-cruda]`), and a non-transparent node, which is most nodes, scans all of them through `xar_obtinere`/`xar_numerus` calls.
- **Mechanism:** when `_transparentiam_legere` / `_titulos_internare` builds the set, also keep a contiguous `chorda**` array and count in ctx, and do the pointer compares inline. Optionally pass "already filtered" into the recursion so L4526 is skipped for children that L4630 already passed. Identical semantics: same pointer-equality test.
- **Estimate:** (1.24×2.5 + 1.28×2.7 + 1.02×2.4)% ≈ 0.090 min × 0.85 ≈ **0.08 min**.
- **API:** none. Risk: low. **Effort:** S.
- **Verify:** probatio_stml_exemplaria.c and probatio_stml_macros.c (TRANSPARENTIA cases), silva exemplaria_lint/obsoleta/inutilia census lines. **Plant:** leave the array one entry short (count−1). A TRANSPARENTIA test with the last-listed tag must fail.

### 7. Test-level: canon_corpus `_involucrum_numerare` runs 8 memcmp-at-every-byte scans of each document (≈0.08 min)
- **Where:** silva/probationes/probatio_silva_canon_corpus.c `_quot_occurrit` (L93) and `_involucrum_numerare` (L120).
- **Evidence:** 5.4% of canon_corpus, all self time, called from main:586.
- **Mechanism:** do one pass: memchr for '<', then test the 8 literals at that position. All of them start with '<' and contain no other '<', so the counts are identical, including prefix matches such as `<error` matching `<errorum`.
- **Estimate:** 1.56 × 5.4% × 0.9 ≈ **0.08 min**.
- **API:** none (test file). Risk: none. **Effort:** S.
- **Verify:** the census printout of canon_corpus is byte-identical (`vidit_*`). **Plant:** count only the first matching literal per '<'. `<expansio` and `<extentum` cannot both match the same position, so that plant is a no-op. Use "skip to after the match" instead, which breaks overlapping `<expansio` and `<expansio-…` counts.

### 8. A parser-local intern cache for trivia, tag names and attribute names (≈0.07–0.10 min)
- **Where:** lib/stml.c: `_liberos_legere` L3245 (`spatia_ante`), `_spatia_apponere` (L2981), `_textum_tractare` (L3186-3196), `_titulum_ex_tokeno_ponere` (L2331), `_tok_legere_attributa` L705/706.
- **Evidence:** `chorda_internare` under `stml_legere` in lint: `_liberos_legere:3241` (the element path, mostly titles) 2.3%, `:3245` (trivia) 1.2%, `_spatia_apponere` (trivia) 1.0%, `_tok_legere_attributa` 0.5%, about 5% in total. obsoleta is similar, the others are roughly half. Pretty-printed arbor STML trivia are almost entirely `"\n"` (spatia_post after the `_spatium_partiri` split) and runs of k spaces (the residue that becomes the next node's spatia_ante). Every one of them pays fnv1a, a probe and a compare.
- **Mechanism (minimal):** keep `chorda* spatia_cache[64]` and `chorda* nl_cache` in `StmlParserContext`. In a `_trivia_internare` helper, if the string is all ' ' and shorter than 64, or exactly "\n", return the cached pointer, filling it lazily through `chorda_internare`. The pointer is identical to what the intern would return.
  **Generalized:** a 128–256-slot direct-mapped cache keyed by (len, first byte, last byte), verified by length and memcmp, in front of every `chorda_internare` in the parser. That covers titles and attribute names as well.
- **Estimate:** the minimal version is Σ M × trivia share (≈2.2/2.0/1.2/0.6/1.0%) ≈ 0.094 min × 0.8 ≈ **0.075 min**. The generalized version reaches about 0.10 min. It applies to every STML reader.
- **API:** none. Risk: low (pointers stay canonical, but watch callers that pre-populate the intern; the lazy fill handles that). **Effort:** S (minimal), M (generalized).
- **Verify:** probatio_stml_aurea.c (fidelity round-trip byte-for-byte), probatio_stml.c trivia sections, probatio_stml_sedes.c. **Plant:** index the cache by len−1, or fill it with a non-interned copy. Fidelity goldens must break, or a pointer-equality assertion if one exists. Consider adding `spatia_ante == chorda_internare(intern, same)`.

### 9. Pretty-printer indentation: one append per level (≈0.07 min)
- **Where:** lib/stml.c `_scribere_indentatio` (L6611), inlined at `_scribere_nucleus` L8253, L8986 and L9114.
- **Evidence:** those lines are 1.6% of lint and 1.9% of inutilia, with children `chorda_aedificator_appendere_literis` and `_appendere_interna`. Arbor STML is nested deeply (20–40 levels), and every level is a separate strlen-plus-memcpy call through `_appendere_interna`.
- **Mechanism:** keep a static 128-space buffer and append `2*nivel` bytes in one `chorda_aedificator_appendere_chorda` call, looping in chunks if deeper. Identical bytes. The same run-batching applies to `_scribere_evasus` (L6623: append runs without `<>&` in one call) and `_crudum_fugatum_scribere` (idea 13).
- **Estimate:** about 1.5% average × 7.0 min × 0.7 ≈ **0.07 min**.
- **API:** none. Risk: very low. **Effort:** S.
- **Verify:** probatio_stml_pulchrum.c, probatio_stml_aurea.c, probatio_materia_scribere.c. **Plant:** write `nivel` spaces instead of `2*nivel`. Every pretty golden fails.

### 10. `_liberum_expandere` checks 7 keywords per expanded node, each with strlen and compare (≈0.07 min)
- **Where:** lib/stml_macros.c `_est_titulo` (L524). The hot calls are `_liberum_expandere` L3091 (COMMUTATIO) and L3111-3116 (EXEMPLAR/PER/TRANSPARENTIA/CATENA/DIRIBITIO/INDAGO). There are 37 `_est_titulo` call sites in all.
- **Evidence:** `chorda_aequalis_literis` from those lines is about 2.1% of lint and obsoleta, about 1.2% of inutilia, and about 0.4–0.6% of canon and plagula.
- **Mechanism:** in `_est_titulo`, compare the first byte before calling `chorda_aequalis_literis`. Guard the empty literal: `titulus[0]=='\0' ? mensura==0 : (mensura>0 && datum[0]==titulus[0] && chorda_aequalis_literis(...))`. Ordinary tag names are lowercase and the keywords are uppercase, so almost every call exits after one byte. **Overlap:** if the primitives batch rewrites `chorda_aequalis_literis` as a single compare-while-scanning pass (exit on the first differing byte, no strlen; correct with embedded NULs, because a NUL in cstr before s.mensura means not equal), most of this gain comes for free everywhere, including canon's `_est_caps_machinae` (≈3.5% of canon_corpus) and the tests' `_invariata_numerare`. **Do whichever lands first; the other then becomes marginal.**
- **Estimate:** ≈0.085 min × 0.8 ≈ **0.07 min** (shared with the primitives batch).
- **API:** none. Risk: very low. **Effort:** S.
- **Verify:** probatio_stml_macros.c and probatio_stml_exemplaria.c. **Plant:** drop the empty-literal guard and add a test calling `_est_titulo(n, "")`, or compare `datum[1]`. Keyword tests must fail.

### 11. Writer: per-session cache of interned tags (genus → `chorda*`) and an element constructor that takes an interned title (≈0.06 min)
- **Where:** lib/stml.c `stml_elementum_creare` (L6262: `chorda_internare_ex_literis` = strlen + fnv + probe per element). Callers: materia_arbor.c `_scribere_nodum_internum` L1781/L1884, `_scribere_lexema` L1311 (after `materia_arbor_lexema_tag` mangling at L1305), `_involucrum_triviorum` L1160/1169.
- **Evidence:** in lint, `chorda_internare_ex_literis` under `stml_elementum_creare` is 1.3%, plus `materia_arbor_lexema_tag` 0.2% in the writer and `stml_attributum_addere` interning of literal names 0.3%. The writer path runs in all five suites.
- **Mechanism:** add `stml_elementum_creare_titulo(Piscina*, InternamentumChorda*, chorda* titulus_internatus)`, a pure API addition; the existing function is unchanged. In `MateriaArborScriptor`, keep `chorda* tag_generis[numerus_generum]`, filled lazily, plus interned pointers for the fixed tags (ante/post/…). The same idea fits `stml_attributum_addere` for fixed attribute names.
- **Estimate:** about 1.2–1.5% × 7.0 × 0.7 ≈ **0.06 min**.
- **API:** addition (one new constructor); stml.h grows. Risk: low. **Effort:** S–M.
- **Verify:** probatio_materia_arbor.c, probatio_materia_scribere.c, silva arbor suites, `stml_scribere` output byte-identical. **Plant:** fill the cache with the un-mangled genus name (no lowercase). Every arbor golden fails.

### 12. Test-level: `_invariata_numerare` runs 6 `chorda_aequalis_literis` calls per node over the whole expanded tree, twice per plagula (≈0.07 min)
- **Where:** probatio_silva_exemplaria_lint.c L345-371 (also obsoleta L430-455 and inutilia).
- **Evidence:** 3.4% lint, 3.5% obsoleta, 2.3% inutilia (its compares are lines 355-360 in the caller table).
- **Mechanism:** prefilter on `titulus->datum[0]` being 'E','P','C','D','I' or 'T' before the 6 compares. Also iterate `liberi` directly rather than through `stml_liberum_ad_indicem`. It becomes mostly free if the `chorda_aequalis_literis` primitive is fixed (idea 10).
- **Estimate:** 0.110 × 0.6 ≈ **0.07 min**.
- **API:** none. Risk: none. **Effort:** S.
- **Verify:** the `invariata violata` census is unchanged. **Plant:** drop 'T' from the prefilter. Only TRANSPARENTIA would escape; check that the census catches it, and if no fixture has a surviving TRANSPARENTIA, note that the plant is invisible.

### 13. `_attributa_elementa_probare`: a whole-tree walk after every parse (≈0.05 min)
- **Where:** lib/stml.c L3300-3445, called from `stml_legere` L4676.
- **Evidence:** 1.5% lint, 1.3% obsoleta, 0.7% inutilia, 0.5% canon, 0.7% plagula. Every check fires only on children with `attributum_titulus != NIHIL`.
- **Mechanism:** during parsing (`_liberos_legere`, when a child with `attributum_titulus` is appended), record the parent once (a flag bit or a small Xar). After capture processing, run the per-parent check only on the recorded parents, in pre-order. Sorting by `positus_initium` gives the same order, so the *first* error, its line and its message are unchanged. Simpler and smaller: skip the pass entirely when the parse saw no '@' element (a ctx flag). Arbor STML does contain `<@…>` argument elements, so the targeted version is needed to gain here. Caveat: capture processing moves nodes between parents, so record the *final* parent: re-check `parens` at check time, or collect in `_processare_captiones` too.
- **Estimate:** 0.064 × 0.8 ≈ **0.05 min**.
- **API:** none. Risk: medium (error ordering, parents changed by capture). **Effort:** M.
- **Verify:** probatio_stml.c par. 6.3 tests (all the "attributum bis nominatum" / "extra praefixum" variants, multi-error documents). **Plant:** record parents in insertion order (post-order) without sorting. A two-error document must report the wrong first line.

### 14. `_crudum_fugatum_scribere` / `_scribere_evasus`: per-byte appends (≈0.03 min)
- **Where:** lib/stml.c L5158-5212 and L6623-6650.
- **Evidence:** `_crudum_fugatum_scribere` is 0.6–0.8% in four suites (0.3% canon): per byte, a `stml_crudi_sequentia_est` call plus a 1-byte `_appendere_interna`.
- **Mechanism:** scan to the next '<' or '\n' and append that run in one call. `stml_crudi_sequentia_est` returns FALSE unless `datum[i]=='<'`, and the prefix logic only reacts to '\n', so the output is identical. `_scribere_evasus` gets the same treatment, with run boundaries at `<>&`.
- **Estimate:** 0.043 × 0.6 ≈ **0.03 min**.
- **API:** none. Risk: very low. **Effort:** S.
- **Verify:** probatio_stml.c raw-element escape-ladder tests (`<\\/T` levels), probatio_stml_aurea.c. **Plant:** stop runs only at '<'. The praefixum after '\n' gets lost and multiline raw goldens fail.

### 15. Smaller items, cheap if someone is in the file anyway (≈0.01–0.02 min each)
- `_sedes_valorum_iungere` (materia_arbor.c L2440-2500, profile L2558, 2.6% of lint in total): uses `_clavis_monstratoris`, which **piscina-allocates an 8-byte key per pair and per sedes entry**. Point the index keys at `&par->elementum` (Xar cells are stable, the same pattern silva_arbor.c uses) and use a stack key for lookups. That saves the allocations (≈0.15%); the hashing itself is the primitives batch's job.
- `materia_arbor_scriptor_repungere` (L2405, 1.0% lint, called twice): return early when `tabula_dispersa_numerus(tabula) == 0`. The gain depends on how often a compression pass substitutes nothing; that was not measured.

---

## Summary table

| # | idea | est. min/merge (7-min base) | effort | API |
|---|---|---|---|---|
| — | **out of batch:** cache the `aedilis --corpus` query in apparatus_clausuras_petere | **~0.9** | S–M | none |
| 1 | tag→genus index in arbor reader | 0.28 | S | none |
| 2 | lint/obsoleta: parse arbor once | 0.26 | S–M | none / addition |
| 3 | capture pass: skip unchanged parents | 0.12 | S | none |
| 4 | bulk tokenizer scanning | 0.09 | S–M | none |
| 5 | `_lineae_contenti` early exit | 0.08 | S | none |
| 6 | contiguous transparency set | 0.08 | S | none |
| 7 | canon test single-pass count | 0.08 | S | none |
| 8 | parser intern cache (trivia → general) | 0.07–0.10 | S/M | none |
| 9 | indentation in one append | 0.07 | S | none |
| 10 | `_est_titulo` first-byte check (or primitive fix) | 0.07 | S | none |
| 11 | interned-tag constructor and genus cache in writer | 0.06 | S–M | addition |
| 12 | `_invariata_numerare` prefilter | 0.07 | S | none |
| 13 | targeted attribute-element check | 0.05 | M | none |
| 14 | run-batched raw/escaped writes | 0.03 | S | none |
| **sum of 1–14** | | **≈1.5 min** (×2.4 ≈ 3.6 if the 17-min base holds) | | |

## Checked, not worth it (or another batch's)
- **Lazy `liberi` Xar** in `_parser_creare_nodus` / `stml_elementum_creare` / `_duplicare_recursivum`. The payoff is real (one xar_creare per leaf element), but callers assume `liberi != NIHIL` for elements, and `stml_elementum_creare` documents it as eager. That is an API-semantics change across many libraries. Better: make `xar_creare` itself cheap (primitives batch).
- **Up-front builder sizing in `stml_scribere_sedibus`** (starts at 512 B). `_crescere` already grows in place with `piscina_conari_ampliare`, so the memmove under `_appendere_interna` is mostly the copy of the appended bytes, not regrowth. Little to gain.
- **`_tok_legere_tag` / `_tok_legere_attributa` self time** (0.7% and 0.5%): nothing structural; the interning inside goes under idea 8.
- **`_folia_colligere`** (1.7–2.2%): writes each candidate leaf with `stml_scribere` and interns it as a signature. That is legitimate work. One could compute a structural hash instead of serializing, but the byte-identity of the signature is the contract ("signa octetim aequalia"). Not worth the risk.
- **`_expandere_nodum` → `stml_duplicare_superficialiter`** (1.5% lint): per-node allocation plus Xar creation is inherent to the two-tree contract.
- **Avoiding the text round trip in lint, obsoleta and arbor_plagula** (expanding the in-memory `involucrum` instead of re-reading `scriptura.textus`): it would save another ~11–12%, but the text artifact *is* what these gates test. Listed as an open question, not proposed.
- **`tabula_dispersa_invenire`, `friatio_fnv1a`, `xar_obtinere`, `_allocare_interna`, `chorda_aedificator` append cost:** primitives batch. I only looked at call patterns: the redundant ones are ideas 1, 6, 8, 10 and 15.
- `silva_c89_semantica_*` and `_nodum_processare` (obsoleta/inutilia, 18–20%): parser/semantics batch.

## Open questions
1. Is the 7-min base (briefing_numeri × 2) or the brief's "~17 min" correct for these suites? The ranking does not change, only the scale.
2. Idea 2 changes how lint and obsoleta build the catena document (graft instead of concatenation). Fran: is the "textus unus" concatenation itself under test, or only the expansion result? If only the result, doing it in the test is fine. A `stml_legere_continuare` API might be generally useful ("layered documents").
3. Does the PORTA CATENAE check (second lint variant, about 20% of lint and obsoleta) need to run on *every* plagula of the corpus, or would a fixed sample give the same guarantee? That is test policy, so it is Fran's call.
4. Idea 1: is there a natural home for a tag→genus index in `MateriaLexiconRatum` (cooked, shared, const today), or should it stay per-lector? Per-lector is the safe default; per-ratum saves the rebuild cost (about 100–150 mangles per plagula, negligible).
5. Do the primitives-batch owners plan a single-pass `chorda_aequalis_literis`? If so, drop ideas 10 and 12, or keep them only as belt-and-braces.
