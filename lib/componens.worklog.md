# componens.worklog.md

## 2026-09-04 — natus (ludus T3)

Logical node: id/actio/titulus/partes/fines/praedicatum/focusabilis +
clip/translate/scale (destinatio needs them; pingere copies them onto
the coetus). STML round-trip, structural equality, find-by-id. Design:
pictor-spec.md §3.1.

Three deviations from the plan draft, all compiler- or crash-taught:
- `ponere_actio` / `ponere_titulum` do NOT alias the caller's literal —
  that needs a `constans character*` → `i8*` cast, refused by
  `-Wcast-qual`. The node carries its `InternamentumChorda*` and
  interns at set time; callers pass any literal, the node keeps the
  canonical copy. Same for the id.
- The label field was drafted as `nomen`. `nomen` is `#define nomen
  typedef` (latina.h) — CLAUDE.md's standing warning, walked into
  anyway ("type name does not allow storage class"). It is `titulus`
  now (field, setter, STML attribute, selector). Lesson: never a
  Latin keyword-macro as an identifier; grep latina.h before naming.
- SEGV at birth: `chorda_internare_ex_literis(intern, "")` returns
  NIHIL (internamentum.c:129 — the interner refuses empty strings)
  and the constructor dereferenced it. Emptiness is a VALUE, not a
  canonical entry: `internare_aut_vacua` returns a zero-length chorda
  over a static byte for "" (and for any NIHIL from the interner),
  with no allocation and no const cast. Found by lldb `bt` in one
  step — exit 139 from the runner says nothing; the backtrace says
  everything.
`PRAEDICATUM_NUMERUS` added so the title table is sized by the enum.
