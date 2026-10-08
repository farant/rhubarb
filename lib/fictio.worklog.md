# fictio worklog

## 2026-10-08 - first implementation (norma-plan-2 N1)

Header approved by Fran (7c635bb5); spec project-specs/norma-spec.md §IV.

**Corpus**: 48 sentences VERBATIM from M. Tullius Cicero, In Catilinam
(Project Gutenberg EBook #226 "Cicero's Orations", Language: Latin, a
public-domain text), read from ../gutenberg-mirror/2/2/226/226.txt on
2026-10-08 - from the orations themselves, not the editor's argumentum
(which already shows a typo, "perditissiis"). Each sentence was checked
to occur verbatim in the file; sentences ending on a praenomen
abbreviation ("C.", "M.") or starting mid-name were excluded. The first
Latin candidate, PG #19635 "Biblia Sacra Vulgata - Psalmi XXII", is a
LibriVox AUDIO index with no text.

**Generators**
- 64-bit integers: two 32-bit sors draws, rejection sampling (limen =
  (0 - spatium) % spatium) - sors_inter only takes s32 bounds; full s64
  range (spatium wraps to 0) returns a raw 64-bit draw.
- Names: praenomen + nomen gentile (2/3), or feminine gens + ordo
  ("Cornelia Tertia"). Emails lowercase name with '.' @ example.org/com/net
  (RFC 2606: never a real mailbox).
- uuid v4: 16 bytes, version nibble 4, variant 10xx.
- Dates: Hinnant's civil-from-days (no fasti dependency in generation;
  fasti_ex_iso judges the output in the tests). Leap day 2000-02-29 pinned.
- Text: runes drawn from a UTF-8 alphabet (NIHIL = a..z), length in runes.
- Difficult drawer: empty, combining mark + precomposed, non-BMP, RTL +
  LTR, quotes + backslashes, U+0000 (explicit lengths), whitespace only,
  emoji + skin-tone modifier, U+FEFF, and a 2000-rune long string.

**Sample (seed 2026)**: Decimus Aemilius / cornelia.tertia@example.net /
2b4b3e2f-872c-4ff1-9781-a759b6915c4d / 2001-12-09T18:49:43Z / "Educ tecum
etiam omnes tuos, si minus, quam plurimos; purga urbem."

**Plants** (2823 assertions): range off by one -> red; uuid version not
set -> red; a difficult string's length cutting a UTF-8 sequence -> red;
civil-date leap term dropped -> red. The plan's "rejection loop removed"
plant was not testable (bias below resolution) - replaced.

**Renames**: local `nomen` (latina.h typedef macro!) -> appellatio (examen
caught it); doe/yoe/doy/alph -> dies_aerae/annus_aerae/dies_anni/litterae.
