# puncta parallela: TL points across worker processes (L3)

Status: DRAFT v2 (2026-11-03). Shape agreed with Fran in chat
(2026-11-03): batch evaluator in interpolatio, a caller-supplied batch
hook in laqueus, workers as exec'd copies of uncinus_mensura, 4 workers
by default. Design review done (consiliarius-parallela: rework H1 and H2,
then approve after fixes); every finding placed in v2, see "Design
review" at the end. T0's concurrency experiment DONE; the rest of T0 was
answered by the review's measurements (see "T0"). Q1-Q3 DECIDED by Fran
2026-11-03 (all as recommended). T1 DONE 2026-11-03 (see "T1 notes").
T2 DONE 2026-11-03 (see "T2 notes").

Agreed order (interpolatio-verificatio-plan.md, status line): early
stopping (done, 624f407c), then parallel points (this plan, temperley-
celeritas-plan.md "L3"), then 16 × 801 as its own step.

## Purpose

After early stopping, a 14 × 801 bracket costs ~3,260 CPU-seconds
(7 of 23 primes, interpolatio-verificatio T3). 16 strands costs ~13×
per point (Catalan(16)/Catalan(14) = 13.2; measured 6.5-9 s/point), so
16 × 801 is ~12-14 h on one core. Each interpolation prime needs
K + puncta_addita independent points (563 at 14 × 801: T3 counted
7 × 563 = 3,941); the points of one prime are the parallel work. Target:
~2× wall time with 4 workers, what this machine gives (T0), so
16 × 801 in ~5-6 h.

## Governing rules

1. **Same answers, byte for byte.** UNCINUS, RELATIO and TRACTUS lines
   are identical for every worker count, including the evaluation
   counts. Parallelism changes how values are computed, never which
   values are used or in what order.
2. **Purity is the contract, and it is enforced.** A batch evaluator's
   result at (p, index) depends on nothing but (p, index). Determinism
   follows from this plus in-order consumption. Because workers are a
   second copy of the code in other processes, purity is also
   *checked*: every worker proves it built the same module
   (fingerprint), and laqueus re-evaluates one index of every batch
   in-process and requires bitwise equality (H2).
3. **Never an index serial would not reach.** Requests are capped by
   the deficit, by the evaluator's capacity and by `conatus_maximi`.
   On success, requested = consumed = serial. On a refusal inside a
   batch, up to (batch − 1) extra indices may have been evaluated;
   they are discarded and not counted, so RELATIO still equals serial
   (H1, M5).
4. **Failure refuses.** A crashed, timed-out, incomplete or mismatching
   worker makes the batch fail: FALSUM, `exitus` untouched. Never
   partial acceptance, never a silent serial fallback, no retry in v1.
5. **The library never spawns processes.** Workers live in the tool;
   the library sees a function pointer. (A design separation: the
   library stays a pure computation that any caller can drive. Not an
   examen rule: 96/97 covers path-based IO only, L1.)
6. **Two guards, two fault classes.** The certificate (certifying
   checks on their own evaluator) catches random faults in main values.
   Systematic divergence between workers and the in-process module
   (wrong word, stale binary, transport) is caught by rule 2's
   fingerprint and spot check. Without the second guard such a fault
   ends as a wrong PROBATUM AD_LIMITEM (H2), so neither guard is
   optional.
7. **Registered before measured**, as in the earlier plans.
8. **Proved stays the default.** Existing entry points, callers and
   tests are unchanged, including call counts on refusal paths; workers
   only on explicit request (`-opifices`).

## Design

### Interpolation (lib/interpolatio)

- New type, argument order aligned with `InterpolatioAestimator`
  (contextus, index, p, ...) (L12):

  ```c
  /* fasciculus: valores in indicibus index_primus .. + numerus - 1
   * primi p. status[j] UTILE/INUTILE/ERRATUM ut InterpolatioAestimator;
   * puncta[j], valores[j] scripti ubi UTILE. Functio PURA (p, index):
   * nihil a fasciculo pendet. FALSUM = defectus totius (ERRATUM). */
  nomen b32 (*InterpolatioAestimatorFasciculi) (
      vacuum* contextus,
         i32  index_primus,
         i32  numerus,
         i32  p,
         i32* puncta,
         i32* valores,
         s32* status);
  ```

- `interpolatio_polynomium_relatum_fasciculis(fasciculus, contextus,
  capacitas, &verificator, q, piscina, &exitus, &relatio)`: same
  contract as `interpolatio_polynomium_relatum`, batch main evaluator,
  `capacitas` = the largest batch it accepts (0 = unbounded).
- **One internal loop with a capacity (H1).** `_primum_aestimare` and
  the self-check take an internal evaluator source {batch function,
  context, capacity}. The existing single-point API passes an adapter
  with capacity 1, so it makes exactly today's calls, in today's order,
  stopping at the same index. All existing call-count tests stay green
  (`_probare_puncta_vitiosa` l.1053, `_probare_defectum_additi`
  l.1196, `_probare_erratum_et_inutile` l.638/645). Public signatures
  of `interpolatio_polynomium`, `_relatum`, `_promovere` unchanged.
- **Per prime:** request min(necessaria − collecta, capacity,
  conatus_maximi − next) indices starting at `next`; consume in index
  order with today's rules (INUTILE skipped, point 0 skipped, repeated
  y checked and skipped, ERRATUM / punctum or valor ≥ p / `_g_ex_valore`
  failure → FALSUM at that index, counted as today: at l.322, before the
  checks). Repeat while collecta < necessaria and next < conatus_maximi.
  Batch buffers are allocated once per prime at `necessaria` (L5).
  Top-ups are expected to be rare: none occurred in T3 (L3).
- **Counting.** `aestimationes` = indices consumed (incremented as l.322
  does). A batch that returns FALSUM (transport failure) counts nothing
  and refuses (L6).
- **Self-check:** a batch of 1 per draw, in `_probare_in_puncto`'s
  documented order (prime, then index, INUTILE/point-0 redraws up to
  LXIV), so the sors stream and TRACTUS are unchanged. Self-checks stay
  outside `aestimationes` (interpolatio.h l.250-252). **Certifying
  checks** go through `InterpolatioVerificator`, single point,
  unchanged. Checks cost ~1% of CPU and ~2% of wall with 4 workers
  (L13); they stay serial in v1.

### laqueus

- **Exported batch evaluator (M7).** `laqueus_modulus_temperley_
  fasciculus(mt, index_primus, numerus, p, puncta, valores, status)`
  evaluates a range with laqueus's own index → ω mapping (today
  `_aestimator_temperley`, +2). The worker calls this, so the mapping
  exists once.
- **Fingerprint (H2, M8).** `laqueus_modulus_temperley_signum(mt)`
  returns a 64-bit hash over fila, longitudo, the copied word, writhe,
  window (imus, summus) and limes. The tool adds its build id.
- **Hook.** `LaqueusOptionesVerificandi` gains
  `InterpolatioAestimatorFasciculi fasciculus_temperley`,
  `vacuum* contextus_fasciculi` and `i32 capacitas_fasciculi`;
  `laqueus_optiones_verificandi_ordinariae` sets them to NIHIL / 0
  (L12).
  - Route TEMPERLEY: the verified entry passes a laqueus wrapper as the
    main evaluator. The wrapper calls the caller's batch function, then
    **re-evaluates one index of the batch on its own module** and
    requires bitwise equality of status, punctum and valor; a mismatch
    → FALSUM (rule 2). The spot index is deterministic and independent
    of the sors stream: index_primus + (p mod numerus). One extra point
    per batch: ~1 per prime, ≤ 0.5% at 16 × 801.
  - Route VERTEX: FALSUM when the hook is set (never silently ignored).
  - Route NULLA (c ≤ 1, closed form): no evaluator is called at all; the
    hook is unused and the relatio is today's (L4).
- **Unchanged:** the AUTOMATICUS cost rule (CPU against CPU; L13 notes
  its wall share roughly doubles), the SUI checker (in-process
  single-point module, "sui:temperley", communia unchanged), the
  frozen-kernel EXTERNUS checker built by the tool.
- **octeti_maximi** documented as per process: it bounds the parent's
  modules (main + checker); each worker builds one module of the same
  size (L12).
- Scope v1: the verified entry only (Q1). Plain PROBATUM and promotion
  keep the single-point path.

### Tool (tools/uncinus_mensura.c)

- **Route per braid (M4).** `-opifices` applies only to braids whose
  route is TEMPERLEY; vertex-route and closed-form braids run serial as
  today. The tool prints `OPIFICES 0 | via vertex` for those, so "all
  45" in P1 means 32 braids with workers and 13 unchanged.
- **Worker mode, parsed strictly (M8, L7):** `-opus -fila N -verbum
  "g1,g2,..." -primus p -ab i -numerus k`, nothing else accepted
  (unknown flags, overflow, letters out of range → exit 1). It builds
  the module, prints `OPUS SIGNUM <hash> <build id> <p>`, then one line
  per index `OPUS index punctum valor status` (ERRATUM and INUTILE are
  data), then `OPUS FINIS k`; checks `ferror(stdout)`; exit 0 only if
  all were written.
- **Frozen binary (M8).** At start the parent copies its own executable
  (path resolved once, absolute, L14) to `build/opifices/<pid>/` and
  execs that copy for the whole run; a rebuild mid-run cannot reach
  later batches. The copy is removed at exit.
- **Parent batch evaluator** (capacity unbounded):
  - every batch goes to workers, including a self-check's batch of 1
    (start-up is 0.04 s at 16 × 801, L2); no in-process main values
    (H2: two implementations of the main evaluator are what made the
    silent failure possible);
  - [i, i+k) splits into min(W, k) contiguous slices, as equal as
    possible (T0: copies run within 0.5% of each other);
  - W processes start through `processus_incipere` (runtime argv,
    timeout below), polled with `processus_pulsare` plus a 20 ms
    `usleep`, as tools/fabrica_sutura.c:907-937 (M3);
  - each output must have: exit 0, `OPUS SIGNUM` equal to the parent's
    fingerprint and p, each index of its slice exactly once and nothing
    else, `punctum, valor < p`, `OPUS FINIS k`. Anything else: all other
    workers are stopped and reaped (`processus_abrumpere`), a diagnostic
    line names the worker, exit code and stderr tail, and the batch
    returns FALSUM (M3, L6, L7);
  - a piscina per batch, reset after each batch (M3).
- **Timeout (Q2, L8):** per worker, 10 × slice size × measured seconds
  per point, with seconds per point estimated from
  `laqueus_temperley_sumptus` and the measured ns per multiplication,
  floor 60 s.
- **Timing (M6):** the tool prints wall time (house clock) and the
  children's CPU next to the parent's `clock()`; T3's scripts read wall
  and total CPU, not the parent's CPU alone.
- **Flags:** `-opifices` (4) or `-opifices N`; absent = serial. With
  `-modus verificatum` on sections VIII and XI.
- **Output:** UNCINUS/RELATIO/TRACTUS unchanged (rule 1); after RELATIO
  `OPIFICES W | fasciculi B | opera O | murale s | cpu_filiorum s`.
- **Process group (L14):** workers share the parent's group: Ctrl-C and
  custos stop them; if the parent crashes, a worker finishes its slice
  and dies on SIGPIPE. Accepted, noted.

## T0 (2026-11-03)

### Concurrency experiment (registered and run before the draft)

Predictions, results and decision rule:
build/verificatio/concurrentia_praedictio.md; raw: concurrentia.out;
worklog lib/laqueus.worklog.md 2026-11-03. N = 1..8 simultaneous copies
of one-point TL evaluation at 12/14/16 × 801, CPU s/point per copy,
12 strands as the cache-resident control. Machine: Apple M2 MacBook Air
(Mac14,15), fanless, 4P + 4E cores, L2 16 MB (P) / 4 MB (E), 16 GB,
shared with other sessions (load 6-75 during the run).

| n | N=2 S / R | N=4 S / R | N=6 S / R | N=8 S / R |
| --- | --- | --- | --- | --- |
| 12 (control) | 1.353 / 1 | 1.600 / 1 | 1.732 / 1 | 1.714 / 1 |
| 14 | 1.351 / 0.998 | 1.866 / 1.167 | 1.869 / 1.079 | 2.039 / 1.190 |
| 16 | 1.314 / 0.971 | 1.755 / 1.097 | 1.899 / 1.097 | 2.030 / 1.184 |

(S = per-copy CPU s/point vs N = 1; R = S / S(control).)

- Bandwidth is not the limit: P-A (R(16,4) ≤ 1.15) and P-B (R(16,8)
  ≤ 1.30) hold. Column blocks stay deferred.
- P-C (R(14,4) ≤ 1.10) falls at 1.167, below the 1.20 that would have
  set a separate worker limit at 14 strands.
- The size-independent slowdown dominates (control 1.35× at N = 2,
  ~1.7× at N ≥ 6): E cores, all-core clocks on a fanless machine, other
  sessions; not separated.
- Throughput vs N = 1 (points per wall second): 16 strands 2.1× at 4,
  ~2.3-2.5× at 6-8; 12 strands 2.3× at 4, 3.5× at 8. Hence default
  W = 4 and the ~2× target.

### Answered by the design review's measurements

(scratchpad recensio_l3/, load 59-101.)
- Start-up: `laqueus_modulus_temperley_creare` 0.6 ms (12 × 801),
  2-4 ms (14), 12 ms (16); a whole small process at 16 strands 0.04 s
  wall; footprint 6.5 MB before the first evaluation. Per-batch spawning
  is fine; P4 is re-measured with the real binary in T3.
- Top-ups: none in T3 (12 × 801: 3,378 = 6 × 563; 14 × 801: 3,941 =
  7 × 563).
- `processus`: 7 children, each echoing a 2,319-byte argv intact and
  writing 600 lines (26.9 KB), all captured, exit 0. Output buffers grow
  in the piscina without limit; timeout = SIGKILL → PROCESSUS_ERROR_
  TEMPUS with partial output kept; `processus_abrumpere` kills and
  reaps; ARG_MAX 1,048,576; maxprocperuid 2,666.

No threshold moved.

## T1 notes (2026-11-03)

- Built as designed. `InterpolatioAestimatorFasciculi` and
  `interpolatio_polynomium_relatum_fasciculis(fasciculus, contextus,
  capacitas, ...)` in interpolatio.h; internally one `FonsAestimationis`
  {single evaluator | batch, context, capacity}. Every main-evaluator
  call (PROBATUM primes, verified stages, the limit, promotion, the
  self-check) goes through `_primum_aestimare` / `_probare_in_puncto`
  with a source; the single-point API and the certifier are sources of
  capacity 1. Before each request the library zeroes puncta and valores
  and presets status ERRATUM (an evaluator that writes nothing refuses).
- `relatio->aestimationes` is documented as indices consumed (equal to
  calls for a single evaluator).
- probatio_interpolatio: 5 new sections (38 in all), 20,074 checks
  (was 17,304): `fasciculi aequales` (24 random polynomials × 6 pure
  fault kinds × PROBATUM/VERIFICATUM × capacity 1, 3, unbounded, reverse
  internal fill), `capacitas`, `fasciculi numerati` (request series
  simulated independently for INUTILE-every-third; the conatus cap),
  `erratum in fasciculo`, `defectus fasciculi` (incl. a failing
  self-check batch of 1). probatio_laqueus_rete 33/33, 152,937 checks,
  unchanged. Tool rebuilt.
- Plants: build/verificatio/plantae_l3_t1.py, 8/8 red as registered
  (plantae_l3_t1.out, rerun of A and H in plantae_l3_t1_b.out).
  - A (consume in reverse) first went red in `capacitas` and `erratum in
    fasciculo` but NOT in `fasciculi aequales`: when no index refuses,
    consumption order cannot matter (all indices are consumed and
    Lagrange does not depend on order). The batch = serial test lacked
    an index-pure refusal; added (ERRATUM at index 2 in every prime,
    `_vitia_pura` case 5). A's registration was widened to the three
    sections it reaches and rerun: red in all three. Recorded as a
    post-hoc amendment.
  - H first failed to plant (silva.planta's format guard: two-line
    anchor, one-line replacement); rewritten on two lines, red.
  - F (repeated-y check skipped) has no batch-specific red by
    construction: one internal loop, so serial and batch break alike;
    caught by `puncta iterata` and `puncta opposita` as registered.
  - B (adapter unbounded) also reddened `puncta opposita` (bonus).

## T2 notes (2026-11-03)

- Built as designed: `laqueus_modulus_temperley_fasciculus`,
  `laqueus_modulus_temperley_signum`, the three hook members (defaults
  NIHIL / 0), the spot-checking wrapper, vertex route → FALSUM, NULLA
  untouched; worker mode, frozen copy, parent batch evaluator (pulsare +
  20 ms), timeout, `OPIFICES` lines, `-opifices [N]`.
- Additions and small deviations:
  - `laqueus_temperley_signum(verbum, ...)`: the same fingerprint without
    a module (word → PD → window), so the parent does not build a third
    module just to compare (P5). A test checks both agree.
  - Build id = the LC_UUID of the *running* image, read from memory
    (`_dyld_get_image_header`): parent code vs worker code, no file read;
    the byte copy keeps the UUID. Printed at start: `OPIFICES W |
    binarium ... | aedificatio <uuid>`.
  - `laqueus_modulus_temperley_fasciculus` takes a typed module, not
    `vacuum*`; callers adapt in one line. It zeroes punctum/valor where
    not UTILE, so worker output is total.
  - The spot check compares status always, punctum/valor only where
    UTILE (the library reads nothing else).
  - The hooked call also requires `electa == TEMPERLEY` itself
    (defensive: a vertex module never reaches the TL spot check).
  - The parent also checks coverage: indices read back from accepted
    workers = the batch (`causa tectura`), not only per-worker
    completeness; a slice never started is not left to the library's
    ERRATUM preset.
  - Fault injection for T3's refusal cases, worker mode only:
    `UNCINUS_OPUS_VITIUM="<genus>@<ab>"`, kinds occide, exitus, statim,
    finis, signum, primus, duplex, extra. Without the variable: nothing.
  - The worker computes its slice, then prints (so `occide` fires after
    the work). `OPIFICES` also prints `cpu_parentis`.
- Smoke test found a bug before any plant: the strict decimal parser
  underflowed (`i64` is unsigned: `maximum - d` wrapped), so `-3` passed
  as a letter on 3 strands. Guarded with `d > maximum` first.
- probatio_laqueus_rete: 4 new sections (37 in all), 153,865 checks:
  `fasciculus temperley`, `fasciculi in via` (3 certain TL brackets ×
  AUTOMATICUS/VIA_ALTERA × capacity unbounded/3: exitus and relatio
  identical; indices requested = aestimationes + probationes_sui),
  `fasciculus extra viam`, `fasciculus divergens`.
  - Neither certain bracket has k_max = 1; the figure-eight
    σ1 σ2⁻¹ σ1 σ2⁻¹ (3 strands) does and is now the third case.
- Acceptance (small; T3 does the full P1): build/verificatio/
  acceptatio_l3_t2.py - 8 × 201 TL at W = 1, 2, 3, 4, 7 identical to
  serial; 12 × 101 vertex identical with `OPIFICES 0 | via vertex`; the 8
  injected faults refuse with the expected `causa`; `statim@0` at
  12 × 801 leaves no worker and no copy behind.
- Plants: build/verificatio/plantae_l3_t2.py, 8/8 red as registered
  (plantae_l3_t2.out; L1 rerun in plantae_l3_t2_b.out).
  - L1 (spot check removed) first stayed GREEN: the sign-flipped worker
    gives values that look random in the true window, so the coefficient
    bound refuses anyway. That is not H2's case. Added a worker that
    returns a *valid* wrong polynomial (bracket + A^e, e inside the window
    and bound): with the spot check removed the k_max = 1 figure-eight
    returns VERUM with the wrong bracket (H2 reproduced); k_max > 1
    braids are still refused by the certificate (DISCORDIA after 4
    batches). With the spot check, refused at the first batch. Recorded
    as a post-hoc amendment of the divergent-evaluator test.
  - T1 and T3 also reddened `refutationes` (bonus: a lying worker makes
    every refusal case refuse for the wrong reason first).

## Registered predictions

- **P1 (exact).** Byte-identical UNCINUS, RELATIO and TRACTUS (fixed
  seed, `-tractus`) against serial: the 32 TL-route D136 brackets at
  W = 1, 2, 3, 4, 7; the 13 others unchanged with `-opifices` given;
  12 × 801 at W = 4 and 7; VERIFICATUM with each checker that applies
  (L11: ~3 h, run detached).
- **P2 (exact).** On success, indices requested = consumed = serial
  `aestimationes`; self-check batches = probationes_sui draws; spot
  checks = batches. Checked by a counting batch evaluator in
  probatio_interpolatio and by the tool's counters (M5).
- **P3 (time).** 12 × 801 VERIFICATUM, frozen-kernel checker, W = 4
  against serial: wall-time ratio ≥ **1.6×**, median of 5 ABBA pairs,
  load recorded, not gated. Expected ~2.2× (review L10); the gate sits
  below T0's pass-to-pass spread (1.68-3.23) so noise alone does not
  decide it. 14 × 801: once, descriptive.
- **P4 (start-up).** Worker start-up (exec to `OPUS SIGNUM`, real
  binary) ≤ 2% of a full batch's per-worker time at 12, 14 and
  16 × 801.
- **P5 (memory, descriptive) (L9).** Peak footprint under
  tools/custos.sh at 14 × 801 reported against (W + parent modules) ×
  octeti, parent modules counted exactly (main/SUI module, frozen
  kernel when used). The 16 × 801 run reports it too.

## Tests

- All existing suites green and unchanged (rule 8), call counts
  included.
- probatio_interpolatio (new sections):
  - **batch = serial** (M1): every existing case whose evaluator is pure
    in (p, index) (faults, if any, injected by index, not by call
    count), PROBATUM and VERIFICATUM, through `_relatum_fasciculis`
    with a batch evaluator that fills each batch in reverse internal
    order: identical exitus and relatio;
  - **capacity**: capacity 1, 3 and unbounded give the same result;
    capacity 1 makes exactly the single-point call sequence;
  - **counting batches**: requested = consumed = serial count on
    success; top-ups happen (INUTILE every third index; a passus with
    repeated y) and each asks exactly the deficit; caps at
    conatus_maximi (the `inutile_semper` shape);
  - **mid-batch ERRATUM** (by index): FALSUM at that index, exitus
    untouched, relatio count = serial's;
  - **batch FALSUM** (transport): FALSUM, nothing counted, exitus
    untouched.
- probatio_laqueus_rete:
  - verified entry with an in-process `fasciculus_temperley` over a
    second module = without it, byte for byte, on the certified brackets
    that take the TL route;
  - hook set on a vertex-route braid → FALSUM; on a closed-form braid →
    today's result;
  - **divergent batch evaluator** (a module built from the word with one
    letter's sign flipped): FALSUM from the spot check, never a
    PROBATUM; and for a braid with k_max = 1;
  - `laqueus_modulus_temperley_fasciculus` = the single-point evaluator
    index by index; fingerprint changes when one letter changes.
- Tool acceptance script (build/, T3): P1, plus refusal cases (worker
  killed, exit 1, missing `OPUS FINIS`, wrong `OPUS SIGNUM`, duplicate
  index line, index outside the slice, wrong p echoed) → FALSUM lines
  with diagnostics, other workers reaped (no stray processes after).
- Plants (named reds, in a script under build/):
  - batch results consumed in reverse → batch = serial red;
  - adapter capacity unbounded → existing call-count tests red
    (`_probare_puncta_vitiosa`);
  - top-up requests `necessaria` instead of the deficit → counting red;
  - top-up not capped at conatus_maximi → `inutile_semper` red;
  - ERRATUM inside a batch skipped like INUTILE → mid-batch red;
  - repeated-y check skipped in the batch path → batch = serial red
    (passus case);
  - spot check removed from the laqueus wrapper → divergent-evaluator
    red (the run returns PROBATUM with the wrong bracket);
  - laqueus ignores the hook on the vertex route → vertex-route red;
  - tool: worker shifts labels (index i's value printed as i+1) →
    spot check / completeness refusal → acceptance red;
  - tool: worker evaluates at p' ≠ p but echoes p → spot check refusal
    → acceptance red;
  - tool: worker flips one letter's sign after parsing → fingerprint
    refusal → acceptance red;
  - tool: one slice never started → acceptance red;
  - tool: exit code not checked, worker exits 1 after full output →
    refusal-case red;
  - tool: failing batch does not stop the other workers → stray-process
    check red.

## Tasks

- **T0. Ground.** Done (concurrency experiment; review measurements).
- **T1. Interpolation.** Batch type, capacity, `_relatum_fasciculis`,
  one internal loop with the capacity-1 adapter, tests and plants.
- **T2. laqueus and tool.** Exported batch evaluator, fingerprint, hook
  with spot check, route rules; worker mode, frozen binary, parent
  batch evaluator, timeout, timing lines, `-opifices`; laqueus tests.
- **T3. Acceptance and measurement.** P1 (detached), P2, P3 ABBA, P4,
  P5; refusal cases; 14 × 801 once.
- **T4. Review.** Independent reviewer, not building in the live tree;
  re-checks rules 2-4 and the refusal paths with its own plants.
- **After the plan:** 16 × 801, VERIFICATUM, W = 4, under custos (with
  `CUSTOS_MORA` raised above its 3,600 s default, L8) and
  `caffeinate -i`, as its own step on Fran's word.

## Open questions for Fran

- **Decided in chat (2026-11-03):** hook = a member of the laqueus
  verified options (the library never spawns); workers live in
  uncinus_mensura; default W = 4, fixed, overridable.
- **Q1. Scope.** DECIDED 2026-11-03 (as recommended). v1 covers the verified entry only. Plain PROBATUM and
  promotion stay single-point until something needs them (a proved
  16 × 801 would need ~3× the primes: ~15-18 h even with workers).
  Recommend: as drafted.
- **Q2. Worker timeout.** DECIDED 2026-11-03 (as recommended). Recommend the bound in "Tool" (10× the
  estimate, floor 60 s), so a hung worker becomes a refusal, not a
  stuck run.
- **Q3. Small batches in-process.** DECIDED 2026-11-03: dropped, as in
  v2. CHANGED by the review: v1 drafted
  them; v2 drops them (H2). Every main value comes from workers; the
  in-process module only spot-checks and serves the SUI checker.
  Recommend: as in v2.

## AUDIENDA

- Real worker start-up through uncinus_mensura: not measured (P4, T3).
- `UNCINUS_OPUS_VITIUM` ships in the tool binary (worker mode only,
  inert without the variable).
- An interrupted run (Ctrl-C) leaves build/opifices/<pid>/ behind
  (atexit does not run); harmless, delete by hand.
- No wall-time speedup of the real design measured yet (P3).
- Frozen kernel memory at 16 × 801: not measured.
- macOS first-exec scan of the copied binary (syspolicyd): may add a
  one-time delay per new copy path; not checked.
- Only probatio_interpolatio was checked for refusal-path call counts;
  other suites rely on the capacity-1 adapter being exact.
- T0's experiment and the review's timings ran under heavy foreign load;
  their ratios are this machine on that afternoon, not constants.
- Column blocks (temperley-celeritas review L3) remain deferred; T0
  bounds their gain under concurrency at ~10-18%.

## Design review (2026-11-03, consiliarius-parallela)

Verdict: rework H1 and H2, then approve after fixes. Report: session
scratchpad recensio_l3/report.md (with creare_tempus.c and
processus_proba.c). Verified: rule 3 on the success path and the
consumption count on every refusal kind; self-check and promotion paths;
`laqueus_modulus_temperley_aestimare` pure in value (its only state: a
per-p cache deterministic in p, counters, fully reinitialised scratch);
hook placement; processus capabilities (measured); examen scope; custos
tree handling; creare cost; top-ups zero in T3; T0 figures against the
raw data.

Findings and where they went:
- **H1** eager adapter calls past a failing index; existing call-count
  tests red → capacity per evaluator source, adapter capacity 1; rule 3
  restated ("Interpolation", rules 3 and 8, capacity test and plant).
- **H2** systematic worker divergence ends as a wrong PROBATUM
  AD_LIMITEM (in-process self-checks reject every stage, certifiers never
  run; k_max = 1 compares nothing) → no in-process main values (Q3
  changed), fingerprint in `OPUS SIGNUM`, laqueus spot check per batch,
  rules 2 and 6 rewritten, divergent-evaluator test, spot-check plant.
- **M1** reverse-order test cannot pass with call-count faults → only
  index-pure cases, faults by index.
- **M2** off-by-one "lying worker" produces no wrong values → plants that
  lie: shifted labels, wrong p, sign flip.
- **M3** polling, cleanup, memory → pulsare + 20 ms usleep
  (fabrica_sutura pattern), abrumpere others on failure, piscina per
  batch.
- **M4** 13 of 45 braids are vertex-route → route per braid; P1 split
  32 + 13.
- **M5** P2 and rule 3 wrong on refusal and silent on self-checks →
  restated; self-check batches and spot checks counted.
- **M6** `clock()` is the parent's CPU only → wall and children's CPU
  printed; T3 reads them.
- **M7** index → ω mapping only in laqueus's internal evaluator →
  `laqueus_modulus_temperley_fasciculus` exported.
- **M8** rebuild mid-run, lax flag parsing → frozen copy of the binary,
  build id in the fingerprint, strict worker-mode parsing.
- **L1** examen is the wrong reason for rule 5 → reason restated.
- **L2** start-up measured (0.6 / 2-4 / 12 ms) → T0; P4 kept for the real
  binary.
- **L3** top-ups zero; necessaria 563 → Purpose, T0.
- **L4** route NULLA → hook unused, today's relatio.
- **L5** cap at conatus_maximi − next; buffers once per prime →
  "Interpolation", plant.
- **L6** transport failure counting, diagnostics, retry → counts
  nothing, diagnostic line, no retry in v1 (rule 4: a retry would hide
  flakiness the run should report).
- **L7** strict parsing → worker echoes p, duplicates/overflow refused,
  ERRATUM is data, ferror checked.
- **L8** timeout basis, CUSTOS_MORA → timeout from sumptus (Q2);
  CUSTOS_MORA raised for 16 × 801.
- **L9** P5 cannot resolve one module → descriptive, parent modules
  counted.
- **L10** 1.8× inside T0's spread → gate 1.6× with 5 pairs, expectation
  ~2.2× stated.
- **L11** P1 cost → fixed seed, `-tractus`, 12 × 801 at two W, detached.
- **L12** argument order, ordinariae defaults, octeti doc → aligned,
  defaults NIHIL / 0, octeti per process.
- **L13** checks' wall share → ~2% stated; cost rule unchanged.
- **L14** orphans, argv[0] → noted; executable path resolved once.
