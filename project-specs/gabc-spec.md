# gabc — a Gregorian chant reader, a converter into carmen, and a chant pane

*2026-11-03. Born in sexta right after the xml memory pass, answering
desideratum 01M62BR9MAJNCSMEGRAP4496NQ ("lectio gabc (Gregorio) in C —
cantus ex GregoBase directe in carmen"). Today chant enters the house
book through hand-written Python generators (`musica/fontes/salve.py`
reads one gabc file with regular expressions; `primus.py` / `ave.py`
hold a melody as MML strings). This spec makes that path a library, and
adds the thing MusicXML never got: a pane that SHOWS the notation.
Decisions recorded below came from a conversation with Fran on
2026-11-03; each says whose call it was.*

**What it is.** Three pieces, built in this order:

1. **`gabc`, the reader**: gabc text in, a **chant model** out that
   keeps everything the notation says — header, text syllables with
   styling, every note with its shape modifiers, the grouping of notes
   into neumes, clefs, bars, spacing, line breaks, custodes, rhythmic
   signs. It knows nothing about carmen or pixels.
2. **`gabc_carmen`, the converter**: chant model in, **carmen text**
   out (one `<carmen>` with `metrum="liberum"`), with a Solesmes
   reading of the rhythm and a chosen absolute pitch. Plus a side table
   linking each carmen note back to the gabc note it came from.
3. **The chant pane in vicus** (working name `antiphonale`): opens a
   `.gabc` file, draws square notes on a four-line staff with the text
   underneath, redraws when the file changes, plays it with the
   sounding syllable lit.

**Who uses it, and how.** The defining use (Fran, 2026-11-03): *Claude
writes gabc in the conversation, and Fran sees the composition change
in the pane as the gabc changes*, and can play it to hear it. Claude
can look too, through vicus screenshots (`-imago`), and fix its own
notation. Second: chants from GregoBase converted into the book like
MusicXML scores. Long-term direction: **the Divine Office** — praying
the hours from the pane (§VIII). Not in this spec's build, but nothing
here may block it.

**Why the name "gabc".** Like xml, toml, plist and musicxml, the reader
keeps the format's own name (glossarium entry, "nomen formae").

---

## 0. Decisions

| # | decision | whose call |
|---|---|---|
| G1 | **Three layers: reader (chant model), converter (carmen text), pane.** The reader does not include `carmen.h` or any drawing header; the converter is the only place that knows both gabc and carmen; the pane uses the reader for drawing and the converter for sound. Same shape as musicxml M1. | Fran agreed 2026-11-03 |
| G2 | **The model keeps the full notation** — every neume shape and grouping — so a renderer never goes back to the source text, even though carmen only needs pitch and length. | Fran: "lets keep neume shapes" |
| G3 | **The model also keeps typesetting information** the pane needs: line breaks the editor chose (`z`, `Z`), custos (`+`, `z0`), spacing between notes and neumes (`/`, `//`, `!`, space, `/[n]`), text styling (`<i> <b> <sc> <ul> <c>`), special characters (`<sp>R/</sp>`, `<sp>V/</sp>`, `<sp>'ae</sp>` …), translations and text above the staff (`[...]`, `<alt>`), the centred vowel (`{}`), elisions (`<e>`), and the header's annotation and initial style. | Claude, from Fran's pane goal; Fran agreed |
| G4 | **Neume shape is derived, not stored as a guess in two places.** gabc does not name neumes; a group of notes like `gh` is a pes because of its contour. The reader stores notes and their grouping exactly as written; **one function in the gabc library** classifies a group into a shape (punctum, virga, pes, clivis, torculus, porrectus, climacus, scandicus, salicus, pressus, bivirga/trivirga, distropha/tristropha, quilisma groups, oriscus groups, liquescent forms, and their compounds). The pane and any later tool use that function. Gregorio's own glyph rules (gregoriotex) are the reference for what a group is called. | Claude's call |
| G5 | **Coverage = what GregoBase contains.** The common syntax is modeled fully (§II). The long tail — TeX verbatim `<v>`, choral signs `[cs:]`, braces and above-staff ornaments `[ob:]`, `[ub:]`, `[oh:]`, note markings `[nm]`, `[ll:]` ledger forcing, polyphony `{ }` — is **kept as an opaque mark attached to its note or syllable, with its source text**, never silently dropped. Anything the reader cannot parse at all is a located diagnostic, and reading continues with the next note or syllable. | Fran agreed to "reject rare things clearly, never drop silently"; opaque marks = Claude's refinement |
| G6 | **Totality.** Any byte string gives a model or located diagnostics (line:col), never a crash or a hang. The xml_arbor / musicxml standard. | house standard |
| G7 | **Reading is fast enough to redo on every save.** A chant is a few KB; the pane re-reads the whole file on change, no incremental parsing. | Claude's call |
| G8 | **Rhythm: Solesmes, in the converter only, as one replaceable step.** The defaults are the ones `salve.py` uses today: a punctum is an eighth; `.` mora doubles; `_` episema lengthens by half; the note before a quilisma lengthens by half; unisons within a syllable are tied (bivirga, tristropha, pressus); a minor bar (`,` `;` `` ` ``) ends an incise with a sixteenth breath, a major bar (`:` `::`) with an eighth breath, the final bar with none. The reader never interprets rhythm. | Claude proposed, Fran agreed |
| G9 | **Pitch: staff position → scale step in the reader; absolute pitch in the converter.** The reader resolves each note to a step relative to the clef's do (or fa), and resolves accidentals (`x` flat, `y` natural, `#` sharp) with gabc's scope rule (an accidental holds until the end of the word or the next bar, whichever comes first). The converter places do at an absolute pitch: by default chosen so the chant sits in a comfortable range for a schola of men's voices; overridable per call (`do` = a pitch). | Claude's call; default range left to the builder (§VII) |
| G10 | **The converter writes the closest carmen it can and reports what it could not say**, musicxml M5's policy: an **approximation** changes what sounds or is sung; **marks** the performer interprets (ictus, episema as a sign, choral signs, ornaments) are counted, never approximations. A strict option refuses on the first approximation. | house policy (musicxml M5) |
| G11 | **The highlight link.** Besides the carmen text, the converter returns a table: for each carmen note (part, onset in pulses) → the gabc note and syllable it came from (index and line:col). The pane, playing through `cantor` in process, maps the current pulse to a note and lights its syllable and neume. Clicking a neume seeks there. | Claude's call, from Fran's "see and play/listen at the same time" |
| G12 | **Tool surface: `auditio -gabc VIA [-strictus] [-titulus T] [-do P]`**, exactly like `-musicxml` (musicxml M12): alone it prints the book on stdout and the report on stderr; with other auditio flags it is a book source. | Claude's call |
| G13 | **The pane follows the file.** It watches the opened `.gabc` file and re-reads and redraws within about a second of a change, with no keypress. On a parse error it **keeps showing the last good version** and shows the error (line:col and message) in a banner — never a blank pane. A save does **not** start playback; play / stop is Fran's key (and mouse, per house rule). | Fran agreed to the loop 2026-11-03 |
| G17 | **One file per chant, and the pane can be steered from outside.** Each composition is its own file under `musica/compositiones/<name>.gabc` (GregoBase imports keep their own place). Claude tells vicus which file the pane shows through a small vicus command, so Claude can open a chant and take a `-imago` screenshot without Fran's help; Fran can open files from the pane too. The mechanism (a `$`-command, or the existing quaestio port `lib/vicus_quaestio_porta.c`) is the builder's choice. **Built (part 2, 2026-11-05):** Fran opens a file with `$antiphonale(path)` in a scriba page (`~/` = home); Claude runs `praeco antiphonale <path>`, which writes an `antiphonale` event (absolute path) to the praeconium log vicus already follows - the newest such event reopens the front antiphonale pane on that file (`vicus_acervo_mutare`). The quaestio port is outbound only, so it was not used. Claude's eyes are `bin/neumae x.gabc -imago out.png` (same drawing as the pane), not a vicus screenshot. First composition: `musica/compositiones/ave_maria.gabc`. | Fran: option B, 2026-11-03; built 2026-11-05 |
| G18 | **A save during playback switches to the new version at the same pulse**, so Fran hears an edit mid-phrase. cantor already reads its song live (`cantor.h`: "mutatio exemplaris proximo transitu auditur"); the pane swaps the chant model, the carmen and the highlight table together, as one unit, and frees the old unit only once cantor holds the new one. If the new version is shorter than the current position, playback stops. On a parse error the old version keeps playing (G13). | Fran: option C, 2026-11-03; the details are Claude's |
| G14 | **Glyphs are drawn procedurally by us**, in a new pure library (working name `neumae_ductae`), the way `glyphae_ductae` draws box characters: given a shape and a staff-space size in pixels, produce an opacity mask, no allocation. No outline-font renderer exists in the house (`fons` is a 6×8 bitmap, `include/fons.h`), so SMuFL's chant fonts are out until one does. | Fran: "we might have to draw our own glyphs"; recon confirmed |
| G15 | **Line breaking in the pane, v1:** honour the gabc's own `z`/`Z` breaks, and otherwise fill each line greedily to the pane width. Knuth–Plass justification is a named future (§VIII), shared later with sheet music. | Claude's call |
| G16 | **Out of scope for this build**: writing gabc from carmen; polyphonic gabc (`{ }` voices) beyond keeping it as a mark; printing / PDF; the Office itself (§VIII); outline fonts. | Claude's call |

---

## I. What exists (2026-11-03)

- **The one real gabc file**: `musica/fontes/salve_2435.gabc` — Salve
  Regina (simple tone), mode V, Liber Usualis 1961 p. 279, GregoBase
  2435. Uses `'` ictus, `.` mora, `_` episema, `{}` centred vowel,
  `[ll:1]`, `[oh:h]`, bars `;` `::`.
- **The generators it replaces**: `musica/fontes/salve.py` (gabc →
  MML + verba + incises, 134 lines — its rhythm rules are G8's
  defaults; its pitch rule: clef c4, do on line 4 = F4 for mode V),
  `primus.py` / `ave.py` (melody typed as MML, not gabc).
- **carmen**: `include/carmen.h` — `metrum="liberum"`, one incise per
  measure (carmen.h:33-39), sung text `<verba!>` with `-` between
  syllables and `_` for melisma (carmen.h:133-139). Modal keys are
  already written as `clavis="f lydius"`, `"d dorius"`.
- **The sibling importer**: `lib/musicxml.c` + `lib/musicxml_carmen.c`
  and their spec `project-specs/musicxml-spec.md` — the approximation
  report, strict mode, the auditio source flag, the corpus discipline.
  Copy their shapes.
- **Playing in process**: `include/cantor.h` — plays a carmen model
  live, position in pulses (the highlight's clock); vicus already
  hosts sound (`lib/vicus_sonus_porta.c`, concentus).
- **Drawing**: vicus panes are pixel areas (≈960×573 of our pixels at
  SCALA II, text in 6-px cells — `lib/vicus.worklog.md`);
  `include/delineare.h` draws lines, polygons, ellipses;
  `lib/glyphae_ductae.c` is the model for procedural glyphs (opacity
  masks, ASCII-picture tests, plants for each rule).
- **Screenshots**: vicus `-imago <path>` (memory: app-screenshots).
- **Liturgical calendar**: `lib/calendarium_liturgicum.c` (secunda's
  US calendar) — for the Office later, not this build.

---

## II. The chant model (what the reader must keep)

The builder designs the structs; this is the content they must hold.

- **Header**: every `key:value;` pair in order, raw; the known ones
  also typed — `name`, `mode` (and mode differentia), `annotation`
  (up to two lines), `office-part`, `initial-style`, `language`,
  `book`, `transcriber`, `commentary`. Unknown keys kept raw.
- **Syllables** in order, each with: its text (as runs with style,
  special characters resolved to what they display, the centred-vowel
  range, elision marks), whether it starts a word, its translation /
  above-staff text if any, and its notes.
- **Notes**, each with: staff position (a–m, plus the extended letters
  modern gabc allows), the resolved scale step and accidental (G9),
  shape modifiers (punctum inclinatum by upper case, virga `v` `V`,
  stropha `s`, oriscus `o` `O`, quilisma `w` `W`, cavum `r`, linea
  `R`, quadratum `q`, liquescence `~` `<` `>`, initio debilis `-`,
  fusion `@`), rhythmic signs (`.` `..`, `_` with its digit, `'` with
  its digit), and its source line:col.
- **Grouping**: which notes belong to one neume group (contiguous in
  gabc), where the neumatic cuts and spacing hints fall (space, `/`,
  `//`, `!`, `/[n]`), and the shape from G4.
- **Staff events between notes**: clef (`c1`–`c4`, `f1`–`f4`, with the
  b-flat clef variants) and clef changes, bars (virgula `` ` ``,
  divisio minima `,`, minor `;`, maior `:`, finalis `::`, and the
  numbered / dotted variants), custos, line breaks, accidentals shown
  as signs.
- **Opaque marks** (G5): kind tag + source text + attachment point.
- **Diagnostics**: located, with kind (unparsed, unknown key, accidental
  out of place, bar inside a neume, …).

---

## III. The converter (gabc → carmen)

- One `<carmen>` with `metrum="liberum"`, one `<pars>` (instrument
  schola, like `salve.py`), one incise per bar-delimited phrase (G8),
  `<verba!>` from the syllables (special characters such as R/ V/ and
  styling dropped from the sung text; melismas as `_`), and a key
  that **describes the notes actually sung** (final pitch plus the
  scale in use: the Salve, sung on F with B♭ throughout, is
  `"f ionius"`, not `salve.py`'s `"f lydius"`, which in carmen means
  B natural). The mode number stays in a comment line. *(Claude's
  call, from the builder check: analysis tools read the key, and a
  mode label would make every B♭ look out of key.)*
- G8 rhythm, G9 pitch, G10 report, G11 link table.
- **Acceptance check**: converting `salve_2435.gabc` reproduces the
  notes, incises and verba that `salve.py` writes today, with `do`
  passed explicitly as F4 (the comment block and the key may differ —
  see above). After that the Python generator can retire.

---

## IV. The glyph library (`neumae_ductae`, working name)

- Pure: shape + staff-space size (px) + variant flags → opacity mask
  (one byte per pixel), plus the metrics the layout needs (width,
  where the stem and connecting lines attach). No allocation, like
  `glyphae_ductae`.
- Shapes: punctum (square), punctum inclinatum (diamond), virga
  (square with stem), quilisma (wavy), oriscus, stropha, liquescent
  heads (small and with tails), cavum and linea variants, the
  porrectus curve (designed by us — the one shape that is not
  rectangles and slants), connecting vertical lines between stacked
  notes, flat / natural / sharp signs, the c and f clefs, custos, the
  bars, mora dot, episema bar, ictus.
- Tested like `glyphae_ductae`: ASCII pictures at two sizes, laws
  across all shapes (nothing written past the mask, stems meet heads),
  a plant per rule.

---

## V. The pane (`antiphonale`, working name)

- Opens one `.gabc` file. Draws the title / annotation, the initial,
  four staff lines, neumes laid out per syllable with the text
  underneath (centred-vowel alignment), bars, custodes at line ends,
  lines broken per G15. Ledger lines for notes off the staff.
- G13 watching and error banner; G17 opening a file (from the pane, or
  steered by Claude's command); G11 play with highlight; G18 swap on
  save during playback; click to seek; keys and mouse both (house
  rule).
- Readable by Claude through `-imago`: the drawing must be legible at
  the screenshot's resolution.

---

## VI. Tests and corpus

- **Reader**: hand-written gabc fixtures for each construct in §II,
  with expected model dumps; totality fuzz (random bytes, truncated
  files) under ASan; diagnostics with exact line:col.
- **Shape classification (G4)**: a table of note groups → expected
  shape names, taken from Gregorio's documentation.
- **Converter**: the `salve.py` parity check (§III); fixtures per
  rhythm rule; the report.
- **Corpus**: a sample of GregoBase chants, downloaded under the house
  rules for untrusted files (own directory, Python `-I`), committed
  with a manifest like `probationes/fixa/musicxml/silvestria.manifestum`;
  every file must read without a crash, and the diagnostic counts are
  pinned so the long tail shows what to model next.
- **Oracle** (open question §VII-1): Gregorio can print its own
  analysis of a gabc file (glyph types per note). If Fran allows it
  once, under the third-party rules, it would give expected shape
  classifications for the whole corpus sample — built once, committed,
  then never needed again (the music21 pattern, musicxml M11).

---

## VII. Open questions

1. **Gregorio as a one-time oracle** for neume shapes (§VI) — it is
   part of TeX Live; installing it is Fran's call under the
   third-party rules (asked 2026-11-03, not yet answered; Claude leans
   yes). Without it, shape tests rest on hand-made tables from the
   documentation. Either way the reader and converter can be built
   first; only the G4 classifier's corpus test waits on this.
2. **Default absolute pitch (G9)**: the builder picks a rule (e.g.
   centre the chant's range near a fixed pitch for men's voices) and
   pins it in tests; Fran may want a different default later.

---

## VIII. Named futures

- **The Divine Office in the pane**: the day's hours from the
  liturgical calendar; antiphons and hymns as gabc; **psalms sung to
  psalm tones**, which needs a generator that takes a tone (a short
  gabc formula) and a pointed psalm text and produces gabc; texts of
  the Office from a public-domain source.
- Knuth–Plass line breaking, shared with sheet music.
- A gabc writer (carmen → gabc) so compositions made other ways can be
  shown.
- An outline-font renderer, then SMuFL plainchant glyphs.
- Sheet-music rendering reusing the pane's staff, spacing and text
  layers.

---

## AUDIENDA (not verified when written)

- GregoBase's download formats and licence terms for bulk use — not
  checked.
- Gregorio's dump output format and whether it names shapes the way
  G4 needs — from memory, not run.
- The exact accidental-scope rule (G9) and the full list of extended
  pitch letters — from memory of the Gregorio docs; check against the
  current documentation before the reader's tests are pinned.
- vicus file-watching: no existing pane watches a file for changes;
  the mechanism (polling mtime through the platform layer) is the
  builder's to choose.
