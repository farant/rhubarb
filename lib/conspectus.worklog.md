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

## 2026-10-21 — D4c: one registry per volume, owned by the host

**D4b broke its own header.** `conspectus.h` says "registrum unum per
volumen, lateribus omnibus commune", but kalendarium created a registry
per MOUNT: the calendar pane, the agenda pane (also a kalendarium) and
any `_2` each caught up and diffed agenda on their own, and since the
registry had no release, every mount leaked its malloc'd arenas
(scratch, one per cached day, per-rem memory). Now the host owns it:
vicus `ContextusKalendarii` creates one (lazily, with the shared agenda
document) and hands it to every calendar pane via
`kalendarium_conspectum_ponere`; kalendarium never creates one.
`conspectus_registrum_destruere` frees what the registry owns.

**Measured before choosing the counter** (month paint, 42 reads):
prepare `max(seq)` per read 0.14 ms; keep the statement 0.07 ms; no
check at all 0.03 ms. The statement must be finalized before the
connection closes - `scrinium_claudere` uses `sqlite3_close`, which
returns BUSY with a live statement and leaves the connection (and its
memory) behind. So it lives in `volumen` (`volumen_seq_ultimum`,
finalized in `volumen_claudere`), not in the registry. Also: the kept
statement MUST be reset after the row is read - left mid-step it holds
a read snapshot and returns DONE next time (plant "sine retexere" red).

**Composition (spec §VI.1/3) parked, not built:** vicus repaints whole
frames; in-process writes come with input events that already repaint;
other processes' writes never reach an open agenda document (it folds
once at open), so demand would recompute from stale state. Needs
documents that follow the journal first.

Plants (silva.planta, all red): volumen - no reset, close doesn't
finalize (memory loop, 500 open/close cycles: +368 KiB green); conspectus -
destroy no-op, entries kept, per-rem memory kept, scratch kept (300
registries: +192 KiB green, bound 1 MiB); kalendarium - setter ignores
the registry. Not plantable without a test accessor: "vicus does not
hand its registry to the pane" (output identical by construction).

## 2026-10-21 — D5 S1: searchable declarations, observer, enumerare

`<quaerendum genus="..."/>` (canon, at most once) marks a declaration
searchable; `conspectus_enumerationem_ponere` (parare refuses a
searchable one without it), `conspectus_quaerenda`,
`conspectus_enumerare`, and a per-declaration observer
(`conspectus_auditorem_ponere`) called from the TWO places every stale
key already passes through: `clavem_irritare` (spatium, per-rem) and
`declarationem_irritare` (omnia -> empty key). It hears keys nobody
has read (the cache lookup comes after). Plants red: observer only
when the declaration has cache entries; never for keys; never for
"all"; parare without the enumerare check; quaerenda listing every
declaration.

**D4c's memory phase D was FLAKY** (300 registries after 20 warm-up,
bound 1 MiB): four clean runs gave 0, 1056, 1024, 1552 KiB. Allocator
noise stays ~1.5 MiB whatever the count; leaks grow with it. Now 2000
registries x 3 reads after 100 warm-up: clean 320-1104 KiB; plants
destroy-noop 33 MiB, entries 19, scratch 8.2, per-rem 6.1; bound 3 MiB.
Lesson: calibrate a footprint bound with the plants' SIZES, not just
their colour, and run the clean case five times first.

## 2026-10-23 — conspectus 'dies': working memory out of the entry

`conspectus_dies_computare` did all its work in the entry's pool: the
liturgy lookup, the birthday query, the agenda lists and above all the
day-notes read, which replays the page's whole edit history. The entry
kept only a few strings, but the pool kept everything. Measured on
Fran's volume (copy, 10-23): ~124 KB per day with a notes book, 540 KB
for one busy day; the October grid (42 days) reached 4.1 MB and the
4 MiB ceiling evicted the first 14 days on every frame - so each mouse
move recomputed them (replays included), and the month painter, which
held pointers into evicted entries, drew garbage (fixed separately in
kalendarium, A2b copy, d68eebf3).

Fix: a scratch pool per computation (`conspectus_dies_scheda`, created
and destroyed inside the call - safe if computations ever nest); only
`v`, its Xars and the transcribed strings go to the entry pool. The
existing chorda_transcribe calls were right in intent but copied into
the SAME pool. Result: test day with 50 edits 178,509 B -> 929 B;
Fran's grid 4.1 MB -> 37.8 KB, nothing evicted; the 14th 540 KB ->
924 B.

Test `magnitudo` (probatio_conspectus_dies): 50 edits, entry < 8 KB,
content = last edit. Plants: book read in the entry pool -> red (size);
lines not transcribed -> red (conspectus_verificare). Liturgy lookup in
the entry pool stayed GREEN: +475 B fixed (1404 vs 929), not
history-proportional; a 1 KB bound would be fragile, the 8 KB bound
exists for replay-scale growth. Kept in scratch anyway.

Open design note: the next projection kind can repeat this mistake.
Structural option, not built: the registry hands `computare` a scratch
pool next to the result pool (API change in conspectus), or
conspectus_verificare flags entries above a size bound.

## 2026-10-27 - own-replica rule (communio plan-2 T1)

`conspectus_renovare` read EVERY act after its cursor. Once a volume imports
other devices' acts (communio), a foreign act whose genus (or plagula via) falls
under a local document or `dies/<k>` spatium marked local views stale - and
for `omnia` sources invalidated whole declarations - though nothing local
changed. Now it reads `volumen_acta_propria_legere` when the volume has a
replica, and sets the cursor to `volumen_seq_ultimum` taken before the read
(foreign acts are never returned, so without this the cheap "nothing new"
check would never short-circuit again). Test: probatio_conspectus "acta
aliena" (RED: day 19 STALUM from a foreign act).
