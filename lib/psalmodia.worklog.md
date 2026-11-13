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
