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
