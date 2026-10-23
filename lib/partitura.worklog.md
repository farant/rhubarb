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

## 2026-10-23 - extra-row hook + public value writer

PartituraForma.additae_titulus / addita / additae_ctx: one analysis row
per bar after the tracks, title in the track-title column (width
included). Added so harmonia (which needs partitura's spelling) can plug
in WITHOUT a library cycle; later rows (dynamics, beat grid) use the same
hook. partitura_valorem_scribere is the score's ':value' grammar made
public for such rows. Callers that build a PartituraForma now memset it.

## 2026-10-23 - partitura_ambitus + partitura_notam_scribere (M5a, park …FXND4)

Found while composing 'stella': `auditio -carmina` printed the melody's range as `A#4..D6`, recensio as `Bb4..D6`. Two copies of "a part's lowest/highest note": auditio's (MIDI -> sharp-only name table, '@' sound notes counted) and recensio's (spelled via partitura_orthographia, '@' skipped). The one with the spelling won; it moved here because partitura owns spelling. `partitura_notam_scribere` is the existing internal `_notam_scribere` behind the same Scriptor wrapper as `partitura_valorem_scribere` - so a '@' note writes `@punctum`, same as in the score.

Contract worth remembering: the pulses in PartituraAmbitus are CARMEN pulses (frustum.pulsus + nota.pulsus). A note-relative pulse spells by the key at the start of the song - the test proves it with a littera-less note in a frustum starting in bar 2 under an Eb major change (Eb4 at the carmen pulse, D#4 at the frustum-relative one). Ties: first wins, at both ends (A#4 before Bb4; D# before Eb). 8 plants red; two only after adding a tied top note and an assertion on the lowest note's pulse in a frustum not starting at 0 (the first draft only checked pulsus_infimae == 0, which a dropped frustum offset also satisfies).
