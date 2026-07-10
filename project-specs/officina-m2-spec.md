# Officina M2 spec — machinula + conexio + builtins

Date: 2026-07-09
Status: the M2 plan. Already codebase-grounded (three grounding agents
ran BEFORE simulatio-4; every design fact below is cited from real
code, not assumption). Lineage: officina-spec-v2.md §III/§IV/§V/§VIII
(corpus bar, builtin surface, dragons, machinula notes) →
officina-simulatio-4.md (terrain + C1-C12 + R1-R8) →
officina-m2-interview.md (16 DECISUS, all recommendations accepted) →
THIS. Companion: officina-m2-diagnostica.md (six diagnostic
investments — flight recorder / deterministic base / poison guards /
classification table / counters / -vestigium — ALL adopted into
chunks below).

## I. What M2 is

The milestone where officina EXECUTES. Three new fontes modules + one
instrument:

- **officina_regio.{h,c}** — the owned memory region. One large
  fixed-base mmap reservation (16-64GB address space, lazy commit);
  areas: globals | stack | heap. Real host pointers ARE VM pointers.
  Region-malloc: free-list allocator, bases >=16-aligned (piscina
  alignment assert takes real-pointer %16), poison-fill + in-region/
  alignment guards under debug flag. Deterministic base = diffable
  runs = the first brick of v2 time-travel.
- **officina_conexio.{h,c}** — linker/loader. Merges moduli via a
  GLOBAL symbol table (tabula_dispersa + friatio over names interned
  length-aware into a conexio piscina) + one translation array per
  module (local index → global); moduli stay IMMUTABLE (no
  instruction rewriting). Places data per ordinatio, copies images,
  patches relocations ADDITIVELY (slot = symbol address + prior 8
  bytes). Allocates a 16B region-resident DESCRIPTOR {s64 tag;
  functio ref} per functio — descriptor address IS the fn-ptr value
  (tag = the M5 native seam). Unresolved symbols bind to LAZY TRAP
  STUBS (halt with symbol name IF CALLED — sistere's posture at the
  link seam). Extern-data builtin channel: region cells for
  stdin/stdout/stderr (opaque handles 0/1/2) + errno (bridge
  write-back after errno-setting builtins).
- **officina_machinula.{h,c}** — interpreter core + builtins. Frame =
  register word file (sized from functio->registra) + arca slab +
  cursors; switch dispatch (C89 forbids computed goto). WIDTH
  CONVENTION (DECISUS): canonicalize on write — every op writes a
  full 64-bit word extended per the op suffix's signedness; f64 as
  bit-pattern; reads take low bits. Flight recorder ALWAYS ON
  (fixed ring of functio/bloccus/instr-index; halt report = current
  position + registers + call stack + ring tail, all with file:line
  via distilled origo, to stderr). Explicit stack check at vocare
  (breach = honest halt). exit = halt-flag unwind; main's redde =
  process status. sistere at runtime = halt printing causa +
  position. Builtins: STATIC internal table (public registration row
  deferred to M4 by name); every builtin call routes through ONE
  chokepoint (named as the v2 record/replay seam, nothing built).
  Dispatch counters (per-op, per-functio, builtin frequency, memory
  high-water) — the jump-table park's evidence instrument.
- **instrumenta/principalia/cursor.c + cursor.sh** — the corpus-bar
  runner (separate from fusor, which stays the standing lowering
  gate). Pipeline per TU (ONCE, cached): parse → semantica → demissio
  → distill origo line-table (instr → interned path + linea; needs
  the SilvaExpansio, so distillation happens while the front-end is
  alive) → FREE front-end arenas. Moduli + line tables resident
  (~100MB-scale; parse trees never accumulate — the 9.7GB-apex
  answer). Per suite: conexio merge (LINK THE WORLD — all lib moduli
  + suite TU, mirroring compile_tests.sh's link-everything reality;
  no per-suite manifests exist or are computable) → find $main → run
  argc=1 → CLASSIFICATION TABLE row: praeteriit | exitus dissimilis |
  sistere (causa) | vitium VM (halt report) | builtin deest
  (symbolum) | tempus (per-suite timing column) — plus the stdout
  byte-diff column vs pinned native capture (STANDING second bar;
  pass-path output proven deterministic + address-free;
  nondeterministic suites get named exclusions). CWD = repo root.

## II. Grounded facts the design stands on (from simulatio-4 §I)

- Entry symbol is literally "main" (latina expands principale
  pre-parse; demissio does zero special-casing).
- MedullaSymbolum EXTERNUM(0)+index -1 = unresolved; symbol identity
  is intra-module only; ALL cross-refs are module-local integer
  indices; symbola/causae by-value Xars, functiones/data pointer Xars.
- Relocations: 8-byte slots only, additive, MAY target function
  symbols; ordinatio per-datum load-bearing; names are
  non-null-terminated chordas in per-module arenas.
- Call operands carry VALUES ONLY (no per-arg types); registers
  untyped; args pre-promoted by M0b annotations.
- stderr/stdout/stdin stay plainly named (silva's own systema_c89.h:
  extern FILE* rows; no __stderrp ever). errno is on the WAVE-0 path
  (lib/entitas.c strtol/ERANGE).
- Suite-#1 builtin floor: malloc free strlen strcpy memcpy memset
  snprintf printf exit (+%zu; %lld linked-unexercised). Pointer
  identity assert = direct equality across arena reset; malloc bases
  must be >=16-aligned.
- No %n anywhere; %p only in debug paths; pass-path stdout
  deterministic. qsort = knotapel-only (12 sites) — NOT needed for
  Waves 0+1; the qsort-as-builtin plan stays parked until a knotapel
  wave.
- 16B struct-by-value on suite #1's path is interpreted-to-interpreted
  — already lowered by M1's %reditus+copia convention. Non-issue.
- Empirical minimal TU sets: piscina=4+test, stml=10+test,
  arbor2_glr=15+test (but cursor links the world anyway).
- origo→(file,line) requires an officina-written first-token descent
  helper + the SilvaExpansio (no public one-call accessor; synthetic
  tokens byte_offset==-1 map to no real location).
- Exit-code judgment verbatim at compile_tests.sh:389,663-667; argv
  safe as argc=1; ~94 suites execute natively (11 GUI + 1 benchmark
  skipped); slowest interpreted = arbor2_glr (2,183 parse-driving
  asserts).

## III. Chunks and bars

**M2a — conexio + regio + loader.**
Deliverables: officina_regio (reservation, areas, region-malloc,
guards), officina_conexio (global table + translations, placement,
additive relocation patching, descriptors, trap stubs, extern-data
cells, origo line-table distillation intake), lector-fed loading of
hand-written .medulla, ~8-12 per-op-family fixtures BEGUN (data +
relocatio + vocationes families need M2b to execute fully; loading
asserted here). Amalgam rows + hospes per standing rule.
BAR: all cached lib moduli + a test modulus LINK (world links: 0
load-time failures, trap stubs counted + named); $main found;
relocations patched (golden: a fixture datum's slot = expected
address+addend); descriptors allocated; summa.medulla LOADS.

**M2b — machinula core + Wave-0 floor builtins.**
Deliverables: dispatch loop (all 35 ops), width convention
implemented, frames + arca + stack check, flight recorder + halt
report (always on), counters, sistere runtime, exit/redde status
plumbing, builtin chokepoint + floor: malloc free strlen strcpy
memcpy memset snprintf printf exit (+ handle cells wired; errno
write-back mechanism in place), .medulla fixture suite COMPLETE and
green (per-op-family exit-value asserts).
BAR: **probatio_piscina GREEN interpreted** (exit 0 + stdout
byte-identical to native) — the memory-model gate: pointer identity,
16-byte alignment, arena reuse, indeterminate malloc.

**M2c — cursor + Wave-0 sweep.**
Deliverables: cursor.c/cursor.sh (world-link from cache, run,
classification table + timing + stdout-diff columns), native capture
pinning, remaining Wave-0 builtins as the classification table pulls
them (strtol family + errno, fabs/sin/cos/pow/fmod/ldexp, atoi/atof/
strtod, ctype, sprintf/fprintf), named exclusions recorded.
BAR: **Wave 0 GREEN** (~65 suites: piscina → chorda/xar/tabula/
internamentum/friatio → encoders → the four parser stacks). Expect
real finds — the corpus-contact law's data points are 3 (M0b) and 8
(M1b); budget the days.

**M2d — Wave 1 + systema_posix.**
Deliverables: file builtins (fopen family via handle table, stat,
mkdir, getcwd), systema_posix pull (struct stat layout — the NAMED
prerequisite; the 1,160 POSIX-classified sistere sites are the
signal: one firing at runtime = pull time), results/ + /tmp write
paths verified, Wave-1b/2/3 deferrals restated by name.
BAR: **Waves 0+1 GREEN (~80 suites) = M2 SEALED = officina v1
execution shipped.**

## IV. Parks with owners (updated from M1)

- Variadic arg-width convention → DISSOLVED by canonicalize-on-write
  (Q4). Remove from parks.
- Jump tables → M2-if-profiled; counters (M2b) are the evidence
  instrument; decision falls out of M2c timing data.
- qsort-as-builtin → parked until a knotapel wave (grounded: zero
  sites in Waves 0+1).
- Public builtin registration row → M4 (first real consumer).
- Builtin record/replay → v2; the chokepoint (M2b) is the named door.
- Textual provenance attribute → M3 unchanged; the distilled line
  table (M2a/cursor) is proto-indicium and should be shaped with M3
  in mind.
- Wave 1b (system/opendir), Wave 2 (time/entropy stubs), Wave 3
  (network), GUI 11 + benchmark → named deferrals per spec-v2 §III.
- Descriptor enrichment (arity/name fields) → M5/vindex on real need.

## V. Vocabulary additions (ceremony-lite; no sealed-surface changes)

New NOUNS only (no ops, no suffixes, no sigils touched):
regio (the owned region), descriptor (function descriptor), ansa
(opaque handle — FILE* cells), cursor (the corpus runner), vestigium
(trace mode/flag), custos (memory guard, debug), tabula causarum →
extended with "builtin deest" genus. Latin identifiers per house rule;
i32-unsigned + latina-reserved-word landmines apply as ever.

## VI. Standing risks (carried, eyes open)

- The seams law (3 data points): expect surprises at conexio↔medulla
  and machinula↔host-libc boundaries, not in the designs above. The
  classification table + flight recorder exist to make those
  surprises cheap.
- snprintf bridge fidelity: per-directive re-emit to host snprintf;
  %zu width tied to host size_t; %.17g/%.10f fidelity inherited from
  host. Differential stdout bar polices this continuously.
- Indeterminate malloc: a corpus test accidentally depending on
  native garbage could diverge interpreted. If found: that is a
  corpus bug, surfaced honestly (poison-fill makes it recognizable).
- arbor2_glr wall-clock: accept minutes; timing column is the
  evidence; no optimization before M2c data.
