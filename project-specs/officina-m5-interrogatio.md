# OFFICINA M5 — INTERROGATIO (design interview)

2026-11-03. Fran's design decisions for M5 (native aarch64), taken
against `officina-m5-fundamenta.md`. Each decision is numbered by its
question group in fundamenta §III. Fran chose the options; any rationale
written here is Claude's summary of the trade-off. The spec is written
from this file.

## A. Scope and first deliverable

**A1. DECISUS: per-function native behind NATIVUM.**
- Chosen functions are compiled to arm64 in memory, and their descriptor
  signum is flipped to NATIVUM.
- machinula keeps running everything else and calls into native code.
- Leaf, integer-only functions come first.
- Smoke test: `interpretare -nativum fasti_est_bissextilis 2024`.
- No re-entrant machinula yet (native → interpreted calls are out of the
  first slice).

**A2. DECISUS: the M5 bar.**
- All machinula fixtures (`fixa/machinula/*.medulla`, each `$main → 42`)
  run native.
- A C-source set (scalaria.c, fluxus.c, fasti) agrees native vs
  interpreted.
- A **measured native/interpreted timing pair** (fundamenta fact 25: the
  gap has never been measured).
- A cursor native lane over the house suites becomes M5b.

**A3. DECISUS: repair the cursor before M5, as task 0.**
- Build the cursor's world through the bibliothecae/aedilis resolver
  (one variant per header). That fixes the testa_principale collision
  (parcum 01M6FZMA), and the same fix shape covers interpretare's
  whole-lib fallback (parcum 01M6FG1G).
- Drop the silent 256-suite cap (parcum 01M6FZMG).
- Re-bless the captare goldens.

## B. Calls and the ABI

**B4. DECISUS: medulla itself carries typed call sites.**
- Each `vocare` argument carries its type.
- The call records its fixed-argument count.
- EXTERNUM symbols gain a signature from semantica.
- The IR text stays self-describing, and machinula can use the same
  information (it already hit the f32-immediate bug, fundamenta fact 5).
- Cost: a sealed-vocabulary change to the textual form, with goldens
  re-pinned.

**B5. DECISUS: Apple AAPCS64 throughout,** for house↔house native calls
as well as at the boundaries.
- chorda (16 B) returns in x0:x1.
- Larger structs return via x8.
- Small structs go in registers; HFAs go in v registers.
- The backend maps medulla's `%reditus` parameter and copy-address
  arguments onto this.
- Any native function is directly callable from C and libc, and there is
  one convention to test.

**B6. DECISUS: narrow values follow clang/Apple rules.**
- The caller extends arguments narrower than 32 bits to 32.
- The callee extends narrow returns to 32.
- Bits 32-63 are never trusted.
- Inside a function, the op suffix decides width: W/X ops plus explicit
  sxt/uxt at `extendere`/`amputare`.
- Machinula's full-64-bit canonical form stays an interpreter detail.

## C. Function pointers and callbacks

**C7. DECISUS: a NATIVUM descriptor holds the code pointer.**
- `signum = NATIVUM (4)`, `index` = the function's entry address.
- An indirect native call is "load index, `blr`".
- machinula's VOCARE arm does the same through a thunk.
- A per-world native table remains for bookkeeping (sizes, pc maps), but
  calls don't go through it.
- The descriptor stays 16 bytes.

**C8. DECISUS: host callbacks are deferred to M5b, with the boundary
named.**
- Passing a function pointer to qsort, signal, sigaction or atexit stays
  an honest halt in M5.
- M5b will probably use executable descriptors (16 B = `ldr x16,#8;
  br x16; .quad target`), which means moving descriptors out of the
  read-write region.

**C9. DECISUS: native → interpreted calls are deferred; native call
graphs are closed.**
- A function compiles natively only if every callee is native or a
  builtin.
- Otherwise the compiler refuses with a named reason (e.g. `vocat $f
  interpretatum`).
- A re-entrant `machinula_vocare(f, args)` becomes its own M5b task.

## D. Runtime and memory

**D10. DECISUS: native code runs on the host thread stack,** nested under
machinula's C frames.
- Debuggers, crash reports and the frame-pointer chain work unchanged.
- `&local` in native code lies outside the region: no custodia, and no
  region snapshots of native locals.
- Overflow is a SIGSEGV, which the fault handler reports (D12).
- Revisit in M5b if the differential needs locals in the region.

**D11. DECISUS: pure builtins are called directly; stateful ones go
through the bridge.**
- About 33 pure builtins (strlen, memcpy, ctype, math) are called
  directly by address.
- About 28 stateful ones go through machinula's `MachinulaPons` bridge,
  made public:
  - malloc → the region heap
  - the printf family
  - FILE* handles
  - errno
  - exit
  - session refusals
- This keeps sessio capture, the region heap, refusals and the
  record/replay seam working for native code.

**D12. DECISUS: escape through setjmp at native entry.**
- machinula enters native code through a thunk that sets the jump point.
- sistere, decipula, exit and hardware faults (a SIGSEGV/SIGBUS/SIGILL
  handler) longjmp back to it.
- Each becomes the same `MachinulaExitus` machinula would report,
  positioned through a pc → flat-instruction table.
- `exit` stays a halt flag, as in machinula: no process exit.
- longjmp across JIT frames is proven safe (fundamenta fact 20).

**D13. DECISUS: sessio recompiles native code every generation for now.**
- The first slice is small, so the cost is negligible.
- Caching library natives across generations waits until it is measured
  to matter, and relies on address stability that is not yet verified.

## E. Semantics the IR leaves to the interpreter

**E14. DECISUS: `arca` is a static slot, and machinula is fixed to
match.**
- IR contract: each arca is one frame slot. Executing it again yields
  the same slot.
- Native code lays out frames statically.
- machinula is changed to allocate all arcae once at frame entry, which
  stops its stack growing in loops (fundamenta fact 6).
- This is a correction, tested in both.

**E15. DECISUS: branches on floats get C semantics, via demissio.**
- Demissio emits an explicit `inaequalis.fT x, 0.0` before branching on
  a float, so `ramus` only ever sees integer 0/1.
- `if (-0.0)` becomes false in machinula and native alike (375 sites in
  lib).
- It is fixed, with a test, before the native differential.

**E16. DECISUS: division traps are emitted and halt the same way.**
- Native code emits checks before sdiv/udiv (zero divisor, and signed
  INT_MIN / −1).
- It halts exactly like machinula ("divisio per zephyrum",
  "divisio: superfluxus"). Division is rare (~1,900 sites in lib).
- Other UB divergences go into a named **"machinula stricter"** class in
  the differential, not DIFFERT:
  - the stack depth cap
  - custodia
  - float→int wrap vs saturate

**E16b. DECISUS: no flight-recorder ring in native code.**
- On a halt, the escape (D12) reports the position through the
  pc → instruction table and the call stack through frame records, in
  the same halt-report format.
- The recorder stays the interpreter's ("develop in machinula, ship
  native").

**E17. CHECK, not a decision.** A fusor census over lib: does demissio
ever feed an s32-written register into a 64-bit op without `extendere`?
If it does, B6 needs sxtw after such writes. This goes into the spec as
a verification task.

## F. Platform layer

**F18. DECISUS: MAP_JIT + write-protect toggling, in a house platform
library.**
- A new library (e.g. `lib/memoria_exsecutabilis`, with a `_macos`
  variant) wraps MAP_JIT, `pthread_jit_write_protect_np` and
  `sys_icache_invalidate` behind write/seal/invalidate calls.
- It survives a future hardened runtime.
- Linux (mprotect) or iOS can add variants later.

**F19. DECISUS: one hinted reservation near regio.**
- One large MAP_JIT region at a fixed hint just below the region base.
  It refuses on a mismatch, as regio does.
- Code addresses are deterministic, so pc maps and halt reports are
  reproducible.
- Native functions call each other with `bl`.
- adrp reaches the region's globals.

**F20. DECISUS: host calls go through a per-region table.**
- A small table of host addresses lives inside the JIT region; a call is
  `adrp/ldr x16, [slot]; blr x16`.
- One place to repoint: rebinding, hot reload, or swapping a builtin for
  a bridged one. It is the same idea as descriptors.

**F21. DECISUS: frame records always,** even in leaf functions. This
gives full backtraces in lldb, crash reports and the halt report's call
stack.

## G. Encoder and tests

**G22. DECISUS: the encoder is a house `lib/` library** (Latin name to
be settled in the spec).
- It is standalone, with its own credo suite.
- The officina backend consumes it.
- Later consumers: a Mach-O writer, a disassembler for vindex, other
  JITs.

**G23. DECISUS: a pinned golden plus a re-pin script.**
- A committed golden table of expected 32-bit words, which the credo
  suite reads. Gates never need Xcode (the runae precedent).
- A re-pin script regenerates it through `clang -cc1as -triple
  arm64-apple-macosx -show-encoding [-show-inst]`.
- Exhaustive enumerations for the tricky classes: all 5,334 bitmask
  immediates and all 256 FMOV immediates.
- Compare words, never disassembly text (the assembler silently
  rewrites, fundamenta fact 23).

**G24. DECISUS: CFG analyses go in a shared medulla module** (e.g.
`officina_medulla_fluxus`).
- It covers predecessors, block order (RPO / loop-contiguous),
  unreachable pruning, liveness and loops.
- It never reorders the IR (PACTUM AMBULATIONIS CANONICAE).
- Usable by the backend now, and later by machinula, indicium, lints or
  an optimizer.

## H. Explicitly OUT of M5 (confirmed)

- Mach-O output: own-signer executables or `.o` + ld. Proven feasible;
  it comes after compile-to-memory.
- Variadic function BODIES (one in the corpus: silex.c `_monere`).
  Variadic CALLS are in M5.
- Switch jump tables (switches stay compare chains), and native debug
  info (vindex/lldb symbols over native code). The pc maps needed for
  halt reports ARE in M5.
- arm64e host, fortify/_chk and stack protectors.

## Summary of M5 as decided

Task 0 repairs the cursor baseline. Then:
- medulla gains typed call sites and fixes two semantics: arca becomes a
  static slot, and float branches get C semantics;
- a shared medulla analysis module;
- a house arm64 encoder library with a pinned clang oracle;
- a house executable-memory platform library;
- a backend that compiles call-graph-closed functions to AAPCS64 code in
  one hinted MAP_JIT region and flips their descriptors to NATIVUM;
- machinula calls into native code through a setjmp entry thunk;
- stateful builtins are bridged, pure ones called directly.

M5 is done when the machinula fixtures and a C set agree native vs
interpreted and a timing pair has been measured.

M5b: callbacks (executable descriptors), re-entrant machinula, the
cursor native lane, sessio caching.
