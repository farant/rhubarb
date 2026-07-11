# ACTA — recording/diffing side effects & memory as a first-class thing
## (vision seed — NOTHING DECISUS; names unsealed)

Born 2026-07-11, mid M4b on-ramp (Fran, during the REPL design
conversation): "what if we had a first class thing for recording /
diffing side effects and/or memory state of programs? ... useful for
things like fuzz testing at some point." This note exists so the idea
survives compaction with its consumer list attached. It is a seed in
the silva-instrumenta-visio tradition: named, not scheduled.

## Why the VM makes this uniquely cheap (the architectural gift)

1. **Completeness by construction.** Every side effect crosses the
   builtin bridge — there is no other door. Recording at that seam
   captures EVERYTHING (native-process tracing is always partial).
2. **Comparable memory.** All program memory lives in ONE owned regio
   at a FIXED base (0x300000000000), and allocation is deterministic:
   snapshots are memcpys of known extent, and two snapshots from two
   runs are directly comparable — pointer values included.
3. **Diff = signal.** The determinism pin means any divergence
   between two traces is attributable to exactly the changed input or
   code. Diffing becomes a measurement instrument, not forensics.
4. **A global clock.** The instruction counter aligns traces, so
   diffs can say WHERE histories diverge, not just that they do.

## The three primitives

- **Trace** (name candidates: acta, vestigium): ordered record of
  builtin crossings — builtin, args, result, payload bytes. The REPL
  overlay is a VIEW of this: the interceptor that records a write is
  the interceptor that redirects it; overlay = trace folded into
  "current file state." One seam, two behaviors.
- **Snapshot**: regio segments + machinula small state. Restore
  trivially correct (fixed base). NB name collision: vindex already
  uses -imago for headless render — snapshot needs a different word.
- **Diff**: over traces (first-divergence, instruction-count aligned)
  and over snapshots — with semantica/indicium type info, a TYPED
  memory diff ("libri[3].titulus changed", not "byte 0x…A0 changed")
  = the probatio-pillar's typed-heap snapshot testing, made concrete.

## The consumer list (the evidence it's a real seam)

1. **REPL (M4b, FIRST consumer)** — needs a slice of all three
   today: per-turn stdout capture (trace filter), overlay filesystem
   (trace fold + redirect), green-invariant / changed-history
   detection (trace diff across replays).
2. **Fuzz testing** — diff traces/memory across mutated inputs for
   behavioral divergence; "fuzz-to-time-travel" (probatio-pillar):
   a crash captured as trace+snapshot = reproducible vindex session.
3. **Golden traces** — GENERALIZES the M2 standing stdout bar:
   pinned native stdout is a special case of a pinned trace; same
   captare/bless workflow, vastly stronger net (every effect).
4. **Time-travel v2 flagship** — decomposes into exactly these
   parts: snapshot ring + trace between snapshots; reverse-step =
   restore nearest + replay forward. This library IS the substrate,
   arriving by pull instead of fiat.
5. **vindex** — "what changed since the breakpoint" command;
   watchpoints as snapshot diffs between steps.
6. **M5 native differential** — machinula-vs-native trace/final-state
   comparison = the haruspex/examen differential pattern extended
   from types/judgments to BEHAVIOR.
7. **Probatio pillar generally** — fault injection = trace
   mutation/replay; world-substitution testing rides the same seam.

## Scope discipline (the Eskil rule)

M4b builds the INTERCEPTOR SEAM and only the REPL's slice: capture,
overlay-redirect, replay-compare. Everything else is a named pull.
The one early INTERFACE decision worth making knowingly: is the trace
a FORMAT (serializable, committable, diffable offline — indicium
precedent, format-first discipline; golden traces in probationes/fixa
want this) or an in-memory structure only? Recommend: design the
record layout as if it will serialize; actually serializing is a
pull.

Interaction with existing plans: Wave-2 time/entropy exclusions are
the record-on-first-contact candidates (nondeterministic builtins
become deterministic under recording); the flight ring in machinula
is a mini-precedent (recent-history ring, persists across currere).

## Cross-references

officina-probatio-pillar.md (fifth pillar; this seed is its
substrate) · officina-spec-v2.md (v2 time-travel flagship; M2
standing stdout bar) · officina-m4b-fundamenta.md (REPL replay
model, overlay, per-turn capture — the pulling consumer) ·
silva-instrumenta-visio.md (the seed-note tradition).
