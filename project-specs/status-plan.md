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

## II. Arcs and slices

### Arc I — vocabulary, proven on pictor

- **S0** pictor's manifest written by hand (doc only; Fran reviews) -
  find the missing words.
- **S1** `organum.canon` + loader library: manifest -> island canon(s),
  owner list, field table. Tests: valid pictor manifest; each citation
  failure (unknown attribute, undeclared action, unknown binding);
  completeness (attribute without campus).
- **S2** pictor loads from its manifest; its three files deleted.
  Behaviour identical (suites). Two-island split kept.
- **S3** transient fields never reach disk (palette open -> quit ->
  reopen -> closed); in-process rehydration keeps them. Starts by
  reading how islands are saved.
- **S4** defaults from the manifest (`absens`), replacing C
  hard-codes and `nota` prose; test-only fields `vita="probatio"`.
- **S5** references with fallback (`referentia` + `orbatus`) checked
  after every document act and undo; replaces `stratum_currens`.
- **Decision point**: collapse to one island per widget ("durable" =
  the binding only)?

### Arc II — scriba and vicus

- **S6** scriba: `clavis_praecedens`, `fd_*`, `nuntius` -> transient;
  `paginae_numerus` read from the book; the yank register = first
  app-wide non-document state.
- **S7** vicus: `horologium_dies` into the island (transient);
  battery as an observation port; the layout index file migrated or
  explicitly classified.

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
