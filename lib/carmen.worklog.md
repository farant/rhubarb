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

## 2026-10-24 - agogica: waltz lilt / swing as weights per bar (desideratum ...4939S)

`agogica` on the carmen (initial metrum) or on a `<metrum>`: K weights for K equal parts
of the bar, normalized to sum K, so the BAR keeps its exact length and only its parts
stretch; `vindobonensis` = 0.86 1.15 0.99 (Viennese waltz: beat 1 short so beat 2 comes
early, beat 2 long so beat 3 is a touch late - my reading of Bengtsson & Gabrielsson,
tune by ear); "2 1 2 1 2 1 2 1" in 4/4 = triplet swing. Valid until the next <metrum>
(a metrum without agogica is even - restate the same meter to switch lilt on/off).
Stored on CarmenMetrum: pondera[CARMEN_PONDERA_MAXIMA = 16], numerus_ponderum, inaequalitas
(text as written, for partitura). Named presets live in INAEQUALITATES_NOMINATAE. Names: the
STML attribute is `agogica` (approved); C says INAEQUALITAS (French baroque notes inegales =
the same idea; "agogica" is a 19th-c. coinage the lint refuses).

Sounding map: _coronas_inserere became _mappam_modulare - breakpoints = old steps +
corona bounds + every part of every lilted bar + the END of each lilted span; tempo =
map tempo / (corona factor x part weight). Needs the millesimae precision (965e7dad):
at whole bpm a 66 bpm waltz ran ~0.6% long per bar. The end-of-span breakpoint was a
first-run bug: without it the last lilted part's tempo carried into the next (even) bar.

Analysis judges written pulsus - censura's strong beats, harmonia windows do not move.
Bar length must split into K whole pulsus (3/4 = 2880 -> 3, 4/4 = 3840 -> 8 ok; 7 parts
of 3/4 refused).

Plants 11/11 red.

## 2026-10-24 - dynamica: crescendo / diminuendo (desideratum ...16EVW)

`<dynamica [pars="P"] mensura="M" [ictus] valor="mf" [usque="U"]/>`: marks pp p mp mf f ff
= 0.2 0.32 0.45 0.6 0.8 1 (roughly 2-5 dB steps) or a number 0..1; default 1 (no
<dynamica> = unchanged songs). Without usque a step, with usque a ramp to the downbeat of
bar U, even in dB (v0 x (v1/v0)^t); a ramp with a 0 end is linear (dB has no 0). Per part
(pars resolved by titulus - parsed AFTER the parts) or for the whole carmen (pars -1);
carmen_intensio(k, pars, pulsus) = carmen value x part value. Stored as CarmenIntensio
(intensio: "dynamica" is not classical Latin; the STML element keeps the musicians'
word, as with <fermata>/corona). Rules per part: increasing positions, no change inside a
ramp, at the SAME position only a ramp after a step (start p, then crescendo - like the
tempo clivus).

partitura: "dynamica mf", "dynamica m ff usque 4".

## 2026-10-24 - instrument attributes unisonus, dispersio, mora (desideratum ...J8Q9H)

CampusInstrumenti grew to CAMPI_NUMERUS = 23 (the table size and the two loops used the
literal XX before - now one constant): unisonus 1..7, dispersio 0..100 cents, mora 0..10 s
(-> TibiaParametra.mora_vibrati). The house instrumentarium has 11 entries now (tests that
counted V updated).

## 2026-10-24 - sectio et repetitio: form without copying bars (desideratum ...67RC)

`<sectio titulus="a" mensura="1" usque="9"/>` names bars 1..8; `<repetitio sectio="a"
mensura="17" [ictus] [semitonia="2"] [partes="m b"]/>` expands, at load, every part's
WRITTEN notes starting in the section into a new frustum at the destination (transposed;
transposed notes lose their letter so partitura spells from the key - E4-1 in C maior
prints D#4). Parts default to all with notes in the section. Tempo, key, dynamics,
fermatas are NOT repeated (positional) - a refrain a step higher writes its own <clavis>
(censura flags every note otherwise). Rules: destination empty per part (no overlap with
any frustum), MIDI 0..127 after transposition, semitonia -24..24, usque after start,
section titles unique, known parts, and no section may cover an expanded frustum (no
repetition of a repetition - the source is written notes only).

Model decision (deviation from the proposal, told Fran): the proposal appended expanded
frusta AFTER the written ones so emendatio's frustum index == STML index stayed true. But
carmen.h documents CarmenPars.frusta "ordine pulsus" and partitura's per-bar cursor relies
on it (never goes back). So expanded frusta are INSERTED in time order, and every frustum
carries repetitio (-1 written) and scriptum (index among written frusta = STML order, -1
expanded). emendatio maps to STML by scriptum (_librum_scribere takes the written index
and finds the re-read frustum by scriptum), and refuses edits landing in expanded material
- ponere, mensuram_invenire/substituere, copiare from repeated bars - with the static
causa "repetitio (frustum iteratum): muta sectionem fontis" (causa strings are static by
contract, so it cannot name the section).

Caveat: censura/recensio findings in a repeat report the expanded frustum's index with
the SOURCE note's text offset ("frustum 1, offset 42" in the waltz demo: C#6 above the
superius range in the +2 refrain - a real finding).

Plants 19/19 red after two rounds. One survived first: "destination not empty" - its
test case (section 1..3 repeated at 2) ALSO tripped the repetition-of-repetition check
(the section covered the new copy), so it failed for the wrong reason. Isolated: section
bar 1 repeated at bar 2, where the written c1 | d1 frustum sounds and the section does not
reach the copy. Lesson again: an error test must fail through exactly one rule.

## 2026-10-26 - <acorda>, <figura>, <comitatus> (Q7G5 step 2)

Declared harmony is song-level data like key and meter: Carmen.acorda (ComitatusAcordum,
song pulses, sorted, non-overlapping) from every <acorda mensura textus> - each element's
bars come from _initia_mensurarum (start, then the following bar starts from the metre
map), so an <acorda> with ictus simply has a short first bar. <figura> is read at book
and song level (_figuras_legere); the song's list is searched first, then the book's.
<comitatus> is a child of <pars> next to <frustum>: _comitatum_legere realizes it into a
CarmenFrustum with comitatus = its index among the part's comitatus, scriptum -1,
repetitio -1, textus = the figure text, no lineae, note pulses relative to the frustum.

Order matters: figures and acorda are read after the metre map and instruments, BEFORE
the parts (comitatus needs both); repetitions run after the parts, and since their source
is "frusta with repetitio < 0" generated frusta repeat (transposed) with no extra code.

scriptum now counts <frustum> elements only (a separate counter): emendatio's
_textum_frusti finds the n-th <frustum> child, so a comitatus in between must not take an
index. Test: frustum / comitatus / frustum -> scriptum 0, -1, 1.

_ambitum_legere (instrument ambitus "N..M") is reused for ambitus= and bassus=; it sits
later in the file, so a static prototype precedes _partem_legere.

## 2026-10-26 - portamento fields; repeats heal cut glides (RTB1F)

CarmenNota copies `illapsus` / `continuatur` from cantus (comitatus and harmonia's
scratch notes set them 0). A <sectio> boundary can cut a glide: _frustum_iterare keeps the
pair only when BOTH notes were copied and are contiguous (pass 1 clears continuatur
without a gliding next note, pass 2 clears illapsus without a continuing previous).
cantor's orphan sweep would catch a dangling one anyway, but partitura would print a
false '/'.

## 2026-10-26 - review batch 1.4: agogica tempo map overflowed by one line when its span was empty

From the sexta review (time finding 1, ASan): _mappam_modulare sized `lineae`/`nova` with
ceil(spatium/l)*K + 1 only when spatium > 0, but always appended the "finis spatii" line - an
agogica metrum with no span (an EMPTY waltz `<carmen metrum="3/4" agogica=...>`, the normal
state of a song being started, or a `<metrum agogica>` at/after the last barline) wrote one
s64 past the buffer; inside a piscina it landed silently in padding / nova[0], so no test could
see it. Fix: the +1 is counted for every agogica metrum. To make it provable without ASan the
write loop now checks n < capacitas and fails cleanly (FALSUM -> 'piscina exhausta') - with the
+1 planted out, the new empty-span tests fail instead of corrupting memory. Plant 1/1.
