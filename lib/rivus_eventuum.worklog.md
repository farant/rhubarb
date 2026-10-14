# rivus_eventuum worklog

## 2026-10-14 — born (quaestio-rivus T3)

Server-Sent Events reader, bytes in / (event, data) out, no I/O - the
middle layer of streaming $question (http_lector body -> this ->
vates' Anthropic event decoder). Named `rivus_eventuum`, not `sse_*`
(plan ruling R3: the Latin lint would refuse the abbreviation).

- Line ends CRLF / LF / CR; a CR at the end of one feed with LF at the
  start of the next is ONE line end (`cr_pendens`). The plant that
  dropped this turned the CRLF case red at every split.
- BOM: a separate "judged" flag; a partial BOM match that turns out
  not to be a BOM is replayed as ordinary text (the plan's sketch
  mixed the two states in one counter and was fragile - rewritten).
  BOM in the middle of the stream is text (tested).
- Dispatch on the empty line only if a `data:` was seen; the event
  name resets after every dispatch (tested: it does not leak into the
  next event). `finire` drops an unterminated event (WHATWG).
- Strings handed to the callback point into the builders and die at
  the next feed: callers copy what they keep.
- Oracle: every case whole, byte by byte, every 2-way split (7/7 first
  run). Plants: empty-line dispatch disabled, CR-pending off - both
  red.
