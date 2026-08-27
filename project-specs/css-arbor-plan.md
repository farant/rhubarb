# css_arbor Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Give CSS a parse tree that projects to canonical STML and emits back byte-exact, by making silva's tree layer language-neutral rather than writing a second one.

**Architecture:** `css_lexema` (exists, root tier) feeds a hand-written recursive-descent parser implementing CSS Syntax L3's algorithms literally. That parser builds `SilvaNodus` trees against a CSS registry. Silva's existing STML writer, reader, comparator, byte emitter, and query engine then serve CSS unchanged — reached through a single new **lexicon seam** that replaces silva_arbor's hardcoded calls into the C89 token vocabulary.

**Tech Stack:** C89 (`-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512`), latina.h macros, credo test library, piscina arenas, chorda strings, Xar dynamic arrays.

**Spec:** `project-specs/css-arbor-spec.md`

> ## ⚠ RETARGETED — read `materia-spec.md` §6 before executing
>
> This plan was written against **silva**. The decision above it changed
> (decree `01M12FJR`): the substrate is being **forked out of silva as
> `materia`**, and CSS is materia's first client, not silva's.
>
> **Do not execute this plan as written.** What changed:
>
> | task | status |
> |---|---|
> | **T1** (G4, vestigial production) | **RETIRED** — hand-written frontends never invoke the LR generator (materia M6) |
> | **T4** (lexicon seam) | **RELOCATED** to materia phase 1. Note its `genus_identificator` correction still applies |
> | **T5** (widen genus to `s32`) | **RELOCATED** to materia phase 1 |
> | **T6** (amalgam export) | **DELETED** — CSS links materia directly |
> | **T2, T3, T7–T17** | **survive**, retargeted from `silva.h` to `materia.h` |
>
> Also superseded: spec D4's placement (`css/` consumes `materia.h`, not
> `silva.h`) and G1's layering inversion, which is now never incurred.
> Roughly 13 of 17 tasks stand.

## Global Constraints

- **Latin throughout** — identifiers, function names, comments, messages. Untranslated technical terms permitted: `offset`, `css`, `stml`, `token` (per house precedent in `silva_token.h`).
- **latina.h words are FORBIDDEN identifiers.** Never `nomen`, `registrum`, `magnitudo`, `structura`, `casus`, `brevis`, `exire`. Use `titulus` where you want `nomen`. `_Capital` is implementation-reserved.
- **`chorda` is NOT null-terminated.** Never pass `chorda.data` to a `str*` function.
- **`i32` and `i64` are UNSIGNED**; `s32`/`s64` are signed. Any subtraction-derived value or loop bound needs `s32`/`s64` or an explicit guard.
- **`??-` in a string literal is a TRIGRAPH** under `-Werror`. Never write one.
- **A .c file using POSIX includes `postulata_posix.h` FIRST** (Codex 85). No CSS file should need it.
- **Positions are byte offsets** plus 1-based line/column (decretum `01M0ATF1E1`).
- **Exit code 2 from any test runner means NOTHING RAN** (bad filter), 1 means failures. Never gate on exit 0 alone.
- After any edit to STML files: `./tools/natura_struere.sh`.

## Task Legend

- **[SPIKE]** — answers a question; may change later tasks.
- **[SEAM]** — touches the silva substrate. **Re-check these against `project-specs/html-arbor-spec.md` before executing**, per the S6 evaluation exercise (parcum `01M12BD0`). Tasks not marked [SEAM] are unaffected by what HTML surfaces.

---

## File Structure

| file | responsibility |
|---|---|
| `silva/fontes/silva_lexicon_vocabularii.h` | the `SilvaLexicon` contract: the six questions silva_arbor asks a token vocabulary |
| `silva/fontes/silva_arbor.c` (modify) | replace hardcoded C89 vocabulary calls with lexicon calls |
| `silva/fontes/silva_token.h` (modify) | `SilvaToken.genus` widens to `s32` |
| `silva/fontes/silva_tabulae.h` (modify) | `SilvaTabSymbolum.genus_lexematis` widens to `s32` |
| `silva/amalgama/silva.h` (modify) | export the node **builder** API (G2) |
| `css/compile_probationes.sh` | subsystem test runner (tessera template) |
| `css/grammatica/css.stml` | node vocabulary, `<genera-extra>` only |
| `css/fontes/css_registrum.{h,c}` | generated (or hand-written, per T1) genera/loci tables |
| `css/fontes/css_lexicon.{h,c}` | the CSS `SilvaLexicon` instance |
| `css/fontes/css_adaptare.{h,c}` | `CssLexema` → `SilvaToken*`, trivia attachment |
| `css/fontes/css_arbor.{h,c}` | CSS Syntax L3 recursive descent |
| `css/fontes/css_selector.{h,c}` | prelude component-values → selector subtree |
| `css/probationes/probatio_css_*.c` | one per module, plus the corpus gates |

Note the split: `css_lexicon` is separate from `css_registrum` because they answer different questions — the registry describes **nodes**, the lexicon describes **tokens**. Silva keeps them separate for the same reason (`silva_tabulae.h` vs `silva_token.h`), and merging them would couple the parser's vocabulary to the tree's.

---

## Task 1: G4 — RESOLVED 2026-08-27, before the plan was executed

**Verdict: path (b) — one vestigial production, plus five REQUIRED structural genera.**
Probe run against `silva/generare.sh`; artifacts discarded. Ledger: quaestio `01M12BDHHQBKEKZEXFBRANKQ5F`, closed.

**What the generator refuses, in order:**

1. No `<initium>` → `silva_gen: <initium> non inventum`
2. `<initium>` naming a `genera-extra` genus → `initium symbolum 'plagula' non inventum`. **`genera-extra` genera are not grammar symbols**, so `<initium>` must name a real nonterminal — which means at least one production must exist.
3. Productions present, structural genera absent → `genus structurale 'ambiguus' in genera-extra REQUIRITUR`, then `'conditionalis'`, one at a time.

**What works** (verified end-to-end, `symbola: 3, productiones: 2, status: 4, conflictus servati: 0`):

```xml
<grammatica>
  <terminalia>
    <terminalis titulus="EOF" genus="SILVA_LEX_EOF"/>
  </terminalia>
  <regulae>
    <!-- VESTIGIALIS: tabulae LR vacuae esse nequeunt, et <initium>
         symbolum VERUM nominare debet. Haec productio ad parsandum
         NUMQUAM adhibetur - CSS descensu recursivo parsatur (D1).
         Vocabularium verum totum in genera-extra vivit. -->
    <regula titulus="radix">
      <productio genus="plagula" id="plagula-vestigialis">EOF@cauda</productio>
    </regula>
  </regulae>
  <genera-extra>
    <!-- QUINQUE structuralia a generatore REQUISITA
         (silva_generare.c:1315, spec-v2 par. 12.2) -->
    <genus titulus="error"         slots="tokens:lista-token"/>
    <genus titulus="ambiguus"      slots="interpretationes:lista-nodus canonica:index"/>
    <genus titulus="conditionalis" slots="rami:lista-nodus finis:lista-token"/>
    <genus titulus="ramus-sumptus" slots="directiva:lista-token contentum:lista-nodus conditio_id:index"/>
    <genus titulus="ramus-omissus" slots="directiva:lista-token cruda:lista-token conditio_id:index"/>
    <!-- ... vocabularium CSS verum sequitur ... -->
  </genera-extra>
  <initium>radix</initium>
</grammatica>
```

**Consequences, all of which correct the plan as first written:**

- **T3 keeps its generated path.** No hand-written `css_registrum.c`, no hand-written canon. G3's species-level limitation still stands (nothing closes over a vestigial production), but T15 stays generated.
- **The generator EMITS the genus enum.** The plan originally said to hand-add it. It emits `<PRAEFIXUM>_GENUS_<TITULUS>` constants — so prefix `CSS` yields `CSS_GENUS_PLAGULA` exactly as wanted, for free.
- **There is NO typedef and NO count constant.** The emitted enum is anonymous (`silva_tabulae_c89.h:21-81`). So `CssGenus` as a *type* does not exist — declare genus variables `s32` — and `CSS_GENUS_NUMERUS` does not exist — use `CSS_REGISTRUM.numerus_generum`. **T3's test is corrected accordingly below.**
- **CSS carries three genera it can never produce.** `conditionalis`, `ramus-sumptus`, `ramus-omissus` are C-preprocessor concepts. Harmless (dead rows in a table), but catalogue it — see the census note below.

### The finding that outranks the answer

**My coupling census had a blind spot, and this probe found it.**

The census in spec §1 scored `silva_generare` at **2** C-specific references and called it neutral. It measured **lexical** coupling — greps for `SILVA_LEX_*` and `C89` — and therefore could not see `silva_generare.c:1315`, where five **structural** genera are required by name, three of them modelling the C preprocessor. A CSS grammar must declare `conditionalis` and `ramus-omissus` to be accepted at all.

This is small in cost and large in method:

- **Lexical neutrality is not structural neutrality.** Any future census must grep for required *node genera* as well as token genera, or it will keep reporting neutral modules that aren't.
- **It vindicates writing the HTML spec before implementing.** A paper exercise found a real coupling site that a measurement missed, at the price of one probe. That is precisely the S6 evaluation working — and it means the spec's "~11 coupling sites" figure is a **floor**, not a count.
- It is the first concrete entry in the S6 evidence file (parcum `01M12BD0`).

**No steps remain in this task.** Proceed to Task 2.

---

## Task 2: The `css/` subsystem skeleton

**Files:**
- Create: `css/compile_probationes.sh`, `css/fontes/.keep`, `css/probationes/probatio_css_skeleton.c`
- Read only: `tessera/compile_probationes.sh` (the template — copy its structure exactly)

**Interfaces:**
- Produces: `./css/compile_probationes.sh [filtrum]` — runner for every later task. Exit 0 = pass, 1 = failures, **2 = nothing ran**.

- [ ] **Step 1: Write a failing probatio that asserts the runner runs it**

```c
/* probatio_css_skeleton.c - Porta plantata: an cursor probationes CSS inveniat */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina*  piscina;

    piscina = piscina_generare_dynamicum("probatio_css_skeleton", MMMMXCVI);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans cursorem ipsum ---\n");
    /* VITIUM PLANTATUM: hoc cadere DEBET donec gradus III id vertat.
     * Porta tacita porta mortua est. */
    CREDO_VERUM (FALSUM);

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
```

- [ ] **Step 2: Create the runner by adapting tessera's**

Copy `tessera/compile_probationes.sh` to `css/compile_probationes.sh` and change exactly four things: the env guard `TESSERA_PROBATIONES_EFFUSIO` → `CSS_PROBATIONES_EFFUSIO`; the log name `tessera.log` → `css.log`; `TESSERA_DIR` → `CSS_DIR`; and the header comment. Leave the flag array, glob discovery, incremental objects, and substring filter untouched — they are the house contract.

The link list starts as the root objects CSS needs:

```bash
declare -a RADIX_FONTES=(
    "piscina" "chorda" "chorda_aedificator" "xar"
    "tabula_dispersa" "internamentum" "friatio"
    "credo" "css_lexema" "stml" "selectio"
)
```

- [ ] **Step 3: Run it and verify the PLANTED FAULT fails**

Run: `./css/compile_probationes.sh`
Expected: exit **1**, with the skeleton probatio reporting one failure. **Exit 2 means the runner found nothing** — the glob or the directory is wrong, fix that before continuing. This step is the gate's own calibration: a runner that reports success on a file containing `CREDO_VERUM(FALSUM)` is not running it.

- [ ] **Step 4: Turn the planted fault into a real assertion**

Replace `CREDO_VERUM (FALSUM);` with:

```c
    CREDO_NON_NIHIL (piscina);
```

- [ ] **Step 5: Run and verify it passes**

Run: `./css/compile_probationes.sh`
Expected: exit **0**, 1/1.

- [ ] **Step 6: Commit**

```bash
git add css/compile_probationes.sh css/probationes/probatio_css_skeleton.c
git commit -m "css: subsystema conditum - cursor probationum cum vitio plantato"
```

---

## Task 3: The CSS registry

**Files:**
- Modify: `css/grammatica/css.stml` (full vocabulary)
- Create: `css/fontes/css_registrum.{h,c}` (generated by T1's chosen path, or hand-written)
- Create: `css/probationes/probatio_css_registrum.c`

**Interfaces:**
- Produces: `extern constans SilvaRegistrumCoctum CSS_REGISTRUM;` — consumed by every later task.
- Produces: genus index constants, **emitted by the generator** as `CSS_GENUS_<TITULUS>` (T1). The enum is **anonymous and has no count member** — declare genus variables `s32`, and take the count from `CSS_REGISTRUM.numerus_generum`.

The five structural genera must be declared first (T1), so they occupy the low indices. Declaration order in `css.stml` sets enum order; the test below pins it by name so a reordering is caught rather than silently absorbed.

- [ ] **Step 1: Write the failing test — the enum must agree with the tables**

This is the real risk in this task: an enum and a table that drift silently. The test asserts they agree **by name**, not by position, so a reordering is caught.

```c
/* probatio_css_registrum.c */
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "silva_tabulae.h"
#include "css_registrum.h"
#include <stdio.h>

/* Tabula EXPECTATA scripta manu - fons veritatis SECUNDUS.
 * Si generator ordinem mutet, haec porta clamat. */
/* Tabula EXPECTATA scripta manu - fons veritatis SECUNDUS.
 * Ordo = ordo declarationis in css.stml. Quinque structuralia
 * PRIMA sunt (T1: a generatore requisita). */
hic_manens constans character* TITULI_EXPECTATI[] = {
    "error", "ambiguus", "conditionalis", "ramus-sumptus",
    "ramus-omissus",
    "plagula", "regula-qualificata", "regula-apud", "declaratio",
    "praevalentia", "lexema-servatum", "functio", "saeptum",
    "regula-mala", "declaratio-mala", "selector-lista",
    "selector-complexus", "selector-compositus", "selector-typi",
    "selector-universalis", "selector-classis", "selector-identitatis",
    "selector-attributi", "pseudo-classis", "pseudo-elementum",
    "selector-malus"
};
#define TITULI_NUMERUS \
    ((i32)(magnitudo(TITULI_EXPECTATI) / magnitudo(TITULI_EXPECTATI[0])))

s32 principale (vacuum)
{
         b32  praeteritus;
    Piscina*  piscina;
         i32  i;

    piscina = piscina_generare_dynamicum("probatio_css_registrum", MMMMXCVI);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);

    imprimere("\n--- Probans concordiam enumerationis et tabulae ---\n");

    /* Nulla constans numeri a generatore emittitur (T1) - ergo
     * tabula manu scripta numerum SUUM fert et registrum ei
     * congruere debet. */
    CREDO_AEQUALIS_I32 (CSS_REGISTRUM.numerus_generum, TITULI_NUMERUS);

    per (i = ZEPHYRUM; i < TITULI_NUMERUS; i++)
    {
        CREDO_VERUM (chorda_aequalis_literis(
            chorda_ex_literis(CSS_REGISTRUM.genera[i].titulus),
            TITULI_EXPECTATI[i]));
    }

    imprimere("\n--- Probans locos plagulae ---\n");
    {
        constans SilvaTabGenus* g =
            &CSS_REGISTRUM.genera[CSS_GENUS_PLAGULA];
        CREDO_AEQUALIS_I32 (g->loci_numerus, II);
        CREDO_AEQUALIS_S32 (
            CSS_REGISTRUM.loci[g->loci_offset + ZEPHYRUM].species,
            (s32)SILVA_LOCUS_LISTA_NODUS);
        CREDO_AEQUALIS_S32 (
            CSS_REGISTRUM.loci[g->loci_offset + I].species,
            (s32)SILVA_LOCUS_TOKEN);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    si (praeteritus) { redde ZEPHYRUM; } alioquin { redde I; }
}
```

- [ ] **Step 2: Run it and verify it fails to compile**

Run: `./css/compile_probationes.sh registrum`
Expected: FAIL — `css_registrum.h` does not exist.

- [ ] **Step 3: Write the full `css.stml` vocabulary**

Both genus clusters from spec §4 and §5, verbatim. Add the runner's include path for `css/fontes` and add `css_registrum` to its object list.

- [ ] **Step 4: Generate (or hand-write) `css_registrum.{h,c}` per Task 1's verdict**

`./silva/generare.sh css/grammatica/css.stml CSS css_registrum`. The genus enum comes out of the generator (T1); do **not** hand-write one — a second source of truth for genus indices is exactly the drift this task's test exists to catch.

- [ ] **Step 5: Run and verify it passes**

Run: `./css/compile_probationes.sh registrum`
Expected: exit 0.

- [ ] **Step 6: Commit**

```bash
git add css/grammatica/css.stml css/fontes/css_registrum.h css/fontes/css_registrum.c css/probationes/probatio_css_registrum.c css/compile_probationes.sh
git commit -m "css: registrum generum - XXI genera, porta concordiae enumerationis"
```

---

## Task 4: The lexicon contract, with C89 as its only client [SEAM]

This task changes silva while leaving CSS out of it entirely. Silva's gates must stay **exactly** green — that is the whole point of doing it separately.

**Files:**
- Create: `silva/fontes/silva_lexicon_vocabularii.h`
- Modify: `silva/fontes/silva_arbor.c` (the sites enumerated in spec §3)
- Create: `silva/fontes/silva_lexicon_c89.c` (the C89 instance)
- Modify: `silva/fontes/silva_arbor.h` (writer/reader take a lexicon)

**Interfaces:**
- Produces:

```c
/* silva_lexicon_vocabularii.h */
nomen structura {
    s32        genus;       /* genus lexematis triviorum */
    character  repetitum;   /* character quem repetit; ZEPHYRUM si varius */
} SilvaTriviumForma;

nomen structura {
                     i32  numerus_generum;
    constans character* (*titulus)(s32 genus);
    constans character* (*orthographia)(s32 genus);  /* NIHIL = varia */
                     s32  genus_eof;
    constans SilvaTriviumForma* trivia;
                     i32  numerus_triviorum;
} SilvaLexicon;

extern constans SilvaLexicon SILVA_LEXICON_C89;
```

> **CORRECTION from the HTML seam probe (`html-arbor-spec.md` §2.2, HG1).**
> An earlier draft of this struct carried a `genus_identificator`
> member. **It does not belong here.**
> `SILVA_LEX_IDENTIFICATOR` appears at `silva_arbor.c:2851` in exactly
> one context: synthesizing a **macro-definition-site token** while
> reconstructing an origin chain from `<expansio def-f=… def-c=…>`.
> That is preprocessor machinery — nothing but C reaches it, because
> it only runs on origin genera only C produces.
>
> Left in, every frontend would supply a value never read: the
> substrate making frontends answer C's questions, which is the leak
> S6 exists to remove. **Keep `SILVA_LEX_IDENTIFICATOR` as a local
> constant in the origin reader**, not as a lexicon question.
>
> Found on paper, before this task was executed. That is the seam
> probe earning its cost.

- Produces: `silva_arbor_scribere_nodum(..., constans SilvaLexicon* lexicon)` and `silva_arbor_legere(..., constans SilvaLexicon* lexicon)` — **new trailing parameter**. Every existing caller passes `&SILVA_LEXICON_C89`.

- [ ] **Step 1: Write the failing test — the C89 instance must agree with the C89 lexer**

This is silva_arbor's existing "porta obligata" generalized. The header of `silva_arbor_orthographia` already warns that the spelling table is *"fons veritatis SECUNDUS"* that can drift from the lexer. The lexicon makes that risk reusable, so the guard must come with it.

```c
/* probationes/probatio_silva_lexicon.c (in silva/probationes/) */
/* Pro CUIUSQUE generis orthographia fixa: eam lexare, et genus
 * IDEM reddi exspectare. Directiones duae divergere non possunt. */
    per (g = ZEPHYRUM; g < SILVA_LEXICON_C89.numerus_generum; g++)
    {
        constans character* orth = SILVA_LEXICON_C89.orthographia(g);
        Xar* lexemata;
        SilvaToken* primum;

        si (orth == NIHIL) { perge; }

        lexemata = silva_lexare(piscina, orth,
                                (i32)strlen(orth), ZEPHYRUM);
        CREDO_NON_NIHIL (lexemata);
        primum = *(SilvaToken**)xar_obtinere(lexemata, ZEPHYRUM);
        CREDO_NON_NIHIL (primum);
        CREDO_AEQUALIS_S32 (primum->genus, g);
    }
```

Also assert the lexicon is not vacuously passing:

```c
    /* Vacuitas custodiae: si nulla orthographia fixa exstet,
     * ansa supra ZERO vices currit et NIHIL probat. */
    CREDO_MAIOR_S32 (numerus_orthographiarum_probatarum, XX);
```

- [ ] **Step 2: Run and verify it fails**

Run: `./silva/compile_probationes.sh lexicon`
Expected: FAIL — `SILVA_LEXICON_C89` undefined.

- [ ] **Step 3: Implement the contract and the C89 instance**

Move `ORTHOGRAPHIAE[]` out of `silva_arbor.c` into `silva_lexicon_c89.c`. Wrap `silva_lexema_genus_nomen` as the `titulus` function pointer. Populate `trivia` with the four C89 forms:

```c
hic_manens constans SilvaTriviumForma TRIVIA_C89[] = {
    { (s32)SILVA_LEX_SPATIA,      ' '  },
    { (s32)SILVA_LEX_TABULAE,     '\t' },
    { (s32)SILVA_LEX_NOVA_LINEA,  '\n' },
    { (s32)SILVA_LEX_CONTINUATIO, ZEPHYRUM }
};
```

- [ ] **Step 4: Rewrite silva_arbor.c's coupling sites to ask the lexicon**

The sites, from spec §3: `:35` (table — now gone), `:384` (`titulus`), `:352` (`genus_eof`), `:2851` (`genus_identificator`), `:1023-1034`, `:1131`, `:1198`, `:2459-2478`, `:3527` (all trivia), and the eight `SILVA_LEX_NUMERUS_GENERUM` bounds checks (now `lexicon->numerus_generum`).

**Do not write a second trivia-classification helper.** Route every site through one lookup over `lexicon->trivia`. The duplicate-surface law cost the arbor arc four copies of one fix; this is the same shape.

- [ ] **Step 5: Run silva's full suite and verify it is UNCHANGED**

Run: `./silva/compile_probationes.sh`
Expected: exit 0, and the **same counts as before the change** — 281/281 subtrees, 78/78 plain-C, 154/154 latinized. Record the before-counts *before* starting Step 3 so this comparison is real.

Run: `./compile_tests.sh` — root suite must also stay green.

- [ ] **Step 6: Commit**

```bash
git add silva/fontes/silva_lexicon_vocabularii.h silva/fontes/silva_lexicon_c89.c silva/fontes/silva_arbor.c silva/fontes/silva_arbor.h silva/probationes/probatio_silva_lexicon.c
git commit -m "silva: foramen lexici vocabularii - arbor vocabularium per lexicon rogat, C89 cliens unicus"
```

---

## Task 5: Widen the token genus to `s32` [SEAM]

**Files:**
- Modify: `silva/fontes/silva_token.h:241`, `silva/fontes/silva_tabulae.h` (`SilvaTabSymbolum.genus_lexematis`)
- Modify: whatever `-Werror` names

- [ ] **Step 1: Make the two field changes**

```c
/* silva_token.h */
structura SilvaToken {
    s32 genus;   /* genus lexematis; per SilvaLexicon interpretatum */
    /* ... cetera invariata ... */
};
```

```c
/* silva_tabulae.h */
nomen structura {
    constans character* titulus;
    s32                 genus_lexematis;  /* per lexicon interpretatum */
    b32                 est_terminale;
} SilvaTabSymbolum;
```

- [ ] **Step 2: Compile and let the compiler enumerate the work**

Run: `./silva/compile_probationes.sh`
Expected: a wall of `-Wconversion` / enum-conversion errors. **This is the task's specification** — there is no logic to design, only sites to fix. Work the list to zero.

- [ ] **Step 3: Verify the generator still emits compilable tables**

Run: `./silva/generare.sh`
Expected: exit 0, and `git diff --stat silva/fontes/silva_tabulae_*.c` shows **no change** — the generator writes the genus as a verbatim string (`silva_coquere.c:450-465`), so widening the field it lands in must not alter its output. **A diff here means something else moved and needs explaining before you continue.**

- [ ] **Step 4: Run every gate**

Run: `./silva/compile_probationes.sh` — same counts as Task 4 Step 5.
Run: `./compile_tests.sh` — green.
Run: `./silva/haruspex.sh` — 243 TUs (takes ~121s).

- [ ] **Step 5: Commit**

```bash
git add -A silva/
git commit -m "silva: genus lexematis ad s32 latum - vocabularium opacum, per lexicon lectum"
```

---

## Task 6: Export the node builder API from the amalgam (G2) [SEAM]

**Files:**
- Modify: `silva/amalgama/silva.h` (hand-extend), `silva/instrumenta/principalia/hospes.c`
- Run: `./silva/amalgamare.sh`

**Interfaces:**
- Produces, through `silva.h`: `silva_nodus_creare`, `silva_nodus_ponere`, `silva_nodus_appendere`, `silva_valor_{nihil,nodus,token,index,lista,lista_nova,lista_appendere}`, `silva_token_ex_fonte`, and `SilvaLexicon`.

- [ ] **Step 1: Extend hospes FIRST to exercise the surface through the header**

Hospes is the gate that proves the header matches the bodies. Write the exercise before the export exists, so the failure is real:

```c
/* in hospes.c - aedificatio arboris PER CAPUT SOLUM */
{
    SilvaNodus* n = silva_nodus_creare(piscina, ZEPHYRUM, I);
    SilvaValor  lista = silva_valor_lista_nova(piscina);
    si (n == NIHIL) { redde I; }
    si (!silva_nodus_ponere(n, ZEPHYRUM, lista,
                            SILVA_LOCUS_LISTA_NODUS)) { redde I; }
}
```

- [ ] **Step 2: Run amalgamation and verify it fails**

Run: `./silva/amalgamare.sh`
Expected: FAIL — hospes cannot see `silva_nodus_creare`.

- [ ] **Step 3: Follow the amalgam ritual in order**

Order matters and drift in the first step is **silent**:

1. `./silva/amalgamare.sh` regenerates from `fontes_generare`
2. `excludenda_generare` — this one fails **loudly** if wrong
3. Hand-extend `silva/amalgama/silva.h` with the declarations above
4. Add `CADENDA_TYPEDEF` entries for `SilvaLexicon` and `SilvaTriviumForma`
5. Re-run `./silva/amalgamare.sh`

- [ ] **Step 4: Verify**

Run: `./silva/amalgamare.sh` — exit 0, hospes passes.
Run: `./compile_tests.sh` — green (`tools/aedilis.c` and officina consume `silva.h`).

- [ ] **Step 5: Commit**

```bash
git add silva/amalgama/silva.h silva/amalgama/silva.c silva/instrumenta/principalia/hospes.c
git commit -m "silva: amalgama API aedificationis nodorum exportat (G2) - hospes eam per caput exercet"
```

---

## Task 7: `css_lexicon` — the CSS token vocabulary

**Files:**
- Create: `css/fontes/css_lexicon.{h,c}`, `css/probationes/probatio_css_lexicon.c`

**Interfaces:**
- Produces: `extern constans SilvaLexicon CSS_LEXICON;`

- [ ] **Step 1: Write the failing test — the same orthography gate, now for CSS**

```c
/* Pro CUIUSQUE generis CSS cum orthographia fixa: eam lexare,
 * genus idem reddi exspectare. */
    per (g = ZEPHYRUM; g < CSS_LEXICON.numerus_generum; g++)
    {
        constans character* orth = CSS_LEXICON.orthographia(g);
        Xar* lexemata;
        CssLexema* primum;

        si (orth == NIHIL) { perge; }
        probata = probata + I;

        lexemata = css_lexare(piscina, orth, (i32)strlen(orth));
        CREDO_NON_NIHIL (lexemata);
        primum = (CssLexema*)xar_obtinere(lexemata, ZEPHYRUM);
        CREDO_NON_NIHIL (primum);
        CREDO_AEQUALIS_S32 ((s32)primum->genus, g);
    }
    /* Custodia vacuitatis - ansa currisse DEBET */
    CREDO_MAIOR_S32 (probata, X);
```

Plus the CSS-specific fact from spec §3, asserted rather than assumed:

```c
    imprimere("\n--- Probans nulla trivia compressibilia ---\n");
    /* CSS cursum spatiorum TOTUM in lexema unum consumit
     * (css_lexema.c:513-519), ergo orthographia VARIA est et
     * via compressionis numquam accenditur. */
    CREDO_AEQUALIS_I32 (CSS_LEXICON.numerus_triviorum, ZEPHYRUM);
    CREDO_NIHIL ((vacuum*)CSS_LEXICON.orthographia(
        (s32)CSS_LEX_SPATIA));
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh lexicon`
Expected: FAIL — `CSS_LEXICON` undefined.

- [ ] **Step 3: Implement**

`titulus` wraps the existing `css_lexema_genus_nomen` (`lib/css_lexema.c:799+`) — **do not write a second name table**. `orthographia` returns the fixed spelling for punctuation only (`:` `;` `,` `[` `]` `(` `)` `{` `}` `<!--` `-->`) and `NIHIL` for everything else including `CSS_LEX_SPATIA` and `CSS_LEX_COMMENTARIUM`. `genus_eof = CSS_LEX_FINIS`, `trivia = NIHIL`, `numerus_triviorum = 0`. **No `genus_identificator`** — see the correction in T4.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh lexicon`
Expected: exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_lexicon.h css/fontes/css_lexicon.c css/probationes/probatio_css_lexicon.c
git commit -m "css: lexicon vocabularii - porta orthographiae bidirectionalis, trivia compressibilia ZERO"
```

---

## Task 8: `css_adaptare` — CssLexema into SilvaToken

**Files:**
- Create: `css/fontes/css_adaptare.{h,c}`, `css/probationes/probatio_css_adaptare.c`

**Interfaces:**
- Produces:

```c
/* Lexemata CSS in lexemata silvae vertere.
 *
 * TRIVIA: spatia et commentaria in spatia_ante/spatia_post
 * lexematum SIGNIFICANTIUM ponuntur (invariatum domini unius).
 * Regula: trivium lexemati SEQUENTI ut 'ante' adhaeret, NISI
 * eodem in versu post lexema praecedens stet et nullum lexema
 * significans sequatur - tunc 'post' ultimi fit. Trivia caudae
 * ad lexema FINIS eunt.
 *
 * Modus VERBATIM (spatia_ut_contentum = VERUM) trivia NON
 * absorbet: quodque lexema, spatia inclusa, significans manet.
 * Hoc est quod par. 6 pro praeludiis et valoribus petit. */
Xar*
css_adaptare (
    Piscina* piscina,
        Xar* lexemata_css,
         b32 spatia_ut_contentum);
```

- [ ] **Step 1: Write the failing test**

```c
    imprimere("\n--- Probans absorptionem triviorum ---\n");
    {
        constans character* fons = "a { color : red }";
        Xar* css = css_lexare(piscina, fons, (i32)strlen(fons));
        Xar* silvae = css_adaptare(piscina, css, FALSUM);
        SilvaToken* t0;

        CREDO_NON_NIHIL (silvae);
        /* Lexemata significantia: a { color : red } FINIS = VII.
         * Spatia sex absorpta sunt. */
        CREDO_AEQUALIS_I32 (xar_numerus(silvae), VII);

        t0 = *(SilvaToken**)xar_obtinere(silvae, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (t0->genus, (s32)CSS_LEX_IDENTIFICATOR);
        CREDO_AEQUALIS_S32 (t0->byte_offset, ZEPHYRUM);
        CREDO_NON_NIHIL (t0->spatia_post);   /* spatium post 'a' */
    }

    imprimere("\n--- Probans modum verbatim (par. VI) ---\n");
    {
        constans character* fons = "1px 2px";
        Xar* css = css_lexare(piscina, fons, (i32)strlen(fons));
        Xar* silvae = css_adaptare(piscina, css, VERUM);
        /* 1px SPATIUM 2px FINIS = IV; nihil absorptum */
        CREDO_AEQUALIS_I32 (xar_numerus(silvae), IV);
    }

    imprimere("\n--- Probans provenientiam ---\n");
    {
        /* Lexema QUODQUE ex fonte venit, ergo byte_offset >= 0.
         * Comparator provenientiam UTROQUE modo confert; lexema
         * syntheticum hic vitium esset, non optio. */
        per (i = ZEPHYRUM; i < xar_numerus(silvae); i++)
        {
            SilvaToken* t = *(SilvaToken**)xar_obtinere(silvae, i);
            CREDO_MAIOR_AUT_AEQUALIS_S32 (t->byte_offset, ZEPHYRUM);
        }
    }
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh adaptare`
Expected: FAIL — `css_adaptare` undefined.

- [ ] **Step 3: Implement**

Each `CssLexema` becomes a `SilvaToken` via `silva_token_ex_fonte` (`silva_token.h:262`), carrying `genus` (cast to `s32`), `valor`, `byte_offset`, `longitudo`, `linea`, `columna`, `fons_index = 0`. `origo` is FONS for every token. `scissurae` stays `NIHIL`.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh adaptare`
Expected: exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_adaptare.h css/fontes/css_adaptare.c css/probationes/probatio_css_adaptare.c
git commit -m "css: adaptator lexematum - CssLexema -> SilvaToken cum absorptione triviorum bimodali"
```

---

## Task 9: `css_arbor` — the stylesheet spine

**Files:**
- Create: `css/fontes/css_arbor.{h,c}`, `css/probationes/probatio_css_arbor.c`

**Interfaces:**
- Produces:

```c
/* Plagulam CSS parsare. TOTALIS: quaelibet series octetorum
 * arborem reddit; nihil umquam cadit (algorithmus L3 ita
 * definitur). Numquam NIHIL nisi memoria deficit. */
SilvaNodus*
css_arbor_parsare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura);
```

- [ ] **Step 1: Write the failing test — the degenerate cases first**

```c
    imprimere("\n--- Probans plagulam vacuam ---\n");
    {
        SilvaNodus* n = css_arbor_parsare(piscina, "", ZEPHYRUM);
        SilvaValor* regulae;
        CREDO_NON_NIHIL (n);
        CREDO_AEQUALIS_S32 (n->genus, (s32)CSS_GENUS_PLAGULA);
        regulae = &n->loci[ZEPHYRUM];
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(*regulae),
                            ZEPHYRUM);
        /* Cauda ADEST etiam in plagula vacua - lexema FINIS
         * semper exstat (par. VII) */
        CREDO_AEQUALIS_S32 (n->loci[I].genus, (s32)SILVA_VALOR_TOKEN);
    }

    imprimere("\n--- Probans plagulam spatiorum solorum ---\n");
    {
        SilvaNodus* n = css_arbor_parsare(piscina, "\n\n  ", VI);
        CREDO_NON_NIHIL (n);
        CREDO_AEQUALIS_I32 (
            silva_valor_lista_numerus(n->loci[ZEPHYRUM]), ZEPHYRUM);
        /* Spatia in caudam eunt ut 'ante' lexematis FINIS -
         * dominus unus, nullus octetus perditus */
        {
            SilvaToken* cauda = n->loci[I].datum.token;
            CREDO_NON_NIHIL (cauda->spatia_ante);
            CREDO_AEQUALIS_I32 (xar_numerus(cauda->spatia_ante), I);
        }
    }
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh arbor`
Expected: FAIL — `css_arbor_parsare` undefined.

- [ ] **Step 3: Implement the spine only**

`css_arbor_parsare` lexes, adapts (non-verbatim), creates a `plagula` node, loops "consume a list of rules" — which for now consumes nothing — and puts the EOF token in `cauda`. Everything else comes in Tasks 10-13.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh arbor`
Expected: exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_arbor.h css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: spina plagulae - nodus radicis cum cauda, casus degeneres viridis"
```

---

## Task 10: Qualified rules, blocks, and the byte-coverage gate

**This is the task where the round trip first becomes meaningful**, so its gate is the important deliverable — more than the parsing.

**Files:**
- Modify: `css/fontes/css_arbor.c`
- Create: `css/probationes/probatio_css_circuitus.c`

- [ ] **Step 1: Write the failing byte-coverage gate**

This is spec gate §9.7 — the specific guard for §6's hybrid seam. It is worth more than any structural assertion, because it cannot pass vacuously.

```c
/* Omnis octetus fontis EXACTE SEMEL apparere debet, sive in
 * lexemate arboris sive in triviis eius. Non 'plerique', non
 * 'summa congruit' - tegumentum per octetum. */
hic_manens b32
_tegumentum_probare (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
       SilvaNodus* radix;
    SilvaScriptura emissa;

    radix = css_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL) { redde FALSUM; }

    /* ORACULUM SEPARANS (par. IX.4): hic emittimus DIRECTE ex
     * parsura, sine STML. Cum porta STML cadat, haec dicit
     * utrum analyzator an proiectio culpanda sit.
     *
     * expansio = NIHIL: CSS praeprocessorem non habet, ergo
     * lexema omne stratum ZERO est et limes expansionis
     * numquam attingitur. */
    emissa = silva_scribere_nodum(piscina, radix,
                                  &CSS_REGISTRUM, NIHIL);

    si (!emissa.successus) { redde FALSUM; }
    si (emissa.textus.mensura != mensura) { redde FALSUM; }
    redde (b32)(memcmp(emissa.textus.data, fons,
                       (size_t)mensura) == 0);
}
```

```c
    imprimere("\n--- Probans circuitum octetorum, regulae ---\n");
    CREDO_VERUM (_tegumentum_probare(piscina, "a{color:red}", XII));
    CREDO_VERUM (_tegumentum_probare(piscina,
        "a , b > c { color : red ; }", XXVII));
    CREDO_VERUM (_tegumentum_probare(piscina,
        "\n/* c */\na{}\n", XIII));
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh circuitus`
Expected: FAIL — rules are not parsed yet, so emission drops them.

- [ ] **Step 3: Implement qualified rules and `saeptum`**

"Consume a qualified rule": accumulate component values into `praeludium` until `{`, then consume the block into `corpus`. "Consume a simple block": `tok_apertum`, contents until the matching close, `tok_clausum`. EOF inside a block closes it — total, never fails.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh circuitus`
Expected: exit 0.

- [ ] **Step 5: Plant a fault and verify the gate catches it**

Temporarily drop the `tok_clausum` assignment in the block builder. Re-run: the gate MUST fail with a length mismatch. Restore. **A gate that stays green with a dropped token is not a gate.**

- [ ] **Step 6: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_circuitus.c
git commit -m "css: regulae qualificatae + saepta; porta tegumenti octetorum cum oraculo separante"
```

---

## Task 11: Declarations, `!important`, and the hybrid whitespace rule

**Files:**
- Modify: `css/fontes/css_arbor.c`
- Modify: `css/probationes/probatio_css_arbor.c`

- [ ] **Step 1: Write the failing test — the §6 seam, asserted directly**

```c
    imprimere("\n--- Probans spatia UT CONTENTUM in valoribus ---\n");
    {
        /* 'margin: 1px 2px' - spatium inter valores nodus est
         * (lexema-servatum), non trivium. Par. VI, D7. */
        SilvaNodus* n = css_arbor_parsare(piscina,
            "a{margin:1px 2px}", XVII);
        SilvaNodus* regula = /* prima regula */;
        SilvaNodus* decl   = /* prima declaratio corporis */;
        SilvaValor  valor  = decl->loci[II];   /* locus 'valor' */

        /* 1px, SPATIUM, 2px = III elementa - non II */
        CREDO_AEQUALIS_I32 (silva_valor_lista_numerus(valor), III);
    }

    imprimere("\n--- Probans spatia UT TRIVIA structuraliter ---\n");
    {
        /* Spatium inter regulas trivium est, non nodus. */
        SilvaNodus* n = css_arbor_parsare(piscina, "a{} b{}", VII);
        CREDO_AEQUALIS_I32 (
            silva_valor_lista_numerus(n->loci[ZEPHYRUM]), II);
    }

    imprimere("\n--- Probans praevalentiam ---\n");
    CREDO_VERUM (_tegumentum_probare(piscina,
        "a{color:red !important}", XXIII));
    CREDO_VERUM (_tegumentum_probare(piscina,
        "a{color:red ! important }", XXV));
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh arbor`
Expected: FAIL.

- [ ] **Step 3: Implement**

"Consume a declaration": ident, optional trivia, `:`, then component values in **verbatim mode** (spec §6 — call `css_adaptare` with `spatia_ut_contentum = VERUM` for this span, or equivalently do not absorb trivia while inside a value). Detect trailing `!` + `important` into `praevalentia`.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh` (whole suite — the coverage gate must still hold)
Expected: exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: declarationes + praevalentia; regula hybrida spatiorum (D7) asserta utroque latere"
```

---

## Task 12: At-rules, component values, and spec-defined recovery

**Files:**
- Modify: `css/fontes/css_arbor.c`
- Modify: `css/probationes/probatio_css_arbor.c`

- [ ] **Step 1: Write the failing test**

```c
    imprimere("\n--- Probans regulas apud ---\n");
    CREDO_VERUM (_tegumentum_probare(piscina,
        "@import url(a.css);", XIX));
    CREDO_VERUM (_tegumentum_probare(piscina,
        "@media screen{a{color:red}}", XXVI));
    /* Apud IGNOTUM: saeptum genericum, numquam fractura */
    CREDO_VERUM (_tegumentum_probare(piscina,
        "@ignotum (x) { y z }", XX));

    imprimere("\n--- Probans functiones et saepta nidificata ---\n");
    CREDO_VERUM (_tegumentum_probare(piscina,
        "a{color:rgba(1,2,3,.4)}", XXIII));
    CREDO_VERUM (_tegumentum_probare(piscina,
        "a{w:calc((1px + 2px) * 3)}", XXVI));

    imprimere("\n--- Probans recuperationem spec-definitam ---\n");
    CREDO_VERUM (_tegumentum_probare(piscina, "}a{}", IV));
    CREDO_VERUM (_tegumentum_probare(piscina, "a{;;color:red}", XIV));
    CREDO_VERUM (_tegumentum_probare(piscina, "a{color}", VIII));
    CREDO_VERUM (_tegumentum_probare(piscina, "a{", II));
    CREDO_VERUM (_tegumentum_probare(piscina, "a{b:\"unterminated", XVIII));
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh arbor`
Expected: FAIL.

- [ ] **Step 3: Implement**

At-rules with the known-at-rule table (spec §4): rule-bodied (`@media`, `@supports`, `@document`, `@layer`) vs declaration-bodied (`@font-face`, `@page`, `@property`, `@counter-style`) vs statement-only (`@import`, `@charset`, `@namespace`). Unknown → generic `saeptum` of component values. `functio` for ident+paren; `saeptum` for `(`/`[`/`{`. `regula-mala` / `declaratio-mala` for the recovery paths.

- [ ] **Step 4: Run and verify it passes**

Run: `./css/compile_probationes.sh`
Expected: exit 0.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_arbor.c css/probationes/probatio_css_arbor.c
git commit -m "css: regulae apud + valores componentes + recuperatio; L3 completa"
```

---

## Task 13: The corpus gate

**Files:**
- Create: `css/probationes/probatio_css_corpus.c`
- Create: `probationes/fixa/css/adversarius_2.css`

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

Every file must satisfy `_tegumentum_probare`. Then the coverage self-check, which is the part that matters:

```c
    /* Porta tegumentum SUUM metiatur: 'VI/VI munda' mentitur
     * si nulla plagula umquam aperta est. */
    CREDO_AEQUALIS_I32 (plagulae_lectae, VI);
    CREDO_MAIOR_I32 (octeti_probati, MMMM);
```

- [ ] **Step 2: Write the second adversarial fixture**

`adversarius_2.css` covering what `adversarius.css` does not: `}` at top level, `@` at EOF, deeply nested blocks, CDO/CDC, mixed `\r\n` and `\n`, non-ASCII identifiers, a bare `!`, `!important` with a comment inside it.

- [ ] **Step 3: Run and record which files fail**

Run: `./css/compile_probationes.sh corpus`
Expected: some failures. **Read failure #1 before reading the tally** — it may be failing for a reason upstream of what you think you are testing. Judge by the **cause tally**, not the total: a headline that holds steady can hide one cause shrinking while another grows.

- [ ] **Step 4: Fix causes until green, one cause per commit**

- [ ] **Step 5: Commit**

```bash
git add css/probationes/probatio_css_corpus.c probationes/fixa/css/adversarius_2.css
git commit -m "css: porta corporis - VI plagulae reales octetim exactae, tegumentum se metiens"
```

---

## Task 14: STML projection round trip

**Files:**
- Create: `css/probationes/probatio_css_stml.c`

- [ ] **Step 1: Write the failing round-trip gate — TWO cycles, not one**

```c
    /* DUO circuitus, non UNUS: vitium quod se COMPONIT in
     * circuitu uno invisibile est (lex ex arbore M1). */
    scriptura_a = silva_arbor_scribere_nodum(piscina, radix,
        &CSS_REGISTRUM, "css", NIHIL, intern, &CSS_LEXICON);
    CREDO_VERUM (scriptura_a.successus);

    relecta_a = silva_arbor_legere(piscina, intern,
        scriptura_a.textus, &CSS_REGISTRUM, "css", &vitium,
        &CSS_LEXICON);
    CREDO_NON_NIHIL (relecta_a);

    scriptura_b = silva_arbor_scribere_nodum(piscina, relecta_a,
        &CSS_REGISTRUM, "css", NIHIL, intern, &CSS_LEXICON);
    CREDO_VERUM (scriptura_b.successus);

    /* Documenta duo IDEM esse debent */
    CREDO_CHORDAE_AEQUALES (scriptura_a.textus, scriptura_b.textus);

    /* Et arbores comparator STRUCTURALITER aequales dicere debet */
    CREDO_VERUM (silva_arbor_aequalis(radix, relecta_a,
        SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia));
```

Plus the census assertion from spec §3 — **presence, never pinned counts**:

```c
    /* Familia compressionis MORTUA est pro CSS (par. III).
     * Exitum VALIDUM incompressum relinquit - ergo ZERO
     * asserimus, et documentum tamen circuit. */
    CREDO_AEQUALIS_I32 (scriptura_a.census.spatia_vocationes,
                        ZEPHYRUM);
    CREDO_CHORDA_NON_VACUA (scriptura_a.textus);
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh stml`
Expected: FAIL.

- [ ] **Step 3: Fix what it names**

Expect refusals from the writer for genera or slot species it has not seen from CSS. Each refusal names its cause — fix the cause, never the assertion.

- [ ] **Step 4: Extend to the whole corpus**

Every file from Task 13 through the full round trip, **beside** the direct-emission gate. When one fails and the other passes, you know which half is broken — that is the separating oracle earning its place.

- [ ] **Step 5: Run and verify**

Run: `./css/compile_probationes.sh`
Expected: exit 0.

- [ ] **Step 6: Commit**

```bash
git add css/probationes/probatio_css_stml.c
git commit -m "css: circuitus STML - duo cycli, comparator structuralis, census compressionis ZERO assertus"
```

---

## Task 15: `css.canon`

**Files:**
- Create: `css/grammatica/css.canon`, `css/canon.sh`
- Create: `css/probationes/probatio_css_canon.c`

**Note:** Task 1 landed on path (b), so this canon is **generated**, not hand-written. G3 still holds — a vestigial production gives `silva_gen_impletiones_computare` nothing to close over, so slot constraints come out **species-level, not genus-level**. Say so in the file header, or a later reader will mistake the weakness for an oversight.

- [ ] **Step 1: Write the failing gate — every produced document must be judged**

```c
    /* canon_iudicare Xar de CanonVitium reddit; VACUUM = sanum
     * (include/canon.h:257). NIHIL redditum fractura est, non
     * sanitas - ergo utrumque separatim asserimus. */
    vitia = canon_iudicare(canon, resultus.radix, piscina);
    CREDO_NON_NIHIL (vitia);
    CREDO_AEQUALIS_I32 (xar_numerus(vitia), ZEPHYRUM);
    documenta_iudicata = documenta_iudicata + I;

    /* Porta tegumentum SUUM metiatur: 'nulla vitia' mentitur si
     * nullum documentum umquam iudicatum est. */
    CREDO_MAIOR_I32 (documenta_iudicata, ZEPHYRUM);
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh canon`
Expected: FAIL — no canon exists.

- [ ] **Step 3: Produce the canon**

Generated per Task 1's path, or hand-written at species level. Record in the file's header comment that slot constraints are **species-level, not genus-level**, and why (G3) — so a later reader does not mistake the weakness for an oversight.

- [ ] **Step 4: Add the freshness gate**

`css/canon.sh -probare` regenerates into a temp file and compares **OUTPUT, never timestamps**. A committed artifact must be invocation-independent, or two correct regenerations disagree.

- [ ] **Step 5: Run and verify**

Run: `./css/compile_probationes.sh` and `./css/canon.sh -probare`
Expected: both exit 0.

- [ ] **Step 6: Commit**

```bash
git add css/grammatica/css.canon css/canon.sh css/probationes/probatio_css_canon.c
git commit -m "css: canon + porta recentiae (comparatio EXITUS, non temporum)"
```

---

## Task 16: `css_selector` — the selector subtree

**Files:**
- Create: `css/fontes/css_selector.{h,c}`, `css/probationes/probatio_css_selector.c`

**Interfaces:**
- Produces:

```c
/* Praeludium (lista valorum componentium) in arborem
 * selectorum vertere. TOTALIS: quod parsari nequit
 * 'selector-malus' fit, numquam fractura. */
SilvaNodus*
css_selector_parsare (
    Piscina*   piscina,
    SilvaValor praeludium);
```

- [ ] **Step 1: Write the failing test — the combinator distinction FIRST**

This is the assertion the whole module exists for. `a b` and `ab` are different selectors; if the tree cannot tell them apart, the design in §6(a) has failed.

```c
    imprimere("\n--- Probans spatium UT COMBINATOREM ---\n");
    {
        SilvaNodus* duo  = _selectorem_parsare(piscina, "a b");
        SilvaNodus* unus = _selectorem_parsare(piscina, "ab");

        /* 'a b' = complexus cum combinatore */
        CREDO_AEQUALIS_S32 (duo->genus,
                            (s32)CSS_GENUS_SELECTOR_COMPLEXUS);
        /* 'ab' = compositus solus, nullus combinator */
        CREDO_AEQUALIS_S32 (unus->genus,
                            (s32)CSS_GENUS_SELECTOR_COMPOSITUS);

        /* Combinator lexema SPATIORUM IPSUM est - promotum in
         * locum, ergo VALOR, non trivium (par. VI(a)) */
        {
            SilvaToken* comb = duo->loci[I].datum.token;
            CREDO_NON_NIHIL (comb);
            CREDO_AEQUALIS_S32 (comb->genus, (s32)CSS_LEX_SPATIA);
            CREDO_MAIOR_AUT_AEQUALIS_S32 (comb->byte_offset, ZEPHYRUM);
        }
    }

    imprimere("\n--- Probans 'a>b' et 'a > b' AEQUALES ---\n");
    {
        SilvaNodus* strictus = _selectorem_parsare(piscina, "a>b");
        SilvaNodus* laxus    = _selectorem_parsare(piscina, "a > b");
        SilvaArborDifferentia d;
        /* Structuraliter idem: spatia circum '>' TRIVIA sunt */
        CREDO_VERUM (silva_arbor_aequalis(strictus, laxus,
            SILVA_ARBOR_COMPARATIO_STRUCTURALIS, &d));
    }
```

- [ ] **Step 2: Run and verify it fails**

Run: `./css/compile_probationes.sh selector`
Expected: FAIL — `css_selector_parsare` undefined.

- [ ] **Step 3: Implement**

Split the prelude on top-level commas into `selector-lista`. Within each: compound selectors separated by combinators, where a combinator is `>` `+` `~` **or a whitespace token that is not adjacent to one of those**. That adjacency rule is the whole subtlety — in `a > b` the spaces belong to `>` as trivia; in `a b` the space *is* the combinator.

- [ ] **Step 4: Run and verify it passes, and that the corpus still round-trips**

Run: `./css/compile_probationes.sh`
Expected: exit 0, corpus gate unchanged.

- [ ] **Step 5: Commit**

```bash
git add css/fontes/css_selector.h css/fontes/css_selector.c css/probationes/probatio_css_selector.c
git commit -m "css: arbor selectorum - spatium ut combinator promotum, 'a b' != 'ab' structuraliter"
```

---

## Task 17: Wire-up and debrief

**Files:**
- Modify: `compile_tests.sh` or `excubitor.sh` (register `css/compile_probationes.sh`)
- Create: `css/fontes/css_arbor.worklog.md`
- Modify: `MEMORY.md`, `project-specs/css-arbor-spec.md`

- [ ] **Step 1: Register the suite so it runs unattended**

Add `css/compile_probationes.sh` wherever `silva/` and `tessera/` runners are invoked. Verify it actually runs — a registered-but-unreached suite is the silent-gate failure again.

- [ ] **Step 2: Write the worklog**

Record: what the corpus gate found that unit tests missed (expect this to be non-empty — *corpus > fixtures*, every time); anything about the lexicon seam that differed from the spec's prediction; and every prediction from the spec's §12 that turned out **wrong**, stated as wrong.

- [ ] **Step 3: File the instrument debrief**

Per house doctrine, every RELATIO carries: `adhibita` / `fructus` / `asperitates` / `desiderata`.

- [ ] **Step 4: Close the ledger items**

`mcp__tabularii__gerere` on decree `01M12BCD` with the outcome. Update parcum `01M12BD0` (S6) with what the second client actually revealed about the seam — **this is the S6 evaluation evidence**, and it is the most valuable thing this project produces beyond the parser itself.

- [ ] **Step 5: Commit**

```bash
git add -A css/ MEMORY.md project-specs/css-arbor-spec.md
git commit -m "css: css_arbor navatum - relatio + acta clausa"
```

---

## Self-Review

**Spec coverage.** §1 measurement → T4/T17 (re-measured, not assumed). §2 architecture → T2/T7/T8/T9. §3 lexicon seam → T4/T5, with §3's compression consequence asserted in T7 and T14. §4 vocabulary → T3, parsed in T10-T12. §5 selectors → T3 (genera) + T16 (parser). §6 whitespace → T11 (both sides of the hybrid rule) + T16 (combinator). §7 document → T14; the `cauda` slot lands in T3 and is exercised in T9. §8 canon → T15. §9 gates 1-9 → T10 (2, 4, 9), T13 (6, and coverage self-measurement), T14 (3, 8), T15 (5), T11 (7). §11 G1 → T17 Step 4; G2 → T6; G3 → T15 Step 3; G4 → T1.

**Placeholder scan.** One deliberate gap remains, and it is the correct kind: T1's Step 2 expected result is "unknown" **by design** — that is what a spike is. Every other expected result is a concrete verdict. T11's test has two `/* prima regula */` navigation comments rather than spelled-out slot walks; the implementer has `CssGenus` and the slot layout from T3 and does not need them dictated.

**Type consistency.** `SilvaLexicon` is defined once (T4) and consumed identically in T7, T14, T16. `CssGenus` is defined once (T3) and used unqualified thereafter. `css_adaptare`'s `spatia_ut_contentum` flag is introduced in T8 and is what T11 relies on. `_tegumentum_probare` is defined in T10 and reused verbatim in T11-T13. `silva_arbor_scribere_nodum` and `silva_arbor_legere` gain their trailing `lexicon` parameter in T4 and are called with it consistently in T14.

**Signatures verified against the headers, not from memory.** Three were
wrong on the first pass and are corrected above: `silva_scribere_nodum`
takes **four** arguments and returns **`SilvaScriptura`**, not `chorda`
(`silva_scribere.h:77`); the source-token constructor is
**`silva_token_ex_fonte`**, not `silva_token_fontis`
(`silva_token.h:262`); and `canon_iudicare` returns an **`Xar*` of
`CanonVitium`** where empty means healthy — it takes no out-parameter
(`canon.h:257`). Confirmed present as written: `silva_lexare` (4 args,
`silva_lexema.h`), `chorda_ex_literis` / `chorda_aequalis_literis`
(`chorda.h:23,103`), `xar_obtinere` / `xar_numerus` (`xar.h:121,270`),
and every `CREDO_*` macro used. `silva_arbor_scribere_nodum` and
`silva_arbor_legere` are called in T14 with the trailing `lexicon`
parameter that **T4 adds** — forward-looking by design, not a mismatch.

**One honest gap.** Task 5's Step 2 is "let the compiler enumerate the work" rather than a list of sites. That is deliberate — the sites are mechanical and `-Werror` is a more reliable enumerator than I am, and any list I wrote would be stale the moment Task 4 moved code. It is the one task whose size I genuinely cannot predict.
