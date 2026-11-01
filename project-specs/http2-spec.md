# http2 - our own HTTP/2: hpack, frames, a sans-IO connection, ALPN (spec)

**STATUS: IMPLETUM 2026-10-31** (phase 1, client role, Mac) - plan
`project-specs/http2-plan.md` T1-T12, commits 04bb7b8b..3262768c in
quarta; final review (fable) found no Critical, two Important
lifecycle gaps fixed with RED->GREEN tests (GOAWAY covering queued
streams; `http2_connexionem_destruere`), ten Minors deferred to the
tabularium. Rulings: lib/http2*.worklog.md, lib/apns.worklog.md.
Server role, Linux, enabling push on pictor: the push-service project.

*Earlier:* **PROPOSITUM 2026-10-30** (quarta; brainstorm with Fran the
same evening, right after communio phase 5 went live; builder check
done - D8, D9 and the D7/III.4 calls came out of it).

**FINIS LINE MET 2026-10-31** (T11, live, Fran's sandbox key
LA8Z282UYC, team Q5WC9FQ3J2, topic earth.golden.pictor): `apns -probare`
-> api.sandbox.push.apple.com answered `:status 400, reason
"BadDeviceToken"` (apns-id 930DF12F-C7C0-D46B-266E-ED7FCD20CB1F) - our
hpack + frames + connection + ALPN over tls, our ES256 token, no
library of theirs. Counter-check with `-clavis_id ZZZZZZZZZZ` -> `403
InvalidProviderToken` (apns-id 6DCCB34B-205F-8159-5652-35C2C0D0A0F6), so
Apple really did verify the signature. Pictor's App ID still has
PUSH_NOTIFICATIONS off: the sandbox accepted the topic without it, so
switching it on waits for the push-service project, on Fran's word.

*Fran 2026-10-30, after TestFlight build 8: "what would be involved in
notifications? ... i don't know if notifications is something that
would be some kind of platform type thing" - then, on hearing that
Apple's push service (APNs) speaks only HTTP/2: "it sounds like http/2
might be a good project in and of itself to focus on first." On the
server side: "replacing nginx is not a pressing need but long term it
could be nice to have one less dependency if we can do a good job
replacing the functionality."*

Goal: **an HTTP/2 client stack of our own** (RFC 9113 + RFC 7541),
built in four layers, the lower three pure (bytes in, bytes and events
out, no sockets), so they test against the RFCs' byte-exact examples
and recorded traffic. The connection layer is designed for both roles
(client and server) from day one; only the client role is built now.
The phase ends when one real request to **Apple's sandbox push
server**, signed with an APNs key and carrying a deliberately fake
device token, comes back over HTTP/2 as `400 {"reason":
"BadDeviceToken"}` - which proves ALPN, HTTP/2, HPACK, flow control and
the signed token against Apple's own server without any phone work.

Motivation, in one line: "Share new pictures" in pictor should end in a
notification on the family's phones; a push to an iPhone has to go
through APNs; APNs is HTTP/2 only. HTTP/2 is also a lasting house
capability (one connection, many requests) independent of push.


## I. What exists (main bc76aff9, 2026-10-30)

- **Transport.** `tcp` (non-blocking connect) and `tls.h` with three
  backends behind one interface (`include/tls.h:60-65`: ORDINARIA,
  BEARSSL, SECURETRANSPORT, TLS13). The non-blocking path is
  `tls_incipere` / `tls_gradus` / `tls_mittere` / `tls_recipere`
  (`include/tls.h:169-235`), returning TLS_ITERUM rather than waiting.
  Our own TLS 1.3 client is the house default (tls13-spec, IMPLETUM
  2026-10-16); BearSSL serves 1.2 servers.
- **No ALPN anywhere.** `TlsOptiones` (`include/tls.h:68-84`) has no
  protocol list; the TLS 1.3 ClientHello is built in
  `lib/tls13_nuntii.c:303-371` (server_name, supported_versions, ...)
  and extensions are read in `tls13_extensiones_legere`
  (`lib/tls13_nuntii.c:491`). The vendored BearSSL has ALPN
  (`br_ssl_engine_set_protocol_names` in
  `vendor/bearssl/inc/bearssl_ssl.h`), unused by `lib/tls_bearssl.c`.
- **HTTP/1.1.** `http.h` (blocking client), `http_lector` (response
  parser), `http_vocatio` (one non-blocking call on the reactor:
  NOMINA -> CONNEXIO -> PACTIO -> MITTENS -> LEGENS,
  `include/http_vocatio.h:1-30`), and `veredarius` (the platform HTTP
  interface from communio plan-5; URLSession on iOS).
- **Signed tokens.** `nundinae_apple_tesseram_facere`
  (`include/nundinae_apple.h:87-100`) makes an ES256 JWT over BearSSL
  for App Store Connect - claims are fixed to that API (`aud`
  "appstoreconnect-v1", `exp`). APNs wants a different claim set
  (`iss` = team id, `iat`; header `kid`), so the signing core is
  reusable but the function is not, as is.
- **Huffman.** `lib/flatura.c` decodes DEFLATE's canonical Huffman
  codes; HPACK uses one fixed table (RFC 7541 Appendix B), so little
  carries over beyond the idea.
- **Oracles on this machine** (2026-10-30): `openssl` 3.3.2
  (Homebrew; already the TLS 1.3 test server via
  `probationes/servus_tls.h`, and `s_server -alpn` does ALPN); Go 1.23.2
  at `/usr/local/go` (standard library serves HTTP/2 over TLS;
  `GODEBUG=http2debug=2` logs every frame; `golang.org/x/net` v0.10.0
  in the module cache has `http2/hpack` and `h2c`); system `curl`
  8.7.1 built with nghttp2. The nghttp2 command-line tools (`nghttp`,
  `nghttpd`) and `h2spec` are **not** installed (only libnghttp2 via
  Homebrew).
- **APNs credentials.** None yet. The existing
  `~/.rhubarb/ios/AuthKey_B5K887DQ73.p8` is the App Store Connect
  team API key, not an APNs key. Secret: mode checked, never read.


## II. Decisions (Fran 2026-10-30)

- **D1 Client first, both roles designed.** The connection object takes
  a role (client / server) from the start - preface direction,
  odd/even stream ids, which frames each side may send. Only the
  client role is implemented and tested in this spec. The server role,
  server-side TLS and replacing nginx are long-horizon (see V).
- **D2 Four layers** (agreed in conversation before the brainstorm):
  1. `hpack` - header compression, pure;
  2. `http2_forma` - frame encoding/decoding and validation, pure;
  3. `http2` connection - pure, bytes in / events and bytes out
     (sans-IO), in the spirit of `http_lector`;
  4. glue - ALPN in `tls.h` and a persistent HTTP/2 connection on the
     reactor.
- **D3 Names.** `hpack` and `http2` stay as technical terms (as
  `tls13` did); everything inside is Latin per house rules.
- **D4 Finish line.** Phase 1 ends at Apple's sandbox
  (`api.sandbox.push.apple.com:443`) answering a fake device token
  with `400` and `{"reason":"BadDeviceToken"}` over our stack. Before
  that, the client works against a local Go server and public HTTP/2
  sites.
- **D5 Oracles, not dependencies.** Reference programs (Go, openssl,
  curl, nghttp2 if installed later) run only as child processes in
  tests and tools, never linked or copied; corpora are kept as test
  data with their source and commit recorded. A missing oracle makes a
  suite exit 2 (NOTHING RAN), never pass.
- **D6 Out of scope here** (next project): the push service in
  exemplar (token registry, send queue, provider adapters), the iOS
  notification service in the shell, and pictor's "Share new pictures"
  (Fran: an option that appears once there are unshared changes) -
  ledger wish 01M67MAP68Y4ECFWFVMJ7YCD57.
- **D8 Mac first** (Fran, after the builder check): this phase is done
  on the Mac. Building and running the stack on the Linux VPS, trust of
  Apple's chain on Linux, and sending from the VPS move to the
  push-service project, where they are exercised for real.
- **D9 Go as a fixture generator, not a test requirement** (Fran: "long
  term ... use it as a way to derive our own test behavior so someone
  doesn't have to install go to run the project or its tests"). One
  small Go program using the **standard library only** (no `go.mod`
  dependencies, no `golang.org/x/net`) lives under a tools directory as
  the local HTTP/2 server oracle. Its job is to **record**: traces of
  real conversations (frames as octets, with `GODEBUG=http2debug=2`
  logs alongside) are committed as fixtures under `probationes/fixa/`,
  with the generating command and Go version recorded in the fixture
  (the house GENERATUM idea: regenerate with Go, compare byte for
  byte). The default suites replay those fixtures and need no Go; the
  live run against the Go server is a separate gate that exits 2 when
  `go` is absent. Long term, our own server role (V) replaces the Go
  server for the local glue test.
- **D7 Fran's one manual step:** creating an APNs key in the developer
  portal (Keys -> Apple Push Notifications service) and saving the
  `.p8` under `~/.rhubarb/ios/` mode 0600; Claude is given the key id
  and team id (not secret) and never reads the key. *(Claude's call,
  after the builder check:)* the key must be usable for **sandbox**
  (choose "Sandbox & Production" or sandbox) and must not be limited
  to topics that exclude `earth.golden.pictor`. The Push Notifications
  capability on the App ID `earth.golden.pictor` is switched on by our
  tooling through the App Store Connect API (`nundinae_apple`,
  create-what-is-missing, never revoke) rather than by hand. A
  **preflight** step runs before the finish line and checks what can
  be checked without reading the key: file present and 0600, key id
  and team id configured, capability on - so an account problem
  surfaces as an account problem, not as a stack bug.


## III. Design

### III.1 `hpack` (RFC 7541)

Pure library: static table (61 entries), dynamic table with size
limit and eviction, integer coding with N-bit prefixes, string literals
raw or Huffman (Appendix B table, 257 symbols incl. EOS), and the four
representations (indexed, literal with incremental indexing, without
indexing, never indexed) plus dynamic table size updates.

- **Decoder** (one per connection direction): header block bytes in,
  list of (name, value, never-indexed flag) out, in the caller's arena.
  Errors are COMPRESSION_ERROR conditions: bad index, integer
  overflow, Huffman padding longer than 7 bits or not all ones, EOS in
  a string, table size update above the SETTINGS limit or not at the
  start of a block. A cap on decoded header list size is a parameter
  (defence against "HPACK bombs"; RFC 7541 §7).
- **Encoder**: header list in, bytes out; maintains its own dynamic
  table mirror; emits a size update when the peer's
  SETTINGS_HEADER_TABLE_SIZE changes. **Sensitive headers are encoded
  never-indexed** - at least `authorization` (APNs sends
  `authorization: bearer <jwt>`) and `cookie` short values; Huffman is
  used when it is shorter.
- No I/O, no allocation outside the given arena, deterministic output
  for a given input and table state (so tests can be byte-exact).

### III.2 `http2_forma` (RFC 9113 §4, §6)

Pure frame codec: the 9-octet frame header (length, type, flags,
stream id with the reserved bit), and the frame types DATA, HEADERS,
PRIORITY (parsed and ignored - RFC 9113 deprecates the scheme),
RST_STREAM, SETTINGS (+ACK), PUSH_PROMISE (recognised; a client that
sent SETTINGS_ENABLE_PUSH=0 treats it as a connection error), PING
(+ACK), GOAWAY, WINDOW_UPDATE, CONTINUATION. Padding (PADDED flag) on
DATA and HEADERS. Unknown frame types are skipped (RFC 9113 §4.1).

Validation per RFC 9113 §6: stream id rules per type, fixed lengths
(PING 8, WINDOW_UPDATE 4, RST_STREAM 4, SETTINGS multiple of 6),
padding not longer than the payload, frame size above
SETTINGS_MAX_FRAME_SIZE -> FRAME_SIZE_ERROR. The codec reports which
error and whether it is a stream or connection error; it does not act.

### III.3 `http2` connection (sans-IO)

One object per connection, created with a **role** (D1) and settings.
The caller feeds received octets in and drains octets to send; the
object produces events. No sockets, no clock - timers (e.g. PING
liveness) are the glue's business, driven through explicit calls.

- **Client role, implemented:** sends the connection preface
  (`PRI * HTTP/2.0...` + SETTINGS), acknowledges the peer's SETTINGS,
  answers PING, opens streams with odd ids, submits a request as
  pseudo-headers (`:method`, `:scheme`, `:authority`, `:path`) +
  headers + optional body (DATA, END_STREAM), respects
  SETTINGS_MAX_CONCURRENT_STREAMS (requests beyond it wait in order).
- **Events out:** response headers (`:status` + headers, incl. trailers
  later in the stream), body chunks, stream end, stream reset (with
  error code), GOAWAY (last stream id, error code, debug data - streams
  above it are reported as not processed, safe to retry), PING ack,
  connection error.
- **Flow control** both directions (RFC 9113 §5.2): connection and
  stream windows, initial 65535; sending stalls when a window is empty
  and resumes on WINDOW_UPDATE; receive windows are replenished
  automatically by policy (left to the builder) so a large body never
  stalls; a window above 2^31-1 is FLOW_CONTROL_ERROR.
- **Our SETTINGS** (defaults, adjustable): ENABLE_PUSH 0,
  HEADER_TABLE_SIZE 4096, INITIAL_WINDOW_SIZE generous enough for
  APNs-sized bodies (builder's call), MAX_HEADER_LIST_SIZE stated.
- **Errors:** stream errors -> RST_STREAM on that stream only;
  connection errors -> GOAWAY with the code and the connection marked
  dead (RFC 9113 §5.4). Header blocks split over CONTINUATION are
  reassembled with a cap on total size and frame count (the 2024
  "CONTINUATION flood" class) - cheap now, essential for a future
  server.
- **Server role:** the type, the role flag and the rule tables exist so
  the API does not change later; constructing a server-role connection
  is refused (or clearly unsupported) in this phase. The "rapid reset"
  (2023) and stream-churn limits are noted for the server phase, not
  built.

### III.4 Glue: ALPN and the reactor

- **ALPN in `tls.h`.** `TlsOptiones` gains an ordered protocol list
  (e.g. `{"h2", "http/1.1"}`); a new accessor returns the protocol the
  server selected (empty if none). Implemented in:
  - our TLS 1.3 (`application_layer_protocol_negotiation`, extension 16,
    in the ClientHello; the server's choice read from
    EncryptedExtensions; a choice we did not offer is a handshake
    error);
  - BearSSL (`br_ssl_engine_set_protocol_names` /
    `br_ssl_engine_get_selected_protocol`), so 1.2 servers work too;
  - SecureTransport: builder's call - implement if cheap, otherwise it
    reports no selection and HTTP/2 refuses to run over it (it is the
    old backend, parked).
  Existing callers that pass no list behave exactly as today.
- **Persistent connection on the reactor.** Unlike `http_vocatio` (one
  call, one connection), an HTTP/2 connection is long-lived and carries
  many requests (APNs asks clients to keep one open). The glue owns
  NOMINA -> CONNEXIO -> PACTIO with ALPN "h2" required (no fallback to
  HTTP/1.1 inside this object - a server that does not pick h2 is an
  error the caller sees), then pumps octets between `tls_*` and the
  connection object, delivers per-request completions (status,
  headers, body) to callbacks, and reports GOAWAY / connection loss so
  a caller can reconnect. Timeouts follow `http_vocatio`'s model
  (connect and idle).
- **Signed token for APNs.** The ES256 JWT core moves out of
  `nundinae_apple` into a small reusable piece (header + claims given
  by the caller); `nundinae_apple_tesseram_facere` keeps its exact
  output on top of it (its suite stays green). APNs claims: header
  `{"alg":"ES256","kid":<key id>}`, claims `{"iss":<team id>,"iat":
  <now>}`; Apple accepts a token for up to an hour and asks that it not
  be regenerated more than every 20 minutes - the APNs tool reuses one.
- **The finish-line tool.** A small command (house tool, name left to
  the builder) that reads the APNs key path, key id, team id and topic
  (`earth.golden.pictor`) from a config file (the
  `~/.rhubarb/ios/nundinae_apple.stml` pattern), opens one HTTP/2
  connection to `api.sandbox.push.apple.com:443`, and sends `POST
  /3/device/<64 hex zeros>` with `apns-topic`, `apns-push-type: alert`,
  `authorization: bearer <jwt>` and a tiny `{"aps":{"alert":"probatio"}}`
  body. Success = `:status` 400 and body reason `BadDeviceToken`. The
  key is never printed, logged or echoed in an error (house secret
  rules).
  *Fallback (Claude's call, after the builder check):* if Apple answers
  the all-zero token with a different JSON reason, the tool retries
  once with a random well-formed 64-hex token; the gate passes on any
  APNs answer that proves the token was accepted - an HTTP/2 response
  with a JSON `reason` that is **not** one of `InvalidProviderToken`,
  `ExpiredProviderToken`, `MissingProviderToken`,
  `TopicDisallowed`, `BadTopic` (those mean account/token trouble and
  fail the gate with the reason shown). The exact answers seen are
  written into this spec's status line when the gate runs.


## IV. Tests and oracles

- **hpack** - byte-exact:
  - RFC 7541 Appendix C: C.1 (integers), C.2 (single literals), C.3/C.4
    (three requests without / with Huffman, dynamic table state after
    each), C.5/C.6 (three responses without / with Huffman, 256-octet
    table with eviction). Encode where the RFC's choices are
    reproducible, decode always.
  - Appendix B: every one of the 257 codes round-trips; padding and
    EOS error cases.
  - The `hpack-test-case` corpus (github.com/http2jp/hpack-test-case;
    JSON "stories" encoded by several implementations): decode every
    story to the given headers; re-encode with ours and decode with
    ours to the same headers. Commit pinned and recorded with the
    fixture; licence checked before vendoring the data (AUDIENDA).
  - Bombs and caps: oversized header list, table size update abuse.
- **http2_forma** - hand vectors for every frame type and every
  validation error in III.2, plus the frames inside the recorded Go
  conversations (D9) decoded and re-encoded byte for byte. (No
  `x/net` Framer tool - D9 keeps Go to the standard library.)
- **http2 connection** - scripted conversations fed as bytes: preface
  and SETTINGS exchange, request/response, interleaved streams, a body
  larger than the initial window (flow control both ways), PING,
  GOAWAY mid-flight (streams above last-stream-id reported retryable),
  RST_STREAM, CONTINUATION reassembly and its caps, every connection
  error path in III.3. Captured traces from the Go server
  (`GODEBUG=http2debug=2`) may be replayed as fixtures.
- **ALPN** - `openssl s_server -alpn h2` (extends the
  `probationes/servus_tls.h` oracle) for TLS 1.3 and for TLS 1.2 via
  BearSSL: offered list, selected protocol, server choosing nothing,
  server choosing something we did not offer (1.3: handshake error).
- **Glue, local** - the stdlib-only Go HTTPS server (D9; child process,
  self-signed certificate trusted via the existing `ancora_via`
  option): GET and POST, many concurrent requests on one connection, a
  multi-megabyte body, the server sending GOAWAY, the server closing.
  This run is the recording session for the replay fixtures and a gate
  of its own (exit 2 if `go` is absent); the default suites replay the
  committed recordings without Go.
- **Glue, live (opt-in, like `probatio_http_vocatio_rete`)** - a
  public HTTP/2 site (e.g. nghttp2.org, www.google.com) over TLS 1.3
  and a 1.2 one if found.
- **Finish line (manual gate, with Fran's APNs key)** - the APNs tool
  against the sandbox: `400 BadDeviceToken` (or the fallback rule in
  III.4). A second run with a deliberately wrong key id must come back
  with a provider-token rejection (`403`, `InvalidProviderToken` or
  close kin), proving the token is actually checked.


## V. Not in this spec

- Server role implementation, server-side TLS, replacing nginx (Fran:
  long term, "if we can do a good job"); h2spec belongs to that phase.
- The push service in exemplar, iOS notification plumbing, pictor's
  "Share new pictures" (D6).
- HTTP/1.1 Upgrade to h2c (removed in RFC 9113), cleartext h2 to
  remote hosts, server push, PRIORITY scheduling, RFC 9218
  priorities, extended CONNECT / WebSockets over HTTP/2, HTTP/3.
- Making `veredarius` or the blocking `http.h` prefer HTTP/2 - welcome
  later, not required here.


## VI. Left to the builder

- HPACK encoder indexing heuristics beyond "sensitive = never indexed"
  and "Huffman when shorter".
- Internal data structures, buffer sizes, arena use, file splits within
  each layer, Latin identifiers.
- Receive-window replenishment policy and our exact SETTINGS values
  (within III.3).
- Whether the ES256 core becomes its own library or a file shared by
  the two users, and the finish-line tool's name and config keys.
- Whether SecureTransport gets ALPN (III.4).
- Whether to install the nghttp2 command-line tools as an extra oracle
  (ask Fran first - it is a Homebrew install on his machine).


## AUDIENDA (not verified while writing)

- `hpack-test-case` licence and current layout (JSON fields per story).
- Exact APNs sandbox answer to an all-zero device token (believed `400`
  `BadDeviceToken`; Apple may instead say `BadDeviceToken` only for
  well-formed tokens of the right length - 64 hex chars assumed) and
  to a wrong key id (`403 InvalidProviderToken` believed).
- Current developer-portal steps for an APNs key (token-based auth key,
  usable for sandbox and production).
- Whether Go 1.23's server needs anything beyond `ListenAndServeTLS`
  to negotiate h2 with a client whose certificate trust comes from
  `ancora_via` (believed: no).
- Whether our TLS 1.3 client builds and passes on Linux the same way
  (out of this phase by D8; exemplar already reaches Resend over HTTPS
  there, so believed yes).
- Whether the house commit gates (Latin identifier lint, examen,
  fabrica classification) ignore a `.go` file cleanly or need an
  exemption - the first Go source in the tree.
