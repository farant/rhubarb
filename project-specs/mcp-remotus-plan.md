# mcp remotus plan - a remote MCP server Claude signs in to

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> subagents. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Claude (web, Desktop, phone) adds `https://exemplar.golden.earth/mcp`
as a custom connector; Fran and Amanda each sign in as themselves and use
their own notes through four tools; every piece is reusable.

**Architecture:** three pieces wired by the app. `lib/mcp_servus` is the
transport-independent MCP core (typed tool table, argument validation,
scopes, deferred results) with a stdio and a Streamable HTTP transport on
hospitium; the HTTP transport checks bearer tokens through a CALLBACK, so
it never depends on the OAuth server. `lib/admissio` is the OAuth 2.1
authorization server (metadata, DCR, consent, codes, tokens, refresh
rotation, revocation) storing hashed opaque tokens in the app's volumen.
`apps/exemplar` declares the note tools, mounts both, and adds a
connected-apps page.

**Tech stack:** C89 + latina.h; hospitium v3 (router, deferred responses,
request helpers, proxy headers), reactor, tabellarius (JSON-RPC + line
framing), json, custodia (sessions, CSRF, Origin, throttle), volumen +
scrinium (SQLite tables), sigillum (SHA-256, constant-time compare),
moneta (random bytes), base64 (+ base64url, new), credo v2; oracles: RFC
7636 Appendix B (PKCE), RFC 4648 section 10 (base64url), Claude Code as an
independent MCP + OAuth client, Claude's apps themselves; Linux via
`tools/linux/` (OrbStack); deploy via `tools/dimittere.sh` to smaragda.

**Spec:** `project-specs/mcp-remotus-spec.md` (quarta 0b24b94a). Read it
with this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh`; identifiers,
  comments, messages Latin; never a latina.h macro as an identifier
  (`magnitudo` = sizeof, `nomen` = typedef, Roman numerals...); `i32`/
  `i64` UNSIGNED, `s32`/`s64` signed; chorda not NUL-terminated.
- POSIX .c files include `postulata_posix.h` FIRST; no raw
  `fopen/open/stat/access` outside filum (examen code 96).
- **Tokens, codes, refresh tokens, client secrets, cookies and passwords
  are never logged, never echoed in errors or pages, never stored
  unhashed** (SHA-256 at rest, constant-time compare). The `-sonda` log
  redacts `Authorization` and `Cookie` values and never logs `/oauth/token`
  bodies.
- HTTP transport: POST only (GET/DELETE 405), JSON replies only (no SSE),
  no `Mcp-Session-Id` issued, body <= 1 MB, batches refused, supported
  `MCP-Protocol-Version` 2025-03-26 / 2025-06-18 / 2025-11-25 (absent =
  2025-03-26), `2026-07-28` -> 400 `UnsupportedProtocolVersion` listing
  ours; every request needs `Authorization: Bearer`; token in query
  string refused.
- 401: `WWW-Authenticate: Bearer resource_metadata="<base>/.well-known/oauth-protected-resource", scope="notae:legere notae:scribere"`;
  403: `WWW-Authenticate: Bearer error="insufficient_scope", scope="<needed>", resource_metadata="..."`
  - byte-exact, asserted.
- `resource` = exactly `https://exemplar.golden.earth/mcp` (from
  `-hospes`); issuer = `https://exemplar.golden.earth`.
- OAuth: PKCE S256 only; public clients (`token_endpoint_auth_methods
  ["none"]`); DCR; redirect URIs exact, plus loopback `http://127.0.0.1`
  / `http://localhost` any port; codes single-use 60 s; access 1 h;
  refresh 30 d rotated, reuse revokes the family; dead refresh ->
  `invalid_grant`; `/oauth/token` form-urlencoded, answered well inside
  10 s; `iss` in authorization responses.
- Droplet `smaragda` is brownfield: only exemplar's own user, dirs, unit
  and nginx site; `nginx -t` before reload; every remote change dry-run
  first, executed only after Fran's OK.
- Commits via detached `silva.commissio` (absolute-path worktree guard);
  formator + Latin lint before; plants via `silva.planta`; a mute plant
  is a test gap fixed before commit; NO installs from the worktree; main
  untouched until Fran's go.

## Rulings made while planning

- **P1 - the token check is a callback.** `mcp_servus_http` takes a
  verifier function `(datum, tessera, resource, persona*) -> b32` in its
  options; `admissio_verificator` is one such function. The MCP core and
  its transports therefore build and test without any OAuth code, and a
  future app could check static keys instead.
- **P2 - the protected-resource metadata belongs to the transport** (it
  knows the resource URL, the issuer and the scopes); the authorization-
  server metadata belongs to admissio.
- **P3 - OAuth routes are outside exemplar's POST rule.** exemplar
  checks Origin on every POST and CSRF after login; `/oauth/register`
  and `/oauth/token` are called by Claude's SERVERS (no Origin, no
  cookie) and `/mcp` carries a bearer token. Only `/oauth/authorize`
  POST (the consent form, a browser page) keeps Origin + CSRF.
- **P4 - the probe is permanent code.** T1 adds exemplar's real
  discovery routes (static JSON at first, replaced by the libraries'
  output in T4/T5) and a `-sonda` flag that logs, redacted, every request
  to `/mcp`, `/.well-known/*` and `/oauth/*`. The flag stays (default
  off) as the diagnostic for "what is Claude sending?".
- **P5 - deferred results** live in a fixed slot table in the servus
  (`MCP_VOCATIONES_MAXIMAE` = LXIV) with a generation counter per slot,
  the `HospitiumDilatum` pattern; the HTTP transport defers the
  colloquium when the handler defers.
- **P6 - grant state is rows, grant lifecycle is events.** Clients,
  codes, tokens and grants are admissio's own SQLite tables in the app's
  volumen (`volumen_scrinium`); exemplar also appends `concessio.creata`
  / `concessio.revocata` events for the audit trail.
- **P7 - exemplar grows a second source file.** `exemplar_nucleus.c` is
  1522 lines; MCP tools and OAuth wiring go in `apps/exemplar/
  exemplar_mcp.c`, with the shared structs moved to an internal header
  `apps/exemplar/exemplar_internum.h`.
- **P8 - test HTTP client** follows the house precedent (raw TCP
  helpers copied per suite, `probatio_hospitium_auxilia.c:26-90`), since
  `http_exsequi` blocks and the server runs on the same thread.
- **P9 - plan location** `project-specs/mcp-remotus-plan.md` (house).
- **P10 - the public base URL is its own flag.** The spec (II.4) says
  issuer and resource come "from the existing `-hospes` flag", but in
  exemplar `-hospes` is the LISTEN address (127.0.0.1 behind nginx). A new
  `-basis https://exemplar.golden.earth` sets the public base; issuer =
  basis, resource = basis + `/mcp`. Without it (local tests) the base is
  `http://<hospes>:<portus>`.

## Rulings made while executing

- **R1 (T1)** - the probe code is committed BEFORE the deploy (plan had
  the commit as the last step): the binary on smaragda then matches a
  gated commit. Findings from Claude's first connection go into a small
  follow-up commit (worklog + spec AUDIENDA). `tools/dimittere.sh`
  gained `-cum-unitate` (installs the unit, `.prior` + daemon-reload,
  restored with the binary on a failed health check) since the unit
  gains `-basis` and `-sonda`; the plan did not name this step.
- **R2 (T2)** - custodia cannot call `base64url_codificare`: it encodes
  into the fixed `CustodiaSessio.signum[XLIV]` and into stack buffers on
  the CSRF verify path, with no piscina. `base64.h` gains a fourth,
  piscina-free primitive `base64url_scribere(datum, mensura, exitus,
  capacitas)` (no NUL; capacity short -> 0, nothing written) plus
  `base64url_longitudo_codificata`; `base64url_codificare` is built on
  it and custodia's `_signa_tuta` shrinks to a two-line wrapper adding
  the NUL. CSRF output pinned BEFORE the switch (`csrf_fixum`, value
  from Python's hmac + base64) and unchanged after.
- **R3 (T2)** - the decoder also refuses non-zero leftover bits ("Zh"
  and "Zg" would both decode to `f`), so every byte string has exactly
  one base64url spelling - matters when a decoded value is compared or
  hashed. `""` decodes to a non-NIHIL empty result (not an error), so
  the 0-length round trip holds. The old `base64.c` was never
  formatted; touching it ran the formatter over the whole file
  (banners to 50, two ternaries reflowed, `c >= 0` dropped - `i8` is
  unsigned).
- **R4 (T3)** - the plan's `mcp_respondere_json(v, JsonValor*
  structura)`: `structura` is a latina.h macro; parameter is `valor`.
- **R5 (T3)** - insufficient scope also carries a JSON-RPC error body
  (stdio needs one): code `MCP_ERROR_SCOPUS` = -32003 (server range;
  -32002 is taken by "nondum initiatum"), message `scopus deest: <s>`.
  Scope is checked before arguments.
- **R6 (T3)** - every tools/call takes a slot, not only deferred ones,
  so "the 65th call" fails with -32603 before its handler runs and the
  handle check is one code path. `reddere` is called exactly once per
  body, notifications included. `required` omitted when empty;
  `isError` always present (false on success). The persona's chordae
  are copied into the request piscina (deferred calls outlive the
  transport's copy).
- **R7 (T4)** - `mcp_servus_http_montare` takes a `Piscina*` (hospitium
  exposes none; options, PRM body and header strings are copied there).
- **R8 (T4)** - `McpHttpOptiones.sonda` became an access-log writer
  (`acta` callback + `acta_datum`, NIHIL = stderr): one line per request
  `mcp status= usor= cliens= instrumentum= exitus= ms=`. exemplar keeps
  its own `-sonda` for raw request capture.
- **R9 (T4)** - `McpReditus` gains `instrumentum` and `exitus` (bene /
  vitium / error / scopus / pravum / nuntiatio) so the transport can log
  the tool and outcome without re-parsing; pinned by T3 section
  `exitus` + a plant.
- **R10 (T4)** - `resource` (plan field and verifier parameter) is
  `audientia` - "resource" is real English and may not enter the
  glossary; the JSON key stays `"resource"`.
- **R11 (T4)** - each HTTP call runs in its own piscina (the
  colloquium's is recycled when the client leaves); access line written
  BEFORE responding. Unsupported-version body = official 2026-07-28
  shape (-32022, data.supported/requested). PRM URL from the audience's
  origin.
- **R12 (T5)** - admissio creates its tables with `CREATE TABLE IF NOT
  EXISTS` + its own `admissio_versio` row: `scrinium_migrare`'s single
  `_migrationes` counter is already the volumen's.
- **R13 (T5)** - names: `AdmissioOptiones.resource` -> `audientia`
  (R10), `admissio_metadata` -> `admissio_documentum` (lint: real
  English). Test hook is always compiled (lib defines
  `ADMISSIO_PROBATIO` before its header), declared only for callers that
  define it.
- **R14 (T5)** - registration limit `ADMISSIO_INSCRIPTIONES_PER_MINUTUM`
  = 20 per IP, checked before parsing, refused ones not stored, 429
  `too_many_requests`; `client_id` 16 random bytes; token family = grant
  id; `admissio_concessiones` lists live grants only; `admissio_putare`
  also purges expired codes and tokens. Loopback = `127.0.0.1` and
  `localhost` (not `[::1]`).

## Review focus

Inputs no task's spec-derived tests would otherwise reach, most likely to
bite first:

1. **Claude reconnecting over and over:** every fresh connection
   registers a new client (DCR). Registration is rate-limited per IP,
   unused clients are pruned after a day, and a burst of 50 registrations
   neither fills memory nor breaks existing grants.
2. **An access token expiring mid-conversation:** the next call gets the
   401, Claude refreshes, the new access token works, the old refresh
   token is dead - and a refresh exactly at the expiry second behaves the
   same.
3. **Revoking on `/applicationes` while Claude is in use:** the very next
   `/mcp` call is 401 and the refresh attempt gets `invalid_grant`; the
   other person's connection is untouched.
4. **Tool arguments as clients really send them:** `arguments` absent
   (tool with no required args), `params._meta` present (ignored), a JSON
   number where a string is declared, `null` for an optional argument
   (treated as absent), unknown keys (refused, named).
5. **The consent page reached the wrong way:** no query parameters, a
   tampered `redirect_uri`, a missing or stale CSRF token, logged out in
   another tab between GET and POST - an error page, never a redirect,
   and no code issued.

Pinned by tests: 1 -> T5 `turba_inscriptionum`; 2 -> T6 `expiratio`;
3 -> T7 `revocatio_viva`; 4 -> T3 `argumenta_vera`; 5 -> T6
`consensus_malus`.

---

### Task 1: probe - discovery routes, the 401, and `-sonda`

**Files:** Create `apps/exemplar/exemplar_internum.h` (structs moved out
of `exemplar_nucleus.c`), `apps/exemplar/exemplar_mcp.c`. Modify
`apps/exemplar/exemplar_nucleus.c`, `apps/exemplar/exemplar_nucleus.h`
(config: `sonda`, `basis` = public base URL), `apps/exemplar/exemplar.c`
(flags `-sonda`, `-basis` - P10), `apps/exemplar/
exemplar.service` (flag only if Fran wants it on), `apps/exemplar/
exemplar.worklog.md`, `probationes/probatio_exemplar_applicatio.c`.

**Interfaces - Produces:**
```c
/* exemplar_nucleus.h (configuratio) */
    constans character* basis;   /* "https://exemplar.golden.earth";
                                  * NIHIL = http://<hospes>:<portus> */
                   b32  sonda;   /* acta petitionum MCP/OAuth ad stderr */
/* exemplar_mcp.c (internum, per exemplar_internum.h) */
vacuum exemplar_mcp_praebere (Exemplar* e);   /* viae MCP + OAuth */
```

- [ ] **Step 1: Failing test** (`probatio_exemplar_applicatio.c`, new
  sections):
  - `sonda_401`: `POST /mcp` with a JSON-RPC `initialize` and no
    Authorization -> 401 with the exact `WWW-Authenticate` header of the
    global constraints (base from config); same with a wrong scheme
    (`Basic ...`).
  - `sonda_metadata`: `GET /.well-known/oauth-protected-resource` and
    `.../oauth-protected-resource/mcp` -> 200 JSON with `resource` =
    `<basis>/mcp`, `authorization_servers` = [`<basis>`],
    `scopes_supported` = [`notae:legere`, `notae:scribere`],
    `bearer_methods_supported` = [`header`]; `GET
    /.well-known/oauth-authorization-server` -> 200 JSON with `issuer`,
    the three endpoints under `/oauth/`, `code_challenge_methods_supported`
    [`S256`], `token_endpoint_auth_methods_supported` [`none`],
    `grant_types_supported`, `response_types_supported` [`code`],
    `scopes_supported` (+ `offline_access`),
    `authorization_response_iss_parameter_supported` true.
  - `sonda_acta`: with `sonda` on, a request carrying `Authorization:
    Bearer SECRETUM` and `Cookie: sid=SECRETUM` writes one log line naming
    method, path, `MCP-Protocol-Version`, `User-Agent`, and the body
    (up to 4 KB) - and the string `SECRETUM` appears NOWHERE in the
    captured stderr; with `sonda` off nothing is logged.
  - `sonda_register`: `POST /oauth/register` (JSON body) is logged and
    answered 503 `{"error":"temporarily_unavailable"}` (the probe stops
    Claude here on purpose; T5 replaces it).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement:** move `Exemplar`, `Usor`, `Sessio`, `Nota`,
  `Opus` into `exemplar_internum.h`; `exemplar_mcp_praebere` registers
  the routes; metadata built with `json` (no hand-written JSON strings);
  the sonda logger as a small function every MCP/OAuth handler calls
  first (redaction by header name, case-insensitive).
- [ ] **Step 4: Run - expect PASS**; existing exemplar sections stay
  green (the move must not change behaviour).
- [ ] **Step 5: Plants:** redaction of Authorization removed ->
  `sonda_acta` red; `scope=` dropped from the 401 header -> `sonda_401`
  red.
- [ ] **Step 6: Linux static build** (`tools/linux/aedificare.sh
  exemplar`) and **deploy** with `-sonda` on: `tools/dimittere.sh
  -siccum exemplar smaragda` shown to Fran, `-agere` on his OK. Check on
  the server that `curl -sI https://exemplar.golden.earth/.well-known/
  oauth-protected-resource` is 200 JSON (certbot's
  `/.well-known/acme-challenge` block must not swallow it - spec
  AUDIENDA).
- [ ] **Step 7: Fran adds the connector** in claude.ai (Settings ->
  Connectors -> Add custom connector, URL
  `https://exemplar.golden.earth/mcp`). Read the sonda lines
  (`journalctl -u exemplar`, read-only): record in the worklog the
  protocol version Claude sends (header and body), its `clientInfo`,
  the paths it fetched in order, and its DCR body (`redirect_uris`,
  `grant_types`, `token_endpoint_auth_method`, `scope`). **Gate:** if
  Claude speaks `2026-07-28` and does not fall back, STOP and re-plan
  T3/T4 with Fran before continuing.
- [ ] **Step 8: Commit** `exemplar: sonda MCP/OAuth - viae inventionis (metadata RFC 9728/8414), 401 cum WWW-Authenticate, acta redacta (-sonda); structurae in exemplar_internum.h (mcp-remotus T1)`.

---

### Task 2: base64url

**Files:** Modify `include/base64.h`, `lib/base64.c`, `lib/custodia.c`
(uses it instead of `_signa_tuta`); Create `probationes/
probatio_base64url.c`.

**Interfaces - Produces:**
```c
/* RFC 4648 sectio 5: '-' '_', SINE '=' */
chorda        base64url_codificare (constans i8* datum, i32 mensura,
    Piscina* piscina);
/* sine '=' tantum; '+' '/' '=' aut longitudo % 4 == 1 -> datum NIHIL */
Base64Fructus base64url_decodificare (chorda codificata, Piscina* piscina);
```

- [ ] **Step 1: Failing test:** RFC 4648 section 10 vectors (`""`, `f`
  `Zg`, `fo` `Zm8`, `foo` `Zm9v`, `foob` `Zm9vYg`, `fooba` `Zm9vYmE`,
  `foobar` `Zm9vYmFy` - unpadded) both ways; bytes `FB FF` -> `-_8`
  (the two characters that differ from base64); refusals: `Zm9v=`, `Zm+v`,
  `Zm/v`, length 5 (`Zm9vY`); round trip of 1000 random byte strings
  (lengths 0..64, fixed `sors` seed); **RFC 7636 Appendix B:** verifier
  `dBjftJeZ4CVP-mB92K27uhbUJU1p1r_wW1gFWFOEjXk` -> SHA-256 ->
  base64url = `E9Melhoa2OwvFrEMTJguCHaoeK1t8URWbuGJSstw-cM` (byte exact);
  custodia's CSRF token for a fixed secret and session is unchanged by
  the switch (assert the value computed before the change).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement**; replace custodia's `_signa_tuta` with
  `base64url_codificare` (32 bytes -> 43 chars, same output).
- [ ] **Step 4: Run - expect PASS**; `probatio_custodia` and exemplar
  green.
- [ ] **Step 5: Plants:** `'-'` and `'+'` swapped in the alphabet ->
  `FB FF` vector red; padding accepted -> refusal red.
- [ ] **Step 6: Commit** `base64url (RFC 4648 sec. 5, sine '='): codificare/decodificare, RFC 7636 App. B octetim; custodia eo utitur (mcp-remotus T2)`.

---

### Task 3: `mcp_servus` - core and stdio

**Files:** Create `include/mcp_servus.h`, `lib/mcp_servus.c`,
`lib/mcp_servus.worklog.md`, `probationes/probatio_mcp_servus.c`,
`probationes/fixa/mcp_servus/instrumenta_index.json` (golden
`tools/list`).

**Interfaces - Produces:**
```c
#define MCP_VOCATIONES_MAXIMAE LXIV
nomen enumeratio { MCP_CHORDA = ZEPHYRUM, MCP_INTEGER, MCP_BOOLEAN } McpGenus;
nomen structura {
    constans character* titulus; constans character* descriptio;
    McpGenus genus; b32 necessarium;
    constans character* constans* electiones;    /* NIHIL-terminata */
} McpArgumentum;
nomen structura McpVocatio McpVocatio;
nomen vacuum (*McpTractator)(McpVocatio* vocatio);
nomen structura {
    constans character* titulus; constans character* descriptio;
    constans McpArgumentum* argumenta; i32 numerus;
    constans character* scopus;                  /* NIHIL = nullus */
    b32 legere_solum; b32 delens; McpTractator tractator;
} McpInstrumentum;
nomen structura {
    chorda usor; chorda cliens;
    chorda scopi;                                /* spatiis separati */
} McpPersona;
nomen structura {
    constans character* titulus; constans character* versio;
    constans character* instructiones;
    constans McpInstrumentum* instrumenta; i32 numerus;
    vacuum* datum;                               /* applicationis */
} McpConfiguratio;
nomen enumeratio {
    MCP_REDITUS_RESPONSUM = ZEPHYRUM,  /* corpus JSON-RPC */
    MCP_REDITUS_NUNTIATIO,             /* sine corpore (202) */
    MCP_REDITUS_SCOPUS,                /* scopus deest: 403 / error */
    MCP_REDITUS_PRAVUM                 /* JSON-RPC malum (400 + corpus) */
} McpReditusGenus;
nomen structura {
    McpReditusGenus genus; chorda corpus;
    constans character* scopus_requisitus;
} McpReditus;
nomen vacuum (*McpReddere)(vacuum* datum, constans McpReditus* r);
nomen structura McpServus McpServus;
nomen structura { McpServus* servus; i32 locus; i32 generatio; } McpDilatum;

McpServus* mcp_servus_creare (constans McpConfiguratio* c, Piscina* p);
/* VERUM si responsum iam redditum; FALSUM = dilatum (reddere postea) */
b32 mcp_servus_tractare (McpServus* s, chorda corpus,
    constans McpPersona* persona, b32 initium_postulatum,
    McpReddere reddere, vacuum* datum_reddendi, Piscina* p);
constans McpPersona* mcp_vocatio_persona (McpVocatio* v);
vacuum*  mcp_vocatio_datum (McpVocatio* v);
Piscina* mcp_vocatio_piscina (McpVocatio* v);
b32 mcp_argumentum_chorda (McpVocatio* v, constans character* t, chorda* e);
b32 mcp_argumentum_integer (McpVocatio* v, constans character* t, s64* e);
b32 mcp_argumentum_boolean (McpVocatio* v, constans character* t, b32* e);
vacuum mcp_respondere_textum (McpVocatio* v, chorda textus);
vacuum mcp_respondere_vitium (McpVocatio* v, chorda textus);  /* isError */
vacuum mcp_respondere_json (McpVocatio* v, JsonValor* structura);
McpDilatum  mcp_differre (McpVocatio* v);
McpVocatio* mcp_dilatum_vocatio (McpDilatum d);   /* NIHIL si irritum */
/* stdio: lineae tabellarii, initium postulatum, persona fixa */
s32 mcp_servus_stdio_currere (McpServus* s, FILE* in, FILE* ex,
    constans McpPersona* persona);
```

- [ ] **Step 1: Failing test** sections (a test servus with three tools:
  `legere` (`notae:legere`, args `id` integer required), `scribere`
  (`notae:scribere`, `textus` string required, `color` string with
  `electiones` {rubrum, viride}), `differre` (answers later)):
  - `index`: `tools/list` result equals the golden JSON byte for byte
    (`inputSchema` with `type`, `properties`, `required`,
    `additionalProperties: false`, `enum`; annotations `readOnlyHint` /
    `destructiveHint`).
  - `argumenta`: unknown key, missing required, string for integer,
    value outside `electiones` -> JSON-RPC error `-32602` whose message
    names the argument; the handler never ran (counter).
  - `argumenta_vera` (Review focus 4): `arguments` absent for a tool
    without required args -> handler runs; `params._meta` present ->
    ignored; `null` for an optional argument -> treated as absent; a JSON
    number for a declared string -> `-32602`.
  - `scopi`: persona with `notae:legere` only calls `scribere` ->
    `MCP_REDITUS_SCOPUS` with `scopus_requisitus = "notae:scribere"`,
    handler not run; `tools/list` still lists all three.
  - `dilatum`: `differre` returns FALSUM from `mcp_servus_tractare`;
    answering through the handle calls `reddere` once with the result;
    a second answer and an answer after the slot was reused (generation)
    do nothing; 65 concurrent deferrals -> the 65th call answers with an
    internal error, not a crash.
  - `rpc`: parse error `-32700` (id null), invalid request `-32600`,
    unknown method `-32601`, a batch `[...]` -> `MCP_REDITUS_PRAVUM`,
    a notification -> `MCP_REDITUS_NUNTIATIO`, `ping` -> `{}`.
  - `initium`: `initialize` with a supported `protocolVersion` echoes it;
    an unsupported one gets our newest (2025-11-25); `capabilities.tools`,
    `serverInfo`, `instructions` present; with `initium_postulatum`,
    `tools/list` before `initialize` -> `-32002`; without it (HTTP) it
    answers.
  - `stdio`: through a pipe pair, three lines in -> three lines out,
    `initialize` first enforced, EOF ends the loop with 0.
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** (extracted from `officina/instrumenta/
  legatus.c` ~5420/~8100 and `gesta/fontes/tabularium.c` ~13647/~14697 -
  their code stays as is in B): parse with `tabellarius_nuntium_legere`;
  schema generation from the table; validation before dispatch; result
  `{"content":[{"type":"text","text":...}],"isError":...}` (+
  `structuredContent` for JSON); slot table per P5.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** scope check skipped -> `scopi` red; unknown
  argument accepted -> `argumenta` red; generation not compared ->
  `dilatum` red.
- [ ] **Step 6: Commit** `mcp_servus: nucleus MCP extractus (servus tertius, decretum ...TKJ0) - tabula instrumentorum typata, inputSchema, iudicium argumentorum, scopi, responsa dilata, stdio (mcp-remotus T3)`.

---

### Task 4: `mcp_servus_http` - Streamable HTTP on hospitium

**Files:** Create `include/mcp_servus_http.h`, `lib/mcp_servus_http.c`,
`probationes/probatio_mcp_servus_http.c`. Modify `lib/mcp_servus.worklog.md`.

**Interfaces - Consumes:** T3. **Produces:**
```c
nomen b32 (*McpVerificator)(vacuum* datum, chorda tessera,
    chorda resource, McpPersona* exitus, Piscina* p);
nomen structura {
    constans character* via;           /* "/mcp" */
    constans character* resource;      /* "https://.../mcp" */
    constans character* auctoritas;    /* issuer */
    constans character* scopi;         /* "notae:legere notae:scribere" */
    constans character* constans* origines;   /* NIHIL-terminata; NIHIL =
                                               * nulla Origin licet */
    McpVerificator verificator; vacuum* verificatoris_datum;
    b32 sonda;
} McpHttpOptiones;
b32 mcp_servus_http_montare (Hospitium* h, McpServus* s,
    constans McpHttpOptiones* o);   /* /mcp + duo PRM viae */
```

- [ ] **Step 1: Failing test** (hospitium in process, raw TCP client per
  P8, test verifier accepting `bona-legere` / `bona-omnia` tokens):
  - exact 401 header (no token, `Basic`, unknown token); exact 403
    header (`bona-legere` calling `scribere`); `?access_token=` in the
    URL -> 400 even with a good header.
  - `GET /mcp` -> 405 (`Allow: POST`); `DELETE` -> 405; notification ->
    202 empty; batch -> 400; body > 1 MB -> 413; wrong Content-Type ->
    415.
  - `MCP-Protocol-Version: 2026-07-28` -> 400 with the JSON-RPC
    `UnsupportedProtocolVersion` body listing our three; `2025-06-18` ->
    200; absent -> 200.
  - `Origin: https://evil.example` -> 403; no Origin -> allowed;
    an allowed origin -> allowed.
  - both PRM paths -> identical JSON; `Mcp-Session-Id` sent by the
    client -> ignored, never echoed.
  - a deferred tool answers on a later `hospitium_gressus`; the client
    disconnecting before the answer -> no crash, slot freed.
  - `acta`: each call writes one access line with user, client, tool,
    outcome and duration; the bearer token's text appears nowhere in the
    captured stderr (also not for a 401 with a bad token).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** on `hospitium_praebere` (POST + GET/DELETE
  handlers for the 405s), `colloquium_caput`, `colloquium_differre`.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** audience (`resource`) not passed to the
  verifier -> a test verifier that checks it turns red; `scope=` missing
  from 403 -> red; query-string token accepted -> red; the bearer token
  written into the access line -> `acta` red.
- [ ] **Step 6: Commit** `mcp_servus_http: Streamable HTTP super hospitium - POST solum, JSON, sine sessionibus, 401/403 octetim, PRM, versio protocolli, Origo, verificator per functionem (mcp-remotus T4)`.

---

### Task 5: `admissio` I - storage, metadata, registration, tokens

**Files:** Create `include/admissio.h`, `lib/admissio.c`,
`lib/admissio.worklog.md`, `probationes/probatio_admissio.c`.

**Interfaces - Consumes:** T2 (base64url), sigillum, moneta, volumen.
**Produces:**
```c
nomen structura {
    constans character* auctoritas;      /* issuer, sine '/' finali */
    constans character* resource;        /* "https://.../mcp" */
    constans character* constans* scopi; /* NIHIL-terminata */
    s64 vita_codicis_ms;      /* 0 = LX s */
    s64 vita_accessus_ms;     /* 0 = I h */
    s64 vita_renovationis_ms; /* 0 = XXX d */
    s64 (*nunc_ms)(vacuum);   /* NIHIL = tempus verum (probationes) */
} AdmissioOptiones;
nomen structura Admissio Admissio;
nomen structura {
    chorda id; chorda cliens; chorda titulus_clientis; chorda usor;
    chorda scopi; s64 creata_ms; s64 usus_ultimus_ms; b32 revocata;
} AdmissioConcessio;
Admissio* admissio_creare (Volumen* v, constans AdmissioOptiones* o,
    Piscina* p);                       /* tabulas creat si desunt */
chorda admissio_metadata (Admissio* a, Piscina* p);   /* RFC 8414 JSON */
/* corpus JSON RFC 7591 -> responsum JSON + status (201 aut 400) */
s32 admissio_inscribere (Admissio* a, chorda corpus, chorda cliens_ip,
    chorda* responsum, Piscina* p);
b32 admissio_verificator (vacuum* a, chorda tessera, chorda resource,
    McpPersona* exitus, Piscina* p);   /* McpVerificator */
Xar* admissio_concessiones (Admissio* a, chorda usor, Piscina* p);
b32  admissio_revocare (Admissio* a, chorda usor, chorda concessio);
s32  admissio_putare (Admissio* a);    /* clientes non usi > I d */
```

- [ ] **Step 1: Failing test:**
  - `metadata`: exact JSON (issuer, endpoints, S256, `none`, grants,
    `code`, scopes + `offline_access`, `iss` flag) parsed back and
    compared field by field.
  - `inscriptio`: `{"redirect_uris":["https://claude.ai/api/mcp/auth_callback"],"client_name":"Claude","token_endpoint_auth_method":"none","grant_types":["authorization_code","refresh_token"]}`
    -> 201 with a `client_id`; refused 400 `invalid_redirect_uri`:
    `http://claude.ai/...` (not https, not loopback), a URI with a
    fragment, no `redirect_uris`; refused `invalid_client_metadata`:
    `token_endpoint_auth_method: client_secret_basic`; loopback
    `http://127.0.0.1/callback` and `http://localhost:3118/callback`
    accepted.
  - `turba_inscriptionum` (Review focus 1): 50 registrations from one IP
    within a minute -> the ones past the limit get 429; `admissio_putare`
    after a simulated day removes the unused clients, keeps clients
    with grants.
  - `tesserae`: storage holds only hashes (scan every table row: no
    stored value equals an issued token or code); verifier accepts a
    fresh access token for the right resource, refuses the wrong
    resource, an expired one (injected clock), a revoked grant's;
    `admissio_concessiones` lists a grant with client name and scopes;
    `admissio_revocare` by another user -> FALSUM.
  (Token ISSUING is exercised here through an internal test hook
  `admissio_concessionem_creare_probationis` declared only under
  `ADMISSIO_PROBATIO`; T6 issues them through `/oauth/token`.)
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement:** tables `admissio_clientes`, `admissio_codices`,
  `admissio_tesserae` (hash, genus accessus/renovatio, concessio,
  expirat, familia, usa), `admissio_concessiones`; 32 random bytes per
  secret, base64url, SHA-256 stored; constant-time compare on lookup by
  hash (hash is the key; compare again with `sigillum_aequale`).
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** token stored in plain -> `tesserae` red;
  resource not compared -> red; https check on redirect URIs removed ->
  `inscriptio` red.
- [ ] **Step 6: Commit** `admissio I: metadata RFC 8414, inscriptio dynamica RFC 7591 (frenata, putata), tesserae opacae digestae, verificator, concessiones/revocare (mcp-remotus T5)`.

---

### Task 6: `admissio` II - authorize, consent, token endpoint

**Files:** Modify `include/admissio.h`, `lib/admissio.c`,
`lib/admissio.worklog.md`, `probationes/probatio_admissio.c`.

**Interfaces - Consumes:** T5, custodia, hospitium. **Produces:**
```c
/* app login: VERUM + usor si sessio valida; aliter app redirigit */
nomen b32 (*AdmissioIntratus)(vacuum* datum, HospitiumColloquium* col,
    chorda* usor, chorda* csrf);
b32 admissio_montare (Admissio* a, Hospitium* h,
    AdmissioIntratus intratus, vacuum* datum,
    constans character* via_intrandi);   /* "/intrare" */
/* /.well-known/oauth-authorization-server, /oauth/register,
 * GET+POST /oauth/authorize, POST /oauth/token */
```

- [ ] **Step 1: Failing test** - the whole flow in process (test login
  callback; PKCE pair from RFC 7636 App. B):
  - `fluxus`: register -> `GET /oauth/authorize?...` logged in -> consent
    page (200, contains the client name, `claude.ai` as the redirect
    host, two checked boxes, a CSRF field; a client whose ONLY redirect
    URIs are loopback gets the extra local-program warning, spec II.3) -> POST approve -> 302 to the
    redirect URI with `code`, `state`, `iss` -> `POST /oauth/token`
    (form) -> JSON `access_token`, `token_type: Bearer`, `expires_in`
    3600, `refresh_token`, `scope` -> the access token passes
    `admissio_verificator` -> `grant_type=refresh_token` -> new pair,
    old refresh dead.
  - `insidiae`: code replay -> `invalid_grant` AND the tokens from the
    first exchange revoked; PKCE verifier wrong -> `invalid_grant`;
    `resource` different -> `invalid_target`; redirect URI with extra
    path, other host, other port (non-loopback) -> error page (400, no
    Location); loopback with another port -> accepted; reused (rotated)
    refresh token -> `invalid_grant` and the whole family revoked;
    `code_challenge_method=plain` -> refused; unknown `client_id` ->
    error page.
  - `expiratio` (Review focus 2): code after 61 s -> `invalid_grant`;
    access token at expiry + 1 ms -> verifier FALSUM; refresh at its exact
    expiry second -> refused.
  - `consensus_malus` (Review focus 5): GET without parameters -> error
    page; POST without/with stale CSRF -> 403, no code; POST after the
    session ended -> redirect to login, no code; deny button -> 302 with
    `error=access_denied` + `state` + `iss`.
  - `scopi_electi`: approve with only `notae:legere` ticked -> token
    scope `notae:legere`; `offline_access` requested -> refresh issued;
    not requested -> no refresh token.
  - `forma`: `/oauth/token` with a JSON body -> 400
    `invalid_request` (form-urlencoded required); answered in < 100 ms
    on the Mac (not a benchmark: a guard against an accidental argon2
    or network wait).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** the routes on hospitium; consent page HTML
  through exemplar-independent minimal markup (escaped); Origin + CSRF
  on the consent POST only (P3); `Cache-Control: no-store` on token
  responses; errors per RFC 6749 section 5.2.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** PKCE comparison skipped -> `insidiae` red;
  refresh not rotated -> red; redirect URI prefix-matched -> red;
  family not revoked on reuse -> red.
- [ ] **Step 6: Commit** `admissio II: authorize cum consensu (scopi electi, CSRF, Origo), token (PKCE S256, rotatio, revocatio familiae, invalid_grant), iss (mcp-remotus T6)`.

---

### Task 7: exemplar - the third MCP server

**Files:** Modify `apps/exemplar/exemplar_mcp.c`, `apps/exemplar/
exemplar_nucleus.c` (`/intrare?redire=`, `/applicationes`, events with
`via`), `apps/exemplar/exemplar_internum.h`, `apps/exemplar/exemplar.c`,
`apps/exemplar/exemplar.worklog.md`,
`probationes/probatio_exemplar_applicatio.c`.

**Interfaces - Consumes:** T3-T6.

- [ ] **Step 1: Failing test** sections (full stack in process):
  - `instrumenta`: the sign-in appended `concessio.creata` (user, client
    name, scopes); Fran's token: `notam_addere` -> event `nota.scripta`
    with `via: "claude"` and the client id; `notas_enumerare` (page 1,
    newest first, 20 per page) shows it; `notam_legere` returns text +
    attachment names and sizes; `notam_delere` removes it.
  - `alieni`: Amanda's token cannot read, list or delete Fran's notes
    (`notam_legere` of Fran's id -> tool error "nota non inventa", not
    a leak).
  - `revocatio_viva` (Review focus 3): revoke Fran's grant on
    `/applicationes` (POST with CSRF) -> event `concessio.revocata`; his
    next `/mcp` call 401, his refresh `invalid_grant`; Amanda's still
    works.
  - `redire`: `/intrare?redire=/oauth/authorize?...` logs in and lands on
    the consent page; `redire=https://evil.example/` and
    `redire=//evil.example` -> lands on `/notae`.
  - `textus_unicode`: a note with `"Grüße – 测试"` survives add -> read
    through JSON byte for byte.
  - `paginae`: 45 notes -> pages of 20, 20, 5; page 4 empty.
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement:** the four tools on `mcp_servus`; mount
  `mcp_servus_http` with `admissio_verificator`; `admissio_montare` with
  exemplar's login callback; `/applicationes` page (escaped, CSRF);
  replace T1's static metadata with the libraries' (sonda stays).
- [ ] **Step 4: Run - expect PASS**; all earlier exemplar sections green.
- [ ] **Step 5: Plants:** owner check removed from `notam_legere` ->
  `alieni` red; `redire` accepts `//` -> `redire` red.
- [ ] **Step 6: Commit** `exemplar: servus MCP tertius - instrumenta notarum (via claude), admissio montata, /applicationes, /intrare?redire (mcp-remotus T7)`.

---

### Task 8: end to end locally, Linux, live smoke

**Files:** Create `tools/exemplar_mcp_fumus.sh`. Modify
`apps/exemplar/exemplar.worklog.md`.

- [ ] **Step 1: Claude Code as the client:** run exemplar locally
  (`./apps/exemplar/exemplar.sh -data /tmp/... -portus 3010
  -invitatio ...`), register a test user, then in a Claude Code session
  `claude mcp add --transport http exemplar-loc http://127.0.0.1:3010/mcp`;
  sign in through the browser; list and add notes. If Claude Code refuses
  plain-http OAuth (spec AUDIENDA), record it and do this check against
  the deployed site in T9 instead. Record what happened in the worklog.
- [ ] **Step 2: Linux:** `./tools/linux/probatio.sh -musl` and `-glibc`
  with `base64url mcp_servus mcp_servus_http admissio
  exemplar_applicatio`; the static build (`tools/linux/aedificare.sh
  exemplar`).
- [ ] **Step 3: Live smoke script** `tools/exemplar_mcp_fumus.sh
  <basis>`: never signs in, writes nothing that matters: `/mcp` -> 401
  with the exact header; both metadata documents parse and agree on
  `resource` and `issuer`; `GET /mcp` -> 405; `/oauth/register` with a
  test redirect URI -> 201 (the test client is pruned after a day). Run
  against a local instance in this task.
- [ ] **Step 4: Commit** `exemplar MCP: fumus vivus, Linux (musl + glibc), probatio cum Claude Code (mcp-remotus T8)`.

---

### Task 9: live - deploy, the phones, close-out

**Files:** Modify `apps/exemplar/exemplar.service` (flags),
`apps/exemplar/exemplar.worklog.md`, `project-specs/mcp-remotus-spec.md`
(status), this plan (rulings), ledger desideratum ...VNA (note).

- [ ] **Step 1: Deploy:** `tools/dimittere.sh -siccum exemplar smaragda`
  shown to Fran; `-agere` on his OK; the deploy's own health check and
  rollback as in hospitium v3.
- [ ] **Step 2: Smoke** `tools/exemplar_mcp_fumus.sh
  https://exemplar.golden.earth` from the Mac.
- [ ] **Step 3: Fran and Amanda connect:** each adds the connector in
  Claude (phone and desktop), signs in as themselves, asks Claude to list
  and add a note; both see their connection on `/applicationes`; Fran
  revokes and re-adds his once. Sonda lines read to confirm the refresh
  path works after an hour (or on the next day).
- [ ] **Step 4: Close-out:** spec status `IMPLETUM` with commits; plan
  rulings; worklogs; ledger note on ...VNA (B done; C and D remain);
  memory pointer.
- [ ] **Step 5: Commit** `mcp remotus: vivum - exemplar.golden.earth/mcp, Franus et Amanda per Claudium (mcp-remotus T9)`.
