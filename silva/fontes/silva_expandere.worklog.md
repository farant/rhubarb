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

## 2026-09-25 — `#line` consumed like `#pragma` (silva-migratio T19a)

Found while re-pinning the stale `examen_vectis -corpus` exclusions: 21
of the 22 new rejections were briar-generated files, all failing at
`#line N "x.thistle"`. `#line` is standard C89 (§6.8.4), but the
directive classifier only knew define/undef/include/conditionals/pragma
— everything else was IGNOTA and its tokens flowed into the grammar.
Now SILVA_DIR_LINEA, captured as a directive line and consumed exactly
like `#pragma` (never in the parse stream; `scribere` re-emits it —
byte round-trip pinned). Positions stay PHYSICAL (silva is byte-faithful);
mapping diagnostics through `#line` would be a separate feature. No
oracle-corpus file uses `#line` → no divergence. `#error` (zero house
uses) left as IGNOTA.

Ripple (same day): the first commit was refused by the oratio gate —
the briar fixtures now PARSE, so the identifier index saw their words
for the first time: `summare` (briar fragmenta fixtures, many pinned
sites) → glossary entry `summo` (Medieval Latin, "to sum up") rather
than a rename through pinned fixtures. A fix that makes more files
readable enlarges every census that reads them.

## 2026-10-15 - lazy header lexing (silva_includendum_lexemata)

Profile (percursus, quinta): header provision was 59% of the run, lexing
48% - every caller that hands silva the repository's headers (percursus:
all 552 per file) paid a full lex of each one, though a file includes a
dozen. `silva_includendum_praebere` now only registers the file (fons
index, map keys, text pointer); `silva_includendum_lexemata(exp, incl)`
lexes on the first resolved #include and caches. No new lifetime rule:
tokens were already views into the provided text (silva_lexare never
copies), and all four real callers read into piscina buffers. The guard
check needs no tokens up front - `est_custos` is set by
_plagulam_processare when the file is first processed. silva_scribere's
EOF lookup reads `incl->lexemata` as-is: a header whose tokens are in the
tree was included, therefore lexed; never-included = NIHIL = the old
"no EOF" fallback.

A/B same machine, best of 3 / 2 runs: percursus lib (273 files) 17.2 s
-> 5.9 s wall (2.9x), totality unchanged (273 trees); examen chorda.c CPU
0.16 -> 0.11 s, silva_c89_semantica.c 0.49 -> 0.45 s (examen provides
only the file's closure, so less to skip). silva 55/55.

## 2026-10-15 - indices for the writer's extent lookups and the oracle's responses

`sample` of percursus after the xar clz change: xar ~31% of the run, and
62% of the `xar_obtinere` samples came from three LINEAR pointer-keyed
lookups - silva_scribere `_extentum_quaerere` (identity) and
`_extentum_continens` (containment), where every expanded token of an
object-like macro (si, redde, ZEPHYRUM, Roman numerals) scanned all
function-like invocations twice to find nothing ("numeri parvi" did
not hold for our Latin-macro C), and `silva_oraculum_responsum_quaerere`.

Now: `SilvaExpansio.indices_extentorum` (a pointer to a cache struct, so
lookups through `constans SilvaExpansio*` update it without a cast):
identity = tabula_dispersa keyed by the BYTES of the `invocatio` field
inside the stable xar element (xar never moves elements, so the key
stays valid; the query key is the bytes of the local pointer), first
recorded wins, indexed lazily past a watermark (extenta are append-only
and filled right after xar_addere). Containment = intervals sorted by
(fons, start, record order) + running max of ends per fons; binary
search to the last start <= offset, scan back while the running max
exceeds the offset, keep the minimum record order = exactly the linear
scan's "first recorded". Rebuilt when the count changed. Allocation
failure -> `fractus` -> the linear code (kept as
`_extentum_*_linearis`), never a wrong answer. Oracle: `responsa_per_sedem`
created with responsa, cleared with them, key = bytes of `slot->sedes`.
Public API: `silva_extentum_invocationis`, `silva_extentum_continens`.

Evidence: temporary instrumentation compared every indexed answer with
the linear one and aborted on mismatch - percursus lib + silva/fontes:
~131K identity lookups (plus their containment fallbacks) and ~90K
oracle lookups, zero discrepancies; totality unchanged (273 + 54 trees).
A/B CPU: lib 4.84 -> 4.15 s (-14%), silva/fontes 1.43 -> 1.32 s.

Not done (not hot in the profile): silva_frons `_silvae_extentum` and
silva_arbor `_extentum_laminam_quaerere` are the same linear identity
scan (frons could call silva_extentum_invocationis for expansio->extenta;
extenta_lecta is a separate list).
