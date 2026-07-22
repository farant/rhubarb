# Annotationes STML — spec v1 (Chunk 0 output)

*The identity + intention layer: one STML grammar for all comment
annotations. Park 01KY3D7EJP (complectitur 01KXTX7FA3, the
annotation/type vision — future type annotations are one more
vocabulary family inside this grammar). Interview:
annotationes-stml-interview.md. 2026-07-21.*

## 1. Purpose

Big-codebase rot that survives the house disciplines is internal:
maps, rationale, and refactors drifting from the tree. The cures
are IDENTITY (anchors that survive edits, carried in the source),
INTENTION (why, machine-visible), and later PROJECTION (docs as
derived views). All ride existing machinery: silva trivia
collection (TOLERA collector precedent), examen lint tiers
(TOLERA/IRRITUM precedent), stml reading (raw/line capture shipped
2026-07-20/21), tabularium anchors, ULID prefix resolution.

## 2. The grammar (DECISUS, interview 2026-07-21)

**2.1 Anchoring.** A comment whose first non-whitespace content is
an STML open tag is an annotation comment; the whole comment body
is handed to stml_legere. Everything else is prose, never scanned.
An ANCHORED comment that fails to parse is a LINT FINDING
(malformed annotation — well-formedness tier), never silently
reclassified as prose: typo'd annotations must clamare, not
vanish.

**2.2 Decoration pre-pass.** Before parsing, each continuation
line is stripped of leading whitespace + one '*' + one optional
space (the house block-comment decoration). Single-line
annotations need no pass.

**2.3 Attachment.** An annotation comment binds to the NEXT
top-level unit, skipping blank lines (EXSPECTA-PROXIMA precedent;
stacks — multiple annotation comments above one unit all bind to
it). No following unit → file-scope annotation.

**2.4 Identity.** Format = ULID, the tabularium's discipline —
ONE identity space for board res and code nodes; citation by
unambiguous prefix ≥6 chars anywhere.
- Standalone: `/* <nid v="01KY3A2FQ8XN4VJ7TT9M2CDE1R"/> */`
- Boolean mint request: `nid` as valueless attribute on ANY
  annotation element (`<intentio nid>`); the minting instrument
  fills it (STML_SPEC boolean-nid-on-format design).
- Identity is granted at authoring, never derived from content
  (haecceitas: names/positions are display; the nid is the thing).

**2.5 Vocabulary v1** (Latin; new families are vocabulary, never
grammar):
- `<nid/>` — bare identity (v= or boolean).
- `<intentio>` — rationale. Light register: line capture
  (`<intentio (>cur ...`). Heavy register: multi-line with
  children. Optional attributes: `nid`, `res="<board ULID
  prefix>"` (citation to a tabularium res). Optional children as
  it graduates: `<causa>`, `<tags>` (content `#tag` /
  `#domain::tag`, speculum-lingua §12 conventions).
- Existing families to migrate later (Chunk E): aedilis verbs,
  TOLERA, EXSPECTA pins.
- Reserved (later pulls): `&nomen;` named-singleton entities,
  selector-scoped presence contracts (QC-gated), type-annotation
  vocabulary (park 01KXTX7FA3).

**2.6 Known STML lanes apply**: `"` unrepresentable in attribute
values (quote-bearing content → text children / raw capture);
raw capture is single-line. Intentio content is Latin (house
comment rule).

## 3. Adoption + lint plan

**On-demand only at birth**: no required classes; mint where an
anchor wants to exist. Presence-lint ships VERDICT-NEUTRAL with
an EMPTY required-class list; classes added by decretum.

Lint tiers (Chunk C, examen codices — all precedented):
1. Well-formedness: anchored comment must parse; malformed = fires.
2. Uniqueness: nid collisions.
3. Rot, bidirectional (IRRITUM twin): reference to nonexistent nid
   = error; unreferenced nid = advisory note.
4. (later) Presence by class; substitution hints (advisory,
   speculum-lingua validator pattern — e.g. intentio containing
   "TODO" suggests a board desideratum).

## 4. Chunk ladder

- **0 CEREMONY — DONE** (this spec).
- **A COLLECTOR**: silva-side trivia walk (TOLERA collector
  precedent) + §2.1 anchoring + §2.2 pre-pass + stml_legere →
  annotation trees attached per §2.3; probatio with fixture pins
  (anchored/prose/malformed/multi-line/stacked/file-scope cases).
- **B INDEX + MINT**: identitates.tsv (nexus.tsv sibling: nid →
  via, unit kind, titulus, positio) rebuilt by the walk; minting
  instrument (fills boolean nids, explicit run); bench verb
  (##prefix → node corpus).
- **C EXAMEN CODICES**: tiers 1–3 above; verdict-neutral.
- **D TABULA**: ancora genus "nid" + resolver against the index;
  the board's CAUTIO-si-inresolutae upgrades from stale-hash
  warning to true resolution.
- **E MIGRATIO**: old families dual-read → auditor to zero →
  retire old parsers (the aedilis-arc dance).

Post-ladder, by name: selector-scoped contracts (needs quaestio
QC), `&nomen;` entities, projection consumers (MAP auditor, living
docs anchors, speculum overlay deep-links), structural diff seed,
speculum-lingua convergence (.c as dual document; the stripper
becomes a view).

## 5. Cautions (from the brainstorm, recorded so they bind)

- Never identity-everywhere (the MPS disease); on-demand +
  by-decretum classes only.
- Mint once, stable forever; adoption diffs are one-time.
- The JS/light consumers of annotations (if any) follow the
  manifest consumer pattern (section-aware reads) until the STML
  query CLI (01KY39N0GK) lands.
- This is the first paying consumer of silva's "comments are
  content with structure" pin (VIGILA-HANC) — the comment query
  layer's representation decisions now have a customer to check
  against.
