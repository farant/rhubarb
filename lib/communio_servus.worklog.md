# communio_servus worklog

## 2026-10-27 - device registry (communio plan-2 T3)

The server is just another volume; its device registry lives in two tables of
that volume (`communio_membra`, `communio_apparatus`). Design calls (plan C2-C4):

- The DEVICE mints its token (`communio_tesseram_facere`: 32 random bytes,
  base64url via the piscina-free `base64url_scribere`, 43 chars) and shows only
  its SHA-256 hex; the admin registers the digest. The token never reaches the
  server's disk, an act, a log or a chat. Lookup by digest + constant-time
  compare (pattern from lib/admissio.c).
- `replica TEXT UNIQUE` allows many NULLs in SQLite (unbound devices); binding
  is a separate step on first contact and appends `communio.replica-ligata`.
- A long-running server must not grow memory per call: every writing function
  marks/resets a private scratch pool (`tempus`); `communio_servum_destruere`
  releases it (T5 will also cancel long-poll timers there).
- Writes join the caller's transaction when one is open (the cives convention),
  else open their own.

Plants (silva.planta, porta radix): verification without the person-active
check and without the revoked check both turned the suite red.

## 2026-10-27 - the protocol on hospitium (plan-2 T4)

Seven routes under /communio/, all behind `Authorization: Bearer` (401 +
`WWW-Authenticate: Bearer` otherwise; the bearer check runs BEFORE any 404, so
an outsider cannot probe which blobs exist). Calls made while building:

- A push with ANY invalid act is refused whole (400 + cause, nothing imported).
  communio_acta_legere validates every act (T2), so a per-act
  `refusum.causa: invalidum` would never be reached; whole-batch refusal is also
  the simpler contract for the client. `invalidum` stays as the name for an
  import that returns VOLUMEN_IMPORTATIO_MALA.
- Replica check happens before any import: one act of another replica -> 403 for
  the whole batch.
- PUT of a blob hashes the bytes FIRST and stores only on a match (in memory);
  above 1 MB hospitium spools to a file and volumen_massam_condere_ex_filo
  re-verifies. A mismatch never leaves a blob behind.
- Pull lists, per act, the 64-hex runs the server holds (deduplicated) so the
  client knows which blobs to fetch before importing.
- Test harness: hospitium with `cfg.reactor` shared with `http_vocare`, pumped by
  `reactor_poll(r, X)` until the call's finis fires - server and client in one
  thread, real sockets on 127.0.0.1.

Plants (porta radix): replica check off, blob hash check off, own replica not
excluded from a pull, space filter off - all four turned the suite red.

## 2026-10-27 - live pull / long-poll (plan-2 T5)

`GET /communio/acta?post=N&mora=S` (S capped at XXV s, under hospitium's 30 s
deferred limit) defers when nothing is there. Waiters live in a FIXED array of
LXIV slots inside the server (no allocation per wait; when full the request is
simply answered at once). Each slot holds the deferred handle, a copy of the
caller's CommunioApparatus (its chordae point into the deferred request's pool,
which lives as long as the deferral - only read after dilatum_colloquium
confirms it is still alive), the cursor, and one non-repeating reactor timer.

A push that IMPORTED something (not only "already present") wakes the waiters of
the same space; each gets a fresh pull body, and only if it now has acts. Dead
slots (client gone, handle invalid) are released on the next wake or the next
wait. communio_servum_destruere cancels every live timer - exemplar must destroy
the server BEFORE the hospitium (a timer firing into a destroyed hospitium would
touch freed memory).

Plant note: "the pusher's own waiter is skipped" is a shortcut only - _tractio
already excludes the caller's replica, so a woken own-waiter finds nothing and
stays deferred. The plant removing the shortcut stayed green by design; the test
"propria non excitat" pins the behaviour, not the shortcut. The plant removing
timer cancellation in destruere turned red.

## 2026-10-27 - final review fixes (plan-2)

- Per-request leak: `communio_apparatum_verificare` -> `_persona_activa` ->
  `cives_per_id` allocated into the server's scratch pool on EVERY request and
  never reset (every other writer marks/resets). Now marked/reset around it;
  `communio_servi_usus` exposes the pool size and a test proves it flat over
  500 verifications.
- Read-after-reset: `colloquium_respondere` may drain synchronously and recycle
  the colloquium pool (hospitium `_paratus_novae`), and `_excitare` then read
  a.spatium/a.replica from it. Wake the waiters BEFORE responding. The same
  family: `communio_replicam_ligare` compared the stored replica after
  `piscina_reficere`. Both found/proved by adding probatio_communio_servus to the
  `probationes_veneni` gate (PISCINA_VENENUM + ASan) - it stays there.
- Server identity: bind + pull responses carry "servus" (this volume's replica)
  so clients can tell a replaced/restored communio.volumen from the same one.
- One blob limit: COMMUNIO_MASSA_MAXIMA (16 MB = http lector cap) -> PUT 413.

## 2026-10-27 - allowlist and sign-in requests (plan-3 T3, spec D6)

- `communio_permissi`: the ONLY place an address is stored in clear (needed to
  send mail), plus its SHA-256 (`digestum`, UNIQUE). Sign-in requests
  (`communio_aditus`) and every act carry only the digest - the act log is kept
  forever and backed up, so it stays free of personal data. Addresses are
  normalised (outer whitespace trimmed, ASCII lowercased, exactly one '@' not at
  either end, no inner spaces/control characters).
- `communio_aditum_petere` ALWAYS returns an id; only an allowlisted address of an
  active member, with a valid unused device digest and a title, stores a request
  and fills the code/link token. The caller cannot tell the difference from the
  return value alone - T4 must keep the HTTP answer identical too.
- Code: 8 Crockford base32 characters from 40 random bits (case and dashes
  ignored on input); link token: 32 random bytes base64url. Only SHA-256 of each
  is stored. A request lives 15 minutes, allows 5 wrong secrets (a wrong code OR
  a wrong link token counts), and is single-use: confirming again with the right
  secret returns 1 and the same device id (a double click is harmless).
- At confirmation the allowlist entry (by digest, same person + space) and the
  person's status are checked AGAIN - revoking an address or suspending a person
  between request and confirmation stops the sign-in.
- Requests expired more than a day ago are deleted on each new request.

Plants (radix): attempt limit off, allowlist re-check off, expiry off - all red.

## 2026-10-27 - sign-in routes (plan-3 T4)

Three routes WITHOUT the bearer check (the device has no token yet):
`POST /communio/intrare` (always 202 {"aditus"} for any well-formed address -
allowlisted or not; 400 only for a malformed body/address), `POST
/communio/confirmare` (code), `GET /communio/confirmare` (the mail's link; a
small HTML page). Calls:

- Throttling through exemplar's Custodia (`custodia_frenum`, setter
  `communio_servum_custodiam_ponere`; absent = no throttling): `intrare` per
  NORMALISED ADDRESS (allowlisted or not, so 429 reveals nothing; caps mails per
  address); confirmation per client IP (`colloquium_cliens`) on top of the
  database's 5-attempt limit per request. A delay answer from custodia
  (>0 ms) is treated as 429 - a device just retries later. Not per IP on
  `intrare`: in-process tests share 127.0.0.1, and the per-address cap is what
  bounds mail volume (unknown addresses cost nothing).
- The mail (English): subject "Sign in to golden.earth", the device title, the
  link `<basis>/communio/confirmare?aditus=<id>&signum=<token>`, "Code:
  XXXX-XXXX", 15 minutes, once, "ignore if not you". Idempotency-Key = request id.
  The title is HTML-escaped in the mail's HTML and the confirmation page (it is
  user input). The text buffers holding the code and token are zeroed right
  after the message is handed to the Epistularius (which copies what it needs).
- No Epistularius set: `intrare` still 202, one stderr line, nothing else.

## 2026-10-27 - final review fixes (plan-3)

- CORRECTION of the T4 entry: per-address throttling in custodia is NOT a mail
  cap - its table (1024 slots) evicts the oldest key, so junk addresses flush a
  real address's counter. The cap now comes from the database: at most
  COMMUNIO_EPISTULAE_PER_HORAM (III) sign-in requests per address per hour and
  COMMUNIO_EPISTULAE_PER_DIEM (XXX) per server per day are stored (= mailed);
  beyond that intrare still answers 202 and one stderr line notes it. Rows of
  unknown addresses are never stored, so they cannot fill the budget.
- intrare answers after a fixed floor (COMMUNIO_INTRARE_MORA_MS, 250 ms from the
  request start) through a deferred response in 16 fixed slots, so an
  allowlisted address (DB write + fsync + mail setup) and an unknown one take
  the same time. Tests set 0 via communio_servum_moram_intrandi_ponere.
- The mail's link no longer signs in on GET (mail scanners prefetch links): GET
  shows "Sign in the device <title>?" with a form button; only the form POST to
  /communio/confirmare (aditus + signum) registers the device. The token also
  leaves the state-changing URL.
- Device titles: 1..64 bytes, no control characters (no newlines in the mail
  from our domain) - otherwise 400, the same for known and unknown addresses.
