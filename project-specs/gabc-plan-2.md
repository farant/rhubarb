# gabc Plan, part 2 — neume glyphs, chant layout, the `antiphonale` pane

**Ledger:** parcum "gabc pars II" is born at P1, with one opus per task
below (part 1's parcum 01M6H2EKYSF4WGP4B2JMTR9W5K is closed, in main
since 6cc483d6).

**STATUS: DRAFT 2026-11-04.** Written after part 1 landed in main and
after a survey of vicus (three read-only surveys: panes and steering,
sound and cantor, drawing and file watching; findings folded in below).
Nothing executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention, no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note, as in part 1.

**Goal:** Fran opens a `.gabc` file in a vicus pane and sees square
notes on a four-line staff with the text underneath; when the file
changes (Claude editing it in the conversation) the pane redraws
within about a second; Fran can play it and see the sounding syllable
lit, click a neume to jump there, and hear an edit mid-phrase. Claude
can open a file in the pane and can see the same drawing as a PNG.

**Architecture.** Four layers, each testable without the one above:

1. **`neumae_ductae`** (pure): one glyph *piece* (note head, stem,
   porrectus stroke, clef, sign …) at a staff-space size → opacity
   mask + metrics. Pieces, not whole neumes (spec §IV lists pieces).
2. **`gabc_compositio`** (pure): `GabcCantus` + sizes + width → pages
   of positioned items (pieces, rectangles, text runs, the initial),
   each tagged with the syllable / element / note it came from, plus
   per-syllable hit boxes. Neumes are composed from pieces here, using
   `gabc_figura` (part 1, G4). Line breaking per G15. A canonical text
   dump makes it testable like the reader.
3. **`gabc_pictura`**: a composed page → `Mandata` (the house drawing
   command list). Masks become rows of 1-px rectangles pre-blended
   against the background colour, as `lib/terminale.c:438
   larvam_pingere` does for box glyphs, because Mandata has no alpha
   blending. Used by the pane and by a headless PNG tool.
4. **`antiphonale`** (vicus pane): file watching, reload, error banner,
   pages, keys and mouse, playback through a new carmen route in the
   vicus sound port, highlight, live swap.

**Survey findings that shape this plan** (each re-checked by hand):
- Panes draw through `Mandata` (rectangles, 1-px lines, aliased
  polygons, on/off-alpha images, unscaled fixed-width text) —
  `include/mandatum.h:139-191`. No alpha, no ellipse, no scaled text.
- Text is fixed-width bitmap only: `FONS_PARVUS` 6×8, `FONS_TITULI`
  8×16 (`include/fons.h:47`). A syllable's width is its rune count ×
  cell width. œ/Œ/ǽ render as TOFU; combining marks take their own
  cell. Pane text falls back to `glyphae_ductae` for runes it draws
  (`lib/fenestra_textus.c:59`), so adding œ/Œ/ǽ there fixes every pane
  and the terminal at once.
- cantor already has seek (`cantor_quaerere`) and position
  (`cantor_pulsus`, check `cantor_sonat` first — the header's "0 if
  not playing" is not what the code does). **No swap exists**;
  `cantor_incipere(c, novum, cantor_pulsus(c))` holds the pulse with a
  short audible cut (≈5 ms fade + device latency + 50 ms). The old
  carmen may be freed as soon as it returns (cantor keeps only the
  pointer; sent commands are copies).
- vicus sound (`include/vicus_sonus.h` `VicusPortaSoni`) has chimes,
  MML tunes and live voices, but no carmen and no position read-back;
  its `Concentus` is private to `lib/vicus_sonus_porta.c`.
  `vicus_applicatio_soni_portam_ponere` rejects a port with any NIHIL
  slot (`lib/vicus_applicatio.c:4298`). The aetherophonum pane is the
  model for a pane-specific port injected at mount.
- File watching: `include/specula.h` (kqueue, survives
  delete-and-rename saves), returned from `VicusFacies.fossa`; the
  admonitiones follower is the model (`lib/vicus_applicatio.c:3244-
  3339`). Read with `filum_legere_totum` (raw fopen is an examen
  violation).
- Steering from outside: the quaestio port is outbound only (spec G17's
  second option is out). The inbound channel is the praeconium log
  (`praeco cantus` already plays tunes in vicus); `vicus_acervo_mutare`
  reuses a pane and sets its argument (`include/vicus.h:628`).
- `-imago` writes the last frame of a `-fumus` run, which shows the
  default layout and a 240×480 pane — no way to aim it at a pane. The
  headless path (`probationes/probatio_vicus_paginae.c:232-250`:
  `tabula_pixelorum_creare_nuda` + rasterize + write PNG) is how Claude
  gets eyes on the chant (P4 tool).

**Tech stack:** C89 via latina.h, house flags, credo, piscina, chorda,
mandatum, thema, fons, glyphae_ductae, specula, filum, cantor,
concentus, insula/organum/dispensator (pane), praeconium (steering),
pythonica (`silva.commissio`, `silva.planta`).

**Spec:** `project-specs/gabc-spec.md` §IV-V and G11, G13-G18.
Precedents: `lib/glyphae_ductae.c` + `probationes/probatio_glyphae_
ductae.c` (masks, ASCII pictures, laws); `lib/chronica.c` +
`apps/chronica/organum.stml` + `probationes/probatio_chronica_latus.c`
+ `probationes/probatio_vicus_chronica.c` (a pane end to end; commit
9e992359b lists every file a pane touches); `include/aetherophonum.h`
(pane sound port); `probationes/probatio_cantor.c:23-152`
(`ExitusFictus`, offline audio); part 1's `project-specs/gabc-plan.md`.

## Global Constraints

- Everything in part 1's Global Constraints still holds (Latin
  identifiers and comments, forbidden latina.h identifiers, `i32`
  unsigned / `s32` signed, chorda not NUL-terminated, scribe.sh for new
  C files, the aedilis tag + `tools/compile_tests_fontes_generare.sh`
  for new `lib/` files, plants that COMPILE for every new test section,
  `silva.commissio` detached with the tree frozen, Latin lint with
  `VOCABULA_VIAE_ADDITAE` for untracked files).
- **Coordinates are logical pixels** (vicus `SCALA II`: one of ours is
  two screen points). `spatium` = staff-space height in our pixels:
  **even, VI..XLVIII; default VIII** (a 4-line staff is 24 px; a step
  is `spatium/2`). The pane's zoom keys move between allowed values.
- **Staff positions** come from `GabcNota.positio` (0 = a … 12 = m);
  on a 4-line staff the lines are d f h j (3, 5, 7, 9); `lineae` from
  `GabcCantus.lineae` (2..5) adds or drops lines from the top.
- **Pure libraries stay pure:** `neumae_ductae` allocates nothing and
  includes only latina.h; `gabc_compositio` includes gabc.h,
  neumae_ductae.h, piscina, chorda — no mandatum, no vicus. Only
  `gabc_pictura` knows Mandata; only `antiphonale` knows vicus.
- **Ink and highlight follow the theme**: ink `COLOR_TEXT`, staff lines
  `COLOR_TEXT_DIM`, highlight `COLOR_ACCENT_PRIMARY`, error banner
  `COLOR_ERROR`, background `COLOR_BACKGROUND` (`include/thema.h:58-
  83`); rubrics (`*`, ℟, ℣, the annotation) in palette red
  (`COLOR_MANDATI_INDEX`, `PALETTE_DARK_RED`).
- **A version is one unit** (G18): its own piscina holds the text, the
  `GabcCantus`, the composition, the carmen and the link table; swap
  replaces the whole unit; the old piscina is destroyed only after
  cantor holds the new carmen.
- **"Good version" = `gabc_successus`** (no VITIUM). Drawing needs
  only that; playing also needs `gabc_carmen_scribere` and
  `carmina_legere` to succeed. A drawable-but-silent version says why
  in the banner when Fran asks to play.
- **The terminal twin** (`apps/vicus/vicus_terminalis.c`) must not
  crash or hang with the pane open; legible chant in 6×8 cells is not
  a goal (Claude's call).
- **No sound in tests or by hand during development.** Song playback
  goes straight to CoreAudio (a PATH stub does not silence it). Tests
  use fake outputs / fake ports; live listening is Fran's, at P8.
- **Headers are reviewed by Fran before implementation**: P1 STOPs.
  P4 STOPs for Fran to look at the Salve picture. P8 STOPs for Fran
  to try the pane.

## Review Focus

Five conditions the spec implies but no feature names; each is pinned
by a test in the named task.

1. **A save that leaves the file broken or missing for a moment** —
   empty file, half-written gabc with a VITIUM, file deleted and
   re-created by the editor's rename: the pane keeps the last good
   drawing, shows the error (line:col, message) or "file missing" in
   the banner, and picks up the next good save without a keypress.
   (P6, `_servatio_probare`.)
2. **Drawable but not playable** — a chant with text and clefs but no
   notes, or one the converter refuses: it draws; play shows the
   converter's reason in the banner; no crash, no stale highlight.
   (P7, `_mutum_probare`.)
3. **Wider than the line** — a 40-note melisma on one syllable, a
   syllable whose text is wider than its notes, a single neume group
   wider than the whole line: the line breaks at a neume boundary with
   a custos; a group wider than the line overflows to the right
   (clipped by the pane) and layout always terminates. (P3,
   `_latitudo_probare`.)
4. **Notes off the staff and staff changes mid-line** — positions a
   and m (ledger lines above and below), a clef change mid-line, the
   b-flat clef `cb3`, an accidental sign, a custos before a clef change
   (it shows the next note under the NEW clef): ledger lines appear
   only where needed; custos height uses the new clef. (P3,
   `_extra_systema_probare`.)
5. **Relayout during playback** — zoom or pane resize while playing,
   and a swap that moves the current syllable to another page: the
   highlight stays on the syllable at the current pulse and the page
   follows it. (P7, `_dispositio_viva_probare`.)

---

## Task P1: Headers — `neumae_ductae.h`, `gabc_compositio.h` (STOP: Fran reviews)

**Files:**
- Create: `include/neumae_ductae.h`, `include/gabc_compositio.h`
  (types and prototypes only, head comments carrying the drawing and
  layout rules below)
- Modify: `oratio/glossarium.stml` only if `./oratio/quaere.sh`
  rejects a word (checked 2026-11-04: compositio, neuma(e), ductae,
  pars, spatium, caulis, custos, antiphonale, specula all pass)

**Interfaces (produced):**

```c
/* neumae_ductae.h */
nomen enumeratio {
    NEUMAE_PUNCTUM, NEUMAE_INCLINATUM, NEUMAE_INCLINATUM_DEMINUTUM,
    NEUMAE_VIRGA, NEUMAE_VIRGA_REVERSA,      /* caput + caulis */
    NEUMAE_STROPHA, NEUMAE_ORISCUS, NEUMAE_QUILISMA,
    NEUMAE_CAVUM, NEUMAE_LINEA_PUNCTUM,
    NEUMAE_DEMINUTUM,                        /* caput parvum (~) */
    NEUMAE_AUCTUM_ASCENDENS, NEUMAE_AUCTUM_DESCENDENS, /* < > */
    NEUMAE_PORRECTUS,      /* parametrum: intervallum gradibus I..V */
    NEUMAE_CAULIS,         /* linea verticalis; parametrum: gradus */
    NEUMAE_BEMOLLIS, NEUMAE_NATURALE, NEUMAE_DIESIS,
    NEUMAE_CLAVIS_C, NEUMAE_CLAVIS_F, NEUMAE_CUSTOS,
    NEUMAE_VIRGULA,
    NEUMAE_PUNCTUM_MORAE, NEUMAE_ICTUS,
    NEUMAE_EPISEMA         /* parametrum: latitudo in pixelis */
} NeumaePars;

nomen structura {
    i32 latitudo;          /* larvae */
    i32 altitudo;
    s32 origo_x;           /* pixel larvae in margine sinistro capitis */
    s32 origo_y;           /*   et in medio positionis notae */
    i32 capitis_latitudo;  /* caulis et lineae connectentes hic
                            * adhaerent (margo dexter capitis) */
} NeumaeMensura;

b32 neumae_ductae_metiri (NeumaePars pars, i32 spatium,
                          s32 parametrum, NeumaeMensura* mensura);
b32 neumae_ductae_pingere (NeumaePars pars, i32 spatium,
                           s32 parametrum, i32 latitudo,
                           i32 altitudo, i8* larva);
/* FALSUM: pars ignota, spatium extra VI..XLVIII aut impar,
 * parametrum extra ambitum, latitudo/altitudo non ut metiri -
 * larva tunc intacta */
```

```c
/* gabc_compositio.h */
nomen enumeratio {
    GABC_ITEM_PARS,        /* neumae_ductae: pars + parametrum */
    GABC_ITEM_RECTANGULUM, /* linea systematis, linea addita,
                            * divisio, linea connectens */
    GABC_ITEM_TEXTUS,      /* cursus textus, fons FONS_* */
    GABC_ITEM_INITIALIS    /* littera initialis magna */
} GabcItemGenus;

nomen structura {
    GabcItemGenus genus;
    s32 x, y;                      /* PARS: punctum originis; alia:
                                    * angulus sinister superior */
    NeumaePars pars; s32 parametrum;
    i32 latitudo, altitudo;        /* RECTANGULUM, INITIALIS */
    chorda textus; i32 fons;       /* TEXTUS, INITIALIS (runa una) */
    b32 rubrum;                    /* rubrica */
    s32 syllaba, elementum, nota;  /* -1 = nulla */
} GabcItem;

nomen structura {
    s32 pagina; s32 x, y; i32 latitudo, altitudo;  /* -1 si sine loco */
} GabcLocus;

nomen structura {
    i32 latitudo;          /* paginae, pixelis */
    i32 altitudo;          /* paginae */
    i32 spatium;
    i32 fons_textus;       /* FONS_PARVUS */
    i32 fons_tituli;       /* FONS_TITULI */
} GabcCompositionisForma;

nomen structura {
    GabcItem*  itemae;     /* omnes, ordine paginarum */
    i32        numerus_itemarum;
    i32*       paginae;    /* index itemae primae per paginam */
    i32        numerus_paginarum;
    GabcLocus* syllabae;   /* per syllabam cantus: capsa notarum et
                            * textus (sonans illuminatur, ictus
                            * quaerit) */
    GabcLocus* notae;      /* per notam cantus */
} GabcCompositio;

vacuum gabc_compositionis_formam_ordinariam (
    GabcCompositionisForma* forma);   /* spatium VIII, CMLX x DLXXIII */
GabcCompositio* gabc_componere (constans GabcCantus* cantus,
    constans GabcCompositionisForma* forma, Piscina* piscina);
    /* NIHIL solum si memoria deficit; cantus cum VITIIS componitur
     * quantum potest */
s32 gabc_compositio_syllaba_ad_punctum (
    constans GabcCompositio* c, i32 pagina, s32 x, s32 y); /* -1 */
b32 gabc_compositionem_scribere (constans GabcCompositio* c,
    ChordaAedificator* exitus);  /* forma canonica, probationes */
```

The executor may rename fields while drafting; every later task uses
whatever P1's approved headers say.

- [ ] **Step 1: Measure.** Read `include/glyphae_ductae.h`,
  `lib/terminale.c:400-470` (mask → rectangles) and `include/gabc.h`
  (positio, formae, figurae, elementa). Look up Gregorio's square-
  notation drawing of pes, clivis, torculus, porrectus, climacus,
  scandicus, salicus, quilisma, oriscus and liquescents (Gregorio
  documentation, GregorioTeX glyph tables — reading web pages, no
  installs) and write down for each: which pieces, which side the
  stems and connecting lines sit, the horizontal offsets. This table
  goes into the `gabc_compositio.h` head comment as COMPOSITIO NEUMARUM.
- [ ] **Step 2: Draft both headers** with the interfaces above and
  head comments: DRAWING RULES (head = one spatium square, line
  thickness `max(I, spatium/VIII)`, virga stem 1.5 spatia below on
  the right, inclinatum = diamond one spatium wide, porrectus =
  oblique band from the first note's position down by `parametrum`
  steps over 2 spatia, connecting lines on the right edge of the lower
  note), LAYOUT RULES (systems: staff, then text baseline
  `spatium*III/II` below the lowest line or below the lowest note,
  whichever is lower; syllable advance = max(neume width, text width)
  + gap; vowel centring: the `{}` vowel, else the first vowel, else
  the middle of the syllable, sits under the first note's head; the
  initial takes the first syllable's first letter, two systems tall,
  in rubric red if `initial-style` asks; title and annotation above
  the first system; custos at every line end before a note; z/Z
  honoured, else greedy fill; pages hold whole systems).
- [ ] **Step 3: `./silva/examen.sh` and `./silva/formator.sh -vitia`**
  on both headers; Latin lint on both.
- [ ] **Step 4: STOP — show Fran both headers.** No implementation
  before approval. Ledger: parcum "gabc pars II" + opera P1-P8.

---

## Task P2: `neumae_ductae` — pieces and metrics

**Files:**
- Create: `lib/neumae_ductae.c`, `lib/neumae_ductae.worklog.md`,
  `probationes/probatio_neumae_ductae.c`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:** Produces P1's `neumae_ductae_metiri` /
`neumae_ductae_pingere`.

- [ ] **Step 1: Measure.** Copy the test scaffolding of
  `probatio_glyphae_ductae.c:24-95` (legend `.` 0, `#` FF, `1 2 3`
  40/80/C0; guard byte 0xA5; side-by-side diff on failure).
- [ ] **Step 2: Write the failing tests.**
  - `_picturas_probare`: ASCII pictures at spatium VIII for
    punctum, inclinatum, virga, stropha, oriscus, quilisma, cavum,
    deminutum, auctum ascendens, porrectus I and III, caulis III,
    bemollis, clavis C, clavis F, custos, virgula, punctum morae,
    ictus, episema 12 — and at spatium XII for punctum, virga,
    porrectus II, clavis C (the size law shows).
  - `_mensuras_probare`: for every pars at VI, VIII, XII, XXIV:
    `metiri` succeeds; `origo` lies inside the mask; for head pieces
    `origo_y` row is inked at `origo_x`; `capitis_latitudo` ≤
    `latitudo`; punctum at VIII is 8×8 with origo (0, 4).
  - `_leges_probare`: nothing written past `latitudo*altitudo` (guard
    bytes) for every pars at every allowed even spatium; every pars
    inks at least one pixel; a caulis of n steps is exactly
    `n*spatium/2` tall; head pieces at the same spatium share
    `altitudo` of the head (stacking law).
  - `_ambitum_probare`: odd spatium, IV, L, unknown pars, porrectus 0
    and VI, wrong latitudo → FALSUM and the mask untouched.
- [ ] **Step 3: Run, see red:** `./compile_tests.sh neumae_ductae` —
  does not link.
- [ ] **Step 4: Implement** in the shape of `glyphae_ductae.c`'s
  `Pictor` (`quadrum` for rectangles, curve sampling into a via for
  the porrectus band and the quilisma's teeth). Partial opacity only
  on slanted edges (porrectus, inclinatum, clef curves).
- [ ] **Step 5: Green**, then **plants** (each compiling, each turning
  a section red): porrectus band one step short; virga stem on the
  left; one guard byte overwritten at spatium XLVIII; caulis one pixel
  short.
- [ ] **Step 6: Worklog + commit** (gate `radix`).

**Executed 2026-11-04.** Follows the APPROVED P1 header, not the sketch
above: no caulis piece (stems are compositio rectangles; the stem law
moved to the virga, parametrum II..VI), BEMOLLE, custos ±I, plus LINEA
and r1-r5 signs. Head = spatium wide, spatium-1 tall (punctum VIII is
8x7, origo (0,3)) so heads centre on a row — Ruling in the ledger and
worklog. Pictures: 20 hand-derived (rectangles, at VIII and XII), 14
pinned by eye (curves, slants; oriscus deepened after a look at XXIV).
Laws at every even spatium VI..XLVIII: guard bytes, ink, origin row
inked, virga stem n*s/2, shared head height, mask ≤ NEUMAE_LARVA_MAXIMA.
Plants: four, all red then green (porrectus short, virga stem left,
guard byte at XLVIII, stem one pixel short). Section names: I quadrata,
II curvae, III mensurae, IV leges, V ambitus.

---

## Task P3: `gabc_compositio` — neumes from pieces, syllables, lines, pages

**Files:**
- Create: `lib/gabc_compositio.c`, `lib/gabc_compositio.worklog.md`,
  `probationes/probatio_gabc_compositio.c`,
  `probationes/fixa/gabc/compositio/*.gabc` (small fixtures) and
  `probationes/fixa/gabc/compositio/salve.compositio` (pinned dump)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: `gabc_legere`, `gabc_figura` (gabc.h), P2's metrics.
- Produces: P1's `gabc_componere`, `gabc_compositio_syllaba_ad_
  punctum`, `gabc_compositionem_scribere`.

**Dump grammar** (one item per line, tab-separated, pinned in the test
file's head comment): `pagina  genus  x  y  detail  syllaba  elementum
nota`, where detail is the pars title + parametrum, `w×h`, or the text
in quotes. Then one line per syllable locus.

- [ ] **Step 1: Measure.** Compose the Salve by hand on paper from
  P1's rules for its first system (clef, first three syllables) — the
  expected x/y of each item is the first test.
- [ ] **Step 2: Write the failing tests.**
  - `_neumae_probare`: one fixture per figura family — punctum, virga,
    pes, clivis, torculus, porrectus, climacus, scandicus, salicus,
    quilisma group, pressus, liquescent pes and clivis — each dumped
    and compared to the hand-written expected dump (pieces, offsets,
    connecting rectangles on the side the P1 table says).
  - `_signa_probare`: mora dot right of its head (in the space above
    when the head sits on a line; `..` two dots); episema above the
    head (`_0` below), spanning consecutive episemata as one bar; ictus
    below (`'1` above); accidental sign left of its note; bars —
    virgula glyph at the top line, minima a short stroke through the
    top line, minor the middle two spaces, maior the whole staff,
    finalis two strokes; `<alt>` text above the staff; translations
    `[...]` not drawn in v1 (kept in the model; worklog note).
  - `_syllabae_probare`: vowel centring (`{}` vowel, first vowel, no
    vowel), word gaps vs syllable gaps, a syllable with text but empty
    notes, notes without text, elision text, a bar with no text.
  - `_lineae_probare`: z forces a break; Z forces a break without
    justification; greedy fill to `latitudo`; custos at each line end
    before a note shows that note's position; title/annotation only on
    page 1; pages hold whole systems and `paginae[]` indexes them.
  - `_latitudo_probare` (Review Focus 3).
  - `_extra_systema_probare` (Review Focus 4).
  - `_ictus_probare`: `gabc_compositio_syllaba_ad_punctum` hits each
    syllable's box, returns -1 between systems and on another page.
  - `_salve_probare`: the dump of `musica/fontes/salve_2435.gabc` at
    the default form equals `salve.compositio`; every note of the
    cantus is placed exactly once (`notae[i].pagina >= 0`).
  - `_corpus_probare`: every file in `probationes/fixa/gabc/gregobase/`
    composes at spatium VI, VIII and XXIV without NIHIL; no item lies
    left of 0 or above 0; every non-overflow item lies inside
    `latitudo`; counts of pages and items pinned as totals.
- [ ] **Step 3: Red** (`./compile_tests.sh gabc_compositio`).
- [ ] **Step 4: Implement:** walk syllables; per syllable, place
  elements left to right (neume groups composed from the P1 table via
  `gabc_figura`; IGNOTA figures fall back to one piece per note, left
  to right, connecting lines where adjacent notes stack — never
  dropped); text below; advance; break per G15; then cut systems into
  pages. Positions to y: `y = linea_ima - (positio - III) * spatium/2`
  for the 4-line staff, shifted for `lineae`.
- [ ] **Step 5: Green, plants:** custos uses the old clef; vowel centred
  on the last note instead of the first; greedy fill one item too
  many; connecting line on the left in a pes.
- [ ] **Step 6: Worklog + commit** (gate `radix`).

**Executed 2026-11-04.** Built as frusta (chunks between breakable
spaces / fractio) placed with two cursors (notes, text) into a system,
finished with custos, justification, hyphens, text row, staff lines,
page. Rulings (ledger): greedy longest-known-figure segmentation instead
of the header's side-by-side fallback; two cursors instead of one; bars
without their own s*3/4 margins. Tests use inline gabc and relative
neume dumps rather than fixture files; dump grammar gained a tinctura
column. Two hand expectations were wrong and corrected (pressus line at
the boundary column 15; Salve text row from the lowest item of the
WHOLE line, clivis stem in rí(dc)). Salve dump pinned (546 lines);
corpus 82 x {VI, VIII, XXIV}: 473 pages, 109925 picta, 0 out of bounds.
Plants: four, all red then green; the vowel plant only trips the Salve
dump.

---

## Task P4: `gabc_pictura`, œ/Œ/ǽ, and Claude's eyes (STOP: Fran looks at the Salve)

**Files:**
- Create: `include/gabc_pictura.h`, `lib/gabc_pictura.c`,
  `probationes/probatio_gabc_pictura.c`, `tools/neumae.c` (+ its
  aedificatio entry; `bin/neumae`)
- Modify: `lib/glyphae_ductae.c` + its test (œ U+0153, Œ U+0152, ǽ
  U+01FD as v2-style 6×8 pictures), `lib/terminale.c` (if the mask →
  rectangles helper is extracted rather than copied — see Step 1),
  `aedificatio.stml` (`<debitum_scripturae>` for the test PNG)

**Interfaces:**
- Consumes: P3's `GabcCompositio`; `mandata_*` (mandatum.h);
  `thema_color`, `figura_color_thematis`.
- Produces:

```c
nomen structura {
    s32 syllaba_illuminata;   /* -1 nulla */
    s32 nota_illuminata;      /* -1 nulla */
} GabcPicturaStatus;

b32 gabc_paginam_pingere (constans GabcCompositio* c, i32 pagina,
    s32 x, s32 y, constans GabcPicturaStatus* status, Mandata* md);
```

- [ ] **Step 1: Measure.** Decide extract vs copy for the mask →
  1-px-rectangle-runs helper (`lib/terminale.c:438 larvam_pingere`,
  with private `rgb_miscere`/`color_rgb`): extracting a public
  `mandata_larvam(md, x, y, latitudo, altitudo, larva, color, fundus)`
  into mandatum is preferred if terminale's tests stay green
  unchanged; otherwise copy and note the debt. Count the rectangles
  the Salve produces (expect a few thousand) and time
  `delineare_mandata` on them; if over ~4 ms, cache each piece's
  rectangle runs per spatium.
- [ ] **Step 2: Write the failing tests.**
  - `_mandata_probare`: one punctum on a staff → the expected
    rectangles (four staff lines in `COLOR_TEXT_DIM`, the head's 8 rows
    in `COLOR_TEXT`); the highlighted syllable's text and notes in
    `COLOR_ACCENT_PRIMARY`; rubrics in palette red.
  - `_initialis_probare`: the initial is drawn as rectangles from the
    8×16 bitmap scaled to two systems' height (no TEXTUS command).
  - `_textus_probare`: syllable text with combining marks is drawn
    without them (U+0300-036F stripped before measuring and drawing);
    œ draws as a `glyphae_ductae` picture, not TOFU.
  - `_imago_probare`: headless render of the Salve page 1 into a
    `tabula_pixelorum_creare_nuda` and write
    `build/gabc/salve_pagina_1.png` (declared in aedificatio.stml);
    assert ink pixels exist inside each staff line's rows and nowhere
    outside the page.
- [ ] **Step 3: Red; implement; green; plants** (highlight ignored;
  initial at one system; combining mark not stripped).
- [ ] **Step 4: `tools/neumae.c`**: `neumae VIA.gabc -imago VIA.png
  [-latitudo N] [-spatium S] [-pagina P]` — reads, composes, renders
  one page headless at 1× logical pixels with title and diagnostics
  banner as the pane would; exit 1 with the diagnostics on stderr when
  the file has a VITIUM (it still draws what it can). Fumus in its
  head comment, gate declared like `auditio-fumus` (registered in
  `pythonica/silva.py` PORTAE — a `<porta>` comment alone does not
  run).
- [ ] **Step 5: Look.** Render the Salve and three corpus chants
  (one with a porrectus, one with quilismas, one with an F clef), Read
  the PNGs, fix what looks wrong, note each fix in the worklog.
- [ ] **Step 6: STOP — show Fran the Salve PNG** (and the three
  others). Taste is Fran's: proportions, stem lengths, the porrectus,
  the initial. Adjust, then commit (gates `radix`, `neumae-fumus`).

---

## Task P5: Sound — carmen in the vicus sound port; the swap

**Files:**
- Modify: `include/vicus_sonus.h`, `lib/vicus_sonus_porta.c`,
  `lib/vicus_applicatio.c` (validation), `probationes/probatio_
  cantor.c`, `probationes/probatio_vicus_sonus.c`

**Interfaces (produced):** four new **optional** slots in
`VicusPortaSoni` (NIHIL allowed; validation keeps rejecting NIHIL
only in the old slots, so the existing fakes at
`probatio_vicus_sonus.c:347,719` and `probatio_vicus_quies.c:1213`
need no change):

```c
b32    (*carmen_incipere)(vacuum* ctx, constans Carmen* carmen,
                          s64 pulsus);   /* novum carmen ad pulsum;
                                          * etiam permutatio */
vacuum (*carmen_quaerere)(vacuum* ctx, s64 pulsus);
vacuum (*carmen_sistere)(vacuum* ctx);
s64    (*carmen_pulsus)(vacuum* ctx);    /* -1 si non sonat */
```

- [ ] **Step 1: Measure.** Read `lib/vicus_sonus_porta.c` whole and
  `lib/cantor.c:640-820`. Confirm `cantor_incipere(c, novum, P)`
  holds P (cantor.c:810-813) and that nothing in Cantor points into
  the old carmen after it returns.
- [ ] **Step 2: Write the failing tests** on `ExitusFictus`:
  - `probatio_cantor` `_permutatio_probare`: play A, run 1 s, swap to
    B (same notes, one pitch changed) with `cantor_incipere(c, B,
    cantor_pulsus(c))`, destroy A's piscina, run 0.5 s: position ≥ the
    swap pulse and < swap pulse + 1 quarter; the changed pitch is
    heard (command log); no read of A's memory (ASan via
    `./tools/sonda.sh`).
  - `probatio_vicus_sonus` `_carmen_probare`: a porta over a fake
    exitus: incipere → `carmen_pulsus` advances with the fake clock;
    quaerere jumps; sistere → -1; `tacere` (mute) also stops the
    carmen; `porta_agere` drives `cantor_agere`.
- [ ] **Step 3: Red; implement** (Cantor created lazily on the porta's
  concentus at the first carmen_incipere; `porta_agere` calls
  `cantor_agere` after `concentus_agere`; `carmen_pulsus` returns -1
  unless `cantor_sonat`); **green; plants** (agere not calling cantor;
  pulsus without the sonat check).
- [ ] **Step 4: Commit** (gates `radix` + whatever commissio owes for
  vicus files; `ludus_tessera` links vicus_applicatio).

---

## Task P6: The pane — open, watch, draw, pages, banner (no sound yet)

**Files:**
- Create: `include/antiphonale.h`, `lib/antiphonale.c`,
  `lib/antiphonale.worklog.md`, `apps/antiphonale/organum.stml`,
  `probationes/probatio_antiphonale_latus.c`,
  `probationes/probatio_vicus_antiphonale.c`
- Modify: `lib/vicus_applicatio.c` (genus, mount/describe, `$antiphonale`
  command), `lib/organa_infixa.c` (regenerated by
  `tools/organa_infigere.sh`), `compile_tests_fontes_generata.sh`
  (regenerated), `ludus_tessera/compile_probationes.sh` (add
  `antiphonale`)

**Interfaces:**
- Consumes: P1-P4; `specula_*`, `filum_legere_totum`.
- Produces:

```c
nomen structura {
    b32  (*incipere)(vacuum* ctx, constans Carmen* carmen, s64 pulsus);
    vacuum (*quaerere)(vacuum* ctx, s64 pulsus);
    vacuum (*sistere)(vacuum* ctx);
    s64  (*pulsus)(vacuum* ctx);
    vacuum* ctx;
} AntiphonalePorta;            /* P7 uses it; NIHIL porta = mutum */

/* AntiphonaleMontatio: chronica's shape (ChronicaMontatio) - ramus,
 * actiones, figurae, figurae_ctx (version unit, specula, banner),
 * the porta copy */

b32 antiphonale_montare (AntiphonaleMontatio* m, Piscina* piscina,
    InternamentumChorda* intern, InsulaRepositorium* repo,
    constans character* id, constans character* via,
    i32 latitudo, i32 altitudo, constans AntiphonalePorta* porta);
Componens* antiphonale_componere (InsulaRepositorium* repo,
    constans Motus* motus, Piscina* piscina,
    InternamentumChorda* intern, vacuum* ctx);
b32 antiphonale_argumentum_ponere (AntiphonaleMontatio* m,
    constans character* via);       /* aliud plagulam aperit */
VicusPulsus antiphonale_pulsare (vacuum* ctx);
s64 antiphonale_mora (vacuum* ctx); /* -1 otiosum; XXXIII dum sonat */
s32 antiphonale_fossa (vacuum* ctx, i32* eventus);
```

- [ ] **Step 1: Measure.** Read `lib/chronica.c` mount/compose/figura/
  agere (`:2318`, `:~1940`) and the admonitiones follower
  (`lib/vicus_applicatio.c:3244-3339`); list the exact files commit
  9e992359b touched and confirm the list above.
- [ ] **Step 2: Write the failing tests** (`probatio_antiphonale_
  latus`, dispensator harness like `probatio_chronica_latus.c`):
  - `_aperire_probare`: mount on a fixture file → page 1's Mandata
    contain the title text and staff rectangles; the pane title is the
    gabc `name` (else the file name).
  - `_servatio_probare` (Review Focus 1): rewrite the file with a
    VITIUM → `pulsare` reports mutatum, the drawing is unchanged, the
    banner text holds `linea:columna` and the message; empty file → same;
    delete + recreate good → new drawing, banner gone.
  - `_paginae_probare`: PageDown / PageUp and clicks on the page
    arrows change the page; the page line shows `n / N`; zoom keys
    (`+` `-`) relayout and keep the first visible syllable on screen.
  - `probatio_vicus_antiphonale` `_iussum_probare`:
    `$antiphonale(path)` in a scriba page opens the pane on that
    file; a second call with another path reuses the same pane
    (`vicus_acervo_mutare`).
- [ ] **Step 3: Red; implement.** View state (page, spatium) in the
  ramus attributes declared in `organum.stml`; the version unit and
  specula in `figurae_ctx`; on change: read → `gabc_legere` →
  `gabc_successus` ? new unit : banner. **Green; plants** (banner
  cleared on a bad save; old unit freed before the new is built).
- [ ] **Step 4: Commit** (gates `radix`, `ludus_tessera`, the
  organa / generated-file gates commissio names).

---

## Task P7: Playback in the pane — play, seek, highlight, live swap

**Files:**
- Modify: `lib/antiphonale.c`, `lib/vicus_applicatio.c`
  (`AntiphonalePorta` over `VicusPortaSoni`'s carmen slots, mute
  respected, aetherophonum pattern), the two P6 test files

**Interfaces:** Consumes P5's slots and P6's `AntiphonalePorta`; the
part 1 link table `GabcCarmenRelatio.nexus` (rows sorted by
`initium`).

- [ ] **Step 1: Measure.** Confirm the link table is sorted and every
  row's `syllaba` indexes `GabcCompositio.syllabae`; decide the lookup
  (binary search on `initium`).
- [ ] **Step 2: Write the failing tests** with a fake
  `AntiphonalePorta` whose clock the test sets:
  - `_sonare_probare`: Space starts at pulse 0 (or at the selected
    syllable); Space again stops; the play button in the header does
    the same by mouse.
  - `_illuminatio_probare`: fake pulse at each nexus row's `initium`
    → `GabcPicturaStatus.syllaba_illuminata` = that row's syllable;
    between rows (a breath) the previous syllable stays lit; after the
    end, nothing is lit.
  - `_quaerere_probare`: a click inside a syllable's box seeks to the
    first nexus row of that syllable (playing) or selects it (stopped);
    ← / → move to the previous / next syllable.
  - `_permutatio_probare` (G18): playing at pulse P, the file changes
    to a version with one pitch different → `incipere(novum, P)` is
    called once with P; the old unit's piscina is destroyed after it;
    a version shorter than P → `sistere`; a bad save → keeps playing
    the old one.
  - `_mutum_probare` (Review Focus 2).
  - `_dispositio_viva_probare` (Review Focus 5); also: while playing,
    the page turns to the lit syllable's page.
- [ ] **Step 3: Red; implement** (`mora` → XXXIII while the porta
  says playing, -1 otherwise; highlight leads the sound by the device
  latency — accepted v1, worklog note); **green; plants** (swap passes
  pulse 0; page does not follow).
- [ ] **Step 4: Commit.**

---

## Task P8: Steering, the first composition, Fran tries it (STOP)

**Files:**
- Modify: `tools/praeco.c`, `include/praeconium.h` (+ test), the
  admonitiones system in `lib/vicus_applicatio.c` (handle the new
  event), `probationes/probatio_vicus_antiphonale.c`
- Create: `musica/compositiones/` with one short composition
  (`kyrie_xvi.gabc` or Fran's pick — CC0 / public domain melody)
- Modify: `project-specs/gabc-spec.md` (G17's mechanism recorded),
  worklogs, this plan's Executed notes

- [ ] **Step 1: Measure.** Read how `praeco cantus` is written and
  consumed (`include/praeconium_cantus.h`; `cantum_recentissimum`).
- [ ] **Step 2: Failing tests:** `praeco antiphonale <via>` writes an
  event with an absolute path (relative paths resolved against the
  caller's cwd; a missing file refused with exit 1); vicus, on the
  newest such event, opens or reuses the antiphonale pane on it
  (`vicus_acervo_mutare`) and brings it to the front; an event older
  than vicus's start is ignored.
- [ ] **Step 3: Red; implement; green; plants; commit.**
- [ ] **Step 4: STOP — Fran tries it live in vicus:** Claude runs
  `praeco antiphonale musica/compositiones/<x>.gabc`, edits the file
  in the conversation; Fran watches it redraw, plays, clicks, edits
  mid-phrase. Fixes from that session are their own commits. Then the
  merge ritual when Fran asks.

---

## Executed notes

*(one per task, as in part 1)*
