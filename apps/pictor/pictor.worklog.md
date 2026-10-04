# pictor.worklog.md

## 2026-09-05 — natus, coctura prima (ludus P3 T11)

The first standalone pictor: a window, a white 320×200 sheet in a
viewport, a brush. `pictor.c` wires what the plan built: a volumen
(temporary under `-fumus`, `pictor.volumen` in the cwd otherwise —
a P3 placeholder; P5's `documentum.aperire/servare` own paths), the
document opened or created, the islands from the canons and the
owner table on disk (`apps/pictor/canones/`; capsula is the P4 pull
when icons arrive), the two registries, the dispensator with
pictor's `componere`, a fenestra, and `ludus_fenestra_currere`.
`pictor.sh` is the apps/ convention minus capsula and speculum:
aedilis derives the closure and emits the build script; `rm -f`
before `cp` because macOS SIGKILLs an overwritten signed binary.
`fumus.sh` runs thirty frames in a real window and greps the
timing line.

The numbers (spec §10 has the same paragraph): closure 41 house
objects + 45 headers, 2,233,103 bytes of house source, plus
`vendor/sqlite3.c` (9,514,279 bytes) through volumen; warm build 0.11 s;
binary 1,854,936 bytes. Frame timings over thirty frames: composition 0,
pingere 0, rasterize 35 ms total, worst frame 2 ms.

Two findings. The single-file amalgam is refused: the ligator will
not carry a vendor unit (v1), and sqlite is in the closure through
volumen, so a one-file pictor needs either a ligator that ships
sqlite beside the amalgam or a document store without sqlite —
a decision for the periodical, not for P3. And `DCXL` is not a
latina numeral; the window width is `(DC + XL)`.

Names introduced by Plan 2, to seal or rename (spec §10 bullet):
`pictor_documentum`, `pictor_componentia`, `pictor_actiones`,
`pictor_figurae`, `figura_tabulae`, `figura_tituli`, `prospectus`,
`ludus_fenestra`, `ludus_quadrum`, `tabula_pixelorum_creare_nuda`,
`ramus` (the undo branch actum), `domini`/`dominus`/`scriptor`
(ownership), `limen` (the delivery boundary), `numerus_vivorum`.

## 2026-10-03 — one assembly, two mains (module 013 A4)

pictor's assembly (volume, document, canons + owners from disk,
insulae, actions, figurae, dispensator) moved out of `principale` into
`include/pictor_applicatio.h` + `lib/pictor_applicatio.c` (beside the
other pictor_* libs - in apps/pictor aedilis could not resolve it from
a test elsewhere). `pictor.c` = assembly + ludus_fenestra;
`pictor_terminalis.c` (+ `pictor_terminalis.sh`, AEDIFICARE_SOLUM) =
assembly + ludus_tessera. Canon paths take a `radix` prefix (tests pass
RHUBARB_RADIX). Find: `pictor_documentum.h` included `fenestra.h` only
for the `TabulaPixelorum` type, which dragged fenestra_macos.m and
Cocoa into EVERY pictor build - now `tabula_pixelorum.h`; the terminal
binary links no Cocoa (otool), root pictor tests unchanged.

## 2026-10-03 — responsive layout (module 013 B3)

`pictor_componere` lays out with `dispositio` in cells from the
`superficies_*` attributes (written by the dispensator on resize):
viewport GROW over a ONE-row status (Fran). Surface-touching edges snap
to the surface (no sliver in a window that is not a cell multiple).
figura_tituli's text is now vertically centred against the 8-px glyph
(it was clipped at y = 2 in the 8-px row - the image specimen caught
it). `PictorCompositio.status_altitudo` → `cellula_*` + `status_lineae`.
