# silex novum -vitrea Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `silex novum <t> -vitrea` scaffolds a complete vitrea+volumen heartbeat app with build scripts generated as stamped projections whose per-target source lists are computed by the closure walk.

**Architecture:** Extends `silex_novum` (lib/silex.c:771): a `vitrea` flag switches the seed set to vitrea templates, vendors the capsula tool from the fons, runs the closure walk per target (app / tool / probatio) with seeds scanned from the templates' own includes, and emits four-tier scripts. Spec: `project-specs/silex-vitrea-spec.md`.

**Tech Stack:** C89 (Latin), existing `_fingere` template machinery (chorda_aedificator), silex closure walk, credo.

## Global Constraints

- Plain (non-vitrea) novum output BYTE-UNCHANGED — existing probatio_silex assertions are the guard.
- Generated scripts carry the stamp header `# GENERATUM a silice (novum -vitrea, <fons titulus>) - PROIECTIO: mutare licet` and NO `lib/*.c` glob anywhere.
- Vitrea scripts use the battery + `-Wno-overlength-strings -fbracket-depth=512` (new macro `SILEX_VEXILLA_VITREA`; plain `SILEX_VEXILLA_COMPILATIONIS` untouched).
- Vendor tier: `-O2 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0 -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1` (stamped copies per the spec ruling), one rule per `vendor/*.c` in the closure.
- Frameworks: `-framework Cocoa -framework WebKit` (hardcoded convention v1).
- Seeds are NEVER a hand-kept list: app seeds = `_inclusiones_scrutari` over the fontes+pipa templates; tool seeds = scan of the tool source read through the fons (`tools/capsula_generare.c` — it is in the corpus); probatio seeds = scan of probatio+pipa templates.
- Template C identifiers are FIXED (`Pipa`, `pipa_salve_tractare`, `pipa_viam_voluminis`) — only file names and strings carry `<t>`.
- Data volumen `~/.rhubarb/<t>.volumen`; loud refusal on open failure; NEVER a volumen in the project dir besides the VCS document.
- Suite `./compile_tests.sh silex`; house rules as ever (Latin, forbidden macros, i32 unsigned, path-staged commits, Co-Authored-By Claude Fable 5).

---

## Target artifacts (what the scaffold must contain — the `_fingere` functions emit EXACTLY these with `<t>` substituted; transcription into builder strings is mechanical)

### fontes/<t>_pipa.h

```c
/* <t>_pipa.h - pipa cordis: tractator salve super volumine.
 * C = pipa stulta; facies in assets/ vivit. (A silice genita.) */
#ifndef PIPA_H
#define PIPA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "volumen.h"

nomen structura {
    Volumen* volumen;
} Pipa;

/* salve {} -> {nuntius, numerus}: actum 'salutatio' appendit et
 * salutationes omnes numerat - pons ET persistentia una probatione */
JsonValor*
pipa_salve_tractare (
    JsonValor* argumenta,
    Piscina*   piscina,
    vacuum*    datum,
    chorda*    culpa);

/* ~/.rhubarb/<t>.volumen; NIHIL si HOME abest */
constans character*
pipa_viam_voluminis (
    Piscina* piscina);

#endif /* PIPA_H */
```

### fontes/<t>_pipa.c

```c
/* <t>_pipa.c - vide caput (a silice genita) */
#include "<t>_pipa.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include <stdlib.h>

JsonValor*
pipa_salve_tractare (JsonValor* argumenta, Piscina* piscina,
    vacuum* datum, chorda* culpa)
{
    Pipa*      pipa = (Pipa*)datum;
    s64        seq;
    Xar*       acta;
    s64        numerus = 0;
    i32        i;
    JsonValor* fructus;

    (vacuum)argumenta;
    seq = volumen_actum_appendere(pipa->volumen, "salutatio",
        chorda_ex_literis("{}", piscina));
    si (seq <= ZEPHYRUM)
    {
        constans character* err = volumen_error(pipa->volumen);

        *culpa = chorda_ex_literis((err != NIHIL)
            ? err : "volumen: actum non scriptum", piscina);
        redde NIHIL;
    }
    acta = volumen_acta_legere(pipa->volumen, (s64)0, piscina);
    si (acta != NIHIL)
    {
        per (i = 0; i < xar_numerus(acta); i = i + 1)
        {
            VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);

            si (a != NIHIL && chorda_aequalis_literis(a->genus,
                "salutatio"))
            {
                numerus = numerus + 1;
            }
        }
    }
    fructus = json_objectum_creare(piscina);
    json_objectum_ponere(fructus, "nuntius",
        json_chorda_creare_literis(piscina,
            "salve, munde - hic <t>"));
    json_objectum_ponere(fructus, "numerus",
        json_integer_creare(piscina, numerus));
    redde fructus;
}

constans character*
pipa_viam_voluminis (Piscina* piscina)
{
    constans character* domus = getenv("HOME");
    ChordaAedificator*  aed;

    si (domus == NIHIL || domus[0] == '\0')
    {
        redde NIHIL;
    }
    aed = chorda_aedificator_creare(piscina, (memoriae_index)128);
    si (aed == NIHIL)
    {
        redde NIHIL;
    }
    chorda_aedificator_appendere_literis(aed, domus);
    chorda_aedificator_appendere_literis(aed,
        "/.rhubarb/<t>.volumen");
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}
```

### fontes/<t>.c

```c
/* <t>.c - fenestra vitreae cordis: pons + persistentia una
 * salutatione probata. Pipa in <t>_pipa.c; facies in assets/.
 * (A silice genita - exemplar: vocabularium/villa.) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "fenestra.h"
#include "capsula.h"
#include "vitrea.h"
#include "internuntius.h"
#include "volumen.h"
#include "<t>_pipa.h"
#include "capsula_<t>.h"
#include <stdio.h>

#define TICTUS_MS 200

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum(
        "<t>", 16777216);
    Piscina* piscina_vocationis = piscina_generare_dynamicum(
        "<t>_vocationes", 8388608);
    constans character* via;
    Pipa          pipa;
    FenestraConfiguratio figura_fenestrae;
    VitreaConfiguratio   figura_vitreae;
    Fenestra*     fenestra;
    Capsula*      capsula;
    Vitrea*       vitrea;
    Internuntius* inx;

    si (piscina == NIHIL || piscina_vocationis == NIHIL)
    {
        redde I;
    }
    via = pipa_viam_voluminis(piscina);
    si (via == NIHIL)
    {
        imprimere("FRACTA: HOME ignotum\n");
        redde I;
    }
    pipa.volumen = volumen_aperire_aut_creare(piscina, via);
    si (pipa.volumen == NIHIL)
    {
        imprimere("FRACTA: volumen non apertum: %s\n", via);
        imprimere("  (directorium ~/.rhubarb exsistat?)\n");
        redde I;
    }

    figura_fenestrae.titulus  = "<t>";
    figura_fenestrae.x        = CC;
    figura_fenestrae.y        = CC;
    figura_fenestrae.latitudo = 640;
    figura_fenestrae.altitudo = 400;
    figura_fenestrae.vexilla  = FENESTRA_CLAUDIBILIS
        | FENESTRA_MUTABILIS | FENESTRA_CENTRATA;
    fenestra = fenestra_creare(piscina, &figura_fenestrae);
    si (fenestra == NIHIL)
    {
        imprimere("FRACTA: fenestra\n");
        redde I;
    }
    capsula = capsula_aperire(&capsula_<t>, piscina);
    si (capsula == NIHIL)
    {
        imprimere("FRACTA: capsula\n");
        redde I;
    }
    figura_vitreae.origo         = VITREA_ORIGO_CAPSULA;
    figura_vitreae.capsula       = capsula;
    figura_vitreae.via_initialis = "index.html";
    figura_vitreae.url           = NIHIL;
    figura_vitreae.inspectabilis = VERUM;
    vitrea = vitrea_creare(piscina, fenestra, &figura_vitreae);
    si (vitrea == NIHIL)
    {
        imprimere("FRACTA: vitrea\n");
        redde I;
    }
    inx = internuntius_creare(piscina, vitrea_missor, vitrea);
    si (inx == NIHIL)
    {
        imprimere("FRACTA: internuntius\n");
        redde I;
    }
    (vacuum)internuntius_praebere(inx, "salve",
        pipa_salve_tractare, &pipa);

    imprimere("[<t>] fenestra aperta (volumen %s)\n", via);
    fflush(stdout);

    dum (!fenestra_debet_claudere(fenestra))
    {
        Eventus            eventus;
        chorda             nuntium;
        VitreaNuntiusGenus genus_nuntii;
        PiscinaNotatio     nota;

        fenestra_expectare_eventus(fenestra, TICTUS_MS);
        dum (fenestra_obtinere_eventus(fenestra, &eventus))
        {
            /* JS omnia tractat */
        }
        nota = piscina_notare(piscina_vocationis);
        dum (vitrea_obtinere_nuntium(vitrea, &nuntium,
            &genus_nuntii))
        {
            si (genus_nuntii == VITREA_NUNTIUS_PONS)
            {
                internuntius_tractare(inx, nuntium,
                    piscina_vocationis);
            }
            alioquin
            {
                vitrea_recargare(vitrea);
            }
        }
        piscina_reficere(piscina_vocationis, nota);
    }

    vitrea_destruere(vitrea);
    fenestra_destruere(fenestra);
    volumen_claudere(pipa.volumen);
    piscina_destruere(piscina_vocationis);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

(`capsula_<t>` is the symbol capsula_generare derives from the toml section name — section = `<t>`, so header `assets/capsula_<t>.h`, symbol `capsula_<t>`. Titulus validation already forbids `/` and `.`; hyphens in a titulus would break the C symbol — the vitrea branch must additionally refuse `-` in titulus with erratum "titulus vitreus characteres '-' ferre nequit (symbolum C fit)".)

### assets/<t>.toml

```toml
# Capsula frontis (a silice genita)
# aedificare.sh eam OMNI aedificatione regenerat - capsula ante omnia

<t>_files = ["index.html", "<t>.js"]
<t>_compress = false
```

### assets/index.html

```html
<!DOCTYPE html>
<html lang="la">
<head>
<meta charset="utf-8">
<title><t></title>
<style>
  :root { color-scheme: light dark; }
  body { font-family: -apple-system, sans-serif; display: flex;
         align-items: center; justify-content: center;
         min-height: 90vh; margin: 0; }
  main { text-align: center; }
  h1 { letter-spacing: .06em; }
  #salutatio { font-size: 1.3rem; }
</style>
</head>
<body>
<main>
  <h1><t></h1>
  <p id="salutatio">…</p>
</main>
<script src="<t>.js"></script>
</body>
</html>
```

### assets/<t>.js

```js
/* <t>.js - pons probatur: una vocatio, responsum in pagina.
 * (A silice genita. internuntius a vitrea iniectus est.) */
internuntius.vocare('salve', {})
  .then(function (r) {
    document.getElementById('salutatio').textContent =
      r.nuntius + ' — visitatio ' + r.numerus;
  })
  .catch(function (e) {
    document.getElementById('salutatio').textContent =
      'CULPA: ' + e.message;
  });
```

### probationes/probatio_<t>.c

```c
/* probatio_<t>.c - pipa cordis: circuitus per volumen
 * (a silice genita) */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "json.h"
#include "volumen.h"
#include "../fontes/<t>_pipa.h"
#include <stdio.h>

#define VIA_PROBATIONIS "build/probatio.volumen"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum(
        "probatio_<t>", 8388608);
    Volumen* volumen;
    Pipa     pipa;
    chorda   culpa;

    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    remove(VIA_PROBATIONIS);
    volumen = volumen_aperire_aut_creare(piscina, VIA_PROBATIONIS);
    CREDO_NON_NIHIL(volumen);
    pipa.volumen = volumen;

    imprimere("\n--- Probans pipam cordis ---\n");
    {
        JsonValor* r;

        culpa.datum = NIHIL; culpa.mensura = ZEPHYRUM;
        r = pipa_salve_tractare(NIHIL, piscina, &pipa, &culpa);
        CREDO_NON_NIHIL(r);
        CREDO_AEQUALIS_I32((i32)culpa.mensura, ZEPHYRUM);
        CREDO_VERUM(json_ad_integer(
            json_objectum_capere(r, "numerus")) == 1);

        r = pipa_salve_tractare(NIHIL, piscina, &pipa, &culpa);
        CREDO_NON_NIHIL(r);
        CREDO_VERUM(json_ad_integer(
            json_objectum_capere(r, "numerus")) == 2);
        CREDO_CHORDA_NON_VACUA(json_ad_chorda(
            json_objectum_capere(r, "nuntius")));
    }
    {
        constans character* via = pipa_viam_voluminis(piscina);

        CREDO_VERUM(via != NIHIL);
    }

    volumen_claudere(volumen);
    remove(VIA_PROBATIONIS);

    imprimere("\n");
    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        redde ZEPHYRUM;
    }
    redde I;
}
```

### README.md (vitrea flavor)

```markdown
# <t>

App vitrea cordis a silice excusa. Veritas fontium in
`<t>.volumen`; data in `~/.rhubarb/<t>.volumen`.

- `./aedificare.sh && ./bin/<t>` — fenestra (numerus visitationum
  trans aperturas crescit — persistentia visibilis)
- `./probare.sh` — probationes
- `silex partes` — clausura; `silex renovare -scribere` post
  inclusiones novas

Scripta aedificationis PROIECTIONES sunt (generatae a silice) —
mutare licet.
```

### Generated aedificare.sh (shape — the lists marked ⟨…⟩ are COMPUTED)

```sh
#!/bin/sh
# GENERATUM a silice (novum -vitrea, ⟨fons titulus⟩) - PROIECTIO: mutare licet
# Ordines IV: instrumentum+capsula / venditorius / app+.m+frameworks / rm-ante-mv
set -e
cd "$(dirname "$0")"
mkdir -p bin build

VEXILLA="⟨SILEX_VEXILLA_VITREA⟩"
VEXILLA_VENDITORIA="-O2 -DSQLITE_ENABLE_FTS5 ⟨…the 7 defines…⟩"

# I. instrumentum + capsula ante omnia
if [ ! -x build/capsula_generare ] \
   || [ instrumenta/capsula_generare.c -nt build/capsula_generare ]; then
    clang $VEXILLA -Iinclude instrumenta/capsula_generare.c \
        ⟨tool lib list: lib/piscina.c lib/chorda.c … computed⟩ \
        -o build/capsula_generare
fi
./build/capsula_generare assets/<t>.toml

# II. ordo venditorius (unus pro quoque vendor/*.c clausurae)
if [ ! -f build/sqlite3.o ] || [ vendor/sqlite3.c -nt build/sqlite3.o ]; then
    clang -c $VEXILLA_VENDITORIA vendor/sqlite3.c -o build/sqlite3.o
fi

# III+IV. app (fontes explicati - numquam globus)
clang $VEXILLA -Iinclude -Iassets \
    fontes/<t>.c fontes/<t>_pipa.c \
    assets/capsula_<t>.c \
    ⟨app lib list .c+.m computed⟩ \
    ⟨vendor objects⟩ \
    -framework Cocoa -framework WebKit \
    -o build/<t>.novum
rm -f bin/<t>
mv build/<t>.novum bin/<t>
echo "aedificatum: bin/<t>"
```

### Generated probare.sh (shape)

```sh
#!/bin/sh
# GENERATUM a silice (novum -vitrea, ⟨fons titulus⟩) - PROIECTIO: mutare licet
set -e
cd "$(dirname "$0")"
mkdir -p bin build
VEXILLA="⟨SILEX_VEXILLA_VITREA⟩"
VEXILLA_VENDITORIA="⟨as above⟩"
if [ ! -f build/sqlite3.o ] || [ vendor/sqlite3.c -nt build/sqlite3.o ]; then
    clang -c $VEXILLA_VENDITORIA vendor/sqlite3.c -o build/sqlite3.o
fi
clang $VEXILLA -Iinclude \
    probationes/probatio_<t>.c fontes/<t>_pipa.c \
    ⟨probatio lib list computed⟩ \
    ⟨vendor objects⟩ \
    -o bin/probatio_<t>
exec bin/probatio_<t>
```

---

### Task 1: Seed templates + -vitrea plumbing + dual closure (inventory test first)

**Files:**
- Modify: `include/silex.h` (SilexNovumOptiones gains `b32 vitrea;`)
- Modify: `lib/silex.c` (template `_fingere` functions; vitrea branch in silex_novum)
- Modify: `tools/silex.c` (`-vitrea` vexillum → optiones.vitrea)
- Test: `probationes/probatio_silex.c`

**Interfaces:**
- Produces: template emitters `_pipa_h_fingere / _pipa_c_fingere / _fontem_vitreum_fingere / _toml_fingere / _index_html_fingere / _js_fingere / _probationem_vitream_fingere / _readme_vitreum_fingere` — each `(Piscina*, constans character* titulus)` → `chorda`, emitting the target artifacts above verbatim with `<t>` substituted. Also `_clausuram_e_contentis(piscina, fons, contenta[], n)` → Xar* of SilexRes: scans the given template chordae with `_inclusiones_scrutari`, dedups names, calls `silex_clausuram_colligere`. Vitrea branch in silex_novum: refuses `-` in titulus; app closure from (fontes template + pipa.c template + pipa.h template); tool source read via `silex_fons_legere(fons, "tools/capsula_generare.c", …)` → added as `instrumenta/capsula_generare.c` with origo `vendicata:tools/capsula_generare.c`; tool closure from its content; merged into res_omnes deduped by via (a `TabulaDispersa` keyed on via). Scripts in Task 1 are still the PLAIN ones (Task 2 replaces) — the scaffold does not build yet, and that is expected.

- [ ] **Step 1: failing inventory test.** New probatio_silex section (after the existing novum tests):

```c
    /* ========================================================
     * PROBARE: novum -vitrea - inventarium seminis cordis
     * ======================================================== */

    {
        SilexNovumOptiones optiones;
        SilexNovumFructus  fructus;

        imprimere("\n--- Probans novum -vitrea ---\n");

        optiones.fons = silex_fons_disci(piscina, ".");
        optiones.destinatio = AREA;
        optiones.titulus = "vitreum";
        optiones.vitrea = VERUM;
        fructus = silex_novum(piscina, &optiones);
        si (!fructus.successus)
        {
            imprimere("novum -vitrea erratum: %s\n",
                fructus.erratum);
        }
        CREDO_VERUM(fructus.successus);
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum_pipa.h"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/fontes/vitreum_pipa.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/vitreum.toml"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/index.html"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/assets/vitreum.js"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/instrumenta/capsula_generare.c"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/probationes/probatio_vitreum.c"));
        /* clausura vitreae tier: .m par + venditorium par */
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/lib/vitrea_macos.m"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/lib/fenestra_macos.m"));
        CREDO_VERUM(filum_existit(
            AREA "/vitreum/vendor/sqlite3.c"));
        /* clausura instrumenti: toml quod app non trahit */
        CREDO_VERUM(filum_existit(AREA "/vitreum/lib/toml.c"));
        /* titulus cum '-' recusatur (symbolum C fieret) */
        optiones.titulus = "malum-nomen";
        fructus = silex_novum(piscina, &optiones);
        CREDO_FALSUM(fructus.successus);
    }
```

Run `./compile_tests.sh silex`: FAIL at compile (`optiones.vitrea` unknown member) — that is the red.

- [ ] **Step 2: header + tool flag.** silex.h: add `b32 vitrea;   /* semen vitreum + ordines generati */` to SilexNovumOptiones. tools/silex.c: `argumenta_addere_vexillum(parser, "-vitrea", "--vitrea", "novum: semen vitreum (cor voluminis + ordines IV)");` and in the novum tail `optiones.vitrea = argumenta_habet_vexillum(lecta, "--vitrea");`. Existing probatio novum sections gain `optiones.vitrea = FALSUM;` beside the other optiones assignments (three sites).

- [ ] **Step 3: template emitters.** Transcribe each target artifact above into a `_fingere` function following `_fontem_fingere`'s exact idiom (builder + appendere_literis segments split at every `<t>`). Place them after `_readme_fingere`.

- [ ] **Step 4: the vitrea branch in silex_novum.** After the titulus validation add:

```c
    si (optiones->vitrea)
    {
        constans character* t = optiones->titulus;

        dum (*t != '\0')
        {
            si (*t == '-')
            {
                fructus.erratum = "titulus vitreus characteres"
                    " '-' ferre nequit (symbolum C fit)";
                redde fructus;
            }
            t = t + 1;
        }
    }
```

Replace the closure block with a branch: plain path unchanged; vitrea path builds the templates first, scans them for seeds, collects app+tool closures, merges deduped, then `_rem_genitam_addere` for the eight generated files (pipa pair, fontes, toml, index.html, js, probatio, README vitreum) and — Task 1 only — the PLAIN aedificare/probare (replaced in Task 2). Helper:

```c
interior Xar*
_clausuram_e_contentis (Piscina* piscina,
    constans SilexFons* fons, constans chorda* contenta,
    i32 numerus);

/* semina = inclusiones contentorum datorum (dedup); NIHIL si
 * clausura fracta */
interior Xar*
_clausuram_e_contentis (Piscina* piscina,
    constans SilexFons* fons, constans chorda* contenta,
    i32 numerus)
{
    Xar*            nomina = xar_creare(piscina,
        (i32)magnitudo(chorda));
    TabulaDispersa* electa = tabula_dispersa_creare_chorda(
        piscina, 32);
    Xar*            semina_ch = xar_creare(piscina,
        (i32)magnitudo(chorda));
    i32             i;

    si (nomina == NIHIL || electa == NIHIL || semina_ch == NIHIL)
    {
        redde NIHIL;
    }
    per (i = 0; i < numerus; i = i + 1)
    {
        _inclusiones_scrutari(contenta[i], nomina);
    }
    per (i = 0; i < xar_numerus(nomina); i = i + 1)
    {
        chorda n = *(chorda*)xar_obtinere(nomina, i);

        /* capita propria templi (<t>_pipa.h, capsula_<t>.h) non
         * quaeruntur in fonte */
        si (tabula_dispersa_continet(electa, n)
            || _suffixum_habet(n, "_pipa.h")
            || _praefixum_habet(_basis_viae(n), "capsula_"))
        {
            perge;
        }
        tabula_dispersa_inserere(electa, n, (vacuum*)nomina);
        {
            chorda* cella = (chorda*)xar_addere(semina_ch);

            si (cella != NIHIL)
            {
                *cella = n;
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
        redde silex_clausuram_colligere(piscina, fons,
            (constans character* constans*)semina,
            xar_numerus(semina_ch));
    }
}
```

Vitrea closure assembly in silex_novum (replacing the plain colligere call inside the vitrea branch):

```c
        chorda contenta_app[3];
        chorda contentum_instrumenti;
        Xar*   clausura_instrumenti;
        b32    inventum = FALSUM;

        contenta_app[0] = fons_c;      /* _fontem_vitreum_fingere */
        contenta_app[1] = pipa_c;
        contenta_app[2] = pipa_h;
        res_omnes = _clausuram_e_contentis(piscina, optiones->fons,
            contenta_app, 3);
        si (res_omnes == NIHIL) { … erratum ut ante … }

        contentum_instrumenti = silex_fons_legere(optiones->fons,
            "tools/capsula_generare.c", piscina, &inventum);
        si (!inventum)
        {
            fructus.erratum = "instrumentum capsulae in fonte"
                " deest (tools/capsula_generare.c)";
            redde fructus;
        }
        clausura_instrumenti = _clausuram_e_contentis(piscina,
            optiones->fons, &contentum_instrumenti, 1);
        si (clausura_instrumenti == NIHIL) { … erratum … }
        /* mergere dedup per viam */
        {
            TabulaDispersa* viae = tabula_dispersa_creare_chorda(
                piscina, 128);
            i32 k;

            per (k = 0; k < xar_numerus(res_omnes); k = k + 1)
            {
                SilexRes* e = (SilexRes*)xar_obtinere(res_omnes, k);

                tabula_dispersa_inserere(viae, e->via, (vacuum*)e);
            }
            per (k = 0; k < xar_numerus(clausura_instrumenti);
                k = k + 1)
            {
                SilexRes* e = (SilexRes*)xar_obtinere(
                    clausura_instrumenti, k);

                si (!tabula_dispersa_continet(viae, e->via))
                {
                    SilexRes* novus = (SilexRes*)xar_addere(
                        res_omnes);

                    si (novus != NIHIL)
                    {
                        *novus = *e;
                        tabula_dispersa_inserere(viae, novus->via,
                            (vacuum*)novus);
                    }
                }
            }
        }
        /* instrumentum ipsum */
        {
            SilexRes* res = (SilexRes*)xar_addere(res_omnes);

            si (res != NIHIL)
            {
                res->via = chorda_ex_literis(
                    "instrumenta/capsula_generare.c", piscina);
                res->contentum = contentum_instrumenti;
                res->origo = "vendicata:tools/capsula_generare.c";
            }
        }
```

(The ellipsis errata are the existing "clausura bibliothecarum fracta" pattern — same message, same early return shape as the plain branch; write them out in full when transcribing.) The `clausura_instrumenti` and probatio closure are ALSO needed by Task 2's script generators — keep them in scope; structure the vitrea branch as one block computing: `res_omnes` (merged), `clausura_app` (the pre-merge app result, aliased), `clausura_instrumenti`, `clausura_probationis` (= `_clausuram_e_contentis` over {probatio template, pipa_c, pipa_h}).

- [ ] **Step 5: green + plain-parity check + commit**

```bash
./compile_tests.sh silex     # new section green, ALL old novum tests green
git add include/silex.h lib/silex.c tools/silex.c probationes/probatio_silex.c
git commit -m "silex: semen -vitrea (cor voluminis) + clausurae per scopum computatae

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 2: Generated four-tier scripts (content test first)

**Files:**
- Modify: `lib/silex.c`
- Test: `probationes/probatio_silex.c`

**Interfaces:**
- Consumes: `clausura_app` / `clausura_instrumenti` / `clausura_probationis` (Task 1), `_suffixum_habet`, fons titulus.
- Produces: `SILEX_VEXILLA_VITREA` macro (= `SILEX_VEXILLA_COMPILATIONIS " -Wno-overlength-strings -fbracket-depth=512"`); `_ordinem_fontium_appendere(aed, clausura, cum_obiectivis)` — appends ` lib/x.c` for every clausura via under lib/ ending .c, plus (when `cum_obiectivis`) ` lib/y_macos.m` entries; `_aedificare_vitreum_fingere(piscina, titulus, clausura_app, clausura_instrumenti, fons_titulus)` and `_probare_vitreum_fingere(piscina, titulus, clausura_probationis, fons_titulus)` emitting the generated-script shapes above (vendor rule emitted per vendor/*.c via in the respective closure; vendor object name = `build/<basis>.o`).

- [ ] **Step 1: failing content test.** Extend the Task 1 probatio section:

```c
        /* ordines generati: stampa, sine globo, listae computatae */
        {
            chorda aed = filum_legere_totum(
                AREA "/vitreum/aedificare.sh", piscina);
            chorda prob = filum_legere_totum(
                AREA "/vitreum/probare.sh", piscina);

            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "GENERATUM a silice (novum -vitrea", piscina));
            CREDO_FALSUM(chorda_continet(aed,
                chorda_ex_literis("lib/*.c", piscina)));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "lib/toml.c", piscina));          /* instrumentum */
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "lib/vitrea_macos.m", piscina));  /* ordo .m */
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "vendor/sqlite3.c", piscina));    /* ordo vendit. */
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "-framework Cocoa -framework WebKit", piscina));
            CREDO_CHORDA_CONTINET(aed, chorda_ex_literis(
                "rm -f bin/vitreum", piscina));
            CREDO_CHORDA_CONTINET(prob, chorda_ex_literis(
                "GENERATUM a silice", piscina));
            CREDO_FALSUM(chorda_continet(prob,
                chorda_ex_literis("lib/*.c", piscina)));
            CREDO_FALSUM(chorda_continet(prob,
                chorda_ex_literis("_macos.m", piscina)));
            CREDO_CHORDA_CONTINET(prob, chorda_ex_literis(
                "fontes/vitreum_pipa.c", piscina));
        }
```

(If `chorda_continet` under a different name, check credo.h's CREDO_CHORDA_CONTINET expansion for the real function and use that.) Run: FAIL (scripts are still plain — no stamp).

- [ ] **Step 2: implement the two generators** per the shapes above; wire the vitrea branch to use them instead of the plain pair. Vendor tier emission: iterate the closure, for each via `vendor/…c`, emit the mtime-guarded rule with `build/<basis>.o`; collect the object names for the link line. Same closure iteration idiom as `_ordinem_fontium_appendere`.

- [ ] **Step 3: green + commit**

```bash
./compile_tests.sh silex
git add lib/silex.c probationes/probatio_silex.c
git commit -m "silex: ordines IV generati ut proiectiones (listae fontium computatae, sine globis)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 3: Corpus rebuild + acceptance walk (the gate)

**Files:** none (build + live verification)

- [ ] **Step 1: rebuild silex with the new lib** — `./tools/silex_struere.sh` (corpus regen fires: lib/silex.c changed). `silex -versio` names the new commit.

- [ ] **Step 2: THE WALK — corpus only, per-step guards:**

```bash
sh -c '
R="$HOME/Documents/projects/rhubarb"
restore() { [ -d "${R}_ABSCONDITUM" ] && mv "${R}_ABSCONDITUM" "$R"; }
trap restore EXIT
mv "$R" "${R}_ABSCONDITUM" || exit 1
cd "$HOME/Documents/projects/silicetum" || exit 1
rm -rf salve_vitreum
rm -f "$HOME/.rhubarb/salve_vitreum.volumen"
silex novum salve_vitreum -d . -vitrea || { echo FRACTUM:novum; exit 1; }
cd salve_vitreum || exit 1
./aedificare.sh || { echo FRACTUM:aedificare; exit 1; }
./probare.sh || { echo FRACTUM:probare; exit 1; }
./bin/salve_vitreum > /tmp/sv.log 2>&1 & P=$!; sleep 3
kill -0 $P || { echo FRACTUM:fenestra; cat /tmp/sv.log; exit 1; }
kill $P; sleep 1
./bin/salve_vitreum > /tmp/sv2.log 2>&1 & P=$!; sleep 3
kill $P
silex partes | grep -E "M ADEST|V ADEST" || { echo FRACTUM:partes; exit 1; }
silex condere -n "ambulatio -vitrea sine fabrica" || exit 1
echo AMBULATIO-VITREA-PLENA
' 2>&1 | tail -12
ls "$HOME/Documents/projects/rhubarb/include/latina.h" && echo RESTITUTA
```

- [ ] **Step 3: persistence proxy** — each launch's JS fires one `salve`, so two launches = ≥2 salutatio acta:

```bash
sqlite3 "$HOME/.rhubarb/salve_vitreum.volumen" \
  "SELECT COUNT(*) FROM acta WHERE genus='salutatio';"
```

Expected ≥ 2. Then the eye test is Fran's: relaunch `bin/salve_vitreum` and watch the visitatio number climb. (If the count is 0 with the window opening fine, the bridge is silently broken — treat as FRACTUM, debug via Safari Inspector.)

- [ ] **Step 4:** nothing to commit (scaffold lives in silicetum); note walk results for Task 4's records.

---

### Task 4: Records + desideratum closure

- [ ] **Step 1: worklog** (lib/silex.worklog.md): the -vitrea arc — template-scan seeding (no hand lists), dual closure merge, script generation numbers, walk transcript summary.
- [ ] **Step 2: ledger** — nota on 01KZ6CSBQN with the walk result; **close it as impletum**; `addere` a fresh desideratum for the two deferred items (vexilla source of truth — trigger: third consumer or battery change; script staleness/regeneration — trigger: renovare adds a file a script doesn't cover), linking back via nexus.
- [ ] **Step 3: memory** — silex-volumen-project.md + MEMORY.md line: `-vitrea` SHIPPED, scaffold = vocabularium architecture, scripts are projections.
- [ ] **Step 4: final** — full suite green, tree clean (staging files only), commit records.
