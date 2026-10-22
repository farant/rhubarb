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
