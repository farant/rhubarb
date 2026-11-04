# gabc Plan, part 1 — the reader, the converter into carmen, auditio

**Ledger:** desideratum 01M62BR9MAJNCSMEGRAP4496NQ (→ tractum at T1);
a parcum "gabc" is born at T1 with one opus per task below.

**STATUS: DRAFT 2026-11-03.** Written after Fran approved the spec
(10ada5ce). Nothing executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention, no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note, as in `project-specs/musicxml-plan.md`.

**Goal:** `gabc` reads any gabc text into a chant model that keeps the
full notation, with located diagnostics; `gabc_carmen` turns that
model into carmen text that `carmina_legere` accepts, with a Solesmes
rhythm, a report, and a note→source link table; `auditio -gabc`
exposes both; `musica/fontes/salve_2435.gabc` converts to the book's
`salve` note for note, and `salve.py` retires.

**Part 2 (not this plan):** the glyph library `neumae_ductae` and the
vicus pane `antiphonale` (spec §IV–V, G13–G18). Its plan is written
when part 1 lands, after a survey of how a vicus pane is built — the
pane plan depends on vicus internals this plan does not touch.

**Architecture:** spec §0–III. `lib/gabc.c` is a hand-written
scanner over the bytes (gabc is not XML; no materia), producing
`GabcCantus`: header pairs, syllables, and per syllable a flat list
of elements (notes, clefs, bars, spacing, breaks, custos, opaque
marks). Pitch is resolved to a scale step relative to do at read time
(G9); shapes are classified by one function (G4). `lib/gabc_carmen.c`
writes carmen STML text plus a report and a link table, in the shape
of `lib/musicxml_carmen.c`.

**Tech stack:** C89 via latina.h, house flags, credo, piscina,
chorda, chorda_aedificator, `carmen.h` (converter and its tests
only), filum, pythonica (`silva.commissio`, `silva.planta`).

**Spec:** `project-specs/gabc-spec.md` (decisions G1–G18 in §0).
Precedents: `project-specs/musicxml-plan.md` (task shape, header
STOP, acceptance test); `lib/musicxml_carmen.c` (report, strict,
MML lengths and `&` ties); `probationes/probatio_musicxml_carmen.c:
639-690` (book parity test via `carmina_ordinaria` /
`carmina_invenire`); `musica/fontes/salve.py` (the rules being
ported); `carmina.stml:657-671` (the parity target);
`tools/auditio.c:549-640` (the `-musicxml` source path to mirror).

## Global Constraints

- **Latin throughout** (docs and worklogs English). `gabc` needs a
  glossarium line (`oratio/glossarium.stml`, class
  `ignotum-permissum`, "nomen formae, ut plist") before the first
  commit that uses it; check every new word with `./oratio/quaere.sh`
  first. Neume names (pes, clivis, torculus, porrectus, climacus,
  scandicus, salicus, quilisma, oriscus, virga, punctum, stropha,
  episema) are Latin already — check each anyway (some may need a
  gloss).
- **Every latina.h macro is a forbidden identifier** (`nomen`, `per`,
  `casus`, `magnitudo`, `imus`, every Roman numeral 0–3999). Watch:
  pitch letters `c` `d` `i` `l` `m` `v` `x` as single-letter
  identifiers are Roman numerals or near them — never name a variable
  by a pitch letter; header key `name` → field `titulus`.
- **`chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED** —
  scale steps go below do (negative), so `gradus` is `s32`; any
  "not found" sentinel is `s32`.
- **The reader does not include `carmen.h`** (G1). It never
  interprets rhythm (G8) or absolute pitch (G9).
- **Totality** (G6): any bytes → a `GabcCantus` (NIHIL only if memory
  fails) with diagnostics; no crash, no hang; tested under ASan.
- **Nothing is dropped silently** (G5): an unmodeled construct is an
  opaque mark with its source text; an unparseable one is a located
  diagnostic, and reading continues.
- **New C files through `./silva/scribe.sh`** (worktree rule); new
  `lib/` files need the aedilis tag in their header and
  `./tools/compile_tests_fontes_generare.sh` re-run (generated list,
  never hand-edited).
- **Root suites** (`probationes/probatio_gabc*.c`) run under gate
  `radix` (`./compile_tests.sh gabc`).
- **Exit 2 from any runner = NOTHING RAN.** Never read `$?` after a
  pipe.
- **Every new test section is born red by a plant that COMPILES**
  (`silva.planta`), recorded in the task's Executed note and the
  worklog.
- **Commit through `silva.commissio`**, detached (message files in
  the scratchpad): `python3 build/plantae_n1/detach.py "python3 -u
  build/plantae_s4/committere.py <nuntius> <portae> <viae>" <log>`;
  tree FROZEN while gates run.
- **Downloaded files are untrusted** (T9): each in its own directory
  under `build/gabc/`; scripts elsewhere; Python `-I`; paths as
  arguments.
- **Headers are reviewed by Fran before implementation**: T1
  (`gabc.h`) and T6 (`gabc_carmen.h`) STOP for review.

## Review Focus

Five input classes the spec implies but no feature list names; each is
pinned by a test in the named task.

1. **Accidental scope** — `ix` flats the i position until the end of
   the word or the next bar, whichever comes first; a `y` natural
   cancels it inside that span; the next word starts clean; an
   accidental at one position does not touch the same letter an
   octave away. (T3, `_alterationis_ambitum_probare`. Check the rule
   against the current Gregorio docs before pinning — spec AUDIENDA.)
2. **Clef changes mid-chant** — a clef change inside a syllable or
   between syllables re-resolves only the notes after it; `cb3`
   (b-flat clef) makes ti flat for the whole staff until the next
   clef; a custos before a clef change is resolved with the NEW clef
   (it shows the next note). (T3, `_clavem_mutatam_probare`.)
3. **Text without notes and notes without text** — `*()` (text, empty
   notes), `(::)` (bar, no text), a syllable whose notes continue a
   melisma with no text, a word split across a `z` line break, an
   elision `<e>i</e>`, accented letters and æ/œ: each keeps its place
   in the model, and in carmen verba they behave as `salve.py` does
   (accents stripped, empty text = melisma `_`, bars carry no
   syllable). (T2 `_textum_sine_notis_probare`; T7
   `_verba_variae_probare`.)
4. **Malformed and truncated input** — unclosed `(`, unclosed `<i>`,
   `[` without `]`, no `%%` at all, `%%` twice, a UTF-8 BOM, CRLF line
   endings, a NUL byte, a file that ends inside a header value: each
   gives a model plus a diagnostic at the right line:col; CRLF and BOM
   give NO diagnostic. (T4, `_vitia_formae_probare`, plus the ASan
   fuzz loop.)
5. **Unison ties and quilisma at the edges** — unisons within one
   syllable are tied even across neume groups (`gh/hg`), never across
   a syllable or a bar; a quilisma as the first note of a syllable
   lengthens nothing (no previous note in the syllable, as
   `salve.py`). (T7, `_rhythmum_marginum_probare`.)

---

## Task T1: Header draft — `gabc.h`, glossarium (STOP: Fran reviews)

**Files:**
- Create: `include/gabc.h` (types and prototypes only)
- Modify: `oratio/glossarium.stml` (`gabc`, and any neume word the
  checker rejects)

**Interfaces — Produces** (proposed; final names as Fran approves —
later tasks use whatever T1's Executed note records):

```c
/* <aedilis corpus="lib/gabc.c"/> */

nomen enumeratio { GABC_VITIUM, GABC_MONITUM } GabcGravitas;
nomen enumeratio { GABC_SINE_SEPARATORE, GABC_CAPUT_MALUM,
    GABC_PARENTHESIS_APERTA, GABC_NOTA_IGNOTA, GABC_TEXTUS_MALUS,
    GABC_SIGNUM_APERTUM, GABC_ALTERATIO_SINE_NOTA,
    GABC_DIVISIO_IN_NEUMA, GABC_CLAVIS_MALA, GABC_NON_UTF8
} GabcVitiumGenus;
nomen structura { GabcGravitas gravitas; GabcVitiumGenus genus;
    i32 linea; i32 columna; chorda causa; } GabcVitium;

nomen enumeratio { GABC_EL_NEUMA, GABC_EL_CLAVIS, GABC_EL_DIVISIO,
    GABC_EL_SPATIUM, GABC_EL_FRACTIO, GABC_EL_CUSTOS,
    GABC_EL_SIGNUM_OPACUM } GabcElementiGenus;
    /* one element = one of these; a NEUMA holds notes [prima, prima+numerus) */

GabcCantus* gabc_legere (chorda datum, Piscina* piscina);
    /* NIHIL only if memory fails */
b32 gabc_successus (constans GabcCantus* cantus);      /* no VITIUM */
b32 gabc_vitia_scribere (constans GabcCantus* cantus,
    constans character* via, ChordaAedificator* exitus);
    /* 'via:linea:columna: monitum|vitium: causa' per line */
b32 gabc_tabulam_scribere (constans GabcCantus* cantus,
    ChordaAedificator* exitus);       /* canonical dump (T2 grammar) */
GabcFigura gabc_figura (constans GabcCantus* cantus,
    constans GabcElementum* neuma);   /* G4 classifier (T5) */
```

plus the structs that hold spec §II: `GabcCantus` (header pairs
`GabcCaput {clavis, valor}` in order + typed `titulus`, `modus`,
`differentia`, `annotationes[2]`, `stilus_initialis`; `syllabae`;
`notae` (one flat array for the whole chant, so elements index into
it); `vitia`), `GabcSyllaba` (text runs `GabcTextus {textus, stilus
bits, genus: plain / special / translation / above-staff /
verbatim}`, `verbum_incipit`, the centred-vowel range, `elementa`),
`GabcElementum`, `GabcNota` (`positio` a..p as 0.., `gradus` s32 step
from do, `alteratio` -1/0/+1, `formae` bitmask, `mora` 0..2,
`episema` + digit, `ictus` + digit, `linea`/`columna`), `GabcFigura`
(enum; `_IGNOTA` member). Arrays as pointer + `i32 numerus_*`
(carmen's and musicxml's shape — consumers index directly).

- [x] **Step 0: Ledger** — desideratum …96NQ → tractum; parcum
  "gabc" with opera T1–T10 (`addere`, `intra` the music region);
  `praeco status "gabc T1: header for fran"`.
- [x] **Step 1: Measure** — list every construct in
  `musica/fontes/salve_2435.gabc` and check each has a home in the
  draft structs; read the current Gregorio gabc reference (notes,
  text markup, header fields) and record in the Executed note: the
  pitch letter range (a–n, p?), the accidental-scope rule, and the
  `z`/`Z`/`z0`/`z+`/`z-` meanings. These pin T2–T4.
- [x] **Step 2: Draft `include/gabc.h`** — head comment with USUS
  (legere → successus → walk syllables/elements/notes → figura), the
  totality promise, "nothing dropped silently" (G5), what `gradus`
  means (step from do; the clef places do), and that rhythm and
  absolute pitch are NOT here (G8, G9). Every field commented in
  Latin.
- [x] **Step 3: Glossarium** — `gabc` and rejected words;
  `./oratio/quaere.sh gabc` finds it.
- [x] **Step 4: STOP — show Fran the header.** Record his changes in
  the Executed note.
- [x] **Step 5: Commit** (no gates beyond lint): header, glossarium.

**Executed 2026-11-03 (Fran approved the header as drafted).**
Grammar source = Gregorio's lexer `src/gabc/gabc-notes-determination.l`
(master, read 2026-11-03), not only the docs. Facts that pin T2-T4:
- pitch letters **a-n and p** (`o` is the oriscus; docs say a-m);
  upper case = inclinatum with optional direction digit 0/1/2;
- accidentals `x` `y` `#`, soft `X` `Y`, parenthesised `x?` `y?` `#?`;
  musica ficta `r6` `r7` `r8` (note only); scope (Gregorio has none):
  end of word, next bar, or another sign on the same position;
- bars: `` ` `` `` `0 `` `` `? `` virgula, `^` `^0` divisio minimis,
  `,` `,0` `,?` minima, `;` minor, `:` maior, `:?` maior punctata,
  `::` finalis, `,1-8` `;1-8` dominican;
- breaks `z` (justified) `Z` (ragged), `+`/`-` force/suppress custos;
  `z0` custos inside a line; `g+` manual custos; `[nocustos]` opaque;
- spaces `!` `/0` `/!` `/` `//` ` ` `/[n]` (n may be negative), `!/`
  `!//` `!/[n]` unbreakable; `@` before a note = fusion;
- shorthand `hvv` `hvvv` `hss` `hsss` (also with `<` `>`) expand to
  2-3 notes; `=` linea; `r` cavum, `R` linea punctum, `r0` both,
  `r1`-`r5` signs above;
- `{` `}` (polyphony boxes), `[[` `]]`, `[cs:]` `[cn:]` `[ob:]` `[ub:]`
  `[ocb:]` `[ocba:]` `[nm1-9]` `[gm]` `[em]` `[altm]` `[nv:]` `[gv:]`
  `[ev:]` `[alt:]` `[oll:]` `[ull:]` `[shape:]` `[ou]slur:` `[ou]h:`
  `[hl:0/1]` `[ll:0/1]` -> opaque marks (alt kept as text above too is
  a T4 choice);
- header values may span lines: omit `;`, end with `;;`; `staff-lines`
  2-5 sets the highest legal pitch; `def-m0..9` macros (header only).
Names as in the header: `GabcCantus`, `GabcSyllaba`, `GabcTextus`,
`GabcElementum` (flat, per-kind fields), `GabcNota`, `GabcFigura`,
`gabc_figurae_titulus`; no `%%` = whole text is body (monitum
`GABC_SINE_CAPITE`). Gotcha: `<i>`-style tag names in C comments are
parsed as STML annotations (examen "annotatio malformata") - the
header writes "signum i". 17 glossary words added (chant terms, `gabc`,
`nlba`).

---

## Task T2: Reader core — header, syllables, plain text, notes, groups, clefs, bars, the dump

**Files:**
- Create: `lib/gabc.c`, `probationes/probatio_gabc.c`
- Create fixtures in `probationes/fixa/gabc/` (hand-written, one
  feature each, line 1 a `%` comment saying what it pins):
  `caput.gabc`, `syllabae.gabc`, `neumae.gabc`, `claves.gabc`,
  `divisiones.gabc`, `textus_sine_notis.gabc`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: T1 header.
- Produces: `gabc_legere` for the core syntax (plain text, pitches
  without modifiers, groups split by space / `/`, clefs c1–c4 f1–f4,
  the five bars); `gabc_tabulam_scribere` grammar, one TSV line per
  item, in order:

```
caput	<clavis>	<valor>
syllaba	<index>	<verbum_incipit 0|1>	<textus as displayed>
clavis	c|f	<linea 1-4>	<b-flat 0|1>
neuma	<prima>	<numerus>	<figura or ->
nota	<positio letter>	<gradus>	<alteratio>	<formae letters>	<mora>	<episema>	<ictus>	<linea>:<columna>
divisio	virgula|minima|minor|maior|finalis
```

  (T3–T5 add columns' content, not new line kinds, except `spatium`,
  `fractio`, `custos`, `signum` in T4.)

- [x] **Step 1: Measure** — time `gabc_legere` target: none needed
  yet; confirm `salve_2435.gabc` header ends with `%%` on its own line
  and the body starts with `(c4)`.
- [x] **Step 2: Failing tests** in `probatio_gabc.c`, each comparing
  the dump of a fixture to an expected string in the test:
  - `_caput_probare` — `name:Salve;` `mode:5;` `annotation:Ant.;`
    twice, an unknown `foo:bar;`, a `%` comment line: pairs in order,
    `titulus` = "Salve", `modus` = 5, both annotations, `foo` kept.
  - `_syllabas_probare` — `Sal(c)ve(e) Re(g)gi(h)na(g)`: five
    syllables, `verbum_incipit` 1 0 1 0 0.
  - `_neumas_probare` — `(gh)` one group of two notes; `(g h)` two
    groups; `(gh/hg)` two groups; positions and `gradus` under c4
    (j = 0, h = -2, g = -3).
  - `_claves_probare` — `(c4) ... (f3) ...`: notes after `f3` resolve
    with fa on line 3; `gradus` values pinned for one note each.
  - `_divisiones_probare` — `` ` `` `,` `;` `:` `::` each as its
    element; a bar inside a group `(g;h)` → `GABC_DIVISIO_IN_NEUMA`
    monitum at its column.
  - `_textum_sine_notis_probare` (Review Focus 3) — `*()`, `(::)`,
    `ve(gh) (hg)` — empty-notes syllable kept; bar with no text kept;
    textless notes after a syllable are their own syllable with empty
    text, `verbum_incipit` 0.
  - `_salve_legere_probare` — the real file reads with
    `gabc_successus` VERUM and 0 VITIUM; syllable count pinned (count
    it in Step 1).
- [x] **Step 3: Run** `./compile_tests.sh gabc` — red (nothing
  implemented; exit must NOT be 2).
- [x] **Step 4: Implement** the scanner: header until a line that is
  exactly `%%` (header values end at `;` and may span lines);
  body: text up to `(`, notes up to `)`. Two passes are fine: scan
  into elements, then resolve `gradus` (needs the clef in force).
- [x] **Step 5: Run** — green. **Plant:** resolve `gradus` against
  the first clef only → `_claves_probare` FRACTUM.
- [x] **Step 6: Commit** (gate `radix`), worklog `lib/gabc.worklog.md`
  started.

**Executed 2026-11-03.** Step 1: `salve_2435.gabc` header ends
with `%%` on its own line (line 6), body starts `(c4)`; counted with
shell tools, not the reader: **141 syllables** (18 bar-only), **145
notes** - both pinned in `_salve_legere_probare`. Red against a stub
(exit 1, not 2); green on the first full run. **Plant:** `gradus`
resolved against the clef of syllable 0 only -> `_claves_probare`
FRACTUM, restored, green (`silva.planta`, gate `radix`, filtrum
`gabc`).
Decisions made while implementing (worklog `lib/gabc.worklog.md`):
- the scanner **consumes** all of Gregorio's note syntax now (so the
  Salve reads with 0 diagnostics) but **records** only notes, neumes,
  clefs, bars and accidentals; shape/rhythm signs (T3), spaces,
  breaks, custos, brackets, `{` `}`, nabc (T4) are consumed and not
  yet kept - a G5 debt T4 closes, said in the `lib/gabc.c` header;
- dump `divisio` line has three columns more than drafted:
  `divisio <genus> <numerus> <alta> <parenthesis>`; all line kinds
  (incl. `alteratio` `spatium` `fractio` `custos` `signum`) already
  print, grammar fixed in the `probatio_gabc.c` header comment;
  `formae` letters fixed now: `I v V s o O w W r R = q ~ < > - @`;
- `verbum_incipit` = text present AND (first text OR whitespace since
  the previous text); textless syllables FALSE (`ve(gh) (hg)re(g)` ->
  `re` begins a word); header comment updated;
- `GabcElementum.parenthesis` also marks `` `? `` `,?` (header comment
  updated);
- notes before any clef resolve as **c3** (old `initial-key` default);
  a secondary clef `@c2` does not move `do`;
- a bar touching a note in the same parentheses (`(g;h)`, before or
  after) -> monitum `GABC_DIVISIO_IN_NEUMA` at the bar;
- header: `%` comment stripped per line; body: `%` is a comment only
  between syllables (inside text it is literal - T4); unclosed `(`
  reads notes to end of input;
- glossarium: `nabc` (ignotum-permissum), `modificator`.

---

## Task T3: Note shapes, rhythmic signs, accidentals, the b-flat clef

**Files:**
- Modify: `lib/gabc.c`, `probationes/probatio_gabc.c`
- Create fixtures: `formae.gabc`, `signa_rhythmica.gabc`,
  `alterationes.gabc`, `claves_mutatae.gabc`

**Interfaces:**
- Consumes: T2.
- Produces: `GabcNota.formae` bits for — inclinatum (upper-case
  pitch), virga `v`, virga reversa `V`, stropha `s`, oriscus `o`,
  oriscus scapus `O`, quilisma `w`, quilisma quadratum `W`, cavum
  `r`, linea `R`, quadratum `q`, liquescens `~`, augmentata `<`,
  deminuta `>`, initio debilis `-` (prefix), fusio `@`; `mora`
  (`.` = 1, `..` = 2), `episema` (`_` + optional digit), `ictus`
  (`'` + optional digit); `alteratio` from `x` / `y` / `#` (and the
  parenthesised `x?` forms) with the scope rule recorded in T1;
  the `cb` clefs. Dump letters: one letter per form bit, fixed
  order, documented in the test.

- [x] **Step 1: Measure** — tally which modifiers occur in
  `salve_2435.gabc` (`'`, `.`, `_`, `[...]`); confirm the T1 rule
  record for accidental scope and pitch range.
- [x] **Step 2: Failing tests:**
  - `_formas_probare` — one note per modifier, dump letters pinned;
    `gw` (quilisma) and `G` (inclinatum) and `-f` (initio debilis).
  - `_signa_rhythmica_probare` — `g.` `g..` `g_` `g_0` `g'` `g'1`
    `g._'` (all three on one note).
  - `_alterationis_ambitum_probare` (Review Focus 1) — `ixi(…)` in
    one word: both i's flat; next word's i natural; `ix i y i` inside
    one word: flat, then natural after `y`; a bar resets; an
    accidental on i does not flat b (octave below).
  - `_clavem_mutatam_probare` (Review Focus 2) — clef change inside a
    syllable `(gh(c3)hg)` if legal gabc else between syllables;
    `cb3`: ti flat until the next clef; a custos `+` before a clef
    change resolved with the new clef (custos itself is T4 — here
    only that the custos note's `gradus` uses the next clef; if T4
    is needed first, move this assertion to T4 and say so).
- [x] **Step 3: Run** — red.
- [x] **Step 4: Implement.**
- [x] **Step 5: Run** — green. **Plants:** accidental scope ends at
  the word but not the bar → `_alterationis_ambitum_probare`
  FRACTUM; `..` read as one mora → `_signa_rhythmica_probare`
  FRACTUM.
- [x] **Step 6: Commit** (gate `radix`).

**Executed 2026-11-03.** Step 1 tally (shell, not the reader): the
Salve uses `'` x39, `.` x14, `_` x8, `w` x2, `[oh:h]` x6, `[ll:1]` x3 -
the four modifier counts are pinned in `_salve_legere_probare`. Scope
rule as recorded in T1 (now also in the `lib/gabc.c` header). Red:
four sections plus the Salve counts; green after Step 4. **Plants:**
bar no longer ends an accidental's scope -> `_alterationis_ambitum_probare`
FRACTUM; `..` read as one mora -> `_signa_rhythmica_probare` FRACTUM;
both restored, green.
Decisions:
- **custos assertion moved to T4**: a custos keeps only its written
  `positio` (the editor already writes it where the next note sits),
  there is no `gradus` on an element to resolve - T4 pins that the
  custos before a clef change is kept with its position;
- clef changes inside a group are just clef tokens: `a(ghc3hg)` =
  neume, clef, neume;
- accidentals also reset at a clef change (positions change meaning);
  soft `X` `Y` and parenthesised `x?` alter like plain signs (the
  element keeps `molle` / `parenthesis` for drawing);
- `cb`/`fb`: every note with `gradus` = 6 mod 7 (si) is flat unless a
  sign at its position is in scope;
- `vv` `vvv` `VV` `VVV` `ss` `sss`: a repeated `v`/`V`/`s` on a note that
  already has that shape adds a new note at the same position (column
  of the repeating letter), so `gvv` = 2 notes, `gsss` = 3;
- a modifier with no open neume (after a space, clef, bar) is a VITIUM
  `GABC_NOTA_IGNOTA` ("signum notae sine nota");
- `r0` sets cavum and linea punctum; `r1`-`r5` bits in `signa`;
  `r6`/`r7`/`r8` set `GABC_SIGNUM_FICTA` and the note's own alteratio
  (-1/0/+1), untouched by scope;
- found while checking: text may contain spaces (`mens :(c.)` is ONE
  syllable "mens :"), which T2 already did right.

---

## Task T4: Typesetting content, opaque marks, malformed input, totality

**Files:**
- Modify: `lib/gabc.c`, `probationes/probatio_gabc.c`
- Create fixtures: `spatia.gabc`, `fractiones.gabc`,
  `textus_ornatus.gabc`, `signa_opaca.gabc`, `vitia_formae/` (one
  small file per malformed case)

**Interfaces:**
- Consumes: T2–T3.
- Produces: elements `GABC_EL_SPATIUM` (space, `/`, `//`, `/0`, `!`,
  `/[n]` kept with its kind and factor), `GABC_EL_FRACTIO` (`z` `Z`
  and the `z+` `z-` variants per T1's record), `GABC_EL_CUSTOS`
  (`+` with its position; `z0` = automatic custos); text runs for
  `<i> <b> <sc> <ul> <c> <tt>` (nesting as bits), `<sp>` special
  characters resolved to UTF-8 display text (℟ ℣ ǽ œ æ ✠ and the
  rest of the Gregorio list), `<e>` elision, `<alt>` above-staff
  text, `[...]` translation, `{...}` centred vowel, `<v>` verbatim
  (kept raw, genus verbatim), `<eu>`, `<nlba>`, `<clear>`, `<pr>`;
  `GABC_EL_SIGNUM_OPACUM` for every `[xx:...]` note attachment
  (`ll`, `oh`, `ob`, `ub`, `uh`, `cs`, `nm`, `alt` …) and for nabc
  after `|`, with the source text and the note it follows; dump lines
  `spatium`, `fractio`, `custos`, `signum`.

- [x] **Step 1: Measure** — tally constructs in `salve_2435.gabc`
  that T2–T3 left unhandled (expect `{}`, `[ll:1]`, `[oh:h]`); none
  may remain after this task.
- [x] **Step 2: Failing tests:**
  - `_spatia_probare`, `_fractiones_probare`, `_textum_ornatum_probare`
    (nested `<b><i>…</i></b>`, `<sp>R/</sp>`, `<e>`, `[Lord]`,
    `{a}` range), `_signa_opaca_probare` (each attachment kept with
    exact source text; `(f|vi)` nabc kept).
  - `_custodem_probare` (moved from T3, Review Focus 2) — a custos
    `g+` before a clef change is kept as `GABC_EL_CUSTOS` with its
    written position, and the notes after the clef resolve with the
    new clef.
  - `_vitia_formae_probare` (Review Focus 4) — one assertion per
    file in `vitia_formae/`: kind + line:col; BOM and CRLF give zero
    diagnostics and the same dump as their clean twins.
  - `_totalitatem_probare` — for every fixture: every prefix
    (truncate at each byte) and 2,000 seeded random single-byte
    mutations read without crash; `gabc_vitia_scribere` succeeds on
    each. Seeded with `sors` so failures repeat.
- [x] **Step 3: Run** — red.
- [x] **Step 4: Implement.** Unknown `<tag>` and unknown `[xx:]` →
  opaque, with a monitum only when the tag is not on Gregorio's list.
- [x] **Step 5: Run** — green; also under ASan (`./tools/sonda.sh`
  on a tiny driver, or the suite's ASan mode if it has one).
  **Plants:** drop `[oh:...]` instead of keeping it →
  `_signa_opaca_probare` FRACTUM; stop scanning at a NUL →
  `_vitia_formae_probare` FRACTUM.
- [x] **Step 6: Commit** (gate `radix`).

---

## Task T5: Neume shape classification (G4)

**Files:**
- Modify: `lib/gabc.c`, `probationes/probatio_gabc.c`

**Interfaces:**
- Consumes: T2–T4 (`GabcElementum` of genus NEUMA, its notes).
- Produces: `gabc_figura`; dump column 4 of `neuma` lines filled.

- [x] **Step 1: Measure** — from the Gregorio documentation, write
  the table of contours → names this task pins, and note each source
  page in the Executed note. Rows that the docs leave unclear are
  `_IGNOTA` (not guessed).
- [x] **Step 2: Failing test** `_figuras_probare`, table-driven, at
  least: `g` punctum · `gv` virga · `G` punctum inclinatum · `gh` pes
  · `hg` clivis · `ghg` torculus · `hgh` porrectus · `gih` scandicus?
  (per docs) · `hGF` climacus · `gwh` pes quilismaticus · `gog`
  pressus/oriscus group per docs · `gvgv` bivirga · `gsgsgs`
  tristropha · `hghg` porrectus flexus · `gihi` torculus resupinus ·
  `giHG` pes subbipunctis · liquescent `gh~` (epiphonus) and `hg~`
  (cephalicus) · `-gh` initio debilis pes. Plus: unknown contours →
  `_IGNOTA`, never a wrong name.
- [x] **Step 3: Run** — red. **Step 4: Implement** as a small rule
  table keyed by contour + form bits (no special cases in code paths
  the table can express).
- [x] **Step 5: Run** — green. **Plant:** swap pes and clivis rows →
  FRACTUM.
- [x] **Step 6: Commit** (gate `radix`). The Gregorio-oracle corpus
  check (spec §VII-1) is NOT here; it waits on Fran's decision.

**Executed 2026-11-03.** Table source = Gregorio
`src/gabc/gabc-glyphs-determination.c` (`add_note_to_a_glyph`),
`src/struct.h` (glyph enum), `src/gregoriotex/gregoriotex-write.c`
(names); the web docs (`gabc/details.html`) only cover pes forms and
liquescence. Answers to the plan's questions: `gih` = torculus (`ghi` is
the scandicus); `gog` = IGNOTA (Gregorio: oriscus + punctum, two glyphs).
Pressus (no Gregorio glyph) = the shape both traditions share: "p=p-p",
"p=o-p" (Claude's call). Rules: unison splits puncta; debilis only on the
first note, liquescence only on the last, else IGNOTA; deminuta last note
gives epiphonus / cephalicus, other liquescence keeps the base name.
Table = `REGULAE_FIGURARUM` (26 rows, signature + debilis + deminuta).
Test `_figuras_probare` 38 named + 18 IGNOTA rows; older dumps' 110 neuma
lines filled by an independent Python oracle (agreed except `-g`: the
dump writes `-` for both "no forms" and initio debilis — T3 grammar
flaw, flagged). Salve: 111 punctum, 6 clivis, 2 pes, 2 pes
quilismaticus, 5 IGNOTA (unisons). **Plant:** pes and clivis rows
swapped → `_neumas_probare` FRACTUM; restored, cmp equal. Enum gaps
(pes quassus, virga reversa, quilisma torculus, ancus, …) listed in the
worklog for Fran.

---

## Task T6: Converter header — `gabc_carmen.h` (STOP: Fran reviews)

**Files:**
- Create: `include/gabc_carmen.h`

**Interfaces — Produces** (proposed):

```c
/* <aedilis corpus="lib/gabc_carmen.c"/> */

nomen structura {
    b32 strictus;          /* refuse on the first approximation (G10) */
    s32 summus;            /* MIDI of the highest note; default 65 (F4) */
    s32 do_fixum;          /* MIDI of do; -1 = use summus (G9) */
    chorda titulus;        /* empty = header name, else file name */
} GabcCarmenForma;

nomen structura {          /* G11: one row per carmen note */
    s64 initium;           /* onset, pulses */
    i32 nota;              /* index into GabcCantus.notae */
    i32 syllaba;
    i32 linea; i32 columna;
} GabcNexus;

nomen structura {          /* G10 report */
    i32 approximationes; i32 signa;   /* + per-kind counts and
                                       * the first few with line:col */
    GabcNexus* nexus; i32 numerus_nexuum;
} GabcCarmenRelatio;

vacuum gabc_carmen_formam_ordinariam (GabcCarmenForma* forma);
b32 gabc_carmen_scribere (constans GabcCantus* cantus,
    constans GabcCarmenForma* forma, Piscina* piscina,
    ChordaAedificator* exitus, GabcCarmenRelatio* relatio);
    /* writes one <carmen>...</carmen> (no <carmina> wrapper, as
     * musicxml_carmen); FALSUM if strict and approximated */
b32 gabc_relationem_scribere (constans GabcCarmenRelatio* relatio,
    constans character* via, ChordaAedificator* exitus);
```

- [ ] **Step 1: Measure** — read `include/musicxml_carmen.h` whole;
  match its naming and report shape where they fit.
- [ ] **Step 2: Draft the header** with the conversion table in its
  head comment (spec §III): pitch (do from `summus`: the chant's
  highest note lands exactly on `summus`; `do_fixum` overrides),
  rhythm G8 rule by rule, incises and breaths, verba purging,
  key = final pitch + rotation name (do→ionius, re→dorius,
  mi→phrygius, fa→lydius, sol→mixolydius, la→aeolius; ti has no carmen
  name → `"<do> maior"` + approximation), mode number in a comment.
  *(Default `summus` = F4 is Claude's call: it reproduces `salve.py`'s
  do = F4 for the Salve without an override.)*
- [ ] **Step 3: STOP — show Fran the header.** Record changes.
- [ ] **Step 4: Commit** (lint only).

---

## Task T7: Converter — pitch, rhythm, incises, verba, key, report, links; the Salve acceptance

**Files:**
- Create: `lib/gabc_carmen.c`, `probationes/probatio_gabc_carmen.c`
- Create fixtures: `rhythmus.gabc`, `verba_variae.gabc`,
  `margines.gabc`

**Interfaces:**
- Consumes: T2–T5 model; T6 header; `carmina_legere`,
  `carmina_ordinaria`, `carmina_invenire` (`include/carmen.h`).
- Produces: `gabc_carmen_scribere`, `gabc_relationem_scribere`.

- [ ] **Step 1: Measure** — read `salve.py` and `lib/musicxml_carmen.c`'s
  MML writer; list every length the Salve produces (in sixteenths:
  2, 3, 4, 6 …) and confirm `&` ties cover sums outside `salve.py`'s
  `VALORES` (salve.py would crash on 5; we must not).
- [ ] **Step 2: Failing tests** (every output re-read with
  `carmina_legere`, as `probatio_musicxml_carmen.c:56` does):
  - `_rhythmum_probare` — punctum = eighth; `.` doubles; `_`
    ×1.5 only without mora; note before `w` max(own, 3 sixteenths);
    `,` `;` `` ` `` → incise + r16; `:` `::` → incise + r8; final
    incise no breath.
  - `_rhythmum_marginum_probare` (Review Focus 5).
  - `_altitudinem_probare` — default: highest note = F4; `do_fixum`
    overrides; `x` flat lowers by one semitone; `cb3` flat.
  - `_verba_variae_probare` (Review Focus 3) — accents stripped,
    first letter case kept and rest lowered (as `salve.py`
    `syllaba_munda`), `{}` braces removed, special characters
    (℟ ℣) not sung, melisma `_` per extra note after unison merging.
  - `_clavem_probare` — Salve → `"f ionius"`; a mode I fixture
    ending on re → `"<pitch> dorius"`; final on ti → approximation.
  - `_nexus_probare` — one row per carmen note, onsets increasing,
    each pointing at the gabc note that started it (a tied unison
    points at its first note).
  - `_strictum_probare` — strict + a ti final → FALSUM, nothing
    written, report names it.
  - `_salve_probare` **ACCEPTANCE** — `salve_2435.gabc` with the
    default form: per note, onset / longitudo / nota / syllaba equal
    to the book's `salve` (`carmina.stml:664`), same number of
    incises; key differs by design (G9 note in spec §III).
- [ ] **Step 3: Run** — red.
- [ ] **Step 4: Implement.**
- [ ] **Step 5: Run** — green. **Plants:** episema ×1.5 even with
  mora → `_rhythmum_probare` FRACTUM and `_salve_probare` FRACTUM;
  unisons tied across syllables → `_rhythmum_marginum_probare`
  FRACTUM.
- [ ] **Step 6: Commit** (gate `radix`), worklog
  `lib/gabc_carmen.worklog.md`.

---

## Task T8: `auditio -gabc`

**Files:**
- Modify: `tools/auditio.c` (usage, head comment, flag, source path),
  `tools/auditio.worklog.md`
- Test: the auditio suite that covers `-musicxml` (find it in Step 1)

**Interfaces:**
- Consumes: T6–T7.
- Produces: `auditio -gabc VIA [-strictus] [-titulus T] [-do P]` with
  `-musicxml`'s exact behaviour (G12): alone → book on stdout,
  report on stderr, exit 3 when approximations; with `-carmen`,
  `-censura`, `-partitura`, `-sonare` … → acts as the book source.
  `-do` takes a pitch name (`F4`, `Bb3`) → `do_fixum`. `-gabc` and
  `-musicxml` together → exit 2 with usage.

- [ ] **Step 1: Measure** — find how `-musicxml` is tested (grep the
  suites for `"-musicxml"`); mirror it.
- [ ] **Step 2: Failing tests** — `-gabc salve_2435.gabc` prints a
  `<carmina>` that `carmina_legere` accepts; `-gabc … -partitura`
  output begins with the expected title line; `-gabc` + `-musicxml`
  → exit 2; `-do Bb3` moves every note down a semitone relative to
  `-do B3`.
- [ ] **Step 3–5:** implement by sharing the `-musicxml` path
  (`tools/auditio.c:549-640`), not copying it — factor a common
  "converted source" helper if the two would otherwise duplicate more
  than the reader/converter calls. Green. **Plant:** ignore `-do` →
  its test FRACTUM.
- [ ] **Step 6: Commit** (gates `radix` + the auditio tool's gate;
  rebuild auditio before commit per house rule). Tell Fran the
  command to hear the Salve: `auditio -gabc
  musica/fontes/salve_2435.gabc -sonare`.

---

## Task T9: GregoBase sample corpus

**Files:**
- Create: `build/gabc/` (downloads, untracked), a fetch script in
  `tools/` or `instrumenta/` (Python, run with `-I`, no third-party
  packages), `probationes/fixa/gabc/gregobase.manifestum` (id, title,
  sha256, licence note per file), the committed sample files under
  `probationes/fixa/gabc/gregobase/`
- Create: `probationes/probatio_gabc_corpus.c` (or a section of the
  existing suite if the sample is small)

- [ ] **Step 1: Measure** — check GregoBase's download format and
  terms (spec AUDIENDA); pick a sample of ~50–100 chants across
  office parts (antiphons, hymns, responsories, introits, Kyriale),
  modes I–VIII, and both clefs; record why each was picked if not
  random. STOP and show Fran the terms and sample size before
  committing third-party text.
- [ ] **Step 2: Test** — every manifest file reads with no crash
  under ASan; VITIUM count 0 for all, or each exception listed in the
  manifest with its reason; total monitum counts by kind pinned (the
  long tail, G5); every file converts and `carmina_legere` accepts
  the output; approximation counts by kind pinned.
- [ ] **Step 3:** fix what the corpus breaks (each fix its own small
  test in T2–T7's suites first).
- [ ] **Step 4: Commit** (gate `radix`). File a ledger nota for the
  top unmodeled constructs, so part 2 and later work see them.

---

## Task T10: The book's Salve from the converter; `salve.py` retires (STOP: Fran approves the book change)

**Files:**
- Modify: `carmina.stml` (the `salve` entry, lines ~657-671),
  `musica/unitas.stml` description if it names generators
- Delete: `musica/fontes/salve.py`

- [ ] **Step 1: Measure** — which tests read the book's `salve`
  (grep suites for `"salve"`); their expectations must hold with the
  key change.
- [ ] **Step 2:** generate the entry with `auditio -gabc
  musica/fontes/salve_2435.gabc`, keep the book's comment block
  (update its "Generator:" line to the auditio command), show Fran
  the diff (key `f lydius` → `f ionius`, comment, nothing else).
- [ ] **Step 3: STOP — Fran approves.**
- [ ] **Step 4:** delete `salve.py`; `_salve_probare` still green
  (it compares to the book, which now came from the converter — so
  also keep one assertion against a fixed copy of the old notes,
  committed as `probationes/fixa/gabc/salve_py_notae.tsv`, so the
  parity survives `salve.py`'s deletion).
- [ ] **Step 5: Commit** (gate `radix`, plus whatever reads the
  book). Ledger: desideratum …96NQ stays open until part 2
  (the pane) lands — note the progress on it.

---

## Executed notes

(appended per task)

### T4 — executed 2026-11-03

- Measured Salve (shell): `[ll:1]` 3, `[oh:h]` 6, `!` 2, `/` 1, `//` 1,
  `{ia}` 1 → reader: 9 opaque marks, 4 spaces, 1 vocalis; still 0
  diagnostics.
- Syllable-text grammar taken from Gregorio's score lexer/grammar and the
  `<sp>` table from `gregoriotex-symbols.tex` (fetched this session). Styles
  `i b ul c sc tt` carry across syllables (Gregorio does); `e` does not.
- Header gaps handled without a header change (flag for T6 review):
  non-breaking spaces (`!/` `!//` `! ` `!/[n]`) = `nullum` + the space;
  `<pr:N>` value → monitum. Unknown `<tag>` = literal text + monitum
  (Gregorio prints it literally).
- Also fixed from T2: lone `]` closing `@[` was a vitium; `$)` closed a
  notes group; `%` inside syllable text was literal.
- New: `GABC_POSITIO_EXTRA` (Gregorio's pitch ceiling), `GABC_NON_UTF8`,
  `GABC_SEPARATOR_ITERATUS` now emitted. `claves_mutatae.gabc` gained
  `staff-lines:5;`.
- Tests: 19 sections green (8 new: spatia, fractiones, custos, textus
  ornatus, signa opaca, vitia formae with 16 files, Salve counts,
  totalitas 68,806 cases). Whole suite clean under `tools/sonda.sh`
  (ASan + UBSan).
- Plants: drop `[oh:` → `_signa_opaca_probare` red; cut at first NUL →
  `_vitia_formae_probare` red; stray `]` → NIHIL → `_totalitatem_probare`
  red (totality was born green on the already-total T3 reader). All
  restored byte-identical.
