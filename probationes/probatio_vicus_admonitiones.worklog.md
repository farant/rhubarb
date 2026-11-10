
## 2026-11-09 — the 'systema' flake (l.206) was the terminal pane, not the clock or mensor

`CREDO_FALSUM(vicus_pulsare(v))` after the first pulse failed 10-20% of
the time under load (ledger question 01M5YTX3XD, seven hits, several
lost merge runs; 2 of 3 cold root runs red today).

Repro: 8 copies of the built test at once, each with its own TMPDIR:
11/48, 7/48, 31/160 red. Temporary `fprintf` at every place
`vicus_pulsare` sets `pingendum` (removed again): in all 38 red runs the
second pulse printed `latus 1_dextrum_terminale mutatum`. The app world
starts on tab 1 = scriba | terminale; the terminal's /bin/sh writes its
prompt whenever it gets scheduled, and when that lands between the two
pulses the visible pane reports a change. Never `hora`, never
`systema N`: the two causes guessed in the ledger (minute rollover, the
live mensor query) did not occur once.

Fix: the section switches to tab 3 (scriba | scriba) first, as
probatio_vicus_paginae already does before its own
`CREDO_FALSUM(vicus_pulsare)`. An invisible pane does not ask for a
repaint, so the assertion again means "the notifications system had
nothing new". After: 0/160 and 0/240.

The mensor hermeticity concern from the ledger is still real in
principle (the section runs the installed ~/.bin/mensor), it just is
not this flake.
