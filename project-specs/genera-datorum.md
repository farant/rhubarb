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

*Status: the TREE is agreed (Fran, 2026-10-20). Approach agreed the
same day: capabilities are built (on PULL) and categories are
compositions of them (§VIII-a); sync to an S3-compatible store is a
target the design must not block (§VIII-b); ordering by hybrid logical
clock and time-ordered ids (§VIII-c). The AXES (§III), the DECISION
TABLE (§VI), the consequences (§VII) and the recommendations in
§VIII-b/c are PROPOSED - each default is a decision for Fran before
code relies on it. Next: the projections spec (`proiectiones`) builds
on §VI ("derived" is the category projections live in).*

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
| **concursus** | commutes / last-writer-wins per field / per-element set / rebase or CRDT / manual | how do concurrent edits from two devices merge? (§VIII-b) |

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

## VIII-a. Capabilities and composition (approach agreed 2026-10-20)

The axes DESCRIBE data; CAPABILITIES are the mechanisms that implement
them. Capabilities are built one at a time when a consumer pulls them,
each with its API decided header-first; a category is a declaration
that COMPOSES capabilities plus the CONSTRAINTS between them (secret
forbids journal and index; derived may not be the source of another
derived table; view state forbids journal) - the same split as
organum's canon (form) and loader rules (cross-attribute laws). The
category registry is written once two or three capabilities exist,
never before.

| capability | does | status (2026-10-20) |
|---|---|---|
| journal | append-only truth, ordered by `seq` | built: `volumen` `acta` |
| blob store | content-addressed bytes, dedup | built: `massae` |
| names | path -> blob, mutable pointer | built: `plagulae` |
| history | replay, checkpoints, undo / redo, branches | built: `historia` |
| schema / migrations | per volume; per derived table | volume: built; derived tables: missing |
| typing / validation | values against declared types | built: canon, organum types |
| id minting | unique ids without coordination | built: `moneta` (ULID monotonic, nanoid, secure random - `include/moneta.h`); NOT used by agenda / personae (`max + 1`) |
| snapshot store | last-writer-wins, NOT journaled | missing (conservatio writes through the journal - the 58 %) |
| coalescing | keep only the latest per key | missing |
| derived tables | projections: declared, catch-up by seq, rebuildable | partial: `historia` in memory, the `acta(genus, seq)` index; the rest = semitae R5 |
| full-text index | text search | missing as a capability; vendored `sqlite3.c` contains FTS5 and the silex build enables it (`include/silex.h:149`) |
| retention / expiry / rollup | | missing |
| secret store | never journaled, indexed or logged | rules only (examen code 96, never through `lectiones`) |
| redaction | personal data out of logs | missing |
| provenance | source, version, citation | conventions only (`FONTES.md`, pinned commits) |
| audit / actor | who wrote this | missing (acts carry a time, no actor) |
| remote object store | S3-compatible client | missing (own TLS 1.3 client and SHA-256 exist; SigV4 needs HMAC-SHA256 + HTTP) |
| outbox / sync state | queued uploads, per-remote cursors | missing |
| encryption | client-side, before upload | missing |
| global act identity | act id independent of local seq | missing (§VIII-c) |
| leases / queue | jobs, retries, dead letters | missing (`processus` has async processes, no queue) |

First pulls proposed: the SNAPSHOT STORE (view state out of the
journal) and DERIVED TABLES (projections, starting with the week and
month views); FULL-TEXT next.

## VIII-b. Sync, multiple devices, offline (Fran 2026-10-20 - target)

Fran: all your data in a single file (federated documents possibly
later), but several clients (desktop, mobile) and backups - an
S3-compatible client library, data continuously syncing to the cloud
both as backup and for multi-device access, with an offline mode;
"one of those things that should be easy to support if you are
building on the libraries."

- **Local-first.** The local volume is primary; sync is a background
  job with an outbox (job-queue category: leases, retries). Offline is
  the default state, not a mode.
- **The journal is the unit of sync.** Acts never change, so a range
  ships as one immutable object (`journal/<device>/<from>-<to>`):
  idempotent uploads, nothing overwritten remotely.
- **Blobs sync by hash.** `massae` is content-addressed: the object key
  is the hash - dedup, upload-once, integrity built in.
- **The categories decide what syncs (§VI):** truth (journal, blobs,
  preferences) syncs; derived (projections, `plagulae`, checkpoints)
  never - each device rebuilds; view state is per device; secrets
  never use this channel.
- **Merging follows the `concursus` axis (§III).** Commuting events
  (analytics, logs, independent journals) and last-writer-wins fields
  (record fields) merge by order alone (§VIII-c); per-element sets
  (tags, phone lists) merge elementwise; DOCUMENTS (positional edits:
  scriba line diffs, pictor stroke order) need their base (§VIII-c)
  and a rebase, stable element ids (CRDT-style) or a person.
  `historia`'s branches (`<ramus ab=...>`) already model divergence
  locally: two devices editing one page = two branches from a common
  point.
- **Encryption on the client, before upload.** The provider sees
  ciphertext only; the keys are secrets. Content addressing of
  PLAINTEXT tells the provider which blobs are equal - hash with a
  per-user key.
- **Federation (later):** documents as separate volumes referencing
  each other; sync stays per volume; cross-volume references need the
  stable global ids of §VIII-c.

## VIII-c. Ordering and identity (PROPOSED)

- **Order by a hybrid logical clock (HLC):** `(physical ms, logical
  counter, device)`. Raw timestamps collide in one millisecond, skew
  between devices and jump backwards; the HLC stays close to wall time
  (human-meaningful, range-queryable), never runs backwards on a
  device, is pushed past any remote event received (causality), and
  the device breaks the last ties - a TOTAL order, the same on every
  device. Merging two logs = union sorted by HLC. Events stamped too
  far in the future are refused (one broken clock cannot poison the
  order).
- **Ids are time-ordered (ULID / UUIDv7 layout: 48 bits of ms + ~74
  random bits)** - unique without coordination and sorted by creation,
  which SQLite's B-tree indexes like. Random ids (UUIDv4, nanoid) are
  as unique but scatter index pages and carry no order: nanoid stays
  for short tokens, never for record keys. The house already mints
  ULIDs (`moneta_ulid`, Crockford base32, 26 characters, monotonic
  within a millisecond) for the ledger and code annotations - one id
  space, one mint: reuse it.
- **An act's id can BE `(hlc, device)`**, printed in the same base32
  form; no separate id needed.
- **Two clocks, two jobs.** Local `seq` stays exactly what it is: the
  ARRIVAL order and the cursor projections catch up by ("what have I
  not processed"). The HLC is the EVENT order a projection DECIDES by
  (last-writer-wins compares the HLC stored per field, never the
  arrival order) - a remote event can arrive after newer local ones.
- **Documents record their base:** each event names the state it was
  made against (a parent id, like git); two events with the same
  parent are a fork. Cheap to record now, expensive to guess later.
- **Ids in routes:** `nomenclator/7` becomes `nomenclator/01JC9X...`;
  a short suffix accepted as an alias (the ledger's habit) fits the
  R4e typed aliases.

Recommendation, in order of how fast they get more expensive:
1. Acts gain `hlc` and `device` beside the local `seq` (a volume
   migration; existing acts get an HLC synthesized from their time and
   seq).
2. Entities move from `max + 1` to `moneta_ulid` (agenda, personae)
   before more data accumulates.
3. Document events carry their base once multi-device editing is
   real (or sooner, since recording it is cheap).

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
6. Act identity (§VIII-c, rec. 1) and entity ids (rec. 2): when? Both
   get more expensive with every day of data.
7. The `concursus` value per category (§III, §VIII-b), especially for
   documents: rebase, stable element ids, or ask the person?

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
- FTS5: present in `vendor/sqlite3.c` and enabled for silex
  (`include/silex.h:149`); whether the apps' builds enable it is NOT
  checked.
- Analytics retention: whether mensura already rolls up or expires
  anything is NOT checked.
- No S3 provider, SigV4 detail or HTTP client path was examined; the
  sync section is a target, not a design.
