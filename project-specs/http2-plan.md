# http2 (hpack, frames, sans-IO connection, ALPN, reactor glue) - Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.
> **House rule (Fran):** executed INLINE, one task per turn, Fran approves each before the next.

**Goal:** an HTTP/2 client stack of our own - HPACK, a frame codec, a pure connection object with a role, ALPN in `tls.h`, and a long-lived connection on the reactor - ending when Apple's sandbox push server answers our request with `400 {"reason":"BadDeviceToken"}`.

**Architecture:** three pure libraries (`hpack`, `http2_forma`, `http2`) that take octets in and give octets and events out, tested byte for byte against RFC 7541 Appendix C, hand vectors and recorded conversations; then ALPN in all TLS backends that can carry it, a reactor-driven `http2_nexus` that pumps `tls_*` into the connection object, a reusable ES256 token (`tessera_es256`) carved out of `nundinae_apple`, and a small `apns` tool for the finish line.

**Tech Stack:** C89 in Latin (latina.h), `tcp`, `tls.h` (tls13 own / BearSSL / SecureTransport), `nomina`, `reactor`, `xar`, `piscina`, `chorda`, `base64url_*`, vendored BearSSL (ECDSA P-256, SHA-256), credo v2 suites, `silva.commissio`; oracles as child processes only: `openssl s_server -alpn` (via `probationes/servus_tls.h`), Go 1.23 standard library (one test server), `curl --http2`.

**Spec:** `project-specs/http2-spec.md` (PROPOSITUM 2026-10-30, approved by Fran the same evening): D1-D9, III.1-III.4, IV.

## Calls made in this plan

- **C1 File and type names.** `include/hpack.h` + `lib/hpack.c` (+ `lib/hpack_huffman.c` for the Appendix B table), `include/http2_forma.h` + `lib/http2_forma.c`, `include/http2.h` + `lib/http2.c`, `include/http2_nexus.h` + `lib/http2_nexus.c`, `include/tessera_es256.h` + `lib/tessera_es256.c`, `tools/apns.c`. Types `HpackCaput`, `HpackTabula`, `Http2Forma`, `Http2Iudicium`, `Http2Connexio`, `Http2Eventus`, `Http2Nexus`, `Http2Responsum`.
- **C2 HPACK encoder policy** (spec VI leaves heuristics to us): exact match in static or dynamic table -> indexed; otherwise literal *with incremental indexing* (name indexed when the name is in a table) - this reproduces RFC 7541 C.3-C.6 byte for byte; `authorization`, `proxy-authorization` and `cookie` values shorter than XX octets are *never indexed* (RFC 7541 §7.1.3); an entry larger than the table is literal without indexing. Huffman mode is a per-table setting: `HPACK_HUFFMAN_SI_BREVIOR` (default), `_SEMPER`, `_NUMQUAM` (the last two exist so C.3/C.4 and C.5/C.6 can be reproduced).
- **C3 One request = one stream id at once.** `http2_petere` returns the stream id immediately; requests above the peer's MAX_CONCURRENT_STREAMS wait in a FIFO and are sent in id order as streams close (ids stay increasing on the wire).
- **C4 Our SETTINGS:** ENABLE_PUSH 0, HEADER_TABLE_SIZE 4096, INITIAL_WINDOW_SIZE 1048576 (1 MiB), MAX_HEADER_LIST_SIZE 65536; connection receive window raised to 16 MiB at start with one WINDOW_UPDATE; receive windows are topped up when half consumed. Caps: CONTINUATION frames per header block VIII, header block octets 65536.
- **C5 Recording, not linking (D9).** `http2_nexus` has a test-only tap (`Http2NexusOptiones.tabula_via`) that appends the decrypted octets each direction to two files; the Go server runs with `GODEBUG=http2debug=2`. A recording session writes `probationes/fixa/http2/<casus>.{servus,cliens}.bin` + `<casus>.go.log` + a `GENERATUM` header file naming the command and `go version`. Default suites replay these with no Go.
- **C6 The Go oracle** is one file, `tools/oracula/http2_servus.go`, standard library only, run with `go run` (no `go.mod`): flags `-portus`, `-cert`, `-clavis`; routes `/salve` (200 "salve"), `/eco` (echoes the POST body), `/magnum?n=` (n octets), `/tarde?ms=` (delays), `/abire` (sends GOAWAY by closing the server gracefully after answering).
- **C7 ALPN API:** `TlsOptiones` gains `constans character* constans* protocolla; i32 protocolla_numerus;` (NIHIL/0 = as today); `chorda tls_protocollum(TlsConnexio*)` ("" when none). SecureTransport: not implemented this phase - `tls_protocollum` returns "" there and `http2_nexus` refuses (spec III.4 builder's call).
- **C8 APNs config:** `~/.rhubarb/ios/apns.stml` (pattern of `nundinae_apple.stml`): `<apns clavis_via="~/.rhubarb/ios/AuthKey_<ID>.p8" clavis_id="…" turma="…" thema="earth.golden.pictor" hospes="api.sandbox.push.apple.com"/>`. The key file is opened only inside `tessera_es256`'s caller with `<tolera codex="CANALIS_PRAETERITUS">` at the open site; its content never leaves that function.
- **C9 Push capability** on `earth.golden.pictor` is ensured by a new `nundinae_apple -capacitas <app> PUSH_NOTIFICATIONS` verb (create if missing, never remove), used by `apns -praeparare`.

## Global Constraints

- C89, house flags; Latin identifiers; `i32`/`i64` UNSIGNED; Roman numerals; `nomen`/`longus`/`interior` are macros, never identifiers; chorda not NUL-terminated; new C files via `./silva/scribe.sh`; examen 97 (raw IO only via `filum_*`/`lectiones_*`).
- `hpack`, `http2_forma`, `http2`: no sockets, no clock, no allocation outside the caller's piscina, deterministic output (spec III.1-III.3).
- Oracles only as child processes; no `go.mod`, no `golang.org/x/net`; a suite whose oracle is missing exits 2 (NOTHING RAN), never passes (D5, D9).
- Existing TLS callers that pass no protocol list behave exactly as today (spec III.4); `nundinae_apple_tesseram_facere` output stays byte-identical (its suite stays green).
- Secrets: the APNs key and App Store Connect key are never read by Claude, printed, logged, put in argv or an error; the JWT is never logged; `authorization` is never indexed (C2). Check modes with `stat -f %Lp` only.
- Mac only this phase (D8). On Apple's side: create what is missing, never revoke or delete. Fran approves each task; ff main only on his word.

## Review Focus

1. The peer changes SETTINGS_INITIAL_WINDOW_SIZE while streams are open (smaller, even making a window negative): open streams' send windows adjust and sending resumes correctly -> T6 "fenestra mutata".
2. A response carries 1xx informational HEADERS before the final status, and trailers in a second HEADERS with END_STREAM -> T5 "informativa et appendices".
3. A header block's CONTINUATION is interrupted by a frame of another stream (or any non-CONTINUATION frame) -> connection PROTOCOL_ERROR, never a crash -> T5 "continuatio interrupta".
4. The TCP connection drops mid-flight with no GOAWAY: every pending request finishes exactly once with failure, the nexus reports the loss once -> T8 "abruptio".
5. A server sends a header larger than our cap or a table-size update above our SETTINGS -> COMPRESSION_ERROR / connection error, never memory growth -> T2 "bombae".

---

### Task 1: `hpack` primitives - integers, Huffman, static table

**Files:**
- Create: `include/hpack.h`, `lib/hpack.c`, `lib/hpack_huffman.c` (Appendix B: 257 codes and lengths as data), `probationes/probatio_hpack.c`, `lib/hpack.worklog.md`

**Interfaces:**
- Produces:
  - `i32 hpack_integrum_scribere(i32 valor, i32 prefixum, i32 primus, i8* exitus, i32 capacitas);` - `prefixum` 1..VIII bits; `primus` = the high bits already in the first octet; returns octets written, 0 if no room.
  - `b32 hpack_integrum_legere(constans i8* datum, i32 n, i32 prefixum, i32* valor, i32* consumpti);` - FALSUM on truncation or a value above 2^31-1.
  - `i32 hpack_huffman_longitudo(chorda s);` `i32 hpack_huffman_scribere(chorda s, i8* exitus, i32 capacitas);` `b32 hpack_huffman_legere(constans i8* datum, i32 n, Piscina* piscina, chorda* exitus);` - FALSUM on padding > VII bits, padding not all ones, or EOS inside.
  - `b32 hpack_staticum(i32 index, chorda* titulus, chorda* valor);` - index I..LXI.

- [ ] **Step 1: failing tests** - "integra": C.1.1 10 in 5 bits -> `0a`; C.1.2 1337 in 5 bits -> `1f 9a 0a`; C.1.3 42 in 8 bits -> `2a`; round-trip at every prefix for 0, 2^N-2, 2^N-1, 2^N, 2^31-1; truncated and overflowing inputs FALSUM. "huffman": `www.example.com` -> `f1e3 c2e5 f23a 6ba0 ab90 f4ff` (C.4.1), `no-cache` -> `a8eb 1064 9cbf`, `custom-key` -> `25a8 49e9 5ba9 7d7f`, `custom-value` -> `25a8 49e9 5bb8 e8b4 bf`; all 256 octet values round-trip singly and as one string; padding of 8 one-bits, padding with a zero bit, and the EOS code inside data all FALSUM. "staticum": 1 = `:authority`/"", 2 = `:method`/`GET`, 61 = `www-authenticate`/""; 0 and 62 FALSUM.
- [ ] **Step 2:** `./compile_tests.sh hpack` -> FAIL (no hpack.h). **Step 3:** implement (decoder walks the code table bit by bit or by a small state table - builder's choice). **Step 4:** PASS; `./tools/compile_tests_fontes_generare.sh`. **Step 5:** commit via `silva.commissio` (this plan goes in this commit).

### Task 2: `hpack` dynamic table, decoder and encoder

**Files:**
- Modify: `include/hpack.h`, `lib/hpack.c`, `probationes/probatio_hpack.c`, `lib/hpack.worklog.md`

**Interfaces:**
- Consumes: T1.
- Produces:
  - `nomen structura { chorda titulus; chorda valor; b32 numquam; } HpackCaput;`
  - `nomen enumeratio { HPACK_HUFFMAN_SI_BREVIOR = 0, HPACK_HUFFMAN_SEMPER, HPACK_HUFFMAN_NUMQUAM } HpackHuffman;`
  - `nomen enumeratio { HPACK_BENE = 0, HPACK_ERROR_INDEX, HPACK_ERROR_INTEGRUM, HPACK_ERROR_HUFFMAN, HPACK_ERROR_MAGNITUDO, HPACK_ERROR_TRUNCATUM, HPACK_ERROR_NIMIA } HpackError;` (NIMIA = decoded header list above the cap)
  - `HpackTabula* hpack_tabulam_creare(Piscina* piscina, i32 magnitudo);` - one per direction; `magnitudo` = SETTINGS_HEADER_TABLE_SIZE in force.
  - `vacuum hpack_magnitudinem_ponere(HpackTabula* t, i32 magnitudo);` - encoder: emits a size update at the start of its next block; decoder: the new upper bound for updates.
  - `vacuum hpack_huffman_ponere(HpackTabula* t, HpackHuffman modus);`
  - `HpackError hpack_decodere(HpackTabula* t, constans i8* datum, i32 n, i32 capita_maxima, Piscina* piscina, Xar* capita);` - appends `HpackCaput`; `capita_maxima` counts name + value + 32 per header (RFC 9113 §6.5.2).
  - `i32 hpack_codificare(HpackTabula* t, constans HpackCaput* capita, i32 numerus, i8* exitus, i32 capacitas);` - policy C2; returns octets, 0 if no room (table unchanged then).
  - `i32 hpack_tabula_numerus(constans HpackTabula* t);` `i32 hpack_tabula_magnitudo(constans HpackTabula* t);` `b32 hpack_tabula_obtinere(constans HpackTabula* t, i32 i, chorda* titulus, chorda* valor);` (i = I newest).

- [ ] **Step 1: failing tests** - "C.2": the four single representations decode to the RFC's header and table state (C.2.1 literal with indexing, C.2.2 without, C.2.3 never indexed - `numquam` VERUM, C.2.4 indexed). "C.3/C.4/C.5/C.6": each of the three blocks decodes to the RFC's headers and table (entries in order; sizes after each block C.3/C.4: 57, 110, 164; C.5/C.6 with table 256: 222, 222, 215), and our encoder, fed the same headers in order with `_NUMQUAM` (C.3, C.5) or `_SEMPER` (C.4, C.6) and table 4096 (C.3/C.4) or 256 (C.5/C.6), produces the RFC's octets exactly, evictions included. "secreta": `authorization: bearer x` and `cookie: a=b` encode with the never-indexed prefix `0001xxxx`, leave the table unchanged, decode with `numquam` VERUM. "magnitudo": encoder after `hpack_magnitudinem_ponere(t, 0)` starts its block with `20`; decoder rejects an update above its bound (HPACK_ERROR_MAGNITUDO) and an update after the first header. "bombae" (Review Focus 5): a block whose header list exceeds `capita_maxima` -> HPACK_ERROR_NIMIA; indices 0 and beyond table -> HPACK_ERROR_INDEX; a block cut inside a string -> HPACK_ERROR_TRUNCATUM.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 3: hpack-test-case corpus

**Files:**
- Create: `probationes/fixa/hpack/` (stories copied as data, with `ORIGO.txt`: repository URL, commit, licence text), `probationes/probatio_hpack_corpus.c`

**Interfaces:**
- Consumes: T2 (`hpack_decodere`, `hpack_codificare`), the house JSON reader (`json.h`).

- [ ] **Step 0:** check the licence of github.com/http2jp/hpack-test-case and its layout (spec AUDIENDA); if the licence does not allow copying the data, record that in the ledger as a ruling and use only RFC vectors + our own round-trips (the task then shrinks to the round-trip section). Copy at most the stories of three implementations (e.g. `nghttp2`, `go-hpack`, `node-http2-hpack`) and `raw-data`.
- [ ] **Step 1: failing tests** - "decodere": every case of every copied story decodes (table carried across cases, size updates honoured as each case's `header_table_size`) to the story's headers in order. "iterum": every `raw-data` case encoded by us and decoded by a fresh decoder yields the same headers; our octets are never longer than the raw literal form.
- [ ] **Step 2:** run -> FAIL (fixtures/suite missing). **Step 3:** copy data, write suite. **Step 4:** PASS. **Step 5:** commit.

### Task 4: `http2_forma` - frame codec and validation

**Files:**
- Create: `include/http2_forma.h`, `lib/http2_forma.c`, `probationes/probatio_http2_forma.c`, `lib/http2_forma.worklog.md`

**Interfaces:**
- Produces:
  - genera `HTTP2_DATA` 0 .. `HTTP2_CONTINUATIO` 9 (`HTTP2_DATA, HTTP2_CAPITA, HTTP2_PRIORITAS, HTTP2_ABROGATIO, HTTP2_POSITIONES, HTTP2_PROMISSIO, HTTP2_PULSUS, HTTP2_DISCESSUS, HTTP2_FENESTRA, HTTP2_CONTINUATIO`); vexilla `HTTP2_FINIS_RIVI` 0x1 (also ACK), `HTTP2_FINIS_CAPITUM` 0x4, `HTTP2_IMPLETUM` 0x8, `HTTP2_CUM_PRIORITATE` 0x20.
  - error codes `Http2Error` 0x0..0xd in RFC order: `HTTP2_ERROR_NULLUS, _PROTOCOLLI, _INTERNUS, _FLUXUS, _TEMPUS, _RIVUS_CLAUSUS, _MAGNITUDO, _RECUSATUM, _ABROGATUM, _COMPRESSIO, _CONNEXIO, _MODERATIO, _SECURITAS, _HTTP_1_1`.
  - `nomen structura { i32 genus; i32 vexilla; i32 rivus; constans i8* contentum; i32 contentum_n; } Http2Forma;` (contentum = payload with padding and priority fields removed)
  - `nomen structura { b32 bona; i32 codex; b32 connexionis; } Http2Iudicium;`
  - `i32 http2_formam_legere(constans i8* datum, i32 n, i32 magnitudo_maxima, Http2Forma* forma, Http2Iudicium* iudicium);` - returns octets consumed, 0 = incomplete; a bad frame returns its length with `iudicium.bona` FALSUM.
  - `i32 http2_formam_scribere(i32 genus, i32 vexilla, i32 rivus, constans i8* contentum, i32 n, i8* exitus, i32 capacitas);`
  - `#define HTTP2_PRAEFATIO "PRI * HTTP/2.0\r\n\r\nSM\r\n\r\n"` (24 octets)
  - field helpers: `http2_positionem_legere(constans Http2Forma*, i32 k, i32* id, i32* valor)`, `http2_u32_legere(constans i8*)` and writers for SETTINGS, PING, GOAWAY, WINDOW_UPDATE, RST_STREAM payloads.

- [ ] **Step 1: failing tests** - "rotundum": every type written then read back (header fields, payload); the reserved bit is ignored on read and cleared on write. "impletum": DATA and HEADERS with padding strip it; padding length >= payload -> PROTOCOL_ERROR connection. "iudicia": PING not 8 / WINDOW_UPDATE not 4 / RST_STREAM not 4 -> FRAME_SIZE_ERROR; SETTINGS not multiple of 6 -> FRAME_SIZE_ERROR; SETTINGS ACK with payload -> FRAME_SIZE_ERROR; SETTINGS, PING, GOAWAY on a stream != 0 -> PROTOCOL_ERROR; DATA, HEADERS, RST_STREAM, CONTINUATION on stream 0 -> PROTOCOL_ERROR; frame above `magnitudo_maxima` -> FRAME_SIZE_ERROR connection; WINDOW_UPDATE increment 0 -> PROTOCOL_ERROR (stream error on a stream, connection error on 0); unknown type 0x42 skipped with `bona`. "partes": a frame fed one octet at a time returns 0 until complete.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 5: `http2` connection - client exchange, streams, errors

**Files:**
- Create: `include/http2.h`, `lib/http2.c`, `probationes/probatio_http2.c`, `lib/http2.worklog.md`

**Interfaces:**
- Consumes: T2, T4.
- Produces:
  - `nomen enumeratio { HTTP2_CLIENS = 0, HTTP2_SERVUS } Http2Munus;`
  - `nomen structura { i32 tabula_capitum; i32 fenestra_initialis; i32 forma_maxima; i32 capita_maxima; i32 continuationes_maximae; i32 octeti_capitum_maximi; } Http2Positiones;` (C4 defaults when NIHIL)
  - `nomen enumeratio { HTTP2_EVENTUS_CAPITA, HTTP2_EVENTUS_DATA, HTTP2_EVENTUS_FINIS, HTTP2_EVENTUS_ABROGATUS, HTTP2_EVENTUS_DISCESSUS, HTTP2_EVENTUS_PULSUS, HTTP2_EVENTUS_ERROR } Http2EventusGenus;`
  - `nomen structura { Http2EventusGenus genus; i32 rivus; Xar* capita; b32 informativa; chorda data; i32 codex; i32 rivus_ultimus; } Http2Eventus;` (`informativa` = 1xx headers; trailers arrive as a second CAPITA event)
  - `Http2Connexio* http2_connexionem_creare(Http2Munus munus, constans Http2Positiones* positiones, Piscina* piscina);` - client: queues the preface + SETTINGS (+ connection WINDOW_UPDATE, C4); `HTTP2_SERVUS` returns NIHIL this phase (D1).
  - `b32 http2_recipere(Http2Connexio* c, constans i8* datum, i32 n);` - FALSUM after a connection error (GOAWAY queued, ERROR event).
  - `i32 http2_mittenda(Http2Connexio* c, constans i8** datum);` `vacuum http2_mittenda_consumere(Http2Connexio* c, i32 n);`
  - `b32 http2_eventum_capere(Http2Connexio* c, Http2Eventus* ev, Piscina* piscina);`
  - `i32 http2_petere(Http2Connexio* c, constans HpackCaput* capita, i32 numerus, chorda corpus, b32 corpus_sequitur);` - capita must hold `:method`, `:scheme`, `:authority`, `:path` first; returns the stream id (C3), 0 if refused (connection dead / after GOAWAY).
  - `b32 http2_rivum_abrogare(Http2Connexio* c, i32 rivus);` `b32 http2_pulsare(Http2Connexio* c, constans i8* octo);` `vacuum http2_discedere(Http2Connexio* c, i32 codex);`
  - `b32 http2_viva(constans Http2Connexio* c);`

- [ ] **Step 1: failing tests** (each conversation is octets fed to `http2_recipere`, written with T2/T4 as the "server"): "praefatio": first octets are the 24-octet preface then SETTINGS with ENABLE_PUSH 0; peer SETTINGS -> our ACK; our SETTINGS ACKed -> no error. "petitio": GET -> HEADERS(END_STREAM) on stream 1; response HEADERS `:status 200` + DATA "salve" END_STREAM -> CAPITA, DATA, FINIS events; POST with body -> HEADERS then DATA END_STREAM; second request uses stream 3. "informativa et appendices" (Review Focus 2): `:status 103` then `:status 200`, DATA, trailer HEADERS END_STREAM -> CAPITA(informativa), CAPITA, DATA, CAPITA(trailers), FINIS. "pulsus": peer PING -> our PING ACK with the same 8 octets; our ping -> PULSUS event on ACK. "discessus": GOAWAY last=1 while streams 1 and 3 open -> stream 3 reported DISCESSUS (retryable), new `http2_petere` returns 0. "abrogatio": peer RST_STREAM -> ABROGATUS with code; our cancel -> RST_STREAM CANCEL. "continuatio": HEADERS without END_HEADERS + CONTINUATION reassembled; more than VIII CONTINUATIONs or above the octet cap -> connection error; "continuatio interrupta" (Review Focus 3): a DATA frame between HEADERS and CONTINUATION -> PROTOCOL_ERROR GOAWAY. "promissio": PUSH_PROMISE -> PROTOCOL_ERROR (we sent ENABLE_PUSH 0). "compressio": a bad HPACK block -> COMPRESSION_ERROR GOAWAY. "servus": `http2_connexionem_creare(HTTP2_SERVUS, …)` == NIHIL.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 6: `http2` flow control and concurrency

**Files:**
- Modify: `lib/http2.c`, `probationes/probatio_http2.c`, `lib/http2.worklog.md`

**Interfaces:**
- Consumes/extends T5 (no new public functions; `Http2Positiones.fenestra_initialis` honoured).

- [ ] **Step 1: failing tests** - "fenestra mittendi": peer initial window 100, POST body 250 -> 100 octets of DATA, nothing more until WINDOW_UPDATE(stream 1, 150) and connection window allows, then the rest with END_STREAM; connection window exhausted stalls all streams. "fenestra recipiendi": a 3 MiB response in 16 KiB frames never stalls (our WINDOW_UPDATEs emitted per C4) and arrives whole; a peer exceeding our window -> FLOW_CONTROL_ERROR. "fenestra mutata" (Review Focus 1): peer SETTINGS INITIAL_WINDOW_SIZE lowered below what a stream already used -> that stream's window negative, sending pauses, resumes after updates; raised above 2^31-1 by an update -> FLOW_CONTROL_ERROR. "concurrentia": peer MAX_CONCURRENT_STREAMS 2, five requests -> streams 1, 3 sent, 5/7/9 wait and go out in order as 1 and 3 finish.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement. **Step 4:** PASS. **Step 5:** commit.

### Task 7: ALPN in `tls.h` (TLS 1.3 own + BearSSL)

**Files:**
- Modify: `include/tls.h`, `lib/tls.c`, `include/tls13.h`, `include/tls13_nuntii.h`, `lib/tls13_nuntii.c`, `lib/tls13.c`, `lib/tls_tls13.c`, `lib/tls_bearssl.c` (+ its header if the accessor lives there), `probationes/servus_tls.h` (ALPN flag pass-through only if needed), worklogs
- Create: `probationes/probatio_tls_alpn.c`

**Interfaces:**
- Produces: C7 (`TlsOptiones.protocolla`, `.protocolla_numerus`, `chorda tls_protocollum(TlsConnexio*)`); `Tls13Optiones` and `Tls13SalutatioParametri` gain the same list; `b32 tls13_extensiones_legere(constans i8* corpus, i32 n, constans i8** protocollum, i32* protocollum_n);` (selected ALPN, n 0 if absent); `chorda tls13_machina_protocollum(Tls13Machina*);`.

- [ ] **Step 1: failing tests** - "salutatio": ClientHello with `{"h2","http/1.1"}` contains extension 0x0010 with `0x000c 02 "h2" 08 "http/1.1"`; without a list the ClientHello is byte-identical to today (compare with the existing RFC 8448 fixed hello path). "extensiones": EncryptedExtensions with ALPN "h2" -> selected "h2"; with a protocol we did not offer -> handshake error, we send alert illegal_parameter (RFC 7301 §3.2). "tls13 vivum" (`servum_tls_incipere_cum(…, "-tls1_3 -alpn h2,http/1.1")`): `tls_protocollum` == "h2"; server with `-alpn http/1.1` -> "http/1.1"; server without -alpn -> "". "bearssl vivum" (`-tls1_2 -alpn h2`, RHUBARB_TLS=bearssl and the descensus path): "h2". Suite exits 2 without openssl.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement (BearSSL: `br_ssl_engine_set_protocol_names` before reset, `br_ssl_engine_get_selected_protocol` after the handshake). **Step 4:** PASS; existing `tls13_*`, `http_vocatio`, `communio` suites green. **Step 5:** commit.

### Task 8: `http2_nexus` on the reactor + the Go oracle + recordings

**Files:**
- Create: `include/http2_nexus.h`, `lib/http2_nexus.c`, `tools/oracula/http2_servus.go` (C6), `tools/http2_tabulare.sh` (recording session), `probationes/probatio_http2_nexus.c`, `probationes/fixa/http2/` (recordings + GENERATUM), `lib/http2_nexus.worklog.md`

**Interfaces:**
- Consumes: T5/T6 connection, T7 ALPN, `tcp`, `nomina`, `reactor`.
- Produces:
  - `nomen structura { b32 successus; i32 status; Xar* capita; chorda corpus; chorda causa; b32 retentabilis; } Http2Responsum;`
  - `nomen vacuum (*Http2NexusFinis)(vacuum* ctx, constans Http2Responsum* r);` (exactly once per request, never inside `http2_nexus_petere`)
  - `nomen vacuum (*Http2NexusStatus)(vacuum* ctx, b32 vivus, chorda causa);` (once when ready, once when lost/GOAWAY)
  - `nomen structura { i32 mora_connexionis_ms; i32 mora_otii_ms; constans character* tls_ancora_via; constans NominaAddressus* addressus; i32 addressus_numerus; constans character* tabula_via; } Http2NexusOptiones;` (`tabula_via` = C5 tap, tests only)
  - `Http2Nexus* http2_nexum_aperire(Reactor* reactor, constans character* hospes, i32 portus, constans Http2NexusOptiones* optiones, Http2NexusStatus status, vacuum* ctx, Piscina* piscina);` - ALPN `{"h2"}` required; anything else -> status(FALSUM, "hospes h2 non elegit").
  - `b32 http2_nexus_petere(Http2Nexus* x, constans character* methodus, constans character* via, constans HpackCaput* capita, i32 numerus, chorda corpus, Http2NexusFinis finis, vacuum* ctx, Piscina* piscina);`
  - `vacuum http2_nexum_claudere(Http2Nexus* x);` (GOAWAY NO_ERROR, pending requests finish with failure)

- [ ] **Step 1: failing tests** ("vivum" sections exit 2 without `go` or openssl): "salve": GET /salve -> 200 "salve". "eco": POST 100 KiB -> echoed. "multae": 20 requests at once on one connection, all answered, one TCP connection (Go log shows one). "magnum": /magnum?n=4194304 arrives whole. "abire": server GOAWAY -> status(FALSUM) once, a request in flight above last-stream-id finishes with `retentabilis`. "abruptio" (Review Focus 4): Go process killed mid-request -> each pending request finishes once with failure, status(FALSUM) once. "non h2": openssl s_server with `-alpn http/1.1` -> status(FALSUM, …h2…). "tempus": /tarde?ms=5000 with idle 1000 -> failure, causa names the timeout. "rete" (opt-in like `probatio_http_vocatio_rete`, env `RHUBARB_RETE=1`): GET https://nghttp2.org/ and https://www.google.com/ -> 2xx/3xx over h2.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement the nexus (state machine NOMINA -> CONNEXIO -> PACTIO -> VIVUS -> CLAUSUS modelled on `http_vocatio`), the Go server, the tap. **Step 4:** PASS; then `tools/http2_tabulare.sh` records cases `salve`, `eco`, `multae`, `magnum`, `abire` into `probationes/fixa/http2/` with GENERATUM. **Step 5:** commit.

### Task 9: replay suite (no Go needed)

**Files:**
- Create: `probationes/probatio_http2_reproductio.c`

**Interfaces:**
- Consumes: T4, T5, T8 recordings.

- [ ] **Step 1: failing tests** - for each recorded case: (a) every server frame decodes with `http2_formam_legere` and re-encodes byte for byte; (b) the frame sequence (type, flags, length, stream) matches the frames the Go log says it wrote, in order; (c) feeding `<casus>.servus.bin` to a fresh client connection (after issuing the same requests) produces the same response events as the live run (status, body length and digest, retryable flags for `abire`); (d) our own `<casus>.cliens.bin` starts with the preface and SETTINGS of C4. The suite needs no `go`; `tools/http2_tabulare.sh -conferre` re-records into a temp dir and compares byte for byte apart from the Go-chosen random fields it documents.
- [ ] **Step 2:** run -> FAIL. **Step 3:** write the suite. **Step 4:** PASS. **Step 5:** commit.

### Task 10: `tessera_es256` - one ES256 token core

**Files:**
- Create: `include/tessera_es256.h`, `lib/tessera_es256.c`, `probationes/probatio_tessera_es256.c`
- Modify: `lib/nundinae_apple.c` (calls the core), `lib/nundinae_apple.worklog.md`

**Interfaces:**
- Produces: `chorda tessera_es256_facere(constans i8* clavis_der, i32 mensura, chorda caput_json, chorda asserta_json, Piscina* piscina);` - `base64url(caput).base64url(asserta).base64url(r||s)`, empty chorda on a bad key (not P-256 / not EC / malformed); the key context is zeroed before return. `chorda tessera_apns_facere(constans i8* clavis_der, i32 mensura, chorda clavis_id, chorda turma, s64 nunc_s, Piscina* piscina);` - header `{"alg":"ES256","kid":<id>}`, claims `{"iss":<turma>,"iat":<nunc>}` (ids checked with the same safe-identifier rule as today).

- [ ] **Step 1: failing tests** - with a throwaway P-256 key generated in the test fixture (`probationes/fixa/tessera_es256/proba.der`, made by `openssl genpkey -algorithm EC -pkeyopt ec_paramgen_curve:P-256 -outform DER`, test-only and labelled so): the token has three base64url parts, header and claims decode to the exact JSON, the signature verifies with BearSSL's `br_ecdsa_i31_vrfy_raw` against the public key; an RSA key and garbage -> empty. `nundinae_apple`'s existing token test passes unchanged (byte-identical output for the same inputs).
- [ ] **Step 2:** run -> FAIL. **Step 3:** move the signing core; `nundinae_apple_tesseram_facere` builds its JSON and calls it. **Step 4:** PASS (`./compile_tests.sh tessera_es256`, `./compile_tests.sh nundinae_apple`). **Step 5:** commit.

### Task 11: `apns` tool, preflight and the finish line (with Fran)

**Files:**
- Create: `tools/apns.c`, `probationes/probatio_apns.c` (config parsing, request shape against a local Go route, preflight messages - no Apple)
- Modify: `tools/nundinae_apple.c` + `lib/nundinae_apple.c` (`-capacitas`, C9), `lib/nundinae_apple.worklog.md`, `project-specs/http2-spec.md` (STATUS line with the answers seen)

**Interfaces:**
- Consumes: T8 nexus, T10 token, C8 config, C9 capability verb.
- Produces: `bin/apns -praeparare` (preflight: config present, key file present and 0600 by `stat`, key id and team set, capability PUSH_NOTIFICATIONS on via C9 - prints what is missing, never the key); `bin/apns -probare [-signum <hex64>]` (one connection to the configured host, `POST /3/device/<token>` with `apns-topic`, `apns-push-type: alert`, `authorization: bearer <jwt>`, body `{"aps":{"alert":"probatio"}}`; prints `:status` and the JSON `reason`); exit 0 when the answer passes the spec III.4 rule (BadDeviceToken, or the fallback with a random well-formed token), 1 otherwise with the reason.

- [ ] **Step 1: failing tests** - "configuratio": missing file / missing field / key file mode 0644 -> exit 2 with the field named and no key path content echoed beyond the path; "forma petitionis": against the Go server route `/3/device/` (added in this task) the request arrives with the four headers, `authorization` starting `bearer ` and the JWT header decoding to `{"alg":"ES256","kid":"PROBA"}`; the decoded HPACK block marks `authorization` never-indexed (Go log); "regula": `reason` BadDeviceToken -> pass; InvalidProviderToken / ExpiredProviderToken / MissingProviderToken / TopicDisallowed / BadTopic -> fail with the reason; other 4xx with zero token -> retry once with a random token.
- [ ] **Step 2:** run -> FAIL. **Step 3:** implement. **Step 4:** PASS.
- [ ] **Step 5 (Fran):** Fran creates the APNs key (D7) and gives the key id and team id; Claude writes `~/.rhubarb/ios/apns.stml` (no secret in it), runs `bin/apns -praeparare` until clean (capability via C9 only after Fran's go), then `bin/apns -probare` against the sandbox -> expect 400 BadDeviceToken; then once with a wrong key id -> expect a provider-token rejection. Record both answers in the spec STATUS line. **Step 6:** commit.

### Task 12: final review and close

- [ ] **Step 1:** whole-branch review (fable) per executing-plans with this plan's Review Focus; fixes RED -> GREEN; minors to the ledger.
- [ ] **Step 2:** spec STATUS -> IMPLETUM with commits and rulings pointer; tabularium (decision + deferred minors), memory (`worktree-quarta`, a new `http2-project` memory), worklogs.
- [ ] **Step 3:** merge main into quarta if it moved, `fusio_probare`, and ff main only on Fran's word; delete this plan's workspace.
