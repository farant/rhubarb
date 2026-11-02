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
| D1-6 | Magnus 24 → 16 B (arm64 passes >16 B structs via memory; ASan makes every copy a call), i64 Euclid when inline, extensio normalisation shortcuts, hoisted axis norms, piscina churn; split `lentus` into its own suite | 2.4-2.9 | S-M | D1+D2 LUCRUM e8dcabb6, D3 e8be2b15, D5 1272c226; lentus split (D8) LUCRUM cb5547a7; D4 LUCRUM (quaternio_proximi_quadrata, Fran 10-31) |
| A1 | `silva/probationes/apparatus.c`: pass `--thesaurus build/aedilis/obiecta` to aedilis (store exists; 15 s → 3 s, byte-identical) | 1.35-1.6 | S | LUCRUM cf6834d6 — 93.1 G → 9.5 G instructions per aedilis call |
| E3 | raw-clang suites (briar, oratio, ludus_tessera, tessera, gesta, officina, ...) → `bin/compilator --plures` (warm ludus_tessera rebuilt 152/155 objects after a header touch) | 1.5-3 | L | ludus_tessera LUCRUM (header touch 139 → 4 s); tessera LUCRUM (B2: warm 4 → 1 s, header touch 48 → 1 s, test link skip); gesta LUCRUM (warm 8 → 7 s, header touch 54 → 10 s); officina LUCRUM (warm 16 → 11 s, header touch 68 → 11 s); briar LUCRUM (warm 12 → 8 s; objects 0 rewrites after header touch — corpus_symbola pre-step 83 s remains, separate); oratio LUCRUM (76 recompiles avoided; wall time unchanged: tests ~60 s, nexus -renovare 51 s after a header touch, separate) |
| B3-14 | stml internals: capture pass skips untouched parents, tokenizer scans runs, `_lineae_contenti` early exit, transparency set as array, parser intern cache, one-append indentation, writer tag cache, ... | ~1 | S-M | B5+B9 20bd074d, B14 67cd1d19, B4 6658d77c, B8 7b8ee375, B6 dbd049a9, B3 c568b6d4 (single computus re-gold) LUCRUM; B11 skipped (API addition); B13 deferred (medium risk) |
| A3 | `tools/amalgama_excludenda_generare.sh`: `-fsyntax-only` (diagnostics byte-identical, 1.96 → 0.79 s per round) + one-pass awk classification | 0.55-1.1 | S | LUCRUM c9c9a0ae — silva 12 → 6.2 s |
| E7 | `vendor/sqlite3.c` and `silva/amalgama/silva.c` compiled once (normalised path via compilator) | 0.5-1 | S | LUCRUM b3a306e9 |
| C1 | xar: header `static __inline__` fast paths for obtinere/numerus/addere behind same-name macros (4.7 → 2.1 ns) | 0.5-0.8 | S-M | REIECTUM (for now) — silva amalgamator breaks on header-inline helpers; needs tooling work; ThinLTO alternative |
| A2 | skip the second semantic analysis when `silva_recanonicare` changed nothing (4 re-canonicalisations in the whole corpus) | 0.6-0.8 | S-M | REIECTUM — purity fails: +3 typations corpus-wide on unchanged trees (ledger) |
| E6 | `--memoria-oraculi` on the derivation calls in `tools/aedilis_porta.sh` and `tools/instrumenta_fumus.sh` (190 `clang -MM` runs for 8 distinct `.m` files) | 0.7-0.8 | S | LUCRUM cfc28972 |
| E8 | `bin/compilator --plures`: compile misses in FILA chunks (serial since 2026-10-23) | 0.5-0.75 | S-M | LUCRUM 72a5f370 — 414 misses 70.5 → 29.2 s |
| A4 | `materia_arbor_lexema_ex_tag`: tag → genus table built once per reader (15.7% of arbor_plagula) | 0.45-0.6 | S | LUCRUM d55701b0 (+7 computus golds: allocationes_arboris +3/doc) |
| C2 | piscina allocation fast path (no udiv, no 12-register prologue; 10.4 → 4.0 ns) | 0.3-0.45 | S | LUCRUM 76d45eb5 (macro fix 20bd074d) |
| E10 | instrumenta: skip relink when up to date; constant provenientia stamp in fumus | 0.3-0.5 | M | LUCRUM (see V) — link skip only (stamp not needed); aedilis gate too |
| A6 | oraculum-silvae: cache the PINNED oracle's output by content | 0.3-0.4 | M | LUCRUM (see V) — warm gate 91 → 42-45 s |
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
| E1 | content-keyed gate receipts survive the commit: fusio re-ran 18 gates already green in the commit on identical content (246 s) | 3-4 | RULED 2026-10-30 (Fran: not a second proof) → LUCRUM (see V) |
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
| D8 | cb5547a7 | pending | lentus section in its own suite (parallel) |
| E1 | 4a938445 | pending | receipts sealed on content; PORTAE_CAPUT keep HEAD |
| A6 | 056c1b7e | warm 91 → 42-45 s (load 13) | pignus cache keyed on objects + file sha |
| E10 | bc9ba4bc | instrumenta 76 → 59 s, aedilis 162 → 108-128 s | link signature (flags + object identity), opt-in by gates |
| B14 | 67cd1d19 | pending | stml raw/escaped writers append runs |
| B4 | 6658d77c | pending | stml tokenizer moves by runs (memchr) |
| B8 | 7b8ee375 | pending | parser trivia intern cache |
| B6 | dbd049a9 | pending | transparency set as inline array |
| B3 | c568b6d4 | allocationes_arboris −17-22%/doc | capture pass keeps lists without captors (computus re-gold) |
| E3 (ludus_tessera) | (this commit) | header touch 139 → 4 s | compilator --plures; excubitor call removed |

First measured merge after tier I (2026-10-30, quinta ff 0a2b4879;
ONE sample, sexta committing concurrently; `mensor quaestio fusio`):

| Part | Typical before | 0a2b4879 |
|---|---|---|
| merge commit | ~17 min | 5.9 min (owed load varies - least trustworthy) |
| fusio gates (excl. frigida) | ~52 min | 41.4 min |
| frigida | ~21 min | 16.3 min |
| **total** | **~90** | **~64 (−26, ~28%)** |

Per gate (before = last 3-5 runs): venenum 264-413 → 46 s (D1-D5);
silva 375-428 → 205 s while running 21/57 members vs 4-5 (A1, A3);
frigida:silva 253-293 → 188 s; frigida:oratio 81-130 → 74 s; flat:
aedilis, instrumenta, pythonica, oratio, examen-corpus, frigida:radix.
Re-check with `mensor quaestio cohortes` after 2-3 more merges.

Lessons (2026-10-28/30): deferred fusio-tier gates hid a C2 regression
for 4 commits (canon_corpus: empty macro) — run touched deferred suites
before committing; computus golds pin allocation counts (A4 re-gold).

## VI. Round 2 (budget B2, 2026-10-31) — analysts F-J

Raw reports: budget-fusionis/ideae-F.md (silva corpus), -G.md (root
suite), -H.md (oratio/pythonica/briar + crusta anomaly), -I.md
(foundation libraries), -J.md (per-process overhead). All measured under
load 6-78 on a shared machine: instruction counts / concurrent A/B
ratios are the solid numbers. Duplicates merged: H4 = J2, G6 ≈ J5a,
J9 = E3 remainder, G8 = D4 (landed), H6 = healing-order fix (landed
f1656a51), F6 overlaps I5.

### Tier 1 (no ruling), ≈ 7-11 min/merge after ~20% overlap discount

| id | Idea | min | eff | Note |
|---|---|---|---|---|
| H2 | excubitor.sh scans only the filter's build dir (3 whole-tree finds over 415k entries → 1 dir); 56-60 s → 1 s per call, byte-identical | 2.3-3.7 | S | LUCRUM 729b7447: 51-56 → 0-1 s, byte-identical on 5 filters; new gate `excubitor` |
| H5 | cursor_communis.sh compiles an instrument closure with one `compilator --plures` (28 wrappers); 0.8-1.1 → 0.1-0.2 s per call | 0.7-1.2 | S | LUCRUM 09b1cde9: extenta.sh warm 0.18-0.21 s; pythonica 189 s at load 10; nexus fumus stage VII |
| H4/J2 | silva wrapper preambles: no `basename` fork per source; materia_substratum.sh one newest-header scan (E2 missed it); hook calls binaries directly after one freshness call | 0.5-1.2 | S | LUCRUM bf8731e5 (preambles + materia; hook-direct part of J2 not done): formator 0.137 → 0.083 s, examen 0.205 → 0.166 s |
| I1 | aedilis existence memo in `_existit_sub_radice` (book 3.8 M lines → 17.9 k unique); 28.3 → 4.6 G instr per `--corpus lib` | 0.4-0.7 | S | LUCRUM 359f1934, scoped PER DERIVATION (fabrica generates headers between derivations): 28.4 → 11.3 G, 5.35 → 2.8 s, book 721 k → 213 k |
| J9/E3 | the five remaining raw-clang runners (tessera, gesta, officina, briar, oratio) via compilator; also stop recompiling/relinking 84 test files every run | 0.4-0.5 | M | pattern = ludus_tessera 765f1f2e; tessera LUCRUM 1b7f5edc; gesta LUCRUM f3a54629 (shared link skip tools/nexus_probationis.sh); officina LUCRUM 85629289; briar LUCRUM c6ab2020; oratio LUCRUM (this commit) — E3 complete |
| F1 | silva_annotationes `_affigere`: unit extents in one walk for all sources; obsoleta share 10.9% → 2.2% | 0.35-0.45 | S | add a two-source annotation fixture first (none today) |
| J4 | content seal reused on a frozen tree, clean-tree fast path, seal once after commit (~260 seals/merge, 62 after one commit) | 0.3-0.5 | S-M | LUCRUM (this commit): (a) seal once per receipt sweep (7 receipts 1.04 -> 0.20 s; ~8 s/commit at 62), (c) clean-tree fast path (identical seal, small gain), (d) index path cache; (b) cross-gate reuse NOT done (needs explicit threading) |
| J6 | aedilis derivation runs `git rev-parse` twice per call for a comment (~611 calls/merge) | 0.3 | S | LUCRUM (this commit): AEDILIS_COMMISSUM from the gate + per-process memo; 48 -> 7 ms per derivation; aedilis gate 130 -> 78 s (one run) |
| J7 | aedilis struere.sh `vetustum()`: newest header once, not every header × every object (~1 M `-nt` per gate) | 0.3 | S-M | LUCRUM (this commit): vicus_iussa warm 150 -> 41 ms; new gate case "caput recentissimum" (no gate touched aedilis headers before) |
| G6/J5a | radix tests link against an archive / their closure, not all 419 objects (18 MB → 130 KB binaries, ~6 GB less per frigida) | 0.2-0.5 | S-M | `.m` objects need `-ObjC` |
| I4 | xar fast path via GNU `extern __inline__` (gnu_inline), guarded off in amalgams; both amalgamators reproduce committed files byte-for-byte | 0.25-0.4 | S-M | the C1 alternative; no API |
| F9 | silva-m3 / nexus compile silva.c through the E7 compilator key (13.3 s per compile) | 0.2-0.45 | S | on merges touching silva.c |
| J10 | vexilla.sh sourcing ~16 ms × ~1 300 per merge | 0.2-0.3 | S | |
| F10+F11 | canon_iudicare element-lookup memo + per-document title index (49% of canon_corpus) | 0.2-0.3 | S-M | profile only |
| G1 | radices bisection: cache left-end sign, defer exact-decimal check; −14% CPU -O2, −11% ASan | 0.15-0.2 | S | add a test for a negative root whose interval straddles 0 (plant survived) |
| I3 | credo child-wait backoff 16 µs → doubling (fixed 1 ms today; toml_totalitas 8.8 of 14 s asleep) | 0.15-0.25 | S | add a 50 ms survival test (timeout ×1000 too short undetected today) |
| F5 | _regionem_texere per-element extent cache (stack, no allocation) | 0.15-0.25 | S | run silva computus first |
| F8 | haruspex: one `clang -fsyntax-only` over 563 TUs, fallback per TU | 0.15-0.2 | S | |
| J8 | single-call compilator skips `clang -print-prog-name` (18 of 24 ms) | 0.15-0.25 | S | persistent memo variant = J8c (ruling) |
| H1 | stml_macros laxa matcher memoizes effective children per query (−17% instr, RSS 223 → 133 MB) | 0.15-0.2 | M | check computus allocation counts |
| I2 | lectiones per-process dedupe of book lines | 0.1-0.3 | S | LUCRUM (this commit): with I1, corpus lib 28.4 → 6.2 G, 5.35 → 2.1 s, book 721 k → 22 k |
| F6+I5 | trivia cache also "\n"+k spaces; `_lineae_excedunt` via memchr | 0.15-0.3 | S | |
| G3/G4/G5 | test wait cuts (exemplar pump-until-readable, hospitium 256 KB buffer, communio pump-until-error) | ~0.12 | S | soak under load before landing |
| J11, G10, I6, G11, G12 | small items | ~0.3 | S-M | |

### Tier 2 (Fran's ruling)

| id | Idea | min | Question |
|---|---|---|---|
| J1 | run-scoped regeneration memo shared by all fabrica processes of one commissio/fusio (115 s of repeat regenerations measured) | 1.3-1.9 | May fabrica trust a cross-process memo within one run? (needs a two-process test) |
| J3 | call /usr/bin/git and clang directly (skip xcrun trampolines, +5-8 ms × ~7 500 execs), export SDKROOT | 0.6-0.9 | Accept losing xcrun's implicit /usr/local/include? iOS/Android builds to verify |
| F12 | silva-m3 reads arbor_* numbers from the fresh content-keyed verdict instead of re-running them | 0.5 | Is re-running arbor_* part of silva-m3's purpose? |
| G2 | vicus_quies: injectable clock so "nothing happens in 5 s" checks advance virtual time (24 s wall, 0.36 s CPU) | 0.3-0.5 | Public setter on vicus (API) |
| F-A5 | A5 again: `_relatum_alterum_expandere` still 22% of lint, 17% of obsoleta | 0.25-0.5 | as B1 A5 |
| F3 | derived-type interning by hash (adds a field to SilvaSemantica in the header) | 0.2-0.3 | API (struct field) |
| G7 | radices: reuse a scratch pool pair instead of create/destroy per call | 0.2-0.3 | gate strength (stale use shows as VENENUM fill, not ASan) |
| J5b | frigida link semantics (closure vs all objects) | — | what must frigida prove about linking? |
| H8 | probatio_oratio_stml: drop the second write→read circuit | 0.1 | test semantics |
| H7 | mensor records system-sleep time per run (the crusta 431 s anomaly was clamshell sleep) | 0 (measurement) | schema change — RECOMMENDED: makes every future timing honest |
| C7, G9, G13, G14 | interning single probe (private header), _termini memo, argon2 test params, hospitium timer | small | API / static state / config |

### Findings that are not speed
- **F-X**: canon citation `lexematis-fons` (`super="*"`) never fires; a dangling `f="77"` gives 0 vitia and no test catches it.
- **G**: the root suite is mostly WAITING: ~130 of 227 s of test time is idle (poll timeouts, sleeps, real-time rules). cellae/cellae_lenta are the only real CPU cost.
- **H**: the 10-30 frigida crusta 431 s was laptop clamshell sleep on battery (pmset), not work; caffeinate does not prevent it.

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
