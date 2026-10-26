# emendatio worklog

## 2026-10-23 - E1: emendatio_notam_ponere (wish …NJ7K, edit-by-position)

**Route chosen: surgical text edit through the stml tree, not a model->MML writer.**
Probe (build/sonda_editio/sonda_editio.c via `tools/sonda.sh -nudus`): on carmina.stml
`stml_scribere(legere(x), pulchrum FALSUM) == x` for TWO cycles (4057 B), and replacing
the `<textus!>` text node's `valor` then re-writing changes exactly the edited byte, at
text-node `positus_initium + CarmenNota.offset`. So the author's MML (l8, octave style,
barlines) survives every edit; the model is always re-derived from text. A canonical
model->MML writer is only needed once notes MOVE in time (DAW drag) - deferred.

**Octave arithmetic.** A token's MML octave is recovered from the OLD note itself:
basis = MIDI - written accidental ('#'/'+' = +1, '-' = -1), octave = floor(basis/12) - 1
(same convention cantus uses: `o4 c` = 60, so `o3 b#` = 60 is octave 3). Delta to the
new note's octave is written as '<'/'>' before the token and undone after it, so later
notes keep their octave. Adjacent opposite shifts are cancelled in pairs ('<' before AND
'>' after for an upward edit) - each pair cancelled keeps the running state, and a
re-edit returns the book to the original BYTE-FOR-BYTE (test 'iterum').

**Guards after writing:** the whole new book goes through carmina_legere, then the edited
frustum's text must equal what we built, and every other note must keep pulsus /
longitudo / altitude, and the target must have the new altitude. The MML range checks
(octave 0..8, one accidental) duplicate what re-judgement would also refuse - they exist
for the MESSAGE, so the tests assert the exact causa (else plants on them survive).

**Non-raw `<textus>` refused** ("textus crudus poscitur"): MML uses '<' for octave-down,
so only raw text can carry it; refusing is simpler than trusting writer escaping.

**Frustum index = document index**: carmen.c refuses frusta out of pulsus order, so the
model index is the n-th `<frustum>` element - the tree walk relies on it.

**Beat unit is the third copy** (harmonia `_tempus`, censura `_fortia`, here `_unitas`):
candidate for `carmen_tempus_mensurae` in carmen.h (asked Fran).

Plants: 16 in emendatio.c + 6 in partitura_notam_legere, all RED then GREEN (silva.planta,
gate radix). Two would have survived without asserting the exact causa (octave range,
double accidental) - see above. Added `3:1+1920` before planting: a beat overflow that
lands EXACTLY on a later note start (3:3 = a4), else dropping the `pulsus >= unitas`
check survived (1:1+960 lands inside d2. and is refused anyway). Smoke on carmina.stml
(build/sonda_emendatio): stella bassus 1:1 F2 -> one line differs, 'd2.' -> '<f2.>'.

## 2026-10-23 - `_unitas` removed

The beat unit is carmen_tempus_mensurae now (hoisted on Fran's OK; see lib/carmen.worklog.md).

## 2026-10-23 - E2: emendatio_mensuram_invenire / _substituere

A bar is the text between WRITTEN barlines (or frustum start/end): start after the LAST
'|' at the bar's start pulse, end at the FIRST '|' at its end pulse (doubled '| |' tested),
edge whitespace trimmed. Commands before a '|' belong to the bar they end (korobeiniki's
trailing '<'). Refusals: silence between frusta, a frustum starting or ending inside the
bar, unwritten barline.

Replacement runs in the bar's START state (cantus_status_ad); after it the OLD end state
is restored, only what differs: ' oN' (relative '<'/'>' if the old octave is outside 0..8),
' lN[.]' (dotted lengths via _longitudinem_scribere), ' vN', ' @x'. A new '@' where the old
end had none is refused - MML cannot clear '@'. No restoration when the frustum ends with
the bar. Then: total frustum length must be unchanged (explicit causa), and after the
re-judged write every note OUTSIDE the bar (pulsus, longitudo, nota, lucrum, praeparatum)
and every barline pulse must be identical (_extra_eaedem).

E1's tree-write + re-read + "textus relectus == scriptus" moved into `_librum_scribere`,
shared by both operations.

TRAP for E3 (smoke on korobeiniki): bar 1's span INCLUDES the setup 'o5 l8', so a
replacement 'e2 e2' plays in the default o4 (and ' l8' is restored for bar 2). Correct, but
an agent will miss it - the CLI must print the start state ("o4 l4") with the bar.

Plants 21/21 red (cantus 4, emendatio 17); the 'l' dot arithmetic needed the 'punctata'
case, and the frustum-boundary guards needed exact causa asserts (else they fall through
to other refusals and survive).

## 2026-10-23 - (4) copy / transpose / sequence (wish …1K9G5)

emendatio_mensuram_scribere is the bar-scoped notes -> MML writer deferred since E1: same
rhythm, explicit length on every note, rests between notes (largest-first, simple dots),
'oN' at the start and where the octave changes, 'vN' where the volume changes (not at
the start when 1.0 - the destination's state is restored by substitution anyway).
Diatonic transposition works on LETTER numbers: D = octave*7 + letter, shifted by the
tonic-letter difference between source and destination keys (normalised to -3..3, so
G -> C moves up a fourth, not down a fifth) plus the interval; the accidental is kept
RELATIVE to each key's signature (F# in G = signature, -> B in C = signature). In minor
the raised VII is an accidental by this rule (G# +1 -> A#) - documented, not "fixed".
Semitone mode spells from the destination key (partitura_orthographia).

emendatio_mensuras_copiare builds ALL destination texts from the ORIGINAL book first,
then applies E2 substitutions one by one (re-reading the book each time) - a destination
overlapping the source copies the original bars. Repetition r is written at K + (r-1)*n
with r times the interval. Refuses a destination whose start state has '@' (the copied
notes would become percussion).

auditio -copiare P M[-N] [Q:]K [+-g] [-semitonia] [-repetere R] [-scribere]: before/after
partitura line per destination bar. Checked on pastorale (bars 1-2 -> 13 up two steps in
G: F#5 B5 D6 C6), a falling sequence, stella melody -> bassus an octave down.

Plants 17/17 red (one first form did not compile: replacing the bar-count check left
_mensurae unused); two paths got tests BEFORE planting (trailing rest in a bar; empty
destination part = source part). Lint renames: _tonica_littera -> _finalis_littera,
kc -> currentis.

## 2026-10-24 - frusta iterata (<repetitio>): STML index by scriptum, edits refused

Expanded frusta (carmen <repetitio>) now sit in CarmenPars.frusta in time order among the
written ones, so the model index is no longer the STML <frustum> index. _librum_scribere
takes the WRITTEN index (CarmenFrustum.scriptum) and finds the re-read frustum by scriptum,
not by position. Edits that land in expanded material refuse with
EMENDATIO_CAUSA_REPETITIONIS ("repetitio (frustum iteratum): muta sectionem fontis"):
notam_ponere, mensuram_invenire (-> substituere, and copiare's destination), and
copiare's SOURCE bars. Test: a written frustum after a repeat (model index 2, STML 1)
edits the right <frustum>.

## 2026-10-26 - generated frusta refused (Q7G5 step 2)

A frustum from <comitatus> (CarmenFrustum.comitatus >= 0) has no MML of its own: the three
places that refuse a repeated frustum (note set, bar substitute, copy source) refuse it
too, naming the source: "comitatus (frustum generatum): muta acorda aut figuram". The
copy test must use the GENERATED bar as the SOURCE - with it as the target the
substitute check fires first and the copy-source check is never exercised.
