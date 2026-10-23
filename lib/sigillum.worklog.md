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

## 2026-10-22 — fast path + ARMv8 SHA2 instructions (~7.8x)

Measured first (probe `build/sonda_partes/sonda_sigillum.c`, gitignored): the old code ran at 228 MB/s on 64 MiB, 227 at 16 KB x 4096 and 215 at 1 KB x 65536. Two costs: `sigillum_addere` copied EVERY byte through the 64-byte `massa` with a test per byte, and the compression was portable C only.

Changes (digests byte-identical; API unchanged plus one new entry):
- `sigillum_addere`: top up a partial `massa`, compress whole blocks DIRECTLY from the input, copy the tail. `sigillum_finire` builds the padding in place (it used to make ~56 one-byte `sigillum_addere` calls). Shared by both paths through `_finire(ctx, compressor)`.
- `_bloccos_comprimere_arm` under `#if defined(__ARM_FEATURE_SHA2)`: sha256h/h2 do 4 rounds per instruction, sha256su0/su1 the message schedule, and the state stays in registers across consecutive blocks. 1787 MB/s against CommonCrypto's 2475 (ceiling; it interleaves two blocks). `arm_neon.h` compiles under the house `-std=c89 -pedantic -Werror` with no extra flags.
- `sigillum_computare_portabile` (new, public): the portable path ALONE, as the witness the tests pin the fast path against.

Silva: `include/postulata_neon.h` + section `postulata_neon.h` in `silva/fontes/systema_posix.h`, following Fran's 10-20 atomics decision (postulate header, not exemption). The section DEFINES `__ARM_FEATURE_SHA2`, so silva takes and EXAMINES the vector branch. Plant: an intrinsic not in the section (`vsubq_u32`) gives REICE. Code 85 counts every section symbol as platform, so `sigillum.c` includes `postulata_posix.h` first. Possible refinement: compiler-postulate sections not counted as POSIX. The intrinsic names are fixed API, so they went into `ignota_symbolorum.txt` (regenerated, +14).

Tests: probatio_sigillum "via portabilis = via ordinaria": lengths 0..1000 at an odd offset, 64 KiB+7, 1 MiB, ~1 MB, and incremental feeding in chunks of 1/7/63/64/65/200 bytes, all equal to the portable witness. Plants: ARM schedule without su1 → red (NIST vectors); tail one byte short → red. Both reverted, green.

Effect: a full `bin/fabrica iudicare -plenus -omnia` went from ~25.5 to ~23 s warm; hashing inside it is now ~0.3 s (it was ~2.3 s). The rest of that judgement is ~70% `poll`, waiting on regenerators: a different target. Radix member rebuilds lose about 0.4 s per member of the ~0.45 s they spent hashing.
