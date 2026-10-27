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
