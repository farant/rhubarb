# tintinnabulum worklog

## 2026-10-22 - notification player (S4 step 4, library half)

The sound half of vicus notifications, kept out of the 3000-line
`vicus_applicatio.c` so it can be tested without a sound device. The host
says "occasion `finis`", the bell picks a `finis` jingle from the
cantuale (never the same twice) and plays it through an output given as
a function table. vicus adapts `auditus`; the suite gives an offline
`mixtura` with a hand-cranked clock (256-frame blocks, host ms advance
even while the device is closed).

Rules (Fran, 10-22): newest only, one at a time, queue of one (a newer
occasion replaces the waiting one), lazy device (open on first sound,
close after 30 s quiet), mutes belong to the host.

**Choose at start, not at arrival.** The waiting slot holds only the
genus; the jingle is picked when it starts. A replaced waiting occasion
consumes nothing, and "avoid the last one" only ever sees jingles that
actually played.

**`tacere` cannot close at once.** Closing the unit mid-waveform clicks
and the fade would never be heard. It sends SISTERE (5 ms fade) to every
voice, drops the waiting occasion, and `agere` closes as soon as the
fade's VOX_FINITA messages are in, not after the 30 s quiet.

**A lost message must not hold the device open.** Each jingle records
`terminus` = last tail end + 1 s in frames; past it the voices count as
finished.

**`pulsare` takes no clock.** The approved header had `pulsare(t,
genus, nunc)`, but quiet is measured in `agere` and `pulsare` never
needed it - removed rather than kept as a dead parameter.

**Three plants first stayed GREEN** - each a real gap in the first suite:
- `vitanda` ignored: one draw from two jingles passed by seed luck. Now
  20 jingles in a row, never the same twice.
- `tacere` keeping the waiting occasion: harmless while closed, but it
  would play after the NEXT jingle. Now asserted.
- 16 voices instead of 32: the 32-note jingle had short notes, so ~5
  overlapped and 16 sufficed. Now the 32 notes are `@ventus intactus`
  (2.5 s tails over 0.5 s): all 32 sound at once, 16 voices steal
  (FINIS_SURREPTA counted).
After the fixes all 9 plants red then green.

Suite: 7 sections (pulsare, exspectatio, quies, tacere, vitanda,
recusat, voces).
