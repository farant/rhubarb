# multipartita worklog

## 2026-10-17 - born (hospitium v3 T3)

- **Matcher = KMP over D = "\r\n--" + limes.** The bytes held during a
  partial match are always D[0..par), so nothing is copied: on a
  mismatch the failure table says how many leading held bytes are data,
  and they are emitted from `delim` itself. That is why a split at any
  byte gives the same transcript without a lookbehind buffer. For RFC
  2046 boundaries (bchars never contain CR) the failure table is all
  zeros, but it is computed generally anyway.
- **The virtual CRLF:** the body is treated as if prefixed by CRLF
  (`par = II` at start), so "--limes" at offset 0 matches with the same
  code path as every later delimiter.
- **PROSPECTUS after a full D:** "--" = close, LWSP* CRLF = next part,
  anything else = D was data (`--limesX`). On failure the held D is
  emitted per the failure table and the lookahead bytes are re-fed
  through `_octetum` (bounded: MP_PROSPECTUS = 64 bytes).
- **LF-only refused** at two places (after a boundary, inside part
  headers) with "LF sine CR ..." - RFC 7578 requires CRLF; being lenient
  here would make "\n--limes" inside data ambiguous.
- Data reach the callback through a 4 KB staging buffer, flushed at the
  end of every `multipartita_adicere` and before each part's `finis` -
  so callback chunking differs with input chunking, the concatenation
  does not (tests compare concatenated transcripts).
- The guard-region sweep (`custodia_regionis`) puts every prefix flush
  against an unmapped page: an over-read would SIGSEGV (tls13 R11).
