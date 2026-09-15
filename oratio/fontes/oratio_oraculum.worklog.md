# oratio_oraculum worklog

## 2026-09-15 — T37 b, `sententia` on the LIS rows

Column 33: the sentence's `# sent_id` (the CoNLL-U reader already kept
it as `OratioConlluSententia.id`), else the sentence's first line
number in the file — a line number greps, an ordinal does not. Law:
non-empty on every contest row of every judged treebank (all pinned
files carry ids). Plant: the field declared but not printed — the T35 c
width law (header = row + 1) went red at once, which is what that law
is for. Purpose: `./oratio/reticulum.sh … -greges … -exempla N` names
the sentences behind a bucket.

## 2026-09-14 — T36 c, `lemma-capitis` on the LIS rows

Column 32, appended under the T35 c header law: the LEMMA token of the
claimed head's first reading (`_lemma_lectionis`, the same walk as
`_accidens_lectionis` but returning the token's chorda), condensed into
the census piscina like the forms. Present on every contest row of the
three pinned Latin treebanks (2,413 / 4,233 / 1,973 — the head of a
subject/object contest is a resolved verb). Purpose: the partition
lattice's `-sortes lemma-capitis`, leave-one-verb-out, to separate a
structural bucket from a lexical one (T36 d). Readers select by title;
the first 31 fields are untouched.

## 2026-09-14 — T35 c, the LIS rows describe themselves

Part of oratio T35, the partition lattice (spec `oratio-spec.md` §2
decisions 49–53, §7 "Design — T35"; plan
`project-specs/partitio-reticulum-plan.md`). The partition instrument
reads contest rows by column TITLE, so the rows now carry a header and
the judgments the census needs.

**The formatter moved into the library.** Until now the LIS row was one
`printf` in `oratio/instrumenta/oraculum.c`, which no probatio can call.
The header law (header width = row width + 1) needs a row built
in-process, so `oratio_oraculum_lis_columnae` and
`oratio_oraculum_lis_linea` now build both lines with a
`ChordaAedificator`, and the instrument prints what the library builds.
The title table `ORATIO_COLUMNAE_LIS` sits beside the formatter,
declared with its size (`ORATIO_COLUMNAE_LIS_NUMERUS`, XXXI) so a missing
initializer is a NIHIL title the probatio refuses rather than a silent
short header.

The instrument's `_titulus_valoris` was used only inside the old LIS
loop: removing the loop without the helper would have broken the build
under `-Wunused-function`. The library carries its own copy,
`_lis_titulus`.

**Judgments equal the contest table by construction.** `_litem_notare`
now receives the same `recta` and `prima_recta` that the call site passes
to `_contentionem_notare` a few lines later, under the same condition.
`aurum-arcus` is IDEM when the winner's head is the loser's head or
absent (the call site then sets `prima_recta = recta`, so the contest
table counts neither side alone); otherwise AMBO / VICTA / VICTOR /
NEUTRA from the two flags — exactly `victa_sola = recta &&
!victor_rectus`. `aurum-lectio` is IGNOTUM unless all three cases are
known (the contest table's `casus_iudicati` condition), then AMBO /
VICTA / VICTOR / NEUTRA by equality with the gold case. The probatio
checks the equality per treebank (cross-count laws), so the two code
paths cannot drift.

**Signature.** The loser's relation is read from the umbra at the call
site. The winner's relation needed a new per-sentence array,
`relatio_nostra`, beside `caput_nostrum` and `lectio_nostra` in
`_ligationes_iudicare`, set at the first claim on each dependent (the
same place `caput_nostrum` is set, so the two always describe the same
claim). Direction and element distances come from the element indices
already in hand; a winner without a head has distance 0.

**`thesaurus`** is the UD file stem before `-ud-` (else before the last
extension): `la_llct-ud-dev.conllu` and `la_llct-ud-test.conllu` are
one fold. The instrument never parses a path.

**Measured.** Oracle probatio green (58.8 s); the cross-count laws hold
on all five pinned treebanks:

| treebank | arc victa / victor | reading victa / victor | judged for case |
|---|---|---|---|
| CIRCSE | 389 / 660 | 243 / 278 | 1,802 |
| LLCT dev | 177 / 697 | 30 / 19 | 2,595 |
| LLCT test | 48 / 474 | 75 / 5 | 826 |
| EWT dev, test | 0 / 0 | 0 / 0 | 0 |

English readings carry no case, so nothing is judged for case there,
and no English contest has one side alone right on the arc.

Byte identity: CIRCSE re-run through the rebuilt instrument — the first
25 fields of all 2,413 LIS rows are byte-identical to the baseline taken
before any change; one header line of 34 fields, every row 33.

CIRCSE distributions of the new columns: arc idem 907, victor 660,
neutra 457, victa 389; reading ambo 1,000, ignotum 611, neutra 281,
victor 278, victa 243. **481 rows (20 %) carry no winner relation** —
every one is IDEM with winner distance 0: the winning claim had no
aligned head recorded (`caput_nostrum` −1), so the arc cannot
discriminate. The instrument will see these as their own group of
`relatio-victoris`; they are not a formatter defect.

**A trap in my own check.** `grep '\tLIS\t'` also matches the new header
line, whose KIND field is `LIS` — the first comparison "failed" at
character 53 of line 1, exactly where `COLUMNAE` stands. Select rows by
the second field (`awk -F'\t' '$2=="LIS"'`). `lites.py` is unaffected
(it tests `c[1] == 'LIS'`).

**Formatting.** These files carry 325 format faults from before T35
(long legacy lines). Only the faults on ADDED lines were fixed — isolated
by intersecting `formator -vitia` with the added ranges of
`git diff -U0` (10 lines, all length).

**Planted fault** (`silva.planta`, gate `oratio` filtered `oraculum`): the
VICTA branch of `iudicium_arcus` set VICTOR instead. RED on the first
cross-count law — `arcus_victa == summa_victa_sola` at
`probatio_oratio_oraculum.c:904` — then GREEN after the automatic revert.
The two independent paths through `_litem_notare` and
`_contentionem_notare` are what the law compares, so a mislabelled
judgment cannot pass unnoticed.

## 2026-09-15 — T38 a: the declared context (dialect and form)

The decoder's table conditions on a DECLARED context: dialect (T30 c,
already per file) and text form, versus | prosa. A CoNLL-U file has no
line structure, so the T6b form layer cannot see verse there; the
oracle DECLARES form per sentence from the ids instead
(`FORMAE_THESAURORUM`). What the ids say: CIRCSE is not wholly verse —
119 Tacitus, Germania sentences carry `_prose`; 647 carry `_poetry`
and 127 Phoenissae ids (`SenPhoen-P…`) carry no suffix and are verse.
Perseus's test file is four works by `# newdoc id`: Vergil (phi0690),
Ovid (phi0959), Phaedrus (phi0975) verse and Petronius (phi0972)
prose — so Perseus is mostly VERSE, though its subject/object contests
behave like prose (T32 d). The declared form may therefore not separate
Seneca from Vergil and Ovid; the lattice with the new `forma` column
decides that in T38 b, not this tranche.

Plumbing: the reader keeps the last `# newdoc id` on every sentence
(`OratioConlluSententia.documentum`, appended field); LIS rows gain
columns 34 `dialectus` and 35 `forma` (append-only header law); the
resolver takes an `OratioContextus {lingua, dialectus, forma}` through
`oratio_resolutio_applicare_contextu`, the old entry delegating with
classicus and prosa; instruments build the context from the document's
censuses (`oratio_forma_documenti_censu`: versus when the words in
versus paragraphs exceed half). Nothing reads the context before T38 c,
so every pin and every non-LIS row is byte-identical after this
tranche — that is the acceptance.

**Built (same day).** The FORMA rows of the nine-file run: CIRCSE
774 versus / 119 prosa; Perseus 392 versus / 547 prosa (Petronius is
the largest work in that file); every other file prosa. Contest rows
by declared context: CIRCSE 2,241 versus + 172 prosa, Perseus 1,274 +
920, UDante 3,484 `medius` (Dante's Latin trips the dialect census at
12 ‰, as T30 c measured — so `medius` covers LLCT and UDante, not the
charters alone; a fact the table's dialect rows must carry). Byte
identity: all 24,306 LIS rows equal on fields 1–35 (the 33 old
columns), every non-LIS row equal, against the T37 baseline. Gates:
oracle and resolver green first run; the form gate's first fixture
(four short lines each ending in a full stop) was judged an INDEX by
the T6b rules — a list, which is what four terminated short lines look
like — replaced by the suite's own Catullus distich (14 words).
Plants, each red on the intended law then green: `_prose` declared
versus (the inline identity law, before the CIRCSE count could speak),
`>=` in the document census (the 14/28 tie became versus), reader
prefix XV for XIV (`documentum` = "hi0690.x").
