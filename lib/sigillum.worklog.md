# sigillum worklog

## 2026-07-14 — born (tabularium-gradus.md phase A)

SHA-256, FIPS 180-4, one-shot + incremental API. Goldens: NIST
vectors (empty, "abc", two-block, million-'a') + block-boundary sweep
(55..64 bytes, incremental-vs-one-shot equality) — the padding
corners. i8 being UNSIGNED char in this house made the byte work
clean; i32 unsigned = the rotr world is native.

Role (vs friatio): friatio hashes to DISPERSE (table buckets);
sigillum hashes to IDENTIFY (content addressing — tabularium §III,
CAS, reducer versions). When a hash IS an identity, collision
resistance is load-bearing; that's why this exists separately.

Note for the eventual judge-push: the resident flagged phantom
errors on this lib's first compile because its praeparatio predates
the new header (new-header judge-miss — known park, second and third
firings today). censor + examen.sh + clang triple-oracle settled it
in one command each.
