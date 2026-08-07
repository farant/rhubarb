# Natura → Canon Generator (`natura_canones`) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Project natura's 33 `.genera` models into generated canons — one per module plus a monolith — so that describing a kind once yields its tag library, its schema, and (via `canon_coquere`) a typed C reader.

**Architecture:** A new tool `bin/natura_canones` loads the whole natura corpus through the existing loader (`natura_legere` × N, then `natura_nectere`), asks `natura_apparatus()` for each kind's inheritance-flattened apparatus, folds that into an element model, and emits canon XML. A driver script `tools/natura_canones.sh` regenerates or, with `-probare`, compares regenerated output against what is committed and reports `RANCIDUS`. Generation is pure projection: output is never hand-edited and self-signs so `generata-custos` denies edits.

**Tech Stack:** C89 (Latin identifiers, `latina.h`), existing libraries `natura`, `canon`, `stml`, `chorda`, `piscina`, `xar`, `filum`, `iter_directoria`; `credo` for tests; bash for drivers and hooks.

**Spec:** `project-specs/natura-canones-generati-spec.md` — read it before starting. Section references below (§3.4, §4.4 etc.) point into it.

## Global Constraints

Every task's requirements implicitly include this section.

- **C89 only.** Compile flags, exactly: `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings`
- **Everything in Latin** — identifiers, comments, diagnostic messages. Worklog prose may be English.
- **`latina.h` macros are FORBIDDEN as identifiers.** Never name anything `nomen`, `ordinarius`, `per`, `registrum`, `magnitudo`, `casus`, `structura`, `externus`, `index`, `constans`, `status`. Use `titulus`, `praestitutum`, `discrimen`, etc. The `censor-custos` hook pushes on `.c`/`.h` edits; heed it.
- **`chorda` is NOT null-terminated.** Never pass `chorda.datum` to a `str*` function. Print with `%.*s` and `(integer)c->mensura, (constans character*)c->datum`.
- **`i32` and `i64` are UNSIGNED**; `s32`/`s64` are the signed forms. Any sentinel or countdown that can go negative must be `s32`.
- **Exit codes:** `0` = clean/fresh, `1` = findings (vitia / rancidity), `2` = NOTHING RAN or tool defect. Never let a tool exit `0` because it did nothing.
- **Never gate on a piped `$?`** — that is `tail`'s exit code. Write output to a file, check the real status, then read the file.
- **Adding a `probationes/probatio_*.c` requires** running `./tools/compile_tests_fontes_generare.sh` to regenerate the sources list.
- **Generated files are committed.** Each must carry, within its **first three lines**, both the word `GENERATUM` and the exact phrase `NOLI MANU MUTARE` — `generata-custos.sh` greps `head -3` for both, and a synonym leaves the file silently unprotected.
- **Never commit** `FAQ.md`, `gesta/annales/forum.jsonl`, `gesta/annales/tabula.md`, `gesta/annales/tabularium.jsonl` — those are Fran's to seal. Stage files explicitly by path; never `git add -A`.
- **Commit messages** end with `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`.

---

## File Structure

| File | Responsibility |
|---|---|
| `tools/natura_canones.c` (create) | The generator: corpus load, name transform, element model, canon emission, CLI |
| `tools/natura_canones.sh` (create) | Driver: regenerate all, or `-probare` staleness gate |
| `tools/natura_struere.sh` (modify) | Build `bin/natura_canones` alongside `bin/natura_examen` |
| `natura/cocta/` (create, generated) | `<modulus>.canon` × 33 plus `individua.canon` |
| `canones.registrum` (modify) | Extension keys for instance files — **never** a root key |
| `.claude/hooks/natura-custos.sh` (modify) | Report stale generated canons on `.genera` edit |
| `cocta.registrum` (modify) | Add the pilot's reader so `canon_coquere` covers it |
| `probationes/probatio_natura_canones.c` (create) | Assertions over the generated pilot canon |
| `probationes/probatio_planta_lectio.c` (create) | Chain proof: read a real document with the generated C reader |
| `natura/natura.worklog.md` (append) | Non-obvious decisions and the two findings in spec §8 |

The generator is one file because its stages are sequential and share one model; if it passes ~900 lines, split emission (`_canonem_emittere` and below) into `tools/natura_canones_emissio.c`.

---

### Task 1: Generator skeleton — corpus load, name transform, `-index`

Establishes that the tool can see the whole corpus and name every kind, with nothing emitted yet.

**Files:**
- Create: `tools/natura_canones.c`
- Modify: `tools/natura_struere.sh:47-50` (add a second link step)

**Interfaces:**
- Consumes: `natura.h` — `natura_bibliotheca_creare`, `natura_legere`, `natura_nectere`, `NaturaBibliotheca.genera_omnia` (Xar of `NaturaGenus*`), `.res_omnes` (Xar of `NaturaRes*`)
- Produces: `NcEns` (the unified kind record) and `_kebab_scribere`, both used by every later task:

```c
nomen structura {
         chorda*  titulus;   /* nomen naturae, snake_case */
         chorda*  modulus;
    NaturaGenus*  genus;     /* genus ipsum, aut genus rei continentis */
      StmlNodus*  nodus;     /* nodus entis (genus aut res) */
             b32  est_res;   /* VERUM = species/individuum/cultivar */
} NcEns;
```

- [ ] **Step 1: Write the generator skeleton**

Create `tools/natura_canones.c`:

```c
/* tools/natura_canones.c - natura -> canon (proiectio pura)
 *
 * Genus quodque in elementum canonis proicitur. Vide
 * project-specs/natura-canones-generati-spec.md.
 *
 * PROIECTIO PURA: nihil hic manu emendatur; si generator
 * distinctionem poscit quam natura non fert, natura eam accipit
 * SOLUM si vere ontologica est (spec par. 3.3).
 */
#include "latina.h"
#include "natura.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO ".genera"

nomen structura {
         chorda*  titulus;
         chorda*  modulus;
    NaturaGenus*  genus;
      StmlNodus*  nodus;
             b32  est_res;
} NcEns;

interior b32       _extensionem_habet(constans chorda* t);
interior vacuum    _stirpem_scribere(constans chorda* t, character* ex,
                                     i32 mensura);
interior vacuum    _kebab_scribere(FILE* f, constans chorda* t);
interior b32       _corpus_onerare(NaturaBibliotheca* bib,
                                   constans character* radix,
                                   Piscina* piscina);
interior Xar*      _entia_colligere(NaturaBibliotheca* bib,
                                    Piscina* piscina);

/* nomen naturae (snake) -> nomen canonis (kebab).
 * Bijectivum: genus 'nomen' naturae lineolam non fert. */
interior vacuum
_kebab_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '_')
        {
            c = '-';
        }
        putc(c, f);
    }
}

interior b32
_extensionem_habet(
    constans chorda*  t)
{
    i32 mensura_ext;

    mensura_ext = (i32)strlen(EXTENSIO);
    si (t->mensura < mensura_ext)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(t->datum + (t->mensura - mensura_ext),
                       EXTENSIO, (size_t)mensura_ext) == ZEPHYRUM);
}

/* 'mensura.genera' -> 'mensura' */
interior vacuum
_stirpem_scribere(
    constans chorda*  t,
    character*        ex,
    i32               mensura)
{
    i32 n;

    n = t->mensura - (i32)strlen(EXTENSIO);
    si (n >= mensura)
    {
        n = mensura - I;
    }
    memcpy(ex, t->datum, (size_t)n);
    ex[n] = '\0';
}

/* exemplaria OMNIA onerare - sub= fines modulorum transit,
 * ergo unum onerare non sufficit (mos natura_examen). */
interior b32
_corpus_onerare(
    NaturaBibliotheca*   bib,
    constans character*  radix,
    Piscina*             piscina)
{
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    i32                   onerata;

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_canones: '%s' aperiri nequit\n", radix);
        redde FALSUM;
    }

    onerata = ZEPHYRUM;
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character via[DXII];
        character stirps[CCLVI];
        chorda    fons;

        si (introitus->genus != INTROITUS_FILUM ||
            !_extensionem_habet(&introitus->titulus))
        {
            perge;
        }

        sprintf(via, "%s/%.*s", radix,
                (integer)introitus->titulus.mensura,
                (constans character*)introitus->titulus.datum);
        _stirpem_scribere(&introitus->titulus, stirps,
                          (i32)magnitudo(stirps));

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "natura_canones: '%s' legi nequit\n", via);
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: NULLUM exemplar in '%s' (extensio '%s')\n",
            radix, EXTENSIO);
        redde FALSUM;
    }
    natura_nectere(bib);
    redde VERUM;
}

/* genera et res dictionarii in indicem unum - discrimen relativum
 * est (spec par. 3.2), ergo ambo elementa fiunt. */
interior Xar*
_entia_colligere(
    NaturaBibliotheca*  bib,
    Piscina*            piscina)
{
    Xar* entia;
    i32  i;

    entia = xar_creare(piscina, (i32)magnitudo(NcEns));

    per (i = ZEPHYRUM; i < (i32)xar_numerus(bib->genera_omnia); i++)
    {
        NaturaGenus* g;
        NcEns*       e;

        g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus  = g->titulus;
        e->modulus  = g->modulus;
        e->genus    = g;
        e->nodus    = g->nodus;
        e->est_res  = FALSUM;
    }

    per (i = ZEPHYRUM; i < (i32)xar_numerus(bib->res_omnes); i++)
    {
        NaturaRes* r;
        NcEns*     e;

        r = *(NaturaRes**)xar_obtinere(bib->res_omnes, i);
        e = (NcEns*)xar_addere(entia);
        e->titulus  = r->titulus;
        e->modulus  = r->modulus;
        e->genus    = r->genus_suum;
        e->nodus    = r->nodus;
        e->est_res  = VERUM;
    }

    redde entia;
}

integer
principale(
    integer  numerus,
    character** argumenta)
{
    Piscina*            piscina;
    NaturaBibliotheca*  bib;
    Xar*                entia;
    constans character* radix;
    b32                 modus_index;
    integer             i;

    radix       = "natura";
    modus_index = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-index") == ZEPHYRUM)
        {
            modus_index = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_canones [-index] [-radix DIR]\n");
            redde II;
        }
    }

    piscina = piscina_generare_dynamicum("natura_canones", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib || !_corpus_onerare(bib, radix, piscina))
    {
        redde II;
    }

    entia = _entia_colligere(bib, piscina);
    si (xar_numerus(entia) == ZEPHYRUM)
    {
        fprintf(stderr, "natura_canones: NULLUM ens inventum\n");
        redde II;
    }

    si (modus_index)
    {
        per (i = ZEPHYRUM; i < (integer)xar_numerus(entia); i++)
        {
            NcEns* e;

            e = (NcEns*)xar_obtinere(entia, (i32)i);
            printf("%.*s\t%.*s\t",
                   (integer)e->modulus->mensura,
                   (constans character*)e->modulus->datum,
                   (integer)e->titulus->mensura,
                   (constans character*)e->titulus->datum);
            _kebab_scribere(stdout, e->titulus);
            printf("\t%s\n", e->est_res ? "res" : "genus");
        }
        redde ZEPHYRUM;
    }

    fprintf(stderr, "natura_canones: nihil petitum (adde -index)\n");
    redde II;
}
```

- [ ] **Step 2: Add the build step**

In `tools/natura_struere.sh`, after the existing `bin/natura_examen` link block (currently ending at line 50), append:

```bash
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SCRIPT_DIR/natura_canones.c" \
    $obj_files -o "$RADIX_DIR/bin/natura_canones" || exit 1
echo "bin/natura_canones paratum" >&2
```

- [ ] **Step 3: Build and verify it fails cleanly with no mode**

```bash
./tools/natura_struere.sh && ./bin/natura_canones; echo "exitus: $?"
```

Expected: builds with zero warnings; prints `natura_canones: nihil petitum (adde -index)`; **exit 2** (nothing ran, not silent success).

- [ ] **Step 4: Verify the corpus is fully seen and names transform**

```bash
./bin/natura_canones -index > /tmp/nc-index.txt; echo "exitus: $?"
wc -l /tmp/nc-index.txt
awk -F'\t' '$4=="genus"' /tmp/nc-index.txt | wc -l
grep -P '^planta\trosa_canina\trosa-canina\tres$' /tmp/nc-index.txt
```

Expected: exit 0; **561** total lines; **177** genus rows (matching `INDEX.md`'s counts of 177 genera and 384 dictionary entries); the `rosa_canina` row present with its kebab form. If the genus count is not 177, the corpus did not fully load — do not proceed.

- [ ] **Step 5: Commit**

```bash
git add tools/natura_canones.c tools/natura_struere.sh
git commit -m "$(cat <<'EOF'
natura: natura_canones skeleton - corpus onerat, entia indicat

Fundamentum proiectionis: exemplaria omnia onerantur (sub= fines
modulorum transit), genera et res in indicem unum colliguntur,
nomina in kebab vertuntur. Nihil adhuc emittitur.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 2: Element model — apparatus folded into attributes and children

Turns each kind's inherited apparatus into the attribute/child model the emitter will print, and makes it inspectable before any file is written.

**Files:**
- Modify: `tools/natura_canones.c`

**Interfaces:**
- Consumes: `NcEns` and `_kebab_scribere` from Task 1; `natura.h` — `natura_apparatus(bib, genus, piscina)` returning `Xar` of `NaturaApparatusMembrum { StmlNodus* nodus; NaturaGenus* auctor; }`
- Produces: the model types every later task emits from:

```c
nomen enumeratio {
    NC_MEMBRUM_ATTRIBUTUM = I,   /* -> <attributum> */
    NC_MEMBRUM_LIBERUM    = II   /* -> <liberum> + <elementum intra=> */
} NcMembrumDiscrimen;

nomen structura {
     NcMembrumDiscrimen  discrimen;
                chorda*  titulus;       /* nomen naturae, snake */
    constans character*  praefixum;     /* "status_" vel NIHIL */
    constans character*  genus_valoris; /* "textus"/"nomen"/... */
                   Xar*  optiones;      /* Xar de chorda* - electio */
                chorda*  praestitutum;  /* ordinarius=, vel NIHIL */
} NcMembrum;

nomen structura {
      NcEns*  ens;
        Xar*  membra;     /* Xar de NcMembrum */
        Xar*  actiones;   /* Xar de chorda* - pro <eventum actio=> */
} NcElementum;
```

**Mapping rules** (spec §4). Dispatch on `membrum->nodus->titulus`:

| node | becomes |
|---|---|
| `proprietas` | `NC_MEMBRUM_ATTRIBUTUM`, `genus_valoris` from its `genus=`, `optiones` from `<optio>` children when `genus="electio"`; if `multiplex="verum"` then `NC_MEMBRUM_LIBERUM` instead |
| `pars` | `NC_MEMBRUM_LIBERUM` (spec §3.4: `necessaria=` is **ignored** — necessity is ontological, not documentary) |
| `machina_statuum` | `NC_MEMBRUM_ATTRIBUTUM` named `status_<machina>`, `genus_valoris = "electio"`, `optiones` = its `<status>` names — **only if `gerens=` is absent or `"individuum"`** (spec §4.1) |
| `actio` | appended to `actiones`, not a member |
| `relatio` | `NC_MEMBRUM_ATTRIBUTUM` with `genus_valoris = "nomen"` (spec §3.5); if `multiplex="verum"` then `NC_MEMBRUM_LIBERUM` |

Two rules that are easy to get wrong:

1. **Deduplicate by name, first occurrence wins.** `natura_apparatus` returns the genus's own members first, then ancestors' — and explicitly does *not* deduplicate, because an inherited narrowing wants both entries present. For schema purposes the most-derived declaration must win, so keep the first and drop later same-named ones.
2. **A `res` uses its containing genus's apparatus** (`e->genus`, set in Task 1), plus its own `<valor>` children as `praestitutum` — that is how `rosa` fixing `habitus` reaches `<rosa-canina>`.

- [ ] **Step 1: Add the model types and the fold**

Add to `tools/natura_canones.c`, above `principale`:

```c
/* nomen membri iam praesens? (apparatus NON deduplicat: primum
 * occurrens = maxime proprium, ergo primum vincit) */
interior b32
_membrum_adest(
    Xar*              membra,
    constans chorda*  titulus,
    constans character* praefixum)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)xar_numerus(membra); i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(membra, i);
        si (chorda_aequalis(*m->titulus, *titulus) &&
            ((m->praefixum == NIHIL) == (praefixum == NIHIL)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* <optio> liberos in Xar de chorda* colligere */
interior Xar*
_optiones_colligere(
    StmlNodus*  n,
    Piscina*    piscina)
{
    Xar* optiones;
    i32  i;
    i32  numerus;

    optiones = xar_creare(piscina, (i32)magnitudo(chorda*));
    numerus  = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(n, i);
        si (l && l->genus == STML_NODUS_ELEMENTUM &&
            chorda_aequalis_literis(*l->titulus, "optio"))
        {
            chorda** locus;

            locus  = (chorda**)xar_addere(optiones);
            *locus = stml_textus_normalizatus(l, piscina);
        }
    }
    redde optiones;
}

/* an machina statuum ad INDIVIDUUM pertineat (spec par. 4.1).
 * gerens= absens: natura praestitutum non dicit - vide spec
 * par. 8.1. Interim 'individuum' sumimus, quod casus usitatus est. */
interior b32
_machina_ad_individuum(
    StmlNodus*  n)
{
    chorda* gerens;

    gerens = stml_attributum_capere(n, "gerens");
    si (!gerens)
    {
        redde VERUM;
    }
    redde chorda_aequalis_literis(*gerens, "individuum");
}
```

Then the fold itself:

```c
interior NcElementum*
_elementum_aedificare(
    NaturaBibliotheca*  bib,
    NcEns*              ens,
    Piscina*            piscina)
{
    NcElementum* el;
    Xar*         apparatus;
    i32          i;

    el           = (NcElementum*)piscina_allocare(piscina,
                       (i32)magnitudo(NcElementum));
    el->ens      = ens;
    el->membra   = xar_creare(piscina, (i32)magnitudo(NcMembrum));
    el->actiones = xar_creare(piscina, (i32)magnitudo(chorda*));

    si (!ens->genus)
    {
        redde el;
    }
    apparatus = natura_apparatus(bib, ens->genus, piscina);

    per (i = ZEPHYRUM; i < (i32)xar_numerus(apparatus); i++)
    {
        NaturaApparatusMembrum* am;
        chorda*                 titulus;
        chorda*                 multiplex;
        NcMembrum*              m;

        am      = (NaturaApparatusMembrum*)xar_obtinere(apparatus, i);
        titulus = stml_attributum_capere(am->nodus, "nomen");

        si (chorda_aequalis_literis(*am->nodus->titulus, "actio"))
        {
            chorda** locus;

            si (!titulus) { perge; }
            locus  = (chorda**)xar_addere(el->actiones);
            *locus = titulus;
            perge;
        }

        si (chorda_aequalis_literis(*am->nodus->titulus,
                                    "machina_statuum"))
        {
            si (!titulus || !_machina_ad_individuum(am->nodus))
            {
                perge;
            }
            si (_membrum_adest(el->membra, titulus, "status_"))
            {
                perge;
            }
            m                = (NcMembrum*)xar_addere(el->membra);
            m->discrimen     = NC_MEMBRUM_ATTRIBUTUM;
            m->titulus       = titulus;
            m->praefixum     = "status_";
            m->genus_valoris = "electio";
            m->optiones      = _status_colligere(am->nodus, piscina);
            m->praestitutum  = NIHIL;
            perge;
        }

        si (!titulus || _membrum_adest(el->membra, titulus, NIHIL))
        {
            perge;
        }
        multiplex = stml_attributum_capere(am->nodus, "multiplex");

        m               = (NcMembrum*)xar_addere(el->membra);
        m->titulus      = titulus;
        m->praefixum    = NIHIL;
        m->optiones     = NIHIL;
        m->praestitutum = NIHIL;
        m->discrimen    = (b32)(multiplex &&
                           chorda_aequalis_literis(*multiplex, "verum"))
                          ? NC_MEMBRUM_LIBERUM : NC_MEMBRUM_ATTRIBUTUM;

        si (chorda_aequalis_literis(*am->nodus->titulus, "pars"))
        {
            /* necessaria= CONSULTO ignoratur: necessitas ontologica
             * obligatio documenti non est (spec par. 3.4) */
            m->discrimen     = NC_MEMBRUM_LIBERUM;
            m->genus_valoris = "textus";
        }
        alioquin si (chorda_aequalis_literis(*am->nodus->titulus,
                                             "relatio"))
        {
            m->genus_valoris = "nomen";
        }
        alioquin
        {
            chorda* g;

            g = stml_attributum_capere(am->nodus, "genus");
            m->genus_valoris = "textus";
            si (g && chorda_aequalis_literis(*g, "electio"))
            {
                m->genus_valoris = "electio";
                m->optiones      = _optiones_colligere(am->nodus,
                                                       piscina);
            }
            alioquin si (g && chorda_aequalis_literis(*g, "numerus"))
            {
                m->genus_valoris = "numerus";
            }
            alioquin si (g && chorda_aequalis_literis(*g, "veritas"))
            {
                m->genus_valoris = "veritas";
            }
            alioquin si (g && chorda_aequalis_literis(*g, "dies"))
            {
                m->genus_valoris = "dies";
            }
        }
    }

    _valores_applicare(el, ens, piscina);
    redde el;
}
```

Write `_status_colligere` as `_optiones_colligere` but matching `"status"` children and reading their `nomen=` attribute instead of their text. Write `_valores_applicare` to walk `ens->nodus`'s `<valor>` children, match each `nomen=` against `el->membra`, and set `praestitutum` from the valor's normalized text (spec §8.2 — both readings of `valor` map safely to a default).

- [ ] **Step 2: Add an `-inspicere` mode**

Add a `-inspicere GENUS` flag that builds one element and prints its model, one member per line:

```
rosa-canina	attributum	habitus	electio	herba|frutex|arbor|scandens	=frutex
rosa-canina	attributum	status-vita	electio	semen|germinans|...
rosa-canina	liberum	radix
rosa-canina	actio	germinare
```

- [ ] **Step 3: Rebuild and inspect the pilot**

```bash
./tools/natura_struere.sh && ./bin/natura_canones -inspicere planta
```

Expected: `habitus` with exactly 4 options; `durata` with 3; `folia-permanentia` with 2; `altitudo-matura` as `numerus`; `status-vita` as an electio of the 7 states; `radix`, `caulis`, `folium`, `flos`, `fructus`, `semen` all as `liberum`; the three relations (`auctor-nominis`, `nomen-publicatum-in`, `pollinatur-a`) as `nomen` attributes; five actions.

- [ ] **Step 4: Verify inheritance flattening and fixed values**

```bash
./bin/natura_canones -inspicere rosa_canina
```

Expected: everything `planta` has, **plus** inherited members from `vivens` and `res_corporea` (flattening crosses the module boundary into `corporalia`), **plus** `habitus` carrying `=frutex` and `durata` carrying `=perennis` from `rosa`'s `<valor>` elements, and `altitudo-matura` carrying `=3` from its own. No member appears twice.

- [ ] **Step 5: Commit**

```bash
git add tools/natura_canones.c
git commit -m "$(cat <<'EOF'
natura: apparatus in exemplar elementi plicatur

natura_apparatus hereditatem iam solvit; hic dispositio: proprietas
-> attributum (multiplex -> liberum), pars -> liberum (necessaria=
CONSULTO neglecta: necessitas ontologica obligatio documenti non
est), machina gerens=individuum -> status-<nomen> electio, relatio
-> nomen, valor -> ordinarius. Primum occurrens vincit, quia
apparatus non deduplicat.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 3: Emit per-module canons, correctly signed

**Files:**
- Modify: `tools/natura_canones.c`
- Create (generated): `natura/cocta/planta.canon`

**Interfaces:**
- Consumes: `NcElementum` from Task 2
- Produces: `_canonem_emittere(FILE* f, Xar* elementa, constans character* dialectus, constans character* fons)` — used unchanged by Task 4's monolith

- [ ] **Step 1: Write the emitter**

The header must place both markers inside the first three lines (`generata-custos.sh` greps `head -3` for `GENERATUM` **and** for `NOLI MANU MUTARE`; a synonym leaves the file unprotected):

```c
fprintf(f, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
fprintf(f, "<!-- GENERATUM a natura_canones e %s - NOLI MANU MUTARE.\n",
        fons);
fprintf(f, "     Regenera: ./tools/natura_canones.sh -->\n");
```

Body shape, per element (kebab names throughout — spec §4.3):

```xml
<elementum nomen="individua" radix="verum">
  <liberum nomen="rosa-canina"/>
  ...one <liberum> per kind...
</elementum>

<elementum nomen="rosa-canina">
  <attributum nomen="nomen" genus="nomen"/>
  <attributum nomen="habitus" genus="electio" ordinarius="frutex">
    <optio>herba</optio>
    ...
  </attributum>
  <attributum nomen="auctor-nominis" genus="nomen"/>
  <liberum nomen="radix" maximum="1"/>
  <liberum nomen="historia" maximum="1"/>
</elementum>

<elementum nomen="radix" intra="rosa-canina">
  <attributum nomen="nota" genus="textus"/>
</elementum>
```

Rules to hold to:

- **No `minimum=` anywhere.** Everything optional (spec §3.4).
- **Every kind element gets `nomen` as `genus="nomen"`.**
- **Parts and multiplex members are declared `intra="<kind>"`** — this is what keeps `directorium`, `eventum` and `proiectio` from colliding with the genera of the same names (spec §4.2).
- **`historia`/`eventum` are emitted verbatim** from natura's own vocabulary, with `eventum/@actio` as an `electio` over the element's collected `actiones`.
- **`<individua>` is the only element carrying `radix="verum"`.**

Add a `-modulus NOMEN -ad VIA` mode that writes one module's canon.

- [ ] **Step 2: Generate the pilot and check the signum lands in head -3**

```bash
mkdir -p natura/cocta
./tools/natura_struere.sh
./bin/natura_canones -modulus planta -ad natura/cocta/planta.canon
head -3 natura/cocta/planta.canon | grep -c "GENERATUM"
head -3 natura/cocta/planta.canon | grep -cE "NE MANU EDITES|NOLI MANU MUTARE"
```

Expected: both greps print `1`. If either prints `0` the file is silently unprotected — fix the header before continuing.

- [ ] **Step 3: Judge the generated canon against `canon.canon`**

```bash
./bin/canon_examen natura/cocta/planta.canon > /tmp/nc-judge.txt 2>&1
echo "exitus: $?"
cat /tmp/nc-judge.txt
```

Expected: **exit 0**, no vitia. The `.canon` extension key in `canones.registrum` routes it to `canon.canon`, so this is the generated canon being judged by the canon-of-canons. Exit 2 means nothing was judged — investigate rather than accept.

- [ ] **Step 4: Verify the protection is live (planted attempt)**

Attempt an `Edit` to `natura/cocta/planta.canon` (change any comment). Expected: **denied** by `generata-custos` with the "Plagula GENERATA" reason. A gate that has never refused anything is not known to work.

- [ ] **Step 5: Verify a real instance document validates**

```bash
cat > /tmp/hortus.stml <<'EOF'
<individua>
  <rosa-canina nomen="rosa ad murum" habitus="frutex"
               status-vita="florens" auctor-nominis="carl_linnaeus">
    <radix nota="muro proxima"/>
    <historia><eventum quando="2024-04" actio="nominari"/></historia>
  </rosa-canina>
</individua>
EOF
./bin/canon_examen /tmp/hortus.stml; echo "exitus: $?"
```

Expected: the dialect is not yet in the registry, so a one-line "dialectus nondum descriptus" notice and exit 0. Task 6 wires dispatch; this step only confirms the document parses.

- [ ] **Step 6: Commit**

```bash
git add tools/natura_canones.c natura/cocta/planta.canon
git commit -m "$(cat <<'EOF'
natura: canon per modulum emittitur (planta primus)

Nomina kebab, nihil poscitur (minimum= nusquam), partes et membra
multiplicia intra= adstricta ne cum generibus homonymis pugnent.
Signum GENERATUM + NOLI MANU MUTARE intra lineas tres, quia
generata-custos head -3 solum legit.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 4: The monolith, with relations as real `citatio`

**Files:**
- Modify: `tools/natura_canones.c`
- Create (generated): `natura/cocta/individua.canon`

**Interfaces:**
- Consumes: `_canonem_emittere` from Task 3
- Produces: `natura/cocta/individua.canon` covering all ~560 kinds

- [ ] **Step 1: Add `-totum`**

`-totum -ad VIA` emits every kind from every module into one canon. Same emitter, larger element set.

Additionally, in monolith mode only, emit a `<citatio>` per relation so references are checked within the document (spec §3.5 — this is the one place the schema can resolve them):

```xml
<citatio nomen="auctor-nominis" attributum="auctor-nominis"
         ad="persona/nomen" super="rosa-canina"/>
```

Use `super=` to scope the citation to the elements that declare that relation, so an unrelated element carrying the same attribute name is not falsely captured — the same discipline `natura.canon`'s own `transitus-a` citation uses.

- [ ] **Step 2: Generate and judge**

```bash
./tools/natura_struere.sh
./bin/natura_canones -totum -ad natura/cocta/individua.canon
grep -c '<elementum' natura/cocta/individua.canon
./bin/canon_examen natura/cocta/individua.canon > /tmp/nc-mono.txt 2>&1
echo "exitus: $?"; cat /tmp/nc-mono.txt
```

Expected: on the order of 560+ elements (561 kinds plus scoped part definitions); exit 0 with no vitia.

- [ ] **Step 3: Verify mixing works and a dangling reference is caught**

Write two documents — one whose relation resolves within the file, one whose does not:

```bash
cat > /tmp/mixtum-sanum.individua <<'EOF'
<individua>
  <rosa-canina nomen="rosa ad murum" habitus="frutex"
               auctor-nominis="linnaeus-noster"/>
  <persona nomen="linnaeus-noster"/>
</individua>
EOF

cat > /tmp/mixtum-fractum.individua <<'EOF'
<individua>
  <rosa-canina nomen="rosa ad murum" habitus="frutex"
               auctor-nominis="nemo-omnino"/>
  <persona nomen="linnaeus-noster"/>
</individua>
EOF
```

Judge both. Task 6 adds `canon_examen -canon VIA`; until then add a temporary `.individua` line to `canones.registrum`, run the two checks, and remove it again (Task 6 adds it permanently):

```bash
./bin/canon_examen /tmp/mixtum-sanum.individua;   echo "sanum:   $?"
./bin/canon_examen /tmp/mixtum-fractum.individua; echo "fractum: $?"
```

Expected: `sanum: 0` with no vitia; `fractum: 1` with a `CITATIO_IRRITA` vitium naming `nemo-omnino`. If the broken one also exits 0, the citation was not emitted or its `super=` scoping is wrong — fix before committing, since a citation that never fires is worse than none.

- [ ] **Step 4: Commit**

```bash
git add tools/natura_canones.c natura/cocta/individua.canon
git commit -m "$(cat <<'EOF'
natura: canon monolithus - mixtio licet, relationes vere examinatae

Modi duo, dispositio una. In monolitho relatio citatio fit
(super= adstricta ne homonyma falso capiantur); in canone per
modulum nomen solum est, quod onerator resolvit.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 5: Driver script and the `-probare` staleness gate

**Files:**
- Create: `tools/natura_canones.sh`
- Create (generated): the remaining 32 `natura/cocta/*.canon`

**Interfaces:**
- Consumes: `bin/natura_canones` modes `-modulus`, `-totum`
- Produces: `./tools/natura_canones.sh` (regenerate all) and `./tools/natura_canones.sh -probare` (exit 0 fresh / 1 rancid / 2 tool defect) — Task 6's hook calls the latter

Note there is **no manifest**: the corpus is the manifest. Every `natura/*.genera` yields `natura/cocta/<stem>.canon`, plus the monolith.

- [ ] **Step 1: Write the driver**

Mirror `tools/canon_coquere.sh` closely — same staleness discipline, same exit codes:

```bash
#!/bin/bash

# tools/natura_canones.sh [-probare]
#
# Canones ex natura/*.genera generare aut rancorem probare.
#   sine argumento   omnes regenerare (in locis veris)
#   -probare         conferre solum: 0 recentes, 1 RANCIDI,
#                    2 defectus instrumenti (numquam sanitas tacita)

set -u
cd "$(dirname "$0")/.." || exit 2

PORTA=bin/natura_canones
COCTA=natura/cocta

if [ ! -x "$PORTA" ]; then
    echo "natura_canones: $PORTA abest - strue: ./tools/natura_struere.sh" >&2
    exit 2
fi
for _f in lib/natura.c include/natura.h tools/natura_canones.c lib/stml.c; do
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_canones: $PORTA STALUS - strue: ./tools/natura_struere.sh" >&2
        exit 2
    fi
done

mkdir -p "$COCTA"
TMPD=build/natura_canones_tmp
mkdir -p "$TMPD"

rancidi=0
facti=0

conferre_aut_scribere () {
    # $1 = via vera, $2... = argumenta portae
    local vera="$1"; shift
    if [ "${PROBARE:-0}" = "1" ]; then
        "$PORTA" "$@" -ad "$TMPD/probandum.canon" \
            || { echo "natura_canones: '$vera' generari nequit" >&2; exit 2; }
        # copia temporalis in UNO loco divergit: iussum
        # regenerationis nullam viam temporalem fert, ergo
        # substitutio nulla opus est - sed viam fontis servamus.
        if ! cmp -s "$TMPD/probandum.canon" "$vera"; then
            echo "natura_canones: RANCIDUS $vera - regenera: ./tools/natura_canones.sh" >&2
            rancidi=$((rancidi + 1))
        fi
    else
        "$PORTA" "$@" -ad "$vera" || exit 2
    fi
    facti=$((facti + 1))
}

[ "${1:-}" = "-probare" ] && PROBARE=1 || PROBARE=0

for fons in natura/*.genera; do
    [ -e "$fons" ] || continue
    stirps=$(basename "$fons" .genera)
    conferre_aut_scribere "$COCTA/$stirps.canon" -modulus "$stirps"
done
conferre_aut_scribere "$COCTA/individua.canon" -totum

if [ "$facti" -eq 0 ]; then
    echo "natura_canones: NULLUM exemplar - nihil factum" >&2
    exit 2
fi

if [ "$PROBARE" = "1" ]; then
    [ "$rancidi" -gt 0 ] && exit 1
    echo "natura_canones: canones $facti recentes"
fi
exit 0
```

Make it executable: `chmod +x tools/natura_canones.sh`

**Important:** the emitted header must not embed a temp path, or `-probare` will report false rancidity the way `canon_coquere.sh` had to patch around with three `sed` substitutions. Keep the header's `Regenera:` line a fixed literal and name the **source** `.genera` file, never the output path.

- [ ] **Step 2: Generate everything and confirm the gate reports fresh**

```bash
./tools/natura_canones.sh; echo "exitus: $?"
ls natura/cocta/*.canon | wc -l
./tools/natura_canones.sh -probare; echo "exitus: $?"
```

Expected: 34 files (33 modules + monolith); `-probare` prints `canones 34 recentes` and exits 0.

- [ ] **Step 3: Plant a SEMANTIC fault and confirm the gate catches it**

A comment-only edit must **not** trip a semantic gate — mistaking that for a working gate has burned this codebase before. Plant a structural change:

```bash
cp natura/planta.genera /tmp/planta.genera.bak
# add a new property inside planta's <proprietates>:
#   <proprietas nomen="probatio_portae" genus="veritas"/>
```

Then:

```bash
./tools/natura_canones.sh -probare; echo "exitus: $?"
```

Expected: `RANCIDUS natura/cocta/planta.canon` (and `individua.canon`), **exit 1**.

Then confirm the *cosmetic* case correctly does not trip it: restore, add only an XML comment to `planta.genera`, re-run `-probare`, expect exit 0. Restore the file fully afterwards:

```bash
cp /tmp/planta.genera.bak natura/planta.genera
./tools/natura_canones.sh -probare; echo "exitus: $?"   # 0
```

- [ ] **Step 4: Judge the whole generated corpus**

```bash
for c in natura/cocta/*.canon; do
    ./bin/canon_examen "$c" > /tmp/nc-one.txt 2>&1
    st=$?
    [ "$st" -ne 0 ] && { echo "VITIA in $c (exitus $st)"; cat /tmp/nc-one.txt; }
done
echo "confectum"
```

Expected: no output before `confectum`. Every generated canon is itself judged by `canon.canon`.

- [ ] **Step 5: Commit**

```bash
git add tools/natura_canones.sh natura/cocta/
git commit -m "$(cat <<'EOF'
natura: porta rancoris + canones omnes generati (33 + monolithus)

Manifestum nullum: corpus ipsum manifestum est. Porta OUTPUT
confert, non tempora - mutatio commentarii sola rancorem NON
parit, et culpa plantata id probavit (proprietas nova parit).

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 6: Registry dispatch and hook wiring

**Files:**
- Modify: `canones.registrum`
- Modify: `.claude/hooks/natura-custos.sh`
- Modify: `tools/canon_examen.c` (add `-canon VIA`)

**Interfaces:**
- Consumes: `tools/natura_canones.sh -probare` from Task 5
- Produces: `bin/canon_examen -canon VIA DOC` for judging against a canon not in the registry

- [ ] **Step 1: Add the extension keys — and no root key**

Append to `canones.registrum`:

```
.individua	natura/cocta/individua.canon
.planta	natura/cocta/planta.canon
```

**Do not add a `<individua>` root key.** Both canons root at `<individua>`, and the registry's own header states `RADIX VINCIT` — a root key would silently bind every instance file to whichever canon claimed it. Add a comment above the two lines recording exactly that, so nobody "fixes" it later.

- [ ] **Step 2: Verify dispatch picks the right canon**

```bash
cp /tmp/hortus.stml /tmp/hortus.planta
./bin/canon_examen /tmp/hortus.planta; echo "exitus: $?"
```

Expected: exit 0, judged against `planta.canon` (no "dialectus nondum descriptus" notice this time).

Then plant a fault — change `habitus="frutex"` to `habitus="lignum"` — and expect exit 1 naming the value as outside the electio.

- [ ] **Step 3: Add `-canon VIA` to `canon_examen`**

Mirror the existing flag parsing in `tools/canon_examen.c`. When `-canon` is given, skip registry lookup and load that canon directly. This is what lets the other 32 per-module canons be used without 32 registry lines (spec §10 open question 1 and 2).

Verify:

```bash
./bin/canon_examen -canon natura/cocta/individua.canon /tmp/hortus.stml
echo "exitus: $?"
```

Expected: exit 0 — the same document is valid under the monolith, since per-module canons are subsets of it.

- [ ] **Step 4: Wire the staleness check into the `.genera` hook**

In `.claude/hooks/natura-custos.sh`, after the existing `natura_examen` judgment, run `./tools/natura_canones.sh -probare` and, on exit 1, add one line to the pushed context: `exemplar sanum, sed canones cocti eius RANCIDI - regenera: ./tools/natura_canones.sh`.

Preserve the hook's existing contract: **one JSON object per invocation**, using the nested `hookSpecificOutput` envelope. A bare `additionalContext` is silently dropped — every house hook was mute for an unknown period because of exactly that.

- [ ] **Step 5: Test the hook with a planted fault**

Edit `natura/planta.genera` to add a property (as in Task 5), then observe the hook output on that edit. Expected: the RANCIDI line appears. Restore the file and confirm the line stops appearing.

- [ ] **Step 6: Commit**

```bash
git add canones.registrum .claude/hooks/natura-custos.sh tools/canon_examen.c
git commit -m "$(cat <<'EOF'
natura: dispositio registri + uncus rancoris

Claves EXTENSIONIS solae: radix <individua> ambobus communis est,
et RADIX VINCIT - clavis radicis canonem unum omnibus plagulis
falso imponeret. canon_examen -canon VIA additum pro canonibus
extra registrum.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 7: Chain proof — `canon_coquere` on the pilot

Proves the whole chain, not just the first hop: a kind described once yields a typed C struct that reads a real document.

**Files:**
- Modify: `cocta.registrum`
- Create (generated): `include/planta_lectio.h`, `lib/planta_lectio.c`
- Create: `probationes/probatio_planta_lectio.c`
- Create: `natura/exempla/hortus.planta` (a committed fixture document)

**Interfaces:**
- Consumes: `natura/cocta/planta.canon` from Task 3
- Produces: `PlantaIndividua* planta_individua_legere(chorda fons, Piscina*, InternamentumChorda*, chorda* causa)` and the per-kind structs beneath it

- [ ] **Step 1: Add the pilot to the reader manifest**

Append to `cocta.registrum` (tab-separated: canon, prefix, header, body):

```
natura/cocta/planta.canon	Planta	include/planta_lectio.h	lib/planta_lectio.c
```

- [ ] **Step 2: Generate the reader**

```bash
./tools/canon_coquere.sh; echo "exitus: $?"
./tools/canon_coquere.sh -probare; echo "exitus: $?"
grep -n 'PlantaRosaCanina' include/planta_lectio.h | head
```

Expected: both exit 0. The kebab name `rosa-canina` must have produced the type `PlantaRosaCanina` — `canon_coquere` already treats `-` as a segment separator, so no generator change should be needed. If the type name is malformed, stop and fix the transform rather than working around it.

- [ ] **Step 3: Write the fixture document**

Create `natura/exempla/hortus.planta`:

```xml
<individua>
  <rosa-canina nomen="rosa ad murum" habitus="frutex"
               status-vita="florens" auctor-nominis="carl_linnaeus">
    <radix nota="muro proxima"/>
    <historia>
      <eventum quando="2024-04" actio="nominari"/>
    </historia>
  </rosa-canina>
  <granny-smith nomen="malus prima" altitudo-matura="4"/>
</individua>
```

- [ ] **Step 4: Write the failing test**

Create `probationes/probatio_planta_lectio.c` following the `probatio_piscina.c` / `probatio_canon.c` shape (`credo_aperire`, assertions, `credo_imprimere_compendium`, return 0 on pass / 1 on fail):

```c
/* probatio_planta_lectio.c - catena tota: genus descriptum ->
 * canon generatus -> lector coctus -> documentum verum lectum. */
#include "latina.h"
#include "planta_lectio.h"
#include "credo.h"
#include "filum.h"
#include <stdio.h>

integer
principale(
    vacuum)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    PlantaIndividua*     ind;
    chorda               fons;
    chorda               causa;
    b32                  praeteritus;

    credo_aperire("planta_lectio");

    piscina = piscina_generare_dynamicum("probatio", 1048576);
    intern  = internamentum_creare(piscina);
    causa   = chorda_vacua();

    fons = filum_legere_totum("natura/exempla/hortus.planta", piscina);
    CREDO_VERUM (fons.mensura > ZEPHYRUM);

    ind = planta_individua_legere(fons, piscina, intern, &causa);
    CREDO_NON_NIHIL (ind);

    /* duo entia: rosa-canina, granny-smith */
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(ind->rosa_canina), I);
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(ind->granny_smith), I);

    {
        PlantaRosaCanina* r;

        r = *(PlantaRosaCanina**)xar_obtinere(ind->rosa_canina,
                                              ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_VERUM (chorda_aequalis_literis(*r->nomen_v,
                                             "rosa ad murum"));
        CREDO_AEQUALIS_I32 ((i32)r->habitus,
                            (i32)PLANTA_ROSA_CANINA_HABITUS_FRUTEX);
        CREDO_AEQUALIS_I32 ((i32)r->status_vita,
                            (i32)PLANTA_ROSA_CANINA_STATUS_VITA_FLORENS);
        CREDO_NON_NIHIL (r->radix);
        CREDO_NON_NIHIL (r->historia);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();

    si (praeteritus) { redde ZEPHYRUM; }
    redde I;
}
```

**Field and enum names must be read from the generated header, not guessed.** `nomen` is a `latina.h` macro so `canon_coquere` sanitizes it to `nomen_v`; confirm the actual spelling in `include/planta_lectio.h` and correct the test to match before running.

- [ ] **Step 5: Register and run the test**

```bash
./tools/compile_tests_fontes_generare.sh
./compile_tests.sh planta_lectio > /tmp/nc-test.txt 2>&1
echo "exitus: $?"; tail -20 /tmp/nc-test.txt
```

Expected first run: failures or a compile error naming the real field spellings. Fix the test to match the generated header, rerun, expect all assertions passing and exit 0. **Exit 2 means nothing ran** — a mistyped filter, not success.

- [ ] **Step 6: Commit**

```bash
git add cocta.registrum include/planta_lectio.h lib/planta_lectio.c \
        probationes/probatio_planta_lectio.c probationes/fontes.txt \
        natura/exempla/hortus.planta
git commit -m "$(cat <<'EOF'
natura: catena tota probata - genus -> canon -> structura C

planta.canon per canon_coquere in lectorem typatum versus; probatio
documentum verum legit. Nomina kebab structuras rectas pariunt
(PlantaRosaCanina), quia coquere lineolam iam ut sectionem tractat.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

### Task 8: Corpus assertions, records, and the two findings natura owes itself

**Files:**
- Create: `probationes/probatio_natura_canones.c`
- Modify: `natura/natura.worklog.md`
- Modify: `natura/natura.canon` (one line — see below)

**Interfaces:**
- Consumes: the committed `natura/cocta/*.canon` from Task 5

- [ ] **Step 1: Write corpus-level assertions**

Create `probationes/probatio_natura_canones.c` asserting properties of the committed pilot canon, so a regression in the generator fails a test rather than silently changing output. Read the file with `filum_legere_totum`, then load it with:

```c
Canon* canon_legere(chorda fons, Piscina* piscina,
                    InternamentumChorda* intern, chorda* causa);
```

and assert:

- the canon loads and `<individua>` is the only element with `radix="verum"`
- element `rosa-canina` exists, and `rosa_canina` does **not**
- its `habitus` attribute is an `electio` with exactly 4 options and `praestitutum` of `frutex`
- `radix` is declared with `intra="rosa-canina"`, not globally
- **no** element declares any `minimum` (spec §3.4 — canon limits, never demands)

That last assertion is the one most likely to catch a future well-meaning change.

- [ ] **Step 2: Register and run**

```bash
./tools/compile_tests_fontes_generare.sh
./compile_tests.sh natura_canones > /tmp/nc-t2.txt 2>&1
echo "exitus: $?"; tail -20 /tmp/nc-t2.txt
```

Expected: all pass, exit 0.

- [ ] **Step 3: Confirm nothing else regressed**

```bash
./compile_tests.sh > /tmp/nc-all.txt 2>&1; echo "exitus: $?"
grep -iE 'fract|FAIL' /tmp/nc-all.txt | head
./tools/natura_visus.sh > /tmp/nc-visus.txt 2>&1; echo "exitus: $?"
tail -5 /tmp/nc-visus.txt
```

Expected: the full suite passes. `natura_visus.sh` exits **1** with exactly the three known standing corpus vulnera (`pharmacon:185`, `vectura:144`, `plagula_computatralis:406`) — that is the pre-existing state, not a regression. Any fourth finding is yours.

- [ ] **Step 4: State `gerens=`'s default in natura**

Spec §8.1: `natura.canon` declares `gerens` as an optional `electio` with no `ordinarius=`, yet the whole instance projection turns on it. Add the stated default to `natura/natura.canon`:

```xml
<attributum nomen="gerens" genus="electio" ordinarius="individuum"
  nota="praestitutum DICTUM 2026-08-06: proiectio instantiarum ab
        hoc pendet (natura_canones), ergo tacere non licet">
```

Then regenerate and confirm nothing shifted:

```bash
./tools/natura_canones.sh -probare; echo "exitus: $?"
```

Expected exit 0 — the generator already assumed `individuum` for the absent case, so stating it changes no output. If output *does* shift, the assumption was wrong and the diff shows exactly where.

- [ ] **Step 5: Record the `valor` ambiguity as a dubium**

Spec §8.2: `valor` both constrains (`rosa` fixing `habitus`) and typifies (`rosa_canina` carrying `altitudo_matura`). Add a `<dubium>` to the `planta` genus in `natura/planta.genera` recording that the two readings exist, that the generator maps both to `ordinarius=` because a stated default is safe under either, and that resolving it is an editorial judgment for Fran.

Regenerate afterwards (`./tools/natura_canones.sh`) since the source changed, and commit the regenerated canons with it.

- [ ] **Step 6: Append the worklog entry**

Add a dated entry to `natura/natura.worklog.md` covering: why `necessaria=` is deliberately ignored; why the first apparatus occurrence wins (the loader does not deduplicate on purpose); why the registry carries no `<individua>` root key; and the two findings above. English is fine.

- [ ] **Step 7: File the ledger note**

Use `mcp__tabularii__addere` with `genus: "nota"` recording that the natura→canon projection shipped, which spec it implements, and the two open natura questions it surfaced. Link it to the canon project entry.

- [ ] **Step 8: Commit**

```bash
git add probationes/probatio_natura_canones.c probationes/fontes.txt \
        natura/natura.canon natura/planta.genera natura/cocta/ \
        natura/natura.worklog.md
git commit -m "$(cat <<'EOF'
natura: assertiones corporis + duo quae generator in natura invenit

gerens= praestitutum nunc DICTUM (proiectio ab eo pendet); valor
ambiguum (figit an typificat?) in dubium relatum. Probatio canonem
pilotem custodit - praesertim quod NULLUM elementum minimum= fert:
canon terminat, non poscit.

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>
EOF
)"
```

---

## Self-Review

**Spec coverage.** §1 idea → Tasks 1-4. §3.1 residence → nothing to implement (a constraint on what is generated, honoured by generating only kinds). §3.2 addressability → Task 1 Step 1 (`_entia_colligere` takes both genera and res). §3.3 pure projection → Tasks 3 and 5 (signum, no manifest, no override path). §3.4 leniency → Task 2 mapping table and Task 8 Step 1's `minimum` assertion. §3.5 relations → Task 2 (`nomen` attributes) and Task 4 (citatio in the monolith). §4 mapping table → Task 2. §4.1 `gerens` → Task 2 `_machina_ad_individuum` and Task 8 Step 4. §4.2 scoping → Task 3 Step 1. §4.3 kebab → Task 1 `_kebab_scribere`, proven downstream in Task 7 Step 2. §4.4 names vs values → Task 3's fixture and Task 7's document both carry `auctor-nominis="carl_linnaeus"`. §5.1/5.2 modes → Tasks 3 and 4. §5.3 document shape → Task 3 Step 5. §5.4 registry trap → Task 6 Step 1. §5.5 freshness → Task 5. §6 verification → Tasks 3, 5, 7, 8. §7 monitum → deferred by the spec, correctly absent here. §8 findings → Task 8 Steps 4-5. §9 `::` → deferred by the spec, correctly absent. §10 open questions 1 and 2 → answered by Task 6 Step 3's `-canon` flag; questions 3 and 4 remain open by design.

**Placeholders.** Task 4 Step 3 contains a deliberate placeholder document that the step's own text instructs the implementer to replace with two real variants; that is an instruction, not an unfilled blank. Two helpers in Task 2 Step 1 (`_status_colligere`, `_valores_applicare`) are specified in prose with their exact shape and their model in the adjacent `_optiones_colligere`, rather than transcribed — acceptable because they are near-duplicates of code given in full immediately above.

**Type consistency.** `NcEns` fields are used identically in Tasks 1 and 2. `NcMembrum.praestitutum` (Task 2) is emitted as `ordinarius=` (Task 3) and asserted as `praestitutum` (Task 8) — matching `lib/canon.c`, where the C field is `praestitutum` because `ordinarius` is a `latina.h` macro. `_canonem_emittere` has one signature, introduced in Task 3 and reused in Task 4.

---

## Execution Handoff

Plan complete and saved to `project-specs/natura-canones-generati-plan.md`. Two execution options:

1. **Subagent-Driven (recommended)** — a fresh subagent per task, reviewed between tasks, fast iteration
2. **Inline Execution** — executed in this session with checkpoints for review

Which approach?
