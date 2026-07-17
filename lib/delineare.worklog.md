
## 2026-07-17 — the dead clamp (found by the severa tier's first pass)

`cohibere(i32 valor)` — i32 is UNSIGNED, so `valor < ZEPHYRUM` was
tautologically false: the lower clamp was dead code since birth.
Every negative sum feeding it (dark pixel + negative Floyd-Steinberg
error, or Bayer threshold in [-128,112]) wrapped to ~4e9 and clamped
HIGH: white speckles in shadows across ALL error-diffusion and Bayer
paths. The `(i32)` casts on the error terms at call sites were the
-Werror-silencing pattern hiding it — the cast made the compiler
quiet and the wraparound permanent. Fix: cohibere takes s32; all 9
call sites feed genuinely signed arithmetic ((s32)ideal + error);
threshold declared s32 (bayer*16-128 is honestly signed now).
Found by adjudicating the strict subtraction rule (TOLERA arc):
the site fired severa, the eyeball found the dead clamp behind it.
Visual verification available via bin/probatio_delineare (GUI).

## 2026-07-17 — VANA class 4: the residual dead clamps + signed radii

The 11 remaining codex-58 rows adjudicated (quaestio 01KXQ9NF8J):
- Circle/ellipse radius params (circulum, circulum_plenum, ellipsin,
  ellipsin_plenam) -> s32: the `radius < 0` rejects were dead, and a
  wrapped-negative radius meant a ~4e9-iteration draw loop (hang),
  not a no-op. Circle midpoint vars x/y/error went s32 too — error
  is a SIGNED error term by construction; as i32 the `error <= 0`
  branch degenerated to `error == 0` (subtly misshapen circles since
  birth). Coordinate args now use the ellipse bodies' established
  pattern: (i32)((s32)centrum_x + x).
- ponere_praecisionem x/y -> s32: the MAXIMUM(0, x) clip-rect clamp
  was dead (author clearly expected negative positions).
- Rounded-rect radius clamps: dead lower clamp DROPPED, radius stays
  i32 — the upper clamps (radius > latitudo/II) already catch
  wrapped values and bound the domain safely.
- invenire_colorem_per_cinereum: distantia -> s32; the absolute-value
  branch was dead, so any palette entry BRIGHTER than the target
  produced distance ~4e9 and could never win — nearest-gray palette
  mapping was biased to darker entries.
Callers touched: elementa.c (2 cast sites), probatio_tempus.c
(radius decl). GUI verification: bin/probatio_delineare.
