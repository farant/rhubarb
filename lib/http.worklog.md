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
