# gesta_expeditio.worklog.md

## 2026-09-24 — natus (expeditio v1 T1)

The pure half of batch jobs (spec project-specs/expeditio-spec.md, decision
…D59G3Z). An expeditio's snapshot, rubric and row statuses live in its
entity's folded state, built by five event kinds: `expeditio-photographia`
(once, at birth), `rubrica-mutata`, `gradus-positus`, `gradus-promotus`,
`ordines-additi`. No I/O, no scrinium: gesta's fold and write-time validation
call in (T2); the MCP tool composes events and writes the inventory cell in the
same batch (T3). Mirrors gesta_inventarium on purpose.

Decisions made while writing it:
- Each tick copies the rubric's CURRENT `versio` into the row (the whole
  point of decision 1: "rows done under an outdated rubric" is a query). The
  plant that stores a constant version turns exactly one assertion red: the
  tick made after a rubric edit.
- `rubrica-mutata` REPLACES the rubric object with versio + 1 - the text of
  older versions is not kept in the state (the event log keeps it; a view that
  needs old text can replay). Enough for "which version", not for "what did
  v1 say" without the log.
- A second photographia is ignored by the fold (defensive) and refused by the
  validator ("use ordines-additi for growth") - a snapshot happens once.
- `_gradum_ponere` keeps a promoted row's `opus` when the row is later ticked,
  so the view can still show which opus did the work.
- Every event but the photographia is refused before a snapshot exists.
- `omissum` needs a `nota` (the cause), like inventory row removal.
