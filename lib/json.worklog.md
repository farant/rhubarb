# json.worklog.md

## 2026-09-22 — audit, step I: two crashes (empty key, unbounded depth)

Fran asked for an audit of json.h/json.c and its tests. Findings and the
five-step plan live in the ledger: park 01M35TSSMM (json audit), tasks
intra it. The suite only ever fed the parser VALID JSON; every finding
was hostile or odd input. A throwaway probe (scratchpad, never
committed) confirmed each suspicion before anything was written.

**Empty key `{"":1}`.** The parser interned the key; `chorda_internare`
refuses "" and returned NIHIL, which went straight into
`JsonPar.clavis`. Parsing reported success. `json_objectum_capere(v, "")`
"worked" only because `chorda_internare_ex_literis("")` ALSO returned
NIHIL and NIHIL == NIHIL. `json_scribere` and the iterator dereference
the key: SIGSEGV.

First attempt fixed internamentum; the radix gate refused it because
"" → NIHIL is a contract actio, insula and STML rely on (see
lib/internamentum.worklog.md — reverted, contract now written and
pinned). The fix lives here instead: `_clavem_internare` /
`_clavem_internare_literis` return ONE static canonical "" (function-
scope `hic_manens`, stable address, datum non-NIHIL) for zero-length
keys and defer to internamentum otherwise. All five interning sites
(parse, capere, capere_chorda, ponere, ponere_chorda) go through them.
They also refuse a NIHIL result (parse = "Allocatio fallita"; capere =
not found; ponere = no-op), so an allocation failure can never plant a
NIHIL key or make two failed lookups "match". Those guards have no test:
only allocation failure reaches them.

**Unbounded recursion.** `_parse_valor` → `_parse_tabulatum` →
`_parse_valor` with no limit: 2,000,000 `[` = SIGSEGV. Every MCP
resident parses client JSON with this, so any client could kill one.
Now `JsonParser.profunditas` counts open containers in `_parse_valor`
(one place for both `{` and `[`); past `JSON_PROFUNDITAS_MAXIMA`
(CCLVI, public in json.h) the parse fails with "Nidificatio nimis
profunda" at the exceeding bracket (line/column asserted). 256 is far
above anything the house writes (tabularium responses nest ~10) and
far below stack danger. Tests: depth = limit passes for arrays AND
objects, limit+1 refused at column limit+1, and the 2,000,000-bracket
attack under CREDO_NON_RUIT.

Born red, observed against the committed json.c: the empty-key block
failed at the NIHIL clavis and the CREDO_NON_RUIT on json_scribere (then
the unguarded call killed the suite, exit 139); with the empty key
healed the depth block showed 7 FRACTA incl. the NON_RUIT. Both green
after the fix.

Note the formatter rewrapped ~20 over-long lines across json.c on the
first structural edit (the file predates the 72-column rule) — cosmetic
diff noise in this commit, not behaviour.

## 2026-09-22 — audit, step II: the whole input is ONE value

`json_legere` parsed the first value and returned success without
looking at what followed: `{"a":1} garbage`, `1 2`, `{"a":1}}`, `[1] @`
(a LEXER error after the root, silently dropped) and `01` (read as 0 —
JSON forbids leading zeros, and this was the only thing standing in for
that rule) all succeeded. The consumer that made it matter:
`gesta_annales_verificare` parses each annal line with json_legere, so
two events glued onto one line (a lost '\n') passed as the first event.

Fix: after the root value, `parser.currens` must be `JSON_TOK_FINIS`;
otherwise "Contentum post valorem radicis" at the first trailing token
(line/column asserted, including a lexer-error token and a multi-line
position). On any failure `res.radix` is now NIHIL (before, a failed
parse could hand back a partial root beside successus FALSUM — no caller
relied on it, all check successus).

Consumer audit BEFORE the change (lesson of step I): every production
call site parses a single value — DB columns written by json_scribere,
MCP arguments, one Content-Length or newline-framed message
(tabellarius, cliens_tabularii cuts at the first '\n', internuntius),
HTTP bodies (manus), one captured response (frigida). Live data
checked with the stricter parser: gesta/annales/forum.jsonl 3640 lines
and tabularium.jsonl 2525 lines, zero refused.

Tests: `probatio_cauda` — 13 refusals via a helper that prints each
input (a FRACTA inside a helper names only the helper's lines), a NUL
byte after the value, and acceptance of all four whitespace kinds on
both sides. Born red: every refusal case failed on all five assertions
before the fix.

## 2026-09-22 — audit, step III: numbers refuse instead of lying

Fran approved all three recommendations.

**Integers past s64** wrapped silently (signed overflow, UB):
`9223372036854775808` → -2^63, 20 digits → garbage. `_parse_integer`
now accumulates the MAGNITUDE in i64 (unsigned) against a limit of
2^63-1 (positive) or 2^63 (negative), with the classic guard
`q > (limes - cifra) / X`; -2^63 is produced as `-(s64)(q - 1) - 1`
to avoid overflow in the negation. Past the limit: "Integer extra s64
(-2^63 .. 2^63-1)" at the number's position. Refusal over float
fallback: a ULID/id-sized integer silently rounded is worse than an
error (refusal-loud).

**Float overflow** (`1e400` → inf) is refused, "Numerus extra f64";
underflow (`1e-400` → 0/denormal) is accepted — nearest value, not a
false one. Detection is `f == ±HUGE_VAL` — the value strtod is
DEFINED to return on overflow. First draft used DBL_MAX from
<float.h>; examen REJECTED it (identificator ignotus): the ISO lexicon
silva/fontes/systema_c89.h has no float.h section. HUGE_VAL is the more
precise test anyway; the lexicon gap is a separate silva task.

**Writer** (`_fluitantem_scribere`): NaN/±inf → `null` (JSON has
neither; the writer has no error channel; JSON.stringify does the same).
Shortest representation that round-trips EXACTLY: try %.15g, %.16g,
%.17g, keep the first that strtod reads back ==, so 0.1 writes "0.1"
not "0.10000000000000001". If the text looks like an integer (no '.',
'e', 'E') append ".0" — `1.0` used to write "1" and re-read as an
INTEGER; the round-trip test also caught -9007199254740992.0 doing the
same. No house code produces floats today (grep: only gesta.c:4679
tests est_fluitans), so no golden moved.

Tests `probatio_numeri`: exact s64 limits both ways (plus writing
-2^63), four integer refusals incl. one inside an array (column
asserted), 1e400 / [-1e400] refused, 1e-400 accepted, NaN/±inf → null,
nine exact writer forms, and seven doubles (0.1, 1/3, DBL_MIN,
DBL_MAX, smallest denormal, 123456.789, -2^53) that must come back
bitwise-equal and still fluitans. Helpers print each case. Born red:
every refusal and every writer form except 2.5 and 1e+20 failed before.
