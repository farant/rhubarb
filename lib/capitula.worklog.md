# capitula.worklog.md

## 2026-08-01 — birth: the ToC parser for the Libri tab

Fran wanted a books tab for reading notes: book metadata first, then a
table of contents, then (later) notes per chapter.

**The finding that changed the work:** the `book` genus already
existed. Fran built it through the genera creator on 2026-07-23 —
`title` / `year_published` / `authors`(relatio→`person`, multa) /
`owned` / `reading_status`(electio) — with three books in it and 39
people behind `authors`. So the metadata half of the ask was already
done and populated; only the ToC was greenfield. **Check the store
before designing the schema.** Twice earlier that same day a requested
change turned out to already be present.

`liber` was NOT available as a genus key — Sententiae owns it
(siglum/slug/status_libri/fons). Hence `capitulum`.

### Why chapters are entities

Decided with Fran. Alternative considered and rejected: the Sententiae
pattern (ToC text as truth, chapters as a rebuildable projection,
notes anchored to authored address + seal). That pattern earns its
complexity when the document is long, edited as prose, and the parser
will keep improving. A ToC is short and stable, and the thing we
actually want — notes/tags/nexus/FTS on a chapter — is free if the
chapter is a res and expensive if it isn't.

So: paste the ToC once to SEED entities; afterwards edit chapters
individually. The pasted text is not retained as truth, which means
the parser never has to become a printer.

### Traps found while building

**The subtitle trap.** A permissive `<label> - <title>` rule turns
"The Long Road - and What Came After" into an inscriptio of "The Long
Road". Since the inscriptio is the future note anchor, this is the
sententiae law again: *a lying anchor is worse than a missing one.* So
the label must contain a digit or be a strict Roman numeral. Also:
only the FIRST separator is a candidate — scanning onward splits
"A - B - C" in the middle.

**Roman numerals need a STRICT validator, not a charset check.** A
naive "all chars in IVXLCDM" test accepts DID, MILD, CIVIC, VIM, DIM,
LIVID — ordinary English words. The subtractive-form validator rejects
all of them because their internal pairs are illegal (I before D, I
before L, …). `MIX` survives and always will — it is a real numeral
(1009). That's fixed in a test so it can't change silently.

**Indentation depth by STACK, not by space count.** Two-space,
four-space, and tab indentation all mean the same thing. The Python
INDENT/DEDENT rule handles all three with no configuration. Known
consequence, documented rather than "fixed": inconsistent indentation
(3 spaces where 2 came before) creates a phantom level. Guessing at a
correction would destroy real indentation just as easily.

**This parser never refuses**, unlike `sententiae_legere`. There is no
structural invariant a ToC can violate — every non-blank line is a
chapter. That's why the consumer MUST show a preview: a silent parse
that writes 40 entities costs 40 corrections.

### Calibration

The suite passed on the first run, so I loosened `_inscriptio_valet`
to always return VERUM and confirmed the subtitle assertions went red
with the right message, then reverted. The Roman tests are
self-calibrating (mixed VERUM/FALSUM assertions on one function can't
both pass under a constant return).

### Seam test

Unit tests prove the parser; they do not prove the bridge. Started
`bin/forum -servire 8791` and POSTed to `/internuntius` with the
`vitrea` cookie + `X-Vitrea` header, confirming `capitula_parsare`
returns exactly the field names the webview reads. This is the check
that today's X-Vitrea outage taught: two green halves can still have a
broken contract between them.

Note for next time: the auth guard requires the token even on
loopback (`_custodia_admittit` only sleeps when no token is
configured, and forum always configures one). Cookie name is
`vitrea=`, not `vitrea_tessera=`.

### Known limit — CLOSED same day (nexus filter shipped)

`legere` has no nexus filter, so the Libri tab reads ALL `capitulum`
entities (quantum 200, the daemon ceiling) and filters client-side by
the `liber` nexus. Past ~200 chapters across all books, a ToC would be
truncated. The UI prints `CAUTIO: lectio tectum CC tetigit, index
INCOMPLETUS esse potest` when the read comes back at the ceiling —
silent truncation would read as "this book has 12 chapters".
Real fix is a nexus-filtered read in the store.

**Done the same afternoon** (Fran asked for it directly). `legere`
now takes `nexus_verbum` + `nexus_ad`; the ceiling measures ONE book
instead of the whole genus. Details in
gesta/fontes/tabularium.worklog.md — the two findings worth carrying:

1. **The both-empty guard is load-bearing far beyond this feature.**
   Without `((? = '' AND ? = '') OR EXISTS (...))` the EXISTS
   degenerates to "has any nexus at all", so every *unfiltered* read
   silently drops entities that have no links. Deliberately breaking
   it took down EIGHT assertions across probatio_tabularium, most of
   them pre-existing (pipata, captured ideas). That is the whole
   argument for calibrating a guard by watching it go red.
2. **`_res_legere` in forum.c selects arguments by NAME — it does not
   forward them.** The two new args would have been dropped in
   silence, and because I had just deleted the client-side filter,
   every book would have shown every chapter with no error anywhere.
   Found by reading the handler, not by testing. Comment added at the
   site so the next argument isn't lost the same way.

## 2026-08-01 (evening) — chapter page + reading notes

Fran's design: click a chapter → its own page, feed growing downward,
editor at the bottom with vertical tabs for the note type. Types:
nota, citatio, persona, eventus, locus; every one carries an optional
page number.

### The shape decision

Person/event/place are **world entities**, not fields on a note. The
same Erasmus Darwin appears in chapter 3 and chapter 11, and in a
different book next year; Birmingham likewise. So the feed entry is
an `adnotatio` that POINTS at a `person`/`locus`/`eventus`. Fran saw
this himself — "this would create a person res but I think it would
also create a 'person note' attached to the chapter."

**One `adnotatio` genus with a `species` field, not five genera.**
The feed is then ONE read (nexus filter, `nexus_verbum:'capitulum'`)
sorted by creation; five genera would mean five reads, a client-side
merge, and new code per note type. `ADNOT_SPECIES` is a table — a new
species is a row, not a function.

**No `ordo` field.** Chapters have STRUCTURAL order (reading
sequence); notes have TEMPORAL order (when you wrote them), and
`creatum` carries that for free. Different kinds of order, different
mechanisms.

### Decisions Fran made

- **Person name**: `first_name`/`last_name` added ADDITIVELY to the
  existing genus (39 rows untouched, blank until edited). `name`
  stays the title field and is DERIVED from the two on every write —
  one-way derivation can't drift. No migration.
- **Coordinates are text.** The genera palette has no float type
  (`numerus` validates `json_est_integer`), so lat/lon would silently
  truncate. One `coordinatae` text field holds what you paste from a
  map. A real decimal type is the named alternative if computation
  (distance, plotting) is ever wanted — noted, not built.
- **Annotation-only editing.** From the feed you change the note, the
  page, and which person it points at; fixing a person's death year
  happens in Res, where the whole record lives. Keeps a shared entity
  from being casually rewritten while reading another book.

### Consequences worth remembering

- **Species is FIXED while editing.** Changing it would orphan an
  already-created world entity and leave a nexus under the wrong
  verbum. A note of another species is a NEW note.
- **`titulus` must never be empty** — `addere` requires it, and a
  person note can legitimately have no free text. `summarium_adnotationis`
  falls back to the subject's title, then to the species name.
- **The feed reads world entities ONCE per chapter open**, not per
  poll. `ad_titulus` comes free through the nexus, so the feed is
  readable without them; the cache only ADDS the year of an event and
  the coordinates of a place. Missing cache degrades information,
  never correctness.

### Verified through the real bridge, not simulated

`bin/forum -servire` + POSTs on the same path the webview uses:
genera created, person written with derived `name`, annotation
written with species/pagina/textus, both nexus attached, and the feed
read back through the nexus filter showing linked and unlinked
species together. `fori res` on the annotation shows NO custodia
notes — the machine accepts the schema. Test entities then tombstoned
(4), verified gone from the feed.

Finding along the way: the app's `transmittere` allowlist is
`addere|gerere|legere|quaerere` — `res` is NOT callable from the
webview. Fine for the app, but it's why a test script driving the
bridge can't inspect an entity that way.
