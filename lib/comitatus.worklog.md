# comitatus worklog

## 2026-10-26 - born (Q7G5 step 2): figures of accompaniment

Pure library below carmen: reads a chord progression (comitatus_acorda_legere, bars
given by the caller as start pulses), reads a figure (comitatus_figuram_legere), and
realizes notes (comitatus_componere). carmen.c turns <acorda>/<figura>/<comitatus> into
a generated CarmenFrustum. Header text approved by Fran; one correction told to Fran
before writing: 'Q' is the chord's OWN fifth (dim: diminished, aug: augmented), root + 7
only when the chord has no fifth - otherwise Bdim's alternating bass lands on F#.

Design notes:
- the note-length grammar is cantus's (divisor of the whole 1..64, up to two dots with
  divisibility), copied as _valorem_legere because cantus keeps it interior.
- voicing: close position, every inversion x every octave whose used voices fit the
  range; cost = sum of |moves| of the voices the figure uses (common count with the
  previous voicing), first chord = mean nearest the range centre; ties go lower. A range
  >= 12 always admits a voicing: start on the first chord tone at or above the floor -
  the k tones from there span 12 - (gap below that tone), so the top stays under
  floor + 12.
- the voicing changes ONLY when the chord changes (content compare: genus, radix,
  bassus, classes) - repeated "C | C" bars keep one voicing and alternate B/Q.
- figure bar index restarts when the chord at a bar start differs from the chord
  sounding just before it (pulse - 1), so "C:2 G:2 | G" puts Q on bar 2.
- 'Q' must differ from the bass note (C/G: other octave); if the range has no other
  octave it falls back to the bass itself (documented in the header).

Hand-computed waltz ("C | C | G7 | C", 55..76 / 36..57): C 60 64 67 over C3, Q = G2,
G7 -> D F G (62 65 67, cost 3 beats B D F at 5), C back to 60 64 67. All 9 sections
passed on the first run; 10 plants red (cost ignores previous, ties upper, figure never
restarts, Q not avoiding bass, roll without stagger, remainder not to last, overflowing
values accepted, gain not reset per figure bar, Q = root+7 always, letters not from the
chord).

Lint renames before commit: comitatus_realizare -> comitatus_componere (realizare is not
Latin; componere = put together), local fm -> mensura_figurae, inversio -> prima (real
Latin but unknown to the oracle - a gloss would move oratio's pins, a rename is free),
test helpers _valsa / VALSA_FIGURA / _sectio_valsa -> _liber_ternarius / FIGURA_TERNARIA /
_sectio_saltatio. Glossary: acordorum (gen. pl.) added to the existing acordum entry.
Locals need renominare -intra <function>; names inside CREDO_* arguments are "manuales"
(perl \b swap).

Observed in an auditio run (scratch waltz C C G7 G7 Dm7 G7 C C): with V = 3 voices on a
four-note chord the voicing uses the three LOWEST tones of the chosen close position, so
minimal motion from C E G picks D F G for G7 - the leading tone B is dropped. Standard
practice omits the FIFTH first. Raised with Fran as a follow-up (header says voices
1..min(V, k) of the close position).

## 2026-10-26 - fifth omitted when voices < chord tones; house figures; lilia (Q7G5 step 3)

Fran approved: when the figure uses fewer upper voices than the chord has classes
(V < k) the voicing drops the chord's FIFTH (_quintam: 7, else 6/8) before choosing the
close position - G7 in three voices is now B F G (59 65 67) from C E G, D7 is F# C D,
C7 Bb C E. V >= k: nothing omitted (four-voice G7 keeps G B D F). Waltz test updated (bar
3 was D F G); new tests: C in two voices -> E C (64 72), G7 in four -> all classes.

Trap seen while checking: bin/auditio built BEFORE the voicing edit still printed the old
D7 (A C D) - after touching lib/, rebuild the tool (bin/fabrica sanare bin/auditio) before
judging its output. The same sanare also re-embeds carmina.stml (lib/carmina_infixum.c).

House figures (carmina.stml, book level): valsa, marcia, albertina, harpa, harpa_ternaria,
pastorale. First song on them: lilia (F major waltz, chordae over harpa/valsa). censura
names the D7's F# three times (melody + two generated chord tones) - deliberate
secondary dominant; a censura rule that accepts tones of the DECLARED chord is the
natural follow-up (asked Fran). Mix rms about -22 dBFS vs -14 for the denser songs: the
harp decays and the peak is already -3.4 dBFS, so it was left there.

## 2026-10-26 - 43HA step 2: root omission

After the fifth, if the figure still has fewer upper voices than classes, the root goes
too (the bass has it): C7 in two voices E Bb, C9 in three E Bb D. Trap: the first test
(single C7, single C9) passed even with the rule planted OFF - minimal motion from the
middle of the range happened to pick E Bb / D E Bb. Discriminating test: a chain C7 F7
Bb7 Eb7 Ab7 Db7 in two voices, no upper note ever equal to its root - voice leading pulls
the root in somewhere without the rule.

## 2026-10-26 - 43HA step 4: added-tone chords drop the root first; lilia's harp splits

lilia's new progression (F6, C9, Bb6, Bbadd9, D7b9, Gm9 - Fran approved) exposed two
voicing facts. (1) In valsa the bass is a crotchet on beat 1, so the beat-2/3 windows had
no bass: with root omission C9 became Bb D E with no C sounding anywhere (`?`), F6 A D F
read Dm/A. Fix in the SONG: harp split into harpa_bassus (B:2. - a harp bass rings) and
harpa (r:4 [1 2 3] [1 2 3]), figures local to lilia. (2) Even with the bass, F6 minus its
fifth = F A D = Dm/F: for triads with an added tone and NO seventh (6, m6, add9) the fifth
is what tells them from their relative, the root is in the bass - so `conversus` drops the
root FIRST, then the fifth (F6 three voices A C D, two A D; Bbadd9 C D F). Sevenths keep
the old order (C9 four voices C E Bb D). Result: every lilia bar names its declared chord;
the end reads Iadd6.

Tests: F6/Bbadd9 classes, C9 in four, and a chain F6 C6 G6 D6 A6 E6 in two voices where no
upper note is ever the fifth - the single two-voice F6 case passed with the 'then the
fifth' step planted out (A D chosen by motion alone), same trap as C7/C9 in step 2. A
'!conversus' guard on the later root step was redundant (removed). Plants 4/4.
probatio_carmen's embedded-book check now expects lilia's three parts (16 bass notes, 96
chord notes).

## 2026-10-26 - review batch 1.1: the root only goes when a bass plays it; altered fifths stay; re-root

From the 4-reviewer sexta review (findings comitatus 1, 2, 8; Fran decided 8 = re-root).
(1) "drop the root, the bass has it" was false for slash chords (F6/A -> C D A, no F
anywhere) and figures without 'B' (albertina: C9 -> D Bb E). A 3x20k fuzz found ~7.5k
rootless bars per seed, all slash chords. Now the root may go only if the chord's bass IS its
root and a bass sounds: a 'B' voice in this figure (ComitatusFigura.bassus, set while
parsing) or `bassus_sonat` - new comitatus_componere parameter; carmen computes it as "some
<comitatus> in the song uses a figure with 'B'" (_bassus_comitatus), which keeps lilia's
split harp (chords part has no B, harpa_bassus has it) unchanged. A kept root must also be
among the voices actually used: with V < k the old search took the lowest V of ANY inversion,
which could still leave it out - now inversions without the root are skipped.
(2) fifth-first omission removed the defining altered fifth (Caug -> C(3)/E, C7#5 -> C7):
an altered fifth (proprietates quinta != VII: o, +, h7, o7, 7#5) is never omitted, so the root
goes instead (Caug two voices E G#; Cm7b5 two voices Eb Gb). An explicit 'altered => root
first' clause was REDUNDANT (blocking the fifth already forces the later root step; plant
survived) - removed; the 2-voice four-note case (Cm7b5, C7#5) is the only one where the fifth
block is observable, so that is the test.
(3) figure bar: new chord = differs from the chord at the start of the PREVIOUS bar (was: the
chord sounding just before the bar), so "C:2 G:2 | G" puts G (root) in bar 2's bass.
Plants 11/11 (two rewritten to compile, one redundant clause removed). House songs
byte-identical (swap built with the objects deleted - see …VWJKVB).
