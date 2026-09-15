# T38 — the decoder — Implementation Plan

> **For agentic workers:** execute INLINE (superpowers:executing-plans),
> one tranche per turn with Fran's approval between tranches (standing
> preference, memory `plan-execution-inline-checkpoints`). No subagents.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** one global decision per sentence over the proposals the
executor already leaves in the tree — a counted, context-conditioned
score orders what the hard constraints let survive — with every
decision a machine row the lattice can read.

**Architecture:** four tranches. (a) CONTEXT: text form per document
and declared per treebank sentence, two appended contest columns, a
context struct into the resolver — no decision changes. (b) TABLE: a
generator over the contest rows of the two shippable corpora, a
committed self-describing table, a loader that refuses a wrong header.
(c) DECODER: a pass after the last stage in `oratio_resolutio.c`,
greedy best-first under the constraints, written back through the
executor's own paths, a profile slot on the word, decision rows in the
oracle. (d) THE RUN on nine treebanks against a prediction written
first.

**Spec:** `project-specs/oratio-spec.md` §2 decisions 48, 57–59; §7
"Design — T38, the decoder". Ledger: desideratum 01M24Z45XK (the
decoder), decretum 01M2HGPT52 (the order), note on 01M2HGQ2HM (why the
agreement step was skipped).

## Status

| tranche | state |
|---|---|
| T38 a context | done — `oratio_forma_documenti_censu`, `OratioConlluSententia.documentum`, `FORMAE_THESAURORUM` + `oratio_oraculum_forma_sententiae`, `FORMA` row, LIS columns 34–35, `OratioContextus` + `oratio_resolutio_applicare_contextu` + `oratio_resolutio_contextus_documenti`, instruments; oracle and resolver gates green on the first run, the form gate's first fixture was judged an INDEX (four short full-stop lines) — replaced by the suite's own Catullus distich (14 words); plants: `_prose` → versus (oracle), `>=` (form census), prefix XIV → XV (reader) |
| T38 b table | done — `pondera.py` (crediting law: each side of a judged contest; same head + unjudgeable reading = unjudged), 13-row hand fixture + 52 expected rows (derivation in the resolver worklog), C loader with header refusal by name and width by line, `oratio_resolutio_pondus`, `pondera_limen`; LATTICE VERDICTS changed the buckets: `positio-numerus` dialect-conditioned like verb number (form REFUSED: inside verse CIRCSE and Perseus flip against each other), `directio-distantia` REFUSED (falls 3 of 6, mutable groups CIRCSE vs rest and LLCT vs rest) — kept as an inactive data row; `#` collided with the comment marker → structural prefix `@` (the first loader silently dropped 14 rows); real table 403 rows, one bucket recounted independently (295/97/328); FINDING: `classicus` = CIRCSE's coin, the prose flip lives only in `medius` — the (d) prediction re-aimed at the charters and Dante; plant = reading ignored in the crediting (Python gate red) |
| T38 c decoder | done — registry slot `habitus` (loci 191, seal c4752e42), enums `decretum` / `praeoccupata` / `decreta`, canon, STML mirror, `praeoccupata` recording at the T34 site, the pass `_decretor` (candidates, per-cell rung, greedy under the constraints, write-back through the executor's paths), `OratioDecretum` records, oracle judgement (`_decretum_iudicare`, `aurum-electio`), `DECRETUM`/`DECRETOR` rows, `-decreta`, Python `habitus` + `decretum`; TWO LAWS THE FIRST RUN FORCED: every candidate of a cell scored on the SAME rung (mixed rungs compared contest rates at 200–300 ‰ with arc precision at 500–700 ‰ — 676 cells changed on Seneca, 27 % right), and a reading contest never reaches the trust rung (R19); DEFAULT OFF (`ORATIO_DECRETOR=1` turns it on) so every pin is byte-identical at this commit — T38 d decides the default; computus golden regenerated (one slot per word node, the alternative nodes); plants: sort ascending (resolver gate red), reading comparison inverted (oracle inline law red) |
| T38 d the run | done — table regenerated post-c (praeoccupata rows); three runs: (1) arcs fell on every file though the changed cells were right more often → two-pass greedy (no effect); (2) the real cause: promoting a reading hid the arcs on the demoted reading → `_arcum_celaret` constraint → right arcs UP on every file (+100 Seneca, +149 Aquinas), case and forced up, precision down on six files (more arcs, less precise); (3) levers alone: plain rung off and limen 40 washes, `numerus-capitis` OFF better on case everywhere → bucket row inactive with cause, table 383 rows; lattice over 6,822 decisions: no next bucket in the row; then the PURE CHOOSER: the 'added' arcs were hidden claims (carrier reading not first) replaced by visible alternatives — laws: a decision changes one arc (no hiding, no surfacing), only a visible standing claim makes a re-decision, additions behind `ORATIO_DECRETOR_ADDITIONES` (1 | classicus | medius); pure chooser: precision +9/+4/+1, right arcs +22/+16/+5, case up, forced up, Seneca primary/gender −1 ‰, subject-precision artifact down; DEFAULT STAYS OFF — configuration and pins = Fran's decision; R14/R20 not re-measured (named) |

---

## Global Constraints

As `partitio-reticulum-t36-plan.md` §Global Constraints, verbatim (C89
house flags, Latin identifiers, latina.h macros forbidden, i32
unsigned, chorda not NUL-terminated, piscina only, no floats, machine
rows self-describing and append-only, decimal CLI flags, new words
checked with `./oratio/quaere.sh` BEFORE staging, formator on added
lines, `silva.commissio` with explicit `viae`, planted fault per gate,
docs first / gates last, select machine rows by FIELD, zsh `${=F}` and
never a command in a variable). Additional to T38:

- The decoder NEVER fires a rule and never reads the pattern engine;
  its inputs are the tree (fillings, alternatives, readings) and the
  table. Rules propose, tables score, the decoder decides (decision 48).
- Weights are COUNTED (decision 57): no constant in the decoder is a
  weight; the only constants are the row minimum (`limen` = XX,
  `ORATIO_PONDERA_LIMEN`) and the ladder order, both in one place.
- The table is fitted on rows whose `thesaurus` is `la_circse` or
  `la_llct` ONLY (decision 57); the generator refuses to write a table
  from a run that lacks either.
- A bucket row in the generator carries its cause with the lattice's
  numbers (decision 58); no bucket without a lattice verdict.
- Profile titles are the lattice's own: `coactus | ordinatus |
  apertus` (`PARTITIO_TITULI_HABITUUM`, decision 56) — the oracle's
  `DECRETUM` rows must be readable by `reticulum.sh -genus DECRETUM`.
- Text form is `forma` everywhere (`genus` is grammatical gender in
  this codebase).
- Every existing pin is byte-identical after tranche (a) and after
  tranche (c) with `ORATIO_DECRETOR=0`; a pin moves only in (d), each
  with a named cause and Fran's decision.
- Expected new identifier words (check before staging): `decretor`,
  `pondus`, `folliculus`, `praeoccupatus`, `contextus`, `electus`,
  `cedens`, `scala`. `habitus`, `decretum`, `forma`, `coactus`,
  `ordinatus`, `apertus` exist.

## File structure

| file | responsibility | tranche |
|---|---|---|
| `oratio/fontes/oratio_forma.h/.c` | `oratio_forma_documenti_censu` | a |
| `oratio/fontes/oratio_conllu.h/.c` | `OratioConlluSententia.documentum` (`# newdoc id`) | a |
| `oratio/fontes/oratio_oraculum.h/.c` | `FORMAE_THESAURORUM`, `oratio_oraculum_forma_sententiae`, `formae[]` census, LIS columns 34–35, per-sentence context to the resolver; (c) `DECRETUM`/`DECRETOR` judgement | a, c |
| `oratio/instrumenta/oraculum.c` | `FORMA` row; (c) `DECRETUM`/`DECRETOR` rows, `-decreta` | a, c |
| `oratio/fontes/oratio_resolutio.h/.c` | `OratioContextus`, `oratio_resolutio_applicare_contextu`; (b) `OratioPondus`, `pondera`, `oratio_resolutio_pondera_legere`; (c) `praeoccupata`, the pass, census, `OratioDecretum` | a, b, c |
| `oratio/instrumenta/verba.c`, `arbor.c` | context from the document's censuses | a |
| `oratio/census/pondera.py` | the generator | b |
| `oratio/probationes/fixa/pondera.tsv` | the committed table | b |
| `oratio/probationes/fixa/pondera/proba_lites.tsv`, `exspectata.tsv` | generator fixture + hand-derived rows | b |
| `oratio/fontes/oratio_registrum.h/.c`, `oratio/grammatica/oratio.canon` | `habitus` slot, `OratioHabitus`, appended enum values, GENERA offsets, seal | c |
| `pythonica/silva.py` | `habitus` on `Oratio.vocabula()`, `decretum` in `ORATIO_PARTITIO` | c |
| `oratio/probationes/probatio_oratio_{forma,conllu,oraculum,resolutio,registrum}.c`, `probatio_silva.py` | gates | a, b, c |
| `oratio/probationes/fixa/computus/*` | golden regenerated with cause | c |
| `oratio/census/README.md`, `oratio/CLAUDE.md`, worklogs, spec as-built, memory, this table | records | b, d |

---

### Task T38 a: context — form per document, declared form in the oracle, LIS columns 34–35, `OratioContextus`

**Files:** `oratio_forma.h/.c`, `oratio_conllu.h/.c`,
`oratio_oraculum.h/.c`, `instrumenta/oraculum.c`,
`oratio_resolutio.h/.c`, `instrumenta/verba.c`, `instrumenta/arbor.c`,
`probatio_oratio_forma.c`, `probatio_oratio_conllu.c`,
`probatio_oratio_oraculum.c`, `probatio_oratio_resolutio.c`,
`oratio_oraculum.worklog.md`, `oratio/census/README.md` (LIS columns).

**Produces (exact):**

```c
/* oratio_forma.h — forma documenti ex formis paragraphorum (T6b):
 * VERSUS si verba paragraphorum versuum plus quam dimidium verborum,
 * aliter PROSA; *verba_versus, *verba (NIHIL licet) numeri */
OratioForma oratio_forma_documenti_censu (constans MateriaNodus* radix,
    i32* verba_versus, i32* verba);

/* oratio_conllu.h — OratioConlluSententia, campus APPENSUS */
chorda documentum;   /* '# newdoc id = ...' ultimum visum (vacua si absens) */

/* oratio_resolutio.h */
nomen structura {
    constans character* lingua;     /* "latina" | "anglica" | NIHIL */
                   s32  dialectus;  /* OratioDialectus */
                   s32  forma;      /* OratioForma: prosa | versus */
} OratioContextus;
b32 oratio_resolutio_applicare_contextu (Piscina*, InternamentumChorda*,
    constans MateriaLexiconRatum*, constans OratioProgramma*,
    s32 regulae_numerus, constans OratioContextus* contextus,
    MateriaNodus* radix, OratioResolutioCensus* census);
/* oratio_resolutio_applicare (vetus) delegat: {lingua, CLASSICUS, PROSA} */

/* oratio_oraculum.h */
OratioForma oratio_oraculum_forma_sententiae (
    constans OratioConlluSententia* s);   /* FORMAE_THESAURORUM */
/* OratioOraculumCensus, appensa */  i32 formae[ORATIO_FORMA_NUMERUS];
/* OratioOraculumLis, appensa */     s32 dialectus; s32 forma;
#define ORATIO_COLUMNAE_LIS_NUMERUS XXXV   /* 34 dialectus, 35 forma */
```

`Cursus` (resolutio.c) gains `constans OratioContextus* contextus`;
`cursus->lingua` stays (read from the struct) so no call site of
`cursus->lingua` changes.

The declaration table, in `oratio_oraculum.c` beside the file list
conventions:

```c
/* FORMAE THESAURORUM (T38 a, 2026-09-15): forma sententiae thesauri
 * DECLARATA per identitatem (CoNLL-U lineas non habet, forma T6b tacet):
 * CIRCSE id '_prose' -> prosa (Tacitus Ger CXIX), ceterae versus
 * (_poetry DCXLVII, SenPhoen CXXVII); Perseus per documentum PHI:
 * phi0690 Vergilius, phi0959 Ovidius, phi0975 Phaedrus versus,
 * phi0972 Petronius prosa; ceterae plagulae prosa. INVENTUM: Perseus
 * tres opera versuum, contentiones tamen prosae similes (T32 d) -
 * reticulum cum columna forma decernit. */
```
`oratio_oraculum_forma_sententiae`: if `s->id` contains `_prose` →
PROSA; else if `s->id` contains `_poetry` or starts with `SenPhoen` →
VERSUS; else if `s->documentum` starts with `phi0690`, `phi0959`,
`phi0975` → VERSUS; else PROSA. Substring tests on chorda by hand
(`_chorda_continet`, length-checked; never a C string function).

- [ ] **Step 1: docs first.** Worklog entry (oraculum) "T38 a: forma
  declarata per identitatem; Perseus versus"; README row for LIS
  columns 34–35.
- [ ] **Step 2: conllu.** `oratio_conllu.c` reader keeps the last
  `# newdoc id = ` comment in a local and stamps every following
  sentence's `documentum` (copy in piscina; empty before the first).
  Gate `probatio_oratio_conllu.c`: an inline three-sentence text with
  one `# newdoc id = phi0690.x` before sentence 2 → sentences 1 empty,
  2 and 3 `phi0690.x`. Plant = the local reset per sentence (3 empty)
  → red, revert → green.
- [ ] **Step 3: forma.** `oratio_forma_documenti_censu` in
  `oratio_forma.c`: walk `documentum.paragraphi` → for each
  `oratio_forma_paragraphi(p)` and count `vocabulum` elements over its
  sentences; VERSUS iff `verba_versus * II > verba`. Gate
  `probatio_oratio_forma.c`: parse a fixture of two paragraphs (a
  four-line verse stanza of 12 words, then a prose paragraph of 30
  words) → PROSA with counts 12/42; the same with the prose paragraph
  cut to 8 words → VERSUS. Plant = `>=` for `>` with equal halves
  fixture (a 10 + 10 fixture must be PROSA) → red.
- [ ] **Step 4: resolver.** `OratioContextus`,
  `oratio_resolutio_applicare_contextu`; the old entry builds the
  struct and delegates. Gate `probatio_oratio_resolutio.c` part I:
  `Puella ambulat.` through both entries → identical `decisio`,
  fillings and first readings (the tree projected to STML byte-equal).
- [ ] **Step 5: oracle.** `FORMAE_THESAURORUM` +
  `oratio_oraculum_forma_sententiae`; `_sententiam_iudicare` computes
  the sentence's forma, counts `census->formae[forma]`, builds
  `OratioContextus {lingua_documenti, census->dialectus, forma}` and
  calls `_contextu`; LIS records get `dialectus`/`forma` from the
  sentence's context; `ORATIO_COLUMNAE_LIS` gains `"dialectus",
  "forma"`, the line writer appends both as titles
  (`ORATIO_TITULI_DIALECTORUM`, `oratio_forma_titulus`).
  `instrumenta/oraculum.c` prints `via TAB FORMA TAB versus N TAB prosa
  M` after `DIALECTUS`. Gate `probatio_oratio_oraculum.c`: on the
  pinned CIRCSE file `formae[VERSUS] == 774` and `formae[PROSA] ==
  119`; LLCT dev all prosa; LIS header titles 34/35 are `dialectus`,
  `forma`; every LIS row's two last fields are in {classicus, medius}
  × {prosa, versus}. Plant = `_prose` → VERSUS (CIRCSE 893/0) → red.
- [ ] **Step 6: instruments.** `verba.c` and `arbor.c` build the
  context from the document: lingua = `oratio_resolutio_lingua_censu`
  (as today), dialectus = `census_partium.dialectus`, forma =
  `oratio_forma_documenti_censu(doc, NIHIL, NIHIL)`; call `_contextu`.
  Output unchanged today (nothing reads the context before (c)).
- [ ] **Step 7: acceptance, byte-identical.** `./oratio/census/metire.sh
  $S/basis_t38a.tsv -lites` before the change (already at
  `$S/lites_t37.tsv`) and after: `diff` of all rows except LIS and
  the new FORMA rows must be empty; LIS rows differ only by the two
  appended fields (`cut -f1-35` equal — the LIS data field count is
  the header's minus one).
- [ ] **Step 8: words, format, gates, commit.** `./oratio/quaere.sh
  contextus documentum` (documentum exists); formator on the touched
  files; `./oratio/compile_probationes.sh conllu forma resolutio
  oraculum`; commit via a scratch `commissio_t38a.py` with the explicit
  `viae` (pattern `commissio_t35e.py`), message in Latin naming the
  FORMA counts and the Perseus finding.

---

### Task T38 b: the table — `pondera.py`, `fixa/pondera.tsv`, the loader

**Files:** `oratio/census/pondera.py`, `oratio/probationes/fixa/pondera/
proba_lites.tsv` + `exspectata.tsv`, `oratio/probationes/fixa/pondera.tsv`,
`oratio_resolutio.h/.c`, `probatio_oratio_resolutio.c`,
`probatio_silva.py` (pythonica), `oratio/census/README.md`,
`oratio_resolutio.worklog.md` (create if absent).

**Produces (exact):**

```
pondera.tsv line 1: # pondera.tsv - tabula ponderum decretoris (T38 b, <date>): contentiones et rectae per (regula, dialectus, forma, folliculus, valor) ex ordinibus LIS plagularum EDITIBILIUM (la_circse, la_llct) solis; scrinium numquam; regeneratio: python3 oratio/census/pondera.py <lites.tsv> -scribere (causa nominata); fons <commit>
pondera.tsv line 2 (caput): regula	dialectus	forma	folliculus	valor	contentiones	rectae	permille
rows: umbra-subiectum-praecedente-proximo	-	-	numerus-capitis	singularis	NNN	RRR	PPP
      umbra-subiectum-praecedente-proximo	medius	-	numerus-capitis	singularis	…
      umbra-obiectum-verbi-praecedente-proximo	-	versus	positio-numerus	1+pluralis	…
      @subiectum+ante+proximo	-	-	-	-	…          (structural rows, "@" prefix: "#" is the comment marker)
      umbra-subiectum-praecedente-proximo	-	-	-	-	…           (plain contest rate)
```
Absent context or bucket = `-`. Permille = rectae × 1000 / contentiones,
integer. Rows with `contentiones < 20` are still WRITTEN (the loader
applies the minimum, so the table shows what was thin).

```python
# pondera.py — folliculi = DATA rows with the lattice's verdict as cause
FOLLICULI = [
    # nomen, columnae per latus (victa, victor), relationes, contextus, causa
    ('numerus-capitis', (('numerus-capitis',), ('numerus-capitis',)),
     ('subiectum', 'obiectum-verbi'), ('dialectus',),
     'T35 e / T36 d: catena vetans sortibus alternis +75, per corpora la_llct cadit 12 -> universalis, chartis condicionatus (decisio 54)'),
    ('positio-numerus', (('ante', 'numerus-victae'), ('ante-victoris', 'numerus-victoris')),
     ('subiectum', 'obiectum-verbi'), ('forma',),
     'T36 d: triplex ante x numerus-victae x numerus-capitis -40 retenta, exacta per corpus (CIRCSE 53:30, Perseus 11:24) -> condicionatus forma; RECUSANDUS si reticulum cum columna forma flexionem intra versum ostendit (T38 a inventum)'),
    ('directio-distantia', (('ante', 'distantia-victae'), ('ante-victoris', 'distantia-victoris')),
     ('caput',), ('forma',),
     'T36 d census arcuum (8,406): directio capitis +555 vetata corporibus versuum, directio x distantia +692'),
]
GRADUS_DISTANTIAE = lambda d: '1' if d == 1 else 'plures'   # lex -gradus reticuli
```
Crediting: for each LIS row with `aurum-caput-idem == 1` (a judged
arc) and `aurum-lectio` in {victor, victa, neutra}: the victor's rows
get `contentiones + 1` and `rectae + 1` iff `aurum-lectio == victor`;
the victa's rows likewise for `victa`; `neutra` = contest for both,
right for neither; `ignotum` skipped. Each side is credited at every
key level it has: (regula, dialectus, forma, folliculus, valor) for
every bucket whose relation matches the side's relation, (regula,
dialectus, forma), (structural key, dialectus, forma), (regula).
Structural key from the title: `umbra-<relatio>-<praecedente|sequente>
[-proximo]` → `@<relatio>+<ante|post>+<proximo|remoto>`; a title
without a direction part → `@<relatio>`. Context columns come from
the row (34, 35); the generator ALSO writes the `-` (unconditioned)
rows by summing over contexts. Refuses (exit 2, message) when no row
carries `la_circse` or none `la_llct`.

```c
/* oratio_resolutio.h */
nomen structura { i32 contentiones; i32 rectae; i32 permille; } OratioPondus;
/* OratioProgramma, appensum */
TabulaDispersa* pondera;   /* clavis 'regula TAB dialectus TAB forma TAB
                            * folliculus TAB valor' -> OratioPondus* */
/* tabulam ex textu legere: caput refusatur nomine (vitium.causa =
 * 'caput ponderum: <titulus>'), latitudo aliena refusatur cum linea */
b32 oratio_resolutio_pondera_legere (Piscina*, OratioProgramma*,
    chorda textus, OratioVocabulariumVitium* vitium);
/* onerare ut fiduciam: radix/oratio/probationes/fixa/pondera.tsv,
 * absens = NIHIL (decretor tabula vacua = exsecutor) */
```
`ORATIO_PONDERA_LIMEN` (default XX) read once in `_pondera_limen()`.

- [ ] **Step 1: docs first.** README rows (`pondera.py`, `pondera.tsv`);
  worklog note on the crediting law and the `-` rows.
- [ ] **Step 2: fixture by hand.** `fixa/pondera/proba_lites.tsv`: the
  LIS COLUMNAE header (35 titles) and twelve rows — six `la_circse`,
  six `la_llct` — built so that the hand count is short: victor always
  `umbra-subiectum-praecedente-proximo`, victa always
  `umbra-obiectum-verbi-praecedente-proximo`, `aurum-caput-idem` 1,
  `numerus-capitis` singularis on eight rows (gold victa on six, victor
  on two), pluralis on four (victor on three, victa on one), forma
  versus on the CIRCSE rows, prosa on LLCT, dialectus classicus /
  medius, `ante` 1 on all, `numerus-victae` pluralis on four of the
  singular rows. `exspectata.tsv` = the rows derived BY HAND in the
  worklog (write the derivation, then the file).
- [ ] **Step 3: generator.** `pondera.py` per the law above; `python3
  oratio/census/pondera.py oratio/probationes/fixa/pondera/proba_lites.tsv
  | diff - oratio/probationes/fixa/pondera/exspectata.tsv` empty.
  Pythonica gate: `probatio_silva.py` asserts the diff is empty and
  that a copy of the fixture without `la_llct` rows makes the
  generator exit 2.
- [ ] **Step 4: loader + gate.** `oratio_resolutio_pondera_legere` +
  file loader called where `_fiduciam_onerare` is; gate in
  `probatio_oratio_resolutio.c` part I c: load `exspectata.tsv` text →
  row count equals the file's data rows; the key `umbra-subiectum-
  praecedente-proximo\t-\t-\tnumerus-capitis\tsingularis` yields the
  hand numbers; a copy of the text with `rectae` renamed `recta` is
  refused with cause `caput ponderum: recta`; a row of seven fields is
  refused with its line. Plant (generator, `silva.planta` on the .py):
  credit `rectae` to the victa when `aurum-lectio == victor` → diff
  non-empty → red; revert.
- [ ] **Step 5: the real table.** `./oratio/census/metire.sh
  $S/lites_t38.tsv -lites` (post-(a) columns), `python3
  oratio/census/pondera.py $S/lites_t38.tsv -scribere` →
  `fixa/pondera.tsv`; header comment names the commit. Read the
  verb-number rows and check them against the GREX-SORS rows of T37's
  third run (singular pooled 910/1348 victa across six corpora; the
  table's CIRCSE + LLCT share of that: rows with those two corpora
  only) — numbers noted in the worklog. Before this step, run the
  lattice with the new column on the position × number triple:
  `./oratio/reticulum.sh $S/lites_t38.tsv … -ubi forma=versus -initium
  ante,numerus-victae,numerus-capitis -greges initium` — if the flip
  is INSIDE versus (Seneca vs Vergil/Ovid), the `positio-numerus`
  bucket's forma conditioning is refused by decision 58 and its row in
  `FOLLICULI` is commented out with that number as the cause.
- [ ] **Step 6: words, format, gates, commit.** `./oratio/quaere.sh
  pondus folliculus`; formator; `./oratio/compile_probationes.sh
  resolutio` + `./tools/compile_tests_fontes_generare.sh`-free (no new
  probatio file); pythonica gate; commit `commissio_t38b.py`.

---

### Task T38 c: the decoder

**Files:** `oratio_registrum.h/.c`, `oratio/grammatica/oratio.canon`,
`probatio_oratio_registrum.c`, `oratio_resolutio.h/.c`,
`probatio_oratio_resolutio.c`, `oratio_oraculum.h/.c`,
`instrumenta/oraculum.c`, `probatio_oratio_oraculum.c`,
`pythonica/silva.py`, `probatio_silva.py`, `fixa/computus/*`,
`oratio_resolutio.worklog.md`, `oratio/census/README.md`.

**Produces (exact):**

```c
/* oratio_registrum.h, APPENSA */
ORATIO_VOCABULUM_HABITUS,          /* INDEX: OratioHabitus (T38 c), post clausula-causa */
nomen enumeratio { ORATIO_HABITUS_COACTUS = 0, ORATIO_HABITUS_ORDINATUS,
    ORATIO_HABITUS_APERTUS, ORATIO_HABITUS_NUMERUS } OratioHabitus;
externus constans character* constans ORATIO_TITULI_HABITUUM[];   /* "coactus" "ordinatus" "apertus" */
ORATIO_DECISIO_DECRETUM,            /* titulus "decretum" (ORATIO_TITULI_DECISIONUM appensum) */
ORATIO_ALTERNA_CAUSA_PRAEOCCUPATA,  /* umbra gradu priore iam scripta (T34) - candidata decretoris */
ORATIO_ALTERNA_CAUSA_DECRETA,       /* petitio a decretore deposita; victor "decretor" */
/* GENERA_ORATIONIS: vocabulum 9..18 (X loci); genera sequentia +1
 * (interpunctio 19, numerus 23, analyses 27.., umbra 173, clausula 182,
 * alterna 186); loci CXC -> CXCI; sigillum novum in canon <optio> */

/* oratio_resolutio.h */
nomen structura {
             i32 dependens;         /* ordinalis vocabuli cellae */
             i32 caput;             /* ordinalis vocabuli capitis electi (carrier) */
          chorda regula_electa;
          chorda regula_cedens;     /* vacua si nulla cessit */
             s32 ante;              /* dependens ante caput: 1 | 0 */
             s32 numerus_capitis;   /* -I absens */
             s32 numerus_lectionis; /* lectionis electae */
             i32 distantia;
             i32 gradus_electae;    /* scala: I exacta .. V fiducia; 0 nulla */
             i32 gradus_cedentis;
             i32 pondus_electae;    /* permille; -I nullum */
             i32 pondus_cedentis;
             s32 habitus;           /* OratioHabitus */
             b32 mutata;            /* decretor petitionem stantem mutavit */
    MateriaNodus* umbra_electa;
    MateriaNodus* analysis_electa;
    MateriaNodus* umbra_cedens;     /* NIHIL si nulla */
    MateriaNodus* analysis_cedens;
} OratioDecretum;
/* OratioResolutioCensus, APPENSA */
    i32 cellae_contestatae;
    i32 decretae;                        /* cellae mutatae */
    i32 habitus[ORATIO_HABITUS_NUMERUS]; /* cellae per habitum */
    i32 alternae_praeoccupatae;
    Xar* decreta;                        /* OratioDecretum per cellam contestatam; NIHIL ante */
```

Decoder internals (`oratio_resolutio.c`, one block "DECRETOR (T38 c)"):

```c
nomen structura {
    MateriaNodus* umbra;      /* umbra carrier */
             i32  v, a;       /* carrier vocabulum, analysis */
             i32  w;          /* implens vocabulum */
    MateriaNodus* analysis;   /* nodus lectionis implentis (referentia) */
          chorda titulus;     /* regula proponens */
             s32  causa;      /* -I stans | RECUSATA | REVOCATA | PRAEOCCUPATA */
             i32  dependens;  /* _dependens_ordinis */
             s32  relatio;
             i32  pondus;     /* permille; -I nullum */
             i32  gradus;     /* scala */
             s32  numerus_capitis, numerus_lectionis;
             i32  distantia;  b32 ante;
             b32  accepta;
} Candidatus;
interior b32 _decretor (Cursus*, MateriaNodus* sententia, b32* explicita,
    Decisio* decisiones);                  /* post gradus, ante legem umbrarum */
interior i32 _candidatos_colligere (Cursus*, MateriaValor elementa, Xar* candidati);
interior i32 _pondus_candidati (constans Cursus*, constans Candidatus*, i32* gradus);
interior b32 _cyclum_facit (constans Xar* candidati, i32 ne, i32 caput, i32 dependens);
interior b32 _decretum_scribere (Cursus*, MateriaValor elementa, Candidatus* electus,
    Candidatus* cedens);                   /* impletio, auctor, alterna decreta, permutatio, habitus, decisio */
```
`_pondus_candidati` ladder, finest first, each rung answering only
with `contentiones >= _pondera_limen()`: 1 `(titulus, dialectus,
forma, folliculus, valor)` for the bucket of the candidate's relation
(`numerus-capitis` and `positio-numerus` for subject/object-of-verb,
`directio-distantia` for caput); 2 `(titulus, dialectus, forma)`; 3
`(@structura, dialectus, forma)`; 4 `(titulus, -, -, -, -)`; 5
`_fiducia_regulae` (auctores.tsv); none → `-I`. Bucket values: the
carrier reading's `numerus` slot; `ante` = `w < v`; the filler
reading's `numerus`; distance `|w − v|` graded `1 | plures`.

Greedy: candidates of contested cells (a cell = `dependens` with ≥ 2
distinct `(umbra, analysis)` candidates) plus every candidate of an
umbra that appears in a contested cell; sorted by `pondus` descending,
standing (`causa == -I`) first among equals, then list order (stable
insertion sort over the small Xar — never qsort). Accept when: the
umbra is not yet accepted; the dependent has no accepted head and no
UNCONTESTED standing claim (a head from outside the contested set);
`!_cyclum_facit`; the explicit-class law holds (the filler word is not
`explicita[]` with a first reading other than `analysis`). Habitus per
cell: COACTUS when exactly one candidate of the cell passed the
constraint checks, ORDINATUS when the accepted one's `pondus` >
runner-up's, APERTUS when equal (standing stands) or when every
candidate fell (the cell keeps the executor's state).

`_decretum_scribere` when the accepted candidate is not the standing
one: previous filling → `_alternam_addere(cursus, elementa, umbra,
w_old, b_old, titulus_old, "decretor", ORATIO_ALTERNA_CAUSA_DECRETA,
fides_old)`; `materia_nodus_reponere(umbra, ORATIO_UMBRA_IMPLETIO,
materia_valor_referentia(analysis), MATERIA_LOCUS_REFERENTIA)`;
`_auctorem_umbrae_scribere` (existing helper at the `Impletio` block)
with the winner's title; the filler reading first in its word through
the SAME code the stage uses (factor the `materia_nodus_lista_permutare`
+ `oratio_partes_compendia_reponere` sequence at l. 2736 into
`_lectionem_promovere(cursus, elementa, w, analysis)` and call it from
both places); an umbra whose standing claim lost and got no
replacement: `materia_nodus_reponere(… MATERIA_VALOR_NIHIL …)` (written
absence, T33 b) and the alterna decreta. On the dependent word:
`habitus` slot written on every contested cell; `decisiones[dependens]
= {ORATIO_DECISIO_DECRETUM, "decretor"}` only when `mutata`.
`praeoccupata`: at the T34 site (l. 2392–2400) after counting
`recusatae_scriptae`, record `_alternam_addere(cursus, *elementa, umbra,
w, b, *titulus, _auctor_petitionis(umbra, impletiones),
ORATIO_ALTERNA_CAUSA_PRAEOCCUPATA, fides)` unless the same `(w, b)` is
already the umbra's filling or an alternative (dedupe by walking
`alternae` — the harmless re-proposals of T34's probe stay out).

Placement in `_sententiam_resolvere`: after the `gradus` loop and
BEFORE `_ligationes_ad_caput_sequi`: `(!_decretor_activus() ||
_decretor(cursus, sententia, explicita, decisiones))`.
`_decretor_activus()` = `ORATIO_DECRETOR` unset or not "0".
`ORATIO_PONDERA_SINE=<folliculus>` makes rung 1 skip that bucket.

Oracle (`oratio_oraculum.c`): after `oratio_resolutio_applicare_contextu`
the sentence's `census_resolutionis.decreta` is judged per record:
`aurum-electio` = `electa` when the chosen arc AND reading are gold
(as LIS judges a victor: gold head equals the carrier word for the
verb star, equals the filler for head umbrae, and the reading's case
equals `Case=`), `cedens` when the displaced one is, `neutra` when
neither, `ignotum` when the dependent has no gold arc; sums into
`census->decretor_*`. Rows (`instrumenta/oraculum.c`, `-decreta` flag,
with `-machina`): `via TAB COLUMNAE TAB DECRETUM TAB sententia
dependens caput regula-electa regula-cedens ante numerus-capitis
numerus-lectionis distantia gradus pondus-electae pondus-cedentis
habitus aurum-electio thesaurus dialectus forma` then one `DECRETUM`
row per record; always the summary `via TAB DECRETOR TAB cellae
contestatae mutatae coacti coacti-recti ordinati ordinati-recti aperti
aperti-recti`. Human `-decreta` without `-machina`: the wrong ones
(`cedens | neutra`) as `sententia: dependens <- caput (electa R1 p1 /
cedens R2 p2) gradus g habitus h`.

Python: `Oratio.vocabula()` rows gain `habitus`; `ORATIO_PARTITIO` gains
`'decretum'`; the oracle partition kind `decretum` appended to
`ORATIO_ORACULUM_PARTITIO_*` with `partitio_verba/primaria` (a word
decided by the decoder counts there, not under `impletio`).

- [ ] **Step 1: docs first.** Worklog "T38 c: decretor" with the
  ladder, the greedy order and the write-back; README rows
  (`-decreta`, DECRETUM/DECRETOR).
- [ ] **Step 2: registry + canon.** Append the slot and enum values,
  shift GENERA offsets, titles arrays, canon `<liberum
  nomen="habitus" maximum="1"/>` + `<attributum nomen="habitus"
  genus="textus" nota="T38 c: speculum loci habitus (titulus
  OratioHabitus)"/>` + `<elementum nomen="habitus" intra="vocabulum"
  textus="verum" nota="T38 c (decisio XL, XLVIII): OratioHabitus - 0
  coactus 1 ordinatus 2 apertus; absens = cellula non contestata"/>`;
  the decisio elementum's nota gains `3 decretum`; the registry seal's
  `<optio>` gains the new seal (the gate prints it; T33's `305a78a5`
  stays for old documents). `probatio_oratio_registrum.c`: loci
  count CXCI, `ORATIO_VOCABULUM_HABITUS` title `habitus`, materia
  round trip with a `habitus` written. Regenerate the computus golden
  ONLY at step 6 (the projection changes only when a habitus is
  written).
- [ ] **Step 3: praeoccupata.** The recording at the T34 site; census
  `alternae_praeoccupatae`. Gate part II e (T34) extended: `Puella
  currit miles pugnat` now carries ONE alterna with cause praeoccupata
  on `currit`'s subject umbra (socius `miles`), and `census.
  alternae_praeoccupatae == 1`; the T34 laws unchanged.
- [ ] **Step 4: the pass.** `_decretor` and helpers; census counters;
  `OratioDecretum` records. Gate part II f: the inline two-rule
  programme of part II d (object rule then subject rule, real trust)
  plus an inline table via `oratio_resolutio_pondera_legere`:
  `umbra-obiectum-verbi-praecedente-proximo\t-\t-\tnumerus-capitis\tsingularis\t100\t70\t700`
  and `umbra-subiectum-praecedente-proximo\t-\t-\tnumerus-capitis\tsingularis\t100\t30\t300`;
  `Puella bellum videt.` → `bellum` first reading accusative, `videt`'s
  object umbra filled with `bellum`, subject umbra filled with
  `Puella`, both words `habitus` ordinatus, `bellum` `decisio`
  decretum / `auctor` decretor, `census.decretae == 2`,
  `census.habitus[ORDINATUS] == 2`, two `decreta` records with
  `gradus_electae == 1`; the SAME programme with an EMPTY table →
  tree byte-identical to the executor's (STML projection equal) and
  `census.habitus[APERTUS] == 2`; `ORATIO_DECRETOR=0` (setenv in the
  gate, unset after) → identical to the executor's, `cellae_contestatae
  == 0`. Corpus invariant on Hilarius: every filling points at a
  reading of its case/number/gender (existing probatio law, must stay
  green). PLANT: the ladder walked coarsest-first (rung 5 before rung 1)
  → both rules score their trust (705 > 555), `bellum` stays subject →
  red on the accusative law; revert → green.
- [ ] **Step 5: oracle rows.** Judgement of `decreta`, `DECRETUM` /
  `DECRETOR` rows, `-decreta`. Gate `probatio_oratio_oraculum.c`: on
  the pinned CIRCSE file `DECRETOR.contestatae == coacti + ordinati +
  aperti`; DECRETUM rows count == contestatae; every row's `habitus`
  is one of the three titles and `aurum-electio` one of four; a
  `reticulum.sh $tsv -genus DECRETUM -aurum aurum-electio -sortes
  thesaurus` call on the gate's own machine output exits 0 (the rows
  are lattice-readable). Forced accuracy pin at birth:
  `DECRETOR_COACTI_PINNA_*` = the run's permille per pinned file, only
  rising. Plant = `aurum-electio` judged on the arc alone (reading
  ignored) → the CIRCSE count of `electa` changes → red against the
  hand-checked number of one sentence (part IX prints it first).
- [ ] **Step 6: Python + golden.** `silva.py` fields; pythonica gate
  asserts `habitus` present on `Oratio('Puella bellum videt.').
  vocabula()[1]` and `partitio()['decretum'] == 1`. Computus golden
  regenerated `COMPUTUS_SCRIBERE=1 ./oratio/computus.sh` with cause
  "T38 c habitus on contested cells" noted in the worklog.
- [ ] **Step 7: byte-identical check with the decoder OFF.**
  `ORATIO_DECRETOR=0 ./oratio/census/metire.sh $S/off_t38c.tsv` vs the
  (a) baseline: every non-LIS row identical (LIS rows gain no field
  here; the praeoccupata alternatives add LIS rows — `lites ==
  alternae` still holds, and the count of new rows is noted).
- [ ] **Step 8: words, format, gates, commit.** `./oratio/quaere.sh
  decretor praeoccupatus electus cedens scala`; formator; registrum,
  resolutio, oraculum, computus, pythonica gates; commit
  `commissio_t38c.py`.

---

### Task T38 d: the run

**Files:** spec §7 "As built — T38", `oratio/CLAUDE.md` T38 paragraph,
`oratio/census/README.md`, worklogs, `oratio/recusata.md` (R14 / R20
re-measured), pins in `probatio_oratio_oraculum.c`, memory
`oratio-project.md` + `MEMORY.md` line, this table.

- [ ] **Step 1: the prediction, written first** (worklog, before any
  run): from the verb-number GREX rows, cells flipped toward the
  object on singular verbs: ITTB ≤ +98, Perseus ≤ +109, PROIEL ≤ +112,
  LLCT ≤ +59, UDante ≤ +37, CIRCSE ≥ −7; expected direction only
  (T32 f); charters' arcs expected flat (proposal residual, T32 b).
- [ ] **Step 2: the run.** `./oratio/census/metire.sh $S/decretor.tsv
  -lites -decreta` on nine files (env `ORATIO_DECRETOR` unset) and the
  baseline `ORATIO_DECRETOR=0`; `compara.py`, `notae.py`,
  `relationes.sh`, and the DECRETOR rows per file; the lattice over
  the decisions: `./oratio/reticulum.sh $S/decretor.tsv -genus DECRETUM
  -aurum aurum-electio -sortes thesaurus -praeter sententia,dependens,
  caput,regula-electa,regula-cedens,pondus-electae,pondus-cedentis
  -profunditas 2` — the next bucket, if any, named in the as-built.
- [ ] **Step 3: levers, each alone.** `ORATIO_PONDERA_SINE=numerus-
  capitis`, `=positio-numerus`, `=directio-distantia`;
  `ORATIO_PONDERA_LIMEN=40`; each on nine files, exact counts in the
  worklog (the gate's permille resolution is not the lever's).
- [ ] **Step 4: pins.** Every pin that rose moves up; every pin that
  fell is listed with the cause for Fran's decision before the commit;
  `DECRETOR_COACTI_PINNA_*` set from the run.
- [ ] **Step 5: walls.** R14 (`ORATIO_OBIECTUM_INFINITA=participium`)
  re-measured under the decoder; R20 recreated as the grammar variant
  of T32 f (subject rules split into a plural copy before the object
  rules) in the scratchpad and measured under the decoder;
  `recusata.md` rows updated with the new numbers (kept or lifted =
  Fran's decision). R11 (loose object tier) and R19 (superseded by the
  decoder) named, not re-measured.
- [ ] **Step 6: records + commit.** Spec as-built (deviations, numbers,
  the lattice-over-decisions finding), `oratio/CLAUDE.md` T38
  paragraph, README, worklogs, memory, plan status; ledger:
  desideratum 01M24Z45XK note (impletum if the pins say so), decretum
  01M2HGPT52 note; commit `commissio_t38d.py`.

---

## Self-review (done at writing)

Spec coverage: candidates and cells → c step 3–4; score and table → b;
declared context → a; constraints then order → c step 4; decode and
write-back → c step 4; placement and switches → c step 4; the decision
row → c step 5; measurement and the bet → d; gates → each tranche;
tranches → the four tasks; left out → not planned (named in the spec).
Type consistency: `OratioContextus`, `oratio_resolutio_applicare_contextu`,
`OratioPondus`, `oratio_resolutio_pondera_legere`, `OratioDecretum`,
`OratioHabitus`/`ORATIO_TITULI_HABITUUM`, `ORATIO_ALTERNA_CAUSA_PRAEOCCUPATA`
/ `_DECRETA`, `ORATIO_DECISIO_DECRETUM` used with the same names in a, b,
c, d. Open at writing, decided in (b) step 5 by measurement: whether the
`positio-numerus` bucket keeps its `forma` conditioning.
