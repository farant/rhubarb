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
