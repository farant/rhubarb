# mcp remotus - a remote MCP server Claude signs in to (spec)

**Status: SPEC 2026-10-19** - written after brainstorming with Fran;
awaiting his review, then a plan.

*Fran 2026-10-19, after hospitium v3 reached main (ff e8cad828): "start
looking up information about the lunch money api and see what kinds of
gaps we might have in our mcp implementation layers if we wanted to
have an oauth authenticated mcp server that was a wrapper for lunch
money that could be used by claude desktop/mobile app", then: work
through the MCP layers first and deploy an MCP app reachable from
Claude, Lunch Money as a second pass. Ledger: desideratum ...VNA
(remote MCP + OAuth, gap list), under the Forum-online park ...AAAA
whose 2026-08-03 note already named "OAuth MCP from the VPS ->
budgeting MCP" as the driving need.*

This is **sub-project B** of four (A pecunia - `project-specs/
pecunia-spec.md`; B this; C the household budgeting app on A + B; D
Lunch Money: `fiducia_linux`, a client library, sync).

Goal: **Claude - on the web, Desktop and the phone app - can add one of
our apps as a custom connector, have each person sign in as
themselves, and call its tools.** The proving app is `exemplar`, live
at `https://exemplar.golden.earth`: its notes become MCP tools. Every
piece in between is reusable: an MCP core shared by every house MCP
server, an HTTP transport on hospitium, and an OAuth 2.1 authorization
server any app can mount.

Decisions (Fran, 2026-10-19, in conversation):
- **Who uses it (for C, shaping B):** one shared household budget;
  three ways in - each person's Claude, and our own phone-friendly web
  app for each of Fran and Amanda. **Per-person logins over shared
  data**: whoever signs in on the consent page is the identity on that
  Claude connection, so writes carry a verified author ("Amanda, via
  Claude"), never a claimed one.
- **Proving app = add MCP to `exemplar`** (approach 1 of three): it
  already has users, logins (custodia) and storage, so multi-user
  identity is proved end to end.
- **Scopes: read and write are separate** (`notae:legere`,
  `notae:scribere`); in practice both are always granted (the 401 names
  both, the consent boxes default to ticked), but read-only and step-up
  work.
- **Three pieces wired by the app** (approach 1 of three): `lib/
  mcp_servus` (core + transports), `lib/admissio` (OAuth server + token
  check), and the app. Not one combined library (OAuth would be trapped
  inside MCP), not a gateway process (premature with one app).
- **Defaults:** client registration by **DCR only** in B (CIMD needs
  outbound HTTPS from the droplet, which waits on `fiducia_linux`, D);
  the **initialize-era protocol** (2025-06-18 / 2025-11-25) first,
  settled by a probe of what Claude actually sends, 2026-07-28 added to
  the same core later.
- **Opaque tokens, not JWT:** the authorization server and the MCP
  server share one process and one volumen, so checking a token is a
  hashed lookup; revocation is instant and the JWT failure modes
  (`alg: none`, algorithm confusion, key rotation) do not exist. JWT
  becomes useful only if a gateway or an outside service must trust our
  tokens (section IV).

## I. What exists (quarta == main e8cad828, 2026-10-19)

- **Two hand-written MCP servers over stdio:**
  `officina/instrumenta/legatus.c` (initialize ~5420, dispatch
  ~8100: `initialize`, `ping`, `tools/list`, `tools/call`, "nondum
  initiatum" before initialize) and `gesta/fontes/tabularium.c`
  (initialize ~14496, dispatch ~14697, tool schemas composed from
  `TabArgumentum` tables ~13647 with unknown-argument checks, ledger
  ...7S). **Ledger decision ...TKJ0 (2026-07-15): the MCP skeleton was
  COPIED into tabularium, not extracted - `lib/mcp_servus` is born with
  the THIRD server.** exemplar is the third.
- **tabellarius** (`include/tabellarius.h`): JSON-RPC 2.0 envelope
  (`tabellarius_nuntium_legere`, `_responsum`, `_errorem`) and both
  framings (Content-Length for LSP, lines for MCP stdio).
- **hospitium v3** (`include/hospitium.h`): router
  (`hospitium_praebere`), deferred responses (`colloquium_differre`
  -> `HospitiumDilatum` value handle with a generation counter, 503 at
  the deadline; ~374-387), shared reactor, request helpers, proxy
  headers trusted only from loopback, header-injection guard.
- **custodia** (`include/custodia.h`): argon2id password jobs,
  sessions (`custodia_sessionem_creare`), CSRF
  (`custodia_csrf`/`_verificare`), Origin check
  (`custodia_origo_congruit`), throttle (`custodia_frenum`), server
  secret. It already encodes 32 bytes as base64url privately
  (`lib/custodia.c:100-131`, `_signa_tuta`); random bytes come from
  `moneta_octeti_fortuiti` (`include/moneta.h:51`).
- **base64** (`include/base64.h`): standard alphabet only - no
  base64url, no unpadded decode.
- **sigillum** (SHA-256) and HMAC; constant-time compare in custodia.
- **volumen** (`include/volumen.h`): event log (`acta`) + blobs, and
  `volumen_scrinium` (:87) for the app's own SQLite tables.
- **exemplar** (`apps/exemplar/`): routes registered at
  `exemplar_nucleus.c:1467-1486` (`/inscribere`, `/intrare`, `/exire`,
  `/notae`, `/notae/:id/delere`, `/fila/:sigillum`, `/salus`); events
  `usor.creatus`, `tessera.mutata`, `sessio.creata`/`.deleta`,
  `nota.scripta`/`.deleta`; **notes are per user** (`Nota.usor`,
  ~363-380); flags include `-hospes`. Deployed on smaragda behind nginx
  (`apps/exemplar/exemplar.nginx`: one `location /` proxying every path
  to 127.0.0.1:3010; certbot added the TLS part on the server).

**External facts (fetched 2026-10-19):**
- Claude's connector auth (`https://claude.com/docs/connectors/building/
  authentication`): sign-in starts **only on a 401** (WWW-Authenticate
  on a 200 is ignored); `resource` in protected-resource metadata must
  equal the URL as entered; only `authorization_servers[0]` is used;
  DCR supported by default, CIMD only if the server advertises
  `client_id_metadata_document_supported: true` AND `"none"` in
  `token_endpoint_auth_methods_supported`; PKCE S256 always;
  `offline_access` appended when listed in `scopes_supported`; hosted
  apps redirect to exactly `https://claude.ai/api/mcp/auth_callback`,
  Claude Code to `http://localhost:<any>/callback` and
  `http://127.0.0.1:<any>/callback`; token endpoint form-urlencoded,
  10 s limit (30 s for refresh); refresh reactive on 401 and up to 5
  minutes early; dead refresh token must get `invalid_grant`;
  public-client refresh tokens rotated; egress from `160.79.104.0/21`.
- MCP 2026-07-28 removed `initialize` and sessions (per-request
  `_meta`, headers `Mcp-Method`/`Mcp-Name` mirrored, `-32020
  HeaderMismatch`), and tells modern clients to fall back when a server
  answers `400` without a recognised modern error. Claude's docs still
  cite the 2025-11-25 spec.

## II. Design

### II.1 `lib/mcp_servus` - the core

```c
nomen enumeratio {
    MCP_CHORDA = 0, MCP_INTEGER, MCP_BOOLEAN
} McpGenus;                       /* nulli fluitantes: pecunia ut
                                   * chorda "-12.34" */
nomen structura {
    constans character*  titulus;
    constans character*  descriptio;
               McpGenus  genus;
                    b32  necessarium;
    constans character* constans* electiones;  /* NIHIL = liber */
} McpArgumentum;

nomen structura {
    constans character*  titulus;        /* "notam_addere" */
    constans character*  descriptio;
    constans McpArgumentum* argumenta;
                    i32  numerus;
    constans character*  scopus;         /* NIHIL = nullus */
                    b32  legere_solum;   /* readOnlyHint */
                    b32  delens;         /* destructiveHint */
           McpTractator  tractator;
} McpInstrumentum;
```

- **A static tool table** generates `tools/list` (`inputSchema` and
  annotations) and validates every `tools/call` **before** the handler
  runs: unknown key, missing required, wrong type, value outside
  `electiones` - each refused with a named reason as a JSON-RPC
  `-32602` whose message names the argument. Handlers see only valid
  arguments.
- **`McpVocatio`** (one per call) gives the handler: the person
  (`mcp_vocatio_persona`: user id, granted scopes, client id; stdio
  servers get a fixed local person), typed getters
  (`mcp_argumentum_chorda(v, "titulus", &t)`, `_integer`, `_boolean`,
  with presence), a per-request piscina, and the app's context pointer.
- **Results:** `mcp_respondere_textum(v, t)`, `mcp_respondere_vitium(v,
  t)` (`isError: true` - the tool ran and failed, as distinct from a
  protocol error), and a JSON variant for structured content.
- **Deferred results:** `mcp_differre(v)` returns `McpDilatum`, a value
  handle with a generation counter (the `HospitiumDilatum` pattern);
  answering through a stale handle does nothing. Over HTTP it rides
  `colloquium_differre`; over stdio the reply is written when the
  handle completes.
- **Scopes are enforced by the core**: if the person lacks a tool's
  `scopus`, the handler never runs and the transport reports it its own
  way (HTTP 403 below; stdio a JSON-RPC error). `tools/list` lists every
  tool regardless - step-up relies on the client trying.
- **Transport boundary:** `mcp_servus_tractare(servus, corpus, persona,
  ...)` takes one JSON-RPC body and yields one reply body (now or
  later). The core knows nothing of HTTP or stdio.
- **Protocol:** `initialize` (echoes a supported `protocolVersion`, else
  our newest; `capabilities.tools`, `serverInfo`, `instructions`),
  `ping`, `tools/list`, `tools/call`; notifications accepted and
  ignored. Over stdio "initialize first" stays enforced; over HTTP each
  request stands alone (no `Mcp-Session-Id` is ever issued - optional in
  the 2025 era, gone in 2026-07-28).
- **stdio transport** in the same library, over `tabellarius` lines.
  Moving legatus and tabularium onto it is NOT part of B (section IV);
  B proves it with its own tests.

### II.2 HTTP transport and the token check

`mcp_servus_http_montare(hospitium, "/mcp", servus, &optiones)`:

- **POST only.** `GET`/`DELETE` -> `405` (the 2025-era answer for a
  server that offers no server-to-client stream; SSE is not in B).
  Body must be `application/json`, at most 1 MB (`413`). JSON-RPC
  batches refused (2025-06-18 removed them).
- **`MCP-Protocol-Version`** must be supported (2025-03-26,
  2025-06-18, 2025-11-25); absent = 2025-03-26 (spec-permitted). A
  `2026-07-28` request gets `400` with a JSON-RPC
  `UnsupportedProtocolVersion` error listing ours - the fallback signal
  the 2026 spec defines - so a newer client negotiates down.
- **Replies:** one `application/json` body per request; notifications
  `202` with no body.
- **Origin:** checked against a configured allow-list when present,
  else `403` (Claude calls server-to-server and normally sends none).
- **Behind nginx:** scheme and host from proxy headers only when the
  peer is loopback (hospitium v3 helpers).
- **Every request needs `Authorization: Bearer`**, `initialize`
  included (Claude starts sign-in only on a 401). A token in the query
  string is refused. Missing or invalid:

  ```
  401
  WWW-Authenticate: Bearer resource_metadata="https://exemplar.golden.earth/.well-known/oauth-protected-resource", scope="notae:legere notae:scribere"
  ```

  Insufficient scope:

  ```
  403
  WWW-Authenticate: Bearer error="insufficient_scope", scope="notae:scribere", resource_metadata="https://exemplar.golden.earth/.well-known/oauth-protected-resource"
  ```

- **Validation:** `admissio_tesseram_verificare(admissio, tessera,
  resource, &persona)` - person or refusal; the token's audience must
  equal the canonical resource URL.
- **Protected-resource metadata** (RFC 9728) at
  `/.well-known/oauth-protected-resource` **and**
  `/.well-known/oauth-protected-resource/mcp` (the path Claude probes
  if the header is lost): `resource` = exactly
  `https://exemplar.golden.earth/mcp`, `authorization_servers` = [the
  issuer], `scopes_supported` = the app's scopes,
  `bearer_methods_supported` = `["header"]`.
- **Logging:** user, client, tool, outcome, duration. Tokens are never
  logged, echoed in errors, or stored unhashed.

### II.3 `lib/admissio` - the OAuth 2.1 server

- **Metadata** (RFC 8414) at `/.well-known/oauth-authorization-server`:
  `issuer`; `authorization_endpoint`, `token_endpoint`,
  `registration_endpoint`; `response_types_supported: ["code"]`;
  `grant_types_supported: ["authorization_code", "refresh_token"]`;
  `code_challenge_methods_supported: ["S256"]`;
  `token_endpoint_auth_methods_supported: ["none"]`;
  `scopes_supported` = the app's scopes + `"offline_access"`;
  `authorization_response_iss_parameter_supported: true`. No CIMD flag
  in B.
- **`/oauth/register`** (DCR, RFC 7591, JSON): `redirect_uris`,
  `client_name`, public client; HTTPS redirect URIs exact, plus
  loopback `http://127.0.0.1` / `http://localhost` with any port.
  Issues a random `client_id`. Rate-limited (Claude registers on every
  fresh connection); clients never used in a completed sign-in are
  pruned after a day.
- **`/oauth/authorize`** (GET page, POST decision):
  - Checks: registered `client_id`; `redirect_uri` registered, exact
    (loopback: port ignored); `response_type=code`;
    `code_challenge_method=S256` with a challenge; `resource` = our MCP
    URL; scopes known. A bad client or redirect gets an **error page,
    never a redirect** (no open redirect); other errors redirect with
    `error`, `state` and `iss`.
  - Not logged in: the app's **login callback** runs (exemplar: custodia
    session, else `/intrare?redire=...`). admissio never sees passwords.
  - **Consent page:** client name, the **redirect host shown
    prominently** ("claude.ai"; an extra warning when the only redirect
    URIs are loopback), one checkbox per requested scope, ticked by
    default; CSRF and Origin checked through custodia.
  - Approve -> a **code**: single use, 60 seconds, bound to client,
    redirect URI, PKCE challenge, `resource`, user and granted scopes;
    redirect with `code`, `state`, `iss`.
- **`/oauth/token`** (form-urlencoded, answered well inside 10 s):
  - `authorization_code`: verifier against the stored S256 challenge;
    redirect URI, `resource` and client must match; the code is burned.
    **A replayed code revokes everything issued from it.**
  - Issues an **access token** (1 hour) and a **refresh token** (30
    days), opaque, audience-bound, stored hashed; the response carries
    the granted `scope`.
  - `refresh_token`: **rotation** - the old token dies when the new one
    is issued; presenting a rotated-out token revokes the whole grant
    family (theft detection). Dead -> `invalid_grant` (RFC 6749 code).
- **Storage** in the app's volumen through `volumen_scrinium`: tables
  for clients, codes, tokens (by hash) and grants (user x client x
  scopes, created, last used, revoked). Codes and tokens are churning
  state, so they are rows; **grant created / revoked are also appended
  as events** for the audit trail.
- **For the app's pages:** `admissio_concessiones(admissio, usor)` (the
  person's connections) and `admissio_revocare(admissio, concessio)`
  (kills a grant and every token in it, effective on the next request).
- **Secrets:** codes and tokens are 32 bytes from
  `moneta_octeti_fortuiti`, written as base64url without padding;
  stored as SHA-256; compared in constant time.
- **base64url** joins `include/base64.h` (encode and decode, padded and
  unpadded, RFC 4648 section 5); custodia's private `_signa_tuta` is
  replaced by it.

### II.4 exemplar as the first remote MCP server

| Tool | Scope | Does |
|---|---|---|
| `notas_enumerare` | `notae:legere` | the caller's notes, newest first, `pagina` |
| `notam_legere` | `notae:legere` | one note: text + attachment names and sizes |
| `notam_addere` | `notae:scribere` | add a note |
| `notam_delere` | `notae:scribere` | delete a note (`delens`: Claude confirms) |

- Tools touch **only the caller's notes**, through the token's user.
  Attachments over MCP (resources / binary content) are not in B.
- **Attribution:** events written through MCP carry `via: "claude"` and
  the client id; web events carry `via: "retis"`.
- **Routes:** `POST /mcp`; the three well-known documents;
  `/oauth/authorize`, `/oauth/token`, `/oauth/register`;
  **`/applicationes`** - the connected-apps page (client name, scopes,
  created, last used, revoke), linked from `/notae`.
- **Login with return:** `/intrare?redire=<path>` returns after login;
  only local paths are accepted (no open redirect).
- **Configuration:** issuer `https://<hospes>` and resource
  `https://<hospes>/mcp` from the existing `-hospes` flag.
- **Deploy:** `tools/dimittere.sh` as in hospitium v3 (dry run first,
  Fran's OK before each push). DNS unchanged. The nginx site already
  proxies every path; check on the server that certbot's
  `/.well-known/acme-challenge` handling does not swallow the OAuth
  well-known paths.

### II.5 Order of work (shape for the plan)

1. **Probe first:** an exemplar build whose `/mcp` answers the correct
   401 and logs the headers and body of Claude's first unauthenticated
   request; deployed; Fran adds the connector once. Settles the
   protocol era with evidence. The rest goes ahead regardless; this
   check is the gate.
2. base64url; `mcp_servus` core + stdio; HTTP transport; `admissio`;
   exemplar wiring and pages.
3. Local end-to-end with Claude Code as the client, then deploy, then
   the phones.

## III. Tests

- **`probatio_mcp_servus`** (credo v2): `tools/list` against a golden
  JSON file; every validation refusal by name; a refused scope never
  reaches the handler; deferred replies, stale handles inert; JSON-RPC
  errors (parse, invalid request, unknown method, batch refused);
  `initialize` version echo; stdio transport in process.
- **`probatio_mcp_servus_http`** (hospitium in process, real HTTP on
  loopback): **exact** `WWW-Authenticate` text for 401 and 403
  `insufficient_scope`; `405` GET; `202` notification; `400` with the
  supported list for `2026-07-28`; `403` bad Origin; `413` over 1 MB;
  token in the query string refused; both protected-resource paths.
- **`probatio_admissio`** - the whole flow in process: register ->
  authorize (test login callback) -> consent POST with CSRF -> code ->
  token -> MCP call -> refresh with rotation. Attacks: reused refresh
  token revokes the family; code replay; PKCE mismatch; wrong
  `resource`; redirect URI prefix or other port refused (loopback port
  excepted); expired code; revocation through `admissio_concessiones`.
- **Byte-exact oracles:** RFC 7636 Appendix B (verifier
  `dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk` -> challenge
  `E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM`); RFC 4648 section 10
  vectors for base64url.
- **exemplar:** tools see only the caller's notes (Amanda's token cannot
  read Fran's); `via: "claude"` in events; `/intrare?redire=` refuses
  non-local targets.
- **Born-red plants** (`silva.planta`), each caught by a named test:
  PKCE check skipped; audience check skipped; refresh not rotated;
  redirect URI prefix-matched; scope check skipped; a token written to a
  log line.
- **End to end:** Claude Code against a local exemplar (a real
  independent client, no npm); a live smoke `tools/exemplar_mcp_fumus.sh`
  that never signs in (`/mcp` -> 401 with the right header; both
  metadata documents parse and agree on resource and issuer;
  `/oauth/register` works; `GET /mcp` -> 405); then Fran and Amanda
  connecting from their phones and checking `/applicationes`.
- **Linux:** the new suites under `./tools/linux/probatio.sh -musl` and
  `-glibc`; exemplar's static musl build as in hospitium v3.

## IV. Not in this spec

- **CIMD** (needs `fiducia_linux`, sub-project D) and the **2026-07-28
  stateless era** (added to the same core when Claude speaks it).
- **SSE**, server-to-client requests, `subscriptions/listen`, resources,
  prompts, attachments over MCP.
- **JWT / token introspection** (RFC 7662) - only if a gateway or an
  outside service must trust our tokens.
- **Moving legatus and tabularium onto `mcp_servus`** - optional later
  work; their behaviour stays as is in B.
- The budgeting app (C) and Lunch Money (D).
- Session idle limit for web logins (hospitium v3 R23) - unchanged here.

## VERIFIED by the probe (T1, 2026-10-20 03:44-03:45 UTC)

Fran added the connector from the Claude desktop app; exemplar ran with
`-sonda`. Two attempts, 39 s apart:

- **Protocol: the initialize era.** `POST /mcp` with body
  `{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-11-25","capabilities":{},"clientInfo":{"name":"Anthropic","version":"1.0.0"}}}`
  and header `mcp-protocol-version: 2025-11-25` already on the
  initialize request. `Accept: application/json, text/event-stream`,
  no Authorization, no Origin, no Cookie.
- **Who calls:** `User-Agent: python-httpx/0.28.1` from 160.79.106.169
  and 160.79.106.164 (inside 160.79.104.0/21) - the desktop app's
  connector runs from Anthropic's servers, not the user's machine.
- **Discovery order:** POST /mcp -> 401 -> GET
  `/.well-known/oauth-protected-resource` (the URL in our
  `resource_metadata`, not the `/mcp`-suffixed path) -> GET
  `/.well-known/oauth-authorization-server` (RFC 8414 path; no OIDC
  fallback tried). Both discovery GETs also carry
  `mcp-protocol-version`.
- **First attempt stopped after discovery** (no register call); the
  second one registered. Observed, not explained.
- **DCR body** (277 bytes, `Content-Type: application/json`):
  `{"redirect_uris":["https://claude.ai/api/mcp/auth_callback"],"token_endpoint_auth_method":"none","grant_types":["authorization_code","refresh_token"],"response_types":["code"],"scope":"notae:legere notae:scribere offline_access","client_name":"Claude","application_type":"web"}`
  - the scope is the AS document's `scopes_supported` (with
  `offline_access`), not the PRM's; no `client_uri`, no `logo_uri`, no
  `software_id`.
- **nginx:** certbot's live config passes every `/.well-known/oauth-*`
  path to the app (one `location /`); checked by curl from outside.

## AUDIENDA (not verified)

- Whether `resource` arrives on authorize and token exactly as
  documented, and whether Claude re-registers (DCR) on every reconnect
  in practice - observed at first sign-in (T6/T9).
- That Claude Code accepts an `http://localhost` MCP server with OAuth
  for the local end-to-end check (else a TLS terminator in front, or
  test against the deployed site).
- Exact line numbers in section I are from e8cad828 and drift.
