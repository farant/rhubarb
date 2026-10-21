# exemplar worklog

## 2026-10-17 - born (hospitium v3 T8)

- **State = events.** Every change writes its actum first, then mutates
  memory; `_replicare` rebuilds users, sessions and notes from
  `volumen_acta_legere(vol, 0)` at start. `sessio.creata` stores
  SHA-256(token) hex, never the token (section `secreta` scans the
  acta for the cookie value and the password).
- **Deferred work lives in a fixed table** (`Opus`, 64 slots): the
  password copy stays there while custodia hashes it and is zeroed
  when the slot is freed. A login whose PHC needs re-hashing keeps its
  slot until the re-hash callback writes `tessera.mutata`.
- **Every POST checks Origin == schema://Host first**, then session,
  then CSRF. Registration/login have no session, so Origin is their
  only cross-site guard (browsers send Origin on form POSTs).
- **Files:** upload = hospitium temp file + hash (T3) ->
  `volumen_massam_condere_ex_filo` inside the same transaction as
  `filum.receptum` + `nota.scripta`. Download = `HospitiumFons` over
  `VolumenMassaLector`, owner check against the user's live notes.
- **Shutdown:** SIGTERM -> `hospitium_sistere_molliter(10 s)` -> loop
  ends -> `PRAGMA wal_checkpoint(TRUNCATE)` -> volumen closed, exit 0
  (smoke-tested with curl: no -wal file left behind).
- **Name clash to remember:** lib `exemplar` (pattern matching) has
  `probatio_exemplar.c` and the `exemplar_` prefix; this app's test is
  `probatio_exemplar_applicatio.c`.
- **Test speed:** client and server share a thread; keep the client's
  recv timeout tiny and pump many cycles per wait, or argon2id (one
  step per cycle) makes each login take seconds.

## 2026-10-19 - mcp-remotus T1: the probe

exemplar grows MCP/OAuth routes in a second file `exemplar_mcp.c`
(nucleus is 1522 lines); the shared structs moved to
`exemplar_internum.h`; `exemplar_mcp.h` makes aedilis pick up the new
`.c` (the test source list is the union of aedilis closures).

**Routes now:** `POST /mcp` -> always 401 with `WWW-Authenticate: Bearer
resource_metadata="<basis>/.well-known/oauth-protected-resource",
scope="notae:legere notae:scribere"` (nothing issues tokens yet); both
protected-resource paths (RFC 9728, also `/mcp`-suffixed); the
authorization-server document (RFC 8414); `POST /oauth/register` -> 503
`temporarily_unavailable` (the probe stops Claude there on purpose). All
JSON built with `json`, none hand-written.

**`-basis URL`** (plan P10): `-hospes` is the LISTEN address (127.0.0.1
behind nginx), so the public base for issuer and resource is its own
flag; trailing `/` stripped; default `http://<hospes>:<port>`.

**`-sonda`:** one line per request to the MCP/OAuth routes: method, URI,
EVERY header (Authorization / Proxy-Authorization keep only the scheme,
Cookie / Set-Cookie fully redacted), body up to 4 KB with non-printables
as `\xHH` - never the body of `/oauth/token`. The writer is a callback in
the config (default stderr), so the test captures lines in memory and
asserts the test token text appears nowhere.

`tools/dimittere.sh -cum-unitate` now also installs the app's systemd
unit (prior kept as `.prior`, daemon-reload; restored together with
the binary if health fails) - the unit gains `-basis` and `-sonda`.

Gotcha: a comment containing the path `/.well-known/*` has `/*` inside a
block comment -> `-Wcomment` error. Write `/.well-known/...`.

## 2026-10-19 (20 UTC) - mcp-remotus T1: what Claude actually sent

Deployed with `tools/dimittere.sh -agere -cum-unitate exemplar smaragda`
(health good; nginx passes the well-known paths). Fran added the
connector from the desktop app. Full findings in the spec, section
"VERIFIED by the probe"; the short version for T3-T6:

- **initialize era, 2025-11-25** - the gate passes, the plan stands.
  `mcp-protocol-version` is sent on EVERY request, even initialize and
  the discovery GETs - so the core must not reject an initialize that
  carries it.
- The caller is python-httpx from 160.79.106.x (Anthropic's servers),
  even for the desktop app: no Origin, no cookies - exemplar's POST
  Origin rule must not touch /mcp or /oauth/* (plan P3 confirmed).
- It fetches the PRM at the exact `resource_metadata` URL (root path),
  then the RFC 8414 document. The `/mcp`-suffixed PRM route stays for
  other clients.
- DCR asks for `scope: "notae:legere notae:scribere offline_access"`
  (taken from the AS `scopes_supported`), `token_endpoint_auth_method:
  none`, `application_type: web`, `client_name: Claude`, one redirect
  URI `https://claude.ai/api/mcp/auth_callback`. T5's register must
  accept `offline_access` in the requested scope and ignore
  `application_type`.
- The first of two attempts stopped after discovery without
  registering; the second registered. Unexplained - watch for it at T9.

## 2026-10-20 - mcp-remotus T7: exemplar is the third MCP server

T1's static discovery documents, the always-401 `/mcp` and the 503
register are GONE; the libraries own those routes now:
`mcp_servus_http_montare` (with `admissio_verificator`) and
`admissio_montare` (login through exemplar's own session:
`exemplar_intratus_usor` returns user + the page CSRF). The sonda
survives through the libraries' new `speculator` hooks - it is still
the raw-request capture; bodies of `/oauth/token` AND of the consent
POST (it carries the CSRF token) are redacted.

**Tools** (`exemplar_mcp.c`): `notas_enumerare` (`pagina`, 20 per
page, newest first, `{pagina, paginae, notae}`), `notam_legere` (text
+ attachment name/mime/size), `notam_addere`, `notam_delere`
(destructiveHint). Only the token user's notes: a foreign or deleted id
gives the SAME tool error "nota non inventa" - nothing leaks. MCP notes
are written by `exemplar_notam_scribere` with `via: "claude"` and the
client id; web notes now carry `via: "retis"`.

**`/applicationes`** (in the nucleus - it needs the page helpers):
client name, scopes, created, last used, revoke (Origin + CSRF). The
next `/mcp` call of the revoked grant is 401, its refresh
`invalid_grant`; other people's grants untouched (`revocare` section).

**`/intrare?redire=`**: only local paths (`/x`; never `//x`, `/\x`,
control chars) are kept - in a hidden field on the login page, then in
the deferred login `Opus`, then as the 303 target. Anything else lands
on `/notae`.

`admissio_putare` runs at start and hourly (reactor timer).

Found by the Linux build, not by clang: `lib/base64.c` (T2) had
`(I << bits) - I` - an `int` into `i32` arithmetic, which gcc's
`-Wsign-conversion` rejects. Always build Linux before calling a C
task done.

## 2026-10-20 - mcp-remotus T8: Claude Code end to end, Linux, fumus

**Claude Code 2.1.303 against a local exemplar** (`http://127.0.0.1:3010`,
`-sonda`; Fran ran `claude mcp add --transport http exemplar-loc ...`
from /tmp, `/mcp`, signed in in Firefox, added a note). It WORKED, and
the sonda shows a DUAL-ERA client:

1. First request: `POST /mcp` `server/discover` with
   `mcp-protocol-version: 2026-07-28` and `_meta`
   (`io.modelcontextprotocol/protocolVersion`, `.../clientInfo`) - no
   token -> our 401 -> OAuth starts.
2. Discovery: both documents (UA `claude-code`, then `Bun/1.4.4`).
   DCR: `client_name "Claude Code (exemplar-loc)"`, redirect
   `http://localhost:53734/callback` (random port - loopback rule hit).
3. Authorize: `resource=http://127.0.0.1:3010/mcp` (RFC 8707, as
   expected) and `prompt=consent` (ignored - we always show consent).
   Consent POST from Firefox with Origin `http://127.0.0.1:3010`.
   Token exchange (body redacted in the sonda).
4. With the token: `server/discover` again -> our 400 `-32022`
   Unsupported protocol version (data.supported = our three) -> it FELL
   BACK to `initialize` `2025-11-25`. The T4 design (official 2026
   error shape) is exactly what made the fallback work.
5. `notifications/initialized` 202, `GET /mcp` (SSE attempt) 405,
   `tools/list`, `tools/call notam_addere` with an emoji - stored and
   logged `usor=fran cliens=7B3B...`.

So the spec's AUDIENDA "does Claude Code accept OAuth on plain
http://localhost" is answered: yes.

**Linux:** `tools/linux/probatio.sh -musl` and `-glibc` with base64url,
mcp_servus, mcp_servus_http, admissio, admissio_fluxus,
exemplar_applicatio: 6/6 each. Static build `build/linux/exemplar` OK
(T7 fixed the gcc sign-conversion it found).

**`tools/exemplar_mcp_fumus.sh <basis>`** - live smoke, never signs in:
exact 401 header, both PRM paths + the AS document agree on resource
and issuer, `GET /mcp` 405, a throwaway registration 201 (pruned after a
day). Local run: all six BENE; a dead port and a host that mismatches
the issuer (`localhost` vs `127.0.0.1`) both exit 1.

## 2026-10-20 - mcp-remotus T9: live

Deployed with `tools/dimittere.sh -agere exemplar smaragda` (binary
only - the unit was already T1's, checked identical). Before it, the
service was STOPPED and `/var/lib/exemplar/exemplar.volumen` copied to
`exemplar.volumen.ante-T9` (stop first = checkpointed, no WAL to copy;
backup stays until the slice is closed). Health `{"status":"ok","acta":6}`
- old events kept; admissio's tables were created on first start.
`tools/exemplar_mcp_fumus.sh https://exemplar.golden.earth`: six BENE.

Live, from the journal: claude.ai behaves like Claude Code - it probes
the 2026 era first (`server/discover`), gets our 400 `-32022`
(`exitus=versio`), falls back to `initialize` 2025-11-25. Fran: two
sign-ins (6 registrations total incl. fumus), `notas_enumerare` and
`notam_addere` from Claude, `/applicationes` viewed. Tool calls answer
in 4-11 ms on the droplet.

Not yet observed: Amanda's own connection, a live revoke + re-add, and
the first refresh after the access token's hour (look for
`POST /oauth/token` with a refresh body, then 200s on `/mcp`).

## 2026-10-20 - accounts through ianua (ianua T5)

exemplar's account layer is gone: `Usor`/`Sessio`/`Opus`, the account
routes, the async hashing slots, the shared `cfg.invitatio` secret and
the `usor.*`/`tessera.*`/`sessio.*` replay. exemplar_nucleus.c 1719 ->
1408 lines (and the 1408 now include the migration, the CLI invite and
a test hook). ianua is mounted with exemplar's page frame (`pagina`
callback), `via_post_intrationem` /notae, invitations required.

- Notes carry `civis` (ULID). Old `nota.scripta` events carry `usor`
  (a handle); replay maps them through `exemplar_nomina_vetera`, filled
  once by the migration. The log is never rewritten (plan P7).
- MIGRATION `_cives_migrare`, at the END of exemplar_creare (after
  admissio created its tables, before replay), ONE transaction: latest
  PHC per handle from `usor.creatus`/`tessera.mutata` -> `cives_creare`
  + `credentialia_tesseram_ponere`; the map row; `admissio_concessiones`
  and `admissio_codices` `usor` re-keyed handle -> ULID (P8); event
  `exemplar.migratio {cives: {handle: ULID}}`; marker row in
  `exemplar_migrationes`. Any failure -> rollback, "exemplar: migratio
  civium fracta: <reason>" on stderr, exemplar refuses to start. A
  fresh volumen writes only the marker.
- Old `sessio.creata` events are ignored: everyone signs in once (P12).
- admissio `concessio.*` events written BEFORE the migration still say
  `"usor":"fran"` (history); new ones carry the ULID. Two suite
  assertions changed accordingly (`_usor_civis` reads the ULID).
- SINGLE WRITER (P1): `data/sera` via `filum_seram_capere` at start; a
  second server or `exemplar -invitare` while running is refused.
  `exemplar -data D -invitare "<nota>" [-basis URL]` prints the code (and
  link with -basis) on stdout only. `-invitatio`/`-invitatio-via`
  removed; exemplar.service updated (P14 - deploy with -cum-unitate).
- Every failure path of exemplar_creare goes through `_creatio_fracta`
  (destroys the hospitium, closes the volumen, releases the lock) - the
  migratio_fracta test relies on the lock being free for the retry.

Tests: probatio_exemplar_applicatio 342/342 (5 new sections: migratio,
migratio_iterata, migratio_fracta, sera, mcp_nota_civis; every sign-up
now uses a fresh code via `exemplar_invitationem_probationis`). Plants
red then green: migration commits instead of rolling back
(migratio_fracta: fran exists after a refused start); replay ignores
the map (migratio: fran's old note gone); lock not taken (sera). Linux
musl + glibc green (7 suites); static build OK (build/linux/exemplar,
2.0 MB).

Local end to end (scratch data, port 3011): CLI invite while stopped ->
code; while running -> exit 2 "servus currit"; link -> form; sign-up 303;
reused code 403; sign-in as "Fran" -> /notae "salve, Fran"; /mcp 401
exact; clean SIGTERM. Claude Code OAuth round trip not repeated locally
(needs a browser) - covered by T6's live check.

## 2026-10-20 - ianua T6: live on exemplar.golden.earth

Deploy: service stopped (clean WAL checkpoint - no -wal/-shm left), backup
`/var/lib/exemplar/exemplar.volumen.ante-ianua` (same size as live), then
`tools/dimittere.sh -agere -cum-unitate exemplar smaragda` (old binary and
unit kept as `.prior`). `/salus` {"status":"ok","acta":11}; journal
`exemplar: migratio civium facta (1)` - one distinct handle in the old
log, so one person; no "fracta". `exemplar_mcp_fumus` 6/6 BENE;
code-less `/inscribere` -> "invitatio non valet"; `/intrare` 200. Fran's
live checks (web sign-in + notes, Claude still connected through the
re-keyed grant, password change, invitations page): "looks good to me".

Rollback if ever needed: stop; restore `.ante-ianua`, `exemplar.prior`,
`exemplar.service.prior`; daemon-reload; start. dimittere's own rollback
restores binary + unit only, never the volumen - after a migration ran,
the volumen backup is part of the rollback.

Left on the server for Fran to decide: the old `/var/lib/exemplar/
invitatio` file (nothing reads it since P14) and the backups
`.ante-T9`, `.ante-recensio`, `.ante-ianua`.
