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
