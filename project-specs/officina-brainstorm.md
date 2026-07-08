# Officina — Compiler, VM, and Debugger for C89 (design)

Date: 2026-07-07
Status: Brainstorm / design (approved in conversation with Fran; DECISUS in §III)
Related: machina-computandi-brainstorm.md (the February vision — V.5–V.9 are this
project's ancestors), debugger.md (ptrace/mach-o decomposition notes),
silva-positio.md, silva-incrementalitas.md, silva-quaestio-design.md
Influences: TCC (Bellard), QBE (Carbonneaux), MIR (Makarov), Lua 5 VM,
RAD Debugger (Epic/RAD), Eskil Steenberg (interface-first)

---

## I. What it is, what it is not

**Officina** is the rhubarb toolchain: a C89 compiler with its own IR
(**medulla**), an interpreter/VM (**machinula**), a debugger (**vindex**),
and its own flat debug-info format. Target applications: REPLs, scripting,
live tooling, teaching — places where iteration speed beats peak
performance.

It is explicitly NOT an LLVM competitor. No optimization arms race. The
premise: once you own the front-end (silva), the parser-to-execution
distance is short if you refuse the parts of compiler engineering that
exist only to win benchmarks.

The positioning insight (silva-positio.md, extended): clang gives correct
trees but eats the surface; tree-sitter gives surface but approximate
semantics; NOBODY gives you compile-and-run-your-own-dialect with a debug
format you own. Officina completes the loop silva opened: the monorepo
becomes self-hosting in the read direction (silva reads it losslessly) and
now in the run direction (machinula executes it).

## II. What already exists

- **silva** — the front-end. All 724 monorepo files parse with ZERO error
  nodes, preprocessor included, byte-exact surface, committed trees with
  pater threading, oracle-resolved ambiguities. Historically the front-end
  is 60% of a C compiler's pain; it is done.
- **quaestio** — the query engine (QA core shipped). Officina's first
  concrete task is a quaestio consumer (§IX, the extern audit).
- **lapifex expander** — preprocessor integration precedent.
- **debugger.md** — Fran's earlier decomposition notes (ptrace, mach-o,
  sectio/symbola/dwarf split). Those become relevant at M5+ (native);
  recorded lore: macOS ptrace is deliberately crippled — real debuggers
  use task_for_pid + mach_vm_read/write and require the debugger binary
  to be codesigned with the com.apple.security.cs.debugger entitlement.
  The VM-first path defers ALL of this ceremony.
- **machina-computandi-brainstorm.md** — the February vision. Its V.5
  ("C89 parser producing AST") is complete via silva, better than the doc
  imagined. Officina is the standalone realization of V.5–V.9; the
  entity-graph integration remains a later CONSUMER of officina, not a
  constraint on it (DECISUS 1).
- Hardware fact: this machine is an Apple M2 (arm64). x86-64 output can
  still be tested here via Rosetta 2; the honest first native ISA is
  aarch64.

## III. DECISUS (Fran, 2026-07-07)

1. **Silva-fed standalone toolchain.** Classic pipeline: silva trees →
   semantica → medulla → machinula/native. Entity-graph vision stays a
   later consumer. (Chosen over: machina-computandi-integrated; fresh
   minimal front-end.)
2. **The corpus bar is the v1 acceptance bar.** Machinula interprets the
   existing libraries and passes their credo suites UNMODIFIED. The
   monorepo is the test corpus, as byte-exactness was for silva. REPL and
   scripting fall out as byproducts. (Chosen over: scripting-harness-first;
   native-demo-first.)
3. **Interpret everything + hand-bridged builtins.** Every monorepo .c
   lowers to medulla and runs in the VM; only true externals (malloc,
   printf, fopen, memcpy…) are builtins bridged to the host. No general
   FFI trampolines in v1. (Chosen over: dlsym+trampoline FFI; precompiled
   native libs.)
4. **Format-first stepper debugger.** Vindex v1 = breakpoints, step,
   stack/locals inspection on the VM, driven ONLY through the debug-format
   file, never through VM internals — the format is battle-tested before
   native codegen exists. TUI rides tessera/saltuarius patterns.
   Time-travel is the NAMED v2 flagship, not v1. (Chosen over:
   time-travel-first; defer-debugger.)
5. **Register-based linear IR** (medulla): basic blocks, virtual
   registers, explicit typed load/store. QBE/MIR/Lua-VM lineage. One IR
   serves both the interpreter and the later linear-scan native backend;
   variables map to named slots so debug info is natural. (Chosen over:
   stack bytecode — would need a second IR for native; SSA-first —
   machinery serving an optimization war we declined. SSA remains
   available later as an internal pass, as QBE does.)
6. **VM memory = one VM-owned mmap'd region, real pointers.** Pointers
   in interpreted code ARE host pointers (builtin bridge is trivial;
   chorda/piscina pointer tricks just work), but every allocation —
   globals, interpreted stack, malloc arena — provably lives inside a
   region the VM owns. This preserves determinism, snapshotting, pointer
   provenance checks, and the time-travel v2. Fixed base address per
   session for replay stability.
7. **Names**: officina (toolchain umbrella), medulla (the IR), machinula
   (the VM), vindex (the debugger). Debug format name still open — see
   §XI (floated: **indicium**, the evidence vindex argues from).
8. **M0 lives silva-side.** The semantic layer is silva's already-named
   typus+index milestone; officina consumes it. Silva remains the single
   home of C89 semantics.

## IV. Architecture

```
fons .c ──silva──▶ arbor commissa ──semantica──▶ arbor typis ornata
                                                      │
                                                  demissio (lowering)
                                                      │
                                          medulla (IR registrorum) ──▶ indicium?
                                                      │              (debug sectiones)
                                       ┌──────────────┼──────────────┐
                                       ▼              ▼              ▼
                                machinula (v1)   aarch64 (M5)    x86-64 (M6)
```

Five libraries, independently testable (house discipline):

1. **semantica** (silva-side, = typus+index) — symbol resolution, type
   checking, layout (size/align/offset) over committed trees. Oracle
   trick baked in from day one: GENERATE a C file of sizeof/offsetof
   asserts from our computed layouts, compile it with clang, run it —
   the host compiler audits our layout math across all 724 files, cheap
   and brutal, forever.
2. **medulla** — IR data structures (functio / blocks / typed
   instructions), builder API, and a TEXTUAL form with a parser
   (round-trip: parse → dump → byte-identical). Golden tests assert on
   dumps; hand-written .medulla text files test the VM without the
   front-end. The textual form is a thousand-year surface — name ops in
   Latin, design them like genus tags (permanent vocabulary).
3. **demissio** — typed trees → medulla. The one home where C89
   semantics become explicit and auditable: integer promotions become
   visible conversion ops, pointer arithmetic gets scaled by the layout
   tables, && and || become control flow, struct assignment becomes a
   copy op, short-circuit/comma/ternary linearize.
4. **machinula** — the region (mmap, fixed base), call frames of virtual
   registers, opcode-switch dispatch loop, the builtins bridge. Traps
   carry medulla position → source position via debug info, so even v1
   runtime errors are source-mapped. Debug builds can bounds-classify
   pointers against the region ("membrana" checks) — a valgrind-flavored
   bonus on a platform where valgrind is dead.
5. **vindex + format** — flat mmap-able sections in the RAD spirit
   (DWARF is a size-optimized state machine; a debugger wants flat
   random-access tables): linea table (medulla index ↔ fons/linea),
   functio table (name, IR range, frame layout), locals with live
   ranges, typus table (from semantica). Emitted during demissio. The
   stepper drives machinula only through a small debug API keyed by
   format contents (breakpoint at medulla index, step, read register /
   region memory).

Memory discipline: one piscina per compilation (silva precedent); the VM
region is session-lifetime and separate. Because we own the compiler we
NEVER write a DWARF parser; DWARF-reading only matters if vindex someday
debugs clang-built binaries (non-goal for now).

## V. Medulla sketch (shape, not final vocabulary)

```
functio summa (i32 n) -> i32
initium:
    s = 0
    i = 1
    salire proba
proba:
    r1 = comparare_maius.i32 i, n
    ramus r1 -> finis | corpus
corpus:
    s = addere.i32 s, i
    i = addere.i32 i, 1
    salire proba
finis:
    redde s
```

- Types on ops, not on registers: i8/i16/i32/i64/f32/f64 (+ signed/
  unsigned variants where semantics differ). Registers hold 64-bit words.
- Locals whose address is taken (and all aggregates) live in explicit
  frame memory; access is load/store with byte offsets from semantica's
  layout tables. Everything else is virtual registers.
- Calls: IR-level vocare with argument registers; struct-by-value via
  copy ops; varargs get an officina-defined representation in the VM
  (each native ABI later defines its own at codegen time).
- Op vocabulary is a naming ceremony item — same standing as genus tags.

## VI. Milestones and bars

No milestone closes without its bar (house rule).

- **M0 — semantica.** Bar: symbols + types + layout for all 724 files;
  clang-assert oracle green on every computed sizeof/offsetof.
  Biggest unknown of the whole map (declarators, promotions, qualifier
  rules) — but we start from committed structure, not text. Side payoff:
  saltuarius type-aware jumps.
- **M1 — medulla + demissio.** Bar: whole monorepo lowers; golden IR
  dumps pinned; textual round-trip byte-identical. Side payoff: an
  analyzable IR for tooling.
- **M2 — machinula + builtins.** Bar: THE CORPUS BAR — existing credo
  suites pass, interpreted, unmodified. This is "v1 shipped": C89-as-
  scripting exists in fact.
- **M3 — indicium + vindex stepper.** Bar: set a breakpoint in a real
  credo test, step, inspect locals and a chorda's bytes — all through
  the format file.
- **M4 — REPL.** Bar: interactive session compiling declarations/
  statements incrementally into a live VM + a #!-style runner. (The
  silva incrementality doc cashes in here.)
- **M5 — native aarch64.** Compile-to-memory first (TCC-style, no
  linker; composes with the REPL as a JIT), mach-o emission second
  (debugger.md's macho/sectio/symbola decomposition activates). Linear
  scan over the same medulla. Native vindex = mach APIs + entitlement.
- **M6 — x86-64.** Testable on this M2 via Rosetta.
- **v2 flagship — time-travel.** Deterministic machinula + recorded
  builtin results + snapshot ring = reverse-step. Named now, built
  after v1.

## VII. Testing

- Per-library credo suites, as always.
- Golden medulla dumps for lowering fixtures.
- Hand-written .medulla files exercising machinula without the front-end.
- The corpus bar as the standing regression wall — every library's test
  suite doubles as a compiler test.
- Differential spot-checks: same test run native (clang) and interpreted;
  outputs must match byte-for-byte.
- Later: the libFuzzer harness (silva roadmap item #4) extends to the
  medulla text parser and machinula.

## VIII. Honest sizing

Silva-scale arc, not saltuarius-scale. M0 and M2 are each multi-week in
the way silva milestones were. Compensations: the front-end is done; the
test corpus pre-exists; every milestone ships something independently
useful, so there is no long dark tunnel.

## IX. First concrete task (before any INTENTIO)

**The extern audit**: a quaestio query over the monorepo enumerating
every called symbol not defined in the monorepo = the exact builtin
surface machinula must bridge. Also audit for semantic dragons (§X).
Dogfooding: the parser's query engine scopes the compiler's runtime.

## X. Known dragons (to be sized by the audit, handled in demissio/machinula design)

- setjmp/longjmp (does credo or anything else use it?)
- varargs functions defined IN the monorepo (not just printf-callers)
- function pointers (definitely used: resolutor callbacks, vtables) —
  VM code pointers must be real-pointer-compatible values in the region
- unions with type punning (SilvaValor and friends)
- bitfields (usage unknown — audit)
- float/double exactness in differential tests (FP determinism)
- recursion depth / interpreted stack sizing (silva's own recursion!)
- const-qualified data placement (write traps or not, v1 = not)

## XI. Open questions

1. **Sequencing** vs the quaestio QB/QC/QD arc. Note: the extern audit
   (§IX) is a quaestio consumer and may itself pull QB attribute
   selectors — the projects interlock rather than compete.
2. **Debug format name** — floated: indicium (the evidence vindex argues
   from). Fran's ceremony.
3. Medulla op vocabulary — naming ceremony, same standing as genus tags,
   to be DECISUS'd during M1 design.
4. REPL statement semantics (top-level expressions? redefinition rules?)
   — decide at M4, not before.
5. Whether machinula's builtin printf bridges per-directive to host
   snprintf or reimplements formatting (leaning: per-directive bridge).
