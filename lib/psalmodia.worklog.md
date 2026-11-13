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
