# officina — orientation

*You are reading this because you touched a file in officina/. This is the
canonical orientation for officina work: vision pins, culture, and the
re-orientation protocol. Silva's CLAUDE.md is this file's parent in spirit —
the same discipline, one project over.*

## Quid sit

Officina is the rhubarb toolchain: a C89 compiler with its own IR
(**medulla**, register-linear, basic blocks), an interpreter/VM
(**machinula**), a debugger (**vindex**), and a flat RAD-style debug format
(name floated: indicium). Silva is the front-end; semantica (M0, silva-side)
is the single home of C89 semantics; officina consumes both THROUGH THE
AMALGAM ONLY (silva/amalgama/silva.h — house rule, no fontes includes).
Explicitly NOT an LLVM competitor: targets REPLs, scripting, live tooling.

## PROTOCOLLUM — re-orientation (session start / post-compaction)

Do these IN ORDER before writing code:

1. **This file** (auto-loaded — done).
2. **officina/phase-log.md** — TAIL first: current INTENTIO/RELATIO.
3. **The spec for what you're touching** — the CURRENT milestone is
   whatever the phase-log tail says, never this file. Specs by layer:
   officina-brainstorm.md (architecture DECISUS + the why),
   officina-spec-v2.md (corpus bar, builtin surface, waves),
   officina-m1-spec.md (medulla core + demissio),
   officina-m2-spec.md + officina-m2-diagnostica.md (VM, conexio,
   cursor, builtins, ansae),
   **officina/vocabularium-medullae.md (the SEALED IR vocabulary — op
   names, suffixes, sigils are ceremony; do not coin new ones
   casually)**. Never implement a chunk from a compaction summary alone.
4. **Any .worklog.md** next to files you're editing. The measured
   interpreter performance decomposition lives in
   officina_machinula.worklog.md (2026-07-10) — read it before ANY
   speed work; optimize only from measurement.
5. Simulation artifacts carry the discovered complications:
   officina-simulatio-3.md (C1-C9) for M1, officina-simulatio-4.md
   (C1-C12) for M2.

At every MILESTONE boundary: full phase-log re-read + audit (silva rule).
Before compaction: append a phase-log addendum with anything in-flight.
When narrowing scope: name the deferred use case and the path back.

## VISIO — what officina is FOR

- **The corpus bar is the acceptance bar.** Machinula passes the existing
  credo suites UNMODIFIED (Waves 0+1 ≈ 80/106 = v1). The monorepo is the
  test corpus, as byte-exactness was for silva.
- **Real pointers in one owned region.** Interpreted pointers ARE host
  pointers; every allocation provably lives in the VM-owned mmap region.
  Preserves determinism, snapshots, and the time-travel v2 flagship.
- **Provenance is a product feature.** Every medulla instruction carries
  origo in memory, always — silva kept expansion provenance so vindex can
  do macro-aware stepping nearly free (M3). Never optional, never debug-only.
- **The textual medulla form is a thousand-year surface.** The dump defines
  canonical form; round-trip is byte-identical; the vocabulary is sealed in
  vocabularium-medullae.md. Changing an op name after goldens pin is a
  ceremony, not a refactor.
- **Demissio makes C89 semantics visible** (conversions, scaling,
  short-circuit control flow) and contains ZERO judgment: type CHECKING is
  the future enforcement layer's; demissio assumes well-typed input. The
  sistere op is that layer's named future runtime seat.
- **Format-first debugger.** Vindex drives the VM only through the debug
  format — the format is battle-tested before native codegen exists.
- **No dead ends for execution or native.** Medulla serves interpreter AND
  the M5 linear-scan aarch64 backend; value representations must admit
  native code pointers later.

## CULTURA

- Bars close milestones; nothing ships without its bar (M1a: byte-identical
  round-trip; M1b: corpus lowers, 0 ruinae, sistere classified; M2: Waves
  0+1 green interpreted + the STANDING second bar — stdout byte-diff vs
  pinned native captures, named exclusions with recorded causes).
- One unit-tested module at a time; interface first; walking skeleton before
  polish. Instruments precedent: haruspex/auspex (clang-certified semantics)
  → M1's analog is golden dumps + fusor + round-trip.
- Standing rule: every new public officina.h row = a hospes call, same change.
- Amalgam discipline: shared mechanism (silva/instrumenta/silva_amalgama);
  run ./officina/amalgamare.sh after ANY fontes edit; VERIFICATUM gates.
- Simulate before building (simulatio-3 preceded M1; both prior sims paid).
- **INSTRUMENTA DEBRIEF (Fran, 2026-07-10):** every chunk/milestone RELATIO
  carries a short instrument debrief: *adhibita* (tools reached for, rough
  counts), *fructus* (reaches that changed the outcome), *asperitates*
  (rough edges — note these in the TOOL'S worklog AT THE MOMENT of the
  reach; the debrief compiles, never recalls), *desiderata* (reaches for
  tools that don't exist = the demand signal; next tool's spec). Sim-2 §5's
  usage prediction is audited from these at M3 close.
- Latin in the code; English fine in meta-documents. Corpus caveat: knotapel
  is plain English C — grep-audits must be bilingual.
- Fran understands all the code; teaching is part of the job.
- Conventions: root CLAUDE.md + silva landmines apply UNCHANGED — i32 is
  UNSIGNED (s32/s64 for signed); every latina.h #define is a reserved word
  (nomen/registrum/magnitudo/exire/structura/casus...); chorda is NOT
  null-terminated.

## Build/test

- ./officina/compile_probationes.sh [filter] — glob-discovery suite runner
  (saltuarius pattern; probatio_officina_*.c in probationes/).
- ./officina/amalgamare.sh — regenerates COMMITTED amalgama/officina.c and
  runs gates (standalone strict compile, hospes, nm-intersection 0).
- ./officina/cursor.sh — THE corpus sweep (Waves 0+1, world-link from
  cache, fork-per-suite, classification TABULA + stdout-diff column).
  Flags: -sola <substr> (solo suites, stdout visible), -mora <n>
  (per-suite ceiling, default 30s), -custodia (wild-deref diagnosis,
  NOT standing), -enumerare (print wave paths; captare.sh consumes),
  -celer (FIRST arg: -O2+LTO build ~3.6x, objects in build/celer/ —
  for lapifex-class blessing runs; daily driver stays -O0).
- ./officina/captare.sh — pins native stdout goldens
  (fixa/stdout_nativa/) via double-run w/ 1s sleep; nondeterministic
  suites become named exclusions in cursor.c.
- ./officina/auspex_posix.sh — clang-certifies systema_posix.h layouts
  ≡ host (sizeof + offsets + macros). Gate for any systema_posix edit.
- ./officina/fusor.sh — corpus lowering sweep (sistere census,
  byte-idem circuli). Probes: -causa <str> (origin lines),
  -textus <file-substr> (dump one TU's medulla).
- Dev-time mains in instrumenta/principalia/.
- Goldens: probationes/fixa/ — .medulla hand-written fixtures +
  .c/.medulla golden pairs (re-pin = delete + suite run + BLESS by
  reading) + stdout_nativa/ pinned captures.
