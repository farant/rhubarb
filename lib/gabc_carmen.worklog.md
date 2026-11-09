# gabc_carmen worklog

## 2026-11-03 — T7: the converter

**Measured first.** `..` in Gregorio is `_AUCTUM_DUPLEX`
(`gregoriotex-write.c` `write_auctum_duplex`): dots on the note AND the
previous note, so the header's "both last notes" rule holds (we look one
note ahead inside the neume). MML octave belongs to the written letter
(`lib/cantus.c:245`: `12*(o+1) + letter ± 1`), so Cb5 = `o5 c-` = 71. The
Salve only produces lengths 2 3 4 6 sixteenths (139 notes, 17 incises,
9 r16, 7 r8). carmen accepts a leading `_` in verba (vocalis -1).

**Shape.** One pass over syllables builds `NotaCarminis` rows in an Xar
(sound from do, length in sixteenths, first gabc note + tied count, end of
incise + breath), writing verba per syllable as it closes. Then pitch
(`_altitudinem_ponere`: do from summus or do_fixum, octave-move
approximation, spelling by `_orthographia`), the key (`_clavem_eligere`),
then comment + carmen + MML + nexus in one go into a private aedificator,
appended to `exitus` only on success (strict leaves it untouched).

**Spelling.** `_orthographia(do_midi, gradus, midi)`: letter = do's letter
+ degree, sign = what's left; a double sign moves the letter (enharmonic);
an octave outside o0..o8 (Cb9) falls back to a sharp-only table. do's
letter by pitch class prefers flats (Gb, not F#).

**Bug the nexus test caught.** Breath pulses were added twice
(`spiritus * II * sixteenth`, but spiritus is already in sixteenths); the
carmen was right, only the link rows drifted. `_nexus_probare` compares
every row's onset with the re-read carmen note, which is why it showed.

**Salve.** Converted output equals the book's `salve` byte for byte inside
`<carmen>` except the key (`f lydius` -> `f ionius`); report: 39 ictus,
9 opaque marks, do = F4. Acceptance test compares onset / length / MIDI /
letter / syllable per note and the incise count.

**Process note.** Code was written before the tests (plan said tests
first). The plants stand in for "born red": episema x1.5 with mora ->
`_rhythmum_probare` red (the Salve has no note with both, so
`_salve_probare` stays green - the plan's prediction was wrong); ties
across syllables -> note count red. Fixtures are inline gabc strings, not
the plan's three `.gabc` files (each test reads alone).

## 2026-11-04 — T9

Combining marks (U+0300..036F, decomposed accents: 16 in the corpus) are now
dropped silently in syllable cleanup instead of counting as unknown letters;
test in `_verba_variae_probare`. Remaining unknown letters in the whole
corpus (303) are mostly Polish (ł ś ż ć) - correct approximations. Corpus
sample: all 82 convert and re-read; 15389 carmen notes; 2 keys without a
mode name.

## 2026-11-04 — T10: the book's Salve comes from the converter

Fran approved the book change. `carmina.stml`'s `salve` is now
`auditio -gabc musica/fontes/salve_2435.gabc -titulus salve` (the
`<carmen>` element only; the book's own comment block kept, generator line
updated, one phrase "= clavis carminis f ionius" added). Inside `<carmen>`
the only change is `clavis="f lydius"` -> `"f ionius"`. `lib/carmina_infixum.c`
regenerated (tools/carmina_infigere.sh). `musica/fontes/salve.py` deleted.

Parity outlives the generator: before deleting it, its notes were written
to `probationes/fixa/gabc/salve_py_notae.tsv` (onset, length, MIDI, letter,
syllable; 139 notes, syllables assigned the way carmen does, `_` -> empty),
and `_salve_fixum_probare` compares the converted Salve to that file - the
book comparison alone would now be circular. Hand plant: one MIDI 60 -> 61
in the TSV -> section FRACTA at note 2; restored. No other suite reads the
book's salve (carmen, censura, partitura, recensio use korobeiniki, stella,
vesperae) - all re-run green.
