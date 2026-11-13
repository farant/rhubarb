# negotia — first slice: Implementation Plan

> **For agentic workers:** executed INLINE in the house way: one task
> per turn, Fran approves each task's header text before code, TDD
> with compiling plants (`silva.planta`), commit only on Fran's word
> via the commissio script. Steps use checkbox (`- [ ]`) syntax.

**Goal:** a project list in vicus; opening a project gives a
right-pane ring (Cmd+H / Cmd+L) of its feature list and its Claude
conversations; a feature drills down into its details with revision
history - dogfooded on negotia itself.

**Architecture:** a library `negotia` keeps projects, features and
project->session links as JSON acts in the vicus volume (the
`navigator_bibliotheca` pattern: append-only acts, in-memory index,
catch-up from a cursor) and polls `volumen_seq_ultimum` so writes from
other processes appear. A CLI `negotia` writes; a pane genus `negotia`
(shaped like `sancti_visus`: list -> detail, Esc back) reads. The vicus
host gains a per-tab ring stepped by Cmd+H/L whose entries may be
"virtual" genera resolved by an opener callback (used for
`colloquium/<session id>`).

**Tech Stack:** C89 + latina.h; volumen (sqlite WAL), json.h, moneta
(ULID), insula/organum manifests, dispositio, figura, membrum__album /
membrum__praecepta, credo v2, manus_ludus (host tests), aedilis.

**Spec:** `project-specs/negotia.md` (approved by Fran 2026-11-04).

## Global Constraints

- C89, the house flags (-pedantic -Wall -Wextra -Werror -Wconversion
  -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
  -Wwrite-strings); 72-column lines; latina.h macros never identifiers.
- Latin identifiers; `VOCABULA_VIAE_ADDITAE="..." ./oratio/vocabula.sh
  -nova` NOVA 0. User-facing UI strings in English (as sancti,
  colloquia: "Back to list").
- chorda is not NUL-terminated; i32/i64 are UNSIGNED.
- Per file: `./silva/examen.sh` ACCIPE (no raw IO: `filum_*` /
  `lectiones_*`); `./silva/formator.sh -scribere` on the in-tree path.
- Layout through `dispositio`; pane state through the organum manifest
  (`apps/negotia/organum.stml`, then `./tools/organa_infigere.sh`).
- Every key action has a mouse path (single click opens - mouse-first).
- Records live in the volume vicus is given (live:
  `~/.rhubarb/paginae-proba.volumen`), shared by every worktree.
- Bodies free text; feature status is the one structured field:
  planned | partial | shipped (Latin tokens `proposita | inchoata |
  perfecta` in the acts).
- Every saved edit of a feature = one act = one revision; an edit
  identical to the current state writes nothing.
- Gates born red by a COMPILING plant; backups in `build/plantae_*`,
  `cmp` after.
- New root lib -> `./tools/compile_tests_fontes_generare.sh`.

## Review Focus

1. **A write from another process while vicus is open** must show in
   the pane within about a second, without restarting -> T1 test
   "alius scriptor" (two handles on one file), T3 test "renovatio".
2. **A session id the session log does not know** (old, pruned, typo)
   must still open something (the card, "not in the session log"),
   never a silent no-op -> T6 test "colloquium ignotum".
3. **A project with no features and no sessions** shows an empty
   state ("No features yet - add one with `negotia facultas`") and its
   ring has one entry, so Cmd+H/L does nothing visible -> T3 test
   "negotium vacuum", T5 test "anulus unius".
4. **An ambiguous id suffix in the CLI** is refused with the
   candidates listed, never applied to the first match -> T1 test
   "suffixum ambiguum".
5. **A long title or multi-line body** is clipped in rows and
   paginated in the details view, never drawn past the pane -> T4 test
   "corpus longum".

---

### Task 1: the `negotia` library

**Files:**
- Create: `include/negotia.h`, `lib/negotia.c`, `lib/negotia.worklog.md`
- Test: `probationes/probatio_negotia.c`
- Regenerate: `compile_tests_fontes_generata.sh`

**Interfaces:**
- Consumes: `volumen_actum_appendere`, `volumen_acta_generum_legere`,
  `volumen_seq_ultimum` (include/volumen.h:153,411,421); `MonetaFons`,
  `moneta_ulid`, `moneta_numerator` (include/moneta.h:27,113,121);
  json.h.
- Produces (include/negotia.h):

```c
nomen structura Negotia Negotia;

nomen enumeratio {
    NEGOTIA_PROPOSITA = 0,   /* planned */
    NEGOTIA_INCHOATA,        /* partial */
    NEGOTIA_PERFECTA         /* shipped */
} NegotiaStatus;

nomen structura {
    chorda id;
    chorda titulus;
    chorda descriptio;
       s64 seq;              /* actum ultimum ipsius */
       s64 seq_activitatis;  /* actum ultimum de eo (facultates,
                              * colloquia comprehensa) - ordo indicis */
} Negotium;

nomen structura {
    chorda id;
    chorda negotium;
    chorda titulus;
       s32 status;
    chorda corpus;
       s64 seq;
    chorda momentum;         /* VolumenActum.momentum */
} NegotiiFacultas;

Negotia* negotia_aperire (Volumen* volumen, MonetaFons fons,
    vacuum* fontis_ctx, Piscina* piscina);
b32 negotia_renovare (Negotia* n);  /* VERUM si acta nova lecta */
i32 negotia_numerus (constans Negotia* n);
constans Negotium* negotia_index (constans Negotia* n, i32 k);
constans Negotium* negotia_negotium_invenire (constans Negotia* n,
    chorda id, b32* ambiguum_out);
chorda negotia_negotium_creare (Negotia* n, chorda titulus,
    chorda descriptio);
b32 negotia_negotium_mutare (Negotia* n, chorda id, chorda titulus,
    chorda descriptio);
Xar* negotia_facultates (constans Negotia* n, chorda negotium,
    Piscina* p);
constans NegotiiFacultas* negotia_facultatem_invenire (
    constans Negotia* n, chorda id, b32* ambiguum_out);
chorda negotia_facultatem_creare (Negotia* n, chorda negotium,
    chorda titulus, s32 status, chorda corpus);
b32 negotia_facultatem_mutare (Negotia* n, chorda id, chorda titulus,
    s32 status, chorda corpus);
Xar* negotia_revisiones (constans Negotia* n, chorda facultas,
    Piscina* p);
b32 negotia_colloquium_adiungere (Negotia* n, chorda negotium,
    chorda sessio);
Xar* negotia_colloquia (constans Negotia* n, chorda negotium,
    Piscina* p);
constans character* negotia_status_titulus (s32 status);
b32 negotia_status_legere (chorda verbum, s32* status_out);
```

Acts (genera kebab-case, datum JSON, each a FULL snapshot):
`negotium` `{"id","titulus","descriptio"}`; `negotii-facultas`
`{"id","negotium","titulus","status","corpus"}`; `negotii-colloquium`
`{"negotium","sessio"}`. Current state = last act per id. Index order:
`seq_activitatis` descending; features and sessions in creation order;
revisions oldest first. `*_invenire` takes a full id or a unique suffix
(the ledger habit); `*_creare` returns the empty chorda on refusal
(empty title, unknown project, bad status); `*_mutare` with no change
returns VERUM and appends nothing; every write appends then catches up
from the cursor (so own and foreign acts interleave by seq).
`negotia_status_legere` accepts `proposita|inchoata|perfecta` and
`planned|partial|shipped`; `negotia_status_titulus` returns the English
word. Reads all replicas' acts (records are shared data, not derived
local state).

- [ ] **Step 1: stub + red suite.** Header as above, `lib/negotia.c`
  with every function returning NIHIL / FALSUM / empty. Sections in
  `probatio_negotia.c` (credo v2, `volumen_temporarium` +
  `volumen_claudere`, ids from `moneta_numerator`):
  - "creare et legere": a project and two features read back; index
    count 1; features in creation order; status titles "planned",
    "shipped".
  - "revisiones": create, mutate status, mutate body -> 3 revisions
    oldest first with the right status/body; a mutation identical to
    the current state -> still 3 and `volumen_seq_ultimum` unchanged.
  - "ordo activitatis": two projects; a feature added to the older one
    moves it to index 0.
  - "colloquia": adjoin s1, s2, s1 again -> exactly [s1, s2].
  - "refutationes": empty title, unknown project, status word "done"
    -> refused, nothing appended.
  - "suffixum ambiguum": ids "...1A" and "...2A" (numerator ids made
    to share a suffix): `invenire("A")` -> NIHIL with `ambiguum`
    VERUM; `invenire("1A")` -> found.
  - "alius scriptor": open a second `Volumen*` on `volumen_via(v)`, a
    second `Negotia*` over it; write a project through B;
    `negotia_renovare(A)` -> VERUM and A finds it; a second
    `renovare(A)` -> FALSUM.
  - "reapertio": close and reopen the volume path -> same state.
- [ ] **Step 2:** `./compile_tests.sh negotia` -> red at every section.
- [ ] **Step 3: implement** in `lib/negotia.c` following
  `lib/navigator_bibliotheca.c` (`acta_legere` cursor pattern,
  :131-160).
- [ ] **Step 4:** `./compile_tests.sh negotia` -> OMNES BONAE.
- [ ] **Step 5: plants** (compiling, each must turn a named section
  red): skip the identical-state check (-> "revisiones"); read only
  `seq > cursor` without polling `volumen_seq_ultimum` in renovare
  (-> "alius scriptor"); suffix match returns the first hit
  (-> "suffixum ambiguum"). Restore, `cmp` IDEM.
- [ ] **Step 6:** examen, formator, lint, regenerate test sources;
  commit on Fran's word.

### Task 2: the `negotia` CLI

**Files:**
- Create: `tools/negotia.c`, `tools/negotia_struere.sh`
- Modify: `aedificatio.stml` (institutio action, like praeco :68-79)

**Interfaces:**
- Consumes: Task 1's API.
- Produces: `bin/negotia` (worktree: `./bin/negotia`), used by
  sessions and by Task 3's seeding.

Usage (single-dash flags, house style; `-volumen V` on every command;
default `~/.rhubarb/paginae-proba.volumen`, the live vicus volume -
Fran confirms at header approval):

```
negotia index
negotia creare <titulus> [-descriptio T]           -> prints id
negotia ostendere <negotium>                       (project, features,
                                                    sessions)
negotia facultas <negotium> <titulus> [-status S] [-corpus T |
                 -corpus-filum F]                  -> prints id
negotia mutare <facultas> [-titulus T] [-status S] [-corpus T |
               -corpus-filum F]
negotia revisiones <facultas>
negotia involvere <negotium> <sessio>
```

`mutare` reads the current feature and replaces only the named
fields. Ids print in full; rows show the last 6 characters. Exit 0 ok,
1 refused (message names why; ambiguous suffix lists candidates), 2
usage. `-corpus-filum` reads through `filum_*`.

- [ ] **Step 1:** smoke script in the scratchpad against a temporary
  volume path: creare, facultas x2, mutare -status shipped, revisiones
  (2 lines), involvere twice (one session listed), ostendere, an
  ambiguous suffix (exit 1 + candidates), a missing argument (exit 2).
  Run it before the tool exists -> fails.
- [ ] **Step 2:** implement `tools/negotia.c` (argv pattern of
  tools/navigator.c:190-235, `_optio` helper as tools/communio.c:55);
  `tools/negotia_struere.sh` as tools/praeco_struere.sh.
- [ ] **Step 3:** smoke script passes; `bin/fabrica` sees the new
  action (`bin/fabrica quid tools/negotia.c`).
- [ ] **Step 4:** examen, formator, lint; commit on Fran's word.

**Then seed (no code; needs Fran's OK because it writes his live
volume):** `negotia creare negotia -descriptio "projects as places..."`;
`negotia involvere <id> 134c8417-d34b-49a2-b8c8-3d3b81913875`; one
feature per spec item, status as of now: library (shipped), CLI
(shipped), project list, feature list, feature details, Cmd+H/L ring,
conversation view, ended-session card + resume (planned); later items
(vision, complications, activity log, ask-on-a-point notes) planned.
Each later task's commit step ends by moving its features to shipped
with `negotia mutare` - the dogfood loop.

### Task 3: pane `negotia` - project list and feature list

**Files:**
- Create: `apps/negotia/organum.stml`, `apps/negotia/unitas.stml`,
  `include/negotia_visus.h` (`/* <aedilis corpus="lib/negotia_visus.c"/> */`),
  `lib/negotia_visus.c`, `lib/negotia_visus.worklog.md`
- Modify: `lib/vicus_applicatio.c` (include, mount, describe,
  `vicus_genus_addere` in `vicus_applicatio_aedificare` near :5674,
  alias `aperiens_fixum_registrare(..., "projects", "negotia", ...)`
  near :5617, a lazily opened `Negotia*` over the app volume like
  `bibliotheca_navigatoris` :4786); `ludus_tessera/compile_probationes.sh`
  (add "negotia_visus" beside "sancti_visus" :156)
- Regenerate: `lib/organa_infixa.c` (`./tools/organa_infigere.sh`),
  test sources
- Test: `probationes/probatio_negotia_visus.c`

**Interfaces:**
- Consumes: Task 1's API.
- Produces:

```c
nomen structura {
    vacuum* ctx;
    b32 (*negotium_aperire)(vacuum* ctx, chorda negotium);
} NegotiaVisusActiones;

b32 negotia_visus_montare (NegotiaMontatio* m, Piscina* piscina,
    InternamentumChorda* intern, InsulaRepositorium* repo,
    constans character* id, constans character* argumentum,
    Negotia* negotia, constans NegotiaVisusActiones* actiones,
    i32 latitudo, i32 altitudo, StatusSystemata* systemata);
chorda negotia_visus_locus_titulus (NegotiaMontatio* m,
    Piscina* p);
```

(`NegotiaMontatio` shaped like `SanctorumMontatio`; Task 6 adds
members to `NegotiaVisusActiones`.)

Manifest fields: `visus` electio `index|negotium|facultas` persistens,
`semita="ligatio" historia="impelle"`, absens index; `negotium` textus
ligatio impelle; `pagina`, `electa`, `super` (hovered row id)
transiens. Cognomina `negotia/index`, `negotia/negotium/{}`. Pane
argument: empty -> index; an id or suffix -> that project's feature
list. Rows: index = title + "3 shipped · 2 partial · 4 planned · 2
conversations"; feature list = status chip (`figura_globulum_pingere`)
+ title. Hover highlights the row (pattern: kalendarium.c:2774,
`EVENTUS_MUS_MOTUS` / `EXIIT` into transient `super`); a single click
opens; j/k, Enter, n/p, Esc as in sancti; page arrows clickable.
`pulsare` every 1000 ms (`mora`) calls `negotia_renovare` and reports
`mutatum`. Locus titles: "Projects", "<project title>". Opening a
project from the list calls `actiones->negotium_aperire` (host: for
now `vicus_acervo_aperire(v, "negotia", id)`). Any new drawing
primitive (e.g. a hover row) goes in figura.h, not the pane.

- [ ] **Step 1: stub + red suite** (headless, the
  probatio_admonitiones_visus.c helpers: `centrum`, `clavis`, `ictus`,
  `reddere`, `textus_pictus`, `campus_est`):
  - "index": two seeded projects -> rows r1, r2 in activity order;
    row text contains "1 shipped".
  - "aperire": click r1 -> `negotium_aperire` called once with its id
    (spy actiones); Enter on r2 likewise.
  - "negotium": argument = project id -> feature rows with chips;
    `campus_est(ramus, "visus", "negotium")`.
  - "negotium vacuum": no features -> the empty-state text drawn.
  - "super": a motion event over r2 -> `super` = "r2" and the row is
    drawn highlighted; exit -> cleared.
  - "renovatio": a second `Negotia*` on the same file adds a feature;
    `pulsare` -> `mutatum` and a new row appears.
  - host hunk in probatio_vicus_acervus.c: `$projects` opens
    "negotia"; `frons_est(v, "<n>_dextrum_negotia")`.
- [ ] **Step 2:** red.
- [ ] **Step 3:** implement (outline of lib/sancti_visus.c:
  `_indicem_pingere` :1541, `fac_aperire` :1099, `fac_ad_indicem`
  :1265; composition with dispositio).
- [ ] **Step 4:** green; also `./compile_tests.sh vicus_acervus`.
- [ ] **Step 5: plants:** drop the `renovare` call in pulsare
  (-> "renovatio"); hover not cleared on exit (-> "super"). Restore,
  `cmp`.
- [ ] **Step 6:** gates; screenshot via `-imago` and Read it (look
  check for Fran); commit on Fran's word; `negotia mutare` project
  list + feature list -> shipped.

### Task 4: feature details with revisions

**Files:**
- Modify: `apps/negotia/organum.stml` (field `facultas` textus ligatio
  impelle; `visus` option `facultas`; cognomen `negotia/facultas/{}`),
  `lib/negotia_visus.c`
- Test: `probationes/probatio_negotia_visus.c`

**Interfaces:**
- Consumes: `negotia_facultatem_invenire`, `negotia_revisiones`.
- Produces: visus `facultas`; locus title "<project> · <feature>".

Details view: title, status chip, body (wrapped to the pane width,
paginated, never drawn past the pane), then "History": one row per
revision, newest first, "<date> · <status> · <first body line>".
Click / Enter on a feature row opens it; Esc returns to the feature
list with the same selection; Esc there pops the pane.

- [ ] **Step 1: red sections:** "facultas" (click a feature -> visus
  facultas, body text drawn); "historia" (3 revisions -> 3 history
  rows, newest first); "redire" (Esc -> visus negotium, `electa`
  unchanged; Esc again -> FALSUM so the host pops); "corpus longum"
  (a 200-line body: page 1 drawn inside the pane bounds, page arrow
  present, n -> page 2).
- [ ] **Step 2:** red. **Step 3:** implement. **Step 4:** green.
- [ ] **Step 5: plants:** history oldest-first (-> "historia"); body
  not clipped (-> "corpus longum"). Restore, `cmp`.
- [ ] **Step 6:** gates, screenshot, commit on Fran's word,
  `negotia mutare` feature details -> shipped.

### Task 5: the ring in the vicus host (Cmd+H / Cmd+L)

**Files:**
- Modify: `include/vicus.h`, `lib/vicus.c`, `lib/vicus_applicatio.c`
  (project opening sets the ring)
- Test: `probationes/probatio_vicus_acervus.c` (new sections after
  XIII)

**Interfaces:**
- Consumes: the cycle machinery as a model (`est_imperium_cycli`
  vicus.c:597, `destinare` :791-795, `radicem_tractare` :1744,
  `PetitioAcervi.gradus` :39, `vicus_pulsare` dispatch :4115,
  `petitionem_applicare`).
- Produces (include/vicus.h):

```c
/* anulus tabulae activae; numerus ZEPHYRUM = tollere; positio =
 * index frontis initio */
b32 vicus_anulum_ponere (Vicus* v,
    constans character* constans* genera,
    constans character* constans* argumenta,
    i32 numerus, i32 positio);
b32 vicus_anulo_gradi (Vicus* v, s32 directio);

/* genus virtuale: gradus ad id aperitorem vocat loco aperitionis */
nomen b32 (*VicusAperitor)(vacuum* ctx,
    constans character* argumentum);
b32 vicus_aperitorem_addere (Vicus* v, constans character* genus,
    VicusAperitor aperire, vacuum* ctx);
```

Per TAB (unlike the per-Vicus cycle): `VicusTabula` gains the ring
(Xar of `LatusCycli`) and a position index. Cmd+L = +1, Cmd+H = -1,
wrapping; the step is queued like the cycle (`PetitioAcervi` gains
`b32 anulus`) and applied in `vicus_pulsare`: entry genus with an
opener -> call it; else `petitionem_applicare` (bring to front or
mount, focus right). Detector `est_imperium_anuli`: DEPRESSUS or
LIBERATUS, exactly MOD_SUPER, runa 'h' / 'l'; routed to the root in
`destinare`; release swallowed. No ring or a ring of one: swallowed,
nothing changes. The ring and position are written in the tab record
beside `<acervus>` (vicus.c:285) and restored. The cycle (Cmd+]/[) is
untouched. Host: `negotium_aperire` now sets the ring [("negotia", id)]
with position 0, then opens ("negotia", id); Task 6 appends the
("colloquium", <sessio>) entries once their opener exists.

- [ ] **Step 1: red sections:** "anulus gradus" (ring of three plain
  genera: Cmd+L x2 -> third entry front; nothing changes before
  `vicus_pulsare`); "anulus circulus" (Cmd+L from the last -> first;
  Cmd+H from the first -> last); "anulus unius" (ring of one / no
  ring: stack and focus unchanged); "anulus aperitor" (genus with an
  opener: the opener gets the argument, the stack is not touched by
  vicus); "anulus et cyclus" (Cmd+] still cycles, ring position
  unchanged); "anulus restitutus" (tab record round-trip keeps ring
  and position); "anulus per tabulam" (a second tab has no ring).
- [ ] **Step 2:** red. **Step 3:** implement. **Step 4:** green;
  whole vicus suites (`./compile_tests.sh vicus`).
- [ ] **Step 5: plants:** ring stored per Vicus not per tab (-> "per
  tabulam"); position not wrapped (-> "circulus"); ring not written
  in the tab record (-> "restitutus"). Restore, `cmp`.
- [ ] **Step 6:** gates; commit on Fran's word; `negotia mutare` ring
  -> shipped.

### Task 6: conversation entries, ended-session card, resume

**Files:**
- Modify: `lib/vicus_applicatio.c` (opener "colloquium"; terminal
  argument form `claude`), `lib/negotia_visus.c`,
  `include/negotia_visus.h`, `apps/negotia/organum.stml` (`visus`
  option `colloquium`, field `colloquium` textus ligatio)
- Test: `probationes/probatio_negotia_visus.c`,
  `probationes/probatio_vicus_acervus.c`

**Interfaces:**
- Consumes: `colloquia_visus_fontis` (colloquia_visus.h:140; newest
  first, match `->sessio`), `saltus` (vicus_applicatio.c:3150),
  `argumenta_terminalis` (:1806), Task 5's `vicus_aperitorem_addere`.
- Produces: `NegotiaVisusActiones` gains

```c
    b32 (*colloquium_legere)(vacuum* ctx, chorda sessio,
        Piscina* p, Colloquium* out);     /* FALSUM: ignotum */
    b32 (*colloquium_resumere)(vacuum* ctx, chorda sessio,
        chorda via);
```

`negotium_aperire` appends one ("colloquium", <sessio>) ring entry per
`negotia_colloquia` session, in link order, after ("negotia", id).
`include/negotia_visus.h` includes `colloquia.h` for `Colloquium`.
Opener "colloquium": session found and `panellus_vivit` -> `saltus(ca,
c->panellus, c->servitor)` (exactly what a click in $sessions does);
otherwise open ("negotia", "colloquium/<sessio>"), the card: title,
working directory, last time, last status line - or "Not in the
session log" with the bare id - and a Resume button (key `r`). Resume
opens a terminal with argument `claude <sessio> <via>`, which
`argumenta_terminalis` turns into `$SHELL -l -c 'cd <via> && claude
--resume <sessio>; exec $SHELL -l'`; the session id must match
`[0-9A-Za-z-]+` and `via` is single-quote escaped, else refused.
Locus of the card = the per-session companion location
`terminale/claude/<sessio>`, so its notes are the conversation's notes.

- [ ] **Step 1: red sections:** "colloquium vivum" (opener with a
  live fake colloquium -> the saltus path is taken: spy on the
  multiplexor call or the focused terminal); "colloquium mortuum"
  (ended -> card pane opened, title drawn); "colloquium ignotum"
  (unknown id -> card with "Not in the session log"); "resumere"
  (click Resume -> `colloquium_resumere(sessio, via)`); "argumentum
  claude" (`claude 134c-x /tmp/a b` -> the exact command with quoted
  path; `claude ;rm x /tmp` -> refused).
- [ ] **Step 2:** red. **Step 3:** implement. **Step 4:** green.
- [ ] **Step 5: plants:** card skipped for unknown ids (-> "ignotum");
  path not escaped (-> "argumentum claude"). Restore, `cmp`.
- [ ] **Step 6:** gates; live check in vicus with Fran (open
  `$projects`, negotia, Cmd+L lands on this conversation); commit on
  Fran's word; `negotia mutare` conversation view + card -> shipped;
  phase-log entry; memory pointer.

## Notes

- Live check: worktrees cannot install, so the live vicus runs main's
  build. Seeing the pane live means running secunda's vicus
  (`./apps/vicus/vicus.sh -volumen ~/.rhubarb/paginae-proba.volumen`)
  in place of the running one - Fran's call at Task 3, never two
  vicus processes on one volume.
- comites (Cmd+T) and semitae are secunda work; this plan assumes
  secunda's tree (where both exist).
- Ledger: one `opus` per task inside a negotia park, filed after Fran
  approves this plan; `silva.commissio(opus=ID)` closes each.
