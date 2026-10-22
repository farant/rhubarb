# sonus_wav worklog

## 2026-10-20 — S1c: WAV writer + audition tool (sexta)

Spec `project-specs/sonus-spec.md` (S1). Fran approved the shape
("excellent! yes lets go ahead"): imago_png's split - pure
`sonus_wav_codificare` (bytes in a piscina, testable without disk) +
`sonus_wav_scribere` (encode + `filum_scribere`, examen 96). Separate
file so `lib/sonus.c` stays free of I/O.

**Bytes are laid out by hand**, little-endian explicit, never by writing
a C struct: host byte order and struct padding cannot change the file.

**Rounding is half-away-from-zero, after clamping**: 0.5 x 32767 =
16383.5 -> 16384 and -0.5 -> -16384 (plain `floor(v + 0.5)` gives
-16383 - the "rotundatio asymmetrica" plant). Out of range clamps;
wrapping would turn a slight overload into a full-scale click. Scale is
symmetric (-1 -> -32767, -32768 unused).

**Golden bytes were computed by hand from the RIFF layout**, not copied
from the encoder's output (48000 = 0xBB80; byte rate = rate x channels
x 2; block align = channels x 2). Mono, stereo, and a zero-frame header.

**Plants 2026-10-20** (silva.planta, 5/5 red then green, source
byte-identical): asymmetric rounding, missing clamp, byte rate without
channels, swapped byte order, write error ignored.

**Glossary**: `wav` added to oratio/glossarium.stml as ignotum-permissum
(like `utf`); `png` lives in the tolerated-unknown copy instead.

**Audition tool** `tools/auditio.c` (aedilis-built, nothing installed):
lists presets, renders one to /tmp/auditio_<name>.wav and plays it with
afplay, `-via` writes only, `-omnes` plays all. macOS `afinfo` reads
the output as "1 ch, 48000 Hz, Int16" with the expected duration;
punctum = 0.135 s, the same as solarium's blip.wav.

**Fran listened 2026-10-20** (auditio, all eight presets + solarium's
blip.wav for comparison): "those all sound good!" - first time the
house made a sound of its own.
