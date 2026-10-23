# signatura_aws worklog

## 2026-10-21 - T1: SigV4 against the aws-c-auth suite

- Vectors: aws-c-auth @ 84a26ea1 `tests/aws-signing-test-suite/v4/`
  (38), vendored IMMUTATA in `probationes/fixa/signatura_aws/v4/` with
  NOTICE; `index.tsv` says `curre` / `exclude <causa>` per vector and
  the `index_integer` section refuses a directory the index does not
  name.
- 29 run in both modes (header + presigned query). 9 excluded, all
  ran first and failed for the stated reason: 6 `*-normalized` (S3
  never normalizes paths; the `*-unnormalized` twins pass) and **3**
  session-token vectors (`get-vanilla-with-session-token`,
  `post-sts-header-before`, `-after`) - the plan expected 2.
- The spec's `semel_codificare` flag was not needed: every remaining
  vector passes with single encoding.
- Query mode: the request's own headers are signed (no x-amz-date /
  x-amz-content-sha256 header); the X-Amz-* params join the caller's
  query and the whole set is sorted by encoded key then value (`%` <
  `X`, so `%E1%88%B4=bar` sorts first).
- `query-signed-request.txt` writes non-ASCII query keys RAW (`ሴ=bar`);
  the test compares decoded pairs as a set. Header-mode
  `Authorization` lines compare exactly.
- -Wcast-qual: `chorda_ex_buffer((i8*)"literal")` drops const and is
  an error under the house flags; literals go through
  `chorda_ex_literis(l, piscina)`.
- Exposed `fasti`'s 1980 epoch (see lib/fasti.worklog.md).
