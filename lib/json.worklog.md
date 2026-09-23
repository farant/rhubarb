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
