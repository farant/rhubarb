# BUDGET FUSIONIS — where a merge's minutes go, and the ledger of cuts

*Born 2026-10-27/28 (budget park 01M5ZVPVG1C6S9X170P726V7D2, rows
B0–B9). Fran's question: "what are some other things we could do
that are single threaded ... that could bring it down?", with the
rule "every intervention is worth it as long as it doesn't break the
usability / maintainability of the library", expecting "40 1-2
minute wins adding up". This file is the B1 deliverable: the
measured anatomy of one merge, five analyst batches' ideas (raw
reports in `budget-fusionis/ideae-A..E.md`), merged, de-duplicated,
ranked, and split into what needs a ruling and what does not.
MAINTAINED: when an idea lands, mark it LUCRUM with its commit and
its measured cohort delta (`bin/mensor quaestio cohortes`).*

*Discipline note: estimates here are PROFILE ARITHMETIC taken at
machine load 11-13, not landed results. Counts (calls, re-runs,
compiles) are exact; per-call seconds are pessimistic. An idea is
only LUCRUM when its cohort says so. Every landing goes through
`silva.commissio(..., interventio="...", budget="B1:<id>")` so the
cohorts can attribute it.*

Status vocabulary: **CANDIDATUM** (no ruling needed; ready to
implement) · **IUDICANDUM** (needs Fran's ruling first) ·
**IN CURSU** · **LUCRUM** (landed; commit + measured delta) ·
**REIECTUM** (ruled out, reason given).

## I. Anatomy of a merge (Methodus)

Merge 7dc70441 into quinta, 2026-10-27, from the run table
(`~/.rhubarb/mensurae.volumen`, genus 'cursus'): 63 root runs,
105.9 min; 2139 leaf runs, 91.1 min (86% of roots — the rest is
parent overhead).

| Part | Measured | Typical | Note |
|---|---|---|---|
| merge commit (`silva.commissio`) | 17.3 | ~17 | git hook 151 s of it (B5) |
| `fusio_probare` gates | ~52 | ~52 | sequential; silva 7.1, venenum 4.4, instrumenta 3.3, pythonica 3.3 |
| frigida (cold clone, every suite) | 36.5 | **~21** | this run overlapped a secunda commit; median of ~23 runs is 18-23 |
| **total** | **~106** | **~90** | |

Library exposure via fabrica's recorded reads (`build/fabrica.db`
lectiones) was USELESS as a cost proxy: only 18.5 of 91 leaf minutes
map, dominated by titles that compile half of `lib/`. What a test
compiles says nothing about where its runtime goes. Ranked leaf
FAMILIES instead (`frigida:`/`porta:` prefixes and `#sectio` stripped)
and profiled the heaviest with macOS `sample -file` at 1 ms.

Top families (min per merge, all runs summed): oratio 10.6 (frigida
8.5 — contention, see above), radix 6.5, pre-commit hook 5.7 (23
runs), briar 5.4, pythonica 4.3, ludus_tessera 4.2, probatio_cellae
3.5 (= all of venenum), instrumenta 3.3; the silva corpus cluster
~17 (exemplaria_lint 3.0, inutilia 2.6, excludenda_silva 2.5,
canon_corpus 2.4, examen-corpus 2.3, arbor_plagula 1.9, obsoleta 1.4,
oraculum-silvae 1.0).

## II. Tier 1 — CANDIDATUM (no ruling needed), ≈ 11-17 min

| id | Idea | min | eff | Status |
|---|---|---|---|---|
| E2 | silva/*.sh and 38 other wrappers: ONE newest-header `find` per call, not one per object (~330 wrapper calls per merge commit; test runners already do "CAPUT RECENTISSIMUM SEMEL") | 1.5-3 | S-M | LUCRUM 9c92a730 |
| D1-6 | Magnus 24 → 16 B (arm64 passes >16 B structs via memory; ASan makes every copy a call), i64 Euclid when inline, extensio normalisation shortcuts, hoisted axis norms, piscina churn; split `lentus` into its own suite | 2.4-2.9 | S-M | D1+D2 LUCRUM e8dcabb6, D3 e8be2b15, D5 1272c226; D4 + lentus split CANDIDATUM |
| A1 | `silva/probationes/apparatus.c`: pass `--thesaurus build/aedilis/obiecta` to aedilis (store exists; 15 s → 3 s, byte-identical) | 1.35-1.6 | S | LUCRUM cf6834d6 — 93.1 G → 9.5 G instructions per aedilis call |
| E3 | raw-clang suites (briar, oratio, ludus_tessera, tessera, gesta, officina, ...) → `bin/compilator --plures` (warm ludus_tessera rebuilt 152/155 objects after a header touch) | 1.5-3 | L | CANDIDATUM |
| B3-14 | stml internals: capture pass skips untouched parents, tokenizer scans runs, `_lineae_contenti` early exit, transparency set as array, parser intern cache, one-append indentation, writer tag cache, ... | ~1 | S-M | B5+B9 LUCRUM 20bd074d; B3/B6 deferred (move computus golds again); rest CANDIDATUM |
| A3 | `tools/amalgama_excludenda_generare.sh`: `-fsyntax-only` (diagnostics byte-identical, 1.96 → 0.79 s per round) + one-pass awk classification | 0.55-1.1 | S | LUCRUM c9c9a0ae — silva 12 → 6.2 s |
| E7 | `vendor/sqlite3.c` and `silva/amalgama/silva.c` compiled once (normalised path via compilator) | 0.5-1 | S | LUCRUM b3a306e9 |
| C1 | xar: header `static __inline__` fast paths for obtinere/numerus/addere behind same-name macros (4.7 → 2.1 ns) | 0.5-0.8 | S-M | REIECTUM (for now) — silva amalgamator breaks on header-inline helpers; needs tooling work; ThinLTO alternative |
| A2 | skip the second semantic analysis when `silva_recanonicare` changed nothing (4 re-canonicalisations in the whole corpus) | 0.6-0.8 | S-M | REIECTUM — purity fails: +3 typations corpus-wide on unchanged trees (ledger) |
| E6 | `--memoria-oraculi` on the derivation calls in `tools/aedilis_porta.sh` and `tools/instrumenta_fumus.sh` (190 `clang -MM` runs for 8 distinct `.m` files) | 0.7-0.8 | S | LUCRUM cfc28972 |
| E8 | `bin/compilator --plures`: compile misses in FILA chunks (serial since 2026-10-23) | 0.5-0.75 | S-M | LUCRUM 72a5f370 — 414 misses 70.5 → 29.2 s |
| A4 | `materia_arbor_lexema_ex_tag`: tag → genus table built once per reader (15.7% of arbor_plagula) | 0.45-0.6 | S | LUCRUM d55701b0 (+7 computus golds: allocationes_arboris +3/doc) |
| C2 | piscina allocation fast path (no udiv, no 12-register prologue; 10.4 → 4.0 ns) | 0.3-0.45 | S | LUCRUM 76d45eb5 (macro fix 20bd074d) |
| E10 | instrumenta: skip relink when up to date; constant provenientia stamp in fumus | 0.3-0.5 | M | CANDIDATUM |
| A6 | oraculum-silvae: cache the PINNED oracle's output by content | 0.3-0.4 | M | CANDIDATUM |
| C3 | `chorda_aequalis_literis` compares while scanning (zero tests today — add first) | 0.25-0.35 | S | LUCRUM 3f011d2a (first tests of the function) |
| C4 | hash-table growth re-places with the cached hash | 0.15-0.25 | S | LUCRUM c87ab9cb (174k-line differential identical) |
| C5-7 | aedificator byte path, `invenire_literis` one strlen, internare single probe | 0.1-0.2 | S | C5+C6 LUCRUM e6d18854; C7 skipped (API addition) |
| A7 | canon_corpus `_involucrum_numerare` single scan | 0.1 | S | LUCRUM 20bd074d |

Sum 13.0-19.7; ~15% overlap discount → **≈ 11-17 min**.

Merged duplicates: A1 = B's out-of-batch flag; A4 = B1; A5 = B2;
A7 = B7; B10+B12 ≈ C3; E7/E8 partly fold into E4; E5 shrinks to
~0.5 once E2 lands.

## III. Tier 2 — IUDICANDUM (Fran's ruling), ≈ 6-10 min

| id | Idea | min | Question |
|---|---|---|---|
| E1 | content-keyed gate receipts survive the commit: fusio re-ran 18 gates already green in the commit on identical content (246 s) | 3-4 | Is fusio's re-run meant as a second proof / flake hunt? |
| E4 | persistent compile cache for frigida (fixed path, or `-fdebug-compilation-dir=.`) | 1-2 | Must frigida exercise the compiler, or is clone content + content-keyed cache + a periodic truly-cold run enough? |
| A8 | one corpus parse shared by the silva suites | 1-1.5 | Worth losing per-suite failure isolation / silva-m3 selection? |
| A-q | `excludenda_silva` runs twice (n=2) in frigida | 0-1.2 | Why twice? |
| E5 | git hook skips work commissio just did (token bound to index blobs) | 0.5 | May the hook trust a token? |
| A5 | lint/obsoleta: read the tree text once, graft the lint docs | 0.45 | Is "textus unus" concatenation itself under test? |
| E9 | merges: `formator -delta` only on blobs differing from both parents | 0.25-0.5 | lex basis for merges |
| D-P2 | every-Nth oracle case under ASan | ? | gate strength |
| C8 | ThinLTO for test links | ? | measure link cost first |
| A9 | expand the writer's in-memory tree, skip the STML text round trip in lint suites | ≤1.9 | probably REIECTUM — the text is what these gates test |

## IV. Totals

- Tier 1: ≈ 11-17 min → typical merge ~90 → ~73-79 min.
- Tier 1 + 2: ≈ 17-27 min → ~63-73 min (−20-30%).
- Not counted: B7 (frigida suites in parallel, 8-12) and B8 (fusio
  gate groups in parallel, 15-25), parked separately.

## V. LUCRUM (landed)

| id | Commit | Cohort delta | Note |
|---|---|---|---|
| A1 | cf6834d6 | pending | aedilis --thesaurus in silva apparatus |
| E6 | cfc28972 | pending | --memoria-oraculi in aedilis/instrumenta gates |
| E2 | 9c92a730 | pending | tools/caput_recens.sh; nexus gate stage VI |
| A3 | c9c9a0ae | pending | excludenda -fsyntax-only + map |
| A4 | d55701b0 | pending | arbor reader tag index |
| E7 | b3a306e9 | pending | vendored sources via compilator |
| C2 | 76d45eb5 | pending | piscina fast path |
| C3 | 3f011d2a | pending | chorda_aequalis_literis |
| C4 | c87ab9cb | pending | hash growth by cached hash |
| C5+C6 | e6d18854 | pending | aedificator byte path; literal length once |
| A7+B5+B9 | 20bd074d | pending | canon single scan; stml line count + indentation |
| D1+D2 | e8dcabb6 | ASan laqueus 48-77 → 16.6 s | Magnus 16 B; i64 Euclid |
| D3 | e8be2b15 | pending | extensio normalisation shortcuts |
| E8 | 72a5f370 | pending | radix libs in FILA parallel parts |
| D5 | 1272c226 | ASan cellae 212-318 → 120-133 s (D1-D5) | 4 KB temp pools |

Lessons (2026-10-28/30): deferred fusio-tier gates hid a C2 regression
for 4 commits (canon_corpus: empty macro) — run touched deferred suites
before committing; computus golds pin allocation counts (A4 re-gold).

## AUDIENDA (not verified)

- Every minute figure above is an estimate from profiles taken under
  load; none is landed. The cohort query is the arbiter.
- Venenum runs its suites in parallel (wall ≈ slowest suite); D's
  A/B shows laqueus 98 → 22 s with the same fixes, so the cap moves —
  not yet confirmed on a real venenum run.
- D only pays when a merge invalidates the cached cellae verdict
  (closure includes piscina, credo, chorda, tabula_dispersa, ...);
  how often that happens is unmeasured.
- B's numbers were scaled ×1.5 (it used a 7-min base for its five
  suites; the family table says 11.3).
- Frigida's compile-vs-test split per suite is estimated from script
  reading, not measured (E §3-4).
- C1 turns three xar names into function-like macros as well — the
  tooling impact (examen, legati, renominare, amalgam prefix renaming)
  is unchecked.
- A2's precondition (semantic analysis does not mutate the parse) is
  unchecked.
