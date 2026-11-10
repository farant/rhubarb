# officina_medulla_fluxus worklog

## 2026-11-04 — born (officina M5 T5, G24)

Read-only analyses of one medulla function for the native backend:
successors/predecessors, reachability, loop-contiguous block order,
natural-loop depth, liveness intervals, live-across-call. The "pactum"
section dumps four fixtures before and after computare and compares
the text byte for byte.

Decisions pinned by the suite:
- Successors = BLOCCUS operands of the block's last instruction (a for
  salire, b then c for ramus). Predecessors ascending, deduplicated
  (ramus to the same block twice), including edges FROM unreachable
  blocks (pure CFG); everything else ignores unreachable blocks.
- Order: DFS from block 0 (b before c) -> RPO; then each loop header
  pulls its natural-loop body (RPO order, nested headers recursively)
  so bodies are contiguous. $dum_probat: RPO 0 1 2 5 7 6 3 8 9 10 4 ->
  order 0 1 2 5 7 4 6 3 8 9 10. Loops found from DFS back edges; same
  header = one loop. Irreducible flow is not special-cased (order may
  be less tidy; liveness is unaffected).
- Positions: 2k use, 2k+1 def, k over instructions in backend order;
  unreachable blocks have none. Call arguments live in the function's
  operand stack (extra_index/extra_numerus), NOT in a/b/c - liveness
  reads them there. Parameters get an implicit def at 0; a register
  live-in at entry starts at 0 (F13 zero-filled file).
- Interval = [min, max] over block-boundary liveness (live-in -> block
  start, live-out -> block end) plus every use and def (dead defs
  included: the write needs a destination). No holes in M5.
- Live across a call: start < call use position AND end > call def
  position. An argument used only by the call, and the call's own
  result, do not cross.

Plants: the plan's "successors swapped -> praedecessores" cannot work
(swapping b/c reorders, the pred SETS are equal) - it breaks only
"ordo". Dropping the second successor breaks praedecessores (and
everything downstream). Ignoring defs in later blocks breaks only
"iunctura": %t1 then appears live-in at the second def's block, so
its interval starts at 0 instead of 3.

Naming: `registrum` is a latina.h macro (register) - the plan's
header used it as a parameter name and examen only noticed in the .c
(a `register` storage class on a prototype parameter is legal C).
Renamed to index_registri.
