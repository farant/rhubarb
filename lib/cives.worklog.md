# cives worklog

## 2026-10-20 - born (ianua T1)

Persons of an app's accounts, extracted in spirit from exemplar's
handle-keyed `Usor` table (exemplar keeps its copy until ianua T5).
Spec `project-specs/ianua-spec.md` II.1, plan T1.

Design notes:
- Two tables. `cives` holds the CURRENT handle (UNIQUE); `cives_nomina`
  holds every handle ever used (PRIMARY KEY). Creation and rename insert
  into both; nothing ever deletes from `cives_nomina`, so a handle is
  reserved forever (plan P4) - old log lines naming a handle can never
  point at a new person.
- Every write checks first (valid handle, handle free, id known) and
  writes second, so a refused call leaves no row and no event. Within
  the caller's transaction (P3) a later failure is undone by the
  caller's rollback.
- Schema via CREATE IF NOT EXISTS (P2, admissio's precedent:
  scrinium_migrare's counter is shared with volumen's own migrations).
- Rename/status/delete take no piscina in the API; they make a short
  dynamic piscina for the event JSON and destroy it.
- `civis.status` carries the numeric status; `civis.deletus` only the id.

Tests: probatio_cives (credo v2, 7 sections, 98 assertions) incl.
reopening the volumen TWICE by path and a rolled-back creation leaving
no row, no event, handle free. Green on macOS, Linux musl + glibc.

Plants (silva.planta, all red then green): rename UPDATEs the reserved
row instead of inserting (old handle freed) -> renominare red; status
bound as ACTIVUS -> delere red; normalization not lower-casing ->
nomina red. The plan's "event before the uniqueness check" plant was
replaced: with the guard removed, the database constraints (UNIQUE /
PRIMARY KEY) still refuse and the caller's rollback removes the partial
row - the guard is a fast path, not the safety (plan ruling T1).
