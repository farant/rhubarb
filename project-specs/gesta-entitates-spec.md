# gesta — `entities/` per-entity projection (spec)

Date: 2026-07-18
Status: design approved, ready for implementation plan
Area: gesta / tabularium (`gesta/fontes/tabularium.c`)

## Context

The tabularium keeps its truth in an append-only event log,
`gesta/annales/tabularium.jsonl`. Everything else is a *projection* of that
log — the doctrine calls this "systema plagularum = proiectio" (the file
system as projection), first recorded as desideratum seq 45.

Two projections exist today:

- the **SQLite `.db`** — what the MCP tools query (`quaerere`, `census`, `res`…);
- **`tabula.md`** (`gesta/annales/tabula.md`) — a single generated markdown
  file, regenerated on every write by `_tabulam_scribere()`, committed next to
  the jsonl. It shows **open items only**, one line each (`- [status] title \`tags\``),
  under sections QUAESTIONES / PARCA / DESIDERATA / DECRETA / OPERA / processus /
  rami / nexus. It is content-deterministic (header keyed to last event's
  seq+creatum, no wall clock) so git diffs stay honest.

This spec adds a **third projection**: one markdown file per entity, organized
into tag directories, each file holding the entity's full body. Its job is
**browsing and reading** — wander the board as a folder tree and read any
entity in full, in an editor or in saltuarius. It is the *archive* that
`tabula.md` (a one-glance dashboard of open items) cannot be.

## Decisions

All four were chosen deliberately during brainstorming (2026-07-18):

1. **Purpose: browse/read as files.** The tree is meant to be wandered and
   read, not queried by tools (the `.db` already serves querying). This is the
   north star that settles every other choice.

2. **Multi-tag handling: duplicate into every tag folder.** An entity with
   tags `silva,examen,mcp` gets a full copy under each of `silva/`, `examen/`,
   `mcp/`. Chosen over symlinks (git fragility), first-tag-only (loses
   cross-tag discovery while wandering), and flat+index (two-hop browsing).
   Duplication is free to maintain because these are *generated* files — the
   generator emits all copies from the one truth; there is no hand-sync. The
   only cost is diff-noise, acceptable for a read-oriented view.

3. **Scope: everything (full archive), flat.** Open *and* terminal entities
   (`clausum` / `relictum` / `impletum`) each get files, sitting flat inside
   their tag folders (closed items are **not** segregated into a subfolder).
   This is the value `tabula.md` cannot give: wander resolved decreta, read a
   closed question's whole arc. The flat/everything choice also simplifies the
   generator — a status change never *moves* a file, it only rewrites content.

4. **Regeneration: live on every write, incremental per-entity.** The tree
   updates on the same write path as `tabula.md`, but — unlike `tabula.md`'s
   wipe-and-rewrite of one file — only the **one entity a write touched** has
   its files rewritten. Wiping and rewriting ~200+ files on every small write
   would be wasteful; incremental is correct here.

## Layout & filenames

Root: `gesta/annales/entities/` (sits with the other projections).
**Gitignored** (decided 2026-07-20) — like the `.db`, it is a rebuildable
artifact regenerated on every write, present on disk for browsing but not
versioned; `tabula.md` carries the committed summary/diff view. See the worklog.

```
gesta/annales/entities/
  rami/
    parcum-rami-copia-plicaturae-01KXPRZ6JG0T3BXQ23PF0RNB59.md
  gesta/
    parcum-rami-copia-plicaturae-01KXPRZ6JG0T3BXQ23PF0RNB59.md   (copy)
  silva/
    quaestio-qc-pseudos-01KX....md
    parcum-annotatio-visio-01KX....md
  examen/
    quaestio-qc-pseudos-01KX....md                                (copy)
```

Filename = `<genus>-<title-slug>-<ULID>.md`:

- `<genus>` — the entity type (`parcum`, `quaestio`, `decretum`, `nota`,
  `desideratum`, `opus`, `ramus`, `nexus`).
- `<title-slug>` — the title lowercased, non-alphanumerics collapsed to `-`,
  truncated (~40 chars). Purely for human readability in a file listing.
- `<ULID>` — the full 26-char entity id, **at the end**, so that a glob of
  `*-<ULID>.md` reliably locates every copy of an entity even after its title
  (and therefore its slug) changes. The ULID, not the slug, is the identity.

Entities with **no tags** land in a reserved `entities/_sine_tag/` folder so
they are still reachable (they would otherwise vanish from the tree).

## File body

Full render — this is the archive, so the whole entity, notes chronological.

```markdown
---
id: 01KXPRZ6JG0T3BXQ23PF0RNB59
genus: parcum
status: parcatum
tags: rami, gesta
creatum: 2026-07-17T05:24:21.182Z
---

# Rami: copia plicaturae ramalis

<corpus — the entity body text>

## Status
- creatum — 2026-07-17T05:24:21.182Z
- tractum — 2026-07-18T09:14:02.001Z

## Notae
- 2026-07-17 (claude): <note text, oldest first>
- 2026-07-18 (fran): <note text>

## Nexus
- impedit → [K5 palatium](../palace/parcum-k5-palatium-01KXP9XNE4....md)

## Ancorae
- symbolum `ramus_fundere` — gesta/fontes/tabularium.c
- via `gesta/annales/tabularium.jsonl`
```

Details:

- **Frontmatter keys are Latin** (`id`, `genus`, `status`, `tags`, `creatum`)
  — on-brand and greppable. Nothing consumes them yet; they exist for the
  reader and for future file-tools. (As shipped, `actor` is not in the
  frontmatter; note authors appear per-line in the Notae section.)
- **Content is deterministic.** Every timestamp shown comes from event data in
  the log, never from a regeneration clock. Two regenerations of an unchanged
  entity produce byte-identical files (no spurious rewrites) — same discipline
  as `tabula.md`.
- **Nexus rendered as relative markdown links** to the linked entities' files,
  turning the tree into navigable hypertext (click from a park to the question
  it blocks). Because every target is duplicated across its tags, the link
  points at the target's **first-tag** copy, chosen deterministically. A link
  whose target has no file (should not happen for live links) degrades to plain
  text, never a broken path.
- **Ancorae** (`[{genus:symbolum|via, scopus, sigillum}]`) render as a list.

## Regeneration mechanism

A new internal function in `gesta/fontes/tabularium.c`:

```
interior vacuum _entitatem_reconciliare(Tabularium* t, constans character* res_id, Piscina* pn);
```

Called from the same write endpoints that already call `_tabulam_scribere`
(the ~12 callsites: addere, gerere note/status/mutatio/nexus/remotio, agere,
ramus ops). Each mutating operation already knows the affected `res_id`
(`nexus` touches two — reconcile both).

Per call, **reconcile one entity against the filesystem**:

1. Load the entity's current state from the store (genus, status, tags, corpus,
   notes, links, anchors, provenance). If the entity no longer exists (removal),
   its desired set is empty.
2. Compute the **desired file set**: one path `entities/<tag>/<genus>-<slug>-<id>.md`
   per current tag (or `_sine_tag/` if none).
3. Find **existing** files for this id by scanning the tag subfolders of
   `entities/` for a file whose name ends in `-<id>.md`.
4. **Write** every desired file (idempotent overwrite with deterministic
   content), creating tag folders as needed.
5. **Delete** every existing file not in the desired set (stale tag folders,
   renamed slug, removed entity).

The filesystem is the record of the previous tag set, so create / note /
status-change / retag / removal all collapse into this one idempotent
operation — no separate bookkeeping of "old tags". Empty tag folders left after
a delete are removed.

Cost per write: load one entity + 1–3 file writes + a directory scan of the
tag folders. Proportionate to `tabula.md`'s existing per-write full DB scan.

## Edge cases

- **Retag** — old-tag file deleted, new-tag file created, shared-tag file
  rewritten (content changed). Handled by reconcile diff (steps 4–5).
- **Title change** — slug changes → filename changes; old file (matched by
  `-<id>.md`) deleted, new written. ULID-suffix match makes this reliable.
- **Removal** — desired set empty → all copies deleted.
- **No tags** — routed to `_sine_tag/`.
- **First run / rebuild** — reconcile is per-entity; a full rebuild
  (`frigida.sh` cold path, or a one-shot) iterates all entities and reconciles
  each. The cold rebuild should first clear `entities/` to purge anything stale
  from before the feature existed.
- **Slug collisions** — two entities with identical slugs are still distinct
  files (different ULID suffix); no collision.

## Non-goals (YAGNI)

- **Not** a queryable store — the `.db` owns querying; these files are for reading.
- **Not** editable — like `tabula.md`, hand-edits are overwritten; corrections
  go through the store (a note), not the file.
- **No** tag hierarchy — flat tag folders, matching the store's flat tags.
- **Does not replace** `tabula.md` or the `.db`; it is a third projection.
- **No** frontmatter-consuming tooling in this pass (keys exist for the future).

## Testing

Extend `gesta/probationes/probatio_tabularium.c` (which already tests
`tabula.md` projection), pointing the entities root at a `gesta/build/`
scratch dir. Cases:

- create with N tags → N files exist, each with full body + frontmatter;
- add note → same files, note present, other content unchanged;
- status change to terminal → files still present (flat archive), status line
  updated, no move;
- retag (drop one, add one) → old-tag file gone, new-tag file present,
  shared-tag file updated;
- title change → old-slug filename gone, new-slug filename present, same id;
- removal → all copies gone, emptied tag folders pruned;
- no-tag entity → lands in `_sine_tag/`;
- determinism → reconcile twice, second run leaves bytes (and mtimes-of-content)
  identical / no spurious rewrites;
- nexus link → renders a relative path that resolves to an existing file.

## Relationship to existing code

- Mirrors `_tabulam_scribere` (`tabularium.c:1148`) in spirit and callsite set;
  reuses `ChordaAedificator`, `scrinium_*` queries, `json_*` for tag/anchor
  extraction, `piscina` for scratch.
- Config: add an entities-root field to the `Tabularium` config next to
  `via_tabulae` (`tabularium.h:225`); `NIHIL` disables the projection (tests,
  cold tools that don't want it).
- Cold path `gesta/frigida.sh` gains an `entities/` clear+rebuild step.
```
