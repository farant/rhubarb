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

## 2026-09-24 — `sigillum_hmac` (lapide feature-requests/005)

HMAC-SHA256 per RFC 2104 on the existing streaming SHA-256: key longer
than the 64-byte block is hashed first, then zero-padded; inner pad 0x36,
outer 0x5c. Expected values in the test were GENERATED with Python's
`hmac` module (RFC 4231 cases 1, 2, 3, 6 + empty key/message + a key of
exactly 64 bytes), never typed. Plant: the long-key branch disabled (key
truncated to 64 instead of hashed) → exactly one case red (RFC case 6),
which is the case that branch exists for. The word `hmac` joined `sha`,
`sha1`, `sha256` in `oratio/glossarium.stml` as a permitted technical
term (the tester searched for exactly that word).
