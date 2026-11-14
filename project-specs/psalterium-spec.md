# psalterium — a psalter pane in vicus: index of the 150 psalms, each sung on a chosen tone

*2026-11-14. Born in sexta the day after the psalter import (psalms
I-CL from Divinum Officium in `musica/psalmi/N.txt`, decree
01M7BHHT1PHJTTDVQX1X5V70Y9). Fran asked: "is there a $ command to view
a particular psalm? what do you think of some kind of paginated browser
interface?" There is none: the only path today is `auditio -psalmus
musica/psalmi/109.txt -signum "8 G" > x.gabc` and then
`$antiphonale(x.gabc)`. Fran approved the two-view shape below on
2026-11-14. Each decision says whose call it was.*

**What it is.** A vicus pane, genus `psalterium`, opened with
`$psalterium`, with two views:

1. **Index**: the 150 psalms as a paged list, one row each: the
   Vulgate number and the opening words ("109  Dixit Dóminus Dómino
   meo"). Pick one to open it.
2. **Psalm**: the chosen psalm fitted to a psalm tone by the existing
   fitter (`psalmodia_aptare`, `include/psalmodia.h`) and drawn as
   square notation, paged, with play-along. It looks and behaves like
   the antiphonale pane does today (`include/antiphonale.h`), and adds
   a key to change the tone and keys to step to the neighbouring psalm.

**Why now.** The psalter import gave the house 150 pointed texts and a
fitter that sets them on any of the 33 tone endings with no warnings
(`_psalterium_probare`, `probationes/probatio_psalmodia.c` - fitted
without Gloria Patri, `sine_gloria` VERUM at :1563; with the Gloria,
as the pane sings it, is untested until this plan's tests). Nothing
shows them except the command line. The pane is also the place the
1962 calendar will later plug into ("open today's Vespers with the
tones set"). Today the person singing picks the tone.

## 0. Decisions

| # | Decision | Whose |
|---|----------|-------|
| D1 | Two views in one pane: index and psalm. | Fran, 2026-11-14 |
| D2 | `$psalterium` opens the index. `$psalterium(109)` opens Ps 109. `$psalterium(109, 8 G)` opens it on 8 G. Arguments are split on commas (`lib/iussum.c`, as every `$` command). | proposed by Claude, approved with D1 |
| D3 | Default tone is 8 G. The person changes it in the psalm view. | Claude (stated to Fran with D1) |
| D4 | The chant drawing, paging, zoom, syllable selection and play-along now inside `lib/antiphonale.c` move into a shared module that takes **gabc text in memory**. The antiphonale becomes a thin file-watching shell around it, and the psalter a second shell. No temporary files. | Claude (stated to Fran with D1, as "the API question") |
| D5 | The psalm texts are **embedded in the binary**, generated from `musica/psalmi/` the same way the tone table is (`tools/toni_psalmorum_infigere.sh` → `lib/toni_psalmorum_infixum.c`, generator action in `aedificatio.stml`). vicus is installed in `~/.bin` and must not read repo paths at runtime. Size: 262,822 bytes of text (`wc -c musica/psalmi/*.txt`, 2026-11-14). | Claude |
| D6 | The chosen tone is **one setting for the pane**. It carries over when you move to another psalm. This replaces "the pane remembers your last choice per psalm" from the chat proposal. Per-psalm memory is state the calendar will make obsolete (the antiphon of the day decides the tone), and one setting covers the common case of singing several psalms on one tone. | Claude; **Fran agreed 2026-11-14** ("one tone for the whole pane sounds good") |
| D7 | Gloria Patri is sung at the end of every psalm (the fitter's default, `sine_gloria` FALSUM), as in the Office. | Claude |
| D8 | The tone cycle is every termination in the embedded tone table, in table order, **skipping** the `ad_libitum` and `monosyllaba` variants. The fitter chooses those variants itself; this is the same set `_psalterium_probare` fits, 33 today. Peregrinus and in directum are in the cycle because the table lists them. | Claude |

## I. What exists (2026-11-14)

- **Fitter**: `psalmodia_aptare(tonus, terminatio, textus, &optiones,
  piscina)` → `PsalmodiaPsalmus*`, then `psalmodia_gabc_scribere` →
  gabc text with header `name:`, `mode:`, `annotation:`,
  `initial-style: 0;` (`include/psalmodia.h`, "EXITUS GABC"). The
  fitter never refuses a text: problems come back as warnings
  (`psalmodia_admonitiones_scribere`, "via:linea:col: causa").
- **Tone table**: `toni_psalmorum_infixum_textus(piscina)` →
  `psalmodia_tabulam_legere` → `PsalmodiaTabula` with
  `toni[i].terminationes[j]` (`include/psalmodia_tabula.h`).
  `psalmodia_signum_invenire(tabula, "8 G", &tonus, causa)` finds an
  ending by its label, and on failure writes a reason that lists the
  tone's endings.
- **Psalm files**: `musica/psalmi/1.txt`–`150.txt`, five `%` header
  lines and then one verse per line (pointed text format,
  `include/psalmodia.h`). Ps 118 has `% Aleph`-style stanza lines.
- **Antiphonale pane** (`lib/antiphonale.c`, 1381 lines;
  test `probationes/probatio_antiphonale_latus.c`, 1147 lines). A
  *version* (`Versio`) is one unit: text, parsed chant, carmen for
  sound, the carmen↔syllable link table, and a lazily built layout.
  A new good version replaces the old one only after it is fully
  built. If playback is running, it carries over at the same pulse
  into the new version (G18). The pane draws the page inside a
  margin and a footer line "< ▶ title  2/5 >". It also draws an
  error banner across the top. Keys:
  Space play/stop, Left/Right syllable, PageUp/PageDown and j/k page,
  `+`/`=`/`-` zoom. Mouse: footer arrows, play button, click a
  syllable. Sound goes through `AntiphonalePorta`, which the host sets
  (one singer per vicus: `PortaAntiphonae` in
  `lib/vicus_applicatio.c`).
- **Two-view model to copy**: lector (`$epubs`, `lib/lector.c`,
  `apps/lector/organum.stml`) has a library list and a page view, a
  `visus` field bound to the route (`semita="ligatio"`), per-view key
  tables (`Praeceptum` with a view number), Esc to go back a level,
  `album_*` (`include/membrum__album.h`) for paged lists with
  clickable rows, and `<cognomina>` for routes.
- **Registration**: `vicus_genus_addere` (genus, montatio size, mount,
  describe) and `iussum_registrare` for the `$` command, both in
  `lib/vicus_applicatio.c` (antiphonale at :6526 and :6596).

## II. The shared chant view (D4)

What moves out of `lib/antiphonale.c` into its own module (name left
to the plan):

- holding a good version and swapping in a new one, with playback
  carried over (G18) and the selection kept by index;
- layout at the pane's width, zoom, and the page that follows the
  sounding syllable;
- drawing: page, footer, banner;
- the key table for play, syllable, page and zoom;
- clicks on a syllable, the footer arrows and the play button.

Its input is **gabc text plus a title**, given by the shell: "here is
the new text". Whether the text came from a file or from the fitter is
not its business. A text that fails to parse keeps the last good
version and shows the banner, exactly as the antiphonale does now.

The antiphonale keeps only what is about files: the path, the file
watcher, reading the file, and the "missing:" and "empty file"
banners.

**Guard:** `probatio_antiphonale_latus` passes **unchanged**. It
embeds `AntiphonaleMontatio` from the public header, so that struct and
the antiphonale's node ids, action names and command titles stay as
they are (the shared module takes them as parameters). If a test
there has to change, the extraction changed behaviour, and that is a
finding to raise, not a test to edit.

## III. The psalter pane

### III.1 Index view

- One row per psalm: number, then the opening words up to the first
  `*` or `†`, with trailing punctuation removed. (Claude's call: this
  is shorter than the file headers, which `tools/psalmi_arcessere.sh:98`
  cuts only at `*`. The two differ on four psalms, Ps 1 among them;
  row 109 is the same either way.) Computed from the embedded text at
  mount, not by parsing the `%` header.
- Paged (house rule: pagination, never scrolling). The page indicator
  goes in the footer, in the antiphonale's style.
- Keys: Up/Down and j/k move the selection, PageUp/PageDown change
  page, Enter opens the selected psalm, Esc closes the pane (the
  vicus default, `effugium_redit`, as the antiphonale). Typing digits selects that
  psalm number (for example "1 0 9" selects 109). How the typed
  number is shown and cleared is left to the plan.
- Mouse: click a row to open it, and the footer arrows change page.

### III.2 Psalm view

- Title: "Psalmus 109" (passed to the fitter as `titulus`, so the
  gabc `name:` carries it). The footer shows the title, the tone
  label ("8 G") and the page.
- Keys: everything the chant view has (II), plus:
  - `t` / `T`: next / previous tone in the D8 cycle; the cycle wraps
    around.
  - `]` / `[`: next / previous psalm. Nothing happens past 150 or
    before 1.
  - Esc: back to the index, with the current psalm selected.
- Mouse: clicking the tone label advances the tone. There must be
  mouse paths for "previous psalm", "next psalm" and "back to index"
  (house rule: every key action has a mouse path). The layout is left
  to the plan.
- Changing the tone or the psalm re-fits and hands new text to the
  chant view. Page 0 for a new psalm. A tone change on the same psalm
  keeps the page if it still exists, otherwise uses the last page.
  If it is playing, a tone change or a psalm change stops the sound:
  the melody is different, so carrying over at the same pulse (G18)
  would be wrong. The shared view therefore needs a way to hand over
  new text WITHOUT carry-over (the antiphonale's live-edit path keeps
  carry-over). Claude's call; going on to the next psalm when one
  finishes is a named future (VI).
- Fitter warnings: the first one shows in the banner; the chant is
  still drawn. There are none today, but the texts may change.

### III.3 `$psalterium` arguments (D2)

| Input | Result |
|-------|--------|
| `$psalterium` or `$psalterium()` | index |
| `$psalterium(109)` | psalm view, Ps 109, current tone |
| `$psalterium(109, 8 G)` | psalm view, Ps 109, tone 8 G (now the pane's tone) |
| `$psalterium(151)`, `(0)`, `(abc)` | error "psalterium: psalm number 1-150" |
| `$psalterium(109, 8 Z)` | error carrying `psalmodia_signum_invenire`'s reason, which lists the endings |

The topmost psalter pane in the stack receives the new argument and
comes to the front (`vicus_acervo_mutare`, `lib/vicus.c:3493-3517`, as
for the antiphonale). If none is open, one is mounted.

### III.4 State (`apps/psalterium/organum.stml`)

| Field | Type | Lifetime | Note |
|-------|------|----------|------|
| `visus` | electio index / psalmus | persistent, route-bound | absent = index |
| `psalmus` | number 1-150 | persistent, route-bound | absent in the index |
| `signum` | text | persistent | tone label; absent = "8 G" (D3) |
| `electus` | number | transient | index selection |
| `pagina` | number | transient | page in the current view |
| `spatium` | number | persistent | staff size, as the antiphonale |

Routes: `psalterium/index` and `psalterium/psalmus/{}`, as lector's
`<cognomina>`. A stored `signum` that the table no longer knows falls
back to 8 G silently. The texts and the fitted chant live in the
mount's memory, never in state.

### III.5 Sound

Same port as the antiphonale: one singer per vicus. A psalter pane
starting playback silently stops an antiphonale that was playing, and
the reverse.

## IV. The embedded psalter (D5)

A generator script in the house pattern (`tools/*_infigere.sh`, an
`aedificatio.stml` generator action, GENERATUM line 1, never edited by
hand) turns `musica/psalmi/1.txt`–`150.txt` into one C file with a
function that returns psalm N's text (1-150) as a chorda. The
`LICENTIA` (MIT notice) must travel with the binary: embed it too, and
make it reachable (exact surface left to the plan). The existing
generated-file gate covers drift between the files and the C file.

## V. Tests

- **Chant view extracted**: `probatio_antiphonale_latus` green and
  unchanged (II).
- **Embedded psalter**: 150 texts, each byte-identical to its file.
  The LICENTIA text starts with "MIT License".
- **Pane**: the index shows 150 rows across N pages, and row 109
  reads "109  Dixit Dóminus Dómino meo". Enter opens it, and the
  fitted gabc has `annotation: 8 G;`. `t` gives the next ending in
  table order (check the header), and the cycle wraps. `]` on 150 and
  `[` on 1 do nothing. Esc goes back to the index with 109 selected.
  All 150 psalms × the 33-ending cycle fit WITH Gloria Patri give no
  warnings (the existing test covers only `sine_gloria`).
  Every row of III.3 is checked. Restoring from a stored route gives
  the same psalm and tone. A tone change while playing stops the
  sound.
- **Fumus**: one screenshot of each view through the app (`-imago`),
  looked at by eye.

Each test must be watched failing before the code is written (house
rule: a gate is born red).

## VI. Non-goals

- Calendar, hours, antiphons, choosing tones by feast: later, with
  the 1962 calendar (deferred in psalmodia spec §VI q1).
- Canticles (Divinum Officium Psalm210-273): not imported.
- Hebrew/modern numbering next to the Vulgate number.
- Editing psalm text in the pane.
- Per-psalm tone memory (D6).
- Singing on into the next psalm when one ends (a set of psalms in a
  row); wanted for the Hours, a later feature.
- A `praeco psalterium` command (the antiphonale has `praeco
  antiphonale`). It would be easy to add later.

## VII. Anti-goals

- Temporary gabc files written so the antiphonale can watch them.
- Reading `musica/psalmi/` from disk at runtime.
- Scrolling instead of pages.
- Changing antiphonale behaviour as a side effect of the extraction.

## VIII. Left to the builder

Module and function names, the footer layout and its click targets,
rows per page, how typed digits are shown and cleared, the generated
C layout, and how the licence is surfaced.

## AUDIENDA (not verified when written)

- That `semita` routes (`<cognomina>`, `semita="ligatio"`) work for a
  new genus exactly as for lector. They were seen only in lector's
  manifest and were not traced.
- That a 263 KB embedded text adds nothing noticeable to vicus build
  time. The tone table is far smaller.
- That `album_*` fits a 150-row list with the house's row height at
  the default pane size. The number of pages was not measured.
