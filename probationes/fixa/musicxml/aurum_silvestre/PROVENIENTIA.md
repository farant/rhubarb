# aurum_silvestre — the wild corpus and its pins (plan P8)

## The corpus (fetched, never committed)

- OpenScore **Lieder** (CC0) at `38c5db510224d9facdc4b08d741fc788cfb58ea8`:
  1,462 `.mxl`; OpenScore **String Quartets** (CC0) at
  `2614b0b2958dd1ce3f52e172a43e5f4d0e29570f`: 196 `.mxl`. Only the
  `.mxl` exports are fetched (60 MB zipped, ~1.49 GB of XML), by
  `./tools/musicxml_arcessere.sh -silvestria` into
  `build/musicxml/silvestria/`; `../silvestria.manifestum` pins every
  file by sha256 (a changed download is reported as such, never as a
  reader regression).
- Gate `musicxml-oraculum` = `./tools/musicxml_silvestria.sh`: runs
  `probatio_musicxml_suite` with `MUSICXML_SILVESTRIA` **under
  `tools/custos.sh` (10 GB, compressed memory counted)**; corpus absent
  = `OMISSA` (named, exit 0). Without the corpus the root gate still
  checks that manifest and sigilla agree line by line.

## The pins

- **41 full goldens here** (25 Lieder chosen greedily for coverage —
  verses, chords, voices, grace, tuplets, ties, pickups, repeats,
  endings, monita — plus `lc28688206`, the voice-change tie; 15 quartet
  movements ≤ 120 KB). Each agreed exactly with music21. A failure shows
  the first differing row. (Plan said ~25 quartets; 15 keeps the repo
  light — the quartets' features are covered.)
- **1,617 sha256 pins** in `../silvestria.sigilla` (one per manifest
  line, same order): OUR dump including diagnostics. Regression pins on
  our own output, after the oracle review below.

## Memory (2026-10-30/31)

The first corpus run kept every file's tree in one piscina and was
started without custos: ~75 GB (Fran saw it). Now: one piscina per file,
custos built into the gate. Peak ~2 GB; the largest score (Reger op. 74,
16 MB of XML) alone needs ~1.7 GB (~105× its size — desideratum
01M69Y8AJGFTZS4E473C2DM0M8); `CREDO_MEMORIA_MB=4096` for that section.
Reader over all 1,658: ~190 s; **no vitium on any file**; monita: 818
shorter-measure (exporter rounding, e.g. 7:4 32nds written as 17
divisions), 44 dangling ties, 32 unreadable numbers, 7 words-only jumps,
3 unclosed endings, 3 clamped backups.

## The oracle run (music21 10.5.2, once, deleted after)

~4.5 h over four resumable runs under custos (10 GB, peak 2.6 GB);
12 files music21 could not read. **1,392 of 1,646 agree exactly.**

Found along the way — **reader** fixes (each with a hand fixture):
- ties join the same pitch in the same voice, else the same staff —
  MuseScore changes voice numbers across barlines (voice 5 → 6); this
  alone took dangling-tie monita from 3,552 to 44 (`ligatura_vox_mutata`);
- leading/trailing U+00A0 is whitespace (lyrics "␣shoots");
- the TSV row order needs a full key (unisons of different voices):
  part, onset, MIDI, duration, grace, syllable count, first syllable;
- **multi-movement files** (Haydn op. 74/2 in one file): D.C. goes to the
  start of the MOVEMENT (measure numbers restart at "1"), and after a
  Fine the performance continues with the next movement when the jump
  measure ends a movement (`motus_plures`) — 90 quartets changed.

**Harness** fixes (not the reader): chord ties are per pitch
(`chord.getTie(pitch)`; the chord's single `tie` merged untied members
and missed tied ones — it had produced a false "music21 doesn't merge
chord ties" class of ~250 files); same sort key as the reader.

The 254 remaining differences, by class (machine signatures, each class
sampled by hand):

| class | files | why ours stands |
|---|---|---|
| jumps (`<sound>` D.C./D.S./coda, Fine) | 122 | music21 mostly ignores `<sound>` jumps (spec AUDIENDA answered: it does NOT turn every sound dacapo/dalsegno into a jump); ours follows §III/P6, incl. movements |
| repeats / endings | 85 | endings are often marked in the TOP part only (MuseScore); we take the union over parts, music21 expands each part alone |
| onsets only | 28 | `<rest measure="yes">` with a duration shorter than the bar (split bars): we follow `<duration>`, music21 pads to the full bar |
| ties | 15 | cross-staff ties in one voice: we join by voice; music21 splits parts into staves |
| other | 4 | not classified (two Lieder with shifted lyrics, one quartet); pinned as ours |
