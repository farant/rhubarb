> Raw analyst report, batch A of budget B1 (2026-10-27/28), filed verbatim as evidence for ../budget-fusionis.md. Paths under `scratchpad/b1/` were session scratch (profiles, prototypes) and no longer exist.

# Batch A — silva corpus pipeline: ranked performance ideas

Per-run minutes used below (from the per-merge leaf numbers; each suite runs in fusio gates + frigida = 2 runs unless noted):
lint 1.5/run ×2 · inutilia 1.3 ×2 · canon_corpus 1.2 ×2 · arbor_plagula 0.95 ×2 + 1.3 under porta:silva-m3 (3 runs, 3.2 min) · obsoleta 0.7 ×2 · excludenda_silva 1.25 ×2 (frigida, n=2) · examen-corpus 2.3 ×1 · oraculum-silvae 1.0 ×1.

Profile shares are inclusive % of `main` from the 1 ms `sample` profiles in b1/ (my own faster analyzer: b1/A/incl.py, b1/A/kids.py; arbor.py's regex hung on the larger files). New measurements are in b1/A/.

| # | Idea | Est. min saved / merge | Effort |
|---|------|------------------------|--------|
| 1 | aedilis closures via the existing thesaurus store (`--thesaurus`) | **~1.35** (1.6 with shared file) | S |
| 2 | Skip the second semantic analysis when `silva_recanonicare` returns 0 | **~0.8** (+ every examen hook/LSP run) | S–M |
| 3 | excludenda: `-fsyntax-only` + one-pass name classification | **~0.55–1.1** | S |
| 4 | `materia_arbor_lexema_ex_tag`: replace the linear re-mangling scan with a lookup table | **~0.5–0.6** | S |
| 5 | lint/obsoleta: read the tree text once, expand twice | ~0.45 | M |
| 6 | oraculum-silvae: content-addressed cache of the PINNED oracle's output | ~0.4 (unmeasured split) | M |
| 7 | canon_corpus `_involucrum_numerare`: one scan instead of 8 naive scans | ~0.12 | S |
| 8 | PARSURA UNA across suites (fused corpus runner or shared doc cache) | ~1.4 (+~0.7 inutilia/obsoleta) | L, needs a ruling |
| 9 | Skip text round trip in the lint suites (writer already builds the StmlNodus tree) | ≤1.9 theoretical | M–L, needs a ruling |

Ideas 1–4 and 7 are independent single-file changes with low risk: together **~3.3–4 min per merge**.

---

## 1. aedilis closures via the thesaurus store — ~1.35 min

**Where:** `silva/probationes/apparatus.c:apparatus_clausuras_petere` (the popen string `./bin/aedilis --corpus '%s' --partes`). Also `apparatus_clausuram_petere` (single file, same fix).

**Evidence:**
- `apparatus_clausuras_petere` (waiting in fgets) = 20.2% of lint, 17.4% of canon_corpus, 20.7% of arbor_plagula.
- In aedilis itself, 72% is `_extractor_silvae → silva_c89_parsare_cum_contextu` (a full parse of every lib file plus headers) and 11% is `system()` (clang -MM for the 8 `.m` files that show up in closures).
- aedilis already has a cross-run, content-addressed extraction store. The key is SHA-256(aedilis binary ‖ aedilis.stml ‖ file bytes), and it is sound because silva does no file IO (lib/fabrica.worklog.md 2026-10-02, "plan 2 T3"). The harness just never passes it. The snippet generators all use `--thesaurus build/aedilis/obiecta`.
- Measured (cwd = repo, scratch store b1/A/thes):
  - no store: ~13–15 s (profile)
  - cold store: 22.0 s
  - **warm store: 2.6–3.3 s** (repeated runs: 3.05, 2.63, 3.29; once 5.0 under load)
  - Output is **byte-identical** to the no-store run (18,347 lines; `cmp` p0 = p1 = p2 = p6).
  - What remains in a warm run: 40% clang -MM for the `.m` files (they are never stored) and 31% `via_existit`/stat.

**Mechanism:** append `--thesaurus build/aedilis/obiecta` to both popen command strings. That is the same store as `FABRICA_THESAURUS`. fabrica_ordo.c:457 says any house action may write into it ("quaevis actio domus (aedilis --thesaurus, compilator) in eo scribit; involucrum"), so the test's child aedilis writing on a miss is sanctioned.

**Estimate:** aedilis minutes per merge = lint 3.0×0.202 + canon 2.4×0.174 + arbor 3.2×0.207 = 0.61 + 0.42 + 0.66 = **1.69 min**. Cutting ~15 s to ~3 s is about −80%, so **≈1.35 min**.
- Variant 1b: one fabrica generator writes the `--corpus lib --partes` answer to a file, and the three suites read it. That is ~2 runs × 3 s instead of 7 × 15 s, ≈1.6 min. But it adds a freshness contract (the generator must declare every lib/*.c and header as input), and a manual run that finds a stale file would be wrong. Do 1a first; 1b only if the remaining 3 s × 7 still matters.

**Risk / maintainability:** one-line change, same tool, same output.
- A cold store (first run in a fresh frigida clone before the snippet generators have warmed it) costs more than no store (22 s vs 15 s here). In practice frigida's build runs `fontes_generare` with the store before the suites, so lib/ should already be warm. Worth confirming with `bin/mensor`.
- Do NOT add `--memoria-oraculi` for cross-run reuse. It got 1.2 s, but the memo is keyed by path only (".mm" files named after the path), so it would go stale when headers change.

**Verify:** the three suites stay green with identical census numbers (`latinizatae == plagulae`, `apparatus_fracti == 0`). One-off: `cmp` the `--partes` output with and without the store (done: identical).
**Plant:** point `--thesaurus` at a store holding a corrupted record (truncate one blob). `_recordum_legere` must reject it and re-extract, and the suites must stay green. Separately, pass a bogus aedilis flag: the popen returns nothing, and the PORTA APPARATUS (`apparatus_fracti > 0`, `latinizatae != plagulae`) must go red.

---

## 2. Skip the second semantic analysis when nothing was re-canonicalised — ~0.8 min

**Where:**
- `silva/instrumenta/principalia/percursus.c` ~l.795–833 (BIS-ANALYSIS), used by the examen-corpus gate via `silva/percursus.sh -semantica`
- `_recipe_examinis` in `probatio_silva_exemplaria_obsoleta.c:320–330` and the verbatim copy in `probatio_silva_exemplaria_inutilia.c`
- presumably `examen.c` itself (same recipe)

**Evidence:**
- `silva_c89_semantica_analysare_cum_systemate` = **50.7%** of a `percursus -semantica` run (profiled here: b1/A/prof_perc.txt, 243 s under load), 20.9% of inutilia, 18.2% of obsoleta. It is called twice per file, unconditionally.
- `silva_recanonicare` returns "Numerum versorum" (silva_commissio.h:209–214). Across the whole percursus run (1,444 report lines): **`clausura: 4 versae`** in total. So for practically every file the tree the second analysis sees is the same tree the first one saw.

**Mechanism:** `versae = silva_recanonicare(...)`, then re-analyse only `si (versae > 0)`. Otherwise keep sem1 as the authoritative result.
- Precondition to check first: sem1 and sem2 are equal when the tree is unchanged. Analysis must be a pure function of (commissio tree, systema), and must not mutate the parsura or read oracle state that `_augere`/`responsa_vacare` changed.
- The comment "sem1 = pabulum clausurae (annotationes eius abiciuntur)" suggests sem1's annotations are discarded rather than written into the tree. Confirm that.
- Side benefit: the two test copies of "recipe examinis verbatim" become a natural place to extract one shared recipe function, so the copies can't drift.

**Estimate:** assume the two analyses cost about the same, so the second is ~half the semantic share.
- examen-corpus: 2.3 min × percursus share of the gate (~0.75, assumed; see open questions) × 0.507/2 ≈ 0.44
- inutilia: 2.6 × 0.209/2 ≈ 0.27
- obsoleta: 1.4 × 0.182/2 ≈ 0.13
- Total **≈0.8 min**. Also ~25% off every `examen` hook/LSP diagnostic call: a usability win outside the merge.

**Risk:** medium-low, and it hinges on the purity precondition. If analysis is impure, results silently differ. That is why the differential check below is mandatory before landing.

**Verify:**
- One-off differential: build both variants in scratch, run percursus over the repo, and diff the per-file diagnostics. They must be byte-identical.
- inutilia/obsoleta divergentes must stay 0 with sums unchanged.
- Keep a fixture where `versae > 0`: there are 4 in the corpus; pin one in `fixa/examinis`.

**Plant:** skip the re-analysis even when `versae > 0`. The pinned flipping fixture must go red (its verdict or diagnostics differ). If no test catches that plant, the gate is missing and must be added first.

---

## 3. excludenda: `-fsyntax-only` and one-pass classification — ~0.55–1.1 min

**Where:** `tools/amalgama_excludenda_generare.sh`: `_amalgama_compilare_admonens` and the per-name `while read` loop (`_classificare`, `_nomen_originale`, the `case " $(echo $PROTECTA) "` subshell, `grep -Fxq`).

**Evidence:**
- The current tree reaches its fixed point in **6 gyri** (build/aedilis/excludenda/silva/admonitiones_1..6.txt); 169 names in total.
- Per-gyrus pieces measured here (unloaded):

| Piece | Time |
|---|---|
| amalgamator build | 0.27 s |
| `--enumerare` + run | 0.44 s each |
| clang `-c` of the 5 MB amalgam | **1.96 s** |
| same amalgam with `-fsyntax-only` | **0.79 s** |
| `aedilis --plures` mechanism closure | 0.49 s |

- **Diagnostics with `-fsyntax-only` are byte-identical to the recorded admonitiones for all 6 gyri**, including the "is not needed and will not be emitted" lines (gyri 3/4) and the implicit-declaration error (gyrus 1). Checked in b1/A/s2_N vs r_N.
- `_classificare` over the 169 final names alone took **8.9 s** (17 bases × up to 3 files, one `grep` process each, plus `_corpora_socia`), before counting the per-name awk in `_nomen_originale`.
- My accounted components come to ~31 s per run against a measured 74 s per run in the gate (load factor ~2.4×).

**Mechanism:**
- (a) Replace `-c ... -o amalgama_messis.o` with `-fsyntax-only`. The .o is never used: only the warnings file is read.
- (b) Once per gyrus (or once per run: the definition files don't change between gyri), build a name→basis map with one awk over the candidate files in BASES order. The first basis whose file defines `^name *(` wins, which keeps "PLAGULA DEFINITIONIS PRIMA"; the longest-prefix fallback stays in awk.
- Likewise load `renominationes.tsv` and PROTECTA once into awk/assoc arrays, and look up each name instead of forking.

**Estimate:**
- (a) 6 × 1.17 s × 2 runs = 14 s unloaded, ≈0.23–0.55 min with the gate's load factor.
- (b) ~9–11 s × 2 runs ≈ 0.33–0.8 min.
- Total **≈0.55–1.1 min**.

**Risk:** low. The classification order must be preserved exactly (the 2026-07-21 lesson about chorda_internare* living in internamentum.c is the case to keep).

**Verify:** `./tools/amalgama_excludenda_generare.sh -probare silva` (and tessera, officina) must say "recens", i.e. the output is byte-identical to the committed `excludenda_generata.h`.
**Plant:** reverse the BASES order in the new classifier, or drop `materia/fontes/$b.c`. `-probare` must report RANCIDUM, or the script must die with "nomen inclassificabile".

---

## 4. `materia_arbor_lexema_ex_tag`: lookup table instead of the linear re-mangling scan — ~0.5–0.6 min

**Where:** `materia/fontes/materia_arbor.c:268` `materia_arbor_lexema_ex_tag`. For every lookup it loops over all `numerus_generum` kinds and calls `materia_arbor_lexema_tag()` for each one (strlen prefix + strlen title + lowercase/`_`→`-` copy), then memcmp. It is called per lexeme and per trivia token by the silva_arbor reader (`_lexema_legere`, `_trivia_legere`).

**Evidence:** arbor_plagula: `materia_arbor_lexema_tag` = **15.7%** inclusive (10.3% under `_trivia_legere`, 4.8% under `_lexema_legere`), inside `silva_arbor_legere_parsuram` 39.4%. The same reader runs twice per file in oraculum-silvae's `vivum -legere` (oraculum_silvae.c:260, 279), and in every tool that reads parse documents.

**Mechanism:** build the forward-mangled tags once, by calling `materia_arbor_lexema_tag` for i = 0..n−1. That keeps the "mangulatio ANTRORSA, directiones divergere non possunt" property: still one definition of the mapping. Put them in a small `tabula_dispersa` (or a sorted array) created once per reader session/document, or lazily cached on the ratum. Lookup becomes O(1).

**Estimate:** arbor_plagula 3.2 min × 0.157 × ~0.9 ≈ **0.45 min**, plus part of oraculum-silvae's vivum read (unmeasured, maybe +0.05–0.15). Total **≈0.5–0.6 min**.

**Risk:** low. Purely internal; same results by construction.

**Verify:** arbor_plagula `octetim_exactae` = all, `directa_exacta` unchanged; oraculum-silvae "sanum".
**Plant:** build the table off by one (index i+1). The octet comparison must report divergences immediately.

---

## 5. lint/obsoleta: read the tree text once, expand twice — ~0.45 min

**Where:** `probatio_silva_exemplaria_lint.c:_relatum_alterum_expandere` (PORTA CATENAE) and `probatio_silva_exemplaria_obsoleta.c:_relatum_alterum_expandere` (diribitio form). Each re-concatenates the same tree text with a second lint document and calls `stml_legere` on the whole thing again.

**Evidence:** `_relatum_alterum_expandere` = 20.7% of lint and 20.6% of obsoleta. Within that, the second `stml_legere` of the full tree text ≈ 10.8% / ~11%. The two lint documents are tiny and constant.

**Mechanism:** `stml_expandere` is non-destructive ("arbor NOVA; originalis intacta", stml_macros.h:223). So:
1. Per file, `stml_legere` the tree text once, plus the two small lint docs with the same intern.
2. Build root = tree children + lint children, then expand.
3. Swap the lint children for the catena/diribitio children, then expand again.

This needs either a detach API or two hand-built roots. You must first confirm that `stml_legere` builds no document-level index at read time that the expansion depends on (fragment ids, `<<#id>>` resolution, line numbers). If it does, this needs an `stml_documenta_iungere` helper in stml (batch B's territory).

**Estimate:** lint 3.0 × 0.10 + obsoleta 1.4 × 0.105 ≈ **0.45 min**.

**Risk:** medium. The test's claim is "textus UNUS"; splicing must give exactly the same tree as reading the concatenation.

**Verify:** catena_divergentes / diribitio_divergentes stay 0. One-off: `stml_scribere` the spliced root vs the concatenated-read root must be byte-equal for every file.
**Plant:** splice the lint doc ABOVE the tree instead of below ("lex stratorum: exemplaria supra vident"). The lint counts must diverge from the oracle.

---

## 6. oraculum-silvae: cache the pinned oracle's output by content — ~0.4 min (split unmeasured)

**Where:** `materia/oraculum_probare.sh` step 4: `$BASIS/$PIGNUS/oraculum -stml "$DOC" <~600 files>`.

**Evidence / reasoning:**
- The pignus is silva at a PINNED commit and silva parsing is pure in file bytes (no IO), so its per-file line and STML document are a fixed function of (pignus binary, file bytes) forever.
- Only vivum's side changes from merge to merge. I did not measure the pignus/vivum split. Vivum reads two documents per file plus its own parse, so the pignus is likely ≤ half.

**Mechanism:** key = SHA-256(pignus oraculum binary ‖ file bytes), stored in the same thesaurus store. On a hit, write the cached document into `$DOC` and emit the cached line; run the pignus only on misses. Simplest version: an `-stml` wrapper in the oraculum tool with `--thesaurus`.

**Estimate:** 1.0 min × ~0.45 pignus share × ~0.9 hit rate ≈ **0.4 min**.

**Risk:** low. The pinned binary is part of the key; a PIGNUS bump invalidates everything.

**Verify:** a cached run and an uncached run produce identical `pignus.txt` and `documenta/`. The gate stays "sanum (nominati K)".
**Plant:** corrupt one cached document. Vivum's `lectio=idem`/`comparator` must name that file.

---

## 7. canon_corpus `_involucrum_numerare`: single scan — ~0.12 min

**Where:** `probatio_silva_canon_corpus.c:93–135`. Eight `_quot_occurrit` calls, each a naive memcmp at every byte offset of the whole document.

**Evidence:** **5.4%** of canon_corpus (a child of `main`).

**Mechanism:** one pass. At each `'<'`, compare against the 8 prefixes.

**Estimate:** 2.4 × 0.054 × 0.9 ≈ **0.12 min**.

**Risk:** trivial.

**Verify:** the `vidit_*` counts are unchanged (they are printed in the census).
**Plant:** drop `"<extentum"`. `CREDO_VERUM(vidit_extentum > 0)` must go red.

---

## 8. PARSURA UNA across suites — ~1.4 min (+~0.7), needs a ruling

**Observation:** lint, canon_corpus and arbor_plagula each do the same thing for lib/*.c:

    apparatus_parsare(..., praebere=VERUM) → silva_arbor_scribere_parsuram(SILVA_C89_REGISTRUM, "c89", fons_princeps)

That produces the same document three times per run. inutilia and obsoleta run the identical examen recipe plus writer on the same lib corpus.

**Shares:**
- canon: parse 9.4% + write 18.1% = 27.5%
- arbor: 13.0% + 25.3% = 38.3%
- obsoleta: recipe parse 14.3% + semantics 18.2% + write 24.0% ≈ 57% (shared with inutilia)

**Estimate:**
- canon 2.4 × 0.275 + arbor 1.9 × 0.383 (the m3 run stays separate) ≈ **1.4 min**
- inutilia+obsoleta fused ≈ 1.4 × 0.5 ≈ **0.7 min** more

**Two shapes:**
- (a) One "corpus" runner that parses each file once and runs all judges, with the existing suites becoming sections that keep their own censuses and CREDO verdicts. This is the house idiom already: "PARSURA UNA, IUDICES DUO".
- (b) An on-disk document cache keyed by (silva code digest ‖ file ‖ closure bytes).

**Cost:** both hurt failure isolation and the `tegit`/porta selection (silva-m3 runs arbor_plagula alone). (b) makes tests consume another test's output. Effort L. I'd do 1–4 first and re-measure; this is a structural decision for Fran.

---

## 9. Skip the STML text round trip in the lint suites — ≤1.9 min theoretical, needs a ruling

**Observation:** `materia_arbor_scriptor_finire(scriptor, involucrum, textum)` already builds the full StmlNodus tree. It has a `textum=FALSE` mode and returns `fructus.arbor`. Text is produced only to be re-read by `stml_legere` in the lint suites. `silva_arbor_scribere_parsuram` doesn't expose the tree today.

**Shares:**
- lint: `stml_legere` 21.6% + `stml_scribere_sedibus` 8.2%
- obsoleta: 21.9% + 8.8%
- inutilia: 13.1% + 10.5%

**Theoretical saving:** 3.0×0.30 + 1.4×0.31 + 2.6×0.24 ≈ 1.9 min.

**Why it needs a ruling:**
- The lint tests' claim is about the document a consumer reads. canon_corpus and arbor_plagula still cover the text path, so coverage of the writer→reader path is not lost.
- But a writer-built tree may differ structurally from a read-back tree (text nodes, fragment flags, intern identity), and lint would then be judging something no consumer sees.
- It also needs the tree interned with the lint doc's intern.
- Only worth pursuing if a one-off check shows `stml_scribere(writer tree)` == `stml_scribere(stml_legere(text))` for the whole corpus.

---

## Checked, not worth it
- **Header IO in `apparatus_parsare`** (re-reading up to ~100 headers per lib file): `apparatus_plagulam_legere` is 0.0–0.1%. Header pre-processing (`silva_expansio_directivas_processare`) is ~3% and inherent.
- **`apparatus_clausuram_ex_corpore`** linear strcmp over 384 paths: 0.0%.
- **Systema lexicon compose/parse per file** in the recipe: `silva_lexicon_componere` 0.2%, `silva_c89_parsare(systema)` 1.3–1.7%. The compose step takes the file's bytes (extern blocks), so it can't simply be hoisted. ≤0.05 min.
- **excludenda amalgamator rebuild per gyrus** (0.27 s) and amalgamator run (0.44 s): small. The mechanism-closure aedilis call goes 0.49 → 0.24 s with `--thesaurus`: trivial (fine to add for consistency).
- **excludenda warm start** (seed lists from the committed header to converge in 1 gyrus): unsound. A seeded run can settle on a non-least fixed point (dead mutual-recursion clusters never warn), and stale over-exclusions get misattributed through signum B (implicit declaration → caller excluded).
- **`--memoria-oraculi` across runs** for the `.m` clang -MM calls (2.6 → 1.2 s): keyed by path only, so it goes stale when headers change.
- **Running PORTA CATENAE / diribitio on a sample of files** instead of all: it would cut idea 5's cost entirely, but it weakens what the gate proves.

## Open questions
1. **Why does `excludenda_silva` run twice (n=2) in frigida?** If it's generator + `-probare` on the same tree, one run could go: another ~1.2 min.
2. **examen-corpus split:** what share is `percursus -semantica` vs the fixture battery (each fixture spawns clang + examen)? `bin/mensor` should know. This sets idea 2's real number. The fixture loop's per-fixture process spawns may themselves be worth a batch mode.
3. **oraculum-silvae pignus vs vivum split** (idea 6).
4. **Is the thesaurus store warm** in frigida by the time the silva suites run? If not, idea 1 costs +7 s per suite on the first run there.
5. **Purity of semantic analysis** (idea 2's precondition): does `silva_c89_semantica_analysare_cum_systemate` mutate the parsura?
6. **Out of batch A:** frigida reruns every suite on the same tree the fusio gates just ran. A verdict memo keyed by binary digest + lectiones inputs would remove that duplication entirely, but it conflicts with frigida's purpose (prove a cold clone works). Fran's call.
7. **Library internals seen in passing** (batch B): `canon_iudicare_expansum` 25.8% + `canon_iudicare` 15.3% of canon_corpus (`_elementum_quaerere_vi` 6.9%, `_est_caps_machinae` 3.5%); `materia_arbor_scriptor_finire → stml_scribere_sedibus` 8–11% everywhere.
