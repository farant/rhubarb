# horreum worklog

## 2026-10-21 - T2: the pure core

- **Signed path == wire path by construction.** The signer gets the
  DECODED path; the URL gets `signatura_aws_codificare(via, VERUM)` -
  the same function the signer uses - and the wire query is the
  canonical query (line 3 of the canonical request). The test does not
  peek at internals: it rebuilds the signature from the wire request
  (`http_petitio_visus`: decoded path, decoded query, headers named in
  SignedHeaders) the way a server would, and compares Authorization.
  That helper is the seed of T3's fake-server verifier.
- **`http.c` passes the URL path through verbatim** (`_parse_url`
  keeps everything after the host) and adds `Host` only when absent,
  so horreum sets `Host` itself (with `:port` for generic) and the
  signed host matches.
- **`xml.h` decodes named entities but not numeric references**
  (`&#x1234;`, `&#13;` stay raw); horreum's `_textus` decodes them to
  UTF-8 (plan P15). Caveat: a key whose literal text is `&#x41;`
  arrives as `&amp;#x41;`, which xml.h turns into `&#x41;` and horreum
  then decodes once more - indistinguishable after xml.h; accepted.
- **Profile errors and secrets.** The obvious rule "name the unknown
  key" leaks: in the old positional format (bare id / secret /
  endpoint lines) the secret IS the "key". An unknown key is named only
  when it is `[a-z_]` and at most 16 chars; otherwise the message gives
  only the line number. Planted red (`profilum_vitia` -> renamed
  `tabella_vitia`).
- **Renamed for the Latin lint:** profile -> `tabella`
  (`horreum_ex_tabella`, "tabella" = a small written tablet);
  `praesignare` kept (classical *praesigno*) with a glossary entry.
- `magnitudo` (the sizeof macro) slipped in as a parameter name;
  examen caught it before the compiler did.
- The `secretum` and `clavis_id` live in fixed arrays inside the
  `Horreum` struct so `horreum_claudere` can zero them; the test scans
  the struct's memory after close (allocated in a fixed-size piscina so
  the scan stays inside the arena).

## 2026-10-21 - T3: the blocking layer and the fake S3

- **Where the client's state lives.** `Horreum` is opaque to
  `horreum_cliens.c`, but the blocking layer needs per-store state
  (options, the jitter PRNG, the learned clock offset). horreum.h got a
  bare slot (`horreum_sedem_ponere` / `horreum_sedes`), and
  `horreum_cliens_parare` takes the piscina that holds the state (it
  must outlive the calls). Without `parare` every call builds defaults
  on the stack and the offset is not remembered.
- **The clock offset persists.** Once a RequestTimeTooSkewed reply has
  given the offset, later calls sign with it on their first attempt
  (`horologium` checks one request). Within one call the correction is
  made at most once; it counts as an attempt but does not sleep.
- **"412 after a retry" is decided by `conatus > 1`, not by why the
  earlier attempt failed.** A 503 means nothing was written, so a HEAD
  then is strictly unnecessary - but harmless, and it keeps the rule
  simple: any 412 on a conditional op that is not the first attempt
  gets the HEAD + `x-amz-meta-sha256` compare. A first-attempt 412
  sends no HEAD (`conflictus_verus` counts requests).
- **CopyObject's 200-with-`<Error>`** is read in the core
  (`horreum_responsum_legere`), not in `horreum_copiare`, so the retry
  policy sees it: `200 InternalError` is retried like a 500. The
  copy's ETag and time come from the `CopyObjectResult` body, not the
  headers.
- **The fake's clock is a server clock, not a one-shot fault.** The
  plan's HORA fault ("reply skewed once") cannot catch "offset not
  applied on the retry" - the retry would pass anyway. The simulacrum
  instead has a clock (`simulacrum_horam_ponere`) and rejects any
  `x-amz-date` more than 900 s off, as S3 does; both plants (offset
  dropped, petition not re-signed) turn `horologium` red.
- **The fake checks what a server checks:** signature recomputed from
  the wire (wrong secret -> 403), `x-amz-content-sha256` against the
  body, Content-MD5 on batch delete, bucket name. Continuation tokens
  are base64 of the last returned name, so `+ / =` cross the query
  encoding on every paged listing.
- **Trap: `chorda_incipit(s, empty)` is FALSUM when the empty prefix
  has a NULL `datum`** (`chorda_ex_buffer(NIHIL, 0)`), VERUM when it
  has a buffer. The fake's unprefixed listing returned nothing until
  the check was guarded by `mensura > 0`. Parked (see ledger).
- Deferred to T4 with the tests that need them: the fake's multipart
  model, TRUNCARE, `simulacrum_corpus_maximum`, and exsequi's
  NoSuchUpload-after-retry branch.

## 2026-10-21 - T4: multipart and the file helpers

- **One part in memory means one piscina per part.** An arena never
  frees, so reading a 40 MiB file part by part into the caller's
  piscina would still hold all 40 MiB. `horreum_ponere_ex_filo` gives
  each part (and `horreum_capere_in_filum` each 8 MiB range) its own
  `piscina_generare_dynamicum`, destroyed after the send; the part's
  ETag and any error text are copied to the caller's piscina first
  (`_errorem_transcribere`). The fake keeps each pending upload in its
  own piscina too, freed on complete/abort (`simulacrum_claudere` frees
  leftovers) - the whole suite peaks at ~180 MB RSS, under credo's
  256 MiB section ceiling without the `gravis` flag.
- **CompleteMultipartUpload's 200 body is read in the core**, like
  CopyObject's (T3): `<Error>` in a 200 is an error the retry policy
  sees, and the final ETag comes from `<CompleteMultipartUploadResult>`
  (the response has no ETag header).
- **NoSuchUpload after a retried Complete** -> HEAD, compare the ETag
  with `perficere_etag_expectatum` (md5 of the part digests + `-N`,
  computed by the T2 builder). Equal -> success `iam_nostrum`.
- **Abort on any final failure** of the multipart path, with the
  abort's own error ignored so the caller sees the first failure.
- **`capere_in_filum` order:** HEAD -> temp file `<via>.horreum-<pid>-
  <sors hex>` (same directory, so the move is a rename) -> ranged GETs
  each with `If-Match: <HEAD ETag>` -> close -> total size check -> md5
  check (single-part ETag form only) -> rename. Any failure deletes the
  temp; an existing destination is never touched before the rename.
  Caveat: on AWS, SSE-KMS objects have 32-hex ETags that are NOT md5;
  DO and R2 are md5. If a provider ever breaks that, the md5 check
  needs a capability bit rather than the ETag's shape.
- **Fake faults matched by query parameter** (`parametrum`:
  `"partNumber=2"`, `"uploadId"`): CreateMultipartUpload and
  CompleteMultipartUpload are both POSTs on the same key, so method +
  key alone cannot target the Complete.
- TRUNCARE returns a short body with a matching Content-Length (a
  server that sends less than asked without a transport error);
  MUTARE replaces the object after serving one request (same size, new
  ETag) - without If-Match the result would be a spliced file of the
  right size, which is exactly what the `capere_mutatum` plant shows.

## 2026-10-22 - T5: reactor layer (horreum_vocatio)

- **The blocking loop re-expressed as a state machine.** At any moment
  at most one thing is pending per vocatio: the `http_vocatio` of an
  attempt, the HEAD child, or a reactor timer (backoff, or the deferred
  finis). Abort cancels/aborts whichever it is; the inner finis that
  `http_vocationem_abrumpere` / the child's abort calls synchronously is
  silenced by the `abrumpens` flag, then our own finis runs once with
  `abrupta`.
- **The 'iam nostrum' HEAD is a child HorreumVocatio** (same machine,
  so the HEAD gets its own retries and clock offset - as the blocking
  layer's nested `horreum_exsequi` does). Its result decides: our
  sha256 meta (conditional PUT) or the expected ETag (Complete) ->
  success `iam_nostrum`, else the saved 412/404.
- **finis never inside `horreum_vocare`:** the only immediate failure is
  the signer (e.g. `h == NIHIL`); it is reported through a 0 ms timer.
  `http_vocare` already defers its own failures (closed port).
- **No shared clock state at this layer.** The blocking layer keeps the
  offset in the horreum's executor slot; here the offset goes out in
  `resultus.deviatio_s` and back in through `optiones.deviatio_s`.
- **Fake over TCP = `probationes/horreum_simulacrum_rete.h`**, a door in
  its own header: putting it into `horreum_simulacrum.h` would force the
  T3/T4 suite (interior-function convention) to call it and link
  reactor/tcp. The request is parsed with `http_parser`, rebuilt as an
  `HttpPetitio` (raw URI, every header but Connection, body) and handed
  to the same `_sim_exsequi`. HEAD goes through
  `http_responsum_capita_serialize` with the model's Content-Length -
  the normal serializer overwrites Content-Length from the (empty) body.
  SCRIBERE_ABRUMPERE = close without a reply.
- **Linux:** `http_vocatio` resolves names through `nomina`, which only
  has `nomina_macos.m`, so no reactor suite can link on Linux (true of
  `probatio_http_vocatio*` too). Checked compile-only under gcc musl +
  glibc with the house flags; the T2-T4 suites run green there. Parked
  ...NPMN7.

## 2026-10-22 - T6: live on DO Spaces and R2

- **What the providers actually do** (horreum probare, both clean, 0
  left behind):

  | | DO Spaces | R2 |
  |---|---|---|
  | If-None-Match: * on PUT | yes | yes |
  | If-Match on PUT | **412 even with the right ETag** | yes |
  | If-Match on GET | yes (capere_in_filum works) | yes |
  | x-amz-acl public-read | yes, readable unsigned | accepted, **no effect** (unsigned GET 400) |
  | x-amz-checksum-crc32/sha256 | **ignored** (wrong value accepted) | verified (BadDigest), echoed |
  | DeleteObjects + Content-MD5 | yes | yes |
  | request id header | x-amz-request-id | cf-ray |
  | Complete ETag | unquoted | `&quot;`-quoted |

- **R2 UploadId quirk:** ListMultipartUploads gives a different UploadId
  string than CreateMultipartUpload; both abort the same upload. The
  probe first failed live at "ListMultipartUploads:" with an empty
  error (the list call succeeded, the id just never matched). Never
  compare upload ids across calls - compare keys.
- **Testing a negative capability needs a falsifier:** "wrong checksum
  refused" and "right ETag accepted" are what separate DO from R2;
  "right checksum accepted" and "wrong ETag refused" pass on both.
- **The probe runs against the fake before it touches a bucket.** The
  fake had to learn presigned verification, checksums, host-style
  requests and unsigned public reads - each of which is now covered
  offline. The accessor table (`simulacrum_aditus`) ends the
  "every includer calls every accessor" rule for this fake.
- Smoke: `tools/horreum_fumus.sh <profile>` (put/get/inspect/list,
  presigned GET via curl, 17 MiB two-part upload, cleanup) - green on
  both. It never prints the presigned URL.

## 2026-10-22 - T7: observer hook + probe causes (dogfooding)

Fran asked whether the T6 response-body capture had helped. Honest
answer: it diagnosed the R2 UploadId mismatch in one look and gave us
the real fixtures, but it was wired by hand into the probe only, covered
only the eight named steps, and saved no headers - DO's If-Match 412
was found without it. Two improvements followed:

- **Observer, not logging.** `HorreumObservator` in both executors'
  options, called after every attempt before the response is read
  (so a 412-after-retry shows the PUT, the retried PUT and the HEAD).
  The vocatio stores the current attempt's `petitio` in the struct
  because `http_vocatio_resultus` does not give it back; the child HEAD
  inherits the options and so the observer. The transcript function
  lives in the pure core (returns a chorda; the caller decides where it
  goes). Redaction is by header name: Authorization,
  X-Amz-Security-Token. The access key id lives only inside
  Authorization (Credential=), so covering that header covers it.
- **Probe causes.** The live R2 failure printed "ListMultipartUploads:"
  and nothing else because the list call SUCCEEDED and the comparison
  after it failed with a zeroed error. `_causa` now never ends after the
  colon; the multipart step separates "call failed" from "comparison
  failed" and names the count / ETag / size. New fake switch
  `celare_onera` (list returns no uploads) is the red test; every probe
  test now asserts `_causae_plenae`.
- `bin/horreum -vestigium VIA` uses a throwaway piscina per attempt so
  a long command's arena does not grow with the trace.
- Live check (read-only HEAD of a missing key, both profiles): DO sends
  an empty-bodied 404 with x-amz-request-id; R2 a 404 with CF-RAY and no
  x-amz-* headers at all. The R2 Host line carries the account id:
  traces are local only.

