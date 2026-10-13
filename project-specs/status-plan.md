# status — plan (widget state model: manifest, categories, nesting)

*Written 2026-10-10 in secunda, approved by Fran the same day. Origin:
designing the agenda/calendar (project-specs/agenda-plan.md) exposed
that live widgets classify state inconsistently. The census, the
categories and the design reasoning live in
project-specs/status-genera-census.md (sections III-VIII) - read it
first.*

*Discipline: vertical slices, each committed with its gates, existing
suites proving no behaviour change except the intended one; red-first
with plants; header and manifest text shown to Fran before coding.
HAND-WRITTEN BEFORE GENERATED: no generator or lints until three
widgets are migrated by hand.*

## I. Decisions (Fran, 2026-10-10)

1. **Classify state properly**, not merely route it through the
   island: (1) shared data/entities, (2) widget state persisted across
   sessions, (3) truly transient widget state. Kinetic is a write
   RHYTHM orthogonal to lifetime (census III).
2. **Shape B: one widget manifest** (STML) per widget, the canon as
   one section (canon infixus), plus `ligamina` (bindings = props),
   `portae` (observation ports), `actiones`, `status` (one `campus` per
   field: vita, rhythmus, absens, scriptor, referentia, orbatus) and
   `montationes` (child slots). Its own canon (`organum.canon`, name
   TBD) checks cross-section citations; loader/lint checks the rest.
   Replaces durabilis.canon + ephemera.canon + domini.stml.
3. **Any widget can mount any widget**: one store, nested slices
   (Redux's real shape), branches addressed by path at any depth.
4. Iterate as dogfooding teaches; raise state-model friction
   explicitly.
5. **The island canon is GENERATED from the campus list** (census IX):
   one definition.
6. **Systems over aspects** (census X): `scriptor` = one writer for
   VALUE fields; cross-cutting behaviour (persistence, kinetic,
   reference, popover, ...) = a framework system over an aspect named
   on the field. An aspect is admitted when a second widget needs it.
7. **A hierarchy of states** (census XI): a general model everything
   satisfies + specialised models for proven categories; state whose
   category is not understood stays in the general model.

## II. Arcs and slices

### Arc I — vocabulary, proven on pictor

- **S0** DONE (2026-10-10): pictor's manifest by hand -
  project-specs/status-s0-pictor.stml; findings in census IX.
- **S1** DONE 2026-10-10 (33c7c859; lib/organum.worklog.md). `organum.canon` + loader library: manifest -> GENERATED
  island canon(s), owner list, field table (with aspects). Tests: valid pictor manifest; each citation
  failure (unknown attribute, undeclared action, unknown binding);
  completeness (attribute without campus).
- **S2** DONE 2026-10-10 (lib/organum.worklog.md). pictor loads from its manifest; its three files deleted.
  Behaviour identical (suites). Two-island split kept.
- **S3** DONE 2026-10-10 (lib/conservatio.worklog.md; census XII: it BUILT persistence - islands never reached disk before). The PERSISTENCE system: transient fields never reach disk (palette open -> quit ->
  reopen -> closed); in-process rehydration keeps them. Starts by
  reading how islands are saved.
- **S4** DONE 2026-10-10 (lib/organum.worklog.md: starting values; reading fallbacks wait for S9). defaults from the manifest (`absens`), replacing C
  hard-codes and `nota` prose; test-only fields `vita="probatio"`.
- **S5a** DONE 2026-10-10: system writers in the owner table
  (`InsulaDominus.systemata`; lib/insula.worklog.md). Fran: S5 split
  a/b/c; undo after an orphan repair is LOSSY for now (selection does
  not return to the restored entity - remembered repair is a follow-up).
- **S5b** DONE 2026-10-10 (lib/relatio.worklog.md). the REFERENCE system (historia observer + entity enumeration
  registered by the document library + repair as system 'relatio'): `referentia` + `orbatus` checked
  after every document act and undo; replaces `stratum_currens`.
- **S5c** DONE 2026-10-10 (lib/dimissio.worklog.md; Esc yields to an in-flight gesture). the POPOVER system: `aspectus="popover"` fields closed by the
  dispatcher (Esc, outside click, any other act, another popover
  opening); pictor's seven `palettam_claudere` calls disappear.
  Second consumer: the calendar's day popups.
- **Decision point**: collapse to one island per widget ("durable" =
  the binding only)?

### Arc II — scriba and vicus

- **S6a** DONE 2026-10-12: shared mount module organum_montatio
  (lib/organum.worklog.md). Fran: S6 split a/b/c; scriba persists cursor
  + page, mode/selection transient; yank register (capsa) per pane +
  transient in S6b, app-wide in S6c/S7.
- **S6b** DONE 2026-10-12: scriba on its manifest; cursor persists;
  pagina_positio/paginae_numerus kept as derived transient copies
  (removal needs the book in ScribaCompositio).
- **S6** scriba: `clavis_praecedens`, `fd_*`, `nuntius` -> transient;
  `paginae_numerus` read from the book; the yank register = first
  app-wide non-document state.
- **S7** vicus: `horologium_dies` into the island (transient);
  battery as an observation port; the layout index file migrated or
  explicitly classified. Fran 10-12: S6c (app-wide yank register)
  folded in; split a/b.
- **S7a** DONE 2026-10-12: host manifest apps/vicus/organum.stml for the
  root (activa/prior = transient copies of vicus/latera, owner
  vicus.radix; horologium_dies transient; clock + battery readings
  tempus_*/pila_* written by system `horologium`, absent = none).
  Struct copies gone; hand canon deleted; organum_ramum_parare on the
  root sets the repository root canon. Layout index stays the host's
  persisted structure (classified, not migrated).
- **S7b** DONE 2026-10-12: app-wide `capsa` on the root, persisted
  (viminfo-like) - first persisted app-wide state. The parent hands
  scriba a branch (`ScribaActiones.ramus_capsae`, NIHIL repo = own
  branch); vicus owns its systems (`vicus_systemata_ponere`, created
  in vicus_aperire, root restored under clavis "vicus");
  systemata_parare deleted. Host manifest declares the child's action
  `pagina.clavis` as the register's owner (manifest checker requires
  declared writers) - a second pane kind wanting the register makes
  the owner a list or a system. The register is written only when it
  changes. Found: our vim had no `y` - added right after (yy, Y,
  visual y; lib/vim.worklog.md).

### Arc III — depth

- **S8** branches as paths at any depth; focus, dispatcher, kinetic
  slot and registry route by path. Test: a toy widget three levels
  deep. Re-measure the store-copy cost (terminal-planning park 010).

### Arc IV — generation and lints (after three hand migrations)

- **S9** typed accessors generated from manifests (GENERATUM,
  committed).
- **S10** silva lints: no string-named island access; widget context
  structs hold only ports/config/caches; transient never on the disk
  path; L4 generalised.

### The calendar, interleaved (agenda-plan.md)

- **A1** (agenda library) independent - any time.
- **A2-A5** (views) after Arc I.
- **A6** (embedded day scriba) after Arc III - the depth-2 consumer.

## III. AUDIENDA

- How islands reach disk today (S3 reads it first).
- Whether app-wide non-document state (yank register, vicus layout,
  agenda pointer) needs its own declaration form (S6/S7).
- Store-copy cost with nesting (S8).
- Root element name of the manifest (`organum` is a placeholder).

### Note (Fran, 2026-10-10): persistent STML trees as a library primitive

The insula write gate copies the WHOLE store per write (serialize,
re-parse, full canon, serialize again for rehydration - park 010:
13.6 us at 2 tabs, 52.7 us at 11). The general fix belongs in the
STML layer (or materia - check first), not in insula: a persistent
tree with structural sharing (path copying; old root stays valid;
"unchanged" = pointer identity, so owner checks, canon judging and the
persistence system skip untouched subtrees). Real design problem:
memory - arenas plus sharing; candidate = compaction at quiet (copy
the live tree into a fresh arena). Read project-specs/stml-visio.md
and check materia before designing. Timing: S8 (nesting grows the
store; re-measure park 010). The one-island collapse makes it simpler
(one tree, one path).

### One island per widget (Fran, 2026-10-10: collapse)

Decided after Arc I. Reordered on contact (canon merging avoided):
- **C1** DONE 2026-10-10: durable island drained - pictor/scriba sizes
  moved to ephemera; durable branches carry only `id`.
- **C2** DONE 2026-10-12 (lib/insula.worklog.md): genera removed
  from insula (one tree, `insula_mutare`); durable canons deleted.
- **C3** folded into C2 (forced): saved state versio 2 (fields on
  <status>), versio 1 migrated by merging its two children.
