# tibia worklog

## 2026-10-22 - S5a: the minimal instrument voice (pulled forward by the theremin)

Why now: a sonitus voice is an EFFECT - its length lives in its
parameters (impetus + mora + remissio) and its pitch only follows its own
slide/vibrato. The aetherophonum (S5c) needs a note that sounds until
released and whose pitch moves on command: the `tibia` of sonus-spec §V.
Fran 10-22: pull a minimal tibia into S5 rather than hack an endless
sonitus.

Shape: oscillator (sonus_unda) x ADSR (sonus_involucrum, real gate) ->
one-pole low-pass -> amplitude, plus vibrato. Pitch is a fractional MIDI
note; `tibia_notam_ponere(v, nota, quadra)` glides LINEARLY IN SEMITONES =
logarithmic in frequency, so a glide sounds even. The suite pins it: 69 ->
81 over 0.5 s measures ~622 Hz at the midpoint (geometric mean), not 660.
A new glide starts from the CURRENT note (redirect mid-glide, no jump).
Loudness is NOT here: mixtura's per-voice gain ramp (VOCEM_PONERE) already
does it - the theremin's y axis reuses it.

Cost: frequency (powf) is recomputed per sample ONLY while gliding or with
vibrato; a held note sets it once. Phase is the unda's 32-bit accumulator,
so a pitch change never clicks (suite: max sample step <= sine slope).
Noise (forma 4) is clamped to sine - a held note has no pitch as noise.

Presets (C table, STML in S6): aether (sine, soft attack 0.04 s / release
0.25 s, 0.12 st vibrato at 5.5 Hz), quadrans (25% pulse, 6 kHz low-pass),
triangulum (bass).

Measurement: interpolated upward zero crossings - mean over a window, or
instantaneous over three cycles around a sample (enough to see a glide's
midpoint within +-3 Hz). Plants 10/10 red (one needed the single-line
form).
