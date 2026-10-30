# Chronica Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans
> to implement this plan task-by-task (Fran's house method: inline, one
> task per turn, Fran approves each task's HEADER TEXT before any code).
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `<event year="...">description</event>` written in any
companion note or the common book appears on a master timeline pane in
vicus, a year at a time, with jump-home and person/place pages.

**Architecture:** a pure scanner turns page lines into events
(`chronica_eventus`); two conspectus declarations gather them per book
(`comes/{clavis}/paginae`) and per common-book page (`paginae/{clavis}`),
and `chronica_fontes` unions them into one sorted table, rebuilt when an
auditor reports a stale key; the pane `chronica` (organum manifest,
paginated like navigator) shows years, people and places, and opens
notes through the existing search-result route `scriba/<p>?liber=..`
extended with `linea=`.

**Tech Stack:** C89 in Latin (latina.h), credo tests, conspectus,
scriba_liber, organum/insula manifests, vicus panes, lector_paginare.

**Spec:** `project-specs/chronica-spec.md` (approved by Fran 2026-10-30).

## Global Constraints

- Flags: `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion
  -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
  -Wwrite-strings`.
- Identifiers, comments, messages in Latin; lint
  `VOCABULA_VIAE_ADDITAE="files" ./oratio/vocabula.sh -nova` = nihil novi.
  Never `nomen`, `duplex`, `per`, `casus`, Roman numerals as names.
- Tag and attributes stay ENGLISH as typed (spec XIII.1): `event`,
  `year`, `month`, `day`, `people`, `place`, `type`.
- Lines <= 72 BYTES; `./silva/formator.sh <files> -scribere` clean;
  `./silva/examen.sh <files>` ACCIPE.
- `chorda` is not NUL-terminated; `i32` is UNSIGNED (counts, indices),
  `s32` for years (negative = BC).
- Each task: header text approved by Fran -> tests RED -> implement ->
  GREEN -> plants (`silva.planta`, compiling, backup + `cmp` IDEM) ->
  worklog -> commit ONLY on Fran's word (detached `silva.commissio`).
- New `lib/*.c` linked by vicus: add to
  `./tools/compile_tests_fontes_generare.sh` output AND the hand list
  in `ludus_tessera/compile_probationes.sh`.
- Manifest edits: `./tools/organa_infigere.sh`; conspectus declarations:
  `./tools/conspectus_infigere.sh`.
- Pane hit tests use `des->punctum_locale`; every coordinate feature
  gets one vicus-level click test (memory pane-click-coordinates).
- Year display: "1446 BC", "AD 1274". Year 0 invalid.

## Review Focus

1. **A note edited while the timeline is open** - the timeline shows the
   change on the next paint; a selected event that vanished clears the
   selection; a year left empty moves to the nearest year with events
   (tests in Task 3 and Task 4).
2. **Many books, nothing changed** - repainting must not rebuild the
   table; the second read is RECENS (test in Task 2).
3. **A description longer than a page** - wraps and paginates with the
   lector break rules, never clipped (test in Task 3).
4. **Names with spaces / accents** - "Moses " = "Moses"; "Tomás de
   Aquino" round-trips byte-exact; "moses" is a different person
   (tests in Task 1 and Task 6).
5. **Event text with `<`, `>` or a second `<event` inside** - `<`/`>`
   inside the text are kept; an unclosed event before a second
   `<event` is an error, the second still parses (test in Task 1).

---

### Task 1 (C1): event scanner

**Files:**
- Create: `include/chronica_eventus.h`, `lib/chronica_eventus.c`
- Test: `probationes/probatio_chronica_eventus.c`

**Interfaces:**
- Produces:
  ```c
  nomen structura {
      s32    annus;        /* negativus = a.C.; numquam 0 */
      s32    mensis;       /* 0 = absens, 1-12 */
      s32    dies;         /* 0 = absens, 1-31 (solum cum mense) */
      chorda descriptio;   /* textus, spatia collapsa */
      Xar*   personae;     /* chorda, ';' divisae, tonsae, vacuae omissae */
      chorda locus;        /* place (vacua = absens) */
      chorda genus;        /* type (vacua = absens) */
      i32    linea;        /* linea (0-) ubi '<event' incipit */
      i32    columna;      /* octetus in linea */
      i32    linea_finis;  /* linea ubi clausura finitur */
      i32    columna_finis;/* octetus post '>' clausurae */
  } ChronicaEventus;

  nomen structura {
      i32    linea;
      i32    columna;
      i32    linea_finis;
      i32    columna_finis;
      chorda causa;        /* Latine: "annus deest", ... */
  } ChronicaVitium;

  /* lineae: Xar de chorda (index = numerus lineae, vacuae servatae);
   * eventa: Xar de ChronicaEventus; vitia: Xar de ChronicaVitium aut
   * NIHIL. Omnia in piscina copiata. FALSUM solum si memoria deficit. */
  b32
  chronica_eventa_legere (
      constans Xar*    lineae,
           Piscina*    piscina,
               Xar*    eventa,
               Xar*    vitia);
  ```
  (Names are proposals; the header step settles them.)

- [ ] **Step 0: Header text to Fran** - `include/chronica_eventus.h` as
  above in house style; wait for approval.
- [ ] **Step 1: Write failing tests** (credo, sections per case; model on
  `probationes/probatio_piscina.c`):
  - `simplex`: `<event year="1274">Aquinas dies</event>` -> 1 event,
    annus 1274, descriptio "Aquinas dies", linea 0, columna 0.
  - `clausura anonyma`: `<event year="1274">Aquinas dies</>` -> same.
  - `ante Christum`: `year="-1446"` -> annus -1446.
  - `attributa`: `month="5" day="1" people="Moses; Aaron" place="Egypt"
    type="exodus"` -> mensis 5, dies 1, personae {"Moses","Aaron"},
    locus "Egypt", genus "exodus".
  - `multilinea`: lines {"Prosa.", "<event year=\"-1446\">Moses",
    "   leads   the", "Exodus</event> post"} -> descriptio "Moses leads
    the Exodus", linea 1, linea_finis 3.
  - `in prosa`: `Hodie legi. <event year="1225">Thomas nascitur</event>
    Plura.` -> columna 13; two events on one line -> both, in order.
  - `lineae vacuae numerantur`: event on line index 3 after two blank
    lines -> linea 3.
  - `vitia`: no year; `year="0"`; `year="abc"`; `month="13"`; `day="4"`
    without month; never closed -> 0 events, 1 vitium each with the
    right linea/columna; good event after a bad one still parses.
  - `nomina`: `people="Moses ;; Tomás de Aquino"` -> {"Moses","Tomás de
    Aquino"} byte-exact (Review Focus 4).
  - `signa in textu`: `<event year="1">a < b > c</event>` -> descriptio
    "a < b > c"; `<event year="1">x <event year="2">y</event>` -> 1
    vitium (first unclosed) + event year 2 (Review Focus 5).
  - `majuscula`: `<Event year="1">x</Event>` -> nothing (tag is
    lowercase `event`).
- [ ] **Step 2: Run, verify RED:** `./compile_tests.sh chronica_eventus`
  -> fails to link (`chronica_eventa_legere` undefined).
- [ ] **Step 3: Implement** in `lib/chronica_eventus.c`. Locating
  `<event` in free prose is a hand scan; parsing the attributes may
  reuse the STML parser on the fragment (builder's choice).
- [ ] **Step 4: Run, verify GREEN:** same command -> `OMNIA
  PRAETERIERUNT`.
- [ ] **Step 5: Plants** (each must turn a test red, then restore IDEM):
  blank lines not counted; `;` split without trim; year 0 accepted;
  `</>` not accepted.
- [ ] **Step 6: Link lists, formatter, examen, lint, worklog**
  (`lib/chronica_eventus.worklog.md`).
- [ ] **Step 7: Commit on Fran's word.**

### Task 2 (C2): gathering - per-book projections and the master table

**Files:**
- Create: `include/chronica_fontes.h`, `lib/chronica_fontes.c`,
  `lib/conspectus_eventa_comitis.stml`, `lib/conspectus_eventa_libri.stml`
- Modify: `include/scriba_liber.h`, `lib/scriba_liber.c` (one read
  function), `lib/conspectus_infixa.c` (regenerated),
  `lib/vicus_applicatio.c:2355-2362` (register beside
  `indagatio_genera_registrare`)
- Test: `probationes/probatio_chronica_fontes.c`

**Interfaces:**
- Consumes: `chronica_eventa_legere` (Task 1).
- Produces:
  ```c
  /* scriba_liber.h: lineae paginae OMNES (vacuae servatae, finales
   * vacuae demptae), LECTIO SOLA; praefixum NIHIL = liber communis */
  b32 scriba_liber_paginam_legere_omnes (Piscina*, InternamentumChorda*,
      Volumen*, constans character* praefixum, chorda titulus,
      Xar* exitus);

  nomen structura {
      ChronicaEventus eventus;
      chorda          liber;    /* "comes/<locus>"; vacua = communis */
      chorda          pagina;   /* nomen paginae */
      chorda          titulus_libri; /* "Notes · Thomas Aquinas" */
  } ChronicaRes;

  nomen structura {
      Xar* res;      /* ChronicaRes, ordine: annus, (mensis, dies)
                      * praesentes primum ordine, ceterae ordine
                      * libri/paginae/lineae */
      Xar* anni;     /* s32 distincti, ascendentes */
  } ChronicaTabula;

  nomen structura {
      Volumen*             volumen;
      InternamentumChorda* intern;
  } ChronicaFontes;

  b32 chronica_registrare (ConspectusRegistrum*, ChronicaFontes*,
                           Xar** vitia);
  /* tabula recens: aedificatur solum si auditor clavem stalam
   * nuntiavit post aedificationem priorem (aliter eadem redditur) */
  constans ChronicaTabula* chronica_tabula (ConspectusRegistrum*);
  /* res personae / loci (nomen exactum post tonsuram), ordine tabulae */
  Xar* chronica_res_personae (constans ChronicaTabula*, chorda nomen,
                              Piscina*);
  Xar* chronica_res_loci (constans ChronicaTabula*, chorda nomen,
                          Piscina*);
  ```
  Declarations: `<conspectus nomen="eventa-comitis" versio="1"><clavis
  typus="textus"/><fons spatium="comes/{clavis}/paginae"/></conspectus>`
  and `eventa-libri` with `<fons spatium="paginae/{clavis}"/>` (as
  `lib/conspectus_liber.stml`). Enumerations follow
  `comites_enumerare` (`lib/indagatio_genera.c:570`) and the `liber`
  genus. Invalidation via `conspectus_auditorem_ponere`.

- [ ] **Step 0: Header text to Fran** (both headers + declarations).
- [ ] **Step 1: Failing tests** (temporary volume, pages written with
  `scriba_liber_*`, `volumen_claudere` at the end):
  - `comes`: page "1" of `comes/navigator/en.wikipedia.org/Thomas_Aquinas`
    with `<event year="1274">Aquinas dies</event>` -> tabula 1 res,
    liber "comes/navigator/en.wikipedia.org/Thomas_Aquinas", pagina "1",
    linea as written.
  - `communis`: common-book page "Historia" event -> liber vacua.
  - `ordo`: events 1274 (no month), 1274 month 3, -1446, 1225 across two
    books -> anni {-1446, 1225, 1274}; within 1274 the dated one first.
  - `mutatio`: edit the page (remove event, add year 800) -> next
    `chronica_tabula` shows 800, not 1274.
  - `recens` (Review Focus 2): two reads without a change -> same
    pointer, `conspectus_status` RECENS for every book key.
  - `personae`: "Moses" in two books, "Moses " in a third ->
    `chronica_res_personae("Moses")` = 3.
  - `paginam_legere_omnes`: blank lines kept, trailing blanks dropped.
- [ ] **Step 2: RED:** `./compile_tests.sh chronica_fontes`.
- [ ] **Step 3: Implement**; regenerate `lib/conspectus_infixa.c`; hook
  `chronica_registrare` at `lib/vicus_applicatio.c:2360`.
- [ ] **Step 4: GREEN** + `./compile_tests.sh indagatio_genera` and
  `./compile_tests.sh conspectus` still green.
- [ ] **Step 5: Plants:** auditor ignored (stale table after edit);
  common book not enumerated; order ignores month; trim missing.
- [ ] **Step 6: Lists, formatter, examen, lint, worklog.**
- [ ] **Step 7: Commit on Fran's word.**

### Task 3 (C3): the pane - year view

**Files:**
- Create: `include/chronica.h`, `lib/chronica.c`,
  `apps/chronica/organum.stml`
- Modify: `lib/vicus_applicatio.c` (genus "chronica" beside
  "navigator", ~5407/5458, context giving the shared registry),
  `lib/organa_infixa.c` (regenerated)
- Test: `probationes/probatio_chronica_latus.c`

**Interfaces:**
- Consumes: `chronica_tabula` (Task 2).
- Produces: `ChronicaMontatio` (model `NavigatorMontatio`,
  `include/navigator.h:120`) with `ConspectusRegistrum* (*registrum)(vacuum*)`
  ligature; `chronica_montare`, `chronica_componere`,
  `chronica_argumentum_ponere` ("" = remembered year, "1274", "-1446"),
  `chronica_semitam_secuta`, `chronica_locus` ("chronica/annus/1274"),
  `chronica_locus_titulus` ("Timeline - AD 1274 - 1/2").
  Manifest fields: `visus` (annus | persona | locus), `annus`,
  `pagina`, `electus`, `nomen`.

- [ ] **Step 0: Header + manifest text to Fran.**
- [ ] **Step 1: Failing tests** (pane harness as
  `probatio_navigator_latus.c`, fixture tabula from a temp volume):
  - heading shows "1446 BC" for -1446 and "AD 1274" for 1274;
  - `l` from 1225 -> 1274, `h` back; `h` at the first year stays;
  - `j` / `k` page within a year of 12 long events; footer
    "Timeline - AD 1274 - 1/3";
  - a description longer than a page continues on the next page
    (Review Focus 3);
  - opening position: remembered year restored after remount; first
    mount -> the year with events nearest to today;
  - edit while open (Review Focus 1): year emptied by an edit -> view
    moves to the nearest year with events;
  - no events -> empty page text "Write <event year=\"1274\">...</event>
    in a note";
  - mouse: click on the right/left year marker = `l` / `h`.
- [ ] **Step 2: RED:** `./compile_tests.sh chronica_latus`.
- [ ] **Step 3: Implement** (layout via `lector_paginare` as navigator;
  position in presence as navigator N3.4c).
- [ ] **Step 4: GREEN**, rebuild vicus, Fran looks at it.
- [ ] **Step 5: Plants:** h/l step by one year instead of to the next
  event year; BC sign shown; position not saved; page count off by one.
- [ ] **Step 6: Lists, organa re-embed, formatter, examen, lint,
  worklog.**
- [ ] **Step 7: Commit on Fran's word.**

### Task 4 (C4a): selection in the year view

**Files:** Modify `lib/chronica.c`, `include/chronica.h`; Test:
`probationes/probatio_chronica_latus.c` (section "electio").

**Interfaces:** Consumes navigator's unit pattern (`lib/navigator.c`
`_unitas_currens` / `_nexum_eligere`, N4.3/N6.3) - copy the shape, not
the code; selectable items = events and their people / places.

- [ ] **Step 0: Header comment text to Fran.**
- [ ] **Step 1: Failing tests:** Tab selects the first event (inverted),
  Tab again its first person, S-Tab back; Tab crosses to page 2; Esc
  clears; footer shows the target ("→ note: Notes · Thomas Aquinas" /
  "→ person: Moses"); selected event removed by an edit -> selection
  cleared (Review Focus 1); mouse click selects.
- [ ] **Steps 2-7:** RED, implement, GREEN, plants (Tab ignores page
  boundary; selection survives a removed event), checks, commit.

### Task 5 (C4b): jump home at the line

**Files:** Modify `lib/vicus_applicatio.c:4972-5040`
(`inventum_aperire`: parse `&linea=N` after `liber=` and alone for the
common book), `include/scriba_applicatio.h` + `lib/scriba_applicatio.c`
(`scriba_paginam_ostendere_ad_lineam (ScribaMontatio*, chorda pagina,
i32 linea)` - page shown, cursor on the line), `lib/chronica.c` (Enter /
double-click -> `aperire(ctx, semita)` callback set by vicus to the same
porta as search). Test: `probationes/probatio_vicus_chronica.c` (new,
vicus-level, model `probatio_vicus_navigator.c`).

- [ ] **Step 0: Header text to Fran** (route grammar
  `scriba/<pagina>?liber=<praefixum>&linea=<n>` and
  `scriba/<pagina>?linea=<n>`).
- [ ] **Step 1: Failing tests:** companion event -> left pane shows that
  companion on the page, cursor at the line; right pane shows the paired
  view; common-book event -> left pane shows the page at the line,
  right pane still chronica (spec XIII.2); old routes without `linea`
  still work (search); a click at a real screen point opens
  (`manus_ludus_ad_schirmum` + `manus_ludus_premere_ad`).
- [ ] **Steps 2-7:** RED, implement, GREEN, plants (linea ignored;
  common book replaces right pane), checks (+ `./compile_tests.sh
  indagatio` and comites suites), commit.

### Task 6 (C5): person and place views

**Files:** Modify `lib/chronica.c`, `apps/chronica/organum.stml`; Test:
`probationes/probatio_chronica_latus.c` (section "personae").

**Interfaces:** Consumes `chronica_res_personae` / `chronica_res_loci`
(Task 2). Loci: "chronica/persona/<nomen>", "chronica/locus/<nomen>",
name escaped as `lib/semita.c` escapes route segments (`/`, `?`).

- [ ] **Step 0: Header text to Fran.**
- [ ] **Step 1: Failing tests:** Enter on "Moses" opens his view: heading
  "Moses", his events in order each with its year ("1446 BC  Moses leads
  the Exodus"), j/k pages; Ctrl+O returns to the year view at the same
  year and selection; "Tomás de Aquino" locus round-trips (Review
  Focus 4); place view the same; Enter on an event there = jump home;
  click on a name opens it.
- [ ] **Steps 2-7:** RED, implement, GREEN, plants (name not escaped;
  back loses the year), checks, commit.

### Task 7 (C6): event tags coloured in scriba

**Files:** Modify `lib/scriba_figurae.c:241` (beside `nexus_colorare`);
Test: the scriba figurae suite (`./compile_tests.sh scriba_figurae` -
name confirmed in the header step).

**Interfaces:** Consumes `chronica_eventa_legere` over the whole visible
sheet (events span lines), cached per paint by the document seal.

- [ ] **Step 0: Header comment text to Fran.**
- [ ] **Step 1: Failing tests:** a good event's tag cells drawn in the
  accent colour; a bad event's (no year) in the error colour; a
  multi-line event coloured on both lines; selected lines keep paper
  colour (as `#` links).
- [ ] **Steps 2-7:** RED, implement, GREEN, plants (only first line
  coloured; bad event coloured as good), checks, commit.

## After the last task

Spec status lines per phase (DONE + commit), worklogs, vicus rebuilt;
merge gates (aedilis, android, ios) owed at merge to main.
