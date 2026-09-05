# briar plan 4 — contextus (fragments and transclusion)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit).
> Steps use checkbox syntax. Written 2026-09-05 from briar-spec.md §3.4
> (v1.6, approved by Fran the same day). Not started.

**Goal:** `<c! id="x">` regions become fragments; a `<<#x>>` line inside
any C region weaves the fragment in; the compiler's errors still name
the right thistle line, inside fragments too.

**Architecture:** one new unit `briar_contextus` between nexus and
silva (spec §3.4 D5): classify regions, weave every root depth-first,
record a line table per root, refuse loudly. Downstream, silva parses
the woven text, the nexus translates lines through the table, the
fabrica emits `#line` per run instead of per unit. No grammar change:
the tree of plan 1 is untouched.

**Tech stack:** C89 in Latin, materia/briar tree, silva (amalgam),
silex closure, credo gates, `tools/briar_fumus.sh`.

**Spec:** `project-specs/briar-spec.md` §2 (the `id=` row, the
transclusion-line law), §3.4 (the whole design), §4.2 (per-run
`#line`), §6 (gate row), §9 (reservations).

## Global constraints

- C89, `latina.h` words; latina macros are forbidden identifiers
  (`nomen registrum magnitudo structura casus brevis exire`, single
  capitals are numerals). `chorda` is not NUL-terminated; `i32` is
  unsigned (`s32` for sentinels and sort indices). `lib/*.c` inside a
  block comment breaks it (`-Wcomment`).
- Worktree `../rhubarb-briar`, level with main: new C files through
  `./silva/scribe.sh <via> <<'EOF'`; script edits through
  `silva.Editio`; format `./silva/formator.sh <files> -scribere` then
  `-vitia` = CONFORMIS before committing; commit through
  `silva.commissio_umbra(msg, viae, portae)` with individual file paths,
  portae `['briar', 'briar-fumus', ('radix','silex'), 'pythonica']`
  (+ `('oratio','glossarium')` when the glossary changes). No ledger
  residents from the worktree. Regex renames hit strings: never.
- No clang in the suite; the fumus compiles. Every new gate is born
  red by a planted fault, and the plant is checked GONE (`grep -c`)
  before "restored".
- A new identifier word needs a glossary line (`oratio/glossarium.stml`)
  or a rename; measure with `./oratio/vocabula.sh -machina` after
  `./silva/nexus.sh -renovare`, delta vs main must be 0.
- Refusals carry the thistle line; the fabrica prints `via:linea:`.

## File structure

| file | responsibility |
|---|---|
| `briar/fontes/briar_contextus.h/.c` (new) | line reader, classification, weave, line table, fragment list, refusals |
| `briar/fontes/briar_nexus.h/.c` | three new record fields; `briar_nexus_linea_silvae` reads the table |
| `briar/fontes/briar_silva.c` | parse the contextus; skip fragments; `_regio_includit` over the contextus |
| `briar/fontes/briar_fabrica.c` | inventory skips fragments; directives, units, main and probatio emitted per run through the table; closure over woven texts |
| `tools/briar.c` | call `briar_contexere`; `-partes` lists fragments |
| `tools/briar_fumus.sh` | fragmenta stages incl. the `#line`-truth stage |
| `briar/probationes/probatio_briar_contextus.c` (new) + fixtures + golden `fixa/contextus/fragmenta.contextus` | the gate |
| `briar/probationes/fixa/thistle/fragmenta.thistle`, `adversa/fragmentum_{absens,circulus,iteratum,munus,sine_id}.thistle`, `adversa/transclusio_malformata.thistle`, `adversa/fragmentum_erratum.thistle` | fixtures |
| `briar/probationes/fixa/fabrica/fragmenta/` | fabrica goldens |
| `briar/CLAUDE.md`, `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md`, spec §3.4 as-built | docs |

---

### Task 1: the unit and its gate

**Files:** create `briar/fontes/briar_contextus.h`, `briar/fontes/briar_contextus.c`, `briar/probationes/probatio_briar_contextus.c`, the fixtures, the golden; modify `briar/fontes/briar_nexus.h` (fields only).

**Interfaces produced:**

```c
/* briar_nexus.h - tres campi novi in BriarNexusRes (post praeludium_octeti) */
                   chorda contextus;       /* C radix: textus contextus;
                                            * fragmentum: contentum */
                      Xar* lineae;         /* i32 per lineam contextus:
                                            * linea .thistle */
                      b32 est_fragmentum;  /* id= datum */
```

```c
/* briar_contextus.h */
nomen structura {
             chorda  id;      /* sine '#' */
     BriarNexusRes*  regio;
                Xar* usus;    /* i32: lineae .thistle transclusionum, ordine */
} BriarFragmentum;

/* lector lineae: I = transclusio (id + praefixum albus positi),
 * ZEPHYRUM = linea ordinaria, -I = malformata ('<<#' initio, forma alia) */
s32 briar_contextus_lineam_legere(chorda linea, chorda* id, chorda* praefixum);

/* classificat regiones C (id= -> est_fragmentum), radices contexit
 * (contextus + lineae; radix sine transclusione = tabula identitatis),
 * recusationes in regione (causa + linea_erroris); *fragmenta (si non
 * NIHIL) = Xar de BriarFragmentum ordine documenti; redde radices
 * contextas aut -I (memoria) */
s32 briar_contexere(Piscina* piscina, Xar* nexus, Xar** fragmenta);
```

**The fixture `fragmenta.thistle`** (also the fabrica and fumus fixture; every line number below is asserted, so do not reflow it):

```
#!/usr/bin/env briar
# Fragmenta

The loop body lives apart from the loop, the loop apart from the
function; one greeting fragment is used by the program and by its
probatio; one fragment carries an include; one is never used.

<c! id="incrementum">
summa = summa + numeri[i];
</c>

<c! id="summa">
per (i = ZEPHYRUM; i < n; i++)
{
    <<#incrementum>>
}
</c>

<c! id="capita">
#include "chorda.h"
</c>

<c! id="salutatio">
chorda salutatio = chorda_ex_literis("salve, fragmenta", piscina);
</c>

<c! id="otiosum">
/* numquam adhibitum */
</c>

<c!>
<<#capita>>

i32
summare (constans i32* numeri, i32 n)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    <<#summa>>
    redde summa;
}

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("fragmenta", 65536);
    i32 numeri[3];
    <<#salutatio>>

    numeri[0] = I;
    numeri[1] = II;
    numeri[2] = III;
    imprimere("%.*s: %d\n", (integer)salutatio.mensura,
        (constans character*)salutatio.datum,
        (integer)summare(numeri, III));
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
</c>

<c! munus="probatio">
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);
    i32 numeri[3];
    <<#salutatio>>

    numeri[0] = I;
    numeri[1] = II;
    numeri[2] = III;
    credo_aperire(piscina);
    CREDO_AEQUALIS_I32(summare(numeri, III), VI);
    CREDO_AEQUALIS_I32(salutatio.mensura, XVI);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
</c>
```

Line facts the gate pins: `incrementum` content line 9; `summa` lines 13–16 (the reference at 15 with four spaces); `capita` line 19; `salutatio` line 23; `otiosum` line 27; app root content starts line 31 (`<<#capita>>` at 31, `<<#summa>>` at 40, `<<#salutatio>>` at 49); probatio root content starts line 62 (`<<#salutatio>>` at 69). The woven app root: line 31 becomes `#include "chorda.h"` mapped to 19; line 40 becomes four lines mapped 13, 14, 9, 16 — the third is `        summa = summa + numeri[i];` (eight spaces: four from the root's reference plus four from the fragment's own); the line after maps to 41.

**Adversa fixtures** (one refusal each; keep each under fifteen lines and note the expected line in the test):
- `fragmentum_absens.thistle`: a root with `    <<#nusquam>>` → `transclusio '#nusquam' (linea N): fragmentum non definitum`, N = the reference line.
- `fragmentum_circulus.thistle`: `<c! id="a">` containing `<<#b>>`, `<c! id="b">` containing `<<#a>>`, a root containing `<<#a>>` → `transclusio '#a' (linea N): circulus #a -> #b -> #a`, N = the `<<#a>>` line inside `b`; recorded on the ROOT's record.
- `fragmentum_iteratum.thistle`: two `<c! id="bis">` → `fragmentum '#bis' iteratum (prima linea M)`, on the second region's record, line = its open tag line.
- `fragmentum_munus.thistle`: `<c! id="x" munus="probatio">` → `fragmentum '#x' (linea N): methodus/munus in fragmento - fragmentum radix non est`.
- `fragmentum_sine_id.thistle`: `<c! id="a b">` → `fragmentum sine id valido (linea N)`.
- `transclusio_malformata.thistle`: a root with `<<#x>> extra` → `transclusio malformata (linea N): '<<#x>>' sola in linea sua exspectata`.

**Golden `fixa/contextus/fragmenta.contextus`**, written by the gate under `BRIAR_CONTEXTUS_SCRIBERE=1`: for each root in document order, `=== radix linea <linea_initium> ===\n`, the contextus bytes, `=== lineae ===\n`, the table as space-separated numbers and a newline.

- [ ] **Step 1: write the fixtures** (`fragmenta.thistle` exactly as above; the six adversa).
- [ ] **Step 2: write the failing gate** `probatio_briar_contextus.c` (helpers copied from `probatio_briar_silva.c`: `_plagulam_legere`, a `_nexus_ex(via)` that parses + `briar_nexus_texere`; no fons needed — the contextus needs no silva). Assertions:
  - `briar_contextus_lineam_legere`: `"    <<#summa>>"` → I, id `summa`, praefixum four spaces; `"<<#x>>  "` → I; `"\t<<#a-b_c>>"` → I with tab prefix; `"<<#x>> extra"` → -I; `"<<#>>"` → -I; `"a << b"` → ZEPHYRUM; `"    redde <<#x>>;"` → ZEPHYRUM (mid-line stays bytes); `""` → ZEPHYRUM.
  - `fragmenta.thistle`: `briar_contexere` returns II; five fragments in document order with ids `incrementum summa capita salutatio otiosum`; `est_fragmentum` on those five, off on the two roots; `usus` of `salutatio` = {49, 69}, of `otiosum` empty, of `incrementum` = {15}; app root: `xar_numerus(lineae)` == number of `\n` in its contextus; the pinned lines (31→19, 40→13, 41→14, 42→9, 43→16, 44→41); the eight-space line present; golden byte-compare; probatio root: line 69 → 23.
  - `salve.thistle`: II roots, no fragments, identity tables (`lineae[k] == linea_initium + k`), contextus byte-equal to contentum.
  - each adversa: refusal text fragment + `linea_erroris`, on the record named above; `briar_contexere` still returns ≥ ZEPHYRUM (refusals are per region, not memory).
- [ ] **Step 3: run** `./briar/compile_probationes.sh contextus` — expect a compile failure (no header yet). That is the red.
- [ ] **Step 4: write the unit.** Sketch of the core:

```c
/* linea: praefixum albus, '<<#', id (sine albo, sine '>'), '>>', albus */
s32
briar_contextus_lineam_legere (chorda linea, chorda* id, chorda* praefixum)
{
    i32 p = ZEPHYRUM; i32 initium;
    dum (p < linea.mensura && (linea.datum[p] == ' ' || linea.datum[p] == '\t')) p++;
    si (p + III > linea.mensura || memcmp(linea.datum + p, "<<#", 3) != 0) redde ZEPHYRUM;
    *praefixum = chorda_sectio(linea, ZEPHYRUM, p);
    p = p + III; initium = p;
    dum (p < linea.mensura && linea.datum[p] != '>' && linea.datum[p] != ' ' && linea.datum[p] != '\t') p++;
    si (p == initium || p + II > linea.mensura || linea.datum[p] != '>' || linea.datum[p + I] != '>') redde -I;
    *id = chorda_sectio(linea, initium, p);
    p = p + II;
    dum (p < linea.mensura && (linea.datum[p] == ' ' || linea.datum[p] == '\t' || linea.datum[p] == '\r')) p++;
    redde p == linea.mensura ? I : -I;
}
```

  The weave: `_contexere_regionem(ctx, r, praefixum, acervus)` walks `r->contentum` line by line (thistle line = `r->linea_initium + k`); an ordinary line appends `praefixum + linea + '\n'` (blank line: `'\n'` alone) and pushes its thistle line; a transclusion looks the id up in a `TabulaDispersa` (chorda id → `BriarFragmentum*`), refuses if absent, refuses `circulus` if the id is on the `acervus` (an Xar of chorda; the message joins the stack from the first occurrence of the id, then the id again), else pushes the id, records the use line on the fragment, recurses with `praefixum + the line's own prefix`, pops. Refusals go on `ctx->radix` for weaving faults and on the fragment's record for definition faults (duplicate at the second definition, `munus`/`methodus` present, invalid id). Classification runs first over all C regions, then every root is woven; a root without a transclusion gets contextus = contentum and the identity table (still built line by line — one shape). Fragments get `contextus = contentum`, `lineae = NIHIL`.

- [ ] **Step 5: run green** `./briar/compile_probationes.sh contextus`; write the golden with `BRIAR_CONTEXTUS_SCRIBERE=1`, inspect it by eye (the eight-space line, the `#include` at file scope), run again without the variable.
- [ ] **Step 6: plant** — drop the `praefixum` prepend in the weave; gate must go red on the golden AND on the eight-space assertion; restore; `grep -c PLANTA` = 0; green again.
- [ ] **Step 7: format + commit** (`formator -scribere`, `-vitia` CONFORMIS; vocabula delta 0; `commissio_umbra` with the five files + fixtures + golden, portae `['briar', 'briar-fumus', ('radix','silex'), 'pythonica']`).

---

### Task 2: nexus and silva read the contextus

**Files:** modify `briar/fontes/briar_nexus.c` (`briar_nexus_texere` zeroes the fields; `briar_nexus_linea_silvae`), `briar/fontes/briar_silva.c` (`_parsare`, `_regio_includit`, `briar_silvam_texere` skips fragments), `briar/probationes/probatio_briar_silva.c` (`_texere_omnia` calls `briar_contexere`; new section), `probatio_briar_fabrica.c` / `probatio_briar_amalgama.c` (`_fabricare` calls `briar_contexere` — needed before Task 3 runs anything).

**Interfaces consumed:** `briar_contexere`, the three fields.

- [ ] **Step 1: failing assertions** in `probatio_briar_silva.c`, new section "fragmenta": `_texere_omnia` on `fragmenta.thistle` (after adding the `briar_contexere` call to the helper): app root's `capita_derivata` = {`piscina.h`} only (chorda.h arrives through the woven `#include`, `_regio_includit` sees it in the contextus); probatio root's = {`chorda.h`, `piscina.h`} (credo.h is its own include; `chorda_ex_literis` lives only inside the fragment); `numerus_errorum` 0 for both; `briar_nexus_linea_silvae(r, r->praeludium + 12 + 1)` on the app root == 9 (woven line index 12 is the `incrementum` line: 31..39 are indices 0..8, then index 9→13, 10→14, 11→9 — recount against the golden and pin the true index); the fragment regions have `silva == NIHIL`.
- [ ] **Step 2: run red** (`./briar/compile_probationes.sh silva`).
- [ ] **Step 3: implement.** `briar_nexus_linea_silvae`: `k = linea_silvae - praeludium - 1`; if `lineae != NIHIL && 0 <= k < xar_numerus(lineae)` return `*(i32*)xar_obtinere(lineae, k)`, else the old formula. `_parsare`: append `r->contextus.datum != NIHIL ? r->contextus : r->contentum`. `_regio_includit`: same choice. `briar_silvam_texere`: `perge` on `est_fragmentum` in both loops. `briar_nexus_texere`: initialize the three fields (contextus = contentum for every region so callers that never weave keep working; lineae NIHIL; est_fragmentum FALSUM).
- [ ] **Step 4: run green**: silva, nexus, fabrica, amalgama gates (the fabrica helper now weaves; existing goldens must not move — salve/derivatum/punctum have no fragments and identity tables).
- [ ] **Step 5: commit.**

---

### Task 3: the fabrica emits per run

**Files:** modify `briar/fontes/briar_fabrica.c`; `briar/probationes/probatio_briar_fabrica.c`; goldens `briar/probationes/fixa/fabrica/fragmenta/`.

- [ ] **Step 1: failing test** section "fragmenta" in `probatio_briar_fabrica.c`: `_fabricare(fragmenta.thistle)` succeeds, `forma` PLANA, `regiones_c` == I (fragments are not app regions), `probatio_adest`; goldens for `fontes/fragmenta.c`, `fontes/fragmenta_regiones.c`, `include/fragmenta_regiones.h`, `probationes/probatio_fragmenta.c`; explicit: `_regiones.c` contains `#line 13 "…fragmenta.thistle"` then `#line 9 …` then `#line 16 …` then `#line 41 …` inside `summare` (in that order); the header's directive `#include "chorda.h"` is preceded by `#line 19` (the fragment's line, not 31); `probatio_fragmenta.c` contains `#line 23` before `chorda salutatio`; `fontes/fragmenta.c` (principale) contains `#line 23` then `#line 50`; closure has `lib/chorda.c`. Refusal cases through `_recusationem_probare` for the six adversa (texts + lines as in Task 1).
- [ ] **Step 2: run red.**
- [ ] **Step 3: implement.**
  - `_inventarium_colligere`: after the `linea_erroris` refusal check, `si (r->est_fragmentum) perge;`.
  - `_directivas_colligere(r, directivae)`: walk `r->contextus`; line of directive = `_linea_tabulae(r, k)` = `lineae[k]` (fallback `linea_initium + k`).
  - `BriarUnitas` gains `constans BriarNexusRes* regio` and `i32 index_contextus` (0-based woven index of its first line). In `_regionem_partiri`: `index_contextus = _linea_octeti(r->textus_silvae, minimum) - r->praeludium - I`; `linea = _linea_tabulae(r, index_contextus)`.
  - New `_textum_mappatum_appendere(a, textus, regio, index, via)`: for each line of `textus`, `t = _linea_tabulae(regio, index + k)`; emit `#line t "via"` when `k == 0` or `t != prior + I`; append the line + `\n`. `_unitates_appendere`, `_principem_fingere`, `_probationem_fingere` (over `probatio->contextus` from index 0) all use it. Old `_lineam_appendere` stays for the single-line case inside the helper.
  - Closure contents: `inv.app[i]->contextus`, `inv.probatio->contextus` (both arms).
- [ ] **Step 4: run green**; write goldens with `BRIAR_FABRICA_SCRIBERE=1 ./briar/compile_probationes.sh fabrica`, inspect `fragmenta_regiones.c` by eye (the four `#line` runs inside `summare`), rerun without the variable; the older goldens must be unchanged (`git status` shows only `fragmenta/`).
- [ ] **Step 5: by hand** `./briar/fabrica.sh briar/probationes/fixa/thistle/fragmenta.thistle <scratch>/fragmenta -fons` then `cd … && ./aedificare.sh && ./bin/fragmenta` → `salve, fragmenta: 6`; `./probare.sh` green. Then break line 9 of a scratch copy (`summa = summa + numeri[i]` → `summa = "x";`) and confirm clang says `fragmenta.thistle:9`.
- [ ] **Step 6: commit.**

---

### Task 4: the binary, the fumus, the docs

**Files:** modify `tools/briar.c`, `tools/briar_fumus.sh`, `briar/CLAUDE.md`, `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md`, `project-specs/briar-spec.md` (as-built notes in §3.4); create `briar/probationes/fixa/thistle/adversa/fragmentum_erratum.thistle`.

- [ ] **Step 1: `tools/briar.c`**: after `briar_nexus_texere`, `si (briar_contexere(piscina, nexus, &fragmenta) < ZEPHYRUM) { "briar: contextus fractus"; redde I; }` before `briar_silvam_texere`. `-partes`: before the derived headers, one line per fragment: `#<id>\tfragmentum:linea <open tag line>\tadhibitum:lineae a, b` or `…\tnon adhibitum`. Rebuild: `./briar/compile_probationes.sh registrum && ./tools/briar_struere.sh`.
- [ ] **Step 2: the `#line`-truth fixture** `adversa/fragmentum_erratum.thistle`: a fragment `<c! id="erratum">` whose single content line is `i32 x = "non numerus";` at a known line (say 7), a root `principale` that references it among its declarations. `./fragmentum_erratum.thistle` must FAIL to build and clang's message must contain `fragmentum_erratum.thistle:7:`.
- [ ] **Step 3: fumus** (`tools/briar_fumus.sh`): copy `fragmenta.thistle` and `adversa/fragmentum_erratum.thistle` into the area; add `fragmenta` to the stage VII amalgam loop; new stage VIII: `./fragmenta.thistle` → stdout contains `salve, fragmenta: 6`; `-probatio` green; `-partes` output contains `#otiosum` with `non adhibitum` and `#salutatio` with two lines; stage IX (`#line` truth): `./fragmentum_erratum.thistle` exits non-zero and its stderr contains `fragmentum_erratum.thistle:7:` (the number computed by `grep -n 'non numerus' fixture | cut -d: -f1` so the fixture may move). Renumber `-agere` to X. Run: `./tools/briar_fumus.sh` → `fumus briar: sanum`.
- [ ] **Step 4: docs**: `briar/CLAUDE.md` (map: `briar_contextus`; laws: fragments and the transclusion line; gates thirteen), FONTES.md (the eight fixtures + golden), worklog entry (findings + numbers), spec §3.4 "as built" line, §8 P5 DONE. Memory `briar-project.md` + MEMORY.md line; ledger nota through the main tree's resident.
- [ ] **Step 5: commit** (`commissio_umbra`, portae incl. `briar-fumus`); merge to main FROM the main tree (`git merge --ff-only rhubarb-briar` as its own call, never inside a `cd worktree &&` chain); rebake `~/.bin/briar` and `~/.bin/silex` from main; `./tools/briar_fumus.sh` from main.

---

## Self-review

- **Spec coverage:** D1 (`id=`, Task 1 classification), D2 (line reader, Task 1), D3 (duplicate refusal, Task 1), D4 (C only: only `c` regions are classified, Task 1), D5 (stage between nexus and silva, Tasks 1–2); the six refusals (Task 1 + Task 3 through the fabrica); the line table and per-run `#line` (Tasks 2–3); `-partes` (Task 4); the `#line`-truth fumus stage (Task 4); unused fragment listed, not refused (Tasks 1, 4); closure over woven texts (Task 3); derived includes over woven text (Task 2 assertions). Reserved items are not built.
- **Type consistency:** `briar_contexere(Piscina*, Xar* nexus, Xar** fragmenta)` and `briar_contextus_lineam_legere(chorda, chorda*, chorda*)` are the only new public names; `BriarFragmentum {id, regio, usus}`; fields `contextus`, `lineae`, `est_fragmentum` on `BriarNexusRes`; `BriarUnitas` gains `regio`, `index_contextus`.
- **Line numbers:** every pinned number above assumes `fragmenta.thistle` verbatim; the gate asserts them, so a reflow reds the gate rather than lying.
