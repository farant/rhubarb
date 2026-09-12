
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

## 2026-09-12 — Task 4: refusals (gate I5)

### What the red run showed, observed rather than read

Tests first, against the Task 3 code. Every red named a real behaviour:

- missing `petitio` / `fructus` / `piscina` → the right status with an
  EMPTY `sedes_vitii` (nothing named);
- `fons` NIHIL → `ICONES_ERROR_FONS`, although the enum's own comment
  says `DESUNT /* fons aut titulus */` — the code contradicted its
  header;
- `pixela` NIHIL, `latitudo` 0 and `altitudo` 0 → all three named
  `"fons"`: three faults, one name;
- an unknown size bit named as the WHOLE request (`129`, not `128`);
- every requested size larger than the source → **`VERUM` with zero
  parts** — D10's forbidden empty success, now measured;
- then SIGSEGV (exit 139) at the first NIHIL writer argument, which
  swallowed every later assertion, as the plan predicted.

Two cases were already GREEN before implementation — `titulus` and the
12 px source. A test that passes before the change cannot show it
reaches its branch; plants R05 and R10 are what prove those two.

### Decisions

- **Nothing requested is covered → `ICONES_ERROR_MINIMUS`** (Fran),
  with `sedes_vitii` naming the requested pixel sizes, `"128 1024"`.
  Same failure `_MINIMUS` already reports — the source cannot cover
  what was asked — so no new status. Built with `chorda_aedificator`,
  which is exactly its job here: text of unknown length. (The `.icns`
  container refused it for the opposite reasons.) A positive twin pins
  the boundary: ONE covered size plus one uncovered still SUCCEEDS with
  omissions, and `sedes_vitii` is cleared after that success.
- **`fons` absent = `DESUNT`, present-but-broken = `FONS`**, each broken
  field named (`pixela`, `latitudo`, `altitudo`).
- **Unknown bits name only the unknown bits.**

### Field names live in static arrays

`sedes_vitii` must name the field even when `piscina` is the missing
argument, so the names cannot be arena allocations. String literals are
`const` under `-Wwrite-strings`, and `-Wcast-qual` forbids casting that
away into `i8*` — so the names are mutable `hic_manens character[]`
arrays, wrapped by `SEDES_FIXA(tabula)`. That macro uses
`magnitudo(tabula) - I` for the length, so it is correct ONLY on an
array: handed a pointer it would silently yield `sizeof(char*) - 1`.

### One shared check for the three writers

`_fructum_iudicare` runs before any writer touches disk: fructus NIHIL,
partes NIHIL or empty, a part whose semita the table does not know
(`LATERA`, naming the semita), a part with empty bytes (`DESUNT`,
naming the semita). A hand-built fructus could otherwise produce a
VALID but empty `.icns` or `.iconset` — a blank icon with no error, the
exact failure this library exists to prevent. The empty semita from
Task 3 lands here as `LATERA` instead of a confusing `SCRIPTIO` on the
directory path. The writers' three `si (!pars)` checks were deleted;
one remains inside the check, commented as unreachable.

### Branches with NO plant, named so the claim is not overstated

Every `_MEMORIA` refusal (xar creation, scaling, part append, path
building, the container allocation, `pars` NIHIL in range) and
`_ERROR_PNG` (its input is already validated) can only fire on
allocation failure or impossible input. No test reaches them, so no
plant can prove them. "A plant per refusal branch" means per REACHABLE
branch: 28 of them.

### Test design worth reusing

- **`RECUSATIO(redditum, sperata, sedes_sperata)` is a MACRO, not a
  function**, so `__LINE__` names the case line; a helper function
  would put every red on one line inside itself. Non-variadic, so
  examen accepts it.
- **Refusal-before-write is proven against a WRITABLE area.** The
  "file not created" assertions only mean something because a positive
  twin then writes successfully to the same path; against an
  unwritable path "not created" would be vacuous.
- **I/O faults without `chmod`:** a regular FILE as a path parent makes
  mkdir -p fail (`DIRECTORIUM`); a DIRECTORY occupying the first file's
  name makes `fopen("wb")` fail (`SCRIPTIO`). Both hold even as root,
  and neither leaves a mode to restore if the run dies — unlike
  fasciculum's 0500 directories.

### Calibration: 28 plants, 28 matched, 35 reds predicted and counted

Plant form: swap the branch's status for a sibling status (equal token
count, no crash), so it can go red ONLY if a test reaches that branch.
The no-channel guards (`!status || !sedes_vitii`) were planted
`FALSUM` → `VERUM` (2 reds each); `codificare`'s `status`-NIHIL guard
by returning a non-empty chorda.

**The multi-site reds are the evidence the check is shared.** F02
(partes) reddened `icns_scribere` twice and `iconset_scribere` once; F03
(unknown semita) reddened `icns_scribere`, `icns_codificare` and
`iconset_scribere`. So each writer's CALL into the check is proven by a
red, not assumed from reading the code.

Not separately proven: the `sedes_vitii` assertions. A status swap
proves reach and status precision; sedes precision rests on the literal
assertions themselves.

### Two tool facts

- **`silva.planta` has no occurrence selector** — it calls
  `Editio.replace` with the default count, so an anchor must be unique
  text. The `si (!status || !sedes_vitii)` guard is identical in four
  functions; the anchors take the token that follows it (`si (!petitio)`,
  `si (!via_radicis)`, `si (!via)`). planta's default `tolerans` matches
  token sequences across line breaks, so a flat anchor survives the
  formatter re-wrapping.
- **A MULTI-LINE replacement with a different token count IS accepted**;
  only a FLAT one with a different count is refused (`forma
  perderetur`). That is how the `codificare` plant returned a
  non-empty value.

The formatter flags lines over 72 columns but cannot wrap them: six
needed hand edits, and the terminal tail hid the one in `lib/icones.c`
— the full `-vitia` log named it. Probatio: 180 assertions.
