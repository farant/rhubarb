# css_arbor Plan B — T10–T17 re-issued materia-native

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> (inline, per house convention — no subagents) to implement this plan
> task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Finish the CSS parser on materia — rules, declarations, at-rules,
selectors, the corpus and STML gates, a hand-written canon — plus the
arbor_aequalitas comparator port that gives the round trip its second oracle.

**Architecture:** unchanged from `css-arbor-spec.md` §2, retargeted to
materia. `css_lexare` → `css_adaptare` (one-for-one) → `CssLigator` (parser
holds trivia state) → recursive descent building `MateriaNodus` against
`CSS_REGISTRUM`. Emission via `materia_scribere_nodum` (separating oracle);
projection via `materia_arbor_scribere_nodum` / `materia_arbor_legere`.

**Why this plan exists:** the original `css-arbor-plan.md` T10–T17 bodies
predate two retargets and the T8/T9 findings — their code steps still carry
silva types, T10 step 1 asks to build a gate T8 already built, and T11's
step 3 names the impossible bimodal-adaptare signature first. This plan
replaces them. T1–T9 remain historical record in the old plan. Audit
provenance: materia/phase-log.md AUDITUM entry, 2026-09-01.

**Tech Stack:** C89 (`-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion
-Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
-Wwrite-strings -Wno-long-long -Wno-overlength-strings`), latina.h, credo,
piscina, chorda, Xar, materia.

**Spec:** `project-specs/css-arbor-spec.md` (+ D7 amendment in B3 below).

## Task mapping

| old | new | what changed |
|---|---|---|
| — | **B1** | NEW: materia_arbor_aequalitas port (desideratum `01M1EZKKP0`) |
| T10 | **B2** | gate already exists (T8); preludes content-mode from birth |
| T11 | **B3** | D7 strengthened: whitespace AND comments are content |
| T12 | **B4** | types only; old byte-count XXVI for `@media…` was WRONG (XXVII) |
| T13 | **B5** | types only |
| T14 | **B6** | materia signatures; comparator from B1; FIDELITAS note |
| T15 | **B7** | canon HAND-WRITTEN (Fran 2026-09-01), drift-guarded |
| T16 | **B8+B9** | registry extension split out; combinator over servatum leaves |
| T17 | **B10** | + spec/ledger closure items from the audit |

## Global Constraints

- **Latin throughout** — identifiers, comments, messages. `offset`, `css`,
  `stml`, `token` stay untranslated (house precedent).
- **Every latina.h word is a FORBIDDEN identifier** (`nomen`, `registrum`,
  `magnitudo`, `structura`, `casus`, `brevis`, `exire`…). `_Capital` is
  implementation-reserved. Use `titulus`/`tabularium`.
- **`chorda` is NOT null-terminated.** **`i32`/`i64` are UNSIGNED**;
  anything subtraction-derived takes `s32`/`s64`.
- **`??-` in a string literal is a trigraph; `<*/>` or `lib/*.c` inside a
  BLOCK COMMENT breaks it.**
- **Exit 2 from any runner = NOTHING RAN.** Never gate on exit 0 alone.
  Never read `$?` after a pipe — zsh: `${pipestatus[1]}`.
- **Runners:** `./css/compile_probationes.sh [filter]` (auto-discovers
  `fontes/*.c` and `probationes/probatio_*.c` — no build wiring per task),
  `./materia/compile_probationes.sh`, `./materia/shim_probare.sh [-stml]`.
  Logs: `build/test_logs/{css,materia}.log`.
- **A planted fault at each gate's birth** — it must fail the assertion,
  not the build. A planted fault that does not fire is a measurement of the
  TEST (find the luck, remove it — the Xar-contiguity lesson).
- **Positions are byte offsets** + 1-based linea/columna.
- After any edit to STML files: `./tools/natura_struere.sh`.
- **Never stage Fran's staging files** (FAQ.md, gesta/annales/*,
  officina/instrumenta/legatus.worklog.md, silva/grammatica/*.stml
  untracked). Stage explicitly by path.
- Silva stays FROZEN: B1 touches `materia/` and `silva/probationes`-class
  files only, never `silva/fontes`.

---

## Task B1: materia_arbor_aequalitas — the comparator port

**Files:**
- Create: `materia/fontes/materia_arbor_aequalitas.c`
- Modify: `materia/fontes/materia_arbor.h` (declarations — silva precedent:
  aequalitas has no own header)
- Create: `materia/probationes/probatio_materia_aequalitas.c`
- Modify: `materia/instrumenta/shim_c89.c`, `materia/shim_probare.sh`

**Interfaces:**
- Consumes: `MateriaNodus` (materia_nodus.h:103 — `genus`,
  `numerus_locorum`, `loci`, `pater`), `MateriaValor`,
  `MateriaListaProspectus` (**compare by `prospectus.mensura`, NEVER
  `xar_numerus` on the repository** — silva's comment explains: the
  repository is shared across GLR forks and append-only),
  `MateriaToken` (materia_token.h:83 — `valor`, `spatia_ante/post` +
  `numerus_ante/post`, `genus`, `byte_offset`, `fons_index`,
  `materia_token_initium_lineae()`).
- Produces (mirror of silva_arbor.h:338-391, renamed):

```c
#define MATERIA_ARBOR_VIA_CAPACITAS 256   /* = silva's value; VERIFIED at port */

nomen enumeratio {
    MATERIA_ARBOR_COMPARATIO_STRUCTURALIS = 0,
    MATERIA_ARBOR_COMPARATIO_FIDELITAS
} MateriaArborComparatioModus;

nomen structura {
      constans character* campus;
   constans MateriaNodus* nodus_a;
   constans MateriaNodus* nodus_b;
   constans MateriaToken* lexema_a;
   constans MateriaToken* lexema_b;
                     s32  locus;
                     s32  index;
               character  via[MATERIA_ARBOR_VIA_CAPACITAS];
} MateriaArborDifferentia;

b32
materia_arbor_aequalis (
          constans MateriaNodus* a,
          constans MateriaNodus* b,
    MateriaArborComparatioModus  modus,
        MateriaArborDifferentia* differentia);
```

**Port scope, decided:** silva compares `standard` and `scissurae`
(silva_arbor.h:367-368). In materia those live in the **frontend tail**,
whose shape materia cannot see. They are OUT of this port, with the landing
spot NAMED: a frontend-comparison hook, owned by phase 5 (the C89
migration needs it; CSS and HTML never will). Provenance stays IN — silva's
comment (:331-337) is explicit that provenance is compared in BOTH modes as
a structural fact (`byte_offset >= 0` vs synthetic), because "the guard
that passes when its subject is absent" is exactly the bug class. Pater is
compared by NULLITY only (:370-376). The STRUCTURALIS/FIDELITAS split is
**measured from silva_arbor_aequalitas.c during the port, not assumed** —
the header lists what is compared but not per-mode; transcribe the actual
branches.

- [x] **Step 1: Write the failing test** — hand-built trees (the SOLUTUM
  precedent from probatio_stml_html: build with `materia_nodus_creare` +
  `materia_nodus_ponere` + `materia_token_creare`). One equal pair asserts
  VERUM. Then one planted divergence per compared field, each asserting
  FALSUM **and** the named `campus`:

```c
    /* aequales */
    CREDO_VERUM  (materia_arbor_aequalis(a, b,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
    CREDO_NIHIL  (d.campus);

    /* genus nodi divergens */
    CREDO_FALSUM (materia_arbor_aequalis(a, b_genus_alius,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
    CREDO_NON_NIHIL (d.campus);
```

  Divergence cases, one tree pair each: genus nodi · numerus locorum ·
  valor genus per locum · lista mensura (via prospectus — build the case
  where the repository Xar is LONGER than the prospectus and assert
  equality still holds) · token genus · token valor octets ·
  fons_index · initium_lineae · trivia series (genus + valor, order, both
  lists) · pater nullity.

- [x] **Step 2: Run and verify it fails**

Run: `./materia/compile_probationes.sh aequalitas`
Expected: FAIL — `materia_arbor_aequalis` undefined. (Exit 2 = filter
matched nothing = you misnamed the file.)

- [x] **Step 3: Port** — silva_arbor_aequalitas.c is 1,000 lines,
  includes only silva_arbor.h + xar + string.h, zero preprocessor
  references (measured 2026-09-01). Mechanical rename pass
  (Silva→Materia), delete the standard/scissurae comparisons with a
  comment naming the phase-5 landing spot, trivia comparison walks the
  exact arrays (`spatia_ante`/`numerus_ante`) instead of silva's Xars.

- [x] **Step 4: Run and verify all divergence cases fire**

Run: `./materia/compile_probationes.sh aequalitas`
Expected: exit 0, every planted case red-capable (they are separate
assertions, all green because each asserts FALSUM).

- [x] **Step 5: Wire into the shim as the tree oracle** — in
  `materia/instrumenta/shim_c89.c`, after the CIRCUITUS read-back,
  compare the converted tree against the read-back tree:

```c
    si (!materia_arbor_aequalis(conversa, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia))
    {
        /* numerare ut ARBOR_DISPAR; campum et viam imprimere */
    }
```

  The counter joins the exit-code sum (the shim's own lesson: an exit
  code blind to a finding class is half a gate). Assert coverage:
  trees compared == files read back, printed in the tally.

- [x] **Step 6: Plant a shim fault** — temporarily skip trivia
  restoration for one genus in the shim's read path; the tree oracle
  MUST catch it (this divergence is byte-invisible on write-back only
  when ownership migrates — if bytes catch it first, move the plant to
  a divisio-style ownership migration, which bytes cannot see). Restore.

- [x] **Step 7: Run the full belts**

Run: `./materia/compile_probationes.sh && ./materia/shim_probare.sh -stml`
Expected: suites green; shim tally gains `ARBOR: idem N, dispar 0`.

- [x] **Step 8: Commit + close the desideratum**

```bash
git add materia/fontes/materia_arbor_aequalitas.c materia/fontes/materia_arbor.h \
        materia/probationes/probatio_materia_aequalitas.c \
        materia/instrumenta/shim_c89.c materia/shim_probare.sh materia/phase-log.md
git commit -m "materia: comparator arborum portatus - oraculum secundum quod octeti videre non possunt"
```

Tabularium: `gerere 01M1EZKKP0 status impletum`, nota naming what the
mode split turned out to be. Phase-log RELATIO entry.

---

## Task B2: Qualified rules, blocks, and content-mode preludes

**Files:**
- Modify: `css/fontes/css_arbor.c` (+ `css_arbor.h` if the parser struct
  moves there — it should not; keep `CssParsura` private)
- Modify: `css/probationes/probatio_css_arbor.c`

**Interfaces:**
- Consumes: `_significans_proximum` (css_arbor.c:37), `CssLigator`
  (css_adaptare.h:92 — `cumulare`/`solvere`), `materia_nodus_creare/
  ponere/appendere`, `materia_valor_token/lista_nova/nodus`,
  slot enums `CSS_REGULA_PRAELUDIUM/CORPUS`, `CSS_SAEPTUM_TOK_APERTUM/
  CONTENTUM/TOK_CLAUSUM` (css_registrum.h).
- Produces: `css_arbor_parsare` now fills `CSS_PLAGULA_REGULAE`; the
  content-mode mechanism B3/B9 reuse.

**THE CONTENT-MODE DECREE (D7, placed correctly from birth).** Inside a
prelude — and in B3, inside a declaration value — **whitespace AND
comments are content**: every `munus SPATIUM` or `munus COMMENTUM` token
becomes a `lexema-servatum` leaf, and the ligator is bypassed entirely.
Zero trivia exist inside content regions. Three reasons, each fatal to
the alternatives:

1. *Byte order.* Trivia emit adjacent to their owner. A comment between
   two content whitespace leaves (`1px /* c */ 2px`) cannot bind to a
   non-adjacent token without reordering bytes on emission.
2. *The T9 constraint never fires.* `materia_arbor.c:945` raw-marks a
   lexeme value only when text is the element's SOLE child — a
   whitespace lexeme carrying trivia is unprojectable. With no trivia in
   content regions, no content lexeme ever carries trivia.
3. *It is CSS's own object model.* L3 preserves the token stream inside
   component values; comments are not tokens there precisely because the
   spec discards them — we keep them, as content, losslessly.

This strengthens spec §6 D7 ("whitespace is content") to "whitespace and
comments are content"; record the amendment in B10.

Mechanism: `CssParsura` gains `b32 contentum`. In `_significans_proximum`,
when `contentum` is set, trivia tokens are NOT cumulated — they are
returned as significants (the caller wraps every component token in
`lexema-servatum`). `ligator.prior` is untouched by content regions, so
structural binding resumes correctly at the region boundary.

**Scope note:** B2 block interiors are GENERIC component values ("consume
a component value": `functio` for ident+`(`, `saeptum` for `(`/`[`/`{`,
`lexema-servatum` otherwise; EOF closes every open block — total, never
fails). Declarations arrive in B3 and will re-shape qualified-rule block
interiors — so B2 asserts byte coverage and rule/prelude counts, NOT
block-interior tree shape.

- [x] **Step 1: Flip the red pins and write the failing tests** — the
  incompleteness assertions at probatio_css_arbor.c:265-266 flip
  `CREDO_FALSUM` → `CREDO_VERUM` (this is the moment T9 built them for).
  Add, using the EXISTING `_octetos_probare` helper (css_arbor.c gate,
  probatio line 79 — do not write a new one):

```c
    imprimere("\n--- Probans circuitum octetorum, regulae ---\n");
    CREDO_VERUM (_octetos_probare(piscina, "a{color:red}", XII));
    CREDO_VERUM (_octetos_probare(piscina,
        "a , b > c { color : red ; }", XXVII));
    CREDO_VERUM (_octetos_probare(piscina, "\n/* c */\na{}\n", XIII));

    imprimere("\n--- Probans praeludium CONTENTUM (D7) ---\n");
    {
        /* 'a b{}' - praeludium III elementa fert: a, SPATIUM, b.
         * Spatium NODUS est (lexema-servatum), non trivium. */
        MateriaNodus* plagula = css_arbor_parsare(piscina, "a b{}", V);
        MateriaNodus* regula;
        MateriaValor  regulae;
        MateriaValor  praeludium;

        CREDO_NON_NIHIL (plagula);
        regulae = plagula->loci[CSS_PLAGULA_REGULAE];
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(regulae), I);
        regula = materia_valor_lista_obtinere(regulae, ZEPHYRUM)->datum.nodus;
        praeludium = regula->loci[CSS_REGULA_PRAELUDIUM];
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(praeludium), III);
    }
```

- [x] **Step 2: Run and verify the right failures**

Run: `./css/compile_probationes.sh arbor`
Expected: FAIL — the flipped pins and new asserts red; nothing else.

- [x] **Step 3: Implement** — "consume a qualified rule": component
  values into `praeludium` (content mode ON) until `{`; then `saeptum`
  into `corpus` (tok_apertum / contentum / tok_clausum; content mode OFF
  inside the block at this stage — structural trivia between block-level
  component values). Top-level `}` and other L3 recovery arrive in B4;
  until then unparseable top-level tokens still fall out (coverage for
  those inputs stays red-capable — do not sweep into regula-mala yet).

- [x] **Step 4: Run and verify**

Run: `./css/compile_probationes.sh`
Expected: exit 0 — including the T8 coverage gate and both regimes.

- [x] **Step 5: Plant the fault** — drop the `tok_clausum` assignment in
  the block builder. `_octetos_probare` MUST fail with a length mismatch.
  Restore. A gate that stays green with a dropped token is not a gate.

- [x] **Step 6: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: regulae qualificatae + saepta; praeludia CONTENTA a nativitate (D7)"
```

---

## Task B3: Declarations, `!important`, and content-mode values

**Files:**
- Modify: `css/fontes/css_arbor.c`
- Modify: `css/probationes/probatio_css_arbor.c`

**Interfaces:**
- Consumes: B2's content-mode mechanism (`contentum` flag), slot enums
  `CSS_DECL_TOK_NOMEN/TOK_COLON/VALOR/PRAEVALENTIA/TOK_TERMINATOR`,
  `CSS_PRAEVALENTIA_TOK_CLAMOR/TOK_VERBUM`.
- Produces: qualified-rule block interiors become declaration lists
  (`declaratio` nodes in `saeptum.contentum`); `declaratio-mala` for
  recovery inside blocks.

Grammar: ident (`tok_nomen`), structural trivia, `:` (`tok_colon`), then
**content mode** until `;` (`tok_terminator`) or the block's `}`
(terminator slot stays `VALOR_NIHIL` — the writer omits absent slots,
measured at materia_arbor.c:1185). Trailing `!` + ident `important`
(ASCII case-insensitive per L3) peels off into `praevalentia` BEFORE the
value list closes. Missing `:` → `declaratio-mala` consuming to the next
`;`/`}` (spec-defined recovery, not an error node).

- [ ] **Step 1: Write the failing tests**

```c
    imprimere("\n--- Probans spatia UT CONTENTUM in valoribus ---\n");
    {
        /* 'a{margin:1px 2px}' - valor III elementa: 1px, SPATIUM, 2px */
        MateriaNodus* plagula = css_arbor_parsare(piscina,
            "a{margin:1px 2px}", XVII);
        MateriaNodus* regula;
        MateriaNodus* corpus;
        MateriaNodus* decl;
        MateriaValor  valor;

        CREDO_NON_NIHIL (plagula);
        regula = materia_valor_lista_obtinere(
            plagula->loci[CSS_PLAGULA_REGULAE], ZEPHYRUM)->datum.nodus;
        corpus = regula->loci[CSS_REGULA_CORPUS].datum.nodus;
        decl = materia_valor_lista_obtinere(
            corpus->loci[CSS_SAEPTUM_CONTENTUM], ZEPHYRUM)->datum.nodus;
        CREDO_AEQUALIS_S32 (decl->genus, (s32)CSS_GENUS_DECLARATIO);
        valor = decl->loci[CSS_DECL_VALOR];
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(valor), III);
    }

    imprimere("\n--- Probans spatia UT TRIVIA structuraliter ---\n");
    {
        /* Spatium INTER regulas trivium manet - II regulae, non III */
        MateriaNodus* plagula = css_arbor_parsare(piscina, "a{} b{}", VII);
        CREDO_NON_NIHIL (plagula);
        CREDO_AEQUALIS_I32 (materia_valor_lista_numerus(
            plagula->loci[CSS_PLAGULA_REGULAE]), II);
    }

    imprimere("\n--- Probans commentarium UT CONTENTUM in valore ---\n");
    CREDO_VERUM (_octetos_probare(piscina,
        "a{margin:1px /* c */ 2px}", XXV));

    imprimere("\n--- Probans praevalentiam ---\n");
    CREDO_VERUM (_octetos_probare(piscina,
        "a{color:red !important}", XXIII));
    CREDO_VERUM (_octetos_probare(piscina,
        "a{color:red ! important }", XXV));
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh arbor` — Expected: FAIL.

- [ ] **Step 3: Implement** (grammar above; B2's mechanism, no new seams).

- [ ] **Step 4: Run the WHOLE suite** — `./css/compile_probationes.sh`
  Expected: exit 0; the coverage gate and B2's tests must still hold.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: declarationes + praevalentia; valores CONTENTI - commentaria quoque (D7 auctum)"
```

---

## Task B4: At-rules, nesting, and spec-defined recovery

**Files:**
- Modify: `css/fontes/css_arbor.c`
- Modify: `css/probationes/probatio_css_arbor.c`

**Interfaces:**
- Consumes: `CSS_APUD_TOK_NOMEN/PRAELUDIUM/CORPUS/TOK_TERMINATOR`,
  `CSS_MALUM_TOKENS`, `CSS_FUNCTIO_*`, B2/B3 machinery.
- Produces: the L3-total parser — any bytes in, tree out, never fails.

At-rule table (spec §4): rule-bodied (`@media`, `@supports`, `@document`,
`@layer`) · declaration-bodied (`@font-face`, `@page`, `@property`,
`@counter-style`) · statement-only (`@import`, `@charset`, `@namespace`).
Unknown at-rule → generic `saeptum` of component values (total, honest;
growth = a table row, never a re-parse). Recovery: top-level `}` and
prelude-less garbage → `regula-mala`; bad declaration → `declaratio-mala`
(already in B3). EOF inside anything closes it.

- [ ] **Step 1: Write the failing tests**

```c
    imprimere("\n--- Probans regulas apud ---\n");
    CREDO_VERUM (_octetos_probare(piscina, "@import url(a.css);", XIX));
    CREDO_VERUM (_octetos_probare(piscina,
        "@media screen{a{color:red}}", XXVII));   /* NB: consilium vetus
        XXVI dicebat - FALSUM erat; numeratum 2026-09-01 */
    CREDO_VERUM (_octetos_probare(piscina, "@ignotum (x) { y z }", XX));

    imprimere("\n--- Probans functiones et saepta nidificata ---\n");
    CREDO_VERUM (_octetos_probare(piscina, "a{color:rgba(1,2,3,.4)}", XXIII));
    CREDO_VERUM (_octetos_probare(piscina, "a{w:calc((1px + 2px) * 3)}", XXVI));

    imprimere("\n--- Probans recuperationem spec-definitam ---\n");
    CREDO_VERUM (_octetos_probare(piscina, "}a{}", IV));
    CREDO_VERUM (_octetos_probare(piscina, "a{;;color:red}", XIV));
    CREDO_VERUM (_octetos_probare(piscina, "a{color}", VIII));
    CREDO_VERUM (_octetos_probare(piscina, "a{", II));
    CREDO_VERUM (_octetos_probare(piscina, "a{b:\"unterminated", XVIII));
```

- [ ] **Step 2: Run and verify it fails** — `./css/compile_probationes.sh arbor`

- [ ] **Step 3: Implement** (table + recovery above).

- [ ] **Step 4: Run the whole suite** — exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: regulae apud + recuperatio spec-definita; L3 totalis"
```

---

## Task B5: The corpus gate

**Files:**
- Create: `css/probationes/probatio_css_corpus.c`
- Create: `probationes/fixa/css/adversarius_2.css`

Unchanged in substance from old T13 — it was already type-agnostic.

- [ ] **Step 1: Write the failing corpus gate**

```c
hic_manens constans character* CORPUS[] = {
    "probationes/fixa/css/componentia_2026-08-18.css",
    "probationes/fixa/css/thema_2026-08-18.css",
    "probationes/fixa/css/adversarius.css",
    "probationes/fixa/css/adversarius_2.css",
    "lib/speculum_assets/speculum.css",
    "knotapel/atlas/web/style.css"
};
```

Every file through `_octetos_probare`-style parse+emit+memcmp (lift the
helper into the corpus probatio or re-declare it there — it is four
lines). Then the self-measurement, which is the part that matters:

```c
    /* Porta tegumentum SUUM metiatur: 'VI/VI munda' mentitur si
     * nulla plagula umquam aperta est. */
    CREDO_AEQUALIS_I32 (plagulae_lectae, VI);
    CREDO_MAIOR_I32 (octeti_probati, MMMM);
```

- [ ] **Step 2: Write `adversarius_2.css`** — what adversarius.css lacks:
  `}` at top level, `@` at EOF, deeply nested blocks, CDO/CDC
  (`<!--`/`-->`), mixed `\r\n` and `\n`, non-ASCII identifiers, a bare
  `!`, `!important` with a comment inside it. NUL rides inline in the
  probatio, never as a fixture.

- [ ] **Step 3: Run and read failure #1 first** — it may be upstream of
  what you think you are testing. Judge by the CAUSE TALLY, not the
  total.

- [ ] **Step 4: Fix causes until green, one cause per commit.**

- [ ] **Step 5: Commit**

```bash
git add css/probationes/probatio_css_corpus.c probationes/fixa/css/adversarius_2.css
git commit -m "css: porta corporis - VI plagulae reales octetim exactae, tegumentum se metiens"
```

---

## Task B6: STML projection round trip — two cycles, two oracles

**Files:**
- Create: `css/probationes/probatio_css_stml.c`

**Interfaces:**
- Consumes (exact, verified against headers 2026-09-01):

```c
MateriaLexiudicium-free setup:
    MateriaLexiconRatum   ratum;
    MateriaLexIudicium    iudicium;
    MateriaArborConsilium consilium;
    MateriaArborVitium    vitium;

    materia_lexicon_ratum_facere(&ratum, &CSS_LEXICON, &iudicium);
    materia_arbor_consilium_nudum(&consilium, &CSS_REGISTRUM, &ratum, "css");

MateriaArborScriptura materia_arbor_scribere_nodum(
    Piscina*, constans MateriaNodus*, constans MateriaArborConsilium*);
MateriaNodus* materia_arbor_legere(
    Piscina*, InternamentumChorda*, chorda textus,
    constans MateriaArborConsilium*, MateriaArborVitium*);
b32 materia_arbor_aequalis(...);              /* B1 */
```

**What the comparator uniquely guards for CSS:** divisio drift. A comment
migrating from `prior.spatia_post` to `sequens.spatia_ante` preserves the
byte stream exactly — only the tree sees it. This gate is why B1 ran
first.

**FIDELITAS is not runnable for CSS, and the reason is a capability
fact:** the reader derives linea/columna from a cursor that only
`munus LINEA` advances, and CSS has no LINEA genus — parse-side tokens
carry real coordinates, read-back tokens cannot. Positions are derivable
from bytes, and the document never carries stale offsets (positions law),
so this is accepted, and STRUCTURALIS is the asserted mode. Say so in a
comment; do not "fix" it by hand-carrying coordinates.

- [ ] **Step 1: Write the failing gate — two cycles + comparator**

```c
    scriptura_a = materia_arbor_scribere_nodum(piscina, radix, &consilium);
    CREDO_VERUM (scriptura_a.successus);

    relecta_a = materia_arbor_legere(piscina, intern,
        scriptura_a.textus, &consilium, &vitium);
    CREDO_NON_NIHIL (relecta_a);

    scriptura_b = materia_arbor_scribere_nodum(piscina, relecta_a, &consilium);
    CREDO_VERUM (scriptura_b.successus);

    /* DUO circuitus: vitium quod se componit uno invisibile est */
    CREDO_CHORDAE_AEQUALES (scriptura_a.textus, scriptura_b.textus);

    /* Oraculum arboris (B1): dislocatio dominii octetim invisibilis */
    CREDO_VERUM (materia_arbor_aequalis(radix, relecta_a,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));

    /* Compressio MORTUA pro CSS (spec par. III): praesentia, non pinnae */
    CREDO_AEQUALIS_I32 (scriptura_a.census.spatia_vocationes, ZEPHYRUM);
    CREDO_VERUM (scriptura_a.textus.mensura > ZEPHYRUM);
```

- [ ] **Step 2: Run and verify it fails** — `./css/compile_probationes.sh stml`

- [ ] **Step 3: Fix what the refusals name** — writer refusals carry
  causes; fix the cause, never the assertion.

- [ ] **Step 4: Extend over the whole B5 corpus**, beside the
  direct-emission gate — when one fails and the other passes, you know
  which half broke (the separating oracle earning its place). Self-
  measure: documents round-tripped == corpus size.

- [ ] **Step 5: Run the whole suite** — exit 0.

- [ ] **Step 6: Commit**

```bash
git add css/probationes/probatio_css_stml.c
git commit -m "css: circuitus STML - duo cycli, comparator structuralis, census praesentia"
```

---

## Task B7: `css.canon` — hand-written, drift-guarded

**Files:**
- Create: `css/grammatica/css.canon`
- Create: `css/probationes/probatio_css_canon.c`

**Decided (Fran, 2026-09-01):** hand-written — the generated path died
with the hand-written registry (no css.stml grammar exists). The drift
guard replaces the generator's freshness gate.

**Interfaces (verified against include/canon.h):**

```c
Canon* canon_legere (chorda fons, Piscina*, InternamentumChorda*, chorda* causa);
Xar*   canon_iudicare (Canon*, StmlNodus* radix, Piscina*);
/* radix = STML document root: stml_legere the projected text first.
 * Xar VACUUM = sanum; NIHIL redditum = fractura, non sanitas. */
```

Model the file on `silva/c89.canon`'s MANU SCRIPTA section (envelope,
lexemata, trivia forms) — read its header comment first; its DERIVATA
half does not apply (no productions). Rules: the `<arbor>` envelope with
`grammatica="css"` + `registrum-sigillum`; one rule per CSS genus (10
now, 21 after B8) constraining slots at SPECIES level; `lex-*` elements
per the CSS lexicon's tag mangling. **State in the file header that slot
constraints are species-level and why** (no productions to close over —
G3), so a later reader does not mistake the weakness for an oversight.

- [ ] **Step 1: Write the failing gate**

```c
    vitia = canon_iudicare(canon, stml_radix, piscina);
    CREDO_NON_NIHIL (vitia);
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);
    documenta_iudicata = documenta_iudicata + I;
    ...
    CREDO_MAIOR_I32 (documenta_iudicata, ZEPHYRUM);   /* se metiens */
```

Every document the B6 corpus loop produces gets judged.

- [ ] **Step 2: Run and verify it fails** — no canon exists yet.

- [ ] **Step 3: Write the canon** (shape above).

- [ ] **Step 4: The drift guard** — in the probatio: walk the canon's
  genus rules and assert (a) every `CssGenus` title has exactly one
  rule, count == `CSS_GENUS_NUMERUS`; (b) the canon's pinned
  `registrum-sigillum` equals `materia_arbor_sigillum(piscina,
  &CSS_REGISTRUM)` computed live. A hand-written canon's failure is
  silent absence; this makes absence loud. B8's new genera will
  fail (a) until the canon learns them — that is the design.

- [ ] **Step 5: Plant the fault** — delete one genus rule; the guard and
  the judgment must BOTH fire (a document using that genus is now
  unjudgeable-clean). Restore.

- [ ] **Step 6: Run whole suite + natura** —
  `./css/compile_probationes.sh && ./tools/natura_struere.sh`

- [ ] **Step 7: Commit**

```bash
git add css/grammatica/css.canon css/probationes/probatio_css_canon.c
git commit -m "css: canon manu scriptum - custos derivae pro porta recentiae generatoris"
```

---

## Task B8: Selector genera — the registry extension

**Files:**
- Modify: `css/fontes/css_registrum.h` (+ enum genera, + loci enums)
- Modify: `css/fontes/css_registrum.c` (table rows)
- Modify: `css/probationes/probatio_css_registrum.c` (drift guards extend)
- Modify: `css/grammatica/css.canon` (+ 11 rules; sigillum re-pin)

**The 11 genera, slots verbatim from spec §5:**

```
selector-lista        selectores:lista-nodus
selector-complexus    sinister:nodus combinator:token dexter:nodus
selector-compositus   partes:lista-nodus
selector-typi         tok:token
selector-universalis  tok:token
selector-classis      tok_punctum:token tok_nomen:token
selector-identitatis  tok:token
selector-attributi    tok_apertum:token tok_nomen:token tok_operator:token
                      valor:token tok_clausum:token
pseudo-classis        tok_colon:token tok_nomen:token argumenta:lista-nodus
pseudo-elementum      tok_colon:token tok_nomen:token
selector-malus        tokens:lista-token
```

Loci enums follow the house pattern (`CSS_COMPLEXUS_SINISTER = 0,
CSS_COMPLEXUS_COMBINATOR, CSS_COMPLEXUS_DEXTER`, …). Append AFTER the
existing 10 genera, before `CSS_GENUS_NUMERUS` — reordering would move
every registry index silently.

- [ ] **Step 1: Extend the drift guards FIRST and watch them fail** —
  the existing probatio asserts loci_offset contiguity + all-loci titles
  + count == numerus; extend the expected tables to the new 21/new slot
  count, run, verify red (the guard sees genera the table lacks). B7's
  canon guard goes red too — both reds are the system working.

- [ ] **Step 2: Add the rows** — table entries + loci enums.

- [ ] **Step 3: Extend css.canon** with the 11 rules; re-pin the
  sigillum (it covers the registry, which just changed).

- [ ] **Step 4: Run whole suite** — exit 0 including B7's canon gate.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_registrum.h css/fontes/css_registrum.c \
        css/probationes/probatio_css_registrum.c css/grammatica/css.canon
git commit -m "css: genera selectorum XI - registrum, loci nominati, canon; custodes primum rubuerunt"
```

---

## Task B9: `css_selector` — the selector subtree

**Files:**
- Create: `css/fontes/css_selector.{h,c}`
- Create: `css/probationes/probatio_css_selector.c`

**Interfaces:**
- Produces:

```c
/* Praeludium (MATERIA_VALOR_LISTA valorum componentium) in arborem
 * selectorum vertere. TOTALIS: quod parsari nequit 'selector-malus'
 * fit, numquam fractura aut NIHIL. */
MateriaNodus*
css_selector_parsare (
        Piscina* piscina,
   MateriaValor praeludium);
```

**Two structural facts, decided here:**

1. *The selector tree is an ANALYSIS view, not an emission view* (spec
   D3: css_arbor stops at L3; analysis on demand). It SHARES tokens with
   the stylesheet tree — the dual-ownership the comparator deliberately
   does not see (silva_arbor.h:378-385). Therefore NO byte-coverage gate
   on selector trees, by design; whitespace adjacent to punctuation
   combinators is simply not referenced by the selector tree. Say so in
   the header so nobody adds the gate later and panics.
2. *The combinator walks servatum leaves, not trivia.* B2 made prelude
   whitespace `lexema-servatum` nodes. The old adjacency rule survives
   over them: a combinator is `>` `+` `~`, or **a whitespace servatum
   not adjacent to one of those**. In `a > b` the whitespace leaves
   flank `>` and are skipped; in `a b` the whitespace leaf IS the
   combinator and fills `CSS_COMPLEXUS_COMBINATOR`. This is why
   `a>b` and `a > b` produce IDENTICAL selector trees — the skipped
   leaves never enter it — and the equality assertion needs no trivia
   subtleties at all.

- [ ] **Step 1: Write the failing test — the distinction the module
  exists for**

```c
    imprimere("\n--- Probans spatium UT COMBINATOREM ---\n");
    {
        MateriaNodus* duo  = _selectorem_parsare(piscina, "a b");
        MateriaNodus* unus = _selectorem_parsare(piscina, "ab");

        CREDO_AEQUALIS_S32 (duo->genus,  (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        CREDO_AEQUALIS_S32 (unus->genus, (s32)CSS_GENUS_SELECTOR_COMPOSITUS);

        {
            MateriaToken* comb =
                duo->loci[CSS_COMPLEXUS_COMBINATOR].datum.token;
            CREDO_NON_NIHIL (comb);
            CREDO_AEQUALIS_S32 (comb->genus, (s32)CSS_LEX_SPATIA);
        }
    }

    imprimere("\n--- Probans 'a>b' et 'a > b' AEQUALES ---\n");
    {
        MateriaNodus* strictus = _selectorem_parsare(piscina, "a>b");
        MateriaNodus* laxus    = _selectorem_parsare(piscina, "a > b");
        MateriaArborDifferentia d;
        CREDO_VERUM (materia_arbor_aequalis(strictus, laxus,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
    }
```

  (`_selectorem_parsare` = test helper: `css_arbor_parsare` the wrapped
  source `"<sel>{}"`, walk to the prelude, call `css_selector_parsare`.)

- [ ] **Step 2: Run and verify it fails** — undefined symbol.

- [ ] **Step 3: Implement** — split on top-level commas →
  `selector-lista`; within each, compounds separated by combinators
  (adjacency rule above); compound parts: type/universal/class/id/
  attribute/pseudo per spec §5; unparseable → `selector-malus`.

- [ ] **Step 4: Run whole suite** — exit 0; corpus and STML gates
  untouched (selector trees are on-demand, nothing upstream changed).

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_selector.h css/fontes/css_selector.c \
        css/probationes/probatio_css_selector.c
git commit -m "css: arbor selectorum - spatium servatum ut combinator; 'a b' != 'ab' structuraliter"
```

---

## Task B10: Wire-up, spec closure, debrief

**Files:**
- Modify: root orchestration (locate with
  `grep -rl "silva/compile_probationes" --include="*.sh" .` and mirror
  the registration for `css/` and `materia/`)
- Create: `css/fontes/css_arbor.worklog.md`
- Modify: `project-specs/css-arbor-spec.md`, `project-specs/materia-spec.md`
- Memory: `css` + `materia` entries

- [ ] **Step 1: Register the runners unattended** — verify they actually
  RUN after registration (a registered-but-unreached suite is the
  silent-gate failure; the shim was invisibly broken for a day for
  exactly this reason — consider registering `shim_probare.sh` too).

- [ ] **Step 2: Spec amendments, each one paragraph:**
  - css-arbor-spec §6 D7: whitespace AND comments are content inside
    preludes/values (B2/B3 rationale, three reasons).
  - css-arbor-spec §8: canon is HAND-WRITTEN (decree 2026-09-01), drift
    guard replaces freshness gate.
  - materia-spec §10: "as-built" note — phase 1 gate met in substitute
    form (named), MateriaContextus dissolved into consilium + frons
    hooks (10 hooks), comparator ported (B1); pointer to
    materia/phase-log.md as the as-built record.

- [ ] **Step 3: Worklog** — what the corpus gate found that unit tests
  missed (expect non-empty); where the seam differed from prediction;
  every spec §12 prediction that turned out wrong, stated as wrong.

- [ ] **Step 4: Ledger** — instrument debrief (adhibita / fructus /
  asperitates / desiderata) in the RELATIO; parcum `01M12BD0` (S6) gets
  the second-client seam evidence; phase-log RELATIO for phase III.

- [ ] **Step 5: Memory** — css/CLAUDE.md praesens status; materia-fork.md
  and MEMORY.md lines.

- [ ] **Step 6: Commit**

```bash
git add <explicit paths — never git add -A>
git commit -m "css: css_arbor navatum - relatio, spec ut-aedificatum, acta clausa"
```

---

## Self-Review

**Spec coverage.** §4 vocabulary → B2-B4 (structure), B8 (selectors).
§5 → B8+B9. §6(a) combinator → B9; §6(b) D7 → B2/B3 (amended: comments
too). §7 document → B6 (cauda landed in T9). §8 canon → B7 (hand-written
per decree; G3 weakness stated in-file). §9 gates: 2+4 → existing
`_octetos_probare` (T8/T10 heritage), 3+8 → B6, 5 → B7, 6 → B5, 7 → the
T8 gate (already standing, both regimes), 9 → every task's plant step.
§10 out-of-scope respected (no property grammars, no cascade, no
matching). §11: G3 → B7 header note; G4 → dead (retarget); G1/G2 → never
incurred (materia).

**Placeholder scan.** No TBDs. B5 step 3-4 prescribe a process (fix
causes one commit each) rather than content — that is the nature of a
corpus gate; the corpus is enumerated and the helper named. B10 step 1
contains its own discovery grep rather than a hardcoded path — the
registration point genuinely varies and the grep is the reliable
enumerator.

**Type consistency.** `materia_arbor_aequalis` + `MateriaArborDifferentia`
+ `MATERIA_ARBOR_COMPARATIO_STRUCTURALIS` defined in B1, consumed in B6
and B9 with identical signatures. `CSS_COMPLEXUS_COMBINATOR` defined in
B8, consumed in B9. Content-mode mechanism defined in B2, consumed in B3
and (via servatum leaves) B9. All materia signatures transcribed from
headers read 2026-09-01, not from memory: `materia_arbor_consilium_nudum`
4 args (materia_arbor.h:292), `materia_arbor_legere` 5 args (:372),
`materia_arbor_scribere_nodum` 3 args (:299), `canon_legere`/
`canon_iudicare` (canon.h:247/278), `MateriaToken` fields
(materia_token.h:83).

**Corrections to the old plan carried, not silently:** `@media
screen{a{color:red}}` is XXVII bytes, not the old T12's XXVI (recounted
2026-09-01); `spatia_ut_contentum` never appears (the T8 decree);
`silva_*` types nowhere; T10's "create the coverage gate" absorbed as
"reuse `_octetos_probare`".

**One honest gap.** B1's STRUCTURALIS/FIDELITAS per-mode split is
transcribed from silva's implementation during the port rather than
specified here — the header documents WHAT is compared but not per-mode,
and guessing would plant a divergence between the port and its oracle.
The port step says to measure it; that is the only task whose exact
assertions cannot be written until the source is open.
