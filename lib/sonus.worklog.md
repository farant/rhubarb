# sonus worklog

## 2026-10-20 — S1a: primitives born (sexta)

Spec: `project-specs/sonus-spec.md` (S1). Fran approved the header shape
in conversation ("that looks good to me"): one sample per call
(`_proximum`), value structs in caller memory like `Sors`, 32-bit
integer phase, levels (not bits) for the crusher, one-pole filter only.

**Integer phase (DDS).** `phasis` is an `i32` where 2^32 = one cycle;
unsigned wraparound is defined in C, so the phase never accumulates
float error and needs no `floorf`. Frequency -> increment is ROUNDED
(`+ 0.5`): truncation gives 440 Hz = 439.99998 cycles per second, which
a wrap-count test cannot see (it allows ±1) - so the suite pins
`incrementum == 39370534` for 440 Hz directly.

**Narrow pulses quantize - do not count them at high frequency.** The
first "duty 0 -> 0.01" check ran at 1000 Hz and failed: a 1% pulse is
0.48 samples wide there, and the phase drifts only 16 units per cycle
(48 x increment = 2^32 - 16), so EVERY cycle catches the same integer
number of positive samples - 1000 or 0 depending on starting phase,
never 480. At 100 Hz (4.8 samples) the same trap gives 500 or 400 (the
test hit 400 because the phase carried over from the previous loop). At
10 Hz (48 samples) the count is 480-490. This is real aliasing, not a
test artifact: a very narrow pulse at a high pitch is mostly silence or
a click train. Worth remembering when sonitus sweeps duty.

**Envelope semantics (chosen, not inherited).** Attack and decay are
fixed SLOPES (attack = 1/n per sample), so a retrigger from 0.25 reaches
1 sooner than from 0. Release is a fixed TIME from the current value
(valor/n), so release always lasts `remissio` seconds, even when the
gate closes mid-attack. Both start from the current value: no click on
retrigger. Times shorter than one sample jump at once.

**Frequency 0 = silence** in every form (not the pulse's stuck +1 DC).

**Noise** draws a new value from `sors` each time the top three phase
bits change: eight values per cycle, so solarium's preset frequencies
keep their meaning ("freq pitches the crunch").

**Filter numbers in the suite are analytic**, not measured-then-pasted:
|H| = a / |1 - (1-a)e^-jw| with a = 1 - e^(-2 pi fc/fs). At fc = 1000,
fs = 48000: low-pass passes 0.995 at 100 Hz, 0.7075 at 1000, 0.107 at
10000; high-pass 0.093 / 0.6617 / 0.931.

**Determinism**: same parameters + same seed = bit-identical samples on
the same binary and machine (the suite runs a whole chain twice). `sin`,
`exp`, `pow` come from libm, so goldens across machines may differ in
the last bits (spec §VI-g) - goldens, when they come, compare with a
tolerance or are regenerated per machine.

## 2026-10-24 - SonusResonans (biquad, RBJ cookbook)

Second-order filter next to the one-pole SonusColum: GRAVIA (low-pass), ACUTA
(high-pass), MEDIA (band-pass, constant 0 dB peak - the formant shape). Coefficients
computed in f64 (cos/sin of w0), stored f32, Direct Form I. Q = `acumen` (<= 0 ->
0.70710678). Frequency <= 0 = exact pass-through (b0 1, rest 0); otherwise clamped to
[1 Hz, 0.49 SR]: at w0 = pi the cookbook degenerates (sin w0 = 0, alpha = 0). ponere
recomputes coefficients and keeps the four memory taps, so a sweep doesn't click (tibia
sweeps every 32 samples).

Tests are measured against the analytic RBJ values: |H(f0)| = Q for LP/HP (0.7071 at Q
0.707, 10 at Q 10), BP 1 at f0 and 1/sqrt(1 + Q^2 (f/f0 - f0/f)^2) away (~0.053 at Q 5,
f0/4 and 4 f0), impulse decays at 23 kHz Q 10, finite at 30 kHz (clamped).

## 2026-10-24 - SonusReverberatio (Freeverb)

Jezar's Freeverb (2000, public domain): per channel 8 parallel low-pass-feedback combs
(1116 1188 1277 1356 1422 1491 1557 1617 at 44.1 kHz), then 4 series allpasses (556 441
341 225, feedback 0.5). The right channel adds 23 samples to every delay, which is
where the stereo comes from. Delays are scaled by SR/44100 and rounded (1116 -> 1215 at
48 kHz). The first-echo test pins exactly that: the first non-zero output is sample
1215 (L) / 1240 (R), and a wrong scale moves it.

Mapping as in Freeverb: feedback 0.7 + 0.28 aula (0.98 max, stable), damping 0.4
mollities, input gain 0.015, width mixing own = 0.5 + 0.5 w, other = 0.5 - 0.5 w (w 0
gives L == R exactly, since IEEE addition is commutative). Comb filter memory under
1e-25 is flushed to 0 (subnormals in long tails). All lines are fixed arrays in the
struct (~135 KB), so no allocation; the mixer embeds one.

Tests: L/R correlation < 0.5 at width 1. A survivor check showed that without it,
dropping the comb stereo offset passed (the allpass offsets alone keep L != R).
