# musicxml — a MusicXML reader, and a converter into carmen text

*2026-10-30. Born in sexta right after xml_arbor closed (parcum
01M65252W1DKBJE10VX0NBQDJH), answering desideratum
01M64ZZYQ59DY0G0104SRTK439. The first polyphony in the house book —
Tallis, O nata lux (`nata_lux`, CPDL #80317) — came in through a
one-off Python generator (`musica/fontes/nata_lux.py`); this spec
makes that path a library. Fran, 2026-10-30: "i think we just want the
library as a modular building block. first user is probably you?" —
and agreed that "complete coverage" means everything in MusicXML that
affects SOUND and TEXT, with page layout read past. Approximation
policy (§0 M5) was Claude's recommendation, accepted.*

**What it is.** Two libraries on the house stack:

1. **`musicxml`, the reader**: a MusicXML document (`.xml`,
   `.musicxml`, or the zipped `.mxl`) in, a neutral **score model**
   out — parts, measures, notes with written spelling, exact
   durations, voices, chords, ties, tuplets, grace notes, lyrics,
   attributes (key, time, clef, transposition), directions (tempo,
   dynamics, wedges, words), and the repeat structure, plus a
   **performance order** helper that unrolls repeats, endings and
   jumps. It knows nothing about carmen.
2. **`musicxml_carmen`, the converter**: score model in, **carmen
   text** (the STML that `carmina_legere` reads, `include/carmen.h`)
   out, plus a list of every **approximation** it made. "Text stays
   truth": what enters the music book is text a person can read and
   edit, judged by the book reader like any hand-written song.

**Who uses it.** Claude first: fetch a score (Fran downloads CPDL
files by hand — CPDL is behind Cloudflare), convert, then play it,
analyse it (censura, explicatio, functio) and edit it (emendatio)
with the music tools already in the house. Later: a notation pane, a
MusicXML writer, the gabc reader (desideratum …96NQ) as a sibling
importer.

**Why the name "musicxml".** Like xml, toml and plist, the library
keeps the format's own name (glossarium entry, `ignotum-permissum`,
"nomen formae, ut plist").

---

## 0. Decisions

| # | decision | rationale |
|---|---|---|
| M1 | **Two layers: reader (score model) and converter (carmen text).** The reader does not include `carmen.h`; the converter is the only place that knows both. | Fran: "a modular building block". A notation pane or a writer reuses the reader without the converter. |
| M2 | **Coverage = everything that affects sound or text.** Pitch (written spelling + alter, microtones kept as decimal alter), durations, rests, chords, voices (`<backup>`/`<forward>`), staves, ties, tuplets, grace notes, cue notes, lyrics (every verse, syllabic, extend, elision), key, time (incl. `senza-misura`, compound, additive `3+2/8`), divisions, transposition, clef (kept, for octave clefs), tempo (`<sound tempo>`, `<metronome>`), dynamics, wedges, fermatas, articulations, ornaments and slurs (kept as marks), words, rehearsal, repeats, endings, segno/coda/fine/da capo/dal segno, unpitched (percussion) notes, part list and instruments (name, MIDI program, channel). **Layout is read past**: `<print>`, `<defaults>`, margins, stems, beams, positions, fonts, colours, `<credit>` (except title/composer). | Fran agreed 2026-10-30. Layout is most of the format and none of the music. |
| M3 | **Both document forms**: `score-partwise` and `score-timewise` read into the same model. MusicXML 1.0 through 4.0; version-specific elements handled where they differ (§III). | Timewise is rare but legal; one model means one converter. |
| M4 | **Exact time.** The model's unit is **one opus-wide division**: the least common multiple of every `<divisions>` value in the document (per quarter note). Every onset and duration is an integer in that unit — no rounding in the reader. Overflow of the LCM (cap 2^31) is a named error. | MusicXML lets `<divisions>` change per part and per measure. One integer unit keeps the model exact and simple; the converter decides what carmen can say (M5). |
| M5 | **The converter writes the closest carmen it can and reports every approximation** (kind, part, measure, source line:col). An **approximation** is anything that changes what SOUNDS or is SUNG: a pitch, an onset or duration, a dropped note (grace, cue), a dropped verse, a moved dynamic or tempo. **Marks** carmen does not carry and a performer interprets — slurs, articulations, ornaments (unrealized), words, rehearsal, pedal, other-direction — are **unconverted marks**: counted by kind in the report, never approximations. A **strict** option refuses on the first approximation (marks never refuse). **The reader never approximates.** *(Claude's call, from the builder check: nata_lux has 50 slurs and 12 words directions and must convert strictly.)* | Claude's recommendation, accepted: most real scores have something carmen cannot yet say (grace notes, endings); refusing would let almost nothing in. The report counts tell which carmen feature to grow next (the xml differential's role). Strict is for tests. |
| M6 | **Free rhythm (`senza-misura`) → `metrum="liberum"`, one incise per MusicXML measure.** Where the phrases go is not guessed (no syllable lists, unlike `nata_lux.py`'s `LINEAE`). | A library cannot know a hymn's line breaks. The hand-curated `nata_lux` in the book keeps its 10 incises; the converted one has the edition's measures. |
| M7 | **Polyphony inside one part becomes several carmen parts.** MML is one line (`cantus.h`, NONDUM "voces plures simul"); each MusicXML voice, and each extra chord note, is its own carmen `<pars>` (`<titulus>_<n>`). Sound is exact; this is a structural note in the report, not an approximation. | carmen frusta in one part must not overlap (`carmen.h:150`). |
| M8 | **Repeats are unrolled** by the reader's performance order (M2); the converter writes the unrolled notes. Mapping simple repeats onto carmen `<sectio>`/`<repetitio>` is a named future. | Exact in sound; `nata_lux.py` already did this. |
| M9 | **Input is bytes; the caller reads the file.** `.mxl` is detected by its ZIP signature and opened with `sarcina.h` (`META-INF/container.xml` names the root file). | Same rule as `xml_documentum_legere` and codex's EPUB path (`lib/codex.c:1253-1290`). No new ZIP code. |
| M10 | **Totality**: any byte string gives a model or located diagnostics, never a crash or a hang — XML errors through `xml_documentum` diagnostics, music errors (a `<note>` without `<duration>`, a `<backup>` before the measure start, a tie to nothing) as the reader's own, with line:col. A dangling tie or an odd backup is a diagnostic, not a refusal, where the rest of the score still makes sense (the reader keeps going, like xml_arbor X6). | The xml_arbor standard. Real exports are sloppy (MuseScore, Finale, Sibelius each have known quirks — §VI wild corpus). |
| M11 | **Oracle-driven** (§VI): music21 builds the expected answers **once**, then leaves. Its output is reviewed and committed as our goldens; no gate, test or build ever needs music21 again (Fran: "use it to build the tests / expected behavior and then not have it as a long term dependency"). The MIT MusicXML test suite is committed with those goldens; CC0 OpenScore corpora are fetched by script. | The shape that made xml_arbor trustworthy, without a standing dependency. |
| M12 | **Tool surface: `auditio -musicxml VIA [-strictus] [-titulus T]`.** Alone it prints the converted book (`<carmina>` with one `<carmen>`) on stdout and the approximation report on stderr (exit 3 when there are approximations, like `-censura`). With any other auditio flag it acts as a book source exactly like `-liber x.stml` (`tools/auditio.c:57`), so `auditio -musicxml s.mxl -carmen T -censura` / `-partitura` / play all work unchanged. | Claude is the first user and works through auditio already; one new source, no new verbs. |
| M14 | **Pickups and irregular bars become `<metrum>` changes.** A measure whose length differs from its time signature (pickup `implicit="yes"`, a short final bar, an editorial odd bar) gets `<metrum mensura="k" valor="N/D">` for that bar and the signature restored after it (D = the smallest power of two that makes N whole). carmen bars are therefore 1..n in order; the **report always uses the written measure numbers**, and the converter writes a comment line mapping carmen bar → written number when they differ. *(Claude's call: exact in sound; censura sees the pickup as a short bar, which is what it is.)* | carmen bars come only from the metre map (`carmen.h:27-39`); there is no partial-bar concept. |
| M15 | **One shared measure timeline.** Measure k starts at the same time in every part: its start is the previous start plus the LONGEST part's measure k (a partwise score's measures are simultaneous by definition). A part whose measure is shorter is padded with rest time and gets a monitum; notes are never moved. The oracle harness applies the same alignment to music21's output (per-measure offsets), so the two compare like for like. *(Claude's call, from the builder check.)* | carmen bars are shared by the whole song; sloppy exports disagree on lengths. |
| M13 | **Out of scope**: writing MusicXML; layout; MIDI import; MusicXML `<sound>` playback overrides beyond tempo/dynamics/jumps (e.g. `<play>`, `<swing>` — read and kept, not converted); non-UTF-8 documents (xml_arbor X8). | Named futures (§VIII). |

---

## I. What exists (2026-10-30)

- **XML**: `xml/fontes/xml_documentum.h` — cooked view, located
  diagnostics, W3C-proven (xml_arbor §X). Walk with `xml_filius` /
  `xml_frater` / `xml_attributum` / `xml_textus_totus`; the briar
  fixture `briar/probationes/fixa/thistle/xml_partitura.thistle` is a
  tiny score walk.
- **ZIP**: `include/sarcina.h` — read-only, stored + DEFLATE (via
  `flatura`), CRC checked, never reads outside the archive.
- **Song model and its text**: `include/carmen.h` (grammar in its head
  comment, `carmen.h:13-195`). Relevant capacities:
  - time in pulses, `CANTUS_PULSUS_QUARTAE` = 960 per quarter
    (`cantus.h`), divisible by 3, 5, 6, 15;
  - MML lengths 1..64 with dots (≤ 2), triplets 3/6/12/24/48,
    quintuplets 5/10/20/40, 15/30/60; ties `&` make any sum;
  - `<metrum>` N/D or `liberum`; `<clavis>` fifths -7..7 + maior /
    minor / church modes (same numbers as MusicXML `<fifths>`);
  - `<tempus>` integer 20..400, steps and ramps; `<dynamica>` per
    part or song, steps and ramps (ramp ends at a measure start);
    `<fermata>`; positions by `mensura` + `ictus` or absolute
    `pulsus`;
  - `<verba!>` one syllable line per frustum, `-`, `_` melisma,
    vowels by ecclesiastical-Latin rules;
  - per-note spelling (`CarmenNota.littera`), parts with instrument,
    gain, pan, reverb.
- **Prototype**: `musica/fontes/nata_lux.py` (166 lines) — partwise
  only, asserts no chords and no ties, expands `<repeat>`, lyrics
  verse 1, hard-coded phrase list. Its output is the book's
  `nata_lux` (`carmina.stml:673`).
- **Test score on disk**: `musica/fontes/nata_lux_80317.mxl` (Sims
  2024, MuseScore export; 5 parts × 4 measures, 373 notes, 320
  lyrics, 1 forward/backward repeat, `senza-misura`, octave clefs on
  the tenors).

---

## II. Architecture and files

| file | role |
|---|---|
| `include/musicxml.h`, `lib/musicxml.c` | reader: bytes → `MusicxmlOpus` (+ diagnostics) |
| `lib/musicxml_ordo.c` (same header) | performance order: repeats, endings, jumps → sequence of measure visits |
| `include/musicxml_carmen.h`, `lib/musicxml_carmen.c` | converter: opus → carmen text + approximations |
| `probationes/probatio_musicxml.c`, `probatio_musicxml_carmen.c` | unit suites (credo) |
| `probationes/fixa/musicxml/` | hand-written small scores, one feature each, committed |
| `tools/musicxml_arcessere.sh` → `build/musicxml/` | fetches the external suite/corpus (gitignored; sha256-pinned manifest committed) |
| `tools/musicxml_oraculum.py` | one-time golden builder (music21 in a throwaway venv); run by hand, never by a gate |
| `tools/auditio.c` | `-musicxml` (M12) |
| `xml/fontes/xml_documentum.h` | **one addition** (reviewed with the step-1 header): `xml_locus(nodus, &linea, &columna)` — the reader's diagnostics and the converter's report need source positions; `XmlCoctus.nodus` (a `MateriaNodus`) already carries them (`xml_coctum.h:94`). **FLAGGED: an xml API change.** |

The reader is a plain library in `lib/` (like plist, which also
stands on `xml_documentum.h`), not a materia client: MusicXML is an
XML vocabulary, not a syntax.

---

## III. The score model (reader)

Sketch only — the header is drafted for Fran's review before any
implementation (the house rule for headers).

```
MusicxmlOpus
  forma            partwise | timewise
  versio           "4.0" as written ("" if absent)
  titulus, opus_titulus, numerus_motus, compositor, poeta (lyricist),
  ius (rights)                         ← identification / work / credit
  divisiones       opus-wide unit per quarter (M4)
  partes[]         MusicxmlPars
  vitia[]          diagnostics (line:col, Latin cause)

MusicxmlPars
  id, titulus (part-name), abbreviatio
  instrumenta[]    score-instrument: name, MIDI channel/program/
                   volume/pan, unpitched note → instrument map
  mensurae[]       MusicxmlMensura

MusicxmlMensura
  numerus          as written ("12", "X1"); implicit flag
  initium          shared across parts (M15), opus divisions
  longitudo        this part's own length (max over voices; may be
                   shorter than the shared one - monitum)
  attributa        changes at their positions: divisions, key (fifths,
                   mode, or non-traditional steps), time (beats/
                   beat-type list, symbol, senza-misura), clefs per
                   staff (sign, line, octave-change), transpose
                   (diatonic, chromatic, octave-change, double), staves
  lineae           barlines: location, style, repeat (direction,
                   times), ending (number list, type), fermata
  signa[]          directions at positions: tempo (sound tempo +
                   metronome text), dynamics (marks + sound dynamics),
                   wedge (crescendo/diminuendo/stop, number), words,
                   rehearsal, segno, coda, fine, dacapo, dalsegno,
                   tocoda, pedal, octave-shift; with voice/staff
  notae[]          MusicxmlNota, in document order

MusicxmlNota
  positio, longitudo   opus divisions (grace: longitudo 0, steal/make
                   time kept as written)
  vox, baculum     voice (string as written), staff number
  genus            pitched | rest | unpitched | (measure-rest flag)
  littera, alteratio, octava   written step, alter (decimal), octave
  sonans           sounding MIDI (transposition applied) + sounding
                   spelling — derived, so consumers need not repeat it
  acordum          continues previous note's chord
  ligatura         tie start/stop (from <tie>, not <tied>)
  gratia           grace (slash, steal-time-previous/following,
                   make-time); cue flag
  tuplus           time-modification actual/normal (+ tuplet marks)
  notae_signa      fermata, articulations, ornaments, slurs (start/
                   stop, number), technical, arpeggiate — as kinds
  syllabae[]       per verse: numerus (verse), syllabic, text,
                   extend, elision
  dynamica         note-level <dynamics>/<notations> dynamics
```

**Performance order** (`lib/musicxml_ordo.c`): from barline repeats
(with `times`), endings (number lists like "1, 2"), and jumps (sound
`dacapo`, `dalsegno`, `tocoda`, `fine`, `segno`, `coda`, plus the
written words only when `<sound>` is absent — reported), produce the
sequence of (measure index, pass number). Passes let a consumer pick
verse *n* on pass *n*. A malformed repeat structure (backward without
forward = from the start, per common practice) is handled with a
diagnostic; an infinite jump loop is cut and named.

**Sounding pitch.** `<transpose>` gives written → sounding; the model
keeps both. The spelling of the sounding note follows the diatonic
interval (a B♭ clarinet's written D sounds C, letter c).

**Version differences handled** (MusicXML 4.0 spec and release notes;
§VI.4): missing `version` = 1.0; 4.0 `<concert-score>` (no second
transposition); `<for-part>`/`<part-transpose>` (4.0);
`<instrument-change>`; `<listen>`/`<play>`/`<swing>` kept, not
converted; `<sound>` inside `<direction>` or standalone; deprecated
`<directive>`; `.musicxml` extension, container `media-type`
optional, `mimetype` entry optional; `opus` roots refused with a
named diagnostic.

---

## IV. The converter

Input: a `MusicxmlOpus`, a form (options), output: carmen text into a
`ChordaAedificator` + a list of approximations.

**Mapping.**

| MusicXML | carmen | exactness |
|---|---|---|
| part / voice / chord layer | `<pars>` (M7), titles `[a-z0-9_]+` from part-name (folded), `_2`, `_3` … | exact |
| pitch | MML note with written letter + `#`/`-`, octave `o N`; MIDI from sounding pitch | exact (microtone alter → nearest semitone, **approx**) |
| duration | MML lengths, tied `&` sums on the 960 grid | exact when on grid; off-grid (e.g. septuplets) rounded with ties keeping the sum, **approx** |
| rest, forward gap | `r` | exact |
| tie | `&` | exact |
| measure | `|` in the text (bars counted by carmen from `<metrum>`) | exact |
| key / time changes | `<clavis>` / `<metrum>` at `mensura` | exact; non-traditional keys, additive time, **approx** |
| pickup, irregular bar | `<metrum>` for that bar (M14) | exact |
| senza-misura | `metrum="liberum"`, incise per measure (M6) | exact |
| tempo | `<tempus>` integer | non-integer rounded, **approx** |
| dynamics marks | `<dynamica pars>` at `pulsus` | exact for pp..ff; sfz, fp etc. **approx** |
| wedge | `<dynamica usque>` ramp | ends mid-measure → moved to next measure start, **approx** |
| fermata | `<fermata>` factor 2 | exact (factor is ours) |
| lyrics | `<verba!>` per frustum, verse 1 (or verse = pass with repeats) | other verses dropped, **approx**; a pitched note with no syllable in the verse → `_` (melisma) whether or not `<extend>` is written (nata_lux has none) |
| grace notes | — | dropped, **approx** |
| articulations, ornaments, slurs, words, rehearsal, pedal, other-direction | — | unconverted **marks** (M5), counted by kind |
| unpitched | part with `@praeparatum` per instrument when mapped | unmapped → dropped, **approx** |
| repeats, endings, jumps | unrolled (M8) | exact |
| instruments | house instrument chosen by a table (voice names → `schola` with `vox`; MIDI program families → house instruments), overridable in the form | ours, noted |
| title, composer | `titulus` (folded) + a comment with work, composer, lyricist, rights, source | — |

**Report.** One entry per approximation: kind (enumerated), part,
measure (as written), pass, source line:col, and a short Latin
description; plus a per-kind count. Structural notes (M7 splits,
unrolled repeats, instrument choices) are listed separately from
approximations.

**Strict.** First approximation → FALSUM with that entry; nothing
written.

**Acceptance for `nata_lux`.** Strict conversion of
`nata_lux_80317.mxl` succeeds, and the converted song's notes —
per part: pitch, spelling, onset and duration in pulses, syllable —
equal those of the book's `nata_lux` (ties are one note in both, so
the different incise lines do not matter). Text equality is NOT
required (M6, instruments).

---

## V. Diagnostics

Reader: XML errors pass through (`xml_documentum_diagnostica`);
music errors get line:col (via `xml_locus`) and a Latin cause, in the
opus's `vitia`. Kinds: missing required child (`<duration>` of a
non-grace note, `<step>`, `<octave>`), number out of range, unknown
enumerated value (kept as text, warned), `<backup>` past the measure
start (clamped), dangling tie start or stop, `<chord>` on the first
note of a voice, repeat/ending structure that does not close, jump
loop. Severity: **vitium** (no usable model: not MusicXML, XML not
well-formed, no parts) vs **monitum** (model produced, something
repaired).

---

## VI. Oracles and gates

From a research pass on 2026-10-30 (web, GitHub API, PyPI, Homebrew;
nothing installed). URLs and licences as read that day.

**1. Conformance core — the MusicXML test suite** (Kainhofer /
LilyPond, donated to the W3C Music Notation CG in 2026):
https://github.com/w3c-cg/musicxmlTestSuite, **MIT**, 183 files
(182 `.musicxml` + `90a-Compressed-MusicXML.mxl`), MusicXML 4.0.
Groups that matter here: 45 repeats (14 files), 46 barlines/pickups,
61 lyrics (incl. elisions), 99 broken exports. **No expected
outputs** — each file carries a prose description only — and the
announced 81–89 "MIDI and sound" range is **empty**.
- MIT allows redistribution: the suite is **committed**, pinned to a
  commit with its LICENSE, under `probationes/fixa/musicxml/suite/`
  (as `toml/probationes/fixa/toml-test` is).
- Expected results are **ours**: a golden note list per file, produced
  by the oracle (2), each disagreement reviewed by hand, committed.
- Our own fixtures fill 81–89: tempo, dynamics, wedges, `<sound>`
  jumps, swing kept-not-converted, concert score.
- The W3C repo (https://github.com/w3c-cg/musicxml, spec under the
  W3C Community Final Specification Agreement, no LICENSE file) adds
  `tests/assertions.json` (files meant to fail the XSD: 41g, 74a,
  99d) and valid/invalid pairs such as `repeats-jumps.musicxml` /
  `.invalid.musicxml`. Not committed (no licence file); fetched by
  `tools/musicxml_arcessere.sh` when needed, read for the repeat
  semantics.

**2. Primary oracle — music21** (BSD-3-Clause, `pip install music21`,
10.5.2 of 2026-10-28, Python ≥ 3.11). Per part:
`expandRepeats().stripTies().flatten().notes` gives onset and
duration as **exact fractions**, MIDI pitches, and lyrics with verse
numbers; `toSoundingPitch()` gives sounding pitch. The harness dumps
one canonical TSV per file (part, onset, duration, MIDI, spelling,
verse/syllabic/text, grace flag) and our reader dumps the same from
the model + performance order; the gate compares.
- **music21 is scaffolding, not a dependency** (Fran's decision). It runs
  once, in a throwaway venv in `build/musicxml/oraculum/` (gitignored),
  with `python3 -I`, and is never imported by house code. It writes
  TSVs. We review every disagreement with our reader by hand, then
  commit the result as **our** goldens. After that the venv can be
  deleted. Every gate compares against committed files only.
- The harness script (`tools/musicxml_oraculum.py`) stays in the tree
  so goldens can be extended later, e.g. when new fixtures are added.
  Running it is a manual, occasional act, never part of a gate.
- Once a golden is committed it belongs to us. Where music21 was
  wrong, the golden carries our answer and a `# divergentia:` comment
  giving the reason, so later readers don't "fix" it back.
- Not verified: whether music21's importer turns every `<sound
  dacapo|dalsegno|…>` and D.C./D.S. words into jump objects; its grace
  notes (believed zero length). Disagreements there are named rows,
  like xml_arbor's `codificatio` rows.
- Second opinion for repeats only: **partitura** (Apache-2.0, pip,
  `unfold_part_maximal`) — optional, consulted when music21 and we
  disagree on a repeat.
- Fuzzy smoke only, never a gate: MuseScore CLI MIDI (GPL, cask) and
  PDMX's paired `.mid` files — both are MuseScore *performances*
  (swing, ornaments) and circular for MuseScore exports.

**3. Wild corpus** (fetched by script into `build/musicxml/`,
manifest of paths + sha256 committed, files never committed):
- OpenScore Lieder (https://github.com/OpenScore/Lieder, **CC0**,
  ~1,462 `.mxl`) and OpenScore String Quartets (CC0, 196 `.mxl`);
- the music21 corpus is **not** used: its per-file licences (some
  non-commercial) make committed goldens derived from it doubtful,
  and it disappears with the venv anyway;
- CPDL files Fran downloads by hand (Cloudflare blocks scripts).
- PDMX (Zenodo, ~254k MuseScore exports, 1.9 GB) is a named future,
  sampled if the others leave gaps.

**4. Format facts the reader must handle** (MusicXML 4.0 spec,
compressed-file page): `.mxl` first `<rootfile>` is the score
(partwise, timewise, or **`opus`** — an opus root is a named
diagnostic in v1); `media-type` may be absent; a `mimetype` entry
(STORED) comes first since 3.1 and may be missing in older files;
accept STORED and DEFLATE for every entry (`sarcina` does both).
Missing `version` attribute = 1.0. **4.0 `<concert-score>`**: pitches
are already concert pitch, `<for-part>`/`<part-transpose>` only
describe the parts — never transpose twice.

**Gates** (names for the plan):
- `musicxml` — unit suites, hand fixtures, the committed suite against
  its committed goldens, `nata_lux` acceptance (§IV).
- `musicxml-oraculum`: the wild corpus, checked against committed
  goldens with no music21 involved. There are two kinds of golden:
  - **full TSVs** for the committed suite (183 small files) and for a
    pinned sample of about 60 wild files (Lieder, quartets, nata_lux).
    A failure shows the exact differing rows.
  - **one sha256 per file** for the rest of the wild corpus, of OUR
    reader's TSV, pinned after music21 agreed with it (or after
    divergences were reviewed). From then on it is a regression pin on
    our own output. A failure names the file, and our dump plus a
    re-run of the oracle script explains it.
  When the corpus files are missing, the gate reports SKIP with its
  reason rather than passing silently.
- totality — truncated/mutated suite files and random bytes: no
  crash, no hang (CREDO_NON_RUIT / NON_PENDET).

---

## VII. Plan outline (detail in the plan document)

1. Header draft (`musicxml.h`) → Fran's review. `xml_locus` added.
2. Reader core: partwise, notes/rests/chords/voices/backup/forward,
   divisions LCM, attributes; `.mxl` via sarcina; hand fixtures.
3. Directions, lyrics, ties/tuplets/grace, transposition, timewise.
4. Performance order (repeats, endings, jumps).
5. Oracle harness + suite + wild corpus differential; totality.
6. Converter header → Fran's review; converter; report; strict.
7. `auditio -musicxml`; `nata_lux` acceptance; worklog, closure.

---

## VIII. Named futures (nothing built)

MusicXML writer (carmen → MusicXML, for notation software round
trips); repeats as carmen `<sectio>`/`<repetitio>`; grace notes,
articulations and multiple verses in carmen itself (the report's
counts choose the order); a notation pane on the score model;
percussion maps; gabc as a sibling importer (…96NQ); MIDI import.

---

## IX. As built (2026-10-31, P1–P11 in sexta)

**Files.** `include/musicxml.h` + `lib/musicxml.c`, `lib/musicxml_ordo.c`
(reader, performance order); `include/musicxml_carmen.h` +
`lib/musicxml_carmen.c` (converter); `tools/auditio.c` `-musicxml`;
suites `probatio_musicxml` (reader), `probatio_musicxml_suite`
(goldens + corpus), `probatio_musicxml_totalitas`,
`probatio_musicxml_carmen` (16 sections, nata_lux acceptance);
gate `musicxml-oraculum` (`tools/musicxml_silvestria.sh`); worklog
`lib/musicxml_carmen.worklog.md`. Plan notes record every re-slice.

**Oracles (music21 10.5.2, run once, venv deleted).**
- W3C suite: 183 files committed (MIT); goldens for 183 suite + 42 hand
  fixtures + nata_lux. 26 suite divergences kept as ours, each with a
  `# divergentia:` line; 4 files music21 can't read.
- Wild corpus: OpenScore Lieder 1,462 + String Quartets 196 = 1,658
  `.mxl` (CC0, pinned, fetched, never committed); 1,536 are MusicXML
  4.0, 122 are 3.1. music21 agreed exactly on 1,392 of 1,646; the 254
  differences are classified in `aurum_silvestre/PROVENIENTIA.md`
  (jumps 122, repeats/endings 85, split-bar rests 28, cross-staff
  ties 15, other 4). Pins: 41 full goldens + 1,617 sha256.
- Reader over the whole corpus: ~190 s, no VITIUM on any file, peak
  ~2 GB under custos (one score alone ~1.7 GB, desideratum
  …DC2DM0M8). Totality: 3,249 broken/random inputs, 0 crashes, ~3 s.
- Largest division LCM: 1,024 in the suite, 9,240 in the wild corpus
  (cap 2^31).

**Converter.** All 1,658 corpus files convert, and `carmina_legere`
accepts every result. The first run rejected 1,246; what carmen
enforces and how the converter now respects it is in the worklog
(dynamics inside hairpins, fermata spans, empty lines, bar lengths
a meter can express). nata_lux strict = the book's nata_lux note for
note in all five parts.

**`auditio -musicxml VIA [-strictus] [-titulus T]`** (M12). On its own:
the `<carmina>` book on stdout, reader monita + conversion report on
stderr; exit 0 clean, 3 with approximations, 1 unreadable file, reader
VITIUM, or strict refusal, 2 usage. With `-carmen`, `-censura`,
`-partitura`, `-recensio`, `-auscultare`, `-tonus`, `-pars`, `-iterare`
or `-via`: the converted book stands in for `-liber`, and `-carmen`
defaults to the converted song's title. Not with `-liber`,
`-carmina`, or the editing modes.

**Still open:** a fixture for "repeats not retaken after D.C." (no
suite file exercises it); the two desiderata above.

---

## AUDIENDA (not verified when written)

*Answered 2026-10-31 (§IX):* octave clefs — `<pitch>` is the written
octave and `clef-octave-change` doesn't move it; music21 agrees on all
357 nata_lux rows. LCM cap — the largest seen is 9,240. Versions —
wild 4.0 ×1,536, 3.1 ×122. music21 and jumps — it mostly ignores
`<sound>` jumps (122 corpus files differ, ours stand); grace notes —
zero length at the next onset on both sides. Instrument table —
drafted with the header (P9), voice words → part-name words → GM
family → aether.

- Whether MusicXML `<pitch>` under an octave clef (`clef-octave-change
  -1`, the nata_lux tenors) is the written or sounding octave: the
  prototype used it as is and Fran heard the result as right; the
  oracle settles it.
- The LCM cap (2^31) against real files — measured in the plan.
- Version differences in §III: from the 4.0 spec pages read by the
  research pass; anything older than 3.0 checked only through the
  suite's files.
- music21's handling of jumps and grace notes (§VI.2) — measured
  when the harness first runs.
- How many wild files are MusicXML 3.0 / 3.1 / 4.0 — not measured.
- Instrument table contents (§IV) — drafted with the converter
  header.
