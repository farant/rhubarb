# gesta — phase log

Subsystem born 2026-07-15. Kernel = event-sourcing/entity engine over
scrinium/SQLite; first tenant = tabularium (issue/doc store + MCP).
IMPLEMENTATION SOURCE: project-specs/gesta-k1-spec-v2.md (self-
contained; lineage tabularium-visio → tabularium-gradus →
gesta-k1-interrogatio → spec v1 → 3 explorer reports → v2). Never
implement from a compaction summary — re-enter via spec-v2 + this
tail.

## K1 CHUNK A — GESTA CORE: INTENTIO (2026-07-15)

Scope (spec §XV-A): schema migration, write path (transaction trio +
annales line), genera+res folds w/ bootstrap order, refold +
refold-from-annales, goldens 1-9/13-14.

IMPLEMENTATION DECISIONS FRESH AT THIS CHUNK (not in spec-v2 —
settled at build start):

1. **Eager plicare**: gesta_scribere advances all folds immediately
   after commit. Always-materialized ⇒ write-time validation reads
   current state trivially. HWM machinery stays REAL (refold replays
   batches; consumptores advance exactly-once in the fold
   transaction). Volumes make eager free; the MCP resident inherits
   always-fresh reads.
2. **Validation-before-write, violation = second event**: status
   events checked against genus machina BEFORE insert; illegal →
   primary event STILL written + a violation nota (actor=machina,
   origo="gesta-custodia") appended after it. Duplicate creatio on an
   existing res_id = REFUSED (identity, not validation). Malformed
   datum JSON = REFUSED (syntax is not record-don't-block's domain).
3. **State = ONE JSON object including titulus/status keys** —
   the fold is the TS reducer VERBATIM over one object
   (smaragda.ts:723-774); res columns (titulus/status) are
   PROJECTIONS of reserved keys after folding. genus lives in the
   column only (immutable-by-column; a 'genus' key in mutatio is
   just a state key).
4. **Canonicalization at the door**: event datum parsed
   (json_legere) and re-serialized compact (json_scribere) before
   storage — tessellae.datum and annales lines are byte-canonical;
   refolds byte-stable.
5. **Annales line appended BEFORE COMMIT** (fflush+fclose per
   write): crash window leaves annales a SUPERSET (one extra tail
   line) — detectable by gesta_annales_verificare, healable (the
   line is valid; re-append to db). The opposite order would lose
   durable history silently.
6. **emendatio-generis = whole-definition replace.** TS's
   upsert-by-(ex,ad)-pair (smaragda.ts:1819-1822) is an artifact of
   TS's sub-event granularity (genus_transition_defined); our events
   carry complete definitions — whole-value replace is the
   consistent philosophy (same as mutatio arrays). Golden 6 pins
   whole-replace, TS ref documented.
7. **Genus events' res_id = the genus titulus** (readable, stable,
   practically collision-free vs ULIDs; documented in gesta.h).
8. **remotio = object rebuild without the key** (json lib has no
   pair delete; arena-friendly).
9. sordidae marked at write NOW, drained chunk B; nexus/denexus
   events WRITTEN now, folded chunk B — the unknown-kind no-op law
   makes both safe today.
10. scrinium grows the transaction trio
    (incipere/committere/revolvere) — thin over exsequi
    BEGIN/COMMIT/ROLLBACK, the one ergonomic gap the infra explorer
    named.

Bars for this chunk: goldens 1-9, 13-14 green under house flags;
refold == standing folds; annales verificare catches torn AND extra
tail lines; suite runs via gesta/compile_probationes.sh (tessera
template, vendor sqlite3.o relaxed own-object).

## K1 CHUNK A — RELATIO (2026-07-15, same night as the interview)

NAVIS in one pass: 76/76 asserts first-fix (one bug total — the
BIND-NULL trap, gesta.worklog.md: json_ad_chorda NIHIL-datum chorda
binds SQL NULL through scrinium; _chorda_tuta guards the seam;
found by golden 13b restituere, exactly what the golden existed
for). Compile was CLEAN under the full house flag set first try;
examen verdict ACCIPE with zero diagnostics all tiers.

Shipped: gesta/fontes/gesta.{h,c} (aperire/scribere/plicare/
replicare/annales_verificare/ex_annalibus_restituere + reads);
scrinium transaction trio (incipere/committere/revolvere);
gesta/compile_probationes.sh (tessera template + relaxed vendor
sqlite3.o own-object); probatio_gesta.c = goldens 1-9, 13-14 with
'TS: file:NNN' oracle refs throughout.

All chunk-A INTENTIO decisions HELD in contact with the code — no
reversals. Verified live in the goldens: 1-based seq (fresh
consumer at hwm=0 skips nothing), raw-injected events invisible
until plicare then folded (HWM honest), idempotent batch replay,
refold == standing tables, restituere-from-annales == db WITH the
honest divergence case (a raw-injected row has no annales line —
verificare names "acta plura quam lineae"; the restored world
correctly lacks the raw event), torn-tail detection ("lacera"),
duplicate-creatio refusal at the door + fold-reset semantics for
raw duplicates (TS :726 literal), violation notae for illegal
transitions AND bad genus definitions (record-don't-block live).

Suites: gesta 1/1 (76 asserts), scrinium root regression green,
excubitor tail silent. NEXT: chunk B (nexus fold, FTS+sordidae
drain, census, anchor hashes at write).
