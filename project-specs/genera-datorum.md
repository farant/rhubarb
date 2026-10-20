# genera datorum — kinds of data

*Born 2026-10-20 in secunda, at the start of the projections design
(semitae R5). Fran: "making all data fall into one of these
categories (and implementing new categories as necessary to cover
omissions) is important because it makes our library able to make
better decisions for different kinds of things." Fran gave the first
tree (entities, filetrees, binary files by media, app state,
analytics, logs, code, secrets, job queue); Claude proposed the axes
and the additions marked + in §IV; Fran: "those are all good
additions."*

*Status: the TREE is agreed (Fran, 2026-10-20). The AXES (§III), the
DECISION TABLE (§VI) and the consequences (§VII) are PROPOSED - each
default is a decision for Fran before code relies on it. Next: the
projections spec (`proiectiones`) builds on §VI ("derived" is the
category projections live in).*

*Discipline note: the PULL rule holds. A category earns code when a
consumer needs its decision (a store, a backup rule, a search index),
never to complete the table. The tree may grow; a new category is a
named bundle of axis values, so most decisions already know what to
do with it.*

## I. Doctrine

1. **Every datum belongs to exactly one category.** Data that seems
   to fit two is two data (a picture DOCUMENT and its exported PNG
   are a document and a derived file).
2. **A category is a named bundle of AXIS values (§III).** The
   library decides by the axes, not by the name: storage, journaling,
   history, backup and sync, search, retention, privacy.
3. **Media type is orthogonal to category (§V).** A photo and a
   screenshot share a media type (how to SHOW them) and differ in
   category (how to TREAT them).
4. **Truth and derived never mix.** Derived data is rebuildable by
   definition and is never the source of other truth.
5. **Categories are data, declared in one place (§VIII)**; stores and
   manifests carry a category name and the library checks behaviour
   against it.

## II. What exists today (cited)

- **The volume** (`include/volumen.h:1-27`): `acta` = append-only
  truth, `massae` = content-addressed blobs (SHA-256), `plagulae` =
  path -> blob manifest (a projection of `acta` + `massae`).
- **Documents on `historia`** (`include/historia.h:1-35`): agenda
  (`include/agenda.h`, one document, item events `<res actio=...>`),
  personae (`include/personae.h`, one document), scriba pages
  (`include/scriba_liber.h`; books per space: shared, `dies/<ISO>`,
  `persona/<id>`; events = line diffs `<mutatio linea=...>`),
  pictures (`include/pictor_documentum.h`). Each replays its acts into
  an in-memory projection with checkpoints.
- **UI state** (`include/conservatio.h:1-19`): persistent fields of
  registered branches are saved as plagula `status/<key>`, i.e. one
  `plagula-condita` act per save. Vicus saves its layout index the
  same way (`lib/vicus.c` `indicem_scribere`, plagula `vicus/latera`).
- **Measured 2026-10-20** on a copy of `~/.rhubarb/paginae-proba.volumen`
  (91 MB; counts by act kind only, no content read; copy deleted):

  | acts | share | kind |
  |---|---|---|
  | 20,473 | 58 % | `status/` snapshots (34 distinct keys) |
  | 3,927 | 11 % | `vicus/` layout index |
  | ~1,300 | 4 % | other snapshot writes (books, day indexes) |
  | ~5,800 | 16 % | pictor strokes (all picture spaces) |
  | 3,058 | 9 % | scriba page edits |
  | 171 / 155 / 104 | 1 % | day notes, agenda, personae |
  | 35,247 | | total |

  Domain events (records and documents) are about a quarter of the
  journal; view-state snapshots are well over half.
- **Analytics** live in their own volume: `~/.rhubarb/mensurae.volumen`
  (121 MB on 2026-10-20; `bin/mensor`, analytica project).
- **App-state classification** already exists for widget fields
  (`include/organum.h:40-71`: vita, rhythmus, sedes; routes add
  semita) - the precedent for this document.
- **Code** has its own kinds registry (`bin/fabrica quid`, unitates
  codicis); **secrets** have one hard rule already (examen code 96:
  raw file access outside `filum`; a secret never goes through
  `lectiones` - the 2026-09-23 key leak).
- **Ledger** items live in `~/.rhubarb/annales/tabularium.jsonl` (its
  own git repo; the `.db` is a rebuildable projection of it).

## III. The axes

| axis | values | question |
|---|---|---|
| **veritas** | truth / derived | lost: is information gone, or rebuildable? |
| **mutatio** | append-only / edited / immutable | how does it change? |
| **historia** | full / latest only | does the past matter (undo, audit, "as of")? |
| **identitas** | id / content address / path / none | how is one found? |
| **retentio** | forever / rolled up / expires | how long does it live? |
| **secretum** | normal / personal / secret | who may see it, and where may it go? |
| **quaestio** | id / relation / text / time range | how is it queried? |
| **auctor** | user / app / system / external | who writes it? |

(Latin axis names are placeholders until code needs identifiers.)

## IV. The categories

Fran's tree, with Claude's additions marked **+** (agreed 2026-10-20).

- **entities**
  - **+ records** - small fields, discrete events (agenda items, people)
  - **+ documents** - a large body edited continuously, with undo
    (scriba pages, pictures)
  - **+ workflow items** - records with a lifecycle (ledger questions,
    parks, decisions, tickets)
  - **+ financial records** (pecunia) - never edited, only reversed;
    strict invariants (balances, double entry)
- **filetrees**
  - **+ source trees** (git-versioned)
  - **+ user folders**
  - **+ mirrors / reference clones** (someone else's tree, pinned)
- **binary files**
  - images: screenshots (generated, usually disposable), photos
    (irreplaceable, personal), illustrations (often an export of a
    document, so derived), game assets (part of a source tree),
    **+ thumbnails / previews** (derived)
  - pdfs: books (reference data), documents/mail (communications),
    **+ generated reports** (derived)
  - build artifacts: docker images, binaries, **+ amalgams and
    generated code** (committed but derived - the `GENERATUM` files),
    **+ test goldens / fixtures** (truth for tests, versioned)
  - music, sound effects, **+ video**, **+ fonts**, **+ archives**
    (containers: their category is their contents), **+ 3D models /
    vector graphics**
- **app state**
  - **+ view state** - routes, selection, open panes (ephemeral, no
    history)
  - **+ preferences / settings** - durable user intent, worth syncing
  - **+ layout / session** - vicus panes and tables
  - **+ drafts** - unsent text, half-made things (small but precious)
  - **+ clipboard**
- **analytics**: **+ metrics / time series**, **+ traces / spans**
  (mensura), **+ test and gate results, benchmarks**
- **logs**: **+ diagnostic logs** (disposable, retention-limited),
  **+ audit logs** (who did what; long retention, never edited)
- **code**: **+ source**, **+ declarations / manifests / schemas /
  canons** (STML: data that acts like code), **+ scripts**,
  **+ generated code** (derived; names its generator)
- **secrets**: **+ API keys / tokens**, **+ passwords**,
  **+ certificates / private keys**
- **job queue**: individual jobs, multi-step saga type things, status
  of jobs, **+ scheduled / recurring jobs**, **+ locks / leases**,
  **+ dead letters** (failed for good, kept for inspection),
  **+ idempotency keys**
- **+ communications** - mail, chat, messages, Claude session
  transcripts (others' words, append-only, personal)
- **+ reference data** - corpora, dictionaries, standards tables
  (saints, UnicodeData, Whitaker's WORDS, ISO 4217): external source,
  versioned, cited, never edited locally
- **+ derived data / caches** - projections, indexes, checkpoints,
  thumbnails: rebuildable, disposable, never backed up as truth
- **+ observed state** - clock, window size, network, sensors: read
  from a port, never stored as truth
- **+ configuration** - machine and build config (`aedificatio.stml`,
  environment), distinct from user preferences

A word on **logs**: the volume's `acta` is a JOURNAL (truth), not a
log. "Log" here means diagnostics and audit trails only.

## V. Media type vs category

The binary subtree splits by media type, but the leaves under one
media type land in different categories: screenshot vs photo
(generated vs captured; disposable vs irreplaceable), book vs mail
(reference vs communication), illustration vs game asset (derived
export vs source asset). So every binary file carries BOTH:

- **media type** (image/png, application/pdf, audio/...) - decides
  display, thumbnailing, decoding;
- **category** - decides backup, history, search, retention, privacy.

One image library can hold photos and screenshots; the photo is
backed up forever, the screenshot expires.

## VI. Decision table (PROPOSED - each default is Fran's call)

| category | veritas | journaled in `acta`? | history | backup / sync | search | retention | secretum |
|---|---|---|---|---|---|---|---|
| records | truth | yes | full | yes | fields + text | forever | personal |
| documents | truth | yes | full (undo) | yes | text | forever | personal |
| workflow items | truth | yes | full | yes | fields + text | forever | normal |
| financial records | truth | yes | full, never edited | yes | fields | forever | personal |
| blobs (photos, books, assets...) | truth | the reference is; bytes in `massae` | latest | yes | metadata | forever | by leaf |
| view state | truth (cheap) | **no** - last-write-wins table | latest | no | no | until replaced | normal |
| preferences | truth | yes, or coalesced per key | latest (+ audit?) | yes (sync) | no | forever | normal |
| layout / session | truth (cheap) | no - snapshot | latest | no | no | until replaced | normal |
| drafts | truth | snapshot | latest | yes | text | until sent / discarded | personal |
| analytics | truth (statistical) | own volume | append-only | optional | time range | rolled up | normal |
| diagnostic logs | truth (disposable) | no | append-only | no | text | expires | personal |
| audit logs | truth | own journal | append-only | yes | time + actor | long | personal |
| code | truth | git | git | git | silva / text | forever | normal |
| secrets | truth | **never** | latest | separate channel | **never** | until rotated | secret |
| jobs, sagas | truth | events (requested / finished) | full | yes | status | until done + window | normal |
| locks / leases | truth (ephemeral) | no | none | no | no | expires | normal |
| dead letters | truth | yes | full | yes | status | long | normal |
| communications | truth | yes (external author) | append-only | yes | text | forever | personal |
| reference data | truth (external) | no - vendored, versioned, cited | by version | the source | text | per version | normal |
| derived / caches | **derived** | **no** | none | **no** | as declared | until rebuilt | inherits its sources |
| observed state | not stored | no | none | no | no | none | normal |
| configuration | truth | git or own file | git | yes | no | forever | normal (secrets split out) |

## VII. Immediate consequences (PROPOSED)

1. **View state leaves the journal.** Route, selection and open-pane
   snapshots (today `status/` and `vicus/`, about 69 % of the measured
   journal) go to a last-write-wins table outside `acta`; projections
   that replay the journal never see them. Preferences are separated
   from view state and stay journaled (or coalesced per key). This
   needs the organum field classes to say which persistent fields are
   preferences and which are view state.
2. **Projections are `derived`.** Rebuildable by definition, excluded
   from backup and sync, droppable at any time; their SOURCES must be
   truth categories (records, documents, communications, workflow,
   jobs) - a projection over derived data is refused.
3. **Search is declared per category.** Records, documents and
   communications are indexed; secrets never; logs with retention.
4. **Backup and sync follow `veritas` and the table**, not a list of
   paths.

## VIII. Where it lives (PROPOSED)

As DATA in one registry, beside the house's other kinds registries
(`natura`, `unitates.stml`): the axes with their values, the
categories as bundles, and the decision rules. Volume spaces, organum
manifests (per persistent field), projection declarations and stores
carry a category name; the library checks declared behaviour against
it (e.g. a projection whose source is derived data is refused; a
field marked `secret` can never be journaled or indexed).

## IX. Open questions for Fran

1. View state out of the journal (§VII.1): yes? And then: where do
   preferences live, and are they journaled or coalesced?
2. The decision-table defaults (§VI), row by row - especially
   personal vs normal, drafts' retention, and whether preferences keep
   an audit history.
3. The registry's home (§VIII): `natura`, a new STML file, or the
   volume itself?
4. Latin names for the axes and categories (needed as soon as code
   carries them).
5. Which consumer pulls first? Candidates: the view-state move (the
   58 %), or the projections spec using `derived`.

## X. AUDIENDA

- The act counts (§II) are from ONE volume on one day; the other
  volumes (`mensurae`, `documenta`, `laboratorium`, ...) were not
  censused.
- The 34 `status/` keys were counted, not classified: which of them
  hold preferences and which view state is NOT known.
- No other store (ledger, gesta, git, keychain) was checked against
  the decision table.
- The table's defaults are proposals written in one sitting; none is
  measured or decided.
