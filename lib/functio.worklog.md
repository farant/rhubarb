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

## 2026-10-26 - 43HA step 3: key changes (dominans praevia)

Probe: a refrain in F, D7 bar, refrain repeated +2 in G (<clavis> + <repetitio
semitonia>). The repeat read right in G already (functio takes the key per window), but
the pivot D7 read V7/ii in F, and D7 -> G (tonic on top) got NO cadence: the arrival's
prior window was an applied chord, so _dominans failed.

Rule: the last window before a key change, if it is the new key's dominant (V/7/9/7b9/(3)
on the new fifth, or o/o7/h7 a semitone under the new tonic), is read in the NEW key
(`_clavis_fenestrae`, FunctioGradus.praevia). The line names the key before it: "G: V7:1"
(minor lowercase, "e: V7:1"). ii-V before the change: only the V moves (Am7 stays iii7).

Picardia: prior and proxima are now read in the key of the judged window (dominant OF
THIS key, IV OF THIS key) instead of each in its own key - matters only around key changes
(A7 before d minor -> D is Picardy; G7 pivoting to C after D in d minor is still IV7 ->
no Picardy). A pivot window itself needs no extra check: its key is the new one, where
its root is never the tonic (first plant of a 'praevia ||' guard survived for exactly
that reason - removed). The cadence loop keeps _clavis(w->initium): a window reading as I
can never be a pivot, so the two are equivalent.

House songs: none changes key; all seven byte-identical before/after.

TRAP (ledger …VWJKVB, links …MQ7RN9): a debug-printf swap and restore within the same
second left the DEBUG object linked (aedilis vetustum() is mtime -nt) while provenance
recorded the clean digest - fabrica then called the stale binary fresh, and my first
before/after comparison silently compared new vs new. Check with `strings bin/auditio`,
fix by deleting build/aedilis/obiecta/lib__functio.o.

Plants 10/10 red (the redundant guard's plant replaced by one on the key choice).

## 2026-10-26 - review batch 1.3: pivot never on the old tonic; rests skipped; ii65 (double emploi)

From the sexta review (harmony findings 2, 8) and Fran's calls ("ii65 and Iadd6 at final",
"picardy sounds good").
- Pivot misfired on the OLD key's tonic when the new key is the subdominant: C: I IV V7 I | F
  read bar 4 "F: V [dimid.]" and bar 5 a false "[imperf.]"; A7 -> D in d minor before G minor
  lost its Picardy third (and censura re-flagged the F#). Now a window whose root is the old
  key's tonic is never praevia. Step 3's test (a) (D after A7 in d minor, then G major) flips
  from "V" to Picardy I, by Fran's decision.
- Rest windows between the dominant and the change are skipped (_proxima_sonans) - both for
  the pivot test and for the line writer's key name ("G: V7:2 -:2", not "F:").
- DUPLEX USUS: a 6/m6 chord on IV followed by a dominant (V/V7/V9/V7b9 via _dominans, or a viio
  form) is re-read as the seventh chord on II: root + 9, genus m7 / m7b5, bass unchanged ->
  "ii65" / "iih65". Second pass after the numeral loop (needs the next window's numeral). IV6
  before I stays IVadd6 (lilia bar 9 unchanged); I6 stays Iadd6.
- Tests also close test-strength gap 14 (pivot dominant forms): D9 before G -> "V9" praevia;
  parallel key G7 in C before c minor -> "c: V7:1".
Plants 9/9 first try. House songs byte-identical.

## 2026-10-27 - F1: tritone substitution (subV)

A post pass (after the first reading and the 6 -> ii65 pass, before clausulae), because subV
is decided by the NEXT sounding window: a dominant shape WITH a seventh (7, 9, 7b9, 13, 7#9,
7#11), outside the key, not applicata, whose root is a semitone above the next root, target
a diatonic major/minor degree (I included) -> "subV7", "subV7/ii", "subV7#11". FunctioGradus
.substituta; applicata = target (0 for I). Without the resolution it stays bII7; a triad stays
bII (Neapolitan). Cadence: subV -> I counts like V -> I (Fran: yes).
Guards: the diatonic check is reachable only in minor (G7 in a minor falling to the raised VI
F# would read subV7/VI) - tested there. The applicata exclusion and _meta_idonea guard are not
reachable by any common case I found (every V7/x that falls a semitone lands outside the key;
the only diminished target, VII, is always reached by an applicata C7) - kept as header
conditions, unplanted.
Refactor: _figura_acordi (figure only for gradus III without added tones) now shared.
House: nocturnum bar 7 ii7 subV7 -> bar 8 Iadd6 [perf.]; bar 5 Bb7 -> Am7 reads subV7/iii (it
IS the tritone sub of E7 -> Am - IV7 was the wrong name). Classical songs unchanged. Plants 6/6.

## 2026-10-27 - F2: blues key (clavis caerulea)

Declared on the KEY, not the style, because functio reads keys and never styles:
clavis="f maior caerulea" (also <clavis valor>, and minor). CarmenClavis.caerulea; one writer
partitura_clavis_modus for every printed key name (partitura, censura, recensio, auditio).
censura: b3 b5 b7 of the TONIC are in key. Subtlety for minor: b5 of the minor tonic is the
same class as b3 of the relative major (A: D#/Eb = C: Eb), so a test on D# cannot tell a
wrong tonic apart - Bb (b7 of C, not blue in A) does; the test uses it.
functio: blue notes do NOT enter functio's scale (else Bdim7 in F would be "diatonic" and read
bVo7, G7 would read II7). Instead: a dominant shape on I or IV is never applicata (I7, IV7),
and an unapplied dominant shape on I may be a cadence arrival. Reachability: in major IV7 is
never applicata anyway (its target bVII is outside the key); in minor plain D7 is diatonic (VI
raised) - the IV clause only matters with a non-scale extension (D7b9 in a: V7b9/VII vs IV7b9),
tested that way. The "k.caerulea" guard in the arrival clause is unreachable without blues (I7
is always applicata outside it) - kept as stated intent, its plant survives by construction.
House: ambulatio now in "f maior caerulea": I7 IV7 I7 v7-I7 ... turnarounds into bars 13 and 25
are cadences [imperf.] (top note G4 in the comping, not F); censura 4 -> 0 (B2 = blue b5).
Plants 8/9 (the survivor above). Note: tools/auditio.c has ~10 pre-existing lines over 72 -
the commit gate does not catch them; not touched here.
