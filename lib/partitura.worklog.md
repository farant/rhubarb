# partitura worklog

## 2026-10-22 - M1: song as text (agent-facing music tooling)

Bar by bar, track by track: notes touching [bar start, bar end) in
order; a cursor from the bar start turns gaps into rests (largest value
first, dotted allowed), a note begun earlier into a remainder '~:v', a
note running past the bar end gets a trailing '~'. Same start + same
length = chord, sorted by pitch. Overlaps without that are written in
order, no rest (cursor never moves back) - MML cannot make them yet.

Spelling: the note's own letter wins (MML ALWAYS records it, so MML
songs spell as written); otherwise the key signature's degree, then a
natural letter, then '#' (keys without flats) / 'b'. The octave follows
the LETTER (Cb4 = MIDI 59, B#3 = MIDI 60).

MML only has power-of-two lengths (dots allowed): triplets and raw tick
lengths are tested by editing the model directly, as the editor will.

Plants 20/20 red in the end. Survivor worth remembering: ignoring the
FLATS of the signature passed in Eb major, because the out-of-scale rule
("flat in flat keys") happens to give the same spellings there. A test
must pick a case where the two rules DISAGREE: Gb major spells B as Cb,
F# major spells F as E#. Three plants first failed to compile (unused
variable / parameter; a multi-line anchor the plant tool refused) -
rewritten, not findings.
