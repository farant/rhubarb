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

## 2026-11-11 - read-mode fopen: rules file fopen_legens.stml (code 96)

silva/transformationes/fopen_legens.stml: `fopen($via, "r")` and
`"rb"` -> `lectiones_fopen`, codex 96, `<inclusio>lectiones.h`. Same
reasoning as the write modes: same signature and FILE*, so the rule
matches the call whatever consumes it. Census over the 139 debt-listed
files: 181 fopen sites - 178 reading ("rb" 152, "r" 26), 2 writing
(tolerated), 1 variable mode (officina amalgam, GENERATUM). Dry run:
174 sites in 131 files, 0 refused.

Kept OUT by hand before running: apps/forum/forum.c (the server token,
already `<tolera>`), apps/quaestio/quaestio.c (the API key - gets a
`<tolera>`, never lectiones), knotapel snapshots, officina's amalgam.
The tool does not know a secret from a fixture; that list is the
operator's job every time this rules file is used.

Two things the mechanical rewrite got wrong in area 1, fixed by hand
(both would have passed the tool's guard):
- materia/instrumenta/oraculum_silvae.c is ALSO built -DORACULUM_PIGNUS
  without lectiones.h (the 2026-10-27 lesson again): the read now goes
  through the file's own `oraculi_fopen` alias.
- materia/fontes/materia_coctor.c already noted its read by hand
  (`lectiones_notare` LEGIT/ABSENS around a raw fopen): after the
  rewrite it would have noted twice. Hand notes removed. Grep the diff
  for `lectiones_notare` next to a rewritten call in every area.

Area 1 (toml css html md materia tessera: 14 files, 15 sites): all 14
debt rows paid and deleted (139 -> 125); six suites green; `md/arbor.sh
README.md` under FABRICA_LECTIONES now records `L README.md`.

## 2026-11-11 — fopen_legens: oratio (area 3)

41 sites in 30 files (2 fontes, 10 tools, 18 suites), 0 refused; 26 files
gained the lectiones.h include. No oratio amalgam, no -D variants, no hand
read-notes next to the sites. Nine of the ten tools are INTECTA (no gate),
so they were compared by hand: 12 invocations (arbor, arbor -partes -sedes,
computus, formae, oraculum -machina, quaere, reticulum, sententiae, verba,
vocabula -nova, vocabularium, vocabularium -coquere) old code vs new,
stdout + stderr + rc identical except timing columns. `oraculum -regulae`
is a 10+ minute cumulative run - do not put it in a differential.
30 debt rows deleted (95 -> 65).

## 2026-11-12 — fopen_legens: officina (area 4)

27 sites in 17 files (officina_indicium, praeparator, six tools, nine
suites), 0 refused. Order held: transformare -> formator on the fontes ->
source lists (unchanged) -> amalgamare (VERIFICATUM, hospes 81/81). Tool
differential old vs new, 7 invocations (cursor -enumerare, cursor -sola,
fusor lib, interpretare x2, sonda lib/chorda.c, colloquium scalaria.c):
identical except timing. `cursor -sola probatio_chorda` is rc 1 on both
sides (DECIPULA getpid) - pre-existing. vindex compiles but does not link
at HEAD (known), so its rewritten site is unexercised. 14 debt rows deleted
(65 -> 51); three files keep their row (other raw IO remains).

Addendum (officina): the amalgam `officina/amalgama/officina.c` carries its
OWN debt row. After the fontes were rewritten and the amalgam regenerated,
that row was paid too, and examen-corpus (deferred gate) went red with
"linea delenda". Run the debt check over ALL remaining rows after an
amalgam regeneration, not just the files handed to transformare.
(50 rows remain.)

## 2026-11-12 — fopen_legens: briar (area 5)

17 sites in 16 files (three tools, thirteen suites), 0 refused. No amalgam,
no hand notes. The three tools (arbor, computus, fabrica) are INTECTA:
compared old vs new - arbor x2, computus, fabrica x2 (app_vestitum and
derivatum projects, 195 generated files, diff -r) identical except
computus timing columns. 15 debt rows deleted (50 -> 35), checked over all
remaining rows.

## 2026-11-12 — fopen_legens: crusta (area 6)

14 sites in 13 files (crusta_facies, crusta_oraculum, four tools, seven
suites), 0 refused. `crusta/instrumenta/interpositio_macos.c` was on the
list but has no fopen call (a dylib injected into bash - it must never
link lectiones anyway); left out, examen ACCIPE. No amalgam. crusta_facies
is also linked by tools/diagnostica.c and crusta/instrumenta/effectus.c
(hook tools) through aedilis closures - gates diagnostica, unci,
iudicium-fumus cover them. Four INTECTA tools compared old vs new (arbor
x2, coctum, computus, facies x2): identical except computus timing.
13 debt rows deleted (35 -> 22).

## 2026-11-12 — fopen_legens: gesta (area 7, last mechanical area)

8 sites in 7 files (gesta.c, tabularium.c, five suites), 0 refused. Read
by hand first: the two library sites read the annals jsonl, nexus.tsv, the
identity and citation indices and repo files named by a scope - no secret
passes through them. No amalgam; the four source lists regenerate
unchanged. Everything is gate-covered (no INTECTA). 7 debt rows deleted
(22 -> 15). What remains of the 139 is by hand or deliberately raw:
apps/quaestio (API key - tolera, never lectiones), apps/forum (tolerated),
knotapel snapshots, the channel itself and files with other raw IO.
