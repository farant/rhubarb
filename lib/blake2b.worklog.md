# blake2b worklog

## 2026-10-17 - born (hospitium v3 T5)

- Straight RFC 7693 (Appendix C re-expressed). The one subtle rule: a
  full 128-byte buffer is compressed only when MORE input arrives, so
  the last block always goes through `blake2b_finire` with the final
  flag - even when the message is an exact multiple of 128 bytes. A
  key is a padded first block (`c = 128` after the key).
- Oracle: the RFC text committed unchanged. Appendix A ("abc") and the
  Appendix E self-test (24 unkeyed + 24 keyed hashes folded into one
  BLAKE2b-256 grand hash) - the test reads the expected bytes AND the
  length tables out of the RFC's own C listing.
