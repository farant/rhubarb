# silva_expandere worklog

## 2026-09-25 — `__attribute__` as a built-in empty macro (silva-migratio T17a; lapide bugs/010)

Before: `__attribute__` was a plain identifier. Prototype-suffix and
leading positions died in the GLR; worse, `struct s {...}
__attribute__((packed));` parsed SILENTLY as a function declarator named
`__attribute__` taking `(packed)` — the error only surfaced on the next
line, or never.

Decision (Fran): accept the extension; the house rule (T17b) names its
use. Route chosen after measuring both: an erasing macro, not a grammar
node. An empty function-like expansion already leaves a
`SilvaExtentumInvocationis` with `vacua` set — its bytes go to
reinserenda and the projection emits `<invocatio-vacua>` (a canon
element, queryable). examen's `tolera` matches by LINE of the node's
first token's root, so a node-less finding can be absorbed by a
per-line variant (T17b). The grammar form is parked (…WFM6): it only
pays when silva must model attribute semantics (packed/aligned change
sizeof; cleanup, noreturn change flow).

Where: `_def_internum_quaerere` supplies a lazily created def
(`exp->def_attributi`) at the expansion loop's lookup ONLY when the
macro table has none:
- NOT in the table: `silva_conditio.c` evaluates `defined()` through
  `silva_expansio_quaerere`, and clang answers FALSE (a keyword, not a
  macro). Test pins `#ifdef` and `#if defined(...)`.
- NO synthetic `<api>` source: `_def_api_creare` would have created one
  (shifting later source indices) in every file that uses an attribute.
  fons_index -1 + ex_api VERUM, the header's documented "from API" form;
  consumers of def->fons_index already skip < 0.
- NO definition event in `acta`.
- A user `#define __attribute__(x) …` wins (table first).
One parameter suffices: `((a, b(c)))` is ONE argument (commas nest).

Costs, all named: sizeof(SilvaExpansio) +8 → computus gold regenerated
(usus/apex +8, otiosa −8, allocations and lexemes unchanged).
syntaxis_v1 corpus 123 → 125: line 2's `__extension__` passes the
SYNTAX layer only (read as an unknown type name; semantics rejects it
— `__extension__` is NOT accepted). Oracle 470 unchanged (no house file
uses an attribute), M3 6/6.
