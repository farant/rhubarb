# briar plan 5 — facies (the literate rendering, `-html`)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit).
> Steps use checkbox syntax. Written 2026-09-05 from briar-spec.md §4.6
> (v1.7, approved by Fran the same day). Not started.

**Goal:** `./x.thistle -html` writes one self-contained `x.html` beside
the file: the prose as prose, the C coloured by silva, every fragment
reference a link to its definition and every definition carrying its
use sites, each root's woven text behind a disclosure with a gutter of
**thistle** line numbers, library symbols answering with their
declaration, and every refusal pinned at its line.

**Architecture:** one new unit `briar_facies` after the fabrica — the
sibling of `briar_amalgama`, consuming the same `BriarFabricaFructus`
in memory. Structures in, one string out: no I/O, no globals. The
chrome (`facies.html/.css/.js` and md's template) is passed in as a
`BriarVestis`, so the gate feeds it fixed test chrome and the binary
feeds it from an embedded capsula — real CSS edits never move a
golden. No clang, no window, no network anywhere in the gate.

**Tech stack:** C89 in Latin, briar/materia tree, silva (amalgam,
`silva_lexare_cruda`), md (`md_html_reddere`), `stml_html`,
`html_lexema`, capsula, credo gates, `tools/briar_fumus.sh`.

**Spec:** `project-specs/briar-spec.md` §4.6 (the whole design, F1–F5),
§5 (the flag table), §6 (gate row), §7 (module row), §8 (P6), §9 (what
step 2 keeps).

## Global constraints

- C89, `latina.h` words; latina macros are forbidden identifiers
  (`nomen registrum magnitudo structura casus brevis exire`, single
  capitals are numerals). `chorda` is not NUL-terminated; `i32` is
  unsigned (`s32` for sentinels). A glob of the form `lib` slash star
  dot c inside a block comment breaks compilation (`-Wcomment`) —
  write it in words.
- **Main tree, branch `main`** (Fran, 2026-09-05: work here, not in
  `../rhubarb-briar`). New C files through `scribe <via> <<'EOF'`;
  script edits through `silva.Editio(via).replace` then
  `f = e.applicare(); assert f.sana`; format with
  `./silva/formator.sh <viae> -scribere` then `-vitia` = CONFORMIS
  before committing; commit through
  `silva.commissio_umbra(msg, viae, portae)` with individual file
  paths, portae `['briar', 'briar-fumus', 'pythonica']`. Regex renames
  hit strings and comments: never.
- The suite runs no clang and opens no window. `-html` opens nothing.
- Every new gate is born red by a planted fault, and the plant is
  checked GONE (`grep -c` = 0) before "restored".
- A new identifier word needs a glossary line (`oratio/glossarium.stml`)
  or a rename. The words this plan coins — `facies`, `vestis`,
  `involucrum`, `styli`, `scriptum`, `exemplar`, `tectum` — are
  classical and should cost nothing; **verify** with
  `./oratio/vocabula.sh -machina` after `./silva/nexus.sh -renovare`,
  delta vs the pin must be 0. Avoid `css` and `js` as identifier
  words: the permitted-technical list carries `html`, `json`, `stml`,
  `utf8` but not those two.
- Goldens are never regenerated without an audit and a named cause.
  Structural invariants and byte goldens are asserted separately (a
  class rename may redden the goldens; it must never redden the
  invariants).

## File structure

| file | responsibility |
|---|---|
| `briar/fontes/briar_facies.h/.c` (new) | `BriarVestis`, `briar_faciem_fingere`: fructus + nexus + fragments → one HTML page |
| `briar/probationes/probatio_briar_facies.c` (new) | the gate: invariants over an `html_lexema` lex, byte goldens, the `-partes` agreement |
| `briar/probationes/fixa/facies/*.html` (new) | byte goldens (test chrome, not the real chrome) |
| `briar/probationes/fixa/thistle/facies_evasio.thistle`, `facies_utf8.thistle` (new) | escaping and UTF-8 fixtures |
| `briar/facies/facies.html`, `.css`, `.js` (new) | the chrome, embedded verbatim; the page's whole style and its one script |
| `tools/briar_facies_capsula.sh` (new) | chrome + `md/html/md-html.stml` → `build/capsula_facies_briar.c` |
| `tools/briar_struere.sh` | link the facies capsula object |
| `briar/compile_probationes.sh` | `md_html.c` in the source list; the `facies` gate |
| `pythonica/silva.py` | four table rows for the new gate |
| `briar/fontes/briar_imperium.h/.c` | `BRIAR_ACTIO_HTML`, the `-html` flag |
| `tools/briar.c` | the action: fabricate, render, write `<t>.html`, print the path |
| `tools/briar_fumus.sh` | stage XI over `salutatio2.thistle` and a refusing fixture |
| `briar/CLAUDE.md`, `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md`, spec §4.6 as-built + §8 P6 | docs |

---

### Task 1: the unit, the page skeleton, the gate

**Files:** create `briar/fontes/briar_facies.h`, `briar/fontes/briar_facies.c`, `briar/probationes/probatio_briar_facies.c`, golden `briar/probationes/fixa/facies/salve.html`; modify `briar/compile_probationes.sh`, `pythonica/silva.py`.

**Interfaces produced:**

```c
/* briar_facies.h */
nomen structura {
    chorda involucrum;  /* facies.html: notae '<!--@corpus-->' etc. */
    chorda styli;       /* facies.css */
    chorda scriptum;    /* facies.js */
    chorda exemplar;    /* md/html/md-html.stml (programma md_html) */
} BriarVestis;

/* pagina una, sui sufficiens; vacua + causa in vitio (nota absens).
 * Nihil scribit, nihil legit: structurae intrant, chorda exit. */
chorda
briar_faciem_fingere (
                          Piscina* piscina,
              InternamentumChorda* intern,
                             Xar*  nexus,       /* BriarNexusRes */
                             Xar*  fragmenta,   /* BriarFragmentum */
    constans BriarFabricaFructus*  fructus,
                           chorda  fons,        /* octeti .thistle */
              constans character*  via,
             constans BriarVestis* vestis,
                           chorda* causa);
```

**The five markers** `facies.html` must carry, each replaced once;
a missing marker is a refusal naming it, never a silent omission:
`<!--@titulus-->`, `<!--@styli-->`, `<!--@caput-->` (head matter),
`<!--@corpus-->` (the document body), `<!--@symbola-->` (the JSON
island), `<!--@scriptum-->`.

**Test chrome** lives in the probatio as a C literal — six lines with
the six markers and nothing else — so the goldens hold the *body*, and
Task 4's real CSS cannot move them.

- [ ] **Step 1: register the gate.** `briar/compile_probationes.sh`: add `md_html.c` to the md source list (briar already links `md_stml`, `stml_macros`, `stml_html` — `md_html` is the composer over them) and `probatio_briar_facies` to the gate list. `pythonica/silva.py`: the four tables (`PORTAE`, `FORMAE`, mensurae prefix, build-dir map — the rows plan 4 added for `contextus` are the model). Run `./briar/compile_probationes.sh` and confirm the runner *sees* a missing test rather than silently skipping (exit 2 = nothing ran).

- [ ] **Step 2: write the failing gate** `probatio_briar_facies.c`. Helpers copied from `probatio_briar_fabrica.c`: `_plagulam_legere(via)`, and `_fabricare(via)` which parses, `briar_nexus_texere`, `briar_contexere`, `briar_silvam_texere`, `briar_fabricare` and hands back all four pieces. Plus two of this gate's own:

```c
/* omnis '<!--@x-->' in involucro consumptus? */
interior b32 _notae_consumptae (chorda pagina);

/* invariantia structuralia per html_lexema (NON per grep):
 *  - omne id="l<N>" unicum
 *  - omne href="#frag-x" ad id="frag-x" emissum ducit
 *  - omnis data-s in insula symbolorum adest
 *  - numerus <details> == numerus radicum
 * redde numerum invariantium probatorum (numquam 0 tacite) */
interior i32 _invariantia_probare (chorda pagina, i32 radices);
```

  Assertions for `salve.thistle`: the page is non-empty and begins
  `<!DOCTYPE html>`; `_notae_consumptae` VERUM; head matter contains
  the source path and briar's version; every prose paragraph reached
  the page (count `<p` against the fixture's paragraphs); every line
  of the C region carries an `id="l<N>"` with N its thistle line, the
  first and last pinned by number; `_invariantia_probare` returns 4;
  the whole page byte-compared to `fixa/facies/salve.html`, written
  only under `BRIAR_FACIES_SCRIBERE=1`.

- [ ] **Step 3: run red** `./briar/compile_probationes.sh facies` — a compile failure (no header yet). That is the red.

- [ ] **Step 4: write the unit.** The spine:

```c
/* octetos evadere: & < > " (et ' in attributis) */
interior vacuum
_evadere (ChordaAedificator* a, chorda t);

/* lineas textus emittere, unaquaeque cum id sua .thistle linea;
 * tabula = Xar de i32 (contextus) aut NIHIL (linea_prima + k) */
interior vacuum
_lineas_emittere (ChordaAedificator* a, chorda textus,
    constans Xar* tabula, i32 linea_prima, b32 numeri);

/* prosa: bytes partis -> md_html_reddere -> fragmentum HTML.
 * Vitium in md = paragraphus vitii, NUMQUAM pagina vacua. */
interior vacuum
_prosam_emittere (Piscina* p, InternamentumChorda* in,
    ChordaAedificator* a, constans BriarNexusRes* r, chorda exemplar);
```

  `_prosam_emittere` takes the part's bytes exactly as
  `briar_nexus.c` does — `briar_lexema(r->pars, BRIAR_PROSA_TOK)->valor`
  for prose, `r->contentum` for an `<md!>` region — and calls
  `md_html_reddere(p, in, fons, mensura, vestis->exemplar)`, so there
  is one markdown→HTML path in the house. The prose is re-parsed
  rather than reusing `r->arbor_md`; md_html's API takes text, the
  cost is microseconds, and a second markdown renderer is the thing
  worth avoiding.

  In Task 1 the C regions emit as escaped `<pre>` with per-line ids
  and no colour, no fragment links, no details — those are Tasks 2
  and 3. The head matter and the foot inventory (fragments, derived
  includes, closure from `fructus->clausura`) land here.

- [ ] **Step 5: run green** `./briar/compile_probationes.sh facies`; write the golden with `BRIAR_FACIES_SCRIBERE=1`, **open it in a browser and look at it**, then run again without the variable.

- [ ] **Step 6: plant** — remove `<` from `_evadere`'s set. The gate must go red on the golden *and* on a direct escaping assertion (add one: a fixture line containing `<` renders as `&lt;`). Restore; `grep -c` the plant = 0; green again.

- [ ] **Step 7: format + commit.** `./silva/formator.sh briar/fontes/briar_facies.c briar/fontes/briar_facies.h briar/probationes/probatio_briar_facies.c -scribere` then `-vitia` = CONFORMIS; `./oratio/vocabula.sh -machina` delta 0; `silva.commissio_umbra` with the created files, the golden, and the two registration edits, portae `['briar', 'pythonica']`.

---

### Task 2: colour and the fragment graph

**Files:** modify `briar/fontes/briar_facies.c`, `briar/probationes/probatio_briar_facies.c`; create `briar/probationes/fixa/facies/fragmenta.html`, `briar/probationes/fixa/thistle/facies_utf8.thistle`.

**Consumes:** Task 1's emitter. **Produces:** the coloured, linked body every later task decorates.

- [ ] **Step 1: failing assertions.** Section "color" over `salve.thistle`: the `#include` line carries `class="ca-sy-praeprocessor"`; a `"salve, munde"` literal carries `ca-sy-chorda`; `redde` carries `ca-sy-clavis`; `i32` carries `ca-sy-genus`; a comment spanning two lines carries `ca-sy-commentarium` on **both** lines (the cover must be split, not dropped). Section "fragmenta" over the line-pinned `fragmenta.thistle`: a section `id="frag-incrementum"` exists; its header names use site `15` and no other; `#salutatio` names **two** use sites; `#otiosum` says `non adhibitum`; the transclusion line at 15 contains `href="#frag-incrementum"`; `<details>` count == 2 (app root and probatio root); inside the app root's details, the gutter of the woven `summa = summa + numeri[i];` line reads **9**, not its composed index. Byte golden `fixa/facies/fragmenta.html`. UTF-8: `facies_utf8.thistle` carries a C region with a comment containing `æ`; assert the two bytes of `æ` are adjacent in the output with no tag between them.

- [ ] **Step 2: run red.**

- [ ] **Step 3: implement.** Colour, carried from laboratorium experiment `0023-color-syntaxis/pipa.c` (`../silicetum/laboratorium/assets/experimenta/`): `_classis(SilvaToken*)` and the three tables `_claves_latinae`, `_genera_nota`, `_constantia_nota`, verbatim in policy. Adapt `_tectum_addere`/`_extensionem_addere` into a per-line cover table over the region text, then emit line by line, escaping between covers. Take the **policy and the cover computation**, not the JSON transport — the page emits its own spans.

```c
/* tecta = Xar (per lineam) de Xar de BriarTectum {ab, ad, cl} */
interior Xar*
_tecta_computare (Piscina* p, chorda textus);

/* lineam cum tectis emittere: octeti evasi, tecta in <span> */
interior vacuum
_lineam_coloratam_emittere (ChordaAedificator* a, chorda linea,
    constans Xar* tecta_lineae);
```

  Fragments: a `<section id="frag-<id>">` per `BriarFragmentum`, its
  header rendering `usus` as links (`non adhibitum` when empty).
  Transclusion lines: re-read with
  `briar_contextus_lineam_legere(linea, &id, &praefixum)` — the same
  reader the weave uses, so the page and the compiler agree on what a
  reference is — and emit the prefix, then a link. Roots: as-written
  text, then `<details><summary>contextum (N lineae)</summary>` over
  `r->contextus` with the gutter read from `r->lineae`.

- [ ] **Step 4: run green**; write both goldens; **look at `fragmenta.html` in a browser** — click a reference, click a backlink, open a disclosure; rerun without the variable.

- [ ] **Step 5: the agreement assertion.** Add a test that renders `fragmenta.thistle` and, from the same `fragmenta` Xar, builds the `-partes` answer, then asserts the page names exactly the same fragments with exactly the same use sites. One truth, two renderings; a divergence names which is lying.

- [ ] **Step 6: plant** — make the gutter emit the composed index instead of `*(i32*)xar_obtinere(r->lineae, k)`. The gate must redden on the gutter assertion and the golden. Restore; `grep -c` = 0; green.

- [ ] **Step 7: format + commit** (portae `['briar', 'pythonica']`).

---

### Task 3: symbols, refusals, escaping

**Files:** modify `briar/fontes/briar_facies.c`, `briar/probationes/probatio_briar_facies.c`; create `briar/probationes/fixa/thistle/facies_evasio.thistle` and six goldens under `fixa/facies/adversa/`.

- [ ] **Step 1: failing assertions.** Symbols: in `salve.thistle`, a use of a name the file defines links to its definition line (`href="#l<N>"`, no `data-s`); `piscina_generare_dynamicum` carries `data-s="piscina_generare_dynamicum"` and the island holds `{"caput":"piscina.h","decl":…}` for it; every `data-s` in the page is a key of the island (this is invariant 3 from Task 1, now non-vacuous — assert the island is non-empty first, since "all zero of them matched" passes silently). Refusals: each of the six `adversa/` fixtures renders a page (never empty), the page contains the fixture's exact refusal text from spec §3.4, and that text sits in an `<aside>` whose line matches the fixture's known error line; goldens under `fixa/facies/adversa/`. Escaping: `facies_evasio.thistle` carries `<script>alert(1)</script>` and `&amp;` and `"` in **prose** and a literal `</script` inside a C string; assert no unescaped `<script` survives outside the chrome's own tag and that the island's `</script` is escaped.

- [ ] **Step 2: run red.**

- [ ] **Step 3: implement.** Symbols come from the region's `SilvaSemantica` — the table `_methodum_probare` walks in `briar_fabrica.c`, which holds header declarations because the closure's header text is parsed to type-check the region. Own-file symbols (depth 0, defined here) become `<a href="#l<N>">`; the rest, if found in the semantica with a header, become `data-s` plus an island entry with `caput` and the declaration text. A symbol in neither set gets no decoration — no guessing. Refusals: walk the nexus for `linea_erroris > 0` with its `causa`, plus `fructus->causa` / `linea_causae` when `!fructus->successus`, and emit each as an `<aside>` anchored at its line; a region with `linea_erroris` shows its as-written text and the cause where the `<details>` would stand.

- [ ] **Step 4: run green**; write the goldens; **look at one refusal page in a browser**; rerun without the variable.

- [ ] **Step 5: plant** — stop escaping inside the JSON island. The escaping assertion must redden. Restore; `grep -c` = 0; green.

- [ ] **Step 6: format + commit** (portae `['briar', 'pythonica']`).

---

### Task 4: the chrome, the capsula, the flag, the fumus, the seal

**Files:** create `briar/facies/facies.html`, `.css`, `.js`, `tools/briar_facies_capsula.sh`; modify `tools/briar_struere.sh`, `briar/fontes/briar_imperium.h/.c`, `tools/briar.c`, `tools/briar_fumus.sh`, docs.

- [ ] **Step 1: the chrome.** `facies.html` — the skeleton with the six markers, `<meta charset="utf-8">`, and nothing else. `facies.css` — the document column, the code blocks, the six `ca-sy-*` colour variables carried from experiment 0023, the fragment section headers, the gutter, the `<aside>` margin, the panel. `facies.js` — **one feature only**: click a `[data-s]`, read the island, show the declaration beside the text, dismiss. Hand-written, no framework, nothing fetched. Keep it under ~120 lines; if it grows past that, something belongs in markup instead.

- [ ] **Step 2: the capsula.** `tools/briar_facies_capsula.sh` on the model of `tools/corpus_infixum.sh`: a generated toml listing `briar/facies/*` and `md/html/md-html.stml`, `_compress = false`, through `bin/capsula_generare` into `build/capsula_facies_briar.c`, with a freshness check against those four files. `tools/briar_struere.sh` sources it and links the object. The consumer in `tools/briar.c` declares `externus constans CapsulaEmbed capsula_facies_briar;` **directly** — never including the generated header (speculum's rule: silva cannot resolve a quoted include containing `..`, board nota 01KY0T6T64) — and fills a `BriarVestis` with `capsula_legere_chorda`.

- [ ] **Step 3: the flag.** `briar_imperium.h/.c`: `BRIAR_ACTIO_HTML`, `-html` recognized before the file and as the shebang form's first argument, added to the refusal message's list; extend `probatio_briar_imperium` (it pins the flag set — that gate will go red until it is updated, which is the correct kind of red). `tools/briar.c`: the action runs the fabrica exactly as `-amalgama` does, calls `briar_faciem_fingere`, writes `<t>.html` beside the thistle, prints the path. **It opens nothing.**

- [ ] **Step 4: rebuild and use it.** `./briar/compile_probationes.sh registrum && ./tools/briar_struere.sh`; then `./project-specs/exempla/salutatio2.thistle -html` and **open the result in a browser and read the file through it**. This is the acceptance test the suite cannot perform: does the page make `salutatio2.thistle` easier to read than the source does? Record what you changed after looking.

- [ ] **Step 5: fumus stage XI.** Copy `salutatio2.thistle` into the area; `-html` writes the file; assert it is non-empty, contains `id="frag-principale"`, and that `#repositorium` names **two** use sites (that fragment is deliberately shared between the program and its probatio — the one place the fixture proves the backlink is a set, not a scalar). Then `-html` over `adversa/fragmentum_circulus.thistle`: the page is still written and contains `circulus`. Renumber `-agere`. Run `./tools/briar_fumus.sh` → `fumus briar: sanum`.

- [ ] **Step 6: docs.** `briar/CLAUDE.md` (map: `briar_facies`; laws: the page reads without JavaScript, the chrome is embedded, goldens use test chrome; gates fourteen). `FONTES.md` (the two new fixtures + the golden directories). Worklog entry: what the page taught, the size of a rendered `salutatio2.html`, anything the browser showed that the gate could not. Spec §4.6 gains an "as built" paragraph with the deviations, §8 P6 DONE. Memory `briar-project.md` + the MEMORY.md briar line. Ledger nota through the main tree's resident.

- [ ] **Step 7: seal.** `formator -scribere` then `-vitia` CONFORMIS on every touched C file; `./oratio/vocabula.sh -machina` delta 0; `silva.commissio_umbra` with portae `['briar', 'briar-fumus', 'pythonica']`; rebake `~/.bin/briar`; run `./tools/briar_fumus.sh` once more from the installed binary.

---

## Self-review

**Spec coverage.** §4.6's F1 (a self-contained file, nothing opened) T1+T4 · F2 (document spine, panel visits) T1 body + T4 chrome · F3 (embedded chrome, generated body, JSON island) T1 `BriarVestis` + T3 island + T4 capsula · F4 (always render, refusals pinned) T3 · F5 (own symbols + library declarations) T3. The law that the page reads without JavaScript: T1–T3 emit anchors only, T4 adds one script for one feature. Escaping and UTF-8: T1 plant, T2 UTF-8 fixture, T3 escaping fixture. Both gate layers: invariants T1 (`_invariantia_probare`), goldens T1–T3. The `-partes` agreement: T2 step 5. Fumus XI: T4. §5's flag row: T4 step 3. §6's gate row: T1 step 1. §7's module row: T1. §8's P6: T4 step 6.

**Refinement made while planning, already folded into the spec:**
§4.6's signature first showed no chrome parameter. Passing the chrome
in as `BriarVestis` is strictly better — the gate feeds fixed test
chrome, so editing the real CSS cannot move a byte golden, and the
module keeps its "no I/O" property. §4.6 now carries the parameter.

**Named risks.** (1) `md_html_reddere` is new to briar's link — if it
drags an unexpected dependency, Task 1 finds out at step 1, before any
design rests on it. (2) The six `adversa` goldens are the most
brittle artifacts here; they exist to prove F4 and should be read, not
regenerated. (3) `probatio_briar_imperium` pins the flag set and will
redden in Task 4 step 3 — expected, and the plan says so, so nobody
"fixes" it by widening the pin without reading it.
