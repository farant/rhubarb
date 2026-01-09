# Arbor v2 - Robust C89 Parser Specification

## Overview

Arbor v2 is a parallel experimental parser designed to handle the full complexity of C preprocessing and parsing with maximum robustness. Unlike typical parsers that fail on missing information, arbor-v2 uses GLR parsing with layered macro expansion to gracefully handle incomplete macro/typedef definitions while still producing useful ASTs.

**Status:** Parallel experiment alongside current arbor
**Shared Infrastructure:** piscina, chorda, xar, internamentum, etc.
**Target:** Full C89 grammar, flag C99 constructs, annotate Dependable C violations

## Core Architecture

### Dual Tree Model

1. **Source Tree** - Grammar-agnostic, preserves original tokens/macros/directives
   - Used for serialization/roundtrip
   - Tokens with trivia (whitespace, line info)
   - Comments as first-class peer nodes
   - Preprocessor directives as interleaved siblings

2. **Semantic AST** - Grammar-aware, queryable
   - Derived from source tree
   - Token slots with provenance links
   - Lossy but analyzable

### Expansion Layers

Macro expansion produces discrete layers:
- **Layer 0:** Original source text/tokens
- **Layer 1:** All macros in layer 0 expanded once
- **Layer 2:** All macros in layer 1 expanded once
- **Layer N:** Continue until no more expansions (fixpoint)

**Key properties:**
- Strict layer semantics: layer N+1 only expands what existed in layer N
- Each layer is independently tokenizable with trivia
- Tokens in expanded layers link back to source via full provenance chain
- Preprocessor directives (#define, #include) persist in layer 0; conditional directives (#if) only in layer 0

### Processing Pipeline

```
Source File
    │
    ▼
┌─────────────────────┐
│ Macro Collection    │ ─── Builds macro lookup table
└─────────────────────┘
    │
    ▼
┌─────────────────────┐
│ Iterative Expansion │ ─── Generates layers 0..N
└─────────────────────┘
    │
    ▼
┌─────────────────────┐
│ Typedef Collection  │ ─── Discovers typedefs across layers
└─────────────────────┘
    │
    ▼
┌─────────────────────┐
│ GLR Parsing         │ ─── Parses each layer with macro lookahead
└─────────────────────┘
    │
    ▼
┌─────────────────────┐
│ Index Building      │ ─── Identifier table built during parse
└─────────────────────┘
```

## GLR Parser

### Overview
Hand-written GLR parser in C89 with:
- Graph-structured stack (GSS) - implementation approach TBD via research
- Fork on ambiguity (e.g., `foo * bar`)
- Merge when paths converge
- Macro lookahead to guide branch decisions

### Macro Lookahead
When encountering potential ambiguity:
1. Check macro lookup table: "Is this identifier a known macro?"
2. If yes, peek at expansion tokens to inform parsing decisions
3. For function-like macros, perform expansion with current argument tokens
4. Use expansion info to prioritize/prune GLR branches

### Ambiguous Node Types

1. **Error Node**
   - Contains skipped tokens from recovery
   - Best-effort classification of what failed
   - Failed alternatives available in "editor mode" only

2. **"Probably Typedef" Node**
   - Identifier in type position without known typedef
   - Heuristic: position is valid for type, no arguments

3. **"Probably Macro" Node**
   - Identifier with arguments in non-function-call position
   - Or: identifier where syntax is incomplete and macro could fill gap

### Error Recovery
- Always recover at statement/declaration boundaries (`;`, `}`)
- Insert error node containing skipped tokens
- Continue parsing subsequent code

### Fork Lifecycle
- Forks created on ambiguity
- Invalid forks pruned when determined impossible
- No immediate memory cleanup (arena model)
- Memory reclaimed when arena is freed

## Token & Trivia Model

### Tokens
- Standard C tokens with position info
- Link to expansion provenance (full chain for nested macros)
- Stored in single arena

### Trivia
- Whitespace between tokens
- Line/column change tracking
- Attached as leading/trailing on tokens

### Comments as Nodes
Comments are first-class AST nodes (not trivia) because they need to be queryable.

**Comment attributes:**
- Position type: own-line, end-of-line, inline, start-of-line
- Comment style: block comment, special block (`/* === ... === */`)
- Multiline flag

**Attachment heuristics:**
- End-of-line comment → attach to preceding node
- Comment followed by code (no blank line) → attach to following node
- Comment with blank lines above and below → freestanding

**AST relationship:**
- Comments are peer siblings to declarations/statements
- If comment falls between tokens of a node, it's a child with position marker

## AST Structure

### Token Slots
AST nodes contain token slots that:
- Reference actual tokens (any layer)
- Preserve trivia for roundtrip
- Enable transformation without losing formatting

### Transformations
1. Modify AST (e.g., rename function)
2. Token slot updated with new token
3. Trivia preserved on token slot
4. Serialize from token slots → roundtrip preserves formatting

### Node Deletion Trivia Handling
When deleting a node:
- Keep leading trivia of first token
- Keep trailing trivia of last token
- Option to create "floating trivia" or attach to adjacent nodes

## Conditional Compilation

### All-Branches Parsing
Parse all conditional branches (`#if`, `#ifdef`, `#elif`, `#else`):
- Track which conditions enable which code paths
- Query API allows filtering by condition configuration
- Enables cross-path analysis (e.g., find function uses in all configurations)

### Condition Tracking
- Each conditional node tracks its enabling condition
- Coupled conditions (same `#ifdef FOO`) are linked
- Conditions affecting typedef/macro definitions flagged for incremental impact

### Representation
Treat conditional blocks similar to macro expansions:
- Condition is like a "macro name"
- Block content is the "expansion"
- Links back to `#if` directive in layer 0

## Identifier Index

Built during parse, contains:
- All identifiers in file
- Array of pointers to tokens/AST nodes using each identifier
- Position context (type-position, expression, etc.)

**Use cases:**
- Incremental re-parse: "Does new macro X affect any identifier in file Y?"
- Query: "Find all uses of identifier Z"
- Ambiguity resolution: "Is Z used as a type elsewhere?"

## Project/File Handling

### Flexible Scope
Parser supports:
- Single file analysis
- File with includes (translation unit)
- Full project with cross-file resolution
- Snippets with injected context

### Configuration
- Include paths (folders/files to search)
- Injected macro definitions
- Injected typedef definitions
- Standard library definition profiles

### Unresolved Includes
When include file not found:
- Mark as unresolved
- Continue parsing with available information
- May result in "probably macro/typedef" nodes

### Standard Library Support
- Compiled-in default definitions for common symbols
- Platform profiles (macOS/x64, Linux/arm, etc.)
- API allows passing additional definitions at parse time

### Header Hints
- Header dependency graph for standard library
- Usage-based suggestions: "FILE used but <stdio.h> not included"

## Output Modes

### Presets
- **Analysis:** Compact, best-guess interpretation, minimal metadata
- **Editor:** Verbose, all GLR alternatives preserved, full trivia
- **Transform:** Token slots emphasized, trivia handling explicit

### Custom Flags
Individual toggles:
- `keep_alternatives`: Preserve failed GLR branches
- `keep_full_chain`: Full macro provenance vs. immediate only
- `keep_conditions`: Condition tracking detail level

## Diagnostics

### Error Reporting
- Parse errors with position and context
- Recovery information (what was skipped)
- Best-effort classification of failed construct

### Linting Annotations
- C99 construct usage (when targeting C89)
- Dependable C violations
- Non-portable constructs

### Suggestions
- Missing include hints based on symbol usage
- Potential typos based on similar identifiers

## Query API

### Layer Selection
- Default: fully expanded layer for semantic queries
- Macro queries: search across all layers
- Source queries: layer 0 for formatting/original text

### Cross-Layer Traversal
- Token provenance chain enables walking expansion history
- "Find where this expanded token came from"
- "Find all expansions of this macro"

### Condition Filtering
- Set active conditions for query
- Filter results to specific configurations
- Find code reachable under condition X

## Memory Model

### Single Arena
All structures allocated from single piscina arena:
- Tokens, AST nodes, trivia, indices
- Simpler lifetime management
- No individual free, bulk release

### GLR Considerations
- Fork handling: likely separate-from-fork or copy-on-write (TBD)
- Invalid forks not immediately cleaned
- Arena freed when parse complete

## Future Considerations

### Not In Initial Scope
- Incremental parsing (re-parse on change)
- User-extensible comment patterns
- LSP integration

### Deferred Decisions
- GLR GSS implementation details (needs research)
- Optimal fork memory strategy
- Performance optimization (solve correctness first)

## Relationship to Current Arbor

Arbor v2 is a parallel experiment:
- Shares infrastructure (piscina, chorda, xar, internamentum)
- Does not modify current arbor code
- May eventually replace arbor if successful
- Different design philosophy (GLR vs. recursive descent, layers vs. modes)
