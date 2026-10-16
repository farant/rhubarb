# tls13 plan - own TLS 1.3 client behind tls.h, BearSSL 1.2 as checked fallback

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> subagents. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `tls.h` gains a third backend - a TLS 1.3 client of our own on
BearSSL's primitives - that becomes the house default, falling back to
the BearSSL 1.2 backend (with the RFC 8446 downgrade sentinel checked by
us) only for servers that answer as 1.2 servers.

**Architecture:** five units - key schedule, record layer, message
codecs, a socket-free engine (BearSSL-style buffers, injectable
randomness, trust by callback), and a thin backend pumping the engine
over `TcpConnexio` exactly as `lib/tls_bearssl.c` pumps BearSSL. The
dispatcher `lib/tls.c` owns the 1.3 -> 1.2 fallback; a new
`tcp_redintegrare` reconnects onto the SAME fd number (dup2), so callers'
reactor registrations stay valid.

**Tech stack:** C89 + latina.h; BearSSL 7bea48e primitives
(`vendor/bearssl_amalgama.h`): br_hmac, br_sha256/384 vtables,
br_ec_get_default (X25519, P-256), br_gcm + br_aes_ct64_ctr +
br_ghash_ctmul64, br_poly1305_ctmul_run + br_chacha20_ct_run,
br_ecdsa_vrfy_asn1_get_default, br_rsa_pss_vrfy_get_default,
br_x509_decoder, br_hmac_drbg + br_prng_seeder_system; fiducia (SecTrust);
openssl s_server 3.3.2 as the offline oracle; RFC 8448 + RFC 5869 as
known-answer oracles; credo v2.

**Spec:** `project-specs/tls13-spec.md` (quarta b2567819). Read it with
this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh`; identifiers,
  comments, messages Latin; never a latina.h macro as identifier
  (`nomen`, `per`, `casus`, `duplex`, `magnitudo`, every Roman numeral
  ...); `i32`/`i64` UNSIGNED, `s32`/`s64` signed; chorda not
  NUL-terminated; new words in identifiers pass the Latin lint (proper
  nouns / abbreviations -> `ignotum-permissum`, real English words ->
  rename).
- Every file that includes the BearSSL header writes
  `#include "../vendor/bearssl_amalgama.h"` AFTER `postulata_posix.h`
  (if any) and BEFORE `latina.h` (tls-bearssl R13, R30).
- No raw `fopen`/`open`/`stat`/`access` outside filum (examen code 96):
  `filum_legere_totum`, `filum_existit`, `filum_scribere`.
- The engine (`tls13.c`) never touches a socket, the platform, the
  clock, or a global: everything is in its `Tls13Machina`, allocated in
  the caller's piscina.
- Security floor exactly as spec II.5: suites 0x1301 0x1302 0x1303;
  groups x25519 (key_share) and secp256r1 (HRR only); CertificateVerify
  ecdsa_secp256r1_sha256 (0x0403), ecdsa_secp384r1_sha384 (0x0503),
  rsa_pss_rsae_sha256/384/512 (0x0804/0x0805/0x0806); no 0-RTT, PSK,
  client certificates, ALPN, record_size_limit.
- Nothing above `tls.h` changes; the stepped and blocking contracts of
  `tls.h` hold for the new backend (drain until TLS_ITERUM; EOF after
  the handshake = 0; blocking timeout = TLS_ERROR_TCP "Tempus handshake
  excessum").
- Offline tests use `probationes/servus_tls.h` and test-time
  certificates; network tests carry `/* <aedilis facultas="rete"/> */`.
- Commits via detached `silva.commissio(nuntius, viae)` with the worktree
  guard; formator `-scribere` + `-vitia` and the Latin lint before;
  plants via `silva.planta` (compile pre-check; red, revert, green); a
  mute plant is a test gap, fixed before commit; NO installs from the
  worktree; main untouched until Fran's go.

## Rulings made while planning

- **P1 - RFC 8448 is read directly by the tests.** A parser in
  `probationes/rfc8448.h` loads the unmodified RFC text
  (`probationes/fixa/tls13/rfc8448.txt`, sha256 6564d137...fba, fetched
  2026-10-16) and returns any field of any action; it checks every
  declared "(N octets)" count against what it collected. No generator,
  no GENERATUM fixtures, no `generata` entries.
- **P2 - the engine has a test seam for the ClientHello.** RFC 8448's
  ClientHellos carry extensions we do not send (record_size_limit,
  session_ticket, psk modes, six groups); the transcript hash covers the
  ClientHello, so a byte-exact replay needs THE RFC's ClientHello.
  `Tls13Optiones.salutationes_fixae` (up to two: CH1, and CH2 after a
  HelloRetryRequest) replaces the encoder's output; the private keys
  come through `fors`. Our encoder is tested separately (structure
  round trip through our own parser, and interop with s_server).
- **P3 - dummy ChangeCipherSpec only when our legacy_session_id is
  non-empty** (RFC 8446 D.4). Our encoder always sends 32 random bytes
  (compatibility mode on); RFC 8448 §3/§5 send none and replay without
  CCS, §7 sends one and replays with it.
- **P4 - `rsa_pss_pss_*` is not offered.** BearSSL's X.509 decoder knows
  only rsaEncryption and id-ecPublicKey keys
  (`vendor/bearssl/src/x509/x509_decoder.t0:204,224`); an RSASSA-PSS-keyed
  certificate could not be decoded anyway.
- **P5 - the sentinel is real in our oracle.** Verified 2026-10-16:
  OpenSSL 3.3.2 `s_server` (1.3-capable) answers a `-tls1_2` client with
  a ServerHello random ending `44 4f 57 4e 47 52 44 01`. The sentinel
  test = an ordinary s_server + a client forced into the fallback path
  by a test hook (`TlsOptiones.descensus_coactus`, tests only).
- **P6 - X25519 scalars are byte-reversed for BearSSL.** BearSSL's EC
  API takes multipliers big-endian (`bearssl_ec.h`; upstream test_crypto
  `revbytes`); RFC 7748 / TLS key shares are little-endian. One helper
  `_x25519_ad_bearssl` reverses; the u-coordinate stays as is (learned in
  tls-bearssl T1). P-256 scalars are big-endian on both sides.
- **P7 - HKDF-Expand-Label is ours, on br_hmac** (BearSSL's br_hkdf is
  one Expand stream per context; the key schedule expands one PRK with
  many labels).
- **P8 - record layer uses constant-time AES (br_aes_ct64_ctr) +
  br_ghash_ctmul64 and br_chacha20_ct_run** on every platform (the
  target is Apple Silicon; x86 AES-NI paths can come later behind the
  same API).
- **P9 - `tcp_redintegrare(conn, optiones, piscina)`:** `optiones NIHIL`
  = non-blocking start (stepped path: wait writable, then
  `tcp_incipere_perficere`); non-NIHIL = timed connect, then blocking
  with `optiones->timeout_ms` socket timeouts (blocking path). Both end
  with `dup2(nova, conn->fd)`; `conn->clausa = FALSUM`.
- **P10 - plan location `project-specs/tls13-plan.md`** (house).

## Rulings made while executing

- **R1 (T1)** - the RFC reader copies each line into a NUL-terminated
  local before `strstr`/`atoi`, and builds field names from that copy
  (`linea + VI`; `s + VI` dropped const under -Wcast-qual).
- **R2 (T1)** - plant `k += II` -> `k += III` is an equivalent mutation
  (hex pairs are single-space separated); replaced by stopping the
  continuation loop 16 octets early -> red through the declared-count
  check.
- **R3 (T1)** - Latin lint: `ignotum-permissum` for tls13, rfc, rfc8448,
  rfc5869, hkdf, prk, ikm, okm, sha384, hrr; house word `traffici`
  (lemma trafficus); renamed `kc` -> `clavis_hmac`, `hc` ->
  `status_hmac`, `th` -> `hash_transcripti`, `zeros` -> `nulli`,
  `verify_data` -> `verificatio`. Later tasks' snippets that say
  `zeros`/`th` use the new names.
- **R4 (T1)** - the transcript keeps no copy of ClientHello1
  (`primum`): `tls13_transcriptum_hrr` works from the digest.
- **R5 (T1)** - in a worktree, `~/.bin/scribe` judges against MAIN's
  root ("via extra repositorium", exit 4); use `./silva/scribe.sh`.

- **R6 (T2)** - `tls13_aperire` copies the received tag into a local
  before `_aead` (no const cast-away; `br_gcm_check_tag` takes a const
  tag anyway).
- **R7 (T2)** - Latin lint: `nonce` is English -> `vector_recordi`
  (`_nonce` -> `_vectorem_formare`); `ignotum-permissum` gcm, chacha,
  chacha20, poly1305, aes256, ccs, aead, aad; house words `alerta`
  (spec's own TLS13_ERROR_ALERTA_RECEPTA) and `cifrare`.
- **R8 (T2)** - plants must compile under -Werror: "content type ->
  ZEPHYRUM" became `genus_contenti & ZEPHYRUM` (unused parameter),
  "ChaCha tag check off" became `|| n > ZEPHYRUM` (self-compare
  warning). Two extra plants beyond the plan: ChaCha tag check, MOLES
  limit - all five red, then green.

- **R9 (T3)** - RFC 8448 §5's HelloRetryRequest already carries a
  cookie (114 octets) and its second ClientHello echoes it: section
  `hrr_crustulum` uses the REAL exchange - our CH2's cookie extension
  equals the RFC CH2's byte for byte - instead of a hand-spliced HRR.
- **R10 (T3)** - `Tls13Cursor` is not public: the cursor (`Cursor`) is
  private to lib/tls13_nuntii.c; no consumer needs it.
- **R11 (T3)** - the truncation sweep as planned was BLIND to
  over-reads: every reader ends with an exact-consumption check, so a
  truncated message is rejected even with the bounds check broken, and
  the over-read lands in the rest of the full message (valid memory).
  The bounds plant passed `CREDO_SECTIO=praecisio`. Fix: the crash pass
  copies each prefix flush against an UNMAPPED region (mmap 2 x 64 KiB,
  munmap the upper half) -> an over-read is SIGSEGV -> CREDO_NON_RUIT
  red (4 sweeps red under the plant). The verdict pass stays on the
  full message. All seven message kinds swept.
- **R12 (T3)** - Latin lint: `ignotum-permissum` keyupdate, ee,
  serverhello, clienthello, nst, secp256, ip; renamed `_sh12` ->
  `_responsum_vetus`, `ku` -> `renovatio`, `truncatio` (real Late
  Latin: no glossary) -> `praecisio`. Sections beyond the plan: `ip`
  (no server_name for IP literals), `keyupdate`. Plants: versio_1_2,
  bounds (see R11), cookie branch, IP test, vector length writer - all
  red, then green.

- **R13 (T4)** - write keys switch to c hs AT ServerHello: alerts
  raised while reading the server flight are sealed under c hs
  (sequence 0); the dummy CCS is emitted lazily before our FIRST sealed
  record (Finished flight, or such an alert). Tests open the alert with
  the RFC's c hs secret.
- **R14 (T4)** - extra section `data_coniuncta` (NST + two data records
  in one feed: the second is not opened until the first is read);
  plant 3 (`dum` -> `si`) does not compile (`frange` inside `si`) ->
  `frange` after one message instead; extra plants: Finished compare,
  trust-callback guard, unread-data stall. All six red, then green.
- **R15 (T4)** - Latin lint: `ack` (spec's API, abbreviation) ->
  `ignotum-permissum`; house word `entropia`; form `alertam` added to
  `alerta`; renamed `hc` -> `status_hash`, `hn` -> `mensura_digesti`,
  `ALERTA_DECRYPTIO` -> `ALERTA_VERIFICATIO`, test `rec` ->
  `recordum_servi`, `_alimentare` -> `_ingerere`,
  `_sectio_s7_compatibilitas` -> `_sectio_s7_ccs` (section title stays
  `s7_compatibilitas`). `silva/renominare.sh` refuses untracked files
  (git is its undo) - word-boundary edits on the new files instead.
- **R16 (T4)** - beyond the plan's text: key-change boundary check (RFC
  8446 5.1), all-zero X25519 secret rejected (7.4.2), warning
  user_canceled ignored, unexpected messages/records -> PARAMETRUM with
  alert 10. Deferred to T5 with their tests: alert NAMES in the causa
  (T4 gives the number), HRR, KeyUpdate. CertificateRequest ->
  unexpected (no client certificates, spec II.5).

- **R17 (T5 scope, Fran 2026-10-16)** - CertificateRequest answered
  WITHOUT a client identity (RFC 8446 4.4.2: empty Certificate, no
  CertificateVerify). Engine: CR parsed in stadium CERT (context <= 255,
  extensions well-formed, transcript add), `cr_visa` + context kept;
  after the server Finished our flight = (CCS) + Certificate `0b 00 00
  04 || ctx_len ctx || 00 00 00` + Finished, both under c hs, the
  transcript for OUR Finished including our Certificate (c ap / s ap
  stay on the transcript through the SERVER Finished). Tests in T5:
  - `s6_sine_identitate`: RFC 8448 §6 replay (its CH fixed, its x25519
    key) - server flight EE || CR || Certificate || CV || Finished
    accepted (real signature, real Finished); our records opened with
    §6's c hs secret = empty Certificate + a Finished the TEST computes
    with T1 (transcript from the RFC's messages + our Certificate);
    then `tls13_machina_claudere` output = §6 {client} "send alert
    record" `complete record` BYTE FOR BYTE (c ap does not depend on
    the client's certificate).
  - `cr_forma`: CR with a truncated extension vector -> FORMA, alert 50.
  - T6: s_server `-verify 1` (optional) handshake succeeds; `-Verify 1`
    (required) -> ALERTA_RECEPTA naming certificate_required (116).
  Plants: our Certificate left out of the transcript -> s6 red; CR
  context not echoed (always empty) -> a cr test with a non-empty
  context red. Real client identities (signing callback, key in
  Keychain / Secure Enclave / card, never in the engine) = ledger
  wish, not this arc.

- **R18 (T5)** - corrects R17: RFC 8446 4.3.2 - in the handshake the
  CertificateRequest context SHALL be empty -> a non-empty one is
  PARAMETRUM / illegal_parameter (47), not echoed; section `cr_contextus`
  inserts one byte into §6's CR and re-seals (the check precedes the
  signature). Found while building it: §6's SERVER is ECDSA P-256 (CV
  0x0403) - §2's RSA key is §6's CLIENT key - so §6 is also our ECDSA
  verification oracle, and a re-signed §6 flight is impossible.
- **R19 (T5)** - second HelloRetryRequest -> alert 10 unexpected_message
  (RFC 8446 4.1.4), not 47; HRR groups other than secp256r1 (0x0018, and
  0x001d already sent) -> 47. KeyUpdate: one octet of another value ->
  47, other lengths -> 50. CCS before CH2 when the session id is
  non-empty: untestable on RFC 8448 (empty ids), owed to T6 `-groups
  P-256`.
- **R20 (T5)** - recordum_moles, recordum_mac, alerta_70 and
  hrr_grex_ignotus were already green at RED (T4 behaviour) - kept as
  pins. Plants (8, all red then green): HRR once-guard, KeyUpdate send
  rotation, T1's 0xfe -> 0xff (red via s5_hrr: R2's mute-by-design plant
  closed), our Certificate out of the transcript, CR context check,
  P-256 x offset, `tls13_transcriptum_hrr` call, alert name. Lint
  renames `xn`/`xo` -> `longitudo_x`/`locus_x`, `KEY_SHARE` ->
  `EXTENSIO_CLAVIUM`; examen rejected a writable static buffer in a
  test section (made local).

- **R21 (T6)** - `s_server -WWW` sends close_notify (verified with
  `s_client -msg`), so the plan's `eof` section would not have tested EOF
  without close_notify. servus_tls.h gains `servum_tls_incipere_modo(...,
  modus, ...)` (`-WWW` stays the default through `_cum`); section `eof`
  uses `-rev` and KILLS the server after the echo -> FIN, no alert ->
  read returns 0. Sections beyond the plan: `cr_optionalis` (`-verify
  1`) and `cr_necessaria` (`-Verify 1`: handshake completes on our side,
  then the first read is -1 with "116 (certificate_required)" in
  `tls_gradus_error`) per R17; `hrr` also exercises R19's CCS before
  CH2 (our session id is 32 octets). `omissa` uses the default (1.2+1.3)
  s_server so the plain `servum_tls_incipere` stays used.
- **R22 (T6)** - EOF rule keyed on `tls13_machina_versio != 0` (engine
  state) instead of a backend `pacta` flag; `tls_tls13_gradus_error`
  also carries post-handshake read errors. Plants (5, red then green):
  trust always VERUM, EOF rule inverted, FIDUCIA dropped from the
  CERTIFICATUM mapping, dispatcher "tls13" misspelt, pump not sending
  first (run with CREDO_SECTIO=gradus - it deadlocks every section).
  Lint: `aes128` -> ignotum-permissum. Owed by hand (MANU):
  apps/forum/fumus.sh, tools/icones_fumus.sh - for Fran.

- **R23 (T7)** - our ClientHello also offers BearSSL's six 1.2 suites
  (c02b c02f c02c c030 cca9 cca8) after 0x1301-3. Without them a server
  from before TLS 1.3 (ignores supported_versions) finds no common suite
  and answers handshake_failure (40) - NOT a fallback trigger (spec
  II.3) - so it would be unreachable. With them it sends a 1.2
  ServerHello -> SOLUM_1_2 -> fallback. (OpenSSL 3 limited to 1.2
  answers protocol_version 70 either way - checked with s_client
  -tls1_3.) The 1.3 engine still rejects any 1.3 ServerHello choosing
  them. T3's exact cipher vector updated; section `servus_vetus` (a fake
  pre-1.3 server answering with a canned 1.2 ServerHello -> a second
  accept carrying BearSSL's ClientHello) pins the behaviour.
- **R24 (T7)** - `tcp_redintegrare` blocking path connects UNTIMED, like
  `tcp_connectere_cum_optionibus` (P9 said "timed connect"; the house
  blocking connect has no connect timeout), then applies the options
  (socket timeouts). Spec AUDIENDA "dup2 keeps the reactor
  registration": TRUE for our poll() reactor, pinned by
  probatio_tcp_redintegrare `gradatim`; would be false under kqueue
  (noted in lib/tcp_posix.worklog.md).
- **R25 (T7)** - `descensus_coactus` skips the 1.3 attempt entirely and
  starts BearSSL-with-sentinel on the ORIGINAL connection (no redial:
  an empty first connection would only exercise s_server's error path).
  `tls_bearssl_incipere_signo` takes `b32 obstruens` (the blocking twin
  folded in; blocking completion shared with `tls_bearssl_connectere`
  as `_pacisci_obstruens`). Stepped redial completion: tls.c polls
  POLLOUT with timeout 0 before `tcp_incipere_perficere` (SO_ERROR
  means nothing before writability). Sections beyond the plan:
  `versio_13`, `servus_vetus`; the stepped AND blocking variants of
  sine_descensu / signum. Plants (5, red then green): sentinel compare
  off, dup2 replaced by use-in-place (fd changes), fallback on any
  error, sine_descensu ignored (stepped), blocking fallback branch off.
  Lint: `_redintegrationem_perficere` (real Latin unknown to the oracle)
  -> `_connexionem_novam_perficere`, `DOWNGRD` -> `SIGNUM_DESCENSUS`,
  test `vo` -> `optiones_vocationis`; house word `scribibilis`.

## Review focus

Inputs no task's tests reach that are most likely to bite, each with
the expected behaviour:

1. **Handshake messages split across records, or several in one
   record** (servers do both): the engine reassembles by the 4-byte
   handshake header, whatever the record boundaries.
2. **A HelloRetryRequest carrying a cookie** (some CDNs): the second
   ClientHello echoes the cookie extension byte for byte.
3. **Peer FIN without close_notify after the handshake:** `tls_recipere`
   returns 0, like the other two backends; during the handshake a named
   error, never a hang.
4. **Servers sending NewSessionTicket(s) right after their Finished,
   interleaved with the first application data:** tickets dropped,
   data delivered in order.
5. **Large responses (>= 256 KB, many records per socket read):** every
   byte delivered, drain-until-ITERUM honoured.

Pinned by tests: 1 -> T5 step 1 (the §3 server flight replayed split
byte-by-byte AND as one buffer); 2 -> T3 (HRR with cookie parsed, CH2
echoes it) and T5; 3 -> T6 "clausa a pare" + EOF section; 4 -> T4 (RFC
8448 §3 sends a NewSessionTicket after the handshake); 5 -> T6/T8
(tls_servus "magnum" under tls13).

---

### Task 1: key schedule `tls13_clavis` + the RFC 8448 reader

**Files:**
- Create: `probationes/fixa/tls13/rfc8448.txt` (the RFC, unmodified),
  `probationes/rfc8448.h`, `include/tls13_clavis.h`,
  `lib/tls13_clavis.c`, `probationes/probatio_tls13_clavis.c`,
  `lib/tls13_clavis.worklog.md`
- Modify: `oratio/glossarium.stml` (`tls13` ignotum-permissum if the
  lint flags it), `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Produces:

```c
#define TLS13_SHA256  I
#define TLS13_SHA384  II
#define TLS13_HASH_MAXIMUS  XLVIII

i32  tls13_hash_mensura (i32 genus);                 /* 32 / 48, 0 ignotum */
vacuum tls13_hkdf_extrahere (i32 genus, constans i8* sal, i32 sal_n,
    constans i8* ikm, i32 ikm_n, i8* prk);           /* sal NIHIL = zeros */
b32  tls13_hkdf_expandere (i32 genus, constans i8* prk, constans i8* info,
    i32 info_n, i8* exitus, i32 mensura);            /* RFC 5869 */
b32  tls13_expandere_titulo (i32 genus, constans i8* secretum,
    constans character* titulus, constans i8* contextus, i32 ctx_n,
    i8* exitus, i32 mensura);                        /* "tls13 " + titulus */
vacuum tls13_secretum_derivare (i32 genus, constans i8* secretum,
    constans character* titulus, constans i8* hash_transcripti, i8* exitus);
vacuum tls13_claves_traffici (i32 genus, constans i8* secretum,
    i32 clavis_n, i8* clavis, i8* iv);               /* iv XII octeti */
vacuum tls13_finis_calculare (i32 genus, constans i8* secretum_base,
    constans i8* hash_transcripti, i8* verify_data);
vacuum tls13_secretum_proximum (i32 genus, constans i8* secretum,
    i8* exitus);                                     /* "traffic upd" */

nomen structura Tls13Transcriptum Tls13Transcriptum;  /* opacum */
Tls13Transcriptum* tls13_transcriptum_creare (i32 genus, Piscina* p);
vacuum tls13_transcriptum_addere (Tls13Transcriptum* t, constans i8* o, i32 n);
vacuum tls13_transcriptum_digerere (constans Tls13Transcriptum* t, i8* hash);
/* RFC 8446 4.4.1: post HRR, ClientHello1 -> message_hash */
vacuum tls13_transcriptum_hrr (Tls13Transcriptum* t);
```

- [ ] **Step 1: Commit the oracle text** - copy
  `$SCRATCH/rfc/rfc8448.txt` (fetched 2026-10-16 from
  https://www.rfc-editor.org/rfc/rfc8448.txt) to
  `probationes/fixa/tls13/rfc8448.txt`; check
  `shasum -a 256` = `6564d1376d1ec744fc7a9993da15ebc1b9be361908b166091f47ef605c537fba`.

- [ ] **Step 2: Write the reader `probationes/rfc8448.h`**

```c
/* rfc8448.h - lector textus RFC 8448 (Example Handshake Traces for TLS
 * 1.3) pro probationibus: textum IMMUTATUM legit
 * (probationes/fixa/tls13/rfc8448.txt) et campum quemvis actionis
 * cuiusvis reddit - nulla hex manu copiata (tls13-plan P1).
 *
 * Forma RFC: sectio '3.  Simple 1-RTT Handshake'; actio '   {server}
 * derive secret "tls13 c hs traffic":' (caput interdum in lineam
 * proximam fluit); campus '      PRK (32 octets):  1d c8 ...' cum
 * lineis continuationis; '(empty)' = octeti nulli. Interruptiones
 * paginae ('Thomson ... [Page N]', 'RFC 8448 ... January 2019',
 * \f) praetereuntur. Numerus declaratus contra collectos verificatur:
 * lectio fracta = FALSUM (lector ipse probatur).
 *
 * Functiones INTERIORES (more ludus_toy.h): includens AMBOS aditus
 * vocare debet - rfc8448_legere et rfc8448_quaerere. */
#ifndef RFC8448_H
#define RFC8448_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include <string.h>
#include <stdlib.h>

nomen structura {
          i32  sectio;    /* 3, 5, 7 ... */
    character  latus;     /* 'c' client, 's' server */
       chorda  actio;     /* caput sine '{latus}  ' et ':' finali */
       chorda  campus;    /* 'PRK', 'complete record', 'key expanded' */
       chorda  octeti;
} Rfc8448Campus;

nomen structura {
    Xar* campi;           /* Rfc8448Campus */
} Rfc8448;

interior b32
_rfc8448_linea_pagina (
    chorda l)
{
    i32 k;

    per (k = ZEPHYRUM; k < l.mensura; k++)
    {
        si (l.datum[k] == '\f')
        {
            redde VERUM;
        }
    }
    redde (l.mensura >= VIII && memcmp(l.datum, "RFC 8448", VIII)
               == ZEPHYRUM)
        || (l.mensura >= VII && memcmp(l.datum, "Thomson", VII)
               == ZEPHYRUM);
}

interior s32
_rfc8448_hex (
    character c)
{
    si (c >= '0' && c <= '9') redde (s32)(c - '0');
    si (c >= 'a' && c <= 'f') redde (s32)(c - 'a' + X);
    redde -I;
}

/* octetos hex lineae (paria 'ab' spatiis separata) ad aedificatorem */
interior b32
_rfc8448_hex_colligere (
    constans character* s,
                   i32  n,
                    i8* capsa,
                   i32* numerus,
                   i32  capacitas)
{
    i32 k = ZEPHYRUM;

    dum (k < n)
    {
        s32 a;
        s32 b;

        si (s[k] == ' ')
        {
            k++;
            perge;
        }
        si (k + I >= n)
        {
            redde FALSUM;
        }
        a = _rfc8448_hex(s[k]);
        b = _rfc8448_hex(s[k + I]);
        si (a < ZEPHYRUM || b < ZEPHYRUM || *numerus >= capacitas)
        {
            redde FALSUM;
        }
        capsa[*numerus] = (i8)((a << IV) | b);
        (*numerus)++;
        k += II;
    }
    redde VERUM;
}

interior b32
rfc8448_legere (
               Rfc8448* r,
    constans character* via,
               Piscina* p)
{
       chorda  textus = filum_legere_totum(via, p);
          i32  i = ZEPHYRUM;
          i32  sectio = ZEPHYRUM;
    character  latus = ' ';
       chorda  actio;
    character  caput[CCLVI];
          i32  caput_n = ZEPHYRUM;
    character  linea[CCLVI];   /* filum_legere_totum NUL non promittit:
                                * strstr/atoi in copia terminata */

    r->campi = xar_creare(p, (i32)magnitudo(Rfc8448Campus));
    actio.datum = NIHIL;
    actio.mensura = ZEPHYRUM;
    si (textus.mensura == ZEPHYRUM || r->campi == NIHIL)
    {
        redde FALSUM;
    }
    dum (i < textus.mensura)
    {
           chorda  l;
              i32  j = i;
        constans character* s;

        dum (j < textus.mensura && textus.datum[j] != '\n')
        {
            j++;
        }
        l.datum   = textus.datum + i;
        l.mensura = j - i;
        i = j + I;
        si (l.mensura == ZEPHYRUM || l.mensura >= CCLVI
            || _rfc8448_linea_pagina(l))
        {
            perge;
        }
        memcpy(linea, l.datum, l.mensura);
        linea[l.mensura] = '\0';
        s = linea;
        /* sectio: '3.  Simple 1-RTT Handshake' */
        si (s[ZEPHYRUM] >= '1' && s[ZEPHYRUM] <= '9')
        {
            sectio = (i32)atoi(s);
            perge;
        }
        /* caput actionis */
        si (l.mensura > XI && memcmp(s, "   {", IV) == ZEPHYRUM)
        {
            latus   = (s[IV] == 'c') ? 'c' : 's';
            caput_n = ZEPHYRUM;
            j = IV;
            dum (j < l.mensura && s[j] != '}')
            {
                j++;
            }
            j += III;   /* '}' + duo spatia */
            dum (j < l.mensura && caput_n + I < CCLVI)
            {
                caput[caput_n++] = s[j++];
            }
            actio = chorda_ex_buffer((i8*)caput, caput_n);
            actio = chorda_transcribere(actio, p);
            perge;
        }
        /* campus: sex spatia + titulus + ' (N octets):' */
        si (l.mensura > VI && memcmp(s, "      ", VI) == ZEPHYRUM
            && s[VI] != ' ')
        {
            constans character* par = strstr(s, " octets):");
            constans character* aperta;
                         i32  numerus = ZEPHYRUM;
                         i32  declaratus;
                          i8* capsa;
                Rfc8448Campus* c;

            si (par == NIHIL || par > s + l.mensura)
            {
                /* caput in lineam secundam fluens ('secret)') aut
                 * 'salt:  0 (all zero octets)' - non campus octetorum */
                perge;
            }
            aperta = par;
            dum (aperta > s && *aperta != '(')
            {
                aperta--;
            }
            declaratus = (i32)atoi(aperta + I);
            capsa = (i8*)piscina_allocare(p, (i64)(declaratus + I));
            si (capsa == NIHIL)
            {
                redde FALSUM;
            }
            par += IX;   /* ' octets):' */
            si (strncmp(par, "  (empty)", IX) != ZEPHYRUM
                && !_rfc8448_hex_colligere(par, (i32)(s + l.mensura - par),
                       capsa, &numerus, declaratus))
            {
                redde FALSUM;
            }
            /* continuationes: novem+ spatia, solum hex */
            dum (numerus < declaratus && i < textus.mensura)
            {
                chorda m;

                j = i;
                dum (j < textus.mensura && textus.datum[j] != '\n')
                {
                    j++;
                }
                m.datum   = textus.datum + i;
                m.mensura = j - i;
                i = j + I;
                si (m.mensura == ZEPHYRUM || _rfc8448_linea_pagina(m))
                {
                    perge;
                }
                si (!_rfc8448_hex_colligere((constans character*)m.datum,
                        m.mensura, capsa, &numerus, declaratus))
                {
                    redde FALSUM;
                }
            }
            si (numerus != declaratus)
            {
                redde FALSUM;
            }
            c = (Rfc8448Campus*)xar_addere(r->campi);
            c->sectio  = sectio;
            c->latus   = latus;
            c->actio   = actio;
            c->campus  = chorda_transcribere(chorda_ex_buffer(
                (i8*)(s + VI), (i32)(aperta - I - (s + VI))), p);
            c->octeti  = chorda_ex_buffer(capsa, numerus);
        }
    }
    redde xar_numerus(r->campi) > ZEPHYRUM;
}

/* campus n-tus (ordo ab 0) sectionis/lateris cuius actio cum 'actio'
 * INCIPIT et campus 'campus' AEQUAT; NIHIL si abest */
interior constans Rfc8448Campus*
rfc8448_quaerere (
       constans Rfc8448* r,
                    i32  sectio,
              character  latus,
    constans character* actio,
                    i32  ordo,
    constans character* campus)
{
    i32 k;
    i32 n = ZEPHYRUM;
    i32 la = (i32)strlen(actio);
    i32 lc = (i32)strlen(campus);

    per (k = ZEPHYRUM; k < xar_numerus(r->campi); k++)
    {
        constans Rfc8448Campus* c = (constans Rfc8448Campus*)
            xar_obtinere(r->campi, k);

        si (   c->sectio == sectio && c->latus == latus
            && c->actio.mensura >= la
            && memcmp(c->actio.datum, actio, la) == ZEPHYRUM
            && c->campus.mensura == lc
            && memcmp(c->campus.datum, campus, lc) == ZEPHYRUM)
        {
            si (n == ordo)
            {
                redde c;
            }
            n++;
        }
    }
    redde NIHIL;
}

#endif /* RFC8448_H */
```

(House helpers verified 2026-10-16: `chorda_ex_buffer(i8*, i32)`,
`chorda_transcribere(chorda, Piscina*)`, `filum_legere_totum(via,
piscina)` - empty chorda on failure, `xar_creare(piscina, i32)`,
`xar_addere`, `xar_obtinere`, `xar_numerus`; credo: the suite fixture's
`c->datum` reaches every section as `c->suita`. RFC 8448 action
headers checked against the text: `awk '/^   \{/'` lists them.)

- [ ] **Step 3: Write the failing test `probationes/probatio_tls13_clavis.c`**

```c
/* probatio_tls13_clavis.c - tabula clavium TLS 1.3 (tls13-spec II.1):
 * RFC 5869 A.1 (HKDF), RFC 8448 §3 (omnes derivationes 'tls13 ...'),
 * lector RFC 8448 ipse. Oraculum: textus RFC immutatus. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "tls13_clavis.h"
#include "rfc8448.h"
#include <stdio.h>
#include <string.h>

#define VIA_RFC "probationes/fixa/tls13/rfc8448.txt"

interior vacuum
_hex (constans character* h, i8* o, i32 n)
{
    i32 k;

    per (k = ZEPHYRUM; k < n; k++)
    {
        unsigned int v = ZEPHYRUM;

        (vacuum)sscanf(h + k * II, "%2x", &v);
        o[k] = (i8)v;
    }
}

interior vacuum
_suitam_parare (CredoContextus* c)
{
    Rfc8448* r = (Rfc8448*)piscina_allocare(c->piscina, magnitudo(Rfc8448));

    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_NECESSE(rfc8448_legere(r, VIA_RFC, c->piscina));
    c->datum = r;
}

/* lector: campi noti adsunt cum mensura declarata */
interior vacuum
_sectio_lector (CredoContextus* c)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* f;

    f = rfc8448_quaerere(r, III, 'c', "construct a ClientHello", ZEPHYRUM,
        "ClientHello");
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_AEQUALIS_I32(f->octeti.mensura, CXCVI);
    f = rfc8448_quaerere(r, III, 's', "derive secret \"tls13 c hs traffic\"",
        ZEPHYRUM, "expanded");
    CREDO_NECESSE_NON_NIHIL(f);
    CREDO_AEQUALIS_I32(f->octeti.mensura, XXXII);
    CREDO_AEQUALIS_I32((i32)(unsigned char)f->octeti.datum[ZEPHYRUM], 0xb3);
    f = rfc8448_quaerere(r, V, 's', "construct a ServerHello", ZEPHYRUM,
        "ServerHello");      /* HRR */
    CREDO_NECESSE_NON_NIHIL(f);
}

/* RFC 5869 A.1 */
interior vacuum
_sectio_rfc5869 (CredoContextus* c)
{
    i8 ikm[XXII], sal[XIII], info[X], prk[XXXII], okm[XLII];
    i8 prk_e[XXXII], okm_e[XLII];

    (vacuum)c;
    memset(ikm, 0x0b, XXII);
    _hex("000102030405060708090a0b0c", sal, XIII);
    _hex("f0f1f2f3f4f5f6f7f8f9", info, X);
    _hex("077709362c2e32df0ddc3f0dc47bba6390b6c73bb50f9c3122ec844ad7c2b3e5",
        prk_e, XXXII);
    _hex("3cb25f25faacd57a90434f64d0362f2a2d2d0a90cf1a5a4c5db02d56ecc4c5bf"
         "34007208d5b887185865", okm_e, XLII);
    tls13_hkdf_extrahere(TLS13_SHA256, sal, XIII, ikm, XXII, prk);
    CREDO_VERUM(memcmp(prk, prk_e, XXXII) == ZEPHYRUM);
    CREDO_NECESSE(tls13_hkdf_expandere(TLS13_SHA256, prk, info, X, okm,
        XLII));
    CREDO_VERUM(memcmp(okm, okm_e, XLII) == ZEPHYRUM);
}

/* omnis 'derive secret ...' §3 (cliens et servus): PRK + hash -> info
 * (titulus encodatus) et expanded */
interior vacuum
_derivatio (CredoContextus* c, character latus, constans character* actio,
    constans character* titulus, i32 ordo)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, latus, actio,
        ordo, "PRK");
    constans Rfc8448Campus* hash = rfc8448_quaerere(r, III, latus, actio,
        ordo, "hash");
    constans Rfc8448Campus* exp = rfc8448_quaerere(r, III, latus, actio,
        ordo, "expanded");
    i8 o[XXXII];

    CREDO_NECESSE(prk && hash && exp);
    CREDO_NECESSE(tls13_expandere_titulo(TLS13_SHA256, prk->octeti.datum,
        titulus, hash->octeti.datum, hash->octeti.mensura, o, XXXII));
    CREDO_VERUM(memcmp(o, exp->octeti.datum, XXXII) == ZEPHYRUM);
}

interior vacuum
_sectio_derivationes (CredoContextus* c)
{
    _derivatio(c, 's', "derive secret for handshake \"tls13 derived\"",
        "derived", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 c hs traffic\"",
        "c hs traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 s hs traffic\"",
        "s hs traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret for master \"tls13 derived\"",
        "derived", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 c ap traffic\"",
        "c ap traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 s ap traffic\"",
        "s ap traffic", ZEPHYRUM);
    _derivatio(c, 's', "derive secret \"tls13 exp master\"",
        "exp master", ZEPHYRUM);
}

/* extractiones: early (sal zeros), handshake, master */
interior vacuum
_sectio_extractiones (CredoContextus* c)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* sec;
    constans Rfc8448Campus* sal;
    constans Rfc8448Campus* ikm;
    i8 o[XXXII], zeros[XXXII];

    memset(zeros, ZEPHYRUM, XXXII);
    sec = rfc8448_quaerere(r, III, 's', "extract secret \"early\"",
        ZEPHYRUM, "secret");
    CREDO_NECESSE_NON_NIHIL(sec);
    tls13_hkdf_extrahere(TLS13_SHA256, NIHIL, ZEPHYRUM, zeros, XXXII, o);
    CREDO_VERUM(memcmp(o, sec->octeti.datum, XXXII) == ZEPHYRUM);
    sal = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "salt");
    ikm = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "IKM");
    sec = rfc8448_quaerere(r, III, 's', "extract secret \"handshake\"",
        ZEPHYRUM, "secret");
    CREDO_NECESSE(sal && ikm && sec);
    tls13_hkdf_extrahere(TLS13_SHA256, sal->octeti.datum, XXXII,
        ikm->octeti.datum, XXXII, o);
    CREDO_VERUM(memcmp(o, sec->octeti.datum, XXXII) == ZEPHYRUM);
}

/* claves traffici: 'derive write traffic keys for handshake data' */
interior vacuum
_sectio_claves (CredoContextus* c)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM, "PRK");
    constans Rfc8448Campus* k = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM,
        "key expanded");
    constans Rfc8448Campus* iv = rfc8448_quaerere(r, III, 's',
        "derive write traffic keys for handshake data", ZEPHYRUM,
        "iv expanded");
    i8 clavis[XVI], vector[XII];

    CREDO_NECESSE(prk && k && iv);
    tls13_claves_traffici(TLS13_SHA256, prk->octeti.datum, XVI, clavis,
        vector);
    CREDO_VERUM(memcmp(clavis, k->octeti.datum, XVI) == ZEPHYRUM);
    CREDO_VERUM(memcmp(vector, iv->octeti.datum, XII) == ZEPHYRUM);
}

/* finished: 'calculate finished "tls13 finished"' (servus) dat PRK
 * (secretum s hs), 'hash (0 octets)' (contextus Expand vacuus),
 * 'expanded' (finished_key) et 'finished' (verify_data) - sed NON hash
 * transcripti: eum ex nuntiis CH SH EE Certificate CV RFC ipsius
 * computamus (sic transcriptum quoque per quinque nuntios probatur) */
interior vacuum
_sectio_finis (CredoContextus* c)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans character* actio = "calculate finished \"tls13 finished\"";
    constans Rfc8448Campus* prk = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "PRK");
    constans Rfc8448Campus* exp = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "expanded");
    constans Rfc8448Campus* fin = rfc8448_quaerere(r, III, 's', actio,
        ZEPHYRUM, "finished");
    constans character* nuntii[V][III] = {
        { "c", "construct a ClientHello", "ClientHello" },
        { "s", "construct a ServerHello", "ServerHello" },
        { "s", "construct an EncryptedExtensions", "EncryptedExtensions" },
        { "s", "construct a Certificate", "Certificate" },
        { "s", "construct a CertificateVerify", "CertificateVerify" } };
    Tls13Transcriptum* t = tls13_transcriptum_creare(TLS13_SHA256,
        c->piscina);
    i8  o[XXXII], th[XXXII];
    i32 k;

    CREDO_NECESSE(prk && exp && fin && t);
    /* finished_key = Expand-Label(base, "finished", "", Hash.length) */
    CREDO_NECESSE(tls13_expandere_titulo(TLS13_SHA256, prk->octeti.datum,
        "finished", NIHIL, ZEPHYRUM, o, XXXII));
    CREDO_VERUM(memcmp(o, exp->octeti.datum, XXXII) == ZEPHYRUM);
    per (k = ZEPHYRUM; k < V; k++)
    {
        constans Rfc8448Campus* n = rfc8448_quaerere(r, III,
            nuntii[k][ZEPHYRUM][ZEPHYRUM], nuntii[k][I], ZEPHYRUM,
            nuntii[k][II]);

        CREDO_NECESSE_NON_NIHIL(n);
        tls13_transcriptum_addere(t, n->octeti.datum, n->octeti.mensura);
    }
    tls13_transcriptum_digerere(t, th);
    tls13_finis_calculare(TLS13_SHA256, prk->octeti.datum, th, o);
    CREDO_VERUM(memcmp(o, fin->octeti.datum, XXXII) == ZEPHYRUM);
}

/* transcriptum: hash ClientHello + ServerHello == hash 'c hs traffic' */
interior vacuum
_sectio_transcriptum (CredoContextus* c)
{
    constans Rfc8448* r = (constans Rfc8448*)c->suita;
    constans Rfc8448Campus* ch = rfc8448_quaerere(r, III, 'c',
        "construct a ClientHello", ZEPHYRUM, "ClientHello");
    constans Rfc8448Campus* sh = rfc8448_quaerere(r, III, 's',
        "construct a ServerHello", ZEPHYRUM, "ServerHello");
    constans Rfc8448Campus* h = rfc8448_quaerere(r, III, 's',
        "derive secret \"tls13 c hs traffic\"", ZEPHYRUM, "hash");
    Tls13Transcriptum* t = tls13_transcriptum_creare(TLS13_SHA256,
        c->piscina);
    i8 o[XXXII];

    CREDO_NECESSE(ch && sh && h && t);
    tls13_transcriptum_addere(t, ch->octeti.datum, ch->octeti.mensura);
    tls13_transcriptum_addere(t, sh->octeti.datum, sh->octeti.mensura);
    tls13_transcriptum_digerere(t, o);
    CREDO_VERUM(memcmp(o, h->octeti.datum, XXXII) == ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lector", _sectio_lector, NIHIL, NIHIL, NIHIL, NIHIL },
    { "rfc5869", _sectio_rfc5869, NIHIL, NIHIL, NIHIL, NIHIL },
    { "derivationes", _sectio_derivationes, NIHIL, NIHIL, NIHIL, NIHIL },
    { "extractiones", _sectio_extractiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves", _sectio_claves, NIHIL, NIHIL, NIHIL, NIHIL },
    { "finis", _sectio_finis, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transcriptum", _sectio_transcriptum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("tls13_clavis", SECTIONES,
        _suitam_parare, NIHIL);
}
```

(`<stdio.h>` for sscanf. HRR transcript (`tls13_transcriptum_hrr`) is
proven in T5 against RFC 8448 §5.)

- [ ] **Step 4: Run - expect FAIL** (`tls13_clavis.h` missing; the
  `lector` section alone, with the include commented, must PASS first -
  run it once that way to prove the reader against the RFC before any
  crypto: `CREDO_SECTIO=lector`).

- [ ] **Step 5: Write `include/tls13_clavis.h`** (Interfaces block above,
  each declaration with a one-line Latin comment) and
  **`lib/tls13_clavis.c`:**

```c
/* tls13_clavis.c - tabula clavium TLS 1.3 (RFC 8446 §7.1): HKDF super
 * br_hmac (Expand-Label nostrum - br_hkdf unum rivum Expand per
 * contextum habet; tls13-plan P7), transcriptum currens. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "tls13_clavis.h"
#include <string.h>

structura Tls13Transcriptum {
                     i32  genus;
    br_hash_compat_context  status;
                      i8* primum;     /* ClientHello1 (pro HRR) */
                     i32  primum_n;
};

interior constans br_hash_class*
_classis (i32 genus)
{
    redde genus == TLS13_SHA384 ? &br_sha384_vtable
        : genus == TLS13_SHA256 ? &br_sha256_vtable : NIHIL;
}

i32
tls13_hash_mensura (i32 genus)
{
    redde genus == TLS13_SHA384 ? XLVIII
        : genus == TLS13_SHA256 ? XXXII : ZEPHYRUM;
}

interior vacuum
_hmac (i32 genus, constans i8* clavis, i32 clavis_n, constans i8* a,
    i32 a_n, constans i8* b, i32 b_n, constans i8* c, i32 c_n, i8* exitus)
{
    br_hmac_key_context kc;
    br_hmac_context     hc;

    br_hmac_key_init(&kc, _classis(genus), clavis, (size_t)clavis_n);
    br_hmac_init(&hc, &kc, ZEPHYRUM);
    si (a_n) br_hmac_update(&hc, a, (size_t)a_n);
    si (b_n) br_hmac_update(&hc, b, (size_t)b_n);
    si (c_n) br_hmac_update(&hc, c, (size_t)c_n);
    (vacuum)br_hmac_out(&hc, exitus);
}

vacuum
tls13_hkdf_extrahere (i32 genus, constans i8* sal, i32 sal_n,
    constans i8* ikm, i32 ikm_n, i8* prk)
{
    i8 zeros[TLS13_HASH_MAXIMUS];

    si (sal == NIHIL)
    {
        memset(zeros, ZEPHYRUM, magnitudo(zeros));
        sal   = zeros;
        sal_n = tls13_hash_mensura(genus);
    }
    _hmac(genus, sal, sal_n, ikm, ikm_n, NIHIL, ZEPHYRUM, NIHIL, ZEPHYRUM,
        prk);
}

b32
tls13_hkdf_expandere (i32 genus, constans i8* prk, constans i8* info,
    i32 info_n, i8* exitus, i32 mensura)
{
    i32 h = tls13_hash_mensura(genus);
    i8  t[TLS13_HASH_MAXIMUS];
    i32 t_n = ZEPHYRUM;
    i32 scripta = ZEPHYRUM;
    i8  numerus = (i8)I;

    si (h == ZEPHYRUM || mensura > CCLV * h)
    {
        redde FALSUM;
    }
    dum (scripta < mensura)
    {
        i32 n;

        _hmac(genus, prk, h, t, t_n, info, info_n, &numerus, I, t);
        t_n = h;
        n = mensura - scripta < h ? mensura - scripta : h;
        memcpy(exitus + scripta, t, n);
        scripta += n;
        numerus = (i8)(numerus + I);
    }
    redde VERUM;
}

/* HkdfLabel = uint16 length || opaque label<7..255> ("tls13 " +
 * titulus) || opaque context<0..255> */
b32
tls13_expandere_titulo (i32 genus, constans i8* secretum,
    constans character* titulus, constans i8* contextus, i32 ctx_n,
    i8* exitus, i32 mensura)
{
    i8  info[II + I + CCLV + I + CCLV];
    i32 lt = (i32)strlen(titulus);
    i32 n = ZEPHYRUM;

    si (VI + lt > CCLV || ctx_n > CCLV)
    {
        redde FALSUM;
    }
    info[n++] = (i8)(mensura >> VIII);
    info[n++] = (i8)(mensura & 0xff);
    info[n++] = (i8)(VI + lt);
    memcpy(info + n, "tls13 ", VI);
    n += VI;
    memcpy(info + n, titulus, lt);
    n += lt;
    info[n++] = (i8)ctx_n;
    si (ctx_n)
    {
        memcpy(info + n, contextus, ctx_n);
        n += ctx_n;
    }
    redde tls13_hkdf_expandere(genus, secretum, info, n, exitus, mensura);
}

vacuum
tls13_secretum_derivare (i32 genus, constans i8* secretum,
    constans character* titulus, constans i8* hash_transcripti, i8* exitus)
{
    i32 h = tls13_hash_mensura(genus);

    (vacuum)tls13_expandere_titulo(genus, secretum, titulus,
        hash_transcripti, h, exitus, h);
}

vacuum
tls13_claves_traffici (i32 genus, constans i8* secretum, i32 clavis_n,
    i8* clavis, i8* iv)
{
    (vacuum)tls13_expandere_titulo(genus, secretum, "key", NIHIL,
        ZEPHYRUM, clavis, clavis_n);
    (vacuum)tls13_expandere_titulo(genus, secretum, "iv", NIHIL,
        ZEPHYRUM, iv, XII);
}

vacuum
tls13_finis_calculare (i32 genus, constans i8* secretum_base,
    constans i8* hash_transcripti, i8* verify_data)
{
    i32 h = tls13_hash_mensura(genus);
    i8  clavis[TLS13_HASH_MAXIMUS];

    (vacuum)tls13_expandere_titulo(genus, secretum_base, "finished", NIHIL,
        ZEPHYRUM, clavis, h);
    _hmac(genus, clavis, h, hash_transcripti, h, NIHIL, ZEPHYRUM, NIHIL,
        ZEPHYRUM, verify_data);
}

vacuum
tls13_secretum_proximum (i32 genus, constans i8* secretum, i8* exitus)
{
    i32 h = tls13_hash_mensura(genus);

    (vacuum)tls13_expandere_titulo(genus, secretum, "traffic upd", NIHIL,
        ZEPHYRUM, exitus, h);
}

Tls13Transcriptum*
tls13_transcriptum_creare (i32 genus, Piscina* p)
{
    Tls13Transcriptum* t;

    si (_classis(genus) == NIHIL)
    {
        redde NIHIL;
    }
    t = (Tls13Transcriptum*)piscina_allocare(p, magnitudo(*t));
    si (t != NIHIL)
    {
        memset(t, ZEPHYRUM, magnitudo(*t));
        t->genus = genus;
        _classis(genus)->init(&t->status.vtable);
    }
    redde t;
}

vacuum
tls13_transcriptum_addere (Tls13Transcriptum* t, constans i8* o, i32 n)
{
    _classis(t->genus)->update(&t->status.vtable, o, (size_t)n);
}

vacuum
tls13_transcriptum_digerere (constans Tls13Transcriptum* t, i8* hash)
{
    /* out() statum non mutat (bearssl_hash.h) */
    _classis(t->genus)->out(&t->status.vtable, hash);
}

/* RFC 8446 4.4.1: Transcript-Hash(CH1, HRR, ...) = Hash(message_hash ||
 * 00 00 Hash.length || Hash(CH1) || HRR || ...). Vocandum post CH1
 * additum, ANTE HRR additum. */
vacuum
tls13_transcriptum_hrr (Tls13Transcriptum* t)
{
    i8  h1[TLS13_HASH_MAXIMUS];
    i8  caput[IV];
    i32 h = tls13_hash_mensura(t->genus);

    tls13_transcriptum_digerere(t, h1);
    _classis(t->genus)->init(&t->status.vtable);
    caput[ZEPHYRUM] = (i8)0xfe;
    caput[I]        = ZEPHYRUM;
    caput[II]       = ZEPHYRUM;
    caput[III]      = (i8)h;
    tls13_transcriptum_addere(t, caput, IV);
    tls13_transcriptum_addere(t, h1, h);
}
```

(`br_hash_compat_context` is BearSSL's union of hash contexts; its
`vtable` member is the class pointer the class functions take.)

- [ ] **Step 6: Run - expect PASS** `./compile_tests.sh probatio_tls13_clavis`
  -> 7/7; examen ACCIPE on the three new C files and `rfc8448.h`.

- [ ] **Step 7: Plants** (silva.planta, filter probatio_tls13_clavis):
  - `info[n++] = (i8)(VI + lt);` -> `info[n++] = (i8)(V + lt);` ->
    derivationes, claves, finis red.
  - `caput[ZEPHYRUM] = (i8)0xfe;` -> `(i8)0xff` - MUTE here (HRR proven
    in T5); record and re-plant in T5.
  - `si (numerus != declaratus)` -> `si (FALSUM)` in rfc8448.h - mute
    on a correct RFC (the check guards against a broken parse): prove it
    instead by planting `k += II;` -> `k += III;` in
    `_rfc8448_hex_colligere` -> lector red (counts mismatch).

- [ ] **Step 8: Commit**
  `tls13 T1: tabula clavium (HKDF-Expand-Label super br_hmac, transcriptum, HRR) + lector RFC 8448 (textus immutatus ut oraculum)`.

---

### Task 2: record layer `tls13_recordum`

**Files:** Create `include/tls13_recordum.h`, `lib/tls13_recordum.c`,
`probationes/probatio_tls13_recordum.c`.

**Interfaces:**
- Consumes: T1 `tls13_claves_traffici`, `tls13_secretum_proximum`,
  `tls13_hash_mensura`, `TLS13_SHA256/384`.
- Produces:

```c
#define TLS13_AES_128_GCM_SHA256        0x1301
#define TLS13_AES_256_GCM_SHA384        0x1302
#define TLS13_CHACHA20_POLY1305_SHA256  0x1303
#define TLS13_RECORDUM_MAXIMUM  (XVI * MXXIV + CCLVI + V)  /* 16645 */

#define TLS13_CONTENTUM_CCS        XX
#define TLS13_CONTENTUM_ALERTA     XXI
#define TLS13_CONTENTUM_PACTIO     XXII
#define TLS13_CONTENTUM_DATA       XXIII

i32  tls13_cifra_genus (i32 cifra);     /* TLS13_SHA256/384; 0 ignota */
nomen structura Tls13Protectio Tls13Protectio;
Tls13Protectio* tls13_protectio_creare (i32 cifra, constans i8* secretum,
    Piscina* p);
/* sigillare: header (17 03 03 len) + AEAD(inner) in exitus; reddit
 * longitudinem totam aut 0 (capacitas) */
i32  tls13_obsignare (Tls13Protectio* pr, i32 genus_contenti,
    constans i8* data, i32 n, i8* exitus, i32 capacitas);
/* aperire recordum totum (header + corpus): plaintext in exitus,
 * *genus_contenti; reddit n >= 0, TLS13_RECORDUM_MAC (-1) aut
 * TLS13_RECORDUM_MOLES (-2) aut TLS13_RECORDUM_FORMA (-3) */
s32  tls13_aperire (Tls13Protectio* pr, constans i8* recordum, i32 n,
    i8* exitus, i32 capacitas, i32* genus_contenti);
vacuum tls13_protectio_renovare (Tls13Protectio* pr);  /* KeyUpdate */
```

- [ ] **Step 1: Failing test `probationes/probatio_tls13_recordum.c`**
  (suite fixture = RFC 8448 reader as in T1). Sections:
  - `obsignare_servus`: protection from §3 server "derive secret
    \"tls13 s hs traffic\"" `expanded` (cifra 0x1301); seal the payload
    of §3 {server} "send handshake record" (ordo 1 - the encrypted flight;
    `payload`) as type 22 -> must equal its `complete record`
    byte for byte.
  - `aperire_servus`: open that complete record -> type 22, payload
    equal.
  - `data`: §3 {client} "send application_data record" (`payload`,
    `complete record`) with the client ap traffic secret; and the
    server's app record opened with the server ap secret.
  - `vitium_mac`: one bit flipped in the tag -> `TLS13_RECORDUM_MAC`;
    `vitium_ordo`: opening the same record twice (sequence advanced) ->
    MAC; `vitium_moles`: header length 16641 -> `MOLES`; inner plaintext
    of zeros only -> `FORMA`.
  - `chacha`: seal+open round trip with 0x1303 and a fixed secret; tag
    tamper -> MAC (no RFC vector; BearSSL's ChaCha/Poly KAT is the
    `bearssl` gate).
  - `renovare`: after `tls13_protectio_renovare` on both sides with the
    same secret, records still round-trip, and a record sealed BEFORE the
    update fails to open AFTER it.

  Code pattern as T1 (`rfc8448_quaerere(..., "send handshake record",
  I, "complete record")`).

- [ ] **Step 2: Run - expect FAIL** (header missing).

- [ ] **Step 3: Implement `lib/tls13_recordum.c`:**

```c
/* tls13_recordum.c - stratum recordorum TLS 1.3 (RFC 8446 §5.2-5.3):
 * nonce = iv XOR numerus (64 bit, dextrorsum), TLSInnerPlaintext =
 * contentum || genus || zeros; AES-GCM (br_aes_ct64_ctr + ghash_ctmul64,
 * tempore constanti - P8) aut ChaCha20-Poly1305 (br_poly1305_ctmul_run
 * + br_chacha20_ct_run). */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include <string.h>

structura Tls13Protectio {
                   i32  cifra;
                   i32  genus;           /* hash */
                    i8  secretum[TLS13_HASH_MAXIMUS];
                    i8  clavis[XXXII];
                   i32  clavis_n;
                    i8  iv[XII];
                   i64  numerus;         /* sequentia */
    br_aes_ct64_ctr_keys  aes;
          br_gcm_context  gcm;
};

i32
tls13_cifra_genus (i32 cifra)
{
    redde cifra == TLS13_AES_256_GCM_SHA384 ? TLS13_SHA384
        : (cifra == TLS13_AES_128_GCM_SHA256
           || cifra == TLS13_CHACHA20_POLY1305_SHA256) ? TLS13_SHA256
        : ZEPHYRUM;
}

interior vacuum
_claves (Tls13Protectio* pr)
{
    tls13_claves_traffici(pr->genus, pr->secretum, pr->clavis_n,
        pr->clavis, pr->iv);
    pr->numerus = ZEPHYRUM;
    si (pr->cifra != TLS13_CHACHA20_POLY1305_SHA256)
    {
        br_aes_ct64_ctr_init(&pr->aes, pr->clavis, (size_t)pr->clavis_n);
        br_gcm_init(&pr->gcm, &pr->aes.vtable, &br_ghash_ctmul64);
    }
}

Tls13Protectio*
tls13_protectio_creare (i32 cifra, constans i8* secretum, Piscina* p)
{
    Tls13Protectio* pr;
    i32 genus = tls13_cifra_genus(cifra);

    si (genus == ZEPHYRUM)
    {
        redde NIHIL;
    }
    pr = (Tls13Protectio*)piscina_allocare(p, magnitudo(*pr));
    si (pr == NIHIL)
    {
        redde NIHIL;
    }
    memset(pr, ZEPHYRUM, magnitudo(*pr));
    pr->cifra    = cifra;
    pr->genus    = genus;
    pr->clavis_n = cifra == TLS13_AES_128_GCM_SHA256 ? XVI : XXXII;
    memcpy(pr->secretum, secretum, tls13_hash_mensura(genus));
    _claves(pr);
    redde pr;
}

interior vacuum
_nonce (constans Tls13Protectio* pr, i8* nonce)
{
    i32 k;

    memcpy(nonce, pr->iv, XII);
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        nonce[XI - k] = (i8)(nonce[XI - k]
            ^ (i8)((pr->numerus >> (VIII * k)) & 0xff));
    }
}

/* AEAD in loco: data (n) cum aad (V) -> tag (XVI); cifrare VERUM aut
 * FALSUM; FALSUM reddit si tag non congruit (solum aperiendo) */
interior b32
_aead (Tls13Protectio* pr, b32 cifrare, constans i8* aad, i8* data, i32 n,
    i8* tag)
{
    i8 nonce[XII];

    _nonce(pr, nonce);
    si (pr->cifra == TLS13_CHACHA20_POLY1305_SHA256)
    {
        i8  computatus[XVI];
        i32 k;
        i8  differentia = ZEPHYRUM;

        br_poly1305_ctmul_run(pr->clavis, nonce, data, (size_t)n, aad, V,
            cifrare ? tag : computatus, &br_chacha20_ct_run,
            cifrare ? I : ZEPHYRUM);
        si (cifrare)
        {
            redde VERUM;
        }
        per (k = ZEPHYRUM; k < XVI; k++)
        {
            differentia = (i8)(differentia | (computatus[k] ^ tag[k]));
        }
        redde differentia == ZEPHYRUM;
    }
    br_gcm_reset(&pr->gcm, nonce, XII);
    br_gcm_aad_inject(&pr->gcm, aad, V);
    br_gcm_flip(&pr->gcm);
    br_gcm_run(&pr->gcm, cifrare ? I : ZEPHYRUM, data, (size_t)n);
    si (cifrare)
    {
        br_gcm_get_tag(&pr->gcm, tag);
        redde VERUM;
    }
    redde br_gcm_check_tag(&pr->gcm, tag) != ZEPHYRUM;
}

i32
tls13_obsignare (Tls13Protectio* pr, i32 genus_contenti,
    constans i8* data, i32 n, i8* exitus, i32 capacitas)
{
    i32 corpus = n + I + XVI;

    si (n > XVI * MXXIV || V + corpus > capacitas)
    {
        redde ZEPHYRUM;
    }
    exitus[ZEPHYRUM] = (i8)TLS13_CONTENTUM_DATA;
    exitus[I]        = (i8)0x03;
    exitus[II]       = (i8)0x03;
    exitus[III]      = (i8)(corpus >> VIII);
    exitus[IV]       = (i8)(corpus & 0xff);
    memcpy(exitus + V, data, n);
    exitus[V + n] = (i8)genus_contenti;
    (vacuum)_aead(pr, VERUM, exitus, exitus + V, n + I, exitus + V + n + I);
    pr->numerus++;
    redde V + corpus;
}

s32
tls13_aperire (Tls13Protectio* pr, constans i8* recordum, i32 n,
    i8* exitus, i32 capacitas, i32* genus_contenti)
{
    i32 corpus;
    i32 k;

    si (n < V)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    corpus = ((i32)(unsigned char)recordum[III] << VIII)
        | (i32)(unsigned char)recordum[IV];
    si (corpus > XVI * MXXIV + CCLVI)
    {
        redde TLS13_RECORDUM_MOLES;
    }
    si (corpus != n - V || corpus < XVII || corpus - XVI > capacitas
        || (unsigned char)recordum[ZEPHYRUM] != TLS13_CONTENTUM_DATA)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    memcpy(exitus, recordum + V, corpus - XVI);
    si (!_aead(pr, FALSUM, recordum, exitus, corpus - XVI,
            (i8*)(recordum + V + corpus - XVI)))
    {
        redde TLS13_RECORDUM_MAC;
    }
    pr->numerus++;
    /* genus = ultimus octetus non-zero; zeros = padding */
    k = corpus - XVI;
    dum (k > ZEPHYRUM && exitus[k - I] == ZEPHYRUM)
    {
        k--;
    }
    si (k == ZEPHYRUM)
    {
        redde TLS13_RECORDUM_FORMA;
    }
    *genus_contenti = (i32)(unsigned char)exitus[k - I];
    si (k - I > XVI * MXXIV)
    {
        redde TLS13_RECORDUM_MOLES;
    }
    redde (s32)(k - I);
}

vacuum
tls13_protectio_renovare (Tls13Protectio* pr)
{
    i8 novum[TLS13_HASH_MAXIMUS];

    tls13_secretum_proximum(pr->genus, pr->secretum, novum);
    memcpy(pr->secretum, novum, tls13_hash_mensura(pr->genus));
    _claves(pr);
}
```

(`_aead` casts away const on the received tag for `br_gcm_check_tag`;
if -Wcast-qual complains, copy the tag into a local first - ruling.)

- [ ] **Step 4: Run - expect PASS** (`probatio_tls13_recordum` 8/8).

- [ ] **Step 5: Plants:** `nonce[XI - k]` -> `nonce[X - k]` (obsignare /
  aperire red after record 0 - record 0 has numerus 0, so the §3 single
  server record would be mute; the `data` section's SECOND record must
  catch it - verify red); `pr->numerus++;` in aperire dropped ->
  vitium_ordo red; `exitus[V + n] = (i8)genus_contenti;` -> `= ZEPHYRUM`
  -> obsignare_servus red.

- [ ] **Step 6: Commit** `tls13 T2: stratum recordorum (AES-GCM, ChaCha20-Poly1305, KeyUpdate) contra RFC 8448`.

---

### Task 3: message codecs `tls13_nuntii` + truncation sweep

**Files:** Create `include/tls13_nuntii.h`, `lib/tls13_nuntii.c`,
`probationes/probatio_tls13_nuntii.c`.

**Interfaces:**
- Produces:

```c
#define TLS13_GREX_X25519     0x001d
#define TLS13_GREX_SECP256R1  0x0017
#define TLS13_NUNTIUS_CLIENTHELLO    I
#define TLS13_NUNTIUS_SERVERHELLO    II
#define TLS13_NUNTIUS_NST            IV
#define TLS13_NUNTIUS_EE             VIII
#define TLS13_NUNTIUS_CERTIFICATUM   XI
#define TLS13_NUNTIUS_CV             XV
#define TLS13_NUNTIUS_FINIS          XX
#define TLS13_NUNTIUS_KEYUPDATE      XXIV

nomen structura {
    constans i8* octeti;  i32 mensura;  i32 locus;  b32 fractus;
} Tls13Cursor;            /* legere: semper intra fines; fractus = FORMA */

nomen structura {
    constans character* hospes;     /* SNI (non IP) */
    constans i8* temere;            /* XXXII */
    constans i8* sessio;  i32 sessio_n;     /* 0 aut XXXII */
    i32 grex;  constans i8* clavis_publica;  i32 clavis_n;
    constans i8* crustulum;  i32 crustulum_n;  /* cookie ex HRR */
} Tls13SalutatioParametri;

/* ClientHello TOTUM (cum capite pactionis IV octetorum); reddit n aut 0 */
i32 tls13_salutatio_scribere (constans Tls13SalutatioParametri* sp,
    i8* exitus, i32 capacitas);

nomen structura {
    b32 hrr;                 /* random = HRR constans */
    b32 versio_1_2;          /* supported_versions abest -> 1.2 */
    b32 signum_descensus;    /* random finit DOWNGRD\0 aut \1 */
    i32 versio;              /* 0x0304 si supported_versions */
    i32 cifra;
    constans i8* sessio;  i32 sessio_n;
    i32 grex;                /* key_share (SH) aut selected_group (HRR) */
    constans i8* clavis;  i32 clavis_n;       /* SH */
    constans i8* crustulum; i32 crustulum_n;  /* HRR cookie */
} Tls13ResponsumServi;
/* corpus SINE capite IV octetorum; FALSUM = FORMA */
b32 tls13_responsum_legere (constans i8* corpus, i32 n,
    Tls13ResponsumServi* r);
b32 tls13_extensiones_legere (constans i8* corpus, i32 n);   /* EE */
/* Certificate: catena DER in exitus (maximum numerus), *numerus */
b32 tls13_certificata_legere (constans i8* corpus, i32 n,
    FiduciaCertificatum* exitus, i32 maximum, i32* numerus);
b32 tls13_cv_legere (constans i8* corpus, i32 n, i32* schema,
    constans i8** subscriptio, i32* subscriptio_n);
b32 tls13_keyupdate_legere (constans i8* corpus, i32 n, b32* rogatur);
```

- [ ] **Step 1: Failing test `probationes/probatio_tls13_nuntii.c`**
  (RFC 8448 fixture). Sections:
  - `serverhello`: §3 ServerHello (strip the 4-byte header) -> hrr
    FALSUM, versio 0x0304, cifra 0x1301, grex 0x001d, clavis = §3
    {server} x25519 `public key`.
  - `hrr`: §5 {server} "construct a ServerHello" ordo 0 -> hrr VERUM,
    grex 0x0017 (P-256).
  - `hrr_crustulum`: a hand-built HRR (from §5's, with a cookie
    extension `00 2c 00 06 00 04 de ad be ef` spliced in, lengths fixed)
    -> crustulum = de ad be ef; then a CH2 written with that cookie
    contains `00 2c 00 06 00 04 de ad be ef` (Review focus 2).
  - `certificata`: §3 Certificate -> one DER, length = the cert's
    3-byte length, first bytes 30 82.
  - `cv`: §3 CertificateVerify -> schema 0x0804, subscriptio 128 octets.
  - `ee`: §3 EncryptedExtensions accepted.
  - `salutatio`: our ClientHello for hospes "server", fixed random and
    session id, x25519 key = §3 client public key: parse it back with a
    small ClientHello reader in the test (extensions list) -> contains
    exactly SNI "server", supported_versions {0304}, groups {001d,0017},
    key_share {001d: key}, signature_algorithms {0403 0503 0804 0805
    0806}, session id 32 bytes, suites {1301 1302 1303}.
  - `versio_1_2`: a 1.2 ServerHello (from probationes: OpenSSL -tls1_2
    capture is NOT committed; build it in the test: legacy 0x0303,
    random ending DOWNGRD\x01, suite c02c, no extensions) -> versio_1_2
    VERUM, signum_descensus VERUM.
  - `truncatio`: every message above cut at every length 0..n-1 ->
    FALSUM, never a crash (`CREDO_NON_RUIT` around the loop).

- [ ] **Step 2: Run - expect FAIL.**

- [ ] **Step 3: Implement `lib/tls13_nuntii.c`** - cursor helpers
  (`_u8`, `_u16`, `_u24`, `_vector8/16/24` returning sub-cursors; any
  over-read sets `fractus` and returns 0), the writer (big-endian puts
  into a bounded buffer with a length stack for nested vectors), and:

```c
/* ServerHello / HelloRetryRequest (RFC 8446 4.1.3, 4.1.4) */
hic_manens constans i8 HRR_TEMERE[XXXII] = {
    (i8)0xcf, (i8)0x21, (i8)0xad, (i8)0x74, (i8)0xe5, (i8)0x9a, (i8)0x61,
    (i8)0x11, (i8)0xbe, (i8)0x1d, (i8)0x8c, (i8)0x02, (i8)0x1e, (i8)0x65,
    (i8)0xb8, (i8)0x91, (i8)0xc2, (i8)0xa2, (i8)0x11, (i8)0x16, (i8)0x7a,
    (i8)0xbb, (i8)0x8c, (i8)0x5e, (i8)0x07, (i8)0x9e, (i8)0x09, (i8)0xe2,
    (i8)0xc8, (i8)0xa8, (i8)0x33, (i8)0x9c };
hic_manens constans i8 DESCENSUS[VII] = { 0x44, 0x4f, 0x57, 0x4e, 0x47,
    0x52, 0x44 };   /* "DOWNGRD" + 00/01 */

b32
tls13_responsum_legere (constans i8* corpus, i32 n, Tls13ResponsumServi* r)
{
    Tls13Cursor c;
    Tls13Cursor sessio;
    Tls13Cursor ext;
    constans i8* temere;
    b32 sv = FALSUM;

    memset(r, ZEPHYRUM, magnitudo(*r));
    _cursor(&c, corpus, n);
    (vacuum)_u16(&c);                        /* legacy_version */
    temere = _octeti(&c, XXXII);
    sessio = _vector8(&c);
    r->cifra = (i32)_u16(&c);
    (vacuum)_u8(&c);                         /* compressio */
    si (c.fractus || temere == NIHIL)
    {
        redde FALSUM;
    }
    r->hrr = memcmp(temere, HRR_TEMERE, XXXII) == ZEPHYRUM;
    r->signum_descensus = memcmp(temere + XXIV, DESCENSUS, VII) == ZEPHYRUM
        && (temere[XXXI] == ZEPHYRUM || temere[XXXI] == I);
    r->sessio   = sessio.octeti + sessio.locus;
    r->sessio_n = sessio.mensura - sessio.locus;
    si (c.locus == c.mensura)
    {
        r->versio_1_2 = VERUM;              /* sine extensionibus */
        redde VERUM;
    }
    ext = _vector16(&c);
    dum (!ext.fractus && ext.locus < ext.mensura)
    {
        i32 typus = (i32)_u16(&ext);
        Tls13Cursor e = _vector16(&ext);

        si (typus == 0x002b)                 /* supported_versions */
        {
            r->versio = (i32)_u16(&e);
            sv = VERUM;
        }
        alioquin si (typus == 0x0033)        /* key_share */
        {
            r->grex = (i32)_u16(&e);
            si (!r->hrr)
            {
                Tls13Cursor k = _vector16(&e);

                r->clavis   = k.octeti + k.locus;
                r->clavis_n = k.mensura - k.locus;
                e.fractus  |= k.fractus;
            }
        }
        alioquin si (typus == 0x002c)        /* cookie */
        {
            Tls13Cursor k = _vector16(&e);

            r->crustulum   = k.octeti + k.locus;
            r->crustulum_n = k.mensura - k.locus;
            e.fractus     |= k.fractus;
        }
        si (e.fractus)
        {
            redde FALSUM;
        }
    }
    si (ext.fractus || c.locus != c.mensura)
    {
        redde FALSUM;
    }
    r->versio_1_2 = !sv;
    redde VERUM;
}
```

  plus `tls13_salutatio_scribere` (fields in the order of RFC 8446 4.1.2
  with extensions server_name 0x0000 (host_name type 0, omitted when
  hospes is an IP literal), supported_groups 0x000a, signature_algorithms
  0x000d, supported_versions 0x002b, cookie 0x002c (only if crustulum),
  key_share 0x0033), `tls13_certificata_legere` (request_context empty
  vector, certificate_list of {cert_data<1..2^24-1>, extensions<0..2^16-1>}),
  `tls13_cv_legere`, `tls13_extensiones_legere` (vector, every extension
  well-formed; unknown ones ignored), `tls13_keyupdate_legere` (one byte
  0 or 1, else FALSUM). Each in the cursor style above; the cursor
  helpers are the only place that reads bytes.

- [ ] **Step 4: Run - expect PASS** (9/9).
- [ ] **Step 5: Plants:** `r->versio_1_2 = !sv;` -> `= FALSUM;` ->
  versio_1_2 red; in `_vector16`, drop the bounds check (compiles) ->
  truncatio red (CREDO_NON_RUIT catches the over-read crash or a
  FALSUM missing); cookie branch removed -> hrr_crustulum red.
- [ ] **Step 6: Commit** `tls13 T3: nuntii (ClientHello, ServerHello/HRR, EE, Certificate, CV, KeyUpdate) + verritio truncationis`.

---

### Task 4: engine - 1-RTT path + CertificateVerify (RFC 8448 §3 and §7 replay)

**Files:** Create `include/tls13.h`, `lib/tls13.c`,
`probationes/probatio_tls13_machina.c`, `lib/tls13.worklog.md`.

**Interfaces:**
- Consumes: T1-T3 (exact names above); `fiducia.h`
  (`FiduciaCertificatum`).
- Produces: spec II.2's API verbatim, plus the test seam (P2):

```c
nomen structura {
    constans character* hospes;
                   b32  verificare;
          Tls13Fiducia  fiducia;   vacuum* fiducia_datum;
             Tls13Fors  fors;      vacuum* fors_datum;
    /* PROBATIONES SOLUM (P2): ClientHello(s) fixa loco encodatoris -
     * [0] CH1, [1] CH2 post HRR; NIHIL = encodator */
    constans i8* salutationes_fixae[II];
    i32          salutationes_fixae_n[II];
} Tls13Optiones;

#define TLS13_RECORDA_MITTENDA   I
#define TLS13_RECORDA_EXSPECTAT  II
#define TLS13_DATA_MITTI_POSSUNT IV
#define TLS13_DATA_ADSUNT        VIII
#define TLS13_CLAUSA             XVI

nomen enumeratio {
    TLS13_ERROR_NULLUS = 0, TLS13_ERROR_SOLUM_1_2, TLS13_ERROR_FIDUCIA,
    TLS13_ERROR_SUBSCRIPTIO, TLS13_ERROR_FINIS, TLS13_ERROR_FORMA,
    TLS13_ERROR_PARAMETRUM, TLS13_ERROR_MAC, TLS13_ERROR_MOLES,
    TLS13_ERROR_ALERTA_RECEPTA, TLS13_ERROR_ENTROPIA
} Tls13ErrorCodex;
nomen structura { Tls13ErrorCodex codex; chorda causa; i32 alerta; }
    Tls13Error;
```

**Randomness order** (documented in tls13.h, used by tests): the engine
calls `fors` for (1) x25519 private key 32, (2) client random 32 (only
without a fixed ClientHello), (3) session id 32 (same), then after an
HRR (4) P-256 private key 32 (retried if invalid).

- [ ] **Step 1: Failing test `probationes/probatio_tls13_machina.c`.**
  Test harness pieces:
  - `_fors_fixa`: returns queued byte strings in call order (from the
    RFC: §3 {client} "create an ephemeral x25519 key pair" `private key`).
  - `_fiducia_ficta`: stub trust callback, records the chain, returns
    VERUM (the RFC certificate is self-made, not from a real CA).
  - `_alimentare(m, octeti, n, per_octetum)`: feeds bytes into
    `tls13_recorda_introitus`, either all at once or ONE BYTE per call
    (Review focus 1), acking what the engine takes.
  - `_exhaurire(m, capsa)`: collects everything from
    `tls13_recorda_exitus` until the engine has nothing to send.

  Sections:
  - `s3_salutatio`: create with the §3 ClientHello fixed -> output
    = §3 {client} "send handshake record" ordo 0 `complete record`.
  - `s3_pactio`: feed §3 {server} "send handshake record" ordo 0 (SH)
    and ordo 1 (encrypted flight) `complete record` -> the engine
    calls `_fiducia_ficta` once with one certificate (the RFC's), the
    RSA-PSS CertificateVerify VERIFIES (real BearSSL verification of
    the RFC's signature), status gains `TLS13_DATA_MITTI_POSSUNT`, output
    = §3 {client} "send handshake record" ordo 1 `complete record`
    (client Finished), `tls13_machina_versio` = 0x0304.
  - `s3_per_octetum`: the same, server records fed one byte at a time ->
    identical output (Review focus 1).
  - `s3_data`: write §3 {client} "send application_data record" `payload`
    (50 octets), flush -> output = its `complete record`; feed the
    server's NewSessionTicket record (§3 {server} "send handshake record"
    ordo 2) and its application_data record -> NST dropped, data
    delivered = server payload, in order (Review focus 4).
  - `s3_clausura`: feed the server's alert record (§3 {server} "send
    alert record", close_notify) -> `TLS13_CLAUSA`, no error.
  - `s7_compatibilitas`: §7 ClientHello fixed (non-empty session id);
    the server's records include its own `send change_cipher_spec
    record` (ignored by us) -> the client's second flight = §7 {client}
    "send change_cipher_spec record" `complete record` (14 03 03 00 01
    01) followed by its "send handshake record" ordo 1 (Finished), byte
    for byte (P3).
  - `subscriptio_falsa`: the server flight is rebuilt by the test, no
    engine hook: take §3's encrypted-flight `payload` (EE || Certificate
    || CV || Finished, plaintext in the RFC), flip one byte inside the CV
    signature, re-seal it with T2's `tls13_obsignare` under the §3 server
    hs traffic secret (type 22, sequence 0) -> `TLS13_ERROR_SUBSCRIPTIO`,
    alert 51 queued.
  - `fiducia_negata`: stub returns FALSUM with causa "ficta" ->
    `TLS13_ERROR_FIDUCIA`, causa "ficta", alert 42 queued.
  - `finis_falsus`: re-sealed flight with one byte of the server
    Finished flipped -> `TLS13_ERROR_FINIS`, alert 51.

  (The exact action names are grepped from the RFC while writing - the
  plan's names are the RFC's §3 headers: "send handshake record",
  "send application_data record", "send alert record"; verify with
  `awk 'NR>=152 && NR<857 && /^   \{/' rfc8448.txt`.)

- [ ] **Step 2: Run - expect FAIL.**

- [ ] **Step 3: Implement `lib/tls13.c`** - structure and the 1-RTT
  path:

```c
/* tls13.c - MACHINA clientis TLS 1.3 (RFC 8446), sine fossis (spec
 * II.2): capsae recordorum et datorum, fors et fiducia per reditus,
 * nihil globale. Stadia: SALUTATIO_MISSA -> (HRR) -> EE -> CERT -> CV
 * -> FINIS -> APERTA -> CLAUSA | ERROR. */
#include "../vendor/bearssl_amalgama.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "fiducia.h"
#include "tls13_clavis.h"
#include "tls13_recordum.h"
#include "tls13_nuntii.h"
#include "tls13.h"
#include <string.h>

#define PACTIO_MAXIMA  (LXIV * MXXIV)   /* catena + nuntii coniuncti */
#define SALUTATIO_MAXIMA  MXXIV

nomen enumeratio {
    STADIUM_SALUTATIO_MISSA = 0, STADIUM_EE, STADIUM_CERT, STADIUM_CV,
    STADIUM_FINIS, STADIUM_APERTA, STADIUM_CLAUSA, STADIUM_ERROR
} Stadium;

structura Tls13Machina {
                Piscina* piscina;
          Tls13Optiones  opt;
                Stadium  stadium;
                    b32  hrr_visa;
                    i32  cifra;
                    i32  genus;            /* hash */
     Tls13Transcriptum* transcriptum;
                     i8  salutatio[SALUTATIO_MAXIMA];   /* CH in transcriptum differtur */
                    i32  salutatio_n;
                    b32  sessio_non_vacua;
                    i8   sessio[XXXII];
                     i8  privata_x25519[XXXII];
                     i8  privata_p256[XXXII];
                    i32  grex;
                     i8  secretum_pactionis[TLS13_HASH_MAXIMUS];
                     i8  c_hs[TLS13_HASH_MAXIMUS];
                     i8  s_hs[TLS13_HASH_MAXIMUS];
        Tls13Protectio* legere;           /* NIHIL = plaintext */
        Tls13Protectio* scribere;
                     i8  in[TLS13_RECORDUM_MAXIMUM];
                    i32  in_n;
                     i8  ex[II * TLS13_RECORDUM_MAXIMUM];
                    i32  ex_n;
                     i8  pactio[PACTIO_MAXIMA];   /* nuntii reassemblandi */
                    i32  pactio_n;
                     i8  data_in[XVI * MXXIV];
                    i32  data_in_locus;
                    i32  data_in_n;
                     i8  data_ex[XVI * MXXIV];
                    i32  data_ex_n;
    FiduciaCertificatum  catena[XVI];
                    i32  numerus;
     br_x509_decoder_context  decoder;
                    i32  versio;
             Tls13Error  error;
    br_hmac_drbg_context drbg;            /* si fors NIHIL */
};
```

  Then, function by function (each written in full in the task, shown
  here by contract and key lines):
  - `_errorem(m, codex, alerta, causa)`: sets `error` once, queues the
    alert as a record (plaintext before keys, sealed after), stadium
    ERROR.
  - `_fors(m, buf, n)`: `opt.fors` or the DRBG (seeded by
    `br_prng_seeder_system` at creation; failure -> ENTROPIA).
  - `_x25519_publica(priv, pub)` / `_x25519_communis(priv, peer, out)`:
    reverse the scalar into a local (P6) and call
    `br_ec_get_default()->mulgen(pub, rev, 32, BR_EC_curve25519)` /
    `->mul(G, 32, rev, 32, BR_EC_curve25519)` (G = copy of peer, result
    in G).
  - `_recordum_plaintext(m, genus, octeti, n)`: header 16/15 03 01 (first
    CH) or 03 03, appended to `ex`.
  - `_mittere_pactionem(m, nuntius, n)`: plaintext before `scribere`,
    else `tls13_obsignare(m->scribere, 22, ...)`; transcript updated by
    the caller.
  - `tls13_machina_creare`: copy options (hospes into the piscina), DRBG
    if needed, x25519 key from `_fors`, ClientHello = fixed[0] or
    `tls13_salutatio_scribere` (random + session id from `_fors`),
    stored in `salutatio`; `sessio_non_vacua` from it (P3); first record
    queued; stadium SALUTATIO_MISSA.
  - `tls13_recorda_introitus_ack(m, n)`: appends to `in`; while `in`
    holds a complete record (5-byte header + length), `_recordum(m)`.
  - `_recordum(m)`: CCS before APERTA -> ignore (1 byte 0x01 else
    FORMA); alert -> `_alerta`; handshake plaintext only while
    `legere == NIHIL`; application_data -> `tls13_aperire(legere)` then
    by inner type: handshake -> `_pactio_addere`, data -> `data_in`
    (only APERTA), alert -> `_alerta`. Errors from aperire -> MAC/MOLES/
    FORMA.
  - `_pactio_addere(m, o, n)`: appends to `pactio`; while it holds a
    complete message (type + 3-byte length), `_nuntius(m, typus, corpus,
    n_corporis, crudum, n_crudi)`, then shift.
  - `_nuntius`, the state machine:
    - SALUTATIO_MISSA + ServerHello: `tls13_responsum_legere`; FORMA on
      failure; `versio_1_2` -> `_errorem(SOLUM_1_2, 0, ...)` (no alert);
      HRR -> T5; checks: version 0x0304, cifra offered, grex = ours and
      key present, session echo equal to ours -> PARAMETRUM otherwise.
      Then: `genus = tls13_cifra_genus(cifra)`; transcript created, CH
      and SH added; shared secret; key schedule:
      ```c
      tls13_hkdf_extrahere(g, NIHIL, 0, zeros, h, primum);          /* early */
      tls13_secretum_derivare(g, primum, "derived", hash_vacuum, sal);
      tls13_hkdf_extrahere(g, sal, h, communis, cn, m->secretum_pactionis);
      tls13_transcriptum_digerere(m->transcriptum, th);
      tls13_secretum_derivare(g, m->secretum_pactionis, "c hs traffic", th, m->c_hs);
      tls13_secretum_derivare(g, m->secretum_pactionis, "s hs traffic", th, m->s_hs);
      m->legere = tls13_protectio_creare(m->cifra, m->s_hs, m->piscina);
      ```
      (`hash_vacuum` = Hash("") of the suite's hash, computed by a fresh
      transcript with nothing added.) stadium EE.
    - EE -> `tls13_extensiones_legere`, transcript add, stadium CERT.
    - CERT -> `tls13_certificata_legere` into `catena` (DER copied into
      the piscina, so they outlive `pactio`), leaf pushed into
      `br_x509_decoder` (init NIHIL, push, check get_pkey), transcript
      add, stadium CV.
    - CV -> trust first: unless `!opt.verificare`,
      `opt.fiducia(datum, catena, numerus, hospes, &causa, piscina)` ->
      FALSUM -> FIDUCIA (alert 42, causa from the callback). Then the
      signature: content = 64 x 0x20 || "TLS 1.3, server
      CertificateVerify" || 0x00 || Transcript-Hash(through
      Certificate); hashed with the scheme's hash; ECDSA (0x0403
      SHA-256, 0x0503 SHA-384) via `br_ecdsa_vrfy_asn1_get_default()(
      br_ec_get_default(), hash, hn, &pk->key.ec, sig, sn)`, RSA-PSS
      (0x0804/05/06) via `br_rsa_pss_vrfy_get_default()(sig, sn, hf, hf,
      hash, hn, &pk->key.rsa)` (salt length = hash length); key type must
      match the scheme; any other scheme -> PARAMETRUM; failure ->
      SUBSCRIPTIO (alert 51). Transcript add, stadium FINIS.
    - FINIS -> expected = `tls13_finis_calculare(g, s_hs,
      Transcript-Hash(through CV))`; constant-time compare -> FINIS
      (alert 51); transcript add. Master:
      ```c
      tls13_secretum_derivare(g, m->secretum_pactionis, "derived", hash_vacuum, sal);
      tls13_hkdf_extrahere(g, sal, h, zeros, h, magister);
      tls13_transcriptum_digerere(m->transcriptum, th);   /* per Finished servi */
      tls13_secretum_derivare(g, magister, "c ap traffic", th, c_ap);
      tls13_secretum_derivare(g, magister, "s ap traffic", th, s_ap);
      ```
      Then the client flight: CCS record `14 03 03 00 01 01` if
      `sessio_non_vacua`; `scribere = protectio(c_hs)`; client Finished =
      `14 00 00 h || tls13_finis_calculare(g, c_hs, th)` sealed as type
      22; then `legere = protectio(s_ap)`, `scribere =
      protectio(c_ap)`; `versio = 0x0304`; stadium APERTA.
    - APERTA + NewSessionTicket -> dropped (well-formedness: length
      within the message); + KeyUpdate -> T5; anything else ->
      PARAMETRUM (alert 10 unexpected_message - add to the error table
      as PARAMETRUM).
  - `_alerta(m, o, n)`: 2 bytes; level/description; close_notify (0) ->
    stadium CLAUSA (status `TLS13_CLAUSA`); protocol_version (70) while
    SALUTATIO_MISSA -> SOLUM_1_2; other -> ALERTA_RECEPTA with the
    description named (`_alerta_titulus`: 40 handshake_failure, 42
    bad_certificate, 47 illegal_parameter, 50 decode_error, 70
    protocol_version, 71 insufficient_security, 80 internal_error, 112
    unrecognized_name, 116 certificate_required).
  - data side: `tls13_data_exitus` gives `data_ex` space while APERTA;
    `tls13_data_effundere` seals `data_ex` into one record (only if `ex`
    has room - else the data stays and the flag drops); `tls13_data_
    introitus` hands out `data_in`; ack consumes.
  - `tls13_machina_status`: RECORDA_MITTENDA if `ex_n`; RECORDA_EXSPECTAT
    unless CLAUSA/ERROR; DATA_MITTI_POSSUNT if APERTA and `ex_n == 0`;
    DATA_ADSUNT if `data_in_n > data_in_locus`; CLAUSA if CLAUSA.
  - `tls13_machina_claudere`: close_notify alert queued (sealed), stadium
    CLAUSA.

- [ ] **Step 4: Run - expect PASS** `probatio_tls13_machina` 9/9.
- [ ] **Step 5: Plants:** RSA-PSS salt length `hn` -> `hn - I` ->
  s3_pactio red (real signature rejected); CCS emission condition
  `sessio_non_vacua` -> `VERUM` -> s3_pactio red (extra CCS bytes);
  `_pactio_addere`'s "while complete message" loop -> "if" ->
  s3_per_octetum or s3_pactio red (two messages in one record).
- [ ] **Step 6: Commit** `tls13 T4: machina clientis 1-RTT (RFC 8448 §3 et §7 octetim), CertificateVerify RSA-PSS/ECDSA, fiducia per reditum`.

---

### Task 5: engine - HelloRetryRequest, alerts, errors, KeyUpdate

**Files:** Modify `lib/tls13.c`, `probationes/probatio_tls13_machina.c`.

- [ ] **Step 1: Failing sections:**
  - `s5_hrr`: §5 replay - CH1 and CH2 fixed (§5 {client} "construct a
    ClientHello" ordo 0 and 1), `fors` queue = §5 x25519 private key,
    then §5 P-256 private key (§5 {client} "create an ephemeral P-256
    key pair" `private key`) -> after the HRR record, output = §5 CH2
    record (no CCS - empty session id); after the server flight, output
    = §5 client Finished record. This is the test for
    `tls13_transcriptum_hrr` (re-plant T1's 0xfe -> 0xff here: red).
  - `hrr_bis`: a second HRR -> PARAMETRUM, alert 47.
  - `hrr_grex_ignotus`: HRR asking for a group we did not offer
    (0x0018) -> PARAMETRUM.
  - `alerta_70`: plaintext alert `15 03 03 00 02 02 46` in answer to CH1
    -> SOLUM_1_2, no alert sent back.
  - `alerta_40`: `... 02 28` -> ALERTA_RECEPTA, causa contains
    "handshake_failure".
  - `keyupdate`: after the §3 handshake, feed a KeyUpdate(update_requested)
    record sealed with the s_ap secret -> engine rotates receive keys,
    queues a sealed KeyUpdate(update_not_requested) under the old c_ap,
    rotates send keys; a following server data record sealed under the
    NEW s_ap secret opens; our next data record opens with the NEW c_ap
    (all via T2's sealer with secrets derived in the test).
  - `recordum_moles`, `recordum_mac` at engine level -> MOLES / MAC with
    alerts 22 / 20.

- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement** in `_nuntius`: HRR branch (once; transcript
  created from the HRR's suite, CH1 added, `tls13_transcriptum_hrr`,
  HRR added; group must be one we offer and not the one we sent; P-256
  key: `_fors(32)` until `mulgen` returns 65; CH2 = fixed[1] or encoder
  with `grex = SECP256R1`, the P-256 public key and the cookie; CCS
  first if `sessio_non_vacua`; CH2 stored and added to the transcript
  when sent - NOT deferred, since the transcript exists now); P-256
  shared secret = x coordinate of `mul(peer65, priv)` at
  `impl->xoff(BR_EC_secp256r1, &len)`; KeyUpdate branch; the remaining
  alert names.
- [ ] **Step 4: Run - expect PASS** (whole suite).
- [ ] **Step 5: Plants:** HRR "once" guard removed -> hrr_bis red;
  KeyUpdate send-key rotation dropped -> keyupdate red; T1's 0xfe plant
  -> s5_hrr red.
- [ ] **Step 6: Commit** `tls13 T5: HelloRetryRequest (RFC 8448 §5, P-256), alertae nominatae, KeyUpdate`.

---

### Task 6: backend `tls_tls13.c` + `TLS_VECTURA_TLS13`

**Files:** Create `include/tls_tls13.h`, `lib/tls_tls13.c`,
`probationes/probatio_tls13_servus.c`; Modify `include/tls.h`
(`TLS_VECTURA_TLS13`, comment), `lib/tls.c` (branches, `RHUBARB_TLS=
tls13`), `oratio/glossarium.stml` (`tls13` if flagged),
`compile_tests_fontes_generata.sh`.

**Interfaces:**
- Consumes: T4/T5 engine API; `fiducia_iudicare`,
  `fiducia_folium_describere`.
- Produces: `TlsTls13`, `TlsTls13Resultus`, `tls_tls13_{connectere,
  incipere, gradus, gradus_error, mittere, recipere, claudere,
  est_valida, certificatum}` - the shape of `include/tls_bearssl.h`
  exactly (copy it, rename `bearssl` -> `tls13`).

- [ ] **Step 1: Failing test `probationes/probatio_tls13_servus.c`** -
  copy `probatio_tls_bearssl.c`'s fixture and helpers, with
  `opt.vectura = TLS_VECTURA_TLS13` / `RHUBARB_TLS=tls13`, servers
  started with `servum_tls_incipere_cum(..., "-tls1_3", ...)`. Sections:
  parvum (EC leaf), rsa (RSA leaf -> rsa_pss_rsae), the three suites
  each forced (`-tls1_3 -ciphersuites TLS_AES_128_GCM_SHA256`, `...
  TLS_AES_256_GCM_SHA384`, `... TLS_CHACHA20_POLY1305_SHA256`), hrr
  (`-tls1_3 -groups P-256` -> handshake succeeds through HRR), alienus /
  expiratus / ignotus / sine ancora (rejected, "certificatum reiectum"),
  gradus (direct stepped API + GET + read to 0), omissa (verificare
  FALSUM), clausa a pare (server reads ClientHello, closes ->
  named error), obstruens (blocking + certificatum: subject localhost,
  365 days), tempus (silent server -> TLS_ERROR_TCP), magnum (256 KB
  body; Review focus 5), eof (server closes after the response without
  close_notify - s_server `-WWW` closes after the body - read returns 0,
  never -1; Review focus 3).

- [ ] **Step 2: Run - expect FAIL** ("vectura" value unknown).
- [ ] **Step 3: Implement** `lib/tls_tls13.c` - the pump, modelled
  line by line on `lib/tls_bearssl.c`'s `_antlia`:

```c
interior Antlia
_antlia (TlsTls13* c, i32 sitis)
{
    dum (VERUM)
    {
        i32 status = tls13_machina_status(c->m);

        si (status & TLS13_RECORDA_MITTENDA)
        {
            i32  n;
            i8*  b = tls13_recorda_exitus(c->m, &n);
            s32  w = tcp_mittere(c->tcp, (constans i8*)b, n);

            si (w < ZEPHYRUM) { _errorem_ponere(c, "scriptio fossae fallita"); redde ANTLIA_ERROR; }
            si (w == ZEPHYRUM) redde ANTLIA_SCRIBERE;
            tls13_recorda_exitus_ack(c->m, (i32)w);
            perge;
        }
        si (tls13_machina_error(c->m).codex != TLS13_ERROR_NULLUS)
            redde ANTLIA_ERROR;
        si (status & sitis) redde ANTLIA_PARATA;
        si (status & TLS13_CLAUSA) redde ANTLIA_CLAUSA;
        si (status & TLS13_RECORDA_EXSPECTAT)
        {
            i32  n;
            i8*  b = tls13_recorda_introitus(c->m, &n);
            s32  r = tcp_recipere(c->tcp, b, n);

            si (r == TCP_ITERUM) redde ANTLIA_LEGERE;
            si (r == ZEPHYRUM)
            {
                si (c->pacta) redde ANTLIA_CLAUSA;
                _errorem_ponere(c, "connexio a pare clausa in pactione");
                redde ANTLIA_ERROR;
            }
            si (r < ZEPHYRUM) { _errorem_ponere(c, "lectio fossae fallita"); redde ANTLIA_ERROR; }
            tls13_recorda_introitus_ack(c->m, (i32)r);
            perge;
        }
        _errorem_ponere(c, "machina TLS 1.3 immota");
        redde ANTLIA_ERROR;
    }
}
```

  `sitis` = `TLS13_DATA_MITTI_POSSUNT` for the handshake and sending,
  `TLS13_DATA_ADSUNT` for receiving. Error mapping per spec II.4 (table)
  in `_categoria(codex)`: FIDUCIA, SUBSCRIPTIO -> CERTIFICATUM; others ->
  HANDSHAKE; causa = engine causa (FIDUCIA: fiducia's reason). The trust
  callback:

```c
interior b32
_fiducia (vacuum* datum, constans FiduciaCertificatum* catena, i32 n,
    constans character* hospes, chorda* causa, Piscina* p)
{
    TlsTls13* c = (TlsTls13*)datum;

    redde fiducia_iudicare(catena, n, hospes, c->ancora_via, causa, p);
}
```

  The rest (`_creare`, incipere, connectere with the same blocking
  mapping, gradus, mittere with `pendens`, recipere, claudere,
  certificatum via `tls13_machina_catena` + `fiducia_folium_describere`)
  follows `lib/tls_bearssl.c` function for function - the shapes are
  identical by design; written out in full in the implementation step.
  `tls.c`: `TLS_VECTURA_TLS13` branches beside the BEARSSL ones; struct
  field `TlsTls13* tls13`; `tls_vecturam_eligere` accepts "tls13". The
  house default stays BEARSSL until T8.

- [ ] **Step 4: Run - expect PASS** (probatio_tls13_servus all
  sections; probatio_tls_vectura, tls_servus, tls_gradus still green).
- [ ] **Step 5: Plants:** `_fiducia` returns VERUM always -> alienus...
  red; pump's "SENDREC first" moved after the read branch -> gradus /
  parvum red (ClientHello never sent before reading); `c->pacta` EOF
  rule inverted -> clausa a pare or eof red.
- [ ] **Step 6: Commit** `tls_tls13 T6: vectura TLS 1.3 (antlia, fiducia per reditum) - s_server -tls1_3: tres cifrae, HRR P-256, EC/RSA, reiectiones`.

---

### Task 7: `tcp_redintegrare`, fallback, sentinel, `sine_descensu`, `tls_versio`

**Files:** Modify `include/tcp.h`, `lib/tcp_posix.c`, `include/tls.h`,
`lib/tls.c`, `lib/tls_bearssl.c`, `include/tls_bearssl.h`; Create
`probationes/probatio_tcp_redintegrare.c`,
`probationes/probatio_tls13_descensus.c`.

**Interfaces:**
- Produces:

```c
/* tcp.h - eundem parem (getpeername), fossam novam, connexionem
 * incipit, dup2 in fossam VETEREM (numerus idem: registrationes
 * reactoris vocantium manent). optiones NIHIL: non-blocans (exspecta
 * scribibilem, deinde tcp_incipere_perficere); aliter connexio cum
 * mora, deinde obstruens cum moris fossae (P9). */
TcpResultus tcp_redintegrare (TcpConnexio* connexio,
    constans TcpOptiones* optiones, Piscina* piscina);
/* tls.h */
TlsOptiones: b32 sine_descensu;          /* 1.3 aut nihil */
             b32 descensus_coactus;      /* PROBATIONES SOLUM (P5) */
i32 tls_versio (TlsConnexio* connexio);  /* 0x0304 / 0x0303; 0 ignota */
/* tls_bearssl.h */
TlsBearsslResultus tls_bearssl_incipere_signo (TcpConnexio* tcp,
    constans character* hospes, constans TlsOptiones* optiones,
    Piscina* piscina);   /* ut incipere + inspectio DOWNGRD post pactionem */
```

- [ ] **Step 1: Failing tests.**
  - `probatio_tcp_redintegrare.c`: a tcp_servus on 127.0.0.1;
    `tcp_incipere` + perficere -> fd F registered in a reactor (read
    callback counting); `tcp_redintegrare(conn, NIHIL)` -> same
    `tcp_obtinere_fd` = F, server `accept`s a SECOND connection, data
    the server writes on the second connection fires the reactor's
    callback registered for F (the dup2 property, spec AUDIENDA); the
    first server-side connection sees EOF. Blocking variant with
    options: connected, `tcp_recipere` honours `timeout_ms`.
  - `probatio_tls13_descensus.c` (servus_tls fixture):
    - `descensus`: `s_server -tls1_2` + `vectura TLS13` -> success,
      `tls_vectura` = BEARSSL, `tls_versio` = 0x0303, GET works (stepped
      via http_vocare with RHUBARB_TLS=tls13, and blocking).
    - `sine_descensu`: same server, `sine_descensu` VERUM -> FALSUM,
      HANDSHAKE, "solum TLS 1.2".
    - `signum`: ordinary s_server (1.3-capable) + `descensus_coactus` ->
      the 1.3 attempt is skipped as if the server had answered 1.2 ->
      BearSSL path sees DOWNGRD\x01 -> FALSUM, HANDSHAKE, "descensus
      vetitus" (P5 - the real sentinel).
    - `nulla_descensio`: a plain test tcp_servus that counts accepts and
      answers any ClientHello with the plaintext alert `15 03 03 00 02 02
      2a` (bad_certificate) -> TLS_ERROR_HANDSHAKE, causa names
      "bad_certificate", and the server counted exactly ONE accept (only
      SOLUM_1_2 falls back; spec II.4).
- [ ] **Step 2: Run - expect FAIL.**
- [ ] **Step 3: Implement.** `tcp_redintegrare` in `lib/tcp_posix.c`:

```c
TcpResultus
tcp_redintegrare (
            TcpConnexio* connexio,
    constans TcpOptiones* optiones,
               Piscina* piscina)
{
    structura sockaddr_storage  par;
                     socklen_t  mensura = (socklen_t)magnitudo(par);
                       integer  nova;
                       integer  r;

    si (!connexio || connexio->fd < 0
        || getpeername(connexio->fd, (structura sockaddr*)&par, &mensura)
           != 0)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, "par ignotus", piscina);
    }
    nova = socket(((structura sockaddr*)&par)->sa_family, SOCK_STREAM,
        IPPROTO_TCP);
    si (nova == -1)
    {
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(errno), piscina);
    }
    (vacuum)_ponere_non_blocans_fd(nova, VERUM);
    _ponere_nosigpipe(nova);
    r = connect(nova, (structura sockaddr*)&par, mensura);
    si (r == -1 && errno != EINPROGRESS && errno != EINTR)
    {
        integer e = errno;

        close(nova);
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(e), piscina);
    }
    si (optiones != NIHIL)
    {
        /* obstruens: exspecta usque ad timeout_ms, SO_ERROR, deinde
         * modus obstruens cum moris (ut tcp_connectere_cum_optionibus) */
        si (!_connexionem_exspectare(nova, optiones->timeout_ms))
        {
            close(nova);
            redde _creare_error(TCP_ERROR_TIMEOUT, "Tempus connexionis excessum",
                piscina);
        }
        (vacuum)_ponere_non_blocans_fd(nova, FALSUM);
        _applicare_optiones(nova, optiones);
    }
    si (dup2(nova, connexio->fd) == -1)
    {
        integer e = errno;

        close(nova);
        redde _creare_error(TCP_ERROR_CONNEXIO, strerror(e), piscina);
    }
    close(nova);
    connexio->clausa = FALSUM;
    redde _successus(connexio);
}
```

  (`_connexionem_exspectare` = poll POLLOUT + SO_ERROR, factored out of
  `tcp_connectere_cum_optionibus`'s existing wait if it has one, else
  written here; `_successus` = the house TcpResultus success
  constructor - name per the file.)

  `tls.c`: in the TLS13 branches, on `SOLUM_1_2` (or immediately if
  `descensus_coactus`): unless `sine_descensu`, `tcp_redintegrare`
  (stepped: NIHIL, then `tls_gradus` returns SCRIBERE until
  `tcp_incipere_perficere` succeeds; blocking: the caller's options),
  then `tls_bearssl_incipere_signo` (stepped) or its blocking twin, and
  `vectura = BEARSSL`. `tls_versio`: tls13 backend ->
  `tls13_machina_versio`; bearssl -> 0x0303 after handshake;
  securetransport -> `SSLGetNegotiatedProtocolVersion` mapped.
  `tls_bearssl.c`: a `signum` flag; after the handshake reaches SENDAPP,
  if `signum`, compare `c->cc.eng.server_random + 24` with
  `"DOWNGRD"` and byte 31 in {0,1} -> `_errorem_ponere(c, "descensus
  vetitus: hospes TLS 1.3 novit")`, ERROR.
- [ ] **Step 4: Run - expect PASS.**
- [ ] **Step 5: Plants:** sentinel compare removed -> signum red;
  `dup2` skipped (use `nova` in place, fd changes) -> redintegrare red
  (fd differs / reactor never fires); fallback on ANY error -> 
  nulla_descensio red (two accepts).
- [ ] **Step 6: Commit** `tls T7: descensus 1.3 -> 1.2 transparens (tcp_redintegrare, dup2), signum DOWNGRD inspectum, sine_descensu, tls_versio`.

---

### Task 8: three-backend matrix, leaks, default -> 1.3, network, live

**Files:** Modify `probationes/probatio_tls_servus.c`,
`probationes/probatio_tls_gradus.c`, `probationes/probatio_tls_vectura.c`,
`lib/tls.c`, `include/tls.h` (comment), `lib/tls13.worklog.md`,
`project-specs/tls13-spec.md` (status line, after Fran's check).

- [ ] **Step 1: Matrix to three** - both suites' principale loops over
  `{"tls13", "bearssl", "securetransport"}` (RHUBARB_TLS); sections
  served by s_server default (1.3-capable) - under tls13 they negotiate
  1.3, under bearssl 1.2. The `tls11` section: tls13 refuses too (it
  offers 1.3 only, falls back to BearSSL, which refuses 1.1).
- [ ] **Step 2: Default flip (test first):** probatio_tls_vectura
  VECTURA_DOMUS -> `TLS_VECTURA_TLS13`, seen red; `lib/tls.c`
  `TLS_VECTURA_DOMUS` -> `TLS_VECTURA_TLS13`; green. tls.h comment: TLS
  1.3 ordinaria, BearSSL 1.2 ut descensus, SecureTransport vetus.
- [ ] **Step 3: Leaks:** `./tools/tls_effluvia.sh -aedificare` - residuum
  0 under all three.
- [ ] **Step 4: Network (by hand):** `./tools/rete_fumus.sh -tls -badssl`
  under each backend; expected: howsmyssl "TLS 1.3" under tls13 (1.2
  under the others), identical badssl verdicts (tls-v1-2.badssl.com:1012
  is 1.2-only -> accepted via fallback under tls13); probatio_tls,
  probatio_http_vocatio_rete green under all three.
- [ ] **Step 5: Fran's live check:** vicus `$question` with the new
  default; once with `RHUBARB_TLS=bearssl`.
- [ ] **Step 6: Close out:** spec status line, ledger note on …BWQS8H,
  worklog, memory; commit
  `tls13 T8: TLS 1.3 ORDINARIA (descensus BearSSL cum signo), matrix III vecturarum, effluvia, rete`.
  No merge to main without Fran's go (and `fusio_probare`).
