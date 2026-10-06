# crusta_fontationes worklog

## 2026-10-03 - born (fabrica slice 3 T2)

What it is: a symbolic evaluator over the crusta AST that lists which
repo scripts a runner sources or executes, for the verdict key (spec 3
§III.2). The read ledger cannot see bash reads, and hand lists rot
(tools/cursor_communis.sh was owed by no suite gate until 2026-10-03).

Design points that were not obvious:
- **Scope = process.** A script plus everything it sources shares one
  variable table (cursor_communis.sh uses the caller's `$RADIX_DIR`).
  An EXECUTED script starts a new scope and does not see the caller's
  variables. The test proves the negative: e.sh's
  `source "$RADIX_DIR/tools/z.sh"` stays irresolutum though z.sh exists.
- **Fixpoint per scope.** A file sourced late can define a variable an
  earlier site needs, so the scope re-collects definitions and
  re-evaluates every site until no new file joins. Only the last
  iteration's results count (an early pass would report false
  irresoluta).
- **`X=1 cmd` is not a definition** (bash sets it for that command
  only). Without this rule, `RHUBARB_RADIX="$RADIX_DIR" "$bin"`-style
  lines would collide with real definitions - plant B made 14 assertions
  fail.
- **`local` lives in its function**: assignments to a name declared
  local in F count only for uses inside F. Plain assignments inside a
  function are global (bash dynamic scoping: cursor_communis sets
  COMPILATOR in a function).
- **Empty definitions** (`MSU_MENSOR=""` then `MSU_MENSOR=".../bin/mensor"`)
  were the only false irresolutum on the real runners. They are dropped
  ONLY when the whole word is the variable (`"$X"`); inside a longer word
  (`"$P/x.sh"`) an empty value changes the path, so it still counts.
- **Unresolved but under build/ = productum.** `bin="$BUILD_DIR/$name"`
  with `$name` from a loop keeps its known prefix; a prefix inside a
  build dir means "built by this run" (its inputs are in the trace).
- **cwd for relative command paths** = the last `cd W` in the same
  catena (`(cd "$RADIX_DIR" && bin/aedilis ...)`), otherwise the tree
  root (gates run from the root). A `cd` to an unknown dir makes the
  site irresolutum.

Survey of all suite runners (2026-10-03): 13/14 resolve fully. Root
`compile_tests.sh` has one irresolutum (`$output_binary` =
`bin/$app_name`, a GUI binary the runner builds under bin/); briar's
chain reaches `tools/corpus_infixum.sh`, whose cwd is
`${CAPSULA_RADIX:-$(pwd)}` - honestly unknown.

Over-approximation seen on toml: the conditional builders
(`[ -x bin/aedilis ] || tools/aedilis_struere.sh`) and their chains are
counted, so editing tools/fabrica_struere.sh would re-run the toml gate.
Conservative, not wrong; a possible refinement is to treat a builder
guarded by `[ -x <instrumentum> ]` as covered by the instrumentum.

Gotchas hit while writing it:
- `vacuum` is a latina.h macro (void) - examen rejects it as a field name.
- `'dir/*'` in a block comment is `-Wcomment` under -Werror.
- `mkdtemp` is not in examen's POSIX lexicon; the test builds its tree
  under crusta/build/fontationes_fixa.<pid> instead (paths are judged
  relative to the fake root, so the /build/ in its own path is harmless).
- silva/renominare.sh refuses untracked files (git is its undo).
- First commit attempt: the `diagnostica` gate went red everywhere (rc 2)
  because tools/diagnostica.sh compiles crusta/fontes/*.c by GLOB but
  links lib/ objects from a HAND list without filum. Fixed by adding
  `filum` to that list; the real fix is diagnostica's W0ZBW row (closure
  from aedilis instead of glob + hand list).

## 2026-10-05 - RETIRED (effectus T8)

crusta_fontationes.{h,c}, crusta/fontationes.sh, crusta/instrumenta/
fontationes.c and probatio_crusta_fontationes.c are deleted. The
evaluator moved into crusta/fontes/crusta_effectus.c in effectus T3; the
fabrica genus `effectus` (T7) replaced genus `fontationes`. Idiom test
ported as crusta/probationes/probatio_crusta_effectus_idiomata.c. This
worklog stays as history; continue in crusta_effectus.worklog.md.
