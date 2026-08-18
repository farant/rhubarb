# Silva Performance Work — Implementation Handoff

**What this is:** a prose view of *De Perficiendo* (siglum **RP**,
`rhubarb-performance-sententiae.md`), rendered for an implementation
agent. The sententiae are canonical; where this document and RP
disagree, RP governs. Citations of the form (RP N) point into it.
RP is currently DRAFT — addresses not yet frozen — so confirm article
numbers against the committed version before citing them in commit
messages or the phase-log.

**Basis:** all figures were measured 2026-07-29 (AD) against commit
`162b3a9`, on Linux x86-64 with `gcc -O2 -std=gnu89`, driving the
standalone amalgam through the public API (`silva_c89_parsare`,
`silva_lexare`). The project's real toolchain is macOS/clang; LP64
layouts should agree, but **every `sizeof` in this document must be
re-confirmed on the real toolchain before anything is committed**
(RP 1.3). More generally, the first step of the work plan exists so
that every number below is replaced by the project's own current
measurement before the repairs land (RP 11.5).

---

## 1. System under analysis

Silva is the C89 parser and source-fidelity analysis substrate in the
rhubarb monorepo. Scope of this work is the `silva/` parse path and its
Tier 0 dependencies — `piscina` (arena allocator, single-shot
destruction, mark/reset available) and `xar` (segmented, pointer-stable
growable array). Officina and the apps are out of scope (RP Status).

Because the repairs land in Tier 0 (`lib/piscina.c`, `include/xar.h`,
`lib/xar.c`, and the mirrored constant in the hand-written region of
`amalgama/silva.h`), the **root suite is the gate, not silva's**
(RP 6.5) — which is a benefit, since every project in the monorepo
inherits the reduction. The silva.h mirror means one of the changes is
an `amalgamare.sh` regeneration, not merely a `fontes/` edit, and a
compile-time assertion guards the two copies' agreement — when it
fires, that is the drift machinery working as designed; treat it as a
checklist item, not an obstacle (RP 7.2).

## 2. Method (RP 1)

Memory figures come from two arena counters: **handed-out bytes** (sum
of block offsets) and **committed bytes** (sum of block capacities);
their difference is **idle** memory. Timing is the mean of 20 runs per
file, arena construction and destruction excluded from the timed
region. The corpus is `lib/*.c` from the repo: three named files for
detail (`piscina.c`, `json.c`, `stml.c`) and the 25 smallest for
aggregates. Every repair claimed as behaviour-preserving was applied
to a patched amalgam and differentially compared against baseline
across the 25 files — token counts, error counts, success flags —
and found byte-identical (RP 1.2).

## 3. Diagnosis

### Baseline (RP 2)

| specimen | source | committed | handed out |
|---|---|---|---|
| lib/piscina.c | 13,276 B | 12.00 MB | 9.97 MB |
| lib/json.c | 42,286 B | 39.01 MB | 33.94 MB |
| lib/stml.c | 112,364 B | 120.01 MB | 72.67 MB |

Committed memory runs roughly **950–1070× source size**. Throughput is
1.3–2.2 MB/s; `stml.c` parses in **85.29 ms**. Lexing is 28–32% of
parse time. The parse path is **allocator-bound, not scan-bound**
(RP 2.4): the four constant-level repairs below, which touch no
parsing logic whatever, cut stml.c from 85.29 ms to 49.29 ms — 1.73×.
Consequence (RP 2.4.2): any repair that reduces allocation count is
also a speed repair, and should be evaluated on both axes.

### Where the memory goes (RP 3–4)

**Not into tokens.** For stml.c the entire token array, trivia
included, is 3.28 MB of 72.67 MB handed out — 4.5% (RP 3.1).

**Into Xar instances.** stml.c creates **62,596 Xars to hold 16,390
tokens** — roughly four growable arrays per token (RP 3.2).
`sizeof(Xar)` is 576 bytes (`segmenta[64]` = 512, debug `titulus[32]`
= 32); `XAR_PRIMUS_SEGMENTUM` is 16, so a pointer Xar's first segment
is 128 bytes; a one-element pointer Xar therefore costs **704 bytes to
store 8 bytes of payload — 88× overhead**. 62,596 × 704 = 44.06 MB, or
**61% of stml.c's handed-out bytes** (RP 3.2.4).

**And into idle committed memory.** The arena's oversized-request path
permanently raises the base block size and never lowers it: in
`piscina_allocare`, `capacitas_nova = necessaria +
mensura_alvei_initia` followed by `mensura_alvei_initia =
capacitas_nova` — the second assignment is the ratchet (RP 4.1.1).
Observed block sequence for stml.c: 1, 2, 3, 6, 9, 18, 27, 54 MB; the
base ratcheted 1 MB → 27 MB, and the final 54 MB block holds 6.67 MB —
**47.34 MB, 39% of total footprint, committed and idle** (RP 4.1.3).

The arena already ships instrumentation that would have surfaced this
— `piscina_summa_inutilis_allocatus`, covered by `probatio_piscina.c`
— but nothing calls it; it sits in the harvested unused-function list
`excludenda_generata.h`. "Instrumentation that is built, tested, and
never called is equivalent to no instrumentation" (RP 4.2.2).

## 4. The four Tier 0 repairs (RP 5–10)

Acceptance rules for this tier (RP 5): a repair must not change what
any silva API returns, **demonstrated by differential run, not by
argument**, and must be small enough to review in one sitting. All
four below are measured, not projected. Effects are cumulative in the
order given.

**Repair 1 — remove the arena block-size ratchet (RP 6).** Delete the
assignment `mensura_alvei_initia = capacitas_nova` in the
oversized-request branch of `piscina_allocare`; oversized requests get
an exact-fit block and the base is left alone. One line, in
`lib/piscina.c`. Measured: stml.c 120.01 → 78.03 MB committed (−35%),
idle 47.34 → 5.36 MB. Handed-out bytes unchanged **to the byte**
(124.41 MB corpus aggregate, before and after) — the cleanest proof
that only allocator policy moved.

**Repair 2 — shrink Xar's segment table (RP 7).** Reduce
`XAR_MAXIMUS_SEGMENTORUM` from 64 to 24 (or 32 — see the caveat).
Touches the constant, its twin in the hand-written region of
`silva.h`, and the compile-time assertion that guards their agreement;
this is an `amalgamare.sh` regeneration. Measured, cumulative with
repair 1: corpus 176.02 → 100.00 MB committed; handed out 124.41 →
63.86 MB — a **48.7% handed-out reduction from one constant**. It is
also a speed repair: `xar_creare` zeroes `segmenta[]` in a loop, so 64
pointer writes per Xar became 24, 62,596 times per file.

> **Caveat (RP 7.obj.1, conceded):** this one is *not* strictly
> lossless — it lowers Xar's maximum capacity to ~134M elements at
> primum 16 (~33.5M at primum 4). The claim of losslessness is
> withdrawn for this item specifically. Implement with **a named limit
> and an explicit refusal rather than silent truncation**. If the
> bound is unwelcome, 32 segments still saves 256 bytes per Xar and
> puts the ceiling back in the billions — "take 32 if the argument is
> not worth having" (RP 7.obj.1.2).

**Repair 3 — delete Xar's debug title field (RP 8).** Remove
`titulus[32]` and the `strcpy` of the literal `"Xar Anonymus"` in
`xar_creare`. Nothing reads the field. Two lines. If it is wanted for
debugging, keep it under a `PISCINA_DEBUG` guard rather than in
release builds. Measured (cumulative with repairs 1, 2, 4): corpus
95.00 → 87.00 MB committed; handed out 59.83 → 53.21 MB.

**Repair 4 — reduce the first-segment size (RP 9).** Reduce
`XAR_PRIMUS_SEGMENTUM` from 16 to 4. Justified by the size
distribution: of 11,010 trivia Xars in stml.c, 10,395 hold exactly one
element and 352 hold two. Measured, cumulative with 1 and 2: corpus
100.00 → 95.00 MB committed; handed out 63.86 → 59.83 MB. Smallest of
the four returns — **take it last, and drop it without regret** if it
interacts badly with the RP 7 capacity bound (RP 9.4).

**Combined result (RP 10).** Corpus: 184.01 → 87.00 MB committed
(−52.7%); handed out 124.41 → 53.21 MB (−57.2%). stml.c: 120.01 →
36.00 MB committed (−70%); handed out 72.67 → 31.52 MB; parse time
85.29 → 49.29 ms (1.73×). Parse output byte-identical across 25 files
at every intermediate step. Total edit surface: **four constants and
two deletions**, all in `lib/piscina.c`, `include/xar.h`, `lib/xar.c`,
and the silva.h mirror. No silva logic touched.

## 5. Scoping repairs (RP 11–13)

**Measurement harness — prerequisite (RP 11).** Nothing in the repo
reports arena statistics, so no performance regression can currently
be detected. Add a tool under `instrumenta/principalia` (alongside the
existing read-bench instruments) that parses a file and reports
handed-out, committed, and idle bytes, block count, token count, and
Xar count. Pin a small corpus with expected figures in
`probationes/fixa` so a regression **fails a probatio** rather than
going unnoticed. Effort is hours, since
`piscina_summa_inutilis_allocatus` already exists and is tested. **Do
this before the Part II repairs land**, so their numbers are the
project's own and not this document's (RP 11.5).

**Phase-scoped arenas (RP 12).** Silva never creates an arena — it
takes the caller's and puts everything in it — so lexer output, the
GLR graph-structured stack and forest, the committed tree, symbol
tables, CFG, and dataflow events are all live simultaneously until the
caller destroys the arena. The GLR working set is dead the instant the
commit pass finishes and is never reclaimed. After the Part II
repairs, stml.c uses 11.87 MB at end of lex and 31.52 MB at end of
parse — roughly 20 MB added by parsing, much of it transient. Piscina
already provides `PiscinaNotatio` (mark/reset); silva uses it nowhere.
Cheapest form: mark before the GLR run, reset after commit — no data
structure changes. Fuller form: permanent + scratch arenas, scratch
reset per phase.

> **Mandatory pre-check (RP 12.obj.1):** the commit pass may alias
> forest storage rather than copy from it, in which case the reset
> would free live data. **This must be checked in the code before the
> repair, not assumed either way.** If `commissio` copies, the reset
> is safe as written; if it aliases, the repair becomes "make
> commissio copy" — larger but still bounded. SC's description of the
> commit pass suggests copying, "but the code, not the doc, decides."

**Const-ify the grammar and code tables (RP 13).** The amalgam has
zero mutable BSS globals, and 15 initialized-data globals
(`SILVA_C89_TABULA`, `SILVA_C89_GRAMMATICA`, `SILVA_C89_REGISTRUM`,
`_codices`, `_tolerabiles`, `_verba_aliena`, and others) that appear
read-only in fact but are not declared `constans`. Declaring them
`constans` is a **one-build experiment: if it compiles, silva is
reentrant and the compiler has proved it.** Reentrancy is the
precondition for all parallelism — do it early even if parallelism is
deferred (RP 13.4).

## 6. Representation items inside silva (RP 14–17)

**Token field reorder (RP 14) — in the plan.** `sizeof(SilvaToken)` is
120 bytes, 15 of them padding (4 after `genus`, 4 inside `chorda`, 3
after `standard`, 4 after `initium_lineae`). Grouping pointers and
`origo` first, then the 4-byte scalars, then `standard`, yields **112
bytes with no semantic change of any kind** — the only Part IV item
with no API consequence. Take it whenever convenient.

**Trivia as exact-sized arrays (RP 15) — deferred.** Trivia lists
(`spatia_ante`/`spatia_post`, each an `Xar*`) are built once at attach
time and never appended to, so the growable array buys nothing; an
inline pointer+count pair allocated exact-size would do. But note the
value **collapsed after Part II**: before, this was ~10% of the arena;
after, roughly 3 MB of 87 on the corpus. "The overhead was in the Xar
header, not in the choice of a growable array — this is the general
lesson of Part II" (RP 15.5).

**Origo as pointer (RP 16) — disputed, deferred.** `SilvaOrigo` is 40
bytes inline in every token, its union unused for source-origin
tokens; measured across 56,686 tokens, 100% are `SILVA_ORIGO_FONS`
with no lexicon loaded, 89.6% with latina loaded and macros genuinely
expanding. Changing to `SilvaOrigo*` (NIHIL meaning FONS) saves 32
bytes on ~nine tokens in ten, and combined with RP 14, RP 15, and the
removal of `scissurae`, the token reaches **72 bytes** (verified by
compiling candidate layouts) without touching `linea`/`columna`. The
standing objection (RP 16.obj.1, **no answer on record**): the gain is
under 2% of post-Part-II footprint while the repair touches all five
constructors and every consumer reading `t->origo.genus`. Defer until
Part II has landed and been re-measured; it may never be worth its
churn.

**Redundant token fields (RP 17) — only with RP 16.** `longitudo`
equals `valor.mensura` in 56,686 of 56,686 tokens; `scissurae` was
non-NULL in 0 of 56,686 (its own comment concedes "NIHIL fere
semper"). Removing `longitudo` alone gains nothing — trailing padding
absorbs it. Take these only as part of RP 16; if `scissurae` is kept,
it belongs in a side table keyed by token index.

## 7. Parallelism (RP 18–21)

The division is not parser versus lexer; it is **across-file versus
within-file** (RP 18.1).

**Fundamentally linear (RP 19)** — and not defects to engineer around:
the preprocessor (macro table at line N depends on every prior line),
the GLR parse of one translation unit (left-to-right by construction),
and name resolution (ISO declaration-point semantics, order-dependent
on purpose — the oracle argument rests on it). Parallelizing these
would forfeit correctness properties the project deliberately bought.

**Embarrassingly parallel (RP 20) — the largest wall-clock win.**
Whole files: translation units share nothing. Covers `examen.sh` over
the corpus, `examen_vectis.sh`, the differential sweeps, and the root
suite (currently ~137 s per SPL). The arena design already suits it:
the piscina is passed in by the caller, so arena-per-thread is the
natural shape with **zero allocator contention** — which at 62,596
allocations per file would otherwise dominate. Precondition is RP 13;
given that, this is a **harness change, not a silva change**.

**The middle case (RP 21) — per-function analysis, deferred.** Once
semantica has built the symbol table, CFG construction, dataflow, and
most of examen are per-function-definition and independent
[conjecture]. This is the **only** form of parallelism that helps LSP
latency, and it is the fastest-growing share of runtime (cf. SPL
entries from 2026-07-27). Shape: fork-join over function definitions,
arena per worker, diagnostics merged at join. Defer until RP 12 has
established phase boundaries — the two repairs want the same seam.

## 8. What not to do (RP 22–24)

**SIMD (RP 22).** Not the next lever, and never to the parser proper —
LR/GLR are data-dependent state machines and nobody has made
vectorized LR pay. Vectorization applies only to stage-one scanning
(the simdjson model). The ceiling is the lex share (~30%): if lexing
became *free*, stml.c goes 49.29 → 33.67 ms; a realistic 2–3× scanner
buys 10–15% overall, against the 73% already collected by four
constants. Intrinsics are not C89 and `-pedantic -Werror` rejects
them; the honest option is **SWAR over `unsigned long`** (portable
C89, worth 2–4× on tight scan loops). If ever done, the order is:
line-table construction first, then trivia skipping, then identifier
runs. C phase-2 line splicing makes the byte stream unclean, requiring
a speculate-then-repair scanner — but the measured `scissurae` count
of 0 in 56,686 tokens says the repair path essentially never fires,
the best case for that shape.

**Line/column from byte offset (RP 23).** Not yet, and not as part of
the memory work. Every other Part IV item is free (same information,
fewer bytes); this one trades a field read for a binary search against
a line table — plausibly the wrong trade for an LSP doing constant
position lookups, and not needed to reach the 72-byte token.

**Structure-of-arrays rebuild (RP 24).** Not now; out of scope by
construction. The case for it is real and recorded elsewhere (the
analysis session estimated a columnar, index-based representation with
phase-scoped arenas in the 20–50× source-size range, with the
traversal-heavy examen passes gaining most); the case against is that
Parts II–III cost days and deliver a large fraction of the available
return. **Reopen rule (RP 24.3):** revisit only after RP 6–13 have
landed and been re-measured against the RP 11 harness. If the result
is near **200× source size** and the LSP feels responsive, the
greenfield case is materially weaker.

## 9. Work plan (RP 25 — order is normative)

1. **RP 11 — the harness**, so every subsequent claim becomes the
   project's own measurement. Wire in
   `piscina_summa_inutilis_allocatus`; pin expected figures in
   `probationes/fixa`.
2. **RP 6, 7, 8, 9 — the four constants**, as one reviewable change
   with a root-suite gate. Order within the change: ratchet (6), then
   segments (7), then title (8), with first-segment (9) last and
   droppable (RP 9.4). Includes the silva.h mirror + `amalgamare.sh`
   regeneration and the RP 7 named-limit/explicit-refusal behaviour.
3. **RP 13 — const-ification**: a single build that, if green, proves
   reentrancy and unlocks parallelism.
4. **RP 12 — phase-scoped arenas**, *after* the commissio
   aliasing check (RP 12.obj.1), then **re-measure**.
5. **RP 20 — file-level parallelism in the harnesses**: largest
   wall-clock win, no C changes beyond step 3.
6. **RP 14 — token field reorder**, whenever convenient.

**Then stop and re-read the numbers** (RP 25.7) before considering
RP 15, 16, 21, or 24.

## 10. Verification protocol (every step)

1. **Differential run, not argument (RP 5.1).** Patched vs. baseline
   across the full 25-file corpus: token counts, error counts, success
   flags byte-identical; for allocator-only changes, handed-out bytes
   identical to the byte.
2. **Root suite (RP 6.5).** All Tier 0 changes gate on the monorepo
   root suite, not silva's alone.
3. **Amalgam discipline (RP 7.2).** silva.h-mirrored constants go
   through `amalgamare.sh`; the compile-time agreement assertion is
   the checklist, not an obstacle.
4. **Toolchain re-confirmation (RP 1.3).** Re-confirm every `sizeof`
   and layout figure on macOS/clang before commit; the measurements
   here are Linux/gcc.
5. **Harness before and after (RP 11, 26.1).** Record harness output
   around each step in the phase-log so RP 26.1's amendment trigger
   ("when the harness disagrees with any figure in Part I or II —
   amend by appending, never by renumbering") can actually fire.

## 11. Posture (RP 26)

The codebase is in exploratory mode and none of this is urgent — that
objection is pre-registered and conceded in RP itself (26.obj.1). The
value of the plan while exploration continues is that it records where
the costs are, so the decision to pay them is made deliberately rather
than by default. When it is made: the order above is the path, the
harness is the referee, and the escalation trigger is RP 26.2 — when
semantica's runtime share exceeds the parse share, per-function
parallelism (RP 21) becomes the leading item rather than a deferred
one.
