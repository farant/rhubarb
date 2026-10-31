# hpack worklog

## 2026-10-30 - T1: primitives (http2-plan T1)

Integers (RFC 7541 5.1), Huffman strings (5.2 + Appendix B), static table (Appendix A).

- **Appendix B is data, taken from the RFC text, not typed.** `rfc7541.txt` was downloaded and the 257 rows parsed by regex; the result matched Go's `vendor/golang.org/x/net/http2/hpack/tables.go` for all 256 octets, and the Kraft sum is exactly 1 (complete prefix code). Lengths/codes are emitted as hex into `lib/hpack_huffman.c` behind `include/hpack_huffman.h` (aedilis links the .c through the header).
- **Static table without casting away const.** -Wcast-qual forbids `(i8*)"literal"`, so the 61 names/values live in one non-const `character` array (concatenated, no NULs) with a table of (start, length, start, length) in hex; `hpack_staticum` hands out views into it.
- **Decoder:** bit by bit, with symbols bucketed by code length (computed once into static arrays from the constant table - deterministic). It does not assume the code is canonical. Padding rule: after the last symbol, at most VII leftover bits and all ones; EOS decoded anywhere is an error.
- **Integer reader** accumulates in i64 and rejects anything above 2^31-1 (the house i32 is unsigned; this also bounds what later code trusts).
- Big Roman numerals are not all defined (`MMMMXCVI` is not) - the test uses `CCLVI * XVI`.

## 2026-10-30 - T2: dynamic table, decoder, encoder

- **RFC examples as generated data.** `probationes/fixa/hpack/rfc7541_exempla.h` was produced from `rfc7541.txt` by script (hex dumps, header lists, "Dynamic Table (after decoding)" entries, table size); each table's entry sizes sum to the RFC's stated size, so the extraction is checked against itself. Gotcha while extracting: "Authors' Addresses" also appears in the table of contents - use the last occurrence as the end marker.
- **Fixed memory per table.** Entry bytes live contiguously in one buffer of `capacitas` octets ([primus_octetus, finis_octetus)), descriptors in an array of capacitas/32+1; eviction only moves the front, and when an append hits the end the live region is memmoved to 0 (O(4 KiB) at worst). A long-lived connection's churn never grows the arena - important since piscina never frees.
- **Encoder atomicity.** `hpack_codificare` returns 0 with the table untouched when the output buffer is too small: it snapshots the struct and both arrays (shadow buffers allocated with the table) and restores on failure. Costs ~4 KiB memcpy per block; fine at our scale.
- **Index search order reproduces the RFC**: exact match static, then exact dynamic (newest first), then name static, then name dynamic. C.5.2's `:status 307` must use static name index 8 (0x48) even though the dynamic table holds `:status 302`.
- **Size updates:** encoder remembers the smallest and the final requested size since its last block and emits min-then-final (4.2); decoder accepts updates only before the first header and only up to `limes`.
- **Never-indexed** applies to `authorization`, `proxy-authorization`, `cookie` < 20 octets and any caller-flagged header; such headers are always literal (even with a static exact match) and never enter the table.
- `magnitudo` is the house sizeof macro - it bit twice (a parameter name copied from the plan, a fixture struct field).
