
## 2026-07-17 — interpolate rounding drift (severa adjudication find)

color_interpolate's t is i32 (unsigned): `(b.r - a.r) * t` on
darkening lerps (b < a) sent the negative diff through unsigned
UAC — the wrap is linear so the final (i8) truncation ALMOST
recovers it, but the intermediate unsigned division rounds toward
-infinity while signed C division truncates toward zero: off-by-one
on non-exact darkening steps. Fixed to explicit s32 arithmetic
(behavior now matches the plain-reading semantics). probatio_color
63+123 asserts unchanged. The fixture copy fixa/roundtrip/color.c
was an exact harvest — resynced.
## 2026-10-13 - spatia colorum (lux linearis, OKLab)

Added for the pattern-dithering mode (dithering_exemplaria, from the
scratchpad experiments of the same day), but generic: any mixing of
colours (dithering, gradients) must happen in LINEAR light - a black/white
checkerboard reads as sRGB ~188, not 128 - and comparison in OKLab.

- OKLab coefficients are Ottosson's published 10-digit matrices. They are
  NOT exact inverses of each other: linear -> OKLab -> linear round trip
  errs by up to 2.6e-7 (measured over the 27-point 0/0.5/1 cube). The
  test tolerance is 1e-6 for that reason; 8-bit precision is ~2e-3, so it
  never matters for pixels. Do not "fix" it with tighter tolerance.
- Negative linear channels (error diffusion pushes targets outside the
  gamut) are clamped to 0 before the cube root; C89 has no cbrt, so the
  root is pow(x, 1/3) guarded for x <= 0.
- color_octetus_ad_linearem uses a lazily filled 256-entry table (no
  threads in the house, so the hic_manens flag is safe).
