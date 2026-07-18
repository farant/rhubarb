# Silva fluxus gradus 1 — interview (raw Q&A)

Date: 2026-07-17 (evening). Context: after the calibration battery
(17 specimens, 4 rounds, Apple clang 16.0.0) and the simulatio
(pretend-build against shipped FLUXUS-0). Park with full ledgers:
01KXSETFAPHYG42S1K5QQC722R. Spec follows from these answers.

## Q1 — The short-circuit gap (&&/||)

Clang fires on uninit patterns involving short-circuit init
(`opaque() || (x = 1)` then use), but the statement-granular CFG has
no edges inside expressions — v1 would under-fire vs the oracle.
Options: accept gap + measure corpus first / expression-level CFG
edges now / may-def permanently.

**A: Expression-level CFG edges now.** (The maximalist option, against
the recommendation to measure first. The CFG grows real edges rather
than accepting a permanent or deferred parity hole.)

## Q2 — Retained tables home

Per-block event streams + may/must bitsets; stage-2 consumer = codex
62 re-arm (def-use chains), later solarium queries.

**A: Sidecar module + one pointer.** New
`silva_c89_fluxus_datorum.{h,c}`; FluxusFunctionis gains a single
`datorum` pointer (NIHIL until stage 1 runs). CFG module stays pure;
data one hop from the graph.

## Q3 — The residual class

The analysis computes clang's off-by-default
-Wconditional-uninitialized class for free (maybe-uses with no
blamable edge). lib/ measurement: 3 sites, all false-positive-shaped.

**A: Ship dormant now.** Codex number reserved, census-visible, never
fires — the DEGRADATA 62 pattern. Full-corpus count for free.

## Q4 — &-const qualifier awareness

Clang: `&x` passed to `const int*` does NOT count as initialization
(s04f). Needs call-site param-type lookup.

**A: Implement in v1.** Unknown callee / variadic / K&R = assume
non-const (silent-leaning).

## Q5 — Surgery shape

Three ways to get expression edges into the CFG.

**A: Split at short-circuit only.** Blocks stay runs-of-flow; only
statements containing &&/||/?: decompose (block ends mid-statement,
VERUS/FALSUS edges with origo = operator node, continuation block
follows). Unaffected statements identical; honest edges in THE graph.
NOT full clang-style linearization; NOT a shadow micro-CFG.

## Q6 — Ternary scope

**A: ?: included in the same surgery.** One granularity change, one
re-verification; `c ? (x=1) : (x=2)` must-join judges correctly.

## Q7 — Arc structure

**A: Surgery is chunk 0 of this arc.** Own commit + full stage-0 bars
re-run (vectis green, census meters 63=0/64=1/65=1 byte-stable)
before chunk A touches events. One arc, one INTENTIO.

## Q8 — Push latency policy

Stage 1 runs at every examen, including the 0.5s post-edit push.

**A: Always run, measure in-arc.** RELATIO reports measured push
latency + piscina delta over the corpus. Gate only if a wall
appears, with data. capsula_libri INFRA-under-load is the canary.

## Q9 — Codex numbers and names

**A: 71 LECTIO_ININITIATA (definite), 72 ININITIATA_QUANDOCUMQUE
(sometimes — the whenever-claim wears its semantics as its name),
73 ININITIATA_FORSITAN (dormant residual).**

## Q10 — Diagnostic anchoring

Clang anchors definite at the USE line, sometimes at the CONDITION
line (use as note); vectis compares per-line.

**A: Clang parity + socius.** 71 anchors at the use; 72 anchors at
the condition with socius = the use site (both positions carried).

## Q11 — TOLERA-bility

**A: Both 71 and 72 TOLERA-bilis.** Consistency with codex 64; the
hatch costs nothing while unused (IRRITUM keeps it honest).

## Q12 — Specimen graduation

**A: All 17 + the 2 missing pins.** Every calibration specimen
becomes a fixa/examinis fixture (silences are pins too); add
sizeof(x) and member-write-then-whole-use specimens found missing by
the simulatio.
