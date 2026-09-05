# briar — plan 2: fabrica (silva in the nexus, silex promotion, headless fabrica)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task, inline, in the worktree `../rhubarb-briar` on branch `rhubarb-briar`. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** From a parsed `.thistle` document, produce a complete silex-shaped project directory (sources cut at silva unit extents with `#line` maps, a generated header of prototypes, a separate probatio unit, assets, closure, build scripts) and its cache key, with no clang run in the suite. Plan 3 wraps this in the `briar` binary.

**Architecture:** Three layers, each with its gate. (1) The nexus grows a fourth inner kind: every `<c!>` region is parsed by silva with macro expansion, headers fed by text from the silex corpus, never from disk; a prelude of `#include "latina.h"` (plus `internuntius.h` and a reference handler declaration for `methodus=` regions) is prepended and subtracted from every reported line. (2) silex promotes its four build-script generators and its closure-from-texts helper to public API, each script generator gaining a source-list parameter; output for silex's own calls is byte-identical (proved by diffing the scripts its gate writes before and after). (3) `briar_fabrica` walks the nexus: main rule, unit partition through silva's tree and symbol table, generated files, closure, scripts, key; a writer puts the project on disk; a shell tool wraps it for hand runs.

**Tech Stack:** C89 in house Latin; materia (briar's outer tree), silva amalgam (`silva/amalgama/silva.c` as one object, header `silva/amalgama/silva.h`), silex (`lib/silex.c`) with its link closure including `vendor/sqlite3.c`, sigillum, filum, credo.

**Spec:** `project-specs/briar-spec.md` v1.1 — §3.3 (silva inner kind), §4 (fabrica), §4.4 (silex changes), §6 (gates), §8 (P2 remainder + P3). This plan amends §4.2 (partition rule), §4.4 (source-list parameter; toml generator stays static), and §3.3 (prelude + reference handler) — Task 6 writes the amendments.

## Global Constraints

- **Worktree:** all work in `/Users/francisarant/Documents/projects/rhubarb-briar` (branch `rhubarb-briar`). Never run a tabularii/fori resident there; never `silva.umbrae_purgare()` there.
- **New C files:** `./silva/scribe.sh <via> <<'EOF' … EOF` (the `~/.bin/scribe` symlink refuses worktree paths). Exit 0 ACCIPE, 1 REICE (file kept — fix it), 3 RECUSO.
- **Edits to existing C:** the Edit tool; examen judges at commit (pre-commit). Before each commit run `./silva/formator.sh <changed .c/.h> -scribere` then `./silva/formator.sh <same> -vitia` and expect 0.
- **Commit rule:** from the worktree, `python3 - <<'EOF'` with `sys.path.insert(0,'pythonica'); import silva; silva.commissio_umbra(msg, viae, portae)`. Gates per task are named in the task. Never gate→commit in bash. No escaped quotes inside commit messages.
- **Forbidden identifiers:** every latina.h word (`nomen`, `casus`, `registrum`, `magnitudo`, `structura`, `brevis`, `exire`, …); single capitals `I V X L C D M` are numerals. No `//`, no declaration after statement, no variadic macros, no `_Bool`.
- **chorda is NOT NUL-terminated; i32/i64 unsigned, s32/s64 signed.**
- **POSIX .c files** (`chmod`, `stat`) put `#include "postulata_posix.h"` FIRST (codex 85).
- **Fixtures under `probationes/fixa/` are exempt from examen** (pre-commit excludes `fixa/`), so goldens may contain `#line`.
- **Every gate is born red by a planted fault** and the plant is reverted before commit.
- **Flags:** `tools/vexilla.sh` (`VEXILLA_C89`), sourced by the runner; never a flag array in a script.
- **Planted faults must be `volatilis` when the optimizer could delete them** (not needed for the plants below — they are structural).
- **Each commit message** records the gate count (`BRIAR n/n`).

## File structure

| path | role |
|---|---|
| `briar/compile_probationes.sh` | MODIFY: link closure grows by silex's sources, `vendor/sqlite3.c`, the silva amalgam object; `-I…/silva/amalgama` |
| `briar/fontes/briar_nexus.h/.c` | MODIFY: silva fields on `BriarNexusRes`; `briar_nexus_silvam_texere`, `briar_nexus_silvam_solvere`, `briar_nexus_linea_silvae` |
| `briar/probationes/probatio_briar_silva.c` | CREATE: gate for the silva inner kind |
| `briar/probationes/fixa/thistle/adversa/c_fractum.thistle` | CREATE: syntax error at a known line |
| `include/silex.h`, `lib/silex.c` | MODIFY: promotion (Task 2) |
| `briar/fontes/briar_fabrica.h/.c` | CREATE: inventory, main rule, partition, generation, closure, scripts, key, writer |
| `briar/probationes/probatio_briar_fabrica.c` | CREATE: goldens + structural assertions + refusals |
| `briar/probationes/fixa/thistle/punctum.thistle` | CREATE: typedef + global + `#define` + helper — the partition fixture |
| `briar/probationes/fixa/fabrica/<t>/…` | CREATE: goldens (generated files, byte-exact) |
| `briar/probationes/fixa/thistle/adversa/{duo_principalia,nihil,fenestra_et_principale,methodus_absens,methodus_prava,html_duplex,fenestra_prava}.thistle` | CREATE: refusal fixtures |
| `briar/instrumenta/fabrica.c`, `briar/fabrica.sh` | CREATE: hand tool — thistle → project dir |
| `briar/CLAUDE.md`, `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md`, `project-specs/briar-spec.md` | MODIFY: docs + amendments |

---

### Task 1: The silva inner kind

**Files:**
- Modify: `briar/compile_probationes.sh`
- Modify: `briar/fontes/briar_nexus.h`, `briar/fontes/briar_nexus.c`
- Create: `briar/probationes/fixa/thistle/adversa/c_fractum.thistle`
- Test: `briar/probationes/probatio_briar_silva.c`

**Interfaces:**
- Consumes: `briar_nexus_texere` (plan 1), `silex_fons_disci` / `silex_clausuram_e_contentis` (the latter becomes public in Task 2 — until then this task calls `silex_clausuram_colligere` through the seed scan below; see Step 4 note), silva amalgam: `silva_piscina_generare_dynamicum`, `silva_expansio_creare`, `silva_includendum_praebere`, `silva_parsare_cum_expansione`, `SILVA_C89_GRAMMATICA`, `silva_c89_semantica_analysare`, `silva_nodus_liberi`, `silva_nodus_extensionem_lineis`, `SILVA_C89_GENUS_ERROR`.
- Produces: on `BriarNexusRes`: `SilvaPiscina* silva_piscina; constans SilvaParsura* parsura; SilvaSemantica* semantica; chorda textus_silvae; i32 praeludium; i32 praeludium_octeti;` and
  ```c
  i32  briar_nexus_silvam_texere (Piscina*, Xar* nexus, constans SilexFons* fons);  /* regiones parsatae; -I = memoria */
  vacuum briar_nexus_silvam_solvere (Xar* nexus);
  i32  briar_nexus_linea_silvae (constans BriarNexusRes*, i32 linea_silvae);       /* linea .thistle */
  ```

- [ ] **Step 1: Grow the runner's link closure**

In `briar/compile_probationes.sh`:

1. Add to `INCLUDE_FLAGS`: `"-I$RADIX_DIR/silva/amalgama"`.
2. Extend `RADIX_FONTES` with silex's closure (from `bin/aedilis lib/silex.c --partes`, measured 2026-09-05):
   ```bash
   "filum" "via" "processus_posix" "iter_directoria" "sigillum" "json"
   "capsula" "differentia" "scrinium" "flatura" "moneta" "fasti" "volumen"
   "silex"
   ```
3. After the `RADIX_FONTES` loop, add the vendor object (flags copied from `compile_tests.sh` lines 47–56 — the same set as `SILEX_VEXILLA_VENDITORIA`):
   ```bash
   # venditorius: sqlite3 (volumen -> silex); vexilla compile_tests.sh
   declare -a VENDOR_FLAGS=("-O2" "-DSQLITE_ENABLE_FTS5" "-DSQLITE_THREADSAFE=0"
       "-DSQLITE_DQS=0" "-DSQLITE_DEFAULT_MEMSTATUS=0" "-DSQLITE_OMIT_LOAD_EXTENSION"
       "-DSQLITE_OMIT_DEPRECATED" "-DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1")
   src="$RADIX_DIR/vendor/sqlite3.c"
   obj="$BUILD_DIR/sqlite3.o"
   if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
       echo "  [vendor] sqlite3.c"
       if ! clang "${VENDOR_FLAGS[@]}" -c "$src" -o "$obj"; then
           echo "FRACTA: sqlite3.c" ; exit 1
       fi
   fi
   obj_files="$obj_files $obj"

   # silva: amalgama ut obiectum UNUM (officina exemplar) - fontes silvae absunt
   src="$RADIX_DIR/silva/amalgama/silva.c"
   obj="$BUILD_DIR/amalgama_silva.o"
   if [ ! -f "$obj" ] || ! [ "$obj" -nt "$src" ]; then
       echo "  [amalgama] silva.c"
       if ! clang "${GCC_FLAGS[@]}" -c "$src" -o "$obj"; then
           echo "FRACTA: amalgama silva" ; exit 1
       fi
   fi
   obj_files="$obj_files $obj"
   ```
4. In the `CAPUT_RECENS` `find`, add `"$RADIX_DIR/silva/amalgama"` so an amalgam header change retires briar objects.

Run: `time ./briar/compile_probationes.sh registrum`
Expected: `[vendor] sqlite3.c`, `[amalgama] silva.c`, then `BRIAR PROBATIONES: 1/1`. Record the cold time in the worklog (Task 6). A second run must show neither `[vendor]` nor `[amalgama]`.

- [ ] **Step 2: The failing gate**

Create `briar/probationes/fixa/thistle/adversa/c_fractum.thistle` (the `@@@` line must be the first line of a top-level unit — a blank line separates it from the closed function above):

```
#!/usr/bin/env briar
# Fractum

<c!>
s32
principale (vacuum)
{
    redde ZEPHYRUM;
}

@@@ fractum @@@
</c>
```

Write `briar/probationes/probatio_briar_silva.c` with `./silva/scribe.sh`:

```c
/* probatio_briar_silva.c - Genus interius quartum: regiones C per
 * silvam cum expansione (praeludium latina.h; capita e fonte silicis,
 * numquam e disco). Probat: 'principale' -> symbolum 'main' (parsura
 * nuda id non daret), regio probationis parsata, methodus cum
 * exemplari tractatoris compatibilis, linea erroris in lineam
 * .thistle translata.
 */

#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_nexus.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "silva.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans character* SALVE =
    "briar/probationes/fixa/thistle/salve.thistle";
hic_manens constans character* VITREUM =
    "briar/probationes/fixa/thistle/salve_vitreum.thistle";
hic_manens constans character* FRACTUM =
    "briar/probationes/fixa/thistle/adversa/c_fractum.thistle";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

/* linea (I-basata) in qua 'acus' primum incipit in columna 0 */
interior i32
_linea_ubi (
    constans character* fons,
                   i32  mensura,
    constans character* acus)
{
    i32 m      = (i32)strlen(acus);
    i32 linea  = I;
    i32 i;

    per (i = ZEPHYRUM; i + m <= mensura; i++)
    {
        si (   (i == ZEPHYRUM || fons[i - I] == '\n')
            && memcmp(fons + i, acus, (size_t)m) == ZEPHYRUM)
        {
            redde linea;
        }
        si (fons[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde ZEPHYRUM;
}

interior SilvaChorda
_silva_chorda (
               Piscina* piscina,
    constans character* literae)
{
    SilvaChorda s;
         chorda c = chorda_ex_literis(literae, piscina);

    s.mensura = (insignatus integer)c.mensura;
    s.datum   = (insignatus character*)c.datum;
    redde s;
}

/* regio C n-ta (0-basata) inter partes nexus, aut NIHIL */
interior BriarNexusRes*
_regio_c (
    Xar* nexus,
    i32  quota)
{
    i32 i;
    i32 visa = ZEPHYRUM;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->genus == BRIAR_NEXUS_REGIO && briar_nexus_titulus_est(r, "c"))
        {
            si (visa == quota)
            {
                redde r;
            }
            visa = visa + I;
        }
    }
    redde NIHIL;
}

interior Xar*
_texere_omnia (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans SilexFons* fons,
    constans character* via,
            character** textus_out,
                   i32* mensura_out)
{
       character* textus;
              i32 mensura = ZEPHYRUM;
    MateriaNodus* doc;
             Xar* nexus;

    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        redde NIHIL;
    }
    doc   = briar_arbor_parsare(piscina, textus, mensura);
    nexus = briar_nexus_texere(piscina, doc, intern);
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    si (briar_nexus_silvam_texere(piscina, nexus, fons) < ZEPHYRUM)
    {
        redde NIHIL;
    }
    *textus_out  = textus;
    *mensura_out = mensura;
    redde nexus;
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_silva",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    radix  = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans salve: principale -> main, probatio parsata ---\n");
    {
        character* textus;
               i32 mensura;
              Xar* nexus = _texere_omnia(piscina, intern, fons, SALVE,
                  &textus, &mensura);
        BriarNexusRes* r;
        SemanticaSymbolum* s;

        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->parsura);
        CREDO_NON_NIHIL (r->semantica);
        CREDO_AEQUALIS_I32 (r->parsura->numerus_errorum, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->linea_erroris, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->praeludium, I);
        /* expansio: 'principale' est macro -> symbolum 'main' */
        s = silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "main"));
        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_S32 ((s32)s->genus, (s32)SYMBOLUM_FUNCTIO);
        CREDO_VERUM (silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "principale")) == NIHIL);
        /* linea silvae II (post praeludium) = linea prima contenti */
        CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(r, II),
            r->linea_initium);

        /* regio probationis etiam parsata (credo.h e fonte) */
        r = _regio_c(nexus, I);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->parsura);
        CREDO_AEQUALIS_I32 (r->parsura->numerus_errorum, ZEPHYRUM);
        CREDO_NON_NIHIL (silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "main")));
        CREDO_NON_NIHIL (silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "credo_aperire")));
        briar_nexus_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans salve_vitreum: methodus cum exemplari tractatoris ---\n");
    {
        character* textus;
               i32 mensura;
              Xar* nexus = _texere_omnia(piscina, intern, fons, VITREUM,
                  &textus, &mensura);
        BriarNexusRes* r;
        SemanticaSymbolum* s;
        SemanticaSymbolum* exemplar;

        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_VERUM (briar_nexus_attributum_habet(r, "methodus"));
        CREDO_AEQUALIS_I32 (r->praeludium, III);
        CREDO_AEQUALIS_I32 (r->parsura->numerus_errorum, ZEPHYRUM);
        s = silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "salve"));
        CREDO_NON_NIHIL (s);
        CREDO_AEQUALIS_S32 ((s32)s->genus, (s32)SYMBOLUM_FUNCTIO);
        exemplar = silva_c89_symbolum_invenire(r->semantica,
            _silva_chorda(piscina, "briar_tractator_exemplar"));
        CREDO_NON_NIHIL (exemplar);
        CREDO_AEQUALIS_S32 ((s32)exemplar->typus->genus,
            (s32)TYPUS_C89_MONSTRATOR);
        CREDO_AEQUALIS_S32 (
            (s32)exemplar->typus->datum.monstrator.internum->genus,
            (s32)TYPUS_C89_FUNCTIO);
        CREDO_VERUM (silva_c89_typi_compatibiles(
            exemplar->typus->datum.monstrator.internum, s->typus));
        CREDO_AEQUALIS_I32 (briar_nexus_linea_silvae(r, IV),
            r->linea_initium);
        briar_nexus_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans c_fractum: linea erroris in .thistle ---\n");
    {
        character* textus;
               i32 mensura;
              Xar* nexus = _texere_omnia(piscina, intern, fons, FRACTUM,
                  &textus, &mensura);
        BriarNexusRes* r;
               i32 linea;

        CREDO_NON_NIHIL (nexus);
        r = _regio_c(nexus, ZEPHYRUM);
        CREDO_NON_NIHIL (r);
        CREDO_NON_NIHIL (r->parsura);
        CREDO_VERUM (r->parsura->numerus_errorum > ZEPHYRUM);
        linea = _linea_ubi(textus, mensura, "@@@");
        CREDO_VERUM (linea > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (r->linea_erroris, linea);
        CREDO_VERUM (r->causa.mensura > ZEPHYRUM);
        briar_nexus_silvam_solvere(nexus);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

Run: `./briar/compile_probationes.sh silva`
Expected: FRACTA (compilatio) — `briar_nexus_silvam_texere` undeclared.

- [ ] **Step 3: Nexus header**

In `briar/fontes/briar_nexus.h`: add includes `#include "silva.h"` and `#include "silex.h"` after `stml.h`; extend the head comment with a paragraph: *"Genus quartum (plan 2): regio 'c' per silvam cum expansione — praeludium `#include "latina.h"` (+ `internuntius.h` + `hic_manens InternuntiusTractator briar_tractator_exemplar;` si methodus=) textui praepositum; capita clausurae e fonte silicis praebita; arbor + semantica in `silva_piscina` propria (solvere!). Lineae silvae per `briar_nexus_linea_silvae` in .thistle."*; add to `BriarNexusRes` after `causa`:

```c
             SilvaPiscina* silva_piscina;    /* C: arena silvae; NIHIL alias */
    constans SilvaParsura* parsura;          /* C: NIHIL si non parsata */
           SilvaSemantica* semantica;        /* C: symbola + typi */
                   chorda  textus_silvae;    /* C: praeludium + contentum */
                      i32  praeludium;       /* lineae praepositae */
                      i32  praeludium_octeti;
```

and the three prototypes after `briar_nexus_titulus_est`:

```c
/* Regiones 'c' (quocumque munere) per silvam parsare: praeludium
 * praepositum, capita clausurae e fonte (numquam discus), expansio
 * obligatoria (parsura nuda C domesticum male legit). Reddit numerum
 * regionum parsatarum; -I = memoria aut argumenta. Regio cum erroribus
 * parsurae: linea_erroris (linea .thistle erroris primi) + causa;
 * arbor tamen manet. */
i32
briar_nexus_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
    constans SilexFons* fons);

/* arenas silvae destruere (parsura/semantica deinde NIHIL) */
vacuum
briar_nexus_silvam_solvere (
    Xar* nexus);

/* linea .thistle ex linea fontis silvae (praeludio dempto) */
i32
briar_nexus_linea_silvae (
    constans BriarNexusRes* res,
                       i32  linea_silvae);
```

- [ ] **Step 4: Nexus implementation**

In `briar/fontes/briar_nexus.c`, initialize the new fields in `_res_incipere` (all NIHIL / `_vacua()` / ZEPHYRUM), then append:

```c
/* ==================================================
 * Genus quartum: regio C per silvam
 * ================================================== */

interior b32
_suffixum_est (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

/* nodus ERROR primus (profunditate prima): linea silvae aut 0 */
interior i32
_errorem_quaerere (
           SilvaPiscina* sp,
    constans SilvaNodus* nodus,
                integer  fons_index)
{
              SilvaXar* liberi;
    insignatus integer  k;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (nodus->genus == (integer)SILVA_C89_GENUS_ERROR)
    {
        insignatus integer la = ZEPHYRUM;
        insignatus integer ca = ZEPHYRUM;
        insignatus integer lb = ZEPHYRUM;
        insignatus integer cb = ZEPHYRUM;

        silva_nodus_extensionem_lineis(nodus, fons_index, &la, &ca, &lb,
            &cb);
        redde (i32)la;
    }
    liberi = silva_nodus_liberi(sp, nodus);
    si (liberi == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < silva_xar_numerus(liberi); k++)
    {
        constans SilvaNodus* filius =
            *(SilvaNodus**)silva_xar_obtinere(liberi, k);
        i32 linea = _errorem_quaerere(sp, filius, fons_index);

        si (linea > ZEPHYRUM)
        {
            redde linea;
        }
    }
    redde ZEPHYRUM;
}

interior b32
_regionem_c_parsare (
              Piscina* piscina,
        BriarNexusRes* r,
    constans SilexFons* fons)
{
    ChordaAedificator* aed;
                  Xar* clausura;
        SilvaExpansio* exp;
                  i32  k;

    aed = chorda_aedificator_creare(piscina,
        (memoriae_index)(r->contentum.mensura + 256));
    si (aed == NIHIL)
    {
        redde FALSUM;
    }
    chorda_aedificator_appendere_literis(aed, "#include \"latina.h\"\n");
    r->praeludium = I;
    si (briar_nexus_attributum_habet(r, "methodus"))
    {
        chorda_aedificator_appendere_literis(aed,
            "#include \"internuntius.h\"\n"
            "hic_manens InternuntiusTractator briar_tractator_exemplar;\n");
        r->praeludium = III;
    }
    r->praeludium_octeti = (i32)chorda_aedificator_longitudo(aed);
    chorda_aedificator_appendere_chorda(aed, r->contentum);
    r->textus_silvae = chorda_aedificator_finire(aed);

    clausura = silex_clausuram_e_contentis(piscina, fons,
        &r->textus_silvae, I);
    si (clausura == NIHIL)
    {
        r->causa = chorda_ex_literis(
            "clausura capitum fracta (fons silicis)", piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    r->silva_piscina = silva_piscina_generare_dynamicum("briar_silva",
        (size_t)8388608);
    si (r->silva_piscina == NIHIL)
    {
        redde FALSUM;
    }
    exp = silva_expansio_creare(r->silva_piscina);
    per (k = ZEPHYRUM; k < xar_numerus(clausura); k++)
    {
        constans SilexRes* res = (constans SilexRes*)xar_obtinere(
            clausura, k);

        si (!_suffixum_est(res->via, ".h"))
        {
            perge;
        }
        silva_includendum_praebere(exp, chorda_ut_cstr(res->via, piscina),
            (constans character*)res->contentum.datum,
            (insignatus integer)res->contentum.mensura);
    }
    r->parsura = silva_parsare_cum_expansione(r->silva_piscina, exp,
        "regio.c", (constans character*)r->textus_silvae.datum,
        (insignatus integer)r->textus_silvae.mensura,
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (r->parsura == NIHIL || r->parsura->commissio == NIHIL)
    {
        r->parsura = NIHIL;
        r->causa   = chorda_ex_literis("silva: parsura fracta", piscina);
        r->linea_erroris = r->linea_initium;
        redde VERUM;
    }
    si (r->parsura->numerus_errorum > ZEPHYRUM)
    {
        i32 linea = _errorem_quaerere(r->silva_piscina,
            r->parsura->commissio->radix.datum.nodus,
            r->parsura->fons_princeps);
        character b[96];

        r->linea_erroris = (linea > ZEPHYRUM)
            ? briar_nexus_linea_silvae(r, linea) : r->linea_initium;
        sprintf(b, "regio C: parsura fracta (%u errores)",
            r->parsura->numerus_errorum);
        r->causa = chorda_ex_literis(b, piscina);
    }
    r->semantica = silva_c89_semantica_analysare(r->silva_piscina,
        r->parsura);
    redde VERUM;
}

i32
briar_nexus_silvam_texere (
                Piscina* piscina,
                    Xar* nexus,
    constans SilexFons* fons)
{
    i32 i;
    i32 parsatae = ZEPHYRUM;

    si (piscina == NIHIL || nexus == NIHIL || fons == NIHIL)
    {
        redde -I;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (   r->genus != BRIAR_NEXUS_REGIO
            || !briar_nexus_titulus_est(r, "c"))
        {
            perge;
        }
        si (!_regionem_c_parsare(piscina, r, fons))
        {
            redde -I;
        }
        parsatae = parsatae + I;
    }
    redde parsatae;
}

vacuum
briar_nexus_silvam_solvere (
    Xar* nexus)
{
    i32 i;

    si (nexus == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->silva_piscina != NIHIL)
        {
            silva_piscina_destruere(r->silva_piscina);
        }
        r->silva_piscina  = NIHIL;
        r->parsura        = NIHIL;
        r->semantica      = NIHIL;
    }
}

i32
briar_nexus_linea_silvae (
    constans BriarNexusRes* res,
                       i32  linea_silvae)
{
    si (res == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde res->linea_initium + linea_silvae - res->praeludium - I;
}
```

Notes for the implementer:
- `chorda_aedificator_longitudo(aed)` (chorda_aedificator.h:198) returns the bytes appended so far — the prelude byte count is read from it, never hand-counted.
- `silex_clausuram_e_contentis` is static in `lib/silex.c` until Task 2. **Do Task 2 Steps 1–4 (the promotion) BEFORE this step compiles**, or temporarily use `silex_clausuram_colligere` with the seeds `{"latina.h", "internuntius.h"}` plus the region's `#include "x.h"` names scanned by hand — do not: the promotion is one task away, so execute Task 2 first if you reach this point with it undone. Order of execution is therefore: Task 2 → Task 1 → Task 3… (Task 2 has no dependency on Task 1.) The plan keeps Task 1 first because it is the smaller conceptual step; the executor runs Task 2 first.
- `sprintf` needs `<stdio.h>`; add the include.

Run: `./briar/compile_probationes.sh silva`
Expected: `probatio_briar_silva praeteriit`; the whole suite `./briar/compile_probationes.sh` → `9/9`.

- [ ] **Step 5: Plant, see red, revert**

Plant: in `_regionem_c_parsare` comment out the `#include "latina.h"` append (keep `praeludium = I`). Run the gate. Expected RED: `main` absent (symbol lookup NIHIL) AND `c_fractum` line assertion off by one. Revert. Expected green.

- [ ] **Step 6: Commit**

`./silva/formator.sh briar/fontes/briar_nexus.c briar/fontes/briar_nexus.h briar/probationes/probatio_briar_silva.c -scribere` then `-vitia` (0). Commit via `commissio_umbra` with portae `['briar']`, viae = the four files + the fixture + `briar/compile_probationes.sh`. Message (Latin, one paragraph): silva as the fourth inner kind, prelude, exemplar tractatoris, closure from the corpus by text, the runner's closure growth with the measured cold times, plant seen red, `BRIAR IX/IX`.

---

### Task 2: silex promotion (behavior-preserving)

**Files:**
- Modify: `include/silex.h` (after `silex_clausuram_colligere`, before `silex_novum`)
- Modify: `lib/silex.c` (macros at 741–744 and 1772–1779; generators at 746–815, 1695–1760, 1908–2060; callers at 2211–2213 and 2290–2295)
- Test: `probationes/probatio_silex.c` UNCHANGED; byte-equality proof over `build/probatio_silex_area`.

**Interfaces:**
- Produces (public):
  ```c
  #define SILEX_VEXILLA_COMPILATIONIS / SILEX_VEXILLA_VITREA / SILEX_VEXILLA_VENDITORIA   (moved)
  Xar*   silex_clausuram_e_contentis (Piscina*, constans SilexFons*, constans chorda* contenta, i32 numerus);
  chorda silex_ordinem_fingere (Piscina*, constans character* titulus, constans character* constans* fontes, i32 n);
  chorda silex_ordinem_probandi_fingere (Piscina*, constans character* titulus, constans character* constans* fontes, i32 n);
  chorda silex_ordinem_vitreum_fingere (Piscina*, constans character* titulus, constans character* constans* fontes, i32 n, Xar* clausura_app, Xar* clausura_instrumenti, constans character* fons_titulus);
  chorda silex_ordinem_probandi_vitreum_fingere (Piscina*, constans character* titulus, constans character* constans* fontes, i32 n, Xar* clausura_probationis, constans character* fons_titulus);
  ```
  `_toml_fingere` STAYS static (briar's asset list is data; §4.4 amendment in Task 6).

- [ ] **Step 1: Capture the scripts silex writes today**

```bash
./compile_tests.sh silex && rm -rf "$SCRATCH/silex_ante" && cp -R build/probatio_silex_area "$SCRATCH/silex_ante"
ls "$SCRATCH/silex_ante"/specimen/*.sh "$SCRATCH/silex_ante"/vitreum/*.sh
```
(`$SCRATCH` = the session scratchpad directory.) Expected: four scripts present (`specimen/aedificare.sh`, `specimen/probare.sh`, `vitreum/aedificare.sh`, `vitreum/probare.sh`). If the area is cleaned by the test at exit, add a `-servare` sleep: instead copy inside the test run is impossible — then take the scripts from a manual `bin/silex novum` run into `$SCRATCH` (plain and `-vitrea`) before and after; the comparison method is the same.

- [ ] **Step 2: Header**

Insert into `include/silex.h` after the `silex_clausuram_colligere` prototype:

```c
/* Vexilla quae ordines generati ferunt. PUBLICA (plan 2 briar): clavis
 * fabricae briar chordam vexillorum ipsam friat - vexilla in proiecto
 * cocta in clave eius sint. */
#define SILEX_VEXILLA_COMPILATIONIS \
    "-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion" \
    " -Wsign-conversion -Wcast-qual -Wstrict-prototypes" \
    " -Wmissing-prototypes -Wwrite-strings -Wno-long-long"
#define SILEX_VEXILLA_VITREA SILEX_VEXILLA_COMPILATIONIS \
    " -Wno-overlength-strings -fbracket-depth=512"
#define SILEX_VEXILLA_VENDITORIA \
    "-O2 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0" \
    " -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0" \
    " -DSQLITE_OMIT_LOAD_EXTENSION -DSQLITE_OMIT_DEPRECATED" \
    " -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1"

/* clausura e TEXTIBUS fontium: semina = lineae '#include "x.h"'
 * contentorum (dedup; <t>_pipa.h et capsula_* omissa - capita
 * templi propria); NIHIL si fracta. Consumptor secundus: briar
 * (regiones C + principale genitum). */
Xar*
silex_clausuram_e_contentis (
               Piscina* piscina,
    constans SilexFons* fons,
       constans chorda* contenta,
                   i32  numerus);

/* Ordines generati (PROIECTIO: mutare licet). 'fontes' = viae
 * fontium proiecto relativae, ordine scripto: silex fontes/<t>.c
 * [+ fontes/<t>_pipa.c]; briar fontes/<t>.c fontes/<t>_regiones.c.
 * Plani: fontes citati + lib/*.c; vitrei: fontes + assets/capsula_<t>.c
 * + clausura explicata (numquam globus) + frameworks. */
chorda
silex_ordinem_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium);

chorda
silex_ordinem_probandi_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium);

chorda
silex_ordinem_vitreum_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium,
                           Xar* clausura_app,
                           Xar* clausura_instrumenti,
            constans character* fons_titulus);

chorda
silex_ordinem_probandi_vitreum_fingere (
                       Piscina* piscina,
            constans character* titulus,
    constans character* constans* fontes,
                           i32  numerus_fontium,
                           Xar* clausura_probationis,
            constans character* fons_titulus);
```

- [ ] **Step 3: Source**

In `lib/silex.c`:

1. Delete the two macro blocks (`SILEX_VEXILLA_COMPILATIONIS` at 741–744; `SILEX_VEXILLA_VITREA` + `SILEX_VEXILLA_VENDITORIA` at 1772–1779). The header provides them.
2. `_aedificare_sh_fingere` → `silex_ordinem_fingere`: delete the `interior` prototype, drop `interior` on the definition, add the two parameters, and replace the segment
   ```c
           "clang " SILEX_VEXILLA_COMPILATIONIS
           " -Iinclude \"fontes/");
       chorda_aedificator_appendere_literis(a, titulus);
       chorda_aedificator_appendere_literis(a, ".c\" lib/*.c -o \"bin/");
   ```
   with
   ```c
           "clang " SILEX_VEXILLA_COMPILATIONIS
           " -Iinclude");
       per (i = ZEPHYRUM; i < numerus_fontium; i++)
       {
           chorda_aedificator_appendere_literis(a, " \"");
           chorda_aedificator_appendere_literis(a, fontes[i]);
           chorda_aedificator_appendere_literis(a, "\"");
       }
       chorda_aedificator_appendere_literis(a, " lib/*.c -o \"bin/");
   ```
   (declare `i32 i;` at the top).
3. `_probare_sh_fingere` → `silex_ordinem_probandi_fingere`: same edit on its segment (`" -Iinclude \"probationes/probatio_"` + titulus + `".c\" lib/*.c -o \"bin/probatio_"` → `" -Iinclude"` + loop + `" lib/*.c -o \"bin/probatio_"`).
4. `_aedificare_vitreum_fingere` → `silex_ordinem_vitreum_fingere`: replace
   ```c
           "clang $VEXILLA -Iinclude -Iassets \\\n"
           "    fontes/");
       chorda_aedificator_appendere_literis(a, titulus);
       chorda_aedificator_appendere_literis(a, ".c fontes/");
       chorda_aedificator_appendere_literis(a, titulus);
       chorda_aedificator_appendere_literis(a,
           "_pipa.c \\\n    assets/capsula_");
   ```
   with
   ```c
           "clang $VEXILLA -Iinclude -Iassets \\\n"
           "    ");
       per (i = ZEPHYRUM; i < numerus_fontium; i++)
       {
           si (i > ZEPHYRUM)
           {
               chorda_aedificator_appendere_literis(a, " ");
           }
           chorda_aedificator_appendere_literis(a, fontes[i]);
       }
       chorda_aedificator_appendere_literis(a,
           " \\\n    assets/capsula_");
   ```
5. `_probare_vitreum_fingere` → `silex_ordinem_probandi_vitreum_fingere`: replace
   ```c
           "clang $VEXILLA -Iinclude \\\n"
           "    probationes/probatio_");
       chorda_aedificator_appendere_literis(a, titulus);
       chorda_aedificator_appendere_literis(a, ".c fontes/");
       chorda_aedificator_appendere_literis(a, titulus);
       chorda_aedificator_appendere_literis(a, "_pipa.c");
   ```
   with
   ```c
           "clang $VEXILLA -Iinclude \\\n"
           "    ");
       per (i = ZEPHYRUM; i < numerus_fontium; i++)
       {
           si (i > ZEPHYRUM)
           {
               chorda_aedificator_appendere_literis(a, " ");
           }
           chorda_aedificator_appendere_literis(a, fontes[i]);
       }
   ```
6. `_clausuram_e_contentis` → `silex_clausuram_e_contentis` (delete the `interior` prototype; drop `interior`).
7. Callers in `silex_novum`:
   - plain branch (≈2211): 
     ```c
     {
         constans character* fontes_app[1];
         constans character* fontes_prob[1];

         fontes_app[0]  = _texere(piscina, "fontes/", optiones->titulus, ".c");
         fontes_prob[0] = _texere(piscina, "probationes/probatio_",
             optiones->titulus, ".c");
         _rem_genitam_addere(piscina, res_omnes, "aedificare.sh",
             silex_ordinem_fingere(piscina, optiones->titulus, fontes_app, I));
         _rem_genitam_addere(piscina, res_omnes, "probare.sh",
             silex_ordinem_probandi_fingere(piscina, optiones->titulus,
                 fontes_prob, I));
     }
     ```
     (keep the surrounding `_rem_genitam_addere` calls for the .c and README as they are; `_texere` returns a NUL-terminated string — verify its return type at line 23 and cast if it returns `character*`).
   - vitrea branch (≈2290): build `fontes_app[2] = { "fontes/<t>.c", "fontes/<t>_pipa.c" }` and `fontes_prob[2] = { "probationes/probatio_<t>.c", "fontes/<t>_pipa.c" }` with `_texere`, and pass them to the two vitrea generators.
   - the four `_clausuram_e_contentis(` calls → `silex_clausuram_e_contentis(`.

Run: `./compile_tests.sh silex`
Expected: `probatio_silex` green, unchanged assertions.

- [ ] **Step 4: Byte-equality proof**

```bash
for f in specimen/aedificare.sh specimen/probare.sh vitreum/aedificare.sh vitreum/probare.sh; do
    cmp "$SCRATCH/silex_ante/$f" "build/probatio_silex_area/$f" && echo "IDEM $f"
done
```
Expected: four `IDEM` lines. Any difference = the promotion changed output; fix before proceeding (the stamp line carries no date — if it does, compare with the stamp line stripped and say so in the commit).

- [ ] **Step 5: Whole-tree checks**

`./compile_tests.sh silex` (done), `./tools/vexilla_fumus.sh` (the macro moved between C files; the script gate must stay `sanum`), `./briar/compile_probationes.sh` (briar links the new header; expect green — 8/8 or 9/9 depending on order).

- [ ] **Step 6: Commit**

Formator on `lib/silex.c include/silex.h`; commit via `commissio_umbra` with portae `['radix', 'briar']`. Message: promotion of five generators + three macros to `silex.h`, source-list parameter, output byte-identical (four `cmp` IDEM), `_toml_fingere` deliberately static, rule of two, decree 01M098M3G6 route kept (silex stays silva-free).

---

### Task 3: Fabrica — the plain shape

**Files:**
- Create: `briar/fontes/briar_fabrica.h`, `briar/fontes/briar_fabrica.c`
- Create: `briar/probationes/fixa/thistle/punctum.thistle`
- Create: `briar/probationes/fixa/fabrica/salve/…`, `briar/probationes/fixa/fabrica/punctum/…` (goldens, generated in Step 6)
- Create: `briar/instrumenta/fabrica.c`, `briar/fabrica.sh`
- Test: `briar/probationes/probatio_briar_fabrica.c`

**Interfaces:**
- Consumes: Task 1 nexus fields; Task 2 silex API; `sigillum_incipere/addere/finire/hex`; `filum_scribere`, `filum_directorium_creare_si_necesse`, `via_directorium`; silva: `silva_nodus_liberi`, `silva_nodus_extensionem`, `silva_c89_definitio_functionis_corpus`, `silva_c89_symbola_numerus`, `silva_c89_symbolum_per_indicem`, `SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS`, `SYMBOLUM_FUNCTIO`, `SYMBOLUM_VARIABILE`.
- Produces: the header below.

- [ ] **Step 1: The partition fixture**

`briar/probationes/fixa/thistle/punctum.thistle`:

```
#!/usr/bin/env briar
# Punctum

A typedef, a macro, a global, a helper, a main — the partition has
to put each in the right file.

<c!>
#include "chorda.h"
#define PUNCTUM_MAXIMUM 3

nomen structura {
    i32 x;
    i32 y;
} Punctum;

hic_manens i32 numerus_vocationum = ZEPHYRUM;

i32
punctum_summa (constans Punctum* p)
{
    numerus_vocationum = numerus_vocationum + I;
    redde p->x + p->y;
}

s32
principale (vacuum)
{
    Punctum p;

    p.x = I;
    p.y = II;
    imprimere("summa %d\n", (integer)punctum_summa(&p));
    redde (punctum_summa(&p) <= PUNCTUM_MAXIMUM) ? ZEPHYRUM : I;
}
</c>

<c! munus="probatio">
#include "credo.h"

s32
principale (vacuum)
{
    Piscina* piscina = piscina_generare_dynamicum("probatio", 65536);
    Punctum  p;

    p.x = II;
    p.y = II;
    credo_aperire(piscina);
    CREDO_AEQUALIS_I32(punctum_summa(&p), IV);
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
</c>
```

- [ ] **Step 2: Header**

`briar/fontes/briar_fabrica.h` via scribe:

```c
/* briar_fabrica.h - Arbor thistle -> proiectum silicis (fontes ad
 * extensiones unitatum silvae secta, '#line' in .thistle, caput
 * prototyporum genitum, unitas probationis separata, clausura, ordines)
 * SINE clang. Clavis = SHA-256(stampa corporis, vexilla, octeti).
 *
 * Regula principalis (spec par. 4.3): unitas cuius declarator post
 * expansionem 'main' est = programma planum; nulla + <fenestra/> =
 * app atrii (principale genitum); duae = recusatio; neutra =
 * recusatio. Regio munus="probatio" unitas propria, extra regulam.
 *
 * Partitio (plan 2): caput <t>_regiones.h = latina.h + directivae
 * omnium regionum C (#include, #define...) + unitates typorum (typedef,
 * tags, prototypi) + prototypi functionum definitarum (textus capitis
 * definitionis + ';'); <t>_regiones.c = obiecta (variabiles scopi
 * plagulae) + definitiones praeter principale. Commenta INTER unitates
 * cadunt (trivia extensionibus aliena); commenta intra unitates manent.
 */

#ifndef BRIAR_FABRICA_H
#define BRIAR_FABRICA_H

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "materia_nodus.h"
#include "silex.h"
#include "briar_nexus.h"

nomen enumeratio {
    BRIAR_FORMA_NULLA = 0,
    BRIAR_FORMA_PLANA,      /* principale in regione C */
    BRIAR_FORMA_VITREA      /* <fenestra/> + methodi */
} BriarForma;

/* plagula genita: via proiecto relativa + contentum */
nomen structura {
    chorda via;
    chorda contentum;
} BriarPlagula;

nomen structura {
                   b32  successus;
                 chorda  causa;           /* recusatio: lineam .thistle nominat */
                    i32  linea_causae;
             BriarForma  forma;
    constans character*  titulus;         /* basis viae sine '.thistle' */
              character  sigillum[17];    /* clavis: XVI hex + NUL */
                   Xar*  genitae;         /* BriarPlagula: fontes/ probationes/ assets/ ordines instrumenta/ */
                   Xar*  clausura;        /* SilexRes: include/ lib/ vendor/ */
                    i32  regiones_c;      /* regiones C non-probatio */
                    b32  probatio_adest;
} BriarFabricaFructus;

nomen structura {
    constans character* via_thistle;   /* in '#line' ut data */
    constans character* stampa;        /* corporis, in clavem */
    constans character* fons_titulus;  /* in stampam ordinum vitreorum */
} BriarFabricaOptiones;

/* nexus post briar_nexus_silvam_texere; octeti = plagula tota (clavis).
 * Recusatio: successus FALSUM, causa + linea_causae; nihil genitum. */
BriarFabricaFructus
briar_fabricare (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti);

/* proiectum in directorium scribere (parentes creat); ordines
 * exsecutabiles (chmod 0755); FALSUM + causa si plagula non scripta */
b32
briar_fabricam_scribere (
                        Piscina* piscina,
    constans BriarFabricaFructus* fructus,
             constans character* radix,
                         chorda* causa);

/* 'a/b/salve.thistle' -> 'salve'; characteres extra [A-Za-z0-9_] -> '_'
 * (symbolum C fit: capsula_<t>, custos capitis) */
constans character*
briar_fabrica_titulus (
               Piscina* piscina,
    constans character* via);

/* vexilla formae, chorda literalis EXACTA quam ordines ferunt */
constans character*
briar_fabrica_vexilla (
    BriarForma forma);

/* clavis: SHA-256(stampa '\n' vexilla '\n' octeti) -> XVI hex minusculae */
vacuum
briar_fabrica_clavem_computare (
    constans character* stampa,
    constans character* vexilla,
                chorda  octeti,
             character* sigillum_xvii);

#endif /* BRIAR_FABRICA_H */
```

- [ ] **Step 3: The failing gate (plain part)**

`briar/probationes/probatio_briar_fabrica.c` via scribe. Golden compare helper: each generated file is compared with `briar/probationes/fixa/fabrica/<t>/<via>`; with `BRIAR_FABRICA_SCRIBERE=1` the golden is written instead and the run prints `AURUM SCRIPTUM`.

```c
/* probatio_briar_fabrica.c - Fabrica sine clang: inventarium, regula
 * principalis, partitio unitatum, plagulae genitae contra AURUM
 * (fixa/fabrica/<t>/...) octetim, clausura, clavis, scriptor.
 * Regeneratio auri: BRIAR_FABRICA_SCRIBERE=1 cum causa nominata.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "briar_arbor.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

hic_manens constans character* AURUM = "briar/probationes/fixa/fabrica/";
hic_manens constans character* AREA  = "briar/build/fabrica/";

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
    /* idem ac in probatio_briar_silva.c */
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    chorda partes[3];

    partes[0] = chorda_ex_literis(a, piscina);
    partes[1] = chorda_ex_literis(b, piscina);
    partes[2] = chorda_ex_literis(c == NIHIL ? "" : c, piscina);
    redde chorda_ut_cstr(chorda_concatenare(
        chorda_concatenare(partes[0], partes[1], piscina), partes[2],
        piscina), piscina);
}

interior BriarFabricaFructus
_fabricare (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans SilexFons* fons,
    constans character* via,
                  Xar** nexus_out)
{
    BriarFabricaFructus  fructus;
    BriarFabricaOptiones optiones;
              character* textus;
                     i32 mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                  chorda octeti;

    memset(&fructus, 0, magnitudo(fructus));
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fructus.causa = chorda_ex_literis("plagula non lecta", piscina);
        redde fructus;
    }
    doc   = briar_arbor_parsare(piscina, textus, mensura);
    nexus = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_nexus_silvam_texere(piscina, nexus, fons);
    optiones.via_thistle  = via;
    optiones.stampa       = "probatio";
    optiones.fons_titulus = "probatio";
    octeti.datum   = (i8*)textus;
    octeti.mensura = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones, octeti);
    *nexus_out = nexus;
    redde fructus;
}

interior constans BriarPlagula*
_genita (
    constans BriarFabricaFructus* f,
             constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(f->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            f->genitae, i);

        si (chorda_aequalis_literis(p->via, via))
        {
            redde p;
        }
    }
    redde NIHIL;
}

interior b32
_clausura_habet (
    constans BriarFabricaFructus* f,
             constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(f->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            f->clausura, i);

        si (chorda_aequalis_literis(r->via, via))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* genita contra aurum; BRIAR_FABRICA_SCRIBERE=1 aurum scribit */
interior vacuum
_aurum_conferre (
                        Piscina* piscina,
    constans BriarFabricaFructus* f,
             constans character* via)
{
    constans BriarPlagula* p = _genita(f, via);
       constans character* via_auri = _texere(piscina, AURUM,
           _texere(piscina, f->titulus, "/", via), NIHIL);

    CREDO_NON_NIHIL (p);
    si (p == NIHIL)
    {
        redde;
    }
    si (getenv("BRIAR_FABRICA_SCRIBERE") != NIHIL)
    {
        chorda parens = via_directorium(chorda_ex_literis(via_auri,
            piscina), piscina);

        filum_directorium_creare_si_necesse(chorda_ut_cstr(parens, piscina));
        CREDO_VERUM (filum_scribere(via_auri, p->contentum));
        imprimere("  AURUM SCRIPTUM: %s\n", via_auri);
        redde;
    }
    {
        chorda aurum = filum_legere_totum(via_auri, piscina);

        CREDO_VERUM (aurum.mensura > ZEPHYRUM);
        si (!chorda_aequalis(aurum, p->contentum))
        {
            imprimere("  DIFFERT: %s\n--- genitum ---\n%.*s\n--- aurum ---\n%.*s\n",
                via_auri, (integer)p->contentum.mensura,
                (constans character*)p->contentum.datum,
                (integer)aurum.mensura, (constans character*)aurum.datum);
        }
        CREDO_CHORDA_AEQUALIS (p->contentum, aurum);
    }
}

s32
principale (vacuum)
{
                  b32  praeteritus;
              Piscina* piscina;
  InternamentumChorda* intern;
   constans character* radix;
   constans SilexFons* fons;

    piscina = piscina_generare_dynamicum("probatio_briar_fabrica",
        33554432);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    radix  = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    fons = silex_fons_disci(piscina, radix);
    CREDO_NON_NIHIL (fons);

    imprimere("\n--- Probans titulus, vexilla, clavis ---\n");
    {
        character a[17];
        character b[17];
        chorda    octeti = chorda_ex_literis("#!/usr/bin/env briar\n", piscina);

        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina,
            "a/b/salve.thistle"), "salve") == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina,
            "salve-mundi.thistle"), "salve_mundi") == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_titulus(piscina, "x"), "x")
            == ZEPHYRUM);
        CREDO_VERUM (strcmp(briar_fabrica_vexilla(BRIAR_FORMA_PLANA),
            SILEX_VEXILLA_COMPILATIONIS) == ZEPHYRUM);
        CREDO_VERUM (strstr(briar_fabrica_vexilla(BRIAR_FORMA_VITREA),
            "-DSQLITE_ENABLE_FTS5") != NIHIL);
        briar_fabrica_clavem_computare("stampa", "-Wall", octeti, a);
        briar_fabrica_clavem_computare("stampa", "-Wall", octeti, b);
        CREDO_AEQUALIS_I32 ((i32)strlen(a), (i32)16);
        CREDO_VERUM (strcmp(a, b) == ZEPHYRUM);
        CREDO_VERUM (strspn(a, "0123456789abcdef") == (size_t)16);
        briar_fabrica_clavem_computare("stampa", "-Wall -O2", octeti, b);
        CREDO_VERUM (strcmp(a, b) != ZEPHYRUM);
        briar_fabrica_clavem_computare("stampa2", "-Wall", octeti, b);
        CREDO_VERUM (strcmp(a, b) != ZEPHYRUM);
    }

    imprimere("\n--- Probans salve (planum): aurum, clausura, scriptor ---\n");
    {
        Xar* nexus;
        BriarFabricaFructus f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/salve.thistle", &nexus);

        si (!f.successus)
        {
            imprimere("  RECUSATIO: %.*s\n", (integer)f.causa.mensura,
                (constans character*)f.causa.datum);
        }
        CREDO_VERUM (f.successus);
        CREDO_AEQUALIS_S32 ((s32)f.forma, (s32)BRIAR_FORMA_PLANA);
        CREDO_VERUM (strcmp(f.titulus, "salve") == ZEPHYRUM);
        CREDO_AEQUALIS_I32 (f.regiones_c, I);
        CREDO_VERUM (f.probatio_adest);
        CREDO_AEQUALIS_I32 ((i32)strlen(f.sigillum), (i32)16);
        _aurum_conferre(piscina, &f, "fontes/salve.c");
        _aurum_conferre(piscina, &f, "fontes/salve_regiones.c");
        _aurum_conferre(piscina, &f, "fontes/salve_regiones.h");
        _aurum_conferre(piscina, &f, "probationes/probatio_salve.c");
        _aurum_conferre(piscina, &f, "aedificare.sh");
        _aurum_conferre(piscina, &f, "probare.sh");
        CREDO_AEQUALIS_I32 (xar_numerus(f.genitae), (i32)6);
        /* clausura: chorda (regio) + credo (probatio) + latina */
        CREDO_VERUM (_clausura_habet(&f, "include/latina.h"));
        CREDO_VERUM (_clausura_habet(&f, "include/chorda.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/chorda.c"));
        CREDO_VERUM (_clausura_habet(&f, "include/credo.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/credo.c"));
        CREDO_FALSUM (_clausura_habet(&f, "include/atrium.h"));
        /* '#line' ad lineam unitatis: 'principale' incipit linea IX
         * (linea VII est '#include "chorda.h"' - directiva, in caput) */
        {
            constans BriarPlagula* p = _genita(&f, "fontes/salve.c");
            constans BriarPlagula* h = _genita(&f, "fontes/salve_regiones.h");

            CREDO_CHORDA_CONTINET (p->contentum, chorda_ex_literis(
                "#line 9 \"briar/probationes/fixa/thistle/salve.thistle\"",
                piscina));
            CREDO_CHORDA_CONTINET (h->contentum, chorda_ex_literis(
                "#line 7 \"briar/probationes/fixa/thistle/salve.thistle\"\n"
                "#include \"chorda.h\"", piscina));
        }
        /* scriptor */
        {
            constans character* dir = _texere(piscina, AREA, "salve", NIHIL);
                         chorda causa;
                  struct stat st;

            CREDO_VERUM (briar_fabricam_scribere(piscina, &f, dir, &causa));
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/fontes/salve_regiones.h", NIHIL)));
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/lib/chorda.c", NIHIL)));
            CREDO_VERUM (filum_existit(_texere(piscina, dir,
                "/include/credo.h", NIHIL)));
            CREDO_VERUM (stat(_texere(piscina, dir, "/aedificare.sh", NIHIL),
                &st) == ZEPHYRUM);
            CREDO_VERUM ((st.st_mode & S_IXUSR) != ZEPHYRUM);
            CREDO_CHORDA_AEQUALIS (filum_legere_totum(_texere(piscina, dir,
                "/fontes/salve.c", NIHIL), piscina),
                _genita(&f, "fontes/salve.c")->contentum);
        }
        briar_nexus_silvam_solvere(nexus);
    }

    imprimere("\n--- Probans punctum: partitio typorum / obiectorum / definitionum ---\n");
    {
        Xar* nexus;
        BriarFabricaFructus f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/punctum.thistle", &nexus);
        constans BriarPlagula* caput;
        constans BriarPlagula* corpus;
        constans BriarPlagula* princeps;

        CREDO_VERUM (f.successus);
        _aurum_conferre(piscina, &f, "fontes/punctum.c");
        _aurum_conferre(piscina, &f, "fontes/punctum_regiones.c");
        _aurum_conferre(piscina, &f, "fontes/punctum_regiones.h");
        _aurum_conferre(piscina, &f, "probationes/probatio_punctum.c");
        caput    = _genita(&f, "fontes/punctum_regiones.h");
        corpus   = _genita(&f, "fontes/punctum_regiones.c");
        princeps = _genita(&f, "fontes/punctum.c");
        CREDO_NON_NIHIL (caput);
        CREDO_NON_NIHIL (corpus);
        CREDO_NON_NIHIL (princeps);
        /* caput: directivae + typus + prototypus; NON obiectum, NON corpus */
        CREDO_CHORDA_CONTINET (caput->contentum,
            chorda_ex_literis("#define PUNCTUM_MAXIMUM 3", piscina));
        CREDO_CHORDA_CONTINET (caput->contentum,
            chorda_ex_literis("#include \"chorda.h\"", piscina));
        CREDO_CHORDA_CONTINET (caput->contentum,
            chorda_ex_literis("} Punctum;", piscina));
        CREDO_CHORDA_CONTINET (caput->contentum,
            chorda_ex_literis("punctum_summa (constans Punctum* p);", piscina));
        CREDO_FALSUM (strstr(chorda_ut_cstr(caput->contentum, piscina),
            "numerus_vocationum = ZEPHYRUM") != NIHIL);
        CREDO_FALSUM (strstr(chorda_ut_cstr(caput->contentum, piscina),
            "redde p->x + p->y") != NIHIL);
        CREDO_CHORDA_CONTINET (caput->contentum,
            chorda_ex_literis("#ifndef PUNCTUM_REGIONES_H", piscina));
        /* corpus: obiectum + definitio; NON principale */
        CREDO_CHORDA_CONTINET (corpus->contentum,
            chorda_ex_literis("hic_manens i32 numerus_vocationum = ZEPHYRUM;",
                piscina));
        CREDO_CHORDA_CONTINET (corpus->contentum,
            chorda_ex_literis("redde p->x + p->y;", piscina));
        CREDO_FALSUM (strstr(chorda_ut_cstr(corpus->contentum, piscina),
            "principale") != NIHIL);
        /* princeps: principale solum, extensione secta ('}' ultimum) */
        CREDO_CHORDA_CONTINET (princeps->contentum,
            chorda_ex_literis("principale (vacuum)", piscina));
        CREDO_FALSUM (strstr(chorda_ut_cstr(princeps->contentum, piscina),
            "punctum_summa (constans Punctum* p)\n{") != NIHIL);
        {
            chorda c = princeps->contentum;
            i32    f2 = c.mensura;

            dum (f2 > ZEPHYRUM && ((character)c.datum[f2 - I] == '\n'
                || (character)c.datum[f2 - I] == ' '))
            {
                f2 = f2 - I;
            }
            CREDO_VERUM (f2 > ZEPHYRUM && (character)c.datum[f2 - I] == '}');
        }
        briar_nexus_silvam_solvere(nexus);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```

Run: `./briar/compile_probationes.sh fabrica` → FRACTA (compilatio): no `briar_fabrica.h`.

- [ ] **Step 4: Implementation**

`briar/fontes/briar_fabrica.c` via scribe. `postulata_posix.h` first (chmod).

```c
/* briar_fabrica.c - Vide briar_fabrica.h. */

#include "postulata_posix.h"
#include "briar_fabrica.h"
#include "briar_arbor.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "sigillum.h"
#include "silva.h"
#include "tabula_dispersa.h"
#include "via.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum   = NIHIL;
    c.mensura = ZEPHYRUM;
    redde c;
}

interior chorda
_literae (
               Piscina* piscina,
    constans character* literae)
{
    redde chorda_ex_literis(literae, piscina);
}

interior constans character*
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)128);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, c);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

/* spatia finalia abscisa */
interior chorda
_detondere (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && ((character)c.datum[c.mensura - I] == '\n'
            || (character)c.datum[c.mensura - I] == '\r'
            || (character)c.datum[c.mensura - I] == ' '
            || (character)c.datum[c.mensura - I] == '\t'))
    {
        c.mensura = c.mensura - I;
    }
    redde c;
}

/* linea (I-basata) in qua offset iacet */
interior i32
_linea_octeti (
    chorda textus,
       i32 offset)
{
    i32 linea = I;
    i32 i;

    per (i = ZEPHYRUM; i < offset && i < textus.mensura; i++)
    {
        si ((character)textus.datum[i] == '\n')
        {
            linea = linea + I;
        }
    }
    redde linea;
}

interior b32
_silva_chorda_est (
            SilvaChorda  s,
    constans character* literae)
{
    redde (b32)(s.mensura == (insignatus integer)strlen(literae)
        && memcmp(s.datum, literae, (size_t)s.mensura) == ZEPHYRUM);
}

interior vacuum
_recusare (
    BriarFabricaFructus* f,
                Piscina* piscina,
     constans character* causa,
                    i32  linea)
{
    f->successus    = FALSUM;
    f->causa        = _literae(piscina, causa);
    f->linea_causae = linea;
    f->genitae      = NIHIL;
    f->clausura     = NIHIL;
}

interior vacuum
_genitam_addere (
    Piscina* piscina,
        Xar* genitae,
    constans character* via,
      chorda contentum)
{
    BriarPlagula* p = (BriarPlagula*)xar_addere(genitae);

    si (p != NIHIL)
    {
        p->via       = _literae(piscina, via);
        p->contentum = contentum;
    }
}

interior vacuum
_lineam_appendere (
    ChordaAedificator* a,
                  i32  linea,
    constans character* via)
{
    character b[32];

    sprintf(b, "#line %d \"", (integer)linea);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a, "\"\n");
}

/* ==================================================
 * Titulus, vexilla, clavis
 * ================================================== */

constans character*
briar_fabrica_titulus (
               Piscina* piscina,
    constans character* via)
{
    constans character* basis = via;
    constans character* p;
             character* t;
                   i32  m;
                   i32  i;

    per (p = via; *p != '\0'; p++)
    {
        si (*p == '/')
        {
            basis = p + I;
        }
    }
    m = (i32)strlen(basis);
    si (m > VII && strcmp(basis + (m - VIII), ".thistle") == ZEPHYRUM)
    {
        m = m - VIII;
    }
    si (m == ZEPHYRUM)
    {
        redde "thistle";
    }
    t = (character*)piscina_allocare(piscina, (memoriae_index)(m + I));
    per (i = ZEPHYRUM; i < m; i++)
    {
        character c = basis[i];
        b32 litera = (b32)((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9') || c == '_');

        t[i] = litera ? c : '_';
    }
    t[m] = '\0';
    redde t;
}

constans character*
briar_fabrica_vexilla (
    BriarForma forma)
{
    si (forma == BRIAR_FORMA_VITREA)
    {
        redde SILEX_VEXILLA_VITREA " | " SILEX_VEXILLA_VENDITORIA;
    }
    redde SILEX_VEXILLA_COMPILATIONIS;
}

vacuum
briar_fabrica_clavem_computare (
    constans character* stampa,
    constans character* vexilla,
                chorda  octeti,
             character* sigillum_xvii)
{
    SigillumContextus ctx;
             Sigillum s;
            character hex[SIGILLUM_HEX_MENSURA];

    sigillum_incipere(&ctx);
    sigillum_addere(&ctx, stampa, (memoriae_index)strlen(stampa));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, vexilla, (memoriae_index)strlen(vexilla));
    sigillum_addere(&ctx, "\n", (memoriae_index)I);
    sigillum_addere(&ctx, octeti.datum, (memoriae_index)octeti.mensura);
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    memcpy(sigillum_xvii, hex, (size_t)16);
    sigillum_xvii[16] = '\0';
}

/* ==================================================
 * Inventarium regionum
 * ================================================== */

nomen structura {
    BriarNexusRes** app;          /* regiones C non-probatio, ordine */
               i32  numerus_app;
    BriarNexusRes*  probatio;     /* aut NIHIL */
    BriarNexusRes*  fenestra;     /* elementum; Task 4 */
    BriarNexusRes*  html;         /* Task 4 */
    BriarNexusRes*  js;
    BriarNexusRes*  css;
} BriarInventarium;

interior b32
_inventarium_colligere (
               Piscina* piscina,
                   Xar* nexus,
      BriarInventarium* inv,
   BriarFabricaFructus* f)
{
    i32 i;
    i32 n = xar_numerus(nexus);

    memset(inv, 0, magnitudo(*inv));
    inv->app = (BriarNexusRes**)piscina_allocare(piscina,
        (memoriae_index)((n + I) * (i32)magnitudo(BriarNexusRes*)));
    per (i = ZEPHYRUM; i < n; i++)
    {
        BriarNexusRes* r = (BriarNexusRes*)xar_obtinere(nexus, i);

        si (r->genus == BRIAR_NEXUS_REGIO && briar_nexus_titulus_est(r, "c"))
        {
            si (r->parsura == NIHIL)
            {
                _recusare(f, piscina, chorda_ut_cstr(r->causa.mensura
                    ? r->causa : _literae(piscina,
                        "regio C non parsata (silvam_texere ante)"),
                    piscina), r->linea_erroris ? r->linea_erroris
                    : r->linea_initium);
                redde FALSUM;
            }
            si (r->linea_erroris > ZEPHYRUM)
            {
                _recusare(f, piscina, chorda_ut_cstr(r->causa, piscina),
                    r->linea_erroris);
                redde FALSUM;
            }
            si (chorda_aequalis_literis(briar_nexus_attributum(r, "munus"),
                "probatio"))
            {
                si (inv->probatio != NIHIL)
                {
                    _recusare(f, piscina,
                        "regio probationis iterata (una in plano I)",
                        r->linea_initium - I);
                    redde FALSUM;
                }
                inv->probatio = r;
            }
            alioquin
            {
                inv->app[inv->numerus_app] = r;
                inv->numerus_app = inv->numerus_app + I;
            }
        }
        alioquin si (r->genus == BRIAR_NEXUS_STML
            && briar_nexus_titulus_est(r, "fenestra"))
        {
            inv->fenestra = r;
        }
        alioquin si (r->genus == BRIAR_NEXUS_REGIO)
        {
            BriarNexusRes** sedes = briar_nexus_titulus_est(r, "html")
                ? &inv->html : briar_nexus_titulus_est(r, "js") ? &inv->js
                : briar_nexus_titulus_est(r, "css") ? &inv->css : NIHIL;

            si (sedes != NIHIL)
            {
                si (*sedes != NIHIL)
                {
                    character b[96];

                    sprintf(b, "regio %.*s iterata (prima linea %d)",
                        (integer)r->titulus.mensura,
                        (constans character*)r->titulus.datum,
                        (integer)((*sedes)->linea_initium - I));
                    _recusare(f, piscina, b, r->linea_initium - I);
                    redde FALSUM;
                }
                *sedes = r;
            }
        }
    }
    redde VERUM;
}

/* ==================================================
 * Partitio unitatum (silva)
 * ================================================== */

nomen structura {
    i32    linea;    /* .thistle */
    chorda textus;
} BriarUnitas;

nomen structura {
    Xar* directivae;   /* BriarUnitas: lineae '#...' regionum */
    Xar* typi;         /* BriarUnitas: unitates sine obiecto */
    Xar* prototypi;    /* BriarUnitas: 'caput;' definitionum */
    Xar* corpora;      /* BriarUnitas: obiecta + definitiones */
    BriarUnitas princeps;
    i32  principalia;  /* numerus unitatum 'main' */
    i32  linea_principalis_secundi;
} BriarPartitio;

interior vacuum
_unitatem_addere (
    Xar*   xar,
    i32    linea,
    chorda textus)
{
    BriarUnitas* u = (BriarUnitas*)xar_addere(xar);

    si (u != NIHIL)
    {
        u->linea  = linea;
        u->textus = textus;
    }
}

/* directivae textuales (lineae quarum character primus non albus '#',
 * cum continuationibus '\') - silva eas consumit, textus eas servat */
interior vacuum
_directivas_colligere (
    constans BriarNexusRes* r,
                      Xar* directivae)
{
    chorda c = r->contentum;
    i32    i = ZEPHYRUM;
    i32    linea = r->linea_initium;

    dum (i < c.mensura)
    {
        i32 initium = i;
        i32 p = i;
        i32 finis;

        dum (p < c.mensura && ((character)c.datum[p] == ' '
            || (character)c.datum[p] == '\t'))
        {
            p = p + I;
        }
        finis = initium;
        dum (finis < c.mensura && (character)c.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        si (p < c.mensura && (character)c.datum[p] == '#')
        {
            i32 f = finis;
            i32 lineae = I;

            dum (f > initium && (character)c.datum[f - I] == '\\'
                && f < c.mensura)
            {
                f = f + I;
                dum (f < c.mensura && (character)c.datum[f] != '\n')
                {
                    f = f + I;
                }
                lineae = lineae + I;
            }
            _unitatem_addere(directivae, linea, chorda_sectio(c, initium, f));
            finis = f;
            linea = linea + lineae - I;
        }
        i = (finis < c.mensura) ? finis + I : finis;
        linea = linea + I;
    }
}

interior constans SemanticaSymbolum*
_symbolum_definitionis (
    constans SilvaSemantica* sem,
        constans SilvaNodus* unitas)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_per_indicem(sem,
            k);

        si (s->declarans == unitas && s->genus == (integer)SYMBOLUM_FUNCTIO)
        {
            redde s;
        }
    }
    redde NIHIL;
}

interior b32
_unitas_obiectum_declarat (
    constans SilvaSemantica* sem,
                    integer  fons_index,
                    integer  minimum,
                    integer  maximum)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(sem); k++)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_per_indicem(sem,
            k);

        si (   s->genus == (integer)SYMBOLUM_VARIABILE
            && s->profunditas == ZEPHYRUM
            && s->lexema != NIHIL
            && s->lexema->fons_index == fons_index
            && s->lexema->byte_offset >= minimum
            && s->lexema->byte_offset < maximum)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_regionem_partiri (
                Piscina* piscina,
    constans BriarNexusRes* r,
         BriarPartitio* part,
    BriarFabricaFructus* f)
{
    constans SilvaNodus* radix = r->parsura->commissio->radix.datum.nodus;
               SilvaXar* liberi = silva_nodus_liberi(r->silva_piscina, radix);
     insignatus integer  k;
                integer  fons_index = r->parsura->fons_princeps;

    _directivas_colligere(r, part->directivae);
    si (liberi == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < silva_xar_numerus(liberi); k++)
    {
        constans SilvaNodus* u = *(SilvaNodus**)silva_xar_obtinere(liberi, k);
        integer minimum = -I;
        integer maximum = ZEPHYRUM;
        chorda  textus;
        i32     linea;

        silva_nodus_extensionem(u, fons_index, &minimum, &maximum);
        si (minimum < ZEPHYRUM || minimum < r->praeludium_octeti)
        {
            perge;   /* syntheticum aut praeludium (exemplar) */
        }
        textus = chorda_sectio(r->textus_silvae, (i32)minimum, (i32)maximum);
        linea  = briar_nexus_linea_silvae(r,
            _linea_octeti(r->textus_silvae, (i32)minimum));
        si (u->genus == (integer)SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
        {
            constans SemanticaSymbolum* s = _symbolum_definitionis(
                r->semantica, u);
            SilvaValor corpus = silva_c89_definitio_functionis_corpus(u);
            integer cmin = -I;
            integer cmax = ZEPHYRUM;
            chorda  caput;

            si (s != NIHIL && _silva_chorda_est(s->titulus, "main"))
            {
                part->principalia = part->principalia + I;
                si (part->principalia == I)
                {
                    part->princeps.linea  = linea;
                    part->princeps.textus = textus;
                }
                alioquin
                {
                    part->linea_principalis_secundi = linea;
                }
                perge;
            }
            si (corpus.genus == SILVA_VALOR_NODUS)
            {
                silva_nodus_extensionem(corpus.datum.nodus, fons_index,
                    &cmin, &cmax);
            }
            si (cmin > minimum)
            {
                ChordaAedificator* a = chorda_aedificator_creare(piscina,
                    (memoriae_index)256);

                caput = _detondere(chorda_sectio(r->textus_silvae,
                    (i32)minimum, (i32)cmin));
                chorda_aedificator_appendere_chorda(a, caput);
                chorda_aedificator_appendere_literis(a, ";");
                _unitatem_addere(part->prototypi, linea,
                    chorda_aedificator_finire(a));
            }
            _unitatem_addere(part->corpora, linea, textus);
        }
        alioquin si (_unitas_obiectum_declarat(r->semantica, fons_index,
            minimum, maximum))
        {
            _unitatem_addere(part->corpora, linea, textus);
        }
        alioquin
        {
            _unitatem_addere(part->typi, linea, textus);
        }
    }
    (vacuum)f;
    redde VERUM;
}

/* ==================================================
 * Textus geniti
 * ================================================== */

interior vacuum
_unitates_appendere (
    ChordaAedificator* a,
                  Xar* unitates,
    constans character* via)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(unitates); i++)
    {
        constans BriarUnitas* u = (constans BriarUnitas*)xar_obtinere(
            unitates, i);

        _lineam_appendere(a, u->linea, via);
        chorda_aedificator_appendere_chorda(a, u->textus);
        chorda_aedificator_appendere_literis(a, "\n");
    }
}

interior constans character*
_custos (
               Piscina* piscina,
    constans character* titulus)
{
    i32 m = (i32)strlen(titulus);
    character* c = (character*)piscina_allocare(piscina,
        (memoriae_index)(m + 12));
    i32 i;

    per (i = ZEPHYRUM; i < m; i++)
    {
        character x = titulus[i];

        c[i] = (x >= 'a' && x <= 'z') ? (character)(x - 'a' + 'A') : x;
    }
    c[m] = '\0';
    strcat(c, "_REGIONES_H");
    redde c;
}

interior chorda
_caput_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    constans character* custos = _custos(piscina, titulus);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": directivae, typi, prototypi regionum */\n#ifndef ");
    chorda_aedificator_appendere_literis(a, custos);
    chorda_aedificator_appendere_literis(a, "\n#define ");
    chorda_aedificator_appendere_literis(a, custos);
    chorda_aedificator_appendere_literis(a,
        "\n#include \"latina.h\"\n");
    _unitates_appendere(a, part->directivae, via);
    _unitates_appendere(a, part->typi, via);
    _unitates_appendere(a, part->prototypi, via);
    chorda_aedificator_appendere_literis(a, "#endif /* ");
    chorda_aedificator_appendere_literis(a, custos);
    chorda_aedificator_appendere_literis(a, " */\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_corpus_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans character* via,
    constans BriarPartitio* part)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": obiecta et definitiones regionum */\n"
        "#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _unitates_appendere(a, part->corpora, via);
    redde chorda_aedificator_finire(a);
}

interior chorda
_principem_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans character* via,
    constans BriarUnitas* princeps)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)2048);

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": principale */\n#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _lineam_appendere(a, princeps->linea, via);
    chorda_aedificator_appendere_chorda(a, princeps->textus);
    chorda_aedificator_appendere_literis(a, "\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_probationem_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans character* via,
    constans BriarNexusRes* probatio)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(probatio->contentum.mensura + 256));

    chorda_aedificator_appendere_literis(a, "/* probatio_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": regio munus=\"probatio\" */\n#include \"latina.h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h\"\n");
    _lineam_appendere(a, probatio->linea_initium, via);
    chorda_aedificator_appendere_chorda(a, probatio->contentum);
    redde chorda_aedificator_finire(a);
}

/* ==================================================
 * Fabricare
 * ================================================== */

BriarFabricaFructus
briar_fabricare (
                          Piscina* piscina,
            constans MateriaNodus* documentum,
                              Xar* nexus,
               constans SilexFons* fons,
    constans BriarFabricaOptiones* optiones,
                           chorda  octeti)
{
    BriarFabricaFructus f;
       BriarInventarium inv;
          BriarPartitio part;
                    i32 i;
     constans character* via;
     constans character* fontes_app[2];
     constans character* fontes_prob[2];

    memset(&f, 0, magnitudo(f));
    si (piscina == NIHIL || documentum == NIHIL || nexus == NIHIL
        || fons == NIHIL || optiones == NIHIL)
    {
        _recusare(&f, piscina, "argumenta nulla", ZEPHYRUM);
        redde f;
    }
    via       = optiones->via_thistle;
    f.titulus = briar_fabrica_titulus(piscina, via);
    si (!_inventarium_colligere(piscina, nexus, &inv, &f))
    {
        redde f;
    }
    f.regiones_c     = inv.numerus_app;
    f.probatio_adest = (b32)(inv.probatio != NIHIL);

    memset(&part, 0, magnitudo(part));
    part.directivae = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.typi       = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.prototypi  = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    part.corpora    = xar_creare(piscina, (i32)magnitudo(BriarUnitas));
    per (i = ZEPHYRUM; i < inv.numerus_app; i++)
    {
        si (!_regionem_partiri(piscina, inv.app[i], &part, &f))
        {
            redde f;
        }
    }

    /* regula principalis */
    si (part.principalia > I)
    {
        character b[96];

        sprintf(b, "duo principalia: lineae %d et %d",
            (integer)part.princeps.linea,
            (integer)part.linea_principalis_secundi);
        _recusare(&f, piscina, b, part.linea_principalis_secundi);
        redde f;
    }
    si (part.principalia == I && inv.fenestra != NIHIL)
    {
        character b[96];

        sprintf(b, "<fenestra> (linea %d) et principale (linea %d): unum elige",
            (integer)inv.fenestra->linea_initium,
            (integer)part.princeps.linea);
        _recusare(&f, piscina, b, inv.fenestra->linea_initium);
        redde f;
    }
    si (part.principalia == ZEPHYRUM && inv.fenestra == NIHIL)
    {
        _recusare(&f, piscina,
            "nec principale in regione C nec <fenestra/>: nihil currendum",
            ZEPHYRUM);
        redde f;
    }
    f.forma = (part.principalia == I) ? BRIAR_FORMA_PLANA : BRIAR_FORMA_VITREA;

    f.genitae = xar_creare(piscina, (i32)magnitudo(BriarPlagula));
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "fontes/", f.titulus, "_regiones.h"),
        _caput_fingere(piscina, f.titulus, via, &part));
    _genitam_addere(piscina, f.genitae,
        _texere(piscina, "fontes/", f.titulus, "_regiones.c"),
        _corpus_fingere(piscina, f.titulus, via, &part));
    si (inv.probatio != NIHIL)
    {
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "probationes/probatio_", f.titulus, ".c"),
            _probationem_fingere(piscina, f.titulus, via, inv.probatio));
    }

    fontes_app[0]  = _texere(piscina, "fontes/", f.titulus, ".c");
    fontes_app[1]  = _texere(piscina, "fontes/", f.titulus, "_regiones.c");
    fontes_prob[0] = _texere(piscina, "probationes/probatio_", f.titulus,
        ".c");
    fontes_prob[1] = fontes_app[1];

    si (f.forma == BRIAR_FORMA_PLANA)
    {
        chorda* contenta;
        i32     n = ZEPHYRUM;

        _genitam_addere(piscina, f.genitae, fontes_app[0],
            _principem_fingere(piscina, f.titulus, via, &part.princeps));
        /* clausura: regiones omnes (probatio inclusa - credo.h) */
        contenta = (chorda*)piscina_allocare(piscina,
            (memoriae_index)((inv.numerus_app + I)
                * (i32)magnitudo(chorda)));
        per (i = ZEPHYRUM; i < inv.numerus_app; i++)
        {
            contenta[n] = inv.app[i]->contentum;
            n = n + I;
        }
        si (inv.probatio != NIHIL)
        {
            contenta[n] = inv.probatio->contentum;
            n = n + I;
        }
        f.clausura = silex_clausuram_e_contentis(piscina, fons, contenta, n);
        si (f.clausura == NIHIL)
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta", ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_fingere(piscina, f.titulus, fontes_app, II));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_fingere(piscina, f.titulus,
                    fontes_prob, II));
        }
    }
    alioquin
    {
        /* Task 4 */
        _recusare(&f, piscina, "forma vitrea nondum (Task 4)",
            inv.fenestra->linea_initium);
        redde f;
    }

    briar_fabrica_clavem_computare(optiones->stampa,
        briar_fabrica_vexilla(f.forma), octeti, f.sigillum);
    f.successus = VERUM;
    redde f;
}

/* ==================================================
 * Scriptor
 * ================================================== */

interior b32
_plagulam_scribere (
               Piscina* piscina,
    constans character* radix,
                chorda  via,
                chorda  contentum,
                chorda* causa)
{
    constans character* plena = _texere(piscina, radix, "/",
        chorda_ut_cstr(via, piscina));
                chorda parens = via_directorium(_literae(piscina, plena),
                    piscina);

    filum_directorium_creare_si_necesse(chorda_ut_cstr(parens, piscina));
    si (!filum_scribere(plena, contentum))
    {
        *causa = _literae(piscina, _texere(piscina, "non scripta: ", plena,
            NIHIL));
        redde FALSUM;
    }
    redde VERUM;
}

b32
briar_fabricam_scribere (
                        Piscina* piscina,
    constans BriarFabricaFructus* fructus,
             constans character* radix,
                         chorda* causa)
{
    i32 i;

    *causa = _vacua();
    si (fructus == NIHIL || !fructus->successus || radix == NIHIL)
    {
        *causa = _literae(piscina, "fructus non sanus");
        redde FALSUM;
    }
    filum_directorium_creare_si_necesse(radix);
    per (i = ZEPHYRUM; i < xar_numerus(fructus->clausura); i++)
    {
        constans SilexRes* r = (constans SilexRes*)xar_obtinere(
            fructus->clausura, i);

        si (!_plagulam_scribere(piscina, radix, r->via, r->contentum, causa))
        {
            redde FALSUM;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(fructus->genitae); i++)
    {
        constans BriarPlagula* p = (constans BriarPlagula*)xar_obtinere(
            fructus->genitae, i);

        si (!_plagulam_scribere(piscina, radix, p->via, p->contentum, causa))
        {
            redde FALSUM;
        }
        si (p->via.mensura > III
            && memcmp(p->contentum.datum, "#!", (size_t)II) == ZEPHYRUM)
        {
            (vacuum)chmod(_texere(piscina, radix, "/",
                chorda_ut_cstr(p->via, piscina)), (mode_t)0755);
        }
    }
    redde VERUM;
}
```

Implementer notes:
- `silva_nodus_extensionem` reports `maximum` as the byte AFTER the last token (exclusive) — the gate's last assertion (main unit ends with `}`) verifies this; if it fails with the text one byte short, change both `chorda_sectio(... maximum)` calls to `maximum + I` and note it in the worklog.
- `chorda_concatenare(a, b, piscina)` exists (chorda.h:44); if its parameter order differs, adapt `_texere` in the test.
- `_recusare` with `chorda_ut_cstr(r->causa, …)` when `r->causa` is empty: guarded by the ternary.
- The `(vacuum)f;` in `_regionem_partiri` silences the unused parameter; drop the parameter instead if examen objects.

Run: `./briar/compile_probationes.sh fabrica`
Expected: compiles; all assertions except the golden compares pass; the golden compares FAIL (`aurum.mensura > ZEPHYRUM` false — no golden yet).

- [ ] **Step 5: Verify the partition by eye, then compile the written project**

```bash
./briar/compile_probationes.sh fabrica 2>&1 | grep -A40 'DIFFERT' | head -80
```
Read the generated `fontes/punctum_regiones.h` text printed under DIFFERT: directives with `#line`, the typedef, the prototype `i32\npunctum_summa (constans Punctum* p);` (multi-line head preserved). Then the tool (Step 7 builds it) — do Step 7 now, then:

```bash
./briar/fabrica.sh briar/probationes/fixa/thistle/punctum.thistle build/fabrica_manu/punctum
(cd build/fabrica_manu/punctum && ./aedificare.sh && ./bin/punctum && ./probare.sh)
./briar/fabrica.sh briar/probationes/fixa/thistle/salve.thistle build/fabrica_manu/salve
(cd build/fabrica_manu/salve && ./aedificare.sh && ./bin/salve && ./probare.sh)
```
Expected: `aedificatum: bin/punctum`, `summa 3`, probatio `OMNIA PRAETERIERUNT`; `salve, munde`; both probare green. A clang error here names a `.thistle` line — that is the `#line` map working; fix the partition, not the fixture. This is the only clang run in plan 2 and it is by hand (spec §8 P3: no clang in the suite).

- [ ] **Step 6: Birth the goldens**

```bash
BRIAR_FABRICA_SCRIBERE=1 ./briar/compile_probationes.sh fabrica | grep 'AURUM SCRIPTUM' | wc -l   # 10
./briar/compile_probationes.sh fabrica                                                          # green
git status --short briar/probationes/fixa/fabrica | head
```
Inspect each golden once (`cat`) — this is the audit the goldens doctrine demands; a golden never regenerates later without a named cause in the commit.

- [ ] **Step 7: The hand tool**

`briar/instrumenta/fabrica.c` via scribe:

```c
/* fabrica.c - thistle -> proiectum in directorio dato (sine clang):
 *   fabrica <plagula.thistle> <directorium> [-fons <radix>]
 * Fons silicis: -fons, alioquin RHUBARB_RADIX, alioquin ascensus
 * (silex_fabricam_invenire). Exitus 0 scriptum | 1 recusatio/fractum
 * | 2 usus. Inventarium in stdout; recusatio in stderr cum linea.
 */

#include "latina.h"
#include "briar_arbor.h"
#include "briar_fabrica.h"
#include "briar_nexus.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

s32
principale (
       integer  numerus_argumentorum,
    character** argumenta)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* via = NIHIL;
     constans character* directorium = NIHIL;
     constans character* radix = NIHIL;
     constans SilexFons* fons;
              character* textus;
                    i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
    BriarFabricaOptiones optiones;
     BriarFabricaFructus fructus;
                  chorda octeti;
                  chorda causa;
                 integer i;

    per (i = I; i < numerus_argumentorum; i++)
    {
        si (strcmp(argumenta[i], "-fons") == ZEPHYRUM
            && i + I < numerus_argumentorum)
        {
            radix = argumenta[i + I];
            i = i + I;
        }
        alioquin si (via == NIHIL)
        {
            via = argumenta[i];
        }
        alioquin si (directorium == NIHIL)
        {
            directorium = argumenta[i];
        }
    }
    si (via == NIHIL || directorium == NIHIL)
    {
        fprintf(stderr,
            "usus: fabrica <plagula.thistle> <directorium> [-fons <radix>]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("fabrica", 33554432);
    intern  = internamentum_creare(piscina);
    si (radix == NIHIL)
    {
        radix = getenv("RHUBARB_RADIX");
    }
    si (radix == NIHIL)
    {
        radix = silex_fabricam_invenire(piscina, ".");
    }
    fons = (radix != NIHIL) ? silex_fons_disci(piscina, radix) : NIHIL;
    si (fons == NIHIL)
    {
        fprintf(stderr, "fabrica: fons silicis non inventus (-fons <radix>)\n");
        redde I;
    }
    textus = _plagulam_legere(piscina, via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "fabrica: plagula non lecta: %s\n", via);
        redde I;
    }
    doc   = briar_arbor_parsare(piscina, textus, mensura);
    nexus = briar_nexus_texere(piscina, doc, intern);
    si (nexus == NIHIL || briar_nexus_silvam_texere(piscina, nexus, fons)
        < ZEPHYRUM)
    {
        fprintf(stderr, "fabrica: nexus fractus\n");
        redde I;
    }
    optiones.via_thistle  = via;
    optiones.stampa       = fons->titulus;
    optiones.fons_titulus = "briar";
    octeti.datum   = (i8*)textus;
    octeti.mensura = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones, octeti);
    si (!fructus.successus)
    {
        fprintf(stderr, "%s:%d: %.*s\n", via, (integer)fructus.linea_causae,
            (integer)fructus.causa.mensura,
            (constans character*)fructus.causa.datum);
        redde I;
    }
    si (!briar_fabricam_scribere(piscina, &fructus, directorium, &causa))
    {
        fprintf(stderr, "fabrica: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    imprimere("fabrica: %s -> %s\n  forma %s  titulus %s  clavis %s\n"
        "  genitae %d  clausura %d  regiones C %d  probatio %s\n",
        via, directorium,
        fructus.forma == BRIAR_FORMA_VITREA ? "vitrea" : "plana",
        fructus.titulus, fructus.sigillum,
        (integer)xar_numerus(fructus.genitae),
        (integer)xar_numerus(fructus.clausura),
        (integer)fructus.regiones_c,
        fructus.probatio_adest ? "adest" : "abest");
    briar_nexus_silvam_solvere(nexus);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
```

`briar/fabrica.sh`: copy `briar/arbor.sh`, rename `arbor`→`fabrica` everywhere (`BIN`, `SRC`, the usage line: `./briar/fabrica.sh <plagula.thistle> <directorium> [-fons <radix>]`), and add `"-I$RADIX_DIR/silva/amalgama"` to `INCLUDE_FLAGS`; the `$OBJ` loop already gathers every `build/*.o` (sqlite, amalgam, silex). `chmod +x briar/fabrica.sh`.

- [ ] **Step 8: Plant, see red, revert**

Plant: in `_unitates_appendere` skip `_lineam_appendere`. Expected RED: every golden differs (and the `#line 7` assertion). Revert. Green: `./briar/compile_probationes.sh` → `10/10`.

- [ ] **Step 9: Commit**

Formator on the four new/changed C files. `commissio_umbra` portae `['briar']`, viae: fabrica .h/.c, test, fixture `punctum.thistle`, goldens dir (`briar/probationes/fixa/fabrica`), `briar/instrumenta/fabrica.c`, `briar/fabrica.sh`. Message: the partition rule, `#line`, goldens born (cause: birth), the by-hand clang run of both projects (`salve, munde`, `summa 3`, probationes green), plant seen red, `BRIAR X/X`.

---

### Task 4: Fabrica — the vitrea shape

**Files:**
- Modify: `briar/fontes/briar_fabrica.c` (replace the Task-3 vitrea stub)
- Modify: `briar/probationes/probatio_briar_fabrica.c` (new block)
- Create goldens: `briar/probationes/fixa/fabrica/salve_vitreum/{fontes/salve_vitreum.c, fontes/salve_vitreum_regiones.c, fontes/salve_vitreum_regiones.h, probationes/probatio_salve_vitreum.c, assets/index.html, assets/salve_vitreum.toml}`

**Interfaces:**
- Consumes: Task 1 exemplar (`briar_tractator_exemplar` symbol), `silva_c89_typi_compatibiles`, `silex_fons_legere(fons, "tools/capsula_generare.c", …)`, the two vitrea generators.
- Produces: `BRIAR_FORMA_VITREA` path of `briar_fabricare`.

- [ ] **Step 1: Failing assertions**

Append to the test's `principale`, before the compendium:

```c
    imprimere("\n--- Probans salve_vitreum: principale genitum, methodus, assets ---\n");
    {
        Xar* nexus;
        BriarFabricaFructus f = _fabricare(piscina, intern, fons,
            "briar/probationes/fixa/thistle/salve_vitreum.thistle", &nexus);
        constans BriarPlagula* aed;
        constans BriarPlagula* prob;
        constans BriarPlagula* princeps;

        si (!f.successus)
        {
            imprimere("  RECUSATIO: %.*s\n", (integer)f.causa.mensura,
                (constans character*)f.causa.datum);
        }
        CREDO_VERUM (f.successus);
        CREDO_AEQUALIS_S32 ((s32)f.forma, (s32)BRIAR_FORMA_VITREA);
        _aurum_conferre(piscina, &f, "fontes/salve_vitreum.c");
        _aurum_conferre(piscina, &f, "fontes/salve_vitreum_regiones.c");
        _aurum_conferre(piscina, &f, "fontes/salve_vitreum_regiones.h");
        _aurum_conferre(piscina, &f, "probationes/probatio_salve_vitreum.c");
        _aurum_conferre(piscina, &f, "assets/index.html");
        _aurum_conferre(piscina, &f, "assets/salve_vitreum.toml");
        princeps = _genita(&f, "fontes/salve_vitreum.c");
        CREDO_NON_NIHIL (princeps);
        CREDO_CHORDA_CONTINET (princeps->contentum, chorda_ex_literis(
            "figura.titulus  = \"salve\";", piscina));
        CREDO_CHORDA_CONTINET (princeps->contentum, chorda_ex_literis(
            "figura.latitudo = 640;", piscina));
        CREDO_CHORDA_CONTINET (princeps->contentum, chorda_ex_literis(
            "internuntius_praebere(atrium_internuntius(atrium),\n"
            "        \"salve\", salve, NIHIL);", piscina));
        CREDO_CHORDA_CONTINET (princeps->contentum, chorda_ex_literis(
            "atrium_vexilla_legere(&figura, argc, argv);", piscina));
        /* ordines vitrei: listae computatae (corpus-dependentes - non aurum) */
        aed  = _genita(&f, "aedificare.sh");
        prob = _genita(&f, "probare.sh");
        CREDO_NON_NIHIL (aed);
        CREDO_NON_NIHIL (prob);
        CREDO_CHORDA_CONTINET (aed->contentum, chorda_ex_literis(
            "fontes/salve_vitreum.c fontes/salve_vitreum_regiones.c \\\n"
            "    assets/capsula_salve_vitreum.c", piscina));
        CREDO_CHORDA_CONTINET (aed->contentum, chorda_ex_literis(
            "-framework Cocoa", piscina));
        CREDO_CHORDA_CONTINET (aed->contentum, chorda_ex_literis(
            "(novum -vitrea, probatio)", piscina));
        CREDO_CHORDA_CONTINET (prob->contentum, chorda_ex_literis(
            "probationes/probatio_salve_vitreum.c fontes/salve_vitreum_regiones.c",
            piscina));
        CREDO_NON_NIHIL (_genita(&f, "instrumenta/capsula_generare.c"));
        CREDO_VERUM (_clausura_habet(&f, "include/atrium.h"));
        CREDO_VERUM (_clausura_habet(&f, "include/json.h"));
        CREDO_VERUM (_clausura_habet(&f, "lib/credo.c"));
        CREDO_VERUM (_clausura_habet(&f, "vendor/sqlite3.c"));
        CREDO_AEQUALIS_I32 (xar_numerus(f.genitae), (i32)9);
        briar_nexus_silvam_solvere(nexus);
    }
```

Run: red (`forma vitrea nondum`).

- [ ] **Step 2: Implementation**

In `briar_fabrica.c` add, before `briar_fabricare`:

```c
/* ==================================================
 * Forma vitrea: <fenestra/>, methodi, principale genitum, assets
 * ================================================== */

interior b32
_numerum_legere (
    chorda c,
      i32* valor)
{
    i32 i;
    i32 v = ZEPHYRUM;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        character d = (character)c.datum[i];

        si (d < '0' || d > '9')
        {
            redde FALSUM;
        }
        v = v * X + (i32)(d - '0');
    }
    *valor = v;
    redde VERUM;
}

/* methodus="nomen": functio 'nomen' in regione definita et cum
 * exemplari tractatoris compatibilis; recusatio nominat lineam tagi */
interior b32
_methodum_probare (
                Piscina* piscina,
    constans BriarNexusRes* r,
                 chorda  methodus,
    BriarFabricaFructus* f)
{
    constans SemanticaSymbolum* functio = NIHIL;
    constans SemanticaSymbolum* exemplar = NIHIL;
            insignatus integer  k;
                     character  b[160];
                           i32  linea_tagi = r->linea_initium - I;

    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(r->semantica); k++)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_per_indicem(
            r->semantica, k);

        si (s->profunditas != ZEPHYRUM)
        {
            perge;
        }
        si (   s->genus == (integer)SYMBOLUM_FUNCTIO
            && s->titulus.mensura == (insignatus integer)methodus.mensura
            && memcmp(s->titulus.datum, methodus.datum,
                (size_t)methodus.mensura) == ZEPHYRUM
            && silva_c89_definitio_functionis_corpus(s->declarans).genus
                != SILVA_VALOR_NIHIL)
        {
            functio = s;
        }
        si (_silva_chorda_est(s->titulus, "briar_tractator_exemplar"))
        {
            exemplar = s;
        }
    }
    si (functio == NIHIL)
    {
        sprintf(b, "methodus '%.*s' (linea %d): functio non definita in regione",
            (integer)methodus.mensura, (constans character*)methodus.datum,
            (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    si (   exemplar == NIHIL
        || exemplar->typus->genus != (integer)TYPUS_C89_MONSTRATOR
        || !silva_c89_typi_compatibiles(
            exemplar->typus->datum.monstrator.internum, functio->typus))
    {
        sprintf(b, "methodus '%.*s' (linea %d): signatura aliena; exspectata"
            " JsonValor* f(JsonValor*, Piscina*, vacuum*, chorda*)",
            (integer)methodus.mensura, (constans character*)methodus.datum,
            (integer)linea_tagi);
        _recusare(f, piscina, b, linea_tagi);
        redde FALSUM;
    }
    redde VERUM;
}

interior chorda
_principem_vitreum_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans character* via,
    constans BriarInventarium* inv,
    BriarFabricaFructus* f)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)4096);
    chorda fen_titulus = briar_nexus_attributum(inv->fenestra, "titulus");
    chorda latitudo    = briar_nexus_attributum(inv->fenestra, "latitudo");
    chorda altitudo    = briar_nexus_attributum(inv->fenestra, "altitudo");
       i32 numerus;
       i32 i;
    character b[64];

    chorda_aedificator_appendere_literis(a, "/* ");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".c - a briar genitum ex ");
    chorda_aedificator_appendere_literis(a, via);
    chorda_aedificator_appendere_literis(a,
        ": fenestra vitrea, methodi regionum praebitae.\n"
        " * Gyrus atrii hic (vide include/atrium.h); status usoris NIHIL"
        " (plan 2). */\n"
        "#include \"latina.h\"\n"
        "#include \"piscina.h\"\n"
        "#include \"chorda.h\"\n"
        "#include \"json.h\"\n"
        "#include \"atrium.h\"\n"
        "#include \"internuntius.h\"\n"
        "#include \"capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, ".h\"\n#include \"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "_regiones.h\"\n"
        "#include <stdio.h>\n"
        "#include <string.h>\n"
        "\n"
        "s32\n"
        "principale (integer argc, character** argv)\n"
        "{\n"
        "    Piscina*            piscina;\n"
        "    AtriumConfiguratio  figura;\n"
        "    Atrium*             atrium;\n"
        "    chorda              causa;\n"
        "\n"
        "    piscina = piscina_generare_dynamicum(\"");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "\", 16777216);\n"
        "    si (piscina == NIHIL)\n"
        "    {\n"
        "        redde I;\n"
        "    }\n"
        "    memset(&figura, 0, magnitudo(figura));\n"
        "    figura.titulus  = \"");
    si (fen_titulus.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_chorda(a, fen_titulus);
    }
    alioquin
    {
        chorda_aedificator_appendere_literis(a, titulus);
    }
    chorda_aedificator_appendere_literis(a, "\";\n");
    si (latitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(latitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): latitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.latitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    si (altitudo.mensura > ZEPHYRUM)
    {
        si (!_numerum_legere(altitudo, &numerus))
        {
            sprintf(b, "<fenestra> (linea %d): altitudo non numerus",
                (integer)inv->fenestra->linea_initium);
            _recusare(f, piscina, b, inv->fenestra->linea_initium);
            redde _vacua();
        }
        sprintf(b, "    figura.altitudo = %d;\n", (integer)numerus);
        chorda_aedificator_appendere_literis(a, b);
    }
    chorda_aedificator_appendere_literis(a, "    figura.capsula  = &capsula_");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        ";\n"
        "    atrium_vexilla_legere(&figura, argc, argv);\n"
        "\n"
        "    atrium = atrium_creare(piscina, &figura, &causa);\n"
        "    si (atrium == NIHIL)\n"
        "    {\n"
        "        imprimere(\"FRACTA: %.*s\\n\", (integer)causa.mensura,\n"
        "            (constans character*)causa.datum);\n"
        "        redde I;\n"
        "    }\n");
    per (i = ZEPHYRUM; i < inv->numerus_app; i++)
    {
        chorda methodus = briar_nexus_attributum(inv->app[i], "methodus");

        si (methodus.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!_methodum_probare(piscina, inv->app[i], methodus, f))
        {
            redde _vacua();
        }
        chorda_aedificator_appendere_literis(a,
            "    (vacuum)internuntius_praebere(atrium_internuntius(atrium),\n"
            "        \"");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, "\", ");
        chorda_aedificator_appendere_chorda(a, methodus);
        chorda_aedificator_appendere_literis(a, ", NIHIL);\n");
    }
    chorda_aedificator_appendere_literis(a,
        "\n"
        "    imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] fenestra aperta\\n\");\n"
        "    si (atrium_portus(atrium) != ZEPHYRUM)\n"
        "    {\n"
        "        imprimere(\"[");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a,
        "] imperium: http://127.0.0.1:%d/imperium\\n\",\n"
        "            (integer)atrium_portus(atrium));\n"
        "    }\n"
        "    fflush(stdout);\n"
        "    atrium_monstrare(atrium);\n"
        "    dum (atrium_currendum(atrium))\n"
        "    {\n"
        "        (vacuum)atrium_gressus(atrium);\n"
        "    }\n"
        "    atrium_destruere(atrium);\n"
        "    piscina_destruere(piscina);\n"
        "    redde ZEPHYRUM;\n"
        "}\n");
    redde chorda_aedificator_finire(a);
}

interior chorda
_toml_fingere (
               Piscina* piscina,
    constans character* titulus,
    constans BriarInventarium* inv)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)512);

    chorda_aedificator_appendere_literis(a,
        "# Capsula frontis (a briar genita) - aedificare.sh eam omni"
        " aedificatione regenerat\n\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_files = [\"index.html\"");
    si (inv->js != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".js\"");
    }
    si (inv->css != NIHIL)
    {
        chorda_aedificator_appendere_literis(a, ", \"");
        chorda_aedificator_appendere_literis(a, titulus);
        chorda_aedificator_appendere_literis(a, ".css\"");
    }
    chorda_aedificator_appendere_literis(a, "]\n");
    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_compress = false\n");
    redde chorda_aedificator_finire(a);
}

/* unio clausurarum per viam (dedup, ordo primae visionis) */
interior Xar*
_clausuras_fundere (
    Piscina* piscina,
        Xar* a,
        Xar* b,
        Xar* c)
{
    Xar* omnes = xar_creare(piscina, (i32)magnitudo(SilexRes));
    TabulaDispersa* viae = tabula_dispersa_creare_chorda(piscina, 64);
    Xar* fontes[3];
    i32  j;

    fontes[0] = a;
    fontes[1] = b;
    fontes[2] = c;
    per (j = ZEPHYRUM; j < III; j++)
    {
        i32 i;

        si (fontes[j] == NIHIL)
        {
            perge;
        }
        per (i = ZEPHYRUM; i < xar_numerus(fontes[j]); i++)
        {
            SilexRes* r = (SilexRes*)xar_obtinere(fontes[j], i);

            si (tabula_dispersa_continet(viae, r->via))
            {
                perge;
            }
            tabula_dispersa_inserere(viae, r->via, (vacuum*)r);
            {
                SilexRes* cella = (SilexRes*)xar_addere(omnes);

                si (cella != NIHIL)
                {
                    *cella = *r;
                }
            }
        }
    }
    redde omnes;
}
```

Replace the `alioquin { /* Task 4 */ … }` branch in `briar_fabricare` with:

```c
    alioquin
    {
        chorda  princeps = _principem_vitreum_fingere(piscina, f.titulus,
            via, &inv, &f);
        chorda  instrumentum;
        chorda* contenta_app;
        chorda* contenta_prob;
        Xar*    clausura_app;
        Xar*    clausura_instrumenti;
        Xar*    clausura_prob = NIHIL;
        b32     inventum = FALSUM;
        i32     n;

        si (!f.successus && f.causa.mensura > ZEPHYRUM)
        {
            redde f;   /* recusatio methodi / attributi */
        }
        si (inv.html == NIHIL)
        {
            _recusare(&f, piscina,
                "<fenestra/> sine regione html: nihil monstrandum",
                inv.fenestra->linea_initium);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, fontes_app[0], princeps);
        _genitam_addere(piscina, f.genitae, "assets/index.html",
            inv.html->contentum);
        si (inv.js != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".js"),
                inv.js->contentum);
        }
        si (inv.css != NIHIL)
        {
            _genitam_addere(piscina, f.genitae,
                _texere(piscina, "assets/", f.titulus, ".css"),
                inv.css->contentum);
        }
        _genitam_addere(piscina, f.genitae,
            _texere(piscina, "assets/", f.titulus, ".toml"),
            _toml_fingere(piscina, f.titulus, &inv));
        instrumentum = silex_fons_legere(fons, "tools/capsula_generare.c",
            piscina, &inventum);
        si (!inventum)
        {
            _recusare(&f, piscina,
                "tools/capsula_generare.c in fonte silicis deest", ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "instrumenta/capsula_generare.c",
            instrumentum);

        contenta_app = (chorda*)piscina_allocare(piscina,
            (memoriae_index)((inv.numerus_app + II) * (i32)magnitudo(chorda)));
        contenta_prob = (chorda*)piscina_allocare(piscina,
            (memoriae_index)((inv.numerus_app + II) * (i32)magnitudo(chorda)));
        per (i = ZEPHYRUM; i < inv.numerus_app; i++)
        {
            contenta_app[i]  = inv.app[i]->contentum;
            contenta_prob[i] = inv.app[i]->contentum;
        }
        contenta_app[inv.numerus_app] = princeps;
        clausura_app = silex_clausuram_e_contentis(piscina, fons, contenta_app,
            inv.numerus_app + I);
        clausura_instrumenti = silex_clausuram_e_contentis(piscina, fons,
            &instrumentum, I);
        n = inv.numerus_app;
        si (inv.probatio != NIHIL)
        {
            contenta_prob[n] = inv.probatio->contentum;
            n = n + I;
            clausura_prob = silex_clausuram_e_contentis(piscina, fons,
                contenta_prob, n);
        }
        si (clausura_app == NIHIL || clausura_instrumenti == NIHIL
            || (inv.probatio != NIHIL && clausura_prob == NIHIL))
        {
            _recusare(&f, piscina, "clausura bibliothecarum fracta", ZEPHYRUM);
            redde f;
        }
        _genitam_addere(piscina, f.genitae, "aedificare.sh",
            silex_ordinem_vitreum_fingere(piscina, f.titulus, fontes_app, II,
                clausura_app, clausura_instrumenti, optiones->fons_titulus));
        si (inv.probatio != NIHIL)
        {
            _genitam_addere(piscina, f.genitae, "probare.sh",
                silex_ordinem_probandi_vitreum_fingere(piscina, f.titulus,
                    fontes_prob, II, clausura_prob, optiones->fons_titulus));
        }
        f.clausura = _clausuras_fundere(piscina, clausura_app,
            clausura_instrumenti, clausura_prob);
    }
```

Note: `_recusare` sets `successus = FALSUM` (already FALSUM from memset) — the check after `_principem_vitreum_fingere` uses `f.causa.mensura > ZEPHYRUM` to detect a refusal from inside. Also the `#define X` numeral is a latina Roman numeral (`X` = 10) — in `_numerum_legere` write `v * X` as above (it is `10`).

Run: `./briar/compile_probationes.sh fabrica` → the vitrea block passes except goldens (absent).

- [ ] **Step 3: Compile the written vitrea project by hand**

```bash
./briar/fabrica.sh briar/probationes/fixa/thistle/salve_vitreum.thistle build/fabrica_manu/salve_vitreum
(cd build/fabrica_manu/salve_vitreum && time ./aedificare.sh && ./probare.sh)
```
Expected: tier I builds `capsula_generare`, tier II compiles sqlite (record the seconds — §10's first number), tier III links `bin/salve_vitreum` with the three frameworks, `aedificatum: bin/salve_vitreum`; `probare.sh` → `OMNIA PRAETERIERUNT`. Do NOT run `bin/salve_vitreum` (opens a window; plan 3's fumus `-agere` does that). Record binary size (`ls -la bin/`) and the cold time in the worklog.

- [ ] **Step 4: Goldens, plant, commit**

`BRIAR_FABRICA_SCRIBERE=1 ./briar/compile_probationes.sh fabrica` (6 more `AURUM SCRIPTUM`), inspect them, then green. Plant: in `_principem_vitreum_fingere` write `figura.capsula = NIHIL;` — golden red + the assertion. Revert. Commit (portae `['briar']`; viae: fabrica.c, test, goldens). Message names the by-hand vitrea build numbers.

---

### Task 5: Refusals

**Files:**
- Create fixtures in `briar/probationes/fixa/thistle/adversa/`: `duo_principalia.thistle`, `nihil.thistle`, `fenestra_et_principale.thistle`, `methodus_absens.thistle`, `methodus_prava.thistle`, `html_duplex.thistle`, `fenestra_prava.thistle`
- Modify: `briar/probationes/probatio_briar_fabrica.c`

- [ ] **Step 1: Fixtures**

`duo_principalia.thistle`:
```
#!/usr/bin/env briar
# Duo

<c!>
s32
principale (vacuum)
{
    redde ZEPHYRUM;
}
</c>

<c!>
s32
principale (vacuum)
{
    redde I;
}
</c>
```
`nihil.thistle`:
```
#!/usr/bin/env briar
# Nihil

<c!>
i32
adiuvare (vacuum)
{
    redde I;
}
</c>
```
`fenestra_et_principale.thistle`:
```
#!/usr/bin/env briar
# Ambo

<fenestra titulus="ambo"/>

<html!>
<p>ambo</p>
</html>

<c!>
s32
principale (vacuum)
{
    redde ZEPHYRUM;
}
</c>
```
`methodus_absens.thistle`:
```
#!/usr/bin/env briar
# Methodus absens

<fenestra titulus="absens"/>

<html!>
<p>absens</p>
</html>

<c! methodus="salve">
#include "json.h"

i32
alia (vacuum)
{
    redde I;
}
</c>
```
`methodus_prava.thistle`:
```
#!/usr/bin/env briar
# Methodus prava

<fenestra titulus="prava"/>

<html!>
<p>prava</p>
</html>

<c! methodus="salve">
#include "json.h"

s32
salve (vacuum)
{
    redde ZEPHYRUM;
}
</c>
```
`html_duplex.thistle`:
```
#!/usr/bin/env briar
# Html duplex

<fenestra titulus="duplex"/>

<html!>
<p>unum</p>
</html>

<html!>
<p>alterum</p>
</html>

<c! methodus="salve">
#include "json.h"

JsonValor*
salve (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    (vacuum)argumenta; (vacuum)datum; (vacuum)culpa;
    redde json_objectum_creare(piscina);
}
</c>
```
`fenestra_prava.thistle`:
```
#!/usr/bin/env briar
# Fenestra prava

<fenestra titulus="prava" latitudo="lata"/>

<html!>
<p>prava</p>
</html>

<c! methodus="salve">
#include "json.h"

JsonValor*
salve (JsonValor* argumenta, Piscina* piscina, vacuum* datum,
    chorda* culpa)
{
    (vacuum)argumenta; (vacuum)datum; (vacuum)culpa;
    redde json_objectum_creare(piscina);
}
</c>
```

- [ ] **Step 2: Assertions**

Add a helper and a block to the test:

```c
interior vacuum
_recusationem_probare (
               Piscina* piscina,
   InternamentumChorda* intern,
    constans SilexFons* fons,
    constans character* fixum,
    constans character* fragmentum,
                   i32  linea)
{
    Xar* nexus;
    BriarFabricaFructus f = _fabricare(piscina, intern, fons,
        _texere(piscina, "briar/probationes/fixa/thistle/adversa/", fixum,
            NIHIL), &nexus);

    imprimere("  %s: %.*s (linea %d)\n", fixum, (integer)f.causa.mensura,
        (constans character*)f.causa.datum, (integer)f.linea_causae);
    CREDO_FALSUM (f.successus);
    CREDO_CHORDA_CONTINET (f.causa, chorda_ex_literis(fragmentum, piscina));
    CREDO_AEQUALIS_I32 (f.linea_causae, linea);
    CREDO_VERUM (f.genitae == NIHIL);
    briar_nexus_silvam_solvere(nexus);
}
```
and in `principale`:
```c
    imprimere("\n--- Probans recusationes: causa + linea .thistle ---\n");
    _recusationem_probare(piscina, intern, fons, "duo_principalia.thistle",
        "duo principalia: lineae 5 et 13", (i32)13);
    _recusationem_probare(piscina, intern, fons, "nihil.thistle",
        "nec principale", ZEPHYRUM);
    _recusationem_probare(piscina, intern, fons,
        "fenestra_et_principale.thistle", "unum elige", (i32)4);
    _recusationem_probare(piscina, intern, fons, "methodus_absens.thistle",
        "functio non definita", (i32)10);
    _recusationem_probare(piscina, intern, fons, "methodus_prava.thistle",
        "signatura aliena", (i32)10);
    _recusationem_probare(piscina, intern, fons, "html_duplex.thistle",
        "regio html iterata (prima linea 6)", (i32)10);
    _recusationem_probare(piscina, intern, fons, "fenestra_prava.thistle",
        "latitudo non numerus", (i32)4);
    _recusationem_probare(piscina, intern, fons, "c_fractum.thistle",
        "parsura fracta", (i32)11);
```
Line numbers, counted in the fixtures above: the `<c!` tag line for the methodus cases is line 10; `<fenestra` is line 4; the second `<html!` is line 10 (first at 6); the second `principale` unit starts at line 13 (first at 5); `@@@` is line 11. Verify each with `grep -n` before trusting the constants; adjust the constants, never the fixtures.

Run: red until the refusal texts match exactly; the implementation from Tasks 3–4 already produces them — fix text mismatches on the implementation side only if a message is worse than the test's; otherwise adjust the fragment.

- [ ] **Step 3: Plant, commit**

Plant: in `briar_fabricare` change `part.principalia > I` to `> II`. Expected: `duo_principalia` red. Revert. `./briar/compile_probationes.sh` → `10/10`. Commit (portae `['briar']`).

---

### Task 6: Seal — docs, spec amendments, memory, ledger

**Files:**
- Modify: `briar/CLAUDE.md`, `briar/fontes/briar.worklog.md`, `briar/probationes/fixa/FONTES.md`, `project-specs/briar-spec.md`
- Memory: `briar-project.md` (+ MEMORY.md line); ledger nota via the MAIN tree's resident.

- [ ] **Step 1: briar/CLAUDE.md**

Add sections: *Fabrica* (what `briar_fabricare` produces, the partition rule, `./briar/fabrica.sh x.thistle dir` and the by-hand `./aedificare.sh` check, goldens under `fixa/fabrica/<t>/` with `BRIAR_FABRICA_SCRIBERE=1` + named cause), *Silva in the nexus* (prelude, exemplar, `briar_nexus_linea_silvae`, `silvam_solvere`), *Limits (plan 2)*: globals in regions are private to `_regiones.c` (share through functions); probatio helpers must be `interior` (`-Wmissing-prototypes`); comments between top-level units are dropped; one region each of html/js/css; the vitrea scripts embed computed closure lists (not goldens); runner cold cost (sqlite + amalgam) with the measured seconds.

- [ ] **Step 2: Worklog**

Dated entry (2026-09-05): what landed, the extent semantics decision (exclusive/inclusive as measured), the prelude byte count method, the toml decision, the `SilexFons.titulus` stamp question for disk sources (plan 3), the first-bake numbers from Task 4 Step 3, any silva quirk met (e.g. which node an ERROR reports).

- [ ] **Step 3: Spec amendments (v1.2)**

- §3.3: prelude lines; `briar_tractator_exemplar`; line formula.
- §4.2: replace the `_regiones.c/.h` row with the partition rule of `briar_fabrica.h`'s head comment; note dropped inter-unit comments.
- §4.4: five generators promoted WITH a source-list parameter (byte-identical output proved); `_toml_fingere` stays static (briar writes its own asset list); the three flag macros public.
- §4.1: for a disk corpus the stamp is the fons title (path) — plan 3 decides (`-struere -iterum` covers dev mode).
- §8: P2 silva DONE, P3 DONE (2026-09-05), P4 → plan 3 (`briar-plan-3-binarium.md`).
- §10: first-bake numbers (sqlite seconds, binary size) from Task 4.

- [ ] **Step 4: FONTES.md** — rows for `punctum.thistle`, the seven adversa, `fixa/fabrica/*` goldens (birth date, regen env, cause rule).

- [ ] **Step 5: Memory + ledger**

`briar-project.md`: state → plan 2 sealed; NEXT = plan 3 (binary: flags, corpus_infixum extraction, struere, cache dir, run/exec, `-probatio`, fumus from outside the repo, first shebang run; stamp for disk corpus). One line in MEMORY.md updated (Edit-anchor, re-read first). Ledger: `mcp__tabularii__gerere` nota on 01M1QR7TXQ from the MAIN session (the worktree runs no resident) — or leave the text in the worklog for the main session to file.

- [ ] **Step 6: Commit**

`commissio_umbra` portae `['briar']` (docs + spec); message: plan 2 sealed, gates `X/X`, numbers.

---

## Self-review (done while writing)

- **Spec coverage:** §3.3 silva kind → Task 1; §4.4 promotion → Task 2; §4.1 key/dir layout → Task 3 (key) + writer; §4.2 files → Tasks 3–4; §4.3 main rule + signature check → Tasks 3–5; §6 `fabrica` gate + `probatio_silex` unchanged → Tasks 3, 2; §8 P3 "no clang in the suite" → by-hand steps only. §4.5 run, §5 binary, fumus → plan 3.
- **Type consistency:** `briar_nexus_silvam_texere(Piscina*, Xar*, constans SilexFons*)` used identically in Tasks 1, 3, 4; `BriarFabricaOptiones` fields used in test, tool, fabrica; `silex_ordinem_*` parameter order (fontes, numerus before closures) identical in header, source, callers.
- **Placeholders:** none; two measured decision points are stated with both branches (extent end; `_texere` return type).
- **Execution order:** Task 2 before Task 1's Step 4 (the promotion provides `silex_clausuram_e_contentis`).
