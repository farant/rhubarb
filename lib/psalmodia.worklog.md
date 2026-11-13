# psalmodia worklog (the fitter)

## 2026-11-12 — T3 measurement and header draft (psalmodia plan 2)

Re-read the table's REGULA ACCENTUS / TEXTUS DEFICIENS; the header
quotes them verbatim. Shapes in the 246 examples: `*` in 17 (the
versus), `†` in 10; casus monosyllaba 6; nota hebr. 26, mediatio
omissa 2, initium solemne 1.

**Hebrew words behave two ways** (the 26 hebr. examples):
- mid-verse, or at a half-verse end in a tone with no monosyllabic
  cadence: an ordinary accent where the music shows one - *in Ísrael
  est* (I A), *ex Basán*, *in Siná in sancto*, *habitántibus Cedar*
  (III C), *eam in Éphrata* (I, III, VII D), *laus Ísrael* (I, VIII);
- at a half-verse end in a tone that prints a casus="monosyllaba"
  cadence: that cadence ("abrupt") - II *ex Sion* h h i, *in Ephrata*
  h h h i; IV *Dómine David*, *sunt Jerúsalem*, *Dóminus ex Sion*;
  VI *Jerúsalem*; in directum *rédimet Ísrael*. AR p. 5*: "in
  monosyllaba vel voce hebraica".
So the list entry is (division, accent syllable, abrupt), and
"abrupt" means: use the monosyllabic sister at a half-verse end, and
hold the tenor at a flex. Cedar added (an example); Amen dropped
(its division and accent are the common ones, and the ending over
"sæculórum. Amen" is the ordinary one - EUOUAE; T8 checks).
Only in directum prints a monosyllabic termination (AR p. 26*).

**Clef**: tone 2 A carries clavis="c3" on the ending = the whole
psalm in that clef with the same letters; the output has one clef.

**GregoBase written psalms** (64, 342, 7330): every verse numbered
from "1.", mediant `*(:)`, mora dots and "Flexa:" lines added by
their transcribers (we add nothing: P11).

### Departures from the plan's T3 interface (rulings)

- `PsalmodiaOptiones.gloria` -> `sine_gloria`, so a zeroed struct (or
  NIHIL) is the default (Gloria on). Added `sine_intonatione` for the
  book's later-verse examples (primus="falsum"). Cost if wrong: a
  flag rename.
- `PsalmodiaPsalmus` carries `tonus` and `terminatio`, so
  `psalmodia_gabc_scribere(psalmus, optiones, exitus)` needs nothing
  else. Cost: two pointers; the table must outlive the psalm.
- `psalmodia_partem_aptare` returns a one-verse `PsalmodiaPsalmus`
  (admonitions inside) instead of a `PsalmodiaVersus*` plus an
  out-parameter, and picks the monosyllabic sister itself, so the
  book's casus="monosyllaba" examples test our detection. Cost: the
  T5 mapping passes the common cadence.
- `PsalmodiaVersus.gloria` marks the Gloria verses (no number).
- gabc-special characters in the text ( ( ) [ ] { } < > | % ) and
  non-UTF-8 bytes: admonition, dropped from the output, so every
  output reads back.
- An unmarked word of 3+ syllables (not Hebrew) is fitted as if it
  had no stress (a cadence accent never moves back into it), with an
  admonition naming it - defined, not guessed.

## 2026-11-12 — T4 reading pointed text (psalmodia plan 2)

**Measurement** (scratchpad `t4/mensura.py`, python3 -I over
chants.csv, office_part ps, notes and tags stripped). Most of the 35
"written-out psalms" are one model verse with notes, not whole
psalms. Flex: `†` 13, `+` 7 - every `+` from one transcriber (Andrew
Hinkley, "Flexa: dux est eórum : +"). Verse numbers: 38, all `N.`
except one `V/. 1 Veni` (versicle sign, number without a dot). Two
files number the Gloria too (4690: 9., 10.; 7536: 11., 12.); we keep
the Gloria unnumbered (T3). 7536 glues `»†` - signs glued to words
must be recognised, as the header says.

**Built**: `lib/psalmodia.c` - lines (BOM, LF/CRLF/CR, blank, `%`
comment, leading number) -> tokens (words, `*`, `†`/`+`, dropped
bytes) -> words (syllabae_latinae, or VOCES_HEBRAICAE by a folded key:
runae_basis, lowercase, j -> i, letters only) -> verse (parts,
admonitions sorted by column per line). `psalmodia_aptare` and
`psalmodia_admonitiones_scribere` defined; `partem_aptare` (T5) and
`gabc_scribere` (T6) declared, not yet defined.

### Rulings at T4

- **Provisional fitter.** T4's `psalmodia_aptare` puts the tenor on
  every syllable and the cadence's last note on the last syllable of
  each half (the monosyllabic sister when the half ends on a
  monosyllable or a Hebrew word). That is the smallest fitting that
  makes "abrupt" visible through the public API, and it equals the
  real rule for a one-locus cadence, so the T4 tests (a hand-built
  tone: mediant f, sister g, ending e, tenor h) stay valid after T5.
  Cost if wrong: none; T5 replaces it.
- **Signs.** A second `*` -> "asteriscus iteratus", dropped; `†` after
  `*` -> "flexa post asteriscum", dropped; a second `†` -> "flexa
  iterata", dropped. No `*` -> "sine asterisco" at the column after
  the line's last non-space character, and the verse is all second
  half (with a `†`, the part before it is still a flex part). Empty
  parts -> "pars ante flexam vacua" (at the `†`), "semiversus vacuus"
  (at the `*`, or at the line end for the second half).
- **Words with no vowel** (`—`, `«`, a mid-line number) join the
  neighbouring syllable with one space: the previous word's last
  syllable, else the next word's first. Cost if wrong: a syllable
  text with a space inside; T6's read-back test decides whether gabc
  takes it, and the joiner changes there if not.
- **Dropped characters** (gabc `( ) [ ] { } < > | %`, bad UTF-8,
  control bytes): admonition each ("signum gabc '(' omissum",
  "octetus 0xFF omissus"); a word containing one is copied without
  it, so its syllable columns after that point are approximate.
- **A line with no syllable** (only a number, only `—`, only `*`) ->
  "versus sine syllabis" at its first non-space column; no verse.
- **Columns**: characters, 1-based; the BOM does not count; a bad byte
  counts as one. Gloria syllables and admonitions: line 0, column 0.
- **Numbers**: digits then `.` (anything may follow: `1.Dixit`), or
  digits then a space or the line end (`12 Deus`). `V/. 1` is not a
  number; `V/.` and `1` become vowel-less text on the first syllable.
- **Layers**: lib/psalmodia.c includes utf8.h and runae.h (runae_basis
  for the Hebrew key, the way sancti and nomenclator fold accents),
  beyond the plan's list (psalmodia_tabula, syllabae_latinae,
  chorda_aedificator). Both are foundation libraries. Cost: check at
  T7 that auditio already links runae.

**Plants** (silva.planta, build/plantae_psalmodia/t4.log), each red
then green, file restored byte-identical: (a) capital letters not
folded in the Hebrew key -> *Speret Israel* red (the plan named
`israel`; with lowercase keys the case-sensitive failure shows on the
capitalised word); (b) columns count bytes -> "t:1:26: sine
asterisco" red (*Laudáte*, *Dóminum*, *cælis* carry two-byte
letters).

## 2026-11-12 — T5 the fitter (psalmodia plan 2)

**Measurement** (scratchpad `t5/mensura.py` over the table). 246
examples: mediatio 92 (none 17, A 19, B 13, C 13, D 11, abbreviata 3,
antiqua 1, contracta 1, monosyllaba 14), terminatio 125 (none 83, A 9,
B 7, C 3, D 6, abbreviata 15, monosyllaba 2), versus 17 (contracta 1),
semiversus 7, initium 3, flexa 2 (the plan said semiversus 8,
mediatio 91 - miscounted then). canticum 11, primus="falsum" 3, casus
monosyllaba 6 (plus forma="monosyllaba"/"contracta" 17 more - the
book's two spellings of the same thing; the fitter detects both
itself). Call per pars (test `_exemplum_aptare`):
- mediatio -> `partem_aptare(t, eligere(t, forma=="antiqua"),
  MEDIATIO, FALSUM)` (the common cadence; the sister is ours to pick);
- semiversus -> the same with intonatio VERUM;
- terminatio -> the common ending named by differentia (or the only
  one), TERMINATIO; flexa -> NIHIL, FLEXA; initium -> NIHIL, NULLUS,
  intonatio VERUM;
- versus -> `psalmodia_aptare` of the one line, sine_gloria, canticum
  from the example, sine_intonatione = !primus.
Every example was worked by hand against the rule before coding; all
245 came out right on paper, and the first green run gave 245/245.

**The rule as built** (`_clausulam_ponere`): loci from the last,
syllables from the part's end. An accent locus whose syllable directly
follows its word's stress (index in word == accent + 1) recedes onto
the stress, the skipped syllable takes superveniens + 'r'. With
anticipata the two swap: stress = superveniens + 'r', skipped = the
accent neuma (I D2 *saé(gr)cu(gf)*, III *po(jr)tens(ih)*, IV E). The
stress syllable is ACCENTUS (<b>) either way.

### Rulings at T5

- **Plan's `_breves_probare` "tone 1 on *Qui* -> hixih" was a
  misreading** of the spec/table: *Qui = h ixi h* is the first syllable
  of the book's *Qui fácit haec* (already an example). The test keeps
  that case and adds a lone *Qui* with the general rule's result.
  Cost if wrong: two expected strings.
- **Deficient mediant, general form**: the first syllable sings tenor
  + every unplaced locus that is not a preparation (in order) + its
  own notes. Book: *Qui* = h ixi h (I), i k j (VII). Extrapolated:
  lone *Qui* (I) = hixihgh, *fácit haec* = hixihg hr h. Unplaced
  preparations are dropped (they are "numero fixo" lead-ins; no book
  case). Cost if wrong: long melismas on 1-2 syllable halves only.
- **Intonation meets a cadence**: in a first part the intonation wins
  (one note per syllable, as many as there are syllables); a mediant
  whose non-preparatory loci do not fit after it - or whose accent
  would recede into it - is dropped entirely (the book's *Magníficat*,
  I and III); a flex inside it is dropped. In an ending the cadence
  wins and the intonation (peregrinus *hixi*) takes only syllables the
  ending left on the tenor. Cost if wrong: short first verses only.
- **Intonation placement**: the first part of the first half (the flex
  part when there is one). A verse with no `*` has no first half, so
  no first-half intonation (peregrinus still intones its second half).
  Verse 1 = output index 0, so a psalm with no verses intones its
  Gloria.
- **Flex**: last syllable, and the penult when the word's stress is
  on the antepenult (accent == numerus - 3; AR "paenultima si
  brevis"); abrupt word (monosyllable or Hebrew) holds the tenor; a
  tone with no `<flexa>` holds the tenor.
- **superveniens vacua** (no table entry has one): the accent does
  not recede. Defined, never hit.
- **`partem_aptare` input**: signs in a part -> "signum in parte
  omissum", dropped; no "sine asterisco"; empty -> "pars vacua" at
  1:1; more than one line -> "pars una exspectatur" at the second
  line's column 1, first line fitted. NIHIL clausula with MEDIATIO or
  TERMINATIO -> "clausula nulla" at 0:0.
- **Lint**: `intonatio` (header's own word, ecclesiastical Latin) got
  a glossary entry beside `mediatio`; the oratio gate ran green 19/19
  (oracle unmoved). `ima` -> `infima` (never `imus`), `proparoxytonon`
  -> `antepaenultima` (known to WORDS).

**Totality now covers the real fitter**: every truncation and random
input of `_totalitas_probare` is also fitted to all 11 real tones
(common, canticum, and partem_aptare for every finis), under
CREDO_NON_RUIT and in-process with `_validus`. First attempt put it
all in the test's arena: 698 MiB > the 256 MiB ceiling (credo_memoria
named it); each input now gets its own dynamic piscina.

**Plants** (silva.planta, build/plantae_psalmodia/t5.log, backup
psalmodia.c.t5.ante), each red then green, file byte-identical after:
(a) accent never recedes -> exempla 123/245 (122 red: every B/C/D
form and every ending with a hollow note), plus four `_breves`
assertions; (b) intonation on every verse -> exempla 242/245 (the
three peregrinus primus="falsum" verses) and `_toni_rari` (peregrinus
verse 2, non-canticum verse 2).

## 2026-11-12 — T6 writing gabc (psalmodia plan 2)

**Measurement** (scratchpad `t6_mensura.py`, GregoBase psalms 64, 273,
342, 4690, 7330, 7536). 7536 (Daniel Luis, tone 1 D) is the house
style we copy: verse number glued to the first syllable's text
(`2. Nam(h)`), verse 1 right after the clef, ` *(:)` and ` (::)`, a
newline per verse, Gloria in the same style. `« Quis(h)` - GregoBase
already writes a syllable text with a space inside. Others: Hinkley
(64, 273, 342) adds `( )` spacers and ` :` before the bar; 4690 uses
`<v>\greheightstar</v>(:)`; flex is `†()` or `+()` everywhere. T3's
header already fixed every byte (no choice left for step 1); we keep
its `†(,)` (a divisio minima under the flex) over GregoBase's empty
`†()`.

**Built**: `psalmodia_gabc_scribere` - name/mode/annotation header
(annotation = modus + " " + differentia, or the modus alone), `%%`,
`(clavis)` (the ending's if it has one), then per verse: number +
". " on the first syllable (Gloria unnumbered), space before every
word-initial syllable and before the signs, `<b>`/`<i>` around the
whole syllable text (punctuation included, as 273/7536 do), the
number outside the tag, ` (::)` and a newline. Tone NIHIL -> header
and `%%` only.

**Read-back found two real bugs** (random text under
`_totalitas_probare`, now also written and re-read for every tone):
gabc text treats `$` as an escape for the next character (a syllable
ending in `$` swallowed the `(` and merged with the next: 620 syllables
read for 623 written) and `~` as U+203F. Both are now dropped with an
admonition like `( ) [ ] { } < > | %`; the header's list says so.

### Rulings at T6

- **Vowel-less words** (T4 ruling): `« Dó(h)mi(h)nus —(h)` reads back
  as written - the joiner stays.
- **Titulus**: `;`, CR and LF in `-titulus` become spaces (they end a
  gabc header value). Cost if wrong: a title loses a semicolon.
- **Expected hyphenation**: the T2 divider gives *o-mnes* (T1's
  approved AR rule, longest onset forward), so the fixed Ps 116 output
  says `o(j)mnes(j)`; notes are the same either way.
- **Header touched**: one line - `$ ~` added to the list of dropped
  gabc characters (behaviour, not interface). Fran reviews headers:
  flagged in the report.

**Plants** (silva.planta, build/plantae_psalmodia/t6.log, backup
psalmodia.c.t6.ante), red then green, file byte-identical after:
(a) no space before `*(:)` -> scriptura, reditus (the glued bar loses
verbum_incipit) and totalitas red; (b) Gloria always added ->
scriptura (sine Gloria) and the T4 sections that count verses red.

## 2026-11-13 - T7: the table in binaries, `auditio -psalmus`

**Measured** how `cantuale_infixum` is embedded and copied it:
`tools/toni_psalmorum_infigere.sh` (od -> byte array, GENERATUM line 1)
writes `lib/toni_psalmorum_infixum.c` (336 KB for the 54 KB table);
`include/toni_psalmorum_infixum.h` carries the aedilis tag; one
`<actio genus="generator">` in `aedificatio.stml` (inputs: the script
and the table); `compile_tests_fontes_generare.sh` adds the .c. New
section `_infixum_probare` (probatio_psalmodia_tabula, now 8 sections,
423 assertions): embedded copy == the file byte for byte, reads with 0
vitia, 11 tones.

**auditio** (`tools/auditio.c`): `-psalmus VIA -signum S [-titulus T]
[-sine-gloria] [-antiqua]`, FRAN 2. `_psalmum_aptare` reads the text,
the embedded table, finds the ending, fits, writes gabc; admonitions
go to stderr through `psalmodia_admonitiones_scribere`
(`via:linea:columna: causa`). `_gabc_convertere` now takes the gabc
text as an argument (caller reads the file), so with any play/analysis
flag (`-sonare`, `-partitura`, `-carmen` ...) the psalm's gabc takes the
`-gabc` path unchanged; carmen title = file name unless `-titulus`.
auditio links runae (9 symbols) through aedilis with no list to edit.

### Rulings at T7

- **Admonitions = exit 1, gabc still printed** (the library always
  writes; the user sees the warnings and the result); with a play flag
  nothing plays - fix the text first. Cost if wrong: one condition.
- **`-antiqua` on a tone without an ad libitum mediant** is an error
  (exit 1, "tonus 8 mediationem ad libitum non habet"), not a silent
  fallback to the common mediant. Cost if wrong: one check.
- **`-do P` allowed with `-psalmus`** (it is gabc underneath).
- **Empty psalm file** reports "legi non potuit (aut vacuus est)" -
  filum_legere_totum gives 0 bytes for both.

**Listening.** I cannot hear; I opened `build/psalmodia/116_8G.gabc` in
the antiphonale pane (`praeco antiphonale`) for Fran, read the
`-partitura` rendering (8 G cadences on the right steps, transposed do
= Eb4, rest at each `*`) and looked at a `neumae` PNG: notation right
(annotation, hollow `r` notes, bars) but **the "1" of "1. Lau" became
the drop-cap initial**. GregoBase pairs `initial=0` with a numbered
verse 1 (64 85 342 355 447 2231 7332-7362 8134) and `initial=1` with
an unnumbered verse 1 (273 288 435 7405 7536); our writer numbers
verse 1 but writes no `initial-style: 0;`, and `gabc_compositio`
ignores `initial-style` anyway. Not fixed here (it changes the T3
output form): ledger question 01M7AQ6TAVDNN9ZMZSG5WBC619 for Fran.

**Plants** (silva.planta, build/plantae_psalmodia/t7.log, t7b.log),
red then green, files byte-identical after: (a) one byte of the
embedded copy changed -> `infixum == fons` red (gate radix, filter
psalmodia_tabula); (b) auditio ignores `-sine-gloria` -> auditio-fumus
"versus II, Gloria nulla" red. The 13 new fumus cases (section VIII)
were also red against the old binary (11 FRACTUM; the two usage cases
pass vacuously there, unknown flags already exit 2).

`fabrica iudicare -tacta`: the new generator RECENS;
`build/capsula_corpus_silicis.c` (embeds every lib/include file) was
stale from the new files - healed with `bin/fabrica sanare`. Lesson:
never send a sanare log into build/ - fabrica calls the run FRACTUM
("scripsit extra vestigium").

## 2026-11-13 - T8: corpus gate, first list (STOP for Fran)

`probationes/probatio_psalmodia_corpus.c` (3 sections) reads the cached
GregoBase corpus (`PSALMODIA_CORPUS` = build/gabc/bacor_v0.4/x, 9130
.gabc files, one arena per file; 11 s, apex 143 MB under custos).
Without the corpus each section says OMISSA and checks only committed
data (table reads clean; manifest has 6 columns per line) - credo
counts a section with no assertion as failed, so there must be one.

**Measured:** 226 537 words in Latin chants (17 tagged "Lingua
Polonica" skipped); 3809 antiphons, 652 with EUOUAE written out (6
syllables in 1239 of 1244 files corpus-wide; 567 more are introits -
other tones - excluded by office-part an), 38 with no label, 78 whose
label the table does not resolve, 536 compared (spec said 610: it
counted introits); 34 written psalms (spec said 35), 12 with a label
we can read (header, annotation, or name: "Ps 111 8 G", "Mode 2D").

**First list: 375 lines** (build/psalmodia/discordiae.tsv).
- EUOUAE 536: 422 agree, 11 through the table's `vicaria`, 1
  transposed, 102 disagree. By source: 2 D do-re on *A-* (43; AM
  1934/2005, AR II 2009, Nocturnale 2002 - spec already noted it);
  tone 3 b-b on *sæ-cu* where AR recites on c (16, AM and AR II); 1
  D/D2 forms crossed (10, mostly AR II 2009); 1 a `g gh` on *-men* (7,
  monastic/OP); the rest GregoBase noise (missing syllables, a "4 c"
  that is an E ending). No table error found.
- Psalms 12: 38 lines - verse 1 with solemn intonation (7404-7406),
  model-verse files from Bloomfield's transcriber (8130-8134: tenor
  lumped in one syllable, a demo flex line), a Graduale psalm (7536:
  intonation repeated, GR ending), editions without bold (4690: 8
  `stili` lines), 7832 "5 a" whose ending is not 5 a (mislabel?), and
  08195 where only our `†(,)` bar differs.
- Syllables 235 distinct (567 occurrences, 0.25%): consonantal i in
  i-spelled texts (*ad-iu-ván-dum*, *sub-iéc-tis*, *iú-gi-ter*: we
  give i a vowel); vowel pairs that are two syllables in names and
  Greek words (*Mí-cha-el*, *Sa-ul*, *á-e-ra*, *Ca-phár-na-um*: we
  read ae/au as one); *Euouae* as one word; *Israel* (161, the
  fitter's Hebrew list handles it); then not ours - *e-lé-i-son* (we
  = Liber), hymn meter (cui, Patre, quia), transcription slips.

### Rulings at T8 (so far)
- **6 columns** (genus, id, titulus, nostra, eorum, numerus): the
  count makes the syllable list one line per distinct word. Cost:
  none (corpus pinned at v0.4).
- **Syllables compare count + accent only** (T1 ruling 4), aggregated
  by distinct word, first id kept.
- **Transposed = not a disagreement** (counted apart, spec).
- **`vicaria` accepted**: AR p. 13* "quando Antiphona notatur cum A*,
  potest ad libitum adhiberi Differentia sequens" - an antiphon
  marked A* sings A or the starred form; 5 of the 10 A* antiphons are
  AR 1912's own and write A. Reading the label the book's way, not
  bending the table. Cost if wrong: 11 hidden lines.
- **Intonation per psalm**: fit with and without intonation on every
  verse, keep the one with fewer disagreeing verses (an edition
  choice, not the table's); the title says "(intonatio omni versu)".
- **Label cleanup in the test only**: mode "p" = peregrinus,
  "-star" = `*`, name patterns; repeat marks (ij, iij) are not words;
  non-Latin chants (GregoBase "Lingua" tag) skipped.
- 08134's header is malformed (multi-line value ending `;` not `;;`):
  our reader reports it as Gregorio would; the name supplies the label.

**After Fran's review (same day).** Fran accepted the sorting; the
375 lines are the manifest `probationes/fixa/psalmodia/discordiae.tsv`
(7-line `#` header; the committed-data check skips `#` lines). Gate
`psalmodia-oraculum` = `tools/psalmodia_oraculum.sh` (custos, OMISSA
without corpus), registered in `pythonica/silva.py`; `PSALMODIA_CORPUS`
declared in aedificatio.stml. Plant (build/plantae_psalmodia/t8.log):
manifest line `euouae 02022` deleted -> "NOVA: euouae 02022 ...", gate
FRACTA; restored, sana. Divider classes are ledger wishes
01M7ATXS3RG8H2R6P5XCTA46RH and 01M7ATXW4WY98G7K3JFHSCG8R4; fixing
either shrinks the manifest (the gate names the solved lines).
