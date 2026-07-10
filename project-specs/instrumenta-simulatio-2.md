# Instrumenta — simulatio 2 (demand-side: M3 work with tools in hand)

*2026-07-10, nocte, post the M2 bar (80/80). Fran's proposed technique:
simulate implementing M3 (indicium + vindex) while PRETENDING the
instrumenta-prima tools exist, and record every moment a hand reaches
for one — the invocation typed, the output needed. Supply-side sim
(simulatio-1) asked "can we build them"; this asks "are they the right
tools." The episodes are drawn from real M3 shape (proto-indicium =
MedullaLineae, format-first vindex, macro-aware stepping via origo) and
real debugging patterns from today's session (the sputnik hunt as the
replay template).*

## Episode 1 — implementing the indicium line-table serializer

Work: design the flat format's line section; serializer + lector;
golden fixtures (the medulla-textual-form precedent).

Reaches:

- `emitte officina/fontes/officina_demissio.c demissio_lineas_colligere`
  — instead of grep -n + Read offset (which I did ~15 times today).
  WANT: the output prefixed with `via:linea` span so I can cite it
  without re-looking. → DESIGN: emitte prints a `[via:linea-linea]`
  header line.
- `nexus MedullaLineae` — who consumes the struct BEFORE I change its
  shape (machinula_lineas_praebere, cursor, fusor?). The killer use is
  blast-radius-before-refactor, not find-usage. → DESIGN: nexus output
  GROUPED BY FILE with per-file counts, definition site marked.
- latina lint — indicium.c is a fresh file; every fresh file this week
  fired a landmine eventually (cursor.c: nomen, #10). Runs at first
  compile. → DECISION INPUT: this argues for lint-in-suite-gates
  earlier than "after false-positive measurement" — or at least in the
  officina compile scripts, where fresh files are born.
- interpretare: NOT reached for in this episode. Serialization work is
  golden-driven. Honest: the read tier carries episode 1 alone.

## Episode 2 — debugging: vindex steps to the wrong line in a macro

The classic M3 bug-shape: stepping over a macro-expanded statement
reports the expansion-body line instead of the invocation line (or
vice versa). Today's sputnik pattern replayed with tools:

Reaches:

- `interpretare -plagulae fasti,filum,piscina fasti_dies_hodie` — run
  the target once, get result + instruction count, confirm behavior
  before touching the stepper. One call replaces today's
  scratch-file + fusor -textus + read loop (~10 min → ~10 sec warm).
- WISH SURFACED: `interpretare -halitus` — run to the crash and print
  the full flight-recorder halt report. Machinula already does this on
  halts; interpretare inherits it FREE. → DESIGN: halt report always
  prints on non-BENE exits (no flag needed; it's stderr).
- **GAP FOUND: `origo` CLI is missing from tier 1 and M3 wants it
  constantly.** "What is the expansion story of the token at
  fasti.c:111:5?" is THE question of macro-aware stepping — I would
  ask it dozens of times while building the feature, and today I have
  no tool shape for it at all (I read expander code instead). It's
  single-TU, same loader as emitte, prints the origo chain
  (invocatio → nomen_macro → corpus, per layer). → RECOMMENDATION:
  promote origo CLI into instrumenta prima as the 4th tool (+0.5 day,
  shares emitte's skeleton).
- NON-GAP confirmed: "dump the line table for $functio" is NOT an
  instrumenta tool — it's an M3 DELIVERABLE (the format's own textual
  form, per the format-first pin: indicium must have a canonical dump
  like medulla's, with goldens). The phase split is correct: generic
  read tier = instrumenta; format-specific dumps = M3 builds its own
  instruments, as medulla did.

## Episode 3 — verifying vindex's backtrace against the recorder

Work: golden test — vindex's format-derived backtrace must match
machinula's native halt report on the same crash.

Reaches:

- `interpretare` with a crash-arg → the halt-report baseline in one
  call (episode 2's design pays immediately).
- `nexus machinula_numerus_op` / recorder rows — verifying the public
  surface I'm allowed to lean on vs machinula internals. nexus over
  OFFICINA sources too, not just lib/ — → DESIGN: nexus sweep covers
  the whole repo (silva/officina/lib/knotapel), not lib/ only. Cost
  unchanged (percursus already walks everything).
- emitte on probatio_officina_machinula.c blocks — pulling the inline
  fixture patterns for reuse. emitte should work on any symbol
  including tests. (Free if single-TU loader is corpus-general.)

## Audit — what the demand side changes

1. **Tool set: +1.** Promote `origo` CLI into instrumenta prima
   (4th tool, ~0.5 d, shares emitte's single-TU skeleton). M3's core
   feature interrogates expansion provenance constantly; building
   vindex without it means reading expander internals instead of
   querying them.
2. **Design pins captured** (cheap now, annoying later):
   - emitte: `[via:linea-linea]` header on output.
   - nexus: group-by-file with counts; definition site marked; sweep
     covers the WHOLE repo.
   - interpretare: halt report always prints on non-BENE exit (free —
     machinula behavior inherited).
3. **Lint gate decision input**: fresh files are where landmines fire;
   the officina/silva compile scripts are where fresh files live. Run
   the lint in those gates from day one, on-demand for the full corpus
   until the false-positive rate is measured.
4. **Phase-split confirmed**: format-specific dumps (line-table textual
   form) are M3 deliverables, not instrumenta — format-first means M3
   births its own read instruments, exactly as medulla did.
5. **Usage-frequency prediction** (auditable after M3): emitte >>
   nexus > interpretare > origo-CLI in raw counts; origo highest in
   leverage-per-call during the macro-stepping work. If M3 ends and
   the counts are wildly different, the sim method needs recalibrating
   — written down so it's checkable.

## Revised instrumenta prima

| tool | estimate | note |
|---|---|---|
| custos latinae (lint) | 1 d | origo query; slot inventory generator-derived (sim-1 A2) |
| emitte (single-TU) | 0.5 d | + via:linea header |
| origo CLI | 0.5 d | NEW — shares emitte skeleton |
| nexus sweep + CLI | 1 d | whole-repo, group-by-file |
| interpretare v0 | 1-1.5 d | wrapper-gen; halt report free |
| **Total** | **~4-4.5 d** | cut line: lint + emitte + origo first |

Open questions for Fran: unchanged from simulatio-1 (names, lint gate
placement — episode 1 argues for gates-now, .tsv scaffolding, scalars
v0) + one new: approve origo CLI's promotion into the phase.

## DECISUS (Fran, 2026-07-10, nocte — all five sealed)

1. **Nomina**: the lint is **censor** (the magistrate who keeps the
   rolls and censures misconduct — and the name has room to grow into
   the general checker lane, censor = checker plugin #0). The rest as
   proposed: **emitte, nexus, origo, interpretare**.
2. **Lint gates**: censor wired into the officina/silva compile-script
   gates FROM DAY ONE (fresh files are where landmines fire);
   full-corpus sweeps on-demand until the false-positive rate is
   measured on one complete run, then promoted to standing.
3. **Cache**: nexus rides a DISPOSABLE build/nexus.tsv (never
   committed, mtime-staleness warning) — explicitly pre-acta
   scaffolding; the dogfooding usage feeds the acta schema ceremony.
4. **Origo CLI**: PROMOTED into instrumenta prima — five tools,
   ~4-4.5 days total.
5. **interpretare v0**: scalars + string literals; aggregates a named
   deferral (hex dump later if pulled).

Phase is fully specified. Sequence: M2 seal ceremony (full phase-log
re-read + audit, fresh session) → instrumenta prima (this doc + sim-1
as the spec) → M3.
