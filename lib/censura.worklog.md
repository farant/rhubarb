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

## 2026-10-23 - rule 'tertia' (M5b, wish …SV2FPM)

Opt-in rule from composing 'stella': at a strong beat, >= 3 sounding notes (all parts, written-score view like the other rules - mutes ignored) whose classes form a no-third shape (harmonia_sine_tertia: unison, fifth, 7 or maj7 without third). Acceptance: stella's first draft fires at bars 1, 9, 15 - exactly the three bars I had fixed by reading the chord row; the saved stella, Korobeiniki and vesperae are clean.

Two design refinements found while testing (Fran told after):
- **Once per sonority.** A whole-note chord in 4/4 fired at beat 1 AND beat 3. Now a strong beat reports only if some sounding note started after the previous strong beat; a no-third chord formed on a weak beat is still caught at the next strong beat. Plant 'sustentum iteratur' proves it.
- **Output count in Arabic** (`voces 3`, not the approved `voces III`): every other censura line uses Arabic numbers, and the writer has only a ChordaAedificator - numerus_romanus_scribere needs a piscina, so Roman would have meant a second numeral writer.

The finding points at the LOWEST sounding note (pars/nota/frustum/offset); the writer recomputes the other notes from carmen + pulse (`_omnes_sonantes`, stable insertion by pitch). A plant on a per-note part-index array survived because only the lowest note's part was ever read - so the helper now reports just `pars_infimae` (first strict minimum in part order). Also folded censura's private `_notam_scribere` into `partitura_notam_scribere` (third copy of the note writer; same output - censura never writes '@' notes). Lint bit on `imae` again (pars_imae) - infimae.

## 2026-10-23 - _fortia on carmen_tempus_mensurae

Strong beats are now counted in units of the shared beat: bar start; 4/x the third beat
(initium + 2 * tempus); compound meters (tempus > 1/denominator) every beat. Same output as
before (all suites green). New test: 3/4 D5/C3 on beats 2 and 3 is NOT reported - nothing
covered "simple meter weak beats stay weak" before (plant survived).

## 2026-10-23 - N2: dissonantia judged through explicatio, ON by default

A strong-beat dissonant pair (same pair test as before) is now looked up in explicatio:
the segment of each note sounding at that pulse (last segment of the note starting at or
before it). EXCUSED if either note is an explained non-chord tone (transitus, vicinus,
innixa, anticipatio, effugiens) or a RESOLVED suspension / retardation / seventh, or if
both are chord tones of a NAMED chord (G#dim's own tritone). REPORTED otherwise, with a
static causa on the inventum: 'nota sine explicatione', 'suspensio / retardatio / septima
non resoluta', 'acordum sine nomine'; the writer appends ' - causa'. No explicatio
(memory) or no segment (muted part) = reported as before, causa NIHIL.

censura_iudicare has no piscina in its API, so it makes its own (piscina_generare_dynamicum)
only while dissonantia is active and destroys it before returning - inventa hold carmen
notes and static strings, nothing from the arena.

Library songs under the new defaults: korobeiniki 0 (was 5 - E7 sevenths/tritones and
passing tones explained), stella 0, vesperae 0, pastorale 0. I PREDICTED stella 1 (the A7
seventh G5 that leaps in bar 14) - wrong: it sits on beat 3, a weak beat in 3/4, and
dissonantia judges strong beats only. An unresolved weak-beat seventh is voice leading,
not strong-beat dissonance - a candidate separate rule (told Fran).

Found on the way: the dissonance line computed the interval as (nota - nota_altera) mod
12 in PART order - when the lower part comes first (bassus/media), a seventh printed as
'secunda'. Every old test had the melody first. Fixed: from the absolute distance;
test 'G2 contra F4, septima'.

## 2026-10-23 - ambitus: chorda 28..88, campana 48..96

New tibia patches (Karplus-Strong string, FM bell) get default ranges; table AMBITUS[V].

## 2026-10-24 - clavis: tertia Picardiae licita (desideratum ...CSJZHF)

The key rule flagged the F# of a final D major in D minor (completorium, 2 findings).
_picardiae_nota: in a minor key, a note whose class is the major third of the tonic
(VII*quintae + IX + IV) is skipped when its window satisfies functio_tertia_picardiae.
Only that one class is excused - any other chromatic note in the same window still counts.
The explicatio is now built when CLAVIS is on too (before: only for DISSONANTIA); a
CLAVIS-only forma pays for the window analysis. Six house songs: only completorium changed
(2 -> 0).

## 2026-10-24 - tertia: judged against the window chord (desideratum ...D03RH)

The rule judged only the instant: vigilia's arpeggio (E2 E3 B3 at the downbeat, G4 on
the fourth eighth) read "voces 3 sine tertia" on 18 downbeats although the explicatio
names the bar Em. Now the instant still triggers (>= 3 voices, a new attack, classes
without third), but _fenestra_cum_tertia stays silent when the window containing the beat
names a chord with a third (window classes = after non-chord-tone removal; sus counts as
"with" since harmonia_sine_tertia treats sus as deliberate). A bare-fifth window still
fires, also when the third only arrives in the next window. Shared lookup
_fenestra_ad_pulsum (index, -I if none) now serves both this and the Picardy skip.

The window's genus is not checked: under an instant of >= 3 notes the window is never
empty, so a NULLA guard would be untestable code - dropped.

Explicatio is now built when TERTIA is on as well. Old 'tertia' fixture: bars 1, 3, 4
(windows Dm, A7, Dm) no longer fire; only bar 6 (D3 D3 D5, window "(D)") remains and still
carries the 'lowest equal = earlier part' and 'sustained once' checks. New 'fractum'
section: bar 1 arpeggio with G (silent), bar 2 E+B only (fires at both strong beats -
the arpeggio restarts), bar 3 G at mid-bar (silent: window Em).

House songs with +tertia: vigilia 18 -> 0, completorium 7 -> 0 (chant + drone over Dm,
C, F and the Picardy D); the others 0 -> 0; default rules unchanged everywhere.

## 2026-10-27 - step 2b: declared harmony in censura - clavis exception, new rule 'acorda'

D03TB part 1 + the new disagreement rule (Fran approved, Q1-Q3 of the declared-intent plan).
- clavis: a note outside the key that belongs to the declared chord at its onset is intended
  (lilia D7b9 F#/Eb: 5 -> 0 monita; oppidum Bbm6 Db). 'r' has no classes, so it excuses
  nothing - the explicit 'nullum' check was redundant (its plant survived) and was removed.
- acorda (ORD.): per declared window (ExplicatioFenestra.declaratum), a shapeless note outside
  the declared chord (EXPLICATIO_IGNOTA already means "outside the declared classes" - a second
  class check was redundant, removed), or a declared root / third that never sounds in the
  window. Once per window: "Bm declaratum, sonat Bm7 (A5)" - CensuraInventum.sonans carries the
  literal whole-window reading (acordum_ex_classibus of every class, bass = lowest note at the
  window start); up to two foreign notes (nota, nota_prior), or causa "radix/tertia deest".
House songs: no-<acorda> songs 0; campus 1 (Bm vs a held melody A - the author could declare
Bm7); proelium +3 of the same kind; oppidum 44 -> 40. recensio tests pinned the default rule
list (now 5). Plants 8/8 after two rewrites (one non-compiling form, one redundant check).
