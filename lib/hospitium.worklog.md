# hospitium.c worklog

## 2026-07-20 — Phase C: the core, first-fix compile, first-run green

95/95 asserts green on the FIRST run of probatio_hospitium — the
calibration-first arc (audit → interview → spec v2 line-level
surgery → phases 0/A/B before this file existed) paid out exactly
like vitrea's did.

Memory story (the load-bearing design): each connection owns a
dynamic piscina. Order matters — tcp_servus_accipere allocates the
TcpConnexio INTO that piscina first, THEN basis = piscina_notare,
THEN the parser. So piscina_reficere(basis) at request boundary
kills parser + buffer + request + response together but keeps the
TcpConnexio. Reset is DEFERRED until the write tail drains (the
response bytes live in the piscina being reset), and the parser is
re-CREATED, never http_parser_reset (would dangle after reficere).

Accept quirk: the accept loop must create the per-connection
piscina BEFORE accepting (accipere needs an arena for the
TcpConnexio), so the drain-terminating TCP_ERROR_ITERUM costs one
piscina create+destroy per drain. Accepting into a long-lived
arena instead would leak a TcpConnexio per connection forever —
the wasted create is the correct price.

503-at-cap is piscina-free by construction: a static string
written best-effort with one tcp_mittere before close. Visible
refusal, not a silent SYN backlog.

HEAD = GET-without-body is implemented at the SERIALIZED level:
run the GET handler, serialize normally (Content-Length states the
true GET body size), then truncate the outgoing chorda at the
first \r\n\r\n + 4. One code path, no handler awareness.

Timeouts: ONE repeating 1s sweep timer walks the connection array
comparing f64-second deadlines against tempus_nunc() — not
per-connection timers (128-timer reactor cap; a 128-slot linear
sweep is nothing). Two deadlines: capita (armed until parse
COMPLETA) and otium (bumped on every read/write).

In-process test pattern that makes fork unnecessary for Phase C:
blocking client with SO_RCVTIMEO 2s (tcp_connectere_cum_optionibus
timeout_ms — the test can fail but never hang) + manual
hospitium_gressus pumping. Send lands in the socket buffer, pumps
run accept/read/dispatch/write, then the client recv finds the
response waiting. fork stays Phase D's business (http_exsequi
blocks on recv and needs a live pump on the other side).

En-route find: compile_tools.sh had NO -framework WebKit — every
tool link was latently broken since vitrea_macos.o joined build/.
One-word fix. Same class as the frigida.sh vigilia.o lesson:
mtime-gated/glob link lists rot silently until a new consumer
forces the link.

Deferrals (named): directory listing hrefs are relative (a
listing reached without trailing slash resolves siblings wrong —
proper fix is a 301 redirect to the slashed path); no HTML
escaping in listing titles; OPTIONS has no automatic handler
(405/404 path; Allow already renders it).
