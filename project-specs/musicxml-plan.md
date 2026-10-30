# musicxml Plan — a MusicXML reader, and a converter into carmen text

**Ledger:** desideratum 01M64ZZYQ59DY0G0104SRTK439 (→ tractum at P1);
a parcum is born at P1 with one opus per task below.

**STATUS: DRAFT 2026-10-30** — written after Fran approved the spec
(e43ceb8d) and decided that music21 builds the goldens once and then
leaves (M11). Nothing executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention — no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note, as in `project-specs/xml-arbor-plan.md`.

**Goal:** `musicxml` reads any MusicXML document (`.xml`,
`.musicxml`, `.mxl`; partwise or timewise; 1.0–4.0) into an exact
score model with located diagnostics and a performance order;
`musicxml_carmen` turns that model into carmen text that
`carmina_legere` accepts, reporting every approximation; `auditio
-musicxml` exposes both; Tallis's *O nata lux* converts strictly and
matches the book's hand-made `nata_lux` note for note.

**Architecture:** spec §§II–V. `musicxml.c` walks `xml_documentum`'s
cooked view (or the root file of an `.mxl` opened with `sarcina`)
into `MusicxmlOpus`: one opus-wide division unit (LCM, M4), one
shared measure timeline (M15). `musicxml_ordo.c` turns barlines and
jumps into a list of (measure, pass) visits. `musicxml_tabulam_
scribere` dumps model + order as the canonical TSV that the oracle
compares. `musicxml_carmen.c` writes carmen STML text and a report.

**Tech stack:** C89 via latina.h, house flags, credo, piscina,
chorda, chorda_aedificator, xar, `xml/fontes/xml_documentum.h`,
`sarcina.h`, `carmen.h` (converter and its tests only), filum,
pythonica (`silva.commissio`, `silva.planta`). Python 3 + music21
10.5.2 in a throwaway venv for P7/P8 golden building ONLY.

**Spec:** `project-specs/musicxml-spec.md` (decisions M1–M15 in §0).
Precedents: `project-specs/xml-arbor-plan.md` (task shape, oracle
material, totality); `lib/plist.c` (a plain `lib/` consumer of
`xml_documentum.h`); `lib/codex.c:1253-1290` (EPUB via sarcina);
`musica/fontes/nata_lux.py` (the prototype this replaces);
`tools/auditio.c:507-560` (book loading).

## Global Constraints

- **Latin throughout** (docs and worklogs English). `musicxml` needs a
  glossarium line (`oratio/glossarium.stml`, class
  `ignotum-permissum`, "nomen formae, ut plist") before the first
  commit that uses it; `mxl`, `midi`, `lcm` likewise if they appear
  in identifiers — check each with `./oratio/quaere.sh` first.
  MusicXML element names (`backup`, `forward`, `divisions` …) appear
  only as string data.
- **Every latina.h macro is a forbidden identifier** (`nomen`, `per`,
  `casus`, `magnitudo`, `imus`, `L`, every Roman numeral 0–3999).
  Collisions to watch: "per" (MusicXML `per-minute`), "casus",
  `nomen` (part-name is `titulus`), `L`.
- **`chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED** —
  onsets and durations that can go negative before clamping (a
  `<backup>` past the start) are `s64`.
- **The reader never approximates** (M5): every onset and duration is
  an exact `s64` in opus divisions; LCM cap 2^31 → named vitium (M4).
- **The reader does not include `carmen.h`** (M1). Only
  `musicxml_carmen.c` and its test know both.
- **Totality** (M10): any bytes → an opus (never NIHIL unless memory
  fails) with diagnostics; no crash, no hang.
- **New C files through `./silva/scribe.sh`** (worktree rule); new
  `lib/` files need the aedilis tag in their header and
  `./tools/compile_tests_fontes_generare.sh` re-run (generated list,
  never hand-edited).
- **Root suites** (`probationes/probatio_musicxml*.c`) run under
  gate `radix` (`./compile_tests.sh musicxml`). The spec's gate
  `musicxml` = those suites; `musicxml-oraculum` is a new PORTAE
  entry (P8).
- **Exit 2 from any runner = NOTHING RAN.** Never read `$?` after a
  pipe.
- **Every gate born red by a plant that COMPILES** (`silva.planta`).
- **Commit through `silva.commissio`**, detached:
  `python3 build/plantae_n1/detach.py "python3 -u build/plantae_s4/committere.py build/plantae_s4/nuntiusNN.txt <portae> <viae>" build/plantae_s4/commissioNN.log`;
  tree FROZEN while gates run (no writes, not even to the plan).
- **Downloaded files are untrusted**: each source in its own
  directory under `build/musicxml/`; harnesses run `python3 -I` and
  take paths as arguments; never a script inside a download dir.
- **music21 is never a dependency** (M11): no gate, test or build
  step invokes it. Goldens are committed data, line 1 `DATA`.
- **Headers are reviewed by Fran before implementation**: P1
  (`musicxml.h` + `xml_locus`) and P9 (`musicxml_carmen.h`) STOP for
  review.

## Review Focus

Five input classes the spec implies but no feature list names; each is
pinned by a test in the named task.

1. **`<chord/>` after `<backup>`/`<forward>`** — a chord note takes
   the onset of the note just before it in document order, wherever
   the cursor moved before that note; chord notes never advance the
   cursor; a chord's members may have different durations (cursor
   advances by the FIRST note's duration). (P2,
   `_probare_acordum_post_regressum`.)
2. **Ties across repeats and endings** — a tie that starts in ending
   1 continues into whatever measure the performance visits next; if
   that measure's first note in the voice is not the same pitch, the
   tie ends there as two notes with a monitum, never a lost note.
   (P6, `_probare_ligatura_trans_voltam`.)
3. **Lyrics on chords and tied notes** — a syllable belongs to the
   chord's first note only; a tied continuation is the same carmen
   note (no `_`); a note with no syllable after a tie stop is NOT a
   melisma `_` of its own. (P10, `_probare_verba_acorda_ligaturae`.)
4. **Old and odd `.mxl` archives** — no `mimetype`, DEFLATE
   `container.xml`, root file in a subdirectory, absent `media-type`,
   extra rootfiles (first wins), a UTF-16 root document (named vitium,
   M13). (P3, `_probare_mxl_varia`.)
5. **Part-list and parts disagree** — a `<score-part>` with no
   `<part>` (monitum, part dropped), a `<part>` with no
   `<score-part>` (monitum, kept with its id as title), `<part-group>`
   elements interleaved (read past). (P2,
   `_probare_index_partium_discors`.)

---

## Task P1: Header draft — `musicxml.h`, `xml_locus`, glossarium (STOP: Fran reviews)

**Files:**
- Create: `include/musicxml.h` (types and prototypes only)
- Modify: `xml/fontes/xml_documentum.h`, `xml/fontes/xml_documentum.c`
  (`xml_locus`), `xml/probationes/probatio_xml_api.c` (its test)
- Modify: `oratio/glossarium.stml` (`musicxml`)

**Interfaces:**
- Produces (proposed; final names as Fran approves — later tasks use
  whatever P1's "Executed" note records):

```c
/* <aedilis corpus="lib/musicxml_ordo.c"/>   (musicxml.c by name) */

b32 xml_locus (constans XmlCoctus* nodus, i32* linea, i32* columna);
    /* FALSUM if no source position (synthetic node); entity-derived
     * nodes report the reference's position */

nomen enumeratio { MUSICXML_PARTWISE, MUSICXML_TIMEWISE } MusicxmlForma;
nomen enumeratio { MUSICXML_VITIUM, MUSICXML_MONITUM } MusicxmlGravitas;
nomen enumeratio { MUSICXML_NON_MUSICXML, MUSICXML_XML_MALUM,
    MUSICXML_SINE_PARTIBUS, MUSICXML_OPUS_RADIX, MUSICXML_MXL_MALUM,
    MUSICXML_CODIFICATIO, MUSICXML_DIVISIONES_NIMIAE,
    MUSICXML_FILIUS_DEEST, MUSICXML_NUMERUS_MALUS, MUSICXML_VALOR_IGNOTUS,
    MUSICXML_REGRESSUS_NIMIUS, MUSICXML_LIGATURA_PENDENS,
    MUSICXML_ACORDUM_PRIMUM, MUSICXML_MENSURA_BREVIOR,
    MUSICXML_PARS_DISCORS, MUSICXML_REPETITIO_APERTA,
    MUSICXML_SALTUS_CIRCULUS } MusicxmlVitiumGenus;

nomen structura { MusicxmlGravitas gravitas; MusicxmlVitiumGenus genus;
    i32 linea; i32 columna; chorda causa; } MusicxmlVitium;

MusicxmlOpus* musicxml_legere (chorda datum, Piscina* piscina);
    /* bytes: XML or .mxl (ZIP signature); NIHIL only if memory fails */
b32 musicxml_successus (constans MusicxmlOpus* opus);  /* no VITIUM */
b32 musicxml_vitia_scribere (constans MusicxmlOpus* opus,
    constans character* via, ChordaAedificator* exitus);
    /* 'via:linea:columna: monitum|vitium: causa' per line */

MusicxmlOrdo* musicxml_ordo (constans MusicxmlOpus* opus, Piscina* piscina);
    /* visits; never NIHIL unless memory; its own diagnostics in
     * MusicxmlOrdo.vitia (the opus stays constans) */

b32 musicxml_tabulam_scribere (constans MusicxmlOpus* opus,
    constans MusicxmlOrdo* ordo, ChordaAedificator* exitus);
    /* canonical TSV (grammar fixed in P2); ordo NIHIL = written order */
```

  plus the structs of spec §III (`MusicxmlOpus`, `MusicxmlPars`,
  `MusicxmlInstrumentum`, `MusicxmlMensura`, `MusicxmlAttributa`,
  `MusicxmlLineaMensurae`, `MusicxmlSignum`, `MusicxmlNota`,
  `MusicxmlSyllaba`, `MusicxmlOrdo` = `{ MusicxmlVisitatio* visitationes;
  i32 numerus; MusicxmlVitium* vitia; i32 numerus_vitiorum; }` with `MusicxmlVisitatio = { i32 mensura; i32
  transitus; s64 initium; }`), arrays as pointer + `i32 numerus_*`
  (carmen's shape, not Xar, so consumers index directly). Enumerated
  MusicXML values (syllabic, wedge type, articulation kinds …) are C
  enums with an `_IGNOTUM` member plus the text as written.

- [ ] **Step 0: Ledger** — desideratum …K439 → tractum; parcum
  "musicxml" with opera P1–P11 (`addere`, `intra` the music region);
  `praeco status "musicxml P1: header for fran"`.
- [ ] **Step 1: Measure** — find the materia call that turns a
  `MateriaNodus` into a `MateriaTractus` (`materia_nodus.h` around
  `materia_tractus_lexematis`); confirm `XmlCoctus.nodus` for an
  entity-expanded text node (NIHIL or the reference) on a three-line
  document with `&amp;`.
- [ ] **Step 2: Failing test** in `probatio_xml_api.c`:
  `_probare_xml_locus` — document `"<a>\n  <b x=\"1\"/>\n</a>"`:
  `xml_locus(b)` → VERUM, linea 2, columna 3; `xml_locus(NIHIL)` →
  FALSUM; a text node from `&amp;` reports the reference's line.
- [ ] **Step 3: Implement `xml_locus`**; `./xml/compile_probationes.sh
  api` green.
- [ ] **Step 4: Draft `include/musicxml.h`** with the API above and
  the §III structs, each field commented in Latin; head comment with
  USUS (read → successus → walk → ordo → tabula), the totality
  promise, the exact-time promise (M4), and what is read past (M2).
- [ ] **Step 5: Glossarium** `musicxml` (and any other format word
  the header needed); `./oratio/quaere.sh musicxml` finds it.
- [ ] **Step 6: STOP — show Fran the header and `xml_locus`.** Record
  his changes in the Executed note; later tasks use those names.
- [x] **Step 7: Commit** (gates `xml`): header, xml_documentum.{h,c},
  probatio_xml_api.c, glossarium.

**Executed 2026-10-30 (Fran approved the header as drafted).**
`xml_locus` needed one more field than planned: a node cooked from an
entity's replacement text carries positions INSIDE that text (an
element from `<!ENTITY a "<e/>">` reported 1:1), so `XmlCoctus` gained
`ancora` (the outermost `&a;` token, set from the cooker's existing
`c->ancora`); `xml_locus` prefers it, else `materia_tractus_nodi(NIHIL,
nodus)`, else walks up `parens`. Final names differ from the sketch
above — later tasks use THESE:
- forms `MUSICXML_FORMA_PARTIUM` / `MUSICXML_FORMA_TEMPORUM`; vitia
  `MUSICXML_ARCA_MALA` (was MXL_MALUM), `MUSICXML_NON_UTF8` (was
  CODIFICATIO), plus `MUSICXML_SALTUS_SINE_SONO` (D.C./D.S. in words
  only); `MusicxmlVitium` has `linea`/`columna` 0 for archive errors.
- key `MusicxmlClavis`, clef `MusicxmlClavicula`; `MusicxmlAttributa`
  flags `clavis_adest`, `metrum_adest`, `transpositio_adest`; additive
  time = `MusicxmlMetrum.partes[]` of `MusicxmlMetriPars {pulsus
  ("3+2"), summa, unitas}`.
- barline `MusicxmlLineaMensurae`: `repetitio` (`MUSICXML_REPETITIO_
  PRORSUM` forward / `_RETRORSUM` backward), `vices`, ending =
  `conclusio` (`MusicxmlConclusio`: `_NULLA/_INITIUM/_FINIS/
  _INTERRUPTA`), `conclusionum_textus`, `conclusiones[]`, fermata =
  `corona`.
- directions `MusicxmlDirectio` in `MusicxmlMensura.directiones`
  (`MUSICXML_DIRECTIO_TEMPUS … _SIGNUM` segno, `_CAUDA` coda, `_FINIS`
  fine, `_A_CAPITE`, `_A_SIGNO`, `_AD_CAUDAM`, `_PEDALE`, `_OCTAVA`,
  `_ALIA`; `valor_adest`/`valor`, `cuneus`, `sonus` = from `<sound>`).
- note marks `MusicxmlSignum` (`MUSICXML_SIGNUM_CORONA`, `_ACORDUM_
  FRACTUM` for arpeggiate, …); syllables `MusicxmlSyllaba {versus,
  versus_textus, locus (MusicxmlLocusSyllabae), textus, extensio,
  iuncta}`; note MIDI = `nota` (as carmen), tuplet = `ratio_notarum` /
  `ratio_spatii`, ties `ligatura_initium`/`ligatura_finis`.
- `musicxml_vitia_scribere(opus, ordo, via, exitus)` (ordo NIHIL
  allowed); `MusicxmlOrdo` carries its own `vitia` and `longitudo`.
- Test and fixture names in P6 say "volta(e)"; use `conclusio` /
  `conclusiones` in identifiers (fixture file names may stay).

---

## Task P2: Reader core — partwise, notes, voices, chords, divisions, attributes, the shared timeline, the TSV

**Files:**
- Create: `lib/musicxml.c`, `probationes/probatio_musicxml.c`
- Create fixtures in `probationes/fixa/musicxml/`: `nota_una`,
  `divisiones_mutatae`, `voces`, `acordum`, `tactus_brevis`,
  `mensurae_inaequales`, `vitia_musica`, `index_partium` (all
  `.musicxml`, hand-written, MusicXML 4.0, one feature each, a
  comment on line 2 saying what it pins)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: P1 header; `xml_documentum_legere`, `xml_filius`,
  `xml_frater`, `xml_attributum`, `xml_textus_totus`, `xml_locus`.
- Produces: `musicxml_legere` for partwise XML; `musicxml_tabulam_
  scribere` with `ordo == NIHIL`; the TSV grammar every oracle uses:

```
DATA musicxml tabula 1
# pars initium longitudo midi littera alteratio gratia versus
P1	0	1	60	C	0	0	-
```
  one row per sounding note after merging ties (rests omitted, as
  music21's `.notes`); `initium`/`longitudo` as reduced fractions of
  a QUARTER (`3/2`, `0`, `1`); chord members one row each, rows sorted
  by (pars index, initium, midi); `littera`/`alteratio` = SOUNDING
  spelling, alter as written (`-1`, `0.5`); `gratia` 0/1 (grace rows
  have longitudo `0`); `versus` = `n:syllabic:text` joined by `|`
  over verses in number order, `-` if none; text with tab/newline →
  space.

- [ ] **Step 1: Measure** — `nata_lux_80317.mxl` unpacked (scratchpad
  `mxl_nata/`): count `<divisions>` values, backups, forwards, chords;
  confirm how `<part-list>` orders parts against `<part>` elements.
- [ ] **Step 2: Fixtures** — write the eight files above. Expected
  values are in the tests below; each fixture is ≤ 60 lines.
- [ ] **Step 3: Failing tests** in `probatio_musicxml.c` (credo, as
  `probatio_piscina.c`; a helper `_legere_fixum(c, "x.musicxml")`
  reads via `filum_legere_totum`):
  - `_probare_nota_una`: forma PARTWISE, versio "4.0", divisiones 1,
    titulus "Probatio" (from `<work-title>`), compositor "Anonymus"
    (`<creator type="composer">`), one part id "P1" titulus "Cantus",
    one measure numerus "1"
    initium 0 longitudo 1, one note littera 'C' alteratio 0 octava 4
    sonans midi 60, positio 0 longitudo 1; `musicxml_successus` VERUM;
    tabula equals the two-line golden above byte for byte.
  - `_probare_divisiones_lcm`: P1 divisions 2, P2 divisions 3, and P2
    changes to 6 mid-measure → opus divisiones 6; P2's duration-1
    note under divisions 3 → longitudo 2; every onset exact.
  - `_probare_divisiones_nimiae`: inline text with divisions
    2147483647 in P1 and 2 in P2 → one VITIUM
    `MUSICXML_DIVISIONES_NIMIAE`, successus FALSUM, no crash.
  - `_probare_voces`: voice 1 four quarters, `<backup>` 4, voice 2
    half rest + half note, `<forward>` gap → voice-2 note positio 2
    (quarters), vox "2"; forward leaves no note.
  - `_probare_acordum_post_regressum` (Review Focus 1): backup then
    C4 (dur 2) + chord E4 (dur 1) + G4 → all three positio equal; next
    non-chord note at positio + 2.
  - `_probare_tactus_brevis`: `implicit="yes"` pickup of 1 quarter in
    3/4 → mensura 0 numerus "0", implicit VERUM, longitudo 1 quarter;
    mensura 1 initium = 1 quarter.
  - `_probare_mensurae_inaequales` (M15): P1 measure 1 = 4 quarters,
    P2 measure 1 = 3 → measure 2 initium 4 quarters in both parts; P2
    mensura 1 longitudo 3; one MONITUM `MUSICXML_MENSURA_BREVIOR`
    naming P2, measure "1".
  - `_probare_vitia_musica`: note without `<duration>` → MONITUM
    FILIUS_DEEST (note dropped) with its line:col; backup past
    measure start → REGRESSUS_NIMIUS, cursor clamped to 0; tie start
    with no stop → LIGATURA_PENDENS; `<chord/>` on a voice's first
    note → ACORDUM_PRIMUM (treated as a plain note); successus VERUM.
  - `_probare_index_partium_discors` (Review Focus 5): part-group
    read past; score-part without part → monitum, absent from
    `partes`; part without score-part → kept, titulus = id.
  - `_probare_non_musicxml`: `"<html/>"` → VITIUM NON_MUSICXML;
    `"<a"` → VITIUM XML_MALUM with xml's diagnostic text passed
    through; empty bytes → VITIUM; all return non-NIHIL.
  - `_probare_attributa`: key fifths -2 mode minor, time 6/8, clef
    G2 and F4 on two staves, staves 2, all at positio 0 in mensura 1;
    a second measure with additive time `3+2`/`8` kept as the list
    (3, 2) over 8, and a non-traditional key (`<key-step>`/
    `<key-alter>`) kept as steps with fifths flagged absent.
- [ ] **Step 4: Run** `./compile_tests.sh musicxml` → fails to link
  (no `lib/musicxml.c`).
- [ ] **Step 5: Implement `lib/musicxml.c`** (via `./silva/scribe.sh`):
  two passes — pass 1 collects every `<divisions>` and computes the
  LCM with overflow check; pass 2 walks measures with a per-part
  cursor in opus divisions (`s64`), chord rule as Review Focus 1,
  measure length = max cursor reached; then the shared timeline
  (M15: start[k+1] = start[k] + max over parts of longitudo[k]).
  Sounding pitch = written here (transposition arrives in P4). MIDI =
  12·(octave+1) + step + round(alter) (microtones keep their decimal
  `alteratio`; the integer MIDI rounds half up — the converter
  reports it, P10).
- [ ] **Step 6: Regenerate** `./tools/compile_tests_fontes_generare.sh`;
  green; plant (`silva.planta`) — break the LCM to max → divisiones
  test red, restore.
- [x] **Step 7: Commit** (gate `radix`).

**Executed 2026-10-30.** Measured first: the Tallis file is MusicXML
2.0 (public DTD, never fetched), divisions 480 everywhere, no
`<forward>`, `<chord>`, `<tie>` or `<backup>` (the "forward" hits are
repeat directions), measures numbered from 0 — so the hand fixtures
carry the coverage. `lib/musicxml.c` (two passes as planned) and a
minimal `lib/musicxml_ordo.c` (written order, one pass per measure —
the header's aedilis tag requires the file to exist; P6 replaces the
body). The suite went green on first build (11 sections, 159
assertions), so the plants mattered: **the LCM plant (`max` instead
of LCM) SURVIVED** — the fixture's divisions 2, 3, 6 have LCM = max =
6. Fixed the fixture (P2 changes to 4 mid-measure → LCM 12, max 4);
then all three plants (LCM, chord onset, shared timeline) went red.
Details fixed here: the TSV header line is tab-separated like the
rows; a `<chord/>` whose previous note (document order, same measure)
is in another voice — or after a `<backup>`/`<forward>` — is
ACORDUM_PRIMUM and becomes a plain note; a pitch without a legible
step or octave becomes a rest with a monitum (time kept); the reader
keeps going after every monitum. `score-timewise` and `.mxl` bytes are
named vitia until P5 / P3 replace them.

---

## Task P3: `.mxl` — container, root file, odd archives

**Files:**
- Modify: `lib/musicxml.c`, `probationes/probatio_musicxml.c`
- Create fixtures: `nota_una.mxl` (mimetype STORED first, DEFLATE
  rest), `sine_mimetype.mxl`, `radix_in_directorio.mxl` (root at
  `score/x.musicxml`, two rootfiles), `opus.mxl` (rootfile is an
  `<opus>`), `container_malum.mxl`, `utf16.mxl` (root in UTF-16LE),
  each built once with `/usr/bin/zip` from files in
  `probationes/fixa/musicxml/mxl_fontes/` (sources committed too;
  `PROVENIENTIA.md` there gives the zip commands)

**Interfaces:**
- Consumes: `sarcina_aperire`, `sarcina_invenire`, `sarcina_legere`.
- Produces: `musicxml_legere` accepting `.mxl` bytes; `.mxl` vitia
  carry linea/columna 0 and the archive entry name in `causa`.

- [ ] **Step 1: Measure** — `nata_lux_80317.mxl`'s entries
  (`sarcina` via a two-line probe, or `unzip -l`): mimetype present?
  container's media-type?
- [ ] **Step 2: Failing tests** `_probare_mxl` (nota_una.mxl → same
  model as nota_una.musicxml: tabulae byte-equal) and
  `_probare_mxl_varia` (Review Focus 4): sine_mimetype ok;
  radix_in_directorio reads the FIRST rootfile; opus.mxl → VITIUM
  OPUS_RADIX; container_malum → VITIUM MXL_MALUM naming
  `META-INF/container.xml`; utf16 → VITIUM CODIFICATIO; random bytes
  starting `PK\x03\x04` → VITIUM MXL_MALUM, no crash.
- [ ] **Step 3: Implement** — detect `PK\x03\x04`; container.xml →
  first `rootfile/@full-path`; read it; recurse into the XML path.
  `.mxl` inside `.mxl` is not followed (VITIUM).
- [ ] **Step 4: Green; plant** (take the last rootfile) → red; restore.
- [x] **Step 5: Commit** (gate `radix`).

**Executed 2026-10-30.** Measured: the Tallis `.mxl` has NO `mimetype`
entry and a DEFLATE `container.xml` (rootfile with `media-type`) — the
real file already pins two of the odd cases. Fixtures as planned plus
`radix_absens.mxl` (rootfile names a missing entry); sources and the
exact `zip -X` commands in `mxl_fontes/PROVENIENTIA.md`. The UTF-16
check (BOM `FE FF`/`FF FE`, or a NUL in the first two bytes) runs at
the top of the XML path, so plain UTF-16 XML is `MUSICXML_NON_UTF8`
too, not an xml diagnostic. An archive inside an archive is refused
by name. New section `nata lux (fons verus)`: the real file reads
with NO diagnostics at all (5 parts × 4 measures, 373 notes,
divisions 480, measure "0" free-time). Both plants red (second
rootfile; UTF-16 check off).

---

## Task P4: Lyrics, ties, tuplets, grace and cue notes, transposition, unpitched parts and instruments

**Files:**
- Modify: `lib/musicxml.c`, `probationes/probatio_musicxml.c`
- Create fixtures: `verba`, `ligaturae_tupli`, `gratiae`,
  `transpositio`, `partitura_concentus` (4.0 concert-score),
  `percussio`

**Interfaces:**
- Produces: `MusicxmlNota.syllabae` / `ligatura` / `tuplus` /
  `gratia` / `sonans`, `MusicxmlPars.instrumenta`, filled as §III.

- [ ] **Step 1: Failing tests:**
  - `_probare_verba`: two verses; syllabic begin/middle/end/single;
    `<elision>` gives two syllabae on one note with `elisio` VERUM;
    `<extend>` flag kept; a note with verse-1 text and none for verse
    2 has one syllaba. TSV `versus` column for the elided note =
    `1:end:a|1:begin:in` (both, in document order).
  - `_probare_ligaturae`: tie across the barline: two notes in the
    model (one per measure) with ligatura start / stop; TSV merges
    them into one row of the summed duration; `<tied>` without
    `<tie>` does not tie.
  - `_probare_tupli`: triplet eighths (time-modification 3:2) under
    divisions 3 → longitudo 1 each; a septuplet (7:4) under
    divisions 7 exact; tuplus actual/normal kept.
  - `_probare_gratiae`: grace with `slash="yes"` → longitudo 0,
    gratia VERUM, next note's positio unchanged; `steal-time-
    previous="20"` kept as written; cue note flagged, duration
    counted.
  - `_probare_transpositio`: B♭ clarinet (`diatonic -1 chromatic
    -2`), written D5 → sonans midi 72, sounding littera 'C'; written
    F♯5 → sounding E5 (littera 'E', alteratio 0); octave-change -1
    applied.
  - `_probare_partitura_concentus`: `<concert-score/>` with the same
    `<transpose>` → sonans = written (never transposed twice).
  - `_probare_percussio`: unpitched note (display-step E4) with
    `<instrument id="P1-I38"/>` → genus UNPITCHED, instrument found
    in `instrumenta` with midi-unpitched 39; TSV midi = 38 (unpitched
    = midi-unpitched − 1, music21's numbering checked in P7).
- [ ] **Step 2: Implement**; sounding spelling = written letter moved
  by `diatonic` (+ octave-change·7), alter chosen so that MIDI moves
  by `chromatic` (+12·octave-change).
- [ ] **Step 3: Green; plant** (ignore `<concert-score>`) → red; restore.
- [x] **Step 4: Commit** (gate `radix`).

**Executed 2026-10-30.** Six fixtures as planned; ties needed no new
code (P2's tie bookkeeping and the TSV merge already held, and the
`ligaturae` section passed before any P4 code). Decisions fixed here:
a `<lyric>` without `number` is verse 1, a non-integer number is verse
0; syllables are stored in verse order (stable — the fixture writes
verse 2 before verse 1 on purpose); `<extend>` marks the last syllable
of its own lyric; transposition is part state from the `<transpose>`
onward, sounding letter = written letter moved by diatonic (+7 per
octave-change), MIDI moved by chromatic (+12 per octave-change),
sounding alter = whatever remains; `<concert-score/>` is read from
`<defaults>` and disables it; an unpitched note's MIDI comes from its
`<instrument>`'s `midi-unpitched` − 1 (or the part's only
instrument), else from the display pitch. Plants (concert-score
ignored; verse sort off; alter without the natural interval) all red.
Lint: `tupli` refused in a test name → `_rationes_temporis_probare`.

---

## Task P5: Directions, barlines, marks, and the timewise form

**Files:**
- Modify: `lib/musicxml.c`, `probationes/probatio_musicxml.c`
- Create fixtures: `directiones`, `lineae_mensurae`, `timewise`
  (the same music as `voces`, timewise), `versio_nulla` (no version
  attribute, 1.0 elements only)

**Interfaces:**
- Produces: `MusicxmlMensura.signa` (tempo, dynamics, wedge, words,
  rehearsal, segno, coda, fine, dacapo, dalsegno, tocoda, pedal,
  octave-shift, each with positio, vox, baculum), `lineae`
  (location, style, repeat direction + times, ending numbers + type,
  fermata), `MusicxmlNota.notae_signa` kinds, `MusicxmlOpus.forma`
  TIMEWISE.

- [ ] **Step 1: Failing tests:**
  - `_probare_directiones`: `<sound tempo="72.5"/>` at quarter 2 →
    tempo signum positio 2, valor 72.5 (as double, text kept);
    `<metronome>` alone (no sound) → tempo from beat-unit + per-minute
    (dotted quarter = 60 → 90 quarters); dynamics `mf` and `sfz`
    kept as kinds + text; wedge crescendo start at quarter 1, stop at
    quarter 3 (mid-measure); words "rit." kept; rehearsal "A" kept;
    an `<offset>` moves the signum's positio (`sound="yes"` only).
  - `_probare_notae_signa`: fermata, staccato, accent, trill-mark,
    slur start/stop number 1, arpeggiate → kinds counted on the note.
  - `_probare_lineae`: forward repeat left, backward repeat right
    `times="3"`, ending number="1, 2" type start/stop, ending 3
    discontinue — all in `lineae` of the right measures.
  - `_probare_timewise`: `timewise` → model equal to `voces`
    (tabulae byte-equal), forma TIMEWISE.
  - `_probare_versio_nulla`: versio "" and reads as 1.0.
- [ ] **Step 2: Implement**; timewise: transpose the measure/part
  nesting into the same per-part walk (one code path after that).
- [ ] **Step 3: Green; plant** (drop `<offset>`) → red; restore.
- [ ] **Step 4: Commit** (gate `radix`).

---

## Task P6: Performance order — repeats, endings, jumps

**Files:**
- Create: `lib/musicxml_ordo.c`; extend `probationes/probatio_musicxml.c`
- Create fixtures: `repetitiones` (forward/backward, times 3),
  `voltae` (endings "1, 2" / "3"), `saltus` (segno, D.S. al coda,
  tocoda, coda, fine via `<sound>`), `saltus_verbis` (D.C. as words
  only), `repetitio_sine_initio` (backward without forward),
  `saltus_circulus` (dalsegno whose target loops forever),
  `ligatura_volta` (tie into ending 1 → after repeat)

**Interfaces:**
- Consumes: `lineae`, `signa` from P5.
- Produces: `musicxml_ordo`; `musicxml_tabulam_scribere(opus, ordo,
  …)` writes rows in performance order with onsets on the unrolled
  timeline (visit start = sum of shared measure lengths visited
  before).

- [ ] **Step 1: Failing tests** (visits as `mensura:transitus`
  lists, measures by index):
  - `_probare_repetitiones`: measures 0..3, repeat 1..2 times 3 →
    `0:1 1:1 2:1 1:2 2:2 1:3 2:3 3:1`.
  - `_probare_voltae`: 0 |: 1 [1,2: 2] [3: 3] :| 4 → `0:1 1:1 2:1
    1:2 2:2 1:3 3:3 4:1`.
  - `_probare_saltus`: segno at 1, tocoda at 2, dalsegno at 3, coda
    at 4 → `0 1 2 3 1 2 4` (passes 1,1,1,1,2,2,1); with `fine` instead
    of coda: stops at fine on pass 2.
  - `_probare_saltus_verbis`: "D.C." words without `<sound>` →
    jump taken AND a monitum naming the words (spec §III).
  - `_probare_repetitio_sine_initio`: backward at 2, no forward →
    repeats from measure 0, monitum REPETITIO_APERTA.
  - `_probare_saltus_circulus`: terminates; visits cut after the
    second identical (measure, state); VITIUM-free, monitum
    SALTUS_CIRCULUS.
  - `_probare_ligatura_trans_voltam` (Review Focus 2): tie start in
    ending 1's last note; the measure after the backward jump starts
    with the same pitch → one merged TSV row; with a different pitch
    → two rows + monitum LIGATURA_PENDENS.
  - `_probare_ordo_nata_lux`: `nata_lux_80317.mxl` → 5 measures
    visited (0 1 2 3 3 with measure 3 pass 2), matching
    `nata_lux.py`'s expansion.
- [ ] **Step 2: Implement** — a state machine over measures: repeat
  stack (start index, times, pass), ending eligibility by pass
  number, jump flags consumed once per pass (D.S./D.C. after the
  jump: repeats not re-taken, the common practice music21 uses —
  verify in P7), loop guard on (measure, pass, jump state).
- [ ] **Step 3: Green; plant** (ignore `times`) → red; restore.
- [ ] **Step 4: Commit** (gate `radix`).

---

## Task P7: The committed suite and its goldens (music21 once)

**Files:**
- Create: `tools/musicxml_arcessere.sh` (fetches the suite at a pinned
  commit and the corpora of P8 into `build/musicxml/<fons>/`, sha256
  checked), `tools/musicxml_oraculum.py` (music21 → TSV, P2 grammar),
  `tools/musicxml_tabula.c` (or an `auditio` dump flag — decide in
  Step 1: a tiny tool that prints our TSV for a path)
- Create: `probationes/fixa/musicxml/suite/` (183 files + LICENSE +
  `PROVENIENTIA.md`: URL, commit, date, licence),
  `probationes/fixa/musicxml/aurum/*.tsv` (one per suite file and per
  hand fixture, line 1 `DATA`), `probationes/probatio_musicxml_suite.c`
- Modify: `.gitignore` (`build/musicxml/` already under build — check)

**Interfaces:**
- Consumes: `musicxml_legere`, `musicxml_ordo`, `musicxml_tabulam_
  scribere`.
- Produces: the golden contract: `aurum/<basis>.tsv` = our TSV for
  `<basis>`; `# divergentia: <reason>` lines (ignored by the
  comparison) above rows where music21 disagreed and we kept ours.

- [ ] **Step 1: Measure** — venv in `build/musicxml/oraculum/`
  (`python3 -m venv`, `pip install music21==10.5.2`); run the harness
  over the suite; record in `PROVENIENTIA.md`: files music21 rejects,
  how it treats grace notes, jumps (§VI.2 unknowns), unpitched MIDI
  numbering, octave clefs (spec AUDIENDA — the nata_lux tenors), and
  the largest LCM seen. A finding that contradicts P2–P6 re-slices
  this task (shown to Fran).
- [ ] **Step 2: Diff** our TSV against music21's for all 183 + the
  hand fixtures; classify each differing file: OUR BUG (fix in the
  reader, with a hand test in `probatio_musicxml.c`), THEIR BUG
  (`# divergentia:` with the reason), FORMAT (harness fix).
- [ ] **Step 3: Write the goldens** from OUR output once the diff is
  only THEIR BUG rows; the 99-group (broken exports) goldens may be
  diagnostics-only (`# vitia:` lines listing kinds).
- [ ] **Step 4: Failing gate** `probatio_musicxml_suite.c`: for each
  golden, read the source, dump, compare ignoring `#` lines; report
  the first differing row with file and line. Plant: one golden row
  changed → red; restore.
- [ ] **Step 5: Delete the venv** (`rm -rf build/musicxml/oraculum`)
  and re-run the gate: green without music21 (proves M11).
- [ ] **Step 6: Commit** (gate `radix`; the suite files are data, so
  check `examen` ignores them — they are not `.c`).

---

## Task P8: Wild corpus differential and totality

**Files:**
- Create: `probationes/fixa/musicxml/silvestria.manifestum`
  (`sha256  via` for every fetched file), `probationes/fixa/musicxml/
  aurum_silvestre/*.tsv` (~60 full goldens: 25 Lieder, 25 quartets,
  nata_lux, the CPDL files Fran supplies), `probationes/fixa/musicxml/
  silvestria.sigilla` (`sha256-of-our-TSV  via` for the rest),
  `tools/musicxml_silvestria.sh` (the gate runner),
  `probationes/probatio_musicxml_totalitas.c`
- Modify: `pythonica/silva.py` (`PORTAE['musicxml-oraculum'] =
  (['./tools/musicxml_silvestria.sh'], r'MUSICXML SILVESTRIA:
  (sana|FRACTA|OMISSA)')`), `tools/musicxml_arcessere.sh`

**Interfaces:**
- Produces: gate `musicxml-oraculum` printing `MUSICXML SILVESTRIA:
  sana N/N` or `FRACTA` with the failing paths, or `OMISSA: <reason>`
  when `build/musicxml/` lacks the corpus (never a silent pass).

- [ ] **Step 1: Measure** — fetch OpenScore Lieder and String
  Quartets (CC0) at pinned commits; count files, MusicXML versions
  (spec AUDIENDA), read time for all, failures by kind.
- [ ] **Step 2: Oracle once more** — new venv; music21 over the
  corpus; triage as P7 Step 2 (fixes get hand tests); pick the ~60
  full-golden files to cover what the suite does not (voices,
  transposing instruments, lyrics in several verses, D.S./coda).
- [ ] **Step 3: Pins** — full goldens for the sample; sha256 of OUR
  TSV for the rest (pinned only where music21 agreed or the
  divergence is recorded in `PROVENIENTIA.md`); delete the venv.
- [ ] **Step 4: Gate runner** — builds/uses the P7 dump tool, checks
  the manifest's sha256 first (a changed download is FRACTA naming
  the file, not a reader regression).
- [ ] **Step 5: Totality** `probatio_musicxml_totalitas.c` (shape of
  `xml/probationes/probatio_xml_totalitas.c`): every suite file
  truncated at every 97th byte; 200 seeded byte flips per file
  (`sors`); 1,000 random strings; 100 random `PK\x03\x04…` blobs;
  every call returns non-NIHIL; `CREDO_NON_RUIT` / `CREDO_NON_PENDET`
  wrap the loop; plus `musicxml_ordo` on each result.
- [ ] **Step 6: Plants** — one sigillum changed → FRACTA; corpus
  directory renamed → OMISSA; restore.
- [ ] **Step 7: Commit** (gates `radix`, `pythonica`,
  `musicxml-oraculum`).

---

## Task P9: Converter header — `musicxml_carmen.h` (STOP: Fran reviews)

**Files:**
- Create: `include/musicxml_carmen.h`

**Interfaces:**
- Produces (proposed):

```c
nomen enumeratio { /* approximations (M5): change what sounds or is sung */
    MXC_MICROTONUS, MXC_EXTRA_RETICULUM, MXC_GRATIA_OMISSA,
    MXC_NOTA_PARVA_OMISSA, MXC_VERSUS_OMISSUS, MXC_TEMPUS_ROTUNDATUM,
    MXC_TEMPUS_EXTRA_FINES, MXC_DYNAMICA_IGNOTA, MXC_CLIVUS_MOTUS,
    MXC_CLAVIS_NON_TRADITA, MXC_METRUM_ADDITIVUM, MXC_PERCUSSIO_OMISSA,
    MXC_APPROXIMATIONUM_GENERA } MusicxmlApproximatioGenus;
nomen enumeratio { /* unconverted marks: counted, never refuse */
    MXC_ARCUS, MXC_ARTICULATIO, MXC_ORNAMENTUM, MXC_VERBA_DIRECTIONIS,
    MXC_SIGNUM_EXERCITII, MXC_PEDALE, MXC_TECHNICUM, MXC_ARPEGGIO,
    MXC_DIRECTIO_ALIA, MXC_SIGNORUM_GENERA } MusicxmlSignumGenus;

nomen structura { MusicxmlApproximatioGenus genus; chorda pars;
    chorda mensura /* as written */; i32 transitus; i32 linea;
    i32 columna; chorda descriptio; } MusicxmlApproximatio;

nomen structura { b32 strictus; chorda titulus /* vacua = ex opere */;
    chorda instrumentum /* vacua = tabula */; } MusicxmlCarmenForma;

nomen structura {
    MusicxmlApproximatio* approximationes; i32 numerus_approximationum;
    i32 signa[MXC_SIGNORUM_GENERA];
    chorda* structurae; i32 numerus_structurarum;  /* M7 splits, unrolled
                                                    * repeats, instrument picks */
} MusicxmlCarmenRelatio;

b32 musicxml_carmen_scribere (constans MusicxmlOpus* opus,
    constans MusicxmlCarmenForma* forma, Piscina* piscina,
    ChordaAedificator* exitus, MusicxmlCarmenRelatio* relatio);
    /* writes ONE <carmen>…</carmen> (no <carmina> wrapper); FALSUM =
     * strict refusal (relatio holds the first approximation, exitus
     * untouched), opus with VITIUM, or memory */
b32 musicxml_relationem_scribere (constans MusicxmlCarmenRelatio* r,
    ChordaAedificator* exitus);
```

  plus the instrument table as data in the header comment: part-name
  words (soprano/superius/cantus → `schola` vox superius; alto/altus/
  contratenor → altus; tenor → tenor; bass/bassus → bassus) first,
  then General MIDI program families → house instruments (Step 1
  picks them from `instrumentarium.stml`).

- [ ] **Step 1: Measure** — list house instruments
  (`instrumentarium.stml`); MML spelling of double sharps/flats in
  `cantus.h` (`c++`? if absent, the converter respells enharmonically
  — an approximation of SPELLING only, recorded as a structural note,
  not M5); how `carmina_legere` reports a bad `<verba!>`.
- [ ] **Step 2: Draft the header**; head comment carries the mapping
  table of spec §IV in brief and the "text stays truth" rule (every
  output is read back by `carmina_legere` in the tests).
- [ ] **Step 3: STOP — Fran reviews.** Record changes in the Executed
  note.
- [ ] **Step 4: Commit** (no gate beyond lint).

---

## Task P10: Converter — mapping, report, strict

**Files:**
- Create: `lib/musicxml_carmen.c`, `probationes/probatio_musicxml_carmen.c`
- Create fixtures as needed (reuse P2–P6 fixtures first)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: P1/P9 headers, `musicxml_ordo`, `carmina_legere`.
- Produces: `musicxml_carmen_scribere`, `musicxml_relationem_scribere`.

Converter decisions this task fixes (spec leaves them open):
- **One frustum per carmen part at `mensura="1"`**, `|` at every
  written measure boundary; rests fill voice gaps.
- **Durations**: pulses = d·960/divisiones; on-grid → the shortest
  MML spelling from {1,2,4,8,16,32,64 with 0–2 dots, 3,6,12,24,48,
  5,10,20,40, 15,30,60}, tied with `&` when one length can't say it
  (greedy largest-first, ties across `|` allowed as in `nata_lux`).
  Off-grid → nearest pulse, the measure's total preserved by
  adjusting the last note, one MXC_EXTRA_RETICULUM per note.
- **Octaves**: `o N` written whenever the octave changes (no `<`/`>`).
- **Positions of song-level elements**: `mensura="M"` at a measure
  start, else `pulsus="N"` (exact; ictus can be fractional).
- **Verba**: syllables of the chosen verse in note order; `-` after
  begin/middle, space after end/single; `_` for each later pitched
  note without a syllable (spec §IV); elided syllables concatenated
  into one (vowel = first, carmen's rule).
- **Titles**: part-name folded to `[a-z0-9_]+` (lowercase ASCII,
  others → `_`, runs collapsed, trimmed; empty → `pars_<k>`);
  collisions and M7 splits get `_2`, `_3`.

- [ ] **Step 1: Failing tests** — each converts a fixture, asserts
  substrings of the text AND `carmina_legere` accepts it:
  - `_probare_carmen_nota_una`: text contains `metrum="4/4"`,
    `<pars titulus="cantus"`, `o4 c4`; carmina_legere VERUM; the
    song's one note: pulsus 0, longitudo 960, nota 60.
  - `_probare_longitudines`: quarter `4`, dotted half `2.`, triplet
    quarter `6`, quintuplet sixteenth `20`, five quarters `1&4`;
    septuplet → MXC_EXTRA_RETICULUM ×7 and the measure still 3840.
  - `_probare_voces_partes` (M7): `voces` → parts `cantus` and
    `cantus_2`; `acordum` → three parts; structural notes list both.
  - `_probare_tactus_brevis_metrum` (M14): pickup → `<metrum
    mensura="1" valor="1/4"/>` and `<metrum mensura="2"
    valor="3/4"/>`, and the comment line mapping carmen bar 1 → "0".
  - `_probare_liberum` (M6): senza-misura → `metrum="liberum"`, one
    `|` per written measure in the first part.
  - `_probare_dynamica_tempus`: `mf` → `<dynamica pars=… valor="mf"`;
    `sfz` → MXC_DYNAMICA_IGNOTA; wedge ending mid-measure → `usque`
    = next measure + MXC_CLIVUS_MOTUS; tempo 72.5 → `72` +
    MXC_TEMPUS_ROTUNDATUM; tempo 500 → 400 + MXC_TEMPUS_EXTRA_FINES;
    fermata → `<fermata … factor="2"/>`; key change in measure 2 →
    `<clavis mensura="2" valor="…"/>`, time change → `<metrum
    mensura="2" …/>`.
  - `_probare_approximationes_variae`: alter 0.5 → nearest semitone +
    MXC_MICROTONUS; additive `3+2/8` → `<metrum valor="5/8"/>` +
    MXC_METRUM_ADDITIVUM; non-traditional key → no `<clavis>` change +
    MXC_CLAVIS_NON_TRADITA; `percussio` with an unmapped instrument
    → MXC_PERCUSSIO_OMISSA; cue note → MXC_NOTA_PARVA_OMISSA; the
    song comment carries title, composer, lyricist, rights, source.
  - `_probare_verba_acorda_ligaturae` (Review Focus 3): chord with
    syllable on first note only; tied note carries no `_`; melisma
    without `<extend>` gets `_`; verse 2 → MXC_VERSUS_OMISSUS once per
    part.
  - `_probare_signa_non_approximationes`: `directiones` slurs,
    staccato, words → `signa` counts, `numerus_approximationum` 0,
    strict SUCCEEDS.
  - `_probare_strictus`: `gratiae` strict → FALSUM, relatio has
    exactly one MXC_GRATIA_OMISSA with its line:col, exitus empty;
    non-strict → VERUM, approximation listed.
  - `_probare_transpositio_carmen`: clarinet part's MIDI = sounding,
    spelling = sounding letter.
  - `_probare_repetitiones_evolutae` (M8): `repetitiones` → notes of
    measures 1–2 three times; structural note "repetitiones evolutae".
  - `_probare_relatio_scripta`: `musicxml_relationem_scribere`
    output for `gratiae` has one line per approximation `P1 m.2
    t.1 12:7 gratia omissa` and a count line per mark kind.
  - **`_probare_nata_lux` (acceptance, spec §IV):** strict conversion
    of `musica/fontes/nata_lux_80317.mxl` succeeds; read back with
    `carmina_legere`; the book's `nata_lux` read from `carmina.stml`
    (`carmina_ordinaria`); for each of the 5 parts BY INDEX, the flat
    note lists (absolute pulsus, longitudo, nota, littera, syllaba)
    are equal element by element.
- [ ] **Step 2: Implement `lib/musicxml_carmen.c`** (scribe.sh;
  regenerate the fontes list).
- [ ] **Step 3: Green; plants** — drop the `_` melisma rule →
  nata_lux red; strict ignoring grace → `_probare_strictus` red;
  restore.
- [ ] **Step 4: Commit** (gate `radix`).

---

## Task P11: `auditio -musicxml`, docs, closure

**Files:**
- Modify: `tools/auditio.c` (flag, book source from a chorda),
  `tools/auditio` help text in its head comment
- Create: `lib/musicxml.worklog.md` (if not already born), notes in
  `musica/` README or `docs/` per the inventory rules
- Modify: `project-specs/musicxml-spec.md` (§IX As built),
  this plan's Executed notes, ledger (parcum closed, desideratum
  clausum), memory `worktree-sexta.md`

**Interfaces:**
- Consumes: `musicxml_legere`, `musicxml_carmen_scribere`,
  `musicxml_relationem_scribere`.
- Produces: `auditio -musicxml VIA [-strictus] [-titulus T]` (M12).

- [ ] **Step 1: Measure** — read `tools/auditio.c:507-600` and
  `:1554-1600`: where `via_libri` becomes text; the smallest change
  that lets a chorda (the converted book) take its place.
- [ ] **Step 2: Implement** — alone: stdout `<carmina>\n` + carmen +
  `</carmina>\n`, report on stderr, exit 0 clean, 3 with
  approximations, 1 strict refusal or reader VITIUM (diagnostics on
  stderr), 2 usage. With other flags: the converted book replaces
  `-liber`'s text; `-carmen` defaults to the converted song's title.
- [ ] **Step 3: Verify by hand** (output pasted into the Executed
  note):
  `auditio -musicxml musica/fontes/nata_lux_80317.mxl -strictus`
  → exit 0, a `<carmina>` book; `… -censura` runs censura on it;
  `auditio -musicxml probationes/fixa/musicxml/gratiae.musicxml` →
  exit 3, one report line; `-strictus` on it → exit 1.
- [ ] **Step 4: Heal and gates** — `bin/fabrica sanare -siccum`, heal
  `auditio` by artifact path (never `institutio_*` from a worktree).
- [ ] **Step 5: Docs** — spec §IX As built (numbers: suite files,
  wild files, goldens, divergences, LCM max, timings); worklog; the
  ledger closed; memory note.
- [ ] **Step 6: Commit** (gates `radix`, plus whatever `fabrica`
  owes for `tools/auditio.c`).
