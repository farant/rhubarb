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

**T3 (2026-10-06): process boundaries (Q7).** `_finis_processus`: a
subshell `( )`, `$( )` / backticks (`PARS_SUBSTITUTIO`), `<( )`
(`PARS_PROCESSUS`), `coproc` (`SOCIUS`), and a member of a `PIPA` that
holds a `|` operator (lastpipe off). A `PIPA` without `|` (`! cmd`,
`time cmd`) runs in the current shell - not a boundary.
`_processum_attingit(locus, usus)`: every boundary above the
definition must also contain the use - inward reaches (`X=a; ( cat
"$X" )`), outward does not. Applied in `_sententiae_effectus` and the
`for X` back-edge; `{ }` is not a boundary. FALLBACK unchanged (a use
whose only definitions are excluded falls back to slice 2, Q3).

House: subset check 316/316; census delta ZERO (10,563 sites; the only
changed rows are tomllib_aurum.sh line shifts from T2's comment edit).
Measured, not assumed: a probe at the exclusion found no house use the
walk places where a boundary definition would reach; the only boundary
definitions with a named use (`for f` inside `( )` in
tools/corpus_infixum.sh) meet uses in functions or in sourcing
plagulae, which fall back before the rule applies (T4/T5 territory).
The rule is a soundness guard today, not a lever. Plants: boundaries
ignored -> section XX red at RF 1, RF 2, `$( )`, pipe segment; every
`PIPA` a boundary -> the `! H=h2` contrary red.

**T4 (2026-10-06): functions.** Opening reading (probe over the house,
1,086 ambitus): plagulae ordered root first, then sourced plagulae in
discovery order (`_ambitum_derivare`, fixpoint; 466 ambitus have more
than one). 245 functions: 190 called only from their own plagula, 22
only from other plagulae (the sourced libraries sera, mensor_suitae,
nexus_recens, cursor_communis, capsula_radicis), 9 both, 24 with no
static call; 0 duplicate names; 17 PASSED (name in a static non-title
word: `trap sera_dimittere EXIT`, fumus `actio`/`ok`/`non`). 190
dynamic command titles: paths (`"$RADIX_DIR/x.sh"`, `"$0"`), binaries
in variables (`"$BIN"`), oracle arrays, `"$@"` (2).

As built: `FunctioNota` per ambitus (`_functiones_parare`, each
fixpoint pass after the definitions): static call sites in every
plagula, `tradita`, `geminata`, `eval` in the body, transitive `vocat`
matrix (Warshall). A function is `incerta` (FALLBACK) if it or any
function that (transitively) calls it is passed or duplicated, or if a
dynamic title may call a function: a title is evaluated (slice-2 union,
the table being built) and calls none when every member contains `/`
or is a temporary; a CERTUS member naming a function makes that one
passed; anything else (`"$@"`, IGNOTUS, a prefix without `/`) makes the
whole ambitus `vocatio_ignota`. The walk (`_ambulare`) leaves a
function body through `_vocationes_ambulare`: every call site walked as
a use; a function already on the stack adds nothing (least fixpoint);
depth XVI and M steps bound it. The top of a non-root plagula falls
back (T5). Statement effects: a definition inside a function DEFINED
in the statement reaches only if the statement calls it (or a caller
of it) by a call that reaches the use and from which the definition
reaches (`_vocatio_attingens`; may, never a kill; local definitions
never leave their function); `local X` (or `declare`/`typeset` inside
a function, without `-g`) kills; `eval` in a defined function counts
only when called (unless a passed function holds one). `$N` in a body
(`_argumentum_functionis`): union of the N-th argument word over the
call sites, each evaluated at its call site; `argumentum` if the
function is uncertain or uncalled, `shift`/`set --` in the body, a call
has fewer words, or a word up to N may expand to several
(`_verbum_singulare`); call recursion guarded (`recursio`). Found on
the way: the slice-2 title guard confused a function's `$1` with the
script's `$1` at `f "$1"` (`recursio` instead of `argumentum`) -
positional titles now skip it (fixture line 21).

House: subset 316/316; census 10,563 -> 10,081 sites (members removed
where ordering now applies: T2 used to fall back for ANY definition in
a function, e.g. `local obj` in `silva/materia_substratum.sh` blocked
the walk in every silva runner), unresolved 1,332 -> 1,268 (nulla 763
-> 696); `argumentum` 508 -> 439, `discordia` 198 -> 192; `$N` resolved
e.g. porta_linux.sh (glibc/musl). Census 3.2-3.3 s; judge 2.05-2.07 s
warm. Toml key 32 unchanged; oracle non tecta 0, explicata 125 - the
SAME with the T3 analyzer (the toml chain itself changed since T2's
75). Plants: call sites ignored -> body union, recursion and RF 3 red;
`$N` binding off -> A4 red; `tradita` ignored -> trap contrary red
(over-exclusion, invisible to the subset check); definition-is-not-a-
call off -> f2 red.

**T5 (2026-10-06): sourced plagulae, script arguments, A1.** Opening
reading: one effectus chain root (`toml/compile_probationes.sh`, the
`porta_toml` ingressus); its gate argv is `PORTAE['toml'] =
['./toml/compile_probationes.sh']` (none) - argv agrees. The FORM did
not: `stml formare` turns `argumenta=""` into a bare attribute, read as
`"true"` (a one-word argv). Shown to Fran; A1 AMENDED (decretum
…CAYCAMW): `<argumenta/>` (none) or `<argumenta><verbum! (>w
</argumenta>` (words, `<mandatum>`'s grammar), absent = unknown;
`stml_textus_valor` is stable under formatting.

As built. Script `$N` (top level of the root plagula, outside
functions; `_argumentum_scripti`): union over the arcs into the
ambitus of word N after the script word (`Arcus` keeps the exec site's
`verba`, the script word's index and the parent plagula; arcs are
deduplicated per parent, child and word), each evaluated in the
parent; a missing word = `""` (an exec site's argv is complete when the
earlier words are single), so `${2:-d}` -> d; `"$@"` alone after the
script forwards the parent's `$N` (`_argumenta_traducta`) - a parent on
the stack (the house's `"$0" "$@" | tee` self-exec, 18 runners) adds
nothing (least fixpoint); the root adds its declared argv
(`crusta_effectus_derivare_argumentis`; lookup
`crusta_effectus_argumenta_radicis` over the aedificatio.stml files,
undeclared or conflicting = unknown), used by `-clavis`, the chain
summaries of the lint/census and `tools/diagnostica.c`. fabrica's API
is unchanged: the key lines carry the argv's effect. Arcs are complete
only after the whole derivation, so PHASE 2 recomputes every ambitus
that asked for a script `$N` and can receive one (a root with declared
argv, an incoming arc) until arcs and ambitus stop growing (bounded;
no convergence -> phase 1, `argumentum`). Sourcing: each fixpoint pass
records its resolved source sites (`Fontatio`); walks use the previous
COMPLETE pass's record and the fixpoint repeats while the record
changes (`_ambitum_computare`, extracted from `_ambitum_derivare`). At
the top of a sourced plagula the walk continues at every site that
sources it (`_fontationes_ambulare`; an unresolved source site anywhere
= FALLBACK); a top-level definition of a sourced plagula reaches
through any statement that (transitively, at top level) sources it -
may, never a kill (`_fontatio_attingens`), allowed only when every
chain of source sites to the root is at top level (`_fontatio_plana`);
`source` of a plagula holding eval = eval. Cross-check (A1):
`silva.argumenta_catenarum` / `_argumenta_discordantia` (pythonica
gate) - declared words == `PORTAE[G][0][1:]` and the root ==
`PORTAE[G][0][0]` for every `porta_<G>` iudicium.

House: subset 316/316; census unchanged (10,081 rows identical): the
census derives every script AS A ROOT (a child run by hand has no
caller - Q4), and the sourced definitions that now go through the walk
(probe: `tools/vexilla.sh` ~3,700 uses, mensor_suitae, sera) are only
defined in the sourced file, so the value is the same. Toml key 32
unchanged: `FILTER="${1:-}"` feeds only a name match, no site, and the
key holds no `ignotum`. Oracle non tecta 0 (explicata 125); judge
1.87-1.89 s; census 3.1-3.5 s (a first build cost 4.3 s: NIHIL vs empty
record forced one extra pass on every ambitus - fixed). Plants: root
argv ignored, arcs ignored, `"$@"` forwarding off, source continuation
off, forward reach off -> section XXII red at each; a mismatched
`<argumenta>` in toml/aedificatio.stml -> pythonica red.

**T6 (2026-10-06): close.** Census by causa, measured on the SAME tree
(today's) with the frozen slice-2 binary and the final slice-3 binary:

| | slice 2 | slice 3 |
|---|---:|---:|
| sites | 10,773 | 10,081 |
| plena | 8,691 | 8,436 |
| partialis | 495 | 572 |
| **nulla** | **1,210** | **696** (-42%) |
| discordia (nulla+partialis) | 531 | 192 (-64%) |
| argumentum | 509 | 439 |
| substitutio | 152 | 121 |
| recursio, ambitus, cwd ignotum, ansa_read, operator, tabulatum, absens | 130, 101, 94, 84, 73, 19, 12 | 130, 104, 94, 84, 73, 19, 12 |

Fewer sites = smaller values (one site per member). Partialis rose
because nulla sites gained a known prefix. The script-argument work (T5)
cannot move the census by design (every script is derived as a root);
it acts in chains. Subset check, final binary: 316/316.

Cost. Census 1.58-1.74 s (slice 2) vs 1.59-1.62 s (final), measured
alternately with nothing else running. Along the way the committed T5
binary measured 2.5-3.2 s: `_scriptum_est` read every exec target WHOLE
(binaries included) to test for `#!`, at every exec site, every
fixpoint pass - T4/T5 multiplied the passes. Now memoized per
derivation (`scripta_nota`) and reading the first line only
(`filum_lector`). Judge 1.87-1.89 s (slice 2 era: 2.03-2.07).

Toml: chain lint 0 errata (5 monita, `mandatum-ignotum`, outside the
verdict's concern), key 32 lines (slice 2: 31; +1 =
`octeti aurum.txt`, T2), oracle non tecta 0 (explicata 125 - the same
with the T3 analyzer; the chain itself grew since T2's 75).

Left (filed): the key shrink for reads of files the run wrote first
(Q5, needs a MUST analysis and oracle proof - `ante_scripta`); the
`recursio` accumulations (A5, 130 sites: a fixpoint over sets);
unmodelled corners named in the worklog (`X=v f` prefix assignment into
a function, `trap 'code'` strings).
