# mandatum.worklog.md

## 2026-09-04 — natus (ludus T2)

Six draw primitives + coetus spans (`magnitudo_arboris`) in a flat
xar; STML round-trip; structural equality. Design: pictor-spec.md §3.2,
ludus-brainstorm.md §XV. Coetus carries clip/translate/integer-scale as
PROPERTIES — no push/pop commands exist, so mismatch is structurally
impossible. No id/actio/partes on any primitive: those live on the
componens (T3); coetus carries only an optional `provenientia` for the
inspector, which targeting never reads. Serialization internals take
`constans Mandata*` (a `-Wcast-qual` lesson from drafting the plan).
Probatio: 16 assertions incl. a nested coetus round-trip and a planted
inequality. Examen ACCIPE on both files at birth.
