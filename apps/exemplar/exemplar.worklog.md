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
