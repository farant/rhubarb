# pictor pinacotheca - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** a ⋯ square in pictor's status row opens a menu (Open picture..., New picture, Import image...); Open picture... shows a paged grid of re-dithered thumbnails, newest edit first; tap one to draw on it; relaunch reopens it.

**Architecture:** two pure library additions in `pictor_documentum` (recency list, thumbnail maker), then UI in `pictor_componentia` (composition, via `dispositio` + `elenchus`), `pictor_actiones` (handlers), `pictor_figurae` (icons, menu figure, thumbnail image source); state in the organum (`palette` values + a page field); startup in `pictor_applicatio`. The app turns it on with one compositio flag.

**Tech Stack:** C89 in Latin (latina.h), credo v2 suites, `bin/aedilis`, `silva.commissio`.

**Spec:** `project-specs/pictor-pinacotheca-spec.md` (APPROBATUM 2026-10-26) - read it first; D-numbers below refer to it.

## Global Constraints

- C89, all house flags (-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion ...); Latin identifiers; `i32`/`i64` are UNSIGNED; Roman-numeral macros (no MMMM); `nomen` is a macro - never an identifier; chorda is not NUL-terminated.
- Menu copy, exactly: `Open picture…`, `New picture`, `Import image…` (UTF-8 "…" as `"\xE2\x80\xA6"` in C strings; drawn by glyphae_ductae once secunda a625afcb is in main). No Export item.
- 12 thumbnails per page; 3 across x 4 down when surface latitudo <= altitudo, else 4 x 3 (D3). Never scrolling.
- Order: seq of the picture's latest act, descending; creation counts; viewing never reorders (D5).
- Thumbnail = visible-layer composite, contain-fit, re-dithered to pictor's palette at pictor pixel size (D4), via `pictor_imaginem_aptare`.
- ⋯ only where the app sets the flag: standalone pictor on every platform; NEVER vicus's pane or `pictor_terminalis` (D11) - their goldens stay byte-identical.
- No delete / rename / names (D9). New-picture names are internal: `pictura-<n>`.
- Thumbnail work uses its own scratch arena; never the montatio's arena or `documenta` table. After any picture switch read `app.montatio.doc`, never `app.doc` (spec I, arena trap).
- Layouts through `dispositio` (cells), no hand pixel arithmetic for the gallery.
- Tests: credo v2 (`CredoSectio` + `credo_suitam_currere`), close every temporary `volumen`. Commit through `silva.commissio` (detached launcher), one commit per task.

## Review Focus

1. Switching pictures with something pending (a half-drawn line, an open palette) must not leak it into the other picture -> T5 test "transitus mundus".
2. After switching, the next stroke must land in the NEW picture (stale `app.doc`) -> T5 test "ictus in pictura nova".
3. Page boundaries: exactly 12 pictures = one page, no arrows; 13 = two pages, page 2 holds one -> T5 test "paginae limites".
4. A reopened picture keeps its undo history (spec II.3) -> T5 test "revocatio post reditum".
5. A volume from before this change (only the root picture "") shows one thumbnail and relaunches as before -> T2 test "volumen vetus", T6 test "radix manet".

---

### Task 1: ⋯ in glyphae_ductae (D10)

**Files:**
- Modify: `lib/glyphae_ductae.c` (symbol table, sorted by runa: right after U+22EE ⋮ at :430), header comment of `include/glyphae_ductae.h` if it lists the ranges
- Test: `probationes/probatio_glyphae_ductae.c`

**Interfaces:** Produces: `glyphae_ductae_est(0x22EF)` VERUM; the glyph draws through every path that asks `glyphae_ductae` - terminale today, all app text once secunda's a625afcb ("glyphae ductae in textu generali", 2026-10-23, NOT yet in main) arrives. That commit also gives "…" its drawn glyph in apps; until then the menu's "…" shows as "." - acceptable, cosmetic.

- [ ] **Step 1: failing test** - section "ellipsis media": `glyphae_ductae_est(0x22EF)`; at 6 x 8 the mask has exactly three lit dots, all on one row, at the height of ⋮'s middle dot (same row as the hyphen, row III), at the same columns "…" (U+2026) uses (`.#.#.#`); at 8 x 16 it scales (three dots, one row).
- [ ] **Step 2:** `./compile_tests.sh glyphae_ductae` -> FAIL.
- [ ] **Step 3:** add the entry `{ 0x22EF, /* ⋯ midline horizontal ellipsis */ { "......", "......", "......", ".#.#.#", "......", "......", "......", "......" } }` in runa order.
- [ ] **Step 4:** PASS; `./compile_tests.sh terminale` green.
- [ ] **Step 5:** commit (spec + plan go in this commit too).

### Task 2: recency list (D5)

**Files:**
- Modify: `include/pictor_documentum.h`, `lib/pictor_documentum.c`
- Test: `probationes/probatio_pictor_bibliotheca.c` (new sections)

**Interfaces:** Produces:
```c
nomen structura {
    chorda spatium;   /* "" = radix */
       s64 seq;       /* actum ultimum picturae (aut conditio) */
} PictorPictura;

/* picturae voluminis, seq descendente (aequales: ordine viae) */
Xar*  /* de PictorPictura */
pictor_documenta_recentia (
    Volumen* volumen,
    Piscina* piscina);
```

- [ ] **Step 1: failing tests** - "recentia ordo": create "", "a", "b" (in that order), then one act on "a" -> spatia `a, b, ""`; then `pictor_picturam_ponere` "b" with no act -> order unchanged; then create "c" (no acts) -> `c, a, b, ""`; `revocare` on "b" -> "b" first (undo is an edit). "volumen vetus": volume with only "" -> one entry, spatium "". Each `seq` strictly descending.
- [ ] **Step 2:** `./compile_tests.sh pictor_bibliotheca` -> FAIL (undefined).
- [ ] **Step 3:** implement with ONE pass of `volumen_acta_legere(volumen, 0, piscina)`: map each act to its picture by the historia genera of that spatium (genus, rami, coniunctionis, cursoris - read how `lib/historia.c` composes them; do not duplicate the format, call/expose the composer if needed) and by the `plagula-condita` act of `<spatium>/documentum`; keep the max seq per spatium from `pictor_documenta_enumerare`'s list; sort descending.
- [ ] **Step 4:** PASS; time it on a volume of 13 pictures x 200 acts (print ms in the test log, no assert).
- [ ] **Step 5:** commit.

### Task 3: thumbnail maker (D4)

**Files:**
- Modify: `include/pictor_documentum.h`, `lib/pictor_documentum.c`
- Test: `probationes/probatio_pictor_bibliotheca.c`

**Interfaces:** Produces:
```c
/* pinax: compositum picturae 'spatium' (strata visibilia) in
 * latitudo x altitudo "contain", ditheratum (pictor_imaginem_aptare),
 * RGBA; vacuum = fundus thematis. Omnia in 'piscina' (scheda
 * vocantis). FALSUM si pictura ignota. */
b32
pictor_pinacem_facere (
                Volumen* volumen,
    InternamentumChorda* intern,
     constans character* spatium,
                    i32  latitudo,
                    i32  altitudo,
                Piscina* piscina,
                  Imago* exitus);
```

- [ ] **Step 1: failing tests** - "pinax": picture 100 x 200 with a filled black rect on its top half; `pictor_pinacem_facere(..., 50, 50, ...)` -> 50 x 50, picture area 25 wide centred (contain), top half of that area black-dominant, margins = background colour, every pixel a palette colour. "pinax strata": hide the layer holding the rect -> thumbnail all background. "pinax ignota": unknown spatium -> FALSUM. "scheda": `pictor_documenta_enumerare` and a montatio's `documenta` table unchanged after 12 calls.
- [ ] **Step 2:** FAIL. **Step 3:** open with `pictor_documentum_aperire` in the given piscina, `pictor_documentum_proiectio`, `pictor_imaginem_aptare`, indices -> RGBA (0xFF -> COLOR_BACKGROUND). **Step 4:** PASS + print ms per thumbnail. **Step 5:** commit.

### Task 4: ⋯ square and menu (D1, D2, D6, D11)

**Files:**
- Modify: `include/pictor_componentia.h`, `lib/pictor_componentia.c`, `include/pictor_actiones.h`, `lib/pictor_actiones.c`, `lib/pictor_figurae.c`, `apps/pictor/organum.stml` (+ its generated canon: `bin/fabrica quid apps/pictor/organum.stml` names the generator), `apps/pictor/pictor.c`
- Test: create `probationes/probatio_pictor_pinacotheca.c`; update `probationes/probatio_pictor_strata_ui.c` (its C1b picture-square section)

**Interfaces:**
- `PictorCompositio.elenchus_praesto` (b32): ⋯ square `quadratum.elenchus`, titulus `elenchus:quadratum`, left of the layers square; replaces `quadratum.imago` (C1b's square goes away; `imago_eligibilis` now only adds the menu item).
- `vacuum pictor_elenchum_parare(b32 importare, ElenchusRes res[III], Elenchus* e);` - the one item table both composer and figure use; item tituli `pinacotheca.aperire`, `pictura.nova`, `imago.eligere`.
- `palette` field value `"elenchus"` = menu open (via `palette.aperire`); menu node from `elenchus_componere(..., "elenchus.agere", ...)` anchored above the ⋯ square, clamped to the surface.
- `b32 pictor_elenchum_agere(...)` (tractator, registered `elenchus.agere`): index -> `pinacotheca.aperire` sets `palette="pinacotheca"`, `pagina_pinacothecae=0`; `pictura.nova` -> `pictor_picturam_condere(m, "pictura-<n>")` (n = 1 + highest existing n) then close; `imago.eligere` -> existing `pictor_imaginem_eligere` path then close.
- Handlers switch pictures without including `pictor_applicatio.h` (it includes `pictor_actiones.h`): new `PictorActiones` fields `vacuum* montatio`, `b32 (*picturam_condere)(vacuum* montatio, constans character* spatium)`, `b32 (*picturam_ponere)(vacuum* montatio, constans character* spatium)`; `pictor_montare` fills them with thin wrappers of `pictor_picturam_condere` / `pictor_picturam_ponere`. NIHIL (tests, vicus) = the items do nothing and return FALSUM.
- `figura_palettae`: node id `elenchus` -> `elenchus_figura` with `pictor_elenchum_parare`'s table (importare read from the item count). Icon `elenchus:quadratum` = three 2 x 2 dots, centred, COLOR_TEXT.
- `apps/pictor/pictor.c`: `compositio.elenchus_praesto = VERUM`.

- [ ] **Step 1: failing tests** (`probatio_pictor_pinacotheca.c`, real app on a temp volume, 215 x 419): "quadratum": ⋯ present, `quadratum.imago` absent. "elenchus": tap ⋯ -> `palette` = elenchus, 2 items; with `imago_eligibilis` 3 items in the order of the spec. "nova": tap New picture -> `montatio.doc` is a new picture, enumerare count +1, spatium `pictura-1`, second time `pictura-2`, menu closed. "importare": tap Import with a fake service -> service called once, menu closed. "extra": tap outside / Esc closes, nothing else changes. "sine flag": flag FALSUM -> no ⋯, tree identical to before.
- [ ] **Step 2:** FAIL. **Step 3:** implement as above (regenerate the canon after organum edits; actions `elenchus.agere`, `pinacotheca.agere` declared). **Step 4:** PASS; `./compile_tests.sh pictor`, `./compile_tests.sh vicus` green with goldens unchanged except the C1b section you updated. **Step 5:** commit.

### Task 5: the gallery (D3, D4, D7)

**Files:**
- Modify: `lib/pictor_componentia.c`, `lib/pictor_actiones.c`, `include/pictor_actiones.h`, `lib/pictor_figurae.c`, `include/pictor_figurae.h`, `lib/pictor_applicatio.c`, `apps/pictor/organum.stml` (+ canon)
- Test: `probationes/probatio_pictor_pinacotheca.c`

**Interfaces:**
- Field `pagina_pinacothecae` (numerus, transiens, absens 0, scriptor `pinacotheca.agere`).
- `palette = "pinacotheca"`: dialog node `pinacotheca` (PARTES_DIALOGUS) over the canvas area, laid out with `dispositio`: grid of up to 12 cells, nodes `pinax.<k>` (k = 0..11 on the page), PARTES_BOTTONE, titulus `pinax:<lat>x<alt>:<seq>:<spatium>` (+ `:electum` for the open picture); bottom row `pinacotheca.prior`, `pinacotheca.pagina` (text "p / n"), `pinacotheca.posterior`, `pinacotheca.claudere`; arrows absent when there is one page. The composer gets the recency list through `PictorCompositio.picturae` (Xar*, refreshed by the action that opens the gallery and after New picture).
- Image source: `PictorFigurae.pinacotheca` -> `PictorPinacotheca { Volumen*; InternamentumChorda*; Piscina* scheda; TabulaDispersa* pinaces; }`; `pictor_imago_fons` resolves `pinax:...` by building on miss with `pictor_pinacem_facere` into `scheda`; `pinacotheca.aperire` empties and recreates `scheda` (memory bounded to one gallery session).
- `b32 pictor_pinacothecam_agere(...)` (`pinacotheca.agere`): `pinax.<k>` -> `picturam_ponere(spatium from titulus)` + close; prior/posterior -> page -1/+1 clamped; claudere -> close.

- [ ] **Step 1: failing tests:** "rete": 13 pictures at 215 x 419 -> 3 x 4, 12 `pinax.*` on page 1, newest first, open one `:electum`; at 512 x 300 -> 4 x 3. "paginae limites": 12 pictures -> no arrows; 13 -> page 2 holds one; posterior on last page stays. "aperire": tap `pinax.3` -> `montatio.doc` is that picture, gallery closed. "ictus in pictura nova": stroke after switching -> act lands in the new picture's historia, the old one's `finis` unchanged. "revocatio post reditum": draw in A, switch to B, back to A, Cmd+Z undoes A's last stroke. "transitus mundus": half-drawn line + switch -> no act in either picture, no pending line. "claudere": × -> nothing changed. "memoria": open/close the gallery 5 times -> `scheda` usage does not grow.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS + `./compile_tests.sh pictor vicus` green. **Step 5:** commit.

### Task 6: relaunch reopens the last picture (D8)

**Files:**
- Modify: `lib/pictor_applicatio.c` (`pictor_applicatio_aedificare`)
- Test: `probationes/probatio_pictor_pinacotheca.c`

**Interfaces:** after `pictor_montare`: restored insula `pictura` non-empty and in the library -> `pictor_picturam_ponere`; non-empty but missing -> first of `pictor_documenta_recentia`; empty -> root as today. Then `app->doc = app->montatio.doc` (and actiones/figurae).

- [ ] **Step 1: failing tests:** "reditus": build app on a volume, New picture, draw, close the app exactly as the app's `finis` does, reopen the volume, build again -> `montatio.doc` is `pictura-1` with its stroke. "radix manet": never switched -> root. "absens": `pictura` names a missing picture -> most recent one.
- [ ] **Step 2:** FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 7: the four surfaces (spec II.6)

**Files:**
- Test: `probationes/probatio_pictor_pinacotheca.c` (section "imagines"); `lib/pictor_componentia.c` only if the screenshots show a fault

- [ ] **Step 1:** section "imagines": 13 varied pictures; for each surface - iPhone 215 x 419, Fire 7 512 x 300, Mac half pane (read the real size: run `bin/pictor` in Fran's pane, print the surface it receives; ask Fran before opening the window), C30 Pro (read on device, `tools/android/apk.sh apps/pictor/pictor.c -fumus` prints it) - render the gallery headless (recipe of `probationes/probatio_ludus_imago.c`) to `build/probatio_pictor_pinacotheca_<lat>x<alt>.png`; assert every node inside the surface, cells disjoint, grid shape by surface shape.
- [ ] **Step 2:** open each PNG (Read) and fix what looks wrong; show Fran the four.
- [ ] **Step 3:** on devices (Fran's word for each): `tools/ios/app.sh apps/pictor/pictor.c -tabula -currere`; `tools/android/apk.sh apps/pictor/pictor.c -instituere` on the Fire 7 and C30 Pro; Fran tries menu, New picture, gallery, relaunch; note how long the gallery takes on the Fire 7 (if noticeable -> a cache follow-up, filed in the ledger, not in this plan).
- [ ] **Step 4:** spec section VI (what building found) + status IMPLETUM; ledger note under …X7RJB; desideratum -> impletum.
- [ ] **Step 5:** commit.
