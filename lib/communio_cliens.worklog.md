# communio_cliens worklog

## 2026-10-27 - the device-side client (communio plan-2 T6)

Reactor-driven state machine with TWO independent lanes, because one loop
(push -> long-poll pull -> push) would hold a device's new strokes back for up
to 25 s while its own pull waited:

- ascensus (push): bind the replica once (POST /communio/apparatus), then own
  syncable acts after the cursor in batches (<= CC acts, ~DXII KB), blobs first
  (desunt -> PUT each), then POST /communio/acta. When nothing is left it polls
  the volume once a second (`communio_cliens_excitare` = now).
- descensus (pull): GET /communio/acta?post=<server seq>&mora=S, fetch the
  listed blobs it lacks (hash checked on store), import in order, advance the
  cursor only when the whole batch imported, call `advenerunt(n)`.

Rules learned while building:

- Never act inside the http_vocare finis callback: the call's own memory lives
  in the lane's piscina. The callback only schedules a 1 ms timer; the response
  is read on the next reactor turn, and the lane's piscina is reset only at the
  start of the NEXT cycle. Timer ids are counters (not slots) in lib/reactor.c,
  so cancelling a fired id is harmless; we still clear `hora_viva` first.
- Every HTTP request goes through ONE static function (`_mittere`) - plan C7:
  phase 5 swaps it for the iOS URLSession host service.
- Cursors live in the volume's praesentia under "communio/<basis>/ascensus" and
  ".../descensus" (decimal text) - device-local, never synced. The token lives
  only in the client struct (zeroed by destruere) and never reaches an error.
- `otiosus` = both lanes saw nothing since the last `excitare` and nothing is in
  flight; with mora > 0 the pull lane is always waiting, so otiosus is a one-shot
  (mora 0) notion - the CLI's -semel and the tests.

Numbers (Mac, in-process, loopback): a 500-stroke picture A -> server -> B in
~120 ms wall. Plants: skipping blob fetch, not reading saved cursors, staying
silent on a network failure - all three turned probatio_communio red.
