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
