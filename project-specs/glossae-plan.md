# Glossae Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `<glossa lingua="...">` localized documentation on genera, carried into generated canons, with a coverage lint (`bin/natura_glossae`) and a fused encyclopedia+worklist HTML page.

**Architecture:** Pure data edits to two hand-written grammars (natura.canon, canon.canon — `lib/canon.c` and `lib/natura.c` need ZERO code changes, confirmed by grounding); one insertion point in the shared projection writer `_elementum_scribere` (piscina threaded through `_canonem_emittere`); one new standalone C tool on the natura loader.

**Tech Stack:** C89, house libs (natura/canon/stml/chorda/piscina/xar/filum/iter_directoria/processus), shell wrappers.

**Spec:** `project-specs/glossae-spec.md` (f87162e). Deviations found by grounding, both simplifications: (1) the loader already "carries" glossae via `NaturaGenus->nodus` (hybrid design — typed skeleton, everything else read off the retained STML subtree), so spec §3's loader work vanishes; (2) `lib/canon.c`'s loader skips element-definition children lacking `nomen=`, so canon-side tolerance is pure canon.canon data and the required-`lingua=` check is the existing generic `necessarium="verum"` machinery.

## Global Constraints

- C89, flags: `-pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings` (+ `-Wno-long-long -Wno-overlength-strings` in tool builds)
- Everything in Latin; latina.h macro words are FORBIDDEN identifiers (nomen, ordinarius, per, casus, registrum, magnitudo, structura, externus, ...)
- `chorda` is NOT null-terminated; `i32`/`i64`/`b32` are UNSIGNED (cast relational results: `(b32)(x >= I)`)
- ISO 639-1 codes (`la`, `en`, `fr`) are untranslated technical terms
- Pre-fix-oracle law: every must-fail test verified failing on pre-change code
- Exit discipline: 2 = NOTHING RAN, 1 = findings/gate, 0 = clean; never gate on piped `$?`
- Cold `./silva/examen.sh <file>` is the authority when hook diagnostics look stale
- After editing tools/natura_canones.* or lib sources: rebuild `./tools/natura_struere.sh`; regenerate order is `./tools/natura_canones.sh` then `./tools/canon_coquere.sh`
- Commits end with `Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>`; stage by explicit path only — NEVER stage `FAQ.md`, `gesta/annales/forum.jsonl`, `gesta/annales/tabula.md`, `gesta/annales/tabularium.jsonl`
- Test runner: `./compile_tests.sh canon`, `./compile_tests.sh natura_canones`, `./compile_tests.sh natura_glossae` (pattern matches filename fragment; `probatio_*` prefixed patterns match NOTHING)
- Pre-existing naming collision, do not touch: `natura_examen.c` uses local "glossa"/glossae.txt for the derived first-sentence-of-`<definitio>` it feeds `natura_visus.sh`. Different layer; leave as is.

---

### Task 1: Grammars admit `<glossa>` (natura.canon + canon.canon)

**Files:**
- Modify: `natura/natura.canon` (genus children ~line 57-81; DESCRIPTIVA section ~line 83-88)
- Modify: `canon.canon` (`<elementum nomen="elementum">` ~line 54-75; new top-level definition)
- Test: `probationes/probatio_canon.c` (new section at end, before teardown; add `#include "filum.h"`)

**Interfaces:**
- Produces: grammar admission consumed by Tasks 2-5. `<glossa>` shape everywhere: required `lingua=` (genus `nomen`), `textus="verum"`, unbounded cardinality.

- [ ] **Step 1: Write the failing tests**

New file-scope fixtures in probatio_canon.c:

```c
/* spec glossae: documentatio vernacula - forma sola canonis est
 * (lingua necessaria), copertura lintri est */
interior constans character* GLOSSA_SUB_ELEMENTO =
    "<canon dialectus=\"probatio\" versio=\"1\">\n"
    "  <elementum nomen=\"radix\" radix=\"verum\">\n"
    "    <glossa lingua=\"en\">A test root element.</glossa>\n"
    "    <glossa lingua=\"fr\">Un element racine d'essai.</glossa>\n"
    "    <attributum nomen=\"nomen\" genus=\"nomen\"/>\n"
    "  </elementum>\n"
    "</canon>\n";

interior constans character* GLOSSA_SUB_GENERE =
    "<natura modulus=\"probatio\" versio=\"1\" lingua=\"latina\">\n"
    "  <genus nomen=\"probandum\">\n"
    "    <definitio>Res probationis.</definitio>\n"
    "    <glossa lingua=\"en\">A thing under test.</glossa>\n"
    "    <glossa lingua=\"fr\">Une chose a l'essai.</glossa>\n"
    "  </genus>\n"
    "</natura>\n";

interior constans character* GLOSSA_SINE_LINGUA =
    "<natura modulus=\"probatio\" versio=\"1\" lingua=\"latina\">\n"
    "  <genus nomen=\"probandum\">\n"
    "    <glossa>Sine lingua.</glossa>\n"
    "  </genus>\n"
    "</natura>\n";
```

New section (tests run from repo root, so relative disk paths work — same convention as probatio_natura_canones):

```c
    /* ========================================================
     * PROBARE: glossae in grammaticis (spec glossae)
     * ======================================================== */

    {
        Canon* canon_canonum;
        Canon* canon_naturae;
        chorda fons;
        chorda causa;
        Xar*   vitia;

        imprimere("\n--- Probans glossas in grammaticis ---\n");

        fons = filum_legere_totum("canon.canon", piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        canon_canonum = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_canonum);

        vitia = iudicare_literis(canon_canonum, GLOSSA_SUB_ELEMENTO,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        fons = filum_legere_totum("natura/natura.canon", piscina);
        CREDO_MAIOR_I32 (fons.mensura, (i32)ZEPHYRUM);
        canon_naturae = canon_legere(fons, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_naturae);

        vitia = iudicare_literis(canon_naturae, GLOSSA_SUB_GENERE,
                                 piscina, intern);
        CREDO_NON_NIHIL (vitia);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), ZEPHYRUM);

        /* lingua necessaria: machina generica (necessarium=verum)
         * eam cogit - nullus codex novus in lib/canon.c */
        vitia = iudicare_literis(canon_naturae, GLOSSA_SINE_LINGUA,
                                 piscina, intern);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vitia), I);
        CREDO_AEQUALIS_I32 (quot_generis(vitia,
            CANON_ATTRIBUTUM_DEEST), I);
    }
```

- [ ] **Step 2: Run to verify RED**

Run: `./compile_tests.sh canon`
Expected: the three new judgments fail — pre-change, glossa produces `CANON_LIBERUM_ILLICITUM` + `CANON_ELEMENTUM_IGNOTUM` (the double-fire; existing test at probatio_canon.c:496-504 documents it), so the `== ZEPHYRUM` counts read 2+ and the SINE_LINGUA case reports the wrong genus. All pre-existing sections stay green.

- [ ] **Step 3: Edit natura/natura.canon**

Inside `<elementum nomen="genus">`, after the `<liberum nomen="differentia" maximum="1"/>` line:

```xml
    <liberum nomen="glossa"/>
```

In the DESCRIPTIVA section, after `<elementum nomen="dubium" textus="verum"/>`:

```xml
  <elementum nomen="glossa" textus="verum"
    nota="documentatio vernacula (spec glossae): lingua ISO 639-1
          (en/fr/...); definitio Latine VERITAS manet - glossa
          explicat, numquam renominat. Copertura lintri est
          (natura_glossae), non canonis">
    <attributum nomen="lingua" genus="nomen" necessarium="verum"/>
  </elementum>
```

- [ ] **Step 4: Edit canon.canon**

Inside `<elementum nomen="elementum">`, after `<liberum nomen="liberum"/>`:

```xml
    <liberum nomen="glossa"/>
```

New top-level definition, after `<elementum nomen="liberum">...</elementum>`:

```xml
  <elementum nomen="glossa" textus="verum"
    nota="documentatio vernacula in canone (spec glossae) - iudicium
          eam numquam AGIT, formam solam cogit (lingua necessaria).
          canon_legere et canon_coquere eam praetereunt (liberum
          sine nomen=)">
    <attributum nomen="lingua" genus="nomen" necessarium="verum"/>
  </elementum>
```

- [ ] **Step 5: Verify green + self-judgment**

Run: `./compile_tests.sh canon` → PASS.
Run: `bin/canon_examen canon.canon && bin/canon_examen natura/natura.canon` → both 0 vitia (canon.canon judges itself; the edits must be self-consistent).
Run: `./tools/canon_coquere.sh -probare` → exit 0 (coquere ignores glossa; readers unchanged).

- [ ] **Step 6: Commit**

```bash
git add probationes/probatio_canon.c natura/natura.canon canon.canon
git commit -m "glossae: grammaticae glossam admittunt (natura.canon + canon.canon)

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 2: Seed glossae on genus planta

**Files:**
- Modify: `natura/planta.genera` (genus planta, after `</definitio>` ~line 48)

**Interfaces:**
- Produces: the corpus content Task 3's sentinels and Task 4/5's real-corpus rows read. Sentinel strings: `<glossa lingua="en">` and `photosynthesis`.

- [ ] **Step 1: Insert the two glossae**

Immediately after `</definitio>` of `<genus nomen="planta" ...>`:

```xml
  <glossa lingua="en">A plant: a living thing rooted in place that
    turns light into food (photosynthesis); it grows by its parts
    and multiplies by seed or cutting. Vegetative soul only -
    nutrition, growth, generation, without sensation.</glossa>
  <glossa lingua="fr">Une plante : un vivant enraciné qui ne se
    déplace pas et transforme la lumière en nourriture
    (photosynthèse) ; il croît par ses parties et se multiplie par
    graine ou bouture. Âme végétative seule - nutrition,
    croissance, génération, sans sensation.</glossa>
```

(Files are UTF-8; accented French is correct, not transliterated.)

- [ ] **Step 2: Verify gates**

Run: `bin/natura_examen -plagula natura/planta.genera -machina` → exit 0 (grammar from Task 1 admits it).
Run: `./tools/natura_canones.sh -probare` → exit 0. **This proves the no-op property**: the generator does not yet read glossae, so generated output is byte-identical — data landed with zero projection change.

- [ ] **Step 3: Commit**

```bash
git add natura/planta.genera
git commit -m "glossae: semina prima - genus planta en/fr glossatur

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 3: Projection emits glossae into generated canons

**Files:**
- Modify: `tools/natura_canones.h` (`_canonem_emittere` prototype gains `Piscina*`)
- Modify: `tools/natura_canones_emissio.c` (`_glossas_scribere` new; `_elementum_scribere` signature + call; forward decls)
- Modify: `tools/natura_canones.c` (two call sites: per-module ~:1461, monolith ~:1664)
- Modify: `natura/cocta/planta.canon`, `natura/cocta/individua.canon` (regenerated)
- Test: `probationes/probatio_natura_canones.c` (new section XII)

**Interfaces:**
- Consumes: Task 2's seeded glossae via `el->ens->nodus` (raw STML subtree).
- Produces: `_canonem_emittere(FILE*, Xar*, constans character*, constans character*, constans character*, Piscina*)` — new final param. Generated elementa carry `    <glossa lingua="X">escaped text</glossa>` lines directly after their opening tag.

- [ ] **Step 1: Write the failing sentinels**

New section in probatio_natura_canones.c after section XI (same shape as the semina sentinels at :1349-1369):

```c
    /* ========================================================
     * XII. glossae in canones transcriptae (spec glossae par. 4)
     * ======================================================== */

    {
        chorda moduli_canon;
        chorda monolithus;
        chorda semina;

        imprimere("\n--- XII. glossae transcriptae ---\n");

        /* COMPLETUDO, non sola validitas (lex 2026-08-08): porta
         * se-iudicans omissionem non videt - sententiae certae
         * asseruntur, in canone moduli ET in monolitho */
        moduli_canon = filum_legere_totum(
                           "natura/cocta/planta.canon", piscina);
        CREDO_MAIOR_I32 (moduli_canon.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(moduli_canon,
            chorda_ex_literis("<glossa lingua=\"en\">",
                              piscina)) >= ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(moduli_canon,
            chorda_ex_literis("photosynthesis",
                              piscina)) >= ZEPHYRUM);

        monolithus = filum_legere_totum(
                         "natura/cocta/individua.canon", piscina);
        CREDO_MAIOR_I32 (monolithus.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(monolithus,
            chorda_ex_literis("<glossa lingua=\"fr\">",
                              piscina)) >= ZEPHYRUM);

        /* census glossas NUMQUAM fert - docs GENERA describunt,
         * individua data sunt (spec par. 4) */
        semina = filum_legere_totum(
                     "natura/cocta/semina.census", piscina);
        CREDO_MAIOR_I32 (semina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (chorda_invenire_index(semina,
            chorda_ex_literis("<glossa", piscina)) < ZEPHYRUM);
    }
```

- [ ] **Step 2: Run to verify RED**

Run: `./compile_tests.sh natura_canones`
Expected: FAIL — committed canons carry no glossa yet. The semina negative assertion passes already (it guards against future regression).

- [ ] **Step 3: Thread the piscina**

`tools/natura_canones.h` — `_canonem_emittere` gains a final parameter:

```c
b32
_canonem_emittere(
    FILE*                f,
    Xar*                 elementa,
    constans character*  dialectus,
    constans character*  fons,
    constans character*  praefatio,
    Piscina*             piscina);
```

`tools/natura_canones_emissio.c` — definition matches; inside it, `_elementum_scribere(f, el, &planata, &aliena, &omissa)` becomes `_elementum_scribere(f, el, &planata, &aliena, &omissa, piscina)`; `_elementum_scribere`'s signature (definition ~:782 and forward decl ~:50) gains `Piscina* piscina`.

`tools/natura_canones.c` — both call sites pass their in-scope piscina:
- ~:1461 `_canonem_emittere(f, elementa, modulus, modulus, NIHIL, piscina)`
- ~:1664 `_canonem_emittere(f, elementa, "individua", "natura tota", praefatio, piscina)`

(Verify the exact local variable name at each site on contact.)

- [ ] **Step 4: Add `_glossas_scribere` and call it**

In the forward-decl block (~:40-60) add:

```c
interior vacuum  _glossas_scribere(FILE* f, StmlNodus* nodus,
                                   Piscina* piscina);
interior vacuum  _chordam_scribere(FILE* f, constans chorda* c);
```

(the second duplicates the mid-file interior prototype at ~:1233 so it is visible early — legal C89, same declaration.)

Definition, placed just before `_elementum_scribere`:

```c
/* glossae generis in canonem transcribere (spec glossae par. 4):
 * documentatio vernacula elementum generatum comitatur - qui
 * canonem solum tenet, docs habet, sine .genera. Textus
 * normalizatus (ut valores), evasus (ut optiones). Res dictionarii
 * glossas hodie non ferunt (natura.canon sub genere solo admittit)
 * - lectio generica tamen: si umquam ferent, sponte transibunt. */
interior vacuum
_glossas_scribere(
    FILE*       f,
    StmlNodus*  nodus,
    Piscina*    piscina)
{
    i32 numerus;
    i32 i;

    si (!nodus)
    {
        redde;
    }

    numerus = stml_numerus_liberorum(nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    lingua;
        chorda     textus;

        l = stml_liberum_ad_indicem(nodus, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "glossa"))
        {
            perge;
        }
        lingua = stml_attributum_capere(l, "lingua");
        si (!lingua)
        {
            perge;   /* natura_examen id iam clamat - hic tacetur */
        }

        fputs("    <glossa lingua=\"", f);
        _chordam_scribere(f, lingua);
        fputs("\">", f);
        textus = stml_textus_normalizatus(l, piscina);
        _textum_evasum_scribere(f, chorda_ut_cstr(textus, piscina));
        fputs("</glossa>\n", f);
    }
}
```

Call site — in `_elementum_scribere`, directly after the opening tag write (`fputs("\">\n", f);` ~:798), before the identity-attributum comment:

```c
    _glossas_scribere(f, el->ens->nodus, piscina);
```

- [ ] **Step 5: Rebuild, regenerate, verify**

Run: `./tools/natura_struere.sh` → both binaries rebuilt.
Run: `./tools/natura_canones.sh` → regenerates; exit 0 (coquere readers unaffected — glossa is skipped by its walker).
Run: `git status --short natura/cocta/` → exactly `planta.canon` and `individua.canon` changed (only canons whose elementa include genus planta).
Run: `bin/canon_examen natura/cocta/planta.canon` → 0 vitia (Task 1's canon.canon admits it).
Run: `./compile_tests.sh natura_canones` → PASS (sentinels green).

- [ ] **Step 6: Commit**

```bash
git add tools/natura_canones.h tools/natura_canones.c \
        tools/natura_canones_emissio.c \
        natura/cocta/planta.canon natura/cocta/individua.canon \
        probationes/probatio_natura_canones.c
git commit -m "glossae: emissio in canones (par. 4) - documenta cum artefacto vadunt

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 4: The lint — bin/natura_glossae

**Files:**
- Create: `tools/natura_glossae.c`
- Create: `tools/natura_glossae.sh` (wrapper, +x)
- Create: `probationes/exempla/gl_plenum/probatio.genera`
- Create: `probationes/exempla/gl_lacunosum/probatio.genera`
- Create: `probationes/probatio_natura_glossae.c`
- Modify: `tools/natura_struere.sh` (third link line)
- Modify: `compile_tests_fontes_generata.sh` (regenerated)

**Interfaces:**
- Consumes: `NaturaBibliotheca` (`bib->exemplaria`, `bib->genera_omnia`, `genus->nodus`, `genus->modulus`); Task 1's grammar (fixtures must be canon-sane — natura-custos judges saved `.genera`).
- Produces: `bin/natura_glossae [-radix DIR] [-linguae CSV] [-porta] [-machina] [-html VIA]`. Machine rows (TAB-separated): `<modulus>\t<lingua>\t<habentia>\t<omnia>`, `CARENS\t<modulus>\t<genus>\t<lingua>`, `GEMINA\t<modulus>\t<genus>\t<lingua>`, `SUMMA\t<lingua>\t<habentia>\t<omnia>`. Exit: 0 report/porta-clean, 1 porta with gaps or duplicates, 2 nothing ran. Task 5 adds `-html` behavior to this same tool.

- [ ] **Step 1: Write the fixture corpora**

`probationes/exempla/gl_plenum/probatio.genera` — full coverage:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- fixtura probationis natura_glossae: copertura PLENA
     (la per definitionem, en/fr per glossas) -->
<natura modulus="probatio" versio="1" lingua="latina">

<genus nomen="alpha">
  <definitio>Prima res probationis.</definitio>
  <glossa lingua="en">The first test kind.</glossa>
  <glossa lingua="fr">La première sorte d'essai.</glossa>
</genus>

<genus nomen="beta" sub="alpha">
  <definitio>Secunda res probationis.</definitio>
  <glossa lingua="en">The second test kind.</glossa>
  <glossa lingua="fr">La seconde sorte d'essai.</glossa>
</genus>

</natura>
```

`probationes/exempla/gl_lacunosum/probatio.genera` — one gap of each kind:

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!-- fixtura probationis natura_glossae: LACUNAE - gamma sine fr,
     delta sine definitione (la) et cum en GEMINA. gamma en fert
     entitatem &lt; ut evasio HTML (opus V) probetur. -->
<natura modulus="probatio" versio="1" lingua="latina">

<genus nomen="gamma">
  <definitio>Res cum lacuna Gallica.</definitio>
  <glossa lingua="en">Gap kind: French missing; uses &lt;markup&gt;
    safely.</glossa>
</genus>

<genus nomen="delta">
  <glossa lingua="en">First English gloss.</glossa>
  <glossa lingua="en">Second English gloss - duplicate.</glossa>
  <glossa lingua="fr">Une glose française.</glossa>
</genus>

</natura>
```

- [ ] **Step 2: Write the failing test file**

`probationes/probatio_natura_glossae.c`. Mirror the self-build/stale-refusal block from probatio_natura_canones.c:845-882 verbatim, adapting the binary path to `bin/natura_glossae` (it execs `./tools/natura_struere.sh` via `processus_exsequi(struere, 300000, piscina)` when the binary is missing, and REFUSES on staleness). Then:

```c
#include "latina.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "filum.h"
#include "processus.h"
#include <stdio.h>
#include <string.h>

/* bin/natura_glossae super corpore dato currere; effusio (stdout)
 * capta - assertio in lineis TSV fit */
interior ProcessusResultus
_censum_currere(
    constans character*  radix,
    constans character*  arg1,
    constans character*  arg2,
    constans character*  arg3,
    Piscina*             piscina)
{
    constans character* argumenta[VIII];
    i32                 n;

    n = ZEPHYRUM;
    argumenta[n++] = "bin/natura_glossae";
    argumenta[n++] = "-radix";
    argumenta[n++] = radix;
    si (arg1) { argumenta[n++] = arg1; }
    si (arg2) { argumenta[n++] = arg2; }
    si (arg3) { argumenta[n++] = arg3; }
    argumenta[n] = NIHIL;

    redde processus_exsequi(argumenta, 60000, piscina);
}

interior b32
_continet_literis(
    chorda               effusio,
    constans character*  literae,
    Piscina*             piscina)
{
    redde chorda_continet(effusio,
                          chorda_ex_literis(literae, piscina));
}
```

Sections in `principale` (after the build/stale block, `credo_aperire`):

```c
    /* ---- I. plenum: copertura tota ---- */
    {
        ProcessusResultus r;

        imprimere("\n--- I. corpus plenum ---\n");

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-machina", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tla\t2\t2", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\ten\t2\t2", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tfr\t2\t2", piscina));
        CREDO_VERUM (!_continet_literis(r.effusio, "CARENS",
                                        piscina));
        CREDO_VERUM (!_continet_literis(r.effusio, "GEMINA",
                                        piscina));

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-porta", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
    }

    /* ---- II. lacunosum: carentia et gemina nominatim ---- */
    {
        ProcessusResultus r;

        imprimere("\n--- II. corpus lacunosum ---\n");

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-machina", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        /* relatio numquam obstat - modus -porta solus */
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
        CREDO_VERUM (_continet_literis(r.effusio,
            "CARENS\tprobatio\tgamma\tfr", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "CARENS\tprobatio\tdelta\tla", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "GEMINA\tprobatio\tdelta\ten", piscina));
        CREDO_VERUM (_continet_literis(r.effusio,
            "SUMMA\tfr\t1\t2", piscina));

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-porta", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, I);
    }

    /* ---- III. -linguae electio postulata mutat ---- */
    {
        ProcessusResultus r;

        imprimere("\n--- III. -linguae ---\n");

        /* fr sola postulata: gamma caret -> porta rubet */
        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-porta", "-linguae", "fr", piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, I);

        /* en sola in pleno: porta viret */
        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-porta", "-linguae", "en", piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);
    }

    /* ---- IV. NIHIL CURSUM = II, numquam sanitas tacita ---- */
    {
        ProcessusResultus r;

        imprimere("\n--- IV. exitus II ---\n");

        r = _censum_currere("probationes/exempla/NON_EST",
                            NIHIL, NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, II);

        r = _censum_currere("probationes/exempla/gl_plenum",
                            "-ignotum", NIHIL, NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, II);
    }
```

(Plus the standard `piscina_generare_dynamicum("probatio_natura_glossae", 8388608)` open and `credo_claudere`-style teardown — copy the frame from probatio_natura_canones.c.)

- [ ] **Step 3: Verify RED**

Run: `./tools/compile_tests_fontes_generare.sh` (new probatio may widen the source union), then `./compile_tests.sh natura_glossae`.
Expected: FAIL — the binary does not exist and natura_struere.sh does not yet build it (the self-build attempt runs but produces no `bin/natura_glossae`).

- [ ] **Step 4: Write the tool**

`tools/natura_glossae.c`:

```c
/* tools/natura_glossae.c - census glossarum (spec glossae par. 5)
 *
 * Copertura documentationis per corpus generum: 'la' per
 * <definitio> exsistentem, ceterae linguae per <glossa lingua=>.
 * Relatio ordinaria numquam obstat (index operum est dum corpus
 * rubet); -porta obstat. Denominator = genera propria (non
 * species/umbrae/individua).
 *
 * Divisio laboris (spec par. 3): canon FORMAM iudicat (lingua
 * necessaria - natura.canon), hic COPERTURA et GEMINA.
 *
 * NB 'glossae' in natura_examen/visus aliud est (sententia prima
 * definitionis, glossae.txt) - stratum vetus, non hoc. */

#include "latina.h"
#include "natura.h"
#include "stml.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "filum.h"
#include "iter_directoria.h"
#include <stdio.h>
#include <string.h>

#define EXTENSIO         ".genera"
#define EXTENSIO_LONGA   VII
#define NG_LINGUAE_MAXIMAE 8
#define NG_CODEX_LONGUS    8
#define NG_COLUMNA         XXIV

nomen structura {
    character  codices[NG_LINGUAE_MAXIMAE][NG_CODEX_LONGUS];
    i32        numerus;
} NgLinguae;

interior b32  _extensionem_habet(constans chorda* titulus);
interior vacuum _stirpem_scribere(constans chorda* titulus,
                                  character* exitus, i32 capacitas);
interior b32  _linguas_legere(constans character* lista,
                              NgLinguae* linguae);
interior i32  _genus_linguae_quot(NaturaGenus* g,
                                  constans character* codex);
interior vacuum _columnam_scribere(FILE* f, constans chorda* t,
                                   i32 latitudo);

interior b32
_extensionem_habet(
    constans chorda*  titulus)
{
    si (titulus->mensura <= (i32)EXTENSIO_LONGA)
    {
        redde FALSUM;
    }
    redde (b32)(memcmp(titulus->datum +
                    (titulus->mensura - (i32)EXTENSIO_LONGA),
                    EXTENSIO, (size_t)EXTENSIO_LONGA) == ZEPHYRUM);
}

interior vacuum
_stirpem_scribere(
    constans chorda*  titulus,
    character*        exitus,
    i32               capacitas)
{
    i32 longitudo;
    i32 i;

    longitudo = titulus->mensura - (i32)EXTENSIO_LONGA;
    si (longitudo >= capacitas)
    {
        longitudo = capacitas - I;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        exitus[i] = (character)titulus->datum[i];
    }
    exitus[longitudo] = '\0';
}

/* 'la,en,fr' -> codices; FALSUM si vacuus/longus/nimii */
interior b32
_linguas_legere(
    constans character*  lista,
    NgLinguae*           linguae)
{
    i32 n;
    i32 j;

    linguae->numerus = ZEPHYRUM;
    n = ZEPHYRUM;
    j = ZEPHYRUM;
    dum (VERUM)
    {
        character c;

        c = lista[n];
        si (c == ',' || c == '\0')
        {
            si (j == ZEPHYRUM ||
                linguae->numerus >= (i32)NG_LINGUAE_MAXIMAE)
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = '\0';
            linguae->numerus++;
            j = ZEPHYRUM;
            si (c == '\0')
            {
                frange;
            }
        }
        alioquin
        {
            si (j >= (i32)(NG_CODEX_LONGUS - I))
            {
                redde FALSUM;
            }
            linguae->codices[linguae->numerus][j] = c;
            j++;
        }
        n++;
    }
    redde VERUM;
}

/* quot vices genus linguam ferat: 'la' = definitio exsistens
 * (0/1 - canon maximum=1 cogit), ceterae = glossae congruentes
 * numeratae (>1 = GEMINA) */
interior i32
_genus_linguae_quot(
    NaturaGenus*         g,
    constans character*  codex)
{
    i32 numerus;
    i32 i;
    i32 quot;

    si (strcmp(codex, "la") == ZEPHYRUM)
    {
        redde stml_invenire_liberum(g->nodus, "definitio")
                  ? I : ZEPHYRUM;
    }

    quot    = ZEPHYRUM;
    numerus = stml_numerus_liberorum(g->nodus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        chorda*    lingua;

        l = stml_liberum_ad_indicem(g->nodus, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM ||
            !chorda_aequalis_literis(*l->titulus, "glossa"))
        {
            perge;
        }
        lingua = stml_attributum_capere(l, "lingua");
        si (lingua && chorda_aequalis_literis(*lingua, codex))
        {
            quot++;
        }
    }
    redde quot;
}

interior vacuum
_columnam_scribere(
    FILE*             f,
    constans chorda*  t,
    i32               latitudo)
{
    i32 i;

    fprintf(f, "%.*s", (integer)t->mensura,
            (constans character*)t->datum);
    per (i = t->mensura; i < latitudo; i++)
    {
        putc(' ', f);
    }
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*              piscina;
    NaturaBibliotheca*    bib;
    DirectoriumIterator*  iter;
    DirectoriumIntroitus* introitus;
    constans character*   radix;
    constans character*   lista_linguarum;
    constans character*   via_html;
    NgLinguae             linguae;
    b32                   modus_porta;
    b32                   modus_machina;
    s32                   i;
    i32                   onerata;
    i32                   lacunae;
    i32                   m;
    i32                   g_i;
    i32                   l_i;
    i32                   summa_habentia[NG_LINGUAE_MAXIMAE];
    i32                   summa_omnia;

    radix           = "natura";
    lista_linguarum = "la,en,fr";
    via_html        = NIHIL;
    modus_porta     = FALSUM;
    modus_machina   = FALSUM;

    per (i = I; i < numerus; i++)
    {
        si (strcmp(argumenta[i], "-porta") == ZEPHYRUM)
        {
            modus_porta = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            modus_machina = VERUM;
        }
        alioquin si (strcmp(argumenta[i], "-radix") == ZEPHYRUM &&
                     i + I < numerus)
        {
            radix = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-linguae") == ZEPHYRUM &&
                     i + I < numerus)
        {
            lista_linguarum = argumenta[++i];
        }
        alioquin si (strcmp(argumenta[i], "-html") == ZEPHYRUM &&
                     i + I < numerus)
        {
            via_html = argumenta[++i];
        }
        alioquin
        {
            fprintf(stderr,
                "usus: natura_glossae [-radix DIR] "
                "[-linguae la,en,fr] [-porta] [-machina] "
                "[-html VIA]\n");
            redde II;
        }
    }

    si (!_linguas_legere(lista_linguarum, &linguae))
    {
        fprintf(stderr,
            "natura_glossae: -linguae '%s' legi nequit\n",
            lista_linguarum);
        redde II;
    }

    piscina = piscina_generare_dynamicum("natura_glossae", 4194304);
    bib     = natura_bibliotheca_creare(piscina);
    si (!bib)
    {
        fprintf(stderr,
            "natura_glossae: bibliotheca creari nequit\n");
        redde II;
    }

    iter = directorium_iterator_aperire(radix, piscina);
    si (!iter)
    {
        fprintf(stderr, "natura_glossae: '%s' aperiri nequit\n",
                radix);
        redde II;
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
        si ((size_t)introitus->titulus.mensura + strlen(radix) + II >
            magnitudo(via))
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
            fprintf(stderr, "natura_glossae: '%s' legi nequit\n",
                    via);
            perge;
        }
        natura_legere(bib, fons, stirps);
        onerata++;
    }
    directorium_iterator_claudere(iter);

    si (onerata == ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: NULLUM exemplar in '%s' - nihil "
            "cursum\n", radix);
        redde II;
    }

    /* vulnera corporis lint alterius sunt (natura_examen) -
     * copertura etiam super corpore vulnerato numeratur */
    natura_nectere(bib);

    /* ---- tabula coperturae ---- */

    summa_omnia = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        summa_habentia[l_i] = ZEPHYRUM;
    }

    si (!modus_machina)
    {
        printf("%-*s", (integer)NG_COLUMNA, "GLOSSAE");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("%-10s", linguae.codices[l_i]);
        }
        printf("\n");
    }

    per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
    {
        NaturaExemplar* ex;
        i32             omnia_moduli;
        i32             habentia[NG_LINGUAE_MAXIMAE];

        ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria, m);
        omnia_moduli = ZEPHYRUM;
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            habentia[l_i] = ZEPHYRUM;
        }

        per (g_i = ZEPHYRUM; g_i < xar_numerus(bib->genera_omnia);
             g_i++)
        {
            NaturaGenus* g;

            g = *(NaturaGenus**)xar_obtinere(bib->genera_omnia,
                                             g_i);
            si (!chorda_aequalis(*g->modulus, *ex->stirps))
            {
                perge;
            }
            omnia_moduli++;
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                si (_genus_linguae_quot(g,
                        linguae.codices[l_i]) >= I)
                {
                    habentia[l_i]++;
                }
            }
        }

        si (omnia_moduli == ZEPHYRUM)
        {
            perge;   /* exemplar sine generibus propriis */
        }
        summa_omnia += omnia_moduli;

        si (modus_machina)
        {
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                _columnam_scribere(stdout, ex->stirps, ZEPHYRUM);
                printf("\t%s\t%u\t%u\n", linguae.codices[l_i],
                       habentia[l_i], omnia_moduli);
                summa_habentia[l_i] += habentia[l_i];
            }
        }
        alioquin
        {
            _columnam_scribere(stdout, ex->stirps, NG_COLUMNA);
            per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
            {
                character fractio[XXXII];

                sprintf(fractio, "%u/%u", habentia[l_i],
                        omnia_moduli);
                printf("%-10s", fractio);
                summa_habentia[l_i] += habentia[l_i];
            }
            printf("\n");
        }
    }

    si (modus_machina)
    {
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            printf("SUMMA\t%s\t%u\t%u\n", linguae.codices[l_i],
                   summa_habentia[l_i], summa_omnia);
        }
    }
    alioquin
    {
        printf("%-*s", (integer)NG_COLUMNA, "SUMMA");
        per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
        {
            character fractio[XXXII];

            sprintf(fractio, "%u/%u", summa_habentia[l_i],
                    summa_omnia);
            printf("%-10s", fractio);
        }
        printf("\n");
    }

    /* ---- carentia et gemina nominatim (index operum) ---- */

    lacunae = ZEPHYRUM;
    per (l_i = ZEPHYRUM; l_i < linguae.numerus; l_i++)
    {
        per (m = ZEPHYRUM; m < xar_numerus(bib->exemplaria); m++)
        {
            NaturaExemplar* ex;

            ex = *(NaturaExemplar**)xar_obtinere(bib->exemplaria,
                                                 m);
            per (g_i = ZEPHYRUM;
                 g_i < xar_numerus(bib->genera_omnia); g_i++)
            {
                NaturaGenus* g;
                i32          quot;

                g = *(NaturaGenus**)xar_obtinere(
                        bib->genera_omnia, g_i);
                si (!chorda_aequalis(*g->modulus, *ex->stirps))
                {
                    perge;
                }
                quot = _genus_linguae_quot(g,
                           linguae.codices[l_i]);
                si (quot == ZEPHYRUM)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("CARENS\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("CARENS %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
                alioquin si (quot > I)
                {
                    lacunae++;
                    si (modus_machina)
                    {
                        printf("GEMINA\t");
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("\t");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\t%s\n", linguae.codices[l_i]);
                    }
                    alioquin
                    {
                        printf("GEMINA %s  ",
                               linguae.codices[l_i]);
                        _columnam_scribere(stdout, g->modulus,
                                           ZEPHYRUM);
                        printf("/");
                        _columnam_scribere(stdout, g->titulus,
                                           ZEPHYRUM);
                        printf("\n");
                    }
                }
            }
        }
    }

    /* via_html: opus V (_paginam_scribere) - hoc loco nondum */

    si (modus_porta && lacunae > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_glossae: PORTA - lacunae %u\n", lacunae);
        redde I;
    }
    redde ZEPHYRUM;
}
```

(Adjust `printf("%-*s", (integer)NG_COLUMNA, ...)` — NG_COLUMNA is a Roman-numeral int macro; if `%-*s` with computed width fights `-Wformat`, use a literal `"%-24s"`. `_columnam_scribere(..., ZEPHYRUM)` = no padding, raw chorda.)

- [ ] **Step 5: Third link line in natura_struere.sh**

After the natura_canones link block:

```bash
clang "${GCC_FLAGS[@]}" "${INCLUDE_FLAGS[@]}" \
    "$SCRIPT_DIR/natura_glossae.c" \
    $obj_files -o "$RADIX_DIR/bin/natura_glossae" || exit 1
echo "bin/natura_glossae paratum" >&2
```

(The shared FONTES closure already contains everything needed: natura canon stml chorda piscina xar internamentum filum iter_directoria...)

- [ ] **Step 6: Build and go green**

Run: `./tools/natura_struere.sh` → three binaries.
Run: `./silva/examen.sh tools/natura_glossae.c` → cold authority, 0 vitia.
Run: `./compile_tests.sh natura_glossae` → PASS (sections I-IV; section V arrives in Task 5).

- [ ] **Step 7: Wrapper**

`tools/natura_glossae.sh`:

```bash
#!/bin/bash

# tools/natura_glossae.sh - census glossarum (spec glossae par. 5)
#
#   sine argumentis   relatio (tabula + CARENS/GEMINA nominatim)
#   -pagina           natura/cocta/glossae.html regenerare
#   -probare          paginam conferre solum: 0 recens, 1 RANCIDA,
#                     2 defectus instrumenti
#   cetera            recta ad bin/natura_glossae
#     (-porta -machina -linguae CSV -radix DIR -html VIA)

set -u
cd "$(dirname "$0")/.." || exit 2

PORTA=bin/natura_glossae
PAGINA=natura/cocta/glossae.html
STRUCTOR=./tools/natura_struere.sh

if [ ! -x "$PORTA" ]; then
    echo "natura_glossae: $PORTA abest - strue: $STRUCTOR" >&2
    exit 2
fi
for _f in tools/natura_glossae.c lib/natura.c include/natura.h \
          lib/stml.c include/stml.h; do
    if [ ! -e "$_f" ]; then
        echo "natura_glossae: custos '$_f' ABEST" >&2
        exit 2
    fi
    if [ "$_f" -nt "$PORTA" ]; then
        echo "natura_glossae: $PORTA STALUS ($_f recentior) - strue: $STRUCTOR" >&2
        exit 2
    fi
done

case "${1-}" in
-pagina)
    exec "$PORTA" -html "$PAGINA"
    ;;
-probare)
    TMP=build/glossae_probandum.$$.html
    trap 'rm -f "$TMP"' EXIT
    "$PORTA" -html "$TMP" >/dev/null || exit 2
    [ -s "$TMP" ] || { echo "natura_glossae: pagina vacua" >&2; exit 2; }
    if ! cmp -s "$TMP" "$PAGINA"; then
        echo "natura_glossae: RANCIDA $PAGINA - regenera: ./tools/natura_glossae.sh -pagina" >&2
        exit 1
    fi
    echo "natura_glossae: pagina recens"
    exit 0
    ;;
esac

exec "$PORTA" "$@"
```

`chmod +x tools/natura_glossae.sh`. (`-pagina`/`-probare` become functional in Task 5; they pass through to `-html` which Task 4's binary already parses — before Task 5 it just doesn't write, which is fine because Task 5 lands before the wrapper is advertised.)

- [ ] **Step 8: Run on the real corpus, then commit**

Run: `./tools/natura_glossae.sh | tail -5` → SUMMA row (expect la 151/177, en 1/177, fr 1/177 — record actuals in the commit message).

```bash
git add tools/natura_glossae.c tools/natura_glossae.sh \
        tools/natura_struere.sh \
        probationes/probatio_natura_glossae.c \
        probationes/exempla/gl_plenum/probatio.genera \
        probationes/exempla/gl_lacunosum/probatio.genera \
        compile_tests_fontes_generata.sh
git commit -m "glossae: census (bin/natura_glossae) - copertura la/en/fr, -porta

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 5: HTML — encyclopedia + gaps fused

**Files:**
- Modify: `tools/natura_glossae.c` (`_paginam_scribere` + helpers; call at `via_html` site)
- Create: `natura/cocta/glossae.html` (generated, committed)
- Test: `probationes/probatio_natura_glossae.c` (section V)

**Interfaces:**
- Consumes: same bibliotheca + `NgLinguae`; `genus->parens` chain for lineage.
- Produces: deterministic single-file page (NO timestamps — `-probare` byte-compares, house law "porta OUTPUT confert, numquam tempora"). Page contains for every genus: name, lineage, `la` entry (definitio) or gap, each vernacular glossa or `⚠ deest` gap.

- [ ] **Step 1: Write the failing sentinels (section V)**

```c
    /* ---- V. pagina html: encyclopaedia + lacunae fusae ---- */
    {
        ProcessusResultus r;
        chorda            pagina;

        imprimere("\n--- V. pagina html ---\n");

        r = _censum_currere("probationes/exempla/gl_lacunosum",
                            "-html", "build/probatio_glossae.html",
                            NIHIL, piscina);
        CREDO_VERUM (r.successus);
        CREDO_AEQUALIS_I32 ((i32)r.codex_exitus, ZEPHYRUM);

        pagina = filum_legere_totum("build/probatio_glossae.html",
                                    piscina);
        CREDO_MAIOR_I32 (pagina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(pagina, "<!DOCTYPE html>",
                                       piscina));
        /* glossa vera transit */
        CREDO_VERUM (_continet_literis(pagina,
            "Gap kind: French missing", piscina));
        /* lacuna VISIBILIS - pagina index operum est */
        CREDO_VERUM (_continet_literis(pagina, "deest", piscina));
        /* evasio: '<' fixturae iterum evasum exit */
        CREDO_VERUM (_continet_literis(pagina,
            "&lt;markup&gt;", piscina));

        /* artefactum verum commissum: sententia stabilis plantae */
        pagina = filum_legere_totum("natura/cocta/glossae.html",
                                    piscina);
        CREDO_MAIOR_I32 (pagina.mensura, (i32)ZEPHYRUM);
        CREDO_VERUM (_continet_literis(pagina, "photosynthesis",
                                       piscina));
    }
```

- [ ] **Step 2: Verify RED**

Run: `./compile_tests.sh natura_glossae` → section V FAILS (`-html` parsed but writes nothing; committed page absent).

- [ ] **Step 3: Implement `_paginam_scribere`**

Helpers in tools/natura_glossae.c (+ forward decls):

```c
/* textus in html: <, >, & evasa; cetera verbatim (UTF-8 transit) */
interior vacuum
_html_textum_scribere(
    FILE*   f,
    chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        character c;

        c = (character)t.datum[i];
        si (c == '<')          { fputs("&lt;", f); }
        alioquin si (c == '>') { fputs("&gt;", f); }
        alioquin si (c == '&') { fputs("&amp;", f); }
        alioquin               { putc(c, f); }
    }
}

/* catena parentum a radice ad genus: 'vivens &rarr; planta' */
interior vacuum
_lineam_scribere(
    FILE*         f,
    NaturaGenus*  g)
{
    NaturaGenus* catena[XXXII];
    i32          n;
    i32          i;
    NaturaGenus* cursor;

    n = ZEPHYRUM;
    per (cursor = g; cursor && n < (i32)XXXII;
         cursor = cursor->parens)
    {
        catena[n] = cursor;
        n++;
    }
    per (i = n; i > ZEPHYRUM; i--)
    {
        _html_textum_scribere(f, *catena[i - I]->titulus);
        si (i > I)
        {
            fputs(" &rarr; ", f);
        }
    }
}
```

`_paginam_scribere(NaturaBibliotheca* bib, constans NgLinguae* linguae, constans character* via, Piscina* piscina)` returning `b32`:

1. `fopen(via, "w")`; on NIHIL → FALSUM (caller prints and `redde II`).
2. Head (fputs lines; palette lifted from natura_visus.sh:364-393):

```c
    fputs("<!DOCTYPE html>\n"
          "<html lang=\"la\"><head><meta charset=\"utf-8\">\n"
          "<title>Glossae - documentatio generum</title>\n"
          "<style>\n"
          " body { font-family: Menlo, monospace; margin: 2rem auto;\n"
          "        max-width: 72rem; background: #14120f;\n"
          "        color: #d8d0c0; line-height: 1.5; }\n"
          " h1 { color: #e8c878; font-size: 1.4rem; }\n"
          " h2 { color: #b8a878; border-bottom: 1px solid #3a352c;\n"
          "      padding-bottom: .3rem; margin-top: 2rem;\n"
          "      font-size: 1.1rem; }\n"
          " h3 { color: #d8d0c0; margin-bottom: .1rem;\n"
          "      font-size: 1rem; }\n"
          " .linea { color: #8a8272; font-size: .85rem;\n"
          "          margin: 0 0 .3rem 0; }\n"
          " .def { color: #c8bfa8; margin: .2rem 0; }\n"
          " .glossa { margin: .2rem 0; }\n"
          " .glossa b { color: #e8c878; margin-right: .5rem; }\n"
          " .deest { color: #c86060; margin: .2rem 0; }\n"
          " .numeri { color: #8a8272; }\n"
          " .numeri b { color: #e8c878; }\n"
          "</style></head><body>\n"
          "<h1>GLOSSAE - documentatio generum</h1>\n", f);
```

3. Summary line: recompute totals with `_genus_linguae_quot` (same loops as the report) and emit `<p class="numeri">la <b>151/177</b> &middot; en <b>1/177</b> ...</p>` via fprintf `%u/%u`.
4. Per exemplar (skip those with zero own genera): `<h2>` stirps escaped, then per-lingua fractions in the same h2 (`fprintf(f, " <span class=\"numeri\">%s %u/%u</span>", ...)`).
5. Per genus of the module: `<h3>` name; `<p class="linea">` lineage via `_lineam_scribere`; then:
   - `la`-style entry: `stml_invenire_liberum(g->nodus, "definitio")` → `<p class="def">` normalized+escaped, else `<p class="deest"><b>la</b> &#9888; deest</p>` (only when `la` is in the demand-list);
   - each vernacular in the demand-list: find the matching glossa child (same loop as `_genus_linguae_quot`, first match), `<p class="glossa"><b>en</b> text</p>` or `<p class="deest"><b>fr</b> &#9888; deest</p>`.
6. Footer: `<p class="numeri">generata a natura_glossae - regenera: ./tools/natura_glossae.sh -pagina</p></body></html>` — **no date, no timestamps** (determinism for `-probare`).
7. `fclose` judged: nonzero → `remove(via)`, FALSUM (mirror natura_canones.c:1470).

Call site in `principale`, replacing the Task 4 placeholder comment:

```c
    si (via_html)
    {
        si (!_paginam_scribere(bib, &linguae, via_html, piscina))
        {
            fprintf(stderr,
                "natura_glossae: pagina '%s' scribi nequit\n",
                via_html);
            redde II;
        }
    }
```

- [ ] **Step 4: Build, generate the real page, go green**

Run: `./tools/natura_struere.sh` && `./silva/examen.sh tools/natura_glossae.c`.
Run: `./tools/natura_glossae.sh -pagina` → writes natura/cocta/glossae.html.
Run: `./tools/natura_glossae.sh -probare` → exit 0 (page fresh).
Run: `./compile_tests.sh natura_glossae` → PASS (all five sections).
Open the page once (`open natura/cocta/glossae.html`) — eyeball planta's entry and a gap marker.

- [ ] **Step 5: Commit**

```bash
git add tools/natura_glossae.c probationes/probatio_natura_glossae.c \
        natura/cocta/glossae.html
git commit -m "glossae: pagina html - encyclopaedia et lacunae fusae

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

### Task 6: Documentation + records

**Files:**
- Modify: `natura/METAMODULUS.md` (format doc learns glossa)
- Modify: `natura/natura.worklog.md` (grammar + seed entry)
- Modify: `tools/natura_canones.worklog.md` (emit-through + piscina threading entry)

**Interfaces:** none (prose).

- [ ] **Step 1: METAMODULUS**

Locate the section describing genus children (grep `definitio` in natura/METAMODULUS.md); add beside it:

```
GLOSSA (spec glossae, 2026-08-07): `<glossa lingua="en">` filius
generis - documentatio vernacula, codices ISO 639-1. `<definitio>`
Latine VERITAS manet ('la' in censu); glossa explicat, numquam
renominat. Vocabularium: natura/natura.canon; emissio in canones
coctos: natura_canones; copertura: ./tools/natura_glossae.sh
(-porta = porta, -pagina = natura/cocta/glossae.html).
```

- [ ] **Step 2: Worklog entries** (English fine; include the two grounding surprises: loader/canon needed zero code — hybrid-depth design and the no-`nomen=` skip both held; and the piscina threading being the whole projection diff besides `_glossas_scribere`.)

- [ ] **Step 3: Commit**

```bash
git add natura/METAMODULUS.md natura/natura.worklog.md \
        tools/natura_canones.worklog.md
git commit -m "glossae: documentatio formae (METAMODULUS) + worklogs

Co-Authored-By: Claude Fable 5 <noreply@anthropic.com>"
```

---

## Named deferrals (spec §9, unchanged)

Species/proprietates surface extension; demand-list config home; `natura_visus.sh` graduation of `-porta`; glossing hand-written canons in practice; librarium rendering. MAP.txt update stays deferred to /update-map.

## Self-review notes

- Spec coverage: §2→T1/T2, §3→T1 (with the two grounded simplifications recorded in the header), §4→T3, §5→T4, §6→T5, §7→oracles in every task, §8-9→T6/deferrals.
- Type consistency: `_canonem_emittere` 6-arg form used in T3 only; `_genus_linguae_quot`/`NgLinguae` shared by T4 report and T5 page; sentinel strings `photosynthesis`/`Gap kind: French missing`/`&lt;markup&gt;` consistent across fixtures and assertions.
- Known soft spots for the implementer to verify on contact: exact local piscina names at the two `_canonem_emittere` call sites; `%-*s` vs literal-width under `-Wformat`; the self-build block line range (probatio_natura_canones.c:845-882); METAMODULUS section placement.
