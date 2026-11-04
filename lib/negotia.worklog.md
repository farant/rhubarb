# negotia.c worklog

## 2026-11-04 — T1: the library (negotia plan)

Spec `project-specs/negotia.md`, plan `project-specs/negotia-plan.md`.
Shape copied from `lib/navigator_bibliotheca.c` (JSON acts, in-memory
index, catch-up from a cursor), with three differences:

- **Every act is a full snapshot** of its record (project, feature),
  so a revision is just "the record at that act" and history needs no
  diffing. An edit identical to the current state appends nothing
  (test "revisiones" checks `volumen_seq_ultimum` is unchanged).
- **Reads ALL replicas' acts**, unlike navigator (own replica only):
  projects are shared data, not locally derived state. If the vicus
  volume syncs through communio, another device's projects appear.
- **Cross-process visibility.** Nothing else in the tree sees writes
  by another process: `insula_versio` is an in-process counter,
  `data_version` is unused, open documents fold once at open
  (`lib/conspectus.worklog.md:140`). `negotia_renovare` compares
  `volumen_seq_ultimum` (one cheap query) with the cursor and only
  then reads. The cursor is advanced to `seq_ultimum` even when the
  newest acts are of other genera, otherwise every poll would re-run
  the genus query for nothing. Safe: the max is taken BEFORE the
  query, and anything committed after it has a larger seq and is
  returned by the query anyway.

Test "alius scriptor" = two `Volumen*` handles on one file (sqlite
sees that exactly like two processes). Links (feature -> project,
session -> project) resolve by EXACT id inside the library; suffix
lookup is only for callers (CLI) via `*_invenire`, and an ambiguous
suffix returns NIHIL with `ambiguum` set - never the first match.

Plants (all red in the named section, restored IDEM): identical edit
appends (revisiones), renovare never reads (alius scriptor), suffix
first-match wins (suffixum ambiguum).
