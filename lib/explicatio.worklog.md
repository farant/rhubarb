# explicatio worklog

## 2026-10-23 - N1: windows, chords without non-chord tones, note genera (wish …DDCMRP)

**Window = while the bass holds** (Fran's choice over per-beat): the lowest sounding
CLASS, so an octave-jumping bass (korobeiniki E2 E3 E2 E3) is one window; barlines cut;
silence is a NULLA window. Built from event points (note starts/ends + bar starts), one
lowest-class per interval, adjacent equal intervals inside a bar merged.

**Chord = minimal removal of candidates.** A candidate is a note with a step or the same
pitch on either side (or sustained into the window - suspension shape); removal works on
CLASSES (a class is removable only if every note of it in the window is a candidate), the
bass class never. Subsets by size; first size with a name wins; ties: more notes kept,
then triad/seventh > sus > dyad > single class, then root = bass. The implied seventh
(tritone dyad -> V7, root by key: diatonic first, then the key's dominant) competes AT
ITS SIZE - before that fix, removing korobeiniki's D left '(G#)' (a single class counts
as named) and won over the E7 that only got tried after all removals failed. Sus ranks
below triads: G + C held + B is a 4-3 suspension over G, not Gsus4 plus a passing B.

Refinements made while testing, all told to Fran:
- root = bass tie-break (Bb A G over C E: remove A -> C7, not Bb -> Am7/C);
- passing seventh counts as resolved (korobeiniki C D E over E7 - else the Tetris theme
  is "wrong"); stella bar 14 G5 over A7 leaps to E5 and stays '!' (a real finding);
- BASSUS TRANSIENS (Fran chose option B): a stepwise bass note whose upper notes all
  belong to the previous (contiguous, named) chord keeps that chord with its own bass
  ("C(3)/D", D transitus). Without it the held upper voices became "unresolved
  suspensions" over a one-class '(D)'. I V6 I still names G/B (uppers changed).

**Two passes**: chords for all windows first (candidacy is shape-only), then genera in
time order - a suspension reads its previous segment's genus (already decided), an
anticipation reads the NEXT window's chord (known from pass 1).

Unnamed windows: every class except the bass is non-chord (genus by shape, else ignota).
Sus chords and real sus: when nothing resolves, harmonia's sus4/sus2 still names the
sonority (classic ambiguity; v1 accepts it). Pedal points: not in v1 (Fran).

Real songs (auditio -partitura -explicatio): stella bar 7 'C7:2. (A5 tr.)' (accented
passing, Bb5 A5 G5), pastorale bar 11 'Am:4. (B5 esc.)', korobeiniki bars 10/14 'G#dim'
(E7 without root), bars 3/7 'E7/G#:1 (C5 tr.)'.

**Restart mid-plant-batch (10-23)**: the machine rebooted during the plant run; /private/tmp
(scratchpad: backup, plant script, log) was wiped and plant 6 ('praeparatio repetita
nulla': `FALSUM && m.d1 == ZEPHYRUM`) was LEFT IN the file - planta's finally never ran.
Found by checking every original anchor line count == 1 plus planted markers; restored by
hand (its Editio had also re-indented the line to 16 spaces). Rerun with backup + script
+ log under build/plantae_n1/ (survives reboot), detached.

Plants 27/27 red (rerun after the reboot). Three first attempts did not COMPILE (planta's
pre-check caught them: `FALSUM &&` inside `||` = -Wlogical-op-parentheses; replacing a
condition left a variable unused) - rewritten as compiling plants. Two survived until
tests were added: 'bassus removibilis' (since bassus transiens, the D-bass case never
reached _acordum - new case C D G: D steps in, LEAPS out, so not transiens, and the
window must stay '(D)', not 'C(3)/D') and 'manentes' (no case had two equal-size removals
keeping different note counts - new: Eb5 E5 E5 over E4 E4 over C -> C(3), not Cm(3)).

Lint: 'acordum/acorda/acordi' added to oratio/glossarium.stml (medieval Latin, Du Cange:
accordum = concordia); renamed tonica -> finalis (mode final), APPOGGIATURA -> INNIXA
('leaning', what appoggiare means - appoggiatura is also an English word, never a
glossary entry), ESCAPATA -> EFFUGIENS, removibiles -> removendae. Displayed names
stay the musical terms (appoggiatura, app., esc.). silva/renominare.sh refuses untracked
files (git is its undo) - exact whole-word replacement instead.

## 2026-10-26 - 43HA step 2: added tones must be held

With 6/9/7sus4/7#5 in the vocabulary, the minimal-removal search started preferring
"bigger" chord names that swallow melody notes (A passing over C E G -> C6; F/D over G C
-> G7sus4). Rule `_additae_tenentur`: a window's name is kept only if every ADDED tone
of the genus (acordum additae, rotated to the root) comes from a NON-candidate note -
i.e. a note without passing/neighbour shape. Otherwise the name becomes IGNOTA and the
search keeps removing. Passing A over C E G stays "C:1 (A4 vic.)"; held A gives
"C6:1"; held G B F A gives "G9:1 (F4 sept.!)"; passing D over C E Bb stays C7.

House songs: explicatio/functio lines unchanged for all eight; only lilia's per-beat
harmonia line changed (bars 3, 6 "[Bb C D E]" -> C9/Bb; bar 10 "[Bb C D F]" -> Bbadd9) -
harmonia names what sounds, by design.

## 2026-10-26 - review batch 1.2: a note that starts the window and lasts it is held; fallback; 7sus4

From the sexta review (harmony findings 1, 3 and the test reviewer's side finding).
(1) ii65 regression: F A C D over F only names F6 (radix_basso), and the whole-bar D read as a
candidate ('ant.': same pitch in the next bar), so F6 was rejected and D removed -> "F (D5
ant.)" / IV. A note that STARTS with the window and sounds through it now counts as held for
the added-tone rule (`tentae`), so the window names F6; functio reads IVadd6 before V as ii65
(batch 1.3, Fran: "ii65, Iadd6 at final"). First version said "sounds the whole window" - it
turned a prepared 4-3 suspension over G7 (C tied in from the bar before) into G7sus4; a
suspension must stay a suspension, hence "starts with the window". A crotchet A at the window
start (A4 G4 E2 over C E G) stays a non-chord tone.
(3) the no-name fallback re-derived the full set name without the added-tone check, so a
rejected G7sus4 / C9/D came back with every note "?" - fallback now checks too (IGNOTA).
The reviewer's C9/D fixture is no longer a fallback case: its uppers start with and fill the
half-note window, so C9/D is a fair literal name now.
(side) 7sus4 absorbed candidate CHORD tones (G + C held, melody e f e d -> G7sus4): its added
tones are now the fourth, fifth AND seventh (ADDITAE_SUSPENSAE) - all must be held. A fully
held G C D F still names G7sus4.
Tests also close two test-strength gaps: G-rooted passing/neighbour cases (the rotation was
untested - all were C-rooted) and m6 / 7sus4 root-in-bass rows in acordum. Plants 8/8 first
try (+1 re-plant on the macro form). House songs byte-identical.
Formatter quirk: a PROPRIETATES row wrapped onto a second line got 'spatium unicum ante
operatorem binarium' on its leading `-I` (read as binary minus) - routed around with the
ADDITAE_SUSPENSAE macro; filed.

## 2026-10-26 - review batch 3: suspension detection no longer depends on part order

From the sexta review (harmony finding 4, pre-existing since N1). _motus read
`segmenta[s - 1]` as "the same note's previous segment" and `segmentum + segmenta - 1` as "the
prior note's last segment", assuming a note's segments are adjacent - but segmenta are qsorted
by (initium, pars, pulsus) across ALL parts, so those slots often held another part's note.
The same 4-3 suspension read "C5 susp." with the soprano listed last and "C5 ?" with it first
(the alto's passing F4 sat at s-1). _segmentum_prius now searches backwards for the same
CarmenNota. First attempt matched on the segment's `fenestra` field (the locata index during
segment building) and found nothing: the second pass overwrites that field with the WINDOW
index as it goes. The note pointer is stable and unique within a part, so no part check is
needed. House songs byte-identical (censura too). Plant 1/1.

## 2026-10-27 - voice tracking (step 1 of the declared-intent plan): a pad is four lines, and its notes are chord tones

Found composing (park ...37ERWF #2): explicatio linked each note to the previous/next note IN ITS PART, but
a comitatus '[1 2 3 4]' pad is one part with four simultaneous voices - "neighbours" were other
chord members. Now CarmenNota.vox (0 = the line of a written MML part or comitatus 'B'/'Q';
1..9 = comitatus upper voice, set by comitatus_componere) and the linking loop keeps one
`prior` per (part, voice); rests still break a line.
Gain: sevenths resolve in their own voice (Dm7 -> G7: C4 -> B3 reads plain 'Dm7:1', was 'C4
sept.!'); oppidum 58 -> 43 censura monita. Regression found on the way: with real voices an
accompaniment voice moves by step / repeats, so its ADDED tones looked like passing notes
(lilia F6 -> "F (D4 ?)", C9 -> C7, ...). The earlier good readings relied on the wrong
neighbour model (chord members = leaps = non-candidates). Fix (Fran approved): CarmenNota.acordi -
a generated note comes FROM the declared chord, so it is never a candidate. lilia identical
to HEAD again. Known debt for step 2 (declared harmony splits windows): a bar with two declared
chords over one held bass (proelium bar 16 B7sus4:2 B7:2, oppidum bar 4 C7sus4:2 C7:2) is now
unnamed - before it named the second chord only because the sus tone looked like a passing
note. Side observation: comitatus matches voices BY RANK, so G7 -> C moves the seventh F up
to G, not down to E (voice-leading quality, not analysis). Plants 5/5.

## 2026-10-27 - step 2a: declared harmony anchors the windows (D03TB redesigned, Fran approved Q1-Q3)

Principle: where the author declared a chord (<acorda>), that chord IS the reading and the
sound is judged against it; the 'harmonia' line stays literal; elsewhere inference as before.
- Window cuts: declared chord starts and ends join the slice points; slices merge within a bar
  while the declared chord is the same, whatever the bass does (alternating bass = one chord).
  Outside declared spans the old "same bass" merge rule stands.
- A declared window takes the declared Acordum (author's letters); its bass = the lowest note
  at the window start if that is a chord tone (C over E -> C/E), else the declared bass (a
  passing D under C stays C). Minimal removal and bassus transiens are skipped there; notes are
  classified against the declared classes (chord tones / shapes).
House songs: the six without <acorda> and lilia identical. Fixed: proelium bar 16 (B7sus4:2
B7:2, final cadence [perf.] back), oppidum bar 4 (C7sus4 C7), proelium bar 7 (D with G tr. B
vic., was Gmaj9/D - park ...37ERWF #3/#4/#5 resolved where declared). New, by design (Q2): a
held melody seventh over a declared triad (campus Bm with A, proelium Em/C/Am) now reads '?'
against the declared chord - 2b's 'acorda' rule reports it. Plants 6/6 (the boundary plant had
to remove both start and end points: a chord's end is the next chord's start).
