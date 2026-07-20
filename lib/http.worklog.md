# http.c worklog


## 2026-07-17 — receive-loop error path was a live crash vector

VANA adjudication find: http_exsequi's receive loop read
tcp_recipere/tls_recipere into unsigned `i32 n`. A transport error
mid-response returned -1 = ~4e9, which PASSED `si (n > 0)` as
"received data" and fed memcpy a (size_t)4e9 length from a 4KB
buffer. Not a dead check — an actively wrong one. `n` is now s32,
errors exit the loop; the positive-n accumulations cast back into
the i32 totals explicitly. Also removed the dead
`max_redirectiones < 0` validation (unsigned param, plain count
semantics, no callers pass negatives — the check could never fire).

## 2026-07-20 — hospitium Phase A: parser guards, lenient-LF, limits, serializer

Guard design that matters: status_suggestus is a FIELD on
HttpParseResultus (and the parser), NOT a new HttpParseStatus —
phases stay a state machine, the guard class is orthogonal
metadata, and every existing phase consumer compiled untouched.
Helpers set parser->status_suggestus and return FALSUM; the two
FALSUM branches in adicere mirror it into the result with a CD
default. Guards: 501 unknown method (O3 hard at parse), 400
version (replaced a SILENT default-to-1.1 — "GET /path" with no
version used to parse fine), 400 dup-CL and CL+TE in both arrival
orders via vidit_* flags, 413 announced-CL fail-fast (fires at
header parse, before any body buffering), 411 chunked (the old
TODO stub silently declared COMPLETA and discarded the body).

Lenient-LF: _parser_invenire_crlf → _parser_invenire_lf; the
'\r' is stripped RETROACTIVELY by the caller (linea_finis--), so
a CRLF split across two adicere feeds is safe — we wait for the
'\n', the '\r' is already buffered. The two parse helpers needed
zero changes: their param was already an exclusive content end.
Dropped the old <II early-out — a lone '\n' at position 0 is a
valid (empty) line.

Limits: total cap check is `longitudo > petitio_maxima -
buffer_mensura` — that subtraction is safe ONLY because the check
itself maintains the invariant buffer_mensura <= petitio_maxima
(i32 is unsigned here; the naive `mensura + longitudo > maxima`
form can wrap). URI cap is safe because uri_finis >= uri_initium
is guaranteed by the empty-URI check above it.

Reliquiae: made end-of-request honest first (cursor +=
content_length before COMPLETA) — before this, cursor stopped at
body start and buffer_mensura - cursor would have counted the
body itself as "leftover".

Serializer: one _serialize_impl; the 2-arg form is (FALSUM,
FALSUM), _cum_conexione is (VERUM, keep_alive) [O2 additive].
CL is now ALWAYS emitted (even 0 — the real keep-alive framing
fix) unless _http_corpus_prohibitum (1xx/204/304) [O1]. Caller
CL/Connection headers are skipped in the user-header loop —
serializer owns framing. Existing-caller bytes unchanged in
practice (all six either have bodies or are 204).

reset stays UNUSED by hospitium (reficere + fresh creare per the
memory story); it now clears status_suggestus + vidit_* flags but
NOT the two config fields — limits survive reset by design.
