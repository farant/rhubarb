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

## 2026-10-23 - partitura_notam_legere (E1 of edit-by-position)

Inverse of partitura_notam_scribere, filling the same PartituraNota that orthographia
returns (signature deviates from the drafted `littera/alteratio` out-params - one type for
both directions). Octave belongs to the LETTER (Cb4 = 59, B#3 = 60), same as the writer.
Accepts lowercase letters for agents ("g2", "bb4" = Bb4); accidentals not mixed (C#b4
refused); one octave digit with optional '-' (C-1 = 0, C10 refused); 0..127. Double
accidentals are READ (C##3) - it is emendatio, not the reader, that refuses them because
MML writes only one.

## 2026-10-23 - lineae additae: list (PartituraAddita[PARTITURA_ADDITAE])

The single extra line (additae_titulus / addita / additae_ctx) became a short ordered list
of {titulus, scribere, ctx}, so analysis lines stack (explicatio + functio, plus harmonia
if wanted). The list ends at the first entry without titulus or scribere - callers memset
the forma (all 7 did); the title column widens to the longest title. All callers migrated
in one commit (harmonia/explicatio docs, recensio, auditio, three suites); output
byte-identical. Plants 3/3 red ('dum' -> 'si' = one line only; width ignoring the extra
titles; ctx not passed - caught by the explicatio suite, which is the one that uses ctx).

## 2026-10-24 - leading tone spelled as the raised 7th when no letter is written

partitura_orthographia, after the in-scale lookup and before the natural/sharp/flat
fallback: the class a semitone under the tonic is spelled on the letter under the tonic
(C# in D minor, F# in G minor - not Db / Gb). Tonic letter = q*4 (+5 in minor) mod 7, one
fifth = 4 letters. It matters for classes nobody wrote (implied chord tones in
explicatio); written letters still come first.

## 2026-10-24 - bar header shows WRITTEN tempo changes (desideratum ...VG8PQ3)

Carmen.tempora became the derived sounding map (a ramp = dozens of 32nd-note steps), so
the bar header now walks Carmen.tempora_scripta: "tempus 90 usque 4 (+960)" for a ramp,
plain "tempus 90" for a step; bars inside a ramp print nothing. The title's tempo is
tempora_scripta[0] (a ramp from bar 1 makes tempora[0] already slower). _mensura_pulsus
turns the ramp's end pulsus back into its bar number.

## 2026-10-24 - fermata in the bar header

"fermata x2" / "fermata x1.5 (+960)" after the tempo changes. _factorem_scribere prints
the factor as an integer when whole, else to hundredths without trailing zero (1.5, 1.25).

## 2026-10-24 - agogica in title and bar header

"carmen r: c maior, 4/4 agogica 2 1 2 1 2 1 2 1, 100" and "metrum 3/4 agogica
vindobonensis" - the lilt is printed as written (_inaequalitatem_scribere), nothing when
even.

## 2026-10-24 - dynamica in the bar header

"dynamica mf" (whole carmen), "dynamica m ff usque 4" (part, ramp) - the value as written
(mark or number), after tempo and fermata.

## 2026-10-24 - repetitio in the bar header

"repetitio a +2" / "repetitio a -1" / "repetitio a" at the destination bar. Transposed
notes carry no letter, so they are spelled from the key (E4 - 1 in C maior = D#4).

## 2026-10-26 - '/' mark for portamento (RTB1F)

A note with `continuatur` prints '/' after its value where it ENDS ("C4:4/"; a note
crossing a bar line prints "E4:2~" then "~:2/"). Chords never carry it (MML parts are
monophonic; generated accompaniment never sets it).
