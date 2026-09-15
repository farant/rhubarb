# oratio_oraculum worklog

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
