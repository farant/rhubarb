# musicxml (reader) worklog

## 2026-10-31 — what the oracles taught the reader

Full stories are in the plan's P2–P8 Executed notes and in
`probationes/fixa/musicxml/{suite,aurum_silvestre}/PROVENIENTIA.md`. These
are the finds a future reader of `lib/musicxml.c` / `lib/musicxml_ordo.c`
would trip over:

- `<chord/>` ALWAYS joins the previous note, even across a voice change
  (W3C 21i). ACORDUM_PRIMUM only when the measure has no previous note.
- Ties: match the same pitch in the same voice, ELSE the same staff.
  MuseScore renumbers voices across barlines (5 → 6); voice-only
  matching left 3,552 dangling-tie monita in the corpus, now 44.
- A tie from ending 1 back to the repeat start has its STOP written
  before its START: unmatched stops wait until the end of the part and
  pair with unmatched starts of the same voice and pitch.
- Multi-movement files (measure numbers restart at "1"): D.C. goes to
  the start of the MOVEMENT; after Fine, the performance continues into
  the next movement when the jump measure ends a movement.
- Unpitched notes without display-step sit on the clef's middle line.
- U+00A0 counts as whitespace when trimming lyric text.
- Memory: the tree costs ~105× the XML (largest score 16 MB → ~1.7 GB).
  Any loop over many files needs one piscina per file and custos.
  Desideratum …DC2DM0M8.
- Divisions LCM: 1,024 max in the W3C suite, 9,240 in the wild corpus;
  the 2^31 cap is far away.

## 2026-11-03 — the XML document lives in a scratch arena (memory pass, task 5)

`musicxml_legere` built the archive inflation, `container.xml`, the
score's XML document and the score model all in the caller's piscina,
so the document (now ~38x the text after the xml/materia passes, 71x
before) lived as long as the score. Now the reader has two arenas:
`Lector.piscina` (the caller's: the model only) and `Lector.scrinium`
(sarcina, container.xml, xml_documentum_legere). After reading,
`_chordas_servare` walks the model and copies every string that lies
outside `datum` into the caller's arena; then the scrinium is
destroyed. Strings inside `datum` stay put, as the header already
promises ("datum non copiatur: in piscina vivere debet dum opus
vivit"), so the header is unchanged.

The walk lists all 26 string fields of include/musicxml.h by hand
(MusicxmlVitium.causa is always built in the caller's arena). A NEW
string field in the model must be added there, or it will point into
freed memory. The corpus probe under ASan catches that if the converter
reads the field.

Reger op. 74: held score 690.2 -> 55.3 MB (3.5x the text; 77x at the
start of the pass). The peak while reading is unchanged (~690 MB). The
copy pass itself costs 0.5 MB on Reger (some strings, e.g. lyric text
from xml_textus_totus, are already in the caller's arena and get copied
again; not worth a piscina ownership query).

Checks: corpus probe under ASan, all 1,658 files convert and
carmina_legere accepts all, ASan clean. Plants under ASan on 12 lieder:
skipping the whole copy pass -> heap-use-after-free on the first file;
omitting only the syllable text copy -> NOT caught, because that text
is built in the caller's arena already (a no-op field, not a gap).
