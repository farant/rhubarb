# effectus slice 2 - interview

Ledger: desideratum …SZVT (region 'flow analysis'), built on decision
…DESGA (flow-insensitive facts never shrink the key). Slice 1 closed
2026-10-05 (13064eed).

## 0. Data (2026-10-05, before any question)

Census `build/effectus/census.tsv`: 7,584 sites, **3,304 unresolved**
(`resolutio="nulla"`, 44%). The census does not carry the site text of
unresolved sites (column `via` = `-`) and `causa` is always "valor
ignotus", so the breakdown below joins the census to context-free
per-file summaries (`crusta/build/effectus -radix $PWD -tabula ... f.sh`)
on (plagula, sedes, elementum). Each site is then classified by its
first variable's definition in the same file. A scratchpad
HEURISTIC (regex over the source): good for planning, not a gate.

| class | sites | share | example |
|---|---|---|---|
| A temporary: `T="$(mktemp -d)"`, `AREA=...` | 1,119 | 34% | `"$T/r"` |
| B flag arrays read as operands: `"${GCC_FLAGS[@]}"` (99x `declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")`) | 558 | 17% | `clang "${GCC_FLAGS[@]}" ...` |
| C1 loop / read variable itself | 400 | 12% | `for src in ...; [ -f "$src" ]` |
| C2 derived from a loop variable | 668 | 20% | `obj="$BUILD_DIR/$f.o"` |
| H chains of assignments | 200 | 6% | `$obj_files` |
| D function arguments (`$1`, `x="$1"`) | 152 | 5% | `"$tool_source"` |
| F/G/E other (`$(...)` values, env-only, literal) | 207 | 6% | `$FABRICA_SCRIPTURA/...` |

Correction: before the data I guessed function arguments were the
biggest class. They are 5%. Most of the unresolved mass is a VALUE
problem (what kind of value a variable holds: fresh temp dir, options
array, pattern over a loop), not an ORDERING problem.

Two notes found while measuring: `effectus -radix .` (relative) answers
"scriptum absens" for every script; a single-file summary of a sourced
library (cursor_communis.sh) leaves `$RADIX_DIR` unresolved, while the
census resolves it through the chain root. Both are correct-but-rough.

## Questions

(asked 2026-10-05; answers recorded below each)

**Q1. Order within slice 2 - DECIDED (a)** (Fran 2026-10-05: "that
sounds like a good plan to me", Q1-Q4 as recommended). VALUES first
(temporary dirs, arrays, loop patterns); ordering / flow becomes the
slice after (2b or 3), once the oracle can measure its payoff.

**Q2. Temporary dirs - DECIDED (a).** A path built from `$(mktemp ...)`
(with or without `-d`, with or without a template) gets class
`temporaria`: out of the key and out of the irresolutum lint. Sound
without flow: a fresh temp path starts empty and is new each run, so
anything read under it was written by this run.

**Q3. Loop values - DECIDED (a).** Patterns: `for f in lib/*.c` binds
`f` to the glob; `obj="$BUILD_DIR/$f.o"` becomes `build/*.o`,
`forma="globus"` (the key already digests globs). Literal word list =
the exact set; `read` / `$(...)` sources = prefix only
(`forma="praefixum"`). Over-approximation keeps the soundness rule.

**Q4. Arrays - DECIDED (a).** Evaluate `declare -a X=(...)` and
`X+=(...)`, including arrays from sourced files (tools/vexilla.sh);
words beginning `-` are options per the command table, not reads.

**Q5. Measuring - DECIDED (a)** (Fran 2026-10-05: "your recommendations
for all the questions sound good", Q5-Q8). The analyzer NAMES the cause
of each unresolved site (`causa`: argumentum, ansa_read, substitutio,
ambitus, ... instead of "valor ignotus"); the census writes the site
text of unresolved sites. A gate pins per-class counts on fixtures;
spec As-built reports before/after by class. The §0 heuristic is
retired by the tool's own numbers.

**Q6. Oracle for temporaria - DECIDED (a).** The oracle comparison
grows a rule: every observed read under a temp root (`$TMPDIR`,
`/var/folders`, `/tmp`) maps to a `temporaria` site or is explained.
Empirical check of Q2's soundness on real runs.

**Q7. Scope - DECIDED.** Assignment chains (H, 6%) IN - string/pattern
propagation once leaves resolve. Function arguments (D, 5%) OUT -
call-site binding is interprocedural, goes with flow in the next slice.

**Q8. Owners for patterns - DECIDED (a).** build-sine-domino matches by
CONTAINMENT: a writer pattern covers a reader pattern
(`build/x/*.o` covers `build/x/a.o`). Fixture: reader pattern wider
than writer -> still monitum.
