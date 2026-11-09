# psalmodia Plan, part 1 — the tone-table reader (`psalmodia_tabula`)

**Ledger:** parcum 01M6V5H5V1EGK1V37TP5M6BFN1 ("psalmodia"); one opus
per task below, filed at R1.

**STATUS: DRAFT 2026-11-09.** Nothing executed yet.

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:executing-plans (inline, per house convention, no
> subagents) to implement this plan task by task, ONE TASK PER TURN
> with Fran's approval. Every task OPENS with a measurement that may
> re-slice it; a re-slice is shown to Fran before work continues.
> Steps use checkbox (`- [ ]`) syntax. Each task ends with an
> "Executed" note.

**Goal:** C code can load `musica/toni_psalmorum.stml` into a typed
table, find an ending from any label a chant carries ("8 G", "VIII g",
"1.d", mode "4" + differentia "A*"), and get the book's examples as
ready-made test cases for the fitter (part 2). Every transcription slip
in the table is caught with its line number.

**Spec:** `project-specs/psalmodia-spec.md` — P2 (table is data, its
format is the interface), P5 (cadence = tagged slots), §II (format and
its checks), §IV (what the fitter will need), §V (examples as
fixtures). The table's own header comment is the grammar.

**Architecture.** One library, `psalmodia_tabula` (`lib/
psalmodia_tabula.c`, `include/psalmodia_tabula.h`): STML text →
`PsalmodiaTabula` (everything in one piscina) + located diagnostics.
Three layers inside, each tested on its own: **reading** (the element
tree → structs; unknown elements and attributes reported), **checking**
(note strings are valid gabc under the tone's clef; ending letter =
last note's pitch; duplicate endings; examples point at real endings),
**lookup** (label → tone + ending; variant selection). Note strings
are checked with the house gabc reader (`gabc_legere`) — one parser of
gabc in the house, not two. The table is read from the file in tests;
embedding it in binaries (like `instrumentarium.stml`) waits for part
2, whose CLI is the first binary that needs it. So does spec §II's
GregoBase ending check (each ending fitted to *sæ-cu-ló-rum. A-men*):
it needs the fitter, so it is a part-2 gate (the throwaway script
already ran it once, §II).

**Measured before writing (2026-11-09):**
- `include/stml.h`: `stml_legere(chorda, Piscina*, InternamentumChorda*)`
  → `StmlResultus` (`elementum_radix`, `status`, `linea_erroris`);
  every `StmlNodus` carries `linea` (1-based source line) — so every
  diagnostic can be located with no extra work.
  `stml_attributum_capere`, `stml_numerus_liberorum`,
  `stml_liberum_ad_indicem`, `stml_textus_valor`.
- `lib/carmen.c:3907 instrumentarium_legere` is the house precedent for
  a data-file reader; it stops at the first error. This reader
  collects all of them instead (a transcription slip list should be
  complete).
- `include/gabc.h`: `gabc_legere(chorda, Piscina*)` is total;
  `GabcNota.gradus` = diatonic step from do, already resolved by the
  clef (c-clef puts do on its line, f-clef fa) — the ending-letter
  check is `"CDEFGAB"[gradus mod 7]` of the last note.
- `include/tonus.h` already defines `Tonus` (mode analysis of a
  melody line) — hence the `Psalmodia` prefix here.
- The table (parsed, comments excluded): 11 `<tonus>`, 20
  `<mediatio>`, 35 `<terminatio>`, 246 `<exemplum>`; 19 attribute
  names, all listed in R1's fields.

## Global Constraints

- C89 with latina.h; Latin identifiers, comments and messages; every
  latina.h macro and every Roman numeral is a forbidden identifier
  (pitch letters `c d i l m v x` never as single-letter names).
- `chorda` is NOT NUL-terminated; `i32`/`i64` are UNSIGNED — line
  numbers `i32`, any "not found" `s32`/NIHIL.
- **Totality:** any bytes → a `PsalmodiaTabula` (NIHIL only if memory
  fails) with diagnostics; no crash, no hang; reading continues past a
  bad element. Tested under the suite's ASan build.
- **Nothing silent:** an unknown element or attribute, a missing
  required attribute, an unparseable note string — each is a
  `PsalmodiaVitium` with the line.
- **Messages in Latin**, format `linea N: causa` (e.g. `linea 64:
  differentia "D" sed nota ultima G`).
- The library includes stml.h, gabc.h, piscina, chorda,
  chorda_aedificator — nothing from carmen or vicus.
- New C files through `./silva/scribe.sh`; the aedilis tag in the
  header; `./tools/compile_tests_fontes_generare.sh` re-run.
- Every new test section born red by a plant that COMPILES
  (`silva.planta`), recorded in the Executed note and the worklog.
- Commit through `silva.commissio` with `portae=[('radix', ...)]`
  (root suite: `./compile_tests.sh psalmodia`); tree frozen while
  gates run; Latin lint with `VOCABULA_VIAE_ADDITAE` for untracked
  files.
- **The header is reviewed by Fran before implementation:** R1 STOPs.

## Review Focus

Five conditions the spec implies but no feature names; each pinned by
a test in the named task.

1. **Labels as chants actually write them** — GregoBase writes `d`
   for the book's `D`, roman or arabic modes, `8.G`, `8 G`, `8G`,
   extra spaces, a lone mode for a one-ending tone ("5", "6",
   "peregrinus"), and an empty differentia: each resolves to the one
   ending a reader would expect, or to NIHIL with a Latin reason that
   lists the tone's endings — never to a wrong ending. (R4,
   `_signa_probare`.)
2. **A transcription slip** — a misspelt slot (`<praeparatoi>`), an
   ending letter that does not match its last note, the same ending
   twice, an `anticipata` without `superveniens`, an example naming a
   missing ending: each is reported with its line, and the rest of
   the table still loads. (R3, `_lapsus_probare`.)
3. **A damaged file** — empty, truncated mid-element, wrong root,
   random bytes, a `<tonus>` with no clef: diagnostics, no crash, the
   good tones still read. (R2, `_totalitas_probare`.)
4. **Two positions of one mode** — tone 4 and tone 4 `positio=
   "altera"` together hold g, E, c, A, A\*: lookup by mode searches
   both, and a name repeated across them is a reported clash, not a
   silent first-wins. (R3 clash, R4 lookup.)
5. **Example text with odd spacing** — `notae` and `textus` values
   spanning lines, double spaces, the `†` and `*` separators, the `r`
   hollow mark: tokens split on any whitespace, separators kept as
   their own tokens, nothing lost. (R2, `_exempla_probare`.)

---

## Task R1: The header `psalmodia_tabula.h` (STOP: Fran reviews)

**Files:**
- Create: `include/psalmodia_tabula.h` (types, prototypes, head
  comment)
- Modify: `musica/toni_psalmorum.stml` — remove the PROPOSITUM markers
  once Fran confirms the format additions (§II list) at this STOP
- Modify: `oratio/glossarium.stml` only if `./oratio/quaere.sh`
  rejects a word (check: psalmodia, tabula, clausula, praeparatio,
  accentus, superveniens, anticipata, vicaria, semiversus,
  monosyllaba, exemplum, signum)

**Interfaces — Produces** (proposed; R2–R4 use whatever the approved
header says):

```c
/* <aedilis corpus="lib/psalmodia_tabula.c"/> */

nomen enumeratio {
    PSALMODIA_PRAEPARATIO, PSALMODIA_ACCENTUS, PSALMODIA_NOTA
} PsalmodiaLociGenus;

nomen structura {
    PsalmodiaLociGenus genus;
    chorda notae;          /* gabc unius syllabae: "ixi", "gvFED" */
    chorda superveniens;   /* ACCENTUS: nota addita; vacua = nulla */
    b32    anticipata;
} PsalmodiaLocus;

nomen structura {          /* mediatio aut terminatio */
    PsalmodiaLocus* loci;
    i32             numerus_locorum;
    b32             monosyllaba;   /* casus="monosyllaba" */
    b32             ad_libitum;
    chorda differentia;    /* terminatio; vacua = unica */
    chorda vicaria;        /* vacua = nulla */
    chorda clavis;         /* vacua = toni */
    chorda tenor;          /* vacua = toni */
    chorda initium;        /* intonatio secundae partis; vacua */
    i32    linea;
} PsalmodiaClausula;

nomen enumeratio {
    PSALMODIA_PARS_VERSUS, PSALMODIA_PARS_SEMIVERSUS,
    PSALMODIA_PARS_INITIUM, PSALMODIA_PARS_MEDIATIO,
    PSALMODIA_PARS_TERMINATIO, PSALMODIA_PARS_FLEXA
} PsalmodiaPars;

nomen structura {
    PsalmodiaPars pars;
    chorda  pagina, forma, differentia, nota;   /* vacua = nulla */
    b32     primus;        /* versus: VERUM nisi primus="falsum" */
    b32     canticum;
    chorda  textus;
    chorda* notae;         /* per syllabam; "†" et "*" signa sua */
    i32     numerus_notarum;
    i32     linea;
} PsalmodiaExemplum;

nomen structura {
    chorda  modus;         /* "1".."8", "peregrinus", "in directum" */
    s32     modus_numerus; /* 1..8; 0 alius */
    b32     positio_altera;
    chorda  clavis, tenor, pagina;
    chorda* initium;       /* per syllabam; numerus 0 = nulla */
    i32     numerus_initii;
    chorda  flexa;         /* vacua = nulla */
    PsalmodiaClausula* mediationes;   i32 numerus_mediationum;
    PsalmodiaClausula* terminationes; i32 numerus_terminationum;
    PsalmodiaExemplum* exempla;       i32 numerus_exemplorum;
    i32     linea;
} PsalmodiaTonus;

nomen structura { i32 linea; chorda causa; } PsalmodiaVitium;

nomen structura {
    PsalmodiaTonus*  toni;   i32 numerus_tonorum;
    PsalmodiaVitium* vitia;  i32 numerus_vitiorum;
} PsalmodiaTabula;

PsalmodiaTabula* psalmodia_tabulam_legere (chorda stml,
                                           Piscina* piscina);
    /* NIHIL solum si memoria deficit */
b32 psalmodia_tabulam_scribere (constans PsalmodiaTabula* t,
                                ChordaAedificator* exitus);
    /* forma canonica, probationes: linea una per rem */
b32 psalmodia_vitia_scribere (constans PsalmodiaTabula* t,
                              constans character* via,
                              ChordaAedificator* exitus);
    /* 'via:linea: causa' per lineam */

constans PsalmodiaClausula* psalmodia_terminationem_invenire (
    constans PsalmodiaTabula* t, chorda modus, chorda differentia,
    constans PsalmodiaTonus** tonus, ChordaAedificator* causa);
constans PsalmodiaClausula* psalmodia_signum_invenire (
    constans PsalmodiaTabula* t, chorda signum,
    constans PsalmodiaTonus** tonus, ChordaAedificator* causa);
    /* NIHIL: causa (si non NIHIL) Latine scribitur, differentiis
     * toni enumeratis */
constans PsalmodiaClausula* psalmodia_mediationem_eligere (
    constans PsalmodiaTonus* tonus, b32 monosyllaba, b32 antiqua);
constans PsalmodiaClausula* psalmodia_clausulam_monosyllabam (
    constans PsalmodiaTonus* tonus,
    constans PsalmodiaClausula* clausula);
    /* soror casus="monosyllaba" eiusdem differentiae, aut ipsa */
```

- [ ] **Step 1: Measure.** Re-read the table header comment and
  spec §II; list every element and attribute the table uses
  (`grep -o '<[a-z_]*\| [a-z_]*="'` over the file) and check each has
  a field above. Check every new identifier word with
  `./oratio/quaere.sh`.
- [ ] **Step 2: Draft the header** with the interfaces above and a
  head comment carrying: the label rules (R4), the checks (R3), and
  the canonical dump grammar (R2), each in a few lines.
- [ ] **Step 3:** `./silva/examen.sh` and `./silva/formator.sh -vitia`
  on the header; Latin lint.
- [ ] **Step 4: STOP — show Fran the header**, and ask him to confirm
  the PROPOSITUM format additions (then remove the markers from the
  table). Ledger: opera R1–R4 under the parcum.

---

## Task R2: Reading — elements to structs, the canonical dump

**Files:**
- Create: `lib/psalmodia_tabula.c`, `lib/psalmodia_tabula.worklog.md`,
  `probationes/probatio_psalmodia_tabula.c`
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:** Consumes R1's header. Produces
`psalmodia_tabulam_legere`, `psalmodia_tabulam_scribere`,
`psalmodia_vitia_scribere`.

- [ ] **Step 1: Measure.** `wc` the table; time one read of it in the
  test (expect well under 50 ms; if not, note it in the worklog).
- [ ] **Step 2: Write the failing tests** in
  `probatio_psalmodia_tabula.c` (credo, as `probatio_piscina.c`):
  - `_tabulam_veram_probare`: read `musica/toni_psalmorum.stml` →
    `numerus_tonorum == XI`, `numerus_vitiorum == 0`; terminationes
    across all tones `== XXXV`, mediationes `== XX`; exempla `== CCXLVI`; tone 1: clavis
    "c4", tenor "h", initium {"f","gh"}, flexa "g", first mediatio =
    ACCENTUS "ixi" sup "h", NOTA "h", ACCENTUS "g" sup "h", NOTA "h";
    terminatio D2 last-but-one locus ACCENTUS "gf" sup "g"
    anticipata VERUM; peregrinus terminatio tenor "g" initium "hixi"
    differentia vacua; tone 4 altera `positio_altera` VERUM.
  - `_scripturam_probare`: the canonical dump of a hand-written
    two-tone fixture (in the test as a string) equals an expected
    string fixed in the test.
  - `_exempla_probare`: an example whose `notae` spans two lines with
    double spaces and contains `†`, `*`, `hr` → tokens exactly
    {"f","gh","h","g","†","h","hr","*","g"}; `primus` VERUM by
    default, FALSUM with `primus="falsum"`; `canticum` read.
  - `_totalitas_probare`: empty input; `<toni_psalmorum>` unclosed;
    root `<instrumentarium>`; 2000 random bytes from `sors` (fixed
    seed); a table with one good tone, one `<tonus>` missing `clavis`,
    and an unknown element `<praeparatoi>` → each returns non-NIHIL
    under `CREDO_NON_RUIT`; the mixed table keeps the good tone and
    reports two vitia with lines 3 and 7 (as laid out in the fixture).
- [ ] **Step 3: Run** `./compile_tests.sh psalmodia` → FAIL (does not
  link: functions undefined).
- [ ] **Step 4: Implement.** Walk the tree with `stml_numerus_liberorum`
  / `stml_liberum_ad_indicem`, skipping comments and whitespace text;
  a known-attribute list per element (unknown → vitium); required
  attributes: `tonus` modus clavis tenor; `exemplum` pars textus
  notae. Token split on any ASCII whitespace. Grow arrays with `xar`
  and copy into the piscina at the end of each element. Canonical
  dump: one line per tone, cadence, locus, example, fields separated
  by tabs, in file order.
- [ ] **Step 5: Run** → PASS. Plants: (a) drop the unknown-attribute
  check → `_totalitas_probare` red; (b) keep whitespace tokens →
  `_exempla_probare` red. Restore.
- [ ] **Step 6: Commit** (`./silva/examen.sh` clean, worklog entry).

---

## Task R3: Checking — gabc validity, ending letters, clashes

**Files:**
- Modify: `lib/psalmodia_tabula.c`, `probationes/
  probatio_psalmodia_tabula.c`, the worklog

**Interfaces:** Consumes R2. No new prototypes: the checks add
`PsalmodiaVitium`s during `psalmodia_tabulam_legere`.

- [ ] **Step 1: Measure.** Feed `"(c4) a(gvFED)"`, `"(c4) a(hixih)"`,
  `"(f3) a(e)"` to `gabc_legere`; confirm `gabc_successus` and the
  last note's `gradus` (D = 1, h under c4 = A = 5, e under f3 = C = 0).
  Write the results in the worklog.
- [ ] **Step 2: Write the failing tests:**
  - `_lapsus_probare`, one fixture per slip, each expecting exactly
    one vitium with its line and a cause containing the quoted word:
    a locus `<nota>q</nota>` (not gabc) → "notae non legibiles";
    `differentia="D"` ending on `g` under c4 → `differentia "D" sed
    nota ultima G`; two `<terminatio differentia="g">` in one tone →
    "bis"; tone 4 and tone 4 altera both with `g` → "bis";
    `anticipata="verum"` without `superveniens`; an exemplum with
    `differentia="x"` in a tone without x; `clavis="c5"`;
    `tenor="hh"`; an empty `<mediatio/>`. In the same fixture a
    correct tone still reads.
  - `differentia="A*"` ending on g (si) and `G*` ending on h → no
    vitium (starred names skip the letter check).
  - `_tabulam_veram_probare` still sees `numerus_vitiorum == 0`.
- [ ] **Step 3: Run** → FAIL (no checks yet).
- [ ] **Step 4: Implement.** Every note string (locus notae,
  superveniens, flexa, initium token, terminatio initium, example
  token other than `†`/`*`) is read as `"(" clavis ") a(" notae ")"`
  with `gabc_legere`; not `gabc_successus` or no note → vitium. The
  clef is the terminatio's `clavis` if set, else the tone's. Ending
  letter: last `GabcNota` of the last locus, `"CDEFGAB"[gradus mod
  7]` against the first letter of `differentia` (case-insensitive);
  skip if the name contains `*` or is empty. Clash key: (modus,
  differentia, monosyllaba, ad_libitum) across all tones of the same
  modus.
- [ ] **Step 5: Run** → PASS. Plants: (a) letter check compares
  without the clef (always c4) → the f3 fixture red; (b) clash key
  per tone instead of per modus → the 4/4-altera fixture red. Restore.
- [ ] **Step 6: Commit.**

---

## Task R4: Lookup — labels, variants

**Files:**
- Modify: `lib/psalmodia_tabula.c`, `probationes/
  probatio_psalmodia_tabula.c`, the worklog; spec §II (one line: the
  label rules, pointing at the header)

**Interfaces:** Consumes R2–R3. Produces
`psalmodia_terminationem_invenire`, `psalmodia_signum_invenire`,
`psalmodia_mediationem_eligere`, `psalmodia_clausulam_monosyllabam`.

Label rules (`signum` → modus + differentia):
- Trim; mode = arabic `1`–`8` or roman `I`–`VIII` (any case), or
  `peregrinus` / `in directum` (any case); then any run of spaces or
  `.`; the rest is the differentia (`8G` also splits: digits end the
  mode).
- Ending match within all tones of that mode (both positions): exact
  name first; else a case-insensitive match if exactly one; empty
  differentia → the tone's only ending, if it has exactly one
  (ignoring `monosyllaba` and `ad_libitum` variants); else NIHIL.
- Never a variant: lookup returns the common form; the fitter asks
  for the monosyllabic one with `psalmodia_clausulam_monosyllabam`.
- Causes (Latin): `tonus "9" nullus`; `tonus 8 differentiam "x" non
  habet; differentiae: G, c, G*`; `tonus 2 plures differentias habet:
  D, A`.

- [ ] **Step 1: Measure.** Tally the distinct (mode, mode_var) pairs
  in the GregoBase CSV (`build/gabc/bacor_v0.4/x/csv/chants.csv`,
  Python `-I`) for modes 1–8; list any label shape the rules above do
  not cover, and re-slice if needed.
- [ ] **Step 2: Write the failing tests** — `_signa_probare` (each
  assertion = label → expected (modus, differentia) or NIHIL + cause
  substring): "8 G"→8 G; "VIII g"→8 G; "8.G"→8 G; "8G"→8 G; " 8  G "
  →8 G; "1 d"→1 D; "1 D2"→1 D2; "4 A*"→4 A* (altera); "4 a"→4 A
  (altera); "4 g"→4 g; "5"→5 a; "6"→6 F; "peregrinus"→peregrinus;
  "In Directum"→in directum; "2"→NIHIL "plures"; "9 G"→NIHIL
  "nullus"; "8 x"→NIHIL "G, c, G*"; "" → NIHIL.
  `psalmodia_terminationem_invenire` with modus "1", differentia "g2"
  → tone 1 g2. `_variantes_probare`: tone 6 `eligere(FALSUM, VERUM)`
  → the ad_libitum mediatio (first locus "ixi"); tone 2
  `eligere(VERUM, FALSUM)` → the monosyllaba mediatio (one NOTA "i");
  tone 1 `eligere(VERUM, FALSUM)` → the common one (tone 1 has
  none); `clausulam_monosyllabam` on in directum's ending → its
  monosyllabic sister.
- [ ] **Step 3: Run** → FAIL.
- [ ] **Step 4: Implement** per the rules above.
- [ ] **Step 5: Run** → PASS. Plants: (a) case-insensitive match
  without the "exactly one" condition → none red today (no tone has
  two cases of one letter) — add a fixture tone with both `g` and `G`
  and assert "g" finds g, "G" finds G, then the plant is red; (b)
  search only the first tone of a mode → "4 A*" red. Restore.
- [ ] **Step 6: Commit; Executed notes; spec §II line; ledger opera
  closed.** Then the report to Fran, and part 2 (syllables + fitter)
  is planned.

---

## Executed notes

(none yet)
