# oratio_resolutio — worklog

## 2026-09-15 — T38 b: the decoder's table (`pondera.tsv`) and its loader

The decoder (T38 c) scores a candidate claim by a COUNTED rate: of the
contests a rule took part in, how many it was right in, under a
declared context and a bucket (spec §2 decisions 57–58). This tranche
builds the table and the loader; nothing reads the table yet.

**Crediting law** (`oratio/census/pondera.py`), one contest row = one
LIS row of `metire.sh x.tsv -lites`, judged only when the dependent has
a gold arc (`aurum-arcus` ≠ `ignotum`) and, when both sides claimed the
same head (`idem`), the reading can be judged (`aurum-lectio` ≠
`ignotum`: a same-head contest is a reading contest, and one the gold
cannot judge says nothing). Each side — the victor (the
standing claim) and the victa (the displaced one) — gets one contest.
A side is RIGHT when its arc is gold AND its reading is not wrong:

- arc gold: `aurum-arcus` names the side (or `ambo`), or the two
  sides claimed the SAME head (`idem`) and that head is the gold one
  (`aurum-caput-idem` = 1);
- reading not wrong: `aurum-lectio` names the side, or `ambo`, or
  `ignotum` (no case to judge — the arc alone decides).

So a same-head contest whose head is wrong counts a contest for both
and a right for neither; a `neutra` reading likewise. That is the
judgement the decoder's own `DECRETUM` rows will use for the chosen
candidate (`aurum-electio`), so the table and its check agree.

**Keys credited per side**, finest first:

1. `(regula, dialectus, forma, folliculus, valor)` for every bucket
   whose relations include the side's relation, under the contexts the
   bucket declares and pooled (`-`) — see the verdicts below;
2. `(regula, dialectus, forma, -, -)` in the four context variants
   (both, dialect only, form only, pooled);
3. `(@structura, dialectus, forma, -, -)` likewise, where the
   structural key is `@<relatio>+<ante|post>+<proximo|remoto>` (`#` opens a comment line in the table, so the structural prefix is `@`) from
   the side's own columns (`relatio-*`, `ante`/`ante-victoris`,
   `distantia-*`): dependent before or after its head, adjacent or
   not — a geometric key the decoder recomputes from the candidate,
   never parsed from a title.

Bucket values are the LIS columns' own values so the generator and the
decoder agree by construction: `numerus-capitis` = singularis |
pluralis (the victor only when its head is the contest's head,
`caput-victoris-idem`); `positio-numerus` = `<ante>+<numerus of the
side's own reading>`; `directio-distantia` = `<ante|post>+<1|plures>`
for head candidates. Rows under twenty contests are WRITTEN (the loader
applies the minimum), so the table shows what was thin.

**Table** `oratio/probationes/fixa/pondera.tsv`: a `#` provenance line,
a header row `regula dialectus forma folliculus valor contentiones
rectae permille`, rows sorted by key; fitted on rows whose `thesaurus`
is `la_circse` or `la_llct` ONLY — the generator refuses a run lacking
either (exit 2). The shelf never enters (decision 57). `medius` in the
dialect column covers the charters AND UDante (T38 a found Dante at
12 ‰ medieval spellings), which is what the census says, not a choice.

**Fixture derived by hand** (`fixa/pondera/proba_lites.tsv`, twelve
rows; S = `umbra-subiectum-praecedente-proximo` always the victor, O =
`umbra-obiectum-verbi-praecedente-proximo` always the victa, same head,
adjacent, both before the verb; the victor's reading number equals the
verb's):

| row | corpus | verb number | O reading number | gold | note |
|---|---|---|---|---|---|
| 1 | circse (classicus, versus) | sg | pl | victa | |
| 2 | circse | sg | pl | victa | |
| 3 | circse | sg | sg | victor | |
| 4 | circse | sg | pl | victa | head NOT gold: neither right |
| 5 | circse | pl | pl | victor | |
| 6 | circse | pl | pl | victor | |
| 7 | llct (medius, prosa) | sg | sg | victa | |
| 8 | llct | sg | pl | victa | |
| 9 | llct | sg | sg | neutra | neither right |
| 10 | llct | sg | sg | victor | |
| 11 | llct | pl | pl | victa | |
| 12 | llct | pl | sg | ignotum | no gold arc: no contest |
| 13 | llct | pl | sg | idem, reading ignotum | same head, reading unjudgeable: no contest |

S right on rows 3, 5, 6, 10 (4 of 11); O right on 1, 2, 7, 8, 11 (5 of
11). By context: S classicus/versus 3 of 6, medius/prosa 1 of 5; O 2 of
6, 3 of 5. Verb number, S: singularis 2 of 8 (1 of 4 per corpus),
pluralis 2 of 3 (2 of 2 CIRCSE, 0 of 1 LLCT); O: singularis 4 of 8,
pluralis 1 of 3 (0 of 2, 1 of 1). Position × number (all `1+`; conditioned by DIALECT after the
verdict below — in the fixture classicus ⇔ versus and medius ⇔ prosa
row for row, so the counts are the same): S `1+singularis` 2 of 8 (1
of 4 per dialect), `1+pluralis` 2 of 3 (2 of 2 classicus, 0 of 1
medius); O `1+pluralis` rows 1 2 4 5 6 8 11 → 4 of 7 (2 of 5 classicus,
2 of 2 medius), `1+singularis` rows 3 7 9 10 → 1 of 4 (0 of 1
classicus, 1 of 3 medius). 52 rows in `exspectata.tsv`: 7 + 7 structural,
19 + 19 per rule.

**Loader** `oratio_resolutio_pondera_legere(piscina, programma, textus,
vitium)`: the first non-`#` line is the header and must equal the eight
titles by name (else `vitium->causa` = `caput ponderum: <titulus>`), a
row of another width is refused with its line; key = the five key
fields joined by TAB, value `OratioPondus {contentiones, rectae,
permille}`; `oratio_resolutio_pondus(...)` looks one key up. The file
loader runs beside the trust table's (`fixa/pondera.tsv`, absent =
no table). `ORATIO_PONDERA_LIMEN` (default XX) is read by
`oratio_resolutio_pondera_limen()`; the loader does not apply it — the
decoder does, so the table is inspectable whole.

**Bucket verdicts by the lattice (same day, `lites_t38a.tsv` with the
`forma` column; `FOLLICULI` rows carry them as their cause):**

- `numerus-capitis` — kept, dialect-conditioned (T35 e / T36 d: +75,
  the charters fall 12).
- `positio-numerus` (the side's position × its own reading's number)
  — kept, but conditioned by DIALECT, not form: the pair
  `ante+numerus-victae` scores +79 held out at depth two, gaining on
  five corpora and falling on one, the charters (`1+pluralis`: pooled
  vote victor, LLCT 2 of 8) — the same shape as verb number. Form was
  the plan's guess and the lattice refused it: restricted to verse,
  the two verse corpora flip against each other on the T32 f triple
  (`1+pluralis+singularis`: CIRCSE 47:29 for the subject, Perseus's
  verse 18:7 for the object), so form does not separate them. The flip
  is Seneca's tragedy against epic and fable, a corpus fact outside any
  declared context; it stays out of the table (decision 58).
- `directio-distantia` (head candidates: 2,061 contests with
  `relatio-victoris` = caput, gold = `aurum-arcus`) — REFUSED: the pair
  `ante-victoris+distantia-victoris` falls on three of six corpora;
  its groups `1+1` and `0+plures` are apertus and MUTABLE, CIRCSE
  against the rest on the first, the charters against the rest on the
  second — one verse-classical, one prose-medieval, so neither dialect
  nor form separates both. The `forma` column itself scores −20 there.
  The row stays in `FOLLICULI` with `activus = False` and this cause,
  so the refusal is data too. Head contests are scored by the rule's
  plain rate and the structural key until a bucket is earned.

Also learned: `#` cannot prefix a key in a `#`-commented table — the
first loader silently dropped the 14 structural rows (38 of 52); the
structural prefix is `@`.

**The real table, read (403 rows from `lites_t38a.tsv`, CIRCSE + LLCT
only).** Verb number for the two star rules, permille right by
dialect:

| rule | classicus, sg | classicus, pl | medius, sg | medius, pl |
|---|---|---|---|---|
| subject (praecedente-proximo) | 294 (231) | 534 (58) | 453 (64) | 967 (124) |
| object of the verb (praecedente-proximo) | 189 (248) | 196 (56) | 794 (34) | 666 (6) |

(contests in parentheses; absolute rates are low because a contested
word often hangs on neither claimed head — both sides then count a
contest and no right). What this means for the decoder: under a
SINGULAR verb the table flips toward the object only in `medius`
(charters 794 vs 453), while `classicus` still prefers the subject
(294 vs 189) — because the only shippable classical corpus is CIRCSE,
whose singular-verb contests are a coin (T32 d: 86:79 the subject's
way). The prose-shelf behaviour (ITTB 157:255, Perseus 57:166, PROIEL
36:148 toward the object) is NOT in the table, by decision 57, and
CIRCSE's own prose is 172 contests of Tacitus, too thin for a bucket
row. So the T38 d prediction must be re-aimed: expect the flips on the
medius files (the charters and UDante, which the dialect census puts
with them), not on the classical shelf; the classical prose gain waits
for a shippable classical prose corpus or for a decision on fitting
(a question for Fran, not this tranche). An independent recount of
the subject rule's singular-verb row from the raw contest rows gives
295 / 97 / 328, the table's numbers.

## 2026-09-15 — T38 c: the decoder (`_decretor`)

A pass after the last stage, before the law of umbrae, lex capitis and
the case prior. It never fires a rule: its inputs are the tree and the
table (decision 48).

**Candidates.** For every umbra of every reading of every word: the
standing filling (`impletio` with a scope) and every alternative whose
cause is a trust decision — `recusata`, `revocata`, and the new
`praeoccupata`, recorded at the T34 site: a row naming an umbra already
filled in an earlier stage stays out of the executor's contest (T34)
but is the second proposal for that umbra, so it is now written as an
alternative (deduplicated against the umbra's filling and alternatives
by filler reading). A candidate's CELL is its dependent word by the
relation's direction (`_dependens_ordinis`, the oracle's law). A cell
with two or more distinct (umbra, reading) candidates is contested.
Uncontested standing claims are fixed first: their heads are taken and
their umbrae marked, so a contested cell's candidate on the same umbra
is blocked (one filling per umbra).

**Score** (`_pondus_candidati`), ladder finest first, each rung under
the four context variants (dialect + form, dialect, form, pooled) and
answering only with at least `limen` contests: I the bucket rows of the
candidate's relation (`numerus-capitis` with the carrier reading's
number, `positio-numerus` with `<ante>+<filler reading's number>`;
`ORATIO_PONDERA_SINE=<folliculus>` skips one), II the rule's plain row,
III the structural row `@<relatio>+<ante|post>+<proximo|remoto>`
recomputed from the candidate, IV the trust table. None → no score
(only the standing claim can hold that cell). The rung that answered
is recorded (`gradus`).

**Decode.** The candidates of contested cells sorted by score
descending, standing first among equals (stable insertion sort);
accepted in order when the umbra is free, the dependent has no head,
no cycle arises (walking accepted heads), and the explicit-class law
holds (a filler word decided by an explicit rule keeps its first
reading). A candidate that passes those checks but finds its cell
already headed is SUPERSTES — it survived the constraints and lost on
order. Habitus per cell: COACTUS one survivor, ORDINATUS several with
the winner's score above the runner-up's, APERTUS a tie at the top
(the standing claim stands, swapped in if a tie put an alternative
first) or no survivor (the cell keeps the executor's state).

**Write-back** when the winner is not the standing claim: the umbra's
previous filling becomes an alternative `decreta` with victor
`decretor`; the winner's reading goes into `impletio` (reponere) and
its rule into `auctor`; the reading is promoted first in its word
(`_lectionem_promovere`: the same two materia calls the stages use,
`materia_nodus_lista_permutare` + `oratio_partes_compendia_reponere`);
the dependent's decision becomes `decretum` by `decretor`, and the word
is marked explicit so the later law of umbrae leaves it. Every
contested cell gets its `habitus` slot. One `OratioDecretum` record per
contested cell goes to the census (`decreta`) for the oracle.

**Deviation from the plan.** The stage code keeps its own permutation
block; the decoder's promotion helper repeats the two calls rather than
refactoring T19d's loop — the calls are the contract, and touching the
stage loop for a decoder tranche would have widened the blast radius.

**Built (same day) — two laws the first run forced, and the default.**
The first oracle run with the decoder on changed 676 cells on Seneca
and its chosen candidates were right in 27 % of the ordered cells,
32 % of the forced ones; every pin fell. Reading the numbers: the
ladder MIXED POPULATIONS. A bucket or plain row counts a contest for
both sides even when neither is right (the word hangs elsewhere), so
its absolute rates sit at 200–300 ‰; the trust table is arc precision
at 500–700 ‰. A candidate scored on rung I lost to one scored on rung
IV by construction. Law now: every candidate of a cell is scored on
the SAME rung, the finest that answers for all of them (`pondera[4]`
per candidate, the rung chosen per cell); no common rung, no scores,
the standing claim stands. Second law, from the same run: a READING
contest (every candidate of the cell claims the same head) never
reaches rung IV — trust cannot judge readings (R19, T32 f); without a
table row the executor's order stands. With both laws the gate's four
scenarios hold: object row 700 keeps the object (ordinatus), subject
row 700 flips `bellum` to the subject with the object claim deposed
(`decreta`, reference NIHIL) and `decisio` = decretum by `decretor`,
no table = executor (apertus), variable unset = untouched. DEFAULT OFF:
`ORATIO_DECRETOR=1` turns the pass on; unset or 0 leaves the executor
alone, so every pin is byte-identical at this commit and the T38 d run
decides the default with Fran (the plan said "on, pins move in d";
pins cannot move at a c commit). The oracle judges each record as LIS
judges a side — arc gold AND reading not wrong — and the inline law on
`Puella bellum videt` with the real programme and table reads `cedens`:
the subject rule kept (294 vs 189 in classicus) is wrong against the
gold `obj`, the deposed object candidate was right. The
`praeoccupata` alternatives add contest rows to `-lites`, so the table
must be regenerated from a post-c run in T38 d (named cause).
