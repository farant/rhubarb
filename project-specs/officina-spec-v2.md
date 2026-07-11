# Officina — spec v2 (codebase-grounded)

Date: 2026-07-08
Status: v2 planning spec. Complements officina-brainstorm.md (the DECISUS +
architecture live there; this doc grounds every milestone in what the
codebase actually contains). Method: four parallel exploration agents —
extern/dragon audit (nm -u cross-checked against include scan), silva
consumer's-guide, credo corpus-bar mechanics, infrastructure conventions.

---

## I. What v2 established (headline per investigation)

1. **The dragon list mostly died.** Monorepo-wide: ZERO setjmp/longjmp,
   ZERO varargs DEFINED in the monorepo (we call variadic libc but never
   interpret a variadic body), ZERO bitfields, ZERO threads, ZERO alloca,
   ZERO inline asm, ZERO __attribute__/#pragma pack. Pure C89, natural
   alignment. The machinula core stays simple.
2. **The corpus bar is exit-code-only.** Nothing parses test output;
   credo folds pass/fail into principale's return. Machinula's obligation:
   run principale, return its int, 0 = pass.
3. **The builtin surface is ~105 libc/POSIX symbols**, grouped and
   attributed (§IV), plus a 19-symbol Objective-C platform tier that is
   NOT interpretable from source (§II decision 2).
4. **Silva's consumer surface is ready but one export gap exists**: the
   58-genus enum + 176 named accessors are compiled into the amalgam but
   not declared in public silva.h (§II decision 1).

## II. Decisions raised by exploration (DECISUS — Fran approved all
three, 2026-07-08)

1. **Silva.h export gap → named pre-M1 task.** semantica lives silva-side
   (DECISUS 8) and can include fontes headers directly, as the oracle
   does. But demissio is officina-side and needs genus-level tree access
   through the amalgam boundary (consumers include amalgama/silva.h ONLY —
   house rule). RECOMMENDATION: extend hand-written silva.h to re-export
   SILVA_C89_GENUS_* + the accessors (silva.h:18-19 already names this as
   "arrives with M2" — it never closed). Mechanical, additive; hospes
   calls per standing rule.
2. **Tier-2 platform layer is out of corpus scope.** Three .m files
   (fenestra_macos.m, clipboard_platform_macos.m, tls_macos.m — Cocoa/
   Security) implement ~19 symbols; they cannot be interpreted from C89
   source. RECOMMENDATION: exclude — the harness already treats every GUI
   suite as build-only (compile_tests.sh skip-list), and TLS defers with
   sockets in Wave 3. No corpus-bar loss.
3. **Corpus bar v1 = Waves 0+1 (~80 of 106 suites)** with named
   deferrals (§III). The brainstorm's "suites pass unmodified" refines to
   this; deferrals are honest and listed, not silent.

Confirmed by evidence (no decision needed):
- **Real-pointers-in-owned-region (DECISUS 6) is REQUIRED, not just
  elegant**: probatio_piscina asserts pointer identity across arena reset
  (CREDO_AEQUALIS_PTR) and 16-byte alignment via pointer→integer casts.
  Handle-based pointers would fail the corpus.
- **Indicium = flat offset-table sections** (brainstorm §XI.2 instinct
  confirmed): nuntium is a sequential-cursor TLV with zero-copy views but
  NO random access — fine for streams, wrong for symbol lookup. Nuntium
  may still serve small header/directory records; the body is flat
  sections + string table, mmap-friendly.

## III. The corpus bar, made precise

**Judgment**: exit code only (compile_tests.sh:389). Run principale to
completion, return the int. argc/argv: argc=1, argv={progname} is safe —
no pure-compute suite consumes real argv. CWD must be repo root
(fixture paths are relative: arbor2_file_roundtrip, macho).

**Wave structure** (106 probatio files total):
- **Wave 0 — pure compute (~65 suites).** Order: piscina FIRST (validates
  the memory model: identity, alignment, reuse), then chorda/xar/
  tabula_dispersa/internamentum/friatio, then encoders (nuntium, base64,
  utf8, url, json, toml, xml, stml, http_parser, multipart), then the
  four parser stacks (arbor*, arbor2*, lapifex*, sputnik*) — large, pure,
  recursion+arena-churn heavy = the best VM stress test. Needs math.h
  doubles already (json strtod, sputnik, CREDO_F64_PROXIMUS).
- **Wave 1 — file builtins (~15 suites).** filum, entitas_repositorium,
  arbor_formator, arbor2_file_roundtrip, macho. Needs fopen/fread/fwrite/
  fclose/fseek/ftell/fgetc/fgets/ungetc/feof/ferror/fflush/remove/rename
  + stat/mkdir/getcwd.
- **Wave 1b — process spawning (gate separately).** iter_directoria,
  generare: need system() + opendir/readdir.
- **Wave 2 — time/entropy (gate or stub).** uuid (format-only asserts —
  deterministic stubs pass), tempestivum, actor, actor_integratio
  (usleep; verify timing sensitivity).
- **Wave 3 — defer, named.** tcp (LIVE INTERNET — fails under clang
  without network; never gate on it), tcp_servus (known flaky), reactor,
  tls; the Cocoa GUI skip-list stays compile-only as it is today.

**v1 bar = Waves 0 + 1 green** (~80 suites), the rest named deferrals.

**Harness facts to preserve**: credo macros use # stringize + __FILE__ +
__LINE__ — the expander path must produce EXECUTION-grade values, not
merely parseable ones (silva parses the corpus, but a wrong __LINE__
would still parse; see §VI pre-M1 verification). snprintf %lld required
(credo formats s64). Static file-scope state (credo's notatio list) must
have whole-run lifetime.

## IV. The builtin surface (the real list)

Two-tier; Tier 1 (~105 libc/POSIX) becomes builtins, mapped to waves:

- **memory** (Wave 0): malloc realloc free — direct callers are ONLY
  piscina, chorda, imago; allocation is already concentrated behind the
  arena. Serve from the VM-owned region.
- **string/mem** (Wave 0): memcpy memmove memset memcmp strchr strcmp
  strlen strncmp strstr strcpy strncpy bzero. (Fortify *_chk variants in
  the nm output are clang artifacts — lowering from source calls base
  names; natives matter only at M5.)
- **stdio/format** (Wave 0 core + Wave 1 files): printf fprintf snprintf
  sprintf (per-directive bridge to host snprintf — brainstorm §XI.5
  confirmed; must handle %lld); file family per Wave 1; stderr distinct.
- **stdlib/convert** (Wave 0): atoi atof strtod strtol strtoul abs exit
  atexit.
- **ctype** (Wave 0): isalpha isdigit islower isupper isspace iscntrl
  tolower toupper.
- **math** (Wave 0): fabs sin cos pow fmod ldexp — full IEEE-754 double.
- **time** (Wave 2): time clock localtime gettimeofday usleep.
- **random** (Wave 2): arc4random_buf (uuid only; deterministic stub OK —
  asserts are format-only).
- **fs/dir** (Wave 1/1b): stat mkdir getcwd opendir closedir readdir
  close read write fcntl system.
- **terminal + signals** (post-corpus, vindex-adjacent): tcgetattr
  tcsetattr cfmakeraw ioctl isatty select poll signal sigaction raise —
  ALL from tessera_pons_posix.c only.
- **networking** (Wave 3, deferred): the socket family — tcp_posix.c only.

**The trampoline trio** (native→interpreted callbacks — the ONLY libc
entry points that receive monorepo function pointers):
1. qsort — 11 sites, all in knotapel. Cleanest v1 answer: implement qsort
   AS a builtin whose comparator invocation is just a VM call (no ABI
   trampoline at all — the "native" side is ours).
2. signal/sigaction + atexit — tessera TUI only; post-corpus. Same
   builtin-owns-the-callback pattern; async-signal caution noted (the
   handlers are written async-signal-safe: write + tcsetattr only).

Function pointers generally: pervasive as INTERNAL dispatch (vtables,
comparators, resolutors) — interpreter-native, a fn-ptr value is a
reference to an interpreted functio. Never crosses an ABI in v1.

## V. Dragons: dead vs live

DEAD (audited, zero uses): setjmp/longjmp, monorepo varargs bodies,
bitfields, threads, alloca, inline asm, __builtin_*, __attribute__,
packed structs, fork/exec (real), long double, _Complex.

LIVE (each with its home):
- **IEEE-754 doubles + 5 math fns** — machinula arithmetic (f64 ops).
- **long long** — s64 = signed long long (latina.h:412, ~624 uses);
  medulla i64 + %lld formatting.
- **Union member aliasing** — 14 files use the const-strip idiom
  (unio { constans character* c; i8* m; }; write .c, read .m). Members
  MUST share storage. Real-memory model (DECISUS 6) satisfies this
  naturally: a union is one region slot, loads reinterpret bytes.
- **Native struct layout** — macho.c freads file bytes into structs
  (little-endian, field offsets). Semantica's layout must match the
  host ABI exactly; the clang sizeof/offsetof oracle (M0 bar) is the
  proof instrument.
- **Indeterminate malloc memory** — do NOT zero-fill allocations;
  model malloc semantics or risk masking/exposing uninitialized reads
  the tests currently tolerate (chorda is length-prefixed and NOT
  null-terminated — padding differences change behavior).
- **Pointer identity + alignment asserts** — region allocator must
  return identical addresses on arena reuse; 16-byte alignment honored
  and observable through pointer→integer casts.
- **CWD sensitivity** — corpus runs from repo root.

## VI. Semantica (M0), grounded

Consumes silva fontes directly (silva-side, like silva_c89_oraculum.c
consumes itself): silva_nodus.h, silva_token.h, silva_c89_oraculum.h,
silva_commissio.h, silva_tabulae_c89.h (58 genus enum + 176 accessors),
grammatica/genera-c89.md as the loci reference.

**Facts that shape the walk:**
- parsura->commissio->radix is a LISTA, not a node (no truncus wrapper).
- Loci are the full-fidelity ordered children (tokens included);
  silva_nodus_liberi is node-only. Semantica/demissio use loci.
- LISTA loci are views: silva_valor_lista_numerus/_obtinere ONLY (never
  xar_numerus on the store). Separated lists interleave separator tokens.
- token->valor is a chorda ALIASING the source buffer — symbol-table
  keys must be copied/interned (internamentum exists for exactly this).
- parsura->fons_princeps is NOT 0 once includes are provided — use it.
- Typedef has NO distinct genus; SilvaDeclaratioVista.est_typedef is the
  precomputed answer. The vista (one row per declarator: genus, titulus,
  linea, situs, est_typedef, fons_index) is the symbol table's seed.
- Declarator chains walk via internum until declarator-titulus/NIHIL;
  type construction reads layers in reverse + specificatores for base.
  Reference loop: silva_c89_declaratoris_titulus (oraculum.c).

**What M0 adds** (M0a + M0b BOTH SHIPPED 2026-07-08 — see
silva/phase-log.md; historical text refreshed per §XII.8):
1. Scoped symbol tables — SHIPPED (M0a). Ordinary/tag namespaces;
   labels deliberately lint's; est_implicitum marks synthesized
   implicit-int callees (enforcement-layer datum, M0b C).
2. Real types — SHIPPED (M0a). Interned derived types (pointer
   equality = identity), nominal tags.
3. Layout — SHIPPED (M0a), clang-certified: haruspex 4,801
   compile-time asserts, 0 dissent.
4. Expression typing + implicit conversions — SHIPPED (M0b, its own
   milestone per silva-semantica-design.md): parallel table nodus →
   {typus naturalis, typus conversus}; UAC clang-certified by
   auspex (3,753 asserts, 0 dissent); demissio reads exactly these
   two columns. (Type CHECKING in the judgmental sense = the future
   enforcement layer, deliberately separate.)
5. Ambiguity closure — SHIPPED (M0a), via the TRIVALENT oracle
   (typus / non-typus / ignotus — the binary set was M1's; the
   census showed 84% of survivors discriminate on NON-types):
   15,261 → 97.3% RESOLVED; residuum classified (nested-chain
   park + POSIX + fixtures), shrinking as knowledge grows
   (M0b: 411 @M0a-close → 414 w/ corpus growth → 305 without
   resolver changes — FILUM flip −59, implicit-callee feed −50).
   Typing is
   CANONICAL-RELATIVE: analyze after the last recanonicare
   (percursus two-pass, M0b D).

**Architecture**: trees are immutable post-commit — semantica builds
PARALLEL tables keyed by SilvaNodus* (the oracle's responsa cache is the
in-repo precedent for this exact pattern).

**Pre-M1 verifications (small, named):**
- Expander execution-grade audit: # stringize, ##, __FILE__, __LINE__
  values must be correct for EXECUTION, not merely parseable (every
  CREDO macro uses them; a wrong __LINE__ parses fine and lies later).
- The silva.h export extension (§II decision 1) + hospes calls.

## VII. Demissio + medulla notes from exploration

- Walk from the root LISTA; descend AMBIGUUS via canonica (record the
  node); ERROR nodes cannot reach demissio (corpus is zero-error).
- Token origo provenance (macro expansion chains, silva_token_radix/
  _profunditas) flows into indicium: the line table can record both the
  expansion site and the spelling site — macro-aware stepping in vindex
  comes almost free because silva kept provenance. byte_offset == -1
  marks synthetic tokens (map to invocation site via origo).
- Binary expression tiers all collapse to genus binarium (tok_operator
  discriminates) — demissio dispatches on ~55 genera total, not
  grammar nonterminals.

## VIII. Machinula notes from exploration

- Memory model obligations (§V): real pointers, identity on reuse,
  16-byte alignment, indeterminate malloc contents, union aliasing,
  host-ABI layout. All satisfied by DECISUS 6 (owned region, real
  pointers) + semantica's layout tables.
- Builtins arrive in waves (§IV); Wave 0 floor is small: malloc free
  realloc, mem*/str* group, printf/fprintf/snprintf/sprintf, exit,
  atexit, atoi/atof/strtod/strtol/strtoul/abs, ctype, fabs sin cos pow
  fmod ldexp.
- qsort-as-builtin dissolves the only corpus-bar trampoline.
- Static storage (file-scope universalis state) has whole-run lifetime
  in the region's globals area (credo's notatio list depends on it).
- Interpreted fn-ptr values = references to interpreted functiones;
  design the value representation so a later native transition can tag
  native vs interpreted targets.

## IX. Vindex + indicium notes from exploration

- Indicium body = flat sections + string table + offset tables,
  mmap-friendly (nuntium ruled out for the body: sequential cursor, no
  random access; MAY serve the small header/directory).
- Vindex TUI is an APP on amalgams (saltuarius precedent), not an
  amalgam: thin principale consuming officina.h + tessera.h. Lives in
  officina/instrumenta/principalia/ (or graduates to sibling vindex/
  later — decide when it exists).
- Tessera embedding pattern (from saltuarius): tessera_pons_posix +
  opus + lector; frame = draw-all → praesentare → eventum_expectare →
  command-mapper → per-view handlers (source/registers/stack/memory/
  breakpoints views). Headless tests via tessera_pons_memoriae +
  PROBA_CELLA-style cell asserts; view-models never touch the
  filesystem (injection seam = unit-testable TUI).
- App-side chorda_aedificator discipline: pre-size + reset per frame,
  never finire (aliasing leak — saltuarius lore).

## X. Project skeleton + build wiring

```
officina/
  CLAUDE.md               # orientation: vision pins + protocol + build/test
  phase-log.md            # append-only; INTENTIO/RELATIO per phase
  fontes/                 # officina_*.{c,h} modular, unit-tested
  probationes/            # probatio_officina_*.c + officina_proba.{c,h}
  instrumenta/
    principalia/          # amalgamator.c, hospes.c, REPL main, vindex main
  amalgama/               # COMMITTED: officina.c + hand-written vanilla officina.h
  compile_probationes.sh  # glob-discovery runner (saltuarius pattern)
  amalgamare.sh           # references SHARED mechanism silva/instrumenta/silva_amalgama.{h,c}
  build/                  # gitignored
```

- Amalgam mechanism is SHARED — officina supplies only its manifest
  (amalgamator.c) + hospes.c gate; amalgamare.sh adds -Isilva/instrumenta
  and compiles silva_amalgama.c directly (tessera precedent).
- VERIFICATUM gates: amalgamator → standalone strict compile (no include
  paths, no -Wno-unused; excludenda lists ARE the zero-unused proof) →
  hospes (latina-pollution probe, equivalence smoke, every-public-
  function-called rule) → nm-intersection 0 vs dep objects (prefix-rename
  discipline).
- Standing rule carried: new public function ⇒ new hospes call, same
  change.
- Tests: drop probatio_officina_foo.c in probationes/ — glob discovery,
  no arrays to edit. Own runner; stays out of root compile_tests.sh's
  SOURCE_FILES array.
- Strict flag block identical to house standard (-std=c89 -pedantic
  ... -Wwrite-strings -Wno-long-long -fbracket-depth=512).

## XI. Milestones restated (grounded bars)

- **Pre-M1 (small, named)**: silva.h genus/accessor export + hospes;
  expander execution-grade audit (#, __FILE__, __LINE__).
- **M0 — semantica** (silva-side). **CLOSED 2026-07-08** (M0a+M0b):
  732-file corpus, 0 crashes; haruspex 4,801/0 + auspex 3,753/0;
  1.6M expressions typed; closure 97.3%+, residuum classified.
  Demissio's input contract {naturalis, conversus} FROZEN.
- **M1 — medulla + demissio.** **CLOSED 2026-07-09** (M1a+M1b, see
  officina/phase-log.md): vocabulary ceremony-sealed
  (vocabularium-medullae.md) and HELD end-to-end; textual round-trip
  byte-identical (571/571 corpus TUs through the lector); whole
  monorepo lowers — 0 ruinae, 1,153,305 instructiones, 106,717 data,
  sistere 2,870 ALL classified (POSIX park / nested-chain park /
  vendored stb / semantica-diagnosed); fusor = the standing sweep;
  amalgama/officina.{c,h} VERIFICATUM (dual-amalgam hospes).
- **M2 — machinula + builtins, staged by waves.** Bar: Wave 0 green
  (piscina suite first — the memory-model gate), then Wave 1 green.
  v1 SHIPPED = Waves 0+1 (~80 suites), deferrals named (1b/2/3).
- **M3 — indicium + vindex stepper.** **CLOSED 2026-07-11** (six
  chunks, two days; see officina/phase-log.md M3 RELATIO): bar met
  verbatim — probatio_officina_vindex 69/69 drives breakpoint/step/
  inspect on probatio_piscina (60/60 interpreted inside it) through
  the .indicium file ONLY, with RDI-style stacked macro line entries
  (root/invocation first) from token origo. Flat instruction freeze
  (3.2x, vim 550→171ms); halt reports carry root-site [via:linea]
  everywhere (always-on lineae); vindex TUI daily-driven live
  (app on three amalgams; positions through the lector only).
  Format name SEALED: indicium (.indicium, magic "INDICIUM").
- **M4 — REPL** (+ #! runner). Bar: interactive session driving
  monorepo libs.
- **M5 — native aarch64** (compile-to-memory first, mach-o second;
  fortify/_chk and stack-guard symbols become relevant HERE, not in
  the VM). **M6 — x86-64** (Rosetta-testable).
- **v2 flagship — time-travel** (deterministic VM + recorded builtin
  results + snapshot ring; Wave-2 time/entropy stubs already point the
  way).

## XII. Open questions (updated)

1. ~~§II decisions 1-3~~ DECISUS 2026-07-08: silva.h export = pre-M1
   task; Tier-2 excluded from corpus scope; Waves 0+1 = the v1 bar.
2. Sequencing vs quaestio QB/QC/QD (carried; NOTE: the extern audit is
   done, so officina no longer waits on quaestio for scoping — the
   projects are fully independent now).
7. Testing as fifth pillar — recorded in
   officina-probatio-pillar.md (VM boundary as universal test seam:
   grammar-aware fuzzing + fuzz-to-time-travel, fault injection,
   typed-heap snapshot testing, headless GUI by world-substitution).
   Later project; pure consumer of M0–M3; no milestone changes.
8. ~~STALE CROSS-REFS after M0a~~ RESOLVED 2026-07-08 (M0b Chunk D):
   §VI rewritten (trivalent oracle, M0a/M0b split with both SHIPPED,
   97.3% resolved, corpus 732); §XI M0 bar restated with actuals.
   M0 CLOSED end-to-end — next officina milestone = M1
   medulla+demissio (see silva/phase-log.md M0b RELATIO).
3. ~~Indicium name~~ SEALED 2026-07-11: **indicium** (M3 interview).
4. Medulla op vocabulary DECISUS (at M1 design).
5. REPL semantics (at M4).
6. ~~vindex location~~ RESOLVED 2026-07-11: officina/instrumenta/
   principalia/ + vindex.sh (M3 interview; graduates only if it
   outgrows the pattern).
