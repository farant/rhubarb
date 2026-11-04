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

## 2026-11-04 — T2: the CLI (tools/negotia.c)

- **stdin body**: `filum_legere_totum` sizes the read with `stat`, which
  is 0 for a pipe, so `-corpus-filum -` would read nothing. The tool
  reads every body file through `filum_octeti_*` ("/dev/stdin" for
  "-") into a chorda_aedificator, then drops trailing newlines so
  `-corpus T` and a heredoc of the same text give the same body (and an
  identical `mutare` writes nothing).
- **Reads never create a volume**: index / ostendere / revisiones open
  with `volumen_aperire` (a typo'd `-volumen` is refused, exit 1);
  only writing commands use `volumen_aperire_aut_creare`.
- Flags are checked per command (bitmask in `IMPERIA`), so
  `creare x -status shipped` is a usage error instead of a silently
  ignored flag. Invalid status token = usage (2), not refusal (1).
- `involvere` checks the session id is `[0-9A-Za-z-]+` already here:
  T6 puts it in a shell command line.
- Ambiguous suffix candidates are enumerated by the CLI (the library
  only says "ambiguous"); for features it walks every project.
- Smoke test (scratchpad `negotia_fumus.sh`) forces an ambiguous
  one-character suffix by creating projects until two ULIDs share a
  last character.
- **Plants by hand** (binary + smoke, planta is blind there); backup
  and script in `build/plantae_negotia_t2/`. Two plants of the same
  byte length written in the same second left the PREVIOUS plant's
  object in place (aedilis struere.sh judges by mtime) - plant 3
  "survived" for the wrong reason. Fixed with a 1.2 s sleep before
  each write and before the restore.
- `-provenientia` (fabrica T7) wired as in praeco: without it
  `bin/fabrica iudicare` calls bin/negotia IGNOTUM.
