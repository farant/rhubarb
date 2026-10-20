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
