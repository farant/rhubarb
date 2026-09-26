# materia_diagnostica — worklog

## 2026-09-25 — emitted supersedes declared; the record split from the engine (silva-migratio T15)

**Supersede rule.** A declared GENUS diagnostic is the genus's default
("every ERROR node is an error", one location — by design, materia never
invents a second location for GENUS). When a parser emits a diagnostic
for the SAME node with the SAME code, it knows the same fault better
(C89: where the GLR died, and why), so the emitted one replaces the
declared one. Different code on the same node = a different diagnostic,
kept. Implemented as tombstones (codex NIHIL) on the walk's own entries,
skipped at sort time. probatio section XII pins all three cases; planted
red (5).

**materia_diagnosticum.h.** The diagnostic RECORD (MateriaDiagnosticum,
MateriaSedesRelata, MATERIA_CODEX_*, MATERIA_NOTA_*) moved out of
materia_diagnostica.h into a header with no body, like
materia_registrum.h. Reason: aedilis maps a header to its body, so an
emitter (silva_frons) that needed only the record type dragged the
whole lint engine — materia_diagnostica.c, excusatio, exemplaria — into
silva's amalgam (and two new static-name collisions with it).
materia_diagnostica.h includes the new header, so every existing
client compiles unchanged.
