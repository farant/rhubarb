# transformare worklog

Library parts and their traps live in silva/fontes/silva_forma.worklog.md
(T1-T4) and silva/instrumenta/silva_emendatio.worklog.md (T5). This log
is the tool and the rules library (T6).

## 2026-10-26 T6 rules library, wrapper, code 97 names its rules

Rules library silva/transformationes/<functio>.stml, one per raw write
function code 97 flags: remove unlink rmdir rename chmod mkdir fopen.
Every replacement checked against lib/filum.c / lectiones.h before
writing a rule, and each file's header comment says exactly where the
behaviour differs:
- remove -> filum_delere: identical (filum_delere IS remove() + S line).
- unlink / rmdir -> filum_delere: via remove(), so an empty DIRECTORY
  given to unlink, or a FILE given to rmdir, now succeeds. Accepted - it
  is what code 97 itself recommends.
- rename -> filum_movere, chmod -> filum_modum_ponere: identical + trace.
- mkdir: ONLY mode 0755 (filum_directorium_creare = mkdir(p, 0755)).
  _cum_modo chmods an existing dir and _si_necesse treats existing as
  success - both change meaning, so other modes stay manual (11 of 50).
- fopen write modes -> lectiones_fopen: same signature and FILE*, so
  the rules match the CALL itself, one rule per mode literal. Read modes
  are code 96 debt, not 97 - left for later (and never route a secret
  through lectiones).
errno survives every failure path (lectiones notes after a successful
remove, before rename/mkdir).

Two tool features the real tree demanded:
- GENERATUM files (line 1) are never rewritten: the census found
  officina/amalgama/officina.c - rewriting a generated file is undone by
  the next generation. Message says to rewrite the generator's sources.
- <inclusio>filum.h</inclusio>: a rewritten file without the header gets
  `#include "filum.h"` after the last `#include "..."` before the first
  edit (keeps postulata_posix.h first). Without it the guard refuses
  every file that lacked the header (implicit declaration grows a code).

Left manual by design: officina's machinula implements remove/unlink/
rename/mkdir FOR its interpreter `(s64)remove(...)` - it must keep the C
int convention; calls inside CREDO_* macro arguments ("in macro").

code 97's message now ends "; emendatio mechanica: ./silva/transformare.sh
-regulae silva/transformationes/<f>.stml <viae>" for exactly the seven
functions with a rules file (_regulae_mechanicae in silva_c89_semantica.c
- keep that list and the directory in step).

Plant reporting quirk: silva.planta(..., 'pythonica') summarized the red
run with text from pythonica's OWN fake-gate fixture ("planta rubra:
ficta ... NIHIL PROBATUM"), not the failing check. Verified by hand that
both plants fail exactly "inclusio addita; GENERATUM intacta". The
compendium for the pythonica gate picks the wrong line.

## 2026-10-27 - the clang guard compiles ONE variant (found by fusio_probare)

Sweep VIII rewrote `materia/instrumenta/oraculum_silvae.c`'s output write to
`lectiones_fopen` and `<inclusio>` put `#include "lectiones.h"` in the file's
`#else` branch (live silva). The guard's before/after clang compiled only the
default configuration, so it passed - but `materia/oraculum_silvae_struere.sh`
also builds the file with `-DORACULUM_PIGNUS` against the frozen amalgam
(7a4847b0), where the header is absent: "implicit declaration of function
'lectiones_fopen'". Gate oraculum-silvae caught it, not the guard.

Fix in the file (not the tool): per-branch alias `oraculi_fopen` (fopen under
a TOLERA in the pignus branch - its ISOLATIO allows amalgam + libc only - and
lectiones_fopen in the live branch), like the file's piscina aliases.

Lesson for the tool: a file with `#if`/`#ifdef` branches can be compiled in
several configurations by different actions; the guard must compile every
variant the build actually uses (the -D flags are in the clang lectiones of
the traces), or at least warn "variants not compiled". Ledger …MDR6.

Same run, related: officina sessions judge turns exactly like examen, so the
97 flip made `remove("x")` a judgment-time REIECTUM before machinula's runtime
refusal - test B3 now pins both layers (direct call = codex XCVII; call
through a function pointer = MACHINULA_RECUSATIO, planted red). A gate run
with a filter (`officina.machinula`) at the sweep commit hid this - when a
sweep touches a subsystem, run its whole gate.
