# ianua worklog

## 2026-10-20 - born (ianua T4)

The door an app mounts on hospitium: composes cives, credentialia,
invitationes, and owns the sessions table. Spec II.4, plan T4. Security
plumbing copied from exemplar (`_redire_tutum`, Origin, CSRF, cookie,
HTML escape - P9); exemplar keeps its copy until T5.

Design notes:
- No in-memory state but the deferred-work slots (`IanuaOpus`): every
  request queries the tables. A slot never holds a password (credentialia
  / custodia do); it holds the invitation code during sign-up, zeroed on
  release.
- Sign-up = hash first, then ONE transaction in the hash callback:
  `cives_creare` -> `invitationes_consumere(code, new id)` ->
  `credentialia_tesseram_ponere` -> commit. A `transactio` flag drives the
  rollback (the first draft inferred it from the message text and would
  have left a transaction open after a late 500 - rewritten before the
  first run).
- Password change: verifying the CURRENT password and hashing the NEW
  one run at the same time (plan ruling T4); the slot joins both
  callbacks. Success = one transaction: new password + every OTHER
  session of the person deleted (each with `sessio.deleta`).
- Sessions: only SHA-256 hex of the `sid` token is stored; a session of
  a person who is not ACTIVE is not signed in. `ultima_ms` is written at
  creation only (YAGNI: no per-request write).
- Pages: the app's `pagina` callback wraps the escaped body and sends
  its own security headers; without it ianua sends a minimal page with
  exemplar's CSP set. The new-invitation page sends
  `Cache-Control: no-store` and is the only place a code is shown.
- Status codes: 400 bad input, 403 Origin/CSRF/dead invitation/wrong
  password, 404 foreign invitation on revoke, 409 handle taken or
  already signed in, 429 rate limit, 503 busy.

Tests: probatio_ianua (credo v2, 13 sections, 223 assertions, ~2.7 s),
each section also run alone. In-process HTTP client adapted from
probatio_exemplar_applicatio.c. FOUND writing the tests: the planned
race (same handle, two codes) never exercises rollback - the loser fails
at its FIRST write. Added the race that does: same code, two handles ->
both create a person, the loser cannot consume -> its person must not
survive.

Plants (all red then green): commit instead of rollback -> the
same-code race (two persons survive); other sessions kept on password
change -> tessera_mutata; `//` accepted as redire -> redire; status not
checked in `_intratus` -> suspensus; form shown for a dead invitation ->
invitatio_mortua. Green on macOS, Linux musl + glibc.

## 2026-10-21 - ianua_sessiones_civis_delere (canalis T3)

Suspension (canalis T4) must end a person's web sessions now. The
password change already deleted "all but this one"; that loop is now one
helper, `_sessiones_civis_delere(ia, civis, praeter, p)` (praeter with
datum NIHIL = no exception), and both callers use it - each session goes
through `_sessionem_delere`, so each still writes `sessio.deleta {sigillum
= SHA-256 hex}` (never the token). Correction to canalis plan P10, which
said sessions were not evented. tessera_mutata (the regression check for
the refactor) stays green. Plant: the no-exception SELECT without `WHERE
civis` -> red (count 3, amanda's cookie dead).
