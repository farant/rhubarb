# OFFICINA M5 — FUNDAMENTA (grounding synthesis)

2026-11-03. This synthesizes four parallel grounding explorers, run before
the M5 interview. M5 is the native aarch64 backend (spec-v2 §XI: "compile-
to-memory first, mach-o second"). The explorers were:

- I. **medulla as a compilation source.** Real IR came from the built
  fusor: all of lib/ was dumped and counted.
- II. **The runtime native code must interoperate with:** machinula,
  regio, conexio, layout, and entry points. A probe ran through the
  existing colloquium binary and through clang.
- III. **Apple arm64 platform facts.** About 30 live probes covered
  MAP_JIT, the ABI, Mach-O and debugging, including a hand-written
  ad-hoc code signer.
- IV. **Checking our own encoder, and the house testing strategy.** A hand
  C89 encoder was checked against clang, and the bitmask-immediate
  encoder was verified exhaustively.

Everything here was measured on Apple M2, macOS 27.0.1, clang 21, at
secunda 68c86834 (== main). Full reports and probe sources are listed in §V.

Cite shorthand:
- `MH`/`MC` = officina/fontes/officina_medulla.{h,c}
- `DC` = officina_demissio.c
- `XC` = officina_machinula.c
- `conexio.h/.c`, `regio.c` = officina/fontes/officina_*
- `sem.c` = silva/fontes/silva_c89_semantica.c
- `VOC` = officina/vocabularium-medullae.md
- **PROVEN** = probe run here; **DOCUMENTED** = source cited;
  **INFERRED** = reasoning only.

## I. Load-bearing facts

### Memory and layout: already native-shaped

**1. Interpreted pointers ARE host addresses. Native code can share the
world with no translation (PROVEN).**
- The region is one 64 GB read-write mmap at `0x300000000000`. The base is
  a hint, not MAP_FIXED, and the region refuses to start on a mismatch
  (regio.c:24-27, 83-94). Its three areas:
  - globalia at +0 (bump allocator: data, literals, descriptors, extern
    cells)
  - stiva at +8 GB (machinula frames: the register file plus arcae)
  - acervus at +12 GB (LIFO first-fit heap)
- Every machinula load and store is a raw memcpy through the pointer value
  (XC 2848-2961).
- Probe (II §E), measured from inside interpreted code:
  - `&global` = 0x300000000000
  - a literal = …010
  - a function pointer = …020 (the descriptor)
  - `&local` = 0x3002…
  - `malloc` = 0x3003…
  - `stdout` = 0x1

**2. Struct layout is already Apple arm64 LP64 (PROVEN). Layout is
semantica's job, not indicium's.**
- `silva_c89_formam_computare` is documented "arm64 LP64 Apple"
  (semantica.h:13-15, sem.c:9682-9790).
- char is signed (DC:294); long is 8 bytes; long double = double.
- `struct {char; long; short; double; int}` gives size 40 with offsets
  8/16/24/32, identical under the interpreter and clang.
- **The one hole is bitfields.** They are parked upstream
  (`EXAMEN_CODEX_CAMPI_PARCATI`), so such structs never lower.
- indicium is the M3 debug format: line tables plus a variable → vreg map.

### The IR: the op set maps cleanly; calls and frames do not

**3. medulla is not SSA.** It has basic blocks, untyped 64-bit virtual
registers, and types carried on the ops (MH:14-15).
- Named locals keep their names and are reassigned.
- `&&`, `||` and `?:` join by writing one temporary in several blocks
  (DC 1848-1897). There are no phis.
- In lib, 6.6% of registers have more than one def.
- Successors are explicit as BLOCCUS operands on the terminator.
- **Nothing computes predecessors, liveness, dominators or loops over
  medulla.** The only dataflow machinery in the tree works on the AST
  (silva_c89_fluxus_datorum).

**4. 35 ops in total. About 28 are one aarch64 instruction or a short
fixed sequence** (I §B, with lib counts). The lib mix is:
movere 17.8%, addere 13.1%, ramus 10.3%, legere 8.9%, vocare 8.6%,
salire 8.5%, arca 5.3%, copia 4.1%.
- There is no switch op (compare chains), no select, no varargs op and no
  intrinsics.
- Width semantics are native-friendly: canonicalize-on-write was hardened
  in M2c to also canonicalize on READ for width-sensitive ops (XC
  2336-2353). The **op suffix alone decides width**, and that maps onto W
  and X registers plus explicit sxt/uxt.
- Same-size sign changes emit nothing. Every width change is an explicit
  `extendere`/`amputare` (DC 834-837).

**5. Call sites are untyped, and externs have no signature. This is the
central IR gap.**
- `vocare` carries only its return type. The arguments are bare operands
  (DC 2553-2637; demissio.worklog "Variadic call args carry NO per-arg
  type tag").
- An EXTERNUM symbol is `{titulus, genus, index}`: no parameter types, no
  variadic flag, no fixed-argument count (MH:212-224).
- **Demissio HAS the callee prototype at the call site and drops it**
  (DC 2486-2499).
- Apple arm64 needs both pieces of information:
  - the class (x vs v register) of every argument;
  - the fixed/variadic split, because **every variadic argument goes on
    the stack in 8-byte slots** (fact 13).
- Machinula has already paid for this gap once: the f32-immediate argument
  bug (XC ~3075-3110).
- The register class per vreg is inferable from the defining ops. In lib,
  0 vregs have both int and float defs, but the IR doesn't guarantee it.

**6. `arca` is an executable bump in the interpreter, not a frame slot.**
- 27,333 of the 40,178 arcae in lib sit outside the entry block. Most are
  16-byte `chorda` return slots at call sites, some inside loops
  (DC 2539-2600).
- Machinula bumps `stiva_cursor` each time one executes and frees only at
  `redde` (XC 2889-2922). So **the interpreter's stack grows on every loop
  iteration that calls a struct-returning function**.
- All arca sizes and alignments are immediates, so a native frame CAN be
  laid out statically.

**7. Aggregate conventions are medulla's own, not AAPCS64.**
- By-value struct arguments are passed as the address of a caller copy.
- Struct returns use an explicit hidden FIRST parameter `%reditus`
  (convention C7: DC 4434-4443; VOC §IV says "native ABIs map it at M5").
- AAPCS64 instead uses x0:x1 for aggregates of 16 bytes or less (chorda
  is 16 B and is everywhere: 24,626 of 31,061 copiae are 16 B), x8 for
  larger returns, and v registers for HFAs.

**8. Block order is creation order, and the IR must not be reordered.**
- Loop exits are created before loop bodies. Unreachable `@insequens_N`
  blocks exist (DC 599-607).
- PACTUM AMBULATIONIS CANONICAE (MH:168-181): the flat instruction index
  is shared by demissio, machinula's frozen copy and indicium's line
  tables. "Whoever reorders … breaks the pact."
- That index is a free instruction numbering for linear scan. But the
  backend needs its own block order (RPO / loop-contiguous) **outside**
  the IR, plus a map back.

**9. Size is modest.** Per function: median 33 instructions, 23 vregs and
8 blocks; p99 is 490 instructions and 315 vregs; the maximum is 1,580
instructions, 1,273 vregs and 429 blocks.
- lib/ alone is 408 TUs, 11,508 functions and 764,081 instructions.
- 80% of functions make calls, so callee-saved vs caller-saved allocation
  matters.
- 199 calls pass more than 8 arguments (max 18), so stack arguments are
  needed.
- Lowering is cheap (demissio 1.1 s for all of lib); parsing dominates
  (34 s).

### The runtime seam: one good seam, four real blockers

**10. Every call, direct or indirect, goes through a 16-byte descriptor
`{signum, index}`** (conexio.h:46-63; XC 3030-3073).
- Signa: 1 INTERPRETATUM, 2 DECIPULA, 3 AEDIFICATUM. The header says
  "M5 addet NATIVUM".
- Descriptors are already rewritten **in place** after link
  (decipula → aedificatum, XC 1996-2021).
- So flipping one function's signum to NATIVUM re-routes **every** call
  site to native code.
- Function identity is one index space shared by conexio, machinula
  `planae[]` and indicium.

**11. Least-disruptive attach point: `machinula_creare`.** All four
execution entries go through it (II fact 26):
- interpretare
- cursor
- sessio/colloquium
- vindex

legatus never runs code. The freeze pass there (XC 2023-2145) is where a
"compile chosen functions, flip their signum" pass would sit.

**12. The four blockers are not memory:**
- **(a) the IR call gap** (fact 5);
- **(b) machinula is not re-entrant.** `machinula_aperire` resets the
  frames, the stack cursor and the halt state, and a depth-1 `redde` ends
  the run (XC 2384-2392, 3008-3013). Interpreted→native calls are easy
  (a signum arm plus a thunk). **Native→interpreted calls need an API
  that doesn't exist.**
- **(c) state the bridges own.** Of the 61 builtins (`AEDIFICATA`, XC
  1861-1925), about 33 are pure and directly callable by address
  (string/mem/ctype/math). About 28 must stay bridged:
  - malloc → the region heap
  - the printf family → machinula's own formatter (`_formare`), going
    through the handle table
  - FILE* → handles (0/1/2 for the std streams)
  - errno → a region cell
  - exit → a halt flag with no unwinding
  - session refusals (RECUSATIO)
  - lectiones notes
- **(d) descriptors are data,** so libc cannot call them. lib passes
  function pointers to qsort ×22 and signal/sigaction ×21. qsort was
  deliberately never made a builtin.

There are no native house libraries today. Everything outside those 61
builtins is interpreted ("link the world").

**13. Semantics that live only in the interpreter, where hardware
differs:**
- **Integer division by zero, and `INT64_MIN / -1`:** machinula halts
  honestly (XC 2555-2570). arm64 `sdiv` silently gives 0 or INT_MIN
  (PROVEN, IV fact 17).
- **A pointer below 4096:** machinula halts. Native code gets SIGSEGV.
  The 4 GB __PAGEZERO is actually a wider net.
- **Stack:** an explicit check plus a 1,048,576-frame recursion cap. A
  native 8 MB thread stack overflows much sooner.
- **float→int on out-of-range (UB) inputs:** machinula wraps;
  `fcvtzs`/`fcvtzu` saturate.
- **`ramus` on a float register tests raw bits** (XC 2974-2990). So
  `if (-0.0)` takes the TRUE branch, while C and clang say false. That
  affects 375 sites in lib. **This is a latent machinula/C divergence, a
  bug on machinula's side.**
- **Zero-filled register file:** an uninitialized read gives 0, so the
  allocator must tolerate a use with no def.
- **The always-on flight recorder** (a 4096-entry ring) is free in the
  interpreter. Natively it would cost something on every instruction.

**14. Capture differs by entry point.**
- sessio captures through bridge handles (tmpfiles via
  `machinula_ansam_ponere`; sessio.c:1352-1369). Direct libc output from
  native code would ESCAPE it.
- cursor uses a real `freopen` on stdout in the forked child
  (cursor.c:946-964). Direct libc output is captured there.

**15. Variadic bodies: the corpus has one,** `lib/silex.c` `_monere`.
- It uses the placeholder `va_list = char*`, `va_start = &last` from
  systema_c89.h:36-53 ("machinula va_* non tangit").
- It calls `vfprintf`, which isn't bridged.
- By coincidence Apple's real va_list IS a `char*` over 8-byte stack
  slots, but `&last` is not how Apple's va_start works.

### Platform: compile-to-memory works today, no entitlement (PROVEN)

**16. The MAP_JIT recipe works in a plain clang-built house binary.**
Steps: mmap(RWX, MAP_JIT) → `pthread_jit_write_protect_np(0)` → write →
`(1)` → `sys_icache_invalidate` → call. It returned 42 (III F1; IV fact
12).
- **Every step is load-bearing** (III F2):
  - without MAP_JIT the mmap fails with EACCES;
  - without the write toggle the store raises SIGBUS;
  - without toggling back, execution raises SIGBUS;
  - without the invalidate, code rewritten in place ran stale on 199,980
    of 200,000 calls.
- The toggle is **per thread** (F3).
- Cost: about 58 ns per toggle pair. icache invalidation is about 143 ns
  for 64 B and 2.7 µs for 16 KiB (F8).
- **MAP_JIT survives fork(), so the cursor's fork-per-suite model carries
  over.** It honours an address hint (0x310000000000 twice) but refuses
  MAP_FIXED (IV 13-14).
- A 16 GiB MAP_JIT reservation mapped fine (F4).
- **Hardened runtime:** MAP_JIT then needs `cs.allow-jit`. mprotect
  RW→RX works unhardened, but under hardening the mprotect *succeeds* and
  the process is SIGKILLed when it first executes (F6). So MAP_JIT is the
  durable path.

**17. Host calls must be indirect.** libSystem is 2.3-35 GiB from the JIT
region, while BL reaches ±128 MiB and ADRP ±4 GiB with no ASLR guarantee
(III F10, IV 15).
- movz/movk into x16 then `blr x16` works for puts and printf (F9).
- **libSystem → JIT callbacks work:** a qsort comparator and a
  `pthread_create` start routine both ran (F11).
- **PAC is a non-issue while the host is plain arm64,** which is clang's
  default and what ~/.bin is. An arm64e host would need
  `ptrauth_sign_unauthenticated` on every JIT entry pointer (F11).

**18. x18 is reserved: the kernel zeroes it** (0x1234 read back as 0 after
a usleep; F12). Never allocate it.

**19. The Apple ABI deltas, each proven by clang disassembly or live**
(III §2 table):
- variadic arguments are all on the stack in 8-byte slots (live: register
  variadics made printf print garbage);
- the **caller** extends arguments narrower than 32 bits; bits 32-63 of
  an int argument are undefined;
- the callee extends narrow returns to 32 bits (clang's behaviour; not in
  Apple's text);
- stack arguments are packed at natural size;
- x29 must always address a valid frame record (leaf functions are
  exempt). Skipping it loses `backtrace()` frames (F17);
- the red zone is 128 B;
- char is signed;
- long double = double;
- sret goes in x8; HFAs go in v registers.

**20. Debugging:**
- Valid frame records are enough for lldb `bt`, `backtrace()` and .ips
  crash reports to show the frames around JIT code. JIT frames appear as
  bare addresses (F28).
- longjmp across JIT frames is safe; nothing in the C89 house needs unwind
  info (F29).
- **A SIGSEGV/SIGBUS/SIGILL handler can catch a fault inside JIT code,
  read pc/fp/lr and siglongjmp out.** The region stays usable afterwards
  (F30). That keeps "halitus honestus" possible natively, through a
  pc → IR-position table.
- Machinula itself uses no setjmp; only credo does.

**21. Mach-O, for "second":**
- **An unsigned binary is SIGKILLed** (F5).
- **We can ad-hoc sign it ourselves.** A ~70-line signer does: SuperBlob,
  then CodeDirectory v0x20400, SHA-256 over **4 KiB** pages, exec-segment
  fields. `codesign -v` accepts the result (F20). The house already has
  SHA-256 (sigillum.h / friatio.h).
- A minimal runnable executable needs **8 load commands**:
  - `__PAGEZERO`, `__TEXT`, `__LINKEDIT`
  - `LC_LOAD_DYLINKER`
  - **`LC_UUID`** (mandatory as of dyld 27)
  - `LC_MAIN`
  - `LC_LOAD_DYLIB` libSystem
  - `LC_CODE_SIGNATURE`
- Imports work through **classic indirect-symbol binding**, with no
  chained fixups (F22-23).
- **Static (LC_UNIXTHREAD) executables are refused,** even when signed
  (F24). PIE is effectively mandatory (F25).
- **Overwriting a binary in place gets it SIGKILLed on the next run**
  (the vnode caches the signature): write a new inode (F21).
- The .o route needs only `LC_SEGMENT_64` + `LC_SYMTAB`, with relocations
  BRANCH26 / PAGE21 / PAGEOFF12 / GOT_LOAD_* / UNSIGNED (F26).
- The house macho/sectio/symbola libraries **read only**. They don't
  expose relocations or signatures (F27), and `sectio_invenire` cannot
  find sections in a `.o` (parcum 01M6FZMQ).

### Encoder and tests: the encoder is small; the oracle is free

**22. The encoder side of M5 is small.**
- About 30 instruction templates cover a C89 backend (IV §B).
- Only five need real logic:
  - bitmask immediates
  - choosing a movz/movk/movn sequence
  - bitfield-alias math
  - the ADR immediate split
  - the inverse of VFPExpandImm (FMOV immediates)
- **A ~20-line bitmask encoder matched clang on all 5,334 valid 64-bit
  patterns in one 0.1 s batch.** A hand C89 encoder matched on a
  9-instruction round trip (IV 9-10).
- There is no arm64 code anywhere in the house. aemulator is the terminal
  emulator, not a CPU emulator.

**23. The oracle is clang itself:**
`clang -cc1as -triple arm64-apple-macosx -show-encoding [-show-inst]
-filetype asm` prints one word per line plus the variant name, 5,334
instructions in 0.08 s. llvm-mc isn't installed; this replaces it.
- The house Mach-O reader matched otool word for word, so clang is the
  only outside tool.
- **The assembler silently rewrites some instructions** (`ldr #4` → LDUR,
  `#4096` → the shifted form). Tests must compare **words**, never
  disassembly text (IV 2-8).
- House precedent for "oracle at pin time, golden at test time": runae
  (`tools/runae_oraculum.sh` → `aurum_icu.txt`, which the credo suite
  reads).

**24. The differential corpus exists, but the baseline lane is broken
right now:**
- The cursor runs house suites interpreted, fork per suite, with stdout
  byte-compared against 67 pinned clang-native goldens (captare.sh). The
  last recorded result was 79 passed of 93, stdout 72/0/0 (2026-07-24).
- **It is red for every suite:** `testa_principale` is defined in both
  lib/testa.c and lib/testa_android.c, and the cursor links all of lib/
  (parcum 01M6FZMA).
- **It silently stops at 256 of the 450 eligible suites,** in
  `-enumerare` and in its main loop (parcum 01M6FZMG).
- A native lane needs both fixed first.
- The machinula fixtures (`officina/probationes/fixa/machinula/
  *.medulla`, each `$main → 42`) are a ready-made first native target
  list that compares against the same input (IV §E).

**25. The native/interpreted gap is UNMEASURED.**
- Measured interpreter speed: about 53M instr/s at -O0 after the M3 flat
  freeze, about 58M with -celer, about 65M in the 07-10 experiment.
- The "150×" figure is in no document. The estimates conflict:
  - 30-60× against a polished interpreter (worklog);
  - 100-200× (phase-log 901);
  - about 300-600× implied by lapifex numbers (II fact 28).
- No native wall-clock pair exists. **M5's first deliverable should
  produce one.**

## II. Existing surface inventory (what M5 consumes/extends)

| piece | where | M5 use |
|---|---|---|
| region, fixed base, 3 areas | regio.c | shared memory; native heap = acervus? native stack = stiva? |
| semantica layout (LP64 Apple) | sem.c:9682-9790 | structs need no work (bitfields parked) |
| medulla ops/blocks/flat index | MH, MC | source IR; flat index = numbering; own block order |
| demissio prototype at call | DC 2486-2499 | where typed call info would be recorded |
| descriptor + in-place rebind | conexio.h:46-63; XC 1996-2021 | NATIVUM signum; per-function re-route |
| freeze pass | XC 2023-2145 | native compile pass attach point |
| AEDIFICATA (61) + `_aedificatum_vocare` | XC 1861-1942 (`interior`) | bridged host calls; needs a public face |
| canonical width rules | XC 159-190, 2336-2353 | suffix-authoritative = native W/X + sxt/uxt |
| halt kinds + report | machinula.h:44-67; XC 362-422 | native trap/escape must yield the same MachinulaExitus |
| fusor `-textus` | officina/build/fusor | real IR dumps for tests and census |
| machinula fixtures (→42) | officina/probationes/fixa/machinula/ | first native targets |
| cursor + captare goldens | cursor.c; captare.sh; fixa/stdout_nativa | native differential lane (after repair) |
| macho/sectio/symbola reader | lib/macho.c etc. | oracle back end; Mach-O self-check later |
| sha256 | sigillum.h / friatio.h | ad-hoc signer (Mach-O phase) |

## III. Open questions → the interview

Merged from the explorers' ~47 questions, grouped by what each decides.
My leaning is marked → where the facts point clearly; Fran decides.

**A. Scope and first deliverable**
1. **Shape of the first slice.** Choose between:
   - per-function mixed mode: flip a NATIVUM descriptor, keep the
     interpreter for everything else;
   - whole-program native: compile all of main's closure.

   Interpreted→native per function is nearly free structurally.
   Native→interpreted needs a re-entrant machinula (fact 12b).
   → A first slice of **leaf, integer-only functions behind NATIVUM,
   called from machinula**, plus `interpretare -nativum` as the smoke
   test. That measures the gap (fact 25) and avoids re-entry.
2. **What "done" means for M5:** the machinula fixtures plus a C-source
   set (scalaria.c, fluxus.c, fasti) agreeing native vs interpreted? A
   cursor native lane over the house suites? A timing pair?
3. **Repair the cursor first** (the testa_principale collision, the
   256-suite cap)? It is the differential baseline (fact 24).
   → Yes, as a pre-M5 task. The same variant-collision class as
   interpretare's whole-lib fallback (parcum 01M6FG1G), so one fix shape
   covers both: build the world through the bibliothecae/aedilis resolver.

**B. Calls and the ABI (the central design)**
4. **Typed call sites:** where do per-argument classes and the fixed count
   come from?
   - extend medulla: `vocare` argument types and/or signatures on EXTERNUM
     symbols (a sealed-vocabulary ceremony; textual form changes);
   - a non-textual side table from demissio;
   - backend recovery: dataflow for classes, semantica for prototypes.

   Apple variadics make the fixed count unavoidable (facts 5, 19).
5. **House↔house native convention:** keep medulla's (hidden `%reditus`,
   copy-address struct arguments) or use AAPCS64 throughout? Host
   boundaries (libc, callbacks, any later clang-linked .o) need AAPCS64
   regardless (fact 7).
6. **Narrow values:** copy clang (the callee extends returns, the caller
   extends arguments to 32 bits, never trust bits 32+) or keep machinula's
   full 64-bit canonical form in X registers? (facts 4, 19)

**C. Function pointers and callbacks**
7. **NATIVUM payload:** a code pointer in `index`, or an index into a
   native table parallel to `planae[]`? Can the descriptor grow? (fact 10)
8. **Host callbacks** (qsort, signal, sigaction, atexit): in M5 scope?
   They need executable function-pointer values. Options:
   - descriptors move to an executable mapping (16 bytes is exactly
     `ldr x16,#8; br x16; .quad target`);
   - per-function thunks;
   - keep "no callback builtins" as the named boundary (fact 12d).
9. **Native→interpreted calls:** build a re-entrant `machinula_vocare(f,
   args)` in M5, or defer and restrict native to leaf / native-closed call
   graphs? (fact 12b)

**D. Runtime and memory**
10. **Native stack:** the host thread stack (simple; overflow is SIGSEGV;
    `&local` outside the region) or SP switched into regio stiva (keeps
    every address in the region, determinism, snapshots)? (facts 1, 13)
11. **Host calls:** pure builtins called directly, stateful ones through
    the `MachinulaPons` bridge? That keeps capture, refusals, region malloc,
    errno and the v2 record/replay seam (facts 12c, 14). It needs
    AEDIFICATA made public. → Bridge the stateful ones; it keeps sessio
    capture and the trace differential.
12. **exit / sistere / decipula escape from native frames:** setjmp at a
    native-entry trampoline, with longjmp proven safe (fact 20)? Does
    `exit` really exit under cursor/interpretare?
13. **sessio:** rebuild native code every turn, or cache library modules'
    native code across generations? Caching relies on stable sedes, which
    is unverified (II Q8).

**E. Semantics the IR leaves to the interpreter**
14. **`arca` contract:** declare it a static frame slot (the same slot on
    every execution) in the IR contract? That also stops machinula's stack
    growing in loops (fact 6). → Yes. It is a correction to machinula, not
    just a native need.
15. **`ramus` on floats:** C semantics (`-0.0` is false) or raw bits? Have
    demissio emit an explicit compare against 0.0? → C semantics. Today's
    behaviour is a latent machinula bug (fact 13). Fix it in
    demissio/machinula before the differential bar, with a test.
16. **Trap policy natively:** emit div-by-zero and INT_MIN/-1 checks
    (honest halts) or adopt arm64 behaviour? Install a fault handler with a
    pc → position table to keep "halitus honestus" (fact 20)? Keep the
    flight recorder per instruction, per call, or not at all? And name
    the differential class "**machinula stricter**" (UB caught by the
    interpreter) as a legitimate divergence, not a DIFFERT.
17. **Canonical register form:** interpreter-only, with native code using
    W/X plus explicit extends? Census check: does demissio ever feed an
    s32-written register into a 64-bit op without `extendere`?
    (IV 29, I 14)

**F. Platform layer**
18. **The W^X API behind a house platform layer** (`memoria_exsecutabilis_*`
    or similar, so Linux or iOS can follow): `pthread_jit_write_protect_np`
    (simple, per thread) or the callback-allowlist API (Apple's
    recommendation)? MAP_JIT or mprotect? (fact 16) → MAP_JIT plus
    write_protect_np, wrapped.
19. **Code placement:** one large hinted MAP_JIT reservation next to regio
    (deterministic code addresses, `bl` between JIT functions,
    ADRP-reachable globals) or anywhere (movz/movk absolute addresses or a
    literal pool)? (facts 16-17)
20. **Host-call shape:** movz/movk inline, or a re-pointable per-region
    table (`ldr x16,[table]; blr x16`) to support rebinding and hot reload?
21. **Frame records:** always (full backtraces; simplest) or non-leaf only
    (matches clang)? (fact 19)

**G. Encoder and tests**
22. **Encoder home:** a house `lib/` arm64 encoder (reusable: a Mach-O
    writer, a disassembler for vindex, future JITs) or officina-internal
    fontes? → `lib/`, by the "independent composable library" rule.
23. **Oracle:** a pinned golden TSV the credo suite reads (no Xcode in
    gates; runae precedent) plus a re-pin script through `clang -cc1as`
    with exhaustive enumerations for the tricky classes? Commit the full
    exhaustive tables (~6k rows) or regenerate them only in the audit
    lane? (facts 22-23)
24. **Allocator scope:** where do CFG analyses (predecessors, RPO,
    unreachable pruning, liveness, loops) live? A shared `medulla_fluxus`
    library usable by machinula, indicium and tools, or private to the
    backend? (facts 3, 8)

**H. Explicitly later (confirm deferral)**
25. **Mach-O route:**
    - write executables directly, with our own signer and classic binding
      (proven feasible);
    - emit `.o` and use `ld`.
26. **Variadic bodies** (one in the corpus).
27. **Switch jump tables:** recover them from compare chains in the
    backend, or add a medulla op.
28. **Native debug info / vindex over native code:** pc maps plus location
    ranges, or lldb's GDB JIT interface.
29. **arm64e host**, and **fortify/_chk and stack guards** (irrelevant to
    compile-to-memory; officina lowers from silva's systema headers with
    base names).

## IV. Corrections to the pre-exploration framing

- **"Medulla was designed to serve the backend"** is true of the op
  vocabulary and only partly true of calls and frames. The real M5 seams
  are untyped calls, signature-less externs, the non-AAPCS aggregate
  convention, and executable arcae. The arithmetic is not one of them.
- **"The native gap is ~150×"** (said in conversation 11-02) has no
  source. The documents disagree, from 30× to 600×, and nothing is
  measured (fact 25).
- **indicium is not type layout.** semantica owns layout, and it is
  already Apple LP64.
- **There are no native house libraries.** Only 61 libc builtins run on
  the host. The "printf bridge" is machinula's own formatter.
- **Descriptors re-route direct calls too,** not only function-pointer
  calls. That makes the NATIVUM seam stronger than "a seam for function
  pointers".
- **"Zero variadic bodies in the corpus"** (VOC §IV) is stale: silex.c
  `_monere`.
- **The cursor is not a working baseline today** (fact 24).
- **The encoder is not the hard part.** The design weight is in register
  allocation, the call ABI, host calls, and the interpreted↔native
  boundary.

## V. Explorer artifacts

All four are in the session scratchpad,
`/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/134c8417-d34b-49a2-b8c8-3d3b81913875/scratchpad/`:

| report | evidence |
|---|---|
| `m5_explorator_I.md` | lib-wide IR census `m5_I/lib.medulla`, `stat.py`, `lib.stat.txt`; dumps via `./officina/build/fusor <dir> -textus <substr>` |
| `m5_explorator_II.md` | probe `m5_II/probe.c` (interpreted via colloquium vs clang) |
| `m5_explorator_III.md` | probes `m5_III/` (`jit1-9.c`, `mprot.c`, `abi.c/abi.s`, `signer.c`, `lcdrop.c`, …), Apple doc text `m5_III/dl/` |
| `m5_explorator_IV.md` | `m5_IV/` (`enc.c`, `bm.c` exhaustive bitmask, `cls.s` + `cls_dis.txt` encoding seed, `jitfork.c`, `div.c`, `lege_textum.c` house-reader oracle) |

Ledger parks filed from the pass:
- 01M6FZMA: cursor link broken by testa_principale;
- 01M6FZMG: cursor 256-suite cap;
- 01M6FZMQ: sectio_invenire on MH_OBJECT.

The M5 ledger entry is parcum 01KXJ2NV.
