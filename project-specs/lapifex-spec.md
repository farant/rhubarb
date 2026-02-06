# Lapifex — C Compilation System for Rhubarb

## Overview

Lapifex ("stone-worker/mason") is an integrated C89 compilation system for rhubarb that enables writing, compiling, executing, and debugging C code from within the rhubarb environment. It treats C as a scripting language with full analysis, roundtrip fidelity, and runtime introspection capabilities.

## Architecture

Lapifex is a pipeline of modular libraries, each independently testable:

```
Pages/Files → Tangling → Lexer → Preprocessor → Parser (GLR) → Type Checker → IR → ARM64 Codegen → JIT Runtime
                                                    ↕                ↕
                                              Analysis API      Lint/Diagnostics
```

### Core Design Principles

1. **Byte-for-byte roundtrip**: Parse → serialize must reproduce identical source. This is non-negotiable and should be tested from day one.
2. **Typedefs and macros first**: These have been the stumbling block in arbor v1 and v2. Lapifex tackles them as core design concerns, not afterthoughts.
3. **Parser generator**: GLR state tables are generated from a declarative grammar spec, eliminating the combinatorial explosion of hand-written states.
4. **Test on real files early**: Test against complete C files, not just expressions/statements.
5. **Modular libraries**: Each pipeline stage is a separate library in `lib/` with its own tests.

## Components

### 1. Parser Generator (`lib/lapifex_generare.*`)

A tool that reads a grammar specification and outputs C code containing GLR state tables, action arrays, GOTO tables, and reduction handler skeletons.

**Input**: Grammar specification in a custom format (possibly STML-based)
**Output**: Generated C source files with state machine tables

**Key requirements**:
- Computes LR(1) item sets with GLR conflict resolution (multiple entries allowed)
- Grammar format supports semantic annotations for:
  - Trivia handling (how whitespace/comments attach to tokens)
  - Typedef feedback (TYPEDEF_NAME as contextual terminal)
  - Reduction actions (what AST nodes to create)
- Generated code follows arbor2's patterns (action arrays, GOTO tables, STATUS_TABULA)
- Roundtrip concerns are handled in generated code, not grammar rules

**Specifier handling**: A shared `declaration_specifiers` nonterminal accumulates type qualifiers, storage class, and type specifiers. Each context (file-scope, struct members, parameters, etc.) references this nonterminal, so adding a new specifier combination requires ONE grammar rule instead of N (one per context).

### 2. GLR Parser (`lib/lapifex_parsere.*`)

The GLR parser engine, driven by generated tables.

**Two-pass strategy**:
1. **Pass 1 — Collection**: Fast scan to collect typedef names, macro definitions, and build identifier tables. Approximate but conservative.
2. **Pass 2 — Full parse**: GLR parse with typedef/macro information available. Forks on genuine ambiguity (unknown identifiers in type positions).

**Handling incomplete information**:
- Files may be parsed without all includes available
- Unknown identifiers in type positions → heuristic classification ("probably typedef", "probably macro")
- API allows caller to provide known typedef/macro definitions
- Built-in latina.h definitions available as compiled-in data

**Macro expansion layers** (from arbor2 brainstorms):
- Layer 0: Original source
- Layer N: All macros in layer N-1 expanded
- Layers continue until no more expansions
- Tokens link back through full expansion chain

**AST requirements**:
- Every token carries trivia (leading/trailing whitespace)
- Comments are first-class nodes with attachment heuristics
- Roundtrip serialization from AST produces byte-identical output
- Error nodes for unparseable regions (always recover, never fail completely)

### 3. Type Checker (`lib/lapifex_typus.*`)

Multiple focused passes over the AST:

1. **Name resolution**: Resolve identifiers to declarations, handle scoping
2. **Type resolution**: Compute types for all expressions and declarations
3. **Constraint checking**: C89 type rules, implicit conversions, promotion rules
4. **Lint rules**: Configurable strictness layers:
   - C89 conformance (base level)
   - Dependable C rules (optional, e.g., no floats)
   - Custom lint rules (arbitrary, extensible)
   - Latin keyword linting (flag non-Latin identifiers)

Type information is attached to AST nodes as annotations, available to both the analysis API and the compilation pipeline.

### 4. Three-Address Code IR (`lib/lapifex_ir.*`)

Intermediate representation for compilation:

```
t1 = a + b
t2 = t1 * c
return t2
```

**Design**:
- Per-function IR units (each function is independently optimizable)
- IR design permits future interprocedural optimization without restructuring
- Temporaries, labels, function calls, memory operations
- Source location mapping preserved for debugging

**Optimization** (low-hanging fruit, not a priority):
- Constant folding
- Dead code elimination
- Basic common subexpression elimination
- Simple peephole optimizations on generated ARM64

### 5. ARM64 Code Generator (`lib/lapifex_codegen.*`)

Translates three-address code IR to ARM64 machine code.

**Register allocation**: Linear scan. ARM64's 31 general-purpose registers make this straightforward — most variables stay in registers.

**Calling convention**: ARM64 AAPCS (standard macOS/Linux ARM64 ABI) for interoperability with system libc.

**Output**: Machine code in memory, ready for JIT execution.

### 6. JIT Runtime (`lib/lapifex_executor.*`)

Executes compiled code in an isolated forked process.

**Execution model**:
1. Parent (rhubarb) compiles C to ARM64 machine code
2. Fork child process
3. Load compiled code into child's address space
4. Execute with debugging hooks

**Libc access**: System libc with abstraction layer providing:
- Latin-named wrappers (optional)
- Mock/sandbox capabilities (control what child can access)
- Security: deny file I/O, network, etc. by default for untrusted code
- Testing: inject faults (malloc failure simulation, etc.)

**Hot reload** (Handmade Hero-style):
- Recompile individual functions
- Patch into running child process
- Signature/type changes trigger process restart
- Developer takes responsibility for correctness (lint rules help)

### 7. Debug Bridge (`lib/lapifex_debug.*`)

Communication between parent (rhubarb) and child (compiled code):

**Three-layer approach**:
1. **Instrumentation**: Compiled code includes hooks for common operations (function entry/exit, variable writes)
2. **Shared memory**: Memory-mapped region for fast state exchange (variable values, execution status)
3. **OS debug APIs**: Mach task APIs (macOS) for breakpoints, single-stepping, crash handling

**Minimum viable debugging**:
- Set breakpoints at function boundaries
- Step through source line by line
- Inspect variable values at breakpoints
- Source-level mapping (machine code ↔ source line)

## Language Configuration

The parser is parameterized by a language configuration:

```c
/* Conceptual API */
LapifexConfig config;
lapifex_config_addere_macros(&config, latina_h_macros);
lapifex_config_addere_typedefs(&config, user_typedefs);
lapifex_config_strictness(&config, LAPIFEX_C89 | LAPIFEX_DEPENDABLE_C);
```

- **Built-in**: latina.h definitions compiled into library
- **API-provided**: Caller supplies additional macros/typedefs
- **Discovered**: Two-pass strategy discovers typedefs in parsed code
- **Extensible**: Could theoretically support arbitrary keyword remapping (C in any language), but Latin + standard C are the only shipped configurations

## Integration with Rhubarb

### Code Storage
- C code lives on rhubarb pages in XML tags
- A tangling layer extracts C fragments and assembles translation units
- Unity build: all fragments concatenated into single TU for compilation
- Filesystem projection: pages can be exported as .c files on demand

### IDE Features (page-aware)
- Syntax highlighting: requires lightweight parsing of page content
- Go-to-definition: requires full parse + name resolution
- Type info tooltips: requires type checker
- Error display: integrated in no-scrolling UI (popovers, paginated logs)

### Literate Programming
- Code interspersed with documentation on pages
- Parser receives assembled TU from tangling layer (page-unaware)
- IDE features operate on page fragments (page-aware)
- Both modes coexist: compilation uses assembled TU, editing uses fragments

## Milestone Plan

### M1: Parser Generator
- Grammar format specification
- LR(1) item set computation
- GLR table generation (allow conflicts)
- Generate tables equivalent to arbor2 (validate against same C inputs)
- STML-based grammar format parsed by existing STML library

### M2: New GLR Parser
- Parser engine driven by generated tables
- Two-pass: typedef collection → full parse
- Macro expansion (at least single-layer)
- Byte-for-byte roundtrip on all arbor2 test files
- Declaration specifiers as shared nonterminal (no combinatorial explosion)

### M3: Type Checker
- Name resolution pass
- Type resolution pass
- C89 constraint checking
- Basic lint framework (Dependable C rules)
- Type annotations on AST nodes

### M4: IR + Codegen
- Three-address code generation from typed AST
- ARM64 code generation with linear scan register allocation
- Basic optimizations (constant folding, dead code elimination)
- Can compile `int main() { return 42; }`

### M5: JIT Runtime
- Fork + execute model
- System libc linking
- Latin libc wrapper
- Basic sandbox (capability control)
- Hot reload for function bodies

### M6: Debug Bridge
- Breakpoints at function boundaries
- Step-through execution
- Variable inspection
- Source mapping display

### M7: Rhubarb Integration
- Page tangling → compilation pipeline
- Syntax highlighting in page editor
- Error display in UI
- Go-to-definition

## 6-Month Target

End-to-end demo: write C on rhubarb pages → compile → run in forked process → see output → set breakpoints → step through code.

## File Layout

```
lib/lapifex_generare.h      — Parser generator
lib/lapifex_generare.c
lib/lapifex_parsere.h       — GLR parser engine
lib/lapifex_parsere.c
lib/lapifex_typus.h         — Type checker
lib/lapifex_typus.c
lib/lapifex_ir.h            — Three-address code IR
lib/lapifex_ir.c
lib/lapifex_codegen.h       — ARM64 code generator
lib/lapifex_codegen.c
lib/lapifex_executor.h      — JIT runtime / forked execution
lib/lapifex_executor.c
lib/lapifex_debug.h         — Debug bridge
lib/lapifex_debug.c
lib/lapifex_grammatica.stml — C89 grammar specification
probationes/probatio_lapifex_*.c — Tests for each module
```

## Relationship to Arbor

Arbor2 remains as working reference code. Lapifex may:
- Reuse arbor2's lexer (arbor2_lexema) as starting point
- Reuse arbor2's token/AST data structures if compatible
- Port arbor2's test input files for validation
- Eventually replace arbor2 for all parsing needs once proven

The parser generator should be able to produce tables equivalent to arbor2's hand-written ones, validating correctness before extending the grammar.

## Key Risks

1. **Parser generator complexity**: LR(1) item set computation is well-understood but non-trivial. Budget time for getting this right.
2. **Macro expansion**: Layered expansion with arbitrary nesting is the hardest parsing problem. Test extensively with edge cases.
3. **ARM64 codegen**: First time writing a code generator. Start with simplest possible subset, expand incrementally.
4. **JIT on macOS**: Code signing, W^X enforcement, and Mach APIs have platform-specific complexity.
5. **Scope creep**: The vision is ambitious. Each milestone should produce a usable, testable artifact.
