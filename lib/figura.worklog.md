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
