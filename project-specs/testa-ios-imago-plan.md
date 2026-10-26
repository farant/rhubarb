# pictor photo import (C1b) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (Fran chose inline, one task per turn, Fran approves each). Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** on Fran's iPhone a picture square beside the layers icon opens Apple's photo picker, and the chosen photo arrives in pictor as a pending paste (A/B dithering, OK = new layer, X = drop).

**Architecture:** the photo reaches pictor as a dropped file (`EVENTUS_DEPOSITIO`), which pictor learns to treat like Cmd+V (T1). The host shell may offer an "ask the user for a picture" service; pictor shows the square only when it is offered (T2). The iOS shell offers it with `PHPickerViewController`, normalizes the photo and delivers the drop (T3).

**Tech Stack:** C89 in Latin (latina.h), credo v2, Objective-C/UIKit + PhotosUI in `lib/testa_ios.m` only, `tools/ios/app.sh`.

**Spec:** `project-specs/testa-ios-imago-spec.md` (quarta, approved 2026-10-24).

## Global Constraints

- C89 + house flags; Latin identifiers; latina.h macros (Roman numerals, `nomen`, `per`...) never identifiers; Latin lint blocks commits.
- Phone only by capability (D1): the new `TestaAmbitus` field stays NIHIL in `lib/testa.c` and `lib/testa_android.c` (no stub). Desktop/Fire/terminal composition, goldens and PNGs stay byte-identical.
- A picked photo = a dropped file (D2): `$TMPDIR/imago_electa.png`, PNG, upright, longest side <= 1024 px (D4).
- One request at a time; taps while choosing/loading ignored; no indicator.
- Square hidden while a paste is pending.
- Main thread only for our code (B1 D1); PhotosUI callbacks hop to the main queue.
- Commit with `silva.commissio`; pre-heal capsule + dependents first; GUI windows / phone only with Fran's OK.

## Review Focus

1. A drop path that is a slice of a larger buffer (chorda, NOT NUL-terminated) - expected: decodes the right file. Pinned in T1 (path given inside a longer chorda).
2. A huge or very long photo (panorama 16000 x 3000) - expected: fits <= 1024 on its longest side, aspect kept, never 0 px. Pinned in T3 by a pure host-tested size function.
3. A drop of several paths where the first is not an image - expected: the first path that decodes is used. Pinned in T1.
4. The app goes to background or the phone locks while a photo loads - expected: no crash; the photo arrives on return or not at all; the flag clears either way. Fran by hand in T3.
5. A drop while a paste is pending - expected: replaces it (new preview), still one pending paste, no layer added. Pinned in T1.

## Rulings (made while planning)

- **R1 names:** service `TestaImaginemEligere imaginem_eligere; vacuum* imaginem_eligere_ctx;` (TestaAmbitus); same pair in `PictorActiones`; flag `b32 imago_eligibilis;` in `PictorCompositio`; action `"imago.eligere"` -> `pictor_imaginem_eligere`; node `"quadratum.imago"`, title `"imago:quadratum"`.
- **R2 placement:** `quadratum.imago` x = (layers square x) - cw - XX, same y, XX x XX, a child of the root (like `quadratum.strata`). At 187 px: size 110..130, picture 135..155, layers 161..181 (builder check).
- **R3 drop paths:** `datum.depositio.viae` split on '\n'; each copied into a NUL-terminated buffer before `imago_caricare_ex_file`; the first that decodes is used; decode into a temporary piscina destroyed after `pictor_documentum_glutinationem_parare`.
- **R4 size rule (pure, host-tested):** `testa_mensuras_aptare(i32 latitudo, i32 altitudo, i32 maxima, i32* lat, i32* alt)` in `testa_initus` - unchanged if both <= maxima, else scale the longest side to maxima, other side rounded, minimum I; FALSUM for a zero input.

## File map

| File | Task | Role |
|---|---|---|
| `lib/pictor_actiones.c` | 1, 2 | drop -> pending paste; action `imago.eligere` |
| `probationes/probatio_pictor_imago.c` | 1 | drop sections |
| `include/testa_cursus.h`, `include/pictor_actiones.h`, `include/pictor_componentia.h` | 2 | service field, actions pair, composition flag |
| `lib/pictor_componentia.c`, `lib/pictor_figurae.c`, `apps/pictor/pictor.c` | 2 | square, icon, wiring |
| `probationes/probatio_pictor_strata_ui.c` | 2 | 7 squares at 187, service call, hidden while pending |
| `include/testa_initus.h`, `lib/testa_initus.c`, `probationes/probatio_testa_initus.c` | 3 | `testa_mensuras_aptare` |
| `lib/testa_ios.m`, `aedilis.stml` | 3 | picker, normalize, drop; PhotosUI link |
| `project-specs/testa-ios-imago-spec.md` | 3 | IMPLETUM + VI |

---

### Task 1: pictor takes an image drop as a paste

**Files:** modify `lib/pictor_actiones.c` (`imaginem_glutinare` ~612, `pictor_instrumentum_eligere` ~813); test `probationes/probatio_pictor_imago.c` (new section).

**Interfaces:**
- Produces: `EVENTUS_DEPOSITIO` reaching the root (`instrumentum.eligere`) enters pending paste exactly as Cmd+V; internal shared helper `interior b32 _imaginem_glutinare_ex (InsulaRepositorium* repo, vacuum* ctx, constans Imago* imago)` used by Cmd+V and the drop.

- [ ] **Step 1: Failing test** - section `"V: depositio imaginis"` in `probatio_pictor_imago.c`, helper `depositio(app, chorda viae)` = `dispensator_tractare` with `EVENTUS_DEPOSITIO` (`numerus` = lines, x = y = 0). Assertions on a 480 x 480 app:
  - viae = `"probationes/fixa/icones/sips_16.png"` given as a SLICE of a longer chorda (e.g. the path + `"XYZ"`, mensura = path length) -> `doc->glutinatio.pendens` VERUM, field `glutinatio` = "B", layers count unchanged, `quadratum.strata` absent (pending row);
  - `glutinatio.confirmare` pressed -> layers + I, not pending;
  - viae = `"/nusquam/nihil.png"` -> not pending, field "";
  - viae = `"probationes/probatio_pictor_imago.c\nprobationes/fixa/icones/sips_16.png"` -> pending (first decodable used);
  - drop sips_16 while pending, then drop `probationes/fixa/icones/fons_256.png` -> still pending, layers unchanged, preview width/height = those of the second image fitted (compare with a fresh drop of fons_256 alone).
  Run `./compile_tests.sh probatio_pictor_imago` -> new section FRACTA (drop ignored today).
- [ ] **Step 2: Implement** - split `imaginem_glutinare` into the shared helper (R1/R3); in `pictor_instrumentum_eligere`, before the `EVENTUS_CLAVIS_DEPRESSUS` gate, handle `EVENTUS_DEPOSITIO` per R3 (temporary piscina via `piscina_generare_dynamicum`, destroyed after use).
- [ ] **Step 3: Green** - `./compile_tests.sh pictor`, `./compile_tests.sh vicus`, `./ludus_tessera/compile_probationes.sh`; desktop golden `probationes/pictor/pictor.arbor.stml` untouched.
- [ ] **Step 4: Commit** (lint: `depositio`, `viae` are Latin).

### Task 2: the picture square, by capability

**Files:** modify `include/testa_cursus.h` (TestaAmbitus), `include/pictor_actiones.h` (PictorActiones), `include/pictor_componentia.h` (PictorCompositio), `lib/pictor_actiones.c` (action + registration), `lib/pictor_componentia.c` (square), `lib/pictor_figurae.c` (icon), `apps/pictor/pictor.c` (wiring); test `probationes/probatio_pictor_strata_ui.c`.

**Interfaces:**
- Consumes: T1 (nothing new).
- Produces (R1): `nomen b32 (*TestaImaginemEligere)(vacuum* ctx);` + `imaginem_eligere`, `imaginem_eligere_ctx` in `TestaAmbitus` and `PictorActiones`; `imago_eligibilis` in `PictorCompositio`; `b32 pictor_imaginem_eligere(...)` registered as `"imago.eligere"` (same tractator signature as `pictor_instrumentum_eligere`), calling the service once, FALSUM if NIHIL.

- [ ] **Step 1: Failing tests** in `probatio_pictor_strata_ui.c`, new section `"quadratum imaginis"` at CLXXXVII x CCCXXIII:
  - flag off (default) -> `quadratum.imago` absent;
  - flag on (`app.montatio.compositio.imago_eligibilis = VERUM`, recompose) -> present, latitudo XX, title `"imago:quadratum"`, action `"imago.eligere"`; the seven squares (six of `_sectio_telephonum` + `quadratum.imago`) pairwise disjoint; `quadratum.imago` right edge <= `quadratum.strata` x;
  - press it with a fake service (counter) in `actiones_ctx` -> counter == I; no palette opened;
  - pending paste (fake `imago_capere` + Cmd+V, as in probatio_pictor_imago) -> `quadratum.imago` absent.
  Run -> FRACTA.
- [ ] **Step 2: Implement** the fields, the action, the square (R2; only when `imago_eligibilis` and not pending), the icon branch for `"imago:quadratum"` in `pictor_figurae.c` before the generic text branch (a frame with a small mountain line and a sun dot, theme colours, like the layers icon), and the wiring in `apps/pictor/pictor.c`: if `a->imaginem_eligere` -> copy the pair into `actiones_ctx` and set `compositio.imago_eligibilis`.
- [ ] **Step 3: Green** - pictor, vicus, ludus_tessera, `probatio_tessellatio` suites; `pictor.arbor.stml` byte-identical (flag off on desktop); `./tools/ios_struere_porta.sh` + `./tools/android_struere_porta.sh` viridis.
- [ ] **Step 4: Commit.**

### Task 3: the iOS shell offers the picker; on the phone

**Files:** modify `include/testa_initus.h`, `lib/testa_initus.c`, `probationes/probatio_testa_initus.c` (R4), `lib/testa_ios.m`, `aedilis.stml` (`testa_ios` rule: `-framework PhotosUI`, `-framework UniformTypeIdentifiers`), `project-specs/testa-ios-imago-spec.md`.

**Interfaces:**
- Consumes: T2's `TestaImaginemEligere` / fields; T1's drop handling.
- Produces: `b32 testa_mensuras_aptare (i32 latitudo, i32 altitudo, i32 maxima, i32* lat, i32* alt);` (R4).

- [ ] **Step 1: Failing test** - section `_sectio_mensurae` in `probatio_testa_initus.c`: (800, 600, 1024) -> (800, 600); (4032, 3024, 1024) -> (1024, 768); (3024, 4032, 1024) -> (768, 1024); (16000, 3000, 1024) -> (1024, 192); (20000, 5, 1024) -> (1024, 1); (0, 10, 1024) -> FALSUM. Then implement (integer arithmetic, rounding half up) -> green; on a plugged Android tablet if one is at hand (`./tools/android/probatio.sh testa_initus`), else ledger "not device-run".
- [ ] **Step 2: Shell** - `lib/testa_ios.m`: a small delegate class `TestaElector` (`PHPickerViewControllerDelegate`); `_imaginem_eligere(ctx)` presents `PHPickerViewController` (images filter, selectionLimit 1) from the root view controller unless the "eligens" flag is set; on finish: dismiss; no result -> clear flag; else `loadObjectOfClass:[UIImage class]` -> main queue: draw upright into an RGBA context of `testa_mensuras_aptare(w, h, M + XXIV)` (1024) (UIGraphicsImageRenderer or CGBitmapContext + `[UIImage drawInRect:]`, which applies orientation), `UIImagePNGRepresentation` -> `$TMPDIR/imago_electa.png`, deliver `EVENTUS_DEPOSITIO` (viae = that path, numerus I) via `_tradere` + `_quadrum_petere`, clear flag; any failure -> print `testa: imago non lecta: <cause>` and clear flag. Fill `ambitus.imaginem_eligere` in `_initium_facere`. Manual retain/release (no ARC): the delegate lives for the process.
- [ ] **Step 3: Link rule + builds** - aedilis.stml rule; `./tools/aedilis_struere.sh`; `bin/aedilis apps/pictor/pictor.c --varians ios,posix --nexus` lists PhotosUI; `./tools/ios_struere_porta.sh` viridis; simulator smoke `./tools/ios/app.sh apps/pictor/pictor.c -fumus` EXITUS=0 (the square appears in its PNG - open it).
- [ ] **Step 4: Phone (Fran unlocks)** - `./tools/ios/app.sh apps/pictor/pictor.c -tabula -currere`; Fran: portrait + landscape photo upright, A/B, OK -> layer, X, cancel picker, double tap = one picker, lock while a photo loads (Review Focus 4).
- [ ] **Step 5: Close + commit** - spec status IMPLETUM + section VI (rulings, measurements, AUDIENDA answered: blocks under manual RR, HEIC via UIImage); worklog `lib/testa_ios.worklog.md`; ledger note under …577GAS; memory `ios-toolchain` line.
