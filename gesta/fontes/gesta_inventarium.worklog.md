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

## 2026-09-23 — wired into gesta (inventarium v1 T2)

`_statum_transformare` delegates the four kinds to
`gesta_inventarium_applicare`; `_eventum_validare` checks the target
exists and IS an inventarium, parses its current state (committed state
+ batch shadows, via `_res_validationis_capere`) and runs
`gesta_inventarium_validare`. Genus `inventarium` seeded (semen v10, no
state machine, like regio).

Doctrine correction to the plan: at the ENGINE, a violation does not
block — "iudicat, non obstat": the event is written and a custody note
("violatio inventarii: ...") joins it in the same batch, exactly like a
status-machine violation. Refusal-before-writing belongs at the
tabularium's DOOR (T3's tool pre-judges with the same validator, as
`_statum_praeiudicare`/`_nexum_praeiudicare` do). Consequence: an
inventory event written straight through gesta onto a non-inventory
still folds (rows appear in that entity's state) — with the note. The
door makes that unreachable for agents.

Limitation, stated: validation reads committed state plus shadows, and
shadows carry existence and genus, not an inventory's rows. So two
inventory events on the same entity in ONE batch (add rows, then set
cells for them) would judge the second against the pre-batch table. T3
writes one event per call, so it never arises; revisit if a batch ever
needs both.

Also: `SEMINA_NUMERUS` in tabularium.c was hand-counted (XVI) — adding
this seed would have required remembering to bump it (the same class as
the tool-schema miscount fixed in e6fdaaa5). Now derived from the array.
Tests: probatio_gesta gains the inventory section (fold, provenance,
custody notes for a missing row and for a non-inventory, replay
byte-identical); planted dead fold -> 4 red, permissive validator -> 1 red.

## 2026-09-23 — the door: MCP tool `inventarium` (T3)

`_tab_inventarium` in tabularium.c, actions `ordines` / `lens` / `cellae` /
`removere`. It PRE-JUDGES with the very validator the engine uses
(`gesta_inventarium_validare`), so door and engine can't drift: a bad
write is refused before anything is written, all causes named. Rows
already present are REPORTED and skipped ("iam praesentes N
(praetermissi)"), and a push with nothing new writes nothing ("nihil novi
- nihil scriptum") — a derivation can re-push its whole list. Rows come as
a JSON array or a comma list. `cellae` takes a JSON array of {ordo, lens,
valor:{genus, valor}}; `fons` defaults to `manu`.

Plants: pre-judgment off -> 5 red (and `res` then SHOWS the bad cell's
row key: the engine wrote it with a custody note — the door is what keeps
it out). Presence filter defeated -> 3 red. A first attempt at the second
plant removed the only call to the presence helper and broke the BUILD
(-Wunused-function); a plant that breaks the build proves nothing, so it
was redone as a helper that always answers "absent".
