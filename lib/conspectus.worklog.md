# conspectus worklog

## 2026-10-21 — D4a: the library (fixture only)

Spec `project-specs/proiectiones.md` (R5, c1f8dc2a). Header approved by
Fran, then amended with his OK during implementation:

- **Cursor instead of historia observers (spec §V amended).** Observers
  only see the `Historia` instances the registry was handed; a day's
  notebook edited in a scriba pane goes through that pane's own
  instance. The registry keeps a seq cursor into the volume and, before
  answering, reads the acts after it and maps each act's genus (or a
  plagula act's `via`, parsed from its JSON datum) onto the sources.
  Sees every writer; undo/redo arrive as `<spatium>/cursor` acts.
- **Documents are identified by SPACE.** In the app agenda opens with
  space "agenda" (`lib/vicus_applicatio.c:1670`), so every act of it,
  notes included, is `agenda/...`. Matching = prefix + '/', so
  "personaealia/x" never touches "personae" (tested).
- **per-rem needs a fingerprint (`vestigium`).** Found while writing the
  memory test: a rename changes no keys, so a keys-only diff left the
  day showing the old title. The library stores a 64-bit FNV-1a of each
  item's fingerprint (`chorda_friare` is only 32 bits: a collision =
  a silent missed invalidation) plus its keys, in a per-source arena
  rebuilt from scratch on every diff (bounded by live items, never by
  history). D4b: the fingerprint should be agenda's own canonical item
  encoding, not hand-picked fields.
- **Arena freed at recompute, not at invalidation**: a pointer handed out
  stays valid until the next read OF THE SAME address finds it stale (or
  the cap evicts it) - catch-up during another read cannot free it.
- `registrum` is a latina.h macro (`register`) - parameters are
  `repertorium`; the type stays `ConspectusRegistrum`.
- `organum_typum_iudicare` checks `dies` by FORM only (2026-13-45
  passes): a projection's `computare` must return NIHIL for an
  impossible key (the fixture does).
- `versio` is parsed and unused until persistence: the cache is memory
  only and starts empty.

**Memory test design (measured).** Process footprint (`vestigium`,
memoria_processus) over 5,000 renames with a read after each: the FIRST
such phase grows ~4 MiB (allocator warm-up), a second identical phase
~0.5-0.7 MiB. So the test runs A (no reads), B1 (warm-up), then asserts
B2 < 4 MiB. The plant "per-rem memory never freed" costs ~1.5 KiB per
diff. `i64` is unsigned: footprint growth is clamped at zero before the
comparison.

**Plants** (silva.planta, filtrum probatio_conspectus): see the commit
message for the list and verdicts.

Not plantable in D4a: "f keeps a pointer instead of copying" - agenda
titles are INTERNED (immutable), so a borrowed title never changes under
a cached view; the copy rule protects against mutable containers (Xar of
rubrics, notes), which D4b's real `dies` will hold. "Version bump keeps
the cache" - no persistence yet.

## 2026-10-21 — D4b: conspectus 'dies' for the calendar

**Correction to the D4a entry above.** "agenda titles are INTERNED
(immutable)" was WRONG: agenda copies titles into its `status` arena
(`lib/agenda.c` `chorda_transcribere(..., doc->status)`), and that arena
is EMPTIED on every re-projection (`piscina_vacare(doc->status)` in the
projection's vacare - undo and redo). A view model that borrowed a title
would read reused memory after an unrelated undo. So the copy rule is
testable after all: `probatio_conspectus_dies` "copia" opens agenda with
checkpoint interval II and creates the OTHER item first, so undo restores
from a checkpoint that writes that item's longer final title first and
every later title moves; plant "titulus non copiatus" is red. (A plain
replay from scratch re-allocates in history order - same addresses - and
would hide the bug.)

**Measured** (`tools/kalendarium_mensura.c`, 50 items, 7 day books, 200
paints; real day books in paginae-proba carry at most 41 acts per day):

| notebook edits/day | week before | week after | month before | month after |
|---|---|---|---|---|
| 10 | 0.77 ms | 0.033 ms | 0.05 ms | 0.143 ms |
| 200 | 3.25 ms | 0.042 ms | 0.05 ms | 0.148 ms |

Week: 95 % of the old cost was re-opening 7 notebooks per paint. Month
got SLOWER: 42 cells = 42 `conspectus_legere`, each catching up. First
version 0.28 ms (a full `volumen_acta_legere` per read); a `max(seq)`
pre-check in `conspectus_renovare` (nothing new -> one indexed query)
brought it to ~0.145 ms. The rest is that one query per read. A cheap
change counter on the volume (no SQL) would remove it - not built (needs
a volumen.h API), noted for D4c where the host can catch up once per
frame.

**The early return must vacate the scratch arena** - first draft did
not: every read with no new acts leaked its key strings. Memory phase C
(500,000 reads, no acts) measures 0 KiB, bound 1 MiB. At 50,000 reads the
plant stayed GREEN: the leaked strings landed in blocks the arena already
held, so the footprint did not move until the reads outgrew them.

**`piscina_vacare` is not free.** It memsets EVERY block the arena holds.
After one big catch-up (thousands of acts) the scratch arena kept its
blocks, and every later read paid for zeroing them: ~30 us per read
instead of ~1. `tempus_renovare` now DESTROYS and recreates the scratch
arena at the end of `renovare` and `parare` (the heavy paths); the cheap
early return still vacates (its arena is small). General lesson: an
arena that is vacated on a hot path must not be allowed to keep the
high-water mark of a rare heavy path.

**Wiring.** The registry lives in kalendarium's PRIVATE paint context
(`KalendariumFigurae`: `diei`, `conspectus`), created in
`kalendarium_montare` when a volume AND an agenda document are given;
otherwise `conspectus_dies_computare` is called directly (same pure f,
no cache) - `kalendarium_montare`'s signature and every existing test
unchanged. Moving the registry to vicus (one per volume) is D4c.
`conspectus_dies_registrare` does NOT call `conspectus_parare`: the
caller prepares once after registering everything (a second projection
registered later would otherwise reset the cursor and skip acts).

Not plantable: "kalendarium never creates the registry" - output is
identical by construction (same f); only the benchmark sees it.
