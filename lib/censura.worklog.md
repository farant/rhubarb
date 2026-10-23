# censura worklog

## 2026-10-23 - M2b: the rule toolbox

Five named rules, each switchable (Fran 10-22: a toolbox with a default
configuration). Default ON: lineae ('|' off a barline), clavis (out of
key; minor allows raised 6th and 7th), ambitus (instrument range;
ordinary table aether 36..96, quadrans 48..96, triangulum 24..72, unknown
instrument = rule silent, or the caller's callback). Default OFF (style):
dissonantia (2nds / 7ths / tritone incl. compound, between tracks, on
strong beats: bar start; mid-bar in 4/x; every dotted quarter in 6/8,
9/8, 12/8) and parallelae (consecutive onsets of a track pair, both move
the same way, same perfect 5th or octave).

Findings point at the CarmenNota (not a bare MIDI number) so text spells
like partitura and a DAW pane can highlight the note; key names come
from partitura_clavis_littera (made public - one owner for naming).
Results: rules run in enum order, then a stable insertion sort by (bar,
tick) - ties keep rule order. Over capacity: the TOTAL is returned and
auditio reruns with exactly that capacity.

Korobeiniki is clean under the defaults; +dissonantia reports D5 over
G#2 tritones - the E7 dominant, i.e. legitimate harmony, which is why
style rules default off.

Dropped a redundant check while planting: "both notes are new" in
parallelae is implied by "both moved the same direction" (a held or
repeated pitch has zero motion). Test fixes found by the first run were
MY counting (offsets) and one real test bug: the dissonance song had an
F# in C major, so the DEFAULT rules (clavis) fired - moved it to G major.

## 2026-10-23 - saltus rule + parallel findings show both pairs

From composing 'vesperae' with the tools (ledger wishes ...DJHY,
...WS0V). saltus: consecutive notes of a track (across rests; a chord =
one onset, so its own notes are never a "leap"; percussion skipped) more
than saltus_maximus semitones apart (0 = XII). I expected it to be
default ON ("a leap over an octave is almost always a forgotten < or
>"), then measured: Korobeiniki's octave-alternating bass has 7 real
leaps of 13..19 (A3 -> D2 at a chord change). So it is a style rule,
default OFF, with a settable limit; the liber test pins the 7 so the
reason survives. Parallels now carry nota_prior / nota_altera_prior and
print 'quintae parallelae C5/F4 -> D5/G4' - composing, I had to
reconstruct the first pair by hand.
