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

## 2026-10-23 - genera CHORDA (Karplus-Strong) et FM; patches 'chorda', 'campana' (wish …P1DXC)

One patch format, a generator kind per patch (TibiaParametra.genus, f32 like the rest).
Old patches keep genus 0 and are byte-identical - FNV-1a of 1.5 s per patch recorded
BEFORE the change (sonda -domus) and asserted in 'vetera' (it passed on the old code
first, proving the test build's flags match). Hand-built patches in the tibia and mixtura
suites now memset first (new fields would otherwise be garbage - genus!).

CHORDA: the delay line lives INSIDE TibiaVox (TIBIA_LINEA 2048 f32 = 8 KB/voice, notes
down to ~23 Hz) so the voice stays a value type with no piscina; mixtura voices are
allocated once. Noise burst seeded from the note (deterministic WAV), DC removed. Filter
y = g(a x[n-D] + (1-a) x[n-D-1]), a = 1/2 + claritas/2, read delay D = mora - (1-a) so the
period is exactly mora (fractional, linear interpolation). Per-cycle gain g from the
decay time, DIVIDED by the filter's and the interpolator's magnitude at the fundamental -
without that the slope ran ~43 dB/s against 40. Decay tests measure the STEADY slope
(from 0.25 s): the noise burst's upper harmonics drop ~8 dB in the first 0.1 s, which is
real plucked-string behaviour, not a decay error. Pitch to +-0.1 sample via parabolic
autocorrelation peak.

FM: carrier = the voice's own SonusUnda phase/increment (glissando, vibrato for free);
modulator increment = carrier increment x ratio each sample; index decays exponentially.
Preset 'campana': ratio 1.4, index 6, 1/e in 0.4 s, attack 2 ms, decay 3 s, no sustain.
'chorda': decay 2.5 s, claritas 0.5, amplitude 1.0 (0.6 measured -40 dBFS RMS as an
inner voice; plucks spend most of their life in the tail).

Not added to the theremin's instrument cycle (proposal said so): both decay by
themselves, a held theremin note would fade out. censura ambitus: chorda 28..88,
campana 48..96.

Plants 10/10 red on the first run - two tests were sharpened BEFORE planting: pitch to
+-0.1 sample (parabolic autocorrelation peak; the integer-lag check could not see the
quarter-sample filter-delay compensation) and FM non-periodicity at the carrier period
(ratio 1 would be harmonic, correlation ~1). Lint renames: TIBIA_FM -> TIBIA_MODULATIO,
lucrum_filtri / lucrum_fractionis, test helpers _congruentia, _efficax (valor efficax =
RMS).
