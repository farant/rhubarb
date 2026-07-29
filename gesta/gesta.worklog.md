
## 2026-07-15 — K2 seal-bar false alarm: WAL-blind sqlite copy

Seal bar II (annales==acta) failed on the store COPY with "linea
extra in cauda (superset)" while the live store was perfectly
consistent (72=72=72). Cause: `cp tabularium.db scratch/` without
`-wal` — recent transactions still lived in the WAL, so the copy's
acta were a prefix while the copied jsonl was complete. Verificare
honestly named exactly what it saw; the corruption was in my copy
procedure, not the store. Law: verification copies of a WAL-mode
sqlite db = copy db+wal+shm together (or `PRAGMA
wal_checkpoint(TRUNCATE)` first). Same family as the stale-object
traps: the artifact you inspect must actually be the artifact.

## 2026-07-29 — renovare arc: the spawn-once guard was a live-only find

The fori proxy's start-if-absent guard (`hic_manens genitus`) was
"spawn once per proxy lifetime" — correct storm protection until
the daemon gained a renovare tool whose contract is "exit cleanly,
next request respawns me." The unit tests could never see this:
the stub-launcher specimens exercise the tabularium machine, and
the proxy sits outside it. Only driving the real daemon through a
real renewal cycle surfaced it. Fix: re-arm the flag on every
successful connection — still at most one spawn per OUTAGE, but a
renewed daemon's respawn is allowed. Law reinforced: a lifecycle
feature isn't shipped until the LIVE cycle has been walked once.
