# Lapifex — C Compilation System for Rhubarb (v2: Implementation Spec)

## Overview

Lapifex ("stone-worker/mason") is an integrated C89 compilation system for rhubarb that enables writing, compiling, executing, and debugging C code from within the rhubarb environment. It treats C as a scripting language with full analysis, roundtrip fidelity, and runtime introspection capabilities.

**6-month target**: End-to-end demo — write C on rhubarb pages, compile, run in forked process, see output, set breakpoints, step through code.

## Architecture

```
Pages/Files → Tangling → Lexer → Preprocessor → Parser (GLR) → Type Checker → IR → ARM64 Codegen → JIT Runtime
                                                    ↕                ↕
                                              Analysis API      Lint/Diagnostics
```

### Core Design Principles

1. **Byte-for-byte roundtrip**: Parse → serialize must reproduce identical source. Tested from day one.
2. **Typedefs and macros first**: The stumbling block in arbor v1 and v2. Core design concern, not afterthought.
3. **Parser generator**: GLR state tables generated from declarative grammar spec, eliminating combinatorial explosion.
4. **Test on real files early**: Complete C files, not just isolated expressions/statements.
5. **Modular libraries**: Each pipeline stage is a separate `lib/lapifex_*.c` with its own `probationes/probatio_lapifex_*.c`.

---

## Component 1: Parser Generator (`lib/lapifex_generare.h/.c`)

### Purpose

Reads a grammar specification in STML format, computes LR(1) item sets with GLR conflict resolution, and outputs C source code containing state tables, action arrays, GOTO tables, and reduction handler skeletons.

### Why

arbor2's hand-written state tables (`arbor2_glr_tabula.c`, 27,627 LOC) suffer from combinatorial explosion — each specifier combo × each parsing context requires separate states and productions. A generator makes the grammar the single source of truth.

### Grammar Format

STML-based, parsed by existing `stml.h` library (4,255 LOC, mature, full DOM + query API):

```xml
<grammatica>
  <non_terminalis nomen="declaration_specifiers">
    <!-- Shared specifier accumulation — used by ALL declaration contexts -->
    <productio nomen="P_DS_1">
      <symbola>type_specifier</symbola>
      <actio>creare_specifiers</actio>
    </productio>
    <productio nomen="P_DS_2">
      <symbola>type_qualifier type_specifier</symbola>
      <actio>creare_specifiers_cum_qualificante</actio>
    </productio>
    <productio nomen="P_DS_3">
      <symbola>type_qualifier type_modifier type_specifier</symbola>
      <actio>creare_specifiers_cum_qualificante_et_modificante</actio>
    </productio>
    <!-- One rule here serves ALL contexts (file-scope, struct, params, etc.) -->
  </non_terminalis>

  <non_terminalis nomen="struct_member_list">
    <productio nomen="P_SML_1">
      <symbola>declaration_specifiers declarator SEMICOLON</symbola>
      <actio>creare_membrum_primum</actio>
    </productio>
    <productio nomen="P_SML_2">
      <symbola>struct_member_list declaration_specifiers declarator SEMICOLON</symbola>
      <actio>addere_membrum</actio>
    </productio>
  </non_terminalis>
</grammatica>
```

### Output

Generated C files following arbor2's patterns:
- `lapifex_tabula_generata.c` — state action arrays, GOTO tables, STATUS_TABULA, production definitions
- `lapifex_actiones_generata.c` — reduction handler skeletons (can be hand-edited for semantic actions)

### Implementation

The generator itself is a C89 program using:
- `stml.h` — parse the grammar STML file
- `xar.h` — collect item sets, productions, states
- `tabula_dispersa.h` — LR(1) item set deduplication
- `piscina.h` — all allocations

**Algorithm**:
1. Parse grammar STML → extract terminals, non-terminals, productions
2. Compute FIRST/FOLLOW sets
3. Build LR(1) item sets (canonical LR(1) — not LALR, to preserve full information)
4. Construct ACTION and GOTO tables; allow multiple entries per cell (GLR)
5. Emit C source code with static arrays matching arbor2's `Arbor2TabulaActio` / `Arbor2StatusInfo` / `Arbor2StatusGoto` structures

### Key Insight: Shared `declaration_specifiers`

The grammar factors specifier accumulation into a shared nonterminal. This means:
- Adding `const unsigned long long` needs ONE production in `declaration_specifiers`
- Every context (file-scope, struct members, function params, local decls, casts, sizeof) automatically supports it via their reference to `declaration_specifiers`
- Eliminates the P348-P504 explosion (currently ~150 productions for specifier combos)

### Validation

Generate tables equivalent to arbor2's hand-written ones, then test against the same 73 roundtrip files in `probationes/fixa/roundtrip/`. If the generated parser passes all 57 roundtrip tests and 2176 GLR tests, the generator is correct.

---

## Component 2: GLR Parser (`lib/lapifex_parsere.h/.c`)

### Relationship to arbor2

The GLR parse engine (`arbor2_glr.c`, 9,449 LOC) is **mostly grammar-agnostic** — the core loop, GSS management, fork/merge, and checkpoint/rollback logic doesn't depend on C specifically. Lapifex will:

1. **Reuse the GLR engine logic** — same graph-structured stack, same fork/merge, same `PiscinaNotatio` checkpointing
2. **Plug in generated state tables** — from the parser generator instead of hand-written
3. **Use generated reduction handlers** — with semantic actions that build lapifex AST nodes

### Two-Pass Strategy

**Pass 1 — Collection** (fast, approximate):
- Scan for `typedef` declarations, `#define` macros
- Build identifier classification table: `{identifier → TYPEDEF_NAME | MACRO_NAME | UNKNOWN}`
- Uses same lexer but simpler parsing (pattern matching, not full GLR)
- API-provided definitions (from `LapifexConfig`) added to table before pass 1

**Pass 2 — Full GLR Parse**:
- Typedef/macro table available as context
- `TYPEDEF_NAME` classified identifiers get different token type than regular `IDENTIFICATOR`
- GLR forks on genuinely ambiguous identifiers (unknown typedefs from missing includes)
- Heuristic classification for unresolved identifiers in type positions

### Lexer

Reuse `arbor2_lexema.c` (mature, handles all C89 tokens + whitespace/comment tokens + provenance chain via `arbor2_token.h`). The lexer already:
- Recognizes 32 C89 keywords
- Handles explicit whitespace tokens (SPATIA, TABULAE, CONTINUATIO)
- Tracks byte_offset, line, column for every token
- Stores leading/trailing trivia (whitespace, comments) on each token

### Macro Expansion Layers

From arbor2 brainstorms, already partially implemented in `arbor2_token.h`:
- `Arbor2OrigoMeta` tracks provenance: FONS (source), EXPANSIO (macro), PASTA (##), CHORDA (#), API
- `Arbor2Token` chains link expanded tokens back to originals
- Layer model: Layer 0 = source, Layer N = all macros in Layer N-1 expanded

### AST Nodes

Start with arbor2's `Arbor2Nodus` structure (tagged union with 40+ node types). Key features already present:
- `specifiers_ordine` (Xar of tokens in source order) for roundtrip preservation
- `commenta_ante` / `commenta_post` for comment attachment
- `spatia_ante` / `spatia_post` on tokens for whitespace preservation
- Error nodes with `nuntius` (message) and `lexemata_saltata` (skipped tokens)

Extend with type checker annotations (added as new union members or sidecar data).

### Language Configuration

```c
typedef struct {
    Xar* macro_definitiones;    /* Xar of LapifexMacroDef */
    Xar* typedef_nomina;        /* Xar of chorda* — known typedef names */
    u32 strictness;             /* LAPIFEX_C89 | LAPIFEX_DEPENDABLE_C | ... */
    b32 latina_integrata;       /* Include latina.h definitions automatically */
} LapifexConfig;

/* Built-in latina.h definitions compiled into library */
extern const LapifexMacroDef LAPIFEX_LATINA_MACROS[];
extern const i32 LAPIFEX_LATINA_NUMERUS;
```

---

## Component 3: Type Checker (`lib/lapifex_typus.h/.c`)

### Multiple Focused Passes

1. **Name resolution**: Walk AST, resolve identifiers to declarations. Build symbol table per scope. Uses `tabula_dispersa.h` for scope lookup.
2. **Type resolution**: Compute types for all expressions. Handle implicit conversions, integer promotions, usual arithmetic conversions.
3. **Constraint checking**: C89 constraint violations. Configurable strictness:
   - C89 conformance (base)
   - Dependable C (optional — no floats, no implicit conversions, etc.)
   - Custom lint rules (extensible)
   - Latin keyword linting (flag non-Latin identifiers)

### Type Representation

```c
typedef enum {
    LAPIFEX_TYPUS_VOID,
    LAPIFEX_TYPUS_CHAR, LAPIFEX_TYPUS_SCHAR, LAPIFEX_TYPUS_UCHAR,
    LAPIFEX_TYPUS_SHORT, LAPIFEX_TYPUS_USHORT,
    LAPIFEX_TYPUS_INT, LAPIFEX_TYPUS_UINT,
    LAPIFEX_TYPUS_LONG, LAPIFEX_TYPUS_ULONG,
    LAPIFEX_TYPUS_LONGLONG, LAPIFEX_TYPUS_ULONGLONG,
    LAPIFEX_TYPUS_FLOAT, LAPIFEX_TYPUS_DOUBLE,
    LAPIFEX_TYPUS_POINTER,
    LAPIFEX_TYPUS_ARRAY,
    LAPIFEX_TYPUS_FUNCTION,
    LAPIFEX_TYPUS_STRUCT, LAPIFEX_TYPUS_UNION, LAPIFEX_TYPUS_ENUM,
    LAPIFEX_TYPUS_TYPEDEF_NOMEN
} LapifexTypusGenus;

typedef struct LapifexTypus {
    LapifexTypusGenus genus;
    u32 qualifiers;             /* CONST | VOLATILE */
    union {
        struct { struct LapifexTypus* basis; } pointer;
        struct { struct LapifexTypus* elementum; i32 mensura; } array;
        struct { struct LapifexTypus* reditus; Xar* parametra; b32 variadic; } functio;
        struct { chorda* nomen; Xar* membra; i32 magnitude; } structura;
        struct { chorda* nomen; Xar* enumeratores; } enumeratio;
    } datum;
} LapifexTypus;
```

### Annotations

Type info attached to AST nodes as sidecar data (not modifying arbor2's Arbor2Nodus):

```c
typedef struct {
    LapifexTypus* typus;        /* Resolved type */
    b32 est_lvalue;             /* Is this an lvalue? */
    b32 est_constans;           /* Compile-time constant? */
    i64 valor_constans;         /* If constant, its value */
} LapifexTypusAnnotatio;

/* Lookup: node pointer → type annotation */
/* Uses tabula_dispersa keyed on node address */
```

---

## Component 4: Three-Address Code IR (`lib/lapifex_ir.h/.c`)

### Representation

```c
typedef enum {
    LAPIFEX_IR_ASSIGN,          /* t1 = t2 */
    LAPIFEX_IR_BINARIUM,        /* t1 = t2 op t3 */
    LAPIFEX_IR_UNARIUM,         /* t1 = op t2 */
    LAPIFEX_IR_LOAD,            /* t1 = *t2 */
    LAPIFEX_IR_STORE,           /* *t1 = t2 */
    LAPIFEX_IR_ADDR,            /* t1 = &t2 */
    LAPIFEX_IR_LABEL,           /* L1: */
    LAPIFEX_IR_JUMP,            /* goto L1 */
    LAPIFEX_IR_COND_JUMP,       /* if t1 goto L1 */
    LAPIFEX_IR_CALL,            /* t1 = call f(t2, t3, ...) */
    LAPIFEX_IR_RETURN,          /* return t1 */
    LAPIFEX_IR_PARAM,           /* param t1 (push argument) */
    LAPIFEX_IR_MEMBER,          /* t1 = t2.field (struct member) */
    LAPIFEX_IR_INDEX,           /* t1 = t2[t3] */
    LAPIFEX_IR_CAST,            /* t1 = (type)t2 */
    LAPIFEX_IR_CONST_INT,       /* t1 = 42 */
    LAPIFEX_IR_CONST_STR,       /* t1 = "hello" */
    LAPIFEX_IR_ALLOCA           /* t1 = alloca(size) — local variable */
} LapifexIRGenus;

typedef struct {
    LapifexIRGenus genus;
    i32 dest;                   /* Destination temporary (-1 if none) */
    i32 src1, src2;             /* Source temporaries */
    i32 label;                  /* Label number (for jumps) */
    LapifexTypus* typus;        /* Type of result */
    chorda* nomen;              /* For named entities (functions, globals) */
    i32 linea_fons;             /* Source line for debug mapping */
} LapifexIRInstructio;

typedef struct {
    chorda* nomen;              /* Function name */
    LapifexTypus* typus_functi; /* Function type */
    Xar* instructiones;         /* Xar of LapifexIRInstructio */
    i32 num_temporaria;         /* Total temporaries allocated */
    i32 num_labels;             /* Total labels allocated */
} LapifexIRFunctio;
```

### Per-function IR

Each function is an independent IR unit. Source location preserved on every instruction for debugger mapping.

### Basic Optimizations (low priority)

- Constant folding (evaluate `2 + 3` at compile time)
- Dead code elimination (unreachable after `return`)
- Copy propagation (`t1 = t2; use t1` → `use t2`)

---

## Component 5: ARM64 Code Generator (`lib/lapifex_codegen.h/.c`)

### Target

ARM64 (AArch64) only. macOS Apple Silicon and future Raspberry Pi.

### Calling Convention

ARM64 AAPCS: x0-x7 for arguments, x0 for return value, x8 for indirect return, x29 frame pointer, x30 link register, sp stack pointer.

### Register Allocation

Linear scan. With 31 general-purpose registers (x0-x30), most local variables stay in registers. Spill to stack when needed.

### Output

Machine code bytes in a `Xar` buffer, ready to be loaded into a forked process's address space.

### First Target

`int main() { return 42; }` → ARM64 that returns 42 when executed.

---

## Component 6: JIT Runtime (`lib/lapifex_executor.h/.c`)

### Execution Model

1. Parent (rhubarb) compiles C to ARM64 machine code in memory
2. `fork()` child process
3. Map compiled code into child's address space (mmap with PROT_READ | PROT_EXEC)
4. Execute with debugging hooks
5. Parent monitors child via Mach task APIs

### macOS Specifics

- Code signing: JIT requires `com.apple.security.cs.allow-jit` entitlement or `MAP_JIT` flag
- W^X: ARM64 macOS enforces write-xor-execute; use `pthread_jit_write_protect_np()` to toggle

### Libc Access

System libc with abstraction layer:
- Latin-named wrappers (optional, via lapifex config)
- Sandbox/capability control (deny/allow file I/O, network, etc.)
- Mock injection (simulate malloc failure, trace allocations)

### Hot Reload (Handmade Hero-style)

- Recompile individual functions
- Patch function pointers in child process via shared memory
- Signature/type changes → restart process
- Lint rules to help developers write reload-friendly code

---

## Component 7: Debug Bridge (`lib/lapifex_debug.h/.c`)

### Three-Layer Approach

1. **Instrumentation**: Generated code includes function entry/exit hooks, optional variable-write callbacks
2. **Shared memory**: Memory-mapped region between parent and child for:
   - Execution status (running, paused, crashed)
   - Variable value inspection
   - Breakpoint hit notification
3. **OS debug APIs**: Mach task APIs (`task_for_pid`, `thread_get_state`, `thread_set_state`) for:
   - Hardware breakpoints
   - Single-stepping
   - Register/memory reads after crash

### Minimum Viable

- Set breakpoints at function entry
- Step through source line by line
- Read local variable values at breakpoints
- Source location display (line ↔ machine code mapping from IR)

---

## Integration with Rhubarb GUI

### Code Storage

- C code lives on rhubarb pages (no-scrolling, paginated UI)
- Code fragments in XML tags, spread across pages
- Pages are the "source of truth"
- Filesystem projection: pages exportable as .c files

### Tangling

A layer above the compiler extracts C fragments from pages, assembles them into translation units. For compilation, this produces a **unity build** (all code in single TU, sidesteps linking).

### IDE Features

Built on the parser + type checker:
- **Syntax highlighting**: lightweight lexer pass on page content (page-aware)
- **Go-to-definition**: full parse + name resolution (TU-aware)
- **Type tooltips**: type checker annotations
- **Error display**: integrated in no-scrolling UI (popovers, paginated error logs)

---

## Milestone Plan

### M1: Parser Generator (`lib/lapifex_generare.*`)
- Define STML grammar format
- Parse grammar → extract terminals, non-terminals, productions
- Compute FIRST/FOLLOW sets
- Build LR(1) item sets
- Generate C code with ACTION/GOTO tables
- **Validate**: generate tables equivalent to arbor2, pass same test inputs
- **Tests**: `probationes/probatio_lapifex_generare.c`

### M2: New GLR Parser (`lib/lapifex_parsere.*`)
- GLR engine driven by generated tables (adapt arbor2_glr.c core loop)
- Two-pass: typedef collection → full parse
- `declaration_specifiers` as shared nonterminal (no combinatorial explosion)
- Macro expansion (at least single-layer)
- Byte-for-byte roundtrip on all 73 roundtrip files from `probationes/fixa/roundtrip/`
- **Tests**: `probationes/probatio_lapifex_parsere.c`, reuse arbor2 test input C files

### M3: Type Checker (`lib/lapifex_typus.*`)
- Symbol table with scoping
- Name resolution pass
- Type resolution pass
- C89 constraint checking
- Basic lint framework (Dependable C subset)
- Type annotations on AST nodes
- **Tests**: `probationes/probatio_lapifex_typus.c`

### M4: IR + Codegen (`lib/lapifex_ir.*`, `lib/lapifex_codegen.*`)
- Three-address code generation from typed AST
- ARM64 code generation with linear scan register allocation
- Basic optimizations (constant folding, dead code elimination)
- Can compile `int main() { return 42; }`
- **Tests**: `probationes/probatio_lapifex_ir.c`, `probationes/probatio_lapifex_codegen.c`

### M5: JIT Runtime (`lib/lapifex_executor.*`)
- Fork + execute model
- mmap with MAP_JIT for code loading
- System libc linking
- Latin libc wrapper
- Basic sandbox (capability control)
- Hot reload for function bodies
- **Tests**: `probationes/probatio_lapifex_executor.c`

### M6: Debug Bridge (`lib/lapifex_debug.*`)
- Instrumentation hooks in generated code
- Shared memory for state exchange
- Mach task APIs for breakpoints
- Step-through execution
- Variable inspection
- Source mapping display
- **Tests**: `probationes/probatio_lapifex_debug.c`

### M7: Rhubarb Integration
- Page tangling → compilation pipeline
- Syntax highlighting in page editor
- Error display in UI
- Go-to-definition
- (Depends on rhubarb GUI state — probatio_combinado entry point)

---

## Existing Infrastructure to Reuse

| Library | Location | Use in Lapifex |
|---------|----------|---------------|
| `piscina.h` | include/ | Arena allocation for all phases. Checkpoint/rollback for GLR branches. |
| `xar.h` | include/ | Token streams, AST children, IR instruction lists, state table arrays. No-reallocation guarantee critical for GSS. |
| `chorda.h` | include/ | All string data. NOT null-terminated — use chorda API only. |
| `internamentum.h` | include/ | Deduplicate identifiers, keywords, type names. All lexer output is interned. |
| `tabula_dispersa.h` | include/ | Symbol tables, typedef lookup, macro definitions, type annotation sidecar. |
| `stml.h` | include/ | Parse grammar specification files. Mature: 4,255 LOC, full DOM + query + serialization. |
| `credo.h` | include/ | Test framework. Pattern: `credo_aperire()`, `CREDO_AEQUALIS_*()`, `credo_imprimere_compendium()`. |
| `arbor2_lexema.*` | include/lib/ | C89 lexer with 32 keywords, 50+ operators, whitespace/comment tokens, provenance. Reuse directly. |
| `arbor2_token.*` | include/lib/ | Token provenance chain for macro expansion layers. Reuse directly. |
| `arbor2_glr.c` | lib/ | GLR engine core logic (GSS, fork/merge, checkpoint). Adapt/extract, don't rewrite. |

---

## Naming Conventions

Following rhubarb patterns:
- Functions: `lapifex_verbum_nomen(...)` (Latin)
- Enums: `LapifexNodusGenus` (PascalCase with Lapifex prefix)
- Structs: `LapifexTypus` (PascalCase)
- Macros: `LAPIFEX_*` (all caps)
- File names: `lib/lapifex_modulis.c`, `include/lapifex_modulis.h`
- Test files: `probationes/probatio_lapifex_modulis.c`
- Grammar: `lib/lapifex_grammatica.stml`
- Generated: `lib/lapifex_tabula_generata.c`

---

## Key Risks and Mitigations

| Risk | Mitigation |
|------|-----------|
| Parser generator complexity (LR(1) item sets) | Well-understood theory. Start with subset grammar, validate incrementally. |
| Macro expansion edge cases | Port arbor v1 and v2 test cases. Focus on macros early. |
| ARM64 codegen (first time) | Start with minimal subset (return constant). Extend one construct at a time. |
| JIT on macOS (code signing, W^X) | Research MAP_JIT and pthread_jit_write_protect_np early. Build spike test. |
| Scope creep | Each milestone produces testable artifact. Don't move to next until current passes. |
| Typedef/macro ambiguity | Two-pass collection + GLR forking. Test with missing-include scenarios. |

---

## Open Questions

1. **Grammar format details**: Exact STML schema for productions, semantic actions, precedence/associativity
2. **AST reuse vs new**: Extend Arbor2Nodus or define new LapifexNodus? Tradeoffs in compatibility vs clean design.
3. **Preprocessor scope**: How much of the layered expansion model to implement for M2 vs defer to later?
4. **Error recovery strategy**: arbor2's approach (error nodes with skipped tokens) or something different?
5. **Build system**: How to integrate generator into compile_tests.sh flow (generate → compile → test)?
