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
