# pulsus worklog

## 2026-11-02 - T1 pulsus_forma (pure forms)

- One module for every JSON shape pulsus moves: event (client -> server),
  inscription (client -> server), APNs payload (server -> Apple), tap
  (payload -> app). Server and clients link only this, so the two ends
  cannot drift.
- Limits are C10 of the plan. Validation lives in the readers only; the
  writers trust their caller (the server judges every event anyway).
- Placeholders: a `{` that does not start `{persona}` or `{numerus}`
  rejects the template, so a typo like `{nomen}` is a 400 at the client's
  first send, not a literal brace on someone's lock screen.
- Payload truncation drops ids from the END one at a time and marks
  `"plures":true`; the app then opens the Shared grid instead of a
  partial group. Linear, but bounded by a batch of at most M ids.
- `json_scribere` is compact and keeps insertion order - the payload test
  pins the exact octets of spec III.1.
- Gotcha: a new lib/*.c needs `./tools/compile_tests_fontes_generare.sh`
  BEFORE its suite links (the plan listed it after PASS).

## 2026-11-02 - T2 pulsus core

- Time is pulled: `pulsus_maturos_mittere` sends every batch whose
  `ultimus_ms + mora_ms <= nunc`; T3's reactor timer will only call it.
  Tests drive a fake clock and call it directly - no sleeping.
- A batch row is DELETED before its notifications go out. The fake (and
  any synchronous) sender calls `finis` inside `mittere`, and a crash
  mid-send must never resend: at most once beats at least once for a
  "Fran shared 3 pictures" banner.
- `PulsusFinis` hands the sender's copy of the message back (ruling vs
  the plan), so pulsus knows which (apparatus, thema, token) finished
  without allocating a context per message. MORTUUM deletes only when the
  token still matches - a re-registration racing a 410 is not lost.
- Recipients are computed from `communio_apparatus_enumerare` each send:
  same space, other PERSON, person active, device not revoked. Revoked
  devices' inscriptions are purged in that same pass (no hook into
  communio_servus needed).
- Notification title = last segment of the thema ("pictor"); the server
  has no other source for the app's name.
- Bug caught on review: `scrinium_gradi` returns SCRINIUM_ERROR (0) on
  failure, not -1 - a `!= -I` test was always true.
- Non-statim event with no ids is a no-op (it would otherwise announce
  "0 pictures"); statim with no ids flushes a pending batch or does
  nothing.
