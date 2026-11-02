> Raw analyst report, batch G of budget B2 (2026-10-31), filed verbatim as evidence for ../budget-fusionis.md.

# Batch G — root suite (probationes/) heaviest tests, normal + venenum (ASan)

Analyst: batch-G agent (Opus 5.5), 2026-10-30/31. Nothing in the repo was
changed except this file. Prototypes, binaries, profiles and diffs:
`/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/e70c1844-689a-4b85-85a0-00afb0db9030/scratchpad/b2/G/`
(`src/` copies, `patch_*.diff` = exact prototype edits, `prof/` = 1 ms
`sample` profiles, `nectere.sh` relinks fabrica's object list with one
object replaced, `par.sh` = concurrent A/B runner, `pa.py`/`lineae.py`/
`folia.py` = profile analyzers).

## 0. Timing basis and multipliers

- Gate run `01a18f3f20b8-bb90-1` (all 53 gates): porta:radix 26.6 s
  (4/450 members ran, rest served from fabrica's verdict cache),
  porta:venenum 15.3 s (cached; communio_servus ran). Frigida run
  `01a18f68da2a-81aa-1`: **frigida:radix 455 s** (history 326-634 s),
  i.e. the root suite runs in full exactly once per merge, via
  `./compile_tests.sh` with `FILA = hw.perflevel0.physicalcpu = 4`
  (`xargs -P 4`, each slot = compile test + link + run).
- Per-test cost: summed CREDO section ms from `build/fabrica/area/
  probationes_radicis/*/credo.tsv` (quinta, 10-29/30): **450 tests,
  227 s**; the 71 tests ≥ 300 ms hold 213 s.
- **New measurement: real vs CPU for those 71 tests** (scratch copies of
  the area binaries, cwd = repo, TMPDIR = scratch; `/usr/bin/time`,
  load 6-10): the 69 non-cellae tests take 182 s wall but only 52 s CPU
  — **≈130 s of the root suite's 227 s is waiting** (poll timeouts,
  nanosleep, real-time rules). cellae + cellae_lenta are the CPU
  (≈56 s at -O2).
- Venenum (ASan+UBSan, -O1, `-DPISCINA_VENENUM=1`), area credo.tsv:
  cellae 107 s, cellae_lenta 69 s, laqueus_rete 18 s, laqueus 10 s,
  communio_servus 5.5 s, reticulum 4.8 s. Venenum runs members in
  parallel, so its wall ≈ cellae ASan whenever cellae's closure is
  invalidated (10 of the last 15 venenum gates were heavy, 46-485 s).
- **Multipliers used below.** (a) frigida: a test's wall W occupies one
  of 4 slots, so cutting W saves ≈ W/4 of frigida wall (slots are busy:
  frigida radix is compile-heavy), plus tail effects. (b) radix gate:
  wall ≈ slowest *invalidated* member; vicus_quies (24 s here, **48 s**
  in secunda's version, see G2) and cellae (27-35 s) are the caps.
  (c) venenum: wall ≈ cellae ASan, ×0.7 frequency.
  Loads are recorded per measurement (they ranged 7-78 during this
  session); only concurrent A/B ratios are used for estimates.

## Ranked table (est. minutes per merge)

| # | Idea | Where | min/merge | Effort | Status |
|---|---|---|---|---|---|
| G2 | vicus_quies: inject the monotonic clock (5 s / 1 s rules) so "nothing happens in 5 s" checks advance virtual time | lib/vicus_applicatio.c + probatio_vicus_quies.c | **0.3-0.5** | M | IUDICANDUM (API) |
| G6 | compile_tests links each test against a static archive, not 419 loose objects | compile_tests.sh `probatio_una` | **0.2-0.3** (+6 GB less written) | S-M | CANDIDATUM (out of batch: frigida owner) |
| G7 | radices temp pools: reuse a scratch pair instead of create/destroy per call (D5 option a, refreshed numbers) | lib/radices.c `_piscina_temporaria` users | 0.2-0.3 | S-M | IUDICANDUM (gate strength) |
| G1 | radices bisection: cache the left-end sign; defer the "(m+1)/10^d exact root" check | lib/radices.c `_seca_medio`, `_ad_chordam_intra` | **0.15-0.2** (measured) | S | CANDIDATUM |
| G3 | exemplar test: pump until the client socket is readable instead of 20 fixed 1 ms polls per step | probatio_exemplar_applicatio.c `_exspectare` | 0.05-0.1 (−8..−12 s wall/run) | S | CANDIDATUM |
| G8 | D4 restated with fresh shares (hoist exact axis norms) | lib/quaternio.c:831 | 0.1 | M | IUDICANDUM (additive API, as before) |
| G4 | hospitium_dilatum: 256 KB client read buffer (TCP loopback window stalls) | probatio_hospitium_dilatum.c:84 | 0.05 (−7.9 s wall/run) | S | CANDIDATUM |
| G10 | fixed-count pumping in other network tests (hospitium, http_vocatio, imperium, mcp_servus_canalis, ianua, ...) | probationes | 0.05-0.08 | M | CANDIDATUM (naive fix flaked — needs read-until-complete) |
| G5 | communio: `agere(MMD)`/`agere(D)` before a positive assertion → pump until the error appears | probatio_communio.c:1419, 1155 | 0.02-0.03 (−3.0 s wall/run) | S | CANDIDATUM |
| G9 | `_termini` memo per process (k → roots), not per activation | lib/cellae.c:1068 | 0.03 (after G1) | S | IUDICANDUM (static state) |
| G11 | fractio: all-inline s64 fast path; `_quotiens` by 1 → return | lib/fractio.c | 0.02-0.04 (CPU, unmeasured) | S-M | CANDIDATUM |
| G12 | contractio inner product loop (65% self of laqueus_rete) | lib/contractio.c:1801-1823 | 0.02-0.04 (CPU, unmeasured) | M | CANDIDATUM |
| G13 | exemplar test argon2 parameters (19 MiB, t=2 per login) | exemplar config | 0.02 (−1.8 s CPU/run) | S | IUDICANDUM (config API; production params by design) |
| G14 | hospitium vigilia timer is 1 s, so a 200 ms `responsum_maximum_ms` fires at ~1 s | lib/hospitium.c:1897 | 0.01 | S | IUDICANDUM (library behaviour) |

Sum of CANDIDATUM ≈ 0.55-0.8 min; with the IUDICANDUM items ≈ 1.2-1.7
min per merge. Most of this batch is *wall* (waiting tests), not CPU:
G1, G6, G7, G8, G11-G13 cut CPU; G2-G5, G10, G14 cut idle time.

---

## G1. radices bisection: cached left sign + deferred exact-decimal check — MEASURED

- **Where:** `lib/radices.c` `_seca_medio` (≈684) and its 8 call sites
  (`radix_signum_polynomii` 802, `_compara_intra` 894/936/942,
  `_angusta_intra` 997, `_ad_chordam_intra` 1106/1161); the
  (m+1)/10^d block in `_ad_chordam_intra` (1139-1158).
- **Evidence:** profile of probatio_cellae at -O2: `_termini` **26.9%**
  inclusive (it turns the roots tan²(mπ/k) into 20-digit text with
  `radix_ad_chordam`, then `strtod`), `_signum_ad` 26.9%,
  `magnus_multiplica` self 24.4% — the multi-limb loop (magnus.c:806)
  inside the homogeneous Horner of `_signum_ad`. Under ASan: `_termini`
  17%, `_seca_medio` 14%. 83% of `_termini` comes from one section,
  "k sine limite" (two activations, k = 1..70).
- **Mechanism (two independent, byte-identical changes):**
  1. In bisection the sign just right of `infra` never changes: infra
     only moves to the midpoint m when sign(m) == sl, and then the sign
     right of m is sign(m) = sl. Today `_seca_medio` recomputes it with
     `_signum_iuxta` every step (one full `_signum_ad`, sometimes two).
     Pass an `s32* notum` (0 = unknown) owned by the calling loop.
  2. `_ad_chordam_intra` evaluates f at c = (m+1)/10^d every iteration
     (plus a `fractio_ex_magnis` gcd against 10^20) to catch roots that
     are exact decimals. That check can only change the outcome once
     supra·10^d < m+2 (otherwise bisection keeps narrowing towards c
     and reaches that state anyway). Guard the block with that compare.
  The output text is floor(r·10^d) for the true root r, independent of
  how the interval narrowed, so any faster narrowing prints the same.
- **Measured** (concurrent A/B, `par.sh` A2/A3, load 11-17, full suite,
  33/33 sections green, stdout identical with ms stripped):

  | build | v0 user | v3 user | ratio |
  |---|---|---|---|
  | -O2 (radix) | 24.83 s | 21.42 s | 0.86 |
  | ASan (venenum) | 81.41 s | 72.18 s | 0.89 (real 97.4 → 87.6 s) |

  Part 1 alone (v2): -O2 0.94, ASan 0.95. "k sine limite": **0.44×**
  (-O2 4.8 → 2.1 s) and 0.42× under ASan (13.2 → 5.6 s); other
  sections 0.87-1.0. probatio_radices and probatio_plexus green.
- **Estimate:** venenum −9..−10 s per heavy run (cellae stays above
  lenta's 69 s) × 0.7 ≈ 0.11 min; radix gate −3.4 s when cellae is the
  cap; frigida −3.4/4 s ⇒ **≈ 0.15-0.2 min**.
- **API:** none (static functions only). **Risk:** low, with one trap
  (below). **Effort:** S (prototype diff `patch_radices_v3.diff`, 147
  lines with context).
- **Plants:**
  - (p2) the deferred check never fires (`si (FALSUM)`) →
    **probatio_radices FRACTA** (4/6 sections). Caught.
  - (p1) after the negation branch in `_ad_chordam_intra` (a ← f(−t) on
    (−supra, −infra)) the cached sign is NOT reset → **survives
    probatio_radices, probatio_plexus** (and cellae, whose roots are
    positive). That is a finding: no test covers `radix_ad_chordam` on a
    negative root whose isolating interval straddles 0. The real change
    should scope the cache to the second loop (a fresh local there, so
    the stale-cache bug cannot be written) and add a radices test for
    that case (e.g. a root near −0.3 isolated in (−1, 1)).

## G2. vicus_quies: an injectable monotonic clock for the tmux rules

- **Where:** `lib/vicus_applicatio.c` `terminalia_debita` (3510-3525:
  `TERMINALIA_MS` 5 s, `TERMINALIA_CONFIRMATIO_MS` 1 s),
  `locum_terminalis_inspicere` (1917, `MEMORIA_LOCI_TERMINALIS_MS` 1 s),
  all via `fenestra_tempus_ms()`; test `probationes/probatio_vicus_quies.c`
  `_probare_frontem` (775-819) and `pulsare_usque` (757-773).
- **Evidence:** real 24.3 s, CPU 0.36 s (load 8). Section "frons" 19.2-
  21.8 s, flat across 83 runs — it is a schedule, not work: fixed waits
  of 2.5 + 5.5 ("no question in 5 s") + 3.5 + 3.5 + 6.5 ("only the 5 s
  rule") s. **Secunda's version (not in main) is 48 s**: new sections
  quaestio_defecta 10.5 s, comes_sine_processu 6.4, frontes_multae 5.7,
  eventus_tmux 3.7, same pattern (`historia titulus=probationes_radicis/
  probatio_vicus_quies`: 48.1 s at 0d4fef552f). Once merged it is the
  slowest root test by far and the radix gate's cap whenever vicus
  libraries change.
- **Mechanism:** route those three reads through a clock the test can
  set (vicus already has `VicusHorologium` for the wall clock — a
  monotonic sibling, e.g. `vicus_tempus_ponere(v, fn, ctx)`, or a field
  on `ContextusAdmonitionum`). The test then advances virtual time by
  5 s instead of sleeping. Real processes stay real: the `sleep 1` typed
  into the emulator shell and the fake tmux script still take their 1 s.
- **Estimate:** frons ≈ 21 → 4-6 s; secunda's version 48 → ≈ 12 s.
  Radix gate (when invalidated): 48 → next member (cellae ~27 s) ⇒
  −21 s; frigida −35/4 ≈ −9 s (and it sits at the alphabetical tail,
  see note N1) ⇒ **≈ 0.3-0.5 min**.
- **API:** additive public setter (header) → **IUDICANDUM**. **Risk:**
  medium — production must keep using the real clock (default = real);
  the test's meaning changes from "5 real seconds" to "5 clock
  seconds". **Effort:** M.
- **Plant:** with a virtual clock, set `TERMINALIA_MS` to 4 s → "nulla
  quaestio in V s" must go red (it would today too, but slowly).

## G3. exemplar test: pump until readable — MEASURED

- **Where:** `probationes/probatio_exemplar_applicatio.c` `_exspectare`
  (190-203): each step is `_pumpare(e, XX)` (20 × `hospitium_gressus(…,
  1 ms)`) then one `tcp_recipere` with a 1 ms `SO_RCVTIMEO` (line 90).
- **Evidence:** real 20.6 s, CPU 3.7 s. Profile: 11.6 s in `poll`
  (reactor_poll ← hospitium_gressus ← `_exspectare`) and 5.0 s in
  `recvfrom`. The response is ready after the first gressus, the other
  19 sleep 1 ms each: every HTTP exchange costs ≥ 20 ms of nothing.
  (argon2 runs as a reactor *labor*, which already makes poll's timeout
  0 — that part is fine.)
- **Mechanism:** one gressus per step, then `poll(fd, POLLIN, 0)` on the
  client (`tcp_obtinere_fd`, public) and `_progredi` only when readable;
  bound the loop at 20× the old count. `_progredi` already accumulates
  partial reads until Content-Length, so split responses are safe.
- **Measured** (concurrent, 4 pairs, loads 23-78): v0 22.6/43.0/35.8/33.7 s
  → v1 15.5/11.9/24.2/27.9 s; 41/41 sections green every time. Typical
  quieter-load saving ≈ 8-12 s per run. CPU unchanged.
- **Estimate:** frigida −2..−3 s; radix gate 0 today (not the cap; it
  becomes the cap after G2 + G1) ⇒ ≈ 0.05-0.1 min. **API:** none.
  **Risk:** low. **Effort:** S (`patch_exemplar_v1.diff`).
- **Plant:** none possible for a pure wait cut — the assertions are
  untouched. Make `_paratum` return FALSUM always → every exchange
  times out → suite red (detected); return VERUM always → old behaviour.

## G4. hospitium_dilatum: TCP window stalls — MEASURED

- **Where:** `probationes/probatio_hospitium_dilatum.c:84`
  (`i8 capsa[XVI * MXXIV]` in `_legere_responsum`).
- **Evidence:** real 9.5 s, CPU 0.08 s. filum_magnum 4.4 s, directorium
  _magnum 4.3 s: 99% in poll inside `hospitium_gressus(h, 10 ms)`. The
  server fills the socket, then waits for writability; the client reads
  16 KB at a time, which on loopback does not reopen the window enough,
  so every 16 KB costs one full 10 ms poll timeout (6 MB / 16 KB ≈ 384 ×
  11 ms).
- **Mechanism:** a 256 KB client buffer (`hic_manens`, or piscina) — the
  client drains the receive buffer, the window update is immediate. What
  the test checks (server-side small connection pool, exact bytes) is
  unchanged.
- **Measured:** 9.54/9.34/9.35/9.50 s → **1.67/1.66/1.55/1.62 s**, 9/9
  sections green each run. Remaining 1.0 s = "expiratum" (G14).
- **Estimate:** ≈ 0.05 min (frigida slot + gate when hospitium changes).
  **API:** none. **Effort:** S (`patch_hosp_dil_v1.diff`, 1 line).
  **Plant:** none (sizing only).

## G5. communio: fixed pumping before positive assertions — MEASURED

- **Where:** `probationes/probatio_communio.c` 1419 (`agere(&o, MMD)`,
  "massa nimia") and 1155 (`agere(&o, D)`, "tessera mala").
- **Evidence:** real 10.4 s, CPU 0.67 s; 2.5 + 0.5 s pump for a fixed
  time, then assert that an error is present.
- **Mechanism:** `donec_error(o, c, tectum)` = pump until
  `status.error.mensura > 0`, same upper bound; the assertion follows.
- **Measured:** 10.3-10.7 s → **7.3-7.8 s** (9/9 runs green).
- **Do NOT** do the same at 1082 ("servus cadit", `agere(&o, MD)`): the
  first prototype did, and under load 30 the next step's
  `synchronizare` failed (rc=1, plus 3 volumes reported unclosed). The
  1.5 s there is load-bearing for the client's reconnect state. Negative
  windows (2240/2244/2250, "nothing is sent") must stay fixed too.
- **Risk:** the new form checks the error when it first appears, no
  longer that it is still there 2.5 s later (stickiness). Fran's/owner's
  call whether stickiness was intended. **Effort:** S
  (`patch_communio_v2.diff`). **Estimate:** ≈ 0.02-0.03 min.
- **Plant:** invert the predicate (stop at once) → "massa nimia"
  assertion red (expected; not run).

## G6. compile_tests: link tests against an archive (out of batch — flag)

- **Where:** `compile_tests.sh` `probatio_una` (659-660): every test
  links `$(cat build/test_logs/obiecta_probationum.txt)` = **419
  objects** + Cocoa/Security/WebKit/IOKit.
- **Evidence:** one link: real 0.30-0.49 s, user 0.31, sys 0.10-0.14;
  binary **18.4 MB**. `build/probationes/` in quinta = **5.9 GB**. With
  `ar rcs libradix.a <same 419>` (0.36 s once): real 0.17-0.22 s, user
  0.23-0.25, sys 0.04-0.05, binary **129 KB**; probatio_chorda passes
  identically.
- **Estimate:** ≈ 0.14 s CPU × 450 ≈ 60 s CPU per frigida ⇒ ≈ 15 s wall
  at 4 slots, plus ~6-8 GB fewer bytes written per cold clone (sys time,
  SSD) ⇒ **≈ 0.2-0.3 min**. (The radix *gate* already links per-closure
  through fabrica; this is frigida and local `./compile_tests.sh`.)
- **Risk:** archives pull members by reference only: `.m` objects with
  categories/`+load` need `-ObjC`; duplicate symbols between unrelated
  libs stop being a link error in this runner (other gates link them
  together). **Effort:** S-M. **Plant:** drop `-ObjC` → any test using an
  ObjC category fails to link or crashes (check which suites do).

## G7. radices temporary pools: reuse instead of create/destroy (D5a, re-raised)

- **Where:** `lib/radices.c` `radix_compara` (960-961),
  `_separa_positivas` (540-549), `radix_signum_polynomii` (832-833),
  `radix_ad_chordam` (1189-1190): two `_piscina_temporaria()` per call.
- **Evidence (after D5's 4 KB pools):** ASan cellae inclusive
  `piscina_generare_dynamicum` 12.0% + `piscina_destruere` 10.2%;
  `wrap_malloc` 14.1%, `wrap_free` 10.8% (most of it these pools:
  radix_compara 16+16%, `_separa_positivas` 27% of create/destroy
  samples). ≈ 20 s of the 107 s venenum cellae run. At -O2 only 3.2%
  (system malloc is cheap) — it is an ASan cost.
- **Mechanism:** module scratch pair marked once, `piscina_reficere`
  per call (nested `radix_compara → radix_signum_polynomii` needs its
  own pair or a mark/reset discipline), exactly D5 option (a).
- **Estimate:** −15..−20 s per heavy venenum ⇒ cellae ≈ 70 s with G1,
  i.e. level with cellae_lenta (69 s), which then caps venenum ⇒
  ≈ 0.2-0.3 min (×0.7). **Gate strength (why IUDICANDUM):** ASan's
  heap-use-after-free on these pools becomes VENENUM's 0xA5 fill on
  reset — still caught, but as wrong values instead of an ASan report.
- **Effort:** S-M. **Plant:** forget `piscina_reficere` on the reused
  scratch → radices apex-bound tests (`radicum_apices_purgare`) red.

## G8. D4, fresh numbers (still open)

`quaternio_proximi` is **24.0%** of probatio_cellae at -O2 and 17.2%
under ASan. Under ASan the two `_productum_scalare` calls in
`_proximus` split 49% (829, v·u_k) / **51% (831, u_k·u_k)**, i.e.
line 831 alone = 5.6% of the ASan run (≈ 6 s); at -O2 roughly half of
24% ≈ 2.5 s. Estimate unchanged from D (≈ 0.1 min); the API question is
unchanged too.

## G9. `_termini` memo across activations

`_termini(a, k)` (cellae.c:1068) caches per *activation*; the roots
depend only on k. "k sine limite" computes k = 1..70 in two fresh
activations. A process-wide table (k → Termini) would halve that: after
G1 ≈ −1 s at -O2, −2.8 s ASan ⇒ 0.03 min. Static mutable state in a
library + memory owned for process lifetime ⇒ IUDICANDUM; not worth
it unless G1 is rejected (then ≈ −3.5 s / −9 s, 0.1 min).

## G10. Fixed-count pumping in other network tests

Same shape as G3 in many suites (from the real-vs-CPU table):
hospitium 2.6 s wall / 0.01 CPU (`_commercium`: `_pumpare(h, X)` = 10 ×
10 ms before one recv, plus `_pumpare(h, V)` after each connect),
mcp_servus_canalis 2.6/0.14, ianua 2.4/0.13, vicus_comites_terminale
2.4/0.19, http_vocatio 1.7/0.03, pseudoterminale 1.7/0.11, imperium
1.4/0.01, vitrea_servus 1.4/0.01, reactor_labores 1.2/0.00,
credentialia 1.2/0.05, cliens_tabularii 1.2/0.07, http 1.0/0.02,
credo_volumina 1.0/0.03 (≈ 22 s wall, < 1 s CPU in total).
**Caution, measured:** a naive "pump until readable, then one recv" in
probatio_hospitium went 2.6 → 0.8-1.0 s but **failed 1 of 2 runs**
("directorium staticum": the body arrives in a second write; the old 10
pumps hid that). Each needs a read-until-complete helper (Content-Length
or EOF). A shared `probationes/` helper header would be a new file
(Fran). Some of these (credentialia "frenum", reactor timers) are real
library timers, not pumping. ≈ 0.05-0.08 min, M.

## G11. fractio all-inline fast path (CPU, unmeasured)

probatio_laqueus (3.7 s CPU, 10 s ASan): `situs_transitus_parametri`
94% → `fractio_multiplica` 53%, `fractio_adde`/`subtrahe` 55%;
self: `magnus_divide` 29.5%, `magnus_divisor_communis` 24.8%,
`magnus_multiplica` 21%. Every fractio op is 2 gcds + 4 divides + 2-3
multiplies through out-of-line Magnus calls, all on the i64 fast paths.
When all four parts are inline and < 2^31, do the Henrici formulas in
s64 inside fractio.c (one function, no calls); and `_quotiens(a, 1)` →
`a` (d1, d2 are usually 1). Guess 30-50% of laqueus ⇒ ≈ 1.5 s -O2,
4 s ASan CPU; not on any critical path ⇒ 0.02-0.04 min. Plants:
probatio_fractio's property suite (a+b−b = a, canonical form).

## G12. contractio inner product (CPU, unmeasured)

probatio_laqueus_rete (5.3 s CPU, 18 s ASan): `contractio_exsequi_modulo`
**65% self**, all at the inlined `_gradum_modulo` loop
(contractio.c:1813-1823): i64 product, add, branchy carry count, strided
indices. Ideas: products are < 2^62, so accumulate up to 3 products then
reduce (no per-term carry branch); a contiguous (pa == pb == 1) variant
the compiler can unroll. Must give identical residues (it is exact
modular arithmetic) — verify on the full tabula tota oracle. 0.02-0.04
min, M.

## G13. exemplar test pays production argon2 per login

custodia defaults m = 19456 KiB, t = 2, 32 blocks per reactor gyrus
(custodia.c:149-160); `_labor` + `argon2id_gradus` = 11% of the
exemplar profile ≈ 1.8 s CPU per run. ExemplarConfiguratio has no
argon2 field, so a test value needs a config addition ⇒ IUDICANDUM
(the production parameters may be the point). 0.02 min.

## G14. hospitium vigilia granularity

`_vigilia_callback` runs on a 1000 ms timer (hospitium.c:1897), so a
200 ms `responsum_maximum_ms` produces its 503 after ~1 s;
hospitium_dilatum "expiratum" spends exactly 1000 ms there (asserts
< 1500). Timer period = min(1 s, responsum_maximum) would answer on
time and cut 0.8 s. Library behaviour ⇒ owner's ruling. 0.01 min.

---

## Waiting tests: inventory (wall vs CPU, load 6-10)

| test | real s | CPU s | where the wall goes | fix |
|---|---|---|---|---|
| vicus_quies | 24.3 (48 in secunda) | 0.36 | 5 s / 1 s real-time rules | G2 |
| exemplar_applicatio | 20.6 | 3.7 | 20 × 1 ms polls per step | G3 |
| communio | 10.4 | 0.67 | fixed `agere` | G5 (partly) |
| hospitium_dilatum | 9.5 | 0.08 | loopback window + 10 ms polls | G4 |
| vicus_saltus | 9.3 | 0.54 | `nanosleep(6 s)` for **tmux's own 5 s query timeout** (restitutio) | none — external program |
| communio_servus | 5.8 | 0.63 | long-poll `mora=1` is in **seconds** (API), 4 × ~1 s; `epistulas_exspectare` negative 1 s windows | none without API (also in venenum, not its cap) |
| pictor_communio | 5.2 | 0.12 | sync client timers (`donec_advenit` already conditional), `agere(M)` negative | library timers |
| colloquia_visus | 4.1 | 0.11 | `nanosleep(1.5 s)` (file must NOT appear) + messem | negative timing, keep |
| pictor_inscriptio | 3.4 | 0.15 | `donec_titulus` + reactor timers | library timers |
| hospitium … credo_volumina | ≈ 22 total | < 1 | fixed pumping / timers | G10 |

## Checked, not worth it

- **radices VCA stack (688 KB per `_separa_positivas`)**: the acervus is
  sized 3 × (4096 + 2) × 56 B and allocated per call — looked like the
  ASan mmap/munmap source. Prototype with a 48-entry growable stack:
  -O2 28.92 → 28.81 s user, ASan 102.1 → 101.1 s (concurrent A1) —
  noise. Large untouched allocations are cheap here.
- **Closed form tan²(mπ/k) for `_termini`**: libm `tan` is not
  correctly rounded; the 20-digit-then-strtod path is. Doubles could
  differ in the last ulp and move a float filter decision. G1 gets most
  of the cost byte-identically.
- **Magnus shift for dyadic q in `_signum_ad`** (bisection midpoints are
  c/2^h, so q^i is a shift): needs a new public magnus function; after
  G1 the remaining share is smaller. Later, if ever.
- **Float pre-filter for `_signum_ad`** (decide early bisection signs in
  rigorous interval doubles): would cut most exact evaluations but needs
  a proven error bound; M-L and risky for a ≤ 0.1 min gain.
- **cellae_lenta (69 s ASan, 29 s -O2)**: still D7 territory —
  `_vector_parare` 37% (-O2) / 32% (ASan) self+inclusive, `_iudica_intra`
  (inlined at cellae.c:3460) 12% self, `_cella_vectoris` 11%; pool churn
  only 3-8%. No cheap win found beyond D7's algorithmic idea.
- **Shortening negative windows** (communio 2240-2250, communio_servus
  `epistulas_exspectare(r, e, I)`, colloquia_visus 1.5 s): they test
  that nothing happens; a shorter window is a weaker test. Not proposed.

## Notes

- **N1 (scheduling, outside the "less CPU" scope — mention only):**
  compile_tests feeds `xargs -P 4` in alphabetical order, which puts the
  long waiters vicus_quies (24-48 s), vicus_saltus (9 s) and the vitrea
  tests at the very end of frigida radix. Longest-first order (from the
  mensor medians) would hide them under the other slots' work; no extra
  cores. Rough gain 10-30 s of frigida tail. Fran's ruling whether this
  counts as "parallelism".
- **N2:** when venenum's cellae drops (G1 + G7), cellae_lenta (69 s)
  becomes venenum's cap; then laqueus_rete (18 s) is far behind.
- **N3:** the G5 flake (servus cadit) and the G10 flake (hospitium)
  both came from removing waits that *looked* idle. Every wait cut in
  this batch should be soak-tested (≥ 5 runs under load ≥ 20) before
  landing.

## Open questions

1. G2: may vicus grow a public monotonic-clock setter (default = real
   clock)? It is the single biggest item here once secunda's sections
   land.
2. G7: is VENENUM's 0xA5 fill an acceptable replacement for ASan's UAF
   report on radices' scratch pools?
3. G5: was "the error is still set after 2.5 s" (stickiness) part of
   what "massa nimia" tests?
4. G6 belongs to whoever owns compile_tests/frigida — please route.
