# mcp_servus worklog

## 2026-10-20 - born as the third server (mcp-remotus T3)

Ledger decision ...TKJ0: legatus and tabularium each COPIED the MCP
skeleton; the library is extracted when the third server (exemplar)
arrives. Those two stay as they are for now - moving them is out of
sub-project B.

**Shape.** One JSON-RPC body in, exactly one `reddere` call out (now or
later), even for notifications (`MCP_REDITUS_NUNTIATIO`, empty body) -
so a transport never has to guess whether to wait. The core knows no
HTTP: it says RESPONSUM / NUNTIATIO / SCOPUS / PRAVUM and the transport
maps them (200 / 202 / 403 / 400).

**Every call takes a slot,** synchronous ones too. The slot is freed and
its generation bumped BEFORE `reddere` runs (reddere may start a new
call). 64 calls pending -> the next call gets -32603 "nimis multae
vocationes pendentes" before its handler runs. A stale `McpDilatum`
(answered, or slot reused) gives `mcp_dilatum_vocatio` -> NIHIL. A raw
`McpVocatio*` kept past its answer is NOT protected once the slot is
reused - always go back through the handle.

**Order inside tools/call:** unknown tool -> scope -> arguments. Scope
before arguments on purpose: a caller without the scope learns nothing
about the argument rules.

**Argument checks** (Review focus 4): absent `arguments` or `null` ->
`{}`; `null` for an argument = absent; `params._meta` never looked at;
a JSON number for a string, `2.5` for an integer -> -32602 naming the
argument. Getters only ever see validated values.

**Schema:** `required` is omitted when empty (draft-04 validators want
minItems 1), `additionalProperties: false` always, annotations always
both present. Golden `tools/list` was written by hand before the code
(probationes/fixa/mcp_servus/instrumenta_index.json, sha256 pinned in
the test) and matched on the first build.

**stdio:** one piscina per line; a deferred line keeps its piscina and
`_stdio_reddere` destroys it after writing. Write failures of deferred
replies are not reported (the loop may be gone).

Gotcha: `structura` is a latina.h macro (`struct`) - the plan's own
signature `mcp_respondere_json(v, JsonValor* structura)` failed to
parse; the parameter is `valor`.

## 2026-10-20 - HTTP transport (mcp-remotus T4)

`lib/mcp_servus_http.c` mounts POST `<via>` (+ GET/DELETE -> 405
`Allow: POST`) and both PRM paths on a hospitium.

**Memory: each call gets its OWN piscina.** The colloquium's piscina is
recycled the moment the connection ends - a deferred call living there
would write into the next request's memory after the client hangs up.
So the body is copied into a fresh piscina, the core allocates there,
and the reply body is copied into the colloquium piscina just before
`colloquium_respondere`. Sync: destroyed after `mcp_servus_tractare`;
deferred: `_reddere` destroys it after answering (or after seeing the
client gone via `dilatum_colloquium` -> NIHIL, logged as status=0).

**Bug found by the acta test:** the 401 log line came out EMPTY - it was
built in the colloquium piscina AFTER `colloquium_respondere`, which had
already recycled it. Rule: log first, respond last.

**Check order** in POST: Origin -> `?access_token=` (400) -> Bearer +
verifier (401) -> size (413; hospitium's own cap may be bigger, e.g.
exemplar uploads, so we check `corpus_via`/length ourselves) ->
Content-Type (415, `; charset` allowed) -> MCP-Protocol-Version.

**Unsupported version** body follows the official 2026-07-28 lifecycle
page: `{"code":-32022,"message":"Unsupported protocol version",
"data":{"supported":[ours],"requested":"..."}}`, request id echoed, HTTP
400. A dual-era client reads that as "modern server, retry with a
supported version"; we only speak the initialize era.

**PRM URL** is derived from the ORIGIN of the audience URL
(`https://h/mcp` -> `https://h/.well-known/...`), not from the issuer.

**Access line** per request: `mcp status=N usor= cliens= instrumentum=
exitus= ms=` - core now reports `instrumentum`/`exitus` in McpReditus.
Tokens never appear (planted: token-as-usor turns `acta` red).

Gotcha: BSD sed has no `\b` - `sed 's/\bFOO\b/BAR/g'` silently changes
nothing. Use python `re`.
