
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

## 2026-07-30 — captura fragmentorum (arc ② of 01KYRGEGV4)

Articulus blocks now mint res. `<ideas>` + lines `<# (> text` /
`<(> text` → daemon-side pass at addere + gerere-mutatio: mints
ULIDs, stamps 12-char prefixes into the text, creates the genus if
absent (definitio res, campus textus, origo `captura:<articulus>`),
creates res (titulus + campus_tituli field), diffs titles on later
saves, soft-flags orphans (`orphanus` key; cleared on reappearance,
counted as `redivivae`). One atomic gesta_fascis_scribere batch —
a crash can never leave unstamped text next to created res, so
re-saves never duplicate.

Traps found and paid for:

- **The folded datum DROPS the genus key.** `_rei_applicare` moves
  genus to the res-table COLUMN; reading `gesta_res_datum` and
  checking `datum.genus` silently never matches. Cost an hour of
  everything-looks-right; the fix is `_cap_genus_rei` (SELECT genus
  FROM res). Law: state columns are projections — the datum object
  is not the whole state.
- **Custodia notes echo your key names forever.** The G-series
  field judge flagged `orphanus` as `clavis extra campos` on every
  orphan event, and the violation nota's TEXT contains the word —
  making "assert absent after remotio" unsatisfiable by strstr.
  Fix was semantic, not test-side: `orphanus` joined
  `_clavis_systematis` (it IS a machine key, set and cleared by the
  machine; judging it was noise).
- **gesta/compile_probationes.sh must run from the REPO ROOT.**
  Fixtures and the daemon script are root-relative; from gesta/ the
  world cannot even open and every test fails from initialize —
  looks like total breakage, is just CWD.
- **moneta_ulid is monotonic ⇒ same-ms twins differ only in the
  LAST char.** Short prefix stamps are structurally impossible with
  the standard mint. New `moneta_ulid_fortuita` (fresh randomness
  per call, stateless) makes 12-char stamps (ts 10 + rand 2) work;
  extension-on-collision guards the rest, and a re-mint loop keeps
  an earlier stamp from prefixing a later sibling's full id.

Line grammar is deliberate (not stml_legere): author bytes outside
the stamp splice stay untouched. The stamped form's STML validity
is pinned in lib/stml (captio fragmentorum, 32a1e00).
