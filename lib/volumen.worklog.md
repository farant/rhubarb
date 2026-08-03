# volumen worklog

## 2026-08-04 — birth (G2 seed, silex as first consumer)

Born as the smallest true slice of the G2 design (see
project-specs/compages-rationes-tutorial.md gap table + ledger res
01KZ4Y9A8T): one sqlite file = one document, with `acta` (append-only
event log, the truth), `massae` (content-addressed blobs by sigillum
hex, INSERT OR IGNORE gives dedup for free), and `plagulae` (the
manifest projection, upsert by via). Deliberately NOT built here yet:
genera, custodia, personae, replication — those arrive on-pull per the
tutorial's G3/G7/G8.

Decisions worth remembering:
- **Timestamps stamped in SQL** (`strftime('%Y-%m-%dT%H:%M:%fZ','now')`
  as column DEFAULT) — the gesta.c precedent. No C time API, no POSIX,
  no fasti dependency. ISO text like the annales.
- **plagulam_condere is one transaction** wrapping massa + actum +
  manifest upsert (the gesta K1 law: the event write wraps the whole
  road). Failure path = single `revolve:` label + rollback.
- **Empty files are legitimate**: promere distinguishes "not found"
  (b32* inventum) from "found, empty" (chorda vacua). scrinium got a
  blob pair (ligare_massam / columna_massa) for this arc;
  sqlite3_bind_zeroblob(0) covers the empty-blob case (a NIHIL datum
  pointer would otherwise bind SQL NULL — measured trap, comment in
  scrinium.c).
- Migration array had to be renamed MIGRATIONES_VOLUMINIS — leading
  `_` + capital is implementation-reserved (C89 7.1.3), house lint
  caught it at edit time.
