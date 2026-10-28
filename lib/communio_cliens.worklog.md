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

## 2026-10-27 - token file + CLI (plan-2 T7)

`communio_tesseram_scribere/_legere` follow lib/oceanus.c's secret-file rules:
O_CREAT|O_EXCL|O_NOFOLLOW 0600 on write (never overwrite), regular file + no
symlink + mode not wider than 0600 on read, raw open/read under
`<tolera codex="CANALIS_PRAETERITUS">` (secrets never go through lectiones;
examen flags a tolera that absorbs nothing, so only the open() sites carry it).
tools/communio.c is a thin wrapper (tessera / replica / currere [-semel]),
built like oceanus: ./bin/aedilis tools/communio.c && bash
build/aedilis/communio/struere.sh -> build/aedilis/communio/communio (never
installed from a worktree).

## 2026-10-27 - final review fixes (plan-2)

- Server identity: the client remembers the server volume's replica
  ("communio/<basis>/servus"); when a bind or pull names another one (the server
  volume was replaced or restored), both cursors go to 0, the replica is re-bound
  and `epocha` increments so a response already in flight to the old server is
  discarded instead of writing a stale cursor back.
- Own acts are read in pages of 400 (`volumen_acta_replicarum_legere` with the
  own replica) - before, every cycle re-read everything after the cursor
  (quadratic first sync). A page of derived acts only advances and re-cycles.
- Blobs above COMMUNIO_MASSA_MAXIMA are refused BEFORE sending (size via the
  massa reader, not by loading it) with a visible error on the writing device.
- Errors are owned per lane: a successful pull no longer wipes a push error.

## 2026-10-27 - sign-in by email, device side (plan-3 T6)

`communio_inscriptionem_petere` mints the token file (refusing an existing one,
nothing changed) and POSTs /communio/intrare with its digest; if the server does
not answer 202 the new token file is deleted again so a retry starts clean.
`communio_inscriptionem_confirmare` (code) and `communio_tessera_valet` (does the
token work yet? = the link was opened) complete it. These are ONE-SHOT helpers:
one request, the reactor is pumped until it answers (MMM turns of X ms) - the CLI
and the tests use them; the sync lanes are untouched. That makes `_vocare_semel` a
second HTTP function in the client (plan C7 said one): phase 5's URLSession swap
covers both.

`communio inscribere -servus URL -email ADDR -tessera FILE -titulus NAME` polls
stdin (2 s) for a typed code and checks the token after each wait, whichever
comes first; 15 minutes then exit 1. EOF on stdin just stops reading (the link
path keeps working).

## 2026-10-27 - correction (plan-3 final review)

The T6 entry said EOF on stdin "just stops reading (the link path keeps
working)": it did not - reactor_poll returns at once when nothing is registered,
so the loop raced through its 450 turns in seconds. With stdin closed the CLI now
sleeps 2 s (poll with no fds) between token checks. The link path also now needs
the page's button press (server change, see communio_servus worklog).

## 2026-10-27 - every request through `veredarius` (communio-plan-5 T1, quarta)

The client's two HTTP users (`_mittere` for the lanes, `_vocare_semel`
for sign-in) now go through `Veredarius` (`include/veredarius.h`): one
request, a finish callback, abort. The host supplies it - the house one
(`lib/veredarius_domesticus.c`, over `http_vocatio`) on the Mac and in
tools; URLSession in the iOS shell (T8). `communio_cliens.c` includes
neither `http.h`, `http_vocatio.h` nor `communio_servus.h` any more (a
test reads the source and fails if `http_vocare` comes back), so an app
linking the client does not link TLS or the server: the token-size
constant and `communio_tesseram_facere` moved to `communio_forma`.

A lane's in-flight state is now `in_itinere` (sent until the response
is handled) + `respondit` (finish arrived) + the abort `signum`, where
it used to be the `HttpVocatio*`. Stopping a lane aborts only a request
that has not answered yet.

Finding: `http_petitio_tempus_ponere` bounds only the CONNECT phase; a
server that accepts and then stays silent is governed by the idle
timeout (`HttpVocatioOptiones.mora_otii_ms`, default 60 s). The house
veredarius sets both from the request's `tempus_ms`, so "tempus" means
what callers think. The client's longest request is the long-poll
(mora 25 s + margin 15 s = 40 s), still above the 25 s answer time.
