
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
