# pictor: a photo from the library, on the phone (spec C1b)

**Status: IMPLETUM 2026-10-24** - designed in conversation with Fran
(2026-10-24), approved ("that looks great"); plan
`project-specs/testa-ios-imago-plan.md`; built in T1-T3 and tried by
Fran on the phone ("that looks great"). Section VI records what
building found. Worktree quarta. Follows
C1 (`project-specs/testa-ios-spec.md`, IMPLETUM: pictor on Fran's
iPhone). Ledger: mobile vision desideratum ...577GAS.

*Fran 2026-10-24: "the next thing that i would like to add is the
ability to add a picture from the photo library? kind of the same as if
it was pasted in the vicus pictor ... next to the layers icon we can add
another icon like a "picture" / pict type icon and clicking on it lets
you choose a photo, add it as a layer, have the same dithering options
that pictor normally has"; on where: "i think it would only be on the
phone for now"; on the design (photo = a dropped file): "sure, that
sounds great".*

Goal: **on the phone, a picture square beside the layers icon opens
Apple's photo picker; the chosen photo arrives in pictor exactly like a
paste in vicus - pending, dithered A (Atkinson) or B (patterns), OK puts
it on a new layer, X drops it.**

Decisions:

- **D1 Phone only, by capability (Fran):** the host shell may offer a
  "let the user choose a picture" service in `TestaAmbitus`. Only the
  iOS shell offers it; pictor shows the picture square only when the
  service is there. No platform test in pictor; macOS, the Fire and the
  terminal pictor are unchanged (their composition, goldens and PNGs
  stay byte-identical). The new field must stay NIHIL in the macOS and
  Android shells - NOT a failing stub like their `imago_glutinata`, or
  the square would appear on the Fire.
- **D2 A picked photo is a dropped file (Fran: yes):** the shell writes
  the chosen photo to a file in the app's temporary directory and hands
  pictor the existing drop event, `EVENTUS_DEPOSITIO` (one absolute path,
  `numerus` 1, `promota` FALSUM). pictor learns to take an image drop as
  a paste. Dragging an image file onto pictor on the Mac later needs
  only the macOS drop wiring, nothing new in pictor.
- **D3 Apple's photo picker** (`PHPickerViewController`, PhotosUI):
  images only, one photo. It runs out of process and hands back only
  the chosen photo, so iOS asks no photo-library permission and the app
  needs no usage string.
- **D4 The shell normalizes (my call):** the photo is drawn upright
  (EXIF orientation applied - a phone portrait arrives rotated
  otherwise) and scaled so its longest side is at most 1024 pixels
  (pictor's canvas is ~215 x 419; the paste path fits and dithers it
  anyway), written as PNG (lossless, decoded by `imago_caricare_ex_file`).
  Cost if wrong: a size constant.
- **D5 Same paste, same buttons:** the pending-paste state already shows
  touch buttons A / B / OK / X in place of the squares
  (`lib/pictor_componentia.c`, "glutinatio pendens"). Nothing new there.

## I. What exists (2026-10-24)

| Fact | Where |
|---|---|
| Paste (Cmd+V): `imaginem_glutinare` asks the host's `imago_capere` service, then `pictor_documentum_glutinationem_parare(doc, &imago)` and field `glutinatio` = "B"; OK -> `glutinationem_confirmare` (new layer above the current one), X -> `glutinationem_abicere` | `lib/pictor_actiones.c:612-695` |
| Pending-paste UI: A / B / OK / X squares replace the status squares; a stroke erases the preview | `lib/pictor_componentia.c:569-630, 700-` |
| Drop event: `EVENTUS_DEPOSITIO`, `datum.depositio {x, y, viae ('\n'-separated absolute paths), numerus, promota}`; pictor does not handle it yet | `include/eventus.h:57, 352-358` |
| Image decoding: `imago_caricare_ex_file(via, piscina)` (PNG, JPEG, ...) | `include/imago.h:43` |
| Host services in `TestaAmbitus`: `imago_glutinata` + `imago_ctx` (clipboard); Android and iOS fill it with a stub that always fails (not NIHIL); pictor wires it in `apps/pictor/pictor.c:72-73`, after `pictor_applicatio_aedificare` | `include/testa_cursus.h:44`, `lib/testa_ios.m`, `lib/testa_android.c` |
| Routing: `EVENTUS_DEPOSITIO` is neither a pointer nor a focus event, so the dispenser gives it to the ROOT whatever x/y say; the root's action is `instrumentum.eligere` (`pictor_instrumentum_eligere`, which today returns FALSUM for non-key events). Delivery is synchronous (`ludus_fenestra_tractare` -> `dispensator_tractare`) | `lib/destinatio.c:13-30, 184-222`, `lib/pictor_componentia.c:603`, `lib/pictor_actiones.c:836` |
| Composition is pure: `pictor_componere` sees `PictorCompositio` + insula state, not the actions context | `include/pictor_componentia.h` |
| Blocks (completion handlers) already compile under the house ObjC flags | `lib/vitrea_macos.m:964` |
| iOS shell: one scene, a root view controller (`TestaRector`) that can present the picker, main thread only | `lib/testa_ios.m` |
| Status row at phone width: tool, colors x2, pattern, size squares + layers square at the right; pop-ups clamp to the surface | `lib/pictor_componentia.c`, `probationes/probatio_pictor_strata_ui.c` |

## II. Design

### II.1 The service (`include/testa_cursus.h`)

```c
/* ask the user for a picture (non-blocking). VERUM = a chooser was
 * shown; the answer, if any, arrives LATER as EVENTUS_DEPOSITIO. */
nomen b32 (*TestaImaginemEligere)(vacuum* ctx);

TestaImaginemEligere imaginem_eligere;   /* in TestaAmbitus; NIHIL = none */
             vacuum* imaginem_eligere_ctx;
```

Exact names are the builder's (Latin lint). macOS and Android shells
leave it NIHIL (memset already does).

### II.2 pictor

- `apps/pictor/pictor.c` wires the service into the actions context
  next to `imago_capere`, and tells the composition it exists (a flag in
  `PictorCompositio` or an insula attribute - builder's choice; the host
  test sets it the same way).
- Composition: when the service is present, a square `quadratum.imago`
  (20 x 20, title `"imago:quadratum"`, action `imago.eligere`, a child
  of the root like `quadratum.strata`) sits immediately left of the
  layers square. `pictor_figurae` draws the icon (a framed picture: a
  little landscape - builder's choice of pixels, same palette as the
  layers icon). Absent service -> no node at all. **While a paste is
  pending the square is hidden** (my call): the pending row (A / B / OK
  / X + size) already replaces the squares and drops the layers square;
  to pick another photo the user presses X first. (At 187 px the
  pending size square would overlap it anyway.)
- Action `imago.eligere`: calls the service. Nothing else changes state.
- **Drop -> pending paste:** a drop with an image path behaves exactly
  as Cmd+V with that image: decode with `imago_caricare_ex_file`, then
  the same `glutinationem_parare` + field "B" path (`imaginem_glutinare`
  is split so both share it). A drop that does not decode (not an image,
  missing) changes nothing. A drop while a paste is pending replaces it,
  as a second Cmd+V does (reachable from the host and from a later Mac
  drop; on the phone the square is hidden then). The drop reaches the
  root, so `pictor_instrumentum_eligere` handles `EVENTUS_DEPOSITIO`
  (x/y unused; the shell sends 0, 0).
- **Memory (my call):** the dropped file is decoded into a TEMPORARY
  pool, destroyed right after `pictor_documentum_glutinationem_parare`,
  which already copies what it keeps (scaled to the document, both
  ditherings, previews) into the document's pool - so a 1024 px photo
  (~4 MB RGBA) does not stay per pick. What the paste keeps grows the
  document pool as Cmd+V always has.

### II.3 The iOS shell (`lib/testa_ios.m`)

- Fills `imaginem_eligere`: presents `PHPickerViewController`
  (filter images, selection limit 1) from `TestaRector`. **One request
  at a time (my call):** a single "choosing" flag covers the picker
  being up AND the photo still loading (an iCloud-only photo can take
  seconds or fail offline); taps on the square meanwhile are ignored.
  No loading indicator in C1b.
- On a result: loads the `UIImage` from the item provider, then **on the
  main queue** (the provider calls back on a queue of Apple's; all our
  work stays on the main thread - B1 D1): draws it upright and at most
  1024 px on its longest side, writes PNG to
  `$TMPDIR/imago_electa.png` (overwritten each time), delivers
  `EVENTUS_DEPOSITIO` through the usual `_tradere` + frame request (the
  path bytes need only live until `_tradere` returns), clears the flag.
- Cancel or a load failure: nothing reaches pictor (failure printed to
  stdout as the shell's other lines are).
- Frameworks: `PhotosUI` and `UniformTypeIdentifiers` join the
  `testa_ios` link rule in aedilis.stml.

## III. Tests

1. **pictor drop (host suite):** a fixture PNG dropped -> pending paste
   ("glutinatio" = "B", the document's paste prepared); A/B/OK as for
   Cmd+V (OK -> one more layer); a non-image path and a missing path ->
   no change; a second drop replaces a pending one.
2. **Capability:** service NIHIL -> no `quadratum.imago` (desktop golden
   `pictor.arbor.stml`, scriba/vicus PNGs, tessera/tessellation suites
   unchanged); service present -> the square exists, its action calls
   the service once.
3. **Phone width:** with the service, the seven status squares are
   pairwise disjoint at 187 x 323 (iPhone SE), as T2's test does for six.
4. **Gate:** `ios-struere` builds with PhotosUI.
5. **Fran by hand on the phone:** tap the picture square, pick a
   portrait and a landscape photo (both upright), A/B, OK -> new layer
   with the picture; X; cancel the picker; draw on top; tap the square
   twice quickly (one picker only).

## IV. Not in this spec

Camera capture, several photos at once, wiring the Mac's drag-and-drop
(falls out of D2 later), Android's photo picker, saving/exporting
drawings to Photos, the pinacotheca.

## AUDIENDA (not verified when written)

- PHPicker / NSItemProvider are block-based: `lib/vitrea_macos.m` shows
  blocks compile under our flags; the iOS shell uses manual
  retain/release (no ARC), so captured objects need explicit care.
- Whether a picked HEIC loads as `UIImage` through the provider without
  extra work (expected yes).


## VI. Built (2026-10-24)

Commits in quarta: T1 976f1d71 (drop -> paste), T2 0e4434b7 (picture
square by capability), T3 (this commit: resize rule, iOS picker).

**Rulings made while building**

- **Drop test at phone size:** a 480 x 480 document with four pastes in
  one section peaked at 433 MiB of arena (credo ceiling 256 MiB); the
  section uses 215 x 419 (the phone's surface) instead of a "gravis"
  flag.
- **Fake service without global state:** examen refused a static call
  counter written in a section and reset nowhere; the counter now
  travels through `imaginem_eligere_ctx`.
- **No UniformTypeIdentifiers link:** the shell uses
  `[PHPickerFilter imagesFilter]`, no UTType constants - the `testa_ios`
  rule gained `-framework PhotosUI` only.
- **Resize rule** `testa_mensuras_aptare` (testa_initus, host-tested):
  integer, round half up, i64 intermediate, minimum I; FALSUM on a zero
  input. Not device-run on Android (no tablet at hand); pure C.
- **Picker in the shell:** `TestaElector` (PHPicker delegate, lives for
  the process), `_rector` = the root view controller presents it; one
  flag `_eligens` covers picker-open and loading. `loadObjectOfClass:
  [UIImage class]` -> main queue -> `UIGraphicsImageRenderer` at scale 1
  (`drawInRect` applies EXIF orientation, alpha kept) -> PNG
  `$TMPDIR/imago_electa.png` -> `EVENTUS_DEPOSITIO`. Blocks compile
  under the house flags with manual retain/release (the image is
  retained across the hop to the main queue).

**Measured / checked:** resize suite 11/11; simulator smoke EXITUS=0 with
the picture square in its PNG; `ios-struere` and `android-struere`
green; desktop golden unchanged; phone: Fran picked photos, A/B, OK
("that looks great").

**AUDIENDA answered:** blocks compile under `-std=c89 -pedantic
-Werror` ObjC (precedent `lib/vitrea_macos.m`); HEIC photos load as
`UIImage` through the provider with no extra work (Fran's library).

