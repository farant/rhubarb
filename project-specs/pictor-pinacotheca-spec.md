# pictor: the gallery and the ⋯ menu (pinacotheca)

**Status: IMPLETUM IN TELEPHONO 2026-10-26; tabellae et Mac
differuntur** - designed in conversation with Fran (2026-10-26),
mockups in the browser companion (choice B, 3 x 4); approved
("everything else looks right", Mac = portrait pane); plan
`project-specs/pictor-pinacotheca-plan.md` built in T1-T7 + a review
fix; Fran tried it on the iPhone ("everything is working, i can see
the glyph came through too"). Section VI records what building found.
Worktree quarta. Follows C1b (`project-specs/testa-ios-imago-spec.md`,
IMPLETUM: photo import) and C2 (TestFlight, in main 83d03e29). Ledger:
desideratum ...X7RJB (pinacotheca), under the mobile vision ...577GAS.

*Fran 2026-10-26: "we can make the image icon button into a three dots
kind of menu icon and then the pop-up menu can have an option to import
image (like the current one) and then option to open picture (see the
gallery), new picture, maybe export picture at some point"; on the
gallery: "a grid of thumbnails (at normal pixel density so shrunk then
re-dithered, etc) with the newest or most recently edited pictures at
the top. then just paginated (no scrolling) ... can tap one to open it
in the image editor"; on the mockup: "B for the thumbnail is good
(3x4)"; on delete: "i'm not worried about adding delete for now"; on
platforms: "good to show them on desktop and on android tablet ... we
don't need to support arbitrary dimensions but for the two android
tablet types and desktop macos we'll just have to make sure the gallery
looks right".*

Goal: **pictor keeps many pictures, and you can move between them: a ⋯
square in the status row opens a small menu - Open picture..., New
picture, Import image... - and Open picture... shows a paged grid of
re-dithered thumbnails, newest edit first; tap one to draw on it.**

Decisions:

- **D1 The ⋯ square (Fran):** the picture square of C1b becomes a ⋯
  square, left of the layers square, on EVERY platform pictor runs on as
  an app (iPhone, Fire 7, C30 Pro, macOS). Its icon is drawn like the
  other squares' icons (three 2 x 2 dots, centred) in
  `lib/pictor_figurae.c`, not a font glyph.
- **D2 The menu is `elenchus` (my call):** the house context menu
  (`include/elenchus.h`, used by calendarium and nomenclator) opens
  above the ⋯ square, the same way pictor's palettes open above their
  squares, and closes on a tap outside or Esc. Items, in this order:
  - **Open picture...** - opens the gallery (D3).
  - **New picture** - D6.
  - **Import image...** - today's photo import (C1b), present ONLY
    when the host offers `imaginem_eligere` (today: iOS). On the Mac and
    Android the menu has two items.
  - Export picture... comes later; it is not shown until it exists.
- **D3 The gallery (Fran, mockup B):** "Open picture..." covers the
  canvas with a dialog: a grid of 12 thumbnails per page, 3 across x 4
  down on a tall surface (iPhone; macOS, which Fran keeps in a
  half-screen pane, so portrait); **4 across x 3 down on a wide one**
  (surface wider than tall: the tablets in landscape) - my call, so wide
  screens don't get tiny cells. The rule follows the surface's actual
  shape at the time, never the platform. Below the grid: ‹, "1 / 2", ›, and an ×
  that closes the gallery and changes nothing. No scrolling, ever.
- **D4 Thumbnails at pictor's pixel size (Fran: "shrunk then
  re-dithered"):** each thumbnail is the picture's composite (all
  visible layers), scaled to fit its cell ("contain", aspect kept) and
  dithered again into pictor's palette, drawn with the same chunky
  pixels as the canvas - never a smooth miniature at device resolution.
  `pictor_imaginem_aptare` (`include/pictor_documentum.h`) already does
  exactly this for photo import (area scale down, Atkinson, palette
  indices). The empty part of a cell is background.
- **D5 Order = most recent edit first (Fran):** the picture whose
  latest edit is newest comes first (top-left of page 1). "Latest edit"
  = the volume sequence number (`VolumenActum.seq`) of that picture's
  last act; a new, still-empty picture counts from its creation.
  Opening or looking at a picture does NOT move it. No clock involved.
- **D6 New picture:** a blank picture sized to this surface, exactly
  as `pictor_picturam_condere` already makes one (B1 rule: a picture
  belongs to the surface it was made on). It gets an automatic internal
  name that is never shown; it becomes the open picture and the first
  thumbnail of the gallery.
- **D7 The open picture:** its thumbnail is outlined in the gallery.
  Tapping any thumbnail opens that picture (`pictor_picturam_ponere`)
  and closes the gallery; tapping the open one just closes it.
- **D8 Remember the open picture (my call, follows from D6):** standalone
  pictor today always opens the root picture at launch
  (`apps/pictor/pictor.c` -> `pictor_applicatio_aedificare`, no state
  restore). With many pictures, relaunching must reopen the picture you
  were last on. If that picture is gone (should not happen - no
  delete), fall back to the most recently edited one.
- **D9 No names, no delete, no rename (Fran):** the gallery shows only
  pictures. Delete (with a confirmation step) is a later round.
- **D10 Glyph ⋯ (Fran):** add U+22EF (MIDLINE HORIZONTAL ELLIPSIS)
  to `glyphae_ductae` (the house's drawn special glyphs, beside ⋮ and
  …). App text draws those glyphs once secunda's a625afcb
  ("fenestra_textus: glyphae ductae in textu generali", 2026-10-23)
  reaches main; it also turns "…" (today "." in the 6 x 8 font,
  `lib/fons.c:90`) into real dots. The ⋯ SQUARE does not depend on it
  (drawn icon, D1).
- **D11 Where it does NOT appear:** vicus's pictor pane keeps its
  current status row (vicus switches pictures through its own routes,
  `pictor/<spatium>`); the terminal pictor (`pictor_terminalis.c`) is
  unchanged. The ⋯ square appears only when the app turns it on, the
  same capability-flag pattern C1b used (`imago_eligibilis`).

## I. What exists (2026-10-26)

| Fact | Where |
|---|---|
| Several pictures per volume, one per *spatium*; list: `pictor_documenta_enumerare(volumen, piscina)` -> Xar of spatium chorda, in PATH order ("" = the root picture) | `include/pictor_documentum.h`, `lib/pictor_documentum.c:1159` |
| Show an existing picture: `pictor_picturam_ponere(m, spatium)`; make a new one: `pictor_picturam_condere(m, spatium)` (FALSUM if the spatium exists); the montatio's `documenta` table caches pictures already shown | `include/pictor_applicatio.h`, `lib/pictor_applicatio.c:273, 330` |
| Open a picture without showing it: `pictor_documentum_aperire` (checkpoint + replay) -> `pictor_documentum_proiectio` (composite RGBA) | `include/pictor_documentum.h` |
| Shrink + re-dither: `pictor_imaginem_aptare(fons, lat, alt, ...)` -> palette indices, contain, centred | `include/pictor_documentum.h`, `lib/pictor_documentum.c:1502` |
| Every act has `seq` (arrival order in this volume); `volumen_acta_generum_legere` reads one document's acts | `include/volumen.h:300-330` |
| Status row squares 20 x 20 (`PARTES_BOTTONE`); palettes (`PARTES_DIALOGUS`) open above their square and clamp to the surface; C1b picture square `quadratum.imago` shown when `compositio.imago_eligibilis` | `lib/pictor_componentia.c:103-230, 805-830` |
| Context menu `elenchus`: `elenchus_componere` -> "elenchus" (PARTES_DIALOGUS) + "elenchus.<k>" items; disabled items; closes via dimissio | `include/elenchus.h`, `lib/elenchus.c`, users `lib/kalendarium.c`, `lib/nomenclator.c` |
| Icons drawn from lines/rects by title (`imago:quadratum`, `strata:quadratum`) | `lib/pictor_figurae.c:669-720` |
| Surfaces (pictor pixels = points / SCALA II): iPhone 15 Pro Max 215 x 419 (portrait -> 3 x 4); Fire 7 1024 x 600 px landscape -> 512 x 300 (-> 4 x 3; Android apps are locked to `sensorLandscape`); C30 Pro: measure, landscape too; macOS: default window 640 x 480, but Fran runs it in a half-screen pane (portrait -> 3 x 4) - check the surface pictor actually receives there | `apps/pictor/pictor.c:17-60`, `tools/android/AndroidManifest.forma.xml:13`, `tools/android/apk.sh:193` |
| Arena trap: `pictor_documentum_aperire` allocates in the arena it is given, and the montatio's arena is never freed; after `pictor_picturam_ponere`, `app.doc` still points at the OLD picture - read `app.montatio.doc` (audit 2026-10-18) | `lib/pictor_applicatio.c`, `include/pictor_applicatio.h` |

## II. What "done" looks like

1. On the iPhone, the Fire 7, the C30 Pro and the Mac: the ⋯ square
   is in the status row; tapping it opens the menu (two items, or three
   on the iPhone).
2. New picture gives a blank canvas; drawing on it, then Open
   picture..., shows it first, outlined, and the older pictures after
   it, newest edit first.
3. Tapping an older thumbnail opens it with its layers and undo history
   intact (it is the same picture, not a copy).
4. More than 12 pictures -> ‹ › page through them; the page count is
   right; no scrolling anywhere.
5. Quitting and relaunching reopens the picture you were last on (D8).
6. Screenshots of the gallery (headless `-imago`, and on the devices)
   at each of the four surfaces look right to Fran - cells evenly
   spaced, thumbnails recognisable, nothing clipped.
7. vicus's pictor pane and the terminal pictor look exactly as before
   (their goldens unchanged).
8. Import image... on the iPhone still works exactly as in C1b.

## III. Non-goals

Delete, rename and visible names (D9); Export (later item); scrolling;
arbitrary window sizes beyond the four surfaces; vicus and the terminal
pictor (D11); moving pictures between devices; drag-and-drop on the Mac.

## IV. Left to the builder

- How the "latest edit" order is computed (scan per open of the gallery,
  or keep it as the volume changes) - but it must agree with D5.
- Whether thumbnails are cached, and where. If cached, a cache is
  derived data only: it must be rebuilt from the pictures, never be the
  only copy of anything, and never go stale silently (a picture edited
  since must show its new thumbnail). The gallery should come up
  without a noticeable wait on the Fire 7, the slowest device.
  Constraint either way: pictures opened only to make thumbnails use
  their own scratch arena, freed after, and never enter the montatio's
  `documenta` table - otherwise memory grows each time the gallery
  opens (see the arena trap in I).
- The automatic internal name of a new picture (unique; never shown).
- Where D8 keeps "the picture you were last on" (in the volume is the
  obvious home - it lives with the pictures).
- The look of ‹ › × and the page text on the first/last page (hidden or
  dimmed); one page of pictures may hide ‹ › entirely.
- Menu row height: `elenchus` was made for the mouse; on touch screens
  rows must be easy to hit (the status squares are 20 pixels) - adjust
  if its rows are too thin for a finger.

## AUDIENDA (not verified while writing)

- The C30 Pro's surface size (not in memory; read it on the device).
- Whether `elenchus` composes inside pictor's status-row flow without
  changes (calendarium/nomenclator use it from their own organa).
- RESOLVED: U+2026 is "." in the 6 x 8 font; the drawn glyph exists in glyphae_ductae and reaches app text with secunda a625afcb (D10).
- Cost of opening a picture for its thumbnail (checkpoint + replay) on
  the Fire 7, with 12 per page.
- The tablets' orientation on the device (the manifest says
  landscape; confirm the gallery screenshots there).

## VI. What building found (2026-10-26)

**Commits (quarta):** T1 5e8db818 (⋯ glyph), T2 68a16462 (recency +
`historia_genera`), T3 c5321551 (thumbnail maker), T4 763787ed (⋯ square
+ menu), T5 49a9cda0 (gallery), T6 be1bfe56 (relaunch), main merged
516f2b7a (secunda's a625afcb: drawn glyphs in app text, so "…" in the
menu labels became real dots), T7 73d8b1f7 (headless screenshots),
review fixes 756c78c2.

**Rulings worth keeping (full list in the executor ledger):**

- *Owner check is silent.* `palette` belongs to `palette.aperire`; a
  write from the menu action was dropped with no error. Menu and
  gallery write it under that owner's name (`palettam_scribere`), as
  `glutinationem_scribere` already did.
- *Dimissio closes popovers after a "claudit" item.* "Open picture…"
  switches `palette` to "pinacotheca" itself, so its item has `claudit`
  cleared - otherwise the gallery would close the instant it opened.
- *A half-drawn line captures the next tap anywhere* and abandons the
  line (existing linea rule). The first tap on ⋯ only cancels the line;
  a picture switch can never carry a pending stroke (Review Focus 1).
- *One gallery state, three readers.* `PictorPinacotheca` lives in the
  montatio (scratch arena emptied with `piscina_vacare` on each opening,
  the newest-first list, a provenance -> thumbnail table); composer,
  actions and figures point at it. The list is read by the opening
  action, never the composer.
- *Thumbnail identity is data.* `pinax:<w>x<h>:<seq>:<spatium>` - the
  seq of the picture's latest edit is in the key, so an edited picture
  gets a fresh thumbnail; nothing persists between openings.
- *Recency without a clock* (D5): one indexed read of the pictures'
  history genera + `plagula-condita`; `historia_genera` makes
  historia's names public instead of copying them. 13 x 200 acts ~2 ms
  (Mac); a thumbnail ~0.9 ms (Mac, small picture).
- *Background is a palette colour* (`PALETTE_WARM_GRAY`), so empty
  areas re-dither to exactly the background - no transparency step.
- *Relaunch* (D8): `pictor_picturam_restituere(app)`, called by the
  window app only. The final review caught the first version inside
  `pictor_applicatio_aedificare`, which also changed the terminal
  pictor (D11).
- *fabrica-8:* a test reading an environment variable must declare it
  (`<ambitus>` on probationes_radicis); the screenshot switch is
  `PINACOTHECA_IMAGINES` (PNGs only when set; gates render in memory).
- *Census pins* that count pictor's state fields moved (owners XVIII ->
  XIX in pictor_canones and organum, defaults X -> XI in
  pictor_montatio) for `pagina_pinacothecae`.

**Deferred minors (final review):** tapping the open picture's
thumbnail re-runs `picturam_ponere` (may push a duplicate route entry)
- short-circuit on `:electum`; a thumbnail that fails to build is
retried every repaint (cache a failure); the third menu item has two
sources (`imago_eligibilis` vs `imaginem_eligere`); ‹ › not dimmed at
the ends; `picturas_facere` test helper re-creates existing pictures;
`spatium_novum` enumerates into the picture's long-lived arena; the
organum note on `pagina_pinacothecae` overstates when it is cleared.

**Still open (Fran, 2026-10-26: "test the tablets later"):** Fire 7
and C30 Pro device runs, the C30 Pro and Mac half-pane surface sizes
(their screenshots), and the Fire 7 gallery timing (spec IV) - if
opening the gallery is noticeable there, a thumbnail cache is the
follow-up. Then status -> IMPLETUM and the desideratum closes.

