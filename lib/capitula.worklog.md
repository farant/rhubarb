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

## 2026-08-01 (late) — five more species, and the forum's first JS tests

Added: **terminus** (word + definition), **societas** (name + year
founded), **scriptum** (a referenced written work), **inventum**
(description + year + inventors), **quaestio** (question, answer
added later).

### Modelling

Four are world entities; **quaestio is not**. A question belongs to
the moment of reading, not to the world, so it lives on the
annotation with a new `responsum` field — empty until filled. That
also means "what didn't I understand in this book?" is answerable
without a status field: species = quaestio, `responsum` empty. The
empty answer IS the status, so the two can never disagree.

**`scriptum` reuses the `book` genus** (Fran's call) rather than a
parallel one: a pamphlet noted today can become a book read next
year, with its author already linked. The cost is that the Libri
index would fill with works never read — so notes write
`owned: false`, and the index shows only owned-or-has-reading-status,
with the hidden count ALWAYS displayed. A silent filter would read
as "my book disappeared."

**A term's definition lives on the term**, so it accumulates into a
glossary across books; a second author's different usage goes in the
note's own text.

### The picker, extracted rather than rewritten

Fran: *"we probably have something we can use from the res editor or
at least build on."* Right — `selector_relationis` already had
search, chips, cardinality, and inline-create (with the two-layer
title fix). Extracted it to `selector_entium(cfg)` taking its state
by PARAMETER instead of reaching for the form's globals; the Res
form is now a five-line wrapper. Three consumers now: the Res form,
the note's subject picker, and relation fields INSIDE entity
creation (inventores, auctores).

One flag earned its place: `creare: false`. The subject picker sits
directly above explicit fields (praenomen/cognomen/annus), so
offering "+ create «Darwin»" there would produce a half-empty person
while the full fields below sat ignored.

### THE trap of this batch: electio options

`species` is an `electio`, and `_campum_iudicare` rejects values
outside `optiones`. The `adnotatio` genus was ALREADY LIVE with five
options — so shipping five new species without widening the live
definition would have every new note rejected by the machine with a
message the webview never surfaces. Handled by `aequare`, an
ADDITIVE alignment: adds missing fields, unions electio options,
never removes or rewrites anything (which is also the genus layer's
own law).

### apps/forum/assets/probatio_fori.js — the forum's first JS tests

The webview now carries real logic that exists nowhere in C: title
derivation, the additive alignment, the index filter. So: the
speculum pattern (JXA + fake DOM), evaluating the REAL `<script>`
extracted from index.html. 53 assertions, wired into
compile_tests.sh beside probatio_velaminis.js.

Two traps hit while writing it, both already recorded and both worth
re-recording because they cost time anyway:

1. **JXA microtasks may never flush** — a real `Promise` chain
   silently half-runs, so assertions inside `.then()` never execute
   and the test reports GREEN. Fixed with a synchronous thenable
   (`Sync`) substituted for `Promise`; the app code can't tell.
2. **My own harness read the wrong envelope.** `instrumentum(n, a)`
   sends `transmittere {instrumentum, argumenta}`, so
   `v.argumenta.actus` is always undefined — the test reported "no
   mutation issued" while the code was mutating correctly. A harness
   bug reads exactly like a product bug.

Calibrated both ways: removing one species option turns 2 assertions
red naming the option; making the alignment replace instead of merge
turns the idempotency assertion red.

### Bridge-level proof

Separately from the JS tests, all five species were driven through
`bin/forum -servire` against the live store: machine accepted every
new species value, an `inventum` linked TWO inventores, a `scriptum`
wrote `owned=false` with its author linked, and a question got its
answer added afterwards. 12 test entities tombstoned after.

## 2026-08-01 (late) — the back-breadcrumb problem

Fran: clicking a person/place chip in the chapter feed lands you in
the Res entity view, whose back link goes to the Res table — which
dumps you out of your reading. *"It's a little tricky since it's not
a full browser and there's not an easy back button."*

**One trail, not a history.** The question "where did I come from"
has exactly one answer, so `res_reditus = {res_id, titulus, ire}` —
the same shape as `forma_reditus`, which the book-edit delegation
introduced this morning. A stack would be more general and would buy
nothing anyone asked for.

**Staleness made structurally impossible, not managed.** The trail
is bound to the `res_id` it was created for. Open any other entity
by any other route and the trail simply doesn't apply — a
comparison, not a discipline of remembering to clear. (It's also
cleared when leaving the Res tab, but that's hygiene, not the
guarantee.) Calibrated: dropping the id comparison turns two
assertions red, and the reported symptom is exactly what a user
would see — an unrelated entity offering "← Riots".

**The label names the destination.** Without a browser there is no
implied "back", so the link reads `← Riots`, not `← retro`. You know
where it goes before you press it.

**The return deliberately does NOT rebuild the note editor.** You
look a person up in the middle of writing a note; if the return
called `adnot_formam_reddere()` your half-written note would be
gone. So the return restores the view and the feed only, leaving
`adnot_lat` pointing at the same live inputs.

Wired into tags too (two call sites, same mechanism) since the same
complaint applies there — `← #nominalismus` instead of `← tabula`.
The STML stamp path keeps the old behaviour: its origin genuinely
varies, and guessing would be worse than the plain default.

## 2026-08-01 (late) — the term's definition in the card body

Fran wanted a term's definition visible on the chapter card, not
just its word.

Done with `campus_corporis` on the species descriptor — the name of
an ENTITY field that belongs in the card body rather than the
header parenthetical. Terminus is the only species that has one
today, and that is not an accident: `inventum` and `eventus` put
their description in the TITLE, and coordinates/years are short
enough for the parenthetical. So the mechanism is general but earns
its keep once.

Ordering matters and is asserted: **definition before the note.**
The definition is the entity's words and is the same in every
chapter; the note is mine about this book. They get different
styling (italic, dimmer) so they can't be misread as one voice.

Degradation is the same rule as the rest of the feed: the definition
comes from the world-entity cache, so before that load the card
still shows the word (via the nexus `ad_titulus`) and the note —
information is reduced, never wrong. Pinned by a test that nulls the
cache.

### The card itself now has tests

`elementum_adnotationis` is called directly against the fake DOM,
asserting what the reader actually sees: definition present, drawn
from the entity, ordered before the note, quotes wrapped in “ ”, an
unanswered question announcing itself, an answered one showing its
answer. 76 assertions total. Calibrated by deleting
`campus_corporis` — one assertion red, naming the empty body.

Also checked through the live bridge that the definition reaches the
client at all (it does), because a rendering test can only prove the
painting, never the data.

## 2026-08-02 — maps + timeline, modelled on franarant.com/events

Fran asked for two things off his own events page: the Leaflet map
component for places with coordinates, and a timeline showing only
notes that carry a year. He flagged the right risk himself — can a
WKWebView on a `capsula://` origin even fetch a CDN?

### The measurement that decided it

Built a throwaway probe binary, ran the real window ~12s, reported
through the bridge:

    scriptum=ONERATUM(typeof L=object) | css=ONERATUM
    tessella=ONERATA(256x256)          | fetch=OK(200)

Script, stylesheet, an OSM tile image, AND `fetch()` all succeed from
the custom-scheme origin. No plan B needed. **Probe reverted, test
pipatum tombstoned** — the measurement was the deliverable, not the
code.

### SRI: caught myself inventing hashes

I first wrote the integrity hashes from a TRUNCATED terminal display
of Fran's page — which would have made the script silently refuse to
load, presenting as "maps don't work". Pulled the real values from
his markup and independently recomputed them from the CDN bytes;
both matched, and a test asserts the split string literals
reassemble to the exact hashes. His page already used SRI, so
copying it verbatim inherits the supply-chain protection — worth
saying, since a CDN in this codebase would otherwise deserve an
argument.

### Year comes from the ENTITY, which is why the filter was free

`eventus.annus`, `persona.year_of_birth`, `societas.annus_conditus`,
`scriptum.year_published`, `inventum.annus` — so "notes with a year"
is one `campus_anni` entry per species, and nota/citatio/terminus/
quaestio/locus drop out because they genuinely have no year. A new
species with a year is a table row, not a branch.

### Two decisions Fran made, and why they mattered

**`eventus` now points at `locus`.** His page carries year + place +
description in ONE record, and 213 of them have coordinates — that
data proves the two belong together. We keep them normalized
(Birmingham is one entity) but linked, so a timeline entry can show
the map exactly like the screenshot.

**Notes now link to the book as well as the chapter.** Deliberate
denormalization: without it a book timeline is one read per chapter
(42 for Lunar Men, every 2s poll). A chapter never changes books, so
drift is nil. **Backfilled the 13 existing live notes** — without
that the book timeline would have been silently empty, which reads
as "no notes have years" rather than "the link is missing."

### Map behaviour copied deliberately

Non-interactive (drag/zoom/wheel/keyboard off, `maxZoom ===
minZoom === zoom`) so it's an illustration, not a widget that
captures the scroll wheel inside a feed. Per-place `zoom` field
added because that authored detail is what makes his page read well
(flood 3, city 5). Lazy `IntersectionObserver` creation, same as his
`onVisible` — a chapter can hold dozens of notes and eager Leaflet
instances would choke the window.

Attribution is left ON here, unlike his page's `// not for
production`.

Failure is loud: no network → the map area says so and prints the
coordinates, rather than showing an empty box. Unparseable
coordinates produce NO map at all — a map of the wrong place is
worse than no map, because a reader believes a map.

### Harness, third and fourth time

- A malformed assertion of mine (`aequale(...) === undefined || ...`)
  asserted the opposite of what it claimed. It failed loudly, which
  is the only reason I noticed.
- The idempotency fixture listed genera BY NAME, so adding two new
  `aequare` calls made it report "not idempotent" while the code was
  correct. Rewritten to replay every recorded mutation generically,
  so the next `aequare` can't break it.

## 2026-08-02 — deletion has TWO shapes, and I used the wrong one

`census` reported `insalubres 1` after a night of schema edits. Chased
it expecting my `pagina` or `descriptio` retype to have orphaned
something — the health check's own comment points there ("orphani post
emendationem schematis HIC apparent"). It was neither.

It was my own CDN-probe pipatum, and the cause is a distinction worth
keeping:

- **User-defined genera** (via `definitio`) carry `campi` and NO
  `attributa`. Nothing is required, so deleting by removing `titulus`
  is correct — that's what `tumulus_est` tests, and health has nothing
  to complain about.
- **Seeded genera** (pipatum, articulus, commentarium, diurnum, …) get
  ATTRIBUTA_V2 merged in, where `titulus` is `necessarium: true`.
  Removing it leaves the entity permanently unhealthy. Their deletion
  shape is removing **corpus** — which is exactly what the app's
  `delere` does.

I'd used the user-genus idiom on a seeded genus a dozen times in test
cleanup and only one landed on a seeded genus, so only one showed.

Fixed by restoring a titulus and removing corpus, giving it the shape
a normally-deleted pipatum has. Back to `insalubres 0`.

**Why bother for one row**: a health counter that permanently reads 1
for a reason you know about is worse than no counter — it trains you
to skip past the number, and the next 1 is real. Same argument as the
false CAUTIO VIGILIAE.

Also worth noting the health check's grades: `attributum-necessarium-
absens` is FATAL (breaks `sanus`), while `campus-extra-specem` and
`typus-campi-pravus` are SOFT — flagged, not fatal. So a user-genus
entity essentially cannot go unhealthy, which is why every retype
tonight left `insalubres 0` even before I fixed this.

## 2026-08-02 — the fuzzy filter, and why toy fixtures lied

Fran wanted a fuzzy filter on the Libri index. Built it the obvious
way — fzf-style subsequence over the whole haystack — and every JS
test passed against fixtures like "The Lunar Men" and "Amazing Grace".

Then I ran it against his ACTUAL 17 books:

    «lnr»   -> 10 of 17 books
    «punch» -> Punched-Card Systems, AND Cyclopaedia, or, An
               Universal Dictionary of Arts and Sciences
    «uglow» -> The Lunar Men, AND An Account of the Foxglove...

Subsequence over a long haystack is nearly a tautology: a 40-word
title contains almost any 3 letters in order. The feature was
useless and every test was green, because my fixtures were SHORT.
Three-word titles cannot exhibit the failure mode of forty-word
titles.

**Rule that replaced it**: a query token matches if it is a
SUBSTRING of the haystack, OR a subsequence of the WORD INITIALS.
Initials are a short string, so they carry the abbreviation power
that makes fzf pleasant (`dlx` → Dealers of Lightning - Xerox)
without the noise, since there is nothing long to get lost in.
Measured after:

    lunar/gombrich/punch/uglow -> exactly one book each
    dlx, tlm, "xerox parc"     -> exactly one book each
    lnr, zzz                   -> nothing

Haystack includes linked entity titles, so `gombrich` and `uglow`
find books whose authors they are — the author's name is nowhere in
the title.

### The lesson, which is not new but keeps arriving in new clothes

This is the corpus-contact law again, in the one place I had been
treating as safely covered: I had 200+ passing webview assertions and
they all agreed the filter worked. They were testing the algorithm I
wrote against data I invented. **A fixture you authored shares your
assumptions; a corpus does not.**

Cheap habit that would have caught it immediately: after building
any filter/search/sort, run it against the real store before
believing the tests. It cost one bridge script and thirty seconds.

The tests now pin the failure explicitly — `lnr` must NOT match The
Lunar Men, `punch` must NOT match Cyclopaedia — so the loose version
cannot come back silently.
