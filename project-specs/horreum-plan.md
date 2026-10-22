# horreum plan - an S3-compatible object storage client

> **For agentic workers:** execution mode is fixed by Fran's standing rule:
> INLINE, one task per turn, Fran approves each task before the next; no
> implementer subagents (a review subagent at the end is allowed). Steps
> use checkbox (`- [ ]`) syntax for tracking.

**Goal:** one C89 client for any S3-compatible store (DigitalOcean
Spaces and Cloudflare R2 first, Backblaze B2 later): SigV4 signing, the
object / listing / multipart / presign operations, typed facts and
errors, provider differences as data with capabilities proven live.

**Architecture:** `signatura_aws` signs (pure, nothing of S3). The
`horreum` core is pure too: provider profiles, operation builders that
validate capabilities and describe a request (`HorreumOpus`), signing
an opus into an `HttpPetitio`, and parsers from `HttpResponsum` to
facts / pages / errors, plus the retry policy. Two thin executors run
any opus: blocking `horreum_exsequi` on `HttpVectura` (plus named
convenience calls and the file helpers) and reactor `horreum_vocare` on
`http_vocare`. `bin/horreum` is the CLI and the live probe.

**Tech stack:** C89 + latina.h; friatio (SHA-256, CRC32, new MD5),
sigillum (`sigillum_hmac`, `sigillum_hex`), http (`http_petitio_*`,
`http_petitio_visus`, `HttpVectura`, `http_responsum_caput`), http_vocatio
(`http_vocare`), reactor (timers), url (`url_codificare`), xml
(`xml_legere`, `xml_invenire_liberum`, `xml_textus_internus`), base64,
fasti (`fasti_ex_unix`, `fasti_ex_iso`), sors (jitter), filum (byte
reader, scriptor, `filum_movere`, `filum_delere`), tcp (`tcp_servus_*`)
for the loopback fake, json (vector contexts), credo v2; Linux via
`tools/linux/` (OrbStack).

**Spec:** `project-specs/horreum-spec.md` (quarta de76a149). Read it with
this plan.

## Global constraints

- House C89 via latina.h, flags of `tools/vexilla.sh`; identifiers,
  comments, messages Latin; never a latina.h macro as an identifier
  (`nomen`, `structura`, `magnitudo`, `per`, `casus`, any Roman numeral
  0-3999...); numerals above 3999 are NOT macros; `i32`/`i64` UNSIGNED,
  `s32`/`s64` signed; chorda not NUL-terminated.
- **`M` is 1000.** A MiB is `MXXIV * MXXIV` (http.h's `M * M` is a
  decimal MB). 604800 s (7 days) is `DCIV * M + DCCC`.
- POSIX .c files include `postulata_posix.h` FIRST; no raw
  `fopen/open/stat/access` outside filum (examen code 96) - the one
  exception is the profile read (P5), annotated like `lib/oceanus.c:54`.
- **No secret ever leaves its buffer:** the access key id is not secret,
  the secret key is. Never in an error, a log line, `imprimere`, a test
  message, a retry note, a fixture, or a commit. Derived signing keys
  are zeroed after each signature; `horreum_claudere` zeroes the struct.
- **Refuse before sending:** an operation needing a capability the
  profile lacks returns `HORREUM_INCAPAX` and builds nothing.
  `solum_si_abest` also needs the bit in `probatae` unless
  `periculum_accipere`.
- **Checksum headers only when the profile lists the algorithm.**
- **S3 paths are never normalized** (`/a/../b`, `//`, `./` go on the
  wire and into the canonical request as given, encoded once).
- Every option struct is valid when zeroed.
- Each library: `include/X.h` with `/* <aedilis corpus="lib/X.c"/> */`,
  `lib/X.c`, `lib/X.worklog.md`, `probationes/probatio_X.c` (credo v2:
  `CredoSectio` table + `credo_suitam_currere`, own piscina per section);
  after adding a suite run `./tools/compile_tests_fontes_generare.sh` and
  commit `compile_tests_fontes_generata.sh` WITH the task.
- New words through `VOCABULA_VIAE_ADDITAE="a.c b.h" ./oratio/vocabula.sh -nova`
  before commit; technical terms (etag, crc32, md5, sha256, acl) go to
  `oratio/glossarium.stml` as `ignotum-permissum`; never `imus`, never a
  real English word.
- Commits through `silva.commissio` (docs first, gates last); Linux
  (musl + glibc) green before a task is called done.
- Fran creates buckets, keys and profile files; Claude never reads a
  `.clavis` file (mode checks with `stat -f %Lp` only).

## Rulings made while planning

- **P1 - responses over 16 MiB fail in http, even streamed.**
  `HTTP_LECTOR_CORPUS_MAXIMUM` (`include/http.h:102`) is enforced in the
  lector (`lib/http.c:852`, "corpus nimium") because the whole body is
  always collected. So `horreum_capere` is for objects up to 16 MiB; the
  error for a larger one is `HORREUM_MALUM` naming
  `horreum_capere_in_filum`, which downloads by **ranged GETs of 8 MiB,
  each with `If-Match: <ETag>`** of the first HEAD (an object replaced
  mid-download -> 412 -> refused, never a spliced file). Cost if wrong:
  N requests per large download (the AWS transfer-manager shape anyway).
- **P2 - no keep-alive:** every request carries `Connection: close`
  (`lib/http.c:456`), so each S3 call is a fresh TCP + TLS handshake.
  Accepted for v1 (resolves spec AUDIENDA "keep-alive"); a keep-alive
  client is http work, not horreum's. Cost: latency on long list walks
  and many-part uploads.
- **P3 - horreum sets `Host` and an explicit `Content-Length: 0`.** http
  adds `Host` only when absent (`lib/http.c:414-432`), so horreum adds
  the exact signed value; http omits Content-Length for an empty body
  (`:448`), and S3 answers 411 to a body-less PUT/POST without it.
- **P4 - SigV4 vectors from aws-c-auth @ 84a26ea1 (2026-10-09,
  Apache-2.0)**, cloned at `../aws-c-auth`:
  `tests/aws-signing-test-suite/v4/` has 38 vectors (context.json,
  request.txt, header-/query- canonical request, string to sign,
  signature, signed request). Copied into
  `probationes/fixa/signatura_aws/v4/` with a NOTICE naming the source
  and commit. Expected exclusions (T1 confirms by running all 38): the 6
  `*-normalized` vectors whose path changes under normalization
  (`get-relative-normalized`, `get-relative-relative-normalized`,
  `get-slash-dot-slash-normalized`, `get-slash-normalized`,
  `get-slash-pointless-dot-normalized`, `get-slashes-normalized` - S3
  never normalizes) and the 2 session-token vectors
  (`post-sts-header-before`, `-after` - no consumer). The spec's
  `semel_codificare` flag is dropped: S3 is the only service; single
  encoding is all the remaining vectors need. The S3 documentation's
  worked examples could not be fetched (JavaScript page, 2026-10-21), so
  the S3-specific oracle is T6 live (any mismatch is 403
  SignatureDoesNotMatch); the fake server verifies signatures with the
  same signer (self-consistency, not an oracle).
- **P5 - the profile file is labelled lines, not STML** (spec II.6 said
  STML): `stml_legere` interns attribute values into a table and a
  piscina that cannot be zeroed; a hand parser over one fixed buffer
  can. Format (one `clavis valor` per line, `#` comments, unknown key
  -> refusal naming the key, never its value):
  ```
  provisor   r2                      # digitalocean | r2 | b2 | generic
  terminus   https://<account>.r2.cloudflarestorage.com   # optional for DO (regio)
  regio      auto                    # DO: nyc3, sfo3 ...
  situla     rhubarb-probatio        # default bucket
  clavis_id  <access key id>
  secretum   <secret access key>
  ```
  Read RAW (`open` O_NOFOLLOW, regular file, `(mode & 077) == 0` else
  refusal naming `chmod 600`) with
  `/* <tolera codex="CANALIS_PRAETERITUS" (>clavis secreta horrei */`
  like `lib/oceanus.c:54` and `lib/custodia.c:684`. This is the third
  copy of that read -> a ledger wish for `filum_secretum_legere`, not
  done here (filum.h's gate fan-out). The file NAME is the profile's
  title (`-profilum cloudflare` -> `~/.rhubarb/horreum/cloudflare.clavis`);
  the provider is the `provisor` line inside. Fran's current
  `cloudflare.clavis` (three bare lines: id, secret, endpoint) gets
  labels before T6 - positional lines are refused, so a misordered file
  can never send the secret as the key id.
- **P6 - builders + two generic executors.** Core builders
  (`horreum_opus_ponere`, ...) validate and fill a `HorreumOpus`; the
  blocking `horreum_exsequi` and reactor `horreum_vocare` run ANY opus
  with signing, retries and the `iam_nostrum` check; core parsers read
  the result. Named blocking calls (`horreum_ponere`, ...) are build +
  exsequi + parse. The spec's "same operations on http_vocare" is met
  by the same builders and parsers under `horreum_vocare`.
- **P7 - timestamps:** ListObjects `LastModified` carries milliseconds
  (`2026-10-21T14:09:00.000Z`) and `fasti_ex_iso` refuses fractions
  (`include/fasti.h:439-443`); horreum drops the fraction itself. HTTP
  `Date` / `Last-Modified` (IMF-fixdate `Tue, 21 Oct 2026 14:09:00 GMT`)
  are parsed by horreum. Both tested in T2.
- **P8 - the fake S3 is `probationes/horreum_simulacrum.h`:** one
  in-memory model (objects, multipart uploads, fault queue) with two
  doors - an `HttpVectura` (T3) and a loopback `tcp_servus` on the
  reactor (T5). `probationes/servus_scriptus.h` restarts its script on
  every connection and holds no state, so it cannot model S3.
- **P9 - clock, sleep and jitter are injected** (`HorreumClientisOptiones`:
  `nunc`, `dormire`, `semen`) so tests never sleep and retries are
  deterministic; the reactor layer takes `mora_basis_ms` (tests: 1).
- **P10 - conditional PUTs carry `x-amz-meta-sha256`** (hex of the body)
  for BOTH `solum_si_abest` and `solum_si_etag`: a 412 after a retry
  means HEAD + compare in either case (`iam_nostrum`).
- **P11 - CreateMultipartUpload is retried** on transient errors: a lost
  response can leave an orphan upload (billed, invisible); accepted -
  `horreum_multipartes_pendentes` + abort is the cleanup, and the file
  helper aborts what it created on failure.
- **P12 - part size:** default 16 MiB, never below 5 MiB, uniform except
  the last; when `ceil(size / part) > 10000` the part size rises to
  `ceil(size / 10000)` rounded up to a MiB (`horreum_partis_mensura`).
- **P13 - no streamed `horreum_capere` in v1** (spec II.4 "to buffer or
  `HttpRivus`"): the 16 MiB cap (P1) applies to streamed bodies too, so a
  rivus variant buys nothing a buffer does not; large objects go through
  `horreum_capere_in_filum`, and the reactor layer hands back the
  `HttpResponsum`. Cost if wrong: one more blocking function later.
- **P14 - checksums v1:** `x-amz-checksum-crc32` / `-sha256` = base64 of
  the big-endian digest, only when listed; `If-Match` on ranged GETs and
  the MD5 check of a single-part ETag (32 hex, no `-`) are horreum's own.

## Rulings made while executing

- **T1: fasti's Unix epoch fixed (1980 -> 1970).** `lib/fasti.c` built
  it as `MCMX + LXX`; `signatura_aws_dies` exposed it (2025 for a 2015
  vector). Audit parcum ...YWRXRC (epoch part only; its two other items
  stay open). Test made independent of the code's own epoch. Callers
  were only signatura_aws + tests. Cost if wrong: none found - nothing
  stored a fasti unix value.
- **T1: 9 vector exclusions, not 8** - three session-token vectors
  (`get-vanilla-with-session-token` too). Recorded per vector in
  `index.tsv`.
- **T1: test compares signed queries as decoded pairs** (the vector
  file writes `ሴ` raw on the request line).
- **T1: literals via `chorda_ex_literis`** (`(i8*)"..."` is a
  -Wcast-qual error).

- **T2: `horreum_responsum_legere` takes `tempus_signandi`.** The
  RequestTimeTooSkewed offset is server `Date` - signing time; only the
  executor knows the latter, and DO/R2 may not echo AWS's
  RequestTime/ServerTime body fields. Cost if wrong: one argument.
- **T2: P15 - numeric XML references decoded by horreum** (`xml.h`
  leaves `&#x1234;` raw).
- **T2: profile -> `tabella`** (`horreum_ex_tabella`; the plan's
  `horreum_ex_profilo` / "profilum" are not Latin). File content and
  `~/.rhubarb/horreum/<titulus>.clavis` unchanged.
- **T2: unknown profile keys are named only if `[a-z_]{1,16}`**, else
  "linea N" - a positional file must never echo its secret.
- **T2: `terminus` checked before `regio`** (generic without terminus
  names the missing endpoint).
- **T2: conditions/checksums refused (MALUM) on copy and multipart
  start** rather than silently ignored.

- **T3: executor state slot in horreum.h** (`horreum_sedem_ponere` /
  `horreum_sedes`); `horreum_cliens_parare` takes the piscina holding
  the state. `Horreum` is opaque to `horreum_cliens.c`. Cost if wrong:
  one pointer field and one argument.
- **T3: the clock offset persists** across calls in that state (a
  second call signs with it first time). Cost if wrong: one field.
- **T3: `horreum_delere_plura` gains `exitus_numerus`** (the planned
  signature had no count).
- **T3: CopyObject `200 + <Error>` read in the core reader**, so retry
  policy applies (`200 InternalError` retried); ETag and time from the
  `CopyObjectResult` body. New section `copiare_error_cc`.
- **T3: the fake's HORA fault became a server clock**
  (`simulacrum_horam_ponere`, 900 s window): a one-shot skewed reply
  cannot catch "offset not applied on the retry".
- **T3: moved to T4** with the tests that use them: the fake's
  multipart model, TRUNCARE, `simulacrum_corpus_maximum`, exsequi's
  NoSuchUpload-after-retry branch (d).
- **T3: plant 5 added** (CopyObject body not read -> `iter` red at the
  copy's ETag).

- **T4: CompleteMultipartUpload's 200 body read in the core** (as
  CopyObject in T3): `<Error>` in a 200 goes through retry policy; the
  final ETag comes from the body.
- **T4: one piscina per part / per range**, destroyed after use (an
  arena never frees, so "one part in memory" needs it). Part ETags and
  error text are copied to the caller's piscina.
- **T4: fake faults gain `parametrum`** (query-parameter match,
  `"partNumber=2"`, `"uploadId"`): Create and Complete are both POSTs
  on the key. New faults TRUNCARE (short body, matching length) and
  MUTARE (object replaced after one request); new fake accessors
  `simulacrum_obiectum_corpus`, `simulacrum_etag_ponere`,
  `simulacrum_si_etag_visa`, `simulacrum_claudere`.
- **T4: `ponere_ex_filo` uses a single PUT when size <= part size**
  (equal counts as one part); parts above DXII MiB refused (chorda is
  i32).
- **T4: two plants added** - exsequi's NoSuchUpload branch removed ->
  `perficere_iam_nostrum` red; md5 check removed -> `md5_etag` red.

## Review focus

1. **Object keys with awkward characters** (space, `+`, `%`, `?`, `#`,
   `&`, unicode, `//`, trailing `/`): the signed canonical path must
   equal the path on the wire, or every such key is 403. -> T2 section
   `via_clavis` (canonical path == `http_petitio_visus(...).via`) and T6
   probe keys `horreum-probatio/<id>/a b+c%d?e#f&g/ሴ//x/`.
2. **Zero-length objects and folder markers** (`""` body, key `foo/`):
   `Content-Length: 0` sent, ETag of empty MD5. -> T2 `ponere_capita`
   (empty body case) and T3 `iter` (empty object round trip).
3. **Keys with XML-special characters in listings** (`a&b<c>"d'`):
   entity-decoded on the way back. -> T2 `enumeratio_legere` fixture
   with `&amp;` / `&lt;` and T3 `enumerare_paginae` round trip of such
   a key.
4. **Large downloads:** `horreum_capere` on a 17 MiB object gives the
   named MALUM (P1), and `capere_in_filum` never leaves a half or
   spliced file. -> T3 `capere_maius`, T4 `capere_mutatum`,
   `capere_mensura_mala`.
5. **Profile file faults never echo a secret:** wrong mode, unknown
   key, missing field, unknown provider - each names the field, never
   a value. -> T2 `profilum_vitia` (every refusal's text checked not to
   contain the planted secret).

---

### Task 1: MD5 in `friatio` + `signatura_aws` (SigV4)

**Files:** modify `include/friatio.h`, `lib/friatio.c`,
`probationes/probatio_friatio.c`; create `include/signatura_aws.h`,
`lib/signatura_aws.c`, `lib/signatura_aws.worklog.md`,
`probationes/probatio_signatura_aws.c`,
`probationes/fixa/signatura_aws/v4/<vector>/` (copied files) +
`probationes/fixa/signatura_aws/NOTICE` +
`probationes/fixa/signatura_aws/index.tsv`; regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** `sigillum_hmac`, `sigillum_computare`,
`sigillum_hex` (`include/sigillum.h`), `url_codificare`, `fasti_ex_unix`,
`chorda_aedificator_*`. **Produces:**
```c
/* friatio.h (addendum) - MD5 (RFC 1321): NON securum; Content-MD5 S3
 * et ETag partis unius */
#define MD5_DIGEST_MENSURA  XVI
nomen structura {
    i32 status[IV];           /* A, B, C, D */
    i32 numerus[II];          /* bits (low, high) */
     i8 buffer[LXIV];
} MD5Contextus;
vacuum md5_initiare (MD5Contextus* ctx);
vacuum md5_addere (MD5Contextus* ctx, constans i8* data, i32 mensura);
vacuum md5_finire (MD5Contextus* ctx, i8 digest[MD5_DIGEST_MENSURA]);
vacuum md5_friare (constans i8* data, i32 mensura, i8 digest[MD5_DIGEST_MENSURA]);
vacuum md5_friare_chorda (chorda clavis, i8 digest[MD5_DIGEST_MENSURA]);
```
```c
/* signatura_aws.h - AWS Signature Version 4 (pura: tempus datur, nihil
 * I/O, nihil S3). Via codificatur SEMEL, numquam normalizatur (modus
 * S3). Secretum numquam in resultu aut errore. */
/* <aedilis corpus="lib/signatura_aws.c"/> */
#define SIGNATURA_AWS_EXPIRATIO_MAXIMA (DCIV * M + DCCC)   /* VII dies */
nomen structura { chorda titulus; chorda valor; } SignaturaPar;
nomen structura {
    constans character* methodus;     /* "GET", "PUT", ... */
                chorda  hospes;       /* valor capitis Host exactus */
                chorda  via;          /* DECODIFICATA ("/a b/ሴ") */
    constans SignaturaPar* quaestio;  i32 quaestio_numerus; /* decodificata */
    constans SignaturaPar* capita;    i32 capita_numerus;   /* praeter host, x-amz-date */
                chorda  corpus_sigillum;  /* hex SHA-256 aut "UNSIGNED-PAYLOAD" */
                   b32  corpus_capite;    /* addit x-amz-content-sha256 signatum */
           MomentumSec  tempus;
                chorda  regio;
                chorda  servitium;        /* "s3" */
                chorda  clavis_id;
                chorda  secretum;
                   i32  expiratio;        /* solum quaestioni: 1..MAXIMA */
} SignaturaPetitio;
nomen structura {
       b32  successus;
    chorda  error;                /* sine secreto */
    chorda  petitio_canonica;
    chorda  chorda_signanda;
    chorda  signatura;            /* hex */
    chorda  auctoritas;           /* valor Authorization (capitibus) */
    chorda  dies_amz;             /* "20150830T123600Z" */
    chorda  quaestio_signata;     /* tota, codificata, cum X-Amz-Signature (quaestione) */
    constans SignaturaPar* capita_addita; i32 capita_addita_numerus; /* x-amz-date [, x-amz-content-sha256] */
} SignaturaResultus;
SignaturaResultus signatura_aws_capitibus (constans SignaturaPetitio* p, Piscina* piscina);
SignaturaResultus signatura_aws_quaestione (constans SignaturaPetitio* p, Piscina* piscina);
chorda signatura_aws_dies (MomentumSec t, Piscina* piscina);   /* YYYYMMDDTHHMMSSZ */
/* RFC 3986 unreserved servantur, cetera %XX maiusculae; '/' servatur si servare_virgulam */
chorda signatura_aws_codificare (chorda s, b32 servare_virgulam, Piscina* piscina);
```
Canonical rules (the code's comments cite them): header names
lowercased, sorted; values trimmed, inner runs of whitespace (and
folded continuation lines) collapsed to one space; duplicate names
joined with `,` in order; query sorted by encoded key then encoded
value, `k=` for an empty value; signing key = HMAC chain over
`"AWS4"+secret`, date, region, service, `aws4_request`, zeroed after.

- [ ] **Step 1: Vendor the vectors.** Copy all 38 directories of
  `../aws-c-auth/tests/aws-signing-test-suite/v4/` (pinned: `git -C
  ../aws-c-auth rev-parse HEAD` = `84a26ea184d9ccd322c8164759510b3604ab13fd`;
  else stop and rule) to `probationes/fixa/signatura_aws/v4/`; write
  `NOTICE` (source URL, commit, Apache-2.0, "test vectors only"). The
  vector credentials are AWS's published example keys, not secrets.
- [ ] **Step 2: Failing tests, MD5** - new sections in
  `probationes/probatio_friatio.c`:
  - `md5 RFC 1321 vectors`: `""` d41d8cd98f00b204e9800998ecf8427e,
    `"a"` 0cc175b9c0f1b6a831c399e269772661, `"abc"`
    900150983cd24fb0d6963f7d28e17f72, `"message digest"`
    f96b697d7cb7938d525a2f31aaf161d0, `a..z`
    c3fcd3d76192e4007dfb496cca67e13b, `A-Za-z0-9`
    d174ab98d277d9f5a5611c2c9f419d9f, `"1234567890"` x8
    57edf4a22be3c955ac49da2e2107b67a;
  - `md5 incrementale`: the 80-byte vector split at every offset 0..80
    equals one-shot; one million `'a'` fed in 1000-byte pieces ->
    7707d6ae4e027c70eea2a935c2296f21.
- [ ] **Step 3: Failing tests, SigV4** - `probatio_signatura_aws.c`. A
  test-local reader turns a vector directory into a `SignaturaPetitio`:
  `context.json` via json.h (credentials, region, service, timestamp
  through `fasti_ex_iso` + `fasti_ad_unix`, `sign_body`,
  `expiration_in_seconds`); `request.txt` = request line split at the
  first space and the LAST `" HTTP/1.1"`, path and query split at the
  first `?`, query pairs `url_decodificare`d, header lines
  `Name:value`, lines starting with whitespace appended to the previous
  value, body after the blank line (`corpus_sigillum` = SHA-256 hex of
  it; `corpus_capite` = `sign_body`). Sections:
  - `vectores_capitum`: for each vector listed `curre` in `index.tsv`:
    `petitio_canonica` == `header-canonical-request.txt`,
    `chorda_signanda` == `header-string-to-sign.txt`, `signatura` ==
    `header-signature.txt`, and the `Authorization:` line of
    `header-signed-request.txt` == `auctoritas`;
  - `vectores_quaestionis`: the same with the `query-*` files and
    `quaestio_signata` against the query of `query-signed-request.txt`
    compared as a SET of pairs (the file orders X-Amz-Expires after
    SignedHeaders; the canonical form is what is signed);
  - `index_integer`: every directory under `v4/` appears in `index.tsv`
    as `curre` or `exclude <causa>` - a new vector cannot be skipped
    silently;
  - `codificatio`: `"a b"` -> `a%20b`; `"/"` kept with
    `servare_virgulam`, `%2F` without; `"~-_."` kept; `"+"` -> `%2B`;
    `"ሴ"` -> `%E1%88%B4`; `"%"` -> `%25`;
  - `dies`: `signatura_aws_dies(1440938160)` == `20150830T123600Z`;
  - `expiratio`: 0 and `MAXIMA + 1` refused, `MAXIMA` accepted;
  - `secretum_tacitum`: a request with an empty region -> FALSUM, and
    `error` does not contain the secret `wJalrXUtnFEMI/K7MDENG+bPxRfiCYEXAMPLEKEY`;
    no `SignaturaResultus` field contains it on success either.
  `index.tsv` starts with all 38 as `curre`.
- [ ] **Step 4: Run** `./compile_tests.sh friatio` and
  `./compile_tests.sh signatura_aws` - expect FAIL (no `md5_*`, no
  header).
- [ ] **Step 5: Implement** MD5 in `lib/friatio.c` (RFC 1321, little-
  endian words, the T table as a named `hic_manens constans i32` array)
  and `lib/signatura_aws.c` per the rules above.
- [ ] **Step 6: Run all 38.** Expected: exactly the 8 vectors of P4 fail.
  Mark those `exclude <causa>` in `index.tsv` (causa
  `normalizatio: S3 numquam normalizat` / `signum sessionis: nullus
  usus`). Any OTHER failure is a signer bug - fix it; if a vector needs
  behavior S3 does not have, stop and rule in the ledger.
- [ ] **Step 7: Run** - expect PASS; `CREDO_SECTIO=<s>` each alone too.
- [ ] **Step 8: Plants** (`silva.planta`): header values not collapsed
  -> `vectores_capitum` red (get-header-value-trim); query sorted by
  raw key instead of encoded -> red (get-vanilla-query-order-encoded);
  signing key derived without the date step -> red; one MD5 round
  constant changed -> `md5 RFC 1321 vectors` red.
- [ ] **Step 9: Linux:** `./tools/linux/probatio.sh -musl friatio signatura_aws`
  and `-glibc` - green.
- [ ] **Step 10: Commit** `friatio, signatura_aws: MD5 (RFC 1321) et
  AWS SigV4 (vectores aws-c-auth 84a26ea1) (horreum T1)`.

---

### Task 2: `horreum` core - profiles, builders, signing, parsers

**Files:** create `include/horreum.h`, `lib/horreum.c`,
`lib/horreum.worklog.md`, `probationes/probatio_horreum.c`,
`probationes/fixa/horreum/aws_documenta/*.xml` (responses written from
the S3 API reference examples: `enumeratio.xml` (ListObjectsV2 with
Contents, CommonPrefixes, IsTruncated, NextContinuationToken, a key
with `&amp;` and `&lt;`, LastModified with `.000Z`),
`multipars_initium.xml`, `multipars_finis.xml`, `multipars_partes.xml`,
`multipartes_pendentes.xml`, `delere_plura.xml` (Deleted + Error),
`error_nosuchkey.xml`, `error_precondition.xml`, `error_skewed.xml`,
`error_slowdown.xml`, `error_signature.xml`); regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** T1 (`signatura_aws_capitibus`,
`signatura_aws_quaestione`, `signatura_aws_codificare`, `md5_friare`),
`sigillum_*`, `crc32_*`, `base64_codificare`, `http_petitio_creare`,
`http_petitio_caput_addere`, `http_petitio_corpus_ponere_chorda`,
`http_petitio_visus`, `http_responsum_caput`, xml, `fasti_*`.
**Produces:**
```c
/* horreum.h - cliens S3-compatibilis, NUCLEUS PURUS (horreum-spec II):
 * provisores, opera (descriptio petitionis), signatio, lectio
 * responsorum, politica iterationis. Nihil I/O nisi
 * horreum_ex_profilo (lectio cruda clavis). */
/* <aedilis corpus="lib/horreum.c"/> */
#define HORREUM_CAP_ACL             0x01
#define HORREUM_CAP_SOLUM_SI_ABEST  0x02
#define HORREUM_CAP_SOLUM_SI_ETAG   0x04
#define HORREUM_CAP_RETENTIO        0x08   /* reservatum v1 */
#define HORREUM_CAP_CHECKSUM_CRC32  0x10
#define HORREUM_CAP_CHECKSUM_SHA256 0x20
#define HORREUM_CAP_DELERE_PLURA    0x40
#define HORREUM_MIB                 (MXXIV * MXXIV)
#define HORREUM_PARS_ORDINARIA      (XVI * HORREUM_MIB)
#define HORREUM_PARS_MINIMA         (V * HORREUM_MIB)
#define HORREUM_PARTES_MAXIMAE      (X * M)
#define HORREUM_FRUSTUM_LECTIONIS   (VIII * HORREUM_MIB)   /* P1 */

nomen enumeratio { HORREUM_STILUS_HOSPES = 0, HORREUM_STILUS_VIA } HorreumStilus;
nomen enumeratio { HORREUM_ACL_NULLA = 0, HORREUM_ACL_PRIVATA, HORREUM_ACL_PUBLICA_LECTIO } HorreumAcl;
nomen enumeratio { HORREUM_CHECKSUM_NULLUM = 0, HORREUM_CHECKSUM_CRC32, HORREUM_CHECKSUM_SHA256 } HorreumChecksum;
nomen enumeratio {
    HORREUM_BENE = 0, HORREUM_ABEST, HORREUM_CONDITIO, HORREUM_VETITUM,
    HORREUM_INCAPAX, HORREUM_TRANSITORIUM, HORREUM_MALUM
} HorreumErrorGenus;

nomen structura {
    constans character* titulus;       /* "digitalocean", "r2", "b2", "generic" */
    constans character* hospes_forma;  /* "{regio}.digitaloceanspaces.com",
                                        * "{rationarium}.r2.cloudflarestorage.com" */
         HorreumStilus  stilus;
    constans character* regio;         /* ordinaria; NIHIL = debet dari */
                   i32  capacitates;
                   i32  probatae;      /* in fonte: "probatum YYYY-MM-DD" aut "creditum <url>" */
                   b32  http_licet;    /* solum generic */
} HorreumProvisor;
constans HorreumProvisor* horreum_provisor (constans character* titulus);

nomen structura {
    constans character* provisor;
                chorda  terminus;      /* "https://host[:portus]"; vacuus = ex forma */
                chorda  regio;
                chorda  rationarium;   /* R2 account id, si terminus vacuus */
                chorda  situla;        /* ordinaria */
                chorda  clavis_id;
                chorda  secretum;      /* copiatur in Horreum, deinde vocans zephyrat */
} HorreumConfiguratio;

nomen structura Horreum Horreum;   /* opacus: secretum in capsa fixa intus */
nomen structura { chorda titulus; chorda valor; } HorreumMeta;  /* titulus sine "x-amz-meta-" */

nomen structura {
           chorda  clavis;
              i64  mensura;
           chorda  etag;              /* sine virgulis */
      MomentumSec  mutatum;           /* 0 = ignotum */
           chorda  genus_contenti;
           chorda  versio;
           chorda  checksum_crc32;    /* base64 ut missum */
           chorda  checksum_sha256;
     HorreumMeta*  meta;   i32 meta_numerus;
              b32  iam_nostrum;       /* II.5 */
} HorreumObjectum;

nomen structura {
    HorreumErrorGenus  genus;
                  i32  status;
               chorda  codex;          /* "NoSuchKey" */
               chorda  nuntius;
               chorda  petitio_id;     /* x-amz-request-id (aut cf-ray si R2) */
                  s64  deviatio_s;     /* horologium correctum (RequestTimeTooSkewed) */
} HorreumError;

nomen structura {
    chorda genus_contenti; chorda cache_control;
    constans HorreumMeta* meta; i32 meta_numerus;
    b32 solum_si_abest; chorda solum_si_etag; HorreumAcl acl;
    HorreumChecksum checksum; b32 periculum_accipere;
} HorreumPonendi;
nomen structura {
    i64 ab; i64 ad; b32 frustum;           /* Range: bytes=ab-ad si frustum */
    chorda si_etag; chorda nisi_etag; MomentumSec si_mutatum_post;
} HorreumCapiendi;
nomen structura {
    chorda praefixum; chorda delimitator; i32 magnitudo_paginae;  /* 0 = M */
    chorda signum_continuationis;
} HorreumEnumerandi;
nomen structura {
    HorreumObjectum* obiecta; i32 obiecta_numerus;
    chorda* praefixa_communia; i32 praefixa_numerus;
    b32 truncata; chorda signum_proximum;
} HorreumPagina;
nomen structura { chorda clavis; b32 deletum; HorreumError error; } HorreumDeletio;
nomen structura { i32 numerus; chorda etag; i64 mensura; } HorreumPars;
nomen structura { chorda clavis; chorda id_oneris; MomentumSec initium; } HorreumOnusPendens;

nomen enumeratio {
    HORREUM_OPUS_PONERE = 0, HORREUM_OPUS_CAPERE, HORREUM_OPUS_INSPICERE,
    HORREUM_OPUS_DELERE, HORREUM_OPUS_DELERE_PLURA, HORREUM_OPUS_COPIARE,
    HORREUM_OPUS_ENUMERARE, HORREUM_OPUS_MP_INCIPERE, HORREUM_OPUS_MP_PARS,
    HORREUM_OPUS_MP_PERFICERE, HORREUM_OPUS_MP_ABICERE, HORREUM_OPUS_MP_PARTES,
    HORREUM_OPUS_MP_PENDENTES
} HorreumOpusGenus;
nomen structura {
    HorreumOpusGenus  genus;
        HttpMethodus  methodus;
              chorda  situla;
              chorda  clavis;             /* decodificata */
       SignaturaPar*  quaestio;  i32 quaestio_numerus;
       SignaturaPar*  capita;    i32 capita_numerus;
              chorda  corpus;
              chorda  corpus_sha256_hex;
                 b32  condicionale;       /* PUT If-None-Match/If-Match (P10) */
                 b32  idempotens;
              chorda  perficere_etag_expectatum;  /* MP_PERFICERE iam_nostrum */
} HorreumOpus;

Horreum* horreum_creare (constans HorreumConfiguratio* c, HorreumError* error, Piscina* piscina);
Horreum* horreum_ex_profilo (constans character* via, HorreumError* error, Piscina* piscina);
chorda   horreum_situla_ordinaria (constans Horreum* h);
vacuum   horreum_claudere (Horreum* h);     /* zephyrat; NIHIL tutum */

b32 horreum_opus_ponere (Horreum*, chorda situla, chorda clavis, chorda corpus, constans HorreumPonendi*, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_capere (Horreum*, chorda situla, chorda clavis, constans HorreumCapiendi*, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_inspicere (Horreum*, chorda situla, chorda clavis, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_delere (Horreum*, chorda situla, chorda clavis, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_delere_plura (Horreum*, chorda situla, constans chorda* claves, i32 numerus, HorreumOpus*, HorreumError*, Piscina*);
/* optiones NIHIL = meta fontis servatur (COPY), aliter REPLACE */
b32 horreum_opus_copiare (Horreum*, chorda situla_fons, chorda clavis_fons, chorda situla, chorda clavis, constans HorreumPonendi*, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_enumerare (Horreum*, chorda situla, constans HorreumEnumerandi*, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_incipere (Horreum*, chorda situla, chorda clavis, constans HorreumPonendi*, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_pars (Horreum*, chorda situla, chorda clavis, chorda id_oneris, i32 numerus, chorda corpus, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_perficere (Horreum*, chorda situla, chorda clavis, chorda id_oneris, constans HorreumPars* partes, i32 numerus, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_abicere (Horreum*, chorda situla, chorda clavis, chorda id_oneris, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_partes (Horreum*, chorda situla, chorda clavis, chorda id_oneris, HorreumOpus*, HorreumError*, Piscina*);
b32 horreum_opus_mp_pendentes (Horreum*, chorda situla, chorda praefixum, HorreumOpus*, HorreumError*, Piscina*);

/* opus -> HttpPetitio signata (Host, x-amz-date, x-amz-content-sha256,
 * Content-Length 0 si corpus vacuum - P3). tempus = horologium + deviatio. */
HttpPetitio* horreum_petitionem_signare (Horreum*, constans HorreumOpus*, MomentumSec tempus, HorreumError*, Piscina*);
/* URL praesignata: GET aut PUT; genus_contenti (PUT) signatur si datur */
chorda horreum_praesignare (Horreum*, HttpMethodus, chorda situla, chorda clavis, i32 secundae, chorda genus_contenti, MomentumSec tempus, HorreumError*, Piscina*);

/* responsum -> successus aut error typatus (II.5 tabula); facta ex capitibus */
b32 horreum_responsum_legere (constans HorreumOpus*, HttpResultus, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_paginam_legere (chorda corpus, HorreumPagina*, HorreumError*, Piscina*);
b32 horreum_deletiones_legere (chorda corpus, HorreumDeletio** exitus, i32* numerus, HorreumError*, Piscina*);
b32 horreum_id_oneris_legere (chorda corpus, chorda* id, HorreumError*, Piscina*);
b32 horreum_partes_legere (chorda corpus, HorreumPars** exitus, i32* numerus, HorreumError*, Piscina*);
b32 horreum_pendentes_legere (chorda corpus, HorreumOnusPendens** exitus, i32* numerus, HorreumError*, Piscina*);
b32 horreum_etag_perfecti_legere (chorda corpus, chorda* etag, HorreumError*, Piscina*);

/* politica (pura): iterandumne post conatum 'conatus' (1-based)? mora in ms;
 * fortuna = valor aleatorius 0..2^32-1 (vocans ex sors) */
b32 horreum_iterandum (constans HorreumOpus*, constans HorreumError*, chorda retry_after, i32 conatus, i32 conatus_maximi, i32 mora_basis_ms, i32 fortuna, i32* mora_ms);
i64 horreum_partis_mensura (i64 magnitudo, i64 petita);   /* P12 */
```
Built-in table (`lib/horreum.c`, a named `hic_manens constans
HorreumProvisor` array), all `probatae = 0` until T6:
- `digitalocean`: `{regio}.digitaloceanspaces.com`, HOSPES, regio NIHIL,
  capacitates `ACL | DELERE_PLURA | SOLUM_SI_ETAG` (creditum DO Spaces
  API docs);
- `r2`: `{rationarium}.r2.cloudflarestorage.com` (EU: terminus given),
  HOSPES, regio `auto`, `SOLUM_SI_ABEST | SOLUM_SI_ETAG | DELERE_PLURA |
  CHECKSUM_CRC32 | CHECKSUM_SHA256` (creditum R2 S3 compatibility page);
- `b2`: `s3.{regio}.backblazeb2.com`, HOSPES, regio NIHIL, `DELERE_PLURA`
  (creditum);
- `generic`: forma NIHIL (terminus required), VIA, regio NIHIL,
  `capacitates` = `probatae` = `ACL | SOLUM_SI_ABEST | SOLUM_SI_ETAG |
  DELERE_PLURA | CHECKSUM_CRC32 | CHECKSUM_SHA256` - whoever points
  generic at a store owns the claim (MinIO, the simulacrum);
  `http_licet` VERUM.

Error table (`horreum_responsum_legere`): 404 / `NoSuchKey`,
`NoSuchBucket`, `NoSuchUpload` -> ABEST; 412, 304 -> CONDITIO; 403
(incl. `SignatureDoesNotMatch`, `AccessDenied`) -> VETITUM, except 403
`RequestTimeTooSkewed` -> TRANSITORIUM with `deviatio_s` = server
`Date` - signing time; 500, 502, 503, 504, `SlowDown`, `InternalError`,
transport failure (`successus` FALSUM) -> TRANSITORIUM; transport
"corpus nimium" -> MALUM, `nuntius` names `horreum_capere_in_filum`
(P1); other 4xx -> MALUM. HEAD errors have no body: from the status
alone.

- [ ] **Step 1: Failing tests** `probatio_horreum.c` (fixed tempus
  1440938160, configuration with clavis_id `AKIDEXAMPLE` and a planted
  secret `SECRETUM-PLANTATUM-0123456789`):
  - `provisores`: the four built-ins found; DO regio `nyc3` -> hospes
    `nyc3.digitaloceanspaces.com`; R2 rationarium `abc123` -> hospes
    `abc123.r2.cloudflarestorage.com`, regio `auto`; terminus
    `https://abc123.eu.r2.cloudflarestorage.com` wins over the form;
    DO without regio -> `horreum_creare` NIHIL, genus MALUM, nuntius
    names `regio`; generic without terminus -> refused; `http://` for
    DO -> refused, for generic accepted.
  - `stilus`: DO, bucket `situla`, key `a.txt` -> `visus.hospes` ==
    `situla.nyc3.digitaloceanspaces.com`, `visus.via` == `/a.txt`;
    generic -> host as configured, via `/situla/a.txt`; bucket `a.b`
    on DO -> MALUM naming DNS; on generic -> path style works; bucket
    `AB` or 2 chars -> MALUM.
  - `via_clavis` (review focus 1): keys `a b+c%d?e#f&g`, `ሴ/x`,
    `dir//x`, `dir/`, `./a/../b` -> canonical request's path line ==
    `visus.via` == the expected once-encoded path (`/situla/a%20b%2Bc%25d%3Fe%23f%26g`,
    `%E1%88%B4/x`, `dir//x`, `dir/`, `./a/../b`).
  - `ponere_capita` (review focus 2): content type, cache-control, two
    meta pairs (`x-amz-meta-<titulus>`), `x-amz-acl: public-read` (DO),
    `If-None-Match: *` (R2 with periculum), `If-Match: "e"`, checksum
    headers (R2, CRC32 base64 of big-endian); `x-amz-content-sha256` ==
    SHA-256 hex of the body; `x-amz-meta-sha256` present on both
    conditional kinds (P10) and absent otherwise; empty body ->
    `Content-Length: 0` header and sha256 of empty; Authorization
    present, its `SignedHeaders` lists every header horreum added.
  - `incapax`: ACL on R2 -> INCAPAX, `HorreumOpus` untouched (memset
    pattern checked); `solum_si_abest` on R2 while `probatae` lacks it ->
    INCAPAX, with `periculum_accipere` -> built; checksum CRC32 on DO ->
    INCAPAX; `delere_plura` on a profile without the bit -> INCAPAX.
  - `delere_plura_corpus`: 3 keys (one `a&b`) -> body
    `<Delete><Quiet>false</Quiet><Object><Key>...</Key></Object>...</Delete>`
    with `&amp;`; `Content-MD5` = base64(md5(body)); query `delete=`;
    1001 keys -> MALUM.
  - `enumerare_quaestio`: `list-type=2`, `prefix`, `delimiter`,
    `max-keys`, `continuation-token` in the signed canonical query,
    sorted; magnitudo 0 -> `max-keys=1000`.
  - `praesignare`: GET, 3600 s -> URL with `X-Amz-Algorithm`,
    `X-Amz-Credential=AKIDEXAMPLE%2F20150830%2F<regio>%2Fs3%2Faws4_request`,
    `X-Amz-Expires=3600`, `X-Amz-SignedHeaders=host`,
    `X-Amz-Signature` (64 hex), and the canonical request's payload line
    `UNSIGNED-PAYLOAD`; PUT with content type -> SignedHeaders
    `content-type;host`; 0 and 604801 s -> MALUM; the URL never contains
    the secret.
  - `tempora` (P7): `2026-10-21T14:09:00.000Z` and
    `2026-10-21T14:09:00Z` -> the same MomentumSec; `Wed, 21 Oct 2026
    14:09:00 GMT` -> the same; garbage -> 0.
  - `responsa_errorum`: each `error_*.xml` with its status through
    `horreum_responsum_legere` -> genus, codex, nuntius, petitio_id per
    the table; skewed with `Date` 20 min ahead -> TRANSITORIUM,
    `deviatio_s` == 1200; HEAD 404 empty body -> ABEST; transport
    failure -> TRANSITORIUM; "corpus nimium" -> MALUM naming
    `horreum_capere_in_filum`.
  - `objectum_ex_capitibus`: ETag `"abc"` -> `abc`; Content-Length,
    Last-Modified, Content-Type, `x-amz-version-id`,
    `x-amz-meta-Auctor: x` -> meta `auctor`/`x`, checksum headers.
  - `enumeratio_legere` (review focus 3): `enumeratio.xml` -> keys
    (with `a&b<c` decoded), sizes, ETags unquoted, LastModified,
    CommonPrefixes, truncated + token.
  - `multipars_legere`, `deletiones_legere`: the matching fixtures.
  - `iterandum`: TRANSITORIUM on an idempotent opus, conatus 1 of 3 ->
    VERUM, mora in `[basis, 2*basis)`; conatus 2 -> `[2b, 4b)`; conatus
    3 of 3 -> FALSUM; `Retry-After: 2` -> mora >= 2000; ABEST / VETITUM
    / MALUM / INCAPAX -> FALSUM; CONDITIO -> FALSUM (the iam_nostrum
    check is the executor's, not a retry).
  - `partis_mensura` (P12): (10 MiB, 0) -> 16 MiB; (1 GiB, 4 MiB) -> 5
    MiB; (200000 MiB, 16 MiB) -> 20 MiB (ceil(200000/10000)); result
    always a multiple of a MiB and >= 5 MiB.
  - `profilum_vitia` (review focus 5; files written by the test under
    `/tmp/horreum-<pid>-<n>/`, mode set with `filum_modum_ponere`):
    mode 0644 -> refusal containing `chmod 600`; unknown key `secretvm`
    -> refusal naming `secretvm`; missing `secretum` -> refusal naming
    `secretum`; provider `aws9` -> refusal naming it; a good file ->
    Horreum; EVERY refusal's nuntius checked not to contain the planted
    secret; after `horreum_claudere` the struct's bytes (still in the
    test piscina) do not contain it.
- [ ] **Step 2: Run** `./compile_tests.sh horreum` - expect FAIL (no
  header). (The pattern also matches `horreum_cliens` / `_vocatio`
  later - use `CREDO_SECTIO` or the exact binary when needed.)
- [ ] **Step 3: Implement** `lib/horreum.c`: provider table, `creare`
  (copies clavis_id and secretum into fixed `character[CCLVI]` arrays
  inside the struct; refuses longer), profile reader (P5), builders,
  signer (`signatura_aws_capitibus` with `corpus_capite` VERUM,
  servitium `s3`, via = `/` + key for HOSPES or `/situla/` + key for
  VIA), parsers (xml via a private `InternamentumChorda` from
  `internamentum_creare(piscina)`), error table, policy, part size.
- [ ] **Step 4: Run** - expect PASS; each section alone too.
- [ ] **Step 5: Plants:** URL path built from the raw key instead of the
  once-encoded one -> `via_clavis` red; capability check removed for ACL
  -> `incapax` red; `Content-Length: 0` dropped -> `ponere_capita` red;
  fraction not stripped -> `tempora` red; a refusal that prints the
  value (`"... %.*s"` of the line) -> `profilum_vitia` red.
- [ ] **Step 6: Linux** musl + glibc: `horreum friatio signatura_aws`.
- [ ] **Step 7: Commit** `horreum: nucleus purus - provisores, opera,
  signatio, responsa, politica iterationis (horreum T2)`.

---

### Task 3: blocking layer + fake S3 (vectura door) + retry semantics

**Files:** create `include/horreum_cliens.h`, `lib/horreum_cliens.c`,
`probationes/horreum_simulacrum.h`, `probationes/probatio_horreum_cliens.c`;
append to `lib/horreum.worklog.md`; regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** T2 (all `horreum_opus_*`,
`horreum_petitionem_signare`, `horreum_responsum_legere`, the `*_legere`
parsers, `horreum_iterandum`), `http_vectura_exsequi`,
`http_vectura_ordinaria`, `http_petitio_visus`, `sors_*`,
`signatura_aws_capitibus` (simulacrum verification). **Produces:**
```c
/* horreum_cliens.h - stratum OBSTANS (instrumenta, opera batch): opus
 * quodlibet per HttpVectura cum signatione, iterationibus, iam_nostrum */
/* <aedilis corpus="lib/horreum_cliens.c"/> */
nomen structura {
          HttpVectura  vectura;                /* exsequi NIHIL = http_exsequi */
    MomentumSec      (*nunc)(vacuum* datum);   /* NIHIL = time() */
               vacuum (*dormire)(vacuum* datum, i32 ms);  /* NIHIL = nanosleep */
               vacuum* datum;
                  i32  conatus_maximi;         /* 0 = III */
                  i32  mora_basis_ms;          /* 0 = CC */
                  i64  semen;                  /* semen sortis; 0 = ex tempore */
} HorreumClientisOptiones;
vacuum horreum_cliens_parare (Horreum* h, constans HorreumClientisOptiones* o);  /* NIHIL = ordinariae */

nomen structura {
             b32  successus;
    HttpResponsum* responsum;     /* ultimum (successus aut error) */
             i32  conatus;
             s64  deviatio_s;
             b32  iam_nostrum;
} HorreumExitus;
b32 horreum_exsequi (Horreum*, constans HorreumOpus*, HorreumExitus*, HorreumObjectum*, HorreumError*, Piscina*);

b32 horreum_ponere (Horreum*, chorda situla, chorda clavis, chorda corpus, constans HorreumPonendi*, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_capere (Horreum*, chorda situla, chorda clavis, constans HorreumCapiendi*, chorda* corpus, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_inspicere (Horreum*, chorda situla, chorda clavis, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_delere (Horreum*, chorda situla, chorda clavis, HorreumError*, Piscina*);
b32 horreum_delere_plura (Horreum*, chorda situla, constans chorda* claves, i32 numerus, HorreumDeletio** exitus, HorreumError*, Piscina*);
b32 horreum_copiare (Horreum*, chorda situla_fons, chorda clavis_fons, chorda situla, chorda clavis, constans HorreumPonendi*, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_enumerare (Horreum*, chorda situla, constans HorreumEnumerandi*, HorreumPagina*, HorreumError*, Piscina*);
```
`horreum_exsequi` loop: tempus = nunc() + deviatio; sign; execute; read;
success -> done; error -> (a) TRANSITORIUM with `deviatio_s` != 0 and
not yet corrected -> store the offset, retry once (counts as an
attempt); (b) `horreum_iterandum` -> dormire(mora), retry; (c) CONDITIO
on a `condicionale` opus AFTER at least one retry -> HEAD the key: its
`x-amz-meta-sha256` == `opus->corpus_sha256_hex` -> success with
`iam_nostrum` (the facts from the HEAD); else CONDITIO; (d)
ABEST/NoSuchUpload on MP_PERFICERE after a retry -> HEAD: ETag ==
`perficere_etag_expectatum` -> success `iam_nostrum`. Never logs.

`horreum_simulacrum.h` (P8; `interior` functions, the servus_scriptus
convention):
```c
nomen enumeratio {
    SIMULACRUM_VITIUM_STATUS = 0,      /* status + codex S3 (503 SlowDown ...) */
    SIMULACRUM_VITIUM_SCRIBERE_ABRUMPERE,  /* operatio fit, responsum perit */
    SIMULACRUM_VITIUM_HORA,            /* RequestTimeTooSkewed, Date + secundae */
    SIMULACRUM_VITIUM_TRUNCARE         /* corpus GET breviter quam Content-Length */
} SimulacrumVitiumGenus;
nomen structura {
    SimulacrumVitiumGenus genus; HttpMethodus methodus;   /* HTTP_METHODUS_IGNOTUS = quaelibet */
    constans character* clavis;    /* NIHIL = quaelibet */
    i32 status; constans character* codex; s32 secundae; i32 numerus;  /* quoties */
} SimulacrumVitium;
interior Simulacrum* simulacrum_creare (chorda clavis_id, chorda secretum, chorda regio, Piscina* p);
interior vacuum simulacrum_vitium_addere (Simulacrum* s, SimulacrumVitium v);
interior HttpVectura simulacrum_vectura (Simulacrum* s);
interior i32 simulacrum_petitiones (Simulacrum* s);        /* numerus receptarum */
interior i32 simulacrum_corpus_maximum (Simulacrum* s);    /* maximum corpus petitionis visum */
interior vacuum simulacrum_obiectum_ponere (Simulacrum* s, chorda clavis, chorda corpus);  /* sine HTTP */
```
Model: path-style `/situla/clavis` (tests use the generic profile with
`http://simulacrum.invalid`); PUT (If-None-Match `*`, If-Match, meta,
`x-amz-copy-source`), GET (Range, If-Match), HEAD, DELETE, POST
`?delete` (Content-MD5 checked), GET `?list-type=2` (prefix,
delimiter, max-keys, continuation-token = last key, keys sorted
bytewise), multipart (POST `?uploads`, PUT `?partNumber&uploadId`, POST
`?uploadId`, DELETE `?uploadId`, GET `?uploadId`, GET `?uploads`); ETag
= md5 hex (multipart: md5 of the part digests + `-N`); every request's
signature recomputed from `http_petitio_visus` with `signatura_aws`
and the known secret -> mismatch 403 `SignatureDoesNotMatch`;
responses as real S3 XML.

- [ ] **Step 1: Failing tests** `probatio_horreum_cliens.c` (fake
  `nunc` returning a counter time, fake `dormire` recording each ms):
  - `iter`: put / inspicere / capere / enumerare / delere round trip;
    empty object round trip (review focus 2); facts filled (size,
    ETag == md5 hex, Last-Modified).
  - `signatura_verificata`: a Horreum with a wrong secret -> VETITUM
    `SignatureDoesNotMatch`; the error's fields do not contain either
    secret.
  - `transitorium`: two 503 SlowDown then OK -> success, `conatus` 3,
    `dormire` called twice with growing waits inside the policy bounds;
    three 503 -> TRANSITORIUM after exactly 3 requests.
  - `retry_after`: 503 with `Retry-After: 2` -> first wait >= 2000.
  - `iam_nostrum` (II.5): PUT `solum_si_abest` with a
    SCRIBERE_ABRUMPERE fault on the first PUT -> retry -> 412 -> HEAD ->
    success, `iam_nostrum` VERUM, the object's content is ours.
  - `conflictus_verus`: another body pre-placed at the key
    (`simulacrum_obiectum_ponere`); PUT `solum_si_abest` with a 503 on
    the first attempt (nothing written) -> retry -> 412 -> HEAD -> sha
    differs -> CONDITIO, `iam_nostrum` FALSUM. Also without any fault:
    412 on the first attempt -> CONDITIO with no HEAD sent.
  - `si_etag_iterum`: PUT `solum_si_etag` with ABRUMPERE -> 412 on
    retry -> HEAD -> `iam_nostrum` (P10).
  - `horologium`: HORA fault +1200 s -> one retry signed with the
    offset -> success, `deviatio_s` 1200 in the exitus.
  - `delere_plura_partialis`: 3 keys, the fake fails one with
    `AccessDenied` -> 2 `deletum`, 1 with VETITUM.
  - `enumerare_paginae` (review focus 3): 2500 keys (one `a&b<c`),
    page 1000 -> 3 pages via tokens, all keys exactly once, the special
    key decoded.
  - `capere_maius` (review focus 4): a vectura that returns transport
    error "corpus nimium" -> MALUM naming `horreum_capere_in_filum`.
- [ ] **Step 2: Run** the suite - expect FAIL.
- [ ] **Step 3: Implement** `lib/horreum_cliens.c` (`postulata_posix.h`
  first for nanosleep / time) and the simulacrum.
- [ ] **Step 4: Run** - PASS; each section alone.
- [ ] **Step 5: Plants:** iam_nostrum HEAD compare removed (412 after
  retry -> success always) -> `conflictus_verus` red; deviatio not
  applied to the retry's time -> `horologium` red; Retry-After ignored
  -> `retry_after` red; re-signing skipped on retry (same x-amz-date) ->
  `horologium` red.
- [ ] **Step 6: Linux** musl + glibc: `horreum_cliens horreum`.
- [ ] **Step 7: Commit** `horreum_cliens: stratum obstans - exsequi cum
  iterationibus et 'iam nostrum', simulacrum S3 (horreum T3)`.

---

### Task 4: multipart + file helpers

**Files:** modify `include/horreum_cliens.h`, `lib/horreum_cliens.c`,
`probationes/horreum_simulacrum.h`, `probationes/probatio_horreum_cliens.c`;
append to `lib/horreum.worklog.md`.

**Interfaces - Consumes:** T2 `horreum_opus_mp_*`, the multipart
parsers, `horreum_partis_mensura`; T3 `horreum_exsequi`; filum
(`filum_octeti_aperire/legere/claudere`, `filum_scriptor_*`,
`filum_movere`, `filum_delere`, `filum_status`), `md5_*`.
**Produces** (in `horreum_cliens.h`):
```c
b32 horreum_mp_incipere (Horreum*, chorda situla, chorda clavis, constans HorreumPonendi*, chorda* id_oneris, HorreumError*, Piscina*);
b32 horreum_mp_partem_mittere (Horreum*, chorda situla, chorda clavis, chorda id_oneris, i32 numerus, chorda corpus, HorreumPars* exitus, HorreumError*, Piscina*);
b32 horreum_mp_perficere (Horreum*, chorda situla, chorda clavis, chorda id_oneris, constans HorreumPars*, i32 numerus, HorreumObjectum*, HorreumError*, Piscina*);
b32 horreum_mp_abicere (Horreum*, chorda situla, chorda clavis, chorda id_oneris, HorreumError*, Piscina*);
b32 horreum_mp_partes (Horreum*, chorda situla, chorda clavis, chorda id_oneris, HorreumPars**, i32*, HorreumError*, Piscina*);
b32 horreum_mp_pendentes (Horreum*, chorda situla, chorda praefixum, HorreumOnusPendens**, i32*, HorreumError*, Piscina*);
nomen structura { i64 pars_mensura; } HorreumFiliOptiones;   /* 0 = HORREUM_PARS_ORDINARIA */
/* infra partem: PUT unum; supra: multipars partibus aequis (P12), pars
 * una in memoria; defectus finalis -> abicere (nihil orbum a nobis) */
b32 horreum_ponere_ex_filo (Horreum*, chorda situla, chorda clavis, constans character* via, constans HorreumPonendi*, constans HorreumFiliOptiones*, HorreumObjectum*, HorreumError*, Piscina*);
/* HEAD, deinde GET per frusta HORREUM_FRUSTUM_LECTIONIS cum If-Match
 * (P1) in filum temporarium eiusdem directorii; mensura (et md5 si ETag
 * partis unius) probata; deinde filum_movere. Numquam filum dimidium. */
b32 horreum_capere_in_filum (Horreum*, chorda situla, chorda clavis, constans character* via, HorreumObjectum*, HorreumError*, Piscina*);
```
Temp name: `<via>.horreum-<pid>-<n>` in the same directory (so
`filum_movere` is a rename); removed on every failure path.

- [ ] **Step 1: Failing tests** (files under `/tmp/horreum-<pid>-<n>/`,
  removed at section end):
  - `multipars_iter`: incipere, 3 parts (5, 5, 1 MiB), perficere ->
    object of 11 MiB, ETag ends `-3`; capere == the concatenation.
  - `ex_filo_parvum`: 1 MiB file -> one PUT (simulacrum saw 1 request).
  - `ex_filo_magnum`: 40 MiB file, part 16 MiB -> parts 16, 16, 8;
    `simulacrum_corpus_maximum` <= 16 MiB; content equal.
  - `pars_iterata`: 503 once on part 2 -> part 2 sent twice, parts 1
    and 3 once; success.
  - `abortus`: part 2 fails 3 times -> TRANSITORIUM, the simulacrum
    holds no pending upload (abort was sent), no object.
  - `perficere_iam_nostrum`: ABRUMPERE on Complete -> retry gets
    NoSuchUpload -> HEAD ETag matches -> success `iam_nostrum`.
  - `pendentes`: two abandoned uploads under `x/` -> both listed;
    `horreum_mp_abicere` each -> none left.
  - `capere_in_filum`: 20 MiB object -> 3 ranged GETs (8, 8, 4 MiB)
    each with If-Match; destination equals; no temp file left.
  - `capere_mutatum` (review focus 4): object replaced after the first
    range -> 412 -> CONDITIO; no destination, no temp file.
  - `capere_mensura_mala`: TRUNCARE fault -> refused (MALUM); nothing at
    the destination; an EXISTING destination file is left untouched.
  - `md5_etag`: single-part object whose stored ETag the fake corrupts
    -> refused naming md5.
- [ ] **Step 2: Run** - expect FAIL.
- [ ] **Step 3: Implement.**
- [ ] **Step 4: Run** - PASS; each section alone.
- [ ] **Step 5: Plants:** abort on failure removed -> `abortus` red;
  If-Match dropped from ranges -> `capere_mutatum` red; rename before
  the size check -> `capere_mensura_mala` red.
- [ ] **Step 6: Linux** musl + glibc: `horreum_cliens`.
- [ ] **Step 7: Commit** `horreum_cliens: multipars, ponere ex filo et
  capere in filum (frusta cum If-Match) (horreum T4)`.

---

### Task 5: reactor layer

**Files:** create `include/horreum_vocatio.h`, `lib/horreum_vocatio.c`,
`probationes/probatio_horreum_vocatio.c`; modify
`probationes/horreum_simulacrum.h` (loopback door); regenerate
`compile_tests_fontes_generata.sh`.

**Interfaces - Consumes:** T2 (builders, signer, parsers, policy), T3
semantics (the same loop, re-expressed as a state machine), `http_vocare`,
`http_vocatio_resultus`, `http_vocationem_abrumpere`,
`reactor_timer_adicere/cancellare`, `tcp_servus_*`, `http_parser_*`
(simulacrum door). **Produces:**
```c
/* horreum_vocatio.h - stratum REACTORIS (servi): opus quodlibet per
 * http_vocare; iterationes per timers; finis SEMEL, numquam intra
 * horreum_vocare */
/* <aedilis corpus="lib/horreum_vocatio.c"/> */
nomen structura HorreumVocatio HorreumVocatio;
nomen structura {
              b32  successus;
    HttpResponsum* responsum;
  HorreumObjectum  obiectum;
     HorreumError  error;
              i32  conatus;
              b32  iam_nostrum;
} HorreumVocatioResultus;
nomen vacuum (*HorreumVocatioFinis)(vacuum* datum, HorreumVocatio* v, constans HorreumVocatioResultus* r);
nomen structura {
    i32 conatus_maximi; i32 mora_basis_ms; i64 semen;
    constans HttpVocatioOptiones* http;     /* NIHIL = ordinariae */
} HorreumVocatioOptiones;
HorreumVocatio* horreum_vocare (Reactor*, Horreum*, constans HorreumOpus*, HorreumVocatioFinis, vacuum* datum, constans HorreumVocatioOptiones*, Piscina*);
vacuum horreum_vocationem_abrumpere (HorreumVocatio* v);   /* idempotens; finis ABRUPTA ante reditum */
```
Simulacrum loopback door: `interior i32 simulacrum_audire(Simulacrum*,
Reactor*)` -> port on 127.0.0.1; one connection at a time; request
parsed with `http_parser`; the same model answers; `Connection: close`.

- [ ] **Step 1: Failing tests** (generic profile, terminus
  `http://127.0.0.1:<port>`, `mora_basis_ms` 1):
  - `iter`: put then get of a 1 KiB object through `horreum_vocare`,
    pumping `reactor_poll` until finis; finis called once.
  - `transitorium`: 503 twice then OK -> success, `conatus` 3.
  - `iam_nostrum`: ABRUMPERE on a conditional PUT -> success
    `iam_nostrum` (the follow-up HEAD runs inside the machine).
  - `horologium`: HORA +1200 -> success.
  - `abrumpere`: abort during the first backoff -> finis ABRUPTA once,
    no later finis, no timer left (`reactor_numerus_timer` back to the
    count before).
  - `finis_non_intra`: finis is never called inside `horreum_vocare`
    (an INCAPAX opus is refused by the builder, so a MALUM opus - an
    unreachable port - finishes on a later poll).
- [ ] **Step 2: Run** - expect FAIL.
- [ ] **Step 3: Implement.**
- [ ] **Step 4: Run** - PASS; each section alone.
- [ ] **Step 5: Plants:** timer not cancelled on abort ->
  `abrumpere` red; finis called synchronously on immediate failure ->
  `finis_non_intra` red.
- [ ] **Step 6: Linux** musl + glibc: `horreum_vocatio`.
- [ ] **Step 7: Commit** `horreum_vocatio: stratum reactoris - opus per
  http_vocare, iterationes per timers (horreum T5)`.

---

### Task 6: `bin/horreum`, live probes on DO and R2, close-out

**Files:** create `tools/horreum.c`, `tools/horreum_fumus.sh`; modify
`lib/horreum.c` (built-in flags promoted, dated), `probationes/probatio_horreum.c`
(parsers over every `probationes/fixa/horreum/<provisor>/`); create
`probationes/fixa/horreum/digitalocean/*.xml`,
`probationes/fixa/horreum/r2/*.xml` (captured, scrubbed); update
`project-specs/horreum-spec.md` (IMPLETUM, commits, AUDIENDA answered),
this plan, worklogs; ledger: close wish …GWM.

**Interfaces - Consumes:** T2-T4. **Produces:** the CLI:
```
horreum -profilum <titulus|via> ponere <clavis> <filum> [-genus <ct>] [-solum-si-abest]
horreum -profilum P capere <clavis> <filum>
horreum -profilum P inspicere <clavis>
horreum -profilum P enumerare [praefixum] [-delimitator /]
horreum -profilum P delere <clavis>...
horreum -profilum P praesignare GET|PUT <clavis> [-secundae N]
horreum -profilum P pendentes [praefixum] [-abicere]
horreum -profilum P probare [-praefixum horreum-probatio/] [-responsa <dir>]
```
`<titulus>` -> `~/.rhubarb/horreum/<titulus>.clavis`. Output never
prints the secret or a presigned URL's signature except for
`praesignare` itself (its whole point). `probare` works under
`<praefixum><ulid>/`, removes everything it made (objects and uploads),
prints one line per capability (`probatum` / `refutatum` /
`non probatum: <causa>`) and the probe keys of review focus 1, and with
`-responsa` saves raw response bodies with `<ID>`, `<DisplayName>` and
owner ids replaced by `SCRUBBED`.

Probe checks: PUT/GET/HEAD/DELETE round trip on the review-focus-1
keys and an empty object; `If-None-Match: *` on an existing key -> 412
(and the same on an absent key -> 200); `If-Match` wrong ETag -> 412;
`x-amz-acl: public-read` accepted or refused; CRC32 / SHA-256 checksum
headers accepted and echoed; DeleteObjects with Content-MD5; multipart
3 x 5 MiB; list with delimiter; presigned GET and PUT fetched through
`http_exsequi` (no Authorization header); request-id header name.

- [ ] **Step 1: Build** `./bin/aedilis tools/horreum.c` and `bash
  build/aedilis/horreum/struere.sh` (read the script first; no ~/.bin
  install from the worktree); `-h` prints usage.
- [ ] **Step 2: Fran's profiles.** Fran writes
  `~/.rhubarb/horreum/digitalocean.clavis` and relabels
  `~/.rhubarb/horreum/cloudflare.clavis` (P5 format; `provisor r2`,
  `terminus` = the endpoint, `regio auto`, `situla`); Claude checks only
  `stat -f %Lp` == 600.
- [ ] **Step 3: Live probe (Fran's OK first - it writes to his
  buckets):** `build/aedilis/horreum/horreum -profilum digitalocean
  probare -responsa probationes/fixa/horreum/digitalocean`, then
  `cloudflare` (into `.../fixa/horreum/r2`). Read the report; a signing failure here (403 SignatureDoesNotMatch)
  is a T1/T2 bug - fix with a failing test first.
- [ ] **Step 4: Promote flags:** for each `probatum` capability set the
  bit in `probatae` with `/* probatum 2026-MM-DD (horreum probare) */`;
  each `refutatum` clears the capability bit with its date; capability
  claims the probe could not test stay `creditum`.
- [ ] **Step 5: Fixtures:** review the captured bodies for anything
  identifying (bucket names are fine; owner ids scrubbed); the parser
  sections now loop over `aws_documenta/`, `digitalocean/`, `r2/` -
  run, fix any parser gap with a failing test first.
- [ ] **Step 6: Smoke script** `tools/horreum_fumus.sh <profilum>`
  (put / get / list / presign / multipart / cleanup; exit non-zero on
  any failure; prints no secrets) - run it on both.
- [ ] **Step 7: Linux** musl + glibc: `horreum horreum_cliens
  horreum_vocatio signatura_aws friatio`.
- [ ] **Step 8: Close-out:** spec status IMPLETUM with commits and the
  answered AUDIENDA; plan rulings; worklogs; ledger wish …GWM ->
  impletum (catalog wish …5E8 stays open); file the
  `filum_secretum_legere` wish (P5).
- [ ] **Step 9: Commit** `horreum: vivum in DO Spaces et R2 - bin/horreum,
  probare, capacitates probatae, responsa vera (horreum T6)`.
