# pictor_componentia.worklog.md

## 2026-09-05 — natus (ludus P3 T7)

Pictor's `componere`: four nodes of ONE record type, roles as data
(brainstorm §XVI §5). `radix` carries the root action
(`instrumentum.eligere`, where hotkeys bubble), `prospectus` is the
viewport (clips; translatio = the pan from `Motus`; scala = the
zoom from the ephemera island, floored at I), `tabula` is the
canvas (document size from the durabilis island, PROPRIUS,
focusable), `status` is a `PARTES_TITULUS` strip whose title is the
current tool. No per-componens struct, no function table: the
figura registry (T2) keys on the role.

Two decisions live here. The tabula's ACTION IS CHOSEN AT COMPOSE
TIME from `ephemera.instrumentum` (`pictor_actio_instrumenti`:
"penicillus" → "penicillus.ictus", unknown → none), so handlers
never branch on the tool — the name is a pure function of state
(spec §5.1). And the pending stroke is COPIED from `Motus` into
`tabula.puncta` (the field the polygon predicate already
serializes), so `pingere` and every figura read the tree alone
(decision 2026-09-05); the tier-3 snapshot therefore shows a stroke
in flight. `puncta` is assigned directly — `Componens` is a plain
record and has no setter; the dual use (polygon hit shape on a
non-PROPRIUS node, pending stroke on the PROPRIUS tabula) is named
here rather than given a second field.

The viewport size comes from the ctx (`PictorCompositio`); the
document size and the zoom from the islands. P4 hangs the palettes
as children of `radix` before `status`.

Gate: `probatio_pictor_componentia` (40); exemplar
`probationes/pictor/pictor.arbor.stml` born on the first run, read,
promoted — it shows the stroke in flight (`puncta="10,20 12,22"`)
and the pan as `translatio_x="-5"`, the signed serializer at work.
Planted fault: the stroke copy skipped — red at `numerus_punctorum`
and at the exemplar, green on revert. Examen ACCIPE; formator 0.
