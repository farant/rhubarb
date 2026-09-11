# moneta worklog

## 2026-09-10 — nanoid (`moneta_nanoid`, `moneta_nanoid_plagulae`, `moneta_nanoid_alphabeto`)

**Decision (Fran):** nanoid lives in moneta beside ULID — one mint for
identities, not a separate `nanoid.h`. Three functions: the default
nanoid (21 chars from `0-9A-Za-z_-`), a file nanoid (12 chars from
`0-9A-Z`), and a configurable one (length + NUL-terminated alphabet).
The two fixed variants just call the configurable one.

**Design notes**

- **Uniform sampling = mask + reject** (nanoid's own algorithm). Each
  random byte is masked to the smallest `2^k - 1` covering the last
  index; indexes past the end of the alphabet are thrown away. Modulo
  would bias: 256 = 7 x 36 + 4, so in a 36-char alphabet the first
  four characters come up 8/256 and the rest 7/256.
- **Batch size** follows nanoid's step formula (1.6 x mask x length /
  alphabet size, clamped to 256) so one `/dev/urandom` read usually
  suffices.
- **No fallback randomness.** Built on `moneta_octeti_fortuiti`, which
  returns FALSUM rather than degrade. ULID may fall back because its
  timestamp + monotonic counter still guarantee uniqueness inside a
  process; a nanoid is nothing but randomness, so a predictable
  fallback would mint duplicates. On any failure `effusio` becomes
  `""` — never a partial id.
- **Duplicate alphabet characters are refused** (they silently skew
  the distribution). This also bounds the alphabet at 255 (distinct
  non-NUL bytes), so there is no separate length cap.
- **Default alphabet in readable order**, not nanoid's
  `useandom-26T198340PX...` order. That order exists to help gzip on
  JS bundles; the distribution depends only on the SET of characters.
- **File variant is uppercase-only** because APFS is case-insensitive
  by default (`aB…` and `Ab…` would be the same file), and has no `-`
  or `_`, so a filename never starts like a command-line flag.
  12 x log2(36) ~ 62 bits.
- Local is named `locus`, not `index`: macOS `<string.h>` pulls in
  `<strings.h>`, which declares the legacy BSD `index()` function.

**Tests** (`probationes/probatio_moneta.c`, 21 -> 66 assertions)

- Both fixed variants: two calls differ; 200 calls all have the right
  length, only valid characters, and every alphabet character appears
  (~66 sightings each expected). Valid characters are checked with
  ranges written out in the test, NOT via the alphabet macros — a
  truncated macro must not drag its own test along with it.
- Uniformity over 144,000 characters from a 36-char alphabet (below).
- Extreme alphabets: `"01"` (mask 1, nothing rejected), `"012"` (mask
  3, index 3 rejected — otherwise a NUL lands mid-id and strlen sees
  it), 255 distinct bytes 1..255 (mask 255; high bytes go through the
  `(i8)` cast into the seen-table).
- Write bound: length 5 writes 5 chars + terminator and nothing past.
- Refusals (NULL buffer, NULL alphabet, length 0, 1-char, empty,
  duplicate) return FALSUM and leave `""`. Positive twin `"ABC"` on
  the same buffer; `"ABCA"` differs from it only by the duplicate.

**Uniformity math.** P = count of the first four characters ~
Binomial(144000, 1/9): mean 16000, sigma ~119. D = 9P - 144000 is 0
+- ~1073 under uniformity; modulo gives P = 18000, D = 18000. The
threshold |D| < 8000 is ~7.5 sigma — never fails by chance, always
catches modulo.

**Calibration (planted fault).** Replaced the mask with
`locus = octeti[k] % mensura` (builds clean, `larva` still used).
Exactly ONE assertion failed: D at `probatio_moneta.c:286`; the other
65 stayed green. So **D is the only load-bearing guard against modulo
bias** — the per-character band 3400..4600 does not catch it (modulo
puts the first four at ~4500, inside the band). The band exists for a
mask that is too SMALL (last characters never appear). Don't delete D
thinking the band covers it.

**Vocabula.** `nanoid` entered `oratio/glossarium.stml` as
`ignotum-permissum` (a foreign algorithm name, like `uuid` / `sha`).

**briar / silex cannot see it yet (measured).** A scratch thistle
calling `moneta_nanoid_plagulae` fails in `~/.bin/briar` with
`implicit declaration of function 'moneta_nanoid_plagulae'`: the
corpus is BAKED into the binary (`tools/corpus_infixum.sh`), so new
lib functions reach thistle scripts only after the corpus is rebaked
and briar + silex are rebuilt.
