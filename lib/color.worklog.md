
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
