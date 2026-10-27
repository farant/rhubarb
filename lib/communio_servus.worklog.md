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
