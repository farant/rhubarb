
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

## 2026-09-25 — the swap: SilvaToken IS MateriaToken (silva-migratio-plan T7b)

**What changed.** `nomen MateriaToken SilvaToken;` — the struct body is
gone. A private C89 tail `SilvaTokenCauda { origo, longitudo,
standard, scissurae }` rides in the SAME allocation, declared through
`MateriaTokenForma FORMA_C89`; `_cauda_c89(t)` reaches it (mutable
from a const token, as `materia_token_cauda`). The allocator is
`materia_token_creare(…, -I, 0, 0, -I)` (the old memset defaults:
origo FONS, standard C89 = 0). `initium_lineae` lives in materia's
`vexilla`; trivia are materia's EXACT series. Because of T7a's seam,
outside this file only the lexer changed: it hands each side's
trivia over ONCE (`silva_token_ante/post_ponere`, from a stack
buffer of XXXII, arena beyond) — per-trivium `_addere` had raised
allocations.

**What the seam did not cover: `genus`'s TYPE.** Same name, but silva's
field was the enum `SilvaLexemaGenus` and materia's is `s32`. 33 sites
passing/returning/assigning it as the enum failed `-Wsign-conversion`
(the T7 measurement had named the numbering, not the type). One
accessor `silva_token_genus(t)` (enum view of materia's s32 — the
lexicon is generated FROM this enum, so the numbering is identical by
construction) at those reads; `(s32)` at the four constructor writes.
The ~260 other `genus` uses compare against enum constants and needed
nothing. A line-wide replacement also wrapped a `SilvaProspectus`
field on the same line (`expandere.c:1800`) — caught by the compiler
at once.

**Tooling trap.** With silva not compiling, `silva.Editio`'s by-name
operations fail (they need the formator, which is built from silva):
the genus fix went in as plain writes, and formator + examen ran over
every file once silva compiled again.

**Two include lists missed `materia/fontes`**: the silva runner's
per-test compile (`probatio_una`'s own `INC`, separate from
`INCLUDE_FLAGS`) and the live oracle's compile line.

**Cost, measured (computus gold regenerated for this cause):** on
`arrays.c` lexing bytes 188,416 → 133,992 (−29%), allocations 8,661 →
8,453, total use −5%; `base64.c` lexing −30%, allocations 18,696 →
17,827. Arena slack rises (fewer bytes, same blocks).

**Behaviour: unchanged.** Oracle clean over 470 files (emission, STML,
errors, semantics identical to pinned silva; every pinned document
reads back), M3 6/6, shim 402 + 401/401/401, silva 54/54. The amalgam
is FROZEN from here to T13 (decree …1BKY; note atop silva/CLAUDE.md).

**Where the bytes went (Fran asked; measured by a probe, 2026-09-25).**
NOT the token: old `SilvaToken` 128 bytes; new `MateriaToken` 64 + C89
tail 64 = 128 (`SilvaOrigo` alone is 48; `longitudo`, `standard`, the
`scissurae` pointer and padding make the other 16 — `materia_token.h`'s
own estimate of a 48-byte tail / 112 total was optimistic). The saving
is TRIVIA storage: a `Xar` holding one trivium costs 272 bytes; materia's
exact series costs 8. Nearly every token carries a trivium on some
side, so that overhead was paid almost everywhere — hence lexing −29%
and fewer allocations. A design choice ("set once, never appended")
inherited for free, not faster code. The tail could be packed later
(e.g. `standard` into materia's flag bits); not worth it next to this.

**Include gaps, swept rather than met one by one.** After the per-test
compile and the live oracle, officina's five scripts (colloquium,
compile_probationes, legatus, sonda, vindex) compiled
`silva_lexicon.c` without `-I materia/fontes` (the T7b commit's
officina gate stopped it). A sweep of every tracked script that
passes silva's source headers found no others outside the frozen
amalgam tooling (`tools/amalgama_excludenda_generare.sh`,
`tools/amalgama_ligare.sh` — they run only inside silva's
`amalgamare.sh`; T13 owes them). And the SHARED amalgamation mechanism
(`tools/mechanismus_fontes.sh`, header: "nobody knew because nobody
ran them") broke exactly as its header warns: officina/ and
tessera/amalgamare.sh compile silva's lexer, which now needs
`materia_token`. Fixed (include + `mech_materia_token.o` + header
guard); both regenerate their committed amalgams byte-identical.
