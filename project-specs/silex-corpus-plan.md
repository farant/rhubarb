# Silex Corpus Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Silex carries the rhubarb library corpus inside itself (second capsula), the closure walk learns the `.m`/vendor rules, renovare re-roots from project sources, and a new `partes` verb makes silex the closure oracle — so a project session never needs the fabrica directory.

**Architecture:** A `SilexFons` lector seam (DISCUS = fabrica on disk | CORPUS = embedded capsula) threads through the one function that reads library files. Struere generates the corpus capsula with a commit stamp. Spec: `project-specs/silex-corpus-spec.md`.

**Tech Stack:** C89 (Latin), capsula (existing embed format), volumen, credo, bash (struere).

## Global Constraints

- Resolution order after Task 6: `-f` > ascent-from-cwd > embedded corpus. **SILEX_FABRICA retired in Task 6** (code branch at tools/silex.c:480 + `~/.zshenv` line). Until Task 6, the env branch STAYS (the corpus fallback doesn't exist yet).
- Corpus contents: `lib/*.c`, `lib/*.m`, `include/*.h`, `vendor/*`, `tools/capsula_generare.c`, plus stamp entry `corpus.versio`. Keys = fabrica-relative paths. Generated pair `build/capsula_corpus_silicis.{h,c}` is NEVER committed; root artifacts `corpus.versio` + `corpus_silicis.toml` are gitignored.
- Suite: `./compile_tests.sh silex` (extends `probationes/probatio_silex.c`; runs from repo root, fabrica = "."). Exit 2 = NOTHING RAN.
- **Pre-fix-oracle law (Task 4):** the ADDENDA-from-authored-include test must be run and SEEN FAILING against pre-change code before the fix lands.
- House: C89 Latin identifiers; latina.h macros forbidden as identifiers; `i32/i64` unsigned, `s32/s64` signed; commits end `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`; stage by explicit path only (NEVER Fran's staging files: FAQ.md, gesta/annales/*).
- Verified facts (2026-08-10): capsula format = header 12B (magic 0x53504143 LE, versio 1, numerus), TOC 20B/entry {path_offset, path_len, data_offset, comp_size, raw_size} — offsets absolute into blob, `compressa = (comp != raw)`; `capsula_aperire(embed, piscina)`, `capsula_legere(cap, via_cstr, piscina)` → `CapsulaFructus{status, datum}`, `capsula_habet(cap, via)`; capsula_generare: `X_files`/`X_compress` toml keys, globs one level (`lib/*.c`), keys stored as listed, output `capsula_X.{h,c}` beside the toml, symbol `capsula_X`; existing closure: `silex_clausuram_colligere(piscina, fabrica, semina, n)` BFS with `visa` dedup tabula, disk reads ONLY in `_plagulam_e_fabrica_colligere` (lib/silex.c:183), `.h→.c` twin at :281 (return value currently ignored); renovare seeds at :1659-1712 ("semina = capita vendicata"); include scanner `_inclusiones_scrutari(contentum, opus)` captures quoted names ending `.h`.

---

## File Structure

- Modify: `include/silex.h` — SilexFons type + constructors + fons ops; signature changes (colligere, renovare, SilexNovumOptiones); SilexPartesRes + silex_partes
- Modify: `lib/silex.c` — fons impl; `_plagulam_e_fonte_colligere`; vendor/.m rules; renovare seeds; `_semina_auctorata_colligere`; silex_partes
- Modify: `tools/silex.c` — fons construction, partes verb, `-machina`/`-versio` flags, resolution rework (Task 6), corpus extern (Task 6)
- Modify: `probationes/probatio_silex.c` — fons migration; corpus fictum builder; differential + rule + oracle + partes tests
- Modify: `tools/silex_struere.sh` — corpus generation step (Task 6)
- Modify: `.gitignore`, `~/.zshenv` (Task 6), `lib/silex.worklog.md` (Task 7)

---

### Task 1: SilexFons seam + API migration (disk parity)

Behavior must be IDENTICAL to today for disk sources; every existing test stays green. This task only introduces the seam and migrates signatures.

**Files:**
- Modify: `include/silex.h`, `lib/silex.c`, `tools/silex.c`, `probationes/probatio_silex.c`

**Interfaces (produced, used by every later task):**

```c
/* silex.h — post #include "xar.h", adde: */
#include "capsula.h"

nomen enumeratio {
    SILEX_FONS_DISCUS = 0,   /* fabrica in disco */
    SILEX_FONS_CORPUS        /* capsula in binario infixa */
} SilexFonsGenus;

nomen structura {
    SilexFonsGenus      genus;
    constans character* fabrica;   /* DISCUS: radix arboris */
    Capsula*            capsula;   /* CORPUS: corpus apertum */
    constans character* titulus;   /* pro nuntiis: via aut stampa */
} SilexFons;

/* DISCUS: include/ adsit; NIHIL si invalida */
SilexFons*
silex_fons_disci (
    Piscina*            piscina,
    constans character* fabrica);

/* CORPUS: capsulam aperit; titulus e clave 'corpus.versio'
 * (absente: "(corpus sine stampa)"); NIHIL si capsula fracta */
SilexFons*
silex_fons_corporis (
    Piscina*               piscina,
    constans CapsulaEmbed* embed);

b32
silex_fons_existit (
    constans SilexFons* fons,
    constans character* via_relativa,
    Piscina*            piscina);

/* *inventum FALSUM si via ignota */
chorda
silex_fons_legere (
    constans SilexFons* fons,
    constans character* via_relativa,
    Piscina*            piscina,
    b32*                inventum);
```

Signature changes: `silex_clausuram_colligere(piscina, constans SilexFons* fons, semina, n)`; `SilexNovumOptiones.fabrica` → `constans SilexFons* fons;`; `silex_renovare(piscina, proiectum_dir, constans SilexFons* fons, scribere)`. Header comments updated accordingly.

- [ ] **Step 1: lib/silex.c — fons implementation** (place after `silex_fabricam_invenire`):

```c
/* ==================================================
 * Fons bibliothecarum (DISCUS | CORPUS)
 * ================================================== */

SilexFons*
silex_fons_disci (Piscina* piscina, constans character* fabrica)
{
    SilexFons* fons;

    si (fabrica == NIHIL || !filum_directorium_existit(
        _texere(piscina, fabrica, "/include", NIHIL)))
    {
        redde NIHIL;
    }
    fons = (SilexFons*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilexFons));
    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    fons->genus   = SILEX_FONS_DISCUS;
    fons->fabrica = fabrica;
    fons->capsula = NIHIL;
    fons->titulus = fabrica;
    redde fons;
}

SilexFons*
silex_fons_corporis (Piscina* piscina,
    constans CapsulaEmbed* embed)
{
    SilexFons* fons;
    Capsula*   capsula = capsula_aperire(embed, piscina);

    si (capsula == NIHIL)
    {
        redde NIHIL;
    }
    fons = (SilexFons*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilexFons));
    si (fons == NIHIL)
    {
        redde NIHIL;
    }
    fons->genus   = SILEX_FONS_CORPUS;
    fons->fabrica = NIHIL;
    fons->capsula = capsula;
    fons->titulus = "(corpus sine stampa)";
    {
        CapsulaFructus stampa = capsula_legere(capsula,
            "corpus.versio", piscina);

        si (stampa.status == CAPSULA_OK
            && stampa.datum.mensura > 0)
        {
            chorda linea = stampa.datum;

            /* linea prima sine terminatione */
            dum (linea.mensura > 0
                && (linea.datum[linea.mensura - 1] == '\n'
                    || linea.datum[linea.mensura - 1] == '\r'))
            {
                linea.mensura = linea.mensura - 1;
            }
            fons->titulus = _texere(piscina, "(corpus ",
                chorda_ut_cstr(linea, piscina), ")");
        }
    }
    redde fons;
}

b32
silex_fons_existit (constans SilexFons* fons,
    constans character* via_relativa, Piscina* piscina)
{
    si (fons->genus == SILEX_FONS_CORPUS)
    {
        redde capsula_habet(fons->capsula, via_relativa);
    }
    redde filum_existit(_texere(piscina, fons->fabrica, "/",
        via_relativa));
}

chorda
silex_fons_legere (constans SilexFons* fons,
    constans character* via_relativa, Piscina* piscina,
    b32* inventum)
{
    chorda vacua;

    vacua.datum = NIHIL;
    vacua.mensura = 0;
    *inventum = FALSUM;
    si (fons->genus == SILEX_FONS_CORPUS)
    {
        CapsulaFructus f = capsula_legere(fons->capsula,
            via_relativa, piscina);

        si (f.status != CAPSULA_OK)
        {
            redde vacua;
        }
        *inventum = VERUM;
        redde f.datum;
    }
    {
        constans character* via_plena = _texere(piscina,
            fons->fabrica, "/", via_relativa);

        si (!filum_existit(via_plena))
        {
            redde vacua;
        }
        *inventum = VERUM;
        redde filum_legere_totum(via_plena, piscina);
    }
}
```

- [ ] **Step 2: rename + rewire the collector.** `_plagulam_e_fabrica_colligere` → `_plagulam_e_fonte_colligere(piscina, constans SilexFons* fons, pars, titulus, fructus, opus)`; body builds `via_rel` as before, then:

```c
    si (!silex_fons_existit(fons, via_rel_cstr, piscina))
    {
        redde FALSUM;
    }
    {
        b32 inventum = FALSUM;

        contentum = silex_fons_legere(fons, via_rel_cstr, piscina,
            &inventum);
        si (!inventum)
        {
            redde FALSUM;
        }
    }
```

(rest unchanged: xar_addere, via/contentum/origo, `_inclusiones_scrutari`). In `silex_clausuram_colligere`: parameter `constans SilexFons* fons` replaces `fabrica`; the `include/` directory validation at the head becomes `si (fons == NIHIL) { redde NIHIL; }` (validation moved into the constructors); both `_plagulam_e_fabrica_colligere` call sites become `_plagulam_e_fonte_colligere(piscina, fons, ...)`.

- [ ] **Step 3: migrate silex_novum and silex_renovare.** In `silex_novum`: `optiones->fabrica` → `optiones->fons` (the colligere call; any error text naming the fabrica uses `optiones->fons->titulus`). In `silex_renovare`: parameter `constans SilexFons* fons`; the colligere call passes `fons`; the erratum "fabrica invalida - clausura colligi non potuit" stays (fons NIHIL cannot reach here — the tool constructs it).

- [ ] **Step 4: migrate tools/silex.c minimally.** After the existing resolution chain (KEEP the SILEX_FABRICA branch this task), construct once:

```c
    SilexFons* fons = NIHIL;

    si (fabrica != NIHIL)
    {
        fons = silex_fons_disci(piscina, fabrica);
    }
```

`renovare` handler: replace the `fabrica == NIHIL` check with `fons == NIHIL` (same message), call `silex_renovare(piscina, via_proiecti, fons, applicare)`, print `fons->titulus` instead of `fabrica`. `novum`: `optiones.fons = fons;` and the fons==NIHIL case reuses the existing "fabrica ignota" refusal; the "(e %s)" print uses `fons->titulus`.

- [ ] **Step 5: migrate probatio_silex.c.** Every `silex_clausuram_colligere(piscina, ".", ...)` → `silex_clausuram_colligere(piscina, silex_fons_disci(piscina, "."), ...)`; the "fabrica invalida" test constructs `silex_fons_disci(piscina, AREA "/nusquam")` and asserts NIHIL (constructor now carries the refusal); renovare calls gain `silex_fons_disci(piscina, AREA "/ficta")`. Add a small section asserting fons basics:

```c
    /* ========================================================
     * PROBARE: fons disci - existit/legere
     * ======================================================== */
    {
        SilexFons* fons = silex_fons_disci(piscina, ".");
        b32        inventum = FALSUM;
        chorda     contentum;

        imprimere("\n--- Probans fontem disci ---\n");
        CREDO_NON_NIHIL(fons);
        CREDO_VERUM(silex_fons_existit(fons, "include/latina.h",
            piscina));
        CREDO_FALSUM(silex_fons_existit(fons, "include/nusquam.h",
            piscina));
        contentum = silex_fons_legere(fons, "include/latina.h",
            piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_VERUM(contentum.mensura > 0);
        CREDO_NIHIL(silex_fons_disci(piscina, AREA "/nusquam"));
    }
```

- [ ] **Step 6: green + commit**

```bash
./compile_tests.sh silex
git add include/silex.h lib/silex.c tools/silex.c probationes/probatio_silex.c
git commit -m "silex: sutura fontis (SilexFons DISCUS|CORPUS) - paritas disci

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

Expected: probatio_silex green, `./tools/silex_struere.sh` still builds.

---

### Task 2: corpus fictum + differential gate

**Files:** Modify: `probationes/probatio_silex.c`

**Interfaces:** Produces `_corpus_fictum(piscina, paria, numerus)` → `CapsulaEmbed` used by Tasks 3–6 tests. `ParFictum {constans character* via; constans character* contentum;}`.

- [ ] **Step 1: builder helpers** (top of probatio, after `_manifestum_continet`):

```c
nomen structura {
    constans character* via;
    constans character* contentum;
} ParFictum;

interior vacuum
_i32_le_scribere (i8* p, i32 v);

interior vacuum
_i32_le_scribere (i8* p, i32 v)
{
    p[0] = (i8)(v & 0xFF);
    p[1] = (i8)((v >> VIII) & 0xFF);
    p[2] = (i8)((v >> XVI) & 0xFF);
    p[3] = (i8)((v >> XXIV) & 0xFF);
}

interior CapsulaEmbed
_corpus_fictum (Piscina* piscina, constans ParFictum* paria,
    i32 numerus);

/* capsula in memoria: caput XII + TOC XX/introitum + chordae +
 * data; comp==raw = introitus crudus (sine flatura) */
interior CapsulaEmbed
_corpus_fictum (Piscina* piscina, constans ParFictum* paria,
    i32 numerus)
{
    CapsulaEmbed embed;
    i32 chordae_mensura = 0;
    i32 datorum_mensura = 0;
    i32 i;
    i32 summa;
    i8* buf;
    i32 chorda_off;
    i32 datum_off;

    per (i = 0; i < numerus; i = i + 1)
    {
        chordae_mensura = chordae_mensura
            + (i32)strlen(paria[i].via);
        datorum_mensura = datorum_mensura
            + (i32)strlen(paria[i].contentum);
    }
    summa = XII + numerus * XX + chordae_mensura + datorum_mensura;
    buf = (i8*)piscina_allocare(piscina, (memoriae_index)summa);
    si (buf == NIHIL)
    {
        embed.datum = NIHIL;
        embed.mensura = 0;
        redde embed;
    }
    _i32_le_scribere(buf, (i32)0x53504143);
    _i32_le_scribere(buf + IV, I);
    _i32_le_scribere(buf + VIII, numerus);
    chorda_off = XII + numerus * XX;
    datum_off  = chorda_off + chordae_mensura;
    per (i = 0; i < numerus; i = i + 1)
    {
        i8* introitus = buf + XII + i * XX;
        i32 via_mensura = (i32)strlen(paria[i].via);
        i32 datum_mensura = (i32)strlen(paria[i].contentum);

        _i32_le_scribere(introitus, chorda_off);
        _i32_le_scribere(introitus + IV, via_mensura);
        _i32_le_scribere(introitus + VIII, datum_off);
        _i32_le_scribere(introitus + XII, datum_mensura);
        _i32_le_scribere(introitus + XVI, datum_mensura);
        memcpy(buf + chorda_off, paria[i].via,
            (size_t)via_mensura);
        memcpy(buf + datum_off, paria[i].contentum,
            (size_t)datum_mensura);
        chorda_off = chorda_off + via_mensura;
        datum_off  = datum_off + datum_mensura;
    }
    embed.datum = buf;
    embed.mensura = summa;
    redde embed;
}
```

(`#include <string.h>` joins the probatio includes if absent.)

- [ ] **Step 2: differential test.** New section (uses AREA/ficta2 on disk AND the same files as corpus):

```c
    /* ========================================================
     * PROBARE: fons corporis == fons disci (porta differentialis)
     * ======================================================== */
    {
        interior constans ParFictum PARIA[] = {
            { "corpus.versio", "commit=abc123 dies=2026-08-10\n" },
            { "include/minima.h", "/* minima */\n" },
            { "lib/minima.c",
              "#include \"minima.h\"\n/* corpus m */\n" },
            { "include/altera.h",
              "#include \"minima.h\"\n" },
            { "lib/altera.c", "/* altera */\n" }
        };
        interior constans character* constans SEMINA2[] = {
            "altera.h"
        };
        SilexFons* discus;
        SilexFons* corpus;
        Xar*       ex_disco;
        Xar*       ex_corpore;
        i32        index;
        CapsulaEmbed embed = _corpus_fictum(piscina, PARIA, 5);

        imprimere("\n--- Probans corpus == discus ---\n");
        filum_directorium_creare_si_necesse(AREA "/ficta2");
        filum_directorium_creare_si_necesse(AREA "/ficta2/include");
        filum_directorium_creare_si_necesse(AREA "/ficta2/lib");
        per (index = 1; index < 5; index = index + 1)
        {
            chorda c = chorda_ex_literis(PARIA[index].contentum,
                piscina);

            CREDO_VERUM(filum_scribere(_texere_probationis(piscina,
                AREA "/ficta2/", PARIA[index].via), c));
        }

        discus = silex_fons_disci(piscina, AREA "/ficta2");
        corpus = silex_fons_corporis(piscina, &embed);
        CREDO_NON_NIHIL(discus);
        CREDO_NON_NIHIL(corpus);
        CREDO_CHORDA_CONTINET(chorda_ex_literis(corpus->titulus,
            piscina), chorda_ex_literis("abc123", piscina));

        ex_disco = silex_clausuram_colligere(piscina, discus,
            SEMINA2, I);
        ex_corpore = silex_clausuram_colligere(piscina, corpus,
            SEMINA2, I);
        CREDO_NON_NIHIL(ex_disco);
        CREDO_NON_NIHIL(ex_corpore);
        CREDO_AEQUALIS_I32(xar_numerus(ex_disco),
            xar_numerus(ex_corpore));
        per (index = 0; index < xar_numerus(ex_disco);
            index = index + 1)
        {
            SilexRes* a = (SilexRes*)xar_obtinere(ex_disco, index);
            SilexRes* b = (SilexRes*)xar_obtinere(ex_corpore,
                index);

            CREDO_CHORDA_AEQUALIS(a->via, b->via);
            CREDO_CHORDA_AEQUALIS(a->contentum, b->contentum);
        }
        /* clausura plena: altera.h -> altera.c + minima.h ->
         * minima.c (ordo BFS idem ambobus) */
        CREDO_AEQUALIS_I32(xar_numerus(ex_disco), IV);
    }
```

Helper `_texere_probationis` (the probatio has no `_texere`):

```c
interior constans character*
_texere_probationis (Piscina* piscina, constans character* a,
    constans character* b);

interior constans character*
_texere_probationis (Piscina* piscina, constans character* a,
    constans character* b)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}
```

(pulls `#include "chorda_aedificator.h"` into the probatio if absent.)

- [ ] **Step 3: green + commit**

```bash
./compile_tests.sh silex
git add probationes/probatio_silex.c
git commit -m "silex: corpus fictum + porta differentialis (corpus==discus)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 3: closure rules — .m twin + vendor pair

**Files:** Modify: `lib/silex.c` (silex_clausuram_colligere), `probationes/probatio_silex.c`

- [ ] **Step 1: failing tests first.** Extend the differential fixture: add to PARIA (and mirrored disk writes) `{"include/obiectiva.h", "/* ob */\n"}`, `{"lib/obiectiva_macos.m", "/* ob m */\n"}`, `{"vendor/parva.h", "/* v */\n"}`, `{"vendor/parva.c", "/* vc */\n"}`, and change `lib/minima.c` content to `"#include \"minima.h\"\n#include \"../vendor/parva.h\"\n"`. Add seed `"obiectiva.h"` (SEMINA2 grows to 2). New assertions in that section:

```c
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "lib/obiectiva_macos.m"));
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "vendor/parva.h"));
        CREDO_VERUM(_manifestum_continet(ex_disco,
            "vendor/parva.c"));
        CREDO_FALSUM(_manifestum_continet(ex_disco,
            "include/../vendor/parva.h"));
```

(and drop the exact-count assertion to `CREDO_AEQUALIS_I32(xar_numerus(ex_disco), IX);` — minima pair 2 + altera pair 2 + obiectiva.h + obiectiva_macos.m + vendor pair 2 + nothing for corpus.versio = 8… count carefully at execution against the real BFS and pin the true number). Run `./compile_tests.sh silex` — MUST FAIL (no .m, no normalized vendor).

- [ ] **Step 2: implement in silex_clausuram_colligere.** Add helper:

```c
interior b32
_vendor_basim (chorda caput, chorda* basis);

/* 'vendor/' in nomine (quocumque praefixo, e.g. '../vendor/x.h')
 * -> basis post eam, si sine '/' ulteriore */
interior b32
_vendor_basim (chorda caput, chorda* basis)
{
    i32 i;

    per (i = 0; i + 7 <= caput.mensura; i = i + 1)
    {
        si (memcmp(caput.datum + i, "vendor/", 7) == 0)
        {
            chorda cauda = chorda_ex_buffer(caput.datum + i + 7,
                caput.mensura - i - 7);
            i32 j;

            per (j = 0; j < cauda.mensura; j = j + 1)
            {
                si (cauda.datum[j] == '/')
                {
                    redde FALSUM;
                }
            }
            *basis = cauda;
            redde VERUM;
        }
    }
    redde FALSUM;
}
```

In the BFS loop, immediately after popping `caput` and BEFORE the visa check, normalize: if `_vendor_basim(caput, &basis)` then `caput = _praefigere(piscina, "vendor/", basis)`-style normalized name is used for the visa key, and the handling branch is:

```c
        si (est_vendor)
        {
            si (!_plagulam_e_fonte_colligere(piscina, fons,
                "vendor/", basis, fructus, opus))
            {
                fprintf(stderr, "silex: monitio - venditorium"
                    " citatum in fonte deest: %.*s\n",
                    (integer)basis.mensura,
                    (constans character*)basis.datum);
            }
            /* geminus .c si adest */
            si (basis.mensura > 2
                && basis.datum[basis.mensura - 2] == '.'
                && basis.datum[basis.mensura - 1] == 'h')
            {
                chorda basis_c = _praefigere(piscina, "",
                    chorda_ex_buffer(basis.datum,
                        basis.mensura - 1));
                /* '.h' -> '.c': litteram ultimam mutare */
                basis_c.datum[basis_c.mensura - 1] = 'c';
                (vacuum)_plagulam_e_fonte_colligere(piscina, fons,
                    "vendor/", basis_c, fructus, opus);
            }
            perge;
        }
```

(`_praefigere` copies into a fresh builder buffer, so mutating the last byte is safe — it is OUR allocation, not the source text.) `.m` twin at the existing lib-twin call (:298):

```c
            si (!_plagulam_e_fonte_colligere(piscina, fons, "lib/",
                titulus_c, fructus, opus))
            {
                /* geminus obiectivus: conventione aedilis
                 * lib/X_macos.m ubi lib/X.c abest */
                ChordaAedificator* aed_m =
                    chorda_aedificator_creare(piscina,
                        (memoriae_index)64);

                chorda_aedificator_appendere_chorda(aed_m,
                    radix_capitis);
                chorda_aedificator_appendere_literis(aed_m,
                    "_macos.m");
                (vacuum)_plagulam_e_fonte_colligere(piscina, fons,
                    "lib/", chorda_aedificator_finire(aed_m),
                    fructus, opus);
            }
```

- [ ] **Step 3: green (both fons genera prove it via the differential loop) + commit**

```bash
./compile_tests.sh silex
git add lib/silex.c probationes/probatio_silex.c
git commit -m "silex: clausura discit gemina .m + paria venditoria (normalizata)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 4: renovare re-rooted (pre-fix oracle FIRST)

**Files:** Modify: `probationes/probatio_silex.c`, `lib/silex.c`

- [ ] **Step 1: the oracle test — written and SEEN FAILING before any lib change.** In the existing renovare section (AREA/ficta + the specimen project already built there), append after the current flows:

```c
        /* re-radicatio: plagula auctorata inclusionem novam accipit
         * -> ADDENDA e clausura recomputata (ORACULUM: ante emendationem
         * hoc FALLIT - 'omnia recentia', mensuratum in vocabulario) */
        CREDO_VERUM(filum_scribere(AREA "/ficta/include/addenda.h",
            chorda_ex_literis("/* addenda */\n", piscina)));
        CREDO_VERUM(filum_scribere(AREA "/ficta/lib/addenda.c",
            chorda_ex_literis("/* addenda c */\n", piscina)));
        CREDO_VERUM(filum_scribere(
            AREA "/specimen/fontes/novus.c",
            chorda_ex_literis("#include \"addenda.h\"\n",
                piscina)));
        {
            SilexRenovatioFructus rr = silex_renovare(piscina,
                AREA "/specimen",
                silex_fons_disci(piscina, AREA "/ficta"), FALSUM);
            b32 addenda_caput = FALSUM;
            b32 addenda_lib = FALSUM;
            i32 k;

            CREDO_VERUM(rr.successus);
            per (k = 0; k < xar_numerus(rr.res); k = k + 1)
            {
                SilexRenovatioRes* r = (SilexRenovatioRes*)
                    xar_obtinere(rr.res, k);

                si (r->status == SILEX_RENOVATIO_ADDENDA)
                {
                    si (chorda_aequalis_literis(r->via,
                        "include/addenda.h"))
                    {
                        addenda_caput = VERUM;
                    }
                    si (chorda_aequalis_literis(r->via,
                        "lib/addenda.c"))
                    {
                        addenda_lib = VERUM;
                    }
                }
            }
            CREDO_VERUM(addenda_caput);
            CREDO_VERUM(addenda_lib);
        }
```

(Adjust the specimen path name to the one the existing renovare test actually uses — read that section first; if the specimen has no `fontes/` dir, `filum_directorium_creare_si_necesse` it. The authored file is DISK-ONLY, deliberately not condere'd — the collector must walk disk, not the manifest, or the vocabularium pre-condere scenario stays broken.)

- [ ] **Step 2: run and verify RED**

```bash
./compile_tests.sh silex
```
Expected: FAIL exactly on `addenda_caput`/`addenda_lib` (current seeds = vendored capita only). If it passes, STOP — the understanding of current behavior is wrong; re-read before touching anything.

- [ ] **Step 3: implement the seed collector.** In lib/silex.c, locate silex_status' disk walk and its skip rules (praetermissa: bin/, build/, *.volumen(-wal/-shm), dotfiles — silex.h:82 documents them); factor the per-via skip predicate into a shared `interior b32 _praetermittenda(chorda via)` if it isn't one already, and reuse the same directory-walk machinery (iter_directoria) for:

```c
interior vacuum
_semina_auctorata_colligere (Piscina* piscina,
    constans character* radix_absoluta, Xar* nomina,
    TabulaDispersa* bases_auctoratae);

/* plagulae auctoratae = arbor disci sine lib/ include/ vendor/
 * (praetermissis solitis); inclusiones earum in nomina, bases
 * earum in bases_auctoratae (capita propria non quaerenda) */
```

Implementation: walk `radix_absoluta` recursively (same recursion the status walk uses); for each via where `!_praetermittenda(via)` and via lacks the three library prefixes and the basename does not start with `capsula_`: insert basename (text after last '/') into `bases_auctoratae`; if via ends `.c`, `.h`, or `.m`: `_inclusiones_scrutari(filum_legere_totum(...), nomina)`.

Then replace the seed block (lines 1659-1712, the `{ ... clausura = silex_clausuram_colligere(...) }` braces) with:

```c
    /* semina = inclusiones plagularum auctoratarum (disci - etiam
     * ante conditionem) + capita vendicata (continuitas); capita
     * propria proiecti (bases auctoratae) excluduntur. Ante:
     * capita vendicata SOLA - codex novus clausuram numquam
     * trahebat (mensuratum in vocabulario 2026-08-09). */
    {
        Xar*            nomina = xar_creare(piscina,
            (i32)magnitudo(chorda));
        TabulaDispersa* bases = tabula_dispersa_creare_chorda(
            piscina, 32);
        TabulaDispersa* electa = tabula_dispersa_creare_chorda(
            piscina, 64);
        Xar*            semina_ch = xar_creare(piscina,
            (i32)magnitudo(chorda));
        TabulaIterator  iter;
        chorda          clavis;
        vacuum*         valor;
        i32             n;

        si (nomina == NIHIL || bases == NIHIL || electa == NIHIL
            || semina_ch == NIHIL)
        {
            volumen_claudere(vol);
            fructus.erratum = "memoria defecit";
            redde fructus;
        }
        _semina_auctorata_colligere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), nomina,
            bases);
        iter = tabula_dispersa_iterator_initium(tunc);
        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
            &valor))
        {
            si (_praefixum_habet(clavis, "include/"))
            {
                chorda* cella = (chorda*)xar_addere(nomina);

                si (cella != NIHIL)
                {
                    *cella = chorda_ex_buffer(clavis.datum + 8,
                        clavis.mensura - 8);
                }
            }
        }
        per (n = 0; n < xar_numerus(nomina); n = n + 1)
        {
            chorda nomen_capitis = *(chorda*)xar_obtinere(nomina,
                n);

            si (tabula_dispersa_continet(bases, nomen_capitis)
                || tabula_dispersa_continet(electa,
                    nomen_capitis))
            {
                perge;
            }
            tabula_dispersa_inserere(electa, nomen_capitis,
                (vacuum*)nomina);
            {
                chorda* cella = (chorda*)xar_addere(semina_ch);

                si (cella != NIHIL)
                {
                    *cella = nomen_capitis;
                }
            }
        }
        si (xar_numerus(semina_ch) == 0)
        {
            volumen_claudere(vol);
            fructus.res = xar_creare(piscina,
                (i32)magnitudo(SilexRenovatioRes));
            fructus.successus = VERUM;
            redde fructus;
        }
        {
            constans character** semina = (constans character**)
                piscina_allocare(piscina, (memoriae_index)(
                    (memoriae_index)xar_numerus(semina_ch)
                    * magnitudo(constans character*)));
            i32 s;

            si (semina == NIHIL)
            {
                volumen_claudere(vol);
                fructus.erratum = "memoria defecit";
                redde fructus;
            }
            per (s = 0; s < xar_numerus(semina_ch); s = s + 1)
            {
                semina[s] = chorda_ut_cstr(
                    *(chorda*)xar_obtinere(semina_ch, s),
                    piscina);
            }
            clausura = silex_clausuram_colligere(piscina, fons,
                (constans character* constans*)semina,
                xar_numerus(semina_ch));
        }
    }
```

NOTE: `radix_absoluta` is currently computed at :1739, AFTER the seed block — MOVE that assignment above this block. The vendor names from scrutari ("../vendor/x.h") ride through as seeds; colligere normalizes them (Task 3).

- [ ] **Step 4: stamp in the conditio actum.** In the renovare apply block's JSON aedificator (:1921), after the additae field:

```c
        chorda_aedificator_appendere_literis(aed, ",\"fons\":\"");
        chorda_aedificator_appendere_literis(aed, fons->titulus);
        chorda_aedificator_appendere_literis(aed, "\"}");
```

(replacing the bare `"}"` append; fons->titulus is a path or our own stamp — no JSON-hostile characters by construction).

- [ ] **Step 5: green + full suite + commit**

```bash
./compile_tests.sh silex
git add lib/silex.c probationes/probatio_silex.c
git commit -m "silex: renovare re-radicatum - semina e plagulis auctoratis (oraculum vocabularii)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 5: silex partes — the oracle verb

**Files:** Modify: `include/silex.h`, `lib/silex.c`, `tools/silex.c`, `probationes/probatio_silex.c`

**Interfaces:**

```c
/* silex.h */
nomen structura {
    chorda              via;
    constans character* origo;
    b32                 adest;   /* in arbore proiecti */
} SilexPartesRes;

/* plagula data (via in disco proiecti) aut NIHIL = auctoratae
 * omnes; Xar de SilexPartesRes; NIHIL si clausura colligi nequit */
Xar*
silex_partes (
    Piscina*            piscina,
    constans character* proiectum_dir,
    constans SilexFons* fons,
    constans character* plagula);
```

NOTE: the spec says `partes [plagula ...]` (plural); v1 narrows to AT
MOST ONE named plagula because the argumenta parser has two fixed
positionals (verbum + titulus). Deliberate narrowing, recorded in the
worklog; plural arrives when the parser grows varargs.

- [ ] **Step 1: failing test.** On the Task 4 fixtures (specimen with `fontes/novus.c` including addenda.h, BEFORE any `-scribere` ran — order this section before the renovare-scribere flows or use a fresh copy):

```c
    /* ========================================================
     * PROBARE: partes - oraculum clausurae cum columna ADEST
     * ======================================================== */
    {
        Xar* partes = silex_partes(piscina, AREA "/specimen",
            silex_fons_disci(piscina, AREA "/ficta"), NIHIL);
        b32  addenda_abest = FALSUM;
        b32  minima_adest = FALSUM;
        i32  k;

        imprimere("\n--- Probans partes ---\n");
        CREDO_NON_NIHIL(partes);
        per (k = 0; k < xar_numerus(partes); k = k + 1)
        {
            SilexPartesRes* r = (SilexPartesRes*)xar_obtinere(
                partes, k);

            si (chorda_aequalis_literis(r->via,
                "include/addenda.h") && !r->adest)
            {
                addenda_abest = VERUM;
            }
            si (chorda_aequalis_literis(r->via,
                "include/minima.h") && r->adest)
            {
                minima_adest = VERUM;
            }
        }
        CREDO_VERUM(addenda_abest);
        CREDO_VERUM(minima_adest);
    }
```

(Assumes the specimen already vendored minima via earlier flows; verify against the actual fixture state when placing the section, and pin to whatever pair IS vendored there.) Run: FAIL (silex_partes undefined).

- [ ] **Step 2: implement silex_partes** in lib/silex.c (after renovare):

```c
Xar*
silex_partes (Piscina* piscina, constans character* proiectum_dir,
    constans SilexFons* fons, constans character* plagula)
{
    Xar*            nomina = xar_creare(piscina,
        (i32)magnitudo(chorda));
    TabulaDispersa* bases = tabula_dispersa_creare_chorda(piscina,
        32);
    TabulaDispersa* electa = tabula_dispersa_creare_chorda(
        piscina, 64);
    Xar*            semina_ch = xar_creare(piscina,
        (i32)magnitudo(chorda));
    chorda          radix_absoluta;
    Xar*            clausura;
    Xar*            fructus;
    i32             n;

    si (nomina == NIHIL || bases == NIHIL || electa == NIHIL
        || semina_ch == NIHIL || fons == NIHIL)
    {
        redde NIHIL;
    }
    radix_absoluta = via_absoluta(
        chorda_ex_literis(proiectum_dir, piscina), piscina);
    si (plagula != NIHIL)
    {
        constans character* via_plena = _texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            plagula);

        si (!filum_existit(via_plena))
        {
            redde NIHIL;
        }
        _inclusiones_scrutari(filum_legere_totum(via_plena,
            piscina), nomina);
    }
    alioquin
    {
        _semina_auctorata_colligere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), nomina,
            bases);
    }
    per (n = 0; n < xar_numerus(nomina); n = n + 1)
    {
        chorda nomen_capitis = *(chorda*)xar_obtinere(nomina, n);

        si (tabula_dispersa_continet(bases, nomen_capitis)
            || tabula_dispersa_continet(electa, nomen_capitis))
        {
            perge;
        }
        tabula_dispersa_inserere(electa, nomen_capitis,
            (vacuum*)nomina);
        {
            chorda* cella = (chorda*)xar_addere(semina_ch);

            si (cella != NIHIL)
            {
                *cella = nomen_capitis;
            }
        }
    }
    {
        constans character** semina = (constans character**)
            piscina_allocare(piscina, (memoriae_index)(
                ((memoriae_index)xar_numerus(semina_ch) + I)
                * magnitudo(constans character*)));
        i32 s;

        si (semina == NIHIL)
        {
            redde NIHIL;
        }
        per (s = 0; s < xar_numerus(semina_ch); s = s + 1)
        {
            semina[s] = chorda_ut_cstr(
                *(chorda*)xar_obtinere(semina_ch, s), piscina);
        }
        clausura = silex_clausuram_colligere(piscina, fons,
            (constans character* constans*)semina,
            xar_numerus(semina_ch));
    }
    si (clausura == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(SilexPartesRes));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    per (n = 0; n < xar_numerus(clausura); n = n + 1)
    {
        SilexRes*       e = (SilexRes*)xar_obtinere(clausura, n);
        SilexPartesRes* r = (SilexPartesRes*)xar_addere(fructus);

        si (r == NIHIL)
        {
            perge;
        }
        r->via = e->via;
        r->origo = e->origo;
        r->adest = filum_existit(_texere(piscina,
            chorda_ut_cstr(radix_absoluta, piscina), "/",
            chorda_ut_cstr(e->via, piscina)));
    }
    redde fructus;
}
```

- [ ] **Step 3: the verb.** tools/silex.c: add `-machina` vexillum (`argumenta_addere_vexillum(parser, "-machina", "--machina", "partes: TSV machinale");`), add "partes" to the VCS verb-gate condition list is NOT right (partes needs fons, not volumen — give it its own branch before the VCS block):

```c
    si (chorda_aequalis_literis(verbum, "partes"))
    {
        constans character* plagula = titulus.mensura > ZEPHYRUM
            ? chorda_ut_cstr(titulus, piscina) : NIHIL;
        b32  machina = argumenta_habet_vexillum(lecta,
            "--machina");
        Xar* partes;
        i32  index;

        si (fons == NIHIL)
        {
            fprintf(stderr, "silex partes: fons deest\n");
            redde II;
        }
        partes = silex_partes(piscina, ".", fons, plagula);
        si (partes == NIHIL)
        {
            fprintf(stderr, "silex partes: clausura colligi non"
                " potuit\n");
            redde II;
        }
        per (index = 0; index < xar_numerus(partes);
            index = index + 1)
        {
            SilexPartesRes* r = (SilexPartesRes*)xar_obtinere(
                partes, index);
            constans character* ordo =
                _praefixum_habet_literis(r->via, "vendor/") ? "V"
                : _praefixum_habet_literis(r->via, "include/")
                    ? "C"
                : (r->via.mensura > 2
                    && r->via.datum[r->via.mensura - 1] == 'm')
                    ? "M" : "O";
            constans character* praesentia = r->adest
                ? "ADEST" : "ABEST";

            si (machina)
            {
                imprimere("%s\t%s\t%.*s\n", ordo, praesentia,
                    (integer)r->via.mensura,
                    (constans character*)r->via.datum);
            }
            alioquin
            {
                imprimere("  %s %s  %.*s\n", ordo, praesentia,
                    (integer)r->via.mensura,
                    (constans character*)r->via.datum);
            }
        }
        si (!machina)
        {
            imprimere("(ABEST = quod 'renovare -scribere'"
                " vendicaret; fons %s)\n", fons->titulus);
        }
        redde ZEPHYRUM;
    }
```

`_praefixum_habet_literis` in tools/silex.c (check `grep -n incipit include/chorda.h` first — if a house `chorda_incipit_literis` exists, use it INSTEAD of defining this):

```c
interior b32
_praefixum_habet_literis (chorda via, constans character* praefixum);

interior b32
_praefixum_habet_literis (chorda via, constans character* praefixum)
{
    i32 m = (i32)strlen(praefixum);

    redde via.mensura >= m
        && memcmp(via.datum, praefixum, (size_t)m) == 0;
}
```

(`<string.h>` is already included.) Update the verb list in usage/description strings to include `partes`.

- [ ] **Step 4: green + commit**

```bash
./compile_tests.sh silex && ./tools/silex_struere.sh
git add include/silex.h lib/silex.c tools/silex.c probationes/probatio_silex.c
git commit -m "silex: verbum partes - oraculum clausurae cum columna ADEST (-machina TSV)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 6: corpus in struere + resolution rework + retirement of SILEX_FABRICA

**Files:** Modify: `tools/silex_struere.sh`, `tools/silex.c`, `.gitignore`, `~/.zshenv`

- [ ] **Step 1: struere corpus step.** Insert BEFORE the `mkdir -p bin` / compile block:

```bash
# corpus bibliothecarum: capsula infixa. Regeneratum SOLUM cum
# fontes mutantur - stampa commit eius temporis fixatur (semantica
# recta: stampa = ultima mutatio corporis, non ultima aedificatio)
CORPUS_C=build/capsula_corpus_silicis.c
regen=0
if [ ! -f "$CORPUS_C" ]; then
    regen=1
elif [ -n "$(find lib include vendor tools/capsula_generare.c \
        -newer "$CORPUS_C" -print -quit 2>/dev/null)" ]; then
    regen=1
fi
if [ "$regen" = 1 ]; then
    echo "  [corpus] stampa + capsula (tardum semel)"
    STAMPA="commit=$(git rev-parse --short HEAD 2>/dev/null \
        || echo ignotum)"
    if [ -n "$(git status --porcelain 2>/dev/null)" ]; then
        STAMPA="$STAMPA SORDIDUM"
    fi
    STAMPA="$STAMPA dies=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
    printf '%s\n' "$STAMPA" > corpus.versio
    cat > corpus_silicis.toml <<'TOML'
# GENERATUM a silex_struere.sh - NE MANU EDITES (gitignoratum)
corpus_silicis_files = ["lib/*.c", "lib/*.m", "include/*.h", "vendor/*", "tools/capsula_generare.c", "corpus.versio"]
corpus_silicis_compress = true
TOML
    if [ ! -x bin/capsula_generare ]; then
        ./compile_tools.sh capsula_generare >/dev/null || exit 1
    fi
    ./bin/capsula_generare corpus_silicis.toml || exit 1
    mv capsula_corpus_silicis.h capsula_corpus_silicis.c build/ \
        || exit 1
fi
```

and add `build/capsula_corpus_silicis.c` to the clang line. `.gitignore` gains (verify `build/` already ignored):

```
corpus.versio
corpus_silicis.toml
capsula_corpus_silicis.h
capsula_corpus_silicis.c
```

- [ ] **Step 2: tool rework.** tools/silex.c: extern near the other capsula include:

```c
/* corpus bibliothecarum a struere genitum - externus directus
 * (mos consumptoris: caput generatum non includitur) */
/* <aedilis obiectum="build/capsula_corpus_silicis.c"/> */
externus constans CapsulaEmbed capsula_corpus_silicis;
```

Resolution block (:470-489) becomes:

```c
    /* fons: optio > ascensus e cwd > corpus infixum
     * (SILEX_FABRICA REMOTUM 2026-08-10 - via missa = via viva) */
    fabrica_opt = argumenta_obtinere_optionem(lecta, "--fabrica",
        piscina);
    si (fabrica_opt.mensura > ZEPHYRUM)
    {
        fabrica = chorda_ut_cstr(fabrica_opt, piscina);
        fons = silex_fons_disci(piscina, fabrica);
        si (fons == NIHIL)
        {
            fprintf(stderr, "silex: fabrica invalida (include/"
                " deest): %s\n", fabrica);
            redde I;
        }
    }
    alioquin
    {
        fabrica = silex_fabricam_invenire(piscina, ".");
        si (fabrica != NIHIL)
        {
            fons = silex_fons_disci(piscina, fabrica);
        }
    }
    si (fons == NIHIL)
    {
        fons = silex_fons_corporis(piscina,
            &capsula_corpus_silicis);
        si (fons == NIHIL)
        {
            fprintf(stderr, "silex: nec fabrica nec corpus -"
                " binarium sine corpore aedificatum?\n");
            redde I;
        }
    }
```

Delete the novum-side `fabrica == NIHIL` refusal (fons is always non-NIHIL now); `-versio` vexillum (`argumenta_addere_vexillum(parser, "-versio", "--versio", "versionem et stampam corporis imprimere");`) handled right after parsing:

```c
    si (argumenta_habet_vexillum(lecta, "--versio"))
    {
        SilexFons* corpus = silex_fons_corporis(piscina,
            &capsula_corpus_silicis);

        imprimere("silex %s\n", SILEX_VERSIO);
        imprimere("corpus: %s\n", corpus == NIHIL
            ? "(FRACTUM aut abest)" : corpus->titulus);
        redde ZEPHYRUM;
    }
```

Update usage text: header comment, `-f` option help ("aut ascensus e cwd, aut corpus infixum"), delete both remaining "SILEX_FABRICA pone" strings, `_status_tractare`'s "(ignota - SILEX_FABRICA pone)" → "(corpus infixum)".

- [ ] **Step 3: retire the env var + rebuild**

```bash
sed -i '' '/SILEX_FABRICA/d' ~/.zshenv
grep -c SILEX_FABRICA ~/.zshenv || true    # 0
./tools/silex_struere.sh                    # corpus step runs (slow once)
./compile_tests.sh silex                    # suite untouched by tool changes but verify
```

- [ ] **Step 4: THE ACCEPTANCE WALK — unplug the fabrica.** One command, trap-guarded so the rename ALWAYS restores:

```bash
sh -c '
set -e
R="$HOME/Documents/projects/rhubarb"
restore() { [ -d "${R}_ABSCONDITUM" ] && mv "${R}_ABSCONDITUM" "$R"; }
trap restore EXIT
mv "$R" "${R}_ABSCONDITUM"
cd "$HOME/Documents/projects/silicetum"
rm -rf probandum
silex -versio
silex novum probandum -d .
cd probandum
printf "#include \"latina.h\"\n#include \"xar.h\"\ns32 principale(vacuum);\ns32 principale(vacuum){redde ZEPHYRUM;}\n" > fontes/probandum.c
silex partes
silex renovare -scribere
./aedificare.sh && ./probare.sh
silex condere -n "ambulatio acceptionis sine fabrica"
silex historia | head -5
'
```

Expected: `-versio` prints the corpus stamp; novum scaffolds FROM THE CORPUS (output names the stamp); partes lists xar pair ABEST; renovare vendors it; build+test green; condere works. Every step with the fabrica renamed away. (`rm -rf probandum` targets only the throwaway from a prior run of this walk.) Afterwards verify the fabrica is back: `ls "$HOME/Documents/projects/rhubarb/include/latina.h"`.

- [ ] **Step 5: commit**

```bash
git add tools/silex_struere.sh tools/silex.c .gitignore
git commit -m "silex: corpus infixum + resolutio -f>ascensus>corpus (SILEX_FABRICA remotum) + -versio

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 7: records

**Files:** Modify: `lib/silex.worklog.md`; tabularium; memory.

- [ ] **Step 1: worklog entry** — the corpus arc: what the differential gate caught (if anything), corpus size numbers (binary before/after, regen time), the acceptance walk transcript summary, the seed-collector design choice (disk walk not manifest — the pre-condere case), vendor normalization details.
- [ ] **Step 2: ledger** — on 01KZ6CSBQN: nota per shipped piece + the acceptance-walk result; if the walk passed, the desideratum's heart is met — ask Fran whether to close it as impletum (the `-vitrea` seed remains open; if closing, spawn a fresh desideratum for `-vitrea` so the remainder has a home).
- [ ] **Step 3: memory** — silex-volumen-project.md: corpus shipped, resolution order, "rebuild silex = update the world" workflow, partes verb; MEMORY.md bench: "will this project build without the fabrica" → the acceptance walk line; note SILEX_FABRICA no longer exists (memories referencing it are stale).
- [ ] **Step 4: final** — full `./compile_tests.sh silex`, tree clean check (`git status` — only Fran's staging files), commit records.
