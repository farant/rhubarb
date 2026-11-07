# psalmodia — psalms sung to psalm tones (tone table, Latin syllables, fitter)

*2026-11-07. Born in sexta after gabc part 2 merged (#8) and its review
minors were closed. The question (Fran, 2026-11-06): what would a
psalm-tone generator involve — the first step toward the Divine Office
named in `gabc-spec.md` §VIII. Answer given in conversation; this spec
records it. Fran agreed the tone table comes first and that its file
format is reviewed before any tone is transcribed. Each decision below
says whose call it was.*

*Discipline: PULL — nothing is built ahead of the phase that needs it.
Every claim about the corpus below was measured on 2026-11-07 (commands
in §I); claims about chant practice that were NOT checked against a
book are marked and repeated in AUDIENDA.*

**What it is.** A generator that takes **a psalm tone** and **a pointed
psalm text** and writes **gabc**. Everything downstream exists: the
gabc reader, the converter to carmen, the antiphonale pane (drawing,
playing, live reload), `auditio -gabc`. Three pieces, built in this
order:

1. **The tone table** — the eight psalm tones with their endings (and
   the special tones) as DATA, one file, plus a small reader.
2. **Latin syllables** — dividing ecclesiastical Latin words into
   syllables and finding each word's accent from the printed accent
   marks. A separate library: `verba` in carmen and the open wish for
   accented sung text (01M62BR70ZS0ZZT6TD9HPQGTNN) want the same thing.
3. **The fitter** — tone + pointed text in, gabc out.

**Who uses it.** Claude and Fran, to sing psalms in the antiphonale
pane: antiphon, psalm, antiphon. Later the Office: the day's hours
from the liturgical calendar (§VII). Not this build, but nothing here
may block it.

---

## 0. What a psalm tone is (the domain in brief)

Psalms are not composed note by note. Each verse is sung to a short
formula. Every verse splits at `*` into two halves:

```
 intonation   tenor (reciting note) ............  mediant  *
              tenor ............................  termination
```

- **Intonation** — two or three rising notes on the first syllables;
  first verse only (every verse in the solemn canticle tones).
- **Tenor** — one repeated pitch for most syllables. This is what lets
  one formula fit a verse of any length.
- **Mediant** — the cadence closing the first half.
- **Termination** — the cadence closing the verse. Each tone has
  several (*differentiae*, labelled like "VIII G"); the antiphon says
  which one, so the psalm leads back into it.
- **Flex** — an optional dip on the tenor in a long first half (`†`).

Cadences attach to **word accents**, not syllable counts, so a
cadence is fitted **backwards from the end** of its half-verse: the
last accent gets the accent note, syllables after it get the closing
notes (an extra unaccented syllable gets an added note — printed as a
hollow note), a fixed number of syllables before it get the
preparatory notes, everything earlier stays on the tenor.

Liturgical books print the pointing: **bold** = the accented syllable
a cadence note falls on, *italic* = preparatory syllables. The written
psalms in GregoBase use exactly this (measured, §I).

---

## 1. Decisions

| # | decision | whose call |
|---|---|---|
| P1 | **Output is gabc text and nothing else.** The fitter writes a gabc file's text (header + body); reading, drawing, converting, playing are the existing libraries'. No new pane, no carmen code in the fitter. | Claude, from the conversation; Fran agreed |
| P2 | **The tone table is DATA**, one file (working path `musica/toni_psalmorum.stml`), read by a small reader. The format is shown to Fran and agreed **before** any tone is transcribed (it is this library's interface). How it reaches binaries (embedded like `instrumentarium.stml` via a GENERATUM C file, or read at run time) is the builder's call; embedding is the house precedent. | Fran: "step 1 would be the tone table"; format review = Fran |
| P3 | **Tones are transcribed from a printed book, never from memory**, each entry citing its source (book, page). Prefer an edition printed by 1930 (the house rule for GregoBase imports, `tools/gabc_arcessere.sh`); a later book (Liber Usualis 1961) may be used to check or to add endings, cited. | Claude's call (§VI q2 asks Fran which book he can see) |
| P4 | **The format must hold every tone the Office needs**, even if v1 transcribes fewer: the eight simple tones with all their endings, the tonus peregrinus, tonus in directum, and the solemn tones of the Magnificat and Benedictus (intonation on every verse, longer mediants). v1 transcribes the eight tones + peregrinus; the solemn tones follow when a consumer pulls. | Claude's call |
| P5 | **A cadence is a list of tagged notes**: *preparatory* (one per syllable, a fixed count), *accent* (falls on an accented syllable), *added* (the note an extra unaccented syllable after an accent takes), *closing* (the notes after the accent). Two-accent cadences are two accent groups. Variant cadences the books prescribe (the "abrupt" cadence for a half-verse ending in a monosyllable or an indeclinable Hebrew name; the flex) are part of the tone's entry, not code. The exact per-tone counts come from the book (P3), not from this spec. | Claude's call |
| P6 | **Pointed text format**: plain UTF-8, one verse per line, `*` at the mediant, `†` (or `+`) at a flex, **accent marks on the stressed vowel** as liturgical books print them (á é í ó ú ǽ). Words of two syllables are accented on the first without a mark. A way to force a syllable division for the rare word the rules divide wrongly is the builder's choice. The bold/italic of a printed book is NOT input — the fitter computes it. | Claude's call |
| P7 | **Latin syllables are their own library** (working name `syllabae_latinae`): division by ecclesiastical rules (vowels and diphthongs ae/oe/au/æ/œ, consonant clusters, mute + liquid, x, qu/gu, initial i/j as consonant) and accent from the marks (P6). Pure, no allocation beyond the caller's piscina. | Claude's call (house rule: split generic pieces into libraries) |
| P8 | **Accent v1 = from the marks.** A word of three or more syllables without a mark is a located diagnostic (line:col, the word), not a guess. Finding accents without marks (vowel-length dictionary) is a named future (§VII): no house data has vowel length (AUDIENDA). | Claude's call |
| P9 | **The fitter is total**: any pointed text gives gabc or located diagnostics, never a crash. Short half-verses (fewer syllables than the cadence wants), a first half too short for intonation + mediant, a flex in a one-word half — each has a defined result, taken from the book's rule where it gives one, else the builder's (worklogged). | house standard |
| P10 | **The output looks like the books**: the cadence's accented syllables in `<b>`, preparatory ones in `<i>`, added notes hollow (`r`), `(:)` at the mediant, `(::)` at the verse end, verse numbers, the clef and header (`name`, `mode`, `annotation` = "8 G") from the tone entry. *Gloria Patri* / *Sicut erat* appended as two verses by default, switchable. The antiphon itself is not part of the output (v1). | Claude's call, from the GregoBase psalms (§I) |
| P11 | **Rhythm is the converter's**: the fitter writes plain puncta on the tenor and the tone's own marks (mora `.`, episema) in the cadences; `gabc_carmen`'s Solesmes reading does the rest. Whether reciting should move faster than a neume's punctum is an open question (§VI q3). | Claude's call |
| P12 | **Tool surface**: one flag on an existing tool, e.g. `auditio -psalmus TEXT_FILE -tonus "8 G"` printing gabc (and, with auditio's play flags, playing it); Claude writes the result to a `.gabc` file and opens it with `praeco antiphonale`. The exact surface is the plan's call. | Claude's call |
| P13 | **Out of scope for this build**: the Office (choosing hours, psalms, antiphons by date); Office texts; automatic accents (P8); polyphonic psalmody (falsobordone); the antiphon + psalm + antiphon page. | Claude's call |

---

## I. What exists (measured 2026-11-07)

- **The gabc stack**: reader `include/gabc.h` (keeps `modus`,
  `modus_numerus`, `differentia` from the header, gabc.h:327-329;
  keeps `<b>` `<i>` styling, G3), converter `include/gabc_carmen.h`,
  layout + drawing + pane (`include/antiphonale.h`), `auditio -gabc`,
  `praeco antiphonale <via>`.
- **The full GregoBase corpus is cached** at
  `build/gabc/bacor_v0.4/x/` (bacor GregoBaseCorpus v0.4, CC0, fetched
  by `tools/gabc_arcessere.sh`; not committed): 9,130 gabc files,
  `csv/chants.csv` 13,785 lines with columns `office_part`, `mode`,
  `mode_var` (= the ending), `gabc`, `gabc_verses`. Measured with
  `python3 -I` scripts reading the CSV:
  - **3,818 antiphons**; **1,333** record their ending in `mode_var`,
    109 distinct (mode, ending) labels — messy: `8 g` 202, `1 f` 117,
    `7 a` 103, `1 g` 70, `2 d` 64, `8 G` 58, also `VIII g` with an
    empty mode, `2 * a`, `4 A*`, `Per.` (peregrinus). Labels need
    normalizing; whether `g` and `G` are the same ending is unknown
    (AUDIENDA).
  - **1,229 gabc files** write a psalm-tone ending out on the vowels
    of *sæculorum. Amen*: `E(i) u(i) o(j) u(i) a(h) e.(gf)` (03290,
    mode 7). **610 antiphons have both** a label and a written ending
    — by mode: 1:105, 2:73, 3:22, 4:51, 5:22, 6:15, 7:79, 8:142,
    unlabelled mode 101. In the committed 82-chant sample
    (`probationes/fixa/gabc/gregobase/`), 15 do.
  - **35 written-out psalms (`office_part` ps) and 11 canticles (ca)**,
    many with the tone in the name or annotation (`109-7c2`,
    `annotation: 8.G`, "Dixit Dominus, Psalm 109, Mode 1g"), typeset
    the book way: `la(j)lem,(j) <b>Dó</b>(k)mi(jr)num :(j.) (:)` — bold
    accent, hollow (`r`) added note, mediant bar (id 273).
- **No Latin syllable or accent code in the house**: `ls lib | grep
  -i "syllab\|latin\|accent"` finds nothing; `verba` in carmen takes
  syllables already divided by the author (`include/carmen.h`:142,
  vowel rules only). `oratio` classifies Latin words from WORDS stems
  (`oratio/fontes/oratio_partes_la.h`) — no vowel length there as far
  as read (AUDIENDA).
- **The liturgical calendar** (`include/calendarium_liturgicum.h`)
  is the US calendar of the current Roman Rite and already computes
  the **psalter week** of the Liturgy of the Hours
  (`calendarium_hebdomada_psalterii`, calendarium_liturgicum.h:334;
  plan `project-specs/liturgia-plan.md`, arc closed 2026-10-15).
- **Data embedded the house way**: `tools/carmina_infigere.sh` turns
  `carmina.stml` + `instrumentarium.stml` into
  `lib/carmina_infixum.c` (GENERATUM).

---

## II. The tone table

One entry per tone and ending. Shape (PROPOSITUM — the real format is
agreed with Fran first, P2; the pitches below are placeholders, not
transcribed):

```xml
<tonus modus="8" clavis="c4" tenor="j" fons="Antiphonale 1912 p. …">
  <initium>g h j</initium>
  <flexa>h</flexa>
  <mediatio>'k (j) j</mediatio>
  <terminatio differentia="G">p:i p:j 'h g</terminatio>
</tonus>
```

(`'` accent, `p:` preparatory, `( )` the added note for an extra
syllable.) The reader gives: the tone by (mode, ending) with label
normalization (`VIII g`, `8 g`, `8.g` → one key), the clef, and each
part as tagged gabc notes. Unknown label = a clear "no such ending"
with the tone's list.

**The check that makes the table trustworthy**: every termination,
fitted to *sæ-cu-ló-rum. A-men* (what EUOUAE stands for), must equal
the endings written out in GregoBase antiphons with that label, after
the pitches are normalized for clef. Where a label's antiphons
disagree among themselves, the report says so (transcription noise or
a real variant) — Fran sees it, the table is not bent to fit. The same
check in reverse names the ending of an unlabelled antiphon.

---

## III. Latin syllables

`syllabae_latinae` (P7): a word (UTF-8 chorda) → its syllables (byte
offsets) and the index of the accented one (−1 if unknown). Rules of
ecclesiastical Latin as Solesmes books divide (the books' division is
the test: every syllable boundary in the GregoBase psalms and
antiphons is a test case — the gabc text is already divided). Accent
from a mark on a vowel (P6); two-syllable words on the first;
monosyllables unaccented for cadence purposes unless the book's rule
says otherwise (AUDIENDA).

---

## IV. The fitter

`psalmodia` (working name): tone entry + pointed text → gabc text +
diagnostics. Per verse: split at `†` and `*`; syllabify; for each
cadence, place accents backwards from the end (P5), preparatory notes
before, added notes for extra syllables, tenor for the rest;
intonation on verse 1 (every verse for solemn tones). Styling and bars
per P10. Header per P10.

**The end-to-end check**: the 35 written-out psalms in GregoBase.
Strip the notes and the bold/italic from one, keep its accents, fit it
to its labelled tone, compare with the printed version (notes,
bold/italic, bars). Differences are findings, sorted into table
errors, fitter errors, and edition variants.

---

## V. Tests and corpus

- **Committed fixtures** follow the GregoBase sample rule (sources
  printed by 1930, `tools/gabc_arcessere.sh`): a handful of endings
  and psalms that satisfy it, plus hand-written cases for every edge
  case (P9), each red first with a plant.
- **The corpus checks** (EUOUAE endings §II, written psalms §IV) read
  the cached corpus in `build/`, as a gate in the shape of
  `musicxml-oraculum` (fabrica action, skips cleanly with a message
  when the corpus is absent). The corpus is CC0, but the editions it
  transcribes claim rights, so it stays out of git.
- **Totality**: random and truncated pointed texts give gabc or
  located diagnostics.

---

## VI. Open questions

1. **Which Office** — the 1962 Breviary (Gallican psalter, texts
   with accents from projects like Divinum Officium) or the Liturgy of
   the Hours (Nova Vulgata psalms, four-week psalter, which our
   calendar already counts). It does not block the tone table, the
   syllables, or the fitter — only which texts the Office step uses.
   Fran's call; licences of both text sources unchecked (AUDIENDA).
2. **Which book Fran can see** for transcription (P3): a scan of the
   1912 Antiphonale Romanum or another pre-1930 edition, or the Liber
   Usualis 1961. Claude transcribes from what is available.
3. **Reciting speed**: should tenor syllables be shorter than a
   neume's punctum (speech rhythm) — a converter option, or not?
4. **Where pointed texts live** (a file per psalm under
   `musica/psalmi/`?) — the plan's call once Q1 is answered.

---

## VII. Named futures

- **The Office in the pane** (`gabc-spec.md` §VIII): the day's hours
  from the calendar; antiphon + psalm + antiphon as one page; the
  psalm's ending taken from the antiphon's `mode-differentia`.
- **Accents without marks**: a vowel-length lexicon (own data), so
  any Latin text can be pointed.
- **Solemn canticle tones** transcribed (the format holds them, P4).
- **Falsobordone** and other harmonized psalmody, through carmen.

---

## AUDIENDA (not verified when written)

- The per-tone structure (how many accents and preparatory syllables
  each mediant and termination has), the abrupt-cadence rule (which
  tones, which endings), and the flex rule — stated from memory of the
  books, deliberately NOT written into this spec; the transcription
  (P3) settles them.
- Whether `g` and `G` (and `g2`, `G*`) in GregoBase labels are
  different endings or transcriber habit.
- That the written-out GregoBase psalms follow the same tone table as
  the labels (edition variants are likely).
- That WORDS (oratio) carries no vowel length.
- How monosyllables at a cadence are treated beyond the abrupt
  cadence.
- Availability of a pre-1930 Antiphonale scan; licences of Divinum
  Officium texts and of the Nova Vulgata.
- Whether existing web tools generate gabc from psalm tones (believed
  to exist; useful once as an oracle, under the third-party rules).
