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

## 2026-10-24 - genus PARTIALES (additive), presets organum / marimba / glockenspiel

Generator 3 = sum of up to TIBIA_PARTIALES_NUMERUS (VIII) sines, each {ratio, amplitudo,
decrementum}. The count macro is TIBIA_PARTIALES_NUMERUS, not TIBIA_PARTIALES as first
proposed: that name is the genus enum value. Each partial's phase step is the voice's
SonusUnda increment x ratio, recomputed every sample, so glissando and vibrato carry
over the same way they do for FM.

- Active partials (ratio > 0 and amplitudo > 0) are packed in order into the voice's
  arrays (rationes[], amplitudines[], detrimenta[], numerus_partialium). Amplitudes are
  divided by their SUM, so peak <= 1 x amplitudo for any phase alignment.
- Per-sample decay exp(-1/(dec*SR)); dec 0 = holds (organ). Decay runs on top of the
  ADSR envelope.
- A partial whose step reaches >= 2^31 (Nyquist) is skipped for that sample. Its phase
  stays put but it keeps decaying. The guard also avoids casting an f64 >= 2^32 to an
  unsigned i32 (UB) when the ratio is 32 at note 127.
- No partials given (book genus="partiales" with no list, no base) = fundamental only.

Presets: organum {1,2,3,4 at 1, .6, .4, .3; hold}; marimba {1, 3.9, 9.2 - the bar's
tuned overtones; dec .6/.15/.05}; glockenspiel {1, 2.76, 5.40, 8.93 - free-bar modes;
dec 1.5/.8/.4/.2}. Tests: single-bin DFT on 110 whole cycles (organum ratios to 0.05,
nothing at 1.5f), marimba overtone dies faster + fundamental at 1/e after its decay
(+-20%), glockenspiel C8 bounded with nothing at the alias 48k - 8.93f, fallback = pure
sine. Organum joins the theremin cycle (sustains; marimba and glockenspiel decay).

## 2026-10-24 - genus FORMANTES (schola), flatus, colum resonans (fistula, nubes)

FORMANTES = the unda (saw by default) into 5 band-pass SonusResonans in parallel, gains
from the Csound "Formant Values" tenor table (a e i o u: F, dB, bandwidth; Q = F/BW).
`vocalis` 0..4, a fraction lerps F, dB and BW between neighbours. The formants don't
follow the note (that is what makes it a vowel). Makeup gain TIBIA_FORMANTIUM_LUCRUM 2.4
measured with a probe (build/plantae_r/sonda.c): unity left RMS ~0.1 x amplitudo; 2.4
gives ~0.23 and peak < 1.2 x amplitudo C3..D5 (D5 is loudest - harmonic 2 at 1174 Hz
sits on F2).

TEST LESSON: "fraction of energy above 1 kHz" is NOT monotone in vocalis between o and
u. Harmonics of a fixed pitch fall into or out of narrow formant bands as the formants
move (at A3 the o/u midpoint F3 = 2650 Hz sits almost exactly on harmonic 12 = 2640).
The interpolation test compares the voice's F2 filter with one built at the midpoint
(700 Hz, Q 10, -15 dB) instead.

flatus (all genera): seeded RNG noise (seed from note, like CHORDA) through a band-pass
at the note (Q 2), gain chosen so its RMS equals a sine's: noise-equivalent bandwidth of
a 2-pole BP = pi/2 x f/Q, so G = (1/sqrt2) / ((1/sqrt3) sqrt(pi BW / SR)). Mixed (1 -
flatus) x + flatus x noise BEFORE the envelope. Filter follows the current note every
32 samples (glissando).

colum resonans (all genera): GRAVIA biquad AFTER the envelope, cutoff = resonans x
2^(motus x env / 12), updated every 32 samples from the previous sample's envelope
value. nubes = saw, 0.8 s attack, 400 Hz Q 4, motus 24 (400 -> 1600 Hz over the swell);
amplitudo 0.25 because Q 4 gives peak ~2.3 x amplitudo.

All three are skipped entirely when 0 (flatus, resonans), so the old presets stay
byte-identical (vetera FNV). Voice keeps copies of flatus/resonans/acumen/motus, plus
lucrum_flatus and involucrum_ultimum - two more fields than the approved header (told
Fran). schola joins the theremin cycle.

## 2026-10-24 - tibia_praeparati_titulus, tibia_generis_titulus

Enumeration for tools (auditio -instrumenta) and one home for genus words (carmen's
parser and auditio's listing used to keep their own tables, and auditio printed
"unda" for partiales before 984564ed). The table of words lives in the function, as
`constans character* constans`.
