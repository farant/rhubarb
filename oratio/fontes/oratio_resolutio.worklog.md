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
