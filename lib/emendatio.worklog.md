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
