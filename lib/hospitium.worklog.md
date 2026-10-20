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

## 2026-10-16 - hospitium v3 T2: deferred responses, shared reactor, body sources, soft stop

- **The colloquium now lives inside its connection slot** (was a stack
  value per request). Needed so a deferred response can outlive the
  handler call. Consequence: a pointer to it is NOT a safe handle - the
  slot is reused by the next connection, and an old pointer would then
  address a live, unrelated request. Hence `HospitiumDilatum` (slot +
  generation) and the generation bump at accept / request / close /
  503 deadline. If you ever add a new path that ends or restarts a
  request, bump `generatio` there too.
- **Plant lesson:** the first `generatio` test was mute - B's request was
  answered immediately, so `conn->dilatum == FALSUM` already rejected
  the old handle. Only a second DEFERRED request in the same slot needs
  the generation. Test now asserts `y.d.locus == x.d.locus` so it
  provably exercises reuse.
- **EXSPECTANS keeps reading:** a deferred connection still polls for
  read. EOF -> close (handle invalid immediately). Bytes (pipelined next
  request) -> `claudenda_post` + interest 0; we do not queue a second
  request behind a deferred one.
- **Body sources** pull HOSPITIUM_CAPSA_FONTIS (16 KB) per write
  opportunity. A source that errors or ends short of its `mensura`
  closes the connection: the client sees a truncated body, never a
  wrong-but-complete one. Directory serving goes through the file
  source now (big files no longer loaded whole).
- **Shared reactor:** `configuratio.reactor` non-NIHIL -> the server
  never destroys it; `hospitium_destruere` removes the listening fd and
  cancels its vigilia timer so the app's reactor stays clean.
- **Test harness:** client recv timeout 20 ms. With 2 s the test client
  blocked while the single-threaded server (same process) needed pumping.

## 2026-10-17 - hospitium v3 T3: request bodies by stream

- **Parser seam:** connections create their parser with
  `http_parser_capita_solum(VERUM)`; at the empty line with
  Content-Length > 0 it stops (CAPITA_COMPLETA) and hands over the
  bytes already buffered (`http_parser_corpus_initium`). From then on
  `_legere` feeds raw reads to the body sink, not the parser.
- **Sinks:** multipart -> multipartita with callbacks (fields in a
  chorda_aedificator, file parts to temp files with SHA-256 per write);
  other bodies <= `corpus_in_memoria_maximum` -> one piscina buffer
  (`petitio->corpus` as before); bigger -> temp file
  (`petitio->corpus_via`).
- **Temp-file lifetime:** paths live in the request piscina, so
  `_corpus_purgare` MUST run before `piscina_reficere` (in
  `_paratus_novae`) and before `piscina_destruere` (in
  `_connexionem_claudere`). `fila_numerus++` happens BEFORE the temp
  file is opened so a failure mid-open is still purged.
- **Bytes past Content-Length** (pipelining) set `reliquiae_corporis` ->
  keep-alive off, same honest close as the old parser path.
- **petitio_maxima now means body size** in this mode; headers have
  their own 64 KB cap (431). The parser's existing CL > petitio_maxima
  check gives the 413 before any body byte is read.
- The whole body is streamed even when the route will 404 - routing
  happens after the body. Cheap to change later (route at
  CAPITA_COMPLETA) if anonymous uploads to unknown paths ever matter.

## 2026-10-17 - hospitium v3 T4: request helpers, cookies, proxy trust

- **One door for response headers:** `_caput_ponere` (title token,
  value without CR/LF/NUL, cap 32 + 2 reserved). Anything refused bumps
  `fructus.capita_abiecta` and the access log line ends with
  `capita_abiecta=N (Title)`. The colloquium counter is reset in
  `_paratus_novae` because router 404s never memset the colloquium.
- **Lazy caches in the colloquium** (quaestiones, forma, cliens, par):
  all in the request piscina, so they die with the request; the
  colloquium memset at handler entry clears them.
- **Proxy trust is double-gated:** config `procurator_fidus` AND the
  socket peer is 127.*. Only then X-Forwarded-For (last header, last
  hop) and X-Forwarded-Proto. The default cookie options read
  `colloquium_schema`, so behind nginx with https the session cookie
  gets `Secure` automatically.
- Testing the access log: the test swaps fd 2 for a pipe with
  dup/dup2 around the requests, then reads the pipe - no file needed.
