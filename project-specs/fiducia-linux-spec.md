# fiducia on Linux - certificate trust for outgoing TLS (spec)

**Status: IMPLETUM 2026-10-22** - built in quarta per
`project-specs/fiducia-linux-plan.md` (four tasks, Fran approving
each): T1 92af2494 (fiducia_der), T2 28d0e581 (house blocklist, both
platforms), T3 d04f3d2f (fiducia_posix on BearSSL), T4 (live from
Linux: DO, Cloudflare, badssl; horreum HEAD 404 against DO Spaces and
R2 run by Fran). Not yet in main. Approved 2026-10-22 (Fran: "that
looks good to me"); amended the same day by the plan's checked facts
(entry kinds, eMudhra already in OneCRL, CN fallback) - designed in
conversation section by section (trust store, override policy,
blocklist, units, verification policy, tests). Ledger: desideratum
...GCMMBX (`01M5JJRA2YWA89HRREY7GCMMBX`). Worktree quarta.

*Fran 2026-10-22, after nomina_posix (cf2e80be) made the reactor layer
run under Linux: "lets anticipate that we'll probably have to implement
the linux certificate verifier next" - then, on revocation, pointing at
Scott Helme's "Eight bits of trust: eMudhra's CA keys cracked in
milliseconds": "i wonder how hard it would be to implement revocation
checking?" - and "i think a house blocklist sounds like it would maybe
address this particular issue fairly well".*

Goal: **outgoing HTTPS from Linux.** Today `lib/fiducia_posix.c:28`
refuses every chain ("certificatum reiectum: fiducia POSIX nondum"),
so no client built on `tls.h` - horreum (both layers), vates,
anything on the droplet - can reach a real server from Linux. This
spec replaces that one decision. The TLS stacks and the `fiducia.h`
API do not change.

Decisions (Fran, 2026-10-22, in conversation):

- **Trust store = the distribution's CA bundle** (II.2), from a fixed
  list of known paths; none found -> every chain refused, the paths
  named. Root additions and removals arrive with OS updates, the way
  distrusted roots are pulled - the same "answers match the rest of
  the machine" rule as nomina_posix. Rejected: our own pinned Mozilla
  bundle (we would own every update; a distrusted root stays trusted
  until we regenerate) and system-then-pinned fallback (two sources
  of truth; the silent fallback hides a broken host).
- **No environment override** (II.2): `SSL_CERT_FILE` is NOT honoured.
  Anything that sets our environment (a unit file, a wrapper) could
  swap the whole trust store invisibly. The existing per-connection
  `ancora_via` (one trusted DER, chosen in code) covers tests and
  pinning; an explicit API bundle path (`TlsOptiones`) is the upgrade
  path if a private CA ever needs it.
- **A house blocklist, compiled in** (II.1, II.3): SPKI SHA-256 and
  issuer+serial entries checked against EVERY certificate of the
  chain, on BOTH platforms, generated from a pinned OneCRL snapshot
  plus a hand list. Rejected: a data file read at run time (another
  trust-critical file to guard on the host, plus a missing-file mode)
  and fetching from Mozilla at run time (network and a second trust
  problem inside the verifier).
- **Classic revocation (CRL / OCSP) declined, with reasons** (IV) -
  so the question is not reopened by accident. **Certificate
  Transparency parked** as its own follow-on.

## I. What exists (quarta cf2e80be, 2026-10-22)

- `include/fiducia.h` - `fiducia_iudicare(catena, numerus, hospes,
  ancora_via, &causa, piscina)` and `fiducia_folium_describere`. The
  header already sketches `fiducia_linux.c (br_x509_minimal +
  fasciculus distributionis)`.
- Callers hand over the server's chain as DER after collecting it:
  `lib/tls_bearssl.c:189` (end_chain of its own X.509 engine; a
  refusal becomes `BR_ERR_X509_NOT_TRUSTED` keeping fiducia's reason)
  and `lib/tls_tls13.c:60`. So fiducia is a synchronous function of
  (chain, host, anchor) - no TLS state.
- `lib/fiducia_macos.m` - SecTrust with an SSL policy carrying the
  host; `ancora_via` -> that DER only (`SecTrustSetAnchorCertificatesOnly`).
- `lib/fiducia_posix.c` - the refusing placeholder (49 lines).
- BearSSL vendored (`vendor/bearssl_amalgama.{c,h}`):
  `br_x509_minimal` (`:9652` context, `:9675` init, `:9790`
  `set_time`, `:9833` `set_minrsa`, `:9852` name elements),
  `br_x509_decoder` (`:9890` notbefore/notafter days+seconds),
  trust anchors (`:9128`, `BR_X509_TA_CA :9137`). Name-element SAN
  extraction supports `rfc822Name`, `dNSName`, URI and `otherName`
  only (`:9449-9459`) - **not `iPAddress`**.
- `probationes/probatio_fiducia.c` - 8 sections (anchor EC + RSA, name
  and IP; foreign host; expired; unknown CA; system store without the
  test CA; non-DER; missing anchor; leaf summary) against certificates
  GENERATED at test time by `probationes/fixa/tls/generare.sh`
  (openssl; nothing secret committed). `probatio_tls_servus` runs
  both stacks against `openssl s_server`.
- Linux images (`tools/linux/Dockerfile.{musl,glibc}`): Alpine 3.20
  (gcc, musl-dev, bash, curl), Ubuntu 22.04 (gcc, libc6-dev, curl,
  ca-certificates). Neither has the `openssl` CLI.
- aedilis picks `lib/<x>_<varians>` by convention and a header's
  `<aedilis corpus>` adds a portable `lib/<x>.c` (nomina, pseudoterminale).

## II. Design

### II.1 Units

| unit | role |
|---|---|
| `include/fiducia.h` | API unchanged; comment updated (Linux implementation real, blocklist). |
| `lib/fiducia_diffisa.c` (new, portable; corpus of `fiducia_diffisa.h`) | the blocklist check; called first by both platform files. (Not `lib/fiducia.c`: silex resolves `fiducia.h` to `lib/fiducia.c` BEFORE `_macos.m`, so that name would shadow the platform files - found in T2.) |
| `include/fiducia_diffisa.h` (new) | entry type + `fiducia_diffisam_invenire` / `fiducia_diffisas_iudicare` taking the TABLE AS A PARAMETER (test seam). |
| `include/fiducia_diffisa_tabula.h` (new) | the built-in table's externs and snapshot date (one header, one implementation). |
| `lib/fiducia_diffisa_tabula.c` | GENERATED table (`GENERATUM` line 1; fabrica generator action; generata gate). |
| `tools/fiducia_diffisa_generare.c` | generator: pinned OneCRL snapshot + hand list -> table. |
| `probationes/fixa/fiducia/onecrl/<YYYY-MM-DD>/` | pinned snapshot + `SHA256SUMS`. |
| `probationes/fixa/fiducia/diffisa.stml` | hand list: entries OneCRL lacks or we add ourselves; each with reason, date, source link. |
| `lib/fiducia_posix.c` | REWRITTEN: the Linux verifier (aedilis variant `posix`, so the name stays). |
| `lib/fiducia_macos.m` | gains the blocklist call before SecTrust; nothing else. |

Entry kinds - Mozilla's two, exactly (checked against a real snapshot
2026-10-22, plan rulings):

- **subject + key** - the subject DN (DER) AND the SHA-256 of the
  SubjectPublicKeyInfo must both match: blocks every certificate of
  that CA key, cross-signs included (a cross-sign keeps subject and
  key) - and, because the bad intermediate stays in the chain, every
  leaf minted under it;
- **issuer + serial** - the issuer DN (DER) and the serial's INTEGER
  octets: blocks one certificate.

### II.2 Trust store

- Known paths, in order, first existing regular file wins:
  `/etc/ssl/certs/ca-certificates.crt` (Debian, Ubuntu, Alpine with
  ca-certificates), `/etc/pki/tls/certs/ca-bundle.crt` and
  `/etc/pki/ca-trust/extracted/pem/tls-ca-bundle.pem` (Fedora, RHEL),
  `/etc/ssl/ca-bundle.pem` (openSUSE), `/etc/ssl/cert.pem` (Alpine's
  own). None -> refused: "fasciculus certificatorum systematis non
  inventus (<paths>)".
- Read via `filum` (public data; examen code 96). Each PEM
  `CERTIFICATE` -> `br_x509_decoder` -> trust anchor (DN + public
  key, CA flag). Undecodable entries are skipped and counted, never
  fatal. An anchor whose key is on the blocklist is DROPPED at load:
  servers usually omit the root, so a blocked root would otherwise
  never appear in the chain the blocklist sees. (macOS: SecTrust
  chooses the anchor, so there the check covers the chain as sent -
  Apple's own distrust data covers its roots.)
- Loaded ONCE per process into its own piscina; reloaded when the
  file's size or mtime changes. Why: horreum opens a connection per
  multipart part (no keep-alive), and re-parsing ~150 roots per
  connection adds up.
- `ancora_via` given -> ONLY that DER is an anchor; the bundle is not
  consulted (as macOS).
- No `SSL_CERT_FILE`, no other override (decision above).

### II.3 Judging a chain (fiducia_posix.c)

Order: arguments -> blocklist (`lib/fiducia_diffisa.c`) -> BearSSL validation
-> IP SAN check when the host is an IP literal.

- **Blocklist first**, so a blocked key gets its own reason: "clavis
  diffisa: <entry reason>, <date>, <source>" naming the chain
  position, not a generic failure.
- **BearSSL** `br_x509_minimal` driven directly: start_chain(host or
  NULL for an IP), each certificate in order, end_chain -> error code.
  The chain is judged as sent; missing intermediates are not fetched.
  Time = system clock.
- **Algorithms**: RSA with a minimum of 2048 bits (`set_minrsa(256)`),
  ECDSA P-256/P-384/P-521; signature hashes SHA-256/384/512 only -
  SHA-1 (and MD5) not registered, so SHA-1-signed certificates are
  refused; root self-signatures are never checked, so SHA-1 roots
  still anchor. Ed25519 certificates unsupported (BearSSL) - refused,
  named.
- **Host names**: DNS hosts via BearSSL's own SAN `dNSName` matching
  (left-most wildcard). IP hosts: BearSSL cannot extract `iPAddress`
  SANs (I.), so the leaf's SAN extension (OID 2.5.29.17) is walked in
  DER by us and the `[7]` entries compared byte-wise (4 or 16
  octets). Whether BearSSL falls back to the CN without a SAN is
  answered: it does (`:8955`), so fiducia_posix refuses a leaf with no
  SAN extension itself - the policy is SAN only.
- **Reasons**: every refusal is "certificatum reiectum: <ratio>"
  with the chain position; BearSSL codes mapped to Latin (auctor non
  fide dignus, expiratum / nondum validum, nomen hospitis non
  congruit, extensio critica ignota, algorithmus non sustentus,
  catena mala), unknown codes by number. Certificates are public; no
  reason carries anything else.
- **`fiducia_folium_describere`**: notBefore/notAfter from the
  decoder (days + seconds since year 0 -> Unix), subject CN from the
  DN (name element `03 55 04 03`).
- Pure CPU after the one bundle read.

### II.4 Blocklist sources and freshness

- Generator inputs: the pinned OneCRL snapshot (date in the directory
  name, `SHA256SUMS` checked) and `diffisa.stml`. Output order is
  stable (sorted by kind then bytes) so regeneration is byte-for-byte.
- The eMudhra case is ALREADY in OneCRL (bug 2083382, checked
  2026-10-22): six issuer+serial records (2026-10-17, under "emSign
  Root TLS CA - G3") and six subject+key records (2026-10-21, "EM
  DV/OV/EV TLS CA - G3B/G3C") - the latter catch the Sectigo
  cross-signs. The two code-signing CAs are not in OneCRL (TLS-only),
  irrelevant to TLS. So the hand list is born EMPTY; its mechanism is
  tested with a fixture list.
- Freshness is REPORTED, not gated: the table carries its snapshot
  date and a diagnostic prints it. A gate that turns red on unchanged
  code with the calendar is rejected; refreshing is a deliberate
  commit (snapshot, regenerate, commit, rebuild, redeploy).

## III. Tests

- `probatio_fiducia` (8 sections) runs unchanged under Linux musl +
  glibc; the images gain `openssl` (fixtures generated at test time)
  and Alpine `ca-certificates`.
- `probatio_tls_servus` under Linux: both stacks against
  `openssl s_server` - fiducia inside a real handshake.
- New blocklist sections, both platforms, with a TEST table passed to
  `fiducia_diffisam_invenire`: key entry blocks the leaf; it also
  blocks a second certificate made with the SAME key (the cross-sign
  case); issuer+serial blocks only that certificate; the refusal names
  the entry; the built-in table holds the eMudhra entries.
- New Linux sections: SHA-1-signed leaf refused; RSA-1024 refused; IP
  host matched against `iPAddress` and refused against a DNS-only SAN.
- generata gate: table == generator output.
- Plants: blocklist bypassed, SHA-1 registered, IP SAN check removed -
  each must turn its section red.
- By hand with network, from a Linux container, recorded in the
  worklog: HTTPS to DO and R2 succeeds; badssl.com expired,
  wrong.host, untrusted-root and a SHA-1 case each refused with the
  right reason; horreum read-only HEAD against DO and R2.
- Not tested: a host with no bundle (every image has one) - ledger
  note.

## IV. Not in this spec

- **CRL / OCSP - declined.** Size: a DER parser for CRLs and OCSP,
  an OCSP encoder, response signature checks, caching - and blocking
  HTTP fetches inside a TLS handshake inside an HTTP call. Weakness:
  clients soft-fail (an attacker in the middle blocks the check);
  against a compromised INTERMEDIATE, leaf OCSP is useless (the key
  holder signs "good" answers too); OCSP is being retired (Let's
  Encrypt ended it in 2025; Chrome never did online OCSP). The
  eMudhra case was stopped in browsers by pushed blocklists (CRLSet,
  OneCRL) and CT, not by revocation checks - and Linux distribution
  bundles carry roots only, never OneCRL, which is the gap II.3 fills.
- **Certificate Transparency - parked** (its own spec): SCT
  verification against a pinned log list. It is what protects in the
  window BEFORE a bad key is known; the blocklist only helps after.
- AIA fetching of missing intermediates; name constraints beyond what
  BearSSL enforces; client certificates (desideratum ...WM3G2R, mTLS).
- An explicit bundle path in `TlsOptiones` (upgrade path, not now).
- Windows (`fiducia_windows.c`).

## V. Order of work (the plan: `project-specs/fiducia-linux-plan.md`)

1. `fiducia_der`: certificate parts (issuer, serial, subject, SPKI)
   and IP SANs from DER - one portable reader for the blocklist and
   the IP check; new test fixtures; `openssl` in the Linux images.
2. The blocklist: pinned OneCRL snapshot + hand list -> generated
   table; `lib/fiducia_diffisa.c`; both platforms call it first.
3. `fiducia_posix.c`: bundle, BearSSL, SAN-only, IP SANs, reasons;
   both suites under Linux.
4. Live checks from Linux (DO, R2, badssl, horreum HEAD); docs.

## AUDIENDA (not verified)

**Answered 2026-10-22 while planning** (`fiducia-linux-plan.md`
rulings): OneCRL = Remote Settings changeset (1907 records: 1898
issuer+serial, 9 subject+pubKeyHash, 645 disabled; base64 DER); the
eMudhra TLS intermediates are in it (above); `br_x509_minimal` DOES
fall back to the subject CN without a SAN (`vendor/bearssl_amalgama.h:8955`),
so the SAN-only rule is enforced by fiducia_posix itself.

**Answered 2026-10-22 live (T4, `tools/linux/fiducia_vivum.sh`, both
libcs, both transports):** DO Spaces, cloudflare.com (the R2 chain:
GTS WE1 -> Root R4, ECDSA), badssl.com accepted; badssl expired,
wrong.host, untrusted-root (refused at chain [1]), self-signed and
sha1-intermediate (refused as EXPIRED - that intermediate has lapsed
too) refused with the matching reason; revoked.badssl.com ACCEPTED
(no revocation - IV, as designed). Found: the bare host
`r2.cloudflarestorage.com` serves a certificate that expired
2026-09-09 (refused, correctly; real R2 hosts are per-account).
horreum `inspicere` of a missing key from a glibc container: 404 from
DO Spaces and from R2 (Fran ran `tools/linux/horreum_vivum.sh`; the
profile mounted read-only, never read by the session).

**Still open:**
- That `br_x509_minimal` accepts a chain whose LAST certificate is the
  root itself (servers sometimes send it) and ignores extra
  certificates after the anchor-signed one.
- BearSSL's handling of a critical `nameConstraints` extension in an
  intermediate (refuse as unknown-critical, or enforce?).
- ~~Parse cost~~ answered T3: ~26-29 ms per full parse (Alpine 121,
  Ubuntu 123 anchors) -> cached; warm ~2 ms. Paths checked on Alpine
  3.20 and Ubuntu 22.04. The droplet's distribution is NOT yet checked
  (`cat /etc/os-release` there; a Debian/Ubuntu family droplet uses the
  first path).
- The article's claims (Sectigo AAA cross-signs; OneCRL and CRLSet
  blocks; NSS root removal) are as reported by Scott Helme,
  2026-10-20/21 - not checked against the Mozilla bug.
