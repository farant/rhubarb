# effectus slice 3 - interview

Ledger: desideratum …SZVT (region 'flow analysis'), measured work list
nota …TKJ5; built on decision …DESGA (flow-insensitive facts never
shrink the key). Slice 2 closed 2026-10-06 (ccc2f3a3).

## 0. Data (2026-10-06, before any question)

Census after slice 2 (315 scripts, 10,759 sites): 1,208 unresolved
(11.2%). Shape of the two big classes (scratchpad heuristic over the
census + source; planning numbers, not a gate):

**`discordia`, 531 sites** (nulla + partialis): the variable has 2
definitions (100), 3-5 (217), 6 or more (151); in essentially every
case at least one definition precedes the use in the same file - the
textbook reaching-definition case (`src=...; obj=...` blocks
reassigned before each compile). 59 have no plain variable.

**`argumentum`, 507 sites**: 329 (65%) are the SCRIPT's own `$1`/`$@`
at top level - not function arguments. Their values come from the
script's callers: house scripts exec'ing it with literal arguments,
and fabrica's declared argv for chain roots (`aedificatio.stml`
mandatum; toml's runner gets `registrum`). 178 are inside functions;
by sites, same-file callers pass only literal arguments for 54, some
`$`-arguments for 60, and 52 have no call in that file (called from a
sourcing script).

Correction of the obvious guess: ordering is the big lever; script
arguments matter more than function arguments.

## Questions

**Q1. Scope - DECIDED (a)** (Fran 2026-10-06: "all those
recommendations make sense to me", Q1-Q4). Ordering (reaching
definitions) first, plus SCRIPT-argument binding from exec callers and
fabrica's declared argv; function-argument binding in this slice only
if it falls out of the same machinery.

**Q2. What "reaching" means - DECIDED (a).** Structured reaching
definitions over crusta's AST (bash has no goto: the AST is the CFG):
sequence - the last earlier assignment in the block wins; `if`/`case`
- union of the branches, plus the definition before it if a branch may
not assign; loops - a definition in the body also reaches the body's
start (back-edge); functions - the body sees the union of what reaches
each call (everything, if a call is unknown).

**Q3. When unsure, fall back - DECIDED (a).** A use the analysis
cannot place keeps today's flow-insensitive union (definitions in a
sourcing script, `eval`, unknown functions, subshell boundaries).
Soundness holds by construction: ordering only shrinks a union that
was already sound.

**Q4. Script-argument binding - DECIDED (a).** A child's `$N` = the
union of the N-th argument over all known exec sites in the house
summary (`Arcus`) plus fabrica's declared argv for chain roots; any
unknown caller, or no house caller (run by hand) -> stays
`argumentum`.

**Q5. Ordering and the key - DECIDED (a)** (Fran 2026-10-06: "those
all make sense to me", Q5-Q7). This slice uses ordering only to
sharpen VALUES (which definitions reach a use); the key gets more
precise only through them (sound by construction). Dropping key lines
for "written earlier in this run" needs a MUST analysis plus oracle
proof (`ante_scripta`, still empty on toml) - its own decision later.

**Q6. Measuring - DECIDED (a).** A mechanical SUBSET check over the
house: every site of the new summary is covered by the slice-2
binary's site at the same sedes and element (equal path, glob match,
prefix) or that old site was unresolved; a new member outside the old
value is a defect by construction. Plus per-pattern fixtures and the
census by causa before/after.

**Q7. Subshells and pipelines - DECIDED (a).** Respect process
boundaries: an assignment inside `( ... )`, `$( ... )` or a pipeline
segment (lastpipe off) does not reach uses outside it. The subset
check (Q6) guards the model.
