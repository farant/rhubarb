# imago_png worklog

## 2026-08-13 — birth, and a lesson about lenient oracles

Built after Fran asked whether PNG encoding was behind a platform
layer. It wasn't — and the answer turned out to be that it shouldn't
be one.

### Why no platform layer

Encoding was inline AppKit, duplicated at two sites
(`lib/vitrea_macos.m:855-890`, `tools/vitrea_spica.m:378-395`), both
running `NSImage → CGImage → NSBitmapImageRep → PNG → writeToFile`.

The seam was drawn in the wrong place. **The platform boundary belongs
at pixel acquisition, not at file format.** "Capture the contents of
this window" is genuinely macOS-bound; "serialize RGBA to PNG" is
arithmetic. AppKit was doing both, which is exactly why the encode
logic got copy-pasted — there was no portable place to put it.

We already owned the hard parts: `flatura_deflare` (RFC 1951) and
`flatura_crc32` (IEEE 802.3 — the same polynomial PNG uses). The
encoder needed only Adler-32 (~25 lines) and chunk framing. No
dependency, no platform code, works under `porta_linux.sh` on day one.

Worth noting the asymmetry that justifies keeping stb_image for
decode: **encoding writes one path; decoding must read all of them**
(bit depths, palettes, interlacing, five filter types, plus
JPEG/BMP/TGA). Encoding portable, decoding vendored, deliberately.

### Format choices

Colour type 6 (RGBA8), filter None on every row, no interlacing, one
IDAT. Output is **deterministic** — same image, same bytes, always.
Test artifacts that vary run-to-run become their own debugging
problem.

Filter None costs file size on photographic content (PNG's five
filters exist to help compression). Accepted deliberately: for
screenshots, which are mostly flat regions, it barely matters, and
adding filters later is a pure implementation change behind an
unchanged interface. That is the Eskil bet paying rent.

### THE LESSON: a lenient oracle makes a lenient test

The round-trip test decodes our output with **stb_image**, an
implementation we did not write. That felt like a strong oracle, and
I said so. It is not strong enough, and I only found out by planting
a fault:

| poison | fracti (before §IX) | fracti (after §IX) |
|---|---|---|
| Adler-32 + 1 | **0 of 54** | 1 |
| every chunk CRC + 1 | — | 2 |

**stb_image validates neither Adler-32 nor chunk CRCs.** It is written
to read real-world files, many of which have broken checksums, so it
is permissive by design. Fifty-four green assertions were compatible
with emitting a file that strict decoders would reject.

The fix was not a better round trip — it was a **second oracle of a
different kind**. Section IX pins three constants computed by Python's
zlib:

- IHDR chunk CRC for a 4×3 image — `0xB4F4AEC6`
- Adler-32 over the raw scanlines — `0x3D9E0D87`
- **IEND chunk CRC — `0xAE426082`**

The last one is the nicest: it is the same value in *every* valid PNG
ever written, because IEND has no data. Pinning it catches a wrong
polynomial, wrong initial value, wrong final XOR, or a CRC computed
over the wrong byte range — four distinct failure modes, one constant.

Generalised: **ask what an oracle is permissive about before treating
its silence as a verdict.** A decoder built for the open web forgives
things a spec does not.

### One-time external audit

Beyond the pinned constants, a real emitted file (32×16) was parsed by
a strict Python reader: signature, every chunk CRC recomputed, exact
file length with no trailing bytes, IHDR fields, `zlib.decompress`
(which verifies Adler), raw length, every filter byte, and all 512
pixels compared. Result: **PLENUM**, zero differences. `file(1)` also
reports `PNG image data, 32 x 16, 8-bit/color RGBA, non-interlaced`.

Not automated — it needs python3, and the suite must stay dependency
free. The pinned constants are the permanent gate; this was the
one-time confirmation that the whole container is well formed.

### Notes

- `_adler32` reduces every 5552 bytes (the canonical NMAX). That value
  is chosen so the accumulators cannot overflow 32 bits between
  reductions; it is not arbitrary and should not be "tidied".
- zlib header is `0x78 0x9C` — the second byte is constrained: the
  16-bit value must be divisible by 31.
- The CRC covers **chunk type + data**, not the length field. Getting
  that wrong produces files most decoders still accept, which is
  precisely why the IEND pin earns its place.
- `imago_png_codificare` is pure (no I/O), so it is testable without
  a filesystem; `imago_png_scribere` is the thin convenience on top.
