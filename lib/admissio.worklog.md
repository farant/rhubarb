# admissio worklog

## 2026-10-20 - part I (mcp-remotus T5)

OAuth 2.1 authorization server for the house's remote MCP servers:
RFC 8414 document, RFC 7591 dynamic registration, opaque tokens,
verifier (an `McpVerificator`), grant listing and revocation.

**Tables, not migrations.** `scrinium_migrare` keeps ONE counter in
`_migrationes`, already used by the volumen's own schema - an app list
would collide with it. admissio runs `CREATE TABLE IF NOT EXISTS` for
its four tables in one transaction and keeps `admissio_versio` (1) for
future changes. First app tables inside a volumen; exemplar has none.

**Secrets:** 32 bytes from `moneta_octeti_fortuiti` -> base64url (43
chars). Only `SHA-256 hex` is stored; lookup by digest, then a
constant-time compare. The test reads the volumen file AND its `-wal`
byte by byte for every issued token (planted: storing the token in
plain turns it red - WAL matters, recent rows live there).

**Registration:** rate limit first (cheap): 20 per IP per 60 s, refused
ones are not stored, 429. Note all Claude users share Anthropic's
egress range, so the limit is per *Anthropic IP*, not per person - fine
for two users. Redirects: `https://` with a host, or
`http://127.0.0.1` / `http://localhost` with any port; `#` refused;
`localhost.evil.example` refused (host compared whole). Claude's real
body from the T1 probe (scope with `offline_access`, `application_type:
web`) registers. `client_id` = 16 random bytes (22 chars).

**Pruning:** clients older than a day with no grant are deleted;
expired tokens and codes go too (not counted).

**Grants:** ULID id, user, client, scopes, audience; token family =
grant id (T6 rotation/theft detection revokes the grant). Created /
revoked are also volumen events (`concessio.creata` / `.revocata`,
ids only, never secrets). Revocation deletes the grant's tokens, so the
next `/mcp` call is 401. Listing returns live grants only.

Bug: a count helper that always bound (text, number) was fed SQL with
dummy `? = ?` padding - the third `?` stayed NULL, `0 = NULL` is never
true, every client looked unknown. Two honest parameter shapes now.

## 2026-10-20 - part II (mcp-remotus T6): authorize, consent, token

`admissio_montare` puts five routes on the app's hospitium: the RFC 8414
document, `/oauth/register`, GET+POST `/oauth/authorize`,
`/oauth/token`.

**Authorize** checks in a fixed order. `client_id` and `redirect_uri`
first, and a failure there is an ERROR PAGE (400, no Location) - never
a redirect to an unverified URI (open redirect). Redirects must match a
registered URI exactly; loopback matches with the port ignored (RFC 8252
7.3) but the path still exact. Everything after that (response_type,
S256 + challenge length, resource == our audience, known scopes)
redirects back with `error`, `state`, `iss`.

**Consent page:** client name, the redirect HOST in `<strong>`, a
"local program" warning when every registered URI is loopback, one
checkbox per requested app scope named `s0..sN` (hospitium's
`colloquium_forma` returns only the first value of a repeated key, so
no `scope[]`), all parameters echoed as escaped hidden fields, CSRF
from the app's session. `Cache-Control: no-store`, `X-Frame-Options:
DENY` (it is a click-to-grant page). POST: Origin must equal the
issuer's origin, then login (gone -> 303 to `/intrare?redire=...`, no
code), then CSRF (constant-time), then the decision. Nothing ticked =
`access_denied`. `offline_access` has no checkbox: carried if requested,
and it alone decides whether a refresh token is issued.

**Token:** form-urlencoded only. The code is BURNED before the other
checks (a failed PKCE attempt consumes it); a replayed code revokes the
grant it produced (`admissio_codices.concessio`). Refresh rotates: old
row `usa = 1` + new pair in one transaction; presenting a rotated-out
token kills the whole family (grant). Expiry is strict `<`: a refresh
at its exact expiry second is refused. All responses `Cache-Control:
no-store`, errors per RFC 6749 5.2.

Test finding: with PKCE checking planted off, the suite SEGFAULTED
instead of failing - the test's `_json` helper returned NULL for an
absent field and `strcmp` died. A plant that crashes is still red, but
it names nothing; `_json` now returns "".

## 2026-10-20 - whole-branch review fixes

- `redirect_uri` on the code exchange and `client_id` on refresh are
  OPTIONAL (OAuth 2.1 4.1.3 / 4.3.1), compared when present. Claude
  Code sends both; the hosted client is unverified on refresh, and a
  missing `client_id` answered `invalid_request` would have broken the
  connector every hour (Claude only re-auths on `invalid_grant`).
- Every OAuth refusal now writes one line: `admissio via= error=
  causa="..." cliens=<sanitised>` (written BEFORE responding - the
  colloquium piscina is recycled after). Never code, token, verifier or
  CSRF; tested by scanning the captured lines for each.

## 2026-10-21 - revocation joins the caller's transaction (canalis T4)

`_concessionem_necare` did its own `scrinium_incipere`/`committere`;
inside exemplar's suspension transaction the nested BEGIN failed, so a
suspension could never revoke grants. Now: `propria =
!volumen_in_transactione(a->volumen)` - own transaction only when the
caller holds none (the /applicationes revoke page path is unchanged);
otherwise the caller's commit or rollback decides, and the
`concessio.revocata` event is part of it. Section
`revocare_in_transactione`: rolled back -> grant alive; committed ->
revoked, one event. Note: the bearer verifier does not consult the civis
status - a suspended person's tokens stay valid unless their grants are
revoked (see exemplar.worklog.md).

Review note (2026-10-21): only the revocation path joins a caller's
transaction; client registration, token issuance and refresh still open
their own (`scrinium_incipere`). No caller holds a transaction there
today - a future one would hit the same nested-BEGIN failure.
