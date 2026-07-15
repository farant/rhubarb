# gesta.c worklog

## 2026-07-15 — chunk A birth (one find)

**BIND-NULL TRAP (the chunk's one bug, found by golden 13b):**
`json_ad_chorda` on a missing/empty-context value returns a chorda
with `datum == NIHIL`; `scrinium_ligare_textum` passes `.datum`
straight to `sqlite3_bind_text`, and a NULL pointer there binds SQL
NULL — so restoring the empty `branch_id:""` from an annales line
hit `NOT NULL constraint failed`. Guard `_chorda_tuta` (NIHIL datum
→ "") wraps every json→bind seam in restituere. General law for
future consumers: never feed a json_ad_chorda result to a bind
without the guard — empty-string and SQL-NULL are different animals
at that seam.

Design notes live in gesta/phase-log.md (chunk A INTENTIO: eager
plicare, validation-before-write + custodia nota, state = one JSON
object w/ column projections, canonicalization at the door, annales
before COMMIT, emendatio = whole-replace, genus res_id = titulus,
remotio via object rebuild).
