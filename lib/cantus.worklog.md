# cantus worklog

## 2026-10-21 - S4 step 1: the MML reader (cantus_legere)

Fran asked whether a jingle could be sent as a text string. Everything
under it already existed (`sonus_notae_frequentia` MIDI -> Hz; mixer
commands carry a frame `momentum` and a full `SonitusParametra`), so the
missing piece was only the notation. This pulls the first slice of the
S6 sequencer forward: text -> notes in frames, pure, nothing sounds.

**Dialect.** A small MML (BASIC `PLAY`, 1981; the Japanese chiptune
lineage, which is also sfxr's): letters + `#`/`+`/`-`, lengths 1..64 with
up to two dots, `r`, `o`, `>`/`<`, `l`, `t`, `v`, `@preset`. Spaces and
`|` are ignored so bar lines can be written. `o4 c` = MIDI 60 (scientific
pitch). `>`/`<` may leave 0..8; only the final MIDI 0..127 is judged, so
`o0 < c` (MIDI 0) and `o8 > g` (127) are legal and `o8 > g#` is an error
at the note. `@titulus` accepts [A-Za-z0-9_] as written; the caller
resolves it - the reader knows no preset names, so it stays pure.

**Time without drift.** Position counts in ticks, 768 per whole note
(enough for a double-dotted 64th = 21 ticks; x3 headroom for triplets
later). A frame is `round(ticks x 48000 x 240 / (768 x t))` measured from
the start of the current tempo segment; `t` closes the segment at its own
rounded frame. Never a sum of rounded lengths: the suite plays 1000
sixteenths at t133 (5413.53 frames each) and asserts note 2 at 10827
(summing gives 10828) and the total at 5 413 534. Rounding is
`(2n + d) / 2d` on s64; overflow is out of reach (text length x 768 x
11.5e6 < 2^63).

**Dots stay integral.** A dot adds `basis >> k`; if the base is not
divisible (a dotted default that is already an odd tick count) the
reader refuses ("punctum longitudinem non dividit") rather than round.

**Two passes, one rule.** `_currere` runs once with `notae == NIHIL` to
count and judge, then again into an exact piscina array. The second pass
cannot fail - same text, same state - so a failure never leaves a
half-filled melody; on error `notae = NIHIL, numerus = 0`.

**Numbers are capped while read** (stop accumulating at 100 000) so
`t99999999999` is "numerus nimius" at the number's offset, never a wrap.

Every error carries the byte offset where the reader stopped and a
static Latin cause - meant for a melody Claude writes (ledger wish
…RJSBFQ): fail with a pointer, never play half a jingle.

Suite: probatio_cantus (credo v2, 5 sections, 199 asserts) - green on
the first implementation run.

**Plants 2026-10-21 (silva.planta, radix/cantus, 9/9 red, source
byte-identical):** rounding -> truncation (drift section), tempo segment
not closed, second dot = half, third dot admitted, octave without +1,
MIDI limit 128, double sharp, offset of the note's end instead of its
start, t401 admitted. Two of the nine first failed on MY anchors (one
ambiguous, one cut inside a string literal) - planta refused them
instead of planting, as it should.

## 2026-10-22 - S4 step 2: notes -> mixer commands (cantus_iussa_parare)

One `MIXTURA_IUSSUM_SONITUM` per note, each on its own voice, so a
release tail can overlap the next note. The note supplies pitch
(`sonus_notae_frequentia`), start (`forma->momentum + initium`) and gain
(`v` x `forma->lucrum`). The seed is `forma->semen + index`, so `varietas`
differs per note but stays deterministic.

**tenere replaces "mora = length - release".** The first idea was to
squeeze the release into the written length. The step-2 design
turned it into a flag on the form: VERUM is a synth gate (the note holds
`longitudo - impetus`, clamped at 0, then the colour's own release tail),
FALSUM leaves the colour untouched (bells, plucks: the note gives only
pitch and onset). A 64th shorter than the attack gets mora 0, never a
negative sustain.

**Resolve every colour before claiming any voice.** `MixturaRector` has
no release call; a voice only frees on VOX_FINITA. If `@ignotum` failed
after the first two notes had claimed voices, those slots would stay
busy forever on a rector nothing will play. So pass 1 resolves all colours
into `iussa[i].parametra`, and pass 2 claims voices. Pass 2 must copy the
parametra out before `mixtura_iussum_parare`, which zeroes the whole
command. A null handle (all voices busy at higher priority) means that
note stays silent, as for any mixer sound: not an error.

`cantus_praeparatum_sonitus` copies the chorda (not NUL-terminated) into a
64-byte buffer for `sonitus_praeparatum`; names over 63 bytes are unknown,
never truncated into a match ("radiu" does not find "radius").

**The render test is exact, not approximate:** "t120 c8 r8 e8" through a
real mixtura must equal, sample for sample, a direct `sonitus_reddere` of
each note placed at its frame times the centre-pan gain 0.70710677. That
pins onset to the sample and silence everywhere else.

Suite: 9 sections, green on the first implementation run. Plants 11/11
red then green (mora sign, clamp, tenere ignored, seed index, momentum
base, form gain, form group, pitch +1, caller resolver ignored, voices
claimed before the error, colour used for '@'); one first draft did not
compile (unused resolver) - planta refused it, redone in a compiling form.

## 2026-10-22 - S4 step 3: `auditio -cantus` (first consumer)

`tools/auditio.c` gained `-cantus TEXT [-praeparatum T] [-intactus]
[-via x.wav] [seed]`: live through `auditus` by default, offline through
`mixtura` into a stereo WAV with `-via`. Same commands either way.

Lessons from the first consumer, for vicus (step 4):
- **Voices = notes.** `cantus_iussa_parare` claims every voice at once.
  At equal priority the rector steals the OLDEST voice, which is a note
  that has not sounded yet. So the mixer gets voices = notes (power of
  two, >= 16) and command/message rings to match (messages 2x voices).
  A long-lived host (vicus) with a fixed mixer must queue melodies or
  cap their length; that is the S6 sequencer's job.
- **Prepare at momentum 0, shift when sending.** Commands are built
  before the device opens; live adds `now + latency + 50 ms` to each
  `momentum` when it sends. A resolution error therefore never opens
  the device.
- **End = last voice finished, not written length.** The exact end is
  `max(momentum + sonitus_longitudo(parametra, semen))`; offline renders
  exactly that many frames (the WAV for the arpeggio is 0.4767 s = the
  printed tail end). Live waits for VOX_FINITA from every non-null voice,
  with a hard cap of end + 1 s.
- Errors print the text with a caret under the offending byte.

## 2026-10-22 - S6a: musical time in ticks (960 per quarter) + tempo map

Why (sonus-spec VIII-d, Fran 10-22): a music-authoring pane, agent
authoring and a future sheet-music view all edit in MUSICAL time, so notes
now carry ticks and a song has a tempo map; frames are derived only at
playback. Resolution moved 768/whole -> 960/quarter (3840/whole), the
DAW / MIDI / MusicXML convention: divides by 3, 5, 6, 15 (quintuplets).
Frames depend only on the ratio, so every existing frame assertion stayed
green unchanged - the regression proof for the switch.

- CantusNota.pulsus / pulsus_longitudo; Cantilena.pulsus, tempora,
  numerus_temporum. The reader keeps `pulsus_totus` (absolute) beside the
  per-segment `pulsus`; a `t` at the position where the last segment
  starts (start of text, or `t` twice) MODIFIES that segment instead of
  adding a zero-length one. Two passes, as before: pass 1 counts notes AND
  segments, pass 2 fills both arrays.
- One rounding rule for reader and map: `_quadra_segmenti(pulsus,
  tempus)` = (2n + d) / 2d. `cantus_tempora_computare` derives segment
  start frames exactly as the reader closes segments, so
  `cantus_quadrum_pulsus` == each note's own frame (suite checks 5 melodies
  incl. tempo changes and the drift case).
- `cantus_pulsus_quadri` (frame -> largest tick not later): within a
  segment, max dp with round(dp a / b) <= r is floor((b(2r+1) - 1) / 2a).
  The "- 1" matters only at exact rounding ties (t240: 12.5 frames per
  tick) - a plant dropping it survived the round-trip loop; a tie case
  pins it now.
Plants 11/11 red.

## 2026-10-23 - M2a: '|' bar marks are recorded

'|' no longer counts as whitespace: the reader records each one as a
CantusLinea {pulsus (TOTAL, not per tempo segment), offset} in
Cantilena.lineae - two passes like notes and tempo changes (count, then
fill; NIHIL when there are none). Playback ignores them; they are facts
about the TEXT so censura (M2b) can say "your '|' at offset 41 lands an
eighth before the barline". Test asserts the frames are identical with
and without marks. Plants 5/5 red (+2 again after a formatting rewrite to
a local pointer): segment pulse instead of total is caught only because
the test puts a 't' change before a mark.

## 2026-10-23 - cantus_status_ad (E2 of edit-by-position)

State after a PREFIX [0, offset): the same lexer (`_currere`) in its counting pass (notae /
tempora / lineae NIHIL) over a chorda that is the text truncated at offset - no second
parser, nothing allocated. An offset inside a token reads that truncated token ('l|8' ->
'l' without length = error), which is the honest meaning of "prefix". `<` / `>` are NOT
range-checked by the lexer (only the next note's MIDI is), so status.octava can be -1 or 9.
`lucrum` is an f32 inside the lector; status rounds `lucrum * C + 0.5f` - of 0..100 only
53 and 59 truncate wrong (0.59f * 100 = 58.999..), so the test uses v59 (v33 let the
truncation plant survive). MML has no way to CLEAR '@' once set - emendatio relies on that.
