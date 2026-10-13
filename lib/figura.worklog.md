# figura.worklog.md

## 2026-09-05 — natus (ludus P3 T2)

The drawing half of the pipeline begins here, still headless. A
figura is keyed by (partes, thema) in a registry and emits
primitives in its OWN space; `pingere` walks the logical tree,
opens one coetus per componens (fines, sectio, translatio, scala,
provenientia = the componens id), calls the figura for that role
if one is registered, then the children — so a parent paints under
its children, which is the z-order the targeting strategy already
assumes. The two-registry rule: a componens kind and a figura are
two entries, never one type; a componens carries no function
pointer, a figura reads only the componens it is handed (brainstorm
XVI §5, spec §2.1). Thema is an `i32` id here and a palette in the
rasterizer: a figura names colours as `ColorMandati`, never as
pixels. `figura_finium` is the fallback nobody registers, and the
probatio's second thema. Duplicate (partes, thema) is refused; a
NIHIL fn is refused.

The probatio's pannus carries a horizontal AND a vertical
translation (10,50), applying T1's lesson about mute plants: a
fault in either term now shows.

Gate: `probatio_figura` (43). Planted fault: figura called AFTER the
children. The first plant was MUTE — no node in the draft test had
both a figura and children, so the order could not show. The pannus
became a dialogus with `figura_finium` registered for it, and the
plant went red at the pannus-rectangle index, green on revert.
Second mute plant in two tasks: a plant must touch a value the test
actually varies. `<purus/>` anchors on `pingere` and `figura_finium`
for lint L2. Examen ACCIPE; formator 0 after two long lines.

## 2026-10-13 — figura_mensam_pingere, FIGURA_EXEMPLAR_FOCI

The "mensa" (pane surface in COLOR_SUPERFICIES, focus dots
EXEMPLAR_PUNCTA_DUPLICIA_DISPERSA in COLOR_BORDER over it when focused)
was painted by hand in scriba_figurae, pictor_figurae and kalendarium -
three copies of the macro and the two rectangles. Now one helper here;
figura.h includes thema.h and exemplaria.h for it (figura.c already
included thema.h). Emission order is part of the contract: surface
first, pattern second (probatio_figura asserts both, by index). The
focus decision itself lives in motus.h (`motus_ramus_focatus`) - this
helper takes a plain b32 so figures stay pure.
Plant: pattern never drawn -> red in probatio_vicus_latera (scriba/
pictor panes) and probatio_kalendarium.

## 2026-10-13 — figura_color_thematis (5 copies -> 1)

`color_thematis(c)` (theme colour -> ColorMandati of genus
COLOR_MANDATI_THEMA) was a private copy in scriba_figurae, pictor_
figurae, kalendarium (taking i32), terminale and vicus. Now
`figura_color_thematis` here; ~70 call sites renamed with
`./silva/renominare.sh -via <file> -scribere`.

Mechanics worth remembering: renominare REFUSES when the new name is
already visible ("collisio: nomen novum iam registratum") - correct, the
header declaration collides with each file's static copy. Sequence that
works: remove the declaration from the header, rename per file (local
definition + calls), restore the header, then delete the now-duplicate
static definitions. `color_thematis_index` in pictor_figurae is a
different function (palette index) and stays.
Plant: valor ignored -> red in probatio_figura, probatio_pictor_figurae,
probatio_scriba_figurae (pixel checks), probatio_kalendarium.
