# effectus slice 3 — spec (which definition REACHES a use)

*Born 2026-10-06 from the slice-3 interview
(`project-specs/effectus-3-interview.md`, Q1–Q7) and its measurement
(§0). Predecessors: `effectus-spec.md` (slice 1), `effectus-spec-2.md`
(slice 2: values; §XV as built). Ledger: desideratum …SZVT, work list
nota …TKJ5, decision …DESGA. Question that prompted it: after slice 2,
1,208 sites are unresolved; the two big classes are `discordia` (531,
variables reassigned in sequence - the union of all their definitions
exceeds the cap) and `argumentum` (507, two thirds of them a SCRIPT's
own `$1`).*

*Status: APPROVED (2026-10-06) - §XII A1–A5 decided by Fran (all recommendations: "that all makes sense to me"); plan `effectus-plan-3.md`.*

## 0. What the measurements say (dated 2026-10-06)

Interview §0 (heuristic over the census): `discordia` 531 - almost
every such variable has a definition earlier in the same file than the
use; `argumentum` 507 - 329 top-level script `$N`, 178 inside
functions. Code facts:

- Definitions are collected per PROCESS (`_definitiones_colligere`,
  `crusta_effectus.c`) and joined without order in
  `_variabilem_intus`: every definition in scope that names the
  variable is evaluated and UNIONED (`_valores_iungere`). The only
  order-aware rule is T5's loop binding (`_ansa_ligans`).
- `Definitio.locus` (T5) holds the defining node (assignment or
  iteration), so the position of every definition in the AST is known.
- Exec arcs (`Arcus`: pater, filius, custodia) carry NO argument list;
  a child's `$1` has no definition and fails as `argumentum`.
- Gates run their chain roots with a fixed argv held in
  `pythonica/silva.py` (`'toml': (['./toml/compile_probationes.sh'],
  ...)`, :1527 - no arguments); the runner reads `FILTER="${1:-}"`.
  Nothing in the effectus ingressus says so.

## I. Principles

1. **The soundness rule stands** (…DESGA). Ordering only REMOVES
   definitions from a union that slice 2 already proved sound: the new
   value of every site is a subset of its slice-2 value (§VIII makes
   this a mechanical check).
2. **Fallback is the slice-2 union** (Q3): any use the walk cannot
   place - a definition that only a sourcing script makes, an
   unhandled construct - gets exactly today's answer.
3. **Values only** (Q5): no key line is dropped for "written earlier in
   this run"; that is a later MUST analysis.
4. **Structure is the CFG** (Q2): bash has no goto; crusta's AST
   (`PROGRAMMA`, `CATENA`, `PIPA`, `GREX`, `CRUSTULA`, `CONDITIO`,
   `ELECTIO`, `ITERATIO`, `CYCLUS`, `REPETITIO`, `FUNCTIO`) is walked
   directly.

## II. Reaching definitions by a backward walk

For a use U of variable X, `_definitiones_attingentes(U, X)` returns
the set R of definitions that may reach U, or FALLBACK:

1. **Sequence.** Walk the statements preceding U in its list, nearest
   first. A statement that DEFINITELY assigns X (A2) ends the walk:
   R gets its definition, done. A statement that MAY assign X adds its
   definitions to R and the walk continues.
2. **Up a level.** At the start of a list, step to the enclosing
   construct and continue before it:
   - `CONDITIO` / `ELECTIO` branch: the condition / subject precedes
     the branch; then the statements before the construct;
   - loop body (`ITERATIO`, `CYCLUS`, `REPETITIO`): definitions of X
     anywhere in the body MAY reach (back-edge); for `for X in L` the
     binding is a DEFINITE definition at the body's start, which kills
     the back-edge for X itself (subsumes T5's `_ansa_ligans`);
   - `GREX` `{ }`, `CATENA` member: transparent;
   - `CRUSTULA` `( )`, `$( )`, `PIPA` segment: transparent INWARD (the
     subshell inherits); but definitions inside such a construct never
     count for a use OUTSIDE it (Q7);
   - `FUNCTIO` body: R = union over the reaching sets at every call
     site of the function in the process (each call site walked as a
     use); a function with no known call site, or one passed around
     (`trap`, `eval`) -> FALLBACK;
   - top of a plagula that another plagula SOURCES: continue at the
     `source` site in the sourcing plagula; top of the process root ->
     done (environment: X undefined).
3. **What "may assign" includes.** A construct (`if`, `case`, loop,
   `{ }`, `&&`-chain) containing an assignment to X not inside a nested
   subshell; a call to a house function whose body (transitively)
   assigns X globally; a `source` of a plagula that assigns X at top
   level; `read`/`mapfile`/`getopts`/`printf -v` naming X (value
   unknown: `ansa_read`).
4. **FALLBACK** when the walk meets `eval` before U in scope (it can
   assign anything), or exceeds a depth bound.

The value of X at U = join (slice-2 `_valores_iungere`) of the values
of the definitions in R, each evaluated as today with ITS defining node
as the use for its own right-hand side (so chains of definitions are
ordered recursively). Empty R with X never defined in the process ->
today's `ambitus` / `argumentum` path.

## III. Arguments

1. **Script arguments (Q4).** `Arcus` gains the argument words of the
   exec site (and the parent's ambitus/plagula to evaluate them). In a
   child process, `$N` (and `"$@"`, `$#` is out) = union over every arc
   into this child of the N-th argument's value; `${N:-d}` per slice-2
   A4 rules with "defined" = some arc supplies argument N. Unknown when:
   a caller passes `"$@"`/unknown words, the script has no house caller,
   or it is a chain root whose argv is not declared (A1).
2. **Function arguments (Q1: if it falls out).** The FUNCTIO rule of
   §II already visits every call site; `$N` inside a function body =
   union of the N-th argument word over those call sites, evaluated at
   each call site. Unknown call sites -> `argumentum`.
3. `shift` inside the body or script renumbers: any `shift` in scope
   -> the affected `$N` stay `argumentum` (named, not modelled).

## IV. Sites, causa, dialect

- No new site forms, no dialect change. Sites gain precision only.
- `causa` gains nothing; `discordia` should fall to the joins that are
  genuinely mixed after ordering.
- `effectus.canon` unchanged unless A1 adds an attribute to
  `aedificatio.canon` (ingressus `effectus`: `argumenta`).

## V. The key and the lint

Unchanged machinery: values feed the same lines. The key can only
shrink through value precision (fewer members, a prefix becoming a
path) - every such shrink is covered by §VIII's subset check.

## VI. The oracle

No change. Expected: `explicata` falls (unresolved static sites that
excused observations become resolved and cover them directly); non
tecta must stay 0 on the toml chain.

## VII. Order (plan sketch; the plan fixes it)

1. **The subset check first** (`effectus -subsumere`): old binary vs
   new over the house; born green (identical binaries), plant red.
2. **Sequence + up-a-level walk** for top-level code and blocks; loops
   (back-edge, for-binding); FALLBACK paths.
3. **Subshell / pipeline boundaries** (Q7).
4. **Functions:** reaching sets via call sites; function arguments.
5. **Sourced plagulae** in the walk; **script arguments** via `Arcus`
   (+ A1).
6. **Close:** census by causa, toml key and oracle, docs, ledger.

## VIII. Measuring (Q6)

`effectus -subsumere <vetus.stml> <novum.stml>` (or a gate over two
binaries): for every site of NOVUM, the site of VETUS with the same
plagula, sedes and element must COVER it - equal path, glob match, or
prefix - unless the VETUS site is unresolved (`nulla`) or `ignotum`.
Any uncovered new site is printed and the check fails. Run over all
tracked `.sh`, old binary = slice-2 (ccc2f3a3) build, frozen in the
scratchpad before the first edit. Plus: per-pattern fixtures (sequence,
`if`, `case`, loop back-edge, for-binding, subshell, function call,
`source`, script argv), census by causa before/after.

## IX. Review focus

1. **A definition in a subshell leaking out** (`( X=a ); cat "$X"`) -
   precision, and the subset check will not catch it (it IS a subset);
   fixture CONTRARY required.
2. **`while read` loops in pipelines** (`cmd | while read x; do
   Y=...; done; cat "$Y"`): the loop runs in a subshell; Y's definition
   must not reach. Same class as 1.
3. **Function bodies called from another plagula** (cursor_communis
   functions called by every runner): the call sites live in the
   SOURCING plagula; the walk must find them through the process, not
   the file.
4. **The kill rule and conditionals** (`[ -n "$X" ] || X=d`): a
   `CATENA` member assignment is a MAY, never a kill.
5. **Recursion through call sites** (a function calling itself, two
   functions calling each other): bound + FALLBACK, never a hang
   (`CREDO_NON_PENDET` on a fixture).
6. **Cost**: a backward walk per use per variable - the census and the
   judge time must stay within ~10% (memoize per (use node, X)).

## X. Done means

- `discordia` and `argumentum` counts fall, by causa, before/after in
  §XI; the subset check passes over all tracked `.sh`.
- Fixture CONTRARIES for §IX 1, 2, 4 pass; no hang on recursion.
- Toml chain: 0 errata; key changes explained; oracle non tecta 0;
  judge time within ~10% of slice 2's 2.03-2.07 s.

## XI. As built

**T1 (2026-10-06): the subset check.** `crusta_effectus_subsumptio`
(header): every site of NOVUM must be covered by a VETUS site at the
same (plagula, sedes, elementum) - equal path, glob match or
containment, prefix containment, or an old `nulla`/`ignotum`; a NEW
site less precise than the old one (prefix where the old was exact,
unresolved where it was resolved) or at a new sedes is a defect - so
the check catches regressions as well as unsound members. CLI
`effectus -subsumere V N`; tool `crusta/effectus_subsumptio.sh
<binarium_vetus>` over every tracked `.sh`. Born green 315/315;
plants: every resolved path + "~" -> 60/315; predicate always true ->
section XVIII red.

Found: effectus summaries were INVALID STML on disk wherever a site's
text held `"` (`via=""$X""`) - STML stores inline attributes raw
(stml-instrumentum-spec §5.4), so the writer must escape; nobody had
read a summary back from disk (census, key and tests use the tree in
memory). `_attributum` now escapes `"` -> `&quot;`, `&` -> `&amp;`
(vitium filed, closed). Consequence: the reference binary had the same
bug, so it was RE-FROZEN after the fix - slice-2 analysis plus escape
(scratchpad `effectus.slice2`, md5 617cd37e); the analyzer diff against
ccc2f3a3 is additions only.

## XII. AUDIENDA — DECIDED (Fran, 2026-10-06: every recommendation (a))

- **A1. Where a chain root's argv comes from.** (a) Recommended: an
  optional attribute on the effectus ingressus in `aedificatio.stml`
  (`argumenta=""` for "none", `argumenta="registrum"`), checked by
  `aedificatio.canon`; absent = unknown (`argumentum` as today); the
  gate's real argv lives in `pythonica/silva.py`, so a small check
  compares the two for the declared roots. (b) Read nothing; chain
  roots' `$N` stay unknown.
- **A2. What counts as a DEFINITE assignment (a kill).** (a)
  Recommended: only a plain assignment statement at the top of the
  same list (`X=...`, `declare/local/export X=...`), and the `for`
  binding at the start of its body; everything inside a compound
  construct is a MAY - simple, and every mistake is on the sound side.
  (b) Also an `if`/`case` whose every branch assigns X.
- **A3. `eval` in scope.** (a) Recommended: `eval` before the use in
  the same function or top level -> FALLBACK for every variable read
  after it (it can assign anything); (b) keep slices 1-2's assumption
  that `eval` assigns nothing.
- **A4. Function arguments.** (a) Recommended: in, through the
  call-site walk of §II (it falls out); (b) defer.
- **A5. `recursio` accumulations** (`X="$X y"` in a loop, 118 sites).
  (a) Recommended: leave named (a finite set needs a fixpoint over
  sets - another slice); (b) attempt a bounded fixpoint now.

**T2 (2026-10-06): the walk.** `_attingentes` (backward walk) +
`_sententiae_effectus` (per statement: none / may / KILL - a plain
assignment statement at the top of the list, A2 / -1 on `eval`, A3).
Sequence lists kill; `elif`/`else` branches and `case` items are
ALTERNATIVES (skipped); `&&`/`||` members and pipeline segments are
may-only; `for X` binds (kills, also for the back-edge); loop bodies
see their own definitions (back-edge); `if` conditions are may.
FALLBACK (slice-2 code path, loop binding and title-level recursion
guard included): use in a function or in a non-root plagula, any
definition outside the use's plagula or inside a function, X set by
`read`/`mapfile`/`getopts`/`printf -v`, `eval` between, or nothing
reaching (unset - kept as slice 2 rather than a regression). The
recursion guard is now per DEFINITION (`definitiones_aestimandae`):
`H=h; H="$H/x"` resolves to `h/x` (was `recursio`); loop accumulations
still reach themselves and stay `recursio`.

House: subset check 316/316 vs the frozen reference. Census:
unresolved 1,210 -> 763; `discordia` 472 -> 75; plena 8,691 -> 8,854,
partialis 495 -> 569 (caps turned into prefixes). Toml's key 31 -> 32:
`octeti toml/probationes/fixa/tomllib/aurum.txt` (the generator's
`grep -c "$EXITUS"` after writing it - resolved now; sound, slightly
more than needed, Q5) - and its excuse at tomllib_aurum.sh:36 died
(removed). Oracle: non tecta 0, explicata 75.

Cost: the first build made the census 2.3x slower (4.57 s vs 1.95 s):
`sample` showed the time in the walk's per-use rescans - an `eval`
subtree scan per preceding statement, and `_a_lectione_positum`
rescanning every command of every plagula per evaluation. Per-plagula
`eval_status` and a per-ambitus cache of read-assigned names: census
1.83 s (faster than slice 2), judge 2.19-2.22 s warm (+7%).

Found on the way: the T1 subset predicate (and the oracle's
`_staticus_tegit`) did not treat the root prefix `./` as covering
every relative path - the one non-subsumed script was exactly that.
Fixed in both.
