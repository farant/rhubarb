# tls13 - our own TLS 1.3 client behind tls.h, BearSSL 1.2 as checked fallback (spec)

*Fran 2026-10-15/16, right after BearSSL became the default (main
f423bcca): "my main concern with 1.3 is if we would end up in a
situation where 1.2 client is deprecated or can't function properly" -
step after the BearSSL arc on the TLS staging wish (…BWQS8H). Chosen
before the hospitium HTTP gaps.*

Goal: **a TLS 1.3 client of our own** (RFC 8446, client side), built
only on BearSSL's vendored primitives, as a third backend behind the
unchanged `tls.h`. A server that speaks only 1.2 is reached through the
BearSSL backend with RFC 8446's downgrade sentinel checked by us. Trust
stays with the platform (`fiducia`). At the end of the arc, 1.3 is the
house default.

Decisions (Fran, 2026-10-16, in conversation):
- **1.2 servers: try 1.3, then reconnect through BearSSL 1.2 and check
  the downgrade sentinel ourselves.** Fall back ONLY when the server
  answers as a 1.2 server, never on network or certificate failures.
- **The reconnect is inside the TLS layer and transparent:** the same
  `TcpConnexio`, a new socket `dup2`ed onto the old fd number, so
  callers' reactor registrations (poll() keyed by fd) stay valid.
  Nothing above `tls.h` changes.
- **Approach A: a socket-free engine** (BearSSL-style buffers, no
  sockets, no reactor, no clock; injectable randomness) plus a thin
  backend that pumps it. That is what lets RFC 8448's traces be
  replayed byte for byte.

## I. What exists (main f423bcca, 2026-10-16)

- `include/tls.h` + `lib/tls.c`: dispatcher. `TlsOptiones.vectura`
  (ORDINARIA / BEARSSL / SECURETRANSPORT), `RHUBARB_TLS` read through
  `lectiones_ambitus` (declared `<ambitus>` on probationes_radicis),
  `tls_vecturam_eligere`, `tls_vectura`. Stepped (`tls_incipere`,
  `tls_gradus`) and blocking (`tls_connectere*`) contracts; drain until
  `TLS_ITERUM`; EOF after the handshake = clean end.
- `lib/tls_bearssl.c`: the BearSSL backend - one pump (`_antlia`) for
  both modes, validator `Iudex` (br_x509_class) handing the DER chain to
  `fiducia_iudicare`, TLS 1.2 only, six suites. The pattern this spec's
  backend copies.
- `include/fiducia.h` + `lib/fiducia_macos.m`: SecTrust judgement of a
  DER chain for a host (anchor only, or the system store) and the leaf
  summary.
- `vendor/bearssl_amalgama.{c,h}` (BearSSL 7bea48e). Primitives this
  spec needs, verified present 2026-10-16: HMAC and HKDF (br_hkdf -
  but its API is one Expand stream per context, so HKDF-Expand-Label is
  ours, on br_hmac), SHA-256/384, X25519 and P-256 (`br_ec_get_default`),
  AES-GCM (br_gcm with constant-time CTR + GHASH, x86/POWER8
  accelerated vtables when present), ChaCha20 + Poly1305
  (`br_chacha20_ct_run`, `br_poly1305_ctmul(q)_run`), ECDSA verify
  (`br_ecdsa_vrfy_asn1_get_default`), RSA-PSS verify
  (`br_rsa_pss_vrfy_get_default`), the X.509 decoder, the system
  seeder. `br_ssl_engine_context.server_random[32]` is a public field
  (needed for the sentinel). BearSSL's 1.2 client does not check the
  1.3 sentinel itself.
- `include/tcp.h`: `tcp_incipere` / `tcp_incipere_perficere`
  (non-blocking connect), `tcp_obtinere_fd`, `tcp_mittere` /
  `tcp_recipere`. `lib/reactor.c` is poll()-based, keyed by fd number.
- Test harness: `probationes/servus_tls.h` (`servum_tls_incipere_cum`
  with extra s_server flags), certificates generated at test time
  (`probationes/fixa/tls/generare.sh`, EC + RSA leaves); the
  two-backend matrix in probatio_tls_servus/_gradus; tools/tls_effluvia.sh;
  tools/rete_fumus.c (-tls, -badssl).

## II. Design

### II.1 Units

| Unit | Files | Job |
|---|---|---|
| Key schedule | `include/tls13_clavis.h`, `lib/tls13_clavis.c` | HKDF-Extract, HKDF-Expand-Label, Derive-Secret on br_hmac; transcript hash (SHA-256/384 by suite, running, with the HRR "message_hash" substitution); early -> handshake -> master secrets; traffic secret -> key/IV; Finished `verify_data`; KeyUpdate's next-generation secret. Pure functions. |
| Record layer | `include/tls13_recordum.h`, `lib/tls13_recordum.c` | Seal/open TLSCiphertext: nonce = IV XOR 64-bit sequence; TLSInnerPlaintext = content + type + zero padding (strip on open, reject all-zero); AES-128/256-GCM or ChaCha20-Poly1305; sequence per direction; limits 2^14 plaintext, 2^14 + 256 ciphertext. |
| Message codecs | `include/tls13_nuntii.h`, `lib/tls13_nuntii.c` | One bounds-checked byte cursor. Encode ClientHello (legacy_version 0x0303, random, 32-byte legacy_session_id, suites, null compression; extensions server_name, supported_versions {0x0304}, supported_groups {x25519, secp256r1}, key_share {x25519}, signature_algorithms). Parse ServerHello / HelloRetryRequest (HRR random constant; supported_versions; key_share / HRR group; detect "answered as 1.2"; read the downgrade sentinel), EncryptedExtensions, Certificate (list of DER, extensions skipped), CertificateVerify, Finished, NewSessionTicket, KeyUpdate. |
| Engine | `include/tls13.h`, `lib/tls13.c` | The client state machine `Tls13Machina` over the three units; verifies CertificateVerify (context string "TLS 1.3, server CertificateVerify", ECDSA P-256/P-384 or RSA-PSS rsae/pss, key from the leaf via br_x509_decoder); trust by CALLBACK; sends alerts; never a socket, the platform or the clock. |
| Backend | `include/tls_tls13.h` (internal), `lib/tls_tls13.c` | Pumps engine <-> `TcpConnexio` (the `tls_bearssl.c` pattern); trust callback = `fiducia_iudicare` with the bound anchor; certificate details = `fiducia_folium_describere` on the kept leaf. |

`tls.c` gains `TLS_VECTURA_TLS13` (`RHUBARB_TLS=tls13`) and owns the
fallback (II.3). `tcp` gains `tcp_redintegrare` (II.3).

### II.2 Engine API and data flow

```c
nomen structura Tls13Machina Tls13Machina;

/* trust: shape of fiducia_iudicare minus the anchor (bound in datum) */
nomen b32 (*Tls13Fiducia)(vacuum* datum, constans FiduciaCertificatum* catena,
    i32 numerus, constans character* hospes, chorda* causa, Piscina* piscina);
/* randomness: NIHIL = HMAC-DRBG seeded by br_prng_seeder_system;
 * tests inject RFC 8448's client random and X25519 private key */
nomen vacuum (*Tls13Fors)(vacuum* datum, i8* exitus, i32 mensura);

nomen structura {
    constans character* hospes;      /* SNI + name for the trust check */
                   b32  verificare;  /* FALSUM: trust callback skipped */
          Tls13Fiducia  fiducia;   vacuum* fiducia_datum;
             Tls13Fors  fors;      vacuum* fors_datum;
} Tls13Optiones;

Tls13Machina* tls13_machina_creare (constans Tls13Optiones*, Piscina*);
i32  tls13_machina_status (Tls13Machina*);   /* TLS13_RECORDA_MITTENDA |
       TLS13_RECORDA_EXSPECTAT | TLS13_DATA_MITTI_POSSUNT |
       TLS13_DATA_ADSUNT | TLS13_CLAUSA */
i8*  tls13_recorda_exitus      (Tls13Machina*, i32* n);  vacuum tls13_recorda_exitus_ack      (Tls13Machina*, i32);
i8*  tls13_recorda_introitus   (Tls13Machina*, i32* n);  vacuum tls13_recorda_introitus_ack   (Tls13Machina*, i32);
i8*  tls13_data_exitus         (Tls13Machina*, i32* n);  vacuum tls13_data_exitus_ack         (Tls13Machina*, i32);
vacuum tls13_data_effundere    (Tls13Machina*);           /* flush */
i8*  tls13_data_introitus      (Tls13Machina*, i32* n);  vacuum tls13_data_introitus_ack      (Tls13Machina*, i32);
vacuum tls13_machina_claudere  (Tls13Machina*);           /* close_notify */
Tls13Error tls13_machina_error (Tls13Machina*);           /* {codex, causa} */
i32  tls13_machina_versio      (Tls13Machina*);           /* 0x0304 once agreed */
constans FiduciaCertificatum* tls13_machina_catena (Tls13Machina*, i32* numerus);
```

Flow:
1. Create: the engine builds the ClientHello and queues it (state
   `RECORDA_MITTENDA`).
2. Handshake:
   - ServerHello -> handshake keys, or HelloRetryRequest -> new
     ClientHello with the requested group (once; a second HRR is
     `PARAMETRUM`). The dummy ChangeCipherSpec is sent before the
     second flight (compatibility mode, RFC 8446 D.4); received CCS
     records are dropped while the handshake runs.
   - Then it decrypts EncryptedExtensions, Certificate, CertificateVerify
     and Finished.
   - It calls the trust callback (unless `verificare` is FALSUM),
     verifies CertificateVerify against the leaf key and the server
     Finished against the transcript, and sends its own Finished.
   - The data flags come up.
3. Data: written bytes are sealed into records on flush. Incoming
   application data is decrypted into the read buffer and stays there
   until read (the drain-until-`TLS_ITERUM` contract).
4. After the handshake: NewSessionTicket is parsed and dropped (no
   resumption). KeyUpdate rotates the receive keys and, when
   `update_requested`, sends our own KeyUpdate and rotates send keys.
   close_notify -> `CLAUSA`; any other alert -> error.

Memory: everything lives in the connection piscina. Buffers are fixed:
one maximum record per direction (2^14 + 256 + 5) plus data buffers. No
allocation after creation except the chain DER kept for
`tls_certificatum_obtinere`.

### II.3 Fallback to 1.2, the sentinel, the dispatcher

- **Trigger:** only `TLS13_ERROR_SOLUM_1_2`. That is a ServerHello whose
  version is 0x0303 without supported_versions, or a `protocol_version`
  alert in reply to our ClientHello.
- **`tls.c` swaps backends** on the trigger, on the same `TlsConnexio`:
  1. drop the 1.3 engine;
  2. `tcp_redintegrare(tcp)`;
  3. start the BearSSL backend with a "downgrade check" flag;
  4. set `vectura = BEARSSL`.
- **New tcp primitive:**
  ```c
  /* same peer (getpeername), new non-blocking socket, connect started,
   * then dup2 onto the OLD fd number (callers' reactor registrations,
   * poll() keyed by fd, stay valid). Completion as for tcp_incipere:
   * wait writable, then tcp_incipere_perficere. */
  TcpResultus tcp_redintegrare (TcpConnexio* connexio, Piscina* piscina);
  ```
  - Stepped: `tls_gradus` returns `TLS_GRADUS_SCRIBERE` while the
    connect runs, then the BearSSL handshake steps.
  - Blocking: the same primitive, then BearSSL's blocking handshake,
    under the same `timeout_ms`.
- **Sentinel:** with the flag set, the BearSSL backend checks after its
  handshake whether the last 8 bytes of `server_random` are
  `44 4F 57 4E 47 52 44 01` ("DOWNGRD\1") or `... 00`. A match ->
  `TLS_ERROR_HANDSHAKE` "descensus vetitus: hospes TLS 1.3 novit". (A
  server that knows 1.3 would have accepted the first attempt; the
  sentinel on the fallback means the downgrade was forced.)
- **tls.h additions:**
  - `TLS_VECTURA_TLS13`.
  - `TlsOptiones.sine_descensu` (default FALSUM: "1.3 or nothing" ->
    with a 1.2 server, `HANDSHAKE` "hospes solum TLS 1.2").
  - `i32 tls_versio(TlsConnexio*)` (0x0304 / 0x0303). `tls_vectura`
    reports the backend actually carrying the connection (BEARSSL after
    a fallback).

### II.4 Errors

The engine reports `Tls13Error {codex, causa}`. The backend maps each
code to the existing `TlsError` categories. RFC 8446's alerts are sent
before closing and flushed best-effort.

| Code | When | Alert | TlsError / caller |
|---|---|---|---|
| `SOLUM_1_2` | answered as a 1.2 server | - | fallback; with `sine_descensu`: HANDSHAKE "hospes solum TLS 1.2" |
| `FIDUCIA` | trust callback refused | bad_certificate 42 | CERTIFICATUM + fiducia's reason |
| `SUBSCRIPTIO` | CertificateVerify does not verify | decrypt_error 51 | CERTIFICATUM "subscriptio CertificateVerify falsa" |
| `FINIS` | server Finished mismatch | decrypt_error 51 | HANDSHAKE |
| `FORMA` | malformed, truncated, length out of bounds | decode_error 50 | HANDSHAKE, message + field named |
| `PARAMETRUM` | not offered / not acceptable (group, suite, version, second HRR, missing key_share) | illegal_parameter 47 | HANDSHAKE |
| `MAC` | AEAD authentication fails | bad_record_mac 20 | HANDSHAKE; after the handshake `tls_recipere` -1 |
| `MOLES` | record too large | record_overflow 22 | as MAC |
| `ALERTA_RECEPTA` | server sent a fatal alert | - | HANDSHAKE "nuntius fatalis a pare: N (name)"; 40, 42, 47, 50, 70, 71, 80, 112, 116 named |
| `ENTROPIA` | system seeder failed | - | refused at creation (HANDSHAKE) |

Unchanged from the other backends:
- TCP EOF without close_notify after the handshake = clean end (0);
  during the handshake it is a named error.
- A blocking timeout = TLS_ERROR_TCP "Tempus handshake excessum".
- Tests assert the category and the key phrase, never whole messages.

### II.5 Security floor

- TLS 1.3: the three suites TLS_AES_128_GCM_SHA256,
  TLS_AES_256_GCM_SHA384, TLS_CHACHA20_POLY1305_SHA256.
- Groups: x25519 (key_share sent), secp256r1 (HRR only).
- CertificateVerify algorithms: ecdsa_secp256r1_sha256,
  ecdsa_secp384r1_sha384, rsa_pss_rsae_sha256/384/512,
  rsa_pss_pss_sha256/384/512.
- No 0-RTT, no PSK, no client certificates, no session tickets used,
  no ALPN, no record_size_limit.
- Fallback: BearSSL's existing 1.2 floor (six ECDHE + AEAD suites) plus
  the sentinel.

## III. Tests

1. **RFC 8448** (Example Handshake Traces for TLS 1.3):
   - The RFC text is committed unchanged in `probationes/fixa/tls13/`
     (IETF terms allow unchanged copies; fetched once from
     rfc-editor.org).
   - A generator parses it into fixtures (secrets, messages, records) -
     no hand-copied hex.
   - Units 1-3 check every published value. The engine replays §3
     (simple 1-RTT: X25519, AES-128-GCM-SHA256, RSA certificate -> RSA-PSS
     verify) and the HRR trace, with the RFC's randomness and keys
     injected and a stub trust callback. Every record must match byte
     for byte.
2. **RFC 5869** HKDF test vectors.
3. **Truncation sweep:** every server message cut at every byte ->
   `FORMA` (or `MAC` for records), never a crash (`CREDO_NON_RUIT`).
   Tampered tag, wrong sequence and oversized records each get a named
   error.
4. **Engine-level KeyUpdate and NewSessionTicket:** records sealed
   under keys the test knows.
5. **Offline against openssl s_server** (servus_tls.h, certificates
   generated at test time):
   - `-tls1_3` with each suite forced by `-ciphersuites`;
   - `-groups P-256` (forces HRR);
   - EC and RSA leaves;
   - every rejection case (alienus, expiratus, ignotus, sine ancora)
     under 1.3;
   - `-tls1_2`: fallback happens (`tls_versio` 0x0303); `sine_descensu`
     gives the named error.
   - Sentinel: a 1.3-capable server reached through the fallback path,
     forced by a test hook, must be refused "descensus vetitus".
6. **`tcp_redintegrare` alone:** same fd number, reconnected peer,
   reactor registration still delivering events.
7. **Matrix:** every tls_servus / tls_gradus section under tls13, bearssl
   and securetransport (RHUBARB_TLS); tools/tls_effluvia.sh then covers
   all three.
8. **Plants** per task via silva.planta (one that compiles; red, revert,
   green).
9. **By hand:** rete_fumus -tls (howsmyssl reports TLS 1.3) and -badssl;
   Fran's live `$question` in vicus.

Task order (the plan refines it):
1. key schedule + RFC 8448 fixture generator;
2. record layer;
3. codecs + truncation sweep;
4. engine 1-RTT + CertificateVerify (RFC 8448 §3 replay);
5. engine HRR, alerts, errors, KeyUpdate/NST;
6. backend + `TLS_VECTURA_TLS13`, stepped and blocking, certificate
   details, s_server -tls1_3;
7. `tcp_redintegrare`, fallback, sentinel, `sine_descensu`, `tls_versio`;
8. three-backend matrix, leaks, default -> 1.3 with fallback, network
   checks, live check.

## IV. Not in this spec

- 0-RTT, PSK resumption (NewSessionTicket dropped), client certificates,
  ALPN, record_size_limit, Ed25519 / Ed448 CertificateVerify (Monocypher
  could add Ed25519 later).
- A TLS 1.2 handshake of our own (BearSSL keeps 1.2).
- Server-side TLS (hospitium) - a separate arc.
- Deleting SecureTransport (park …GP4S).

## AUDIENDA (not verified)

- RFC 8448's section numbering and contents: §3 simple 1-RTT; the HRR
  trace (§5?); whether a compatibility-mode trace exists (§7?) - checked
  on fetching, before the fixture generator is written.
- That OpenSSL's s_server (3.3.2) sets the downgrade sentinel when it
  supports 1.3 but negotiates 1.2 with a 1.2-max client (RFC 8446 says
  MUST; verify empirically - the sentinel test depends on it).
- That `br_rsa_pss_vrfy` handles both rsa_pss_rsae (rsaEncryption key)
  and rsa_pss_pss (RSASSA-PSS key) as needed, and what the X.509 decoder
  does with an RSASSA-PSS-keyed certificate (likely unsupported - then
  rsa_pss_pss is offered only if decodable, else dropped from the list).
- `-ciphersuites` / `-groups` behaviour of the local s_server build for
  each forced case.
- The `dup2`-onto-the-old-fd behaviour under poll() on macOS when the
  old socket had pending events in the same pass (expected harmless:
  the reactor rebuilds its poll set each pass) - test 6 pins it.
