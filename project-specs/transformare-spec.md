# transformare - AST patterns with captures: census, plan, rewrite

Ledger: decretum …68V6 (fabrica-8 Q3: an examen rule for raw write
functions, every occurrence fixed at once). Builds on silva (parse,
byte-exact emit, `silva_quaestio` selectors with captures,
`silva_arbor_aequalitas`), `emendare` (the verify-before-write guard)
and `renominare` (plan by default, `-scribere` applies).

*The question: the fabrica-8 sweep is ~380 call sites (238 raw write
calls, ~140 write-mode fopen) whose return conventions differ from the
filum replacements. Text search-and-replace is whitespace-sensitive,
blind to strings/comments/macros, and cannot SEE which syntactic
variations exist. Can silva give us a tool that shows every structural
variation of a call site, lets us write one rule per variation with
wildcards that keep arguments in place, and rewrites only what a
rule covers - verified before it writes?*

Status: DECIDED (Fran 2026-10-24, VII). Name `transformare`.

## I. Prior art (C culture)

**Coccinelle** (Inria/LIP6, used across the Linux kernel since ~2008):
"semantic patches" in SmPL - `-`/`+` lines over C with metavariables
(`expression E; identifier f;`), matched on the parsed program, not the
text; built precisely for API migrations over thousands of call sites.
Later: comby (structural, language-light), semgrep and ast-grep
(pattern = code fragment with `$X` holes). We take the shared core:
**the pattern is a fragment of the language itself, holes are
metavariables, matching ignores layout.** We do NOT take SmPL's
control-flow matching (`...` across statements) in v1.

## II. Terms

- **site**: one call to a target function whose callee token lies in
  the file's OWN text (not inside a macro body or an included file).
  Calls produced by macro expansion are never rewritten (Fran,
  2026-10-24); the census lists them separately as `in macro`.
- **consumer**: walking up from the call through parentheses, the
  first node that uses the call's value - comparison, `!`, cast,
  assignment, argument, condition of `si`/`dum`/`per`, `redde`, or the
  expression statement itself.
- **shape**: the consumer's token sequence with the call's arguments
  replaced by holes `$1..$n`, layout and comments dropped. Two sites
  with the same shape need the same rewrite.
- **pattern**: a C fragment (expression or statement) in which `$name`
  is a hole. `$name` matches exactly one expression subtree;
  `$...name` matches the rest of an argument list (zero or more). The
  same `$name` twice = the two subtrees must be equal (trivia-blind).
- **rule**: pattern -> replacement fragment, holes reused. Captured
  subtrees are carried over VERBATIM (their own text, comments and
  spacing inside them kept); only the tokens the rule names change.

## III. The three modes

**Census (query).** For a target (function name or pattern), every
site in the given files, grouped by shape, counted, with locations:

```
$ ./silva/transformare.sh -census remove lib tools probationes
  87  (vacuum)remove($1);                      lib/x.c:12, tools/y.c:40, ...
  12  remove($1);                              ...
   9  remove($1) != ZEPHYRUM                   ...
   3  remove($1) != 0 && errno != ENOENT       ...
   1  r = remove($1)                           probationes/p.c:88
   2  in macro (not rewritable)                include/m.h:30 (via X)
```

Shapes keep the consumer's other tokens literally (`!= 0` and
`!= ZEPHYRUM` are DIFFERENT rows - the census shows the real variety;
a rule can name both). This half is useful on its own: "show me every
way we call X".

**Plan.** A rules file (STML, data only - patterns are matched, never
evaluated: Canon's "no expression language" holds):

```xml
<transformatio titulus="remove -> filum_delere" functio="remove">
  <regula si="(vacuum)remove($via)"        fac="(vacuum)filum_delere($via)"/>
  <regula si="remove($via);"               fac="(vacuum)filum_delere($via);"/>
  <regula si="remove($via) != ZEPHYRUM"    fac="!filum_delere($via)"/>
  <regula si="remove($via) != 0"           fac="!filum_delere($via)"/>
  <regula si="remove($via) == ZEPHYRUM"    fac="filum_delere($via)"/>
</transformatio>
```

Each site is matched against the rules in order; the first rule whose
pattern matches the consumer (or an ancestor up to the statement)
applies. Sites no rule covers are REFUSED and listed by shape. The
plan prints per-file diffs; nothing is written.

**Apply (`-scribere`).** Per file, in memory: apply the plan, re-parse,
then the emendare guard - the file must parse without new errors, the
examen code the transformation targets (if named, e.g.
`codex="SCRIPTURA_PRAETERITA"`) must lose exactly the rewritten sites,
no other code may grow, no REICE introduced, and `clang
-fsyntax-only` with the house flags must pass (catches a wrong
replacement name or arity). Any failure = that file untouched, reason
named. Then `formator -scribere` on the touched spans only.

## IV. Pattern language v1

- A pattern is parsed by silva as C: an expression pattern is wrapped
  as `vacuum _p_(vacuum) { (fragment); }`, a statement pattern as
  `vacuum _p_(vacuum) { fragment }`; `$name` / `$...name` are lexed as
  reserved identifiers before parsing (they never reach a real file).
- Matching = tree comparison, trivia-blind, where a hole identifier
  matches any expression subtree (`$...` any argument-list suffix).
- Latin macros are ordinary tokens in the file's text: a pattern
  written with `si`/`ZEPHYRUM` matches text that uses them; the
  census shows when a file spells `0` or `if` instead.
- v1 holes are untyped. Later (if needed): `$name:identificator`,
  `$name:litteralis`, type-aware holes via the oracle.
- Out of v1: matching across statements (SmPL `...`), declarations and
  types as targets, rewriting inside macro definitions.

## V. Surfaces

- C tool `silva/instrumenta/principalia/transformare.c`, entry
  `./silva/transformare.sh` (recompiles when stale, like emendare):
  `-census <functio|pattern> <viae...>`, `-regulae <file.stml>
  <viae...>` (plan), `-scribere`, `-machina` (TSV for tools).
- pythonica: `silva.transformare(regulae, viae, scribere=False)` and
  `silva.census(functio, viae)` - thin: argv in, parsed TSV out.
  Nothing a bun wrapper would reimplement.
- Library parts land in silva where reusable: fragment parsing
  (`silva_fragmentum_parsare`), trivia-blind subtree equality,
  pattern matching (`silva_forma_congruit`) - the census and plan are
  thin over these.

## VI. First use: the fabrica-8 sweep

Rules files per replacement (`remove/unlink -> filum_delere`,
`rename -> filum_movere`, `mkdir -> filum_directorium_creare_cum_modo`,
`chmod -> filum_modum_ponere`, `rmdir -> filum_directorium_delere`
(new), write-mode `fopen -> lectiones_fopen`), applied area by area
(lib+tools, tests, gesta, knotapel, officina/silva/rest), refused
shapes fixed by hand, each area commit green. Then code 96's 164
read-debt files are the same kind of migration.

## VII. Decisions (Fran 2026-10-24)

- **N1** name `transformare`.
- **N2** rules are STML files, a committed LIBRARY
  (`silva/transformationes/*.stml`), reviewable and reused.
- **N3** a shape stops at the first consumer of the call's value (II).
- **N4** `renominare` stays separate (symbol-aware: scopes, members,
  prototypes - a pattern engine sees shapes, not bindings); emendare's
  code 85 (insert an include line) stays. Instead an examen code whose
  fix is mechanical NAMES its rules file in its message ("emendatio
  mechanica: silva/transformationes/X.stml") - the rule library doubles
  as the house's fix-its; transformare reuses emendare's guard.

## AUDIENDA (not verified)

- Fragment parsing via a synthetic function wrapper: untested; holes
  as reserved identifiers may collide with the Latin lint (they never
  reach a file, but the lexer must accept `$`).
- Subtree equality: `silva_arbor_parsurae_aequales` compares whole
  parses; a subtree comparator is assumed buildable from it, not read.
- How `silva_quaestio` captures (`tag$nomen`) and these holes relate -
  possibly the census can be a selector + shape key; not designed.
- Counts in VI are from `git grep` (I, fabrica-8), not from silva.
- STML quoting: patterns contain `<` and `"` (`remove($v) < 0`,
  `fopen($p, "w")`); attribute form in III may need STML's raw-text
  form instead - not checked against the STML grammar.
