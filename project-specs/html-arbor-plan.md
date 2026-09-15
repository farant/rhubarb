# html_arbor Plan — the HTML client on materia (phase 4)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents) to implement this plan
> task-by-task, ONE TASK PER TURN with Fran's approval. Steps use checkbox
> (`- [ ]`) syntax for tracking.

**Goal:** The HTML tree on materia — the simple builder of
`html-arbor-spec.md` (void elements, implied closes, case-insensitive
matching, raw text owned by the lexer), byte-exact through the emitter,
projected to STML and read back through two cycles and the tree comparator,
judged by a hand-written canon, benched by a computus twin, and proven on a
consumer of a consumer (md's rendered HTML). Ten gates, each born red.

**Architecture:** `html-arbor-spec.md` §11 (the addendum GOVERNS the body).
`html_lexare` (exists) → `html_adaptare` one for one → `HtmlLigator` (trivia
inside tags only) → `html_arbor_parsare`, an ITERATIVE builder over an
explicit open-element stack producing `MateriaNodus` against the generated
`HTML_REGISTRUM`. Emission `materia_scribere_nodum`; projection
`materia_arbor_scribere_nodum` / `materia_arbor_legere`; oracle
`materia_arbor_aequalis`.

**Tech Stack:** C89 via latina.h, house flags (`tools/vexilla.sh`), credo,
piscina, chorda, Xar, materia (`materia/fontes`), the generator
`./materia/coquere.sh`, canon, pythonica (`silva.commissio`, `silva.planta`).

**Spec:** `project-specs/html-arbor-spec.md` — §11 first, then §§4–8 for
the intent §11 retargets. Sibling precedents: `css-arbor-plan-B.md` (task
shape), `css/CLAUDE.md` and `md/CLAUDE.md` (the client shape as built).

## Global Constraints

- **Latin throughout** — identifiers, comments, messages. `html`, `stml`,
  `token`, `offset`, `doctype`, `cdata` stay untranslated (house
  precedent; the lexer already uses them).
- **Every latina.h word is a FORBIDDEN identifier** (`nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `brevis`, `duplex`, `exire`…);
  single capitals `I V X L C D M` are Roman numerals; two-letter locals
  are refused by the identifier lint; a digit suffix is a NEW word.
- **`chorda` is NOT null-terminated. `i32`/`i64` are UNSIGNED**; anything
  subtraction-derived or a sentinel takes `s32`/`s64`.
- **`??-` in a string literal is a trigraph; `<*/>` or `lib/*.c` inside a
  BLOCK COMMENT breaks it; `" */"` inside a doc comment closes it; a
  `<tag>` at the START of a comment is an annotation anchor** — write
  `sedes/…` or put the tag mid-sentence.
- **New C files are written with `./silva/scribe.sh <via> <<'EOF'`**; new
  identifier WORDS are checked with `./oratio/quaere.sh w1 w2…` BEFORE
  staging (untracked files are invisible to the lint); a missing word is a
  glossary line in `oratio/glossarium.stml`.
- **House format**: `./silva/formator.sh <viae> -scribere` then `-vitia`
  before every commit (banners of 50 `=`, lines ≤ 72).
- **Generated files are never hand-edited**: `html_registrum_coctum.{h,c}`
  come from `./materia/coquere.sh html/grammatica/html.registrum.stml
  -scribere`; the registrum probatio compares bytes. Genera and loci are
  APPENDED, never reordered; the seal in `html.canon` moves by hand with a
  cause.
- **Exit 2 from any runner = NOTHING RAN.** Never gate on exit 0 alone;
  never read `$?` after a pipe (zsh `${pipestatus[1]}`).
- **Runners**: `./html/compile_probationes.sh [filter]` (auto-discovers
  `fontes/*.c` and `probationes/probatio_*.c`; compiles materia's and, for
  H10, md's sources into `html/build/`), `./materia/compile_probationes.sh`,
  `./materia/shim_probare.sh`. Logs: `build/test_logs/html.log`; one
  probatio's credo lines: `build/test_logs/singulae/<probatio>.log`.
- **Every gate red at birth by a planted fault** that fails the
  ASSERTION, not the build: `silva.planta(via, vetus, novus, porta_nomen,
  filtrum, tolerans=…)` (multi-line anchors carry the newline in both
  strings). A plant that does not fire is a measurement of the TEST.
- **Docs first, gates last, then commit** through
  `silva.commissio(msg, viae, portae)` with explicit paths — never
  `git add -A`; deletions staged first with `git rm --cached`. A gate
  already green via `silva.porta`/`planta` on an unchanged tree is not
  rerun by commissio (write worklog/README first).
- **Never stage Fran's files**: `FAQ.md`, `gesta/annales/tabula.md`,
  `gesta/annales/tabularium.jsonl`; never touch the untracked
  `project-specs/exempla/*.html`.
- **Silva stays FROZEN**; this plan touches `html/`, `probationes/fixa/html/`,
  `pythonica/silva.py`, `project-specs/`, `materia/phase-log.md` and, only
  if a substrate defect is found, `materia/fontes` with its own gate.
- **Positions are byte offsets** + 1-based linea/columna; a source is
  never copied (tokens point into it), so test sources are allocated
  from the piscina, never a stack buffer (the css birth lesson).
- After any edit to `.stml`/`.canon` files: `./tools/natura_struere.sh`.

---

## Task H1: Scaffold, declaration, generated registry, lexicon, runner

**Files:**
- Create: `html/grammatica/html.registrum.stml`
- Generate: `html/fontes/html_registrum_coctum.{h,c}`
- Create: `html/fontes/html_registrum.h`, `html/fontes/html_lexicon.{h,c}`
- Create: `html/compile_probationes.sh` (from `css/compile_probationes.sh`)
- Create: `html/probationes/probatio_html_registrum.c`
- Create: `html/CLAUDE.md` (orientation stub: what this is, the plan, the runner)

**Interfaces:**
- Consumes: `include/html_lexema.h` (`HtmlLexemaGenus`, 23 members incl.
  `HTML_LEX_FINIS = 0` … `HTML_LEX_NUMERUS_GENERUM`), `materia_lexicon.h`
  (`MateriaLexGenus {titulus, orthographia, species, munus}`,
  `MateriaLexiconCoctum {genera, numerus_generum, praefixum_tagi,
  genus_spatii}`), `materia_coctor.h` (`materia_registrum_recens`),
  `materia_arbor.h` (`materia_arbor_genus_index`, `materia_arbor_consilium_nudum`).
- Produces: `HTML_REGISTRUM`, `HtmlGenus` (`HTML_GENUS_DOCUMENTUM` …
  `HTML_GENUS_ELEMENTUM_MALUM`, `HTML_GENUS_NUMERUS_GENERUM`),
  `HTML_LEXICON`, the slot enums below.

- [x] **Step 1: The declaration** — `html/grammatica/html.registrum.stml`
  verbatim from spec §11.3 (ten genera in that order; `elementum`'s loci in
  BYTE order; `documentum` = `liberi` + `cauda`). Add to the `<registrum>`
  header comment the four extension points as prose (`ancora`,
  `spatium`, `clonatum`, `reinserendum` — "nominata, non declarata; vide
  spec par. XI.6"). Judge it: `bin/canon_examen html/grammatica/html.registrum.stml`
  (root `<registrum>` is registered) — expect no vitia.

- [x] **Step 2: Generate** —
  `./materia/coquere.sh html/grammatica/html.registrum.stml -scribere`,
  then `./materia/coquere.sh html/grammatica/html.registrum.stml` (compare
  mode, exit 0 = recens). Read the generated header once: it defines
  `HtmlGenus`, `HTML_GENUS_NUMERUS_GENERUM`, `HTML_REGISTRUM`.

- [x] **Step 3: Slot enums (hand-written, `html_registrum.h`)** — includes
  the generated header; one enum per genus, in declaration order:

```c
nomen enumeratio { HTML_DOCUMENTUM_LIBERI = 0, HTML_DOCUMENTUM_CAUDA }
    HtmlDocumentumLocus;
nomen enumeratio { HTML_DOCTYPE_TOK = 0 } HtmlDoctypeLocus;
nomen enumeratio {
    HTML_ELEMENTUM_TOK_APERTURA = 0, HTML_ELEMENTUM_ATTRIBUTA,
    HTML_ELEMENTUM_TOK_FINIS, HTML_ELEMENTUM_LIBERI,
    HTML_ELEMENTUM_TOK_CLAUSURA, HTML_ELEMENTUM_TOK_CLAUSURA_FINIS
} HtmlElementumLocus;
nomen enumeratio {
    HTML_ATTRIBUTUM_TOK_NOMEN = 0, HTML_ATTRIBUTUM_TOK_AEQUALE,
    HTML_ATTRIBUTUM_TOK_VALOR
} HtmlAttributumLocus;
nomen enumeratio { HTML_TEXTUS_TOK = 0 } HtmlTextusLocus;
nomen enumeratio { HTML_REFERENTIA_TOK = 0 } HtmlReferentiaLocus;
nomen enumeratio { HTML_CRUDUS_TOK = 0 } HtmlCrudusLocus;
nomen enumeratio { HTML_COMMENTARIUM_TOK = 0 } HtmlCommentariumLocus;
nomen enumeratio { HTML_CDATA_TOK = 0 } HtmlCdataLocus;
nomen enumeratio { HTML_MALUM_TOKENS = 0 } HtmlMalumLocus;
```

- [x] **Step 4: The lexicon (`html_lexicon.c`)** — 22 rows in
  `HtmlLexemaGenus` order, per spec §11.4. Transcribe from
  `css/fontes/css_lexicon.c`'s shape; the rows that are not VERBATIM /
  SUBSTANTIVUM:

```c
    { "FINIS",          "",   MATERIA_LEX_FIXUM,    MATERIA_MUNUS_FINIS },
    { "TAG_FINIS",      ">",  MATERIA_LEX_FIXUM,    MATERIA_MUNUS_SUBSTANTIVUM },
    { "TAG_FINIS_SOLUS","/>", MATERIA_LEX_FIXUM,    MATERIA_MUNUS_SUBSTANTIVUM },
    { "AEQUALE",        "=",  MATERIA_LEX_FIXUM,    MATERIA_MUNUS_SUBSTANTIVUM },
    /* trivia INTRA tag sola (spec par. XI.5). DELIM = '/' solivagus:
     * HTML5 'unexpected solidus in tag', IGNORATUR - trivium lingua
     * ipsa definiente. COMMENTARIUM* SUBSTANTIVUM manet: nodus est
     * (H6), et munus COMMENTUM ligatorem id vorare sineret. */
    { "SPATIA",         NIHIL, MATERIA_LEX_VERBATIM, MATERIA_MUNUS_SPATIUM },
    { "DELIM",          NIHIL, MATERIA_LEX_VERBATIM, MATERIA_MUNUS_SPATIUM },
```

  `HTML_LEXICON = { GENERA_HTML, count, "lex-", (s32)-I }`.

- [x] **Step 5: The runner** — copy `css/compile_probationes.sh` to
  `html/compile_probationes.sh`; rename every `css`/`CSS` (dir, log
  `build/test_logs/html.log`, banner `HTML PROBATIONES: N/M praeteritae`,
  mensor prefix `html.`); `RADIX_FONTES` = css's list with `css_lexema` →
  `html_lexema` and `stml_html` added (md's list is the model). Keep the
  materia sub-fontes loop, the header guard, the sera lock.

- [x] **Step 6: Write the failing registrum probatio** — transcribe
  `css/probationes/probatio_css_registrum.c` for html: `ORDO_EXSPECTATUS`
  = the 22 titles in lexer order; `GENERA_EXSPECTATA` = the ten titles;
  the recens gate over `"html/grammatica/html.registrum.stml"`; the
  `munus LINEA` absence + the line-sensitive capability REFUSED
  (positive assertions); contiguity of `loci_offset`; one minimal tree
  (`<br>` as `elementum` with `tok_apertura` + `tok_finis` only, inside a
  `documentum`) through writer → reader → writer, byte-equal. Add the
  named-slot table (`LOCI_NOMINATI[]`, the css pattern at
  `probatio_css_arbor.c:43-120`) asserting every enum member above
  against the table's titles and that the count equals `numerus_locorum`
  (18) — put it HERE, not in the arbor probatio, so H3 inherits it.
  Register the suite in pythonica NOW (`PORTAE`, `FORMAE`, `SUITAE`;
  the mensor prefix and `metiri` wait for H9): a runner outside the
  gate table is a dead gate, and the H1 commit gates on `html`.

- [x] **Step 7: Run and verify it fails** —
  `./html/compile_probationes.sh registrum`. Expected: compile of the
  probatio fails until the fontes exist; then green. Exit 2 = the runner
  matched nothing = a path is wrong.

- [x] **Step 8: Plant** — `silva.planta('html/fontes/html_lexicon.c',
  <the SPATIA row>, <the DELIM row swapped into its place>, 'html',
  'registrum')` → the order-by-title assertion must go red; reverted
  green. Second plant: edit one byte of the generated `.c` → the recens
  gate names the line.

- [x] **Step 9: Words, format, commit** — `./oratio/quaere.sh` on every new
  identifier word; `./silva/formator.sh html/fontes/*.c html/fontes/*.h
  html/probationes/*.c -scribere` then `-vitia`; then

```python
silva.commissio("html: cliens materiae natus - declaratio registri, tabulae coctae, lexicon, cursor, porta registri (rubra nata: ordo lexici)",
    ["html/grammatica/html.registrum.stml", "html/fontes/html_registrum_coctum.h",
     "html/fontes/html_registrum_coctum.c", "html/fontes/html_registrum.h",
     "html/fontes/html_lexicon.h", "html/fontes/html_lexicon.c",
     "html/compile_probationes.sh", "html/probationes/probatio_html_registrum.c",
     "html/CLAUDE.md"], ["html"])
```

---

## Task H2: Adaptare, ligator, byte-coverage gate

**Files:**
- Create: `html/fontes/html_adaptare.{h,c}`
- Create: `html/probationes/probatio_html_adaptare.c`

**Interfaces:**
- Consumes: `html_lexare(piscina, fons, mensura)` → `Xar<HtmlLexema>`
  (`genus, valor, offset, linea, columna`); `materia_token_creare(piscina,
  forma, genus, valor, offset, linea, columna, fons_index)` (8 args, as
  `css_adaptare.c:67`); `materia_token_trivia_post_ponere` /
  `_ante_ponere(token, piscina, MateriaToken** plana, n)`;
  `materia_lexicon_munus`.
- Produces:

```c
Xar* html_adaptare (Piscina*, Xar* lexemata_html,
                    constans MateriaTokenForma* forma);   /* UNUM PRO UNO */
nomen structura {
             Piscina* piscina;
    constans MateriaLexiconRatum* lexicon;
                 Xar* pendentia;
        MateriaToken* prior;
} HtmlLigator;
vacuum html_ligator_incipere (HtmlLigator*, Piscina*, constans MateriaLexiconRatum*);
b32    html_ligator_cumulare (HtmlLigator*, MateriaToken* trivium);
b32    html_ligator_solvere  (HtmlLigator*, MateriaToken* sequens);
```

- [x] **Step 1: Write the failing test** — for each fixture in
  `probationes/fixa/html/` (the three): lex, adapt, assert
  `xar_numerus(exitus) == xar_numerus(lexemata)`, and per index genus,
  offset, and value bytes equal. Then the BYTE COVERAGE gate: walk the
  adapted tokens through a ligator exactly as the parser will (trivia
  cumulated, significants solved), then concatenate every token's
  `spatia_ante`, value, `spatia_post` in order and `memcmp` against the
  source; assert the byte count too. Then the segmented-Xar contract:
  build a 23-trivia run while allocating from the same piscina between
  `cumulare` calls (the css lesson: contiguity by luck), solve, assert
  all 23 land. Self-measure: fixtures read == 3.

- [x] **Step 2: Run and verify it fails** — `./html/compile_probationes.sh adaptare`.

- [x] **Step 3: Implement** — transcribe `css/fontes/css_adaptare.c`
  (one-for-one loop; `fons_index` ZEPHYRUM; `initium_lineae` never set,
  same comment: html has no `munus LINEA`). The ligator is css's with the
  divisio rule unchanged (`_terminatorem_lineae_fert` over `\n \r \f`).
  Header comment: trivia exist INSIDE TAGS ONLY (spec §11.5); between
  tags every byte is `TEXTUS` and the ligator is never consulted.

- [x] **Step 4: Run and verify** — green, including coverage.

- [x] **Step 5: Plant** — in `html_ligator_solvere`, `plana + divisio + I`
  for one branch → coverage red by a byte count; revert.

- [x] **Step 6: Format, words, commit**

```python
silva.commissio("html: adaptatio UNUM PRO UNO + ligator triviorum intra tag; porta tegumenti octetorum (rubra nata: trivium omissum)",
    ["html/fontes/html_adaptare.h", "html/fontes/html_adaptare.c",
     "html/probationes/probatio_html_adaptare.c"], ["html"])
```

---

## Task H3: The builder, part one — document, elements, attributes, content

**Files:**
- Create: `html/fontes/html_arbor.{h,c}`
- Create: `html/probationes/probatio_html_arbor.c`

**Interfaces:**
- Consumes: H1 enums, H2 adaptare + ligator, `materia_nodus_creare(piscina,
  genus, numerus_locorum)`, `materia_nodus_ponere(nodus, locus, valor,
  species)`, `materia_nodus_appendere(piscina, nodus, locus, valor,
  species)`, `materia_valor_token/nodus`, `materia_arbor_patres_figere`
  (exported B6: parents fixed by the placing verbs since 2026-09-10 — call
  it once on the root anyway, css precedent, and assert it is a no-op),
  `materia_scribere_nodum` (the emitter).
- Produces:

```c
/* Documentum parsare. TOTALIS: quaelibet series octetorum arborem
 * generis HTML_GENUS_DOCUMENTUM reddit; NIHIL solum memoria deficiente.
 * ITERATIVUS: acervus elementorum apertorum Xar est, numquam recursio
 * C (spec par. XI.7) - profunditas fontis acervum C non tangit.
 * FONS NON COPIATUR. */
MateriaNodus* html_arbor_parsare (Piscina*, constans character* fons, i32 mensura);
```

**The builder, this task's scope** (void/implied/raw arrive in H4; here
every open element closes only by its own end tag or EOF):

- `documentum` root; `HtmlParsura {piscina, tokens, index, ligator,
  acervus (Xar<MateriaNodus*>), documentum}`; the "current parent" is the
  top of the stack or the document.
- `TAG_APERTURA` → new `elementum`; `tok_apertura`; then attributes until
  `TAG_FINIS`/`TAG_FINIS_SOLUS`/EOF: `ATTRIBUTUM_NOMEN` opens an
  `attributum` (`tok_nomen`), `AEQUALE` → `tok_aequale`, `ATTRIBUTUM_VALOR*`
  → `tok_valor`; an `AEQUALE` or value with no open attribute → an
  `attributum` node with that token alone (total; H4-shaped absence);
  `SPATIA`/`DELIM` → `ligator_cumulare`; every significant → `ligator_solvere`
  first. `TAG_FINIS*` → `tok_finis`; the element is appended to the
  current parent's `liberi` and PUSHED. EOF inside the tag: `tok_finis`
  absent, element still pushed (truncation is expressed, §4.2).
- `TAG_CLAUSURA` → find the nearest stack entry whose tag name equals
  this one ASCII-case-insensitively (compare `valor` after `<`/`</`);
  found → pop to it (entries above close with absent `tok_clausura*`),
  set `tok_clausura` and the following `TAG_FINIS` as
  `tok_clausura_finis`; not found → `elementum-malum` with the clausura
  token (+ its `>`) in `tokens`, appended to the current parent.
- Content tokens → the genus of spec §11.3's mapping, appended to the
  current parent's `liberi`.
- `FINIS` → `ligator_solvere(finis)`; `documentum.cauda = finis`; every
  open element stays as it is (absent closes).

- [x] **Step 1: Write the failing tests** — `_octetos_probare(piscina,
  fons, mensura)` = parse → `materia_scribere_nodum` → length equal and
  `memcmp` (lift the css helper, four lines). Cases:

```c
    CREDO_VERUM (_octetos_probare(piscina, "<p>a</p>", VIII));
    CREDO_VERUM (_octetos_probare(piscina, "<div id=\"x\" class='y' z>t</div>", XXXI));
    CREDO_VERUM (_octetos_probare(piscina, "<a  href = x\n  >b</A >", XXII));
    CREDO_VERUM (_octetos_probare(piscina, "x &amp; y <!-- c --> <![CDATA[z]]>", XXXIV));
    CREDO_VERUM (_octetos_probare(piscina, "<!DOCTYPE html><html></html>", XXVIII));
    CREDO_VERUM (_octetos_probare(piscina, "<div cl", VII));        /* EOF intra tag */
    CREDO_VERUM (_octetos_probare(piscina, "</x>a", V));            /* malum */
    CREDO_VERUM (_octetos_probare(piscina, "<div>", V));            /* apertum ad EOF */
    CREDO_VERUM (_octetos_probare(piscina, "<img/>", VI));
    CREDO_VERUM (_octetos_probare(piscina, "<p>a</P>", VIII));      /* casus */
    CREDO_VERUM (_octetos_probare(piscina, "", ZEPHYRUM));
```

  Structure (§4.2 rows this task can assert): `<div>` at EOF →
  `tok_clausura` and `tok_clausura_finis` NIHIL; `<img/>` → `tok_finis`
  holds `/>`, clausura slots NIHIL; `<p>a</P>` → one `elementum`, closed
  (case-insensitive); `</x>a` → `liberi` = [`elementum-malum`, `textus`];
  `<div cl` → `elementum` with one `attributum` (`tok_nomen` only) and
  `tok_finis` NIHIL; `<div id="x" class='y' z>` → three attributes, the
  third with `tok_aequale` and `tok_valor` NIHIL. Count byte lengths by
  hand and write the count beside each literal (the css plan's XXVI/XXVII
  lesson).

- [x] **Step 2: Run and verify it fails** — `./html/compile_probationes.sh arbor`.

- [x] **Step 3: Implement** (scope above). Write with `./silva/scribe.sh`.

- [x] **Step 4: Run the whole suite** — `./html/compile_probationes.sh`,
  exit 0; registrum's named-slot table still holds.

- [x] **Step 5: Plant** — drop the `tok_clausura_finis` assignment →
  `_octetos_probare("<p>a</p>")` red by one byte; revert.

- [x] **Step 6: Format, words, commit**

```python
silva.commissio("html: aedificator simplex I - documentum, elementa, attributa, contentum; acervus apertorum Xar (numquam recursio); octeti idem (rubra nata: finis clausurae omissa)",
    ["html/fontes/html_arbor.h", "html/fontes/html_arbor.c",
     "html/probationes/probatio_html_arbor.c"], ["html"])
```

---

## Task H4: The builder, part two — void elements, implied closes, raw text

**Files:**
- Modify: `html/fontes/html_arbor.c`
- Modify: `html/probationes/probatio_html_arbor.c`

**Interfaces:** none new. Two TABLES (M7), `hic_manens constans`:

```c
/* WHATWG, XIII: numquam aperiuntur */
hic_manens constans character* VOID_ELEMENTA[] = {
    "area", "base", "br", "col", "embed", "hr", "img", "input",
    "link", "meta", "source", "track", "wbr"
};

/* Clausurae implicitae (spec par. XI.7): elementum apertum -> tags
 * apertionis quae id claudunt. Ordo apertus: linea nova = regula. */
nomen structura {
    constans character*  apertum;
    constans character** claudentia;
                    i32  numerus;
} ClausuraImplicita;
/* "p" <- BLOCA (address article aside blockquote details div dl
 *   fieldset figcaption figure footer form h1 h2 h3 h4 h5 h6 header
 *   hgroup hr main menu nav ol p pre section table ul)
 * "li" <- li · "dt"/"dd" <- dt dd · "option" <- option optgroup
 * "optgroup" <- optgroup · "tr" <- tr tbody thead tfoot
 * "td"/"th" <- td th tr tbody thead tfoot
 * "thead"/"tbody"/"tfoot" <- tbody thead tfoot */
```

Rules: on `TAG_APERTURA` of name N, BEFORE pushing: while the top of the
stack is an element E such that N is in `CLAUSURAE_IMPLICITAE[E]`, pop E
(absent closes). A void N is appended, never pushed. Raw text needs no
rule: the lexer emits `TEXTUS_CRUDUS` after `script style title
textarea`, the builder appends `textus-crudus` (H3 already does).

- [x] **Step 1: Write the failing tests** — §4.2's remaining rows and the
  tables:

```c
    CREDO_VERUM (_octetos_probare(piscina, "<p>a<p>b", VIII));
    CREDO_VERUM (_octetos_probare(piscina, "<ul><li>a<li>b</ul>", XIX));
    CREDO_VERUM (_octetos_probare(piscina, "<br><br/><BR>x", XIV));
    CREDO_VERUM (_octetos_probare(piscina, "<p>a<div>b</div>", XVI));
    CREDO_VERUM (_octetos_probare(piscina, "<table><tr><td>1<td>2<tr><td>3</table>", XXXVIII));
    CREDO_VERUM (_octetos_probare(piscina, "<script>a</b></script>", XXII));
    CREDO_VERUM (_octetos_probare(piscina, "<textarea><p></textarea>", XXIV));
    CREDO_VERUM (_octetos_probare(piscina, "</br>", V));
    CREDO_VERUM (_octetos_probare(piscina, "<div><p>x</div>", XV));
```

  Structure: `<p>a<p>b` → two sibling `p`, the first with absent
  `tok_clausura*` and `liberi` = [`textus`]; `<br><br/><BR>x` → three
  `elementum` siblings and a `textus`, none with children; `<p>a<div>b</div>`
  → `p` and `div` are SIBLINGS; the table case → 2 `tr`, 3 `td`, every
  implied close absent; `<script>a</b></script>` → `script.liberi` = one
  `textus-crudus`, no `elementum-malum`; `</br>` → `elementum-malum`
  (named deviation); `<div><p>x</div>` → `p` inside `div`, `p` unclosed,
  `div` closed.

- [x] **Step 2: Run and verify it fails** — `./html/compile_probationes.sh arbor`.

- [x] **Step 3: Implement** — the two tables + the pre-push loop + the
  void check. Case-insensitive comparisons through one helper
  `_titulus_aequalis(chorda, constans character*)` (ASCII fold; no
  `strncasecmp` — it is not C89).

- [x] **Step 4: Run the whole suite** — exit 0.

- [x] **Step 5: Plant** — remove `"p"` from the block list → the
  `<p>a<div>` sibling assertion red; revert.

- [x] **Step 6: Format, commit**

```python
silva.commissio("html: aedificator simplex II - elementa vacua (XIII), clausurae implicitae (tabula), textus crudus lexatoris; loci absentes = sensus (H4) (rubra nata: p ex blocis sublatum)",
    ["html/fontes/html_arbor.c", "html/probationes/probatio_html_arbor.c"], ["html"])
```

---

## Task H5: The corpus gate and the second adversarial fixture

**Files:**
- Create: `probationes/fixa/html/adversarius_2.html`
- Create: `html/probationes/probatio_html_corpus.c`

- [ ] **Step 1: Write `adversarius_2.html`** — spec §11.9's list: CRLF line
  endings THROUGHOUT (write it with `printf` or Python so the `\r\n` are
  real), unclosed everything, `</` at EOF, `<b><i>x</b></i>`, `<table>`
  with stray text between rows, `<script>var s = "</script";</script>`,
  `<DiV>` / `</div>`, duplicate attributes, `<p>` chains, `<input
  disabled>`, `<a href=/x/y>`, a bare `&`, `]]>` in text, `<div>` nested
  200 deep, and the literal strings `</lex-textus>` and
  `</lex-textus-crudus>` inside text. No NUL (rides inline in H8).

- [ ] **Step 2: Write the failing corpus gate** — `CORPUS[]` = the four
  fixtures + the ten live house pages of spec §11.9 (paths relative to
  `RHUBARB_RADIX`, the css pattern). For each: read (piscina), parse,
  `materia_scribere_nodum`, `memcmp`; tally causes (`idem / NIHIL /
  emissio fracta / dispar`), print the first divergent offset. Self-measure:

```c
    CREDO_AEQUALIS_I32 (plagulae_lectae, XIV);
    CREDO_MAIOR_I32    (octeti_probati, C * M);   /* > 100 000 */
```

- [ ] **Step 3: Run and read failure #1 first** — judge by the cause tally;
  a failure here may be upstream (adaptare, ligator). One cause per commit.

- [ ] **Step 4: Fix causes until green** — a substrate defect (materia)
  gets its own gate in `materia/probationes` and its own commit before
  this one continues.

- [ ] **Step 5: Plant** — skip `documentum.cauda` for one file → that file
  short by its trailing bytes → red; revert.

- [ ] **Step 6: Commit**

```python
silva.commissio("html: porta corporis - XIV plagulae (fixa IV + paginae domus X, > C milia octetorum) octetim exactae, tegumentum se metiens; adversarius II (CRLF, </lex-textus> in textu, nidificatio CC)",
    ["probationes/fixa/html/adversarius_2.html",
     "html/probationes/probatio_html_corpus.c"], ["html"])
```

---

## Task H6: STML projection — two cycles, two oracles

**Files:**
- Create: `html/probationes/probatio_html_stml.c`

**Interfaces (verified against `materia_arbor.h` 2026-09-15):**

```c
    MateriaLexiconRatum ratum; MateriaLexIudicium iudicium;
    MateriaArborConsilium consilium; MateriaArborVitium vitium;
    materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium);
    materia_arbor_consilium_nudum(&consilium, &HTML_REGISTRUM, &ratum, "html");
    MateriaArborScriptura s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    MateriaNodus* r = materia_arbor_legere(piscina, intern, s.textus, &consilium, &vitium);
    b32 materia_arbor_aequalis(a, b, MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d);
```

FIDELITAS is not runnable (no `munus LINEA`; the reader's cursor never
advances lines) — STRUCTURALIS is the asserted mode; say so in a comment.

- [ ] **Step 1: Write the failing gate** — inline cases (every H3/H4 case
  string) AND the whole H5 corpus: `s1 = scribere(radix)`; `r1 =
  legere(s1)`; `s2 = scribere(r1)`; `CREDO_CHORDAE_AEQUALES(s1, s2)`;
  `CREDO_VERUM(aequalis(radix, r1, STRUCTURALIS))`; `s1.census.spatia_vocationes
  == 0` (compression dead: VERBATIM spatia); on refusal print
  `s.causa`. Self-measure: documents round-tripped == 14 + inline count.
  The `</lex-textus>`-in-text case is the one to watch: the raw form
  refuses a value containing its own closing tag — the writer must fall
  back to escaped text, not refuse the document. If it refuses, that is
  a substrate finding (md split its values by hand; html cannot): stop,
  name it, gate it in materia.

- [ ] **Step 2: Run and verify it fails** — `./html/compile_probationes.sh stml`.

- [ ] **Step 3: Fix what the refusals name** — causes, never assertions.

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Plant** — before the comparator, `materia_nodus_ponere(r1,
  HTML_DOCUMENTUM_CAUDA, VALOR_NIHIL, TOKEN)` on the re-read tree → the
  tree oracle red while bytes of `s1`/`s2` stay equal (this is the
  oracle-separation pin: bytes IDEM, tree DISPAR); revert.

- [ ] **Step 6: Commit**

```python
silva.commissio("html: circuitus STML - duo cycli octetim idem, comparator STRUCTURALIS (FIDELITAS: sine munere LINEA), census compressionis nulla, corpus totum (rubra nata: cauda relectae sublata - octeti idem, arbor dispar)",
    ["html/probationes/probatio_html_stml.c"], ["html"])
```

---

## Task H7: `html.canon` — hand-written, drift-guarded, seal pinned

**Files:**
- Create: `html/grammatica/html.canon`
- Create: `html/probationes/probatio_html_canon.c`

Model on `css/grammatica/css.canon` (read its header first): the `<arbor>`
envelope with `grammatica="html"` + `registrum-sigillum` pinned; fragments
`<#lexema-unum>` / `<#lexemata-multa>` over the 22 `lex-*` tags (tag
mangling: `_` → `-`, lower case — check one against `s1.textus` from H6
rather than assuming); one rule per genus constraining loci at SPECIES
level (G3, stated in the header); trivia forms `<ante>`/`<post>` allow
`lex-spatia` and `lex-delim` only. NOT in `canones.registrum`. `id` on
node elements is NOT declared (html has no references; an `id` appearing
would be a refusal, correct).

- [ ] **Step 1: Write the failing gate** — `canon_legere(chorda fons,
  piscina, intern, &causa)`; for every H6 document (inline + corpus):
  `stml_legere` the projected text, `canon_iudicare(canon, radix,
  piscina)`, assert non-NIHIL and `xar_numerus(vitia) == 0`; self-measure
  `documenta_iudicata > 0`. The drift guard: walk the canon's rules —
  every `HtmlGenus` title has exactly one rule and the count is
  `HTML_GENUS_NUMERUS_GENERUM`; every locus title appears under its
  genus's rule; every lexicon title has its `lex-*` in both fragments;
  every genus rule in the canon names a live genus (reverse); the pinned
  `registrum-sigillum` equals `materia_arbor_sigillum(piscina,
  &HTML_REGISTRUM)` live.

- [ ] **Step 2: Run and verify it fails** — no canon yet.

- [ ] **Step 3: Write the canon**; `./tools/natura_struere.sh`.

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Plant** — delete the `textus` genus rule → the guard AND the
  judgment must both fire (a document with text is now unjudgeable-clean);
  revert. Second plant: pin a false sigillum → guard red.

- [ ] **Step 6: Commit**

```python
silva.commissio("html: canon manu scriptum (X genera, loci ad gradum speciei, lexemata XXII, trivia intra tag sola) - custos derivae utrimque, sigillum pinnatum, corpus iudicatum (rubra nata: regula textus deleta)",
    ["html/grammatica/html.canon", "html/probationes/probatio_html_canon.c"], ["html"])
```

---

## Task H8: The reservation gate and the totality gate

**Files:**
- Create: `html/probationes/probatio_html_reservatio.c`
- Create: `html/probationes/probatio_html_totalitas.c`

- [ ] **Step 1: Reservatio — write the failing gate** — build documents by
  hand as text from a real projection (take `s1.textus` of `<p>a</p>` and
  splice): (a) `<reinserendum/>` as a child of `<elementum>` →
  `materia_arbor_legere` NIHIL, `vitium.causa` equals `"locus generi
  ignotus"`; same for `<ancora/>`, `<spatium/>`, `<clonatum/>`; (b)
  `<reinserendum/>` as a child of `<liberi>` → `"genus registro ignotum"`;
  (c) `ancora="x"` / `spatium="svg"` / `clonatum="verum"` as attributes
  on `<elementum>` → the READER accepts (assert non-NIHIL: this is the
  measured fact of spec §11.6, pinned so a future attribute census in
  materia is noticed) AND the canon judges it with ≥ 1 vitium naming the
  attribute. Every message compared by `strcmp`, not by prefix.

- [ ] **Step 2: Totalitas — write the failing gate** — transcribe
  `css/probationes/probatio_css_totalitas.c` (LCG random bytes,
  mutation 1/40, 23 truncation steps, nesting forms) over the html
  corpus with `html_arbor_parsare` + `materia_scribere_nodum` under
  `CREDO_NON_RUIT`; failing source written to
  `html/build/totalitas_fractum.html`. The depth section: parse ALONE of
  `<div>` × 100 000 (open, and open+closed) → `CREDO_NON_RUIT` — the
  builder is iterative, this MUST hold; then parse + emit at 100 000 →
  measure: if `materia_scribere_nodum` survives, `CREDO_NON_RUIT`; if it
  dies, `CREDO_RUIT_CUM(…, SIGSEGV)` with the depth named and `01M1FAD8`
  cited in the comment (a pin that goes red on the fix, css precedent).
  Then the STML write at the same depth, same rule. NUL: an inline
  source with a NUL byte parses and emits byte-exact; the STML writer's
  refusal of it is asserted as a NAMED refusal (`causa` non-NIHIL), not
  as a crash.

- [ ] **Step 3: Run both and verify they fail** — `./html/compile_probationes.sh
  reservatio` then `… totalitas`.

- [ ] **Step 4: Implement nothing unless red names a defect** — the
  reservatio gate tests materia's behaviour; totalitas tests the
  builder. A builder crash found here is fixed in `html_arbor.c` with
  the failing source added as a fixture line.

- [ ] **Step 5: Plant** — reservatio: compare against `"locus ignotus"` →
  red; revert. Totalitas: replace the stack loop for `TAG_CLAUSURA` with
  a recursive helper → the 100 000 parse dies → red; revert.

- [ ] **Step 6: Commit**

```python
silva.commissio("html: porta reservationis (lector elementa ignota RECUSAT, attributa ignota IGNORAT - mensuratum, canon ea iudicat) + porta totalitatis (fortuiti, mutati, truncati, nidificatio C milium parsata sine recursione; scriptor/proiectio ad profunditatem mensurati, 01M1FAD8)",
    ["html/probationes/probatio_html_reservatio.c",
     "html/probationes/probatio_html_totalitas.c"], ["html"])
```

---

## Task H9: Computus twin, instruments, pythonica registration

**Files:**
- Create: `html/fontes/html_computus.{h,c}`, `html/instrumenta/computus.c`,
  `html/computus.sh`, `html/arbor.sh`
- Create: `html/probationes/probatio_html_computus.c`,
  `html/probationes/fixa/computus/basis.tsv`
- Modify: `pythonica/silva.py` (`PORTAE`, `FORMAE`, the mensor `praef`
  dict, `SUITAE`, `metiri`'s client-by-suffix)

- [ ] **Step 1: Write the failing computus gate** — transcribe
  `probatio_css_computus.c`: corpus = the four html fixtures, the same
  eleven deterministic columns, golden `basis.tsv`, `COMPUTUS_SCRIBERE=1`
  regenerates (a named cause in the commit).

- [ ] **Step 2: Run and verify it fails** — no `html_computus` yet.

- [ ] **Step 3: Implement** — `html_computus_metiri(fons, mensura,
  &exitus)` = css's twin (`HtmlComputus` with the same fields; parse in
  its own piscina with `HTML_COMPUTUS_ALVEUS_INITIUM`; emit; STML write /
  read / compare in a second piscina; `clock()` ms). `html/computus.sh`
  and `html/arbor.sh <x.html>` (projection to stdout, md's `arbor.sh`
  shape) from the css/md scripts. Generate the golden once with
  `COMPUTUS_SCRIBERE=1`, read it, commit it.

- [ ] **Step 4: Register in pythonica** — `PORTAE['html'] =
  (['./html/compile_probationes.sh'], r'HTML PROBATIONES: \d+/\d+')`;
  `FORMAE['html'] = 'suita'`; the mensor `praef` dict gains
  `'html': 'html.'`; `SUITAE['html'] = ('html/probationes',
  'html/build/%s')`; `metiri`: `.html` → `html/computus.sh`. Then
  `silva.porta('html')` from Python must run the suite and parse the
  banner; `./pythonica/probare.sh` stays green.

- [ ] **Step 5: Plant** — edit one `nodi` count in `basis.tsv` → red; revert.

- [ ] **Step 6: Commit**

```python
silva.commissio("html: computus gemellus (aurum fixa/computus/basis.tsv), instrumenta computus.sh/arbor.sh, registratio pythonica (PORTAE/FORMAE/SUITAE/metiri) (rubra nata: aurum mutatum)",
    ["html/fontes/html_computus.h", "html/fontes/html_computus.c",
     "html/instrumenta/computus.c", "html/computus.sh", "html/arbor.sh",
     "html/probationes/probatio_html_computus.c",
     "html/probationes/fixa/computus/basis.tsv", "pythonica/silva.py"],
    ["html", "pythonica"])
```

---

## Task H10: Consumer of a consumer — md's rendered HTML through the parser

**Files:**
- Modify: `html/compile_probationes.sh` (compile `md/fontes/*.c` into
  `html/build/` as dependencies, the way materia's are; md's own
  `RADIX_FONTES` are already a subset of html's after H1)
- Create: `html/probationes/probatio_html_md.c`

**Interfaces:**
- Consumes: `md_html_reddere(piscina, intern, fons, mensura, programma)` →
  `MdHtmlResultus {successus, html, causa, stml}`; the programme text is
  `md/html/md-html.stml` read by the caller (`probatio_md_html.c:137-142`
  pattern, `RHUBARB_RADIX`); md fixtures `md/probationes/fixa/md/*.md`
  (four) and every tracked `*.md` under `md/` and `project-specs/`? — NO:
  the four fixtures plus `md/CLAUDE.md`, `css/CLAUDE.md`, `materia/CLAUDE.md`
  (house prose with code spans, lists, tables). Seven inputs, listed.

- [ ] **Step 1: Write the failing gate** — for each input: render; assert
  `successus`; parse the html; emit → byte-equal to the rendering;
  project + read back + compare (H6's trio); judge with `html.canon`
  (H7's loader); walk the tree and count `elementum-malum` — assert
  ZERO (md's HTML is well-formed: every end tag has its partner). Self-
  measure: inputs rendered == 7, total html bytes > 0.

- [ ] **Step 2: Run and verify it fails** — link errors until the runner
  compiles md's sources; then green or a real finding (a rendering the
  parser mis-nests is a finding about md OR html — name which by
  reading the bytes).

- [ ] **Step 3: Runner** — add the md loop (`for m in md_*`, list them
  explicitly as the materia loop does; exclude nothing).

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Plant** — the plant lives in the DATA: append
  `"<p>unclosed<b>x</p>"` as an eighth input rendered by hand (not by
  md) and assert the malum count is still zero → red, proving the
  counter sees a mismatch; then move that input under a separate
  assertion that EXPECTS one malum (it stays as the positive control).

- [ ] **Step 6: Commit**

```python
silva.commissio("html: consumens consumentis - html redditum ab md (VII initus) parsatur, octetim idem, proicitur, canone iudicatur, elementum-malum NULLUM (testis positivus: unum exspectatum); cursor fontes md compilat",
    ["html/compile_probationes.sh", "html/probationes/probatio_html_md.c"], ["html"])
```

---

## Task H11: Closure — records, audit, ledger, memory

**Files:**
- Create: `html/fontes/html_arbor.worklog.md`
- Modify: `html/CLAUDE.md` (full orientation: map, gates, laws, birth
  lessons — the md/css shape)
- Modify: `project-specs/html-arbor-spec.md` (§11 "as built" paragraphs
  where the build diverged from §11), `project-specs/materia-spec.md`
  (§10 "As built (phase 4)" paragraph), `materia/phase-log.md` (RELATIO:
  what HTML found in the substrate — expect an answer either way — with
  the instrument debrief and the SHIM AUDIT line), `MAP.txt` if it lists
  clients
- Memory: `materia-fork.md`, `MEMORY.md` materia line

- [ ] **Step 1: Run the phase-boundary audit** — `./materia/shim_probare.sh`;
  record `probatae N, fractae 0` and the wall in the phase-log RELATIO.
  Then every client suite that consumes materia: `css`, `md`, `oratio`,
  `materia`, `html`, `briar` — all green, no pin moved (or the pin's
  cause named).

- [ ] **Step 2: Worklog** — what the corpus gate found that unit tests
  missed; where the seam differed from §11's predictions; every §11.10
  question answered (absent slots, empty trivia, raw form refusal,
  depth, consumer of consumer); the substrate changes html forced (count
  them: css forced four, oratio five).

- [ ] **Step 3: Spec + phase-log** — paragraphs, not rewrites.

- [ ] **Step 4: Ledger** — `gerere 01M12BD0 status clausum` (S6 IS
  materia; the second-client evidence is this client's gates);
  `gerere 01KY0P4TFN status impletum` (analyzator HTML); a note on decree
  `01M2KC03NG` (HTML done, JS next); the instrument debrief (adhibita /
  fructus / asperitates / desiderata) in the RELATIO.

- [ ] **Step 5: Memory** — materia-fork.md paragraph "2026-09-xx (html,
  fifth client)"; MEMORY.md materia line: HTML DONE, NEXT = JS (spec
  §10's exercise on paper first: regex-vs-division, template
  interpolation).

- [ ] **Step 6: Commit**

```python
silva.commissio("html: cliens navatus - relatio phasis IV (auditus shim N/N), worklog, spec ut aedificatum, acta clausa (S6 clausum, analyzator HTML impletum), memoria",
    ["html/fontes/html_arbor.worklog.md", "html/CLAUDE.md",
     "project-specs/html-arbor-spec.md", "project-specs/materia-spec.md",
     "materia/phase-log.md"], ["html", "materia"])
```

---

## Self-Review

**Spec coverage (§11 governs).** §11.3 declaration → H1. §11.4 lexicon →
H1 (rows), asserted by title in H1's probatio. §11.5 trivia inside tags →
H2 (ligator) and the coverage gate. §11.6 reservations → H8 (three rows,
each a named cause; the reader-ignores-attributes fact PINNED). §11.7
tables and the iterative stack → H3/H4 (builder), H8 (depth). §11.8 gate
table → H1 registrum, H2 adaptare, H3/H4 arbor, H5 corpus, H6 stml, H7
canon, H8 reservatio + totalitas, H9 computus, H10 md; the shim audit →
H11. §11.9 corpus → H5. §11.10 questions → H11's worklog. Body §4.2 rows →
H3 (EOF, `<img/>`, case) and H4 (void, implied). Body §8 gates 1–8 → all
mapped (gate 1 is the existing root probatio; gate 8 is every task's
plant step). Body §6 parity mechanisms → out of scope by H1, format
readiness re-stated in §11.6.

**Placeholder scan.** None. Every byte count beside a literal is to be
hand-counted at implementation (the css plan carried one wrong count into
its tests; the step says count). H5 step 3–4 prescribe a process (one
cause per commit) because a corpus gate's content is its corpus, which is
enumerated.

**Type consistency.** `html_arbor_parsare(Piscina*, constans character*,
i32)` defined H3, consumed H5–H10. `HtmlLigator` + three verbs defined H2,
consumed H3. Slot enums defined H1, consumed H3/H4/H6/H8. `HtmlComputus`
H9 mirrors `CssComputus` field for field. Materia signatures transcribed
from headers read 2026-09-15: `materia_arbor_consilium_nudum` 4 args,
`materia_arbor_legere` 5, `materia_arbor_scribere_nodum` 3,
`materia_token_creare` 8 (as `css_adaptare.c:67`), `materia_registrum_recens`
4, `canon_legere` / `canon_iudicare` as the css plan cites.

**One honest gap.** H6's `</lex-textus>`-in-text case may refuse at the
writer: whether materia's lexeme path falls back to escaped text when the
raw form is impossible, or refuses, is not known until the bytes run. The
step says stop and name it. That is the first place this client could
pay a substrate change, and it is named before the run.

**One decision Fran can veto (spec §11.6).** v1 declares nothing
reserved. If Fran wants the four extension points as loci from birth, H1
appends them to the declaration (species `index` for `spatium`/`clonatum`,
`token` for `ancora`, a genus `reinserendum`), H8 asserts that a document
filling any of them is refused by a client-side walk after
`materia_arbor_legere` — one function, one gate, no other task changes.
