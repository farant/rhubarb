# auscultatio worklog

## 2026-10-23 - M3: the render as numbers

Two steps by design: reddere (real sound, deterministic) and metiri (pure
over samples), so the measuring maths is tested on synthetic buffers with
exact values (peak 1.25, clipping counts |x| >= 1.0, RMS by bar under a
3/4 + 60 bpm change) while the render is tested by properties
(byte-identical twice, covers the song + tail, a rest bar is silent).

reddere is the offline exitus that used to live in tools/auditio.c (-via)
- one copy now. It renders until the WHOLE song is out AND no voice
lives (so a song ending in rests still has full length), capped at song
+ AUSCULTATIO_CAUDA. The concentus_initium margin at start is CUT so that
frame 0 = tick 0 and bars can be cut straight from the tempo map; the
test pins it exactly (silent before frame 24000, sound in the 480 frames
after). Side effect: Korobeiniki's WAV is 26.750 s, not 26.800.

metiri divides a bar's energy by the FULL bar length (samples beyond the
buffer count as silence). tacitae = bars below -60 dBFS where a track
that SHOULD sound (not muted; if any track is solo, only solo tracks) has
a note - catches an unknown instrument or gain 0. partem renders a
shallow copy of the song with solus moved to one track.

Seen while smoke-testing: aether's release tail is long (the bar after a
whole note still measured -25.8 dBFS), and a solo track's RMS can read a
hair above the full mix when another track holds a SILENT voice (gain 0)
that keeps the render running longer - longer denominator, not a bug.

## 2026-10-24 - tail grows while alive (reverb)

The offline render preallocated song + AUSCULTATIO_CAUDA (2 s) and stopped when full.
That was enough for release tails, but a reverb room of size 0.85 needs ~5 s to fall to
-96 dB, and the first #5 demo WAV ended at -55 dB. Now, when the buffer is full and the
concentus is still alive (which includes mixtura_cauda_vivit), capacity doubles
(piscina allocation + copy) up to song + AUSCULTATIO_CAUDA_MAXIMA (20 s): still a hard
guard against a sound that never ends. Songs without reverb render the same length as
before.

## 2026-10-24 - census at the end of the render; clipping = strictly above 1.0

auscultatio_reddere_censum: the same render, and at the end a CENSUM_PETERE command plus
one more pulse; the CENSUS message is caught in _nuntium_legere (concentus would have
dropped it) and returned. auscultatio_reddere = the _censum variant with NIHIL.
tonsa now counts |x| > 1.0 only: the limiter puts overs exactly on 1.0 (legal 16-bit
maximum), and the old >= would have reported every limited peak as clipping.
