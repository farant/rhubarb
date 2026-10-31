# hpack worklog

## 2026-10-30 - T1: primitives (http2-plan T1)

Integers (RFC 7541 5.1), Huffman strings (5.2 + Appendix B), static table (Appendix A).

- **Appendix B is data, taken from the RFC text, not typed.** `rfc7541.txt` was downloaded and the 257 rows parsed by regex; the result matched Go's `vendor/golang.org/x/net/http2/hpack/tables.go` for all 256 octets, and the Kraft sum is exactly 1 (complete prefix code). Lengths/codes are emitted as hex into `lib/hpack_huffman.c` behind `include/hpack_huffman.h` (aedilis links the .c through the header).
- **Static table without casting away const.** -Wcast-qual forbids `(i8*)"literal"`, so the 61 names/values live in one non-const `character` array (concatenated, no NULs) with a table of (start, length, start, length) in hex; `hpack_staticum` hands out views into it.
- **Decoder:** bit by bit, with symbols bucketed by code length (computed once into static arrays from the constant table - deterministic). It does not assume the code is canonical. Padding rule: after the last symbol, at most VII leftover bits and all ones; EOS decoded anywhere is an error.
- **Integer reader** accumulates in i64 and rejects anything above 2^31-1 (the house i32 is unsigned; this also bounds what later code trusts).
- Big Roman numerals are not all defined (`MMMMXCVI` is not) - the test uses `CCLVI * XVI`.
