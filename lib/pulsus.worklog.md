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
