# briar plan 8 — visio in the app (Visio ⌘⇧V: the literate page in a second window)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit).
> Steps use checkbox syntax. Written 2026-09-14 from briar-spec.md §4.9
> (v1.10, approved by Fran the same day, commit `d511f950`). Not started.

**Goal:** every vitrea program briar builds carries its own literate page
(`<t>.visio.html`, the page `-html` writes) and shows it from a **Visio**
item (⌘⇧V) in its app menu, in a second window — for `./x.thistle`,
`-struere` and a double-clicked `x.app` alike.

**Architecture:** three small additions, one per layer. **fenestra**
gains its first app-menu seam: an item above `Exire` whose click arrives
as an event (`EVENTUS_MENU`) in the window's queue, never a callback.
**atrium** gains one optional field, `visio` — a path inside the app's
capsula — and owns the item and the second window inside
`atrium_gressus`, as it owns the speculum shortcut; `NIHIL` (every
existing atrium app) changes nothing. **briar** renders the page when it
writes a vitrea project, adds it as an asset, puts the chrome's hash in
the key, and the generated main sets `figura.visio`.

**Tech stack:** C89 in Latin (`latina.h`), Objective-C (MRC) in
`lib/fenestra_macos.m`, atrium / vitrea / internuntius, briar modules,
credo gates, `tools/briar_fumus.sh` with `bin/manus`.

**Spec:** `project-specs/briar-spec.md` §4.9 (decisions V1–V5, the three
layers, gates, the known wrinkle), §6 (`visio` row), §7 (files), §8 (P9),
§9 (deferrals), §10 (risks).

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers; single capitals are Roman numerals. `chorda`
  is not NUL-terminated; `i32`/`i64` are UNSIGNED.
- Edits to C/H through `silva.Editio(via).replace(…)` (token anchors; a
  multi-line anchor needs a multi-line `novus`; exact bytes with
  `tolerans=False`). **Edits to `lib/fenestra_macos.m` and to shell
  scripts go through `silva.Textus`** (exact text) — silva's editor
  parses C, not Objective-C. Then `./silva/formator.sh <viae> -scribere`
  and `-vitia` on the C files: no NEW divergence (record the baseline
  first). Generated goldens are formatter-exempt.
- Lines ≤ 72 columns; banners are 50 `=`.
- No angle-bracket tag openings (`<word`) inside C comments or string
  literals — the tag scanner evokes them.
- Commit only with explicit paths through `silva.commissio(msg, viae,
  portae)`; never Fran's uncommitted files (`FAQ.md`, `gesta/annales/*`,
  `project-specs/exempla/kalendarium.html`, `salutatio2.html`,
  `kalendarium.app`). Tasks 1–2 gate on `[('radix', None)]`; Task 3 on
  `[('radix', None), ('briar', None)]`; Task 4 adds
  `('briar-fumus', None)`. Verify each gate by its log's mtime against
  the commit time.
- **Before trusting `./oratio/vocabula.sh -nova`, run `./silva/nexus.sh
  <a new symbol>`** — `-nova` reads the identifier index without
  refreshing it. Words this plan introduces: `menu` (IGNOTUM — Task 1
  adds an `ignotum-permissum` line), `scopus`, `pressa`, `aequivalens`,
  `littera`, `locus`, `modificantes`, `visio`, `visionem`, `pulsare`,
  `signum`, `vestire`, `vestis` — check each with
  `./oratio/quaere.sh -machina <verbum>` before committing.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  whose red count is PREDICTED before the run, and the plant is
  confirmed gone.
- A window cannot be tested by the automatic suites
  (`probatio_atrium.c` says so). GUI behaviour is proven by hand in
  Task 2 and by fumus `-agere` in Task 4.
- After touching `lib/`, the embedded corpus is stale: rebuild
  `bin/briar` (`./briar/compile_probationes.sh registrum` then
  `./tools/briar_struere.sh`) before any step that builds a program
  OUTSIDE the tree. Inside the tree briar reads the DISK corpus.

## Beyond the spec (small, named here so the seal can say "as built")

1. atrium refuses `visio = ""` (an empty path would open a window on a
   404) — before the capsula check, so the refusal gate never touches
   AppKit.
2. The Visio window inherits `-retro` (`FENESTRA_RETRO`) from the main
   one, so fumus runs don't steal focus twice.
3. `fenestra_menu_addere` writes Shift + a letter as the UPPERCASE key
   equivalent without Shift in the mask — the `@"Z"` form of `Iterare`,
   the one `fenestra_claviarius`'s injected keys were MEASURED to match
   (2026-08-15).
4. The chrome hash is length-prefixed per file
   (`briar_stampam_vestire`), so moving bytes from `facies.css` to
   `facies.js` changes the key too.
5. Known and accepted: a project written by the dev instrument
   `./briar/fabrica.sh` gets `figura.visio` but no page (the instrument
   renders none), so its Visio window shows the capsula's 404. Every
   project briar itself writes has the page.

## File structure

| file | responsibility |
|---|---|
| `include/fenestra.h`, `lib/fenestra_macos.m` | `EVENTUS_MENU`, `datum.menu.signum`, `fenestra_menu_addere`, one modifier parser (T1) |
| `lib/eventus_stml.c`, `probationes/probatio_eventus_stml.c` | the new genus's title, in enumeration order (T1) |
| `oratio/glossarium.stml` | `menu` as `ignotum-permissum` (T1) |
| `include/atrium.h`, `lib/atrium.c`, `probationes/probatio_atrium.c` | `visio`, `ATRIUM_ACTUM_VISIO`, the second window (T2) |
| `briar/fontes/briar_fabrica.h/.c` | `briar_visionem_addere`; the generated main sets `figura.visio` and prints `visio aperta` (T3) |
| `briar/fontes/briar_facies.h/.c` | `briar_stampam_vestire` (T3) |
| `briar/probationes/probatio_briar_facies.c` | helpers extracted; the visio block (T3) |
| `briar/probationes/probatio_briar_fabrica.c`, golden `fixa/fabrica/salve_vitreum/fontes/salve_vitreum.c` | the main's visio lines (T3) |
| `tools/briar.c` | chrome hash in the key; page rendered and added when a vitrea project is written (T3) |
| `tools/briar_fumus.sh` | stage IV (page in the project, equal to `-html`'s) and XIV (`manus clavis Cmd+Shift+v` → `visio aperta`) (T4) |
| spec §4.9/§8, `briar/CLAUDE.md`, worklogs, plan header, memory, ledger | seal (T4) |

---

### Task 1: fenestra — the app-menu seam

**Files:** modify `include/fenestra.h`, `lib/fenestra_macos.m`,
`lib/eventus_stml.c`, `probationes/probatio_eventus_stml.c`,
`oratio/glossarium.stml`; append `lib/fenestra_macos.worklog.md`.

**Interfaces produced** (Task 2 consumes all three):

```c
/* include/fenestra.h - eventus_genus_t, appended LAST, after
 * EVENTUS_FOCUS_PETITUS (which gains a comma): */
    EVENTUS_FOCUS_PETITUS,
    /* res menu applicationis pressa (fenestra_menu_addere) */
    EVENTUS_MENU

/* Eventus.datum, a new arm of the union after 'rotula': */
        structura {
            i32 signum;         /* a fenestra_menu_addere datum */
        } menu;

/* prototype, after fenestra_magnitudinator: */

/* Rem in menu APPLICATIONIS addere, supra separatorem et 'Exire'.
 *
 * Menu UNUM toti applicationi est. Pressio NON revocatio est: eventum
 * EVENTUS_MENU cum datum.menu.signum in caudam HUIUS fenestrae ponit,
 * per eandem caudam ac claves - app eum in gyro suo legit.
 *
 * clavis: forma claviarii ("Cmd+Shift+v": modificantes, deinde
 * littera UNA) aut NIHIL (sine aequivalente).
 *
 * Redde FALSUM si fenestra aut titulus NIHIL, si menu applicationis
 * abest (fenestra_creare id struit), aut si clavis prava est. */
b32
fenestra_menu_addere (
               Fenestra* fenestra,
    constans character* titulus,
    constans character* clavis,
                    i32  signum);
```

- [ ] **Step 1: the header and the failing test.** Add the three pieces
  above to `include/fenestra.h`. In `probationes/probatio_eventus_stml.c`,
  after `CREDO_VERUM (eventus_genus_ex_titulo("ignotissimum")
  == EVENTUS_NIHIL);`, insert:

```c
    /* EVENTUS_MENU (plan 8): titulus ultimus, ordine enumerationis */
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_MENU), "menu")
        == ZEPHYRUM);
    CREDO_VERUM (eventus_genus_ex_titulo("menu") == EVENTUS_MENU);
```

  Do NOT touch the title table yet.

- [ ] **Step 2: run red.** `./compile_tests.sh eventus_stml`. Predict **2
  reds**: `tituli[]` has 18 entries and `EVENTUS_MENU` is 18, so
  `eventus_genus_titulus` answers `"ignotum"`, and `ex_titulo("menu")`
  answers `EVENTUS_NIHIL`.

- [ ] **Step 3: the title.** In `lib/eventus_stml.c` append `"menu"`
  after `"focus_petitus"` in `tituli[]` (the table's own comment: order =
  enumeration order, extend together). Run green.

- [ ] **Step 4: every switch over the enumeration.** `./silva/nexus.sh
  EVENTUS_FOCUS_PETITUS` and `./silva/nexus.sh eventus_genus_t`, then read
  each `commutatio` over a genus. Known today: `lib/eventus_stml.c`
  (serializer, `ordinarius: frange;` — a menu event is written as its
  title with no attributes; accepted, nothing replays menu events) and
  `lib/dispensator.c` (a call, not a switch). A switch with no
  `ordinarius` would fail `-Wswitch` under `-Werror` in Step 7 and name
  itself. Record the list in the worklog.

- [ ] **Step 5: the Objective-C seam** (`silva.Textus` on
  `lib/fenestra_macos.m`, exact text; the file is MRC — `mainMenu`,
  `addItem`, `setSubmenu`, `insertItem` retain, ours are released).

  (a) **One modifier parser.** Read `fenestra_claviarius` and cut its
  `/* Praefixa modificatorum, cumulabilia */ per (;;) { … }` block
  (anchor on the exact text). Put it, unchanged in behaviour, into a
  static placed immediately BEFORE the `Claves NOMINATAE` banner (after
  `fenestra_magnitudinator`):

```objc
/* Praefixa modificatorum ('Cmd+' 'Ctrl+' 'Shift+' 'Alt+' 'Opt+'),
 * cumulabilia; *p post ea ponitur. SEDES UNA: fenestra_claviarius
 * (claves immissae) et fenestra_menu_addere (aequivalens rei). */
interior i32
_modificantes_legere (
    constans character** p)
{
    i32 modi = ZEPHYRUM;

    per (;;)
    {
        si (strncmp(*p, "Cmd+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagCommand;
            *p = *p + 4;
        }
        alioquin si (strncmp(*p, "Ctrl+", 5) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagControl;
            *p = *p + 5;
        }
        alioquin si (strncmp(*p, "Shift+", 6) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagShift;
            *p = *p + 6;
        }
        alioquin si (strncmp(*p, "Alt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            *p = *p + 4;
        }
        alioquin si (strncmp(*p, "Opt+", 4) == 0)
        {
            modi = modi | (i32)NSEventModifierFlagOption;
            *p = *p + 4;
        }
        alioquin
        {
            frange;
        }
    }
    redde modi;
}
```

  In `fenestra_claviarius` the cut block becomes
  `modi = _modificantes_legere(&p);`. Fumus `-agere` XIV (`premere`,
  `clavis`) exercises the unchanged behaviour in Task 4.

  (b) **The target class**, immediately AFTER `extrahere_eventum` (it
  calls the static `impellere_eventum` defined just above):

```objc
/* Scopus rei menu: pressio in EVENTUS_MENU fenestrae suae vertitur -
 * nulla revocatio C, eventus per caudam ut claves. Res menu scopum
 * NON retinet (target assign), ergo scopus numquam dimittitur: vivit
 * quamdiu applicatio (res menu non removentur). */
@interface FenestraMenuScopus : NSObject
@property (assign) Fenestra *fenestra;
- (void)pressa:(id)sender;
@end

@implementation FenestraMenuScopus
- (void)pressa:(id)sender {
    Eventus eventus;

    memset(&eventus, 0, magnitudo(eventus));
    eventus.genus              = EVENTUS_MENU;
    eventus.datum.menu.signum  = (i32)[sender tag];
    impellere_eventum(self.fenestra, &eventus);
}
@end
```

  (c) **The function**, immediately AFTER `fenestra_claviarius` (before
  `/* Implementatio tabulae pixelorum */`):

```objc
b32
fenestra_menu_addere (
               Fenestra* fenestra,
    constans character* titulus,
    constans character* clavis,
                    i32  signum)
{
                 NSMenu* app_menu;
             NSMenuItem* res;
     FenestraMenuScopus* scopus;
               NSString* aequivalens = @"";
               NSInteger  locus;
     constans character* p;
                     i32  modi = ZEPHYRUM;

    si (   fenestra == NIHIL || titulus == NIHIL || NSApp == nil
        || [NSApp mainMenu] == nil)
    {
        redde FALSUM;
    }
    @autoreleasepool {
        app_menu = [[[NSApp mainMenu] itemAtIndex:0] submenu];
        locus    = (app_menu != nil)
                 ? [app_menu indexOfItemWithTitle:@"Exire"] : -1;
        si (locus < 0)
        {
            redde FALSUM;
        }
        si (clavis != NIHIL)
        {
            character littera[2];

            p     = clavis;
            modi  = _modificantes_legere(&p);
            si (p[0] == '\0' || p[1] != '\0')
            {
                redde FALSUM;   /* littera UNA post modificantes */
            }
            littera[0]  = p[0];
            littera[1]  = '\0';
            /* Shift + littera = aequivalens MAIUSCULUM sine Shift in
             * masca: forma '@"Z"' rei Iterare, quam claves immissae
             * congruere MENSURATAE sunt (fenestra_claviarius) */
            si (   (modi & (i32)NSEventModifierFlagShift) != ZEPHYRUM
                && littera[0] >= 'a' && littera[0] <= 'z')
            {
                littera[0]  = (character)(littera[0] - ('a' - 'A'));
                modi        = modi & ~(i32)NSEventModifierFlagShift;
            }
            aequivalens = [NSString stringWithUTF8String:littera];
        }
        scopus           = [[FenestraMenuScopus alloc] init];
        scopus.fenestra  = fenestra;
        res = [[NSMenuItem alloc]
            initWithTitle:[NSString stringWithUTF8String:titulus]
                   action:@selector(pressa:)
            keyEquivalent:aequivalens];
        [res setTarget:scopus];
        [res setTag:(NSInteger)signum];
        [res setKeyEquivalentModifierMask:
            (NSEventModifierFlags)(unsigned long)modi];
        /* separator SEMEL ante 'Exire'; res priores supra eum */
        si (   locus > 0
            && [[app_menu itemAtIndex:locus - 1] isSeparatorItem])
        {
            locus = locus - 1;
        }
        alioquin
        {
            [app_menu insertItem:[NSMenuItem separatorItem]
                         atIndex:locus];
        }
        [app_menu insertItem:res atIndex:locus];
        [res release];
    }
    redde VERUM;
}
```

  The app menu is `[Exire]` today (`_menu_ordinarium_ponere`). One item
  gives `[Visio][separator][Exire]`; a second
  `[item1][item2][separator][Exire]`.

- [ ] **Step 6: the word.** `./oratio/quaere.sh -machina menu` says
  `IGNOTUM`. In `oratio/glossarium.stml`, after the `enoent` line:

```xml
  <vocabulum lemma="menu" lingua="anglica" classis="ignotum-permissum" contextus="latinus" nota="nomen rei interfaciei (menu applicationis macOS), ut plist; fenestra_menu_addere et EVENTUS_MENU 2026-09-14 (plan 8)"/>
```

  Check `scopus pressa aequivalens littera locus modificantes` the same
  way; `bin/canon_examen oratio/glossarium.stml` → VITIA 0.

- [ ] **Step 7: root suite** `./compile_tests.sh` (compiles
  `lib/fenestra_macos.m` and every switch): all green. The menu itself is
  exercised by hand in Task 2 Step 6.

- [ ] **Step 8: plant.** In `lib/eventus_stml.c`, `"menu"` → `"menus"`.
  Predict **2 reds** (the title; `ex_titulo("menu")` answering
  `EVENTUS_NIHIL`). Restore; confirm gone.

- [ ] **Step 9: worklog + commit.** `lib/fenestra_macos.worklog.md`,
  dated `## 2026-09-14 — menu applicationis: fenestra_menu_addere (plan
  8)`: the first app-menu seam, event not callback, the separator rule,
  the extracted modifier parser, Shift → uppercase, the switch list, the
  appended genus and its title. Commit `include/fenestra.h`,
  `lib/fenestra_macos.m`, `lib/eventus_stml.c`,
  `probationes/probatio_eventus_stml.c`, `oratio/glossarium.stml`, the
  worklog; portae `[('radix', None)]`.

---

### Task 2: atrium — the Visio item and the second window

**Files:** modify `include/atrium.h`, `lib/atrium.c`,
`probationes/probatio_atrium.c`; append `lib/atrium.worklog.md`.

**Interfaces consumed:** `fenestra_menu_addere`, `EVENTUS_MENU`,
`datum.menu.signum` (Task 1); `fenestra_creare`, `fenestra_monstrare`,
`fenestra_debet_claudere` (the delegate's `windowShouldClose` answers
NO and only raises the flag; the window is `setReleasedWhenClosed:NO`,
so `fenestra_destruere` is the one release), `fenestra_destruere`,
`vitrea_creare`, `vitrea_obtinere_nuntium`, `vitrea_recargare`,
`vitrea_destruere`, `vitrea_missor`, `internuntius_creare(piscina,
missor, datum)`, `internuntius_tractare(inx, nuntium, piscina)`,
`atrium_nuntium_discernere`. vitrea keeps no global state
(`lib/vitrea_macos.m` has no mutable statics), so a second web view is
independent of the first.

**Interfaces produced** (Task 3's generated main consumes them):

```c
/* include/atrium.h - AtriumActum, appended after INTERITUS: */
    ATRIUM_ACTUM_INTERITUS = 4, /* processus interretialis obiit;
                                 * atrium IAM recargavit */
    ATRIUM_ACTUM_VISIO     = 8  /* fenestra Visio hoc tictu aperta */

/* AtriumConfiguratio, after sine_inspectore_webkit: */
    /* VISIO (optiva, briar-spec par. 4.9): via paginae INTRA capsulam
     * ("x.visio.html"). NIHIL = nulla res menu, nulla fenestra altera:
     * applicationes quae thistle non sunt nihil mutant. Posita: res
     * 'Visio' (Cmd+Shift+v) supra 'Exire'; pressio fenestram ALTERAM
     * super eandem capsulam aperit, aut apertam ante ponit. Vacua
     * recusatur. */
    constans character* visio;
```

- [ ] **Step 1: the header and the failing test.** Add both pieces to
  `include/atrium.h` (and extend the `atrium_gressus` comment: "Visio
  pressa fenestram alteram aperit"). In `probationes/probatio_atrium.c`,
  inside the `--- Probans creare: recusationes ---` block, after the
  `capsula deest (titulo praesente)` case, insert:

```c
        /* visio VACUA recusatur, ANTE capsulam: sine capsula probatur,
         * ergo AppKit numquam tangitur (NIHIL = sine Visio) */
        memset(&figura, 0, magnitudo(figura));
        figura.titulus  = "probatio";
        figura.visio    = "";
        causa.datum     = NIHIL; causa.mensura = ZEPHYRUM;
        atrium = atrium_creare(piscina, &figura, &causa);
        CREDO_NIHIL (atrium);
        CREDO_VERUM (causa.mensura > ZEPHYRUM
            && strstr(chorda_ut_cstr(causa, piscina), "Visio vacua")
                != NIHIL);
```

- [ ] **Step 2: run red.** `./compile_tests.sh atrium`. Predict **1 red**:
  without the check the refusal is `Capsula deest…`, which lacks `Visio
  vacua`; `CREDO_NIHIL` passes. No window opens.

- [ ] **Step 3: implement** in `lib/atrium.c`.

  (a) Beside the size defines:

```c
#define ATRIUM_SIGNUM_VISIO         VII
#define ATRIUM_VISIO_LATITUDO      1100
#define ATRIUM_VISIO_ALTITUDO       860
```

  (b) `structura Atrium` gains, after `datum`:

```c
                   b32  retro;             /* Visio vexillum heres */
    constans character* titulus;           /* copia: titulus Visionis */
    constans character* visio;             /* copia; NIHIL = sine */
               Piscina* visio_piscina;     /* vita fenestrae Visio */
              Fenestra* visio_fenestra;    /* NIHIL = clausa */
                Vitrea* visio_vitrea;
          Internuntius* visio_internuntius;  /* sine methodis */
```

  (c) In `atrium_creare`, right after the `titulus deest` refusal and
  BEFORE `capsula deest`:

```c
    si (figura->visio != NIHIL && figura->visio[0] == '\0')
    {
        redde _frangere(causa,
            "Visio vacua - via paginae intra capsulam aut NIHIL",
            piscina);
    }
```

  after `atrium->datum    = figura->datum;`:

```c
    atrium->retro    = figura->retro;
    atrium->titulus  = chorda_ut_cstr(chorda_ex_literis(figura->titulus,
        piscina), piscina);
    atrium->visio    = (figura->visio != NIHIL)
                     ? chorda_ut_cstr(chorda_ex_literis(figura->visio,
                         piscina), piscina)
                     : NIHIL;
```

  and after the `Internuntius creari non potuit` check (the menu exists
  once a fenestra does):

```c
    /* --- VISIO (optiva): res menu; fenestra altera in gressu --- */
    si (   atrium->visio != NIHIL
        && !fenestra_menu_addere(atrium->fenestra, "Visio",
               "Cmd+Shift+v", (i32)ATRIUM_SIGNUM_VISIO))
    {
        redde _frangere(causa, "Visio: res menu addi non potuit",
            piscina);
    }
```

  (d) Before the `Gyrus` banner, three statics:

```c
/* ==================================================
 * Visio: fenestra altera (briar-spec par. 4.9)
 * ================================================== */

/* ordo: vitrea ANTE fenestram, deinde piscina; partes absentes
 * praetereuntur, ergo et apertio fracta hic purgatur */
interior vacuum
_visionem_claudere (
    Atrium* atrium)
{
    si (atrium->visio_vitrea != NIHIL)
    {
        vitrea_destruere(atrium->visio_vitrea);
    }
    si (atrium->visio_fenestra != NIHIL)
    {
        fenestra_destruere(atrium->visio_fenestra);
    }
    si (atrium->visio_piscina != NIHIL)
    {
        piscina_destruere(atrium->visio_piscina);
    }
    atrium->visio_vitrea        = NIHIL;
    atrium->visio_fenestra      = NIHIL;
    atrium->visio_internuntius  = NIHIL;
    atrium->visio_piscina       = NIHIL;
}

/* Aperire (VERUM) aut apertam ante ponere (FALSUM). Piscina PROPRIA:
 * clausa destruitur, ergo apertio iterata nihil cumulat. Eadem capsula
 * (infixa aut -radix): pagina eodem modo legitur ac principalis. */
interior b32
_visionem_aperire (
    Atrium* atrium)
{
    FenestraConfiguratio  figura_fenestrae;
      VitreaConfiguratio  figura_vitreae;
               character  titulus[CCLVI];

    si (atrium->visio_fenestra != NIHIL)
    {
        fenestra_monstrare(atrium->visio_fenestra);
        redde FALSUM;
    }
    atrium->visio_piscina = piscina_generare_dynamicum("atrium/visio",
        (memoriae_index)(M * M));
    si (atrium->visio_piscina == NIHIL)
    {
        redde FALSUM;
    }
    /* lineola longa UTF-8 (e2 80 94) inter titulum et 'visio' */
    sprintf(titulus, "%.200s \xe2\x80\x94 visio", atrium->titulus);
    memset(&figura_fenestrae, 0, magnitudo(figura_fenestrae));
    figura_fenestrae.titulus   = titulus;
    figura_fenestrae.x         = CC;
    figura_fenestrae.y         = CC;
    figura_fenestrae.latitudo  = (i32)ATRIUM_VISIO_LATITUDO;
    figura_fenestrae.altitudo  = (i32)ATRIUM_VISIO_ALTITUDO;
    figura_fenestrae.vexilla   = (i32)(FENESTRA_CLAUDIBILIS
                               | FENESTRA_MUTABILIS
                               | FENESTRA_CENTRATA);
    si (atrium->retro)
    {
        figura_fenestrae.vexilla = figura_fenestrae.vexilla
                                 | (i32)FENESTRA_RETRO;
    }
    atrium->visio_fenestra = fenestra_creare(atrium->visio_piscina,
        &figura_fenestrae);
    si (atrium->visio_fenestra == NIHIL)
    {
        _visionem_claudere(atrium);
        redde FALSUM;
    }
    memset(&figura_vitreae, 0, magnitudo(figura_vitreae));
    figura_vitreae.origo          = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula        = atrium->capsula;
    figura_vitreae.via_initialis  = atrium->visio;
    figura_vitreae.url            = NIHIL;
    figura_vitreae.inspectabilis  = VERUM;
    atrium->visio_vitrea = vitrea_creare(atrium->visio_piscina,
        atrium->visio_fenestra, &figura_vitreae);
    /* internuntius SINE methodis: vocationes paginae 'methodus ignota'
     * statim accipiunt et pagina ad insulam suam labitur (V4) */
    atrium->visio_internuntius = (atrium->visio_vitrea != NIHIL)
        ? internuntius_creare(atrium->visio_piscina, vitrea_missor,
              atrium->visio_vitrea)
        : NIHIL;
    si (atrium->visio_internuntius == NIHIL)
    {
        _visionem_claudere(atrium);
        redde FALSUM;
    }
    fenestra_monstrare(atrium->visio_fenestra);
    redde VERUM;
}

/* per tictum: clausuram videre, pontem exhaurire (nihil cumulatur).
 * Pumpa fenestrae eventus TOTIUS applicationis in caudam principalem
 * trahit; cauda Visionis vacua manet et legenda non est. */
interior vacuum
_visionem_pulsare (
    Atrium* atrium)
{
                chorda  nuntium;
    VitreaNuntiusGenus  genus;

    si (atrium->visio_fenestra == NIHIL)
    {
        redde;
    }
    si (fenestra_debet_claudere(atrium->visio_fenestra))
    {
        _visionem_claudere(atrium);
        redde;
    }
    dum (vitrea_obtinere_nuntium(atrium->visio_vitrea, &nuntium,
            &genus))
    {
        si (genus == VITREA_NUNTIUS_INTERITUS)
        {
            vitrea_recargare(atrium->visio_vitrea);
            perge;
        }
        si (atrium_nuntium_discernere(nuntium) == ATRIUM_NUNTIUS_RPC)
        {
            internuntius_tractare(atrium->visio_internuntius, nuntium,
                atrium->arena);
        }
    }
}
```

  (e) In `atrium_gressus`, FIRST inside the event loop (before the
  speculum):

```c
        /* VISIO PRIMUM: res menu atrii, non applicationis. Ceterae
         * res menu (app eas ipsa addere potest) eventori transeunt. */
        si (   eventus.genus == EVENTUS_MENU
            && atrium->visio != NIHIL
            && eventus.datum.menu.signum == (i32)ATRIUM_SIGNUM_VISIO)
        {
            si (_visionem_aperire(atrium))
            {
                actum |= (i32)ATRIUM_ACTUM_VISIO;
            }
            perge;
        }
```

  and after the main bridge loop, before `vivarium_gressus`:
  `_visionem_pulsare(atrium);`.

  (f) `atrium_destruere`: first line after the NIHIL check,
  `_visionem_claudere(atrium);` (with a comment: Visio PRIMUM, before
  the main window's vitrea).

- [ ] **Step 4: run green** (`./compile_tests.sh atrium`), then the root
  suite; formator `-scribere`/`-vitia` on `lib/atrium.c`,
  `include/atrium.h`, `probationes/probatio_atrium.c`.

- [ ] **Step 5: plant.** In the refusal, `figura->visio[0] == '\0'` →
  `figura->visio[0] == 'x'` (equal tokens). Predict **1 red**. Restore;
  confirm gone.

- [ ] **Step 6: BY HAND — the seam and the window, live.** No file of
  the tree changes; the main is patched inside a cached project only.
  From the repo root (inside the tree, so briar reads the DISK corpus
  and the project's closure carries the new atrium and fenestra):

```bash
D="$(bin/briar -struere briar/probationes/fixa/thistle/salve_vitreum.thistle | tail -1)"
python3 - "$D" <<'PY'
import sys
via = sys.argv[1] + '/fontes/salve_vitreum.c'
s = open(via).read()
a = '    atrium_vexilla_legere(&figura, argc, argv);'
b = '        (vacuum)atrium_gressus(atrium);'
assert s.count(a) == 1 and s.count(b) == 1
s = s.replace(a, '    figura.visio = "index.html";\n' + a)
s = s.replace(b, '        if (atrium_gressus(atrium) & 8) {'
    ' printf("[probatio] visio aperta\\n"); fflush(stdout); }')
open(via, 'w').write(s)
PY
/bin/sh "$D/aedificare.sh"
SESSIO="$(bin/manus incipere "$D/bin/salve_vitreum" -vivum -retro)"
PORTUS="$(printf '%s\n' "$SESSIO" | grep -oE '[0-9]{4,5}' | head -1)"
bin/manus -s "$PORTUS" clavis Cmd+Shift+v
bin/manus -s "$PORTUS" effusio | grep 'visio aperta'   # poll a few times
```

  (`visio = "index.html"` shows the app's own page in the second window —
  Task 3 supplies the real one.) Expected: `[probatio] visio aperta`.
  **Fran looks** at the running app: `Visio ⌘⇧V` above a separator and
  `Exire`; a second window "salve — visio"; ⌘⇧V again raises it; closing
  it leaves the main window running; the main page still answers
  `tange`. Then `bin/manus -s "$PORTUS" finire`. If a CLICK on Visio
  works but ⌘⇧V does not, WebKit's `performKeyEquivalent` is swallowing
  the key: stop and report before changing anything.

- [ ] **Step 7: worklog + commit.** `lib/atrium.worklog.md`, dated `##
  2026-09-14 — Visio: fenestra altera (plan 8)`: V2 (owner, optional),
  refusal order (visio before capsula, so the gate never touches AppKit),
  own piscina per opening, the drained bridge, `-retro` inherited, what
  Fran saw, the wrinkle (keys in the Visio window reach the main queue,
  so ⌘⇧D there toggles the MAIN speculum). Commit `include/atrium.h`,
  `lib/atrium.c`, `probationes/probatio_atrium.c`, the worklog; portae
  `[('radix', None)]`.

---

### Task 3: briar — the page in every vitrea build

**Files:** modify `briar/fontes/briar_fabrica.h`,
`briar/fontes/briar_fabrica.c`, `briar/fontes/briar_facies.h`,
`briar/fontes/briar_facies.c`, `briar/probationes/probatio_briar_facies.c`,
`briar/probationes/probatio_briar_fabrica.c`, `tools/briar.c`; regenerate
`briar/probationes/fixa/fabrica/salve_vitreum/fontes/salve_vitreum.c`;
append `briar/fontes/briar.worklog.md`.

**Interfaces consumed:** `ATRIUM_ACTUM_VISIO`, `AtriumConfiguratio.visio`
(Task 2 — used only inside generated text); `briar_vestem_legere`,
`briar_faciem_fingere`, `BriarVestis {involucrum, styli, scriptum,
exemplar}`; `BriarFabricaFructus {successus, forma, titulus, genitae}`,
`BriarPlagula {via, contentum}`; `briar_fabrica_clavem_computare`,
`briar_fabrica_vexilla`; `sigillum_incipere/addere/finire/hex`,
`SIGILLUM_HEX_MENSURA` (65).

**Interfaces produced:**

```c
/* briar_fabrica.h, after briar_fabricare */

/* VISIO (par. 4.9): paginam litteratam in proiectum VITREUM addere -
 * plagula genita assets/TITULUS.visio.html et nomen eius in lista
 * _files plagulae assets/TITULUS.toml. Post briar_fabricare, ante
 * briar_fabricam_scribere. FALSUM, nihil mutato, si fructus non
 * sanus, forma non vitrea, pagina vacua aut toml non inventum. */
b32
briar_visionem_addere (
                Piscina* piscina,
    BriarFabricaFructus* fructus,
                 chorda  pagina);

/* briar_facies.h, after briar_vestem_legere */

/* stampa clavis VESTITA (par. 4.9 V5): stampa + "\nfacies " + XVI hex
 * SHA-256 vestis, quaeque plagula mensura sua praefixa (octeti inter
 * plagulas migrantes clavem quoque mutant). Programma vitreum paginam
 * in binario fert: sine hoc briar cum vestibus novis binarium cum
 * pagina VETERE e cache daret. */
constans character*
briar_stampam_vestire (
                Piscina* piscina,
    constans character* stampa,
  constans BriarVestis* vestis);
```

- [ ] **Step 1: prototypes and stubs.** Add both prototypes. Stub bodies
  so the gate compiles and its reds are countable — in `briar_fabrica.c`
  under a new banner `Visio (par. 4.9)` placed before `Scriptor`:
  `(vacuum)piscina; (vacuum)fructus; (vacuum)pagina; redde FALSUM;`; in
  `briar_facies.c` after `briar_vestem_legere`:
  `(vacuum)piscina; (vacuum)vestis; redde stampa;`.
  `./briar/compile_probationes.sh facies` → green; record the assertion
  count N.

- [ ] **Step 2: extract two helpers in the facies gate** (no behaviour
  change). In `probatio_briar_facies.c`, replace `_paginam_fingere` with:

```c
/* plagulam in fructum vertere (via ut data, stampa "probatio");
 * octeti_out->datum NIHIL si plagula non lecta */
interior BriarFabricaFructus
_fructum_fingere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans SilexFons* fons,
     constans character* via,
                   Xar** nexus_out,
                   Xar** fragmenta_out,
                 chorda* octeti_out)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;

    memset(&fructus, 0, magnitudo(fructus));
    *nexus_out            = NIHIL;
    *fragmenta_out        = NIHIL;
    octeti_out->datum     = NIHIL;
    octeti_out->mensura   = ZEPHYRUM;
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde fructus;
    }
    doc         = briar_arbor_parsare(piscina, textus, mensura);
    *nexus_out  = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, *nexus_out, fragmenta_out);
    (vacuum)briar_silvam_texere(piscina, *nexus_out, fons);
    optiones.via_thistle   = via;
    optiones.stampa        = "probatio";
    optiones.fons_titulus  = "probatio";
    octeti_out->datum      = (i8*)textus;
    octeti_out->mensura    = mensura;
    redde briar_fabricare(piscina, doc, *nexus_out, fons, &optiones,
        *octeti_out);
}

/* vestis probatoria: notae solae, sine stylis, sine scripto */
interior BriarVestis
_vestem_probatoriam (
    Piscina* piscina)
{
    BriarVestis  vestis;
         chorda  vacua;

    vacua.datum        = NIHIL;
    vacua.mensura      = ZEPHYRUM;
    vestis.involucrum  = chorda_ex_literis(INVOLUCRUM_PROBATORIUM,
        piscina);
    vestis.styli       = vacua;
    vestis.scriptum    = vacua;
    vestis.exemplar    = filum_legere_totum(
        "briar/facies/md-html-facies.stml", piscina);
    CREDO_VERUM (vestis.exemplar.mensura > ZEPHYRUM);
    redde vestis;
}

/* plagulam thistle in paginam vertere; nexus/fragmenta redduntur */
interior chorda
_paginam_fingere (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans SilexFons* fons,
     constans character* via,
                   Xar** nexus_out,
                   Xar** fragmenta_out,
                 chorda* causa)
{
    BriarFabricaFructus  fructus;
            BriarVestis  vestis;
                 chorda  octeti;
                 chorda  vacua;

    vacua.datum     = NIHIL;
    vacua.mensura   = ZEPHYRUM;
    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    fructus = _fructum_fingere(piscina, intern, fons, via, nexus_out,
        fragmenta_out, &octeti);
    si (octeti.datum == NIHIL)
    {
        *causa = chorda_ex_literis("plagula non lecta", piscina);
        redde vacua;
    }
    vestis = _vestem_probatoriam(piscina);
    redde briar_faciem_fingere(piscina, intern, *nexus_out,
        *fragmenta_out, &fructus, octeti, via, &vestis, causa);
}
```

  Run `facies` → green with the SAME count N (one exemplar assertion per
  page, as before). Equal count + green goldens = the refactor is proven.

- [ ] **Step 3: the failing block**, immediately before
  `credo_imprimere_compendium();`:

```c
    imprimere("\n--- Probans visionem in proiecto (par. 4.9) ---\n");
    {
        /* pagina in proiectum vitreum addita: plagula genita octetim
         * pagina ipsa, toml eam nominat; programma planum nihil
         * accipit; clavis cum vestibus differt, etiam cum octeti
         * tantum inter plagulas vestis migrant */
                           Xar* nexus;
                           Xar* fragmenta;
                        chorda  octeti;
                        chorda  causa;
                        chorda  pagina;
           BriarFabricaFructus  vitreum;
           BriarFabricaFructus  planum;
                   BriarVestis  vestis;
                   BriarVestis  styli_mutati;
                   BriarVestis  scriptum_mutatum;
        constans BriarPlagula* genita = NIHIL;
        constans BriarPlagula* toml   = NIHIL;
           constans character* vexilla;
           constans character* via;
                          i32  numerus;
                          i32  i;
                     character  clavis_a[17];
                     character  clavis_b[17];
                     character  clavis_c[17];
                     character  clavis_d[17];

        via      = _texere(piscina, FIXA, "salve_vitreum.thistle");
        vitreum  = _fructum_fingere(piscina, intern, fons, via, &nexus,
            &fragmenta, &octeti);
        CREDO_VERUM (vitreum.successus);
        CREDO_AEQUALIS_S32 ((s32)vitreum.forma,
            (s32)BRIAR_FORMA_VITREA);
        vestis         = _vestem_probatoriam(piscina);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        pagina = briar_faciem_fingere(piscina, intern, nexus, fragmenta,
            &vitreum, octeti, via, &vestis, &causa);
        CREDO_VERUM (pagina.mensura > ZEPHYRUM);

        /* clavis: eadem vestis eadem clavis; styli mutati aliam;
         * iidem octeti in scripto aliam quoque */
        vexilla                    = briar_fabrica_vexilla(
            BRIAR_FORMA_VITREA);
        styli_mutati               = vestis;
        styli_mutati.styli         = chorda_ex_literis("body{}",
            piscina);
        scriptum_mutatum           = vestis;
        scriptum_mutatum.scriptum  = chorda_ex_literis("body{}",
            piscina);
        briar_fabrica_clavem_computare(briar_stampam_vestire(piscina,
            "probatio", &vestis), vexilla, octeti, clavis_a);
        briar_fabrica_clavem_computare(briar_stampam_vestire(piscina,
            "probatio", &vestis), vexilla, octeti, clavis_b);
        briar_fabrica_clavem_computare(briar_stampam_vestire(piscina,
            "probatio", &styli_mutati), vexilla, octeti, clavis_c);
        briar_fabrica_clavem_computare(briar_stampam_vestire(piscina,
            "probatio", &scriptum_mutatum), vexilla, octeti, clavis_d);
        CREDO_VERUM (strcmp(clavis_a, clavis_b) == ZEPHYRUM);
        CREDO_VERUM (strcmp(clavis_a, clavis_c) != ZEPHYRUM);
        CREDO_VERUM (strcmp(clavis_c, clavis_d) != ZEPHYRUM);
        CREDO_VERUM (strncmp(briar_stampam_vestire(piscina, "probatio",
            &vestis), "probatio\nfacies ", (size_t)XVI) == ZEPHYRUM);

        /* pagina in fructum */
        numerus = xar_numerus(vitreum.genitae);
        CREDO_VERUM (briar_visionem_addere(piscina, &vitreum, pagina));
        CREDO_AEQUALIS_S32 ((s32)xar_numerus(vitreum.genitae),
            (s32)(numerus + I));
        per (i = ZEPHYRUM; i < xar_numerus(vitreum.genitae); i++)
        {
            constans BriarPlagula* p = (constans BriarPlagula*)
                xar_obtinere(vitreum.genitae, i);

            si (chorda_aequalis_literis(p->via,
                "assets/salve_vitreum.visio.html"))
            {
                genita = p;
            }
            si (chorda_aequalis_literis(p->via,
                "assets/salve_vitreum.toml"))
            {
                toml = p;
            }
        }
        CREDO_VERUM (genita != NIHIL
            && chorda_aequalis(genita->contentum, pagina));
        CREDO_VERUM (toml != NIHIL && _continet(piscina, toml->contentum,
            "salve_vitreum_files = [\"index.html\","
            " \"salve_vitreum.visio.html\"]\n"));

        /* programma planum: nihil */
        planum = _fructum_fingere(piscina, intern, fons,
            _texere(piscina, FIXA, "salve.thistle"), &nexus, &fragmenta,
            &octeti);
        CREDO_VERUM (planum.successus);
        numerus = xar_numerus(planum.genitae);
        CREDO_FALSUM (briar_visionem_addere(piscina, &planum, pagina));
        CREDO_AEQUALIS_S32 ((s32)xar_numerus(planum.genitae),
            (s32)numerus);
    }
```

  (The toml golden today is `salve_vitreum_files = ["index.html"]` —
  salve_vitreum has no js/css region.)

- [ ] **Step 4: run red.** `./briar/compile_probationes.sh facies`.
  Predict **7 reds**: `a != c`, `c != d`, the `"probatio\nfacies "`
  prefix (the stub returns the stampa unchanged; `a == b` passes); the
  addition returning VERUM, the count `+1`, the genita, the toml (the
  stub adds nothing). Green: successus ×2, forma, page, `a == b`, the
  plain program's FALSUM and unchanged count.

- [ ] **Step 5: implement.** `briar_facies.c` gains `#include
  "sigillum.h"` and:

```c
constans character*
briar_stampam_vestire (
                Piscina* piscina,
    constans character* stampa,
  constans BriarVestis* vestis)
{
    SigillumContextus  ctx;
             Sigillum  s;
               chorda  partes[4];
            character  hex[SIGILLUM_HEX_MENSURA];
            character  mensura[24];
            character* exitus;
                  i32  i;
               size_t  m;

    partes[0] = vestis->involucrum;
    partes[1] = vestis->styli;
    partes[2] = vestis->scriptum;
    partes[3] = vestis->exemplar;
    sigillum_incipere(&ctx);
    per (i = ZEPHYRUM; i < (i32)4; i++)
    {
        sprintf(mensura, "%lu\n", (unsigned long)partes[i].mensura);
        sigillum_addere(&ctx, mensura, (memoriae_index)strlen(mensura));
        si (partes[i].mensura > ZEPHYRUM)
        {
            sigillum_addere(&ctx, partes[i].datum,
                (memoriae_index)partes[i].mensura);
        }
    }
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    m       = strlen(stampa);
    exitus  = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + (size_t)25));
    memcpy(exitus, stampa, m);
    memcpy(exitus + m, "\nfacies ", (size_t)8);
    memcpy(exitus + m + 8, hex, (size_t)16);
    exitus[m + 24] = '\0';
    redde exitus;
}
```

  `briar_fabrica.c`, replacing the stub:

```c
b32
briar_visionem_addere (
                Piscina* piscina,
    BriarFabricaFructus* fructus,
                 chorda  pagina)
{
    constans character* via_toml;
                   i32  i;
                   i32  k;

    si (   piscina == NIHIL || fructus == NIHIL || !fructus->successus
        || fructus->forma  != BRIAR_FORMA_VITREA
        || pagina.mensura  == ZEPHYRUM)
    {
        redde FALSUM;
    }
    via_toml = _texere(piscina, "assets/", fructus->titulus, ".toml");
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        BriarPlagula* p = (BriarPlagula*)xar_obtinere(fructus->genitae,
            i);

        si (!chorda_aequalis_literis(p->via, via_toml))
        {
            perge;
        }
        /* lista _files (_toml_fingere) ']' PRIMO clauditur */
        per (k = ZEPHYRUM; k < p->contentum.mensura; k++)
        {
            ChordaAedificator* a;

            si ((character)p->contentum.datum[k] != ']')
            {
                perge;
            }
            a = chorda_aedificator_creare(piscina,
                (memoriae_index)(p->contentum.mensura + 64));
            chorda_aedificator_appendere_chorda(a,
                chorda_sectio(p->contentum, ZEPHYRUM, k));
            chorda_aedificator_appendere_literis(a, ", \"");
            chorda_aedificator_appendere_literis(a, fructus->titulus);
            chorda_aedificator_appendere_literis(a, ".visio.html\"");
            chorda_aedificator_appendere_chorda(a,
                chorda_sectio(p->contentum, k, p->contentum.mensura));
            p->contentum = chorda_aedificator_finire(a);
            _genitam_addere(piscina, fructus->genitae,
                _texere(piscina, "assets/", fructus->titulus,
                ".visio.html"), pagina);
            redde VERUM;
        }
    }
    redde FALSUM;
}
```

- [ ] **Step 6: run green, then two plants.** (a) In
  `briar_stampam_vestire`, `partes[1] = vestis->styli;` →
  `partes[1] = vestis->scriptum;`. Predict **1 red** (`a != c`: with
  `styli` never hashed, the changed styli leave the key equal to `a`;
  `c != d` still holds). (b) In `briar_visionem_addere`, the toml
  literal `".visio.html\""` → `".visio.htm\""`. Predict **1 red** (the
  toml; the genita's own path is a separate literal). Restore each;
  confirm gone.

- [ ] **Step 7: the generated main — failing assertions.** In
  `probatio_briar_fabrica.c`, `salve_vitreum: principale genitum`, after
  the `atrium_vexilla_legere(&figura, argc, argv);` assertion:

```c
            /* VISIO (par. 4.9): via paginae ante vexilla; vexillum
             * gressus in lineam effusionis */
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "    figura.visio    = \"salve_vitreum.visio.html\";\n"
                "    atrium_vexilla_legere(&figura, argc, argv);"));
            CREDO_VERUM (_continet(piscina, princeps->contentum,
                "        si (atrium_gressus(atrium)"
                " & (i32)ATRIUM_ACTUM_VISIO)\n"
                "        {\n"
                "            imprimere(\"[salve_vitreum] visio"
                " aperta\\n\");\n"
                "            fflush(stdout);\n"));
```

  `./briar/compile_probationes.sh fabrica` → predict **2 reds**.

- [ ] **Step 8: the template.** In `_principem_vitreum_fingere`
  (`briar_fabrica.c`), exact bytes (`tolerans=False`). The capsula line's
  continuation

```c
    chorda_aedificator_appendere_literis(a,
        ";\n"
        "    atrium_vexilla_legere(&figura, argc, argv);\n"
```

  becomes

```c
    chorda_aedificator_appendere_literis(a,
        ";\n"
        "    figura.visio    = \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ".visio.html\";\n"
        "    atrium_vexilla_legere(&figura, argc, argv);\n"
```

  and the loop inside the last literal chain

```c
        "    dum (atrium_currendum(atrium))\n"
        "    {\n"
        "        (vacuum)atrium_gressus(atrium);\n"
        "    }\n"
        "    atrium_destruere(atrium);\n"
```

  becomes

```c
        "    dum (atrium_currendum(atrium))\n"
        "    {\n"
        "        si (atrium_gressus(atrium)"
        " & (i32)ATRIUM_ACTUM_VISIO)\n"
        "        {\n"
        "            imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] visio aperta\\n\");\n"
        "            fflush(stdout);\n"
        "        }\n"
        "    }\n"
        "    atrium_destruere(atrium);\n"
```

  Run `fabrica` → predict **1 red**: the two structural assertions pass,
  the golden of `fontes/salve_vitreum.c` prints `DIFFERT`.
  `xar_numerus(f.genitae) == 9` still holds (the fabrica adds no page;
  `tools/briar.c` does).

- [ ] **Step 9: regenerate the golden with its cause.**
  `BRIAR_FABRICA_SCRIBERE=1 ./briar/compile_probationes.sh fabrica`, then
  `git status --short briar/probationes/fixa/fabrica` → exactly ONE file
  (`salve_vitreum/fontes/salve_vitreum.c`); `git diff` on it → exactly
  the visio line and the loop body. Anything else: stop. Rerun without
  the variable → green.

- [ ] **Step 10: `tools/briar.c`.** (a) Local, after `character
  clavis[17];`: `BriarVestis vestis_visionis;`. (b) The key — after

```c
    stampa = e_disco ? briar_stampa_clausurae(piscina, fructus.clausura)
                     : fons->titulus;
```

  insert:

```c
    /* VISIO (par. 4.9): programma vitreum paginam suam in binario
     * fert, ergo vestis clavem intrat (V5) - briar cum vestibus novis
     * aedificat, paginam veterem e cache non reddit */
    si (fructus.forma == BRIAR_FORMA_VITREA)
    {
        chorda causa;

        si (!briar_vestem_legere(piscina, &capsula_facies_briar,
            &vestis_visionis, &causa))
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        stampa = briar_stampam_vestire(piscina, stampa,
            &vestis_visionis);
    }
```

  (c) In the write block, before `si (!briar_fabricam_scribere(…))`:

```c
        /* pagina SOLUM cum proiectum scribitur: ictus cache nihil
         * reddit. Optiones eaedem ac '-html' (briar_optiones_plagulae),
         * ergo pagina eadem PER CONSTRUCTIONEM - fumus IV id probat */
        si (fructus.forma == BRIAR_FORMA_VITREA)
        {
            chorda pagina;

            causa.datum    = NIHIL;
            causa.mensura  = ZEPHYRUM;
            pagina = briar_faciem_fingere(piscina, intern, nexus,
                fragmenta, &fructus, octeti, optiones.via_thistle,
                &vestis_visionis, &causa);
            si (   pagina.mensura == ZEPHYRUM
                || !briar_visionem_addere(piscina, &fructus, pagina))
            {
                fprintf(stderr, "briar: visio non reddita: %.*s\n",
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
                redde I;
            }
        }
```

- [ ] **Step 11: build and look.** `./briar/compile_probationes.sh
  registrum && ./tools/briar_struere.sh` (the corpus now carries Tasks
  1–2). From the repo root:

```bash
D="$(bin/briar -struere briar/probationes/fixa/thistle/salve_vitreum.thistle | tail -1)"
ls -l "$D/assets/"                     # salve_vitreum.visio.html present
cat "$D/assets/salve_vitreum.toml"     # lists it
grep -n 'visio' "$D/fontes/salve_vitreum.c"
time bin/briar -struere briar/probationes/fixa/thistle/salve_vitreum.thistle   # same dir, a hit: no render
```

  `capsula_generare` must accept the two-dot file name (the build
  succeeds). **Fran's look:** `./project-specs/exempla/kalendarium.thistle`
  → Visio in its app menu, the literate page in a second window beside
  the calendar (a symbol click shows the declaration panel's island
  text), ⌘⇧V raises it, closing it leaves kalendarium running. Fran's own
  `project-specs/exempla/kalendarium.app` is rewritten with `-app` ONLY
  if Fran says so. Record the page and binary sizes.

- [ ] **Step 12: suites, format, worklog, commit.** Briar suite (16
  files) and root suite green; formator on the touched `.c/.h` (not the
  golden). `briar/fontes/briar.worklog.md`, dated `## 2026-09-14 — visio
  in applicatione, opus III (plan 8)`: the key composition and why
  length-prefixed, render-on-write only, the toml splice, the helper
  extraction, the golden's cause, what Fran saw. Commit the eight files +
  golden + worklog; message names the golden cause ("aurum
  salve_vitreum.c regeneratum: figura.visio et 'visio aperta', par.
  4.9"); portae `[('radix', None), ('briar', None)]`.

---

### Task 4: fumus, measurements, seal

**Files:** modify `tools/briar_fumus.sh`, `project-specs/briar-spec.md`,
`briar/CLAUDE.md`, this plan's header, `briar/fontes/briar.worklog.md`;
memory `briar-project.md`, `atrium-manus-stack.md`, `MEMORY.md`; ledger.

- [ ] **Step 1: rebake.** `./briar/compile_probationes.sh registrum`,
  `./tools/briar_struere.sh`, `./tools/briar_spectator_struere.sh` (an
  atrium app: new atrium), `./tools/silex_struere.sh` (the corpus is
  shared). Record `bin/briar` size.

- [ ] **Step 2: stage IV** (`silva.Textus` on `tools/briar_fumus.sh`).
  Echo line `(+ -probatio)` → `(+ visio, -probatio)`. After the
  `binarium vitreum abest` check, before `-probatio`:

```sh
# visio (par. 4.9): pagina in proiecto, octetim pagina quam '-html' in
# eadem area scribit - duo cursus binarii installati, nulla fides
[ -s "$VITREUM_DIR/assets/salve_vitreum.visio.html" ] \
    || deficere "pagina visionis in proiecto abest: $VITREUM_DIR/assets" "$AREA/vitreum.err"
grep -q 'salve_vitreum.visio.html' "$VITREUM_DIR/assets/salve_vitreum.toml" \
    || deficere "toml capsulae paginam visionis non nominat" "$VITREUM_DIR/assets/salve_vitreum.toml"
PAGINA_VITREA="$( cd "$AREA" && ./salve_vitreum.thistle -html 2>"$AREA/vitreum_html.err" | tail -1 )" \
    || deficere "salve_vitreum -html defecit" "$AREA/vitreum_html.err"
cmp -s "$VITREUM_DIR/assets/salve_vitreum.visio.html" "$PAGINA_VITREA" \
    || deficere "pagina visionis a pagina '-html' differt ($PAGINA_VITREA)" "$AREA/vitreum_html.err"
echo "FUMUS:    visio: $( wc -c < "$PAGINA_VITREA" | tr -d ' ' ) octeti, eadem ac '-html'"
```

- [ ] **Step 3: stage XIV.** After the `CORPUS=…` line, before
  `sublevare`:

```sh
# visio (par. 4.9): Cmd+Shift+v per menu applicationis -> fenestra
# altera; principale genitum 'visio aperta' imprimit (atrium tacet)
"$RADIX/bin/manus" -s "$PORTUS" clavis Cmd+Shift+v > "$AREA/visio.log" 2>&1 \
    || { sublevare; deficere "clavis Cmd+Shift+v defecit" "$AREA/visio.log"; }
k=0
until "$RADIX/bin/manus" -s "$PORTUS" effusio 2>>"$AREA/visio.log" | grep -q 'visio aperta'; do
    k=$((k + 1))
    if [ "$k" -ge 20 ]; then
        sublevare
        deficere "effusio 'visio aperta' non continet (res menu Visio non pressa?)" "$AREA/visio.log"
    fi
    sleep 0.25
done
echo "FUMUS:    visio aperta (Cmd+Shift+v)"
```

  The closing `FACTUM (…)` line gains `visio`.

- [ ] **Step 4: headless run + plant.** `./tools/briar_fumus.sh` →
  green; record the time. Plant in `tools/briar.c`'s page render:
  `optiones.via_thistle` → `imp.via` (equal tokens; the relative path
  lands in the page header — exactly the drift
  `briar_optiones_plagulae` exists to prevent). Rebuild
  `./tools/briar_struere.sh`; predict the fumus FAILS at stage IV with
  `pagina visionis a pagina '-html' differt` and the briar suite stays
  green (it cannot see `tools/briar.c`). Restore, rebuild, green.

- [ ] **Step 5: `-agere` run + plant.** `./tools/briar_fumus.sh -agere`
  → green through XVI; record the time. Plant in `lib/atrium.c`:
  `actum |= (i32)ATRIUM_ACTUM_VISIO;` → `actum |= (i32)ATRIUM_ACTUM_NIHIL;`
  (the root suite cannot see it — no window — which is why XIV exists).
  Rebuild (`registrum` + `briar_struere.sh`); predict I–XIII green and
  FAIL at XIV with `effusio 'visio aperta' non continet`. Restore,
  rebuild, `-agere` green again.

- [ ] **Step 6: seal the documents.**
  - `project-specs/briar-spec.md`: §4.9 gains an **As built** paragraph
    (the five "beyond the spec" items, sizes, fumus times, both plants'
    results); §8 P9 → **DONE** with the four commit hashes.
  - `briar/CLAUDE.md`: a **Visio** bullet beside `-app` (every vitrea
    build carries `assets/<t>.visio.html`, rendered at write time with
    the embedded chrome, whose hash joins the key through
    `briar_stampam_vestire`; `briar_visionem_addere` adds the file and
    the toml entry; the generated main sets `figura.visio` and prints
    `[<t>] visio aperta`; atrium owns ⌘⇧V and the second window); fumus
    IV/XIV in the gate paragraph.
  - This plan's header: DONE + commits. `briar/fontes/briar.worklog.md`:
    the fumus entry.
  - Memory: `briar-project.md` (plan 8 sealed, the numbers),
    `atrium-manus-stack.md` (the menu seam, `visio`, the ⌘⇧D wrinkle),
    the `MEMORY.md` briar line.
  - Ledger: a nota for plan 8 sealed (`mcp__tabularii__addere`, genus
    nota).

- [ ] **Step 7: commit** `tools/briar_fumus.sh`,
  `project-specs/briar-spec.md`, `briar/CLAUDE.md`,
  `project-specs/briar-plan-8-visio.md`, `briar/fontes/briar.worklog.md`;
  portae `[('radix', None), ('briar', None), ('briar-fumus', None)]`.

---

## Spec coverage (self-review against §4.9)

| §4.9 | where |
|---|---|
| V1 page as an asset in the app's own capsula | T3 Steps 5, 10 |
| V2 one optional atrium field, `NIHIL` = untouched | T2 |
| V3 every vitrea build, automatically | T3 Steps 8, 10 |
| V4 Visio bridge without methods | T2 Step 3(d) |
| V5 chrome in the key of vitrea programs | T3 Steps 3–6, 10 |
| fenestra seam: signature, above separator + Exire, `EVENTUS_MENU` appended, `datum.menu.signum`, queued on the adding window, switch check | T1 |
| atrium: ⌘⇧V, consumed in gressus, title and 1100×860 closable/resizable/centred, same capsula (embedded or `-radix`), raise, drain every tick, vitrea-before-fenestra on close, app keeps running, flag 8, destroyed first, prints nothing | T2 Step 3 |
| briar: vestis read for vitrea, render on write only, file + toml entry, main sets `figura.visio` and prints, `-app` unchanged, plain programs nothing | T3 Steps 3, 8, 10 |
| gates: briar asset + key; fabrica golden; fumus IV; fumus XIV; by hand | T3 Steps 3–9, T4 Steps 2–5, T2 Step 6, T3 Step 11 |
| the ⌘⇧D wrinkle recorded | T2 Step 7, T4 Step 6 |
