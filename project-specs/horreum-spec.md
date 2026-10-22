# horreum - an S3-compatible object storage client (spec)

**Status: IMPLETUM 2026-10-22** - built in quarta per
`project-specs/horreum-plan.md` (six tasks, Fran approving each): T1
5a0f3e61 (MD5 + SigV4), T2 b7006108 (pure core), T3 780e65c6 (blocking
layer + fake S3), T4 46e22e1b (multipart + file helpers), T5 1fbaed79
(reactor layer), T6 (`bin/horreum`, `horreum probare`, capabilities
probed live on DO Spaces and R2, real response fixtures) ef203a40;
after-dogfooding addendum T7 (observer hook, II.5 end). Not yet in
main. Spec written 2026-10-21 and approved in conversation section by
section (units + names, API scope, provider profiles, errors + retries,
tests). Follow-on: `inventarium` (the
catalog, II.7) gets its own spec after this one is built.
Ledger: desideratum ...GWM (`01KY0P61RX7JFR91110PHRVGWM`, filed
2026-07-20, "bibliotheca s3-compatibilis"); the catalog is desideratum
...5E8 (`01M5FFT865VVE362RK29V1K5E8`). **Naming open**: `inventarium`
is already a tabularium genus (batch-job inventories), so the catalog
likely needs another name (e.g. `repertorium`) - decided in its spec.

*Fran 2026-10-21, after canalis went live: "one thing that i am
thinking of us building next is a general purpose library for s3
compatible APIs, so like Digital Ocean, Backblaze, Cloudflare, etc." -
then "i think A & B & C would all be realistic applications. There
would also be something at some point that might allow volumens to by
synced to s3 as an immutable log. But we don't have to worry
necessarily about implementing any of these use cases, i think we
would just try to make a solid general purpose s3 library" - and, on
usage tracking: "i know in google cloud wasn't the recommendation that
you sort of set it up yourself to sync metadata/stats to a database".*

Goal: **one C89 client for any S3-compatible store**, complete enough
that its four foreseen consumers fit without reshaping the interface:

- (A) off-site backups (volumens, annales, `~/.rhubarb`): large
  uploads, listing, pruning;
- (B) app file storage (receipts, images in exemplar-family apps):
  presigned URLs so browsers upload/download directly, metadata;
- (C) public assets: PUT with public-read where the provider allows;
- (D) volumens synced as an immutable log: **conditional create**
  (`If-None-Match: *`) that is proven, not assumed.

None of the four is built here. Providers first: DigitalOcean Spaces
and Cloudflare R2 (Fran has DO, will set up R2); Backblaze B2 later.

Decisions (Fran, 2026-10-21, in conversation):
- **One API, provider differences as data** (II.3): a profile per
  provider with capability flags; a call needing a capability the
  profile lacks fails BEFORE sending.
- **Capabilities are proven live** (II.3): each built-in flag is
  `probatum` (a live probe passed, dated in source) or `creditum`
  (provider docs say so). Conditional create is relied on only when
  `probatum`, or when the caller explicitly accepts the risk.
- **Layered I/O, approach A** (II.1): a pure core (sign, build,
  parse - no I/O), a blocking layer on `http_vectura_exsequi`, a
  reactor layer on `http_vocare`. Rejected: blocking only (server
  consumers would block the reactor; async added later reshapes the
  API) and reactor only (callback ceremony for every script).
- **No usage API in S3, so a catalog is ours** (II.7): S3 has no
  "bucket size" call; provider metrics (CloudWatch, R2 GraphQL) lag,
  are coarse and per-provider. Every horreum operation reports the
  object's facts; the catalog `inventarium` (later spec) keeps them
  in a volumen - the caller's volumen, `~/.rhubarb` being the machine
  default for house tools - and reconciles against listings.
- **Names**: `horreum` (the Roman warehouse, horrea of Ostia),
  `inventarium` for the catalog; SigV4 in its own library
  `signatura_aws`.
- **MD5 joins `friatio`** (II.5): DeleteObjects needs `Content-MD5`;
  a single-part ETag is usually the body's MD5.


## I. What exists (quarta 28122213, 2026-10-21)

- Hashes: `include/friatio.h` - SHA-1 (78), SHA-256 (128-153), CRC32
  (169-191). **No MD5.**
- HMAC: `include/sigillum.h:72` `sigillum_hmac` - HMAC-SHA256, any key
  length; its comment already names "subscriptio petitionum AWS
  SigV4". `sigillum_hex` (64) gives lowercase hex, which SigV4 wants.
- HTTP, blocking: `include/http.h:286` `http_exsequi`; `:324`
  `http_exsequi_rivus` (2xx body streamed to an `HttpRivus` (`:90`) as
  it arrives). **Transport seam**: `HttpVectura` / `HttpVecturaRivi`
  (`:310-360`) - a function + datum that executes a petitio; tests
  insert a scripted one (herbarium does).
- HTTP, non-blocking: `include/http_vocatio.h:69` `http_vocare` on the
  reactor; streamed 2xx body, connection + idle timeouts, finish once.
- **Request bodies are whole buffers** (`http_petitio_corpus_ponere`,
  `http.h:233`): no streaming upload. Multipart bounds memory instead
  (II.4) - the HTTP library is not touched.
- Methods: `HTTP_PUT`, `HTTP_DELETE`, `HTTP_HEAD` exist (`http.h:33-36`).
- URL: `include/url.h:47` `url_codificare` - unreserved
  `A-Za-z0-9-_.~` kept, rest `%XX` uppercase (`lib/url.c:18`) - the
  SigV4 query rule. Path encoding (keep `/`) is not there yet.
- Time: `include/fasti.h:428` `fasti_ad_iso` gives
  `YYYY-MM-DDTHH:MM:SSZ`; SigV4 wants the basic form
  `YYYYMMDDTHHMMSSZ` - a few lines over `fasti_ex_unix`.
- XML: `include/xml.h` - `xml_legere` (101), `xml_invenire_liberum`
  (124), `xml_invenire_omnes_liberos` (133), `xml_textus_internus`
  (159); writer `xml_elementum_creare` (187) / `xml_scribere` (270).
- Base64: `include/base64.h` (Content-MD5 is base64 of the digest).
- Key handling pattern: `include/oceanus.h:9-13` (DO DNS client) - key
  file in `~/.rhubarb/`, mode 0600 enforced, read RAW (never
  lectiones - fabrica would seal the content), zeroed on close, never
  in an error, a log or a retry message.


## II. Design

### II.1 Units

| unit | files | knows | tests |
|---|---|---|---|
| MD5 | `include/friatio.h`, `lib/friatio.c` | RFC 1321 | RFC 1321 vectors |
| `signatura_aws` | `include/signatura_aws.h`, `lib/signatura_aws.c` | SigV4 only, nothing of S3 | AWS signing test suite, byte-exact |
| `horreum` core | `include/horreum.h`, `lib/horreum.c` | profiles, request building, response parsing, presigned URLs - **no I/O** | built petitiones, recorded responses |
| blocking layer | `include/horreum_cliens.h`, `lib/horreum_cliens.c` | retries, file helpers, on `HttpVectura` | scripted vectura (fake S3) |
| reactor layer | `include/horreum_vocatio.h`, `lib/horreum_vocatio.c` | same operations on `http_vocare` | fake S3 on loopback, on the reactor |
| CLI | `tools/horreum.c` -> `bin/horreum` | put/get/ls/rm/presign/probare | live, by hand |

`signatura_aws` is separate because other AWS-shaped APIs (SES mail,
for one) sign the same way. The signer takes the TIME as an argument -
pure and testable; only the I/O layers read the clock.

### II.2 `signatura_aws` - SigV4

Input: method, host, canonical URI, query pairs, headers, payload hash
(hex SHA-256 or `UNSIGNED-PAYLOAD`), time (unix s), region, service,
access key id, secret. Output, either:

- `Authorization: AWS4-HMAC-SHA256 Credential=.../SignedHeaders=...,
  Signature=...` plus the `x-amz-date` / `x-amz-content-sha256` it
  signed; or
- a presigned query (`X-Amz-Algorithm`, `-Credential`, `-Date`,
  `-Expires` <= 604800, `-SignedHeaders`, `-Signature`).

Canonical forms per the SigV4 rules: query sorted by encoded key then
value, header names lowercased + sorted + values trimmed and inner
spaces collapsed, path segments URI-encoded once for S3 (S3 does NOT
double-encode, unlike other services - a flag, `semel_codificare`).
The derived signing key (date/region/service/`aws4_request`) is
computed per call; zeroed after use. Intermediate strings (canonical
request, string to sign) are exposed for tests and for the error that
SignatureDoesNotMatch diagnostics need - never the key.

### II.3 Provider profiles and capabilities

```c
nomen structura {
    chorda titulus;          /* "digitalocean", "r2", "b2", "generic" */
    chorda hospes_forma;     /* "{regio}.digitaloceanspaces.com" */
    HorreumStilus stilus;    /* HOSPES (bucket.host) aut VIA (host/bucket) */
    chorda regio;            /* "nyc3", "auto", ... */
    i32    capacitates;      /* HORREUM_CAP_* bits */
    i32    probatae;         /* subset of capacitates proven live */
} HorreumProvisor;
```

- Built-ins: `digitalocean`, `r2` (account id fills the host; EU
  jurisdiction host variant), `b2`, `generic` (everything supplied).
  A profile file names a built-in and may override fields.
- Capability bits (v1): `ACL` (per-object canned ACL),
  `SOLUM_SI_ABEST` (`If-None-Match: *` on PUT), `SOLUM_SI_ETAG`
  (`If-Match` on PUT), `RETENTIO` (object lock headers - reserved, no
  operation uses it in v1), `CHECKSUM_CRC32`, `CHECKSUM_SHA256`,
  `DELERE_PLURA` (DeleteObjects).
- Next to each built-in flag in source: `probatum YYYY-MM-DD` (with
  the probe run) or `creditum` (doc URL). Starting state: all
  `creditum`; T6 promotes.
- A call needing a capability the profile lacks -> `HORREUM_INCAPAX`,
  nothing sent. `solum_si_abest` additionally requires the bit in
  `probatae` unless the call sets `periculum_accipere`.
- Address style: bucket-in-host default; path style for `generic`
  (MinIO, the test server). A bucket name that is not a valid DNS
  label (dots under TLS) -> path style if the profile allows, else
  `HORREUM_MALUM` naming why (no confusing certificate failure).
- **Checksum headers are sent ONLY when the profile lists the
  algorithm** - never AWS's 2025 always-CRC32 default, which broke
  third-party providers. Integrity in transit is the signed SHA-256.

### II.4 Operations (core builds + parses; both layers execute)

| function | S3 | options |
|---|---|---|
| `horreum_ponere` | PutObject | content type, cache-control, user metadata, ACL (if `ACL`), `solum_si_abest`, `solum_si_etag`, checksum |
| `horreum_capere` | GetObject | to buffer or `HttpRivus`; byte range; if-match / if-none-match / if-modified-since |
| `horreum_inspicere` | HeadObject | - |
| `horreum_delere` | DeleteObject | - |
| `horreum_delere_plura` | DeleteObjects | <= 1000 keys, Content-MD5, per-key results |
| `horreum_copiare` | CopyObject | keep or replace metadata |
| `horreum_enumerare` | ListObjectsV2 | prefix, delimiter (common prefixes), page size; returns a page + continuation token |
| `horreum_multipars_*` | Create / UploadPart / Complete / Abort / ListParts | - |
| `horreum_multipartes_pendentes` | ListMultipartUploads | prefix; for cleanup of abandoned uploads (billed, invisible to listing) |
| `horreum_praesignare` | (presigned URL) | GET or PUT, expiry <= 7 days, optional content type the browser must match; `UNSIGNED-PAYLOAD` |

Every option struct is valid when zeroed (the `HttpVocatioOptiones`
convention). Every operation touching an object fills a
`HorreumObjectum`: key, size, ETag, last-modified, checksum, version
id, content type, user metadata; fields the provider did not send stay
empty. In-memory bodies (single PUT, each part) are signed with their
real SHA-256.

Sketch (names final in the plan):

```c
nomen structura {
    chorda genus_contenti;        /* vacua = non mittitur */
    chorda cache_control;
    constans HorreumMeta* meta;  i32 meta_numerus;
    b32    solum_si_abest;        /* If-None-Match: * */
    chorda solum_si_etag;         /* If-Match */
    HorreumAcl acl;               /* ordinarius = non mittitur */
    b32    periculum_accipere;    /* solum_si_abest sine probatione */
} HorreumPonendi;

b32 horreum_ponere(Horreum* h, chorda situla, chorda clavis,
                   chorda corpus, constans HorreumPonendi* optiones,
                   HorreumObjectum* exitus, HorreumError* error,
                   Piscina* piscina);
```

File helpers (blocking layer):
- `horreum_ponere_ex_filo`: at most one part in memory. Below the part
  size: single PUT. Above: multipart with a **uniform part size**
  (default 16 MiB; S3 allows 5 MiB..5 GiB, <= 10000 parts; R2
  requires equal parts except the last). Each part read, hashed,
  signed, sent, dropped; a failed part retries alone; any final
  failure aborts the upload (no billed orphans left by us).
- `horreum_capere_in_filum`: stream to a temporary file in the target
  directory, check the size (and the MD5 against a single-part ETag
  when it has that form), then rename - never a half file at the
  destination.

Out of v1, room left in the option structs: object lock / retention
(bit reserved), tagging, version listing, ACL edits on existing
objects, browser POST-policy uploads, all bucket administration
(create, CORS, lifecycle - done once in the provider's dashboard).

### II.5 Errors and retries

```c
nomen enumeratio {
    HORREUM_BENE = 0,
    HORREUM_ABEST,         /* 404 NoSuchKey / NoSuchBucket / NoSuchUpload */
    HORREUM_CONDITIO,      /* 412 (aut 304): condicio non tenuit */
    HORREUM_VETITUM,       /* 403 AccessDenied, SignatureDoesNotMatch */
    HORREUM_INCAPAX,       /* provisor caret - ante missionem, nihil missum */
    HORREUM_TRANSITORIUM,  /* 5xx, 503 SlowDown, rete, tempus */
    HORREUM_MALUM          /* 400 ceterae: petitio nostra mala */
} HorreumErrorGenus;
```

`HorreumError` also carries the HTTP status, S3's code string, its
message and the request id (`x-amz-request-id`). Never a key, a
signature, or a presigned query.

Retries (policy in the core, executed by both layers):
- Transient errors on idempotent operations (GET, HEAD, unconditional
  PUT, DELETE, list, UploadPart): at most III attempts, exponential
  backoff with jitter, `Retry-After` honoured. Each attempt re-signed.
- `RequestTimeTooSkewed`: the server's `Date` header gives an offset;
  one retry with it; the offset is reported in the result.
- **Conditional create retried.** Attempt 1 may succeed on the server
  and lose its response; attempt 2 then gets 412 for our own write.
  So a conditional create stores our SHA-256 of the body in
  `x-amz-meta-sha256`; a 412 AFTER a retry triggers a HEAD and a
  compare - equal -> success flagged `iam_nostrum`; different -> real
  `HORREUM_CONDITIO`. Provider-independent (no reliance on ETag form).
  The same for a retried CompleteMultipartUpload that returns
  NoSuchUpload (HEAD, compare the expected ETag/size).
- DeleteObjects: per-key results, never all-or-nothing.

**Observation (T7, added 2026-10-22 after dogfooding).** horreum still
never writes anything itself. Both executors take an optional
`HorreumObservator` (`HorreumClientisOptiones` / `HorreumVocatioOptiones`
/ `HorreumProbandi`: `observator` + `observatoris_datum`), called after
EVERY attempt (retries and the iam_nostrum HEAD included) with the
signed request and the raw response, or the transport error. The core
`horreum_observationem_describere` turns one attempt into a readable
transcript: `Authorization` and `X-Amz-Security-Token` values are
`[TECTUM]`, binary bodies give only their size, text over IV KiB is cut.
`bin/horreum -vestigium VIA` appends that transcript for any command.
A trace is LOCAL debugging output: it keeps host names (R2's carries the
account id) and raw bodies - never commit one; fixtures go through
`horreum_responsum_purgare`.

### II.6 Credentials and profile files

- The library takes the key pair from the caller IN MEMORY
  (`HorreumClaves`); it never decides where keys live.
- Helper `horreum_profilum_legere(via, ...)`: one file per profile,
  e.g. `~/.rhubarb/horreum/<titulus>.clavis` (server apps: their data
  directory, beside `secretum`). Mode 0600 enforced (anything wider ->
  refusal naming `chmod 600`), read RAW per `oceanus` (examen 96
  `tolera` with cause), zeroed in `horreum_claudere`. Content: STML -
  provider, overrides (region, host, account id), default bucket,
  access key id, secret.
- Fran creates the buckets, keys and profile files; Claude never reads
  them. Recommended: one test bucket per provider, a key scoped to
  that bucket (DO per-bucket keys, R2 token scopes).

### II.7 The catalog `inventarium` (follow-on, NOT this spec)

Recorded here so horreum serves it: a library over horreum + volumen
that writes one row per put/delete (key, size, ETag/checksum, time,
plus the caller's meaning), answers usage queries locally, and
reconciles against a listing. Placement: the catalog takes a
`Volumen*` - an app's tables live in the app's own volumen (the
domain row and the catalog row commit in ONE transaction; one writer
per volumen); house tools use one machine catalog under `~/.rhubarb`.
A catalog owns a scope (bucket + prefix), so several share a bucket;
the bucket is truth, the catalog a rebuildable projection (as
`tabularium.jsonl` -> `.db`). What horreum owes it: facts from every
operation (II.4), the paged listing iterator, the pending-multipart
listing.

### II.8 Order of work (shape for the plan)

1. MD5 in `friatio` + `signatura_aws` (vectors).
2. `horreum` core: profiles, request building, response parsing,
   presigned URLs.
3. Blocking layer + fake S3 (scripted vectura) + retry semantics
   (incl. `iam_nostrum`).
4. Multipart + file helpers (uniform parts, abort on failure,
   pending-upload listing, download via temp + rename).
5. Reactor layer (fake S3 on loopback).
6. `bin/horreum` + `probare` + live DO and R2: capture fixtures,
   promote flags to `probatum`, close out.


## III. Tests

1. **MD5**: the RFC 1321 vectors (empty, "a", "abc", the alphabet,
   the 80-digit string) plus a streamed split equals one-shot.
2. **SigV4**: AWS's published signing test suite, byte-exact for the
   canonical request, string to sign and signature; plus the S3
   documentation's presigned-URL example (`examplebucket`, known
   signature) and its header-signed GET example. Vectors from a pinned
   clone (reference-clones convention), license checked first.
3. **Core**: per operation, the exact method, path, query and headers,
   per address style and profile; refusals (`INCAPAX`, unproven
   `solum_si_abest`, non-DNS bucket); parsing on **recorded real
   responses** from DO and R2 (list pages, common prefixes, multipart
   XML, each error code) committed as fixtures.
4. **Blocking layer** on a scripted `HttpVectura`: 503 / SlowDown with
   backoff and `Retry-After`; write-then-drop -> 412 on retry -> HEAD
   -> `iam_nostrum`; a real conflict stays a conflict; clock skew; one
   key failing in a batch delete; the file helpers (multipart path,
   abort on failure, temp + rename, size mismatch refused).
5. **Reactor layer**: a fake S3 on loopback driven by the reactor,
   the same scenarios as 4 that apply.
6. **Live, by hand** (network + real keys - not a commit gate):
   `horreum probare digitalocean` / `horreum probare r2` on a scratch
   prefix, cleaning up after itself: conditional create on an existing
   key -> 412; if-match with a wrong ETag -> 412; public-read ACL
   accepted or refused as claimed; each checksum algorithm; multipart
   limits; presigned GET and PUT round trips. Prints a capability
   report. Plus a smoke script in the shape of
   `tools/exemplar_mcp_fumus.sh`.

All suites green on macOS and Linux musl + glibc
(`./tools/linux/probatio.sh`).


## IV. Not in this spec

- `inventarium` (II.7) - its own spec, next.
- The consumers: backups, app storage, public assets, log sync.
- Provider features outside S3: the DO Spaces CDN, R2 public buckets
  / custom domains / Workers bindings / GraphQL usage metrics, the B2
  native API.
- Bucket administration; object lock retention (bit reserved);
  tagging; versions; POST-policy uploads.
- Streaming request bodies in `http` (multipart makes it unneeded).


## AUDIENDA (not verified)

**Answered 2026-10-22 (T6, `horreum probare` live on DO Spaces nyc3 and
R2; bodies in `probationes/fixa/horreum/{digitalocean,r2}/`):**

- Capability matrix: DO - `If-None-Match: *` yes, per-object
  `public-read` ACL yes (read back unsigned), DeleteObjects with
  Content-MD5 yes; `If-Match` on PUT NO (412 even with the right ETag;
  If-Match on GET works), checksum headers IGNORED (a wrong value is
  accepted). R2 - `If-None-Match: *`, `If-Match`, DeleteObjects,
  CRC32 and SHA-256 checksums all yes (wrong value -> BadDigest, value
  echoed); `x-amz-acl` accepted but without effect (unsigned GET 400:
  public access is per bucket). B2 not probed - stays `creditum`.
- R2 quirk not foreseen: ListMultipartUploads returns a DIFFERENT
  UploadId string than CreateMultipartUpload; both abort the upload.
  Match pending uploads by key, never by id.
- Request-id header: DO `x-amz-request-id`; R2 has none of the AWS
  ones - `cf-ray`.
- SigV4 vectors: aws-c-auth @84a26ea1, Apache-2.0
  (`probationes/fixa/signatura_aws/NOTICE`, T1); the signer encodes
  with its own `signatura_aws_codificare`, checked against them.

**Still open:**

- B2: every capability (never probed; `horreum probare` on a B2
  profile answers it).
- R2's "equal part sizes except the last" multipart rule - from
  memory; the uniform part size satisfies it either way (probe used
  equal parts only).
- Whether `http_exsequi` keeps connections alive across calls (cost
  of many small requests in a list walk) - not measured.
- 7-day presign maximum on DO and R2 - assumed equal to AWS (probe
  presigns for 300 s).
- The reactor layer (`horreum_vocatio`) cannot run on Linux until
  `nomina` has a POSIX implementation (parcum ...NPMN7).
