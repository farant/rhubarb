# fasti_fontes worklog

## 2026-10-15 - P6a: the typed date-source port (nomenclator-plan decision 14)

Shape (header approved by Fran): a registry of up to VIII sources, each a
`legere(ctx, prima, ultima, exitus, piscina)` that appends CONCRETE dated
entries for an inclusive range, plus an optional `aperire(ctx, introitus)`
(P6b: click opens the source). The port stamps `fons` with the registered
title, then sorts by day, genus, title (qsort over a flat copy - Xar
storage is not guaranteed contiguous).

Decisions (Fran 10-15): yearly entries are repeated BY THE SOURCE via
`fasti_annuum_expandere` - the port only ever sees concrete dates, so
the recurrence library (decision 15) can replace the helper without
touching the port. 29 Feb in a common year -> 28 Feb.

Bug the test caught on first run: `fasti_fontes_addere` stored the
caller's title pointer; a caller reusing one buffer for several titles
renamed every source. Titles are now copied into the registry's pool.

Limits worth knowing: `personae_fasti_legere` expands at most XVI years
per contact per query (calendar queries span <= 6 weeks).
