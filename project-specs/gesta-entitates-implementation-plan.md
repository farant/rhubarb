# gesta `entities/` per-entity projection — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add a third projection of the tabularium event log — one markdown file per entity, duplicated into a folder per tag, holding the entity's full body — regenerated incrementally on every write.

**Architecture:** New internal functions in `gesta/fontes/tabularium.c` render one entity to markdown and reconcile its per-tag files against the filesystem. Reconcile is called at the same write endpoints that already call `_tabulam_scribere` — incrementally for single-entity writes (addere, gerere), and via a full sweep for bulk/multi-entity ops (agere, ramus) and on session `initialize`. The `.db` and `tabula.md` projections are untouched.

**Tech Stack:** C89 (Latin identifiers, `latina.h` macros), SQLite via `scrinium`, `gesta` event store, `json`, `chorda`/`chorda_aedificator`, `piscina` arena, `credo` tests. POSIX filesystem (`<sys/stat.h>`, `<dirent.h>`, `<unistd.h>`) — macOS only.

## Global Constraints

- Language standard: **C89 strict**, compiled with `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings` (see `gesta/compile_probationes.sh`). Every new function that is file-local MUST be `interior` (a `staticus` macro) or it trips `-Wmissing-prototypes`.
- All identifiers, comments, and user-facing strings in **Latin**. Never use a word `#define`d in `latina.h` as an identifier (e.g. `nomen`, `magnitudo`, `structura`, `casus`). Use `titulis` etc.
- `chorda` = `{i32 mensura; i8* datum}`, **NOT null-terminated**. Never pass `chorda.datum` to libc string functions. Convert to a C-string with `_litterae(pn, ch)` (piscina-allocated, NUL-terminated).
- `i32` is **unsigned**; use `s32`/`s64` for signed. Loop counters over `json_tabulatum_numerus` are `i32`.
- The projection is a **read-only generated artifact**. Every file begins with a "GENERATUM" banner; hand-edits are overwritten.
- Content is **deterministic**: every timestamp shown comes from event/row data, never a regeneration clock. Two reconciles of an unchanged entity produce byte-identical files (so `git diff` shows a file only when the entity actually changed).
- Compile+run tests with: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`. Exit `0` = all `credo` asserts passed.
- **Commit discipline:** we are on `main`. Before the first commit of this work, create a branch: `git checkout -b gesta-entitates`. Do not commit to `main`.

---

## Reference: the code you are extending

Read these before starting; the plan cites exact lines.

- `gesta/fontes/tabularium.h:29-46` — `TabulariumConfiguratio` (has `via_tabulae`; you add `via_entitatum`).
- `gesta/fontes/tabularium.c:1148-1331` — `_tabulam_scribere`: the sibling projection. Shows `ChordaAedificator`, `scrinium_praeparare/ligare_textum/gradi/columna_textus/finire`, `fopen("wb")/fwrite/fclose`, and (1319) `chorda_aedificator_finire`.
- `gesta/fontes/tabularium.c:2289-2568` — `_tab_res`: the template for gathering one entity's full state. Shows `_res_solvere`, `gesta_res_datum`, `_ancoras_reddere`, `gesta_socii_rei` + `GestaSocius {pars, membrum, nexus_res}` + `_titulus_membri` + `_verbum_vinculi`, and the annales query (2540-2543).
- `gesta/fontes/tabularium.c:1697-1724` — note/status event datum shapes: `nota` → `{"textus": "..."}`, `status` → `{"novus": "..."}`.
- `gesta/fontes/gesta.c:20-44` — schema: `tessellae(seq,id,res_id,genus_eventus,datum,actor,origo,branch_id,creatum)`, `res(res_id,genus,titulus,status,datum,creatum,mutatum)`.
- `gesta/fontes/gesta.h` — `gesta_res_datum`, `gesta_res_status`, `gesta_socii_rei`, `gesta_scrinium`, `GestaSocius` (`:219`).
- Callsites of `_tabulam_scribere(t, pn)`: `1594` (`_tab_addere`), `1782`/`1883`/`2015` (`_tab_gerere`), `2819`/`2873` (`_tab_agere`), `3055`/`3264`/`3316` (`_tab_ramus`), `3626` (`_initialize_tractare`).
- `gesta/probationes/probatio_tabularium.c` — test harness: `VIA_*` defines (`:15-22`), `_purgare` (`:24`), `_plagula_litterae` (`:52`), and the `_mitte(t, piscina, "<jsonrpc>")` → response-string + `CREDO_VERUM(strstr(r, "...") != NIHIL)` pattern.

Helper signatures already in `tabularium.c` you will reuse:
- `_litterae(Piscina* pn, chorda ch)` → `constans character*` (NUL-terminated copy).
- `_ch(constans character*)` → `chorda` (view over a literal).
- `_arg(JsonValor* argumenta, constans character* clavis)` → `chorda`.
- `_res_solvere(Tabularium* t, chorda clavis, Piscina* pn, b32* ambiguum)` → `chorda` res_id (empty if unknown).
- `gesta_res_datum(t->mundus, constans character* res_id, Piscina* pn)` → `chorda` (datum JSON: keys `corpus`, `tags`, `ancorae`; empty if absent).
- `_titulus_membri(Tabularium* t, chorda membrum_id, Piscina* pn)` → `chorda` title.
- `_verbum_vinculi(Tabularium* t, chorda nexus_res, Piscina* pn)` → `chorda` verb.

---

## File Structure

- **Modify** `gesta/fontes/tabularium.h` — one field on `TabulariumConfiguratio`.
- **Modify** `gesta/fontes/tabularium.c` — all new functions (a new `entitates` section placed just after `_tabulam_scribere`, before `/* instrumenta */` at `:1333`), plus config storage in `tabularium_creare` and reconcile calls at the write callsites.
- **Modify** `gesta/instrumenta/tabularium_principale.c` — set `cfg.via_entitatum`.
- **Modify** `gesta/probationes/probatio_tabularium.c` — tests (new `VIA_ENT`, dir purge, a directory-glob test helper, and a new test block).

All new C lives in `tabularium.c` — it is the projection owner; keeping the reconcile beside `_tabulam_scribere` follows the existing pattern (one projection function set per file section).

---

### Task 1: Config field `via_entitatum`

**Files:**
- Modify: `gesta/fontes/tabularium.h:35-38` (struct field)
- Modify: `gesta/fontes/tabularium.c` — the `Tabularium` struct (near the `via_tabulae` member) and `tabularium_creare` (near `:3818` where `t->via_tabulae` is stored)
- Modify: `gesta/instrumenta/tabularium_principale.c:49`

**Interfaces:**
- Produces: `t->via_entitatum` (`constans character*`, `NIHIL` = projection disabled) available to every function in `tabularium.c`.

- [ ] **Step 1: Add the config field.** In `gesta/fontes/tabularium.h`, after the `via_tabulae` member (line 38):

```c
    constans character* via_entitatum; /* radix entities/ (proiectio una-
                                        * plagula-per-rem); NIHIL = sine */
```

- [ ] **Step 2: Add storage on the `Tabularium` struct.** In `gesta/fontes/tabularium.c`, find the `Tabularium` struct definition (it contains `constans character* via_tabulae;`). Add beneath it:

```c
    constans character* via_entitatum;
```

- [ ] **Step 3: Store it in `tabularium_creare`.** Near `tabularium.c:3818` (where `t->via_tabulae = cfg->via_tabulae != NIHIL ? _litterae(...) : NIHIL;`), add the parallel line:

```c
    t->via_entitatum = cfg->via_entitatum != NIHIL
        ? _litterae(piscina, _ch(cfg->via_entitatum)) : NIHIL;
```

- [ ] **Step 4: Wire the real server.** In `gesta/instrumenta/tabularium_principale.c`, next to `cfg.via_tabulae = "gesta/annales/tabula.md";` (line 49):

```c
    cfg.via_entitatum = "gesta/annales/entities";
```

- [ ] **Step 5: Cold-path parity check (no code unless needed).** Run `grep -n via_tabulae gesta/instrumenta/nota_frigida.c`. If it sets `via_tabulae`, add a matching `cfg.via_entitatum = "gesta/annales/entities";`. If it does **not** set `via_tabulae` (cold writes skip projections by design), leave `nota_frigida` alone — the tree rebuilds on next server `initialize` (Task 7).

- [ ] **Step 6: Build (nothing consumes the field yet — just verify it compiles).**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: PASS (existing tests still green; new field unused is fine — an unused struct field does not warn).

- [ ] **Step 7: Commit.**

```bash
cd /Users/francisarant/Documents/projects/rhubarb
git checkout -b gesta-entitates
git add gesta/fontes/tabularium.h gesta/fontes/tabularium.c gesta/instrumenta/tabularium_principale.c
git commit -m "gesta entitates: via_entitatum config field

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 2: Slug + filename helpers (pure)

**Files:**
- Modify: `gesta/fontes/tabularium.c` — add a new section `/* === entitates: proiectio una-plagula-per-rem === */` immediately after `_tabulam_scribere` (after line 1331).
- Test: `gesta/probationes/probatio_tabularium.c`

**Interfaces:**
- Produces:
  - `interior chorda _titulum_ad_slug(chorda titulus, Piscina* pn);` — lowercase ASCII; every run of non-`[a-z0-9]` becomes a single `-`; leading/trailing `-` trimmed; truncated to 40 bytes (no trailing `-` after truncation). Empty/no-alnum title → `"sine-titulo"`.
  - `interior chorda _entitatem_nomen_plagulae(chorda genus, chorda slug, constans character* res_id, Piscina* pn);` — returns `"<genus>-<slug>-<res_id>.md"` as a `chorda`.

Because `probatio_tabularium.c` cannot see `interior` functions, expose them for testing behind a test hook: add near the top of the entitates section:

```c
#ifdef TABULARIUM_PROBATIO
#define ENT_PROBABILE
#else
#define ENT_PROBABILE interior
#endif
```

and declare `_titulum_ad_slug` / `_entitatem_nomen_plagulae` as `ENT_PROBABILE`. The test file `#define TABULARIUM_PROBATIO` before `#include`-ing nothing extra — instead, prototype them at the top of the test file. **Simpler alternative (use this):** test these two pure functions *indirectly* through the file paths produced in Task 5's tests, and keep them plain `interior`. Adopt the simpler alternative — do **not** add the macro. Steps below reflect that: no direct unit test, correctness verified in Task 5.

- [ ] **Step 1: Implement `_titulum_ad_slug`.**

```c
interior chorda
_titulum_ad_slug (chorda titulus, Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, LXIV);
    b32 lineola_pendens = FALSUM;   /* an ultimum emissum fuit '-' */
    b32 aliquid = FALSUM;           /* an character alphanumericus emissus */
    i32 emissa = ZEPHYRUM;          /* longitudo currens */
    i32 i;
    character unum[II];

    unum[I] = '\0';
    per (i = ZEPHYRUM; i < titulus.mensura && emissa < XL; i++)
    {
        i8 c = titulus.datum[i];

        si (c >= (i8)'A' && c <= (i8)'Z')
        {
            c = (i8)(c + (i8)('a' - 'A'));
        }
        si ((c >= (i8)'a' && c <= (i8)'z')
            || (c >= (i8)'0' && c <= (i8)'9'))
        {
            si (lineola_pendens && aliquid)
            {
                chorda_aedificator_appendere_literis(aed, "-");
                emissa++;
                lineola_pendens = FALSUM;
            }
            unum[0] = (character)c;
            chorda_aedificator_appendere_literis(aed, unum);
            emissa++;
            aliquid = VERUM;
        }
        alioquin
        {
            lineola_pendens = VERUM;   /* mora - lineolam solum si sequitur alnum */
        }
    }
    si (!aliquid)
    {
        redde _ch("sine-titulo");
    }
    redde chorda_aedificator_finire(aed);
}
```

- [ ] **Step 2: Implement `_entitatem_nomen_plagulae`.**

```c
interior chorda
_entitatem_nomen_plagulae (chorda genus, chorda slug,
    constans character* res_id, Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, LXIV);

    chorda_aedificator_appendere_chorda(aed, genus);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_chorda(aed, slug);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_literis(aed, res_id);
    chorda_aedificator_appendere_literis(aed, ".md");
    redde chorda_aedificator_finire(aed);
}
```

- [ ] **Step 3: Build (functions unused so far — confirm they compile clean under `-Werror`).**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: FAIL with `-Werror=unused-function` for the two new `interior` functions (they have no caller yet).

**This is expected.** To keep the build green between tasks, temporarily mark them used by adding, at the end of the entitates section, a forward reference that the next task consumes. Simplest: **do Steps 1-2 but defer the build to Task 5**, where the first caller appears. Mark this task's commit as combined with Task 5 if your executor requires green-between-commits. Otherwise commit with a `(vacuum)` self-reference guard:

```c
/* provisorium: silere -Wunused usque ad Task 5 nexum */
interior vacuum _entitates_provisorium(vacuum);
interior vacuum _entitates_provisorium(vacuum) {
    (vacuum)_titulum_ad_slug; (vacuum)_entitatem_nomen_plagulae;
}
```

Add that guard, build (expect PASS), then **remove the guard in Task 5 Step 1**.

- [ ] **Step 4: Commit.**

```bash
git add gesta/fontes/tabularium.c
git commit -m "gesta entitates: slug + filename helpers

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 3: POSIX filesystem helpers

**Files:**
- Modify: `gesta/fontes/tabularium.c` — top of the file, add includes; entitates section, add helpers.

**Interfaces:**
- Produces:
  - `interior vacuum _directorium_facere(constans character* via);` — `mkdir(via, 0755)`; ignore `EEXIST`.
  - `interior b32 _semita_iungere(character* buf, memoriae_index cap, constans character* a, constans character* b);` — writes `"a/b"` into `buf`; `FALSUM` if it would overflow.
  - `interior vacuum _plagulam_chorda_scribere(constans character* via, chorda textus);` — `fopen(via,"wb")`/`fwrite`/`fclose` (mirror of `tabularium.c:1320-1330`).
  - `interior vacuum _entis_plagulas_omnes_delere(Tabularium* t, constans character* res_id, Piscina* pn);` — across every tag subdir of `t->via_entitatum`, `unlink` any file whose name ends `"-<res_id>.md"`; `rmdir` a subdir left empty.

- [ ] **Step 1: Add POSIX includes.** At the top of `tabularium.c`, after the existing `#include`s, add:

```c
#include <sys/stat.h>   /* mkdir */
#include <dirent.h>     /* opendir/readdir/closedir */
#include <unistd.h>     /* unlink/rmdir */
#include <errno.h>
#include <string.h>     /* strlen/strcmp/strstr - C-strings solum, non chorda */
```

(If `<string.h>` is already included, don't duplicate it.)

- [ ] **Step 2: Implement `_directorium_facere` and `_semita_iungere`.**

```c
interior vacuum
_directorium_facere (constans character* via)
{
    si (mkdir(via, 0755) != ZEPHYRUM && errno != EEXIST)
    {
        /* tacite pergere - proiectio non est via critica */
    }
}

interior b32
_semita_iungere (character* buf, memoriae_index cap,
    constans character* a, constans character* b)
{
    memoriae_index la = strlen(a);
    memoriae_index lb = strlen(b);

    si (la + I + lb + I > cap)
    {
        redde FALSUM;
    }
    memcpy(buf, a, la);
    buf[la] = '/';
    memcpy(buf + la + I, b, lb);
    buf[la + I + lb] = '\0';
    redde VERUM;
}
```

- [ ] **Step 3: Implement `_plagulam_chorda_scribere`.**

```c
interior vacuum
_plagulam_chorda_scribere (constans character* via, chorda textus)
{
    FILE* pl = fopen(via, "wb");

    si (pl == NIHIL)
    {
        redde;
    }
    si (textus.mensura > ZEPHYRUM)
    {
        (vacuum)fwrite(textus.datum, I,
            (memoriae_index)textus.mensura, pl);
    }
    fclose(pl);
}
```

- [ ] **Step 4: Implement `_entis_plagulas_omnes_delere`.**

```c
interior vacuum
_entis_plagulas_omnes_delere (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    DIR* radix;
    structura dirent* e;
    character acus[XL + IV];   /* "-<ulid>.md" */

    (vacuum)pn;
    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    radix = opendir(t->via_entitatum);
    si (radix == NIHIL)
    {
        redde;
    }
    sprintf(acus, "-%s.md", res_id);
    dum ((e = readdir(radix)) != NIHIL)
    {
        character semita_dir[MDXX];
        DIR* sub;
        structura dirent* f;
        b32 vacua = VERUM;

        si (e->d_name[0] == '.')
        {
            perge;
        }
        si (!_semita_iungere(semita_dir, magnitudo(semita_dir),
                t->via_entitatum, e->d_name))
        {
            perge;
        }
        sub = opendir(semita_dir);
        si (sub == NIHIL)
        {
            perge;   /* non directorium */
        }
        dum ((f = readdir(sub)) != NIHIL)
        {
            character semita_plag[MMXL];
            memoriae_index ln = strlen(f->d_name);
            memoriae_index la = strlen(acus);

            si (f->d_name[0] == '.')
            {
                perge;
            }
            si (ln >= la
                && strcmp(f->d_name + (ln - la), acus) == ZEPHYRUM)
            {
                si (_semita_iungere(semita_plag, magnitudo(semita_plag),
                        semita_dir, f->d_name))
                {
                    unlink(semita_plag);
                }
            }
            alioquin
            {
                vacua = FALSUM;   /* alia plagula manet */
            }
        }
        closedir(sub);
        si (vacua)
        {
            rmdir(semita_dir);   /* directorium tagi vacuum purgare */
        }
    }
    closedir(radix);
}
```

Note the numeral macros are `latina.h` Roman numerals (`XL`=40, `MDXX`=1520, `MMXL`=2040, `IV`=4). If any is absent from `latina.h`, use the arabic literal.

- [ ] **Step 5: Silence unused until Task 5.** These are consumed in Task 5. If your executor needs green now, extend the `_entitates_provisorium` guard from Task 2 to reference them, else fold Tasks 3-5 into one commit. Build:

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: PASS (with guard) — confirms POSIX calls compile under the strict C89 flags. **If it fails with implicit-declaration of `mkdir`/`opendir`/`unlink`:** add `#define _DARWIN_C_SOURCE` as the very first line of `tabularium.c` (before all includes) and rebuild.

- [ ] **Step 6: Commit.**

```bash
git add gesta/fontes/tabularium.c
git commit -m "gesta entitates: POSIX fs helpers (mkdir/scan/delete)

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 4: Entity → markdown renderer

**Files:**
- Modify: `gesta/fontes/tabularium.c` — entitates section.

**Interfaces:**
- Consumes: `_titulum_ad_slug` (Task 2), the reused helpers listed in the Reference section.
- Produces:
  - `interior chorda _entitatem_semita_relativa(Tabularium* t, chorda membrum_id, Piscina* pn);` — for a linked member, returns `"../<primum-tag>/<genus>-<slug>-<id>.md"`, or `chorda` of length 0 if `membrum_id` does not resolve to a `res` row (then the caller renders plain text). `<primum-tag>` = first element of the target's `tags`, or `"_sine_tag"`.
  - `interior chorda _entitatem_ad_markdown(Tabularium* t, constans character* res_id, Piscina* pn);` — the full file body (frontmatter + `# titulus` + corpus + `## Status` + `## Notae` + `## Nexus` + `## Ancorae`). Assumes the entity exists (caller checked `gesta_res_datum` non-empty).

- [ ] **Step 1: Implement `_entitatem_semita_relativa`.**

```c
interior chorda
_entitatem_semita_relativa (Tabularium* t, chorda membrum_id, Piscina* pn)
{
    ScriniumEnuntiatum* e;
    chorda genus_ch;
    chorda titulus_ch;
    chorda datum;
    chorda primum_tag;
    chorda slug;
    ChordaAedificator* aed;
    b32 inventa = FALSUM;

    primum_tag = _ch("_sine_tag");
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT genus, titulus FROM res WHERE res_id = ?");
    si (e == NIHIL)
    {
        redde _ch("");
    }
    scrinium_ligare_textum(e, I, membrum_id);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        genus_ch = scrinium_columna_textus(e, ZEPHYRUM, pn);
        titulus_ch = scrinium_columna_textus(e, I, pn);
        inventa = VERUM;
    }
    scrinium_finire(e);
    si (!inventa)
    {
        redde _ch("");   /* membrum non est res -> sine ligamine */
    }
    datum = gesta_res_datum(t->mundus, _litterae(pn, membrum_id), pn);
    {
        JsonResultus r = json_legere(datum, pn);

        si (r.successus && json_est_objectum(r.radix))
        {
            JsonValor* tags = json_objectum_capere(r.radix, "tags");

            si (tags != NIHIL && json_est_tabulatum(tags)
                && json_tabulatum_numerus(tags) > ZEPHYRUM)
            {
                JsonValor* t0 = json_tabulatum_obtinere(tags, ZEPHYRUM);

                si (t0 != NIHIL && json_est_chorda(t0))
                {
                    primum_tag = json_ad_chorda(t0);
                }
            }
        }
    }
    slug = _titulum_ad_slug(titulus_ch, pn);
    aed = chorda_aedificator_creare(pn, LXIV);
    chorda_aedificator_appendere_literis(aed, "../");
    chorda_aedificator_appendere_chorda(aed, primum_tag);
    chorda_aedificator_appendere_literis(aed, "/");
    chorda_aedificator_appendere_chorda(aed,
        _entitatem_nomen_plagulae(genus_ch, slug,
            _litterae(pn, membrum_id), pn));
    redde chorda_aedificator_finire(aed);
}
```

- [ ] **Step 2: Implement `_entitatem_ad_markdown` — frontmatter + title + corpus.**

```c
interior chorda
_entitatem_ad_markdown (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    ChordaAedificator* aed = chorda_aedificator_creare(pn, MMXL);
    chorda datum = gesta_res_datum(t->mundus, res_id, pn);
    JsonValor* st = NIHIL;
    chorda genus_ch = _ch("");
    chorda titulus_ch = _ch("");
    chorda status_ch = _ch("");
    chorda creatum_ch = _ch("");
    JsonResultus r;

    /* res tabula: genus/titulus/status/creatum */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus, titulus, status, creatum FROM res"
            " WHERE res_id = ?");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                genus_ch = scrinium_columna_textus(e, ZEPHYRUM, pn);
                titulus_ch = scrinium_columna_textus(e, I, pn);
                status_ch = scrinium_columna_textus(e, II, pn);
                creatum_ch = scrinium_columna_textus(e, III, pn);
            }
            scrinium_finire(e);
        }
    }
    r = json_legere(datum, pn);
    si (r.successus && json_est_objectum(r.radix))
    {
        st = r.radix;
    }
    /* frontmatter */
    chorda_aedificator_appendere_literis(aed, "---\nid: ");
    chorda_aedificator_appendere_literis(aed, res_id);
    chorda_aedificator_appendere_literis(aed, "\ngenus: ");
    chorda_aedificator_appendere_chorda(aed, genus_ch);
    chorda_aedificator_appendere_literis(aed, "\nstatus: ");
    chorda_aedificator_appendere_chorda(aed, status_ch);
    chorda_aedificator_appendere_literis(aed, "\ncreatum: ");
    chorda_aedificator_appendere_chorda(aed, creatum_ch);
    chorda_aedificator_appendere_literis(aed, "\ntags: ");
    si (st != NIHIL)
    {
        JsonValor* tags = json_objectum_capere(st, "tags");
        i32 i;
        i32 n = (tags != NIHIL && json_est_tabulatum(tags))
            ? json_tabulatum_numerus(tags) : ZEPHYRUM;

        per (i = ZEPHYRUM; i < n; i++)
        {
            JsonValor* tg = json_tabulatum_obtinere(tags, i);

            si (tg == NIHIL || !json_est_chorda(tg))
            {
                perge;
            }
            si (i > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed, ", ");
            }
            chorda_aedificator_appendere_chorda(aed, json_ad_chorda(tg));
        }
    }
    chorda_aedificator_appendere_literis(aed,
        "\n---\n\n> GENERATUM (proiectio entitatum - noli manu edere)\n\n# ");
    chorda_aedificator_appendere_chorda(aed, titulus_ch);
    chorda_aedificator_appendere_literis(aed, "\n\n");
    /* corpus */
    si (st != NIHIL)
    {
        JsonValor* corpus = json_objectum_capere(st, "corpus");

        si (corpus != NIHIL && json_est_chorda(corpus))
        {
            chorda_aedificator_appendere_chorda(aed,
                json_ad_chorda(corpus));
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    /* (sectiones Status/Notae/Nexus/Ancorae additae in Step 3) */
    redde chorda_aedificator_finire(aed);
}
```

- [ ] **Step 3: Append Status / Notae / Nexus / Ancorae sections.** Replace the `redde chorda_aedificator_finire(aed);` at the end of Step 2 with the following blocks, then the `redde`:

```c
    /* ## Status - chronologia ex eventis creatio/status */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus_eventus, datum, creatum FROM tessellae"
            " WHERE res_id = ? AND genus_eventus IN ('creatio','status')"
            " ORDER BY seq");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            chorda_aedificator_appendere_literis(aed, "\n## Status\n\n");
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda ge = scrinium_columna_textus(e, ZEPHYRUM, pn);
                chorda ed = scrinium_columna_textus(e, I, pn);
                chorda ec = scrinium_columna_textus(e, II, pn);

                chorda_aedificator_appendere_literis(aed, "- ");
                si (_chorda_est(ge, "creatio"))
                {
                    chorda_aedificator_appendere_literis(aed, "creatum");
                }
                alioquin
                {
                    JsonResultus rd = json_legere(ed, pn);

                    si (rd.successus && json_est_objectum(rd.radix))
                    {
                        JsonValor* nv = json_objectum_capere(rd.radix,
                            "novus");

                        si (nv != NIHIL && json_est_chorda(nv))
                        {
                            chorda_aedificator_appendere_chorda(aed,
                                json_ad_chorda(nv));
                        }
                    }
                }
                chorda_aedificator_appendere_literis(aed, " — ");
                chorda_aedificator_appendere_chorda(aed, ec);
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    /* ## Notae - omnes, vetustissima primum */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT datum, actor, creatum FROM tessellae"
            " WHERE res_id = ? AND genus_eventus = 'nota' ORDER BY seq");
        b32 caput = FALSUM;

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            dum (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                chorda ed = scrinium_columna_textus(e, ZEPHYRUM, pn);
                chorda ea = scrinium_columna_textus(e, I, pn);
                chorda ec = scrinium_columna_textus(e, II, pn);
                JsonResultus rd = json_legere(ed, pn);

                si (!caput)
                {
                    chorda_aedificator_appendere_literis(aed,
                        "\n## Notae\n\n");
                    caput = VERUM;
                }
                chorda_aedificator_appendere_literis(aed, "- ");
                chorda_aedificator_appendere_chorda(aed, ec);
                chorda_aedificator_appendere_literis(aed, " (");
                chorda_aedificator_appendere_chorda(aed, ea);
                chorda_aedificator_appendere_literis(aed, "): ");
                si (rd.successus && json_est_objectum(rd.radix))
                {
                    JsonValor* tx = json_objectum_capere(rd.radix,
                        "textus");

                    si (tx != NIHIL && json_est_chorda(tx))
                    {
                        chorda_aedificator_appendere_chorda(aed,
                            json_ad_chorda(tx));
                    }
                }
                chorda_aedificator_appendere_literis(aed, "\n");
            }
            scrinium_finire(e);
        }
    }
    /* ## Nexus - socii ut ligamina relativa */
    {
        Xar* socii = gesta_socii_rei(t->mundus, res_id, pn);
        i32 i;
        i32 n = (socii != NIHIL) ? xar_numerus(socii) : ZEPHYRUM;

        si (n > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "\n## Nexus\n\n");
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            GestaSocius* s = (GestaSocius*)xar_obtinere(socii, i);
            chorda verbum;
            chorda titulus_m;
            chorda semita;

            si (s == NIHIL)
            {
                perge;
            }
            verbum = _verbum_vinculi(t, s->nexus_res, pn);
            titulus_m = _titulus_membri(t, s->membrum, pn);
            semita = _entitatem_semita_relativa(t, s->membrum, pn);
            chorda_aedificator_appendere_literis(aed, "- ");
            chorda_aedificator_appendere_chorda(aed, verbum);
            chorda_aedificator_appendere_literis(aed, " → ");
            si (semita.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(aed, "[");
                chorda_aedificator_appendere_chorda(aed, titulus_m);
                chorda_aedificator_appendere_literis(aed, "](");
                chorda_aedificator_appendere_chorda(aed, semita);
                chorda_aedificator_appendere_literis(aed, ")");
            }
            alioquin
            {
                chorda_aedificator_appendere_chorda(aed, titulus_m);
            }
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    /* ## Ancorae */
    si (st != NIHIL)
    {
        JsonValor* anc = json_objectum_capere(st, "ancorae");
        i32 i;
        i32 n = (anc != NIHIL && json_est_tabulatum(anc))
            ? json_tabulatum_numerus(anc) : ZEPHYRUM;

        si (n > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, "\n## Ancorae\n\n");
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            JsonValor* a = json_tabulatum_obtinere(anc, i);
            JsonValor* ag;
            JsonValor* as;

            si (a == NIHIL || !json_est_objectum(a))
            {
                perge;
            }
            ag = json_objectum_capere(a, "genus");
            as = json_objectum_capere(a, "scopus");
            chorda_aedificator_appendere_literis(aed, "- ");
            si (ag != NIHIL && json_est_chorda(ag))
            {
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(ag));
                chorda_aedificator_appendere_literis(aed, " ");
            }
            si (as != NIHIL && json_est_chorda(as))
            {
                chorda_aedificator_appendere_literis(aed, "`");
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(as));
                chorda_aedificator_appendere_literis(aed, "`");
            }
            chorda_aedificator_appendere_literis(aed, "\n");
        }
    }
    redde chorda_aedificator_finire(aed);
```

Note: `—` (em dash) and `→` (→) are shown here for clarity — C89 has no `\u`. Use the raw UTF-8 bytes in string literals instead: em dash = `"\xE2\x80\x94"`, right-arrow = `"\xE2\x86\x92"`. (The codebase is UTF-8; `chorda` is byte-based.)

- [ ] **Step 4: Verify `_chorda_est` exists** (used above): `grep -n "_chorda_est" gesta/fontes/tabularium.c` — it is used throughout `_tab_gerere`. Confirmed available.

- [ ] **Step 5: Build (still no caller — keep the provisorium guard extended to `_entitatem_ad_markdown`).**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: PASS.

- [ ] **Step 6: Commit.**

```bash
git add gesta/fontes/tabularium.c
git commit -m "gesta entitates: entity->markdown renderer

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 5: `_entitatem_reconciliare` (incremental, one entity)

**Files:**
- Modify: `gesta/fontes/tabularium.c` — entitates section.
- Test: `gesta/probationes/probatio_tabularium.c`

**Interfaces:**
- Consumes: everything from Tasks 2-4.
- Produces: `interior vacuum _entitatem_reconciliare(Tabularium* t, constans character* res_id, Piscina* pn);` — writes the entity's file into every tag folder (or `_sine_tag/`), deleting any prior copies first. Removed/absent entity (`gesta_res_datum` empty) → all copies deleted.

- [ ] **Step 1: Remove the `_entitates_provisorium` guard** added in Task 2/3, since real callers now exist within this function.

- [ ] **Step 2: Implement `_entitatem_reconciliare`.**

```c
interior vacuum
_entitatem_reconciliare (Tabularium* t, constans character* res_id,
    Piscina* pn)
{
    chorda datum;
    chorda genus_ch = _ch("");
    chorda titulus_ch = _ch("");
    chorda slug;
    chorda nomen;
    chorda md;
    JsonValor* st = NIHIL;
    JsonValor* tags = NIHIL;
    i32 n_tags = ZEPHYRUM;
    i32 i;

    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    /* semper primo reliquas plagulas huius entis purgare (tags/slug
     * mutati, remotio) - systema plagularum = memoria status veteris */
    _entis_plagulas_omnes_delere(t, res_id, pn);

    datum = gesta_res_datum(t->mundus, res_id, pn);
    si (datum.mensura == ZEPHYRUM)
    {
        redde;   /* remotum/absens -> nullae plagulae optatae */
    }
    {
        JsonResultus r = json_legere(datum, pn);

        si (r.successus && json_est_objectum(r.radix))
        {
            st = r.radix;
        }
    }
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            gesta_scrinium(t->mundus),
            "SELECT genus, titulus FROM res WHERE res_id = ?");

        si (e != NIHIL)
        {
            scrinium_ligare_textum(e, I, _ch(res_id));
            si (scrinium_gradi(e) == SCRINIUM_ORDO)
            {
                genus_ch = scrinium_columna_textus(e, ZEPHYRUM, pn);
                titulus_ch = scrinium_columna_textus(e, I, pn);
            }
            scrinium_finire(e);
        }
    }
    slug = _titulum_ad_slug(titulus_ch, pn);
    nomen = _entitatem_nomen_plagulae(genus_ch, slug, res_id, pn);
    md = _entitatem_ad_markdown(t, res_id, pn);

    si (st != NIHIL)
    {
        tags = json_objectum_capere(st, "tags");
        si (tags != NIHIL && json_est_tabulatum(tags))
        {
            n_tags = json_tabulatum_numerus(tags);
        }
    }
    _directorium_facere(t->via_entitatum);

    si (n_tags == ZEPHYRUM)
    {
        character dir[MDXX];
        character plag[MMXL];

        si (_semita_iungere(dir, magnitudo(dir), t->via_entitatum,
                "_sine_tag"))
        {
            _directorium_facere(dir);
            si (_semita_iungere(plag, magnitudo(plag), dir,
                    _litterae(pn, nomen)))
            {
                _plagulam_chorda_scribere(plag, md);
            }
        }
        redde;
    }
    per (i = ZEPHYRUM; i < n_tags; i++)
    {
        JsonValor* tg = json_tabulatum_obtinere(tags, i);
        character dir[MDXX];
        character plag[MMXL];

        si (tg == NIHIL || !json_est_chorda(tg))
        {
            perge;
        }
        si (_semita_iungere(dir, magnitudo(dir), t->via_entitatum,
                _litterae(pn, json_ad_chorda(tg))))
        {
            _directorium_facere(dir);
            si (_semita_iungere(plag, magnitudo(plag), dir,
                    _litterae(pn, nomen)))
            {
                _plagulam_chorda_scribere(plag, md);
            }
        }
    }
}
```

- [ ] **Step 3: Write the failing test.** In `gesta/probationes/probatio_tabularium.c`, first add near the other `VIA_*` defines (line ~19):

```c
#define VIA_ENT "gesta/build/probatio_entities"
```

Add a directory-glob helper near `_plagula_litterae` (a test-only reader that returns the first `.md` file's full text inside a tag dir, empty if none):

```c
interior constans character*
_prima_plagula_md (Piscina* piscina, constans character* dir)
{
    DIR* d = opendir(dir);
    structura dirent* e;
    character semita[MMXL];

    si (d == NIHIL)
    {
        redde "";
    }
    dum ((e = readdir(d)) != NIHIL)
    {
        memoriae_index ln = strlen(e->d_name);

        si (e->d_name[0] != '.' && ln > III
            && strcmp(e->d_name + (ln - III), ".md") == ZEPHYRUM)
        {
            sprintf(semita, "%s/%s", dir, e->d_name);
            closedir(d);
            redde _plagula_litterae(piscina, semita);
        }
    }
    closedir(d);
    redde "";
}
```

(Add `#include <dirent.h>` and `#include <sys/stat.h>` to the test file if not present, and confirm `MMXL`/`III` resolve, else use `2040`/`3`.)

In `_purgare`, add a recursive wipe of the tree so tests start clean — simplest is a shell-out:

```c
    system("rm -rf " VIA_ENT);
```

(`#include <stdlib.h>` for `system`.)

Then in the test body (inside the block where `t` and `piscina` exist, after the config sets `cfg.via_tabulae = VIA_TB;`), add `cfg.via_entitatum = VIA_ENT;`, and append a new test section:

```c
    /* === proiectio entitatum === */
    {
        constans character* md;

        /* creare rem cum duobus tags */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":200,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"parcum\",\"titulus\":"
            "\"Probatio Entitatum Alpha\",\"corpus\":\"corpus alpha\","
            "\"tags\":\"silva, examen\"}}}");

        /* plagula in utroque folder tagi, corpus praesens */
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "genus: parcum") != NIHIL);
        CREDO_VERUM (strstr(md, "# Probatio Entitatum Alpha") != NIHIL);
        CREDO_VERUM (strstr(md, "corpus alpha") != NIHIL);
        CREDO_VERUM (strstr(md, "GENERATUM") != NIHIL);
        md = _prima_plagula_md(piscina, VIA_ENT "/examen");
        CREDO_VERUM (strstr(md, "Probatio Entitatum Alpha") != NIHIL);
    }
```

- [ ] **Step 4: Run test to verify it fails.**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: FAIL — the reconcile is not yet wired into `addere` (Task 7), so no files are written; `strstr` returns `NIHIL`. (This proves the test exercises the real path.)

- [ ] **Step 5:** Leave the failing test; it goes green in Task 7. **Commit the reconcile function + test now** (red test committed intentionally; Task 7 turns it green):

Actually — to keep committed states green, **defer the test's assertions to Task 7**. For this task, commit only `_entitatem_reconciliare` (still uncalled but no longer guarded, because Task 7 immediately follows and is committed together). If your executor requires each commit to build green, **combine Task 5 and Task 7 into one commit**. Build with reconcile present but uncalled:

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: FAIL `-Werror=unused-function` for `_entitatem_reconciliare`. Therefore **do not commit Task 5 alone** — proceed directly to Task 6 and Task 7, which add the callers, then commit Tasks 5-7 together at Task 7 Step 4.

---

### Task 6: `_entitates_reconciliare_omnes` (full sweep / rebuild)

**Files:**
- Modify: `gesta/fontes/tabularium.c` — entitates section.

**Interfaces:**
- Consumes: `_entitatem_reconciliare` (Task 5), `_directorium_purgare` (defined here).
- Produces: `interior vacuum _entitates_reconciliare_omnes(Tabularium* t, Piscina* pn);` — clears `entities/` then reconciles every row in `res`. Used at `initialize` and after multi-entity ops.

- [ ] **Step 1: Add a recursive directory-purge helper** (clears the tree so orphaned files from removed entities vanish):

```c
interior vacuum
_directorium_purgare (constans character* via)
{
    DIR* d = opendir(via);
    structura dirent* e;

    si (d == NIHIL)
    {
        redde;
    }
    dum ((e = readdir(d)) != NIHIL)
    {
        character semita[MMXL];

        si (e->d_name[0] == '.')
        {
            perge;
        }
        si (_semita_iungere(semita, magnitudo(semita), via, e->d_name))
        {
            _directorium_purgare(semita);   /* si directorium */
            unlink(semita);                  /* si plagula */
            rmdir(semita);                   /* si directorium iam vacuum */
        }
    }
    closedir(d);
}
```

(`unlink` on a directory fails harmlessly; `rmdir` on a file fails harmlessly; recursion empties subdirs first. This is a small tree — clarity over cleverness.)

- [ ] **Step 2: Implement `_entitates_reconciliare_omnes`.**

```c
interior vacuum
_entitates_reconciliare_omnes (Tabularium* t, Piscina* pn)
{
    ScriniumEnuntiatum* e;

    si (t->via_entitatum == NIHIL)
    {
        redde;
    }
    _directorium_purgare(t->via_entitatum);
    _directorium_facere(t->via_entitatum);
    e = scrinium_praeparare(gesta_scrinium(t->mundus),
        "SELECT res_id FROM res ORDER BY res_id");
    si (e == NIHIL)
    {
        redde;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda rid = scrinium_columna_textus(e, ZEPHYRUM, pn);

        _entitatem_reconciliare(t, _litterae(pn, rid), pn);
    }
    scrinium_finire(e);
}
```

- [ ] **Step 3: Build (both still uncalled → proceed to Task 7 before committing).**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: FAIL `-Werror=unused-function`. Expected — callers arrive in Task 7.

---

### Task 7: Wire reconcile into the write callsites

**Files:**
- Modify: `gesta/fontes/tabularium.c` — 10 callsites.
- Test: `gesta/probationes/probatio_tabularium.c` (extend the Task 5 block).

**Interfaces:**
- Consumes: `_entitatem_reconciliare` (single entity), `_entitates_reconciliare_omnes` (bulk).

At each `_tabulam_scribere(t, pn);` call, add the reconcile call(s) on the **next line**. The affected `res_id` differs per callsite — use exactly these:

| Line | Function / action | Add after `_tabulam_scribere(t, pn);` |
|---|---|---|
| 1594 | `_tab_addere` | `_entitatem_reconciliare(t, res_id, pn);` — `res_id` is the `character[]` buffer, a C-string |
| 1782 | `_tab_gerere` nexus | `_entitatem_reconciliare(t, _litterae(pn, res_id), pn); _entitatem_reconciliare(t, _litterae(pn, membrum_b), pn);` |
| 1883 | `_tab_gerere` denexus | `_entitatem_reconciliare(t, _litterae(pn, res_id), pn); _entitatem_reconciliare(t, _litterae(pn, membrum_b), pn);` |
| 2015 | `_tab_gerere` note/status/mutatio/remotio | `_entitatem_reconciliare(t, _litterae(pn, res_id), pn);` |
| 2819 | `_tab_agere` | `_entitates_reconciliare_omnes(t, pn);` |
| 2873 | `_tab_agere` | `_entitates_reconciliare_omnes(t, pn);` |
| 3055 | `_tab_ramus` | `_entitates_reconciliare_omnes(t, pn);` |
| 3264 | `_tab_ramus` | `_entitates_reconciliare_omnes(t, pn);` |
| 3316 | `_tab_ramus` | `_entitates_reconciliare_omnes(t, pn);` |
| 3626 | `_initialize_tractare` | `_entitates_reconciliare_omnes(t, pn);` |

`res_id`/`membrum_b` are `chorda` at the gerere callsites (confirmed: `tabularium.c:1640`, `:1734`); `res_id` at `_tab_addere` is a C-string buffer (confirmed: passed bare to `gesta_scribere`/`gesta_res_status`).

- [ ] **Step 1: Add the reconcile calls** at the 10 lines above. Line numbers shift as you insert — insert bottom-up (3626 first, then 3316, … then 1594) so earlier line numbers stay valid, or re-`grep -n "_tabulam_scribere(t, pn)"` after each edit.

- [ ] **Step 2: Complete the Task 5 test block** — append lifecycle assertions after the create asserts:

```c
        /* addere nota -> nota apparet, eadem plagula */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":201,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"gerere\","
            "\"arguments\":{\"res\":\"Probatio Entitatum Alpha\","
            "\"actus\":\"nota\",\"textus\":\"nota prima hic\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "## Notae") != NIHIL);
        CREDO_VERUM (strstr(md, "nota prima hic") != NIHIL);

        /* status -> lineae status, plagula manet (archivum planum) */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":202,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"gerere\","
            "\"arguments\":{\"res\":\"Probatio Entitatum Alpha\","
            "\"actus\":\"status\",\"novus\":\"tractum\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/silva");
        CREDO_VERUM (strstr(md, "status: tractum") != NIHIL);

        /* retag: mutatio tags -> folder vetus purgatur, novum apparet */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":203,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"gerere\","
            "\"arguments\":{\"res\":\"Probatio Entitatum Alpha\","
            "\"actus\":\"mutatio\",\"clavis\":\"tags\",\"valor\":\"mcp\"}}}");
        /* NOTA: mutatio clavis+valor pono chorda, non tabulatum - vide sub */
        md = _prima_plagula_md(piscina, VIA_ENT "/mcp");
        CREDO_VERUM (strstr(md, "Probatio Entitatum Alpha") != NIHIL);
        CREDO_VERUM (_prima_plagula_md(piscina, VIA_ENT "/silva")[0]
            == '\0');   /* folder silva purgatum (vacuum aut abest) */
    }
```

**Caveat on the retag test:** `gerere mutatio clavis=tags valor=mcp` writes `tags` as a JSON *string*, not an array (see `tabularium.c:1918` `json_objectum_ponere_chorda`). The reconcile reads `tags` expecting an array (`json_est_tabulatum`). Verify how the store materializes a `tags` mutation:
  - Run this test. If the `/mcp` assertion fails, the store keeps `tags` as an array only via `addere`; a `mutatio` of `tags` may not reshape it. In that case, **change the retag test to use `mutatio` with a raw `datum` object**: `\"datum\":\"{\\\"tags\\\":[\\\"mcp\\\"]}\"` — which sets `tags` to a real array. Use whichever form makes the store's materialized `res.datum.tags` an array; that is what reconcile consumes. This is a genuine store-behavior question the test pins — do not guess, run it.

- [ ] **Step 3: Run tests to verify they pass.**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: PASS — all `CREDO_VERUM` asserts green, exit 0. The full lifecycle (create → note → status → retag) is exercised and files reflect each write.

- [ ] **Step 4: Commit Tasks 5-7 together.**

```bash
git add gesta/fontes/tabularium.c gesta/probationes/probatio_tabularium.c
git commit -m "gesta entitates: reconcile + wire write callsites + tests

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 8: Removal + no-tag + nexus-link coverage

**Files:**
- Test: `gesta/probationes/probatio_tabularium.c` (extend the block).

**Interfaces:** none new — hardens Task 5-7 behavior.

- [ ] **Step 1: Add removal + no-tag + nexus assertions.**

```c
    {
        constans character* md;

        /* rem sine tags -> _sine_tag */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":210,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"nota\",\"titulus\":"
            "\"Sine Tag Res\",\"corpus\":\"nulla tag\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/_sine_tag");
        CREDO_VERUM (strstr(md, "Sine Tag Res") != NIHIL);

        /* nexus -> sectio Nexus cum ligamine relativo in ambabus rebus */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":211,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"addere\","
            "\"arguments\":{\"genus\":\"quaestio\",\"titulus\":"
            "\"Quaestio Nexa\",\"tags\":\"silva\"}}}");
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":212,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"gerere\","
            "\"arguments\":{\"res\":\"Probatio Entitatum Alpha\","
            "\"actus\":\"nexus\",\"verbum\":\"impedit\",\"alterum\":"
            "\"Quaestio Nexa\"}}}");
        md = _prima_plagula_md(piscina, VIA_ENT "/mcp");   /* Alpha nunc in mcp */
        CREDO_VERUM (strstr(md, "## Nexus") != NIHIL);
        CREDO_VERUM (strstr(md, "impedit") != NIHIL);
        CREDO_VERUM (strstr(md, "](../silva/quaestio-") != NIHIL);

        /* remotio rei -> omnes plagulae eius abeunt */
        (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":213,"
            "\"method\":\"tools/call\",\"params\":{\"name\":\"gerere\","
            "\"arguments\":{\"res\":\"Sine Tag Res\",\"actus\":"
            "\"remotio\",\"clavis\":\"corpus\"}}}");
        /* NOTA: 'remotio' hic removet CLAVEM 'corpus', non rem totam.
         * Si remotio-totius-rei non exsistit, muta hoc ut probet quod
         * plagula manet sed corpus abest. Vide caveat infra. */
    }
```

**Caveat on removal:** the store's `remotio` action (`tabularium.c:1928`) removes a **key** from the entity's datum (`{"clavis": "corpus"}`), not the whole entity — there is no "delete entity" verb in `gerere`. So an entity's files are never deleted by a normal `gerere`; `_entitatem_reconciliare`'s empty-datum branch is defensive (covers `gesta_res_datum` returning empty, e.g. via branch/GC paths). **Adjust the removal test** to assert the *content* changed (corpus gone) rather than the file gone:

```c
        md = _prima_plagula_md(piscina, VIA_ENT "/_sine_tag");
        CREDO_VERUM (strstr(md, "Sine Tag Res") != NIHIL);   /* manet */
        CREDO_VERUM (strstr(md, "nulla tag") == NIHIL);      /* corpus abest */
```

Confirm the actual `remotio` semantics by running; keep whichever assertion matches the store.

- [ ] **Step 2: Run tests.**

Run: `cd /Users/francisarant/Documents/projects/rhubarb/gesta && ./compile_probationes.sh tabularium`
Expected: PASS.

- [ ] **Step 3: Commit.**

```bash
git add gesta/probationes/probatio_tabularium.c
git commit -m "gesta entitates: removal/no-tag/nexus-link tests

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

### Task 9: Generate the live tree + worklog + ignore check

**Files:**
- Modify: `gesta/fontes/tabularium.worklog.md` (append)
- Possibly modify: `.gitignore`
- Generated: `gesta/annales/entities/**`

- [ ] **Step 1: Decide committed vs ignored.** `tabula.md` is committed (a projection diffed per commit). By the spec, `entities/` is the same. Confirm intent with Fran before committing ~200+ files. Default: **commit it** (matches `tabula.md`). If Fran prefers not to bloat history, add `gesta/annales/entities/` to `.gitignore` and skip Step 3.

- [ ] **Step 2: Generate the tree once** by starting the real server so `initialize` fires the full sweep:

Run: `cd /Users/francisarant/Documents/projects/rhubarb && printf '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{}}\n' | ./gesta/tabularium.sh`
Expected: `gesta/annales/entities/<tag>/*.md` now exist. Spot-check: `ls gesta/annales/entities/ && cat gesta/annales/entities/silva/*.md | head -40`.

- [ ] **Step 3: Append a worklog note** to `gesta/fontes/tabularium.worklog.md` (English is fine here) describing: the three-projection model, why reconcile deletes-then-writes (filesystem = memory of old tags), why bulk ops full-sweep, and the `remotio`-removes-a-key (not the entity) subtlety.

- [ ] **Step 4: Commit.**

```bash
git add gesta/fontes/tabularium.worklog.md gesta/annales/entities .gitignore
git commit -m "gesta entitates: generate live tree + worklog

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>"
```

---

## Deviations from the spec (recorded)

- **Cold rebuild lives at `initialize`, not `frigida.sh`.** Analysis showed `_initialize_tractare` fires the full sweep on every real session start, and cold writes (`nota_frigida`) flow through the same `tabularium` write path (incremental reconcile). So `frigida.sh` needs no change. A standalone `entitates_exportare` tool is a possible future add, not v1.
- **Reconcile deletes-then-writes** every touched entity's files (rather than diffing old vs new tag sets). Simpler and correct; deterministic content keeps `git diff` honest for unchanged files.
- **`remotio` removes a datum key, not the entity.** The empty-datum deletion branch in `_entitatem_reconciliare` is therefore defensive, exercised by branch/GC paths, not by normal `gerere`.

## Self-Review

- **Spec coverage:** layout+filenames (Task 2,4,5) ✓; duplicate-into-every-tag (Task 5) ✓; full archive incl. closed, flat (Task 5 writes regardless of status; no status filter) ✓; live+incremental on write (Task 7 single-entity callsites) ✓; per-entity reconcile against filesystem (Task 5) ✓; full body frontmatter+corpus+status+notae+nexus+ancorae (Task 4) ✓; nexus relative links (Task 4 `_entitatem_semita_relativa`) ✓; deterministic content (no clock; all timestamps from rows/events) ✓; `_sine_tag` (Task 5) ✓; config `NIHIL` disables (Task 1, every function guards `via_entitatum == NIHIL`) ✓; cold rebuild (Task 7 init) ✓.
- **Placeholder scan:** no "TBD"/"handle edge cases"; the two store-behavior questions (tags-mutation reshape, remotio semantics) are pinned by concrete runnable tests with both resolution branches spelled out, not left vague.
- **Type consistency:** `_entitatem_reconciliare(constans character* res_id)` called with a C-string at every callsite (bare buffer at addere; `_litterae(pn, chorda)` at gerere). `_entitatem_ad_markdown` and `_entis_plagulas_omnes_delere` take `constans character* res_id`; `_entitatem_semita_relativa` takes `chorda membrum_id` (that's what `GestaSocius.membrum` is). Slug/filename return `chorda`, consumed via `_litterae` when a C-string path is needed. Consistent.
```
