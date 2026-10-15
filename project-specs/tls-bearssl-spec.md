# tls bearssl - BearSSL's TLS 1.2 engine behind tls.h, SecureTransport retired (spec)

*Fran 2026-10-15, after the in-process HTTP client (route B,
http-vocatio) shipped and the offline TLS test server existed: "should
we look at step 0 first?" - step 0 of the TLS staging (ledger wish
…BWQS8H): replace the deprecated SecureTransport with BearSSL's TLS 1.2
client engine, as the ground floor for our own TLS 1.3 later.*

Goal: **the same `tls.h` API runs on BearSSL**, with SecureTransport
kept as a selectable fallback for a transition. Nothing above `tls.h`
changes (http, http_vocatio, vates, vicus). The security floor stays
exactly where it is today: TLS 1.2 only, ECDHE + AEAD suites only, real
certificate validation with system trust.

Decisions (Fran, 2026-10-15):
- **Trust comes from the platform** (option a): BearSSL does the
  handshake; a small platform trust layer (`fiducia`) decides whether a
  chain is trusted for a host. macOS: `SecTrustEvaluateWithError` with
  an SSL policy carrying the host - keychain roots, configuration
  profiles, Apple distrust decisions, revocation all honoured; no root
  store to maintain. Later platforms add their own `fiducia_*` file
  (Windows: CertGetCertificateChain + CertVerifyCertificateChainPolicy
  SSL; Linux: br_x509_minimal with the distro bundle).
- **Both backends, chosen at runtime, during a transition** (option a):
  BearSSL default, SecureTransport via `TlsOptiones.vectura` or
  `RHUBARB_TLS=securetransport`; the whole TLS suite runs against both
  (a difference is a finding); deleting SecureTransport is a separate
  later step.
- **Vendor the whole upstream tree** at a pinned commit under
  `vendor/bearssl/` (byte-identical, MIT), fetched once into a
  reference clone `../bearssl` from `https://www.bearssl.org/git/BearSSL`.

## I. What exists (measured 2026-10-15, quarta 3de6d2c9)

- **`include/tls.h`** (219 lines): blocking `tls_connectere`,
  `tls_connectere_cum_optionibus` (`:85-98`), stepped `tls_incipere` /
  `tls_gradus` / `tls_gradus_error` (`:122-139`, http-vocatio T4),
  `tls_mittere` / `tls_mittere_omnia` / `tls_recipere` / `tls_claudere`
  (`:153-191`), `tls_est_valida`, `tls_certificatum_obtinere`
  (`:209`), `tls_error_descriptio`. `TLS_ITERUM` (-2, `:106`).
  `TlsOptiones` = timeout_ms, verificare_certificatum, ancora_via
  (anchor DER, 91af1146).
- **`lib/tls_macos.m`** (1132 lines): the only implementation.
  SecureTransport with I/O callbacks over our TCP; context setup shared
  by both paths (`_contextum_parare`); TLS 1.2 minimum + an ALLOW-list
  of six suites (`_cifra_admissa`, `:207-216`: ECDHE-ECDSA/RSA with
  AES-128/256-GCM or ChaCha20-Poly1305); anchor trust via
  `_ancoram_legere` + `_fiduciam_iudicare` (SecTrust with an explicit
  SSL policy, anchors only); gradatim mode (would-block is not
  timeout; libcurl's SSLWrite flush quirk); R5 context-release fix.
- **Users of tls.h:** `lib/http.c` (blocking, http_exsequi*),
  `lib/http_vocatio.c` (stepped), `lib/tls_macos.m` itself; tests
  `probatio_tls` (network: httpbin), `probatio_tls_gradus` (3 offline
  sections), `probatio_tls_servus` (7 offline sections against
  `openssl s_server`, certs generated at test time,
  `probationes/fixa/tls/generare.sh`), `probatio_http_vocatio_rete`
  (network: api.anthropic.com 401); `tools/rete_fumus.c` (network smoke:
  `-tls` howsmyssl, `-badssl` matrix, `-anthropic`);
  `tools/tls_effluvia.sh` (`leaks --atExit`, 4f30f5a8).
  `lib/silex.c:2404,2524` know tls pulls in Security.framework (still
  true: fiducia_macos.m needs it).
- **Vendoring precedent:** `vendor/sqlite3.c` - one amalgamated file;
  declared to aedilis as `<vendor><fons via="vendor/sqlite3.c">` with
  its own flags (`aedilis.stml:82-92`); compile_tests.sh compiles
  `VENDOR_SOURCES` relaxed (`:40-52`, `:202-208`); house tooling skips
  `vendor/` (`tools/compile_tests_fontes_generare.sh:72-73`). Its header
  is included BEFORE latina.h (`lib/scrinium.c:3-6`): our Roman-numeral
  macros collide with vendor parameter names.
- **BearSSL** is not on this machine. Upstream facts (to verify after
  fetching, AUDIENDA): ~300 `.c` files in `src/`, headers in `inc/`;
  last tagged release 0.6 (2018), later fixes on master; the TLS engine
  is a pure state machine (four buffers: sendrec, recvrec, sendapp,
  recvapp; `br_ssl_engine_current_state`), TLS 1.0-1.2 only; custom
  X.509 validators via `br_x509_class`; `br_x509_decoder` extracts a
  certificate's public key; constant-time implementations; system
  entropy seeder.

## II. Design

### II.1 Vendoring

`vendor/bearssl/` = upstream `inc/`, `src/`, `LICENSE`, byte-identical
at a pinned master commit chosen from the upstream log, plus
`VENDOR.md`: URL, commit, commit date, licence, compile flags, and a
SHA-256 over the sorted list of (path, content hash). A check (gate or
test) recomputes it, so an accidental edit inside `vendor/bearssl/` is
caught. Update = replace the directory, update VENDOR.md.

### II.2 Build

aedilis gains a DIRECTORY form of vendored source, e.g. `<vendor>
<bibliotheca via="vendor/bearssl/src" capita="vendor/bearssl/inc">`
with relaxed flags: every `.c` compiled once into cached objects,
linked into any binary whose closure includes a BearSSL header.
Fallback if the directory form fights aedilis's design: a generated
`<fons>` list. compile_tests.sh: the same through VENDOR_SOURCES (or its
directory equivalent). The plan reads aedilis's vendor code and picks.

### II.3 The C89 boundary

House code compiles `-std=c89 -pedantic -Werror`; BearSSL's public
headers are C99-flavoured (stdint types; possibly `static inline`
helpers - `inline` is not a C89 keyword, a hard error). Mitigations, in
order, chosen on the first look at the fetched headers: (1)
`-isystem vendor/bearssl/inc` (warnings from them suppressed); (2)
include BEFORE latina.h (sqlite rule); (3) `inline` -> `__inline__`
around the include if needed; (4) if still not clean - or if examen
cannot parse the vendor headers in `tls_bearssl.c`'s closure - a TINY
adapter compiled with vendor flags exposes a C89-clean API, and house
code never includes BearSSL headers.

### II.4 The trust layer: `fiducia`

`include/fiducia.h` + `lib/fiducia_macos.m`:

```c
/* catena: certificata DER, folium primum; hospes: nomen (SNI/SAN);
 * ancora_via: NIHIL = repositorium systematis, aliter SOLA ancora */
b32 fiducia_iudicare(constans FiduciaCertificatum* catena, i32 numerus,
    constans character* hospes, constans character* ancora_via,
    chorda* causa, Piscina* piscina);
/* folii summarium et validitas (tls_certificatum_obtinere) */
b32 fiducia_folium_describere(constans FiduciaCertificatum* folium,
    TlsCertificatum* exitus, Piscina* piscina);
```

macOS: SecCertificate from each DER, SecTrust with
`SecPolicyCreateSSL(true, host)`, anchors-only when an anchor is given,
`SecTrustEvaluateWithError`; the named reason from the CFError. The
SecureTransport backend's `_ancoram_legere` / `_fiduciam_iudicare`
move here and both backends use it.

### II.5 The dispatcher

`lib/tls.c` implements the public `tls.h`; `TlsConnexio` records its
backend. `TlsOptiones.vectura`: `TLS_VECTURA_ORDINARIA` (= BearSSL),
`TLS_VECTURA_BEARSSL`, `TLS_VECTURA_SECURETRANSPORT`; ORDINARIA reads
`RHUBARB_TLS` once (`securetransport` -> fallback). Backends:
`lib/tls_bearssl.c`, `lib/tls_macos.m` (renamed internally, behaviour
unchanged), each behind an internal header.

### II.6 The BearSSL backend: `lib/tls_bearssl.c`

Configuration (today's floor, exactly): `br_ssl_client_zero` + explicit
versions TLS 1.2..1.2; the SAME six suites; default constant-time EC
(X25519, P-256/384), AES-GCM, ChaCha20-Poly1305, SHA-256/384; SNI and
host via `br_ssl_client_reset(cc, host, 0)`; system entropy (failure =
refuse); one bidirectional buffer (`BR_SSL_BUFSIZE_BIDI`) per
connection in its piscina.

The pump: `_pump` drives the engine by `br_ssl_engine_current_state` -
SENDREC: write records (EAGAIN -> wait writable); RECVREC: read from
the socket (EAGAIN -> wait readable); SENDAPP/RECVAPP: application data;
CLOSED: `br_ssl_engine_last_error` (0 = clean close).
- `tls_gradus`: pump until SENDAPP opens (PERFECTUS), or it must wait
  (LEGERE/SCRIBERE), or CLOSED with error (ERROR).
- `tls_recipere`: pump; RECVAPP data copied and acknowledged; otherwise
  TLS_ITERUM, 0 on clean close (drain-until-ITERUM is natural: decrypted
  data stays in the engine until read).
- `tls_mittere`: copy into SENDAPP, flush, pump records; returns bytes
  accepted, TLS_ITERUM if none.
- blocking `tls_connectere*`: the same pump with `poll()` on the socket
  under `timeout_ms`.
- `tls_claudere`: `br_ssl_engine_close`, one best-effort pump
  (close_notify), close the socket; nothing to release beyond the
  piscina (the R5 leak class cannot exist).

The validator (`br_x509_class`): start_chain/start_cert/append/end_cert
collect each certificate's DER; end_chain calls `fiducia_iudicare` and
returns 0 or `BR_ERR_X509_NOT_TRUSTED` (keeping fiducia's reason);
get_pkey returns the leaf key decoded with `br_x509_decoder`.
`verificare_certificatum = FALSUM` keeps today's meaning (key decoded,
trust skipped).

Errors (same categories as today; tests assert categories): rejected
certificate -> `TLS_ERROR_CERTIFICATUM` / `TLS_GRADUS_ERROR` with
fiducia's reason; handshake failure -> BearSSL error code named
(`BR_ERR_BAD_VERSION`, peer alert N, ...); I/O -> `TLS_ERROR_IO`.

## III. Tests

- **Both backends, every TLS test:** `probatio_tls_gradus`,
  `probatio_tls_servus`, the network files, and the vates/vicus suites
  run each TLS scenario for BOTH `vectura` values and assert identical
  outcomes (13 offline sections x 2).
- **BearSSL-only:** the validator class against chains served by
  s_server (leaf key decoded; untrusted chain rejected with fiducia's
  reason); pump edges (EAGAIN in every state; peer close mid-handshake;
  close_notify on claudere).
- **Vendor integrity:** VENDOR.md hash recomputed.
- **Known-answer smoke** of the vendored library (e.g. SHA-256 of a
  fixed input) - proves it links and runs before anything uses it.
- `tools/tls_effluvia.sh` for both backends; `tools/rete_fumus.c`
  `-badssl` matrix against BearSSL (network, by hand).
- Plants per task via silva.planta; Fran's live `$question` on BearSSL.

Task order (the plan refines it): vendor + build + C89 boundary + KAT
-> fiducia (anchor code moves) -> dispatcher with SecureTransport
behind it (all current tests green) -> tls_bearssl stepped -> blocking
+ certificate details -> both-backend matrix, leaks, BearSSL default,
live check.

## IV. Not in this spec

- TLS 1.3 (BearSSL's engine stops at 1.2 - our own client is step 3 of
  …BWQS8H); session resumption; client certificates; ALPN (not set
  today either).
- Deleting SecureTransport (separate step, after a period of both).
- Windows / Linux `fiducia` files.
- Monocypher (no TLS role in step 0).

## AUDIENDA (not verified)

- BearSSL upstream facts (file count, master commits after 0.6, the
  exact header constructs) - verified on the first fetch, before any
  design choice depends on them.
- Whether BearSSL headers compile under `-std=c89 -pedantic` via
  `-isystem`, and whether examen parses them (II.3 decides between
  mitigations 1-3 and the adapter).
- Whether aedilis's vendor model takes a directory form cleanly (II.2).
- That `br_x509_decoder` yields the key BearSSL's engine expects from
  `get_pkey` for both EC and RSA leaves (the s_server fixtures are EC;
  an RSA case needs an RSA fixture).
- BearSSL's AES-GCM on Apple Silicon has no ARMv8 crypto path (constant-
  time bitsliced code): throughput for large bodies not measured; API
  traffic is small.
- That a SecTrust evaluation fed DER from BearSSL equals what
  SecureTransport did internally (both use SecTrust; the policy is made
  explicit in both).
