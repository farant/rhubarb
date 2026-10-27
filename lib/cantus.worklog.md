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

## 2026-10-24 - tempus in millesimis (praeparatio agogicae, ...4939S)

CantusTempus gained `millesimae` (sounding tempo in thousandths of a quarter per minute);
the frame math (_quadra_segmenti, cantus_tempora_computare, cantus_quadrum_pulsus,
cantus_pulsus_quadri) reads ONLY that field. `tempus` stays the whole number ('t',
debugging). Every producer fills both: the MML reader (tempus x M), carmen's sounding map
(ramps, coronae, soon agogica - exact). Why: a lilt changes tempo on every beat; at whole
bpm a 66 bpm waltz would run ~0.6% long per bar (half a second over 32 bars).

Range: the numerator 2 x dp x 48000 x 240 x 1000 stays inside s64 while dp < ~4e8 pulses
in ONE segment (~58 hours at 120 bpm) - no reduction needed. (I first added a gcd
reduction believing a 10-minute segment overflowed; a plant that undid it stayed green,
the arithmetic was wrong by 1000x - removed.) Same rational as before, so whole tempos
round identically: stella/pastorale WAVs byte-identical before/after.

Plants 9 + 2: two survived first and showed test gaps - the default first segment's
millesimae was only checked on a melody starting with 't' (which overwrites it), and the
only inverse check sat on a segment boundary where the fractional tempo cannot matter.

Hand-built maps (tests) must set millesimae too - it is not derived from tempus.

## 2026-10-24 - ligaturae '&' et trioli (desideratum ...MYH24)

TIES: '&' after a note means the NEXT note (same MIDI pitch) continues it - the reader
extends the previous CantusNota (pulsus_longitudo and frame longitudo, recomputed as
_quadrum(end) - initium in the same segment) instead of emitting a new one, so analysis,
cantor and partitura all see ONE note; partitura's existing bar split already prints
"D4:2~ | ~:2". Between the two notes only spaces, '|' and octave marks (o N, < >) are
allowed - in particular no 't' (a tie never spans a tempo segment, which keeps the
frame recomputation valid), no 'r', 'l', 'v', '@'. State in CantusLector: ligata,
post_notam (a note was the last musical token; cleared by r l t v @), nota_ultima. Errors:
different pitch (that is legato, not a tie), '&' before a rest or at the end, '&&', '&'
with no note before. Enharmonic ties (c#&d-) are the same MIDI pitch and allowed; the
first note's letter wins.

Caveat: emendatio / 'M:T' edit addressing works on notes by onset - the second half of a
tie is not a note; CantusStatus does not carry 'ligata' (editing in the middle of a tie is
not supported).

TUPLETS: a length is legal iff it divides the whole note (3840 pulsus) and is 1..64:
1 2 3 4 5 6 8 10 12 15 16 20 24 30 32 40 48 60 64 (triplets 3 6 12 24 48: c12 d12 e12 =
three in a quarter; quintuplets 5 10 20 40). Dots stay integral for all of them. The BASIC
PLAY convention (L3 is legal there too). Four older tests used 'c3'/'l3' as their example
of an INVALID length (cantus, carmen, praeconium_cantus, cantuale) - switched to 7/9.

Test sections: ligatura, ternarii ("trioli" is not Latin - ternarius, as partitura's existing "ternaria" flag).

Plants 12/12 red after three rounds. Two showed gaps: an explicit '&&' check in the '&'
branch could never fire (the "after '&' only spaces/bars/octaves/a note" filter rejects
the second '&' first) - removed, the '&&' test stays; and "segment position does not
advance after a tie" stayed green because the test checked the next note's PULSUS
(from pulsus_totus) but not its frame onset - now k.notae[1].initium == 2 s is asserted.
Note for plants: silva.planta matches anchors whitespace-insensitively - two lines that
differ only in alignment ('l->ligata = FALSUM' in the reader init and in the tie block)
count as the same anchor; give a second line of context.

## 2026-10-26 - portamento '/' (RTB1F)

`/` [length] after a note: the note's voice continues into the NEXT note and glides to
its pitch from that note's onset over the length (MML divisor, default 16, capped at the
next note's length). CantusNota gains `continuatur` (on the source) and `portamentum`
(pulses, on the target). Reader state: `illapsus_proximus` pending + its offset. Errors: '/'
not right after a note (post_notam false - so '//' and 'v50 /' fail too), a rest or the
end before the next note, the same pitch (write '&'), '&/' (the tie rule's own message).
Spaces, '|', o/</>, l, v, @ may sit between '/' and its note.

Test lesson: "c4/ r4" alone did NOT prove the rest check - without it the end-of-text
check fires at the same offset 2. "c4/ r4 e4" does. Same trap as before: an error test
must fail through exactly one rule.
