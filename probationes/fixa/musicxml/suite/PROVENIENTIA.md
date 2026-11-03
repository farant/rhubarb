# suite — the MusicXML test suite (committed, MIT)

- Source: https://github.com/w3c-cg/musicxmlTestSuite (Kainhofer /
  LilyPond, donated to the W3C Music Notation CG), `xmlFiles/` only.
- Commit `77c19f7e819154c70ca1a1992e80dcda8ff82fea` (2026-09-12,
  "Rest display step position example (#18)"), fetched 2026-10-30 as
  `https://codeload.github.com/w3c-cg/musicxmlTestSuite/tar.gz/<commit>`,
  tarball sha256 `5717b897bbc64e7f3b882618f0b60f421b36fe008f83867da42adeb32d620cd4`.
  `tools/musicxml_arcessere.sh -suita` refetches and checks it.
- 183 files (182 `.musicxml` + `90a-Compressed-MusicXML.mxl`), MusicXML
  4.0; `LICENSE` is the repository's MIT licence (redistribution
  allowed). `INDEX` = sha256 + name of every file, sorted (line 1 `DATA`).
- No expected outputs come with the suite; ours are in `../aurum/`
  (plan P7), built ONCE with music21 and then music21 deleted (M11).

## The oracle run (2026-10-30, music21 10.5.2, Python 3.12.6)

Harness `tools/musicxml_oraculum.py` (expandRepeats, toSoundingPitch,
shared measure timeline M15, own tie merge mirroring ours), venv in
`build/musicxml/oraculum/`, run under `tools/custos.sh` (2 GB).

- 218 sources (183 suite + 35 hand fixtures) + nata_lux: **192 agreed
  on the first comparable run after fixing three harness artefacts**
  (music21 renames parts / splits staves into `P1-StaffN` — the real id
  is in `part.groups`; lyrics of chord notes merged onto the chord;
  `<harmony>` chord symbols returned as notes).
- **Reader bugs found and fixed** (each with a hand test): `<chord/>`
  across voices (21i — a chord note always joins the previous note);
  nested repeats inside an ending (45i — inner pairs skipped, endings
  chosen by their governing backward repeat); unpitched without
  display-step (34b/34c — middle line of the current clef); "repeat
  without forward" monitum was noise (45a…) — common practice, now
  silent; REPETITIO_APERTA now means an ending that never closes.
- **26 divergences kept as ours**, each with a `# divergentia:` line in
  its golden (music21's choices: trailing `<forward>`, default
  divisions 10080, chord broken by an interposed direction, chord
  members forced to one duration, concert-score transposed twice,
  first rootfile ignored, zero-length measure padded, nested / split-
  barline repeats, D.C. in our directions fixture …).
- music21 could not read 4 fixtures (timewise, opus, a part-list /
  part mismatch, a missing root entry); `saltus_circulus` was skipped
  (expanding `times="100000000"` reached 1.4 GB before it was stopped).
- **Spec AUDIENDA answered:** octave clefs — music21 and we agree on
  every pitch of the Tallis tenors (MusicXML `<pitch>` is used as
  written; 357/357 rows of nata_lux equal, ours adds the measure-3
  repeat music21 does not expand); grace notes — both zero length at
  the following note's onset (24a–24h agree); jumps — the 45 group
  agrees except the three repeat cases above; largest opus division
  in the suite 1024 (03a), far from the 2^31 cap.
