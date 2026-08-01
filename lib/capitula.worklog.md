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

### Known limit, deliberately loud

`legere` has no nexus filter, so the Libri tab reads ALL `capitulum`
entities (quantum 200, the daemon ceiling) and filters client-side by
the `liber` nexus. Past ~200 chapters across all books, a ToC would be
truncated. The UI prints `CAUTIO: lectio tectum CC tetigit, index
INCOMPLETUS esse potest` when the read comes back at the ceiling —
silent truncation would read as "this book has 12 chapters".
Real fix is a nexus-filtered read in the store.
