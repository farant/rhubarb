
## 2026-09-25 — the accessor seam (silva-migratio-plan T7a)

**Why.** Phase 5 makes `SilvaToken` a `MateriaToken` (T7b). Seven fields
have no place in `MateriaToken` — `origo`, `longitudo`, `standard`,
`scissurae` (→ C89 tail), `initium_lineae` (→ materia's `vexilla`),
`spatia_ante/_post` (→ materia's trivia ARRAYS, not `Xar*`). T7a puts
every use of them outside this file behind accessors, representation
unchanged, so T7b changes only accessor bodies, the allocator and the
lexer.

**API added here** (declared in silva_token.h and hand-written in
silva/amalgama/silva.h): `silva_token_origo` (→ MUTABLE `SilvaOrigo*`
from a const token — `(size_t)` round trip, the same deliberate
cast-qual as `materia_token_cauda`), `_longitudo`/`_ponere`,
`_standard`/`_ponere`, `_scissurae`/`_ponere`, `_initium_lineae`/
`_ponere`, trivia `_ante_numerus`, `_ante(t, i)`, `_post_numerus`,
`_post(t, i)`, builders `_ante_addere`/`_post_addere`, and
`_ante_vacare`/`_post_vacare` (test plants). `longitudo` is NOT
`valor.mensura`: a token holding a `\<nl>` continuation has source
bytes its value lacks.

**How the ~430 sites moved.** 321 by a driver (sites from `renominare
-membrum SilvaToken` dry runs; base expression by balanced back-scan;
shapes `x->origo.` → `silva_token_origo(x)->`, reads, writes →
`_ponere`, trivia count/index/NIHIL-test) through one `silva.Refactio`;
~43 by hand — trivia HELPERS that took a whole `Xar*` now take
`(token, post)` (`_emittere_trivia`, `_involucrum_triviorum`,
`_trivia_legere`, `_trivia_fontem_ponere`, `_arbor_trivia_aequalia`,
the amalgama/differre tools' helpers, four test helpers); the lexer's
trivia split and the STML reader build through `_addere` (after
`_vacare`: the old code ASSIGNED a new list, so vacate-then-add keeps
the exact semantics). The lexer was already batch-shaped: it collects
pending trivia and splits it at the first newline — T7b can hand
materia whole arrays at that one point.

**The census has a blind spot: uses inside macro ARGUMENTS.** 63 test
sites inside `CREDO_*(…)` never appeared in `renominare -membrum`'s
list (and so would not have appeared in its "zero left" check
either). Found by a plain text scan over files that see `SilvaToken`,
each hit judged by type (other structs share these names: semantic
diagnostics' `longitudo`, `margo->origo`, productions' `longitudo`,
officina IR `origo`, the frons tail's `standard`/`scissurae`). The
completeness check is therefore TWO angles — semantic census AND the
judged text scan — both at zero. T7b's compile (fields gone) is the
final net.

**Unchanged:** silva 54/54, oracle clean (468+), M3 6/6 (haruspex 281:
T6b's two headers joined its live corpus — a note, not a failure),
shim 402 + 401/401/401, silva amalgam (hospes 40/40) and officina
amalgam (hospes 78/78) regenerated with the accessors — the last
regeneration before the T7b→T13 freeze (decree …1BKY).
