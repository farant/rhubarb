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

## 2026-11-02 — the "seen tokens" set moves to a scratch arena

`_visum()` keeps derivare from checking a token twice when a tree
shares it (a `<<#lexN>>` transclusion read back from STML makes one
token object appear in two places). It used a string-keyed
`TabulaDispersa` in the CALLER's piscina: an 8-byte arena key per token,
40-byte slots, and every doubling left the old table behind. On the
largest MusicXML score (Reger op. 74, 15.8 MB XML, 3.07M tokens) that
was 343.6 MB and 3.07M allocations that stayed in the document's arena
for its whole life, half of what the "cooked view" seemed to cost. XML
trees never share tokens, so it never found anything there, but it
can't be skipped per grammar: sharing is a property of the tree, not
the grammar.

Now: an open-addressing set of raw token pointers (8-byte slots,
Fibonacci hash of the pointer, linear probing, grows at half full) in
a scratch piscina created inside derivare and destroyed right after
the walk. Nothing stays in the caller's arena. Reger: xml_documentum
1127.9 -> 784.5 MB, 15.62M -> 12.55M allocations; the set peaks at
about 96 MB during the walk, all freed.

Two guards the first draft lacked: growth allocates the new table into
a local, so on failure the old table stays valid (the walk continues
after memoria_defecit and reports it at the end); and the last free
slot is never filled, because lookup stops at an empty slot - a full
table after repeated failed growth would loop forever.

Goldens: xml computus usus_coquendi -41 KB per document (the old
table's 1024-slot start), toml computus allocationes_arboris -26 on one
file. Plant: `_visum` never reports "seen" -> probatio_materia_diagnostica
FRACTA. Note for plants here: fabrica links materia, so the materia
runner refuses ("iudex ipse non recens") until fabrica is rebuilt WITH
the plant; silva.planta can't do this one, it was done by hand.
