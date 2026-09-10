# oratio_arbor worklog

## 2026-09-04 — T1: registry and lexicon; whitespace as tails

Spec §3 sketched a `spatium` genus beside `vocabulum`. Building the
registry I dropped it: a node per space would double the node count
of the corpus (the md corpus has 1.2M tokens) for no query anyone
wants, and it would make "the word and its spacing" two things. The
tree now follows STML's possessor law instead: the whitespace after a
word or punctuation mark, up to the first newline inclusive, is the
element's `cauda`; the remainder of a blank-line run belongs to the
paragraph's `cauda`; leading whitespace to `documentum.praefixa` or
`paragraphus.praefixa`. A sentence has no whitespace slots at all —
its last element carries the space before the next sentence. PUNCTUM
is its own token genus because the period is where abbreviations,
initials and sentence ends are decided; everything else punctuation is
a run of `INTERPUNCTIO`. The vocabulum carries its stage-3 slots from
birth (`analyses`, `classes`, `linguae`) so the registry never has to
be reordered; absent lists are empty under materia's write-once law.
First run green (130 assertions, round trip of "a\n"); the planted
fault (loci_numerus of `sententia` 1 → 2) turns the contiguity check
red.

## 2026-09-04 — T2: one lexer for the whole stream, and the unicode floor's two exceptions

Prose needs no container context, so `oratio_lexare` is a whole-stream
lexer, unlike md's per-line scanners. The corpus gate ran it over every
tracked markdown file on the first green run: 1,134 files, 17.45 MB,
6,696,475 tokens, byte-exact, 0.68 s for the whole suite. Two things
the decision "any UTF-8 sequence is a letter" could not survive contact
with real text: NBSP (C2 A0) is a space, and the General Punctuation
block (U+2000–U+206F: curly quotes, dashes, ellipsis, thin spaces) is
punctuation, with U+2019 an apostrophe between letters. Those are the
only two exceptions, both named in the header; everything else at or
above 0x80 joins a letter run, unvalidated, until the unicode story.
Joiners are decided by the bytes on either side (a hyphen or
apostrophe between letters), a lone period is PUNCTUM, a run of dots
is punctuation, and other punctuation is one token per byte except
runs of the same byte — so the parser sees `"` `)` `.` separately but
`---` and `!!!` whole. The const source pointer goes into the chorda
through the same union md uses (-Wcast-qual). The planted fault
(hyphen emitted as punctuation) went red on `well-known`.

## 2026-09-04 — T3: the tree, two passes per paragraph, and 81,844 paragraphs first try

`oratio_arbor_parsare` builds the tree in two passes per paragraph:
first the token stream is grouped into elements (vocabulum, numerus,
interpunctio) with their tails, and a whitespace run with two or more
newlines closes the paragraph — the element's tail takes the run up to
the first newline inclusive, the paragraph's tail the rest up to the
last newline, and the spaces after that become the next paragraph's
prefix; then the paragraph's elements are segmented into sentences with
one element of lookahead. The corpus gate walks every markdown
paragraph through md's own tree (extent = first to last source token
of the subtree, no STML in the loop) and round-trips its bytes through
oratio: 81,844 paragraphs, 0 fractures, 155,328 sentences, 1,918,204
words, 1.45 s for the gate. All sixteen sentence fixtures — the cases
`sententia_fissio`'s header lists plus initials, internal-period
words, and a non-ASCII capital — passed on the first run; the two
failures were my own byte counts on fixtures with escapes.

Decisions worth naming. Elements are "letters first": a token starting
with digits is a numerus and may take a letter suffix (3rd, 1980s),
a token starting with letters is a vocabulum and may take digits
(B2). A period joins letters on both sides without space (e.g, i.e,
www.x) as a part; the trailing period is always its own element, and
the abbreviation table (v1: English titles and citations plus a
handful of Latin ones — cap, lib, q, a, ad, art, dist) is consulted
for the vocabulum before it, with a single capital letter counting as
an initial. Bytes at or above 0x80 count as a capital when deciding a
boundary: splitting is preferred over merging until the unicode story
gives real case. Control bytes are whitespace-class for structure.
Planted fault (`mr` dropped from the table) went red on the first
fixture.

## 2026-09-04 — T4: projection, canon, and the price of a node per word

Three things the STML round trip taught. (1) The structural comparator
compares parent pointers, and the reader sets them; a parser must
call `materia_arbor_patres_figere` after building or every round trip
fails at path 0.1 with "pater-nullitas" — md's parser does, mine did
not until now. (2) The canon's corpus judgment found a real shape the
canon did not describe: after a number's letter suffix, a joiner
(`1980s-era`, `1980s'`) finds no word to join and becomes punctuation
with an `or-hyphen` or `or-apostrophus` signum — 104 and 31 sites over
the corpus. The parser is right (the tokenizer only made a joiner
because letters flanked it), so the canon now admits both in `signum`
with a nota; "parser decides, canon describes, corpus falsifies".
(3) Cost: the prose tree has a node per word (1.9M words in the
corpus), and the STML round trip of the whole corpus took 40 s, the
canon judgment 20 s, against 1.5 s for the byte gate. Both corpus
loops now sample every fifth file (226 of 1,134; 8.5 s and 4 s), and
`ORATIO_CORPUS_TOTUS=1` runs everything. Sampling is honest as long as
the thresholds say so — the self-measure floors were lowered with it.
The derived-token channel (stage 3's `classes`) was exercised now: a
hand-placed derived token writes with `f="1"` and a carried position,
reads back structurally equal, and the emitter omits it. Planted
faults: the position hook declaring every token a source token (red on
that channel test) and a renamed canon rule (red in the drift guard).
The first plant I tried — removing the hook — was refused by
`planta`'s pre-flight as an unused variable, exactly the refusal it was
built for.

## 2026-09-04 — T5: totality first try, and the projection is 34× the source

The totality gate (random bytes, mutated and truncated fixtures, nested
parentheses, brackets, straight and curly quotes, chained
abbreviations, and fifty thousand of each of parentheses, words,
paragraphs, and periods) passed on its first run: 386 assertions, no
crash, every emission byte-exact. That is the byte law doing its work
— a tree that is total by construction cannot lose bytes — and it made
the planted fault hard to choose: most parser mutations keep the tree
total. The plant that works is in the lexer, one byte off every token
length, which no consumer above the lexer can repair.

The computus twin over five vendored Gutenberg texts (three Latin, two
English, boilerplate stripped, FONTES.md) published the number T4 had
paid for without seeing: the STML projection is about 34× the source
(Cicero: 92,723 bytes → 3,167,255 bytes of STML, 18 MB of arena), and
STML write plus read cost about ten times the parse. Parse itself is
fast (4 ms for 92 KB). The node-per-word tree is the right shape for
the classifier, and this is what it costs to serialize; if the corpus
gates ever need to run whole, the answer is a leaner projection of
`vocabulum`, not a different tree. `silva.metiri('x.txt')` now routes
to `oratio/computus.sh`. The fixture swap bit the zsh trap again: an
unquoted variable does not word-split, so the loop fed awk one long
file name — `${=var}` or no loop.

## 2026-09-04 — T6: sententia_fissio measured; stage 1 complete

The comparison gate normalizes both splitters' sentence starts and
merges them: over the five fixtures and 227 sampled markdown files,
fissio finds 38,899 sentences, oratio 36,762, and 35,922 starts are
common — 92.3% of fissio's, 97.7% of oratio's. On English prose they
agree exactly (Lincoln 140/140) and oratio adds only paragraph-end
units without terminal punctuation (headings, table rows: Trinity +44).
The disagreements fall into three classes, all visible in the first
ten lines printed per fixture: (1) fissio treats a line end as a
boundary — every verse line of Propertius is a sentence to it (247
against 52), which is arguably right for verse and wrong for wrapped
prose; (2) fissio ends a sentence at `?` or `!` even when a lowercase
word follows ("quam diu etiam…"), where oratio's capital rule keeps
going; (3) oratio ends a sentence at a paragraph end without
punctuation, fissio does not. Nothing here is pinned: the gate holds
two sanity floors (both sides over a thousand sentences, common starts
over half of each). Whether oratio replaces fissio behind its API is
Fran's call from this report; if it does, class (1) suggests a verse
mode (short lines in a paragraph as units) before the swap.
`./oratio/sententiae.sh <x.txt>` prints one sentence per line with
byte extents. Stage 1 is complete: six tasks, seven gates, every one
born red.

## 2026-09-04 — T6b: the text-form layer (`forma`) before the sentence reader

Decision 23 built: a fourth `paragraphus` slot, `forma` (INDEX), decided
by `oratio_forma.c` between the element pass and the sentence pass, and
consulted by the reader — versus, tabula and index read a LINE as the
unit, titulus is one unit, prosa keeps the v1 rule. Seal moved
b27fe13a → 93c1c9cf (canon pin, computus golden regenerated twice: once
for the slot bytes, once for the verse units). Rules are a data table
(`ORATIO_REGULAE_FORMAE`, ten rows, first match wins, no match = prosa
= the merge bias of decision 24): two titulus rules (one line, all
capitals / short without terminal punctuation), tabula (inner gaps of
two spaces after a word or number, a tab, or two pipes on half the
lines), two index rules (lines opening with a number, a list sign with
a following space, a letter or Roman numeral plus `.`/`)`, or closing
with a page number after leaders; and short capitalised lines all
terminally punctuated — a Contents block), three versus rules and two
distichon rules. `./oratio/formae.sh <x.txt> [-machina]` prints each
paragraph's forma, the rule that fired and every indicium;
`sententiae.sh` gained a forma column.

The primary indicium is the VOLUNTARY BREAK: a line ended although the
next line's first word would have fit within the paragraph's width
(longest line, lines carrying a token of 24+ bytes excluded so a URL
cannot fake the width). A greedy wrapper never produces one. Two things
the corpus taught in the first hour: (1) Gutenberg prose was wrapped by
TYPISTS with a ragged margin — Lincoln's paragraphs show 70–100 %
"voluntary" breaks at widths 69–79 — so the fill test alone misreads
wide prose as verse; the fix is a width bound (verse never reaches the
prose wrap width: hexameters top out near 56 columns, prose wraps at
70+), `latitudo <= 64`, plus one more signal per verse rule (line ends
punctuated ≥ 40 %, or capitalised starts ≥ 60 %), which also rejects
prose typed in a narrow column (Lincoln's "Resolved:" quotation, width
48, 36 % punctuated). (2) An elegiac stanza of four lines has near-equal
widths (44–48), so nothing "would have fit"; the punctuation rule
(width ≤ 60, four or more lines, ≥ 70 % of lines ending in punctuation)
catches it — the threshold sits between Propertius I.5 (62 lines, 72 %)
and Lincoln's Constitution quotation (6 narrow lines, 66 %), both
recorded here as the boundary cases.

Two substrate lessons. The paragraph's `praefixa` does not hold the
first line's indentation (the document praefixa or the previous
paragraph's cauda does), so the first line's column-0 start comes from
the first token's `columna` (1-based bytes) — without it every first
line was measured from its first word and the elegiac indentation
alternation read as 0/6. And a hand-built paragraph node in the
registry gate carried the slot count as a literal `III`: the fourth
slot read out of bounds and the writer crashed; nodes built by hand
now take `loci_numerus` from the registry.

Markdown corpus (227 files sampled, 14,862 paragraphs): prosa 11,879,
titulus 2,869, versus 76, index 21, tabula 17. The first census showed
117 index paragraphs — 40 of them `**Who:** …` lines in the episode
files, because the `**` run matched `*` as a list sign; a list sign is
now one byte followed by a space. What remains under versus is the
prayer in CLAUDE.md, the one-liner list in RELAX.md, and 26 two-line
`**Who:**`/`**Where:**` field pairs read as two units each (correct);
one wrapped bullet item in a task file reads as two units (a false
split, count 1, left as data).

Fixture census: Propertius 21 versus / 25 titulus / 0 prosa — 731 units
where T6 found 52; Lincoln 41 prosa / 2 titulus / 0 versus; Cicero
42 / 5; Hilarius 28 / 4; Trinity 90 prosa, 19 titulus, 2 versus (the
title block and an address block, both correctly line-by-line), 1 index
(the Contents block). The fissio comparison re-run: Propertius fissio
247 / oratio 731 / common 239 (class 1 closed from the other side —
oratio now covers 97 % of fissio's starts there and adds the
unpunctuated lines by decision); Lincoln exact 140/140 with five
heading units extra; overall common 93.0 % of fissio's, 95.7 % of
oratio's.

Hand-judged set (`fixa/iudicia`, `[[` before each unit start, pins
only rising): Propertius 8/8, Lincoln 8/8, Trinity 25/25 (title block
per line, Contents per line, the quotation's four sentences, "Brig.
Gen. Thomas Farrell" as ONE unit — the titulus rule spares it the
abbreviation split), Cicero 10/14. The four Cicero misses are one
class: in this Gutenberg edition a sentence boundary is marked by TWO
SPACES after `?` or `.` even when a lowercase word follows ("nostra?
quam diu", "intellegit.  consul") — the typewriter convention as
editorial punctuation. Recorded as data (count 4 in 14); a rule
"terminal + two spaces + lowercase = boundary" is the candidate, to be
measured over the corpus before it is added. Gate born red by a plant
in the percentage helper (hundredths → tenths, every threshold
unreachable). 171 assertions, 0.5 s.

## 2026-09-04 — T7: Whitaker's WORDS vendored and compiled to a sealed table

Stage 2 opens. The four data files of `mk270/whitakers-words` (commit
1f2f0fb0, 2026-08-26) are vendored verbatim, CRLF and all, under
`oratio/vocabularium/la/` with `LICENTIA.txt` (the README's Licensing
section) and `FONTES.md` (URL, commit, byte and record counts, the
column law). `oratio_vocabularium.c` reads them and emits ONE nuntium
stream: a header record (version, source, counts) then 39,335 stem
records, 1,797 inflection records, 343 addon records and 79 uniques,
each a nested message with numbered fields; empty strings are simply
absent; the five one-letter codes ride as varints. The stream is
3,816,483 bytes — 0.62 × the sources — sealed by SHA-256 (first 16
hex: f598155c06f52682) and committed as `oratio/vocabularium/la.bin`.
Coction takes 24 ms, the full read-back (recensio) 3 ms.
`./oratio/vocabularium.sh [-coquere] [-scribere]` is the instrument;
the gate `probatio_oratio_vocabularium` (77) pins source bytes, record
counts, table bytes and seal, proves coction(sources) == committed
table byte for byte, decodes sample records against the sources, and
shows that a truncated table is refused and a flipped byte moves the
seal. Born red by moving the translation column (DL_TRAN_AB 100 → 101):
the first record no longer fits and coction stops at line 1.

What the sources taught. DICTLINE.GEN is fixed-width exactly as the Ada
declares it: four stems of eighteen columns each followed by a space
(76), the part record from column 77 (24 wide), the five codes from
101 separated by spaces, a space at 110, the meaning from 111 to the
end of line (padded to 80 in most records, hence lines of 187–191
bytes). The compiler asserts every one of those positions on every
record, so the byte pin guards the file and the assertion guards the
reading. INFLECTS.LAT is whitespace-separated with trailing `--`
comments on some records; the tail is regular once they are stripped
— stem key, ending length, the ending only when the length is not 0,
age, frequency — and declension/variant are present only for N V VPAR
ADJ PRON NUM SUPINE. The first record in file order is an ADV (line
27), not the noun `a` ending my first test assumed: pin records by
SEARCH, not by position. ADDONS.LAT obeys a three-line law by POSITION
(head, record, meaning): twelve meaning lines begin with the text
"TACKON …" and eleven with "PACKON …", so a grep for heads counts 355
and lies; the positional loader counts 343. UNIQUES.LAT is 79 clean
triples.

Substrate lessons: the xar reset is `xar_vacare` (examen's implicit-call
warning named it before the compiler did); a `const char*` into a
chorda needs the union laundering again; a record ordinal passed to an
`s32` parameter must itself be `s32`. Next: T8 — load the stream into a
`tabula_dispersa` by the folded key (u/v, i/j, case), then lookup as
stem + ending with tackons and addons.

## 2026-09-04 — T8: Latin lookup on the sealed table

`oratio_vocabularium_la` loads `la.bin` once (22–25 ms: the stream is
read back into record arrays, then three hashes by FOLDED key — stems
with their stem key, non-blank endings, unique forms — plus lists of
blank endings, internal blank stems and enclitic tackons) and answers
`quaerere(forma)` with an ordered list of analyses. Folding is
decision 3: ASCII lower-case, v→u, j→i, the ligatures æ/œ opened,
macrons and breves dropped, anything else left as it is — both the
table's keys and the query pass through the same function, so the
match is WORDS' `Equ` exactly. The matching law is transcribed from
`words_engine-word_package.adb` (Reduce_Stem_List): part of speech with
VPAR and SUPINE resolving to V and PACK entries accepting PRON
inflections; stem key equal unless the inflection says 0 (numerals
exact); declension/variant with the inflection's 0 0 meaning any
declension but 9 and d 0 meaning declension d; noun gender with X any
and C either M or F; adjective and adverb degree with the entry's X
meaning "derive the degree from the stem key" (1–2 positive, 3
comparative, 4 superlative; adverbs 1/2/3) — without that rule `amare`
came back four times (X, POS, COMP, SUPER all on key 1); prepositions by
governed case. Search order is WORDS' too: unique forms, then every
stem+ending split (ending 0–7 bytes, stem 1–18), and enclitic tackons
(-que -ne -ve, the three with base X) ONLY when the whole word found
nothing — `sine` is never si+ne, `virumque` yields the tackon record
first and eight hosts that carry it. Results are ordered by dictionary
line (WORDS' own order), longer ending first, then inflection order.
The lemma is a first form derived from the inflection table itself
(stem 1 + the first key-1 inflection that is NOM S, or NOM S M, or PRES
ACTIVE IND 1 S — PASSIVE for deponents, 3 S for impersonals; the stem
alone for indeclinables): vir amo puella bonus rex ago loquor licet
amare et. The full dictionary form with principal parts is later work.

`./oratio/quaere.sh forma…` prints what WORDS would: stem.ending, part,
declension, accidents, lemma, the five codes, the sense. Gate
`probatio_oratio_vocabularium_la` (94): folding, load, twenty-odd forms
against WORDS' behaviour (vir virum puellam rex regis amat amavit amatus
amare bonus melior optime amariter et in a virumque sine agantur),
ordering, unknowns as findings (xyzzy, Troiae — WORDS has Trojanus but
not Troia — the empty form, a forty-letter form, an em dash), lemmas,
and a CORPUS MEASURE: every vocabulum of the three Latin fixtures looked
up — Hilarius 93.4 %, Propertius 93.6 %, Cicero 95.8 %, 18,362 of
19,315 known (95.1 %), about 8 µs a word. The first unknowns printed
are proper names (Cynthia, Catilina, Milanion), an OCR slip
(perditissiis), a syncopated perfect (norit), and — the finding of the
day — `sit`, `erat`, `fuit`: the verb `sum` is NOT in this commit's
DICTLINE.GEN. Every V 5 1 line is a compound (absum, adsum, desum…), and
INFLECTS.LAT carries nineteen forms of esse only (essem, forem, es,
este, esto, esse, sunto); `est` resolves to `edo` alone. WORDS keeps a
"blank dictionary list" for exactly this entry (stem 2 of to_be is
blank — `'' + essem`); the loader indexes INTERNAL blank stems the same
way, ready for the entry. Recorded in FONTES.md and pinned in the gate
as a finding (erat and fuit unknown today), so the supplement — a
glossary entry in T9 or a SUPPLEMENTUM file with a named source — turns
the pin red and moves it. A first attempt that indexed every blank stem
made `a` match 716 entries (nouns' empty third and fourth stems);
trailing blanks are nothing, internal blanks are stems. Planted fault:
the 0 0 wildcard removed — `amavit` loses its PERF row. Deviations from
the spec, both deliberate: tackons are tried last (WORDS' order; the
merge bias), and prefixes/suffixes with the tackons that carry a base
part (est, cumque, pte, met…) are T8b, data-counted.

## 2026-09-04 — T9: the house glossary, first source of the lookup

`oratio/glossarium.stml` is hand-owned STML judged by
`oratio/grammatica/glossarium.canon` (registered under `<glossarium>` in
`canones.registrum`, so `bin/canon_examen oratio/glossarium.stml` works
from the shell): `<vocabulum lemma lingua classis nota>` with an
optional `<sensus>` and `<forma textus …>` children carrying the
UNIVERSAL accidents of the stage-three spec — casus, numerus, genus,
persona, tempus, modus, vox, forma-verbi, gradus — each an enumerated
attribute, so the seventeen UD classes in Latin and every accident
value are pinned by the canon from birth, before the analysis genera
of T11 exist. The canon is real: an unknown class, a missing lemma and
a fourth person are each refused with the line. The first entries are
`sum` (the whole finite paradigm, 77 forms, plus esse/fuisse/fore —
the T8 finding), `Vergilius` (a proper name with a V and a capital,
there to prove folding on BOTH sides of the hash), and sixteen
technical terms of class `ignotum-permissum` seeded from the symbol
table (offset, index, token, byte, bit, hash, cache, buffer, json,
utf8, html, stml, xml, url, api, ascii), each with the count of its
sites in `build/nexus.tsv` in its note; the list grows from T10's
report.

`oratio_glossarium.{h,c}` reads the file through the house STML
parser, keeps entries and forms in arrays, and hashes every form by the
SAME folding function the WORDS table uses, so one lookup serves both
sources; the lemma itself is a form unless a listed form already
spells it (offset with offsets; sum once). `oratio_vocabularium_la`
takes the glossary as an optional first source: its hits come before
uniques and stems, carry `genus = GLOSSARIUM` and the form index, and
count as found, so the enclitic phase is not tried (estque still
splits, because the whole word is not in the glossary). `./oratio/
quaere.sh` attaches the glossary when the file exists. Gate
`probatio_oratio_glossarium` (84): canon judgement of the live file
and three mutations, the loader with three malformed sources refused
by line, lookups with folding both ways (VERGILIVS, uergilius), the
integration order (est → sum first, edo second; erat, sit, fuit found;
offset permitted; amat untouched), and the corpus re-measure: the three
Latin fixtures go from 95.1 % to 96.3 % known (+245 words, Cicero
95.8 → 97.0), all of it the verb to be. The T8 pin stays as it was
(erat and fuit unknown in the table alone) with its comment pointing
here.

Two lessons. A field named `casus` is `case` after latina.h — the
struct field is `casus_grammaticus`. And the first planted fault was
invisible: dropping the fold on the index side changed nothing because
every glossary form was already lower-case ASCII without v or j; the
gate is only as real as its data, so the Vergilius entry exists to
make that fault visible, and it did.

## 2026-09-04 — T10: the vocabulary report over the house's own Latin

`oratio_vocabula.{h,c}` collects words from two corpora and looks each
up once. Identifiers come from the definition rows of `build/nexus.tsv`
(149,471 rows), split at underscores and at the lower-to-upper case
boundary (piscina_allocare → piscina, allocare; MateriaNodus → materia,
nodus; STMLNodus → stml, nodus), lower-cased, parts with fewer than two
letters dropped (s32, b32, x). Comments come from every tracked
`lib/*.c` and `silva/fontes/*.c` through silva's TOTAL lexer
(`silva_lexare_cruda`, comment tokens only), each comment body then
read by oratio's tree so its words are the tree's vocabula and the
`*` margins are punctuation; the line of each word is the comment's
line plus the newlines before it. Every distinct word is looked up
glossary first, WORDS second, and gets a status: notum (one lemma),
ambiguum (several lemmata — status: sisto/status; genus: genu/genus;
est: sum/edo), permissum (a glossary term of class ignotum-permissum),
ignotum (nothing — a finding). Sites are counted per source and the
first site kept. `./oratio/vocabula.sh [-symbola | -commenta | -omnia]
[-machina] [-omnes]` prints the summary, the unknown words by
top-level directory of first use, the unknown list by site count, and
the twenty most ambiguous; `silva.vocabula(fons)` returns the same as
records. Gate `probatio_oratio_vocabula` (69): the splitting rule on
an inline symbol table, comment lines on an inline C source, the
status rules with the real table and glossary, ordering, and the
corpus run with sanity floors (no pin yet — the "only falling" pin
comes after this report is read). Born red by removing the case
boundary. The runner now compiles silva's lexer and token files into
the oratio build and lists the C corpus as `oratio/build/corpus_c.txt`.

The report, day one. Identifiers: 10,060 distinct words over 192,239
sites — 3,109 known, 1,416 ambiguous, 20 permitted, 5,515 UNKNOWN
(55 %), in 144 ms. Comments: 14,205 words over 154,179 sites in 179
files — 6,174 known, 2,275 ambiguous, 5,736 unknown, in 467 ms.
Together 19,371 words, 346,418 sites, 49 % unknown, 559 ms. What the
unknown list is made of, from the top: abbreviations (tok 1,251, idx,
ctx, aed, sem, ptr, len, val, elem, neg, exp, cx, tt), English (max,
goto, main, out, count, result, total, cell, slot, part, best, tag,
after 2,104, expects, states, list), C keywords quoted inside the
generated grammar comments of the arbor2 and lapifex generation (int
1,722, long, const, unsigned, signed, short, sizeof, struct, char, enum,
typedef, void — one archived generation supplies most of them), vendor
(sqlite 508), and HOUSE COINAGES that WORDS does not know: transponere
(2,153 sites — real Latin, not in this dictionary), lexema and
lexemata (838 + 302: Greek loan, medieval), xar, lista (medieval),
arbor2, md. The per-directory line says where the English lives:
knotapel's demos and the archived generations, not lib/ and silva/.
Two consequences for the plan. The glossary grows from this list, not
from guesses — the house coinages first (lexema, lista, transponere,
xar as a name), then the abbreviations the house actually uses (tok,
idx, ctx, ptr, len) as permitted terms or as findings to rename. And
T8b stays parked: none of the top unknowns is a prefix or suffix case;
they are English, abbreviations, and quoted keywords, which addons
would not touch. The "only falling" pin should be set after the first
glossary pass, on the identifier corpus alone, which is the one the
house controls.

## 2026-09-04 — Glossary pass I from the T10 report, and the first pin

Seventy-seven entries added to `oratio/glossarium.stml`, all taken from
the top of the identifier report: house coinages and post-classical
Latin that WORDS lacks (lexema, lista, machinula, resultus, registrum,
parsura, parsator, octetus, sceletum, instructio, pixelum, coloratio,
specificator, importatio, ordinatio, syntaxis, bloccus, subscriptio,
indentatio, scopus, selector; the adjective imparilis; the verbs
parso, evaluo, processo, registro, transpono with their infinitives,
third persons and participles), fifteen house proper names (lapifex,
saltuarius, sputnik, arbor2, xar, silva, materia, oratio, natura,
gesta, villa, vitrea, atrium, tessera, officina) as nomen-proprium, and
thirty-six C, API and format names as permitted terms (goto, main, int,
char, long, union, enum, typedef, argc, argv, cstr, fd, eof, fcntl,
posix, http, tcp, uuid, toml, css, md, sha, sha1, sha256, stbi, macho,
sqlite, sqlite3, glr, xor, hex, utf16, crc32, fnv1a, djb2). Two kinds
were left unknown ON PURPOSE. Common English words in identifiers
(count, found, first, name, text, file, open, end, list, entry, result,
test, cell, slot, tag …) are exactly what the lint exists to name, so
permitting them would blind it. Abbreviations (tok, idx, ctx, ptr, aed,
sem, cx, tt, nt, pn …) are Fran's call — permit or rename — and stay
findings until then.

Identifier unknowns went from 5,515 to 5,409 over 10,062 words: the
glossary reaches the house's own Latin quickly, and the remainder is
English and abbreviation, most of it in knotapel's demos (2,425), the
headers (832), lib (509) and silva (464) by directory of first use. The
gate now PINS the identifier unknown count at 5,409, only falling: a
red means a new identifier introduced an unknown word, and the remedy
is a glossary entry (a house coinage or a permitted term) or a rename,
with the pin moved for a named cause. The comment corpus is not pinned:
its unknowns are dominated by the archived generation's generated
grammar comments and by English prose, neither of which the house
writes today.

## 2026-09-04 — The lint's corpus is the house

Fran's call on reading the report: knotapel stays English for now, so
it is out of the Latin lint. `ORATIO_VOCABULA_EXCLUSA` is the one place
that says so — `knotapel/`, `vendor/` (third-party) and `archivum/`
(the archived generations) — used by the instrument (`-omnes-viae`
puts them back), the gate, and `silva.vocabula(omnes_viae=)`. Over the
house alone: 7,469 identifier words, 3,022 unknown (40 %), top of the
list unchanged in kind — tok, ctx, intern, aed, op, sem, out, slot,
ptr, tag, idx, elem — abbreviations and English, which is what Fran
decides next. Pin moved 5,409 → 3,022 with this cause.

## 2026-09-04 — Abbreviations kept; glossary entries get a CONTEXT

Fran's two calls on the report: the abbreviations stay (tok, idx, ctx,
ptr, len, arg, attr, elem, decl, def, sym, tmp, buf, cfg, fn, err, val,
msg, op, aed, intern, expr, param, cnt, min, max, pos, dir, ref, mod,
info, opt, lit, seq, ms — 36 entries, class ignotum-permissum), and the
glossary should know WHERE a word is allowed. So `<vocabulum>` carries
`contextus="latinus | anglicus | ambo"` (absent = ambo, enumerated in
the canon): latinus = identifiers and comments, anglicus = English
prose (worklogs, markdown — the lint over that corpus is future work),
ambo = both. The loader exposes `latine`/`anglice`; `permissum(…,
latine)` takes the context; the identifier and comment lint is a Latin
context and IGNORES English-only entries entirely — neither permitted
nor known — so `worklog` (the one anglicus example) stays a finding in
an identifier while it will pass in prose. House identifier unknowns
3,022 → 2,987 of 7,469; pin moved with this cause. Whole English words
in identifiers (count, found, name, text, result …) remain findings by
design.

## 2026-09-04 — T15a: the English context — Moby vendored, the prose lint

Fran's warmup after decision 26: the English-context lint over worklogs
and markdown. An English lint needs an English word list, so stage 4's
dictionary came forward: Moby Part-of-Speech II (Gutenberg #3203,
public domain by grant, January 2001) was already in the Gutenberg
mirror. Measured before a line was written: 3,237,558 bytes, 233,356
records, CRLF throughout; the separator is a BACKSLASH in the Gutenberg
copy (the spec said `×`, which is the CD-ROM edition); codes present
A N h v P p t V i ! C D r — the legend's I (indefinite article) and o
(nominative) never occur; one record `cowardic\Ne` carries a code
outside the legend (a typo for N); 35,969 records are phrases with
spaces; 1,231 carry CP437 accent bytes (é = 0x8E, the legend's "ASCII
142"); 4,437 case-duplicate forms (AA/aa, OF/of, BE/Be/be). The legend
warns that -ed/-ing/-ly forms are often not listed — and it is right.

Four calls by Fran, all the recommended ones (decision 27): (1) a Latin
word in English prose gets its own status LATINUM — known and counted,
never a finding; (2) the prose corpus is the house INCLUDING knotapel
(it is English), minus vendor/, archivum/ and the generated files
(gesta/annales/tabula.md, md/CENSUS.md) — `ORATIO_PROSA_EXCLUSA`; (3)
Moby is vendored verbatim and loaded DIRECTLY — no coction, the source
is already one table; the gate pins bytes and the SHA seal of the
source (`cc81458b820a3625`); (4) morphology comes AFTER the report, as
data rows with counts.

Built. `oratio/vocabularium/en/` (mobypos.txt; LICENTIA.txt = the
Gutenberg documentation note verbatim, grant and legend; FONTES.md with
the record law and the findings). `oratio_vocabularium_en`: the loader
asserts the record law on every line — CRLF, one backslash, non-empty
form and codes — and stops with the line; a code letter outside the
legend is COUNTED, not a stop, the first named; words are hashed by
lower-cased form, phrases counted and not indexed; `quaerere` returns
records in file order (the chain is newest-first, reversed on the way
out); code letter → universal class. Gate
`probatio_oratio_vocabularium_en` (81: pins, samples, classes, eight
bad inline sources stop with the right line; load 27 ms); planted fault
= '!' removed from the legend → 434 unknown codes → red.
`oratio_vocabula_prosa`: md's tree, TEXTUS nodes only — code spans,
fences, link destinations, html blocks and front matter never reach the
lookup BY CONSTRUCTION, and the gate's inline md proves each absence;
each text node's byte extent goes through oratio's tree; line = the
first token's. `oratio_vocabula_creare_anglice` + `_iudicare_anglice`:
glossary entries allowed in English first (lingua latina → LATINUM,
permissum → PERMISSUM, else NOTUM), Moby second (class of the first
code, lemma = the form itself), Latin table third → LATINUM; ambiguum
never (Moby carries no lemmata). `OratioSedesGenus` (symbolum /
commentum / prosa) replaces the b32 on `verbum_addere` (kept as a
wrapper); `sedes_prosae`, `ex_prosa_prima`. Instrument
`./oratio/vocabula.sh -prosa` (machina output gains a `prosa` column
and a `latina` count), `silva.vocabula('prosa')`. Second planted fault
on the vocabula gate: the glossary context inverted (`e->anglice`) →
tok permitted, worklog unknown → red.

Two things the corpus taught while building. A lone arrow `→` (E2 86
92) was counted as a three-letter word because every byte ≥ 0x80 was a
letter — the word filter now counts UTF-8 LEAD bytes (≥ 0xC0) as
letters, so a lone symbol is not a word (6,885 sites of `→` gone; `s²`
and `ζ₈` stay, two characters each). And `oratio_vocabula_ordinata` was
quadratic — one scan of all words per site count, invisible at 200
sites, 20 s at 100k sites × 62k words — now a counting sort (the gate
went from 21 s to 3 s).

The report, day one (English prose, house + knotapel): 1,139 files,
62,524 distinct words, 1,894,264 sites, 1.6 s. notum 15,098, permissum
57, latinum 11,254, IGNOTUM 36,115 (58 %). What the unknown list is
made of, from the top: REGULAR INFLECTIONS Moby does not list —
plural/third-person -s (values 2,076, functions, tests, cells,
solutions, types, weights, results, returns … gets, sets), -ed (added,
needed, tested, shared, recorded, expected), -ing (existing, matching,
tracking, nesting, testing, interning), possessive 's (pliny's,
casey's, fran's), -ally (computationally); hyphenated compounds whose
parts are known (a-plot, non-semisimple, non-null); house English terms
(dkc 2,842, vs 2,461, xor6, methos, voronoi, npn, sqrt, mcp, mvn,
fibonacci, braids); Unicode math (s², ζ₈, ζ₁₂); `struct` (691 — the C
keyword in prose; int, char, long … were permitted in pass I, struct
was not). By directory of first use: project-specs 9,130, knotapel
6,906, silva 3,455, episodes 3,337, lib 2,512. Consequences: the
morphology rules the spec names are exactly the top of this list and
land next as DATA rows with these counts as their cause (-s/-es, -ed,
-ing, 's, -ly); a glossary pass II with `contextus="anglicus"` entries
follows (vs, dkc, xor6, methos, voronoi, npn, sqrt, mcp, mvn, struct
…); the gate's corpus floor is a third known (42 % measured), to rise
with the morphology; no prose pin yet.

Identifier-pin note. The new code introduced `recordum` (the house
coinage for "record", used since T7 as `recorda`) — WORDS knows
`recorda` only as a form of the verb `recordor`, so
`oratio_vocabularium_en_recordum` turned the identifier pin red (2,988
> 2,987): exactly what the pin is for. Glossary entry added (neuter
noun, forms listed); unknowns 2,986, pin moved down with this cause.
Note for the next reader of that pin: the machine output gained a
column, so a query by first site must read field 11, not 10 — half an
hour went to a wrong column.

## 2026-09-04 — T15b: English morphology as data, glossary pass II, the prose pin

Straight after T15a, with the day-one report in hand. The suffix rules
the spec names are a DATA table now, `ORATIO_REGULAE_EN` in
`oratio_vocabularium_en.c`: one row per suffix with its base
substitution (ed→"", ed→e, ies→y …), a gemination flag (planned →
plan), a minimum base length, the Moby codes the base must carry (a
plural needs a noun or verb base; a comparative an adjective or adverb
— `near` is `PvVN`, so `nearest` needs the `v`), the analysis class
(or "from the base" for plurals), and a cause that cites the report's
words and now the measured tally. Twenty-five rows: three plural, four
past, three participle, possessive, six contractions, three adverb,
two comparative, two superlative, and `compositum`. A new function
`oratio_vocabularium_en_analysare` returns analyses in order — exact
records first, always, then the rules in table order — each carrying
its rule index, its base, and its class; the judge takes the first
analysis for class and lemma (so `values` gets lemma `value` and
`regula` `pluralis-s`), counts distinct bases as `lemmata`, and
`OratioVerbum.regula` names the rule; the machine output gained a last
column and the instrument a per-rule tally.

Three things learned from the corpus while the rules went in. (1)
Moby lists more than the legend admits: `entries` is there as `p`,
`tried` and `planned` as `V`, `making` as `N`, `simply` and `happily`
as `v`, `larger` and `largest` as `A`, `I'm` and `don't` as entries —
so the gate asserts "exact first, rule present after", not "rule
first", and two contraction rows explain nothing in this corpus; they
stay as rows with a zero. (2) Compounds are the biggest single rule:
`compositum` (every hyphen part a known word, or a prefix from
`ORATIO_PRAEFIXA_EN` — non, multi, semi, pseudo … — with at least one
real word among the parts, and parts themselves may go through the
suffix rules, so `consists-in` and `discovered-while` resolve) explains
13,508 distinct words and 45,304 sites; plurals 3,327 / 135,056;
possessives 1,129 / 8,957; `-ed` 644 / 16,372; `-ing` 570 / 12,097.
(3) The typographic apostrophe: `i’m` and `dkc’s` carried U+2019, so
the English fold maps E2 80 99 to `'` before anything else.

The pass II glossary followed the post-rule report: 117 entries under
an ANGLICA PROSA section — proper names (Voronoi, Fibonacci, Hopf,
Galois, Seifert, Temperley-Lieb, Methos, Teal'c …) with possessive
forms where the corpus has them; the computing English Moby's 1990s
list lacks (whitespace, roundtrip, lookup, lexer, runtime, subtree,
codebase, newline, fallback, database, callback, tuple, typedef,
stdout, metadata, timestamp, workflow, monorepo …) as real classes with
explicit plurals, because the rules reduce to MOBY bases only, not to
glossary lemmata; abbreviations (dkc, vs, xor with its numbered forms,
npn, sqrt, mcp, mvn, e.g, i.e, tsv, utf …) as permitted; `struct` and
`sizeof` joining the C names for both contexts; three house Latin
coinages the identifier lint had also been missing (internamentum,
diagnostica, friatio; amalgama as medieval Latin); and the Roman
numerals ii–xxx as `numerale`, both contexts, since the house writes
them in prose and in comments (`iii` alone had 262 prose sites). The
plural rule's minimum base went from three letters to two for `ids`.

Numbers. Prose unknowns 36,115 → 18,949 after the rules (58 % → 30 %)
→ 16,416 after pass II (26.2 %); known share 73.8 %. The identifier pin
fell 2,986 → 2,950 on the `ambo` entries and moved with that cause.
The gate: `probatio_oratio_vocabularium_en` section VI (160; every row
on a listed base, prefixes, contractions, the fold, limits; planted
fault = the plural rule's base codes replaced by `A` → red at
`values`); `probatio_oratio_vocabula` (165) now judges `values` and
`tested` through the rules. The prose pin: not a count "only falling"
like the identifiers' — prose gains new words every day and a count
pin would go red on the next worklog — but a CEILING on the unknown
SHARE (29 %, three points above measured) beside a floor on the known
share (70 %); a red there means a rule or a glossary section broke, not
that one new word appeared. That is a deviation from what I said
before building; the reason is above.

What is left at the top of the unknown list is now genuinely
interesting: Unicode math from knotapel (s², ζ₈, ζ₁₂), FILE NAMES
written in prose without backticks (silva.h 298, latina.h 234,
claude.md, stml.c — a style finding the lint was built for), knotapel's
own codes (m0b, m2c, xor7, xnor3, rkhs, gptq, bitnet), hyphen compounds
with one unknown part (nazer-gastpar, pseudo-anosov,
delta-parameterized), `won't` (base `wo`), and a thin tail of English
Moby lacks (uniqueness, bitwise, whiteboard, resize, lifecycle). A
compositum that consults the glossary for parts would take the
compounds; a numeral-like filter would take the math; the file names
should stay findings.

## 2026-09-04 — T11: partes_registrum — the seventeen analysis genera

Stage 3 opened with its smallest task: the vocabulary of grammar into
the registry, nothing annotated. Seventeen genera appended after
`numerus`, one per universal class in UD order, `analysis-substantivi`
… `analysis-ignoti`, with the invariant genus = PRIMUM + classis and an
`OratioClassis` enum whose titles are exactly the `classis` options
the glossary canon already had (the registry gate reads that canon and
proves each title is an option — one vocabulary, two homes). Every
analysis genus has the same five slots first: `lemma` (derived token),
`lingua` and `fons` (INDEX), `nativum` (derived token — ONE token
holding the source's own code verbatim, Fran's call; WORDS gives a
line, Moby a code string plus a rule name, one string covers both),
`sensus` (derived, optional). Then the accidents as INDEX slots into
small enumerations that are appended and never permuted, with title
arrays for each: nouns and proper nouns carry case, number, gender,
declension; verbs and auxiliaries person, number, tense, mood, voice,
verb form, conjugation, plus case and gender because participles
agree; adjectives case, number, gender, degree, declension; adverbs
degree; pronouns case, number, gender, person; determiners case,
number, gender; adpositions the governed case; numerals case, number,
gender and a kind (cardinal, ordinal, distributive, adverbial);
conjunctions, particles, interjections, symbols, punctuation and the
unknown class carry the common five only. Case has locative and
vocative, number has dual, voice has deponent, verb form has gerund,
gerundive and supine, tense has the English simple past beside the
Latin six — the lists are permanent from here. 146 slots in all.

The word's own slots (`analyses`, `classes`, `linguae`) had been
reserved since T1, so the STML of every existing parse is byte-for-byte
what it was: an absent list is not written, and the seal attribute
changed value but not length. The computus golden and the STML
round-trip gate did not move; only the registry seal (93c1c9cf →
87b35173) and the canon did. The canon gained the seventeen genus
rules, the seventeen liberi under `analyses` and under `arbor`, and
129 slot rules, one per slot INSIDE its genus, because the drift guard
demands exactly one rule per slot intra its genus and a global
fallback would not satisfy it. Those rules were generated from the
table by a script once and are kept by hand from now on, like the rest
of the file; the rule ceiling went 256 → 512 (182 now).

Gates. `probatio_oratio_registrum` (860): contiguity over 23 genera,
the class helpers round-tripped for all seventeen, the five common
slots by title and species on every analysis genus, every accident an
INDEX, the per-class slot counts of the decision above, auxiliaries
laid out exactly as verbs, the enumeration ends by title, the glossary
canon drift, and the materia round trip of a word carrying an
`analysis-substantivi` with lemma, lingua and case — green first run.
`probatio_oratio_canon` (219) now also judges a hand-built analysis
document (0 vitia) and two mutations of it: `gradus` under a noun
analysis and `casus` under an adverb analysis, each refused with a
named vitium. Planted fault: the verb genus's slot count 14 → 13 →
red at contiguity, green on revert.

One naming slip worth remembering: the slot-count enumerator
`ORATIO_ANALYSIS_SUBSTANTIVI_NUMERUS` collided with the `numerus`
accident's enumerator of the same name — counts are
`_NUMERUS_LOCORUM` now. Next is T12: the WORDS mapping table and the
annotation pass that fills what T11 reserved.

## 2026-09-04 — T12: partes_la — the first parsed word that carries analyses

The mapping and the annotation pass, and with them the second substrate
change materia has ever needed.

`oratio_partes_la` turns one source analysis into a universal
DESCRIPTION: class, twelve accidents as enumeration indices (−1 = not
given: WORDS' `X`, person `0`), lemma, `nativum` (the source's own
code verbatim: `N 1 1 ACC S C`, `V 3 1 PRES PASSIVE SUB 3 P IMPERS …`
for a unique form, `TACKON que`, `glossarium verbum`), sense, source,
language. The table is the spec's, with the corpus deciding the
details: a noun is `nomen-proprium` when WORDS' kind is N (name) or L
(locale), so Roma and Caesar are proper and vir is not; a noun's gender
comes from the STEM when it is M/F/N and from the inflection when the
stem says common — so `puellam` is feminine although its ending record
says `C`; a verb whose stem kind is DEP or SEMIDEP is voice `deponens`
regardless of the passive ending; an infinitive ending sets both mood
`infinitivus` and form `infinitivum`; a participle is form
`participium` unless it is FUT PASSIVE, which is the gerundive; supine
is its own form; a preposition's governed case is read from the ending
record and, failing that, from the stem; conjunctions are coordinating
unless the lemma is in `ORATIO_CONIUNCTIONES_SUBORDINANTES` (ut, cum,
si, ne, quod, quia …, a data list); the enclitic analysis WORDS adds
for `-que`/`-ve` is a coordinating conjunction, `-ne` a particle; a
glossary form maps its class name straight to `OratioClassis` and its
accident strings through the enumeration title arrays, and an
`ignotum-permissum` entry lands in class `ignotum`.

Which is why the enumeration titles had to be reconciled first: T11
had spelled the genitive `genetivus` (WORDS' classical spelling) with
vocative before locative and persons `prima/secunda/tertia`, while the
glossary canon written in T9 says `genitivus`, locative before
vocative, and `I II III`. One vocabulary, two homes, and the registry
gate now proves the accident option lists of the glossary canon equal
the title arrays IN ORDER, the way it already proved the class names.
That reconciliation put `genitivus` into an identifier, and the
identifier pin caught it at once, because WORDS only knows
`genetivus`: a medieval-spelling glossary entry now, beside
`glossarium` (Greek loan, WORDS lacks it; its genitive had just entered
the tree instrument) and `subordinans`.

`oratio_partes` is the pass: after the tree exists, every `vocabulum`
is looked up once (Latin table with the glossary first, as T8/T9 left
it), each analysis described, each description built as a node of the
class's genus — genus = PRIMUM + classis — with the five common slots
as derived tokens and INDEX values, and the accidents placed BY SLOT
TITLE through the registry (`oratio_partes_locus` looks the accident's
name up in the genus's slot list, so the C never carries a hand table
of slot indices and a genus that lacks an accident silently omits
it). Then `classes` and `linguae`: distinct class titles in order of
the analyses, space-separated; a word with nothing gets no analyses,
`classes` = `ignotum` and no `linguae`. Write-once is respected: a
word whose `classes` slot is filled is skipped, so a second pass
annotates nothing. And `materia_arbor_patres_figere` runs at the end
— the round trip went red on `pater-nullitas` without it, the same
lesson every materia parser has learned once.

The substrate change. Selection wants `vocabulum[classes~=verbum]`,
and selectio matches attributes, not child text. The frons hooks
materia offers decorate TOKEN elements only, so `MateriaArborFrons`
gained a ninth, last field, `nodum_ornare`, called by the writer right
after a node's element is created, and the reader keeps ignoring
attributes on node elements — the slots stay the truth, the attribute
is a mirror. oratio's consilium now carries a frons whose only hook
copies the `classes` and `linguae` token values onto the `<vocabulum>`
element; the canon declares the two attributes; the round trip is
unaffected because the reader never looks at them. Existing parses do
not change: no annotation, no slot, no attribute.

Then selectio lied. `vocabulum[classes~=substantivum]` matched all four
words of the fixture. The word-match operator computed `len - val_len`
in unsigned arithmetic and, when the attribute was SHORTER than the
value (`verbum` against `substantivum`), the loop bound underflowed
and the scan walked off the attribute into neighbouring memory, where
it found the word. A guard (`val_len == 0 || val_len > len` → no
match) fixed it; the root selectio tests were green before and after
because none of them tried a value longer than the attribute. Found by
the first real consumer of `~=`, which is what consumers are for.

The gate (`probatio_oratio_partes`, 171): the code tables; twenty
forms against the decision above (puellam, Roma, Caesar, vir, amat,
amavit, amatus, amandi, amare, loquitur, bonus, melior, optime, in ×2,
et, ut, tres, ego, hic, virumque, agantur, est, offset, xyzzy); the
annotated fixture "Puella amat. Xyzzy virumque." node by node — three
analyses for Puella with lemma, nativum, sense, language, source, case
NOM, gender F, declension I, the derived token positioned at its
origin; amat a verb; Xyzzy `ignotum` with no `linguae`; virumque
`coniunctio-coordinans substantivum` with nine analyses; a second pass
a no-op — the projection (`<vocabulum classes="…" linguae="…">`, the
analysis elements, `<casus(> 0` because INDEX projects as text), the
full round trip STRUCTURAL and FIDELIS, the canon judging the annotated
document at zero vitia, selectio counting one verb, two nouns, three
Latin words, one unknown, one coordinating conjunction, no adjective;
and the corpus. Planted fault: the stem-gender rule dropped → puellam
loses its F → red.

Day one over the Latin fixtures:

| text | words | analyses | unknown | ms |
|---|---|---|---|---|
| Hilarius | 1,678 | 10,352 | 5.2 % | 81 |
| Propertius | 4,423 | 27,819 | 4.6 % | 176 |
| Cicero | 13,214 | 114,876 | 2.9 % | 858 |

Six to nine analyses per word, and the class census says where they
come from: Cicero's 114,876 analyses hold 72,965 PRONOUN analyses,
because WORDS lists every `qu-`/`-cumque`/`-dam`/`-libet` packing as
its own entry and `quis` alone brings 215. That is faithful to the
source and useless as a candidate list; stage 5's ordering, or a
dedup by (class, accidents, lemma) before it, is where it gets fixed,
and the oracle in T13 will say whether the noise costs coverage. The
instrument: `./oratio/arbor.sh <x.txt> -partes` annotates before
projecting (`-tacitus` prints the census). Identifier pin 2,950 →
2,958: no unknown word is new relative to HEAD's sources (word-set
difference over the diff and the new files, and the three coinages
got entries), the regenerated symbol table simply indexes seventy more
identifier words of the existing tree — the cause is coverage, and it
is named here.

## 2026-09-04 — T13: the oracle — treebanks grade the parser, and the mapping learns from them

Two Latin treebanks from Universal Dependencies are vendored under
`oratio/probationes/fixa/ud/`, both CC BY-SA 4.0 with their licence
and README verbatim: CIRCSE (Seneca, Hercules Furens, 893 sentences,
11,503 tokens, no punctuation, `u` for `v`, 316 enclitic ranges) and
the LLCT charter treebank's dev and test splits (Fran's call — train
is a quarter million tokens and UD scores on dev/test anyway; 850 and
884 sentences, 24k tokens each, medieval spelling, punctuation). The
non-commercial ones (ITTB, PROIEL, Perseus, UDante) are never
vendored: `./oratio/oraculum.sh -petere` fetches their test files into
the build directory and reports. FONTES.md carries upstream commits,
byte counts and seals.

`oratio_conllu` reads the format: ten tab-separated fields or the
reader stops with the line, `# sent_id` and `# text` kept, ranges
`a-b` marked and their words following, empty nodes `a.b` skipped, a
blank line closes a sentence, CRLF tolerated, `SpaceAfter=No` read
from MISC, and the sentence text taken from `# text` or reconstructed
from the forms when absent (ranges give the surface, their words are
skipped). Chords reference the file, nothing is copied.

`oratio_oraculum` judges a sentence: its text is parsed and annotated
exactly as a document would be, the elements (word, punctuation,
number) get byte extents from their tokens, each gold token's form is
located in the same text in order, and the element containing the
gold start — or every element the gold extent touches — lends its
class set. A range is one surface token judged for each of its words,
so `pronumque` meets one element whose classes are the enclitic's and
the adjective's. Three measures per gold word: TECTUM, the gold class
among ours, which is coverage and gets pinned only rising in permille
per file; PRIMARIUM, our first class equals gold, reported; LEMMA, the
gold lemma folded among our folded lemmata, reported; plus our
`ignotum` and unaligned counts, and the first five uncovered examples
per class, which the instrument prints with `-exempla`. Alignment was
perfect from the first run: zero unaligned tokens across 60k, zero
broken sentences.

Day one, before touching the mapping:

| treebank | coverage | primary | lemma | ours unknown |
|---|---|---|---|---|
| CIRCSE | 84.2 % | 66.0 % | 87.7 % | 4.1 % |
| LLCT dev | 71.9 % | 61.7 % | 64.0 % | 13.4 % |
| LLCT test | 72.5 % | 62.3 % | 63.7 % | 13.2 % |

The per-class table said exactly what T12 predicted and more. Three
classes at ZERO because WORDS does not carve them: `auxiliare` (every
one of them lemma `sum`, lemma agreement 100 %), `determinans` (hic,
ille, meus, noster, omnis, solus, uterque, suprascriptus — WORDS says
pronoun or adjective), `particula` (non, haud — WORDS says adverb).
Subordinating conjunctions at 74–84 % because `cum`, `unde`, `qualiter`,
`quam` reach us as adverb or preposition and the CONJ rule never
fired. Proper nouns at 13–23 %: the names of Seneca's myth and of the
charters' witnesses are simply not in WORDS, so they were `ignotum`.
Ordinals (tertiae, octavo) are ADJ in UD and numerale in WORDS. And
WORDS marks `deus`, `chaos`, `Manes` as kind N (name), which T12 had
turned into proper nouns while UD calls them nouns.

So the mapping pass II, every rule a data list with the oracle's
counts as its cause (`oratio_partes_la.h` records them): SECONDARY
descriptions appended after the primary one, never replacing it —
`sum` gets an `auxiliare` reading beside the verb; pronouns,
adjectives and numerals whose lemma is in `ORATIO_DETERMINANTIA` get a
`determinans` reading; adverbs in `ORATIO_PARTICULAE` a `particula`
reading; adverbs and prepositions in the subordinating list a
`coniunctio-subordinans` reading (and the list grew: qualiter, unde,
quam, quatenus, quomodo …); ordinals an `adiectivum` reading; kind N
names a `substantivum` reading — kind L places like Roma stay proper
only, which is where the CIRCSE pin lost one permille and was moved
with that cause. And the one rule without a dictionary: a word nothing
knows whose first letter is a capital becomes a `nomen-proprium`
candidate with source `regula` (a fourth value appended to the source
enumeration) and nativum `capitalis`. The primary reading is
unchanged everywhere, so PRIMARY moved only where a class went from
nothing to something.

After the pass:

| treebank | coverage | primary | lemma | ours unknown |
|---|---|---|---|---|
| CIRCSE | 93.7 % | 67.8 % | 88.7 % | 0.8 % |
| LLCT dev | 88.9 % | 68.1 % | 65.0 % | 7.0 % |
| LLCT test | 88.2 % | 68.2 % | 64.6 % | 7.1 % |

Per class: auxiliare 0 → 99–100, determinans 0 → 78–84, particula 7 →
65–87, nomen-proprium 13–23 → 79–90, subordinating 74–84 → 94–100,
adjective on the charters 61–65 → 74–75. The pins are 937, 889, 882
permille, only rising. What is left is honest: the charters' medieval
spellings (`ecclesie` for ecclesiae, `episcupatui`), participles UD
tags as adjectives (furens, tonantem), and PRIMARY at two thirds,
which is the unordered list and stage 5's job.

Gate `probatio_oratio_oraculum` (104): the reader on an inline
two-sentence source with every feature and three refused sources with
their line; the judge on `Puella rosam amat.` (4/4 covered, primary 3
because WORDS lists rosam as a participle first, lemma 3 because
punctuation has none), on a range, an unknown, an unaligned token and
an out-of-table UPOS; and the three treebanks with sentence counts
pinned, the table printed, coverage pinned, unaligned under 2 %,
broken sentences zero. Planted fault: PROPN removed from the UPOS
table → red. The partes gate grew to 203 with the secondary rules and
the capital rule; its annotated fixture had to keep its capital
`Xyzzy`, because a lowercase unknown after a period does not start a
sentence — which briefly made the gate dereference a sentence that was
not there.

## 2026-09-05 — T14: verba.sh and the Python face of stage 3

The instruments and the query surface, thin by design: everything they
need existed in C already.

`oratio/instrumenta/verba.c` + `./oratio/verba.sh <x.txt>... [-machina]
[-analyses]`: each file is parsed and annotated exactly as `arbor.sh
-partes` does (la.bin + glossary from RHUBARB_RADIX; the wrapper now
exports it as the repo root when unset, so the instrument works from
any directory), then one row per word in document order: via, index,
byte extent, line, paragraph and sentence ordinals, form, classes,
linguae, the first analysis's lemma, the count of analyses.
`-analyses` prints one row per analysis node instead: class, lemma,
lingua, fons, nativum, sensus, then the twelve accidents as the
registry's option titles (declinatio and coniugatio as numbers, absent
ones empty). The accidents are read from the tree by SLOT TITLE through
`oratio_partes_locus`, the same way the annotator wrote them, so the
instrument reports what the tree holds and never re-derives. Several
files are accepted so that the via column means something (a corpus
query across the five fixtures is one invocation). Sentence ordinals
skip element-less sentences exactly as `sententiae.sh` does, so the two
instruments' ordinals agree.

`silva.Oratio(via | text | bytes)`: `sententiae()`, `vocabula(classis,
lingua, ignota, sententia)`, `ignota()`, `analyses(vocabulum | index)`;
a text is written to build/pythonica for the duration of one call; each
query's rows are cached. `classis=` matches ANY candidate class, not the
first — `vocabula(classis='verbum')` returns rosam (participle of rodo)
as well as amat — because ambiguity is a list at every layer and the
primary is stage 5's business. `Prosa.sententia(n, intra=)` delegates:
the markdown paragraphs inside the extent are concatenated with blank
lines, parsed once, and the sentence extents mapped back to file bytes
with line and column; the anchor counter got a `sententia` branch (its
fallback was the markdown selector, which has no such genus).

Measured: Cicero 13,214 word rows in 1.0 s, 131,161 analysis rows in
1.24 s (T12 counted 114,876 analyses before the pass II secondaries);
Hilarius 1,678 words, as T12.

Findings while writing the tests:
- Two short punctuated, capitalised lines are VERSE to the form layer,
  so the sentences follow the lines: the first fixture had two sentences
  where I expected three. Correct (T6b's rule); a fixture lesson: prose
  fixtures are one line, or blank-line separated.
- `Oratio('')` resolved to the repo root: `os.path.exists` accepts a
  directory. `isfile` plus a non-empty check.
- Markdown paragraphs of list items start at the marker, so the item's
  first sentence begins with `-`; and `- item one. item two.` is ONE
  sentence because a lowercase word after a period does not start one.
- `virum` gets lemma `virus` first: WORDS' order. Stage 5.

Gate: the pythonica suite (20 assertions: three sentences of a one-line
prose text, byte extents equal the forms, classes/lemma/linguae/
ordinals, class and sentence filters, the unknown, the capital rule,
accidents by title with absences omitted, Hilarius by path, a missing
path refused, bytes and empty text; Prosa.sententia inside a section
with line and column, list item, code fence and headings never, out of
range, stale extent, edit + applicare with the anchor counted). Planted
fault: the word end shortened by one byte in verba.c → red on "extenta
octetim == forma".

One hardening from the plant. The wrapper scripts exit 1 both when the
instrument finds nothing and when clang fails (`clang … || exit 1`), so
a Python face that reads "rc 1 = empty list" turns a broken build into
"no words" — the lying-green shape. `Oratio._machina` now demands the
`#` header line in `-machina` mode as proof the instrument ran; rc 1
without it is a fracture with stderr attached.

And a lesson about the ritual itself. The first `silva.planta` run
ended with "signum absens": the gate emitted no verdict at all. I was
writing this worklog, the orientation file and the spec while it ran,
and the pythonica suite lints the house prose (`vocabula -prosa` reads
every markdown file). Run alone, the same plant was red and the
reversion green, 283 s. A gate that reads the tree is not to be run
while the tree is being written — the same law as the shadow gates'
snapshot, applied to the live one.

The commit was then refused by the identifier pin: 2,973 unknown words
against 2,958. Attribution through nexus.tsv (which files hold each
unknown word's symbols): NONE is unique to verba.c — every unknown
word it uses already sits in older files — and nine are unique to
T13's files: circse, conllu, llct, misc, permille, rangae, rangam,
treebank, upos. The symbol table is renovated by the excubitor AFTER
a successful gate, so T13's own gate ran on the pre-T13 table and
never saw its words; T14 is the first gate to see them. Glossary
section for the house terms (ranga as a first-declension coinage,
permille as an adverb, conllu/upos/misc permitted, CIRCSE/LLCT proper
names); `treebank` renamed `thesaurus arborum` in the oraculum gate
(the English word in an identifier is a finding by design). Pin 2,958
→ 2,964 with the cause in the gate; the residual six are words of old
files newly indexed, the same coverage growth T12 saw. Lesson for the
lint: a pin over a build artifact that is renovated post-success
lags one commit; the next task pays for the last one's words.

## 2026-09-05 — T16: the English mapping, both dictionaries, and the EWT oracle

Stage 4 closes. The annotator now consults BOTH dictionaries for every
word — `OratioVocabularia {la, en}`, one struct, one loader
(`oratio_vocabularia_onerare` reads la.bin, the glossary and Moby from
the repo root; the three instruments and two gates dropped their
copies of that loading code) — and appends the English readings after
the Latin ones. `oratio_partes_en` maps a Moby analysis to
descriptions: an exact record yields one description per class among
its code letters in Moby's preference order (`p` adds numerus
pluralis, letters of one class coalesce, `nativum` = the code letters
verbatim); a rule analysis takes its accidents from the rule's title
(pluralis → one description per class of the BASE, so `cats` is a
plural noun AND a third-person verb; praeteritum → a finite past and a
participle; participium; possessivum → genitive of the base's class,
plus particula and auxiliare for the clitic; contractions →
base + particula for `n't` or + auxiliare for `'re 'm 'll 've 'd`;
adverbium; comparativus/superlativus; compositum takes the last
part's class). Lemma = the folded base, source `vocabularium-en`,
language `anglica`, no sense (Moby has none). `nativum` reads
"NV pluralis-s": the base's codes and the rule.

EWT dev and test (2,001 / 2,077 sentences, 25k tokens each, CC BY-SA,
commit 4a4d77f5) vendored beside CIRCSE and LLCT; the oracle takes
the pair of dictionaries; the instrument's default list now iterates
the array instead of a literal four (the fourth entry was the bug that
hid EWT from the default run). Day one, before any English pass II:

| treebank | coverage | primary | lemma |
|---|---|---|---|
| EWT dev | 77.1 % | 56.6 % | 75.2 % |
| EWT test | 77.0 % | 56.9 % | 74.7 % |

The per-class table had the same shape as Latin's day one: auxiliare
0 (has, been, is: Moby says verb), coniunctio-subordinans 0 (that,
because, since: Moby says C = coordinating), particula 0 (to, not,
n't, 's), nomen-proprium 22.6 % (President, Bush, Washington: Moby
lists them as nouns, so the capital rule never fired), symbolum 0
($, /, :-)), interiectio 16.5 %, numerale 79.6 % (one, two).

And CIRCSE had DROPPED, 93.7 → 93.5 %: Moby knows Hercules, Juno,
Thebes and Cerberus as English nouns, so those names no longer
counted as "unknown" and the T13 capital rule stopped making them
proper-noun candidates. The rule is now: a capitalised word that NO
LATIN SOURCE knows (unknown, or known to Moby alone) and that is a
noun or nothing gets the proper-noun candidate, with lingua anglica
when its other readings are English. That recovers Seneca's names and
covers the treebank's presidents.

Pass II for English, data lists in `oratio_partes_en.h` with the
day-one counts as cause, applied once per word after the Moby
readings, source `regula`, nativum = the list's name: AUXILIARIA
(be … ought), SUBORDINANTES (that … like), PARTICULAE (to not),
NUMERALIA (zero … trillion), INTERIECTIONES (yes no please …); the
rule-based ones above for `'s` and `n't`; and in the oracle a sign
element carries BOTH interpunctio and symbolum (UD's PUNCT/SYM split
runs through the same characters: `-` is either). After:

| treebank | coverage | primary | lemma | ours unknown |
|---|---|---|---|---|
| EWT dev | 91.3 % | 56.6 % | 75.2 % | 1.3 % |
| EWT test | 91.8 % | 56.9 % | 74.7 % | 1.3 % |
| CIRCSE | 94.0 % | 67.3 % | 89.2 % | 0.7 % |
| LLCT dev | 89.5 % | 67.7 % | 66.7 % | 6.7 % |
| LLCT test | 88.7 % | 67.6 % | 66.2 % | 6.9 % |

Per class on EWT dev: auxiliare 0 → 90.7, subordinans 0 → 75.8,
particula 0 → 89.6, nomen-proprium 22.6 → 77.5, symbolum 0 → 93.8,
interiectio 16.5 → 71.3, numerale 79.6 → 98.4. The Latin pins rose
too (940 / 895 / 887 permille): Latin words that are also English
(in, a, is) gained classes, and the wider capital rule gave the
charters' names back. What is left on EWT: proper nouns that Moby
knows as common nouns AND that are not capitalised in the text
(usernames, product names), adjectives that are participles, the
primary rate at 57 % — Latin readings come first, so `in` is an
ablative preposition before it is an English one; stage 5's ordering
by document language is the lever.

Measured on the fixtures: Lincoln 3,699 words, 0 unknown, 11,144
analyses (2,302 Latin, 8,842 English); Cicero 13,214 words, unknowns
383 → 178 (names Moby knows), 141,446 analyses (10,303 English).
Nothing slowed noticeably: EWT dev judges in 250 ms.

Gates: partes 203 → 275 (section VI: the, cats, planned, quickly,
higher, Fran's, it's, don't, we're, the five lists, Moby-unknown;
section VII: an English sentence annotated with the census by
language, `Bush` ending in a proper-noun node with lingua anglica and
source regula, `The` without one, Latin-only annotation with `en`
NIHIL, the loader refusing a missing root and naming la.bin); oraculum
104 → 115 with five pins; planted fault = plural noun made singular in
`_pluralis` → red on `cats`. Python: `silva.Oratio` gained no code —
the English readings simply appear — and three assertions.

Lessons. A declaration after a statement is still the first thing I
write when adding a local mid-block; examen catches it before clang
does. The oracle instrument's default file list was bounded by a
literal, and the symptom (a run that silently judged three of five
files) looked exactly like success. A dictionary that knows names as
common nouns changes what "unknown" means: every rule keyed on
"nothing known" must be re-read when a source is added.

## 2026-09-05 — T17: the walking skeleton of stage 5 — resolution as pattern arms

Eight decisions from the interview (spec §7, 28–35), then one rule
end to end. The shape: the annotated SENTENCE is written to STML with
ORDINALS (`n=` on the sentence, every element and every analysis,
stamped by the projection hook from the node's index in its parent's
list; `lingua=` from the annotator's census) and concatenated with
the program `oratio/partes/resolutio.stml`; one `stml_legere`, one
`stml_expandere`; each `<regula>` element's EXEMPLAR fills a PER that
instantiates `<praelatio vocabulum= analysis= regula=/>` rows inside
`<consilium>`; the C executor (`oratio_resolutio`) reads the rows in
program order, first rule wins per word, and permutes the word's
analyses list — preferred analysis first, the rest in their order —
through a new materia verb. The plan is a judged document: the
oratio canon declares `regula`/`consilium`/`praelatio`, and the
resolutio gate judges the expanded sentence-plus-plan with 0 vitia.

Two materia verbs (substrate change #3): `materia_nodus_lista_permutare`
(the new order must be a permutation — each index exactly once — else
refused loudly; new repository, old prospectus intact, so forks are
not harmed) and `materia_nodus_reponere` (replace a slot already
written, species-checked; the mirror of `ponere`'s once-only law).
The write-once law still guards construction; mutation enters only
through decreed verbs with checkable contracts, which is the
mutatio-visio's thesis in its first consumer. The `classes`/`linguae`
summaries are rebuilt after a permutation
(`oratio_partes_compendia_reponere`), because the oracle's PRIMARY
reads the first class of that summary — without the rebuild a
permutation would have been invisible to the very measure it serves.

The probe came before the C, run with the `stml` CLI over a projection
with hand-stamped ordinals, and it settled the rule's formulation:

- A captured case (`<casus>$c</casus>` on the preposition, `$c` again
  on the noun) binds to the preposition's FIRST adposition analysis
  and is never retried: `in urbem` yields nothing, because WORDS lists
  `in` with the ablative first. Per-case LITERAL rules find it — the
  matcher scans forward for a literal but not for a capture.
- A pattern rooted at `<sententia>` gives ONE greedy match per
  sentence: `Ad urbem et in silvam it` preferred `urbem` and never saw
  `silvam`. Floating on `<vocabulum>` enumerates every word, but the
  preposition is a sibling, and sibling-run matching is a reserved
  door of the pattern spec. So: two literal rules (accusative,
  ablative), one pair per sentence each, accepted and measured per
  decision 31; the door is the finding.

Measured with the cumulative table (`oraculum.sh -regulae`):

| treebank | primary crude | rule 1 | rules 1–2 |
|---|---|---|---|
| CIRCSE | 67.4 % | 67.7 % | 67.9 % |
| LLCT dev | 67.8 % | 67.8 % | 68.0 % |
| LLCT test | 67.7 % | 67.7 % | 67.9 % |
| EWT dev | 56.6 % | 56.6 % | 56.7 % |
| EWT test | 56.9 % | 56.9 % | 57.0 % |

Half a point on Seneca from two rules that touch one pair per
sentence: the skeleton walks. The primary pins start (678 / 679 / 679
/ 566 / 569 permille, only rising); coverage is unchanged, as it must
be — nothing is deleted. The projection grew 4.2 % (ordinals:
Hilarius 436,678 → 454,849 bytes), the computus golden regenerated
with that cause. Instruments: `verba.sh` and `arbor.sh -partes` apply
the program by default when the file exists, `-crudus` shows the
source order; `silva.Oratio(…, crudus=True)`; the oracle prints the
crude primary beside the resolved one.

Gates: `probatio_oratio_resolutio` (87: program loads with two
titled rules, bad programs refused with a line, missing root named,
language from the census; `Cum puella ambulat` — ablative first
after, nominative and vocative in their order, three analyses still,
summaries unchanged, census per rule, idempotent second run, zero
rules do nothing, the accusative rule alone leaves `cum puella`
alone, `In urbem venit` already first so nothing permuted, a sentence
without a preposition untouched, an empty program harmless; the
expanded plan judged by the canon; a malformed plan's three bad rows
refused and the good one applied; the verb refusing a
non-permutation), oraculum 115 → 127 (primary pins), materia nodus 92
(both verbs, refusals, fork safety, reponere on an unwritten slot
refused), canon (drift guard knows the plan vocabulary; a second
`radix="verum"` made the judge demand `regula` as THE root — a canon
declares one root, further roots ride as declared elements).

Lessons. The canon's root declaration is singular: two `radix`
elements and the last wins, silently changing what every document is
judged against. A doc-comment anchor in an edit script rots faster
than a code anchor. A skeleton rule that gains half a point is
exactly the right size for a skeleton: the mechanism is proven on
five treebanks before a single agreement rule exists.
A full-suite run must be read at its SUMMARY line: I grepped the
first twenty lines of a red run, saw only the computus golden, and
called it "only computus" — the partes gate's five exact-text
assertions on the projection (now carrying `n=`) were red beneath the
cut, and the commit's gate found them for me. And a regex
replacement's string processes `\\n` into a real newline: twice it
put a line break inside a C string literal; plain `str.replace` for
any replacement carrying escapes.

## 2026-09-05 — Glossary: the pictor merge's twenty-eight words

The rhubarb-pictor merge (177eab66) put 2,987 unknown identifier words
against the pin of 2,964, and the attribution through nexus.tsv named
28 words that live only in the merged files. Same mechanism as T14's
nine: a branch's new words meet oratio's lint one merge later. Glossary
section TERMINI PICTOR: seventeen coinages as Latin entries with the
forms the code uses (addressare, centrare, incrementare, maximizare,
recomponere, registrare, exeo's syncopated `exiit`, claudibilis,
durabilis, focusabilis, maximizabilis, minuibilis, numericus, defocus,
ephemera, rotula, schirmum, strategia), six permitted with a "rename if
you like" note for the pictor session (dctx, toy, botton, index2,
radix2, repo2). Count 2,987 → 2,958; pin lowered to 2,958. The words
were Fran's to permit or rename; permitting with a note keeps main
green without editing another session's identifiers.

## 2026-09-07 — The identifier pin was measured on a broken instrument

The pythonica gate's `usus: sedes definitionis`, red since
2026-09-05, led to the index defect recorded in silva's
nexus.worklog (01M1TD1FMFT3): a briar fixture's truncated latina.h
shadowed the house header in the sweep, the table lost 13 % of its
rows, and oratio's identifier lint counted on it. The pin of 2,958
(pictor pass) was a snapshot of a partially degraded table: the old
sweep on today's tree gives 2,732, the whole table 2,959 over 7,706
words. The 227 words the degraded table hid are enum constants and
locals inside functions the truncated header broke — Bible book
names (biblia.h), lapifex/arbor2 internals, and oratio's own
accident titles. Eight of those are grammarians' Latin WORDS lacks
(locativus, superlativus, infinitivus, imperativus,
plusquamperfectum, dualis, numerale, pronomen; ten forms with
infinitivum and pronominis): glossary section TERMINI GRAMMATICI,
count 2,949, pin 2,958 → 2,949 with the cause. Lesson for the lint:
a count that FALLS without a glossary or rename commit is the
instrument degrading, not the code improving — the slide the briar
session measured in its worktree (2,990 → 2,958 → 2,732) was the
alarm, misread as a worktree quirk.

## 2026-09-07 — The pin becomes a set, the corpus becomes the tracked tree, the runner heals the index

Three rough edges from the index incident, all decided with Fran
today. (1) A count pinned only falling could not tell "the code got
better" from "the instrument shrank" — the degraded table went 2,990
→ 2,958 → 2,732 green. The pin is now a committed SET,
`fixa/vocabula/ignota_symbolorum.txt` (2,949 words, byte order, `#`
comments): the gate prints every unknown word not on it with its
first site, and the three exits (glossary entry, ignotum-permissum,
rename). The set only shrinks; `ORATIO_VOCABULA_SCRIBERE=1`
regenerates it deliberately, like the computus golden. Vanished
words are reported, never red. The denominator — distinct
identifier words, 7,703 — is pinned only rising, so a shrinking
index goes red at the place its number is read. (2) The index walks
the filesystem, the other two corpora used `git ls-files`; another
session's uncommitted file counted against my gate. The identifier
corpus is now the git-tracked `.c/.h` set (`corpus_tractatae.txt`,
written by the runner and the wrapper; `-omnes-viae` lifts it with
the exclusions). Three words left with the untracked briar file.
(3) The oratio runner never renovated the index — only the silva,
officina and root suites' watchdog did, and the pythonica gate as a
side effect — which is the whole "one commit later" law. The runner
now runs `nexus.sh -renovare` before writing the corpora: under two
seconds when nothing changed, a full sweep only when the tool itself
changed. Planted fault: membership test inverted → red on `nova ==
0`, restored green.

Also today: the planting helper's pre-flight compiles with the house
include set, which lacked the amalgam world — `silva/amalgama` and
`silva/instrumenta` appended last, so nexus_ordines.c and the
praeparator can be planted. And the bash 3.2 same-second `-nt` trap
(a source restored in the second its object was compiled is never
rebuilt) inverted in 193 conditions across 73 scripts, `src -nt obj`
→ `! obj -nt src`; three "artifact strictly newer = fresh" tests kept
their original form, which was already the conservative one.

Still open, Fran's call: the pre-commit hook does not run this lint,
so a session that never lists the oratio gate still lands words on
main; with the set the report would be exact and three seconds.

## 2026-09-07 — The lint's failure carries its own remedy

Fran's question — "what does the message say and how is it
actionable?" — answered by demonstration: the direct gate output
named the word and one site; the commit helper's summary showed only
`nova == ZEPHYRUM at …:651`, because the words were printed before
the assertion and the summary copies the assertion. Four changes,
all in `oratio_vocabula_nova`, one function shared by the gate and
the instrument (`./oratio/vocabula.sh -nova`, exit 1 when new words
exist): (1) the failure is a `CREDO_CULPA` whose message IS the
payload — `IGNOTA NOVA 2: sem (443 sedes, briar/…:465), slot (…) -
exitus: …` — so every consumer of the FRACTA line sees the words;
(2) each word carries its first whole identifier (`adjacentiam in
_adjacentiam_aedificare`; new `OratioVerbum.identificator_primus`,
last TSV column `identificator`, Python `Verbum.identificator`);
(3) the report prints the three exits with the decision rule (many
sites → glossary entry or ignotum-permissum, few → rename; index
restored → regenerate the set with a cause) and ready-to-paste
glossary skeletons for the first five words; (4) the identifier
verdict moved to its own block IV b after the comments report, whose
unknown list (`after`, `const`) is a report and was sitting right
under the failure. The denominator failure carries its two causes
the same way. Planted fault (membership inverted) red with the
payload listing nineteen words and "+2930", restored green.

Found on the way: the report function returned −1 through an
unsigned `i32`, the exact vanity the house law names; it is `s32`
now. And a library file without `<stdio.h>` compiled for a day
because nothing in it printed until today.

## 2026-09-07 — The Latin lint blocks at commit (Fran's decision)

`tools/unci-git/lint_latinus.sh`, one function used by `pre-commit`
(before examen) and by a new `pre-merge-commit`: an automatic merge
never runs pre-commit, which is exactly how the branch words arrived.
Staged `.c/.h/.m` outside fixtures and vendor → the index heals
(`nexus.sh -renovare`, incremental in about a second; a full sweep is
announced when the tool changed or the table is absent) →
`./oratio/vocabula.sh -nova`: 0 is one quiet line, 1 is the full
report with the exits and the commit is refused, 2 means the
apparatus is broken and the commit is refused loudly. Bypass with
`--no-verify` and the cause in the message, as for examen. The
wrapper builds oratio's objects itself now, so a fresh worktree's
first commit pays once instead of failing. The nexus sweep took a
directory lock: two sessions committing at once, or a hook and the
watchdog, no longer race on the same `.nova`. Birth gate stages
IX–XI: a staged new file with `xyzzyquux` is refused with the
report, the merge hook refuses the same, a Latin-only file passes —
eleven of eleven in sixteen seconds, through the real index.

## 2026-09-07 — T18: the language rule, and what it exposed

Probe at the real seam instead of a hand-composed document: the rule
went into `resolutio.stml` and a real English sentence went through
`verba.sh`. Two answers in one run: the wildcard element takes
attributes and children (`<* n="$a"><lingua>1</lingua></>`, anonymous
closer), and a two-stage chain works inside a `<regula>` — a floating
`<sententia lingua="anglica"/>` narrows to the sentence, a floating
`<vocabulum>` stage under `de=` enumerates its words. No C in the
executor changed. The reference documents only `<*/>` and `<**>`; the
engine's tag test is a plain wildcard.

Then the measurement said 56.9 → 60.0 on EWT test, far below the
expectation, and the cause was not the rule. Moby lists case
variants before the lowercase record — `In\N` (indium), `Can\N`,
`Is\N`, `i\N`, `Me\N`, `US\N` before `in\PvAN`, `can\VN`, `is\V`,
`I\r`, `me\rN`, `us\r` — and the analyser returned records in file
order, so the first English reading of `in` was a noun and EWT's
adposition primary sat at 35 %. Lex casus in
`oratio_vocabularium_en_analysare`: a lowercase token prefers its
lowercase record; a Capitalized token (sentence start or name)
prefers the lowercase record, then the exact one; anything else
(`I`, `US`, `iPhone`) prefers the exact record; file order within a
rank. Crude EWT rose 56.9 → 60.1 from this alone.

Then the census: it counted ANALYSES per language, and `a` alone
carries twenty Latin analyses. Word votes instead
(`vocabula_linguarum`, one per word per language): the Latin
treebanks went from 5 / 4 / 12 sentences judged English to zero, so
the rule stopped costing Seneca a tenth. The remaining 134 / 197
English sentences judged Latin were ties (`I have.`), name-only
lines and lines without words — an artifact of judging sentences in
isolation, which real documents never do. The oracle now censuses
the whole treebank once, as the instruments census a whole document,
and passes that language to every sentence; the per-sentence census
stays as a diagnostic line.

Result: EWT dev 56.6 → 67.7 → 67.8, test 56.9 → 67.9 → 68.0 (pins
566/569 → 677/679); CIRCSE and LLCT exactly where T17 left them
(678/679/679), the adposition rules still adding their half point
after the language rule. The rule is FIRST in the program: document
language before case. Gates: resolutio section V (English sentence:
`in` from Latin ablative first to English adposition first, seven
analyses kept, six rows, two permutations, idempotent; Latin
sentence: zero rows), partes (case law on in / I / Me / US / can;
votes), oraculum (document language asserted per treebank, no Latin
sentence judged English). Planted fault (`<lingua>0</lingua>`) red,
restored green.

What remains for the English primary: readings the pass II lists
APPEND (auxiliare, particula, subordinans, symbolum at 0 %), and
Moby's first code where it is not the frequent use. Those are
class-prior rules, T19's territory with the agreement rules.

## 2026-09-07 — T19a: the oracle names its primary misses

Before writing a class-prior rule I wanted the data, not a guess:
which forms sit under which gold class with which of our classes
first, and how often. `_verbum_iudicare` now tallies every gold word
that is COVERED but not FIRST as (gold class, folded form, our first
class) in a hash keyed on two class bytes plus the form, with stable
Xar cells so the table can hold the cell pointers. `oraculum.sh
-discrepantiae` prints the twelve most frequent per class with a
remainder line; `-machina` adds `DISCREPANTIA` rows. The gate holds
the law that makes the listing trustworthy: per class the tally sums
to tecta − primaria, sorted non-increasing, and on CIRCSE's crude
order `est` gold auxiliare with verbum first appears at least fifty
times (105). Planted fault: counting hits instead of misses breaks
the sum law on the first treebank.

Two lessons on the way. The first stack-buffered key capped forms at
about 240 bytes and the law failed by exactly two words on EWT test:
two URL tokens tagged PROPN, 473 and 295 bytes long. Web text has no
maximum word. The key is allocated per form now. And the tally must
not use `sprintf` in the library: the key is binary, two class bytes
and the form, which a length-carrying chorda holds without a
terminator.

What the listing says, for T19b (read from the report, not from
memory): Latin `sum` forms are always verbum first (168/510/442 gold
auxiliare, none primary); `ad`, `a`, `cum`, `post`, `ante` have the
adverb or the abbreviation first (LLCT 374 `ad`); `non` is first the
abbreviation `Non.` (84/63); demonstratives and possessives are
pronoun or adjective first where the charters want determinans
(2096 gold, 0 primary); `qui` is DET 161 times in LLCT. English:
`is/be/are/was/will/would/have/can` verbum first (1543 gold
auxiliare), `to` adposition first 371 times against particula, `not`
adverb first (98), `if` coordinans first (86), `that` determinans
first (80 SCONJ + 91 PRON), `I` substantivum first 155 times, `just`
adjective first 57, `please` verb first 33, `$ % # +` interpunctio.
Ranges (enclitic `-que` 248 in CIRCSE, English `'s`/`n't`) can never
be primary under the current one-element-per-range judgment: a
measurement ceiling of about 2 % on Seneca, noted, not fixed.

## 2026-09-07 — T19b: class priors, written from the listing

Eleven rules and a glossary pass, every one with a number from
`-discrepantiae` as its cause, measured cumulatively after each
edit. What the measurement corrected on the way, in order:

1. **Order.** Appended after the language rule, no class-prior rule
   fired at all: first wins per word, and the language rule claims
   every English word. They sit before it now, each gated by the
   document language through the T18 chain so a Latin document never
   takes the English `is` auxiliary.
2. **Blind class preference is wrong for dictionary codes.** "Any
   interjection reading first" lost 1.2 points on EWT: Moby's `!`
   code sits on good, thanks, there, he, what. The rule now matches
   the pass II list's TITLE in `<nativum>` — the probe with a wrong
   title proved derived-token text is compared. Same for the
   subordinators, whose Latin list carries relative adverbs (ubi 68,
   unde 25 gold ADV in the charters) and whose English list carries
   so/when/where (58/30/16): the ambiguous members moved to
   `*-ambigua` lists, reading kept (coverage), title different (rule
   blind).
3. **Two exceptions by lemma.** `supra` is an adverb 209 times in
   the charters and the adposition rule flipped it; the article `a`
   is a preposition in Moby ("twice a day") and the English
   adposition rule flipped 492 of them. A one-lemma rule placed
   before the class rule claims the word first. Both matched nothing
   until I wrote `<lemma>` before `<lingua>`: forma children are an
   ordered subsequence of the analysis children, and lemma comes
   first. Silent, as the spec says loose matching is; now a law in
   the program header.
4. **Lists.** nec, neque, vero left the Latin particle list (gold
   conjunction 36+3, adverb 18, particle 0); `no` left the English
   interjection list for an ambiguous one (DET 36 > INTJ 9) with a
   glossary determiner entry first in source order.
5. **Glossary pass III** = priors that are not context: iam (WORDS
   gives the subjunctive of eo first), vel and nec (adverb first),
   the sanctus paradigm (304 charter words as participle of sancio),
   deus, casa, pars (participles of cado and partio first), four
   saints, English I (lowercase web `i` is the letter in Moby), just,
   even, only, still, however, go, said, no. Crude primary +1.5 to
   +3.4 per treebank from data alone.

Result: CIRCSE 67.9 → 72.8, LLCT 68.0/67.9 → 77.0/77.1, EWT
67.8/68.0 → 75.3/75.5, coverage unchanged. What the listing still
shows and this stage cannot reach: Latin determiners (2,096 charter
words at 0 %: pronoun vs determiner is adjacency to a noun), English
proper nouns (1,056: capitalized but not sentence-initial), `to`
before a verb, `that` as pronoun vs conjunction, ordinals as
adjectives. All of it is T19c's door and T19d's rules.

Planted-fault lesson: retitling a `<regula>` is NOT a fault. The
executor applies `<praelatio>` rows in program order whatever the
rule's title says; the title only names the census entry. The gate
stayed green and planta refused it as "porta muta". The real mute is
the language index in the pattern (`<lingua>1</lingua>` → `0`): the
rule then selects nothing English and the EWT pin goes red.

## 2026-09-07 — T19c: the door, and what it did not buy

`cursus="fratrum"` landed in the pattern engine (increment 20,
b29ce72f) and the two adposition rules moved from a sentence root to
an `<elementa>` root under it. At the seam it does what T17 asked
for: `Cum puella et cum femina ambulat` gives both nouns the ablative
first, where the greedy match served only `puella`. On the treebanks
the cumulative table is byte-identical to the run before the door.
Sentences with two prepositions governing the same case, whose
second noun did not already have that case first, are simply too
rare in Seneca and the charters to move a permille. The right
reading of that: the door was pulled for the rules that need a
neighbour and do not exist yet, not for these two, and building it
before them was the correct order because the umbra design needs
it. Also learned, cheaply: `Ad urbem et in silvam it` is not a
diagnostic sentence, both nouns have one reading.

Fran's direction the same afternoon reshapes T19d. Agreement and
government are not to be written as a dozen hand rules but as UMBRA
nodes in the tree: a reading carries the dependent it expects, with
its conditions drawn from the dictionary's own codes and an empty
binding; one generic pattern per relation fills them from siblings
using the door and the existential; a bound umbra is a dependency
edge and an unfilled one a finding, and a reading whose umbrae fill
outranks one whose do not. The first umbra will be the adposition's
object, because the hand rule for it exists and gives an exact
comparison on the same numbers.

## 2026-09-07 — T19d skeleton: the first umbra

The adposition's object, as a node in the tree. Registry first: one
list slot appended to all seventeen analysis genera and a genus after
them, which meant the class mapping needed an explicit upper bound
because the old law "analyses run to the end of the table" was in
three places (the mapping function, the registrum gate, the ornament
hook's `>= PRIMUM` test, which now stamps ordinals on umbrae too, as
wanted). The annotator gives every adposition reading with a case an
object umbra in that case; nothing else changes shape, absent lists
are not written, so every document without prepositions projects
byte-identically to yesterday.

The rules then shrank the right way: the two T17 rules said
"adposition with case c, then noun with case c"; the two umbra rules
say "any reading whose umbra wants case c, then noun with case c".
Same numbers plus one permille on each Latin treebank, because the
carrier reading is now preferred as well as the filler. The
constraint moved from the rule into the tree, which is the whole
point: a transitive verb's accusative umbra will match the same rule
without a new rule.

Two details worth recording. Binding must happen after the
permutation, because the plan's ordinals describe the projection as
it was before readings were reordered; the executor keeps the
umbra's node pointer (stable across the list permutation) and remaps
the filler's index through the same "preferred first, rest in order"
law the permutation uses, so `puella`'s ablative, third in source
order, is bound as reading 0. And the projection writes list-of-node
slots in the compact layout form (`<umbrae(>`, `<umbra n="0" ((>`),
which looked like a defect for a minute; it is the pretty printer's
notation and the reader takes it back byte-exactly.

The editing tools cost the usual: an all-or-nothing transaction
refused twice on anchors I had already rewritten inside the same
script (the editor applies replacements cumulatively, so a later
anchor must name the text as it is after the earlier ones), and an
Editio rejection still wrote the gate file, which then carried a call
to a helper that did not exist until I added it.

## 2026-09-07 — T19d head umbra: stages, and the vindicated filler

The second umbra is the adjective's head. The annotator gives every
adjective and determiner reading with a case a CAPUT umbra carrying
the reading's own case, number and gender, so the rule has nothing
to say about agreement except "same": fourteen generated rules (seven
cases, head before or after) capture number and gender at the umbra
and demand them of the filler. The case stays literal; a capture
binds to the first reading it meets and never retries, and the umbra
is the first place a capture on number or gender is read, which is
why those two may be captured and the case may not.

Single-stage this was wrong on the first sentence with a
preposition. `Cum puella bona ambulat`: the rules are ordered by case,
so the nominative head rule ran before the ablative one, found
`puella`'s nominative reading (still in the list — the object umbra's
row had preferred the ablative, but the nominative is a valid
filler for a nominative `bona`) and bound `bona` nominative. Two
things fix it and both are executor law, not rule cleverness. Stages:
a rule carries `gradus`, the program runs stage by stage, and every
stage composes against a fresh projection of the sentence, so the
head rules see `puella` with its ablative first. And the vindicated
filler: a word decided by an earlier row is spoken for, so a filler
row that names one of its readings other than the first is repetita.
With both, the nominative row on `puella` is refused (puella is
vindicated, its nominative is now reading 2) and the ablative row
binds. The ordering of the head rules by case is therefore harmless
— any order would give the same answer once the object umbra has
spoken — which is the property to keep: the evidence decides, not
the rule index.

`Hoc templum est` showed the other half. `templum`'s crude first
reading was a genitive plural (WORDS lists `templum` under a stem
whose first inflection is that); `hoc`'s determiner reading wants a
nominative singular neuter head, the rule finds `templum`'s
nominative reading further down the list, prefers both, binds. And
`Hoc est` leaves `hoc` a pronoun because nothing fills the umbra: the
unfilled umbra is the finding the design promised.

Two gate notes. The filler is written `<* n="$b">` (any class) rather
than pinned to reading 0, because pinning it hid the very rows the
vindicated-filler law needs to refuse; and pins are taken from the
gate's own permille (computed from counts), not from the instrument's
percentages rounded — the two differ by one in the last place and
the gate is the one that judges.

## 2026-09-07 — T19d gamma: the law of umbrae, measured four ways

The law Fran named — a reading whose umbrae cannot be filled yields
to one whose can — went in as C after the stages, and the first
shape I wrote was wrong in two instructive ways. I had generalised
it to three ranks (filled, no umbrae, unfilled), applied to every
word. Every treebank fell, English included, which was the tell:
English readings carry no case and no umbra, so the only way English
could move was a Latin reading jumping over an English one. That is
what happened — `a` in "a die" has a Latin ablative-preposition
reading whose object umbra the stage-1 rule bound to `die`'s Latin
ablative, and the law promoted it over the determiner the language
rule had chosen. The same on the charters: `supra`, decided adverb
by its exception rule, bound to any accusative in the sentence and
promoted to preposition. First lesson: a binding is weaker evidence
than an explicit rule, and the executor must know which words a
`<praelatio>` row decided. A second array beside `vindicata`
(`explicita`) carries that, and the law skips those words.

Second: the demotion. Prepositions whose object is a proper name
lost 11 points on the charters, because the capital rule's
nomen-proprium reading has no case and cannot fill a case umbra. The
"no object" evidence is real (it is what would fix `cum venit`), but
it cannot be used before unknown-case fillers exist. So the law is
promotion only, which is in fact what Fran's sentence says.

Third, and the one I did not predict: promotion at any distance cost
Seneca half a point in nouns. The `praecedente` head rule takes the
noun as the start and the carrier as a later sibling, so a carrier
is bound to the FIRST agreeing earlier word in the sentence, not the
nearest; in Seneca's long sentences a noun's adjective homograph
finds an agreeing word somewhere often enough. Counting only fillers
at distance one removed the loss and kept most of the charters'
gain; it is a stand-in for "nearest agreeing word", which needs the
pattern engine's reserved `strictus` door.

The measurement discipline paid for itself: four variants, each on
the five treebanks with per-class tables, about a minute each after
the runner rebuilt. The corpus check in the gate (every bound umbra
points at a reading of the umbra's case, number and gender, over all
of Hilarius) is what makes the index remap trustworthy — permuting a
word after bindings point into it is exactly the kind of defect that
compounds silently.

Tooling notes. Editio re-lays the declaration block around an
insertion and can push a two-column glomus past 72 (the oracle's
`principale`) or over-indent a single line; the `-vitia` diff against
HEAD, line numbers stripped from both the position and the message
(`l.NNN`), is the reliable check. An Editio REICE still writes the
file. The oracle wrapper's `"${ARGS[@]}"` under `set -u` is an error
in bash 3.2 when the array is empty — `${ARGS[@]+"${ARGS[@]}"}`.

## 2026-09-07 (late) — T19e: English by evidence, and what the order of rules means

The English tranche was supposed to be cheap and it was, once one
thing was understood: an evidence rule must run before a default
rule, because the executor's first-wins law makes the earlier row
final. The first measurement had proper nouns up 46 points and
particles down 55, and the probe showed `to buy` with its particle
umbra bound and the adposition still first — the adposition prior at
index 7 had spoken before the umbra rule at index 13. Moving the
evidence rules to the head of the program fixed it in one run.

The class umbra worked as designed without an executor change: an
English reading has no case, so its umbra names a class (`classis`
verbum) and the rule fills it from the next word's verb reading,
`strictus` doing the adjacency. Two data traps cost a round each.
Moby codes `a` as a verb (`a\DNVP`), so `have a car` bound `a` and
made `have` an auxiliary; the fix was in the executor, not the data:
within a stage a filler word an explicit rule already decided (the
`a` exception) is not re-read as another reading — the same law the
umbra ranking already respected across stages. I first wrote it for
any earlier decision and the charters lost 0.2, because there the
earlier decision is the accusative-before-ablative order of the
object rules, which is no evidence at all; restricted to explicit
rows it costs nothing. And Moby lists `is`, `it's`, `I'm` as nouns,
which is why a determiner head umbra (`the` + noun) preferred the
noun reading of `is` after that/this/it and cost more auxiliaries
than it gained nouns — measured and dropped, umbra and rule both.

Two smaller findings. The auxiliary prior selected by list title lost
the contraction auxiliaries (`'s 'm 'll` carry Moby's rule text as
their nativum), so a second prior selects them by source. And the
capital rule fires on `I'm` because Moby knows it as a noun; the
exclusion that works is "has a pronoun reading", not "has an
apostrophe", because possessive names (`Debra's`) must keep their
capital reading — proper-noun coverage is pinned and went red the
first way. EWT ends at 78.6 / 79.0 from 75.3 / 75.5; Latin did not
move by a permille through any of it.

## 2026-09-07 (night) — T19f: fillers without a case, and the demotion measured a third time

The umbra says "object in the ablative"; a proper noun the capital
rule invented has no case at all. Letting it fill anyway is a rule,
not an executor change: three rules after the exact ones, filler = a
reading whose nativum is `capitalis`, carrier = a Latin reading with
an umbra of the relation, any case. Every Latin treebank rose, the
corpus invariant learned that an unwritten accident is compatible,
and the gate pins moved. Then I re-ran the experiment the fillers
were meant to unblock, and it failed the same way as this afternoon:
without the adposition prior the charters lose 0.4, with demotion
they lose more. The reason was in the ignota column all along - an
unknown word has no reading node, so a preposition before an unknown
noun still looks objectless, and 18 % of charter nouns are unknown.
So the next lever is not a rule but a registry-visible change: give
unknown words an `analysis-ignoti` reading (the genus exists, unused)
so they can be bound, then measure demotion a fourth time. I did not
start it tonight: it moves the projection of every unknown word, the
computus golden, and the Python face's `ignota()` contract, and that
deserves a fresh session. Method note: three variants, one script,
under a minute each; the numbers refused the idea faster than an
argument could have.

## 2026-09-08 — T19g: the decision in the tree, and the first honest profile

Fran's sudoku doctrine (spec §2, decisions 36–42) asked for one
instrument before anything else: say, per word, what kind of act put
the first reading first, so that forced decisions can be measured
apart from priors and from dictionary luck. The tree did not know:
bindings were recorded on the reading, but a `<praelatio>` row lived
only in the plan document and was discarded. Two slots on the word
fix that — `decisio` (praelatio | impletio | umbra) and `auctor` (the
rule's title) — written by the executor after the last stage, and the
whole rest of the tranche is reading them back: verba columns, the
Python face, the oracle's partition and per-rule table, and pins on
the accuracy of forced decisions.

Two things the first run taught about the label itself. The law of
umbrae reorders a word whenever any neighbour-filled reading sits
below an unfilled one, and in `Puella bona ambulat` the vocative head
rule binds a second reading of `bona` in the same stage (the filler
law only blocks vindicated words, and vindication happens after the
stage), so the law lifted the vocative reading to second place and I
had labelled the word `umbra` although the nominative rule had made
it first. The decision is what made the FIRST reading first; the law
is the author only when it changed that. And `in` before `bona`
carries `praelatio` by the Latin adposition prior, not `impletio` by
the object rule, because the Latin program still runs priors before
evidence — the English order was inverted on 2026-09-07 for exactly
this reason and the Latin one never was. The label surfaced it in the
first probe.

The numbers. On Seneca and the charters the forced decisions are the
least trustworthy tier: 72 / 69 / 69 % against 85–98 % for the priors
and 80–92 % for words nobody touched. The law of umbrae is the worst
single decider (51–57 %); the dative following head rule is right
37–41 % on the charters; the vocative rules under 55 %. The adposition
prior is right 99 % on the charters, which says preposition homographs
hardly exist there and the object umbra adds nothing to CLASS
accuracy — its value is the edge. The unknown-case object filler is
99 % on the charters and 57 % on Seneca: names after prepositions are
what charters are made of; on Seneca the capital rule's readings are
often not names at all. English is the other picture: 79 % of words
decided by the language rule at 78 %, the class umbrae 92 % but rare,
and the contraction prior 0 % because contractions are ranges judged
by their first word (`it's` → `it`), which the new `ranga` bucket now
shows as a ceiling instead of hiding.

What this changes in the plan: no new rule until the head rules and
the law are made trustworthy, and the instrument for that is the
next one — the wrong-forced listing per rule, which the per-rule
table already keys. Method notes: the sum law of the partition needs
every judged word in exactly one bucket (range words after the first
get their own), and the identifier lint refused `profilum`, which is
not Latin; `partitio` is.

## 2026-09-08 — T19g bis: the listing, and what distance says

The per-rule accuracy table said which rules decide wrongly; the
listing says on which forms and, more usefully, bound to what and
how far away. The partner is found through the parent pointer: a
carrier's first reading names its filler in the umbra; a filler has
to be found by scanning the sentence for the nearest reading whose
umbra points back at it, and the first version of that scan looked
only at neighbours and came back empty for most fillers, because
the `sequente` head rules run on `cursus="fratrum"` and bind past
any gap. That emptiness was the finding. Splitting each rule's
accuracy by partner distance made it a number: the genitive head
rule on the charters is right 99 % adjacent and 33 % distant, the
accusative 84 against 52, Seneca's nominative 82 against 70 with
three quarters of its decisions distant. The law of umbrae was
restricted to neighbours on 2026-09-07 after the same lesson, but
the row's preference — the part that actually puts a reading first
— never was. The object rules are the opposite and bind legitimately
past a modifier, so the restriction belongs to the head rules alone.

The charters have a second failure that distance does not explain:
their nominative head rule is wrong 39 % of the time even adjacent,
and the dative one 73 %. The listing shows why in three lines:
`suprascripti`, `mea`, `alio`, `nostro`, `omnia` — all DET in UD's
convention, all carrying both a determinans and an adjective reading
with a head umbra, and the adjective wins the tie because it comes
first in the list. That is the "two filled readings tie by rule
order" finding from the law's first measurement, now with a name and
a count, and it is data, not a rule.

Method notes. Editio judges a file against the headers on disk, so
an instrument that references a new struct field must be edited
after the header, not before; I did it the wrong way round twice.
And the sum law per rule (errata equal words minus primary) is what
made me trust the partner search: it held before and after the
rewrite, so the rewrite changed only what the rows say, not how many
there are.

## 2026-09-08 — T19h: the strict tier, measured both ways

The listing said distance discriminates the head rules, so the first
experiment was the obvious one: all sixteen head rules strict. It
raised the forced accuracy to 76 % and lost primary on every Latin
treebank, because half the forced decisions disappeared and the words
they had decided fell back to the crude order, which for a noun with
an adjective homograph is worse than a 70 % guess. That is the
sudoku doctrine meeting its own numbers: a decision at 70 % is not a
constraint, but removing it is not free either. The version that
gains is the tier: strict copies of the sixteen rules in their own
stage before the loose ones, so that an adjacent agreeing word of any
case beats a distant one of an earlier case. The gain is small
(Seneca +0.1, charters +0.1 / +0.2, forced +0.2 / +0.2 / +0.6) and
it costs sixteen generated rules, which is now a real argument for
the template the head rules should have been from the start. What
remains in the listing is not distance: the charters' possessives
and quantifiers that UD calls determiners, and Seneca's long tail of
homographs. The first is data; the second needs the loose rules to
bind without deciding.

## 2026-09-08 — T19i: determiners by data, and what a prior may not freeze

The listing's second family was a convention, and conventions are
data. Counting the gold class per lemma over the three Latin
treebanks gave a list with no exceptions at all — thirty lemmas that
are DET every single time, four thousand words — and a short list of
pronouns that never are, with `unus` split between the treebanks
(numeral in Seneca, determiner in the charters) and therefore left
alone. The prior on the certain list moved Seneca three points and
the charters four, and its own errors turned out to be shapes, not
lemmas: `tantum` and `quantum` are adverbs by form, `hic` the adverb
is the nominative masculine while `hoc` and `hac` are determiners, and
`toto` is somebody's name in Lucca. The exclusion is therefore by
lemma plus accidents, and `solus` went out altogether because the sun
and the soil share its forms.

The prior exposed a flaw in first-wins that the class metric cannot
see: a `<praelatio>` row decides a READING, and the first determiner
reading of `hoc` in the list is the ablative, so "Hoc templum" kept
`hoc` ablative while binding its nominative twin to `templum`. The
executor now distinguishes what the prior decided (the class) from
what agreement decides (the reading within that class): an impletio
row on an explicit word is accepted when it names a reading of the
decided class, or the twin of the bound reading in that class, and
that reading goes first. The first draft of the law was too broad
twice, and the treebanks said so within a minute each time: without
requiring the same lemma and language, a Latin reading of the English
word `die` refined the English noun and English lost 0.3; without
restricting carriers to head umbrae, the accusative object rule
refined `in` before the ablative one could, and "in bona terra" turned
accusative. Agreement is evidence about the carrier; object case is
not, so object carriers keep the preposition's decided reading and the
object follows it. Both restrictions are in the code with their
sentences.

## 2026-09-08 — T19j: the tail, measured four ways and left alone

Seneca's remaining forced errors are adjective homographs of nouns and
participles bound far away, so the obvious laws were tried, each on
all five treebanks: (A) a head must be a strong class — noun, proper
noun, pronoun — and a weak one is refused (Seneca +0.2, charters
−0.1 / −0.3); (B) a weak head binds but prefers nothing and the law of
umbrae ignores it (identical numbers, so the preference on weak heads
is not where the charters' value sits); (C) only the substitution of
a strong twin with the same accidents (Seneca +0.1, charters −0.2 /
−0.1: `sancta` the noun preferred over `sancta` the adjective); (D) a
loose-stage carrier whose crude first reading is a noun or verb binds
without preferring (Seneca −0.4, charters −0.2 / −0.4). The last one
is the instructive one: a distant adjective binding at seventy percent
still beats the crude order for exactly those words, on Seneca too.
Before that the noun-only pattern had been measured as well (charters
−0.6 / −1.2), which says the charters' heads are proper nouns and
pronouns as often as nouns, and, when the noun is one of the eighteen
percent the dictionary does not know, the agreeing adjective next to
it. None of the four is kept; the executor is back at 1e2fdd9d. The
tail waits for the ignoti reading, which puts those unknown heads on
the board, and for a dimension beyond agreement — a verb's slots — for
Seneca's long sentences.

## 2026-09-08 — T19k: the unknown word gets a cell

Decision 37 of the doctrine says an unknown word is a cell with every
candidate, not a hole in the grid, and the executor had been saying
the opposite for two days: `ignotum` was a summary token, there was
no reading node, and nothing could bind to it. The reading is small —
folded form as lemma, `ignotum` as nativum, source `regula`, and no
language at all, which needed a flag on the description so the slot
stays unwritten and the census and the language rule stay blind to
it. The summary token does not change, so every selector and the
Python face keep working; the projection grows one node per unknown
word and the computus golden moved with a named cause.

Two findings on the first run. Unknown HEADS are not evidence: an
adjective homograph next to an unknown word is an adjective about
half the time, on both treebanks, and the two strict head rules for
unknown heads cost a tenth of primary each; they were measured and
dropped, only the object rule stays. And the partition had been
flattering the forced tier: a filler with a single reading was being
labelled "decided by impletio" and was trivially right, so the
charters' forced accuracy read 76 %; with vacuous decisions no longer
recorded it reads 71 %, which is the true number and the pins moved
down with the cause written beside them. Both are the doctrine doing
its job: the honest number is lower and the useless rule is gone.

## 2026-09-08 — T19l: the demotion, measured a fourth time on a complete grid

This was the experiment the ignoti reading was built for, and it
fails cleanly. Three steps, each on the five treebanks: (E1) the
Latin adposition prior removed so prepositions are decided by evidence
alone — Seneca −0.1, charters −0.3 / −0.2, because the object rules
accept only noun fillers and "cum eo", "in ipsa" have no evidence;
(E3a) the prior kept and the object filler widened to any reading in
the preposition's case — charters +0.1 / +0.1, Seneca −0.1; (E3b) on
top of that, an explicit adposition reading with no object bound
anywhere in the sentence demoted below the readings that carry no
empty object — no change at all. The listing says why: when a
preposition has no object, its other readings are mostly worse than
the adposition — the capital rule's name for a sentence-initial `A`,
`de` as a noun, `super` as a verb, `ad` as an adverb — and the one
real target, `cum` the conjunction, is two words on Seneca. The
contradiction is sound now that unknown objects bind; it is just not
worth anything, because the candidate that would replace the
preposition is not on the board either. The executor is back at
e6538dec. What the tail and the demotion both point at is the same
missing dimension: a finite verb's slots, which would make `cum` a
conjunction by the verb it governs rather than by the object it
lacks.

## 2026-09-08 — T20a: the clause, stamped and measured on day one

The box between adjacency and the whole sentence now exists in the
tree. A `clausula` genus (semen, species, pater, two umbrae born from
the schema: the finite verb by `forma-verbi = finitum`, the subject by
the nominative) sits in a `clausulae` list on the sentence, and every
element points at its clause with `clausula` + `clausula-causa`, the
causa being the layer that placed it. Registry 184 slots, seal
62a7afd8. One definition refinement against the spec: the umbra's new
condition slot is `forma-verbi`, not `modus`, because one value says
"finite" where mood would need three.

The seed lists were measured before they were written. A gold clause
is the subtree of a finite root (finite mood and not aux/cop, or the
predicate of a finite aux/cop), and a candidate is scored by how often
its own clause differs from the previous non-punctuation
non-coordinator word's. The Python census and the C instrument
(`oraculum.sh -semina`) agree line for line. Certain: si, quia,
qualiter, dum, quod, sicut (86–100 % on the charters, 73–100 % on
Seneca). Corroborated (a finite-verb-capable word must follow before
the next seed): ut (57 % on Seneca because of the comparative "ut
leo"), cum, nisi, ubi, unde. Relatives by lemma `qui` except the forms
quis/quid, because WORDS gives one lemma to the relative, the
interrogative and the indefinite, and `si quis` is the charters'
favourite opening. Coordinators only between finite-verb-capable words
(76–78 % against 22–31 % otherwise); `-que` never (0–3 %: it stands
after the boundary). The comma is not a seed (23–37 %) but closes a
subordinate clause (53–59 %); Seneca's treebank has no punctuation at
all.

The stack layer had two bugs the hand cases caught: a parenthesis
alone did not count as structure, so the sentence fell into the
seedless branch, and a subordinate clause that opens the sentence
could not close because closure assumed the base of the stack was the
main clause. The fix is a per-clause closability flag; the main clause
is born lazily after the first closure and adopts the orphans.

The oracle scores purity per layer (majority gold clause of each of
our clauses), coverage, and the share of sentences with the gold
clause count, and lists the misplaced words with their layer, our
species and the gold root. Birth: semen 89/96/95, extentum 76/86/89,
clausura 49/65/66, unica 74/98/99 on Seneca and the two charter files;
overall purity pinned 738/829/864 (English 726/739, all `unica`), count
agreement 52/74/77 %. Two findings. The closure layer is the weakest,
as the design predicted, and on the charters it carries 29 % of the
words, so it is where the next point lives. And the residual on all
three Latin files is one shape: a second finite verb with no seed in
front of it — Seneca's imperatives and verse, the charters' formulae
"alio lato tenet in terra", "ego ... filio ... manifestu sum" — which
is exactly law 1 of T20c (a certain finite verb in a clause that has
one already is a missed seed, a fifth layer). Primary and forced
accuracy did not move by a permille: no rule reads the clause yet.
The computus golden moved with the cause named (two umbra nodes and a
clause node per sentence). Planted fault (si removed from the certain
list) red on the `si quis` case, green restored.

## 2026-09-08 — T20a bis: the verb split, measured both ways

The birth listing said the residual was a second finite verb with no
seed, so the fifth layer came before the chain layer, as Fran chose.
A certain finite verb in a non-closable clause that already holds its
verb opens a new coordinate clause with the verb as its seed; the verb
and the words after it up to the next seed carry the reason `verbum`.
The one design choice was where the cut falls, and it was measured
rather than guessed: A puts the words between the two verbs into the
new clause (verb-final order: a clause's words precede its verb), B
cuts right before the second verb. A wins on all three Latin files —
overall purity 73.8 → 82.2 on Seneca (B 80.5), 82.9 → 84.8 and 86.4 →
88.1 on the charters (B 84.5, 87.5) — and both variants create the
same number of clauses, so the difference is purely the placement of
the middle words. Verb-final holds for the charters too, which I had
doubted after the `est` errata; those are the closure layer's
problem, not this one's. Count agreement on Seneca rose from 52 to 65
percent and the clause count from 1.68 to 2.07 per sentence against
2.27 gold; the charters now over-segment slightly (2.46 against 2.25
on the test file), which the listing will explain next. The new
layer scores 69 / 69 / 76 percent on its own words and lifted
`extentum` and `unica` by ten points on Seneca by taking their bad
words away, which is the doctrine's picture of a layer: a reason
that can be scored, added without touching the others.

## 2026-09-08 — T20a ter: the closure layer, and the measure that had to change first

The listing said `est` was the closure layer's top error on the
charters, but the listing has no sentence context, so the first
thing built was a scratch view that prints charter sentences with our
clause and reason beside the gold root, word by word. It showed four
shapes, and only one of them is about closure. `qui supra` is the
charters' fixed phrase for "the aforesaid", a relative with no verb
that UD keeps in the main clause; we opened a relative clause that
never closed. A copula closes a clause too early, because the
predicate follows it (`qui fuit quondam Filippi presbiteri`). A
sentence-initial `unde` is a connective, not a subordinator, and the
seed census had already said so. And a coordinator right before a seed
belongs to the seeded clause, which is how the census defined a
boundary in the first place. A fifth came from `tenet`, which never
split a clause because Moby knows an English noun `tenet`: the
certainty test counted every reading, and English readings should not
vote in a Latin sentence.

Each went behind a switch and was measured alone on the three Latin
files, and the first measurement taught me something about the
measure. Purity said `qui supra` lost half a point while the clause
count moved toward gold, because a false relative clause of three
words is three "pure" words, and merging them into the main clause
makes them compete in its majority. Purity rewards over-segmentation;
the count rewards nothing in particular. The doctrine asked for a
profile, not a score, but a decision between variants needs one
number that both directions can lose on, and that is pairwise
concordance: for every pair of words in a sentence, do we and the
gold agree on whether they share a clause. Under that measure, from a
base of 79.9 / 76.9 / 78.1: `qui supra` = / +0.5 / +0.2, the copula
+0.2 / +1.0 / +0.2, the coordinator = / +0.1 / +0.3, Latin-only
certainty +1.9 / +1.0 / +0.5, all four together +2.1 / +4.3 / +1.8,
which is more than their sum on the charters. The initial-adverb rule
lost 0.2 alone and added nothing in combination, so it is gone.
Concordance is now the pinned floor; sentence-count agreement is
reported, since it fell six permille on one charter file while both
other measures rose, and it is as one-sided as purity.

Latin-only certainty then broke English, in an instructive way: `it`,
`sit` and `do` are finite Latin verbs, and once English readings
stopped voting, English sentences split at every `it`. The stamper
had never known the document language; it takes it from the resolver
now, and an English document is not seeded by Latin readings at all,
only by parentheses. That cost two permille of English purity, which
the accidental Latin seeds had been buying, and the pins moved down
with that cause named.

Final: purity 850 / 870 / 893, concordance 819 / 811 / 798, clauses
per sentence 2.22 against 2.27 on Seneca. The charters still
over-segment (2.65 and 2.51 against 2.52 and 2.25), and the closure
layer itself is still the weakest at 66 / 74 / 69; both point at the
same next question, which of our extra clauses are the verb split
firing inside formulae where the gold sees one clause.

## 2026-09-08 — T20a quater: the sentence view in the instrument, and the split's extra clauses

The scratch view that found the four closure shapes is now
`oraculum.sh -clausulae -sententiae N [-causa T]`: N sentences with a
misplaced word of layer T, each with its text and, per gold word, our
clause, the layer and the gold root, the wrong ones marked. The
oracle collects the records and the instrument prints them, so a gate
could assert on them later.

Read for the `verbum` layer on the charters, it showed that the verb
split's extra clauses are mostly not the split's fault. `id est`,
"that is", is a fixed phrase whose `est` is a certain copula in a
clause that already has its verb, so we split there and, cutting after
the first verb, dragged the object of `dedisti` into a false clause.
An appositive comma closed a subordinate clause before its verb had
come, so the verb landed in the main clause and split it. A comma
before `et` stopped the coordinator's corroboration scan, so the
coordination was never seeded and split instead. And `ubi vocitatur
Iuveiano` closed at the naming verb and dropped the name into the
parent. Four switches, each measured alone by concordance from
82.0 / 81.2 / 79.9: `id est` = / +0.4 / +0.2, the name after the verb
= / +0.1 / +0.1, and those two together = / +0.5 / +0.4. The comma
rule (a clause without its verb does not close at a comma) was +0.8
on the test file and −0.1 on dev alone, and −0.6 on dev beside the
other two, so it is out despite being the principled one; the
coordinator scan through commas lost 0.9 on test and is out. Seneca
did not move by a permille under any of the four: these are charter
formulae. Concordance pinned 820 / 816 / 802; clause counts 2.58 and
2.44 per sentence against 2.52 and 2.25 gold, so the charters still
over-segment a little and the test file's excess is the next thing
the view should be pointed at, with `-causa extentum` this time.

## 2026-09-08 — T20a quinquies: the extentum view, `cum`, and the second measure demoted

The view on the `extentum` layer of the charters' test file gave three
shapes. `una cum` is "together with", a preposition in all 112 gold
cases and never a boundary, and we were seeding a `cum` clause after
it whenever any finite-capable word followed. `cum` itself is a
preposition in 98 percent of charter occurrences, so corroboration by
a merely capable verb, which in the charters is any homograph, seeds
far too often. And `quantum ... est pertenentes` is a relative-style
clause the gold sees, opened by a form of `quantus`.

Measured alone by concordance from 82.0 / 81.7 / 80.3: `quantus`
= / +0.2 / +0.3 and clean; `una cum` = / +0.3 / −0.1 alone but nothing
lost beside `quantus`; `cum` requiring a certain verb −0.1 / +0.8 /
+0.3, the Seneca loss being the classic `cum` clause whose subjunctive
verb is a homograph. The middle reading, certain or subjunctive,
took the charters' gain and held Seneca at the loop's resolution,
then lost one permille at the gate's: 39 pairs in 125 thousand. The
rule is strict at the gate's resolution, so one more shape: the
distinction Latin makes is the word after `cum`. If it can be
ablative, the preposition is likely and a certain or subjunctive verb
is required; if it cannot, any capable verb corroborates as before.
That returned the 39 pairs and kept the charters: final 820 / 826 /
809 against 820 / 816 / 802.

The other thing the run taught: purity fell three permille on the
test charters while concordance rose seven and the clause count moved
toward gold, which is purity rewarding over-segmentation once more.
It joins count agreement as a reported number; concordance is the
only pinned decision measure now, which is what the ter entry
already argued and I had left half done.

## 2026-09-08 — T20a sexies: the concordance floor in exact pairs

Fran asked whether the invisible pair loss could be addressed. It was
two things. The measurement loop read the human table at a tenth of
a percent while the gate compared truncated permille, so the loop
could not see what the gate would judge; it reads the machine rows
now, and the instrument's human line prints the permille exactly as
the gate computes it, with the counts beside it. And truncation put
the boundary where luck put it: a base of 820.4 hides a 39-pair loss
that a base of 820.0 shows. The concordance pins are now the exact
number of concordant pairs per treebank, only rising: no boundary, no
luck, and the strictest reading of the rule that no treebank may
fall, which is the reading that produced the ablative test for `cum`
today instead of a tolerance that would have kept the worse rule. If
a tolerance is ever needed, the right one is a sentence-level
comparison (how many sentences improved against how many worsened),
which distinguishes noise from a real minority class; it is not
built.

## 2026-09-08 — T20b: the chain layer, measured and left as a counter

The design had the chain layer fill open words along bound umbrae.
There are no open words: the span layers place everything, so the
chain could only count discordiae, bindings whose two ends sit in
different clauses, and the real question was the override: should
the weaker placement follow the binding? The strength of a placement
is its layer's measured purity, so semen, unica, extentum, verbum,
clausura in that order, and the weaker end moves to the stronger's
clause. Four variants against the count-only base, in exact pairs:
adjacent bindings move almost nothing, five words on Seneca, one and
two on the charters, and lose 33 pairs on Seneca; any-distance
bindings move 265 to 624 words per file and lose a full percent
everywhere. Adjacent bindings do not cross clauses because spans are
contiguous; distant bindings cross them 547 / 1016 / 956 times and are
wrong more often than the span is. The layer stays as a counter with
the override at zero, the reason `catena` exists in the enumeration
and places nothing, and the discordia count is now read the other
way: a binding that crosses our box is evidence against the binding.
That is law 3 of T20c, the loose head rules confined to the clause,
which the T19g bis listing already wanted when it found distant head
bindings at 33 to 70 percent. The oracle reports the counts; nothing
is pinned on them.

## 2026-09-08 — T20c I: the box as a constraint on the loose heads, measured seven ways

The payoff direction T20b named: a head binding whose ends sit in
different clauses is wrong more often than the box, so refuse it.
Two mechanisms. The executor predicate `_trans_clausulas` refuses an
impletio row whose ends are stamped into different clauses (the
relative pronoun's binding to its antecedent excepted), switched by
`RECUSATIO_TRANS_CLAUSULAS`; and the pattern itself, `clausula="$c"`
on both words of the sixteen loose head rules, a repeated capture
being equality in the pattern machine. The pattern cannot say "only
where the stamp is pure", the executor can.

The numbers, primary / forced permille per file, base Seneca 799 /
765, dev 838 / 710, test 831 / 711, EWT untouched by every variant:

- I executor, heads, pure layers (semen, extentum, unica):
  798 / 769, 838 / 711, 832 / 716 — Seneca −12 words primary
- II all relations, pure layers: 798 / 769, 839 / 711, 832 / 716
- III heads, any layer: 795 / 775, 839 / 718, 833 / 727 — Seneca −46
- IV as I, distant bindings only: as I
- pattern, hard: 797 / 782, 841 / 723, 834 / 729 — Seneca −26 words,
  forced +17 / +13 / +18, charter primary +58 / +55 words
- pattern, soft (in-clause copies at stage 3, the unrestricted rules
  as a stage 4 fallback): 799 / 764, 837 / 707, 830 / 707 — worse
  than the base on the charters and on every forced number
- V as I but neither clause a verb-split coordinate: 798 / 765,
  838 / 711, 832 / 714 — Seneca −10, so the split boundary is not
  where the wrong refusals come from
- VI the CONTROL without the box: refuse a head binding whenever a
  certain seed word lies between the two words, stamps ignored:
  797 / 772, 841 / 726, 834 / 731

What the soft variant says is the important thing: preferring the
in-clause candidate gains nothing, because the loose rule's nearest
candidate is already inside the box nearly always. The whole gain of
the hard variant is the refusal — on Seneca it removes 787 forced
decisions of which 558 were right (709 permille, below the forced
average but far above zero), and that is exactly the 26-word primary
loss: the removed bindings were less reliable than the kept ones and
still better than the default reading. The box is a real signal and
not a certain one; eliminating over an uncertain cell costs, which
is decision 37 measured rather than argued. Every variant depresses
Seneca primary at the gate's resolution, so under the rule that no
treebank may fall none is kept: the predicate stays in the executor
at zero with the numbers, the pattern file is unchanged, and the
forced gains (up to +18 permille) are recorded as the price the
Seneca pin is holding. The way to earn them is a purer stamp, not a
softer rule.

And the control is the finding of the day: refusing across a seed
word, with no clause structure at all, reproduces the hard pattern
law almost exactly — the charters' primary to the word (841 / 834),
the forced numbers within a few permille, Seneca the same loss. The
box's only information beyond the seeds is where a clause CLOSES,
and closure is the weakest layer, so for this law the box is the
seeds. Law 3 as designed (`cursus="clausulae"` on the loose heads)
is not built; the predicate and the control stay in the executor at
zero, and the bar for every later clause law is now explicit: it
must beat the seed-between control, or the clause is not earning
its machinery there. Laws 1, 2 and 4 use membership (which verb is
inside the box with which nominative), which the control cannot
express; that is where the box has to prove itself next.

## 2026-09-08 — T21: the enclitic split

The residual census after T20c I put the largest single loss outside
resolution altogether: 302 Seneca words sat in multiword ranges and
counted as wrong by construction, and every one of them was the
enclitic `-que` (265 tokens), `-ne` or `-ve`. The dictionary already
knew: contract III of the Latin vocabularium takes the tackon path
only when the whole form has no reading and the form minus the
tackon has one, and puts the TACKON analysis first. The parser kept
`armaque` as one element, so the cell held two words.

The split is a pass inside `oratio_partes_annotare`, before the
host is annotated: a vocabulum with one part whose first dictionary
analysis is a TACKON, and which Moby does not know whole (`unique`,
`mosque`), becomes two elements. `oratio_lexema_pars` slices the
token with exact byte offset and column, so emission equals the
source and the oracle's extents stay honest; the host's trailing
space list moves to the enclitic and the host keeps an empty one;
the enclitic gets its one TACKON reading (coordinator for `que` and
`ve`, particle for `ne`) with class and language summaries; the
host is annotated afterwards by its own form. The sentence's
element list is rebuilt through `materia_nodus_reponere` with the
enclitics inserted after their hosts, and the parent fix at the end
of the pass covers the new nodes. `quoque`, `neque`, `itaque`,
`usque` and every other whole-word entry never split, exactly as
in WORDS.

The oracle's one change is not forgiveness: a component of a
multiword range whose form equals exactly one of our elements
inside the range's span is judged against that element as a first
word; otherwise the old path (first component against all touching
elements, the rest as `ranga`) still runs, which is what EWT's
contractions get. Each gold token is judged against one cell.

Two things the numbers taught before the pins moved. First, my
helper returned an unsigned index with a negative sentinel; the
examen's house notes flagged the always-true comparison before the
suite did, and the first run's oracle numbers were garbage until the
return became `s32`. Second, the charters' forced accuracy fell by
four permille with the split on, and the census of actual splits
found the cause in one word: `Lucane`, the medieval spelling of
`Lucanae` ("of Lucca"), split 67 times into the name `Luca` and a
bogus particle. A guard against `-ne` on hosts with only proper-noun
readings did nothing, because `luca` is also the common noun for an
elephant. The honest fix was the dictionary: `lucanus` is a glossary
entry now, with the ae > e forms, and `Lucane` reads as the
adjective. `pleve` (`plebe`) still splits into `ple` + `ve` once;
`utraque` splits into `utra` + `que` four times because PACK tackons
are T8b; both named, neither worth a rule.

Numbers on Seneca: primary 799 → 834 permille, ranges 302 → 31,
single-reading words 2224 → 2549 (the `que` cells, 91 % right),
concordance 102551 of 125027 pairs → 109193 of 132303 (the enclitic
words now count). Charters primary 838 / 831 → 840 / 833
once `Lucane` read as the adjective, forced 710 / 711 unchanged, EWT
unchanged everywhere (one English word, `communique`, splits; Moby
lacks it). Pythonica: Hilarius 1678 → 1682 words.

The forced tier on Seneca lost five words, and the author table says
exactly where: the strict adjacent head rules (`-proximo`) decided
fewer words (338 → 319, 137 → 125) and the loose rules took them at
their lower accuracy, because the enclitic element now stands
between its host and the next word. `senatusque populus` was one
adjacent pair and is now `senatus`, `que`, `populus`. The strict
cursor is the pattern machine's, so making it transparent to an
enclitic coordinator is a substrate question, named as the next
thing to measure and not done here. The Seneca forced pin moves
down by one with that cause; the charters' forced pins hold.

## 2026-09-08 — T20c law 1: refuted by census before it was built

The verb hidden single was measured as a census, not as code: for
each Latin treebank, our words with their readings, finiteness and
clause stamp, aligned to the gold. Three numbers settle it. The
reachable population is 68 gold finite verbs on Seneca (of 1791) and
25 and 24 on the charters — the finite verbs that carry a verb
reading which is not first. The sentence-level single (a sentence
with exactly one finite-capable word) gains 3 and loses 8 on Seneca,
1 and 2 on the dev charters, 0 and 6 on the test charters: the one
capable word in such a sentence is usually a noun with a verb
homograph, and the real ambiguous verbs sit in sentences with two or
more capable words (693 of 893). The clause-level single is worse,
−49 / −53 / −74 net, and its losses are nouns, determiners,
pronouns and non-finite verbs — the clause holds no finite verb, or
its verb is already certain (791 clauses on Seneca, where the law
is vacuous). The constraint "every clause has one finite verb" is not
true of these texts as the treebanks annotate them, and where it is
true the verb is already known. Law 1 is not built.

The same census showed where the verb errors are: on Seneca, 253
gold verbs read as something else, of which 152 are PARTICIPLES read
as adjectives (107) or nouns (40), mostly by the head rules choosing
the adjective twin that agrees; 82 finite, of which 48 open with the
default order wrong; 18 infinitives. On the charter test file 597,
of which 409 are UNKNOWN medieval forms (`pertenentes`, `pegiorata`,
`adinpleta`), 162 participles. So the participle-first prior was
measured next, as a census: participle twin before adjective and
noun of the same form. It loses everywhere — Seneca +154 / −269, the
charters +161 / −984 — and the losses are lexical: `sancti`, `casa`,
`censum`, `mense`, `missus`, `suprascripta` all carry a spurious
participle twin. A per-lemma exception list would be the treebank
fitted back into the rule; not done.

What the charter breakdown pointed at instead is the third census:
of the charters' 3145 unknown word occurrences (449 forms, names and
numbers excluded), a short list of medieval orthographic
correspondences recovers 1869, with the gold class among the
recovered readings in 1697 and first in about the same number:
`ecclesie` → `ecclesiae` (340), `pertenentes` → `pertinentes` (118),
`soledos` → `solidos`, `seo` → `seu`, `comutationem` →
`commutationem`, `abere` → `habere`, `hec` → `haec`, `adinpleta` →
`adimpleta`. Per correspondence, precision (gold class covered /
recovered): e→ae 573/581, e→i 436/437, h added 220/229, single
consonant doubled 146/149, b→p 104/104, inp→imp 97/97, o→u 99/108,
d→t 37/38, i→e 52/59, p→b 40/48; poor: t→d 5/140 (`petia` is not
`pedia`), h removed 86/159, u→o 113/122 covered but 31 first. That
is the charters' enclitic: a lookup-time fallback in the dictionary,
each correspondence measured alone on all five treebanks, kept only
where no treebank falls. Scratch: `verbum_unicum.py`,
`participium.py`, `orthographia.py`.

## 2026-09-08 — T22: medieval orthography as the dictionary's fourth contract

The residual census after T21 put the charters' largest loss in the
dictionary again: 409 of their 597 misread verbs, and 3145 word
occurrences in all, are medieval spellings WORDS does not know —
`ecclesie` for `ecclesiae`, `abere` for `habere`, `pertenentes`,
`adinpleta`, `comutationem`, `soledos`, `seo`. The correspondences
behind them are few and known (e for ae, e for i, h dropped, single
for double consonants, b for p, inp for imp, o for u), so the fix is
a lookup-time fallback, and the census measured each correspondence
before any code: how many unknown occurrences it recovers and
whether the gold class is among the recovered readings.

Contract IV of the Latin vocabularium
(`oratio_vocabularium_la_quaerere_variantes`) generates variant forms
of the folded form under a data table `ORATIO_ORTHOGRAPHIA` — one
correspondence at a time in table order, one occurrence at a time
left to right — runs the full lookup on each (tackons included), and
the first variant with readings wins; its analyses carry `varians`,
and the describer marks them with the new source `orthographia` so
the oracle, the instruments and the lint see them. The dictionary
never varies on its own: the annotator asks for variants only when
the form itself is unknown and Moby does not know it whole (the T21
guard), so the identifier lint, which judges forms as written, is
untouched. `ORTHOGRAPHIA_CAPITALIA` lets capitalised forms vary too
(`Ecclesie`), measured; `ORTHOGRAPHIA_ACTIVA` is the base switch, and
`ORATIO_ORTHOGRAPHIA_SOLA` / `_SINE` in the environment isolate one
correspondence for the loop.

Two things the census decided before the loop. The order: by class
precision `e-i` comes before the h-prefix and turns `abere` into
`abire` (verb, class right, lemma wrong); a second census comparing
lemmas put the h-prefix first and raised right lemmas among the
recovered from 1207 to 1307 of 1504. And one correspondence died on
lemmas alone: h inserted after c, p, t recovers 17 with 11 right
classes and no right lemma (`tecum` → `thecum`, `cun` → `chun`).
Refused from the first census: t→d (5 of 140, `petia` is not
`pedia`), h removed (86 of 159), u→o (97 covered, 31 first), ci→ti,
double→single. `ORTHOGRAPHIA_SOLA=nulla` reproduces the T21 base.

The loop, each correspondence alone against the T21 base (primary /
forced permille; Seneca 834 / 764, charters 840 / 710 and 833 / 711,
EWT 786 / 913 and 789 / 913, never moved by any Latin rule except as
noted):

- h-prefix: 834 / 765, 845 / 711, 837 / 712 — kept
- e→ae: 834 / 764, 852 / 714, 845 / 715 — kept, the big one
- inp→imp: 842 / 711, 835 / 712 — kept
- b→p: 842 / 710, 835 / 712 — kept
- e→i alone: 847 / 703, 840 / 704 — the forced floor falls by seven:
  its recovered readings enter bindings and are wrong three times in
  four; but measured AFTER the h-prefix and e→ae, which take its
  words first, the four plus e→i give 863 / 712 and 856 / 713 with
  every floor held, and it is kept in that position
- doubled consonant: 844 / 712, 835 / 712 but EWT dev forced 912 —
  one English unknown gets a doubled Latin reading; refused
- d→t: EWT forced 911 / 912 — refused; o→u: charter dev forced 709 —
  refused; i→e: 708 — refused; p→b: EWT dev 912, charter dev 709 —
  refused; inb→imb: changes nothing — removed
- all eleven together: charters 868 / 859 but Seneca 833, EWT dev
  785, forced 909 / 912 — the refused ones drag

Two Seneca words fell with the five survivors and the truncation put
834 at 833: `mecum` (five times, e→i to `micum` = mica) and `temet`
(three times, to `timet`), pronouns with `-cum` and `-met` attached
that WORDS reads only through pronoun-base tackons (T8b), plus three
Greek names (`Erebo` → `hereo`, `Pelei` → `pileus`, `Polybo` →
`polypus`). The pronouns are glossary entries now (lemma ego, tu,
sui, nos, vos, qui with their attached forms; the gold splits `mecum`
into `me` + `cum`, so the split is the real fix when T8b lands), and
capitalised forms are excluded from variation: names stay names by
the capital rule, and the charters lose three words for it. Final,
five correspondences without capitals: Seneca 835 / 764, charters
863 / 712 and 856 / 713, EWT unchanged; charter unknowns 1622 → 1009
and 1664 → 1061.

One more thing the gates taught: a glossary entry with `lemma="ego"`
makes `ego` itself a first-source form, so `ego` lost its case; the
attached forms are entries with lemma equal to form now, and the
gold lemma sits in the note. Pins from the gate: tecta 941 / 922 /
913, primary 835 / 863 / 856, forced 764 / 712 / 713, concordance
109193 / 439845 / 445916 (recovered verbs change seeds and certain
verbs), EWT all unchanged. Planted fault: e→ae switched off turns
`ecclesie` unknown and the hand case red.

Named, not done: `manifestu` and other truncations, `petia`
(`pecia`) and the rest of the unrecovered residual (`presbitero`,
`subcessoribus`, `livellario`, `pegiorata`: two changes at once, or
words WORDS lacks outright), which a glossary of charter vocabulary
would take; a lemma-spelling normalisation (`uester`/`vester`) that
only the census noticed. Scratch: `orthographia.py`,
`orthographia2.py`, `mensura_orthographiae.sh`.

## 2026-09-08 — T23: the case column, or what primary accuracy never saw

Fran asked whether the task is choosing the right reading for an
ambiguous word, and the honest answer exposed a gap in the measure:
primary accuracy asks only whether the first reading's CLASS is the
treebank's. A noun read as a noun in the wrong case counts as right,
and for Latin the case is the hard half of the ambiguity. Law 4 (the
subject) would change nothing but cases, so the measure had to exist
before the law.

The oracle now carries a CASUS measure. Population: gold words that
bear a `Case=` feature, whose class we got right, and whose first
reading is Latin and has a case slot. Right: the first reading's case
equals the gold's. It is conditional on the class on purpose, so it
measures case disambiguation alone and does not move when class
accuracy moves. It is counted globally, per gold class, and per
deciding rule (the author table), printed in the human tables and
appended as two columns to the `SUMMA`, class and `AUCTOR` machine
rows, and pinned per Latin treebank, only rising; EWT is reported
only, since English case lives on a few pronouns and our first
readings there are rarely Latin.

The first reading of the number is the finding. Seneca: class right
83.5 %, and among those words with a gold case the case is right
60.9 % (3312 of 5433). The charters: 56.5 % and 56.7 % (5176 of 9156,
5324 of 9387). Per class on Seneca, nouns 58.6 % of 2941, adjectives
57.2 % of 1180, pronouns 69.3 %, determiners 69.0 %, participles
72.2 %. Per rule, the two big nominative head rules choose the right
case 52.0 % (604 words) and 45.0 % (353) of the time: agreement with
a neighbouring noun picks the CLASS well and the CASE barely better
than chance among the readings that agree. That is the number law 4
has to move, and the number that says the Latin problem is case, not
part of speech. Pins 609 / 565 / 567, EWT reported at 0 of 0.

Two small things from writing it. `casus` is a latina.h macro (it
is `case`), so the parameter is `casus_iudicatus`; and the examen
refused an anonymous `struct` table inside a function, which is
plain `si` chains now. The pythonica face reads the machine rows by
position, and the new columns are appended, so nothing there moved.

## 2026-09-08 — T20c law 2: `cum` the conjunction, refuted by census, and the seed rule it pointed at

Law 2 promised to let the clause stamp's decision speak: a `cum` that
seeded a clause gets its conjunction reading first. The census put
our seed decision against the gold class for every `cum` on the
three Latin files. Seneca: 26 aligned, seeded and conjunction 17,
seeded and preposition 7, unseeded preposition 2 — the law would
gain 17 and lose 7. The charters: 106 and 124 `cum`, all but four of
them prepositions, and the stamper seeds 12 and 29 of those
prepositions (`capo uno cum uno lato tenet`, `casa cum curticella et
orto`): the law loses 11 and 27. The box-free control, a finite-
capable word anywhere after `cum`, gains 17 and loses 8 on Seneca and
loses 103 and 116 on the charters, so the seed decision is worth a
great deal over the control on the charters and one word on Seneca —
just not enough to be promoted into a reading.

The census pointed at the seed rule instead: the false seeds have an
ablative-only word after `cum`. A switch `CUM_ABLATIVO_SOLO` refuses
the seed when the next word can be ablative, cannot be nominative and
is not a finite verb. It cuts the false seeds to 3 / 15 and 3, at the
cost of four Seneca conjunctions whose next word is such an ablative,
and measured by exact concordance pairs it gains 157 on Seneca and
1021 on the test charters but loses 312 on the dev charters. Under
the rule it is refused; the code keeps the switch at off with the
numbers. On that refined stamp law 2 would still lose on the
charters (+10, −2, −14). Law 2 is not built. Scratch: `cum_census.py`.

## 2026-09-08 — T20c law 4: the subject, refuted by census; and the case prior the matrix found

Law 4 was censused as a hidden single: in a clause (or sentence)
with exactly one certain finite verb, exactly one nominative-capable
member agreeing with it in number becomes the subject and takes its
nominative reading first. Reach first: gold nominative subjects
number 882 on Seneca, 699 and 710 on the charters; 626 / 378 / 372
already carry the right case, and only 165 / 44 / 54 have a
nominative reading that is not first — the charters' subjects mostly
lack the nominative reading altogether (medieval forms). The single
itself: sentence level +24 / −11 on Seneca, −1 and −3 on the
charters; clause level +65 / −64, −55, −56. Its losses are objects
whose accusative was right: the one agreeing nominative-capable word
is the object as often as the subject, because nominative and
accusative are homographs for neuters and plurals. Not built.

The case column's confusion matrix says where the wrong cases are.
Seneca, 5002 words with class right and a gold case, 1945 wrong:
accusative read as nominative 574, ablative as dative 367 and as
locative 134, genitive as nominative 160, nominative as accusative
121. Charters (test): accusative as nominative 857, ablative as
dative 611 and as accusative 392, genitive as nominative 337 and as
VOCATIVE 197. The gold has one locative on Seneca and a handful of
vocatives on the charters; our first readings sit there 218 and 197
times. Most of these are open-tier defaults (799 of 1945 on Seneca),
the rest bindings that picked the first agreeing pair in dictionary
order. A third of the charter errors (1297) are unreachable: no
reading of the gold case exists (`casa` for `casam`).

So the census of a CASE PRIOR BY DATA: for each candidate set (the
cases the readings of the first class offer), the gold's majority.
Today 61.1 / 56.7 / 56.9 %; the best fixed choice per set 73.4 /
73.8 / 76.4 %; one global order 68.6 / 71.7 / 72.9 %. The gain is
in both tiers, open and bound, since the head rules bind the first
agreeing pair and the order decides which. The frequent sets agree
across the three files (accusative before nominative in acc/nom/voc,
ablative before dative, ablative before locative, accusative first
in acc/gen/nom/voc); the one that disagrees is abl/acc/nom/voc. That
is the next tranche, a prior that orders and never eliminates.
Scratch: `subiectum_census.py`, `casus_confusio.py`, `casus_prior.py`.

## 2026-09-08 — T24: the case prior, and where in the pipeline a prior may stand

The case column's confusion matrix said two fifths of Seneca's wrong
cases were words no rule touched, standing in the dictionary's
inflection-table order: nominative before accusative, dative before
ablative, the locative wherever the table lists it. The census of a
CASE PRIOR (majority gold case per candidate set) promised 73 % from
61. This is the prior, and the lesson is where it must stand.

The first placement was the obvious one: before all rules, as the
default order of every word's candidates, so the open tier gets it
and the head rules walk it. Class accuracy collapsed: Seneca primary
835 → 808, the charters 863 / 856 → 822 / 811, for a case gain of a
few points. With ablative first on every nominal, the loose head
rules find spurious ablative agreement between neighbours, bind the
wrong pair, and the law of the explicit class promotes the wrong
twin. A prior that changes what the evidence rules see is not a
prior. So it runs LAST, after all stages, decisions and the chain,
and only on words nothing touched: not decided, not carrying a
filled umbra, not the target of one (binding indices point into the
target's list, and a permutation would break them). Primary and
forced are then identical to the base on every file, and only case
moves — which is what a prior may do.

Two defects on the way, both caught by house tools. The examen's
note "comparatio vana: semper falsum" was my group index: an
unsigned array initialised to minus one, so "not yet assigned" was
never true, every reading fell into one group, and the sort ran
across classes by case rank — nouns ahead of verbs, 207 Seneca
words changing class, and, by accident, 70 of them for the better.
And the first correct version consolidated a class's readings
(`Cum`'s two preposition readings pulled together past its
conjunction ones), which the gate's count exposed: the prior now
reorders only the readings of the FIRST class, each into the
positions that class already held, and everything else stays put.

Measured after the rules (case permille, Seneca / dev / test; primary
and forced unchanged in every row): base 609 / 565 / 567; global
order abl > acc > gen > nom > dat > voc > loc 636 / 617 / 630, kept;
entry abl/acc/nom/voc → nominative 645 / 622 / 637, kept; entry
abl/nom/voc → nominative 640 / 617 / 629, the test charters fall by
four words at the boundary, refused; entry abl/acc → accusative
648 / 596 / 606, the charters fall hard, refused. The census's 73 %
was the ceiling over ALL words; the prior reaches the open ones, and
the bound ones keep the order their rule found — the rest of the
distance is the head rules' 52 % / 45 % case choice, which is a
rule question, not a prior. The gate's older hand cases that said
"no rule fired, so the nominative stands" now say "no rule fired, so
the prior's ablative stands", with the rule count still zero beside
them. Scratch: `mensura_prior.sh`.

## 2026-09-08 — The treebank shelf: four Latin treebanks no rule has seen

Fran asked what we could say about Latin from elsewhere. The spec's
licence table from the fourth already settled the policy: share-alike
treebanks are vendored and pinned, the non-commercial ones (Aquinas,
PROIEL, Perseus, UDante) are fetched into `oratio/build/ud/` by
`oraculum.sh -petere`, reported and never committed. The build tree is
git-ignored, so the files never enter the repository. Today's numbers
on their test splits, with every rule and prior as committed at
76bfe4f8 and nothing tuned on them:

- Perseus (Cicero, Vergil, Ovid, Sallust, Jerome): 10,964 words,
  coverage 949, primary 862, lemma 742, case 722, forced 749
- ITTB (Aquinas): 29,842 words, coverage 914, primary 824, lemma 757,
  case 707, forced 707, 512 unknown
- PROIEL (Vulgate, Caesar, Cicero, Palladius): 14,091 words, coverage
  934, primary 790, lemma 915, case 723, forced 690
- UDante (Dante's Latin): 13,474 words, coverage 859, primary 780,
  lemma 693, case 676, forced 626, 749 unknown

Against the tuned files (Seneca 835, charters 863 / 856; case 645 /
622 / 637) that is the same band, above it on Perseus, and case is
higher on every unseen file than on verse and charters. So nothing
this session fitted the three files it was measured on, and the
calibrated expectation for other Latin is class between 78 and 86,
case around 70, coverage 86 to 95. The lemma numbers differ by
convention as much as by us (ITTB and UDante lemmatise differently
from WORDS). These are reported figures; the shelf is a calibration
instrument and a source of errata, not a gate.

## 2026-09-08 — T25: the pair policy, a null result on readings, and the measure it needs

The head rules take the first admissible row of a rule in pattern
order. The sibling cursor yields one row per start, the start's first
match, so for the preceding-partner rules a carrier with two agreeing
nouns before it gets two rows, farthest first, and the farthest wins:
the T19g bis finding. The policy reorders each rule's rows within
groups of the same carrier and umbra, nearest partner first, then the
partner reading's case rank, then the original order, a stable sort
before the first-wins loop. It works: `Terra puella non magna est`
binds `magna` to `puella` with it and to `Terra` without.

Measured on nine treebanks, the five vendored and the four on the
shelf, at none, distance, and distance then case rank: primary,
forced and case identical to the word on every file. The per-rule
rows explain why. The policy moved between one and four remote
partners per rule on Seneca, and in every such case both candidates
agreed with the same reading of the carrier, so the promoted reading
did not change. The remote head rules' 33 to 70 percent is not a
wrong choice among partners; it is a single agreeing partner that
agrees with the wrong reading, and no ordering of partners reaches
that. The case-rank part had nothing to rank for the same reason.

The policy stays on: it costs nothing, nearest partner is the right
attachment more often than farthest, and the attachment is what the
search index and the dependency design will consume. But its value
is unmeasured until the oracle measures attachment, and the treebanks
carry the gold heads for it: for a head umbra, is the partner the
gold head of the carrier; for an object umbra, is the carrier the
gold head of the partner. That column is the next oracle addition,
and until it exists the policy is a principled default, not a
measured gain. Switch `POLITICA_PARIUM`, scratch `mensura_politica.sh`.

## 2026-09-08 — T26: the attachment column, and how much of the tree we actually assert

T25 changed partners without changing readings, and nothing we
measured could see it. The bindings have always been in the tree,
the umbra of a reading naming its partner word and reading; the
oracle only ever asked whether the first reading's class and case
matched the gold. The treebanks carry a head for every word, so the
question was answerable all along.

The measure. Every element remembers the gold token it aligned to.
After alignment, a pass walks each aligned element's FIRST reading
and its filled umbrae with an aligned partner. For a head umbra the
binding says "my head is the partner", right when the gold head of
the carrier is the partner. For an object umbra the direction
depends on the carrier: a preposition, an English particle or an
auxiliary hangs from its object in Universal Dependencies (case,
mark, aux), so the binding is right when the carrier's gold head is
the partner; a verb's object hangs from the verb, so it is right when
the partner's gold head is the carrier. My first cut had prepositions
the other way round and every one of them counted wrong, which is
why the first Seneca number was 35 percent. Precision is right over
asserted, recall is right over the gold arcs among aligned non-
punctuation words, and both are split by head umbrae and by adjacent
partners. Human line `LIGATIO`, machine row `LIGATIO`, precision
pinned per file only rising, recall reported.

Numbers (precision permille, recall permille): Seneca 431 of 2345
bindings, recall 95 of 10565 gold arcs; charters 364 and 377, recall
95 and 98; EWT 815 and 826 on few hundred bindings, recall 19 and 22.
Adjacent bindings 586 on Seneca and 730 on the test charters; head
umbrae 408 and 363. Pins 431 / 364 / 377 / 815 / 826.

Two readings of the numbers. Adjacent bindings are decent, remote
ones are poor, which is the T19g finding again but now about the
arc and not the reading, and it is the number the pair policy and
any remote-rule variant will be judged on. And recall is under ten
percent: we assert about a fifth of the gold arcs and get half of
those right. The subject, the verb's object, the adverbial, the
coordination are arcs we never draw. That is the honest distance
between the present resolver and the dependency structure of the
roadmap's second step, measured before that structure is designed.
Named, not done: attachment per deciding rule (the author table),
which needs the umbra to carry its author.

## 2026-09-08 — T27: the retry door measured; first reading per case is a prior

Fran's question was whether adding tracked features (number, gender,
person, mood) would multiply the hand-written rules. Reading the rule
file again: number and gender are already captures, unified by name
through regula V; only CASE is literal, and only because the matcher
never retried a capture bound on the carrier's first reading. So the
multiplication I feared (84 rules per motif) was wrong — the literal
dimension is one, seven values, and it costs 28 head rules where 4
would do and 2 object rules where 1 would. New motifs (verb with a
nominative subject, an accusative object) each need one or two
literal cases, so growth is already linear in the features; the
collapse is worth 28 → 4, not 84 → 2.

The door was opened in the macro engine as a complete leftmost-first
search (spec decree 21) and measured here with every rule as written.
Primary and forced identical on all five files; case 645/622/637 →
641/619/635; attachment precision 431/364/377 → 438/359/374 (Seneca
up 7, charters down 5 and 3); the law-of-umbrae fallback grew on
every Latin file (48 → 53, 143 → 164, 133 → 158). The author table
shows the trade: `umbra-caput-nominativus-sequente` 798 → 823
decisions, the strict one 319 → 361, `umbra-caput-accusativus-
sequente-proximo` 125 → 87 at lower accuracy (792 → 735). Retry
lets the nominative rule, first in order, claim a carrier's rarer
nominative readings (bona NOM P N) before the accusative rule sees
its first accusative reading (bona ACC P N), and the dictionary's
first reading per case is right more often than the rarer one that
happens to agree. That prior was implicit in greedy matching plus
rule order; a unified rule enumerating the carrier's readings in
dictionary order would lose it the same way.

Decision: the search is an option per EXEMPLAR (`quaesitio=
"completa"`), the default stays greedy, no rule here changed, every
pin identical to the word. Next (tranche 2): write the unified head
and object rules WITH the attribute beside the literal ones and
measure them on the five files; if the loss reappears, the fix is an
enumeration order that puts each case's first reading before any
case's second — a policy to name and measure, not a return to
twenty-eight rules. Scratch: `aurum_census.py`, `verbum_caput.py`
(the gold census behind the morning's answer), `ante.log`/`post.log`
author-table diff.

## 2026-09-08 — T27 b: the strict head tier unified; the retry finding overturned

The one-hour measurement from the morning's plan was run, and it did
not confirm the morning's reading. Nine treebanks (the five pinned,
the four on the shelf), seven variants, each a swap of the rule file
and one gate run plus the shelf:

| variant | outcome |
|---|---|
| all 30 literal rules → 5 unified, first completion | case up everywhere; the object rule alone loses 50–150 bindings per file (refused) |
| head unified, object literal | case +22/+12/+34, charter attachment −8/−11 |
| head unified, every completion, nearest first | Seneca case 698 (+53), charters −17 attachment, Aquinas −19 case |
| loose tier only, every completion | carries all of the attachment and Aquinas loss |
| STRICT tier only, every completion | up or equal on every column of every file, except forced −1 on both charters |
| strict tier, first completion only | worse than every-completion on every charter column |
| refuse a row whose carrier holds another reading | primary and forced down on every Latin file (refused) |
| distance-only pair policy (I) under the adopted rules | worse than distance-then-case (II) on every file |

Adopted, with Fran's decision on the forced pins: the fourteen strict
head rules become two, `$cas` captured on both nodes like number and
gender always were, `quaesitio="omnes"` so that every agreeing pair
is a row, and the T25 pair policy picks the nearest. Fifty-four rules
become forty-two. Pins: case 645/622/637 → 662/644/665, attachment
431/364/377 → 437/368/378, forced 764/712/713 → 765/711/712 — the
two charter pins move DOWN with the named cause below; on the shelf
case rises +21/+3/+9/+10 and attachment +5..+8, primary and EWT are
identical.

The named cause. The strict family itself gains right decisions on
all three files (Seneca 717 → 740, dev charters 950 → 968). The dip is
the law-of-umbrae fallback tier growing (143 → 160 words on dev, at
lower accuracy): with every completion a carrier's LOSING reading
still gets its umbra filled and still decides its partner, and the
fallback then promotes some of those. Refusing such rows outright
was the seventh variant and it is far worse — those bindings carry
real information even when the carrier's own preference lies
elsewhere. In absolute terms the forced tier gains three right words
on dev and loses three on test.

What this overturns. T27 a concluded that "the dictionary's first
reading per case is an informative prior". It is not, or not in the
way stated: dictionary order with every completion beats rule order
on case by twenty to thirty permille on the adjacent tier. What
T27 a had actually measured was nominative-FIRST rule order plus
retry — retry let the first rule claim rarer readings before later
rules saw the word at all. Two other laws came out of the isolation
runs. The loose tier must stay literal for now: "the nearest agreeing
word anywhere in the sentence" is not the head often enough, and
every-completion there costs the charters 17 permille of attachment.
And the T25 pair policy's second key, the partner reading's case rank
(ORDO_CASUUM, ablative first), which was a null result on readings
when it was built, is now real work: distance-only ordering under the
adopted rules loses 7/15/10 permille of case on the pinned files and
22 on Perseus. That key is also the reason `Puella bona ambulat`, the
resolver's oldest hand sentence, now reads ablative — among three
equal-distance agreeing pairs the ablative one sorts first. The bet
wins on nine files and loses on that sentence; the pythonica hand
case moved to `Puerum bonum videt`, which has one agreeing pair, and
the miss is recorded.

Method. Every variant was a swap of the rule file from a scratch
generator (`unita.py`), one gate run, four shelf runs, `git checkout`
of the rule file; the family sums over the author tables (scratch
`post_*.log`) said where each loss lived before any code was written.
Two engine changes were needed on the way and both are general:
`quaesitio="omnes"` in the macro engine, and nothing in oratio's C —
the policy-level-III grouping I wrote turned out redundant (a head
umbra is always index 0, so the (carrier, umbra) group already holds
all of a carrier's readings) and was dropped before commit.

## 2026-09-09 — T27 b, second half: stale binding indices, the emendation law, the pair order

Three things surfaced while wiring the adopted rules into the gates.

**Two discordant bindings on Hilarius.** The resolutio probatio checks
that every bound umbra points at a reading of its own case, and two
did not: the carrier's preferred reading said ablative or accusative,
the partner's reading 0 was nominative. Traced with a temporary print
in the checker: both partners were class-explicit words (determiners
under the T19i prior) that the strict tier had emended at stage 2 and
a loose literal rule re-emended at stage 3 — the "once" in the T19i
emendation law was once per STAGE — and the stage-3 permutation moved
the partner's readings under a stage-2 binding that nobody remitted.
Two fixes. `_ligationes_remittere`: whenever a stage permutes a
word's readings, every umbra in the sentence that points at that
word is remapped through the inverse permutation (first written with
the write-once setter, which refused — the re-put form is the one the
law of umbrae's own remit uses). With the emendation law unchanged
this changes NOTHING on nine treebanks and discordes is 0: a pure
correctness fix. `EMENDATIO_SEMEL_PER_SENTENTIAM`: emend a
class-explicit word once per sentence instead of once per stage —
that is the "first wins across stages" law applied to emendation.
Measured: attachment 437/368/378 → 441/386/394, shelf attachment
+10 to +12 on all four, case up on eight files (charters +10/+6,
shelf +4 to +7), Seneca case 662 → 660 (nine words). Left OFF with
the numbers in its comment; it is the next pin decision.

**The pair order.** The T25 policy's second key is the partner
reading's case rank, borrowed from the T24 prior (ablative first).
Under the unified strict rules it is real work: distance-only
ordering loses 7/15/10 permille of case, and a nominative-first order
(either nom-acc-abl or nom-abl-acc) loses 6/10/19 on the pinned files
and up to 14 on Perseus. So among equal-distance agreeing pairs the
ablative and the accusative win over the nominative — which is why
`Puella bona ambulat` reads ablative and `Hoc templum est` reads
accusative now. The table is now `ORDO_CASUUM_PARIUM`, identical to
the prior's but measured on its own; the four hand cases that
asserted nominative for those sentences say ablative and accusative
with the reason, and the pythonica case moved to `Puerum bonum
videt`, which has one agreeing pair.

**Rule counts.** 54 → 42; the resolutio probatio's stage boundaries
moved from 22/38 to 22/26.

## 2026-09-09 — T27 b closed: emendation once per sentence adopted

Fran's decision: `EMENDATIO_SEMEL_PER_SENTENTIAM` on, the Seneca case
pin 662 → 660 with the cause named (nine words; the law "first wins
across stages" now covers the T19i emendation as well). Pins: case
660/654/671, attachment 441/386/394, forced unchanged 765/711/712.
Shelf: case 751/715/738/693, attachment 531/529/492/354. Suite 18/18,
pythonica green, discordes 0. Two pin moves down in two days, each
named, each against a column rising on eight of nine files; the
rule stays that a pin never moves down silently.

## 2026-09-09 — T28: the loose tier's law, censused; the clause refusal re-judged on arcs

The tranche set out to find a better law for the remote head rules
and ended by adopting a law that already existed, once it was judged
on the right column.

**The census.** For every adjective and determiner in the seven Latin
treebanks (`caput_census.py`, scratch): with the gold's own features,
the NEAREST agreeing nominal is the head 96–99 % of the time on every
file, verse included. So "nearest agreeing" is the right law; our
remote errors come from spurious agreement between over-generated
readings, not from choosing the wrong candidate. Two more facts: a
verb stands between adjective and noun in 30 % of Seneca's arcs
(nearest-with-no-verb-between drops to 70 % there, against 93–98 %
in prose), so a clause bound inside the MATCH would cost verse; and
the head lies after the adjective 70 % of the time, so "prefer after"
beats "prefer before" by 8 points on Seneca.

**Every generalization of the loose tier loses attachment.** A per-rule
policy attribute (`politica="casus"`) was built and the unified loose
rule with every completion measured under four orderings: case then
distance (nominative-first), the same with the ablative-first table,
both with the carrier's reading index as a second key, and a global
case-major sort across carriers imitating rule order. All four lose
6–15 permille of attachment on the shelf and 7–15 on the charters;
the ablative table swings case +34 on Seneca and −26 on Aquinas. The
literal tier's conservatism — one completion, first reading per case,
rule order — is a reading-quality filter no ordering reproduced. The
loose tier stays literal and is the first family for the §8.5
template door; the policy code was reverted (numbers in this entry,
not in the tree).

**The clause refusal, judged on arcs.** T20c's LEX I refused every
variant on primary alone. With the attachment column, level II
(all relations, pure strata): attachment 441/386/394 → 462/398/412,
EWT dev 815 → 817, shelf +20/+44/+41/+16; forced 765/711/712 →
769/713/717; case 660/654/671 → 666/653/671 (charters dev −1, seven
words, named; Aquinas −2 reported); primary unchanged; recall level,
so the removed arcs were wrong ones. Level I: 458/391/405. Adopted,
Fran's decision. The T20c comment's "every variant depresses Seneca
primary" stands as written — it was the wrong column.

**Lesson.** The right column changes the verdict on a law measured a
day earlier. And a census over gold features tells the ceiling of a
law before any code: here it said the law was fine and the readings
were not, which is where the next work goes (feature columns, then
the verb star).

## 2026-09-09 — T29: the feature columns

The case column (T23) generalised: a table in the oracle maps each
gold feature key to one of our accidents and its values to our
indices — Number, Gender, Person, Mood, Voice, VerbForm by value
lists, Tense by a small function because UD Latin spells the perfect
as Tense=Past plus Aspect=Perf. Same condition as case: class right,
gold feature present, first reading Latin and carrying the accident.
Voice accepts WORDS's deponent for both Act and Pass, which is a
convention and says so in the table. Every element remembers its
first reading's seven accident indices beside its case; the census
carries verba/recti per key; the instrument prints a `NOTA` line and
machine row per key; the gate prints all seven and pins number and
gender only, the others reported until someone works on them.

First numbers (permille, Seneca / charters dev / charters test):
number 914 / 928 / 933; gender 837 / 813 / 810; person 997 / 979 /
977; mood 961 / 992 / 994; voice 928 / 867 / 870; verb form 925 /
840 / 832; tense 931 / 995 / 997. EWT has five and six pronouns with
Number and nothing else, as expected. Reading: the Latin problem
after case is GENDER — a fifth of class-right words with a gold
gender carry the wrong one, which is exactly the spurious agreement
T28's census predicted (an adjective's rarer gender reading agreeing
with a neighbour); on the charters verb form and voice lose a sixth
each, which is the participle-versus-finite and deponent question.
Planted fault: swapping the singular mapping went red at the number
pin. Pins 914/837, 928/813, 933/810, only rising.

## 2026-09-09 — T30 a: the gender census, and two oracle conventions

The gender column (T29) was 837 / 813 / 810 on Seneca and the two
charter files, and the plan was the house one: census first, then a
law. The census needed an instrument, so this tranche is mostly
instrument, and the two things it found were oracle conventions rather
than parser errors.

**Feature errata.** `oraculum.sh -errata -nota genus` (or any accident
title, or `omnes`) lists every class-right word whose first reading
carries the wrong value: form, class, gold and our value, decision and
rule, binding partner and distance, and ATTINGIBILE — whether any
Latin reading of that class carries the gold value at all — or ABSENS.
Machine rows `ERRATUM-NOTA`. The element now remembers a bitmask of
the values its readings could reach. Law in the gate: the numbers of
an accident's errata sum to verba − recti; the planted fault (refuse
to record reachable errors) went red on that line.

**Multi-value gold.** UD Latin writes `Gender=Fem,Masc` on ambiguous
words (2–4 % of Seneca, 12 % of PROIEL's determiners). The substring
reader saw only the alphabetically first value. `_nota_aurea_valores`
parses the field and returns a bitmask, any listed value accepted.
Seneca 837 → 847, the charters unchanged.

**The census** (Seneca 755 wrong; charters dev 1415):
- WORDS common gender `C` against gold Masc/Fem: 32 % / 41 %
  (`dies`, `trux`, `tu`/`vobis`, participles); PROIEL 60 % because
  it marks personal pronouns masculine.
- other unreachable 16 % / 24 %: dictionary holes (`Luca` masculine),
  and known medieval forms — `sancte`, `bone`, `suprascripte`, `ipse`
  — where the vocative reading blocks the `-ae` variant, because T22
  asks for variants only on UNKNOWN forms.
- reachable, untouched nouns 20 % / 5 %: homonym lemma order —
  `armus` before `arma`, `caelus` before `caelum`, `tela` before
  `telum`, `census` before `censum`. The dictionary carries WORDS's
  frequency codes; the readings do not.
- reachable, untouched verbs 7 % / 12 %: gerundive masculine listed
  before the neuter reading (`faciendum`), and UD calls it a gerund.
- reachable, determiner prior 8 % / 11 %: `alio lato`, `uno capo` —
  the partner's rarer masculine reading agrees; the gold has medieval
  neuter nouns the dictionary lacks.
- reachable, head rules 13 % / 6 %: spurious agreement in the loose
  tier, the smallest bucket. The pair policy is not where the gender
  points are.

**The commune convention** (Fran's decision). `commune` is accepted
for gold Masc and Fem, as deponent voice is for Act and Pass: WORDS
says "either", the gold picked one, the parser is not wrong. So it
cannot hide, every feature line now prints `conventione N` — the
words right only by the alternate — in the gate, the instrument and
the machine row (`NOTA` has six columns). Gender: Seneca 847 → 897
(248 by convention), charters 813 → 890 (584), 810 → 885 (588);
shelf 829 → 856, 850 → 901, 805 → 923, 819 → 866. Voice's deponent
count is now visible too: 163 / 48 / 45. Pins 897 / 890 / 885.

**Next, by the census's own ranking:** a frequency prior for homonym
lemmas (needs the code on the reading), the medieval variant on known
forms as a T22 extension, the gerund reading; and a "derived gender"
count, resolving commune through the agreement partner, if we want
the convention earned rather than granted.

**Wart.** `silva.Editio` on a `.py` file runs the C formatter, which
refuses (no fixed point) after the write; the edit lands, the
exception is noise. Worth a `.py` guard in applicare.

## 2026-09-09 — T30 b: the frequency order

The census ranked homonym lemma order first among the reachable
gender errors: WORDS lists `armus` before `arma`, `tela` before
`telum`, `testa` before `testis`, `census` before `censum`, and the
first reading of a case is whatever entry the dictionary happens to
list first. The dictionary carries WORDS's frequency code per stem
(A very frequent … F very rare, X unknown), and the readings did not.

**What was built.** `OratioDescriptio.frequentia` carries the stem's
code (zero for glossary, unique, rule and English readings). Before
nodes are built, `_descriptiones_frequentia_ordinare` stable-sorts the
readings of each (class, language) group among their own positions by
frequency rank. Class order and language order are untouched, so the
first class of every word is what it was and primary cannot move;
nothing is eliminated. Equal ranks keep dictionary order (`caelus` and
`caelum` are both A, so `caelus` still leads). `ORATIO_ORDO_FREQUENTIAE=0`
turns it off for measurement without a rebuild.

**Where it runs, and why before the rules.** T24 taught that a prior
may not change what evidence rules see: the case prior before the
rules wrecked primary because loose head rules bind the first
agreeing pair, and ablative-first found spurious pairs. The frequency
order changes no case, number or gender evidence — the same tuples
are there — only which reading of a case is met first, so the rules
bind the same evidence with the frequent lemma. Bound words are
fixed too (`caelum` under the accusative object rule), which a
last-stage prior on untouched words could not reach.

**Measured, nine treebanks, off → on.** Primary identical on every
file. Gender: Seneca 897 → 907, charters 890 → 895 and 885 → 892,
Aquinas 856 → 862, Perseus 901 → 904, PROIEL 923 → 931, Dante 866 →
869. Number: Seneca 914 → 917, others +0..1. Case: Seneca 666 → 666
(+3 words), charters equal, shelf +2/−1/+3/+3 words. Attachment:
Seneca 462 → 463, charters equal, shelf +3/+1/+1/0 right. Forced
equal. Pins: gender 907/895/892, Seneca number 917, Seneca attachment
463.

**One thing the `conventione` count caught.** On the charters the
words right by convention rose from 584/588 to 744/794 while the
total rose 38/53, so strictly-right fell by about 120/150. The probe
named it: `testes` (the charters' witnesses, hundreds of them) now
reads WORDS's common-gender `testis` entry before its masculine entry
— same lemma, both entries correct, the convention covers it. The
count did what it was built for: it showed that part of a rise was
the convention, and where. `comes` improved as a lemma (was the
genitive of `come`).

**Not done here.** Inflection frequency (the flexion's own code,
which would push locatives back) as a second key; the age code
(medieval entries first on the charters); a derived gender through
the agreement partner for commune words.

## 2026-09-09 — T30 c (T22 b): medieval variants on known forms, gated by dialect

The gender census had a second unreachable bucket after the commune
convention: known medieval forms. `sancte`, `bone`, `suprascripte`,
`ipse` are all in WORDS as vocatives or nominatives, so T22 never
asked for their `-ae` variant — contract IV asks only for UNKNOWN
forms — and the feminine genitive the charters meant was never a
candidate.

**First measurement, no gate.** Appending the variant readings of
every active correspondence to every known form lost primary on all
seven Latin files (Seneca −182 words, the charters −225 / −334):
the readings won spurious agreement, T28's lesson again. One
correspondence at a time on known forms: `h-praefixa` and `e-i` lose
everywhere and are refused; `e-ae` alone gains the charters +2 / +6 /
+9 / +17 / +15 (primary, forced, case, gender, attachment) and Dante,
but costs Seneca −3 / −7 / −5 and PROIEL −10 / −38: classical adverbs
and vocatives in `-e` (`male`, `vere`, `domine`) become feminine
genitives.

**The dialect.** What separates the files is evidence the pipeline
already produces: the share of words T22 recovers as unknown medieval
spellings — charters 29 per thousand, Dante 12, Perseus and PROIEL
under 1, Seneca and Aquinas 0. So a document has a DIALECT, decided
the way its language is (T18): a pre-scan over the document's words
(one lookup per folded form, cached) counts recovered forms, and at
five per thousand the document is `medius`. The oracle decides per
treebank file in the same pass that decides the language and hands
the dialect to every sentence; a document annotated on its own
decides for itself. Known forms vary only in a medieval document.

**Two more filters, both measured.** Only `e-ae` has the `notis`
flag in the correspondence table (the others refused above). And a
variant reading is kept only if its WORDS part of speech is already
among the form's native readings: `que` (the enclitic standalone,
very common in charters as `quae`) otherwise became a pronoun and
determiner with 140 readings and bound the nearest noun on either
side — `ecclesie` went plural 35 times. With the filter the number
column recovers 1 permille and attachment 3 on the test file; Dante
loses 4 permille of primary and gains 5 of case and 7 of attachment.

**Adopted (Fran's decision), against the committed state.** Charters
dev / test: primary 863 → 865 / 857 → 858, forced 713 → 719 / 717 →
728, case 653 → 662 / 671 → 685, gender 895 → 913 / 892 → 912,
attachment 398 → 413 / 412 → 428; number 928 → 925 / 933 → 932, the
one fall, CAUSE NAMED from the errata: the loose tier's literal head
rules run nominative first, so `sancte … ecclesie` at distance two
binds as a plural nominative pair before the genitive rule can, and
both go plural (13 + 13 + 8 words). That is the §8.5 family's known
order problem, to be fixed there. Dante 781 → 780 / 631 → 633 / 696 →
705 / 932 → 935 / 869 → 878 / 370 → 374. Every classical file and EWT
byte-identical. Gates: partes asserts the pre-scan counts, the
threshold on both sides, `bone` carrying `bonae` after its native
Latin readings in a medieval document and `male` carrying nothing in
a classical one; the oracle asserts the dialect per treebank; the
inverted threshold went red.

**Left on the table.** Capitalised known forms (`Sancte` at charter
openings) still do not vary, as in T22; the loose tier's nominative-
first order; a sentence-level dialect for mixed documents.

## 2026-09-09 — T30 d: the gerund reading, which became the rarity of inflections

The census bucket was "gerundive listed before the neuter reading":
`faciendum` is ACC S M first in WORDS, and the charters mean the
neuter every time. Two gold facts reshaped the tranche. Only CIRCSE
and PROIEL tag gerunds and gerundives at all (Ger without gender, Gdv
with); the charters, Aquinas, Perseus and Dante tag every `-nd-` form
as a participle with a gender, neuter 97 % of the time for
`-ndum/-ndi/-ndo`. And the verb-form column's biggest loss was not
the gerund: 332 of the charters' 467 wrong verb forms were
infinitives read as finite — `scribere`, `reddere`, `abere` — because
WORDS lists the second-person passive (`PRES PASSIVE IND 2 S are`)
before the infinitive of the same form, and the voice column lost
the same 312 words as passives.

**Two oracle conventions**, counted under `conventione`: gold `Part`
accepts our `gerundivum` (the four treebanks' convention), and gold
`Ger` accepts it too, since WORDS has no gerund and the neuter
gerundive is the reading we have. Charters verb form 840 → 883.

**The rarity table.** A second key in the reading order after the
stem's frequency (T30 b): a verb reading carrying a RARE inflection
sorts after the others of its stem. Data-driven, titled, each entry
measured alone on nine treebanks with `ORATIO_ORDO_RARITATIS`:
- `gerundivum-masculinum`: gender charters +16 / +18, Aquinas +12,
  Dante +17, PROIEL +3, Seneca +1, nothing else moves. The first
  version demoted every non-neuter gerundive and cost number on
  Seneca and PROIEL through `-nda`, where feminine singular beats
  neuter plural on three files of four; the dictionary order stays.
- `passivum-secundae`: verb form Seneca 930 → 978, charters 883 →
  986 / 989, Aquinas 938 → 983, PROIEL 929 → 982, Dante 913 → 988;
  voice Seneca 930 → 982, charters 871 → 999 / 999, the shelf 988–999.
  The first version also demoted deponents and cost Seneca fifteen
  imperatives (`sequere`, `loquere`, `morere`) to a spurious active
  infinitive, and 25 words of number and person population; a
  deponent's passive form is its form, so they are exempt.
- `participium-vocativus` (`date` as the vocative of `datus`):
  measured, changes nothing on any file; kept inactive in the table.

Primary, forced, case and attachment are identical on every file;
Seneca number 917 stays with three fewer words judged. Pins: gender
908 / 929 / 930, and the verb-form and voice columns are pinned for
the first time, 978 / 986 / 989 and 982 / 999 / 999.

**Lesson.** The order of a dictionary's inflection table is a prior
nobody chose. Two rare inflections listed first cost a sixth of the
verb-form column on the charters; the fix was a table of named
rarities with a census behind each row, not a rule.

## 2026-09-09 — T30 e: the loose tier's order, measured at last

The fourteen literal head rules of stage 3 had run in the order of
the case enumeration since T19d — nominative first — and no one had
measured that order on its own. T28 measured replacing the tier with
one unified rule under four case orders and lost attachment every
time; T30 c named the tier's nominative-first as the cause of the
charters' number loss (`sancte … ecclesie` at distance two binds as
a nominative plural pair before the genitive rule can speak).

**The experiment** was pure data: permute the rule blocks in
`resolutio.stml`, no code, nine treebanks each. Four orders:
- genitive, nominative, dative, accusative, ablative: case Seneca
  666 → 670, charters 662 → 665 / 685 → 692, Aquinas and Perseus +2,
  Dante −3; number Seneca 917 → 920, charters 925 → 928 / 932 → 939;
  charters primary −1 (dev), forced −1 / −1, attachment −2 / −1,
  gender −1 (dev). ADOPTED, Fran's decision.
- the strict tier's pair-policy order, ablative first: case +28 /
  +34 / +36 on Seneca and the charters, but gender −11 / −5 / −3 and
  attachment −5 / −8 / −9, and Aquinas case −26. Refused: at distance
  the ablative finds spurious pairs, which is what T24 saw when the
  case prior ran before the rules.
- genitive, accusative, nominative: gender −10 everywhere, charter
  forced −12 / −7. Refused.
- genitive, nominative, accusative, dative: identical to the first on
  every number. The dative-accusative order is inert at distance.

**The mechanism**, from the per-rule tables on the charters: the
genitive rules take 227 distant bindings from the nominative rules.
The bound words get the right case and number more often, but the
genitive-preceding rule is 39 % right on class and the moved arcs
are the gold head a little less often, so five pins slip by one or
two while four rise by three to seven. The named cause on every
lowered pin is that trade.

**Not done.** The tier is still literal and greedy, and its order is
still one global order; Dante loses where the charters gain, and a
dialect-specific order was not tried. The §8.5 template family is
where this tier gets structure.

## 2026-09-09 — T31 a: the verb star begins — the subject slot, and the law of the head

**The census first** (`stella_census.py`, seven Latin treebanks, gold
only). Finite verbs have an explicit subject 40–46 % of the time in
classical prose and verse, 69 % in Aquinas, 47–51 % in the charters:
half the slots will rightly stay empty. Subjects are nominative
77–89 % everywhere except the charters (55–61 %, and 32–34 %
accusative: accusative-with-infinitive formulae). The subject stands
before the verb 63–88 % of the time and within three words 65–74 %,
except the charters, where a third of subjects are seven or more
words away. Number agrees 92–98 %; the exceptions are coordinated
subjects. Nearest agreeing nominative is the subject 69–81 % with
gold cases, 45–52 % on the charters. Objects are accusative 89–97 %
and the nearest accusative is the object 68–79 %.

**What was built.** The annotator writes a `subiectum` umbra on every
Latin finite third-person verb reading (nominative, the verb's
number; impersonal verbs excluded; readings with a rarity excluded,
below). Four rules fill it, written with every completion from birth:
adjacent ones in the strict tier beside the strict head rules,
distant ones at the end of the loose tier, preceding before
following in each. The clause refusal applies. The oracle counts
arcs per relation and the gold's subject arcs, so subject precision
and recall are printed and precision is pinned. The dictionary's verb
kind (`species`) reaches the annotator; WORDS marks 3065 verbs
transitive, 1003 intransitive, 67 impersonal.

**Measured.** Nouns-only candidates: refused, worse everywhere.
Adjacent tier alone: precision 63–87 %, recall 14–30 %, attachment up
on every file and nothing down. Both tiers: precision 50–61 % on
classical text, 24–39 % on the charters, recall 36–52 % and 7–12 %;
Seneca case 670 → 691, forced 770 → 801, primary +5, number +12,
attachment 463 → 479; Aquinas case +40, PROIEL +21, Dante +16;
charters case −1 / −8, attachment −9 / +1, number −2, gender −1 on
test. Adopted, Fran's decision: both tiers.

**The law of the head.** `Puella bona ambulat` showed the flaw: the
adjacent adjective is taken as the subject. The adjective already
carries a filled head binding naming its noun, so a final pass in
the executor follows a subject or object binding whose partner is an
adjective with an ADJACENT filled head binding to that noun
(`_ligationes_ad_caput_sequi`, `ORATIO_LEX_CAPITIS=0` to measure).
Three versions measured: any partner with any head binding lost on
the charters and Dante (determiners: a relative pronoun IS the
subject, its head is the antecedent); adjectives only still lost
where the loose head tier's bindings are wrong; adjectives with an
adjacent head: attachment Seneca +4, Aquinas +3, Perseus +4, charters
−1 / 0, Dante −2, subject precision up or level everywhere. Adopted.
This is the first law that uses one binding to correct another.

**Two more rarities.** The subject binding promoted the finite
reading of `residere`, `defendere`, `uidere` over the infinitive:
the syncopated perfect third plural in `-ere`, which WORDS's own
inflection table marks B where `-erunt` is A. And `negato`,
`expleto`: the future imperative. Both are rows in the rarity table
now (`flexio-rarior` from the inflection's frequency code, which the
description carries as `frequentia_flexionis`; `imperativum-futurum`),
and a reading with any rarity gets no subject slot. Verb form went
past its pins: Seneca 978 → 981, charters 986 → 993 / 989 → 992.

**Pins**, Seneca / charters dev / test: primary 841 / 866 / 859,
forced 801 / 742 / 749, case 690 / 663 / 684, number 931 / 930 / 937,
gender 913 / 930 / 929, attachment 485 / 402 / 428, subject precision
520 / 244 / 394 at birth, verb form 981 / 993 / 992, voice 981 / 999
/ 999. Down with named causes: charters case, attachment (dev),
number and gender (test) — the loose subject tier on medieval
formulae; Seneca voice 982 → 981 — fourteen fewer words judged, the
promoted first reading of one carries no voice.

**Fixtures.** `Puella ambulat` reads nominative now, the price
recorded at T24 repaid; `Cum puella bona ambulat` binds `bona` as
subject since `puella` is vindicated ablative, a hand case noted.

**Not done.** Objects (the second ray of the star), first and second
person subjects, accusative subjects of infinitives, a dialect-gated
loose tier for the charters, coordinated subjects.

## 2026-09-09 — T31 b: the object slot, and the carrier law

**Census** (`obiectum_census.py`, gold only): finite active verbs
carry an explicit object 45–54 % of the time on classical text, 43 %
in Aquinas, 33 % on the charters; passive finite verbs 0–8 %, except
Seneca's 25 % which are deponents tagged passive. Objects are
accusative 89–97 %, nouns 49–77 % and pronouns 11–37 %, before the
verb 54–79 %, adjacent 36–59 %, within three words 65–92 %. The
nearest accusative is the object 68–79 % (charters 52–54 %), and
69–82 % once accusatives already belonging to an adposition are
excluded — a fifth to a third of all accusatives. A fifth to two
fifths of objects hang on infinitives and participles.

**Built.** A fifth relation, `obiectum-verbi`, so the adposition
rules of stage 1 cannot fill it. The annotator writes it on Latin
finite active or deponent readings of verbs WORDS marks transitive,
deponent, semi-deponent or of unknown kind (`mitto` is unknown;
excluding those collapsed recall to a sixth), never on passives or on
readings with a rarity. Two adjacent rules beside the subject's
strict pair. The oracle counts the gold's object arcs; object
precision is pinned, recall reported.

**The carrier law.** The first measurement lost primary on every
file, 32 permille on Seneca. The class errata named it: nouns with a
homograph verb reading — `amor`, `regis`, `duce` — had that reading
bound to an adjacent accusative and promoted, 178 nouns and 76
adjectives became verbs on Seneca alone. So a verb-object order whose
verb reading is not already the word's first reading is refused: a
bare case match may fill a slot but never decide a class, where the
subject's case plus number plus person may. With it, the same tier
gains on every classical file.

**Refused, all measured on nine treebanks with the carrier law:**
the loose tier (attachment −49 / −50 on the charters, −14 to −24 on
the shelf; distant accusatives are somebody else's); refusing a
partner reading that changes the partner's class (−1 to −3 attachment,
recall 47 → 44 %: it refused right promotions too); refusing a
partner already bound as an adposition's object (−1 to −4: our
adposition bindings are wrong often enough that trusting them
costs); refusing the verb's own subject as its object (inert: the
subject slot is not yet filled when the object order is applied).
The three partner laws stay in the code inactive, under
`ORATIO_OBIECTUM_SOCIUS`.

**Adopted, Fran's decision, against the committed state.** Seneca
attachment 485 → 503, case 690 → 704, verb form 981 → 989, voice 981 →
991, object precision 55 % at recall 47 %; Aquinas, Perseus, PROIEL,
Dante attachment +10 / +24 / +21 / +26 and case +7 / +16 / +12 / +11;
charters case +3 / +2, forced +2 / +3, object precision 42 / 47 % at
recall 25 / 27 %. Named falls: Seneca primary 841 → 836, forced 801 →
796, gender 913 → 909, charters gender 930 → 926 / 929 → 927,
attachment 402 → 397 / 428 → 426, primary dev 866 → 865, subject
precision dev 244 → 242, voice 999 → 998 twice, verb form dev 993 →
992 — one cause, about thirty words on Seneca: the partner's promoted
accusative reading changes its class or gender, and the refusal that
would prevent it costs more than it saves.

**Fixture.** `In urbem venit` now produces a third order, the object
order of `venit` refused by the carrier law.

**Not done.** Objects on infinitives and participles (two fifths of
the charters' objects); the dative and ablative kinds; the subject
refusal once slots fill in order; a dialect-gated anything for the
charters.

## 2026-09-09 — T31 c: objects on infinitives; participles refused

**Census** (`infinitivum_census.py`, gold only). Active infinitives
carry an explicit object 38–61 % of the time; the objects are
accusative 92–99 %, before the infinitive 43–75 %, adjacent 48–60 %
on classical text but only 8–11 % on the charters. An adjacent free
accusative before an active infinitive is its object 41–59 %
(charters 16–20 %), its accusative subject 3–33 % (Aquinas the high
end, Seneca 3 %), and no dependent of it at all 8–55 % (charters
72–80 %: the formula's accusatives belong elsewhere); after the
infinitive, object 50–69 %. Active participles carry an object
11–36 %; an adjacent free accusative is the participle's object
39–49 % and belongs to another verb 24–49 % of the time, usually the
main verb the participle stands between. Gold tags no tense on
participles outside PROIEL, so no tense split is available.

**Built.** `_obiectum_forma_fert` in the annotator decides which verb
forms carry the object slot: finite always, infinitives by default,
participles under `ORATIO_OBIECTUM_INFINITA=participium` (measured,
refused, kept inactive); `=0` restores finite only. The verb block in
`_umbras_ponere` no longer requires the finite form; the subject slot
still does. Same transitivity, voice and rarity conditions; deponent
infinitives and participles already carry the deponent voice from
`oratio_partes_la`. No rule change: the strict object rules match any
`analysis-verbi` bearing the relation, whatever its form.

**Measured** on eight files against 09590900. Infinitives: attachment
Seneca 503 → 505, Aquinas 612 → 615, Perseus 585 → 586, PROIEL 581 →
584, Dante 421 → 431; case +2..+4 and forced +4..+10 on every Latin
file; primary flat; object recall Seneca 47 → 52 %, Aquinas 53 → 63 %,
PROIEL 55 → 64 %, Dante 45 → 60 %, charters 25 / 27 → 31 / 33 %. The
new bindings are 57 % right on Seneca and 64–78 % on the shelf, above
every file's standing precision. Named falls: charters attachment
397 → 396 / 426 → 422 and object precision 42 → 39 / 47 → 41 % — the
formula accusatives before infinitives that are not their objects;
gender −1 on Seneca, charters test, Aquinas and Dante — promoted
accusative readings, the T31 b cause. Participles on top of that:
case +4..+13 and gender +2..+6 on every classical file, but attachment
Seneca −5, Aquinas −3, PROIEL −2, object precision −2..−4 everywhere,
Seneca primary −1 and forced −4. The participle bindings are 31–56 %
right, below every file's precision, exactly the census band: the
accusative is promoted rightly while the arc goes to the wrong verb.
A right case with a wrong owner is the loose-tier shape again.
Refused, Fran's decision.

**Pins**, Seneca / charters dev / test: primary 836 / 866 / 859,
forced 800 / 751 / 760, case 707 / 668 / 688, number 932 / 931 / 937,
gender 908 / 926 / 926, attachment 505 / 396 / 422, subject precision
524 / 242 / 397, object precision 551 / 397 / 417, verb form and
voice unchanged.

**Not done.** Participle objects: what they need is the main verb
claiming an accusative across an intervening participle, the
distant-object question refused at T31 b. The accusative subject of
infinitives as a slot of its own. The dialect gate for the charters,
whose infinitive objects are nine tenths non-adjacent.

## 2026-09-09 — T31 d: the multipass probe, the census moved home, the refusal registry

**The probe.** Fran asked whether re-evaluating decisions in the light
of other decisions (a multipass) would pay. The cheapest test needed
no new machinery: run the resolution a second time on its own output.
A switch in `oratio_resolutio_applicare` does it
(`ORATIO_RESOLUTIO_CURSUS=N`; `ORATIO_RESOLUTIO_PRIOR_ULTIMO=1` runs the
case prior only in the last pass, `=numquam` never; default one pass,
prior active, nothing changes). Two passes: inert on classical text
(attachment −1..−3, primary flat) and harmful on the charters
(attachment 396 → 391 / 422 → 416). The author and relation tables
explain it: the second pass adds about a hundred arcs per charter file
from the GREEDY rules, adposition objects at 19 % right and the literal
head tier at 2 %, because pass one reordered readings through the
loose tier, the law of umbrae and the prior, and a greedy capture
takes the first reading met. The strict tiers use every completion, so
order tells them nothing and they add nothing. Prior-last was
byte-identical. Reading decisions did improve slightly (a hundred
charter words moved from the law of umbrae to the strict head rule at
70 %), which is why case rose while attachment fell: right case, wrong
owner, for the third time this week.

**Side finding.** With the prior off entirely, case falls 41 on Seneca
and 53 / 63 on the charters, so the prior still earns its place; but
gender rises 9 and number 4 on Seneca and both rise on every Latin
file. The prior picks a reading by case and takes whatever number and
gender that reading carries. A case-only prior that keeps the standing
number and gender where a reading of the target case allows it is a
cheap future experiment.

**What the probe decides.** Feedback through reading order is real and
poisonous; the rules must see decisions as features; and nothing in a
later pass can correct an earlier one while a filled claim is never
contested. So the order of work, Fran's decision: trust from the
auctor tables as a fixed artifact the executor reads, one head per
word with best-wins, then a strict-decided tier. Encoding before
readout before rules — the DKC hierarchy read into oratio.

**The census moved home.** Every census script of the last week lived
in the session scratchpad. They are now `oratio/census/` with a shared
CoNLL-U reader (`conllu.py`), Latin names, a README, and the
measurement harness (`metire.sh`, `compara.py`, `notae.py`,
`relationes.sh`, `errata_delta.py`) beside them. Each moved script was
checked byte-for-byte against its original on the same files.

**The refusal registry.** `oratio/recusata.md` lists sixteen refusals
with the pipeline component each wall sat in (annotatio, exsecutor,
regulae, substratum, oraculum) and the condition under which it is
re-measured. Four of this week's refusals are readout or encoding
walls, not rule walls: the loose object tier, the partner refusals,
the participle slot, the naive second pass. They come back for
measurement once the one-head law and the trust table exist.

## 2026-09-09 — T32 a: author on the arc, one head per word, trust on contests

**Author on the arc.** The umbra carries an `auctor` slot (token, like
the word's decision author), written when the executor fills it with
the rule's title, or `lex-capitis` when the head law rewrites it. The
oracle judges arcs as before and now counts them per author, adjacent
and distant, and counts CONTESTED dependents: words claimed by more
than one filled umbra, with the direction table it already uses (the
carrier depends on the partner for head, adposition, particle and
auxiliary umbrae; the partner depends on the carrier for subject and
object). Gate law: the sum of author arcs equals the arcs judged, so an
unauthored filling is red. Canon: one liberum, sigillum 62a7afd8 →
53bb8633 (XXV genera, CLXXXV loci). No behaviour change; every pin held.

**What the arc table said.** Reading accuracy and arc precision are
different animals: the object rules decide readings 85 % right and
arcs 55–66 %; the loose nominative head rules are 11–18 % right on
arcs; the following-object rule 34 %; the strict subject rule 70 %;
the charters' loose tiers 1–5 %. A third of Seneca's arcs sat on
contested dependents (607 words, 1331 claims) and 95 % of those
dependents had a right claim among their competitors: first-wins by
rule order kept it 46 % of the time. The trust table
(`oratio/probationes/fixa/auctores.tsv`, from `census/fiducia.py` on a
measurement of the pinned files, regenerated only with a named cause)
is fitted on the committed state, before the law, so that it records
what each rule does on its own and never ranks on numbers the ranking
produced.

**One head per dependent, first-wins.** An order whose dependent is
already claimed by another umbra is refused (`_dependens_ordinis`,
`capita_data` per sentence across stages, census `recusatae_capitis`).
Measured alone: attachment up everywhere (Seneca 505 → 520, charters
396/422 → 405/428, shelf +19..+45), contested dependents to zero,
forced down 4 on Seneca and 7 on the charters test because refused
orders no longer promote readings.

**Trust on contests.** Two ways to use the table were measured. Rule
ORDER by trust (stages II+ applied in descending arc precision):
attachment up but case −10 on five files and forced −14 on the
charters, because rule order also decides readings and that order was
tuned (T30 e); REFUSED, kept as `ORATIO_FIDUCIA=ordo`. CONTEST only: a
higher-trust claim withdraws the standing claim on the dependent
(`_petitionem_revocare`: the stage's cell is voided, or an earlier
stage's filling set to −I), rule order untouched; attachment up again
everywhere, forced back above every pin, charters +50/+51 and their
right arcs UP (+466), Seneca −114 right arcs because the table is
charter-dominated for the loose rules. ADOPTED, Fran's decision, with
the per-dialect table named as the next refinement.

**LEX CAPITIS in the contest.** `Puella bona ambulat` showed the
interplay: the adjacent subject rule (705) outranked the head-preceding
rule (624) on `bona`, the head claim was withdrawn and the post-hoc
head law had nothing to redirect. So the head law now lives in the
contest: a subject or object claim on an adjective already bound to an
ADJACENT noun follows to the noun when the noun's reading agrees with
the umbra (case, number), and is refused otherwise; it never unseats
the head. Right arcs +13..+54 on every file. The post-hoc law stays
(+2 arcs on Seneca and Perseus).

**Adopted, against 2a26ab96**, Seneca / charters dev / test: attachment
505 → 526 / 396 → 459 / 422 → 480, subject precision 524 → 581 / 242 →
315 / 397 → 478, object 551 → 553 / 397 → 545 / 417 → 503, case 707 →
714 / 668 → 677 / 688 → 697, forced 800 = / 751 → 760 / 760 → 766,
primary 836 = / 866 → 867 / 859 → 861; shelf attachment +60 / +61 / +39
/ +67. Named falls: Seneca voice 991 → 990, charters gender dev 926 →
925, number test 937 → 936 (refused orders no longer promote readings).
Right arcs: Seneca 2057 → 1676, charters 2369/2469 → 2440/2513, shelf
−7..−11 %, and a NEW PIN: right arcs as a floor (only rising), so no
tranche can buy precision by pruning without a named cause.

**The registry's promise kept.** R11 (loose object tier), R12 (subject
partner refusal) and R14 (participle objects) were re-measured under
the law: R11 still loses (charters attachment −40; distant accusatives
fill dependents nobody claims, wrongly), R12 still inert, R14 the same
shape (case +10, attachment mixed). All three stay refused, dated.

**Fixtures.** `Terra puella non magna est`: `magna`'s distant head claim
yields to the subject claim of `est` (filling −I). `Cum puella bona
ambulat`: the subject claim on `bona` cannot follow to ablative
`puella` and does not unseat the head, filled II not III. `Cum puella
venisset, bona cantat` (the T20 b chain fixture): the distant head
claim of `bona` yields to the adjacent subject claim of `cantat`, so no
binding crosses the clause and the chain counts no discordance.

**Not done.** Keep losing claims as alternatives with arc coverage
printed (so the refused right arcs stop being lost); the per-dialect
trust table; adjacency as the tie-break inside contests; the
strict-decided tier.

## 2026-09-09 — T32 b: losing claims kept as alternatives; arc coverage; the contest census

**Why.** After T32 a a claim that lost, refused on a claimed dependent
or withdrawn by a higher-trust claim, vanished. The 409 right arcs
Seneca lost were unrecoverable in the tree and invisible after the
fact. Fran's point: keep them, with the reason, so that later laws can
act on sets of decisions.

**Built.** A genus `alterna` (six loci: partner word and reading,
proposing rule, cause, trust, winning rule) and an `alternae` list on
the umbra, so an umbra has the shape a word has: a primary filling and
ranked alternatives, nothing deleted. The executor writes an
alternative at every refusal (`recusata`) and, before withdrawing a
standing claim, records it on its own umbra (`revocata`, with the
withdrawing rule as victor). Alternatives are remapped with the
permutations like fillings. Canon sigillum 53bb8633 → bc662e39 (XXVI
genera, CXCII loci). Behaviour unchanged, every column identical.

**Oracle.** Arc coverage: an umbra of the first reading is "petita" if
it carries any claim and "tecta" if any claim, primary or alternative,
is right by the same direction rule; printed beside precision. The
contest table counts, per (winner, loser) pair of rules, how many
contests and how often the loser was right. Gate laws: tectae between
rectae and petitae; contests == alternatives (every alternative names
its victor; the planted fault, victor unwritten, is red).

**What the numbers say, Seneca / charters dev / test / Aquinas /
Perseus / PROIEL / Dante.** Umbrae with a right claim among their
candidates: 2096 / 2543 / 2609 / 4424 / 1823 / 2423 / 1483; right
primaries 1676 / 2440 / 2513 / 3977 / 1573 / 2171 / 1341. Recoverable
by ranking alone: 420 / 103 / 96 / 447 / 250 / 252 / 142. So Seneca's
residual is a RANKING problem, a quarter more right arcs are already
proposed, and the charters' is a PROPOSAL problem: their coverage is
43–45 % and almost nothing is waiting in the alternatives.

**The contest census** (`census/contentiones.py`), seven files:
- adjacent subject vs adjacent object on the same word (a word with a
  nominative and an accusative reading): 1764 contests, the subject
  rule wins by order, the OBJECT claim was right 85 % (Seneca 64 %).
- adjacent head-preceding vs adjacent subject on an adjective: 339,
  the subject claim right 84 % — the redirect could not follow (no
  agreement) and refused it.
- the loose subject rule against itself, two verbs claiming one distant
  nominative: 7947 contests, the loser right 7 % — order is fine there.
- adjacent head-following vs adjacent object: 353, loser right 47 %,
  86 % on Seneca.
These are the tie-break rules of the next tranche, read off a table
rather than guessed.

**Not done.** The tie-break inside contests (relation pair and
adjacency below trust), the per-dialect trust table, promotion of
alternatives by later laws, the strict-decided tier.

## 2026-09-09 — T32 c: the contest judge from the pair table, measured and refused; the census corrected

**The plan.** A tie-break below trust, read off the contest table: for
a pair (winner rule, loser rule) where the loser was right more than
half the time with enough support, the loser wins. A second fitted
table, `oratio/probationes/fixa/contentiones.tsv`, written by
`census/contentiones.py -scribere` from the pinned files, read by the
executor (`_contentiones_onerare`, `_iudex_contentionis`), consulted
before the trust comparison and also where the redirect to an
adjacent head fails.

**First fit, and why it was wrong.** The headline pair, adjacent
subject beats adjacent object on a word with both readings, 1764
contests, loser right 85 %: both claims hang the word on the SAME
verb. The arc oracle judges heads, not relations, so both claims are
right together, and the contest is about the reading, nominative
against accusative, which the arc census cannot see. Flipping it
promoted accusative readings: case −19 on Seneca, −10 / −16 on the
charters. Second fit with joint counts, loser right while the winner
was wrong, was still wrong, because an alternative lives on the
loser's umbra and I had compared it with that umbra's own primary.
Third fit: the oracle now remembers the primary head each dependent
received and judges the loser against THAT; contests with the same
head count as neither. The same-verb pair vanishes from the
discriminating table, as it should.

**What discriminates.** Few pairs: adjacent head-following beats
adjacent object-preceding (125, loser right alone 94 %), adjacent
object-following beats the adposition object rule (36, 97 %), the
unknown-noun head rule beats the adjacent subject rule (34, 85 %),
accusative-following beats nominative-following in the loose tier (48,
67 %). Everything else is under 25 % or under 20 contests.

**Measured, nine files.** Threshold 500 / support 20: Seneca
attachment 526 → 530 and +43 right arcs, case 714 → 710; charters
attachment 459 → 450 / 480 → 471, primary −1 / −1, forced −5 / −3;
shelf mixed. Threshold 800 / support 30: right arcs up on every file
(+9 to +79) but attachment charters −5 / −5, Dante −2, case −3 / −3 /
−2, forced −2 / −6 / −4 on the pinned files. REFUSED (R19): the pinned
files lose case, forced and attachment for a recall gain the floor
would allow but the pins would not. Kept in the code inactive
(`ORATIO_IUDEX=1`), the table committed as the measured artifact.

**What it taught.** The contests that matter most are reading
contests on a single arc, and the instrument to judge them is a
case-aware contest census: for each contest, whether the loser's
reading matches the partner's gold case and whether the winner's
does. That is the next instrument before any judge.
