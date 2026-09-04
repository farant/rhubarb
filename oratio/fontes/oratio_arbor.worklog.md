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
