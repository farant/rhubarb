# carmen worklog

## 2026-10-22 - S6b: the song model and the book carmina.stml

Model (sonus-spec VIII-d): song -> tracks (instrument by NAME, gain,
mute, solo) -> clips at tick positions -> notes in ticks relative to the
clip, with pitch, velocity, written letter (spelling) and an optional '@'
preset (percussion). Song maps: tempo (cantus's CantusTempus - one map
type for reader and songs), meter, key (fifths -7..7 + minor, as
MusicXML). Pure data; nothing plays. MML is one way to write a clip; a
future editor edits notes directly.

Reading order matters: initial attributes, then METER changes in document
order (a bar position needs the meter map so far), then tempo and key
changes (full meter map), then tracks. A change at the position of the
previous entry REPLACES it (tempo at bar 1 = the initial tempo).
Meter changes take `mensura` only (they must sit on a bar line); tempo,
key and clips accept `mensura` or `pulsus`, never both.

Rules worth knowing:
- A clip's MML must not contain `t` (tempo belongs to the song). The scan
  skips `@preset` names - `@punctum` contains a 't'.
- Clips of a track must be in position order and must not overlap (a
  score view needs that). Empty clips and rest-only clips are errors.
- Song length = end of the last clip, trailing rests included - the loop
  point for cantor.
- CarminisVitium.frustum is s32 (-1 = no clip): i32 is UNSIGNED here.

Korobeiniki (my transcription of the public-domain folk tune): A minor,
4/4, 144, 16 bars - A section (1-8) and the slow half-note B section
(9-16); quadrans melody + triangulum bass in root/octave eighths. Both
parts checked by script to be exactly 16 bars; the bass bar pattern first
drifted up an octave per bar (4 '>' vs 3 '<') - fixed with a closing '<'.

Suite probatio_carmen: 3 sections (legere, vitia x24, ordinarium); green
on the first implementation run; plants 18/18 red after adding a mute-only
track (a track with BOTH mute and solo hid a mute/solo swap).

## 2026-10-23 - M2a: clips carry their bar marks

CarmenFrustum.lineae / numerus_linearum point at the Cantilena's array
(same piscina - no copy); pulse is inside the clip, offset inside the
clip's text.

## 2026-10-23 - carmen_tempus_mensurae (beat unit hoisted)

The beat rule (1/denominator; 6/8 9/8 12/8 = three eighths: denominator 8, numerator > 3
and a multiple of 3) lived in THREE places: harmonia `_tempus` (chord line per beat),
censura `_fortia` (strong beats), emendatio `_unitas` ('M:T' locus). Fran OK'd the hoist
10-23; all three now call carmen_tempus_mensurae(carmen, pulsus). 3/8 and 6/4 are NOT
compound by this rule (tested). Plants 9/9 red - one (censura `tempus > unitas` ->
`>=`, every beat strong in 3/4) survived until a 3/4 weak-beat dissonance case was added
to probatio_censura 'dissonantia'; the old code had the same gap.

## 2026-10-23 - instrumenta libri et carminis (wish …WN96C4, Fran's #1)

`<instrumentum>` at book level (siblings of <carmen>) and inside a song. Every attribute is
a TibiaParametra field (Latin names; genus/forma as words), plus ambitus="N..M" for
censura; unknown attribute, bad / out-of-range number, unknown basis, duplicate title at
the same level all refuse the whole book. basis = a built-in (tibia_praeparatum) or a
book instrument defined ABOVE (chains: cembalo_mollis on cembalo); song-level instruments
may base on any book instrument (the book list is read first) or on earlier song ones.
Without basis: unda/sinus, officium 0.5, tenor 1, amplitudo 0.5, rest 0.

Each Carmen carries the merged list (its own first, then the book's), so carmen_instrumentum
answers alone and every consumer gets book sounds with NO API change: cantor looks in the
song before its resolver (book beats even a custom resolver - the song defines its own
sound), auscultatio / WAV / live all through cantor; censura's ordinary ambitus uses an
instrument's ambitus when given, else the table (a redefined built-in without ambitus
keeps the built-in's range, not 0..127 - tested); auditio's instrument check accepts book
instruments and -carmina lists them. A song-level instrument is invisible to other songs.

Plants 14/14 red (one rewritten to compile: '&&' inside '||'). Two cases were added before
planting: a song-level instrument based on a book instrument, and a redefined built-in
without ambitus (censura must keep the table range).

## 2026-10-24 - instrument attribute partiales="r:a:d ...", empty values

`partiales` replaces ALL partials (unlisted slots go to zero), so basis="organum"
partiales="1:1:0" is a bare sine and not organum with its first partial overwritten. With
a basis and no attribute the partials are inherited (marimba_mollis). Bounds: ratio
(0, 32], amp [0, 1], dec [0, 60]; 1..8 space-separated triples, exactly two colons each.
genus word "partiales" added (GENERA[IV]).

Bug found while testing (it was already in the #1 commit): an EMPTY attribute value
(`claritas=""`) reaches the parser with t->valor == NIHIL. stml gives boolean attributes
the interned "true", but gives "" no value at all. Every branch dereferenced *t->valor ->
segfault. Now one guard after the titulus/basis skip -> "valor malus aut extra fines";
tests for claritas="", ambitus="", partiales="".

## 2026-10-24 - vocalis, flatus, resonans, acumen, motus; genus 'formantes'

CAMPI grows to XX. vocalis is the one field that is word OR number: the VOCALES word (a
e i o u -> 0..4) is tried first, then the normal numeric path with bounds 0..4 - so
"1.5" works and "y" / "5" are refused. motus is the first field with a negative lower
bound (-48..48). The words check changed from `k >= XIII` to `k == XIII || k == XIV`,
since fields after forma are numeric again.

## 2026-10-24 - instrumentarium (house instruments as data)

instrumentarium.stml (repo root, next to carmina.stml) holds instruments in the book
grammatica, under <instrumentarium>. tools/carmina_infigere.sh embeds it into the same
generated lib/carmina_infixum.c (instrumentarium_infixum_textus). The generator action
gained the input, so there is no second generator.

The tiers fell out of what already existed. _instrumenta_legere always took "externa" as
bases; the book-level call passed NIHIL. Now carmina_legere_cum passes the
instrumentarium there, and each song gets a "visibilia" list (book instruments, then
the instrumentarium) as its externa. So Carmen.instrumenta = song, book, instrumentarium
in lookup order, and carmen_instrumentum's first match gives "lower tier redefined
wins". Carmen.numerus_instrumentarii = how many trailing entries are the
instrumentarium, so listings (auditio -carmina) can leave them out.

carmina_legere = instrumentarium_ordinarium + carmina_legere_cum; a broken house
instrumentarium fails every book (it's a house bug - the carmen suite parses it).
carmina_legere_cum(..., NIHIL, 0, ...) is the isolated form for tests.

Genus words now come from tibia_generis_titulus (_genus_verbi loops until NIHIL), and
the GENERA table here is gone. Existing count asserts in 'instrumenta' subtract
numerus_instrumentarii.
