# vicus_quaestio_porta worklog

## 2026-10-14 — born (quaestio-rivus T8, ruling R10)

The real $question port: `bin/quaestio` via `processus_incipere`
(180 s hard limit), frames read per pulse from
`processus_effusio_hactenus` (parsed immediately - the buffer may move
on the next pulse), raw text collected and planned
(`quaestio_textum_planare`, R9). A helper that ends without an F frame
-> "helper exited without answer" + the processus error or the first
200 bytes of its stderr; a malformed frame -> killed, "helper: bad
frame"; abort = `processus_abrumpere` (SIGKILL + reap).

Moved out of `apps/vicus/vicus.c` (where it was `interior` and
untestable): the plan's Review Focus promised tests for a dying helper
and Esc mid-write, which only a real process can exercise.
`probatio_vicus_quaestio_porta` drives the real port against
`probationes/fixa/quaestio/adiutor_fictus.sh` (frames per scenario:
bonum, mors, pravum, error, lente) - no key, no network. Written before
its test (moved code), so proven by plants instead: "no F" detection
removed -> mors red; abort without kill -> lente red.

`vicus_quaestio_adiutoris_via(argv[0])`: helper next to the binary
(plan R4: vicus runs from the tree via vicus.sh, not installed).

## 2026-10-14 — closure too big for silva's census (frigida 2e4172e0 red)

frigida on 2e4172e0: 16/17 green, silva RED -
`probatio_silva_arbor_plagula`: `clausurae_truncatae == 0` failed. The
silva test apparatus caps one file's include closure at 96 headers
(`silva/probationes/apparatus.h` CLAUSURA_MAXIMA); this file had 117
because it included `vicus_applicatio.h` (the whole app stack) only
for the two port types. The commit's owed gates did not include the
silva suite (nothing in silva's coverage changed), so only the
fresh-clone run saw it. Fix by design, not by raising the cap: the
types moved to `include/vicus_quaestio.h` (types only), included by
vicus_applicatio.h and by the port header -> closure 46. Largest lib
closure now vicus_applicatio.c at 91 (measured over all lib/*.c with
`bin/aedilis <f> --partes | grep -c '^C'`). Lesson: a new lib that
includes an app-level header can blow silva's census - measure closure
when a header pulls in a "hub".
