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

## 2026-10-24 - positio, reverberatio (pars) and <reverberatio> (carmen / liber)

The pars gets positio -1..1 and reverberatio 0..1 (its send). The room element
<reverberatio aula mollities latitudo lucrum/> sits in a song or at book level. Defaults
are 0.5 0.5 1 0.25 when the element is present, and lucrum 0 when there is none. A
song's own element replaces the book's WHOLE form, not attribute by attribute, so a
song writing only mollities gets aula 0.5, not the book's aula (tested). The book
default is threaded into _carmen_legere as two parameters.

Gotcha: the book's second children loop (the one that reads songs) skipped only
`instrumentum` - a book-level <reverberatio> was then read as a song and failed. Both
loops now skip it.

## 2026-10-24 - tempus continuum: rampae et ictus (desideratum ...VG8PQ3)

Name: a ramp is a CLIVUS (slope) in code - "rampa" is not Latin. Model decision: the SOUNDING map stays piecewise constant. A written ramp
`<tempus mensura="M" [ictus="N"] valor="T" usque="U"/>` (prevailing tempo -> T, linear in
pulsus, reaching T at the downbeat of bar U) is expanded at load into steps of
CARMEN_GRADUS_TEMPORIS = 120 pulsus (a 32nd note), each at the tempo of its midpoint,
rounded to whole bpm, plus a hold step at U. So cantus_quadrum_pulsus/its inverse, cantor,
auscultatio and recensio did not change, and the "no accumulated rounding" property holds
(every step closes on its rounded quadrum). The rejected alternative - a true curve - needs
log/exp in both directions and gives that property up. Measured: 120 -> 60 over 8 quarters
is within 0.2% of the exact integral (8 ln 2 s).

Carmen.tempora is now DERIVED; what was written lives in Carmen.tempora_scripta
(CarmenTempus {pulsus, tempus, usque}; usque 0 = step). Everything that shows the
"initial tempo" (partitura title, recensio text + JSON, auditio -carmina) reads
tempora_scripta[0] - with a ramp from bar 1 the first sounding step is already slower.
partitura's bar header prints the written change ("tempus 60 usque 4"), not the steps.

Ordering rules: a step at the same position as the previous step replaces it (as before);
a RAMP at the position of a step keeps that step as its start (ramp from bar 1 starts at
the carmen's tempus attribute); no change may fall inside a ramp (positio < usque).

ictus="N" refines mensura wherever pulsus is admitted (frustum, tempus, clavis): beat unit
= carmen_tempus_mensurae (6/8 = dotted quarter), same as 'M:T' edit addressing. Rejected
with pulsus, with metrum, 0, or past the bar. NOTE: unknown attributes on <tempus> were
silently ignored before - the red tests for usque/ictus errors first came back "accepted".

auditio -carmina's tempo column is the one consumer without a test (tool output).

Plants 14/14 red after two rounds. One survived first: recensio's title read through the
sounding map still said 100 because a 100 -> 60 ramp over TWO bars has its first step at
99.7, which rounds back to 100 - the fixture now ramps over one bar (first step 99). Lesson
for "written vs sounding" tests: make sure the first sounding step actually differs. The
ramp-from-start carmen test likewise moved 120 -> 100: at 120 the carmen tempus equals the
house default, so 'ramp starts from the default' was indistinguishable.

## 2026-10-24 - fermata (desideratum ...VG8PQ3, part II)

`<fermata mensura="M" [ictus="N"] [factor="F"]/>` (or pulsus=): the span from its position
to the next note START in any part (end of piece if none) sounds F times longer (1..4,
default 2). Same model as the clivus: written fermatae live in Carmen.fermatae
(CarmenFermata {pulsus, finis, factor}; finis derived); the sounding map gets breakpoints
at every old step plus each fermata's start and end, and inside a fermata every step's
tempo is divided by F (rounded, floor 1 bpm). So a fermata inside a ritardando stretches
the ramp steps it covers - measured ratio 2.0 within 2%.

Ordering: the map is now built AFTER the parts are read (finis needs the notes);
nothing before that read Carmen.tempora. Fermatae must be in increasing order, start
before the end of the piece, and not start inside the previous fermata's span. The span
runs to the next ONSET, not the next note end: on a held chord followed by rests and then
a new phrase, the rests stretch too (a breath) - deliberate.

partitura: "fermata x2", "fermata x1.5 (+960)".

Names: the STML element is <fermata> (the musicians' word, approved), but C identifiers say
CORONA (CarmenCorona, Carmen.coronae, _coronam_legere ...) - the historical Latin name of
the sign; "fermata" is Italian and the lint refuses it.

Plants 11/11 red, plus one that survived and showed a redundant check: an explicit
"fermatae in increasing order" test at parse time could never fire alone, because a
fermata written before the previous one always starts before that one's finis (finis >
pulsus), so the overlap check already refuses it. Removed; the overlap message now names
both ("ordine positionis, non intra priorem"); the reverse-order test stays.

## 2026-10-24 - sounding map carries exact millesimae

_gradum_addere now takes an f64 tempo and fills both fields: tempus (rounded, >= 1) and
millesimae (thousandths, >= 1, used by the frame math - cantus.worklog.md). Ramp steps keep
their exact midpoint value (120 -> 60 first step = 119.531), coronae divide the exact
millesimae. Effect: the ramp test now holds to 0.02% of the exact integral (was 0.2%), a
fermata inside a ramp to 0.01% of 2x (was 2%).
