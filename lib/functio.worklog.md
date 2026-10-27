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

## 2026-10-24 - tertia Picardiae (desideratum ...CSJZHF)

A major tonic in a minor key used to be named V/iv (D major in D minor is the dominant of
G minor and not diatonic, so the applied-chord branch took it). completorium ended "V/iv".
New predicate functio_tertia_picardiae(carmen, explicatio, fenestra): minor key, root =
tonic, genus MAIOR or TERTIA_MAIOR, and either (a) the last sounding window, or (b) a
mid-piece arrival (_adventus: the window fills its bar) right after a dominant, whose next
window is not on IV (then it really is V/iv). _gradum_scribere gained 'b32 picardia' and
skips the applied branch when set, so the chord reads I and the cadence logic sees V -> I
(completorium's last bar is now "I:1 [perf.]").

The predicate calls _gradum_scribere(..., FALSUM) for the neighbours - no recursion into
the predicate, and the neighbour labels are the plain ones (good enough: the previous one
only needs to be dominant, the next one only needs a scale degree).

Tests: four-voice A7 -> D (V7 -> I, PERFECTA), i iv I at the end, V/iv iv V i stays
applied, mid-piece i V I V i true, i V I iv V i false (the next chord is iv: V/iv), i iv
I V i false (no dominant before), half-bar D after A false (not an arrival), a final D+F#
without fifth true, a final Dm false, D major key false. The first set of fixtures doubled notes and the
analysis read "I(3)" dyads - write full triads.

Plants 12/12 red, after three rounds: two first attempts did not compile (removing a use left
a parameter / helper unused - write '!(x && FALSUM)' instead) and two survived until cases
were added: the only V/iv case had D in window 0, which returns FALSE before the 'next is
IV' check; no fixture ended on a dyad (TERTIA_MAIOR). And one plant was itself wrong: '&& FALSUM'
on the genus test removed the whole check instead of rejecting the dyad - it survived
because nothing asked about a minor tonic at the end (now 'final Dm false'), and the
dyad plant proper is '!= TERTIA_MAIOR' -> '!= MAIOR'.

## 2026-10-26 - 43HA step 2: new chords in the numerals

Numerals from the row: Iadd6, Iadd9, V9, V7b9, V7sus4, V+7, V9/V. Inversion figures only
for gradus III rows WITHOUT additae (Iadd9 over E: no figure - "Iadd9 6" would misread).
Role lists extended: dominant forms (applicata and cadence) += 9, 7b9; cadence-final +=
6, m6, add9; picardy += 6, add9. Operetta endings (V7 -> Iadd6) are perfect cadences.
Plants 4/4 red first time.
