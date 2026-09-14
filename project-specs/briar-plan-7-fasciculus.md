# briar plan 7 — fasciculus (`-app`: a thistle as a double-clickable `.app`)

> **For agentic workers:** execute task by task with the executing-plans
> discipline (test first, run red, implement, run green, plant, commit).
> Steps use checkbox syntax. Written 2026-09-12 from briar-spec.md §4.8
> (v1.9, approved by Fran the same day, commit `b32016c9`). DONE
> 2026-09-14 (T1 d84399c5, T2 f5c7c2de, T3 200592be, T4 dbec0b2f, T5
> bd0e1831, default icon af2c7fa8, T6 fumus + seal; spec §8 P8).

**Goal:** `briar -app x.thistle` (or `./x.thistle -app`) builds the
program and writes `x.app` beside the thistle, with an icon from
`-icon <via>`, a `<briar icon="…">` attribute, or the embedded
`default-thistle.jpg`.

**Architecture:** no new library logic. **fasciculum** writes the
bundle, **icones** writes the `.icns`, and briar embeds the default icon
the way it embeds its page chrome. One new briar module,
`briar_fasciculum`, holds a pure `consilium` (identity, name, version,
icon source, output path) and a writer that takes an already-decoded
`Imago`. Image decoding (`stb_image` through `lib/imago.c`) lives only in
`tools/briar.c`, so no briar gate links a decoder. Two sealed libraries
gain one function each.

**Tech stack:** C89 in Latin (`latina.h`), briar modules, fasciculum,
icones, plist, filum, capsula, credo gates, `tools/briar_fumus.sh`,
`bin/manus` for the `-agere` stage, `plutil` and `iconutil` as oracles.

**Spec:** `project-specs/briar-spec.md` §4.8 (decisions A1–A8, the
pipeline, the two calls, the library changes, gates), §5 (flag rows),
§6 (gate row), §7 (module row), §8 (P8), §9 (deferred), §10 (risks).

## Global constraints

- C89 under `tools/vexilla.sh` flags. Latin identifiers; latina macros
  are forbidden identifiers (`nomen`, `magnitudo`, `interior`, …);
  single capitals are Roman numerals. `chorda` is not NUL-terminated;
  `i32`/`i64` are UNSIGNED.
- New C files through `./silva/scribe.sh <via> <<'EOF'`; edits to C
  through `silva.Editio(via).replace(vetus, novus, numerus=1)` (token
  anchors; comment anchors `tolerans='verba'`; a multi-line anchor
  needs a multi-line `novus`). Then `./silva/formator.sh <viae>
  -scribere` and `-vitia`: no NEW divergence (record the baseline
  first — several of these files carry old ones).
- Lines ≤ 72 columns; banners are 50 `=`.
- Commit only with explicit paths through `silva.commissio(msg, viae,
  portae)`. Library tasks gate on `[('radix', None)]`; briar tasks on
  `[('radix', None), ('briar', None)]`; the fumus task adds
  `('briar-fumus', None)`. Verify each gate by its log's mtime against
  the commit time.
- Every new gate is born red; every fix is proven by a PLANTED FAULT
  whose red count is PREDICTED before the run and read after; the plant
  is confirmed gone (`grep -c` = 0).
- **Never plant a fault that removes the `/` refusal of the tree
  removal** (Task 2): a planted fault there deletes the disk.
- Words checked: fasciculus, fasciculum, consilium, arborem, delere,
  icon, vexilli, identitas, versio, app, valida, signa, vestitum,
  alienus are all in the lexicon. Check any other new word with
  `./oratio/quaere.sh -machina <verbum>` before writing it.
- Line conventions (measured): for an STML element (`<fenestra>`,
  `<briar>`) `BriarNexusRes.linea_initium` IS the tag's line; for a raw
  region the tag's line is `linea_initium - I`.

## File structure

| file | responsibility |
|---|---|
| `include/fasciculum.h`, `lib/fasciculum.c` | `FASCICULUM_ERROR_IDENTITAS` + public `fasciculum_identitas_valida` (T1) |
| `tools/fasciculum_instrumentum.c` | the new status's name (T1) |
| `probationes/probatio_fasciculum.c` | block H7 (T1) |
| `include/filum.h`, `lib/filum.c`, `probationes/probatio_filum.c` | `filum_arborem_delere` (T2) |
| `briar/fontes/briar_fasciculum.h/.c` (new) | consilium (T3), writer (T4) |
| `briar/probationes/probatio_briar_fasciculum.c` (new) | the gate (T3, T4) |
| `briar/probationes/fixa/thistle/app_vestitum.thistle`, `adversa/app_identitas_prava.thistle`, `adversa/app_briar_duplex.thistle` (new) | fixtures, listed in `fixa/FONTES.md` (T3) |
| `briar/compile_probationes.sh` | library objects (T3, T4) |
| `briar/fontes/briar_imperium.h/.c`, `briar/probationes/probatio_briar_imperium.c` | `-app`, `-icon <via>` (T5) |
| `briar/icon/default-thistle.jpg` (moved from the repo root), `tools/briar_icon_capsula.sh` (new), `.gitignore` | the embedded default (T5) |
| `tools/briar.c`, `tools/briar_struere.sh` | dispatch, decode, link (T5) |
| `tools/briar_fumus.sh` | headless stage XIII, `-agere` stage XVI (T6) |
| spec §4.8 as built + §8 P8, `briar/CLAUDE.md`, worklogs, memory, ledger | seal (T6) |

---

### Task 1: fasciculum judges the identity's characters

**Files:** modify `include/fasciculum.h`, `lib/fasciculum.c`,
`tools/fasciculum_instrumentum.c`, `probationes/probatio_fasciculum.c`,
`lib/fasciculum.worklog.md`.

**Interfaces produced:**

```c
/* in include/fasciculum.h, appended LAST to FasciculumStatus */
    FASCICULUM_ERROR_IDENTITAS   /* characteres extra A-Z a-z 0-9 . - */

/* CFBundleIdentifier: litterae, cifrae, '.' et '-' SOLAE (Apple).
 * Vacua VERUM reddit - absentia DESUNT est, non characteres pravi.
 * Publica ut briar eandem regulam UNO loco adhibeat. */
b32
fasciculum_identitas_valida (
    chorda identitas);
```

- [ ] **Step 1: header + CLI name.** Append the status after
  `FASCICULUM_ERROR_MEMORIA` (add the comma). Add the prototype after
  `fasciculum_reddere`. In `tools/fasciculum_instrumentum.c`,
  `_titulus_status` gains `casus FASCICULUM_ERROR_IDENTITAS: redde
  "IDENTITAS";` — without it `-Wswitch` stops the tool's build. Add a
  stub in `lib/fasciculum.c` so the suite compiles:

```c
b32
fasciculum_identitas_valida (
    chorda identitas)
{
    (vacuum)identitas;
    redde VERUM;
}
```

- [ ] **Step 2: the failing test.** In `probationes/probatio_fasciculum.c`,
  insert immediately before the line `/* ---- H2: claves Info.plist et
  directio fusionis ---- */`:

```c
    /* ---- H7: identitas characteribus iudicata ---- */
    {
              Fasciculum  f;
                     Xar* partes;
        FasciculumStatus  status;
                  chorda  sedes;

        imprimere("\n--- H7: identitas iudicata ---\n");

        f = _fasciculum_minimum(via_binarii, piscina);
        f.identitas = chorda_ex_literis("org.rhubarb.salve_vitreum",
                                        piscina);
        CREDO_FALSUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_S32((s32)status,
                           (s32)FASCICULUM_ERROR_IDENTITAS);
        CREDO_CHORDA_AEQUALIS_LITERIS(sedes,
                                      "org.rhubarb.salve_vitreum");

        f.identitas = chorda_ex_literis("org.rhubarb.salve vitreum",
                                        piscina);
        CREDO_FALSUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                        piscina));
        CREDO_AEQUALIS_S32((s32)status,
                           (s32)FASCICULUM_ERROR_IDENTITAS);

        /* gemellus positivus: '-', '.', cifrae licita */
        f.identitas = chorda_ex_literis(
            "org.rhubarb.briar.salve-vitreum2", piscina);
        CREDO_VERUM(fasciculum_reddere(&f, &partes, &status, &sedes,
                                       piscina));
        CREDO_AEQUALIS_S32((s32)status, (s32)FASCICULUM_SUCCESSUS);

        CREDO_VERUM(fasciculum_identitas_valida(
            chorda_ex_literis("a.B-9", piscina)));
        CREDO_FALSUM(fasciculum_identitas_valida(
            chorda_ex_literis("a/b", piscina)));
    }

```

- [ ] **Step 3: run red.** `./compile_tests.sh fasciculum`. Predict
  **6 reds**: case one 3 (reddere VERUM, status SUCCESSUS, sedes empty),
  case two 2, and the `a/b` check 1 (the stub says VERUM). Positive twin
  and `a.B-9` stay green.

- [ ] **Step 4: implement.** Replace the stub:

```c
b32
fasciculum_identitas_valida (
    chorda identitas)
{
    i32 i;

    per (i = ZEPHYRUM; i < identitas.mensura; i++)
    {
        character c = (character)identitas.datum[i];

        si (!(   (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
              || (c >= '0' && c <= '9') || c == '.' || c == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
```

  and in `fasciculum_reddere`, directly after the empty-identity
  refusal:

```c
    /* Nomen pravum a Launch Services TACITE fallit: hic nominatur */
    si (!fasciculum_identitas_valida(f->identitas))
    {
        redde _recusare(status, sedes_vitii, FASCICULUM_ERROR_IDENTITAS,
                        f->identitas);
    }
```

- [ ] **Step 5: run green**, then `./compile_tools.sh
  fasciculum_instrumentum` must build.

- [ ] **Step 6: plant.** Through `silva.planta`, add `|| c == '_'` to the
  accepted characters. Predict **3 reds** (case one only; the space
  case and the `a/b` check still refuse). Confirm the plant is gone.

- [ ] **Step 7: worklog + commit.** `lib/fasciculum.worklog.md`: why
  (found designing briar `-app`; a bad identity fails silently in
  Launch Services), the rule, the plant. Commit the five files, portae
  `[('radix', None)]`.

---

### Task 2: filum removes a directory tree without following links

**Files:** modify `include/filum.h`, `lib/filum.c`,
`probationes/probatio_filum.c`, `lib/filum.worklog.md`.

**Interfaces produced:**

```c
/* Delere ARBOREM directorii (rm -r) SINE nexibus symbolicis sequendis.
 *
 * CUR NON directorium_ambulare: id stat() vocat, ergo nexum ad
 * directorium ut directorium tractat et IN EUM descendit - arbor aliena
 * extra viam datam deleretur. Hic lstat(): nexus UT NEXUS removetur,
 * destinatio eius intacta manet.
 *
 * Recusat NIHIL, viam vacuam et "/". Via non exsistens = VERUM (nihil
 * delendum). Filum aut nexus = ipse removetur. Directorium relegitur
 * donec vacuum sit (readdir post unlink introitus omittere potest).
 * FALSUM ad primum defectum; quod iam deletum est manet deletum.
 * POSIX solum (sub _WIN32 recusat). */
b32
filum_arborem_delere (
    constans character* via);
```

- [ ] **Step 1: prototype + stub.** Prototype after `filum_delere` in
  `include/filum.h`; in `lib/filum.c` a stub after `filum_delere` that
  returns `FALSUM` (so the test compiles and runs red):

```c
b32
filum_arborem_delere (
    constans character* via)
{
    (vacuum)via;
    redde FALSUM;
}
```

- [ ] **Step 2: the failing test.** `probationes/probatio_filum.c`: add
  `#include "postulata_posix.h"` as the FIRST include, after the opening
  comment (for `symlink`) and
  `#include <unistd.h>` after `<string.h>`. Insert this block
  immediately before the file's final `credo_imprimere_compendium();`
  section. Paths are fixed and unique to this block; the pre-clean
  uses `filum_delere` (plain `remove`), never the function under test.

```c
    /* ==================================================
     * Probare filum_arborem_delere (nexum NON sequitur)
     * ================================================== */

    {
        constans character* radix  = "/tmp/test_rhubarb_arbor";
        constans character* aliena = "/tmp/test_rhubarb_arbor_aliena";
        constans character* testis =
            "/tmp/test_rhubarb_arbor_aliena/testis.txt";
        constans character* planum = "/tmp/test_rhubarb_arbor_filum.txt";

        imprimere("\n--- Probans filum_arborem_delere ---\n");

        /* reliquiae cursus prioris, a fundo sursum */
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub/subsub/c.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub/subsub");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub/b.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/sub");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/a.txt");
        (vacuum)filum_delere("/tmp/test_rhubarb_arbor/nexus");
        (vacuum)filum_delere(radix);
        (vacuum)filum_delere(testis);
        (vacuum)filum_delere(aliena);
        (vacuum)filum_delere(planum);

        /* arbor: a.txt, sub/b.txt, sub/subsub/c.txt, nexus -> aliena */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            "/tmp/test_rhubarb_arbor/sub/subsub"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/a.txt", "a"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/sub/b.txt", "b"));
        CREDO_VERUM(filum_scribere_literis(
            "/tmp/test_rhubarb_arbor/sub/subsub/c.txt", "c"));
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(aliena));
        CREDO_VERUM(filum_scribere_literis(testis, "testis"));
        CREDO_VERUM(symlink(aliena, "/tmp/test_rhubarb_arbor/nexus")
                    == ZEPHYRUM);

        CREDO_VERUM(filum_arborem_delere(radix));
        CREDO_FALSUM(filum_directorium_existit(radix));
        /* destinatio nexus INTACTA - hoc est ius functionis */
        CREDO_VERUM(filum_existit(testis));
        /* iterum: nihil delendum = VERUM */
        CREDO_VERUM(filum_arborem_delere(radix));

        CREDO_FALSUM(filum_arborem_delere(NIHIL));
        CREDO_FALSUM(filum_arborem_delere(""));
        CREDO_FALSUM(filum_arborem_delere("/"));

        CREDO_VERUM(filum_scribere_literis(planum, "planum"));
        CREDO_VERUM(filum_arborem_delere(planum));
        CREDO_FALSUM(filum_existit(planum));

        (vacuum)filum_delere(testis);
        (vacuum)filum_delere(aliena);
    }

```

- [ ] **Step 3: run red.** `./compile_tests.sh filum`. Predict **5
  reds** against the stub: first delete (FALSUM), tree still exists,
  second delete, the plain-file delete, and the plain file still
  existing. The alien witness and the three refusals stay green.

- [ ] **Step 4: implement.** In `lib/filum.c`, add inside the
  non-`_WIN32` branch of the platform headers
  `#include <dirent.h>` and `#include <limits.h>`. Replace the stub:

```c
b32
filum_arborem_delere (
    constans character* via)
{
#ifdef _WIN32
    (vacuum)via;
    _filum_error_ponere("filum_arborem_delere: POSIX solum");
    redde FALSUM;
#else
    structura stat  st;
               b32  deleta;

    si (!via || via[0] == '\0' || strcmp(via, "/") == ZEPHYRUM)
    {
        _filum_error_ponere("via NIHIL, vacua aut radix");
        redde FALSUM;
    }
    _filum_error_purgare();
    si (lstat(via, &st) != ZEPHYRUM)
    {
        redde (errno == ENOENT) ? VERUM : FALSUM;
    }
    si (!S_ISDIR(st.st_mode))
    {
        /* filum aut NEXUS: ipse removetur, numquam destinatio */
        si (unlink(via) != ZEPHYRUM)
        {
            _filum_error_ponere("unlink fracta");
            redde FALSUM;
        }
        redde VERUM;
    }
    fac
    {
                   DIR* d;
        structura dirent* e;
              character  filius[PATH_MAX];

        deleta  = FALSUM;
        d       = opendir(via);
        si (d == NIHIL)
        {
            _filum_error_ponere("opendir fracta");
            redde FALSUM;
        }
        dum ((e = readdir(d)) != NIHIL)
        {
            integer n;

            si (   strcmp(e->d_name, ".") == ZEPHYRUM
                || strcmp(e->d_name, "..") == ZEPHYRUM)
            {
                perge;
            }
            n = snprintf(filius, magnitudo(filius), "%s/%s", via,
                         e->d_name);
            si (n < ZEPHYRUM || (size_t)n >= magnitudo(filius))
            {
                closedir(d);
                _filum_error_ponere("via nimis longa");
                redde FALSUM;
            }
            si (!filum_arborem_delere(filius))
            {
                closedir(d);
                redde FALSUM;
            }
            deleta = VERUM;
        }
        closedir(d);
    } dum (deleta);
    si (rmdir(via) != ZEPHYRUM)
    {
        _filum_error_ponere("rmdir fracta");
        redde FALSUM;
    }
    redde VERUM;
#endif
}
```

- [ ] **Step 5: run green.** Then the whole root suite once
  (`./compile_tests.sh`), since `filum.c` is linked everywhere.

- [ ] **Step 6: plant.** Through `silva.planta`, replace `lstat(via,
  &st)` with `stat(via, &st)`. Predict **3 reds**: the first delete
  (it descends through `nexus`, empties the alien directory, then
  `rmdir` on the link fails), the tree still existing, and the witness
  gone. The second delete of the leftover tree also fails, so read the
  count: 3 or 4 both mean the link was followed; anything else means a
  different defect. Restore, confirm gone. **Do not plant anything in
  the `/` refusal.**

- [ ] **Step 7: worklog + commit.** `lib/filum.worklog.md`: why
  (briar `-app` replaces its own bundle), the `stat` vs `lstat`
  finding in `iter_directoria.c`, why the directory is re-read until
  empty. Commit the four files, portae `[('radix', None)]`.

---

### Task 3: `briar_fasciculum` — the consilium (pure) and its fixtures

**Files:** create `briar/fontes/briar_fasciculum.h`,
`briar/fontes/briar_fasciculum.c`,
`briar/probationes/probatio_briar_fasciculum.c`,
`briar/probationes/fixa/thistle/app_vestitum.thistle`,
`briar/probationes/fixa/thistle/adversa/app_identitas_prava.thistle`,
`briar/probationes/fixa/thistle/adversa/app_briar_duplex.thistle`;
modify `briar/compile_probationes.sh`, `briar/probationes/fixa/FONTES.md`.

**Interfaces consumed:** `fasciculum_identitas_valida` (Task 1);
`briar_nexus_titulus_est`, `briar_nexus_attributum`,
`briar_nexus_attributum_habet`, `BRIAR_NEXUS_STML` (`briar_nexus.h`);
`briar_fabrica_titulus` (`briar_fabrica.h`: file stem, characters
outside `[A-Za-z0-9_]` become `_`); `via_directorium` (`"foo.txt"` →
`"."`), `via_iungere`, `via_est_absoluta` (`via.h`).

**Interfaces produced:**

```c
/* briar_fasciculum.h - '-app': thistle in fasciculum .app (spec par.
 * 4.8).
 *
 * Consilium PURUM: ex nexu (elementa <briar> et <fenestra>) et via
 * plagulae omnia decernit quae sine disco decerni possunt - identitas,
 * nomen, versio, fons iconis, via fasciculi. Scriptor (Opus 4) pixela
 * IAM decodificata accipit: decodificatio in tools/briar.c vivit, ergo
 * nulla porta briar stb_image nectit (icones D7). */

#ifndef BRIAR_FASCICULUM_H
#define BRIAR_FASCICULUM_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"

#define BRIAR_FASCICULUM_PRAEFIXUM "org.rhubarb.briar."

nomen structura {
    chorda  identitas;   /* A4: characteres iudicati */
    chorda  titulus;     /* A5: <fenestra titulus> aut nomen plagulae */
    chorda  versio;      /* A5: <briar versio>; vacua = ordinaria */
    chorda  via_icon;    /* A7 resoluta; vacua = icon infixus */
    chorda  via_app;     /* <directorium plagulae>/<t>.app */
       i32  linea_briar; /* linea tagi <briar>; ZEPHYRUM si abest */
} BriarFasciculumConsilium;

/* VERUM + consilium; FALSUM + causa + linea (tag <briar>) */
b32
briar_fasciculum_consilium (
                     Piscina* piscina,
                         Xar* nexus,
          constans character* via_thistle,
          constans character* via_icon_vexilli,
    BriarFasciculumConsilium* consilium,
                      chorda* causa,
                         i32* linea_causae);

#endif /* BRIAR_FASCICULUM_H */
```

- [ ] **Step 1: fixtures.** Line numbers are pinned; the gate asserts
  them. `app_vestitum.thistle` (the `<briar>` tag on line 8):

```
#!/usr/bin/env briar
# App vestitum

Fixum portae fasciculi (plan 7, Opus 3): elementum briar cum omnibus
attributis. Lineae pinnatae - ne reflue.

Consilium nexum solum legit; icon non exsistere debet.
<briar identitas="org.exemplum.vestitum" versio="2.1" icon="signa/vestitum.png"/>

<fenestra titulus="Vestitum" latitudo="320" altitudo="200"/>

<html!>
<h1>vestitum</h1>
</html>
```

  `adversa/app_identitas_prava.thistle` (tag on line 4):

```
#!/usr/bin/env briar
# App identitas prava

<briar identitas="org.exemplum.vitium_malum"/>

<fenestra titulus="prava"/>
```

  `adversa/app_briar_duplex.thistle` (tags on lines 4 and 6):

```
#!/usr/bin/env briar
# App briar duplex

<briar versio="1"/>

<briar versio="2"/>
```

  Add three rows to `fixa/FONTES.md` in its table's style (plagula,
  forma, partes, nota "plan 7, Opus 3").

- [ ] **Step 2: header + stub.** Write the header above through
  `./silva/scribe.sh`. `briar_fasciculum.c` starts as a stub that zeroes
  the consilium, sets `causa` to `"non implementatum"`, sets the line to
  `ZEPHYRUM` and returns `FALSUM`. In `briar/compile_probationes.sh`,
  add to `RADIX_FONTES` after `"silex"`:

```bash
    # briar -app (spec par. 4.8): fasciculum et quae trahit
    "xml"
    "base64"
    "plist"
    "fasciculum"
```

  If a link error names a missing symbol, find its library with
  `./silva/nexus.sh <symbol>` and add it to the same list.

- [ ] **Step 3: the failing gate.** `probatio_briar_fasciculum.c`:

```c
/* probatio_briar_fasciculum.c - '-app' (spec par. 4.8) SINE fenestra
 * et SINE decodificatore: consilium ex nexu fixorum (Opus 3), scriptor
 * super Imago ficta (Opus 4). */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_fasciculum.h"
#include "briar_nexus.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* FIXA =
    "briar/probationes/fixa/thistle/";

/* fixum legere, nexum texere, consilium reddere */
interior b32
_consilium (
                     Piscina* piscina,
         InternamentumChorda* intern,
          constans character* titulus,
          constans character* via_icon,
    BriarFasciculumConsilium* c,
                      chorda* causa,
                         i32* linea)
{
    constans character* via = chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(FIXA, piscina),
        chorda_ex_literis(titulus, piscina), piscina), piscina);
                 chorda textus = filum_legere_totum(via, piscina);
          MateriaNodus* doc;
                   Xar* nexus;

    *linea = ZEPHYRUM;
    si (textus.mensura == ZEPHYRUM)
    {
        *causa = chorda_ex_literis("fixum non lectum", piscina);
        redde FALSUM;
    }
    doc    = briar_arbor_parsare(piscina,
        (constans character*)textus.datum, (i32)textus.mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    redde briar_fasciculum_consilium(piscina, nexus, via, via_icon, c,
        causa, linea);
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_briar_fasciculum",
        (memoriae_index)4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans consilium: salve_vitreum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern,
            "salve_vitreum.thistle", NIHIL, &c, &causa, &linea));
        /* '_' in '-': identitas characteres suos non habet */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve-vitreum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
        CREDO_CHORDA_VACUA(c.versio);
        CREDO_CHORDA_VACUA(c.via_icon);
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_app,
            "briar/probationes/fixa/thistle/salve_vitreum.app");
        CREDO_AEQUALIS_I32(c.linea_briar, ZEPHYRUM);
    }

    imprimere("\n--- Probans consilium: salve (sine fenestra) ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "salve.thistle", NIHIL,
            &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.rhubarb.briar.salve");
        /* nomen e plagula, nulla fenestra */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "salve");
    }

    imprimere("\n--- Probans consilium: <briar> vestitum ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            NIHIL, &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.titulus, "Vestitum");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.versio, "2.1");
        /* icon attributi ad directorium PLAGULAE resolvitur */
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon,
            "briar/probationes/fixa/thistle/signa/vestitum.png");
        CREDO_AEQUALIS_I32(c.linea_briar, VIII);

        /* vexillum -icon attributum vincit, et intactum manet */
        CREDO_VERUM(_consilium(piscina, intern, "app_vestitum.thistle",
            "alibi/icon.png", &c, &causa, &linea));
        CREDO_CHORDA_AEQUALIS_LITERIS(c.via_icon, "alibi/icon.png");
        CREDO_CHORDA_AEQUALIS_LITERIS(c.identitas,
            "org.exemplum.vestitum");
    }

    imprimere("\n--- Probans recusationes cum linea ---\n");
    {
        BriarFasciculumConsilium c;
                          chorda causa;
                             i32 linea;

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_identitas_prava.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, IV);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("vitium_malum", piscina));

        CREDO_FALSUM(_consilium(piscina, intern,
            "adversa/app_briar_duplex.thistle", NIHIL, &c, &causa,
            &linea));
        CREDO_AEQUALIS_I32(linea, VI);
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("linea 4", piscina));
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

- [ ] **Step 4: run red.** `./briar/compile_probationes.sh fasciculum`.
  25 assertions. Predict **20 reds** against the stub: salve_vitreum 4
  (VERUM, identitas, titulus, via_app; the empty and zero checks pass),
  salve 3, vestitum 9, each refusal 2 (line and cause; FALSUM passes).

- [ ] **Step 5: implement** `briar_fasciculum.c`:

```c
/* briar_fasciculum.c - Vide briar_fasciculum.h (spec par. 4.8). */

#include "briar_fasciculum.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "chorda_aedificator.h"
#include "fasciculum.h"
#include "via.h"
#include <string.h>

interior b32
_fasciculi_recusare (
                  Piscina* piscina,
                  chorda* causa,
                     i32* linea_causae,
    ChordaAedificator* textus,
                     i32  linea)
{
    (vacuum)piscina;
    *causa         = chorda_aedificator_finire(textus);
    *linea_causae  = linea;
    redde FALSUM;
}

b32
briar_fasciculum_consilium (
                     Piscina* piscina,
                         Xar* nexus,
          constans character* via_thistle,
          constans character* via_icon_vexilli,
    BriarFasciculumConsilium* consilium,
                      chorda* causa,
                         i32* linea_causae)
{
         constans BriarNexusRes* elementum_briar = NIHIL;
         constans BriarNexusRes* fenestra        = NIHIL;
             constans character* titulus_plagulae;
              ChordaAedificator* aed;
                          chorda  vacua;
                          chorda  directorium;
                          chorda  partes[II];
                             i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    memset(consilium, ZEPHYRUM, magnitudo(*consilium));
    *causa         = vacua;
    *linea_causae  = ZEPHYRUM;

    per (i = ZEPHYRUM; nexus != NIHIL && i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r =
            (constans BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->genus != BRIAR_NEXUS_STML)
        {
            perge;
        }
        si (briar_nexus_titulus_est(r, "briar"))
        {
            si (elementum_briar != NIHIL)
            {
                aed = chorda_aedificator_creare(piscina,
                    (memoriae_index)96);
                chorda_aedificator_appendere_literis(aed,
                    "elementum <briar> iteratum (primum linea ");
                chorda_aedificator_appendere_s32(aed,
                    (s32)elementum_briar->linea_initium);
                chorda_aedificator_appendere_literis(aed, ")");
                redde _fasciculi_recusare(piscina, causa, linea_causae,
                    aed, r->linea_initium);
            }
            elementum_briar = r;
        }
        alioquin si (briar_nexus_titulus_est(r, "fenestra"))
        {
            fenestra = r;
        }
    }

    titulus_plagulae = briar_fabrica_titulus(piscina, via_thistle);

    /* A4: identitas - attributum, alioquin praefixum + nomen */
    si (   elementum_briar != NIHIL
        && briar_nexus_attributum_habet(elementum_briar, "identitas"))
    {
        consilium->identitas = briar_nexus_attributum(elementum_briar,
            "identitas");
    }
    alioquin
    {
        constans character* p;

        aed = chorda_aedificator_creare(piscina, (memoriae_index)64);
        chorda_aedificator_appendere_literis(aed,
            BRIAR_FASCICULUM_PRAEFIXUM);
        per (p = titulus_plagulae; *p != '\0'; p++)
        {
            chorda_aedificator_appendere_character(aed,
                (*p == '_') ? '-' : *p);
        }
        consilium->identitas = chorda_aedificator_finire(aed);
    }
    si (   chorda_vacua(consilium->identitas)
        || !fasciculum_identitas_valida(consilium->identitas))
    {
        aed = chorda_aedificator_creare(piscina, (memoriae_index)128);
        chorda_aedificator_appendere_literis(aed, "identitas '");
        chorda_aedificator_appendere_chorda(aed, consilium->identitas);
        chorda_aedificator_appendere_literis(aed,
            "': characteres soli A-Z a-z 0-9 . -");
        redde _fasciculi_recusare(piscina, causa, linea_causae, aed,
            elementum_briar != NIHIL ? elementum_briar->linea_initium
                                     : ZEPHYRUM);
    }

    /* A5: nomen et versio */
    si (fenestra != NIHIL)
    {
        consilium->titulus = briar_nexus_attributum(fenestra,
            "titulus");
    }
    si (chorda_vacua(consilium->titulus))
    {
        consilium->titulus = chorda_ex_literis(titulus_plagulae,
            piscina);
    }
    si (elementum_briar != NIHIL)
    {
        consilium->versio = briar_nexus_attributum(elementum_briar,
            "versio");
        consilium->linea_briar = elementum_briar->linea_initium;
    }

    /* A7: vexillum (ad cwd) > attributum (ad plagulam) > infixus */
    directorium = via_directorium(chorda_ex_literis(via_thistle,
        piscina), piscina);
    si (via_icon_vexilli != NIHIL)
    {
        consilium->via_icon = chorda_ex_literis(via_icon_vexilli,
            piscina);
    }
    alioquin si (   elementum_briar != NIHIL
                 && briar_nexus_attributum_habet(elementum_briar,
                        "icon"))
    {
        chorda icon = briar_nexus_attributum(elementum_briar, "icon");

        si (via_est_absoluta(icon))
        {
            consilium->via_icon = icon;
        }
        alioquin
        {
            partes[ZEPHYRUM]     = directorium;
            partes[I]            = icon;
            consilium->via_icon  = via_iungere(partes, II, piscina);
        }
    }

    /* A3: iuxta plagulam */
    partes[ZEPHYRUM]    = directorium;
    partes[I]           = chorda_concatenare(
        chorda_ex_literis(titulus_plagulae, piscina),
        chorda_ex_literis(".app", piscina), piscina);
    consilium->via_app  = via_iungere(partes, II, piscina);
    redde VERUM;
}
```

  C89: the declaration `chorda icon = …` sits at the head of its own
  block, and `constans character* p;` at the head of its block — keep
  both there if the formatter moves anything.

- [ ] **Step 6: run green**, then the whole briar suite
  (`./briar/compile_probationes.sh`): no other gate reads the new
  fixtures by name, and none globs the fixture folders (checked
  2026-09-12), so the rest must be unchanged.

- [ ] **Step 7: plants**, each predicted, run, restored and confirmed
  gone:
  - `'-'` → `'_'` in the default identity: the identity becomes
    invalid, so the consilium REFUSES — predict **4 reds** (salve_vitreum:
    VERUM, identitas, titulus, via_app; salve has no underscore).
  - resolve the icon attribute without the directory
    (`consilium->via_icon = icon;` in both arms): **1 red**.
  - drop the duplicate refusal (let the second `<briar>` win): **3
    reds** (FALSUM, line, cause).
  - drop the validity check: **3 reds** (the prava block).

- [ ] **Step 8: format, lint, commit.** `./silva/formator.sh` on the
  three C files; `./oratio/vocabula.sh -nova` shows no new word. Commit
  the new files plus `compile_probationes.sh` and `FONTES.md`, portae
  `[('radix', None), ('briar', None)]`.

---

### Task 4: `briar_fasciculum_scribere` — the writer over a decoded `Imago`

**Files:** modify `briar/fontes/briar_fasciculum.h`,
`briar/fontes/briar_fasciculum.c`,
`briar/probationes/probatio_briar_fasciculum.c`,
`briar/compile_probationes.sh`.

**Interfaces consumed:** `filum_arborem_delere` (Task 2); `icones_reddere`,
`icones_icns_scribere`, `Icones`, `IconesFructus`, `ICONES_SUCCESSUS`,
`ICONES_RECIDERE_CENTRUM` (`icones.h`; `latera_petita = ZEPHYRUM` means
Apple's whole set); `fasciculum_reddere`, `fasciculum_scribere(partes,
<the .app path>, …)`, `fasciculum_legere(via, f, status, sedes, piscina,
intern)` (returns `exsecutabile`/`icon` as paths INSIDE the bundle and
refuses a bundle whose executable is missing); `Imago` (`imago_typus.h`:
`pixela` RGBA, `latitudo`, `altitudo`).

**Interfaces produced** (appended to `briar_fasciculum.h`, with
`#include "imago_typus.h"` beside the other includes):

```c
/* Scribere consilium->via_app: icon -> <domus>/fasciculum/<t>.icns
 * (icones; <t> = nomen exsecutabilis), deinde fasciculus. Fasciculus
 * EXSISTENS reponitur SOLUM si proprius est (identitas Info.plist
 * aequalis); alienus aut plagula quae fasciculus non est recusatur et
 * intacta manet (A6). FALSUM + causa. */
b32
briar_fasciculum_scribere (
                               Piscina* piscina,
    constans BriarFasciculumConsilium* consilium,
                       constans Imago* icon,
                   constans character* exsecutabile,
                   constans character* domus,
                               chorda* causa);
```

- [ ] **Step 1: stub + objects.** A stub in `briar_fasciculum.c` sets
  `*causa` to `"non implementatum"` and returns `FALSUM`. In
  `briar/compile_probationes.sh` add after `"fasciculum"`:

```bash
    "imago_opus"
    "imago_png"
    "icones"
```

  (`flatura` is already listed.) `imago` is NOT added: no briar gate
  links `stb_image`.

- [ ] **Step 2: the failing gate.** Add `#include "fasciculum.h"` to
  the probatio's includes and insert before `credo_imprimere_compendium();`:

```c
    imprimere("\n--- Probans scriptorem: proprius, alienus, planum ---\n");
    {
        constans character* area =
            "/tmp/probatio_briar_fasciculum";
        constans character* exsecutabile =
            "/tmp/probatio_briar_fasciculum/bin/salve";
        constans character* domus =
            "/tmp/probatio_briar_fasciculum/domus";
        constans character* app =
            "/tmp/probatio_briar_fasciculum/Salve.app";
        constans character* aliena =
            "/tmp/probatio_briar_fasciculum/Alienum.app";
        constans character* planum =
            "/tmp/probatio_briar_fasciculum/Planum.app";
        constans character* stala =
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/"
            "Resources/vetus.txt";
        BriarFasciculumConsilium c;
                      Fasciculum lectus;
                FasciculumStatus status;
                          chorda sedes;
                          chorda causa;
                          chorda icns;
                           Imago imago;
                             i32 k;

        (vacuum)filum_arborem_delere(area);
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            "/tmp/probatio_briar_fasciculum/bin"));
        CREDO_VERUM(filum_scribere_literis(exsecutabile,
            "#!/bin/sh\n"));
        CREDO_VERUM(filum_modum_ponere(exsecutabile, 0755));

        /* imago ficta LXIV x LXIV: nullus decodificator in porta */
        imago.latitudo  = LXIV;
        imago.altitudo  = LXIV;
        imago.pixela    = (i8*)piscina_allocare(piscina,
            (memoriae_index)(LXIV * LXIV * IV));
        per (k = ZEPHYRUM; k < LXIV * LXIV * IV; k++)
        {
            imago.pixela[k] = (i8)0x7F;
        }

        memset(&c, ZEPHYRUM, magnitudo(c));
        c.identitas  = chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        c.titulus    = chorda_ex_literis("Salve", piscina);
        c.versio     = chorda_ex_literis("2.1", piscina);
        c.via_app    = chorda_ex_literis(app, piscina);

        /* I. scriptio prima */
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_VACUA(causa);
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.rhubarb.briar.salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.titulus, "Salve");
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.versio, "2.1");
        CREDO_VERUM(filum_existit(
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/MacOS/"
            "salve"));
        icns = filum_legere_totum(
            "/tmp/probatio_briar_fasciculum/Salve.app/Contents/"
            "Resources/salve.icns", piscina);
        CREDO_VERUM(icns.mensura > VIII);
        CREDO_VERUM(icns.mensura > VIII
            && memcmp(icns.datum, "icns", (size_t)IV) == ZEPHYRUM);

        /* II. PROPRIUS reponitur: plagula stala evanescit */
        CREDO_VERUM(filum_scribere_literis(stala, "vetus"));
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_FALSUM(filum_existit(stala));
        CREDO_VERUM(fasciculum_legere(app, &lectus, &status, &sedes,
            piscina, intern));

        /* III. ALIENUS recusatur et intactus manet */
        c.identitas  = chorda_ex_literis("org.aliud.alienum", piscina);
        c.via_app    = chorda_ex_literis(aliena, piscina);
        CREDO_VERUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        c.identitas  = chorda_ex_literis("org.rhubarb.briar.salve",
            piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("alienus", piscina));
        CREDO_VERUM(fasciculum_legere(aliena, &lectus, &status, &sedes,
            piscina, intern));
        CREDO_CHORDA_AEQUALIS_LITERIS(lectus.identitas,
            "org.aliud.alienum");

        /* IV. plagula quae fasciculus non est: recusata, intacta */
        CREDO_VERUM(filum_scribere_literis(planum, "non fasciculus"));
        c.via_app = chorda_ex_literis(planum, piscina);
        CREDO_FALSUM(briar_fasciculum_scribere(piscina, &c, &imago,
            exsecutabile, domus, &causa));
        CREDO_CHORDA_CONTINET(causa,
            chorda_ex_literis("neque fasciculus", piscina));
        CREDO_VERUM(filum_existit(planum)
            && !filum_directorium_existit(planum));

        (vacuum)filum_arborem_delere(area);
    }

```

- [ ] **Step 3: run red.** The new block has 25 assertions (3 of setup
  that pass). Predict **17 reds** against the stub: block I 9, II 3
  (the stale file cannot be written into a bundle that does not exist;
  its absence check passes), III 4 (the refusal itself passes), IV 1
  (the cause). Count before running.

- [ ] **Step 4: implement** the writer in `briar_fasciculum.c` (add
  `#include "fasciculum.h"` — already there — plus `"filum.h"`,
  `"icones.h"`, `"internamentum.h"`):

```c
interior b32
_scriptoris_recusare (
               Piscina* piscina,
               chorda* causa,
    constans character* textus,
                chorda  sedes,
                   s32  status)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)160);

    chorda_aedificator_appendere_literis(aed, textus);
    si (status >= ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, " (status ");
        chorda_aedificator_appendere_s32(aed, status);
        chorda_aedificator_appendere_literis(aed, ")");
    }
    si (sedes.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, ": ");
        chorda_aedificator_appendere_chorda(aed, sedes);
    }
    *causa = chorda_aedificator_finire(aed);
    redde FALSUM;
}

b32
briar_fasciculum_scribere (
                               Piscina* piscina,
    constans BriarFasciculumConsilium* consilium,
                       constans Imago* icon,
                   constans character* exsecutabile,
                   constans character* domus,
                               chorda* causa)
{
                 Icones  petitio;
          IconesFructus  fructus;
           IconesStatus  st_icon;
             Fasciculum  f;
             Fasciculum  vetus;
       FasciculumStatus  st_fasc;
                 chorda  sedes;
                 chorda  vacua;
                 chorda  titulus_exs;
                 chorda  directorium;
                 chorda  partes_viae[II];
                   Xar* partes;
     constans character* via_icns;
     constans character* via_app;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *causa         = vacua;
    via_app        = chorda_ut_cstr(consilium->via_app, piscina);
    titulus_exs    = via_nomen(chorda_ex_literis(exsecutabile, piscina),
        piscina);

    /* I. pixela -> .icns in domo proiecti */
    memset(&petitio, ZEPHYRUM, magnitudo(Icones));
    petitio.fons           = icon;
    petitio.titulus        = titulus_exs;
    petitio.recidere       = ICONES_RECIDERE_CENTRUM;
    petitio.latera_petita  = ZEPHYRUM;
    si (!icones_reddere(&petitio, &fructus, &st_icon, &sedes, piscina))
    {
        redde _scriptoris_recusare(piscina, causa, "icon reddi non potuit",
            sedes, (s32)st_icon);
    }
    partes_viae[ZEPHYRUM]  = chorda_ex_literis(domus, piscina);
    partes_viae[I]         = chorda_ex_literis("fasciculum", piscina);
    directorium            = via_iungere(partes_viae, II, piscina);
    si (!filum_directorium_creare_cum_parentibus(
            chorda_ut_cstr(directorium, piscina)))
    {
        redde _scriptoris_recusare(piscina, causa,
            "directorium iconis creari non potuit", directorium, -I);
    }
    partes_viae[ZEPHYRUM]  = directorium;
    partes_viae[I]         = chorda_concatenare(titulus_exs,
        chorda_ex_literis(".icns", piscina), piscina);
    via_icns = chorda_ut_cstr(via_iungere(partes_viae, II, piscina),
        piscina);
    si (!icones_icns_scribere(&fructus, via_icns, &st_icon, &sedes,
            piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "icon scribi non potuit", sedes, (s32)st_icon);
    }

    /* II. planum fasciculi (purum) */
    memset(&f, ZEPHYRUM, magnitudo(Fasciculum));
    f.identitas     = consilium->identitas;
    f.titulus       = consilium->titulus;
    f.versio        = consilium->versio;
    f.exsecutabile  = exsecutabile;
    f.icon          = via_icns;
    si (!fasciculum_reddere(&f, &partes, &st_fasc, &sedes, piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "planum fasciculi recusatum", sedes, (s32)st_fasc);
    }

    /* III. exsistens: PROPRIUS reponitur, cetera recusantur (A6) */
    si (filum_existit(via_app) || filum_directorium_existit(via_app))
    {
        InternamentumChorda* intern = internamentum_creare(piscina);

        si (   !filum_directorium_existit(via_app)
            || !fasciculum_legere(via_app, &vetus, &st_fasc, &sedes,
                                  piscina, intern))
        {
            redde _scriptoris_recusare(piscina, causa,
                "exsistit neque fasciculus est - non tangitur",
                consilium->via_app, -I);
        }
        si (!chorda_aequalis(vetus.identitas, consilium->identitas))
        {
            redde _scriptoris_recusare(piscina, causa,
                "fasciculus alienus est - non tangitur (identitas)",
                vetus.identitas, -I);
        }
        si (!filum_arborem_delere(via_app))
        {
            redde _scriptoris_recusare(piscina, causa,
                "fasciculus vetus deleri non potuit", consilium->via_app,
                -I);
        }
    }

    /* IV. scribere */
    si (!fasciculum_scribere(partes, via_app, &st_fasc, &sedes, piscina))
    {
        redde _scriptoris_recusare(piscina, causa,
            "fasciculus scribi non potuit", sedes, (s32)st_fasc);
    }
    redde VERUM;
}
```

- [ ] **Step 5: run green**, then the whole briar suite.

- [ ] **Step 6: plants**, predicted, run, restored, confirmed gone:
  - skip the identity comparison (`si (FALSUM && …)`): **3 reds**
    (block III: the refusal, its cause, the alien identity — the bundle
    now reads back as ours).
  - skip the deletion before writing: **1 red** (the stale file
    survives; fasciculum never deletes).
  - treat an existing plain file as absent (drop the
    `!filum_directorium_existit` arm): **1 red** (block IV's cause —
    the write still fails, which is exactly why the cause is asserted).

- [ ] **Step 7: format, lint, commit** the four files, portae
  `[('radix', None), ('briar', None)]`.

---

### Task 5: the binary — `-app`, `-icon`, the embedded default, the link

**Files:** modify `briar/fontes/briar_imperium.h`,
`briar/fontes/briar_imperium.c`,
`briar/probationes/probatio_briar_imperium.c`, `tools/briar.c`,
`tools/briar_struere.sh`, `.gitignore`; create
`tools/briar_icon_capsula.sh`; move `default-thistle.jpg` (repo root,
untracked, Fran's) to `briar/icon/default-thistle.jpg`.

**Interfaces consumed:** `briar_fasciculum_consilium`,
`briar_fasciculum_scribere` (Tasks 3–4); `imago_caricare_ex_file(via,
piscina)`, `imago_caricare_ex_memoria(datum, mensura, piscina)` →
`ImagoFructus { successus, imago, error }` (`imago.h`);
`capsula_aperire(&capsula_icon_briar, piscina)`, `capsula_legere(capsula,
via, piscina)` → `CapsulaFructus { status, datum }` (`capsula.h`).

**Interfaces produced:** `BRIAR_ACTIO_APP` (inserted before
`BRIAR_ACTIO_VERSIO`) and `constans character* icon;` on `BriarImperium`
(`-icon <via>` aut NIHIL).

- [ ] **Step 1: failing imperium assertions.** Add the enum value and
  the `icon` field to `briar_imperium.h` first so the test compiles. In
  `probatio_briar_imperium.c` (add `#include <string.h>` if absent),
  after the `-visio` assertions — the helper `_legere(piscina, &imp, n,
  a1, a2, a3)` takes at most three arguments, so the four-argument case
  builds its own `argv`:

```c
    /* briar -app x.thistle; shebang ./x.thistle -app (par. 4.8) */
    CREDO_VERUM (_legere(piscina, &imp, II, "-app", "x.thistle",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_APP);
    CREDO_NIHIL (imp.icon);
    CREDO_VERUM (_legere(piscina, &imp, II, "./x.thistle", "-app",
        NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_APP);
    /* -icon sine -app recusatur, et -app nominatur */
    CREDO_FALSUM (_legere(piscina, &imp, III, "-icon", "signum.png",
        "x.thistle"));
    CREDO_CHORDA_CONTINET (imp.causa,
        chorda_ex_literis("-app", piscina));
    /* -icon sine via recusatur */
    CREDO_FALSUM (_legere(piscina, &imp, III, "-app", "-icon",
        "x.thistle"));
    CREDO_CHORDA_CONTINET (imp.causa,
        chorda_ex_literis("-icon sine via", piscina));
    {
        constans character* argv_icon[6];

        argv_icon[0] = "briar";
        argv_icon[1] = "-app";
        argv_icon[2] = "-icon";
        argv_icon[3] = "signum.png";
        argv_icon[4] = "x.thistle";
        argv_icon[5] = NIHIL;
        CREDO_VERUM (briar_imperium_legere(piscina, V, argv_icon,
            &imp));
        CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_APP);
        CREDO_VERUM (imp.icon != NIHIL
            && strcmp(imp.icon, "signum.png") == ZEPHYRUM);
    }
```

  Run `./briar/compile_probationes.sh imperium`. 12 new assertions;
  predict **8 reds**: `-app x.thistle` is an unknown flag (VERUM and
  action red; `imp.icon` NIHIL passes); `./x.thistle -app` parses with
  `-app` as a program argument (VERUM passes, action red); `-icon …` is
  refused as unknown (FALSUM passes) but the message does not name
  `-app` (red); `-app -icon` is refused on `-app` (FALSUM passes), cause
  lacks `-icon sine via` (red); the argv block, 3 reds.

- [ ] **Step 2: implement the flags** in `briar_imperium.c`:
  `_actio_vexilli` gains `si (_est(a, "-app")) { redde
  (s32)BRIAR_ACTIO_APP; }`; the before-file loop gains, beside `-f`:

```c
        alioquin si (_est(a, "-icon"))
        {
            si (   i + I >= argc || argv[i + I][0] == '-'
                || strstr(argv[i + I], ".thistle") != NIHIL)
            {
                _recusare(imp, piscina, "-icon sine via");
                redde FALSUM;
            }
            imp->icon  = argv[i + I];
            i          = i + I;
        }
```

  the unknown-flag message's list gains `-app -icon <via>`; and after
  section II (the reserved first argument, so the shebang `-app` is
  seen):

```c
    si (imp->icon != NIHIL && imp->actio != BRIAR_ACTIO_APP)
    {
        _recusare(imp, piscina, "-icon solum cum -app");
        redde FALSUM;
    }
```

  Update the header comment's flag list. Run green. **Plant:** drop the
  `-icon solum cum -app` check — predict **2 reds** (the refusal and its
  cause). Restore, confirm gone.

- [ ] **Step 3: the embedded default.** `mkdir -p briar/icon && mv
  default-thistle.jpg briar/icon/default-thistle.jpg`. Create
  `tools/briar_icon_capsula.sh`:

```bash
#!/bin/bash
# tools/briar_icon_capsula.sh - icon ordinarius '-app' -> capsula infixa
#
# build/capsula_icon_briar.{c,h} e briar/icon/default-thistle.jpg.
# Bloccus COMMUNIS cum briar_struere.sh (exemplar:
# tools/briar_facies_capsula.sh). Separata a capsula faciei: spectator
# iconem numquam ostendit, ergo eam non vehat (spec par. 4.8).
#
# Non compressa: JPEG iam compressum est (386 KB).
briar_icon_capsula_regenerare () {
    local TABULA="build/capsula_icon_briar.c"
    local ICON="briar/icon/default-thistle.jpg"

    mkdir -p build
    if [ -f "$TABULA" ] && ! [ "$ICON" -nt "$TABULA" ]; then
        return 0
    fi
    cat > icon_briar.toml <<'TOML'
# GENERATUM a tools/briar_icon_capsula.sh - NE MANU EDITES (gitignoratum)
icon_briar_files = ["briar/icon/default-thistle.jpg"]
icon_briar_compress = false
TOML
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || return 1
    fi
    ./bin/capsula_generare icon_briar.toml || return 1
    mv capsula_icon_briar.h capsula_icon_briar.c build/ || return 1
    rm -f icon_briar.toml
    return 0
}
```

  `.gitignore` gains `capsula_icon_briar.h`, `capsula_icon_briar.c`,
  `icon_briar.toml` beside the facies lines.

- [ ] **Step 4: the link.** `tools/briar_struere.sh`, after the facies
  capsula block:

```bash
source "$SCRIPT_DIR/briar_icon_capsula.sh"
briar_icon_capsula_regenerare || exit 1
if [ ! -f build/imago.o ]; then
    echo "briar_struere: build/imago.o abest - ./compile_tests.sh prius" >&2
    exit 1
fi
```

  and the clang line gains `build/capsula_icon_briar.c build/imago.o`
  after `build/capsula_corpus_silicis.c`.

- [ ] **Step 5: the dispatch** in `tools/briar.c`. Includes gain
  `"briar_fasciculum.h"` and `"imago.h"`; beside the other capsula
  declaration:

```c
/* icon ordinarius (build/capsula_icon_briar.c, par. 4.8): symbolum
 * contractus est, caput genitum non includitur */
externus constans CapsulaEmbed capsula_icon_briar;
```

  A helper before `principale`:

```c
/* -app (par. 4.8): consilium, icon DECODIFICATUS, fasciculus. Hic
 * SOLUM stb_image vivit (per imago.h): moduli briar pixela accipiunt,
 * numquam vias imaginum (icones D7). */
interior s32
_fasciculum_facere (
                  Piscina* piscina,
                      Xar* nexus,
    constans BriarImperium* imp,
       constans character* via_thistle,
       constans character* domus,
       constans character* binarium)
{
    BriarFasciculumConsilium  consilium;
                      chorda  causa;
                         i32  linea = ZEPHYRUM;
                ImagoFructus  icon;

    si (!briar_fasciculum_consilium(piscina, nexus, via_thistle,
            imp->icon, &consilium, &causa, &linea))
    {
        fprintf(stderr, "%s:%d: %.*s\n", imp->via, (integer)linea,
            (integer)causa.mensura, (constans character*)causa.datum);
        redde I;
    }
    si (chorda_vacua(consilium.via_icon))
    {
              Capsula* capsula = capsula_aperire(&capsula_icon_briar,
                  piscina);
        CapsulaFructus  lectum;

        si (capsula == NIHIL)
        {
            fprintf(stderr, "briar: capsula iconis non aperta\n");
            redde I;
        }
        lectum = capsula_legere(capsula,
            "briar/icon/default-thistle.jpg", piscina);
        si (lectum.datum.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "briar: icon ordinarius in capsula deest\n");
            redde I;
        }
        icon = imago_caricare_ex_memoria(lectum.datum.datum,
            lectum.datum.mensura, piscina);
    }
    alioquin
    {
        icon = imago_caricare_ex_file(
            chorda_ut_cstr(consilium.via_icon, piscina), piscina);
    }
    si (!icon.successus)
    {
        fprintf(stderr, "briar: icon non decodificatus: %.*s: %.*s\n",
            (integer)consilium.via_icon.mensura,
            (constans character*)consilium.via_icon.datum,
            (integer)icon.error.mensura,
            (constans character*)icon.error.datum);
        redde I;
    }
    si (!briar_fasciculum_scribere(piscina, &consilium, &icon.imago,
            binarium, domus, &causa))
    {
        fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    imprimere("%.*s\n", (integer)consilium.via_app.mensura,
        (constans character*)consilium.via_app.datum);
    redde ZEPHYRUM;
}
```

  In `principale`, directly after the block that ends `si (imp.actio ==
  BRIAR_ACTIO_STRUERE) { imprimere("%s\n", dir); redde ZEPHYRUM; }` —
  so the build has already happened:

```c
    si (imp.actio == BRIAR_ACTIO_APP)
    {
        redde _fasciculum_facere(piscina, nexus, &imp,
            optiones.via_thistle, dir, binarium);
    }
```

  `_auxilium` gains `  -app        <t>.app iuxta thistle scribere
  (fasciculus cum icone)` and `  -icon <via> cum -app: fons iconis`.
  `optiones.via_thistle` is absolute (`briar_optiones_plagulae`), so the
  printed path is absolute, as `-html`'s is.

- [ ] **Step 6: build and look.** `./briar/compile_probationes.sh`
  (all green), `./tools/briar_struere.sh`; record `bin/briar`'s size
  (10.7 MB before). Then by hand, inside the repo:

```bash
cd project-specs/exempla && briar -app kalendarium.thistle
plutil -p kalendarium.app/Contents/Info.plist
open kalendarium.app
```

  Expected: the path printed; `CFBundleIdentifier` =
  `org.rhubarb.briar.kalendarium`; the window opens from Finder. **Fran
  looks at the icon** (Dock and Finder). Run `briar -app` a second time:
  it must replace silently. Then `briar -app -icon
  ../../probationes/fixa/icones/fons_256.png kalendarium.thistle` and
  look again — if Finder shows the OLD icon, that is A8's measurement:
  record it, and add a timestamp update only then. Remove
  `kalendarium.app` afterwards (it is not committed).

- [ ] **Step 7: format, lint, commit** the imperium files, `tools/briar.c`,
  both scripts, `.gitignore` and `briar/icon/default-thistle.jpg`, portae
  `[('radix', None), ('briar', None)]`.

---

### Task 6: fumus stages and the seal

**Files:** modify `tools/briar_fumus.sh`, `briar/CLAUDE.md`,
`briar/fontes/briar.worklog.md`, `project-specs/briar-spec.md` (§4.8 as
built, §8 P8); memory (`briar-project.md`, `MEMORY.md`); a ledger nota.

`tools/briar_fumus.sh` already carries an uncommitted header edit from
the spectator session (stages XI–XIV documented). **Confirm with Fran
before committing it together with this task.**

- [ ] **Step 1: prerequisites.** Beside the existing checks, exit 2
  naming the cure when absent: `bin/fasciculum_instrumentum`
  (`./compile_tools.sh fasciculum_instrumentum`), `plutil`, `iconutil`;
  copy `probationes/fixa/icones/fons_256.png` into `$AREA`.

- [ ] **Step 2: headless stage XIII** (after XII; renumber the two
  `-agere` stages XIII→XIV and XIV→XV in the header and their `echo`
  lines):

```bash
# ---- XIII. fasciculus: -app (par. 4.8; nihil aperit) ----
echo "FUMUS: XIII. ./salve_vitreum.thistle -app (fasciculus cum icone)"
APP="$( cd "$AREA" && ./salve_vitreum.thistle -app 2>"$AREA/app.err" | tail -1 )" \
    || deficere "salve_vitreum -app defecit" "$AREA/app.err"
case "$APP" in
    */salve_vitreum.app) ;;
    *) deficere "fasciculus non iuxta thistle: [$APP]" "$AREA/app.err" ;;
esac
[ -d "$APP" ] || deficere "fasciculus abest: $APP" "$AREA/app.err"
ID="$(plutil -extract CFBundleIdentifier raw "$APP/Contents/Info.plist" 2>/dev/null)"
[ "$ID" = "org.rhubarb.briar.salve-vitreum" ] \
    || deficere "identitas [$ID], exspectata org.rhubarb.briar.salve-vitreum" "$APP/Contents/Info.plist"
[ -x "$APP/Contents/MacOS/salve_vitreum" ] \
    || deficere "exsecutabile in fasciculo abest" "$AREA/app.err"
ICNS="$APP/Contents/Resources/salve_vitreum.icns"
# iconutil rc=0 etiam in continente fracto (icones par. 12.3): NUMERUS
iconutil -c iconset "$ICNS" -o "$AREA/ordinarius.iconset" >/dev/null 2>&1
N_ORD="$(ls "$AREA/ordinarius.iconset" 2>/dev/null | wc -l | tr -d ' ')"
[ "$N_ORD" = 10 ] \
    || deficere "icon ordinarius (MXXIV px): X plagulae exspectatae, inventae $N_ORD" "$AREA/app.err"

# proprius REPONITUR: plagula stala evanescit
touch "$APP/Contents/Resources/vetus.txt"
( cd "$AREA" && ./salve_vitreum.thistle -app ) > "$AREA/app2.log" 2>&1 \
    || deficere "-app iteratum (fasciculus proprius) defecit" "$AREA/app2.log"
[ -f "$APP/Contents/Resources/vetus.txt" ] \
    && deficere "plagula stala superest - fasciculus non repositus" "$AREA/app2.log"

# -icon: forma vexilli (CCLVI px -> VII plagulae)
( cd "$AREA" && "$BRIAR" -app -icon fons_256.png salve_vitreum.thistle ) \
    > "$AREA/app_icon.log" 2>&1 || deficere "-app -icon defecit" "$AREA/app_icon.log"
iconutil -c iconset "$ICNS" -o "$AREA/proprius.iconset" >/dev/null 2>&1
N_PROP="$(ls "$AREA/proprius.iconset" 2>/dev/null | wc -l | tr -d ' ')"
[ "$N_PROP" = 7 ] \
    || deficere "icon CCLVI px: VII plagulae exspectatae, inventae $N_PROP" "$AREA/app_icon.log"

# ALIENUS recusatur et intactus manet
mkdir -p "$AREA/alienum" && cp "$AREA/salve_vitreum.thistle" "$AREA/alienum/"
"$RADIX/bin/fasciculum_instrumentum" -radix "$AREA/alienum/salve_vitreum.app" \
    -identitas org.aliud.alienum -exsecutabile /bin/echo > "$AREA/alienum.log" 2>&1 \
    || deficere "fasciculus alienus creari non potuit" "$AREA/alienum.log"
if ( cd "$AREA/alienum" && "$BRIAR" -app salve_vitreum.thistle ) > "$AREA/alienum_app.log" 2>&1; then
    deficere "fasciculus ALIENUS repositus est" "$AREA/alienum_app.log"
fi
grep -q 'alienus' "$AREA/alienum_app.log" \
    || deficere "recusatio 'alienus' non nominat" "$AREA/alienum_app.log"
[ "$(plutil -extract CFBundleIdentifier raw "$AREA/alienum/salve_vitreum.app/Contents/Info.plist")" = org.aliud.alienum ] \
    || deficere "fasciculus alienus mutatus est" "$AREA/alienum_app.log"
echo "FUMUS:    fasciculus: $ID, icon $N_ORD/$N_PROP plagulae, proprius repositus, alienus recusatus"
```

  The 256 px count of 7 is reasoned from the codes a 256 px source
  covers (ic04 ic11 ic05 ic12 ic07 ic13 ic08); if the first run
  measures otherwise, find out why before pinning.

- [ ] **Step 3: `-agere` stage XVI** (after XV):

```bash
# ---- XVI. agere: fasciculus per Launch Services aperitur ----
echo "FUMUS: XVI. open salve_vitreum.app (Launch Services)"
PORTUS_APP=18765
open -n "$APP" --args -vivum -portus "$PORTUS_APP" -retro \
    || deficere "open fasciculi defecit"
k=0
until "$RADIX/bin/manus" -s "$PORTUS_APP" affordantiae -machina > "$AREA/app_afford.tsv" 2>/dev/null; do
    k=$((k + 1))
    [ "$k" -ge 20 ] && deficere "fasciculus apertus in portu $PORTUS_APP non respondit"
    sleep 0.5
done
N_APP="$(wc -l < "$AREA/app_afford.tsv" | tr -d ' ')"
"$RADIX/bin/manus" -s "$PORTUS_APP" finire >/dev/null 2>&1
[ "$N_APP" -ge 1 ] || deficere "fasciculus apertus: affordantiae vacuae"
echo "FUMUS:    fasciculus apertus, affordantiae $N_APP"
echo "FUMUS:    ASPICE MANU (A8): icon in Dock et Finder - post '-icon' NOVUS?"
```

  Both FACTUM lines gain "fasciculus". `open --args` reaching atrium's
  `-portus` is unmeasured: if the port never answers, record it and
  drive the bundle's executable directly with `bin/manus incipere`,
  keeping `open` for the look.

- [ ] **Step 4: calibrate the fumus.** `./tools/briar_fumus.sh` green.
  Then plant Task 4's "skip the deletion" fault, rebuild
  (`./briar/compile_probationes.sh registrum && ./tools/briar_struere.sh`),
  run the fumus: it must fail at "plagula stala superest". Restore,
  rebuild, green again.

- [ ] **Step 5: by hand.** `./tools/briar_fumus.sh -agere`; Fran looks
  at the icon, and at it again after the `-icon` rebuild (A8). Record
  what Finder showed.

- [ ] **Step 6: seal.**
  - Spec §4.8 **as built**: binary size after the link; A8's finding;
    **A5 correction** — fasciculum writes `CFBundleShortVersionString`
    `1.0` when `versio` is empty (`lib/fasciculum.c`, the
    `FASCICULUM_VERSIO_ORDINARIA` default), so "omitted" was not what
    ships; every deviation measured. §8 P8 → DONE with the commits.
  - `briar/CLAUDE.md`: the flag line gains `-app [-icon <via>]`; the
    `<briar>` element (`identitas`, `versio`, `icon`); the law that
    decoding lives only in `tools/briar.c`; `briar_icon_capsula.sh` in
    the build notes.
  - `briar/fontes/briar.worklog.md`: what the build taught.
  - Memory: `briar-project.md` paragraph + the `MEMORY.md` line; a
    ledger nota naming the commits.
  - Commit `tools/briar_fumus.sh`, `briar/CLAUDE.md`, the worklog and
    the spec, portae `[('radix', None), ('briar', None), ('briar-fumus',
    None)]`; rebake `~/.bin/briar` (and `~/.bin/briar-spectator`).

---

## Self-review

**Spec coverage.** A1 (decoding in the binary only) — T5 step 5, and
T3/T4 link no `imago`. A2 (any program) — no vitrea test anywhere. A3
(beside the thistle) — T3 `via_app`, T6 stage XIII. A4 (identity) — T1
(`fasciculum_identitas_valida`, one rule) and T3 (default, override,
refusal at the tag line). A5 (name, version) — T3; the version default
is corrected at the seal. A6 (replace own only) — T2 (safe removal), T4
(own replaced, foreign and plain file refused), T6 (both again through
the installed binary). A7 (icon precedence) — T3 (flag, attribute
resolved against the thistle, default), T5 (embedded default decoded
from memory, file decoded from disk), T6 (`-icon`). A8 — T5 step 6 and
T6 step 5, by hand. `<briar>` element — T3. Flags — T5. Gates: root
`fasciculum` T1, root `filum` T2, `probatio_briar_fasciculum` T3–T4,
`imperium` T5, fumus T6.

**Placeholders.** None left open. Two numbers are reasoned rather than
measured and say so where they stand: the 256 px iconset count (7) and
whether `open --args` reaches atrium's port.

**Type consistency.** `BriarFasciculumConsilium` fields (`identitas`,
`titulus`, `versio`, `via_icon`, `via_app`, `linea_briar`) are the same
in T3's header, T4's writer and T5's dispatch.
`briar_fasciculum_consilium(piscina, nexus, via_thistle, via_icon_vexilli,
consilium, causa, linea_causae)` and `briar_fasciculum_scribere(piscina,
consilium, icon, exsecutabile, domus, causa)` match every call.
`imp.icon` (T5) is the `via_icon_vexilli` argument.

**Named risks.**
1. **A half-written bundle of our own** (fasciculum never rolls back)
   has no readable executable, so `fasciculum_legere` refuses it and
   T4's writer says "neque fasciculus" — the user removes it by hand.
   Accepted; the message names the path.
2. **Finder's icon cache** (A8) is unmeasured until T5/T6's look.
3. **Line-pinned fixtures**: `app_vestitum.thistle` asserts its `<briar>`
   tag on line 8; editing its prose moves that.
4. **Commit order**: T1 and T2 change sealed libraries used across the
   tree, so each gates on the whole root suite; the briar tasks add the
   briar suite.
