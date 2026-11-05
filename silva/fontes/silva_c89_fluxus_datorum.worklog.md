# silva_c89_fluxus_datorum.c — worklog

## 2026-11-05 — variable index maps (budget fusionis, main walk item 1)

`_variabilis_index` and `_variabilis_membri_index` were find-or-add by linear scan. Corpus-wide that was 1.52 M calls / 15.2 M steps here, and semantics' interval lookups did another 2.9 M calls / 72 M steps over the same list. Now `FluxusDatorum` carries two `TabulaDispersa*` (NIHIL below `FLUXUS_INDEX_LIMEN` = 16 variables; the average function has ~7, where a linear scan is cheaper than hashing).
- **Key:** the identity pointer's bytes (+ member title bytes), composed in a caller stack buffer of 256 (lookups allocate nothing). Keys are copied into the arena only on insert.
- **Order:** insert only if absent, so the FIRST index wins, which is what the linear scan returned. (There are no duplicates anyway; every variable is created by find-or-add.)
- **Threshold crossing:** the map is built from all existing entries in order, then maintained on each append.
- **Long titles** (> 248 bytes) aren't indexed, and their lookups fall back to the linear scan, so the two paths agree.
- **Public:** `silva_c89_fluxus_datorum_variabilis` / `_membrum` (map or linear), used by semantics' interval lookups. Data flow builds its own index and semantics only reads it, so the header rule (semantics reads datorum, never the reverse) holds.

## 2026-11-05 — census of the three fixed points (budget fusionis, main walk item 3 prep)

Measurement only, no code change. Scratch probe (s17) over `percursus -semantica`,
report identical to stock apart from the timing line.

Instructions per step, corrected for probe overhead (~5.5k instr per counter read):

| step | G |
|---|---|
| event extraction (tree walk + callbacks) | 2.53 |
| may/must (already bitsets) | 0.91 |
| forms fixed point | 2.54 |
| sign-provenance fixed point | 5.36 |
| total over 65,245 builds | ~11.3 |

65,245 builds = 52,007 main path (`_fluxum_examinare`) + 13,238 from the credo
paths (`_credo_scripta_colligere` reads only `eventa`; `_credo_suitam_profundam_colligere`
reads only stirpes).

Lesson: wrapping per-call callbacks with proc_pid_rusage counters is useless here -
21 M counter reads = ~234 G overhead; `canonicum` is called 12.4 M times and costs ~nothing.
Keep counters at phase granularity, count everything else.

Census (corpus, all builds):
- 715,874 blocks, 371,112 variables (60,144 member rows), 1.52 M events,
  318,695 DEFINITIO, 28,740 MEMBRUM_DEFINITIO, 0 def-omnia.
- forms and stirpes each: ~1.47 M visits, 104 M cells copied, 145 M cells joined.
- forms: only 3,693 variables (1.0%) ever get a SUBTRACTIO definition; 62,422 builds
  (95.7%) have none. The sole reader (`_formas_examinare`) acts only on SUBTRACTIO.
  Builds without subtraction hold 67% of the cells (the 4.3% with subtraction are big).
- stirpes: only 663 variables (0.18%) ever hold PRIMA+n; 64,790 builds (99.3%) have none
  (99.3% of cells too). Main path: only 7,461 / 52,007 functions sit in files with any
  signed type registered; elsewhere `_datorum_stirps_ambitu_ligamen` returns NEUTRA at
  once and the reader returns before reading.
- stirpes exit computation scans ALL variables for members of the defined base on every
  DEFINITIO on every visit: 64.9 M iterations.

## 2026-11-05 — item 3: sign-provenance fast path + member chain

**What changed** (`_punctum_fixum_stirpium`):
- **Member chain.** `_membra_basium_ligare` builds `primus[basis]` / `proximus[membrum]` once per
  build. The full path's "clear all members of the defined base" now walks that chain instead
  of scanning every variable on every DEFINITIO on every visit (64.9 M iterations per corpus run before).
- **Fast path** (`_stirpes_celeres_licent` -> `_punctum_fixum_stirpium_celere`). Taken when there is
  no context-aware classifier (`stirps_valoris_ambitu == NIHIL`) and no DEFINITIO /
  MEMBRUM_DEFINITIO carries a provenance outside {IGNOTA, NEUTRA, AMISSA}. Then two bitsets per
  block side: ATTACTA and AMISSA; IGNOTA (0,0), NEUTRA (1,0), AMISSA (1,1); join = OR of both,
  which equals `_stirpem_iungere` on those three values. Bits past n_var are kept 0 (def-omnia
  masks the last word), so "some bit changed" <=> "some cell changed" and the work list runs in
  exactly the same order as the full path. At the end the bits are expanded into the same s32
  `stirpes_introitus/exitus` tables (unvisited blocks stay IGNOTA, as before).
- **Semantics gate** (`_datorum_stirpes_ligare`, silva_c89_semantica.c): after `_contractus_colligere`,
  an empty `signata_typorum` means both `_datorum_stirps_ligamen` and `_datorum_stirps_ambitu_ligamen`
  would return NEUTRA immediately, so it passes NIHIL for both. Data flow then writes NEUTRA into
  `ev->stirps` itself (same value) and takes the fast path. Used by the main path and
  `_credo_scripta_colligere`; the credo deep suite keeps its own `_credo_*` classifiers (full path).

**Why exact.** The three-value lattice has no widening and the encoding is a bijection, so even the
visit sequence is unchanged, not just the fixed point.

**Checks.**
- **Whole-corpus differential** (scratch: HEAD datorum renamed `vetus_*`, stock callbacks, vs new
  code with gated callbacks): 64,829 builds (12,815 credo), 53,907 on the fast path (83%),
  26.9 M provenance cells + every event / variable / may-must / forms cell: 0 mismatches.
- **Suite:** fast vs forced-full (`_probatio_stirps_neutra`, an always-NEUTRA context-aware
  classifier) cell comparison on: two members + base redefined on one branch, a one-branch
  definition (NEUTRA / IGNOTA), def-omnia via `sizeof(g2)`, a loop + unreachable block, and
  70 members (second word, long chain).
- **Plants, 5/5 red:** OR->AND on the lost bit; members set NEUTRA on the fast path; chain unlinked;
  expansion reads word 0; semantics gate inverted (probatio_silva_signata: two codex-82 flow cases
  lose their diagnostic).
- **Not testable:** the last-word mask has no observable effect (junk bits are never expanded, and
  extra visits can't change a lattice with no widening). It is kept for tidiness.

**Result** (overhead-corrected phase probe):
- provenance 5.36 -> 1.33 G; data flow ~11.3 -> ~7.3 G.
- Full-path visits 1.47 M -> 122 k; cells copied 104 M -> 2.6 M.
- `percursus -semantica` median 355.5 -> 351.0 G; report identical (3/3 runs).
