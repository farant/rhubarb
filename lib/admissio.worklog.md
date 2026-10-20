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
