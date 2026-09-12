
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

## 2026-09-12 — Task 5 (part A): two defects the new oracles found

### What was shipped wrong, and why nothing saw it

Building I7 (quality against `sips`) and the CLI surfaced two real
defects in code committed in Tasks 2–3:

1. **Every icon was bilinear.** icones requested `IMAGO_SCALA_AREA`
   through `imago_extrahere_et_scalare`, whose dispatch was `si
   (PROXIMUS) … alioquin bilinear` — Task 1 had added `AREA` only to
   `imago_scalare`. Fixed at the source in `4fe5ab6f` (see
   `lib/imago_opus.worklog.md`).
2. **Off-by-one dimensions for any non-power-of-two source.**
   `_quadratum_scalare` passed exact sizes as MAXIMUM bounds, and the
   fit-within arithmetic truncates twice: a 1000 px source gave 15, 31,
   63, 127, 255, 511; a 100 px source gave 15, 31, 63. Measured through
   the CLI's `-legere`. **Its own comment claimed the opposite** ("we
   compute the dimensions OURSELVES and do not ask 'within bounds'") —
   the comment was written knowing the hazard, and the code beneath it
   walked straight into it.

I1–I6 could see neither: none looks at a pixel value, and every fixture
was 64, 256, 512 or 1024 — sizes where the truncation happens to be
exact. Same class as `imago_opus` I2 a: a fixture whose varied property
is constant along the axis that fails.

### The fix: crop once, scale to exact dimensions

`_quadratum_recidere` crops the centred square ONCE (a square source is
returned as-is, no copy), and each size is `imago_scalare(&quadratum,
latera, latera, IMAGO_SCALA_AREA)` — which takes exact dimensions. icones
no longer calls `imago_extrahere_et_scalare` at all.

### I7 measures the INTERIOR — the plan's metric could not work

The plan asked for a ceiling on `delta_maximum`. Measured against the
frozen oracle, straight RGBA cannot separate the modes: max delta area
224, bilinear 255, dominated by partly-transparent disc-edge pixels where
Apple's filter legitimately reaches further (pixel (2,4): `sips` alpha
111, ours 224). Premultiplying did not rescue it either (>32: area 44,
bilinear 46). **Restricted to pixels opaque in both images** (112 of 256):

| vs `sips` | px delta > 16 | max | mean ×100 |
|---|---|---|---|
| area | 0 | 14 | 226 |
| bilinear | 75 | 202 | 2113 |
| nearest | 112 | 202 | 9341 |

Limits: at most 8 differing pixels, mean at most 600. The gate asserts
that bilinear and nearest EXCEED both (calibration kept in the test, not
only in a one-off plant), and that the interior holds more than 100
pixels (an over-masked comparison would pass vacuously). Counting goes
through `imago_conferre` on masked copies, so "differs" keeps the
library's own definition. The edge ring stays covered by `imago_opus`
I2's exact alpha pins.

Colour management was ruled out first: `sips` tagged its output
`sRGB IEC61966-2.1` (the source had no profile), but interior values agree
within 7, so no conversion shifted them.

**The oracle is frozen, not live.** `sips`' output depends on the fixture
alone, never on our code, so `probationes/fixa/icones/generare.py` writes
`fons_256.png` deterministically (stdlib `zlib`/`struct`) and runs `sips
-z 16 16` once. Same discipline as Python-pinned constants. The fixture is
built to falsify: transparent WHITE outside the disc (a halo shows in
every channel), and 1 px black lines every 8 rows — at 16× nearest lands
on a line every time, area shades them.

### I8: exact dimensions, and the crop that was never tested

A 100×100 source must give 16/32/32/64, read from each PNG's IHDR. And a
120×100 source with red 10-column margins and a blue centre must render
64×64 with ZERO red pixels — **centred cropping (spec D1) had never been
tested**, because every fixture was square. The positive twin (exactly
64×64 blue) stops an empty image from passing.

### Calibration: 3 plants, 15 reds predicted, 15 counted

| Plant | Red | Where |
|---|---|---|
| `AREA` → `BILINEARIS` in the new call | 2 | I7 only |
| crop column offset × 0 (left-aligned) | 2 | I8 crop only |
| route back through `imago_extrahere_et_scalare` | 11 | I8 dims (8) + crop size/blue (3); I7 green |

The third plant is the regression that actually happened, re-created on
purpose: with `extrahere`'s `AREA` arm now fixed, it no longer breaks I7 —
only I8 — which is itself the evidence that the two defects were
independent.

### A prediction I got wrong, and why

Before the icones fix I predicted the new tests would show 13 reds; they
showed 11. I7 was already GREEN (interior 112, differing 0, mean 226 —
identical to the probe's area row), because `4fe5ab6f` had fixed
`extrahere` one commit earlier and icones still called it. I predicted
against a picture of the code one commit stale. So I7 was never red
against the icones path until the mode plant made it red — which is why
that plant was not optional.

### Correction to Task 3

Task 3 chose a 512 source for I6 citing "~13 MB of the 16 MB arena". That
premise was wrong: `piscina_generare_dynamicum` GROWS (`est_dynamicum`;
a full block adds one of twice the initial size, or `request + initial`
for an oversized request — `lib/piscina.c` `_allocare_interna`). The 512
choice still stands on its own merit (two duplicate pairs on disk), but
the arena was never the constraint.

### Apple's behaviour, measured for Task 5's remaining gates

- **`iconutil` is a lenient oracle.** Of six corruptions of our `.icns` it
  rejected only a declared length LARGER than the file. It returned
  `rc=0` for: declared length one short; a zeroed PNG signature; an
  unknown chunk code (silently dropped, 9 files out); and **a chunk length
  missing its 8-byte header — one file extracted, success reported**. Our
  `-legere` rejects the first two of those. So I4 must assert file COUNT
  and each file's dimensions, never the return code alone.
- **When `iconutil` builds an `.icns` from our PNGs, `ic04`/`ic05` come out
  as `ARGB` payloads, not PNG** (`ic11` stays PNG; `info` is a `bplist00`).
  Evidence behind spec D9 — and the reason the Finder check needs an
  `ic04`-only bundle. `-legere` now labels them `argb`.
- **Apple re-encodes every PNG, ~2.4× smaller than ours** (our `ic10`
  221,158 bytes vs Apple's 91,157). Our encoder writes filter type NONE.
  Named, not fixed: size, not correctness.

`sips` joined the glossary as `ignotum-permissum` beside `apple`, `icns`
and `iconset`. Probatio: 212 assertions.

## 2026-09-12 — Task 5 (part B): the CLI and the `iconutil` gate (I4)

### `tools/icones_instrumentum.c`

`-fons x.png -radix dir [-titulus AppIcon] [-latera 16,32,…]` writes
`<radix>/<titulus>.iconset` and `<radix>/<titulus>.icns` and prints any
omitted sizes; `-legere x.icns` prints one line per chunk —
`ic04 434 png 16x16`, `ic04 324 argb` (Apple's own 16/32 px payloads),
or `info 318 aliud`. Exit 0 done, 1 refused (cause named), 2 nothing done.
**`stb_image` lives here and nowhere else**: the library still takes an
already-decoded `Imago` (D7), so its closure stays vendor-free. The reader
refuses a bad magic, a declared length that does not match the file, and
a chunk header that is truncated or overruns — before every read. It is
the third copy of the four-line big-endian helper (library writes,
probatio and tool read); a shared home was not worth inventing for four
lines. The chunk reader lives in the TOOL because a library reader is a
§10 deferral.

An unknown pixel size in `-latera` is an argument error (exit 2), not a
library refusal: there is no flag it could map to.

### The gate, `probationes/probatio_icones_iconutil.sh`

Built on the leniency measured in part A, so it never trusts `rc`:

- **0.** Relinks the CLI from the CURRENT `build/*.o` every run. The
  suite's `compile_tool_if_needed` compares the tool binary against the
  tool's own source only, never the library objects — a gate relying on
  it would run a tool linked against yesterday's `lib/icones.c` and
  report today's library green.
- **I. Control:** Calculator's own `AppIcon.icns` must convert, so the
  oracle is proven before it judges ours.
- **II.** A 1024 px source (`sips` upscale of the committed fixture) →
  our `.iconset` (10 files) and `.icns`.
- **III. Direction A:** our `.icns` → `iconutil` → exactly 10 files, each
  at its own size by `sips -g`.
- **IV. Direction B:** our `.iconset` → `iconutil` → Apple's `.icns`;
  `-legere` both; the code SETS must match (Apple adds `info` and orders
  differently), and sizes must agree wherever both carry PNG — at least 8
  such chunks, or "no discrepancy" would prove nothing. A note fires if
  Apple ever stops writing `ic04` as `ARGB` (D9 would need revisiting).
- **V. Adversarial:** declared length +1 — `iconutil` and `-legere` must
  both refuse (the oracle can say no).
- **VI. Adversarial:** a chunk length missing its header — prints the
  measured `rc=0`-with-one-file behaviour as a note, and `-legere` must
  refuse with `CHUNKUS`.

Byte patching uses `perl`, which the suite already requires
(`Time::HiRes`), rather than adding Python to a root gate.

**Registration** (`compile_tests.sh`, after the `plutil` block): unfiltered
runs always; filtered runs only when the filter matches
`probatio_icones_iconutil` (so `icones` and `iconutil` do). The gate
relinks a Cocoa/WebKit tool and makes a dozen `iconutil`/`sips` calls; an
unrelated `./compile_tests.sh stml` should not pay for that. `plutil`
always runs because it is cheap.

### Calibration — and what it revealed about coverage

Plant: the 128 px row's code `"ic07"` → `"ic13"`. **`probatio_icones`
stayed GREEN at 212**; the gate went red with *"iconutil ex .icns nostro 9
plagulas extraxit, X exspectatae (rc=0 non sufficit)"* — `iconutil`
reported success on a mislabelled container, and only the file count
caught it. planta's green re-run was also the gate's first run inside a
filtered suite.

**So the code → size mapping above 64 px is guarded ONLY by I4.** I3 pins
codes for a 64 px source (`ic04 ic11 ic05 ic12`), and nothing in C checks
128 and up. Left that way deliberately: Apple's tool knows Apple's truth,
while a hand-written table in the probatio would restate `ORDINES` from
the same memory that wrote it — self-consistency, not evidence (the
lesson of `manus`' 200-vs-202). The consequence is worth naming: a
machine without `iconutil` (exit 2, a named skip) runs with that mapping
unguarded.
