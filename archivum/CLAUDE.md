# archivum/ — earlier generations, frozen

Everything under this folder is **read-only historical reference**. The
unit model says so (`archivum/unitas.stml`: `vita="archivata"`, meaning no
rules and no gates), and `bin/fabrica quid <via>` reports it for every
file here.

- **Nothing here is built or tested.** No suite main reaches these files.
  The generated manifest (`compile_tests_fontes_generata.sh`) does not list
  them, and the FAQ collector skips them (`unitates_interrogationes`).
- **Nothing here is formatted or swept.** The pre-commit formatter and
  examen skip `archivum/`, and so does `tools/forma_evolutio.sh`.
- **Do not edit.** Paths inside these files (`#include`, scripts that say
  `lib/...`, worklogs) are the paths of their own time. That is the point
  of an archive. Do not "fix" them.
- **To restore**, `git mv` an engine back together with its suites, as one
  unit, so that the pair still compiles. Moving one file alone gives you
  dangling includes.

## What is here

| Generation | Files | Moved |
|---|---|---|
| arbor v1 (recursive descent, January 2026) | `include/arbor*.h` (v1), `lib/arbor.c`, `lib/arbor_{lexema,praeparator,syntaxis,formator,index,typus}.c`, `probationes/probatio_arbor{,_formator,_index,_lexema,_praeparator,_syntaxis,_typus}.c`, `probationes/fixa/arbor_index/` | 2026-10-19 |
| arbor2 (hand-written GLR) | `include/arbor2_*.h`, `lib/arbor2_*.c`, `probationes/probatio_arbor2_*.c`, `probationes/probatio_compound.c`, `ARBOR2_MAP.txt`, `scripts/validate_glr_counts.sh` | 2026-10-19 |
| lapifex as a C89 parser | `include/lapifex_c89.h`, `lib/lapifex_c89_grammatica.c`, `include/lapifex_glr.h`, `lib/lapifex_glr.c`, `probationes/probatio_lapifex_glr.c` | 2026-10-19 |
| lapifex C89 suites | `probationes/probatio_lapifex_c89_*.c` | 2026-09-02 |
| orphaned consumers | `lint/lint_probatio.c` + `run_lint.sh`, `tools/glr_debug.c` + `glr_debug.sh`, `tools/glr_quaestio.c` + `glr_quaestio.sh` | 2026-10-19 |
| officina goldens of the suites above | `officina/probationes/fixa/stdout_nativa/probatio_{arbor*,lapifex_c89_*,lapifex_glr}.stdout` | 2026-10-19 |
| materia (old) | `materia/` | earlier |

**Lapifex is split, on purpose.** `lib/lapifex_generare.c`,
`lib/lapifex_lexere.c` and `lib/lapifex_parsere.c` are **alive**: they are
the engine under nuntium's schema compiler (`lib/nuntium_schema.c`), and
the box comment in `include/lapifex_generare.h` says so. Only the parts
that served the retired C89 role are here. Do not "finish the job".

**Kept outside the archive:** `probationes/fixa/roundtrip/` (the shared
v1+v2 corpus that silva adopted and still runs), and the 29 design
documents in `project-specs/`. silva's orientation sends future sessions
back to them.

## Why moved, not deleted

spec-v2 §4.1: "a deleted test suite is a test suite nobody ports".
`probatio_arbor_typus.c` and `probatio_arbor_index.c` still hold the bars
for silva milestones that have not happened yet (the type resolver and the
project index). The quaestio and formator bars were harvested into silva
before the move. The full analysis is in
`project-specs/generationes-praecedentes-archivandae.md`.

## Frozen residue (examen code 58, VANA)

Decree 01KXSAK5K7HW (Fran, 2026-07-17) kept the dead comparisons in the
legacy parsers as named, visible residue: census 58 = 8, not fixed, not
tolerated, not excluded. Moving them out of the swept tree would have made
that count drop silently. Decree **01M5APKK3WMT** (Fran, 2026-10-19)
supersedes it. Archiving is now the form of visible frozen residue, and the
sites are listed here. They were verified after the move with
`./silva/examen.sh <via>`:

- `archivum/lib/arbor2_glr.c:382`
- `archivum/lib/arbor2_glr_tabula.c:25882`
- `archivum/lib/arbor2_glr_tabula.c:25892`
- `archivum/lib/arbor_praeparator.c:2897`
- `archivum/lib/arbor_praeparator.c:2928`
- `archivum/lib/arbor_praeparator.c:2955`
- `archivum/lib/lapifex_c89_grammatica.c:4116`

The eighth (arbor_quaestio) had already left the tree. In the live tree,
census 58 is now 0, so any new code-58 site outside `archivum/` is a real
defect, not residue.

## Known defects at the time of archiving (not fixed: frozen)

The library audit of 2026-10-18 found these in the archived code. They are
recorded so that anyone who restores a generation knows what they are
restoring. Ledger: the per-library parks under the audit park
01M56KN2H67C, closed as moot when the code moved.

- **arbor_syntaxis: `recursio_profunditas` is never reset.**
  `arbor_syntaxis_creare` leaves it as whatever the piscina held, and a
  parse aborted at depth leaves it high for the next parse. The parser then
  sees "too deep" at once, or wraps, and runs away. This is the likely cause
  of `probatio_arbor_syntaxis` reaching 49 GB (compressed footprint) on
  2026-10-19. The fix was proven by a probe, then not applied because the
  family was archived. It is two lines: `syn->recursio_profunditas =
  ZEPHYRUM;` in `arbor_syntaxis_creare`, after `syn->keyword_macros =
  NIHIL;`, and again at the start of `arbor_syntaxis_parsere`.
- **arbor_syntaxis: variadic prototypes (`...`) never terminate.**
  `_parsere_translation_unit` only guards progress when the declaration
  comes back NIHIL. At `...`, `_parsere_external_declaration` returns
  non-NIHIL without advancing, and the parameter loop has no
  `ARBOR_LEXEMA_ELLIPSIS` case. The result is an infinite loop with
  unbounded allocation. This is a limit of the old parser, not a fix
  waiting to happen.
- **arbor2_glr: P17 (`statement_list -> statement_list statement`)** grows
  the body Xar in place, and that Xar is shared between GLR forks. An
  ambiguous statement is written once per surviving interpretation.
- **arbor_lexema_omnia stops at the first ERROR** (line continuation, `\f`,
  `\v`, bytes >= 0x80): the promised EOF never comes and the rest of the
  input is lost.
- **arbor2_expandere RESET_PER_PARSE:** a nested `processare` clobbers
  `included_viae`, so an include can be included twice, and a
  self-including header recurses until the stack overflows.
- Medium and low findings for arbor_syntaxis, arbor_lexema, arbor2_glr,
  arbor2_expandere, arbor2_lexema and arbor2_token are in the audit report
  and its parks.
