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
