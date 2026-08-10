# Necessitudines Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** First-class relation kinds for natura — `<necessitudo>` declarations, `<termini>` frames, name-based binding on `<relatio>` slots — per project-specs/necessitudines-spec.md.

**Architecture:** Grammar first (natura.canon), then loader (structs → binding pass → apparatus), then the projection + differential chain, then tools, then the core module and exemplar migrations. Byte-identity of generated canons is the regression spine throughout.

**Tech Stack:** C89 (latina.h), STML, canon, house shell tools. No new libraries.

## Global Constraints

- C89, Latin identifiers/comments/messages; latina.h macro words are FORBIDDEN identifiers (nomen, casus, per, magnitudo, structura, registrum…).
- Flags: `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings` (via tools/natura_struere.sh).
- chorda is NOT null-terminated → compare with `chorda_aequalis_literis`, never strcmp on `->datum`.
- Suite: `./compile_tests.sh natura` (4 files). Build binaries: `./tools/natura_struere.sh`. Exit 2 = NOTHING RAN, never health. Never gate on piped `$?`.
- Cold `./silva/examen.sh <file>` is the authority after edits (hooks race; legati header cache lies).
- Pre-fix-oracle law: every gate proven RED with a planted fault before trusting green.
- NO expression language in canon (house law). TSV formats are APPEND-ONLY (consumers assert on existing columns).
- Byte-gate: `./tools/natura_canones.sh -probare` must exit 0 with no `.genera` edited, at every task boundary until Task 7 (which owns the expected diffs).
- Commits end with `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`. Never stage FAQ.md or gesta/annales/* — stage by explicit path.

## New rule numbers (extend METAMODULUS §8 in Task 8)

| Regula | Gradus | What |
|---|---|---|
| XX | MONITUM | per-model aggregate: N slot names bind to no necessitudo (top names + count) |
| XXI | VULNUS | explicit `necessitudo=` citation unresolvable; ambiguous spelling (one name → two declarations) |
| XXII | VULNUS | end-constraint: site `ad=` outside declared end; child necessitudo widens parent's `a=`/`ad=` |
| XXIII | VULNUS | duplicate `munus=` within one frame |
| XXIV | VULNUS | (reserved, unused v1) |
| XXV | VULNUS | model declares neither genus nor necessitudo (replaces canon `minimum="1"`) |

necessitudo `sub=` resolution failures reuse **rule IV** (same meaning as genus sub=).

## File map

- `natura/natura.canon` — grammar (Task 1)
- `natura/METAMODULUS.md` — §3 regenerated (Task 1), §8 + prose (Task 8)
- `include/natura.h`, `lib/natura.c` — structs, parsing, binding, apparatus (Tasks 2-4)
- `tools/natura_examen.c` — -corpus kind table, -tabulae, summary (Tasks 4, 5b)
- `tools/natura_canones.c` — fold: munus= + terminus arm (Task 4)
- `tools/natura_apparatus_umbra.sh` — fifth pass (Task 4)
- `tools/natura_quaesitor.c` — quaere/apparatus/relationes/census (Task 5a)
- `tools/natura_visus.sh` — INDEX section, shadow classifier (Task 5b)
- `natura/necessitudines.genera` — NEW core module (Task 6)
- `natura/commercium.genera`, `natura/animal.genera`, `natura/planta.genera`, + the two standing-vulnus files (pharmacon/vectura area — read at execution) (Task 7)
- `probationes/probatio_natura.c`, `probatio_natura_canones.c`, `probatio_natura_quaesitor.c` — tests throughout
- `natura/natura.worklog.md`, memory, ledger (Task 8)

---

### Task 1: Grammar — natura.canon + METAMODULUS §3

**Files:** Modify `natura/natura.canon` (root :33-39, genus :59-82, relatio :184-195, unicitas :279-283, + new blocks); regenerate `natura/METAMODULUS.md` §3.
**Interfaces — Produces:** the closed vocabulary every later task writes against.

- [ ] **Step 1: Red oracle.** Write the frames fixture to the scratchpad (reuse the explorer's `frames.genera` if present, else):

```xml
<?xml version="1.0" encoding="UTF-8"?>
<natura modulus="probatum_necessitudines" versio="0">
<necessitudo nomen="continet" conversum="continetur_in" a="*" ad="*">
  <definitio>Totum quod partem intra se habet.</definitio>
  <scriptio>contenta_in</scriptio>
</necessitudo>
<genus nomen="actus_probandus">
  <definitio>Actus fictus probationis.</definitio>
  <termini>
    <terminus munus="agens_probans" necessitudo="continet" ad="actus_probandus"/>
  </termini>
  <relationes>
    <relatio nomen="continet" ad="actus_probandus" necessitudo="continet"/>
  </relationes>
</genus>
</natura>
```

Run `bin/canon_examen <fixture>` — expect VITIA (liberum illicitum: necessitudo/termini; attributum ignotum: necessitudo on relatio). Record the count.

- [ ] **Step 2: Edit natura.canon.**
  - Root (:38-39): add `<liberum nomen="necessitudo"/>`; change `<liberum nomen="genus" minimum="1"/>` → `<liberum nomen="genus"/>` with `nota="minimum sublatum 2026-08-10: modulus sine genere licet si necessitudines fert - regula XXV (co-occurrentia supra canonem, ut 'ad AUT a')"`.
  - `<genus>` liberi: add `<liberum nomen="termini" maximum="1"/>`.
  - `<relatio>`: add `<attributum nomen="necessitudo" genus="nomen"/>`.
  - New blocks (after relatio, before dictionarium):

```xml
  <elementum nomen="necessitudo">
    <attributum nomen="nomen"     genus="nomen" necessarium="verum"/>
    <attributum nomen="conversum" genus="nomen"/>
    <attributum nomen="sub"       genus="nomen"/>
    <attributum nomen="modulus"   genus="nomen"/>
    <attributum nomen="a"         genus="textus"
      nota="finis gerentis: quod genus eam ferre potest ('*' apertum)"/>
    <attributum nomen="ad"        genus="textus"
      nota="finis petiti ('*' apertum)"/>
    <attributum nomen="nota"      genus="textus"/>
    <liberum nomen="definitio" maximum="1"/>
    <liberum nomen="glossa"/>
    <liberum nomen="scriptio"/>
    <liberum nomen="nota"/>
    <liberum nomen="dubium"/>
  </elementum>

  <elementum nomen="scriptio" textus="verum"
    nota="scriptura altera eiusdem necessitudinis (concordantia
          grammatica: situm/sita/situs_in) - identitas una"/>

  <elementum nomen="termini">
    <liberum nomen="terminus" minimum="1"/>
    <liberum nomen="nota"/>
  </elementum>

  <elementum nomen="terminus">
    <attributum nomen="munus"       genus="nomen" necessarium="verum"/>
    <attributum nomen="necessitudo" genus="nomen"/>
    <attributum nomen="ad"          genus="textus"/>
    <attributum nomen="modulus"     genus="nomen"/>
    <attributum nomen="multiplex"   genus="veritas"/>
    <attributum nomen="necessaria"  genus="veritas"
      nota="apparatus solum - proiectio eam numquam legit
            (necessitas ontologica obligatio documenti non est)"/>
    <attributum nomen="nota"        genus="textus"/>
    <liberum nomen="nota"/>
  </elementum>
```

  - Unicitas (:279-283): `super="genus species individuum cultivar"` → `super="genus species individuum cultivar necessitudo"`.

- [ ] **Step 3: Green.** `bin/canon_examen -canon canon.canon natura/natura.canon` → VITIA 0. `bin/canon_examen <fixture>` → VITIA 0.
- [ ] **Step 4:** `./tools/natura_metamodulus_generare.sh` then `-probare` → exit 0. `./tools/natura_canones.sh -probare` → exit 0 (no .genera touched). `bin/natura_examen` → same 2 VULNERA as baseline, no new.
- [ ] **Step 5: Commit** `natura: canon - vocabularium necessitudinum (necessitudo/scriptio/termini/terminus, regula XXV parata)`.

---

### Task 2: Loader — structs, parsing, registration, rules XXV + II-discriminant

**Files:** Modify `include/natura.h` (:99-102 discrimen, :104-127 bibliotheca, prototypes), `lib/natura.c` (arborem_legere :254 area, natura_legere :427 area, rule II :800), `probationes/probatio_natura.c`.
**Interfaces — Produces:**

```c
/* natura.h */
nomen structura NaturaNecessitudo NaturaNecessitudo;
structura NaturaNecessitudo {
              chorda*  titulus;    /* nomen= (internatum) */
              chorda*  conversum;  /* conversum= aut NIHIL */
              chorda*  modulus;
    NaturaNecessitudo*  parens;    /* sub= resolutum (nectere) */
           StmlNodus*  nodus;      /* a=/ad=/scriptiones hinc */
};
/* NaturaEnsDiscrimen += NATURA_ENS_NECESSITUDO = III */
/* NaturaBibliotheca += Xar* necessitudines_omnes;
                        TabulaDispersa* scriptiones;  (nectere aedificat) */
NaturaNecessitudo* natura_necessitudo(NaturaBibliotheca* bib,
                                      constans character* titulus);
```

- [ ] **Step 1: Failing test** (probatio_natura.c, new section + fixture):

```c
interior constans character* FIXTURA_NECESSITUDINES =
"<natura modulus=\"nexus_probandus\" versio=\"0\">\n"
"<necessitudo nomen=\"continet\" conversum=\"continetur_in\""
" a=\"*\" ad=\"*\">\n"
"  <definitio>Totum partem intra se habet.</definitio>\n"
"  <scriptio>contenta_in</scriptio>\n"
"</necessitudo>\n"
"<necessitudo nomen=\"pars_de\" sub=\"continet\" a=\"*\" ad=\"*\"/>\n"
"</natura>\n";
```

```c
CREDO_VERUM (natura_legere(bib,
    chorda_ex_literis(FIXTURA_NECESSITUDINES, piscina),
    "nexus_probandus"));
vulnera = natura_nectere(bib);
CREDO_AEQUALIS_I32 ((i32)xar_numerus(bib->necessitudines_omnes), II);
CREDO_NON_NIHIL (natura_necessitudo(bib, "continet"));
CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXV), ZEPHYRUM);
```

Plus rule XXV red case: a model that is `"<natura modulus=\"vacuus\" versio=\"0\"><nota>nihil</nota></natura>"` → `vulnera_regulae(bib, XXV) == I`. Plus discriminant hole: a genus with `<relatio nomen="x" ad="continet"/>` in the necessitudines fixture's module → rule II vulnus (target is a necessitudo, not an ens).

- [ ] **Step 2:** Run `./compile_tests.sh natura` → FAIL (fields/functions absent = compile error; that is the red).
- [ ] **Step 3: Implement.**
  - natura.h edits per Produces block above.
  - `natura_bibliotheca_creare`: allocate the new Xar (scriptiones map stays NIHIL until nectere).
  - `arborem_legere` (:254 area): branch `alioquin si (chorda_aequalis_literis(*titulus_e, "necessitudo"))` — build NaturaNecessitudo (titulus/conversum internated via the same intern the genus branch uses; conversum NIHIL when attribute absent), push to `necessitudines_omnes`, register via `ens_registrare` with discrimen III (buys entia + nomina + homonym sentinel). Do NOT recurse into it as ambient context.
  - Rule XXV: in `natura_legere`, snapshot `xar_numerus(bib->genera_omnia)` + `xar_numerus(bib->necessitudines_omnes)` before `arborem_legere`; both unchanged after → `diagnosticum_addere(… VULNUS, XXV, ex->stirps, NIHIL, "modulus nec genus nec necessitudinem declarat")`.
  - Rule II discriminant (:800 block): where `scopus` resolves, add `|| ((NaturaEns*)scopus)->discrimen == NATURA_ENS_NECESSITUDO` to the failure condition, message `"ad= necessitudinem nominat, non genus"`. Same check in the relatum branch (:848) and verify the sub= pass (:998) and proprietas branch (:737) already discriminate (they do — leave).
  - `natura_necessitudo`: v1 resolves via `bib->nomina` requiring discrimen III (spellings arrive Task 3).
- [ ] **Step 4:** `./silva/examen.sh lib/natura.c` + `include/natura.h` → ACCIPE. `./compile_tests.sh natura` → PASS. `bin/natura_examen` after `./tools/natura_struere.sh` → baseline unchanged (2 VULNERA).
- [ ] **Step 5: Commit** `natura: onerator - necessitudo legitur et registratur (discrimen III, regula XXV, foramen II clausum)`.

---

### Task 3: Binding — spelling index, defaulting, rules XX/XXI/XXII/XXIII, relatum-over-termini

**Files:** Modify `lib/natura.c` (natura_nectere :1041-1114, arborem_nectere relatio branch :755-807, relatum :808-858, in_catena_invenire :438-520), `include/natura.h`, `probationes/probatio_natura.c`.
**Interfaces — Produces:** `natura_necessitudo()` now resolves nomen/conversum/scriptio (one identity, any spelling); `bib->scriptiones` = TabulaDispersa spelling→NaturaNecessitudo*, built in nectere; `in_catena_invenire`/`in_membris_invenire` gain a `clavis` parameter (the key attribute: "nomen" or "munus").

- [ ] **Step 1: Failing tests** (extend the fixture: a genus bearing bound/unbound/broken slots and a frame):

```xml
<genus nomen="actus_probandus">
  <definitio>Actus fictus.</definitio>
  <termini>
    <terminus munus="possessor_primus" necessitudo="continet" ad="actus_probandus"/>
    <terminus munus="possessor_alter"  necessitudo="continet" ad="actus_probandus"/>
  </termini>
  <relationes>
    <relatio nomen="pars_de"     ad="actus_probandus"/>
    <relatio nomen="contenta_in" ad="actus_probandus"/>
    <relatio nomen="ignotum_x"   ad="actus_probandus"/>
    <relatio nomen="y" necessitudo="non_exstat" ad="actus_probandus"/>
  </relationes>
</genus>
```

```c
/* contenta_in (scriptio) ligatur ad continet */
CREDO_NON_NIHIL (natura_necessitudo(bib, "contenta_in"));
CREDO_VERUM (natura_necessitudo(bib, "contenta_in")
          == natura_necessitudo(bib, "continet"));
CREDO_NON_NIHIL (natura_necessitudo(bib, "continetur_in"));
/* XXI: citatio fracta */
CREDO_AEQUALIS_I32 (vulnera_regulae(bib, XXI), I);
/* XX: unum nomen insolutum (ignotum_x) -> monitum aggregatum I */
CREDO_AEQUALIS_I32 (monita_regulae(bib, XX), I);
```

Separate fixtures for: duplicate munus (`XXIII == I`); end-violation — `continet` narrowed `ad="actus_probandus"` in a child necessitudo, site cites it with an `ad=` outside → `XXII == I`; widening — child necessitudo `sub=` with `ad="*"` under a parent with `ad="actus_probandus"` → `XXII == I`; a `<relatum nomen="possessor_primus" …/>` on a species of actus_probandus → `vulnera_regulae(bib, XI) == ZEPHYRUM` (relatum finds termini). Promote the inline monita loop (:509-528) to a `monita_regulae` helper, twin of `vulnera_regulae`.

- [ ] **Step 2:** Run → FAIL (monita_regulae undefined; then assertion reds).
- [ ] **Step 3: Implement.**
  - New nectere pass between :1041 and :1044: for each necessitudo — resolve `sub=` (entia lookup, discrimen III required, else VULNUS IV); build `bib->scriptiones`: insert titulus, conversum, every `<scriptio>` child text (via `stml_textus_internus` + squeeze, the examen pattern); insertion collision within the necessitudo layer → VULNUS XXI ("scriptura ambigua") + sentinel so lookups return NIHIL. Widening check: child a=/ad= vs nearest ancestor's non-`*` constraint — child `"*"` under bounded parent, or child target not subsumed by parent target (genus parens walk) → VULNUS XXII.
  - `natura_necessitudo` reads `bib->scriptiones` (sentinel-aware).
  - `in_catena_invenire` + `in_membris_invenire`: add `constans character* clavis` parameter (existing call sites pass `"nomen"`); relatum (rule XI) tries `("relationes","relatio","nomen")` then `("termini","terminus","munus")`.
  - relatio branch, ABOVE the inversa short-circuit (:761): resolve the slot — explicit `necessitudo=` via natura_necessitudo (NIHIL → VULNUS XXI); else `nomen=` via natura_necessitudo; bound + site has `ad=` not `"*"` → end check (direction: if the matched spelling equals the declaration's conversum, the effective end constraint is the declaration's `a=`, else its `ad=`; site target resolved via ens_quaerere, subsumption by genus-parens walk; violation → VULNUS XXII); unbound → append the name to a per-model Xar (dedup by name).
  - termini walk: in arborem_nectere's genus branch, on meeting `<termini>`: n² duplicate-munus check (VULNUS XXIII), then per `<terminus>` the same binding/end logic keyed on `munus=`/`necessitudo=`.
  - Monitum XX: after each model's `arborem_nectere` call in natura_nectere (:1107-1114), if the model's unbound list is non-empty: build message with chorda_aedificator — `"nomina relationum insoluta N: a, b, c, d, e et M alia"` (cap V names) — `chorda_ut_cstr` it, add MONITUM XX with `modulus = ex->stirps` (the -plagula trap: never NIHIL).
- [ ] **Step 4:** silva examen ACCIPE; `./compile_tests.sh natura` → PASS; `bin/natura_examen` on corpus → baseline 2 VULNERA + new MONITA (rule XX per model; expected — nothing is declared yet, every model with relations gains one aggregate monitum). Record the count in the commit message.
- [ ] **Step 5: Commit** `natura: ligatio necessitudinum (scripturae/conversa/praesumptio, regulae XX-XXIII, relatum per terminos)`.

---

### Task 4: Apparatus chain — termini through loader, projection, differential

**Files:** Modify `lib/natura.c:1289-1293`, `tools/natura_canones.c` (:1222, :1293), `tools/natura_examen.c` (-corpus kind table :820-821, :856-861, :981), `tools/natura_apparatus_umbra.sh` (:27-34), `probationes/probatio_natura.c`, `probationes/probatio_natura_canones.c`.
**Interfaces — Consumes:** binding from Task 3. **Produces:** termini visible in natura_apparatus(); terminus projects as NC_ORIGO_RELATIO/referentia keyed by munus=.

- [ ] **Step 1: Failing apparatus test** (probatio_natura.c): apparatus of actus_probandus contains the two terminus members (walk the Xar counting nodes whose titulus == "terminus"); a sub-genus of actus_probandus inherits them (ancestor walk).
- [ ] **Step 2:** Run → FAIL (termini not in whitelist → count 0).
- [ ] **Step 3:** lib/natura.c:1289-1293 — extend the disjunction with `|| chorda_aequalis_literis(*titulus_e, "termini")`. Tests PASS.
- [ ] **Step 4: Planted fault for the silent drop** (probatio_natura_canones.c): cook a fixture module whose genus holds ONLY `<termini>` (one terminus, munus="probandus_finis", ad= a fixture genus). Assert the generated canon CONTAINS `probandus-finis` (kebab) with `genus="referentia"`. Run → FAIL two ways in sequence: first the terminus is dropped (no titulus at natura_canones.c:1266) — this is the measured silent-drop, now red in a test.
- [ ] **Step 5:** tools/natura_canones.c:
  - :1222 area: `titulus = stml_attributum_capere(am->nodus, chorda_aequalis_literis(*am->nodus->titulus, "terminus") ? "munus" : "nomen");`
  - :1293: `alioquin si (chorda_aequalis_literis(*am->nodus->titulus, "relatio") || chorda_aequalis_literis(*am->nodus->titulus, "terminus"))` — the branch body (origo RELATIO, referentia, _petitum_citabile) is unchanged; `_petitum_citabile` reads `ad=`/`modulus=` which terminus carries identically.
  - Run Step 4's test → PASS.
- [ ] **Step 6:** tools/natura_examen.c -corpus: array dims :820-821 IV→V; kind table :856-861 gains `invol[IV] = "termini"; membra[IV] = "terminus";`; loop bound :861 IV→V; the member-name read in that walk keys on munus for terminus (same conditional as canones). Row letter: emit `T` (:981 area writes the letter per kind — follow the existing R pattern). -tabulae relation-arc walk (:500-586): add terminus alongside relatio/relatum (reads munus= as name, ad= as target).
- [ ] **Step 7:** tools/natura_apparatus_umbra.sh :27-34: fifth awk pass emitting `T` rows (copy the R pass at :33, container "termini", member "terminus", key munus). Run `./tools/natura_apparatus_umbra.sh` → zero diff (corpus has no termini yet — trivially green; the venditio migration in Task 7 is its real exercise).
- [ ] **Step 8:** `./tools/natura_canones.sh -probare` → exit 0 (no .genera edited — byte-gate holds through the fold change; this is the (i) result from the exploration, now verified). Suite PASS.
- [ ] **Step 9: Commit** `natura: termini per catenam apparatus (onerator + plicatio + corpus + umbra; casus plantatus lapsus taciti)`.

---

### Task 5a: Quaesitor — quaere, apparatus display, relationes, census

**Files:** Modify `tools/natura_quaesitor.c` (:415-506, :515-702, :811-928, :1027-1127, :1191-1293, :1295-1530, :1574-1592), `probationes/probatio_natura_quaesitor.c`.
**Interfaces — Consumes:** `bib->necessitudines_omnes`, `natura_necessitudo()`.

- [ ] **Step 1: Failing tests** (probatio_natura_quaesitor.c drives bin/natura against fixture dirs the way existing tests do):
  - quaere on a declared relation name → SENTENTIA OCCUPATUM (today: LIBERUM — the pollinat blindness, now red in a fixture);
  - quaere on a scriptio spelling → OCCUPATUM; on a conversum → OCCUPATUM;
  - relationes -machina rows carry a 7th column `ligata`/`soluta`; an `a=`-only (inversa) site shows its target in column 6 (a= fallback);
  - census TSV gains `relationes_ligatae` + `relationes_solutae` + `necessitudines` keys (append-only; existing key assertions untouched);
  - apparatus of a frame-bearing genus prints a `TERMINI` block listing each munus.
- [ ] **Step 2:** Run → FAIL per case.
- [ ] **Step 3: Implement.**
  - QuaesitiCongruentia (:415-420): add `NaturaEnsDiscrimen`-style discriminant enum {GENUS, RES, NECESSITUDO} replacing the `si (c->genus)` truthiness at :435/:473; congruentia writer prints `NECESSITUDO      /modulus/nomen   (conversum X, scripturae N)` for the new kind.
  - Fourth walk between :625 and :647: loop `bib->necessitudines_omnes`, score titulus + conversum + every scriptio at PUNCTUM_NOMEN_IPSUM (exact) / PUNCTUM_NOMEN (substring); exact sets `exacta`.
  - LIBERUM prose (:695-696) → `"LIBERUM est (genera, res, necessitudines, umbrae omnia inspecta)."` — update probatio_natura_quaesitor's verdict assertions accordingly.
  - Fix the stray-paren bug (:475-483): move `")"` into the format string before `\n`.
  - `_membrum_scribere` (:811-928): terminus arm beside relatio (:890) reading munus= — use `chorda_aequalis_literis` on the node title; convert the existing `strcmp`-on-`datum` dispatch (:817-818) to `chorda_aequalis_literis` while here (unterminated-chorda hazard, measured). Machina mode (:1039-1066): munus fallback for the name field.
  - `_apparatum_scribere`: `--- TERMINI ---` block between :1124 and :1126, following `_species_scribere`'s shape (:995): one line per terminus member — `munus`, bound identity (via natura_necessitudo on the necessitudo=/munus chain), target, `[necessaria]` when set.
  - relationes (:1228-1293): after the relatio loop (:1291), second `_membra_directa(genus, "termini", "terminus")` pass sharing the row writer (name = munus). Column 6: fall back to `a=` when `ad=` absent. Column 7 appended: `ligata` if explicit necessitudo= resolves or the name resolves via natura_necessitudo, else `soluta` (both formats; human column header updated :1238-1239).
  - census (:1295-1530): `quot_ligatarum`/`quot_solutarum` counters in the existing relation loop (:1341-1371) + termini pass; TSV keys appended after :1486-1487 (`relationes_ligatae`, `relationes_solutae`, `necessitudines`); human lines after :1528.
  - Usage text (:1574-1592): document the 7th column and the necessitudines count.
- [ ] **Step 4:** silva examen ACCIPE; struere; suite PASS; live smoke: `bin/natura quaere pollinat` still LIBERUM (nothing declared yet — becomes OCCUPATUM after Task 6; note in commit).
- [ ] **Step 5: Commit** `natura: quaesitor - ambulatio quarta (necessitudines), termini in apparatu, columna ligata/soluta, census auctus`.

---

### Task 5b: Examen tables + visus INDEX + shadow classifier + summary

**Files:** Modify `tools/natura_examen.c` (-tabulae :311-764, summary :1427-1442), `tools/natura_visus.sh` (:133, :494-615, :500, :618-621).

- [ ] **Step 1:** natura_examen -tabulae: new `necessitudines.txt` (columns `modulus|nomen|conversum|sub|a|ad|scriptiones`, scriptiones space-joined) written beside the existing 13.
- [ ] **Step 2:** Summary (:1436-1440): `exemplaria %u / genera %u / res %u / necessitudines %u / VULNERA %u / MONITA %u` (insert before VULNERA; nothing greps this line positionally — natura-custos greps `^VULNUS` rows, visus greps porta.txt).
- [ ] **Step 3:** natura_visus.sh: new INDEX section between IV (Dictionarium, ends :559) and V (Umbrae, :561) rendering necessitudines.txt as a table (Familia spine visible via the sub column); recipe block I (:506-513) gains the line `bin/natura quaere <nomen>  # necessitudines quoque inspicit`; counts line :500 and terminal report :618-621 gain the necessitudines count; shadow classifier :133 treats citing element `terminus` like `relatio` (genus-wanted agenda).
- [ ] **Step 4:** Run `./tools/natura_visus.sh` → exit 0, INDEX.md contains the new section (empty table until Task 6 — confirm it renders, not errors, on zero rows). All 11 gates green (G5 stays green — canon untouched since Task 1's regen).
- [ ] **Step 5: Commit** `natura: visus/examen - sectio necessitudinum in INDICE, tabula nova, summa aucta`.

---

### Task 6: The core module — natura/necessitudines.genera

**Files:** Create `natura/necessitudines.genera`.
**Interfaces — Consumes:** everything above. The names below were classified from the live corpus this session (432 usages); collision-check EVERY name in Step 1 and adjust on OCCUPATUM (UMBRATUM = fulfil the shadow, proceed).

- [ ] **Step 1:** For each name in the tables below: `bin/natura quaere <n> -machina | tail -1` — record verdicts. Known safe: munus, necessitudo, participatio-family names unchecked — measure, don't assume. Known hazards to AVOID as necessitudo names (live genera): `editio`, `mutatio`, `causa`, `iudicium`, `versio`, `norma`.
- [ ] **Step 2: Write the module.** Root: `<natura modulus="necessitudines" versio="0">`, header prose naming the classification provenance. No `<genus>` (rule XXV satisfied by the declarations). FAMILIA SPINE (all `a="*" ad="*"` unless noted; each with definitio + `<glossa lingua="en">`):

| nomen | conversum | sub | notes |
|---|---|---|---|
| participatio | — | — | standing of a participant in an act; the frame-end family |
| obiectum_actus | — | participatio | the patiens/object end |
| origo | ortum_dat | — | efficient origin |
| promulgatio | — | origo | issuing into public |
| pars_totum | — | — | part-whole |
| situs | — | — | placement |
| dependentia | — | — | depends-on/uses |
| successio | — | — | succession/version/replacement |
| conformitas | — | — | governed-by/conforms/violates axis |
| fundatio | — | — | epistemic grounding/aboutness |
| causalitas | — | — | cause/end (the census spine's family) |
| exsecutio | — | — | performing/running |
| effectio | — | — | change wrought on another |
| translatio_nuntii | — | — | communication/transfer |
| custodia | — | — | keeping/administering |
| repraesentatio | — | — | measure/schema/aboutness-of-data |

WORKHORSES (multi-use names from the corpus; sub= as shown; a=/ad= `"*"` in v1 — narrowing arrives with real need):

| nomen | conversum | sub | scriptiones |
|---|---|---|---|
| de | — | fundatio | |
| continet | continetur_in | pars_totum | |
| pars_de | — | pars_totum | |
| membrum | membrum_de | pars_totum | |
| asseritur_per | — | fundatio | |
| fundat | fundatur_in | fundatio | |
| auctor | — | origo | |
| parit | — | origo | |
| orta_ex | — | origo | ortum_ex |
| edita_a | — | promulgatio | |
| emissa_a | — | promulgatio | |
| lata_a | — | promulgatio | latum_per, latus_a |
| sequitur | praecedens | successio | |
| substituit | — | successio | |
| versio_de | — | successio | |
| habet_versionem | — | successio | |
| normata_per | — | conformitas | |
| secundum_normam | — | conformitas | secundum_legem |
| violat | — | conformitas | |
| implet | impletur_a | conformitas | |
| pendet_ex | — | dependentia | dependet_ex |
| adhibet | — | dependentia | |
| praebet | praebetur_a | dependentia | |
| scripta_in | — | dependentia | |
| intra | — | situs | |
| situm_in | — | situs | sita_in, situs_in |
| exercet | exercetur_a | exsecutio | |
| exsequitur | — | exsecutio | |
| currit_ut | — | exsecutio | |
| efficit | efficitur_a | effectio | |
| afficit | — | effectio | |
| causat | — | causalitas | |
| tendit_ad | — | causalitas | |
| natum_ex | — | causalitas | |
| pertinet_ad | — | fundatio | |
| minatur | — | causalitas | |
| minuitur_per | — | causalitas | |
| pollinat | pollinatur_a | effectio | |
| fert | — | dependentia | |
| constituta_per | — | origo | constituitur_per |
| ex_fonte | — | fundatio | ex_fontibus |
| ex_vitio | — | causalitas | |
| mensuratur_per | — | repraesentatio | |
| mittit | missus_a | translatio_nuntii | |
| accipit | — | translatio_nuntii | |
| custodit | custoditur_ab | custodia | custodita_a, custodita_per |
| tenetur_a | — | custodia | |
| administrat | administrata_a | custodia | |

Worked declaration examples (the pattern for all — definitio Latin, one en glossa):

```xml
<necessitudo nomen="continet" conversum="continetur_in"
    sub="pars_totum" a="*" ad="*">
  <definitio>Totum quod partem intra se habet; conversa via,
    pars quae in toto iacet. Necessitudo generalissima familiae
    pars-totum.</definitio>
  <glossa lingua="en">Containment: the whole holds the part.
    Bound from either direction by name.</glossa>
</necessitudo>

<necessitudo nomen="situm_in" sub="situs" a="*" ad="*">
  <definitio>Res quae in loco posita est. Scripturae genere
    gerentis flectuntur - identitas una.</definitio>
  <glossa lingua="en">Located-in; the three spellings are one
    relation agreeing with the bearer's gender.</glossa>
  <scriptio>sita_in</scriptio>
  <scriptio>situs_in</scriptio>
</necessitudo>

<necessitudo nomen="causat" sub="causalitas" a="*" ad="*">
  <definitio>Quod alterum efficit aut ad esse adducit - spina
    iustificationis censuum (pendet-ex conversa directio eiusdem
    spinae, sed nomen suum servat).</definitio>
  <glossa lingua="en">Causes; the census justification spine's
    forward direction.</glossa>
</necessitudo>
```

(dependet_ex-as-scriptio is hereby the ruling on spec open question 8.2 — identical sense, one identity.)

- [ ] **Step 3:** `bin/natura_examen` → module loads, VULNERA stays 2 (baseline), MONITA (rule XX) count DROPS vs Task 3's recorded count — record before/after in the commit message (this is the burn-down metric's first payment). `./tools/natura_visus.sh` → green, INDEX populated. `./tools/natura_canones.sh -probare` → exit 0 (genus-less module cooks no canon; verify no registry complaint).
- [ ] **Step 4:** Live: `bin/natura quaere pollinat` → OCCUPATUM. `bin/natura quaere sita_in` → OCCUPATUM. `bin/natura census | grep ligat` → nonzero.
- [ ] **Step 5: Commit** `natura: modulus necessitudinum - spina familiarum XVI + nomina multi-usus XLVIII (classificatio 2026-08-10)`.

---

### Task 7: Exemplar migrations — venditio frame, pollinat converse, the two standing vulnera

**Files:** Modify `natura/commercium.genera` (:204 area), `natura/animal.genera` (:94-98), `natura/planta.genera` (pollinatur_a site), the two files behind the standing VULNERA (memory: pharmacon:185, vectura:144 — READ them first), regenerate `natura/cocta/`.

- [ ] **Step 1: venditio.** Replace its four role relatio lines with `<termini>` AT THE EXACT DOCUMENT POSITION the `<relationes>` block occupies, keeping internal order venditor/emptor/res_vendita/solutum, munus= equal to the old nomen=, then the residual `<relationes>` holding `parit` AFTER `<termini>`:

```xml
  <termini>
    <terminus munus="venditor"    necessitudo="participatio" ad="persona"/>
    <terminus munus="emptor"      necessitudo="participatio" ad="persona"/>
    <terminus munus="res_vendita" necessitudo="obiectum_actus" ad="*" multiplex="verum"/>
    <terminus munus="solutum"     necessitudo="obiectum_actus" ad="pecunia" modulus="commercium"/>
  </termini>
  <relationes>
    <relatio nomen="parit" ad="obligatio" modulus="obligatio"/>
  </relationes>
```

(Carry over each old line's exact ad=/modulus=/multiplex=/nota= values from the current file — read them, don't trust this sketch's attribute details.)

- [ ] **Step 2: Byte-gate.** `./tools/natura_canones.sh -probare` → **exit 0**: commercium.canon byte-identical (munus names = old names, position preserved, citation venditio-solutum untouched). If it fires, diff the temp canon — the failure is ordering or a renamed munus, both migration bugs by definition.
- [ ] **Step 3: pollinat.** animal.genera:94-98 → `<relatio nomen="pollinat" ad="planta" modulus="planta" nota="…"/>` (drop a=, drop inversa=); planta's site → `<relatio nomen="pollinatur_a" ad="animal" modulus="animal"/>`. EXPECTED canon diffs for animal/planta (citation closures appear where inversa sites emitted none) — regenerate via `./tools/natura_canones.sh`, eyeball the diff matches exactly that shape, commit regenerated canons.
- [ ] **Step 4: The two standing VULNERA.** Read the two flagged sites (bin/natura_examen names them with lines). Migrate each off the inversa boolean/name conflation onto declared conversum names (declare any needed necessitudo in the core module). `bin/natura_examen` → **VULNERA 0** for the first time. Suite's tolerated-by-name entries for these (semina/probatio, if any) are removed in the same commit so regression teeth return.
- [ ] **Step 5:** `./tools/natura_apparatus_umbra.sh` → zero diff (first real termini exercise). `./compile_tests.sh natura` → PASS. `./tools/natura_visus.sh` → green.
- [ ] **Step 6: Commit** `natura: exemplaria migrata (venditio compages, pollinat conversum, vulnera stantia sanata - corpus SANUM)`.

---

### Task 8: Docs, worklog, memory, ledger, spec corrections

**Files:** Modify `natura/METAMODULUS.md` (§8 rules XX-XXV + a §-level prose subsection on necessitudines/termini/binding), `project-specs/necessitudines-spec.md` (append a "Rulings at build" section: necessaria= apparatus-only; the ordering rule for frame migrations; canon minimum relaxed + rule XXV; dependet_ex ruling), `natura/natura.worklog.md` (dated entry: the classification findings — the 17 families, the five structural discoveries, the numbers — plus the arc record), memory `natura-project.md` + MEMORY.md index line, ledger.

- [ ] **Step 1:** METAMODULUS §8 table rows for XX/XXI/XXII/XXIII/XXV (mark all [E] enforced); prose subsection after the relatio section documenting declaration/binding/frames with one example each. `./tools/natura_metamodulus_generare.sh -probare` still 0 (the §3 table regenerated in Task 1; §8 is hand-prose).
- [ ] **Step 2:** Worklog entry (English allowed) — include the family table and discovery list from the session classification so the analysis survives the scratchpad.
- [ ] **Step 3:** Spec appendix + memory updates.
- [ ] **Step 4:** Ledger: nota on decretum 01KZP4B8KY (arc shipped, numbers before/after: monitum-XX count, VULNERA 2→0); status clausum on 01KZC7EFMR (inversa wound — resolved by conversum mechanism, cite the commit); nota on 01KZMA913V (gaps 1+2 addressed; gaps 3 finis + 4 genus-species remain open).
- [ ] **Step 5:** Final green sweep: `./compile_tests.sh natura`, `bin/natura_examen`, `./tools/natura_visus.sh`, `./tools/natura_canones.sh -probare`, `./tools/canon_coquere.sh -probare`, `./tools/natura_apparatus_umbra.sh`. **Commit** `natura: doctrina necessitudinum in METAMODULUM, worklog, spec-appendix`.

---

## Self-review notes (done at write time)

- Spec §5.1-5.6 all have owning tasks (5.1→1-4, 5.2→6, 5.3→7, 5.4→3, 5.5→5a, 5.6→8). §6 byte-gate → Tasks 4 + 7; planted faults in Tasks 4 (silent drop) and 7 (ordering).
- Types consistent: NaturaNecessitudo/natura_necessitudo/monita_regulae/clavis parameter named identically across tasks.
- Known open risk, accepted: Task 6's names may hit unmeasured collisions — Step 1 measures every one before writing; adjustments are execution-normal, and UMBRATUM fulfils rather than collides.
