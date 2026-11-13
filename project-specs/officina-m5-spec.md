# OFFICINA M5 — dorsum nativum aarch64

*Designed with Fran 2026-11-03 in secunda, right after
officina-bibliothecae landed in main (ff 68c86834). It was written from
two documents:*
- *`officina-m5-fundamenta.md`: four grounding explorers, live probes on
  this M2 / macOS 27 / clang 21;*
- *`officina-m5-interrogatio.md`: 29 decisions, Fran's choice on each.*

*Fran's direction: "evergreen modular library type stuff like missing
interpreter/arm compilation functionality". M5 is the native half of
the toolchain. Latin names below are PROPOSALS until each task's header
text is approved. Ledger: parcum 01KXJ2NV (M5 dorsum aarch64).*

## I. Intent

Officina already runs house C correctly, interpreted. The pipeline is
silva → demissio → medulla → machinula, at about 53-58M instr/s. It has
never run code natively. **M5 compiles medulla to arm64 machine code in
memory and runs it inside the same process and the same world as the
interpreter.** That means the same region, the same pointers and the
same struct layout (fundamenta facts 1-2). It is per function: a
compiled function's descriptor is flipped to NATIVUM, and every call
site, interpreted or native, then runs the native code (fact 10).

Why per function, inside the interpreter's world, rather than a
standalone compiler:
- **The interpreter stays the oracle.** "Develop in machinula, ship
  native" (officina-probatio-pillar.md). Every native function has an
  interpreted twin on identical input, in the same process.
- **No big bang.** A function goes native only when everything it calls
  is native or a builtin (C9). Everything else keeps working
  interpreted.
- **The seams were built for it.** NATIVUM was reserved in conexio.h;
  descriptors are already rebound in place after link; one function
  index space is shared by conexio, machinula and indicium.

M5 also measures, for the first time, how far native is from
interpreted. The documents disagree from 30× to 600×, and nothing has
been measured (fact 25).

## II. Goals

1. **Native leaf and call-closed functions run from machinula** through
   NATIVUM descriptors. The smoke test is
   `./officina/interpretare.sh -nativum fasti_est_bissextilis 2024`
   printing `1`.
2. **The IR states what native code needs.** medulla gains:
   - typed call sites (B4);
   - a static-slot `arca` contract (E14);
   - C semantics for float branches (E15).
   
   machinula is corrected to match, so both executors agree with C.
3. **Two new evergreen house libraries,** each with its own credo suite:
   - an A64 instruction encoder, checked against a pinned clang golden
     (G22-23);
   - an executable-memory platform layer (F18).
4. **A shared medulla analysis module** (G24): predecessors, block
   order, unreachable pruning, liveness, loops. It never reorders the
   IR.
5. **Honest halts natively.** sistere, decipula, exit, division traps
   and hardware faults come back as the same `MachinulaExitus`,
   positioned through a pc → flat-instruction table (D12, E16).
6. **A measured native/interpreted timing pair** on named workloads.
7. **A working differential baseline again.** The cursor links and runs
   every eligible suite (task 0, A3).

## III. Non-goals (M5b and later; confirmed, interrogatio H)

- **Callbacks into house code from libc** (qsort, signal, sigaction,
  atexit). Passing a function pointer to such a builtin stays an honest
  halt (C8).
- **Native → interpreted calls** (re-entrant machinula). A function that
  calls an interpreted function stays interpreted (C9).
- A cursor native lane over the house suites (A2 → M5b).
- sessio caching native code across generations. M5 recompiles per
  generation (D13).
- Mach-O output (executables or .o).
- Variadic function bodies (variadic calls ARE in M5).
- Jump tables.
- Native debug info for vindex/lldb.
- arm64e.
- Stack protectors and fortify.
- Optimisation beyond what linear scan plus straightforward instruction
  selection give. No inlining, no SSA construction.

## IV. Anti-goals (failure even if it "works")

- **Silent divergence.** A native result that differs from machinula
  with nothing flagging it. Every known divergence is either fixed in
  both (E14, E15, E16) or a named class ("machinula stricter").
- **Native code that guesses.** Argument classes, the variadic split and
  struct passing must come from the IR (B4), never from heuristics over
  register history.
- **A second calling convention** inside native code (B5: AAPCS64
  throughout).
- **Breaking PACTUM AMBULATIONIS CANONICAE.** Analyses and the backend
  never reorder, insert into or delete from the IR. Their own orders
  live beside it, with a map back.
- **Gates that need Xcode.** Encoder tests read a committed golden; only
  the re-pin script calls clang (G23).
- **x18, unsealed pages, a missing icache invalidate.** Each was proven
  fatal or flaky (facts 16, 18). The platform library makes them
  impossible to forget.
- **Platform calls (MAP_JIT, pthread_jit_*) outside the platform
  library.**

## V. Decided

These are the decisions from `officina-m5-interrogatio.md`, by number.

**Scope**
- A1 per-function behind NATIVUM.
- A2 the bar (§VI).
- A3 cursor repair first.

**Calls and ABI**
- **B4 medulla carries typed call sites:**
  - each `vocare` argument carries its type;
  - the call carries its fixed-argument count;
  - EXTERNUM symbols carry a signature from semantica.
- **B5 Apple AAPCS64 for every native function,** house↔house as well:
  - chorda (16 B) returns in x0:x1;
  - larger structs return via x8;
  - small structs go in registers; HFAs go in v registers.
- **B6 narrow values:**
  - the caller extends narrow arguments to 32 bits;
  - the callee extends narrow returns to 32 bits;
  - bits 32-63 are never trusted;
  - inside a function, the op suffix decides width.

**Function pointers**
- C7 NATIVUM = signum 4, with the entry address in `index`.
- C8 callbacks are deferred, with the boundary named.
- C9 native call graphs are closed; anything else is refused with a
  named reason.

**Runtime**
- D10 native code runs on the host thread stack.
- D11 pure builtins (~33) are called directly by address. Stateful ones
  (~28) go through the `MachinulaPons` bridge, made public.
- D12 escape through setjmp at the native entry thunk. A
  SIGSEGV/SIGBUS/SIGILL handler catches faults. `exit` stays a halt
  flag.
- D13 sessio recompiles per generation.

**Semantics**
- E14 arca = one static frame slot. machinula allocates all arcae at
  frame entry.
- E15 demissio emits `inaequalis.fT x, 0.0` before a float `ramus`.
- E16 native code emits checks for division by zero and signed
  INT_MIN / −1, with the same halt texts. Other UB goes into the named
  class "machinula stricter".
- E16b native code has no flight-recorder ring.
- E17 is a census check (§VII).

**Platform**
- F18 MAP_JIT + `pthread_jit_write_protect_np` + `sys_icache_invalidate`,
  only inside a house platform library.
- F19 one hinted reservation just below the region base, refusing on a
  mismatch. Code addresses are deterministic, and native functions call
  each other with `bl`.
- F20 a per-region host-call table (`ldr x16,[slot]; blr x16`).
- F21 frame records always.

**Encoder and tests**
- G22 the encoder is a `lib/` library.
- G23 a pinned golden plus a re-pin script through `clang -cc1as
  -show-encoding`. Exhaustive for bitmask (5,334) and FMOV (256)
  immediates. Words are compared, never disassembly text.
- G24 a shared medulla analysis module.

### Proposed components (names are proposals)

| component | proposed home | role |
|---|---|---|
| A64 encoder | `include/codificator_a64.h`, `lib/codificator_a64.c` | ~30 templates; bitmask, move-wide, bitfield, ADR and FMOV-imm algorithms; branch fixups |
| encoder golden + re-pin | `probationes/fixa/codificator_a64/aurum.tsv`, `tools/codificator_a64_oraculum.sh` | words from clang; exhaustive classes |
| executable memory | `include/memoria_exsecutabilis.h`, `lib/memoria_exsecutabilis_macos.c` | reserve(hint) / scribere-aperire / sigillare / invalidare; refuse on mismatch |
| medulla analyses | `officina/fontes/officina_medulla_fluxus.{h,c}` | preds, RPO / loop order, unreachable, liveness intervals, loops |
| backend | `officina/fontes/officina_dorsum.{h,c}` (+ allocator split if large) | instruction selection, linear scan, frames, AAPCS64 mapping, pc map, refusal reasons |
| native entry + escape | machinula side (`machinula_nativum_*`), conexio `CONEXIO_SIGNUM_NATIVUM 4` | thunk with setjmp, fault handler, MachinulaExitus mapping, public bridge |

### Proposed textual form for typed calls (B4; settled in its task)

```
%t8 = vocare.s32 $printf, i64 %t6 ... i64 %s, s32 %t7, f64 1.5
externum $printf (i64, ...) -> s32
```

Each argument is prefixed by its type. ` ... ` separates the fixed
arguments from the variadic ones; it is absent for non-variadic calls.
Variadic arguments carry their C-promoted types. The exact grammar and
the extern declaration line are decided in the task that changes the
vocabulary. That task must re-pin every `.medulla` golden and keep
`dump(parse(t)) == t`.

## VI. Acceptance (the bar)

1. **Cursor baseline (task 0).** `cursor` links every suite, with no
   `NEXUS FRACTUS (testa_principale)`, and enumerates all eligible
   suites with no cap (count printed). The captare goldens are
   re-blessed. The interpretare whole-lib fallback (parcum 01M6FG1G)
   runs.
2. **IR changes.** Typed calls round-trip through the dumper and lector.
   machinula uses argument types for builtin and extern calls, and the
   f32-immediate case is pinned by a test.
   - arca: a loop calling a struct-returning function N times leaves
     machinula's stack cursor unchanged per iteration.
   - `if (-0.0)` is false in machinula.
   - officina suite and root suite green.
3. **Encoder.** The credo suite passes over the golden: all templates,
   with range edges ±1, the 5,334 bitmask immediates and the 256 FMOV
   immediates. The re-pin script regenerates the golden byte for byte.
4. **Executable memory.** The credo suite covers:
   - write, seal, call;
   - rewrite in place without staleness;
   - refusal on a hint mismatch;
   - surviving fork.
5. **Analyses.** Predecessors, RPO and liveness match hand-computed
   expectations on the fixture CFGs (including multi-def join temps and
   unreachable `insequens` blocks). The IR stays byte-identical after
   analysis.
6. **Native fixtures.** Every `officina/probationes/fixa/machinula/
   *.medulla` that is call-graph-closed returns 42 natively as well as
   interpreted, in a new officina suite:
   - arithmetica, fluxus, comparationes, bitalia, conversiones, memoria,
     vocationes, data, sistere;
   - and `summa.medulla`.
   
   `sistere` halts natively with the same causa and position as
   machinula. A division-by-zero fixture halts with the same text.
7. **C set.** Every function in `officina/probationes/fixa/scalaria.c`
   and `fluxus.c`, plus `lib/fasti.c` `fasti_est_bissextilis`, gives the
   same result native and interpreted on a table of inputs.
   `interpretare -nativum fasti_est_bissextilis 2024` prints `1`.
   - A function that calls an interpreted function is refused with a
     named reason and runs interpreted.
8. **Faults.** A native null dereference becomes an honest VITIUM halt
   with a position, not a crash.
9. **Timing pair.** For at least three workloads, record native and
   interpreted wall time and instruction counts in the phase-log
   RELATIO:
   - a fasti loop;
   - an integer kernel;
   - a memory/struct kernel.
   
   This replaces the unsourced estimates.
10. Every gate is green at commit: officina, root, generata, Latin lint
    NOVA 0, examen ACCIPE on new C files.

## VII. Left to the builder (decide in plan or task text)

- **The task cut.** A likely order:
  0. cursor repair
  1. typed calls (+ machinula use)
  2. arca + float branch
  3. encoder library
  4. executable memory
  5. medulla analyses
  6. backend I: integer leaf (summa, arithmetica, fluxus, comparationes,
     bitalia) + NATIVUM + entry thunk + `interpretare -nativum`
  7. backend II: frames/memory/calls/data/builtins/structs (memoria,
     vocationes, data)
  8. backend III: FP + sistere + faults + division
  9. C set, timing, RELATIO
- **Register allocation.** The interval construction for multi-def
  registers, spill heuristics, and the callee- vs caller-saved split
  (80% of functions call).
- **Instruction selection.** How much folding: compare + `ramus` →
  `cmp; b.cond`; `addere` + `legere` → `[base,#k]`; immediate forms.
- **Placement constants.** The reservation's size and exact hint below
  0x300000000000, so that adrp reaches globalia.
- **The pc → flat-instruction table** format, and how the halt report
  prints native frames.
- **E17 census.** Does demissio ever feed an s32-written register into
  a 64-bit op without `extendere`? If so, add sxtw after such writes.
- **How the bridge goes public:** a header, or making `AEDIFICATA`
  queryable.
- **Final Latin names** (lint must show NOVA 0; prefer renaming over
  glossary entries).

## AUDIENDA (not verified)

- **MAP_JIT under a hardened runtime** was probed (it needs allow-jit),
  but no house binary is hardened. Nothing here re-verifies that if
  that changes.
- **The F19 hint below the region base** is assumed to be honoured, as
  0x310000000000 was (IV fact 14). The exact address is untested.
- **The 33 pure / 28 stateful builtin split** is explorer II's reading
  of the bridge code. The backend task must confirm each entry.
- **That every machinula fixture is call-graph-closed** (A6) is
  assumed. `vocationes.medulla` calls through a descriptor, which must
  resolve to a native target.
- **The cursor fix shape** (world through the bibliothecae resolver) is
  INFERRED from the interpretare fix. The cursor builds its world
  differently (a lowered `lib/*.c` cache), so task 0 may find more.
- **Timing gap expectations:** none are claimed. That is the point of
  VI.9.
