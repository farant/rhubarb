# invitationes worklog

## 2026-10-20 - born (ianua T3)

Per-person invitation codes replacing exemplar's one shared
`cfg.invitatio` secret (exemplar keeps it until ianua T5). Spec II.3,
plan T3.

Design notes:
- Code = 16 random bytes (`moneta_octeti_fortuiti`, no fallback - a
  secret must be unpredictable) as base64url, 22 chars. Returned ONCE;
  the table keys on SHA-256 hex of the code (UNIQUE), so lookup is by
  digest and the code is never stored. A code of any other length is
  invalid before any query.
- `consumere` decides with ONE guarded UPDATE (`usus_numerus <
  usus_max AND expirat_ms > now AND revocata_ms = 0`) and then checks
  `SELECT changes()` == 1. `inspicere` promises nothing: two sign-ups
  that both inspected the same single-use code -> exactly one wins
  (section certamen).
- Valid means `expirat_ms > now` (strict): at expirat_ms it is dead.
- Empty `auctor` is legal and means "issued from the CLI" (plan P1);
  empty `nota` is legal. FOUND IN T3: an empty chorda can arrive with
  datum NIHIL, and the first draft refused it - which would have refused
  the bootstrap invitation itself. Now normalized to "" and pinned in
  section creare.
- A used invitation can still be revoked (keeps the history honest; it
  changes nothing for the person who already used it).

Tests: probatio_invitationes (credo v2, 7 sections, 93 assertions).
Plants (all red then green): usage guard dropped -> semel red; code
stored instead of its digest -> creare red (lookup breaks first); code
leaked into the nota column with lookups intact -> codex_occultus red
on its own (line "code found in a column"); expiry compared with >= ->
terminus red at the exact ms.

Linux: first run BLOCKED after the main merge bf073999 - credo now calls
`memoria_processus_legere`, which had only a macOS body, so every Linux
test link failed (not an invitationes defect). Fixed by Fran's choice
in 045b7159 (lib/memoria_processus_posix.c); then invitationes green on
musl + glibc.
