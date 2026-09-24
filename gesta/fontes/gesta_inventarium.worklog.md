# gesta_inventarium.worklog.md

## 2026-09-23 — natus (inventarium v1 T1)

The pure half of inventories (spec project-specs/inventarium-spec.md,
decision …FTB8XR3). An inventory's table lives in its entity's folded
state, built by four event kinds — `ordo-additus`, `ordo-remotus`,
`lens-addita`, `cella-posita`. This module folds one such event into the
state object and validates one against the current state. No I/O, no
scrinium: gesta's fold (`_statum_transformare`) and its write-time
validation call in (T2), the MCP tool composes the events (T3).

State shape: `ordines` [{clavis, additus, actor}] in order of addition;
`lentes` [{nomen, genus_valoris, corpus?}]; `cellae` {clavis: {lens: {genus,
valor, actor, creatum, fons, per?}}}. Values are TAGGED (genus + valor)
so later kinds need no history rewrite. Provenance comes from the event
itself (actor, creatum) plus `fons` (manu | derivatum) and optional `per`.

The fold is DEFENSIVE (a duplicate row or a cell for a missing row is
skipped, never an error): folding happens on replay too, and replay must
never fail. The validator is the strict side, at write time, and names
every cause at once. Whether the target entity IS an inventarium is not
visible here (genus is a column, not state) — T2 checks it.
