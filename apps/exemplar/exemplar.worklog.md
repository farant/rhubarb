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
