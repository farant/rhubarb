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
