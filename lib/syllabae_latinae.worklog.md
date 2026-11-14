# syllabae_latinae worklog

## 2026-11-12 — T1 measurement (psalmodia plan 2)

Scripts: scratchpad `t1/mensura.py`, `editiones.py`, `ar1912.py`,
`vocales.py` (python3 -I; data untrusted). Two witnesses:

- **GregoBase** csv (build/gabc/bacor_v0.4): 221 595 words, 515 360
  syllables, divided as each transcriber divided them.
- **AR 1912 OCR** (archive.org `antiphonale00anonuoft_djvu.txt`, saved
  to build/ar1912_textus/): the book our tones come from, Vatican
  edition. OCR drops hyphens and reads the acute as a circumflex, so
  only a hyphen that IS present counts as evidence (24 238 hyphenated
  words). A missing hyphen proves nothing.

### Finding 1: the count is unanimous, the style is not

Where the consonants go between two vowels changes the hyphens, never
the number of syllables. The fitter needs only the count and the
accent. The rules that decide the count (diphthongs, consonantal i/u)
are near-unanimous; the consonant placement splits GregoBase into
three house styles:

| group | Vatican-tagged | Solesmes-tagged | Dominican | **AR 1912** |
|---|---|---|---|---|
| st | \|st 822 : s\|t 265 | 2384 : 961 | 1605 : 38 | **205 : 3** |
| ct | \|ct 242 : c\|t 194 | 939 : 689 | 784 : 33 | **92 : 0** |
| mn | \|mn 210 : m\|n 302 | 626 : 711 | 573 : 82 | **13 : 3** |
| gn | 316 : 15 | 1168 : 126 | 593 : 7 | **34 : 0** |
| nct | n\|ct 151 : nc\|t 150 | 690 : 579 | 530 : 23 | **43 : 0** |
| str | 210 : 90 | 496 : 291 | 340 : 7 | **55 : 0** |
| pt | \|pt 32 : p\|t 87 | 183 : 282 | 12 : 229 | **19 : 0** |
| x (not ex-) | \|x 171 : x\| 14 | 575 : 133 | 345 : 21 | **200 : 3** |
| squ | s\|qu 33 : \|squ 5 | 184 : 61 | 54 : 67 | **s\|qu 4 : 0** |

GregoBase's "Vatican" tag is a mix of transcribers. The book itself is
consistent: the next syllable takes the longest group of consonants
that can open a syllable (*ju-stí-ti-a*, *San-ctus*, *pro-pté-re-a*,
*surre-xí-sti*, *As-sum-ptió-nis*, *us-que*).

Ruling (T1): the house style is AR 1912's, not GregoBase's majority
(the plan said "majority = rule"). Why: the tones and the examples are
AR 1912's, and GregoBase mixes three styles. Cost if wrong: hyphen
positions only, never a syllable count.

### Finding 2: AR divides compounds at the prefix

Prefix + vowel at word start, hyphen after the prefix (AR, hits where
the hyphen survives): sic-ut 77:2, in- 49:3 (*in-imí-cus*,
*in-í-qui-tas*, *in-í-ti-o*), ex- 46:3 (*ex-áu-di*, *ex-ér-ci-tus*),
per- 40:15 (*per-én-nis*, *per-í-mus*; the 15 are fragments and
*su-pe-rá-vit*), red- 36:2 (*red-émp-tor*), ab- 12:2 (*ab-í-it*), ad-
6:5 (*ad-o-rá-re*; the 5 are *A-dam*, *á-di-pe*, *A-do-nai*), sub-
5:0, ob- 2:1 (*ob-é-di-ens*, *ob-um-brá-bit*), trans- 1:0.
Look-alikes that are not compounds (GregoBase stems by frequency):
abyss-, Abel, Adam, adip-/adeps, Adonai, reduc-/redux-, redupl-.
Unknown, left to the rule: inan-, pericul-, subit-.

### Finding 3: vowels (GregoBase, decides the count)

- ae / oe / au diphthongs: 2260:251, 112:75, 3840:17. Every split
  case is a Hebrew or Greek name (*Is-ra-el* 251 written without
  diaeresis, *Mi-cha-el*, *Jo-el*, *No-e*, *Sa-ul*). So the divider
  gives *Is-rael*; the fitter's VOCES_HEBRAICAE (plan 2 FRAN 1) or a
  diaeresis (*Israël*, 76:0 split) or a forcing hyphen fixes it.
- eu splits (*De-us*, *me-um*) 3812:27; the 27 joined are heu, ceu,
  seu, eu-ge, pneu-ma, Eu-cha-ri-sti-a.
- i between vowels is a consonant: 4079:10 (*al-le-lú-ia* 3710,
  *e-jus/e-ius*, *cu-jus*, *hu-jus*, *ma-jor*, *e-ia*). The 10:
  *I-sa-i-as* (name), *in-tro-i-e-rit* (compound).
- initial i + vowel is a consonant: 413:13 (*Ie-sus*, *Io-án-nes*);
  the 13 are forms of *is/eo* (*i-e-rit*, *i-is-dem*).
- u after q, and after ng before a vowel, is a consonant: qu 1961:3;
  ngu+V never split in AR (43 hits: *sán-gui-ne*, *lin-gua*,
  *un-guén-tum*, *lán-gue-o*).
- su + vowel splits: *su-á-vis* 44:0, *man-su-e-tú-do* 17:0.
- ui splits: *cu-i* 64 vs *cui* 28 (hymn meter), *hu-ic* 16:0.
- ei splits (*me-i*, *De-i*) 2310:59 (*e-lei-son* minority).

### Consequence for T2

The 82 committed sample files mix house styles (00537 has *promp-sit*,
*cunc-ta* next to *o-mnis*) and hymn meter (*cui* one syllable). They
can test the COUNT and the accent syllable per word, not the hyphen
positions. Hyphen style is tested by hand cases taken from AR hits.

### Rulings at T1

- The plan's field `signatus` is a latina.h macro (`signed`); the
  field is `notatus` ("marked by an acute"). Cost if wrong: a rename.
- T2's sample oracle compares COUNT and ACCENT per word, not hyphen
  positions (finding above); hyphen style is pinned by AR hand cases.
  Cost if wrong: style drift goes unseen in the sample (hand cases
  still catch each rule).

## 2026-11-12 — T2 divider built

`lib/syllabae_latinae.c`: letters (UTF-8 -> base letter, acute,
diaeresis; combining U+0300-036F stick to the letter before; '-'
between two letters = forcing) -> vowels (rules V, left to right, so
`quia` sees the u of qu as a consonant before judging the i) ->
nuclei (ae oe au; eu only in the word list) -> boundaries (forcing,
then prefix/whole-word table, then the longest onset) -> spans.
Words up to 64 bytes run on the stack; longer ones borrow the piscina.

More AR 1912 evidence found while writing the test (OCR grep):
*sus-cé-pit* 3, *sus-ci-pe* 2, *sus-ti-nens*, *sus-pén-di-mus* (prefix
sus- before a consonant); *nobis-cum*, *vobis-cum*; *super-ex-altâ-te*
4; *red-émptor* 9 and *redem-ptorem* (so *red-ém-ptor*, the header's
*red-émp-tor* example was wrong); *di-sci-pu-lis* (not a prefix).
Added to the tables; header comment updated.

Header change: the "Eu- + consonant" exception would have joined
*eum* (common in psalms) into one syllable. GregoBase's 27 joined eu
are only heu, ceu, seu, euge, pneuma, Eucharistia: the exception is
now that list. *tl* and *dl* are not onsets (At-lan-tis).

Sample oracle (82 files): 3295 words judged. Skipped: syllables with
no vowel (`Ps.`, `℣.`, `†`), syllables with no notes (the cue
"Glória() laus()" in 00537), elision, text with spaces or `*`.
Every judged word has the source's syllable count except three,
listed in DISCORDIAE with these rulings:
- *Israel* 3 vs 2 (00537): Hebrew; the fitter's VOCES_HEBRAICAE.
- *cui* 1 vs 2 (00537): hymn meter (the same book writes *Cu-i* in
  the same hymn); psalms follow prose, *cu-i*.
- *Patre* 1 vs 2 (03324): hymn elision written `Patr<i>e</i>`.
Every word with the same count also has the accent on the same
syllable as the source (0 mismatches).

Plants (silva.planta, build/plantae_psalmodia/t2.log), each red then
green: (a) diaeresis ignored in diphthongs; (b) st not an onset;
(c) i between vowels kept a vowel - red in the hand cases AND in the
sample (27 count errors, all *allelúia* as 5).

Bug caught before the first run: i32 is unsigned, so `k - I` at
k = 0 and a `limes = -I` sentinel silently broke the `>= 0` / `< 0`
tests. Index sentinels are s32; neighbour checks guard `k > 0`.
