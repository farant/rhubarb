# Officina simulatio-4 — M2 machinula + conexio + builtins

Date: 2026-07-09
Status: simulation artifact (sim-3 precedent). Three grounding agents ran
against the real codebase before the pretend-build; every complication
below is grounded in cited fact, not speculation. Feeds the M2 interview
→ INTENTIO. Companion: project-specs/officina-m2-diagnostica.md (the six
diagnostic investments, written pre-sim, all confirmed relevant).

## I. Terrain (grounding-agent findings, condensed)

**fundus-medulla (what the loader receives):**
- MedullaModulus: functiones/data = Xar of POINTERS; symbola/causae =
  Xar BY VALUE. All cross-refs are MODULE-LOCAL integer indices; -1 =
  universal "none". MedullaSymbolum genus EXTERNUM(0) + index -1 =
  referenced-but-undefined; intern promotes to FUNCTIO/DATUM only
  intra-module. Cross-module identity is 100% conexio's job.
- Call-site operands carry VALUES ONLY — no per-arg types; the .T on
  vocare is the RETURN type; registers are untyped 64-bit words. Args
  are pre-promoted (M0b default-arg promotions annotated), but width
  tagging is ABSENT from operands.
- stderr/stdout/stdin stay literally named (silva parses its own
  synthetic systema_c89.h: `extern FILE* stderr;` — no __stderrp macro
  ever enters the pipeline). Extern DATA symbols are shaped identically
  to extern FUNCTION symbols; only usage distinguishes them.
- Entry symbol = "main" (latina `#define principale main` expands
  pre-parse; demissio has zero special-casing).
- origo is a bare SilvaNodus*; (file,line) derivation needs (a) a
  descend-to-first-token helper officina must write (no public one-call
  accessor) and (b) the owning SilvaExpansio for fons_index→path.
  Synthetic tokens have byte_offset == -1.
- Relocations: 8-byte slots ONLY, additive, may target FUNCTION symbols
  (statica.medulla $tabula_functionum golden). ordinatio per-datum is
  load-bearing. Names are non-null-terminated chordas in per-module
  arenas (length-aware compare; never strcmp).

**fundus-libc (runtime surface):**
- credo's complete libc: printf, snprintf, memcpy, fabs, exit. No
  atexit/time/fflush. Exit code = main's return value, nothing else.
- Suite-#1 (piscina) builtin floor, deduped: malloc free strlen strcpy
  memcpy memset snprintf printf exit. snprintf needs %d (%lld/%.10f
  linked-but-unexercised); printf needs plain + "." + %zu.
- Memory-model asserts: CREDO_AEQUALIS_PTR is DIRECT pointer equality
  across arena reset (allocator must return bit-identical addresses);
  alignment assert casts real pointer to size_t, % 16 — malloc BASES
  must be >=16-aligned.
- %p only in debug dumps, never on pass paths. No %n anywhere. Float
  directives off the Wave-0 pass path. PASS-PATH STDOUT IS
  DETERMINISTIC AND ADDRESS-FREE → byte-for-byte stdout diff vs native
  is a safe bar.
- errno IS on the Wave-0 path: lib/entitas.c does errno=0/strtol/
  ERANGE. qsort: 12 sites, ALL knotapel — not needed for Latin suites.
- 16-byte struct-by-value (chorda, PiscinaNotatio) on suite #1's path
  is interpreted-to-interpreted — ALREADY DISSOLVED by M1's explicit
  %reditus + copia convention. Non-complication, confirmed.

**fundus-corpus (suite composition):**
- NO per-suite dependency lists exist. compile_tests.sh links ALL ~116
  lib objects into EVERY test. Header-driven deps (credo.o needs
  chorda.c with no #include; stml.h drags xar) make computed .c-level
  closures WRONG — verified empirically.
- Ground-truth minimal sets (link+run exit 0): piscina = 4 lib TUs +
  test; stml = 10 + test; arbor2_glr = 15 + test.
- Exit-code judgment confirmed verbatim (compile_tests.sh:389,663-667).
  argv: argc=1 safe, no suite consumes real argv.
- CWD = repo root load-bearing; probationes/results/roundtrip/ is a
  WRITE target; arbor_formator writes /tmp paths (Wave 1).
- Top assert counts: arbor2_glr 2,183; stml 456; arbor2_scribere 397.
  Slowest interpreted = arbor2_glr (every assert = a full GLR parse).
- ~94 suites actually执行 natively (11 GUI built-not-run + 1 benchmark
  skipped, hardcoded in the harness; no network skip — tcp runs live).

## II. Complications surfaced (C1-C12)

- **C1 — Merge = indirection, not rebasing.** Rebasing symbol indices
  means rewriting 1.15M instructions and breaks moduli immutability.
  RESOLUTION: global symbol table (tabula_dispersa + friatio over
  interned names, conexio-owned piscina) + a per-module translation
  array (local index → global). Instructions never change.
- **C2 — Link the world.** No manifests exist; closures computed from
  includes are provably wrong. RESOLUTION: mirror native — lower all
  lib TUs ONCE, cache moduli, conexio merges world + suite TU per run.
  The manifest problem is deleted, not solved.
- **C3 — Function-pointer values need REGION-RESIDENT DESCRIPTORS.**
  Relocations/locus target function symbols; a function's address must
  be a real, distinct, region-valid 8-byte value, but functions are
  interpreter structs. RESOLUTION: conexio allocates a descriptor per
  functio in the region; its address IS the fn-ptr value; indirect
  vocare reads descriptor→functio. Descriptor carries a tag field =
  the M5 native-vs-interpreted seam, designed in now. (Vision pin
  "values must admit native code pointers later" lands HERE.)
- **C4 — Extern-data builtin channel.** stderr/stdout/stdin/errno are
  EXTERNUM data symbols with plain names. The builtin table needs a
  DATA side: conexio allocates region cells; bridge maintains them.
  FILE* representation forks: opaque handles (0/1/2; region closed;
  deterministic) vs raw host FILE*. Leaning HANDLES. errno: bridge
  copies host errno into the VM errno cell after errno-setting
  builtins (strtol family — entitas suite is Wave 0!).
- **C5 — Register width: CANONICALIZE ON WRITE (the named DECISUS).**
  Operands carry no types; recovering widths at call sites = dataflow
  walk per call. RESOLUTION: every op writes a full 64-bit word,
  extended per the op suffix's signedness (movere.s32 sign-extends,
  legere.i8 zero-extends; f64 = bit-pattern). Variadic bridge then
  reads plain words. One rule, zero per-call analysis.
- **C6 — Trap stubs unify with sistere.** Link-the-world imports the
  socket/Cocoa tail: no definition, no builtin. RESOLUTION: unresolved
  symbols bind to lazy traps that halt-with-symbol-name IF CALLED.
  Load never fails on unused symbols; linked-but-unexercised libc
  (fabs, %lld) costs nothing. Classification gains "builtin deest".
- **C7 — origo distillation must happen AT LOWER TIME.** Node→line
  needs a hand-written token-descent helper + the SilvaExpansio, which
  is only alive during lowering. RESOLUTION: per-TU pipeline = parse →
  semantica → demissio → distill line-table (instr → interned path +
  linea) → FREE front-end arenas. Also the memory answer: 116 live
  parse trees would recreate the 9.7GB apex; moduli + line tables are
  ~100MB-scale. The line table IS proto-indicium (M3's first section,
  born early — as predicted in officina-m2-diagnostica.md).
- **C8 — exit/unwind + happy-path status.** exit is reachable in Wave 0
  (fatal-alloc, credo not-opened). RESOLUTION: halt-flag unwind (no
  host longjmp needed); main's redde is the status otherwise (no
  atexit hooks exist — grounded).
- **C9 — Interpreted wall-clock is a corpus-bar property.** arbor2_glr
  (2,183 parse-driving asserts) plausibly runs MINUTES interpreted;
  C89 -pedantic forbids computed goto → switch dispatch is the ceiling
  until M5. RESOLUTION: accept for v1; runner gets per-suite timing
  columns day one; dispatch counters (diagnostica #5) decide if anyone
  ever cares.
- **C10 — The differential bar can be STRONGER than exit codes.**
  Pass-path stdout is deterministic and address-free (grounded) →
  runner captures stdout and byte-diffs vs native as the second
  column. %p stays in debug paths; pass through unmodified.
- **C11 — .medulla fixtures are executable unit tests.** The lector +
  interpreter = hand-written fixture runner with no demissio in the
  loop (summa.medulla from vocabularium §VIII is the first walking
  skeleton). The M1 textual investment pays its second dividend.
- **C12 — Wave-1 environment facts.** CWD = repo root; results/
  roundtrip is a WRITE target; /tmp writes flow through host-fopen;
  struct stat = the systema_posix pull, confirmed as the named Wave-1
  prerequisite (unchanged from the M1 addendum).

## III. Predictions vs surprises

HELD (from the M2 walkthrough + diagnostica note, pre-sim): dragons
dead; exit-code bar mechanics; piscina-first as memory-model gate;
qsort-not-needed-until-knotapel; origo→line-table convergence with
indicium; flight-recorder/deterministic-base/classification-table all
confirmed necessary. Struct-by-value worry dissolved by M1 convention
(predicted "no new aggregate ABI needed" — confirmed).

SURPRISES (all at SEAMS, consistent with the M1 law):
1. Link-the-world (C2) — the native harness's own laziness becomes the
   design; nobody predicted there were NO manifests at all.
2. Function descriptors (C3) — the fn-ptr value question was invisible
   until relocations-target-functions met the owned region.
3. errno on the Wave-0 path (C4) — entitas, not filum, pulls errno
   bridging first.
4. No public node→line accessor (C7) — the provenance vision pin has a
   missing rung officina must build itself.

## IV. Recommendations (R1-R8)

- R1: Conexio = global-table + per-module translation (C1); moduli
  stay immutable; names interned length-aware.
- R2: Link-the-world with cached moduli + lazy trap stubs (C2, C6).
- R3: Function descriptors with a tag field, designed for M5 (C3).
- R4: Builtin DATA channel; FILE* as opaque handles; errno write-back
  (C4). Interview: handles vs host FILE*.
- R5: Canonicalize-on-write register convention → DECISUS (C5).
- R6: Per-TU lower pipeline distills line tables then frees front-end
  arenas (C7); the line table is proto-indicium and should be shaped
  with M3 in mind.
- R7: Runner columns: exitus | stdout-diff | tempus | classification
  genus (praeteriit / exitus dissimilis / sistere+causa / vitium VM /
  builtin deest / tempus excessum) (C9, C10).
- R8: M2 chunk sketch — M2a conexio + region + descriptors + loader
  (bar: world links, $main found, relocations patched, hand-written
  .medulla executes); M2b interpreter core + flight recorder + Wave-0
  floor builtins (bar: probatio_piscina GREEN interpreted); M2c Wave-0
  sweep + classification runner (bar: Wave 0 green); M2d Wave-1 +
  systema_posix pull (bar: Waves 0+1 green = v1).

## V. Interview questions (for the M2 DECISUS conversation)

1. FILE* representation: opaque handles vs raw host FILE* in the
   stderr/stdout/stdin cells? (R4; handles recommended — region stays
   closed, deterministic, time-travel-friendly.)
2. Trap-stub policy: lazy-fault-on-call for ALL unresolved symbols, or
   load-time error for anything outside a known-external allowlist?
   (Lazy recommended; allowlist adds ceremony without safety.)
3. Function descriptor shape: minimal {tag, functio-ref} 16 bytes, or
   reserve extra fields (arity? module?) for M5/vindex now?
4. Canonicalize-on-write: accept as THE register convention (C5), or
   keep sub-64 residue undefined and tag widths at the bridge only?
5. Link-the-world memory posture: accept lower-once-cache-all (~100MB
   moduli resident) + free-front-end-per-TU as the standing pipeline?
6. Wave-0 wall-clock tolerance: is minutes-scale acceptable for the
   standing corpus bar, or does arbor2_glr get a named budget?
7. Chunk split: adopt R8's M2a-M2d, or fold conexio+interpreter into
   one chunk (riskier, fewer gates)?
8. stdout differential bar: adopt byte-diff-vs-native as a STANDING
   second column (C10), or exit-code-only per the original spec?
