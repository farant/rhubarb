# functio worklog

## 2026-10-23 - F2: Roman numerals + cadences on explicatio windows (wish …5BHW2C)

Numerals from the explicatio window's chord: root degree in the key at the window (major;
minor = natural + raised 6/7 as their own degrees, so G#dim in A minor is viio, F#dim is
vio), case from quality, 'o' / 'h7' / 'M7' / '+' / sus / dyad '(3)' '(5)', inversion
figures from bass - root (triad "" 6 64, seventh 7 65 43 42). Roots outside the key are
ALWAYS the lowered degree above ('b': bVII, bVI, bII6) - header said "from spelling", but
key spelling would make Bb in C '#VI' (sharp-side default); analysts write bVII (told Fran).
Applied: dominant-shaped (major / dominant 7) or diminished-shaped chords that are NOT
diatonic, whose target (a fifth below / a semitone above the root) is a diatonic,
non-tonic, non-diminished degree -> V/V, V7/IV, viio7/V. B D F Ab in C stays viio7 (target
= tonic). C7 in D natural minor is diatonic -> VII7 (stella bar 7), not V7/III.

Cadences: arrival = window from a barline to the bar's end AND the top part (part of the
highest note at the downbeat) starts no note in the bar's second half (holds or rests).
First version used "last top note >= one beat" (as approved) and labelled almost every
bar of korobeiniki (quarter-note melody) and a false 'deceptive' at stella bar 5 (phrase
restart after a half cadence) - the second-half rule keeps pastorale 4 / 8 / 16 and
stella 4 / 16, drops the rest (told Fran). Tonic / dominant also accept dyads (cadences
drop the fifth: C E C) and a tonic unison (two-voice endings - vesperae). Half-note
melodies (korobeiniki B section) still read as arrivals every bar: the heuristic's limit,
form labels (#5) would make it exact.

Names: the lint flagged 'cadentia' - renamed to CLAUSULA (the medieval theorists' word
for a cadence): FunctioClausula, .clausula, functio_clausulae_titulus; DECEPTIVA -> FALLAX;
helpers _gradum_scribere, _finalis. Displayed labels unchanged ("perfecta", "decept.").

Plants 26/26 red; four first attempts did not compile (removing a call left a helper or a
variable unused; '&&' inside '||' without parentheses) and were rewritten; one survived
until a case was added: V6 -> I with the tonic on top must stay IMPERFECTA (the only V6
case had G on top, imperfect for the soprano reason, not the inversion).
