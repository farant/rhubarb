
## 2026-09-12 — Task 2: the pure core (recorded late, at Task 3)

These were written to memory at the time and belong here too.

- **`_ORDINES` is a reserved identifier** — underscore plus capital is
  reserved to the implementation (C89 7.1.3). **examen caught it;
  clang said nothing.** Renamed `ORDINES`. A clean clang build is not
  evidence of conformance.
- **`imago_extrahere_et_scalare` takes `i32` (unsigned).** Passing the
  `s32` dimensions was six `-Wsign-conversion` errors; explicit casts.
- **The part count was my arithmetic, not the table.** From a 256
  source I predicted six parts; it is seven, because 32 px and 256 px
  each appear twice and both fall inside 256. The count is now
  enumerated in the probatio's comment, not asserted from my head.

## 2026-09-12 — Task 3: the writers and the container

### An API gap the spec carried

`icones_iconset_scribere` receives only an `IconesFructus`, but
`titulus` lived on the `Icones` petitio — so the header's own promise,
`<via_radicis>/<titulus>.iconset`, could not be derived by the function
making it. Neither the spec nor the plan noticed; it surfaced while
writing I6 against the signature.

Fran chose to APPEND `chorda titulus` to `IconesFructus` (`reddere`
copies it from the petitio) rather than make `via_radicis` the full
directory path. Reasons: the `.iconset` suffix is a format requirement
(`iconutil` refuses a directory without it), so the library should own
it; and under the alternative `Icones.titulus` would be a field
`reddere` insists on and nothing reads. No signature changed. The
asymmetry that remains — `icones_icns_scribere` takes a full `via` —
was already in spec §6. Spec §3's struct predates this; Task 5's
as-built corrects it. Decretum filed in the tabularium.

### Why not `chorda_aedificator`, which the plan named

Its own header says it is for "quando magnitudo finalis ignota" and
for structured TEXT; it has no raw-byte append. The `.icns` total is
known before a byte is written — `VIII + Σ(VIII + payload)` — so the
container is computed, allocated once, and filled by an
offset-returning `_ordinem_scribere`. That is `lib/imago_png.c`'s
shape exactly; PNG is also a chunked container with big-endian
lengths.

imago_png ends with a size self-check (`sedes != png_mensura` →
refuse). **Not carried over**, deliberately: a mismatch has no honest
`IconesStatus` to refuse with, and inventing one mid-task widens the
API. I3's independent walker catches the "wrote less than declared"
direction; the "wrote more" direction is a buffer overflow that a check
after writing cannot prevent anyway.

The chunk length written (`(i32)VIII + octeti.mensura`) and the offset
returned are computed SEPARATELY on purpose. A fault in the length then
fools a reader without corrupting the buffer — which is what made plant
one a planted lie rather than a planted overflow.

### Byte comparisons use `memcmp`, explicitly

A PNG carries NUL bytes eight in: the IHDR length is `00 00 00 0D`. Any
comparison that stops at NUL would judge only the signature and pass.
I did NOT read `chorda_aequalis` to learn whether it would; I
sidestepped it with a local `_octeti_aequales` whose semantics are on
the page. `filum_scribere` / `filum_legere_totum` WERE read: `"wb"` and
`"rb"`, so the on-disk claims rest on the bytes.

### I6 asserts nine files, not the spec's ten

Ten files needs a source ≥ 1024 px, and a 1024² fixture is roughly
13 MB of arena on its own (4 MB image, ~4.2 MB filtered raw, deflate
output, the PNG buffer) inside the probatio's 16 MB pool. A 512 source
costs ~4 MB, emits nine files, and is STRONGER evidence: it reaches two
duplicate pairs on disk (32 px and 512 px) where 256 reaches one. The
omission of 1024 is asserted, not left implicit.

### Calibration: three plants, three disjoint targets

| Plant | Fault | Red | Where |
|---|---|---|---|
| 1 | chunk length omits the 8-byte header | 13 | I3 only |
| 2 | `.icns` twin writes one byte short | 1 | I6 `:429` only |
| 3 | every `.iconset` file gets part 0's bytes | 8 | I6 per-file loop only |

Each count was PREDICTED before its run and matched.

**Plant one.** The walk stopped at offset 755 having read one chunk:
8 (the `icns` header) + 747 (chunk 0's payload, now written without
its header). That lands eight bytes before chunk 1, where the walker
reads the last eight bytes of a PNG as a chunk header. I infer those
are `IEND` + CRC `0xAE426082` — imago_png's pinned constant — which as
a length is about 2.9 GB, so the guard breaks the walk. **Inferred, not
observed**: the acta does not print the length read. The passing checks
corroborate it: `codices[0]` read `ic04` and chunk 0's PNG signature was
intact, only its length was short.

**Plant two — a completed-but-wrong write is invisible to status.**
Under the plant `icones_icns_scribere` still returned `VERUM` with
`ICONES_SUCCESSUS`; those assertions stayed green. Only the byte
comparison of file against pure output saw it. That is the measurement
behind the pure/twin check plist established, not just the habit.

**Plant three — the finding of this task.** Writing part 0's bytes into
all nine files reddened the per-file loop eight times, and BOTH
duplicate-pair checks stayed GREEN: when every file is identical, every
pair is identical. The pair checks read like the D6 evidence, and alone
they would pass a writer emitting one image nine times. **Equality
between outputs proves consistency, not correctness; each output must
be compared against its own source.** Same family as "a test that
varies one field proves one field is compared".

### `via_iungere`, as read

A plain join: no normalization, no cwd resolution, one `/` added only
when neither side has one — and it **silently skips empty parts**. So a
part with an empty `semita` collapses to the directory path itself, and
`filum_scribere` then fails with `_SCRIPTIO`. Legible, not silent; a
Task 4 case.

### For Task 4, found by reading (not yet run)

- **`reddere` returns `VERUM` with ZERO parts** when every requested
  size exceeds the source (e.g. `latera_petita = ICONES_LATERA_MXXIV`
  from a 256 source): `_MINIMUS` only checks side < 16. That is exactly
  the "success that produced no icon" D10 exists to refuse.
- **The writers have no argument guards** (`fructus`, `via`,
  `via_radicis` NIHIL dereference) — left out so Task 4's tests are
  born red.
- **`si (!pars)` → `_MEMORIA` in the writers is unreachable by any
  test** (`xar_obtinere` within range). The plan's "a plant per refusal
  branch" cannot cover it; Task 4 should keep or remove it on purpose.
