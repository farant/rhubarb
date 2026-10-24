# custodia worklog

## 2026-10-17 - born (hospitium v3 T7)

- **One reactor job per running hash.** `_labor` does
  `argon2id_gradus(opus, caudices_per_gradum)` per reactor cycle; with
  32 blocks per step a 19 MiB / t=2 hash is ~1200 steps, each well
  under a millisecond, so the loop keeps serving requests and timers.
- **Order at the end of a job matters:** destroy the job's piscina
  (19 MiB back), zero the password copy, free the slot, start the next
  waiting job, THEN call the app's callback. The callback may enqueue a
  new job (re-hash after login) and must find a free slot and an honest
  concurrency count.
- **Unknown user = full dummy hash** with the configured parameters and
  a fixed salt; the result is forced FALSUM. Same number of steps as a
  real verify (the test counts them), so response time does not reveal
  whether an account exists. Unparseable or oversized PHC strings take
  the same path.
- **Session tokens:** 32 random bytes -> 43 chars base64url; only
  SHA-256(token) is meant to be stored. CSRF = base64url(HMAC-SHA256(
  secret, session hash)) - stateless, per session, compared in
  constant time.
- **Throttle memory is fixed** (1024 entries, key = SHA-256 of the IP
  or account name), so an attacker rotating IPs evicts old entries
  instead of growing memory.
- **Secret file is read raw** (see plan R19) - never through filum or
  lectiones.

## 2026-10-19 - `_signa_tuta` now calls base64url_scribere (mcp-remotus T2)

The private base64url alphabet and loop are gone; `_signa_tuta` wraps
`base64url_scribere` (piscina-free, `base64.h`) and adds the NUL.
`csrf_fixum` pins the CSRF value for a fixed secret and session; it was
green on the old encoder before the switch and stays green after.
