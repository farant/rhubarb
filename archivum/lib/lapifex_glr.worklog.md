# lapifex_glr.c Worklog

## 2026-02-07 — M2.1 Generic GLR Engine Complete

### Summary
Implemented generic GLR engine that consumes `LapifexTabula*` (ACTION/GOTO tables) and handles ambiguous grammars via fork/merge on a Graph-Structured Stack (GSS).

### Key Design Decisions

**Worklist-based reduction**: The REDUCE phase uses a worklist of `(node_index, production_index)` pairs. Each pair is processed exactly once. When a new node is created from a reduction, its reduce actions are added to the worklist. This prevents infinite loops that occurred with the earlier `do { } while (reductio_facta)` approach.

**Value-based forking**: Standard GLR merges nodes by state (same state = merge). But our GSS stores values in nodes, not on edges. When two reductions produce the same target state but different values, we create SEPARATE nodes (fork) rather than merging. This preserves ambiguity. Example: for `2 + 3 * 4`, reductions produce st=1/val=20 ((2+3)*4) and st=1/val=14 (2+(3*4)) as separate frontier nodes.

**ACCEPT reads from predecessors**: The ACCEPT action fires after shifting EOF. The EOF node's value is 0 (the EOF token's value), not the parse result. The actual result values are in the predecessor nodes. For ambiguous parses, the EOF node has multiple predecessors (from the shift phase merging), and we collect all their values.

**Path enumeration**: Recursive DFS through GSS predecessors to enumerate all paths of length N (= production RHS length). This is needed because GSS nodes can have multiple predecessors, and each path represents a different parse history.

### Bugs Encountered

1. **Infinite loop (v1-v2)**: The original `do/while` reduction loop kept creating new nodes that triggered more reductions, forever. Fixed by switching to worklist with deduplication.

2. **Single parse for ambiguous grammar (v2)**: Only found value 20 for `2+3*4`, not 14. Root cause: when reducing to the same state with a different value, the code merged (added predecessor) instead of forking (creating separate node). The value 14 was lost because the merged node kept value 20.

3. **ACCEPT returning 0 (v1)**: Was reading the ACCEPT node's own value (0, the EOF token value) instead of the predecessor's value (the start symbol reduction result).

### Architecture
- `GSSVia`: Struct holding values array + basis node for path enumeration
- `ReductioParum`: Worklist entry (node index + production index)
- Three-phase per-token: REDUCE(worklist) → ACCEPT → SHIFT
- ~717 lines total (clean, no debug code)
