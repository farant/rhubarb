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
