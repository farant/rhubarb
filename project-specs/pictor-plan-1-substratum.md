# Ludus substratum (pictor P0–P2) Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Build the headless ludus substrate — timestamped events, the two vocabularies (componens tree + draw commands), three-genera state with gates and restore, kinetic struct with quiet flush, the targeting strategy, the action registry, the dispatch loop, an in-process test hand, and the replay harness — proven end to end on a toy app with no window, no font, no pixel.

**Architecture:** Pure pipeline `Eventus[] → derivare → destinatio → actio → componere → arbor componentium`, with drawing (`pingere → mandata`) as a separate pure function of the logical tree. State is an STML node tree in a piscina behind a repository with one write gate per genus; kinetic state is an in-memory struct flushed to the ephemera island only in quiet. Every module is C89 in Latin with a credo probatio; tests snapshot STML text.

**Tech Stack:** C89 (clang, house flags), rhubarb `lib/` — `piscina`, `chorda`, `xar`, `internamentum`, `stml`, `canon`, `credo`, `fenestra`. No new dependencies.

**Spec:** `project-specs/pictor-spec.md` (sections 2, 3, 6.1–6.3, 7, 8 P0–P2). Rationale: `project-specs/ludus-brainstorm.md` §X–XV.

**Scope of THIS plan:** spec §8 phases P0, P1, P2 only. P3–P6 (pictor's document, componentia, figurae, rasterizer, bake) are subsequent plans. The examen CODICES (spec §6.4) are a separate silva/examen plan; this plan writes the annotation comments (`<componens/>`, `<purus/>`, `<quies/>`, `<tractator/>`) so the codices have anchors when they land.

## Global Constraints

- **Language:** C89, compiled with `tools/vexilla.sh` VEXILLA_C89 verbatim: `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512 -O2 -g`. Declarations at block top; `/* */` comments only.
- **Dialect:** `#include "latina.h"` first in every file. Keywords are Latin (`si`, `alioquin`, `per`, `dum`, `redde`, `nomen structura`, `constans`, `vacuum`, `interior` = static fn, `hic_manens` = static data). Small literals as Roman numerals (`I`, `II`, `XVI`, `CCC`, `ZEPHYRUM` = 0). **Never** use `nomen` as an identifier (it is `typedef`); use `titulus`. Types: `i32` (unsigned int), `s32`, `s64`, `b32`, `i8`, `f32`, `memoriae_index`.
- **Strings:** `chorda` = `{ i32 mensura; i8* datum; }`, NOT null-terminated. Use `chorda_ex_literis`, `chorda_aequalis`, `chorda_aequalis_literis`, `chorda_ut_i32`, `chorda_ex_s32`, `chorda_ut_cstr`. Never pass `.datum` to libc string functions.
- **Memory:** all allocation via `Piscina*` (`piscina_allocare`, `piscina_notare` / `piscina_reficere` for mark/reset). No `malloc`/`free` anywhere.
- **Identifiers/comments/messages in Latin.** Technical terms like `offset`, `index`, `token` may stay untranslated.
- **Every new header** starts with `/* <aedilis corpus="lib/<name>.c"/> */` inside the guard (see `include/lapifex_c89.h` for placement), then the house banner. **Every new module** gets `lib/<name>.worklog.md` with a dated birth entry (English allowed in worklogs). **Every module** gets `probationes/probatio_<name>.c` using credo: `credo_aperire(piscina)`, `CREDO_VERUM/FALSUM/NON_NIHIL/NIHIL/AEQUALIS_I32/AEQUALIS_S32/CHORDA_AEQUALIS/CHORDA_AEQUALIS_LITERIS`, then `credo_imprimere_compendium(); redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;`. Entry point is `s32 principale (vacuum)`.
- **Build:** after adding ANY file under `lib/`, `include/`, or `probationes/`, run `./tools/compile_tests_fontes_generare.sh` (regenerates the aedilis-derived source list). Run one suite with `./compile_tests.sh <filter>` (matches `probatio_*<filter>*.c`). Exit 0 = green.
- **Time:** `Eventus.tempus` is `s64` milliseconds. Nothing below `fenestra` calls `tempus_nunc()` or any clock. `QUIES_MS` default 300.
- **Git:** repo is `rhubarb/` (has unrelated uncommitted changes — `git add` ONLY the files named in the task, never `-A`). Subject line `ludus: T<n> - <Latin summary>`; Fran expands bodies in house style.
- **Names SEALED 2026-09-04 (Fran):** `ludus` (the set) · `pictor` (the editor) · `componens`/`componere` · `pingere` · `mandatum`/`mandata` · `figura` · `coetus` · `insula` · `actio` · `partes` · `destinatio` (targeting; `scopus` = scope in the house) · `mobilis`/`Motus` (the third genus; gate `mutare_motum`) · `restituere` (rebuild from islands; the lab's *remontatio* stays as history) · `dispensator` · `derivare` · `delineare_mandata` (the rasterizer, no new noun). Working names still open: `manus_ludus` (until the harnesses unify). All sealed names are in WORDS; no glossary entries needed.

---

## File Structure

| file | responsibility |
|---|---|
| `include/fenestra.h` (modify :194, :939), `lib/fenestra_macos.m` (modify :256 `impellere_eventum`, add helper near :1502) | add `Eventus.tempus`; `fenestra_tempus_ms()`; stamp once at enqueue |
| `include/mandatum.h`, `lib/mandatum.c`, `probationes/probatio_mandatum.c` | six draw primitives, `Mandata` list, coetus spans, STML round-trip, equality |
| `include/componens.h`, `lib/componens.c`, `probationes/probatio_componens.c` | logical node, tree building, STML round-trip, equality, find-by-id |
| `include/derivare.h`, `lib/derivare.c`, `probationes/probatio_derivare.c` | double-click derivation by timestamp |
| `include/eventus_stml.h`, `lib/eventus_stml.c`, `probationes/probatio_eventus_stml.c` | `Eventus[]` ⇄ STML (replay logs) |
| `include/insula.h`, `lib/insula.c`, `probationes/probatio_insula.c` | three-genera repository, gates, restore, mendacium detection, canon hook |
| `include/motus.h`, `lib/motus.c`, `probationes/probatio_motus.c` | in-memory kinetic struct, gate, quiet judgment, flush |
| `include/destinatio.h`, `lib/destinatio.c`, `probationes/probatio_destinatio.c` | strategy interface + default geometric strategy; focus nav; capture |
| `include/actio.h`, `lib/actio.c`, `probationes/probatio_actio.c` | action registry; both-ways resolution |
| `include/dispensator.h`, `lib/dispensator.c`, `probationes/probatio_dispensator.c` | the loop; staleness rule; hover + focus derivation |
| `include/manus_ludus.h`, `lib/manus_ludus.c`, `probationes/probatio_manus_ludus.c` | in-process test hand; `CREDO_MANUS_LUDUS_*` |
| `probationes/pictor_commune.h`, `probationes/pictor/toy.eventus.stml`, `probationes/pictor/toy.arbor.stml`, `probationes/probatio_pictor_toy.c` | exemplar/candidatus/differentia convention; tier-3/4/5 on the toy app |

Shared geometry types (`Punctum`, `Fines`) live in `mandatum.h`; `componens.h` includes it.

---

### Task 1: `Eventus.tempus` — time becomes data in the event

**Files:**
- Modify: `include/fenestra.h:193-222` (the `Eventus` struct) and add one prototype near `fenestra_tempus_obtinere_pulsus` (`:930`)
- Modify: `lib/fenestra_macos.m:256-267` (`impellere_eventum`, the single enqueue path) + add `fenestra_tempus_ms` beside `fenestra_tempus_obtinere_pulsus` (`:1502`)
- Test: `probationes/probatio_fenestra_tempus.c`

**Interfaces:**
- Produces: `Eventus.tempus` (`s64`, ms); `s64 fenestra_tempus_ms(vacuum)`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_fenestra_tempus.c`:
```c
/* probatio_fenestra_tempus.c - Eventus.tempus et fenestra_tempus_ms */
#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
    Piscina* piscina;
        s64  a;
        s64  b;
    Eventus  e;

    piscina = piscina_generare_dynamicum("probatio_fenestra_tempus", IV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);

    imprimere("\n--- Probans fenestra_tempus_ms ---\n");
    a = fenestra_tempus_ms();
    b = fenestra_tempus_ms();
    CREDO_VERUM (a >= ZEPHYRUM);
    CREDO_VERUM (b >= a);

    imprimere("\n--- Probans campum Eventus.tempus ---\n");
    e.genus  = EVENTUS_NIHIL;
    e.tempus = CCC;
    CREDO_VERUM (e.tempus == CCC);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [x] **Step 2: Regenerate the source list and run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh fenestra_tempus`
Expected: FAIL — `no member named 'tempus'` / `implicit declaration of fenestra_tempus_ms`.

- [x] **Step 3: Add the field and the helper**

In `include/fenestra.h` at `:194`, directly after `eventus_genus_t genus;`:
```c
    /* Tempus eventus in MILLISECUNDIS (s64). A fenestra stampatum in
     * productione, a plagula in replay. TEMPUS EST DATUM IN EVENTU -
     * nihil infra fenestram horologium vocat (ludus-brainstorm.md
     * XIV). ZEPHYRUM = nondum stampatum: immittere id implet. */
    s64 tempus;
```
In `include/fenestra.h` after the `fenestra_tempus_obtinere_pulsus` prototype (`:930-932`):
```c
/* Tempus currens in millisecundis - pulsus * M / frequentia. Sedes
 * UNICA horologii pro eventibus. */
s64
fenestra_tempus_ms (
    vacuum);
```
In `lib/fenestra_macos.m`, add the implementation next to `fenestra_tempus_obtinere_pulsus`:
```c
s64
fenestra_tempus_ms (
    vacuum)
{
    f64 pulsus;
    f64 frequentia;   /* pulsus per secundum (f64 - fenestra.h:939) */

    pulsus     = (f64)fenestra_tempus_obtinere_pulsus();
    frequentia = fenestra_tempus_obtinere_frequentiam();
    si (frequentia <= 0.0) { redde ZEPHYRUM; }
    redde (s64)((pulsus * 1000.0) / frequentia);
}
```
**One stamp site.** Every event — the NSEvent construction sites at `:484`, `:671`, `:681` AND both `immittere` functions — reaches the queue through the static `impellere_eventum` (`lib/fenestra_macos.m:256-267`). Stamp there, once, only when the caller left `tempus` at ZEPHYRUM (so a replayed or injected event keeps its own time):
```c
interior vacuum
impellere_eventum (
    Fenestra* fenestra,
    constans Eventus* eventus)
{
    Eventus* sedes;

    si (fenestra->eventus_numerus >= MAXIMUS_EVENTUUM)
    {
        redde; /* Cauda eventuum plena */
    }

    sedes  = &fenestra->eventus[fenestra->eventus_cauda];
    *sedes = *eventus;
    si (sedes->tempus == ZEPHYRUM) { sedes->tempus = fenestra_tempus_ms(); }
    fenestra->eventus_cauda = (fenestra->eventus_cauda + I) % MAXIMUS_EVENTUUM;
    fenestra->eventus_numerus++;
}
```
(Refinement for later, not v1: the `:484` site could use the NSEvent's own `timestamp` — seconds since boot, the same clock family as `mach_absolute_time` — for sub-millisecond fidelity. Enqueue time is within a millisecond of it and keeps the change to one function.)

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh fenestra_tempus`
Expected: PASS, compendium all green. Also run `./compile_tests.sh fenestra` to confirm the existing fenestra probatio still passes.

- [x] **Step 5: Worklog + commit**

Append to `lib/fenestra.worklog.md` (create if absent):
```
## 2026-09-04 — Eventus.tempus

Added s64 tempus (ms) to Eventus and fenestra_tempus_ms(). Stamped ONCE in
impellere_eventum (the single enqueue path) when the caller left it
ZEPHYRUM — construction sites and immittere alike. Reason: ludus
dispatch must be replayable — time is data in the event, never a
clock call below fenestra (ludus-brainstorm.md §XIV).
```
```bash
git add include/fenestra.h lib/fenestra_macos.m lib/fenestra.worklog.md probationes/probatio_fenestra_tempus.c compile_tests_fontes_generata.sh
git commit -m "ludus: T1 - Eventus.tempus (s64 ms) et fenestra_tempus_ms; tempus est datum in eventu"
```

---

### Task 2: `mandatum` — the six draw primitives and the command list

**Files:**
- Create: `include/mandatum.h`, `lib/mandatum.c`, `lib/mandatum.worklog.md`
- Test: `probationes/probatio_mandatum.c`

**Interfaces:**
- Produces: `Punctum {x,y}`, `Fines {x,y,latitudo,altitudo}`, `MandatumGenus`, `ColorMandati`, `Mandatum`, `Mandata`; `mandata_creare(piscina, intern)`, `mandata_vacare(m)`, `mandata_numerus(m)`, `mandata_obtinere(m, i)`, emitters `mandata_rectangulum/linea/polygonum/imago/textus`, `mandata_coetus_incipere(...) → i32`, `mandata_coetus_finire(m, index)`, `mandata_scribere_stml(m, piscina, pulchrum) → chorda`, `mandata_legere_stml(cstr, piscina, intern) → Mandata*`, `mandata_aequalia(a, b) → b32`, `fines_continet(f, p) → b32`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_mandatum.c`:
```c
/* probatio_mandatum.c - Probationes mandatorum (sex primitiva) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "mandatum.h"
#include "credo.h"
#include <stdio.h>

interior ColorMandati
color_thema (i32 valor)
{
    ColorMandati c;
    c.genus = COLOR_MANDATI_THEMA;
    c.valor = valor;
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Mandata* m;
                Mandata* m2;
                  Fines  f;
                Punctum  puncta[III];
                    i32  coetus;
                 chorda  textus;
              Mandatum* md;

    piscina = piscina_generare_dynamicum("probatio_mandatum", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans emissionem et spatia coetuum ---\n");
    m = mandata_creare(piscina, intern);
    CREDO_NON_NIHIL (m);
    CREDO_AEQUALIS_I32 (mandata_numerus(m), ZEPHYRUM);

    f.x = X; f.y = XX; f.latitudo = C; f.altitudo = L;
    coetus = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM, I,
                                     chorda_ex_literis("bottone_servare", piscina));
    CREDO_AEQUALIS_I32 (coetus, ZEPHYRUM);
    mandata_rectangulum(m, f, color_thema(II), VERUM);
    puncta[0].x = ZEPHYRUM; puncta[0].y = ZEPHYRUM;
    puncta[1].x = X;        puncta[1].y = ZEPHYRUM;
    puncta[2].x = V;        puncta[2].y = V;
    mandata_polygonum(m, puncta, III, color_thema(III), FALSUM);
    mandata_textus(m, XII, XXII, chorda_ex_literis("Servare", piscina), ZEPHYRUM, color_thema(I));
    mandata_coetus_finire(m, coetus);
    mandata_linea(m, puncta[0], puncta[1], I, color_thema(I));

    CREDO_AEQUALIS_I32 (mandata_numerus(m), V);
    md = mandata_obtinere(m, ZEPHYRUM);
    CREDO_VERUM (md->genus == MANDATUM_COETUS);
    CREDO_AEQUALIS_I32 (md->magnitudo_arboris, IV);   /* coetus + III liberi */
    md = mandata_obtinere(m, IV);
    CREDO_VERUM (md->genus == MANDATUM_LINEA);
    CREDO_AEQUALIS_I32 (md->magnitudo_arboris, I);

    imprimere("\n--- Probans fines_continet ---\n");
    puncta[0].x = XV; puncta[0].y = XXV;
    CREDO_VERUM  (fines_continet(f, puncta[0]));
    puncta[0].x = V;
    CREDO_FALSUM (fines_continet(f, puncta[0]));

    imprimere("\n--- Probans scribere/legere STML (circuitus) ---\n");
    textus = mandata_scribere_stml(m, piscina, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    m2 = mandata_legere_stml(chorda_ut_cstr(textus, piscina), piscina, intern);
    CREDO_NON_NIHIL (m2);
    CREDO_AEQUALIS_I32 (mandata_numerus(m2), V);
    CREDO_VERUM (mandata_aequalia(m, m2));

    imprimere("\n--- Probans inaequalitatem ---\n");
    md = mandata_obtinere(m2, I);
    md->fines.x = md->fines.x + I;
    CREDO_FALSUM (mandata_aequalia(m, m2));

    imprimere("\n--- Probans vacare ---\n");
    mandata_vacare(m);
    CREDO_AEQUALIS_I32 (mandata_numerus(m), ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh mandatum`
Expected: FAIL — `'mandatum.h' file not found`.

- [x] **Step 3: Write the header**

`include/mandatum.h`:
```c
/* mandatum.h - Mandata delineandi: sex primitiva et index eorum
 *
 * Effusio PURA functionis pingere (ludus-brainstorm.md XV par. 1).
 * Nulla identitas, nulla actio, nulla pars in primitivis - ea in
 * arbore componentium vivunt (componens.h). Coetus solus nidificat:
 * sectio et translatio proprietates coetus sunt, non mandata
 * push/pop, ergo discrepantia structuraliter impossibilis.
 *
 * In memoria: structurae typatae in xar; STML est SERIALIZATIO
 * (imago, inspectio, quaestio, exspectationes replay).
 *
 * USUS:
 *   Mandata* m = mandata_creare(piscina, intern);
 *   i32 c = mandata_coetus_incipere(m, fines, VERUM, 0, 0, I, id);
 *   mandata_rectangulum(m, fines, color, VERUM);
 *   mandata_coetus_finire(m, c);
 *   chorda s = mandata_scribere_stml(m, piscina, VERUM);
 */

#ifndef MANDATUM_H
#define MANDATUM_H

/* <aedilis corpus="lib/mandatum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"


/* ========================================================================
 * GEOMETRIA COMMUNIS (componens.h quoque utitur)
 * ======================================================================== */

nomen structura { i32 x; i32 y; } Punctum;

nomen structura { i32 x; i32 y; i32 latitudo; i32 altitudo; } Fines;

b32
fines_continet (
      Fines  f,
    Punctum  p);


/* ========================================================================
 * TYPI
 * ======================================================================== */

nomen enumeratio {
    MANDATUM_COETUS = ZEPHYRUM,
    MANDATUM_RECTANGULUM,
    MANDATUM_LINEA,
    MANDATUM_POLYGONUM,
    MANDATUM_IMAGO,
    MANDATUM_TEXTUS,
    MANDATUM_GENUS_NUMERUS
} MandatumGenus;

/* Color: signum thematis (thema.h resolvit), index palettae, aut
 * RGBA literalis (0xRRGGBBAA). Rasterizator solus resolvit. */
nomen enumeratio {
    COLOR_MANDATI_THEMA = ZEPHYRUM,
    COLOR_MANDATI_INDEX,
    COLOR_MANDATI_RGBA
} ColorMandatiGenus;

nomen structura {
    ColorMandatiGenus genus;
                  i32 valor;
} ColorMandati;

nomen structura {
    MandatumGenus  genus;
            Fines  fines;
     ColorMandati  color;
              b32  impletum;            /* rectangulum/polygonum */
              i32  crassitudo;          /* linea */
         Punctum*  puncta;              /* linea (II) / polygonum (n) */
              i32  numerus_punctorum;
           chorda  textus;              /* textus: contentum; imago: fons */
              i32  fons;                /* textus: index fontis */
    /* coetus */
              b32  sectio;              /* praecidere ad fines */
          Punctum  translatio;
              i32  scala;               /* >= I, integer */
           chorda  provenientia;        /* id componentis; INSPECTORI SOLI */
              i32  magnitudo_arboris;   /* coetus: mandata in subarbore
                                           se incluso; alia: I */
} Mandatum;

nomen structura {
                    Xar* elementa;      /* Xar de Mandatum */
                Piscina* piscina;
    InternamentumChorda* intern;
} Mandata;


/* ========================================================================
 * INDEX
 * ======================================================================== */

Mandata*
mandata_creare (
                Piscina* piscina,
    InternamentumChorda* intern);

vacuum
mandata_vacare (
    Mandata* m);

i32
mandata_numerus (
    constans Mandata* m);

Mandatum*
mandata_obtinere (
    constans Mandata* m,
                 i32  index);


/* ========================================================================
 * EMISSIO
 * ======================================================================== */

vacuum
mandata_rectangulum (
        Mandata* m,
           Fines fines,
    ColorMandati color,
             b32 impletum);

vacuum
mandata_linea (
        Mandata* m,
         Punctum a,
         Punctum b,
             i32 crassitudo,
    ColorMandati color);

vacuum
mandata_polygonum (
             Mandata* m,
    constans Punctum* puncta,
                  i32 numerus,
         ColorMandati color,
                  b32 impletum);

vacuum
mandata_imago (
    Mandata* m,
      chorda fons,
       Fines fines);

vacuum
mandata_textus (
        Mandata* m,
             i32 x,
             i32 y,
          chorda textus,
             i32 fons,
    ColorMandati color);

/* Redde indicem coetus; finire eum cum eodem indice. */
i32
mandata_coetus_incipere (
    Mandata* m,
       Fines fines,
         b32 sectio,
         i32 translatio_x,
         i32 translatio_y,
         i32 scala,
      chorda provenientia);

vacuum
mandata_coetus_finire (
    Mandata* m,
         i32 index);


/* ========================================================================
 * SERIALIZATIO ET COMPARATIO
 * ======================================================================== */

chorda
mandata_scribere_stml (
    constans Mandata* m,
             Piscina* piscina,
                 b32  pulchrum);

Mandata*
mandata_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

b32
mandata_aequalia (
    constans Mandata* a,
    constans Mandata* b);


#endif /* MANDATUM_H */
```

- [x] **Step 4: Write the implementation**

`lib/mandatum.c`:
```c
/* mandatum.c - Mandata delineandi: sex primitiva et index eorum */

#include "mandatum.h"
#include "stml.h"

#include <string.h>


/* ========================================================================
 * TABULAE
 * ======================================================================== */

hic_manens constans character* tituli_generum[MANDATUM_GENUS_NUMERUS] = {
    "coetus", "rectangulum", "linea", "polygonum", "imago", "textus"
};

hic_manens constans character* tituli_colorum[III] = {
    "thema", "index", "rgba"
};


/* ========================================================================
 * GEOMETRIA
 * ======================================================================== */

b32
fines_continet (
      Fines  f,
    Punctum  p)
{
    redde (p.x >= f.x && p.y >= f.y
        && p.x < f.x + f.latitudo && p.y < f.y + f.altitudo);
}


/* ========================================================================
 * INDEX
 * ======================================================================== */

Mandata*
mandata_creare (
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Mandata* m;

    m = (Mandata*)piscina_allocare(piscina, magnitudo(Mandata));
    si (!m) { redde NIHIL; }
    m->elementa = xar_creare(piscina, (i32)magnitudo(Mandatum));
    m->piscina  = piscina;
    m->intern   = intern;
    redde m;
}

vacuum
mandata_vacare (
    Mandata* m)
{
    xar_vacare(m->elementa);
}

i32
mandata_numerus (
    constans Mandata* m)
{
    redde xar_numerus(m->elementa);
}

Mandatum*
mandata_obtinere (
    constans Mandata* m,
                 i32  index)
{
    redde (Mandatum*)xar_obtinere(m->elementa, index);
}


/* ========================================================================
 * EMISSIO
 * ======================================================================== */

interior Mandatum*
mandatum_novum (
        Mandata* m,
    MandatumGenus genus)
{
    Mandatum* md;

    md = (Mandatum*)xar_addere(m->elementa);
    memset(md, ZEPHYRUM, magnitudo(Mandatum));
    md->genus             = genus;
    md->scala             = I;
    md->magnitudo_arboris = I;
    redde md;
}

interior Punctum*
puncta_copiare (
             Piscina* piscina,
    constans Punctum* puncta,
                  i32 numerus)
{
    Punctum* copia;
    i32      i;

    copia = (Punctum*)piscina_allocare(piscina, magnitudo(Punctum) * (memoriae_index)numerus);
    per (i = ZEPHYRUM; i < numerus; i++) { copia[i] = puncta[i]; }
    redde copia;
}

vacuum
mandata_rectangulum (
        Mandata* m,
           Fines fines,
    ColorMandati color,
             b32 impletum)
{
    Mandatum* md;

    md = mandatum_novum(m, MANDATUM_RECTANGULUM);
    md->fines    = fines;
    md->color    = color;
    md->impletum = impletum;
}

vacuum
mandata_linea (
        Mandata* m,
         Punctum a,
         Punctum b,
             i32 crassitudo,
    ColorMandati color)
{
    Mandatum* md;
    Punctum   ab[II];

    ab[0] = a; ab[1] = b;
    md = mandatum_novum(m, MANDATUM_LINEA);
    md->puncta            = puncta_copiare(m->piscina, ab, II);
    md->numerus_punctorum = II;
    md->crassitudo        = crassitudo;
    md->color             = color;
}

vacuum
mandata_polygonum (
             Mandata* m,
    constans Punctum* puncta,
                  i32 numerus,
         ColorMandati color,
                  b32 impletum)
{
    Mandatum* md;

    md = mandatum_novum(m, MANDATUM_POLYGONUM);
    md->puncta            = puncta_copiare(m->piscina, puncta, numerus);
    md->numerus_punctorum = numerus;
    md->color             = color;
    md->impletum          = impletum;
}

vacuum
mandata_imago (
    Mandata* m,
      chorda fons,
       Fines fines)
{
    Mandatum* md;

    md = mandatum_novum(m, MANDATUM_IMAGO);
    md->textus = fons;
    md->fines  = fines;
}

vacuum
mandata_textus (
        Mandata* m,
             i32 x,
             i32 y,
          chorda textus,
             i32 fons,
    ColorMandati color)
{
    Mandatum* md;

    md = mandatum_novum(m, MANDATUM_TEXTUS);
    md->fines.x = x;
    md->fines.y = y;
    md->textus  = textus;
    md->fons    = fons;
    md->color   = color;
}

i32
mandata_coetus_incipere (
    Mandata* m,
       Fines fines,
         b32 sectio,
         i32 translatio_x,
         i32 translatio_y,
         i32 scala,
      chorda provenientia)
{
    Mandatum* md;
    i32       index;

    index = xar_numerus(m->elementa);
    md = mandatum_novum(m, MANDATUM_COETUS);
    md->fines        = fines;
    md->sectio       = sectio;
    md->translatio.x = translatio_x;
    md->translatio.y = translatio_y;
    md->scala        = scala < I ? I : scala;
    md->provenientia = provenientia;
    redde index;
}

vacuum
mandata_coetus_finire (
    Mandata* m,
         i32 index)
{
    Mandatum* md;

    md = mandata_obtinere(m, index);
    md->magnitudo_arboris = xar_numerus(m->elementa) - index;
}


/* ========================================================================
 * SERIALIZATIO
 * ======================================================================== */

interior constans character*
numerus_ut_literae (
    Piscina* piscina,
         i32 n)
{
    redde chorda_ut_cstr(chorda_ex_s32((s32)n, piscina), piscina);
}

interior vacuum
attributum_numericum (
              StmlNodus* nodus,
                Mandata* m,
     constans character* titulus,
                     i32 n)
{
    stml_attributum_addere(nodus, m->piscina, m->intern, titulus,
                           numerus_ut_literae(m->piscina, n));
}

interior vacuum
puncta_scribere (
    StmlNodus* nodus,
      Mandata* m,
     Mandatum* md)
{
    chorda  s;
    chorda  spatium;
    chorda  comma;
    i32     i;

    s       = chorda_ex_literis("", m->piscina);
    spatium = chorda_ex_literis(" ", m->piscina);
    comma   = chorda_ex_literis(",", m->piscina);
    per (i = ZEPHYRUM; i < md->numerus_punctorum; i++)
    {
        si (i > ZEPHYRUM) { s = chorda_concatenare(s, spatium, m->piscina); }
        s = chorda_concatenare(s, chorda_ex_s32((s32)md->puncta[i].x, m->piscina), m->piscina);
        s = chorda_concatenare(s, comma, m->piscina);
        s = chorda_concatenare(s, chorda_ex_s32((s32)md->puncta[i].y, m->piscina), m->piscina);
    }
    stml_textum_addere(nodus, m->piscina, m->intern, chorda_ut_cstr(s, m->piscina));
}

/* Scribere mandatum ad nodum; redde numerum mandatorum consumptorum. */
interior i32
mandatum_scribere (
    StmlNodus* parens,
      Mandata* m,
           i32 index)
{
    Mandatum*  md;
    StmlNodus* nodus;
    i32        consumpta;
    i32        i;

    md    = mandata_obtinere(m, index);
    nodus = stml_elementum_creare(m->piscina, m->intern, tituli_generum[md->genus]);

    si (md->genus != MANDATUM_TEXTUS && md->genus != MANDATUM_LINEA
        && md->genus != MANDATUM_POLYGONUM)
    {
        attributum_numericum(nodus, m, "x", md->fines.x);
        attributum_numericum(nodus, m, "y", md->fines.y);
        attributum_numericum(nodus, m, "latitudo", md->fines.latitudo);
        attributum_numericum(nodus, m, "altitudo", md->fines.altitudo);
    }
    si (md->genus == MANDATUM_TEXTUS)
    {
        attributum_numericum(nodus, m, "x", md->fines.x);
        attributum_numericum(nodus, m, "y", md->fines.y);
        attributum_numericum(nodus, m, "fons", md->fons);
    }
    si (md->genus != MANDATUM_COETUS && md->genus != MANDATUM_IMAGO)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern, "color_genus",
                               tituli_colorum[md->color.genus]);
        attributum_numericum(nodus, m, "color", md->color.valor);
    }
    si (md->genus == MANDATUM_RECTANGULUM || md->genus == MANDATUM_POLYGONUM)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern, "impletum",
                               md->impletum ? "verum" : "falsum");
    }
    si (md->genus == MANDATUM_LINEA)
    {
        attributum_numericum(nodus, m, "crassitudo", md->crassitudo);
    }
    si (md->genus == MANDATUM_LINEA || md->genus == MANDATUM_POLYGONUM)
    {
        puncta_scribere(nodus, m, md);
    }
    si (md->genus == MANDATUM_TEXTUS || md->genus == MANDATUM_IMAGO)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern,
                               md->genus == MANDATUM_TEXTUS ? "textus" : "fons",
                               chorda_ut_cstr(md->textus, m->piscina));
    }
    si (md->genus == MANDATUM_COETUS)
    {
        stml_attributum_addere(nodus, m->piscina, m->intern, "sectio",
                               md->sectio ? "verum" : "falsum");
        attributum_numericum(nodus, m, "translatio_x", md->translatio.x);
        attributum_numericum(nodus, m, "translatio_y", md->translatio.y);
        attributum_numericum(nodus, m, "scala", md->scala);
        si (md->provenientia.mensura > ZEPHYRUM)
        {
            stml_attributum_addere(nodus, m->piscina, m->intern, "provenientia",
                                   chorda_ut_cstr(md->provenientia, m->piscina));
        }
    }
    stml_liberum_addere(parens, nodus);

    consumpta = I;
    si (md->genus == MANDATUM_COETUS)
    {
        i = index + I;
        dum (i < index + md->magnitudo_arboris)
        {
            i += mandatum_scribere(nodus, m, i);
        }
        consumpta = md->magnitudo_arboris;
    }
    redde consumpta;
}

chorda
mandata_scribere_stml (
    constans Mandata* m,
             Piscina* piscina,
                 b32  pulchrum)
{
    StmlNodus* radix;
    Mandata*   mm;
    i32        i;
    i32        n;

    mm    = (Mandata*)m;   /* scribere non mutat; intern/piscina leguntur */
    radix = stml_elementum_creare(piscina, m->intern, "mandata");
    n     = mandata_numerus(m);
    i     = ZEPHYRUM;
    dum (i < n) { i += mandatum_scribere(radix, mm, i); }
    redde stml_scribere(radix, piscina, pulchrum);
}

interior i32
attributum_i32 (
    StmlNodus* nodus,
    constans character* titulus)
{
    chorda* c;
    i32     v;

    c = stml_attributum_capere(nodus, titulus);
    v = ZEPHYRUM;
    si (c) { chorda_ut_i32(*c, &v); }
    redde v;
}

interior b32
attributum_b32 (
    StmlNodus* nodus,
    constans character* titulus)
{
    chorda* c;

    c = stml_attributum_capere(nodus, titulus);
    redde (c && chorda_aequalis_literis(*c, "verum"));
}

interior chorda
attributum_chorda (
    StmlNodus* nodus,
    constans character* titulus,
      Piscina* piscina)
{
    chorda* c;

    c = stml_attributum_capere(nodus, titulus);
    si (c) { redde *c; }
    redde chorda_ex_literis("", piscina);
}

interior vacuum
puncta_legere (
    StmlNodus* nodus,
     Mandatum* md,
      Piscina* piscina)
{
    chorda                textus;
    chorda_fissio_fructus paria;
    chorda_fissio_fructus xy;
    i32                   i;
    i32                   v;

    textus = stml_textus_valor(nodus, piscina);
    paria  = chorda_fissio(textus, ' ', piscina);
    md->numerus_punctorum = paria.numerus;
    md->puncta = (Punctum*)piscina_allocare(piscina, magnitudo(Punctum) * (memoriae_index)paria.numerus);
    per (i = ZEPHYRUM; i < paria.numerus; i++)
    {
        xy = chorda_fissio(paria.elementa[i], ',', piscina);
        v = ZEPHYRUM; si (xy.numerus > ZEPHYRUM) { chorda_ut_i32(xy.elementa[0], &v); }
        md->puncta[i].x = v;
        v = ZEPHYRUM; si (xy.numerus > I)        { chorda_ut_i32(xy.elementa[1], &v); }
        md->puncta[i].y = v;
    }
}

interior vacuum
mandatum_legere (
    StmlNodus* nodus,
      Mandata* m)
{
    Mandatum*   md;
    i32         index;
    i32         g;
    StmlNodus*  liberum;
    chorda*     cg;

    index = xar_numerus(m->elementa);
    g = ZEPHYRUM;
    dum (g < MANDATUM_GENUS_NUMERUS
         && !chorda_aequalis_literis(*nodus->titulus, tituli_generum[g])) { g++; }
    si (g == MANDATUM_GENUS_NUMERUS) { redde; }

    md = mandatum_novum(m, (MandatumGenus)g);
    md->fines.x        = attributum_i32(nodus, "x");
    md->fines.y        = attributum_i32(nodus, "y");
    md->fines.latitudo = attributum_i32(nodus, "latitudo");
    md->fines.altitudo = attributum_i32(nodus, "altitudo");
    cg = stml_attributum_capere(nodus, "color_genus");
    md->color.genus = COLOR_MANDATI_THEMA;
    si (cg && chorda_aequalis_literis(*cg, "index")) { md->color.genus = COLOR_MANDATI_INDEX; }
    si (cg && chorda_aequalis_literis(*cg, "rgba"))  { md->color.genus = COLOR_MANDATI_RGBA; }
    md->color.valor  = attributum_i32(nodus, "color");
    md->impletum     = attributum_b32(nodus, "impletum");
    md->crassitudo   = attributum_i32(nodus, "crassitudo");
    md->fons         = attributum_i32(nodus, "fons");
    md->sectio       = attributum_b32(nodus, "sectio");
    md->translatio.x = attributum_i32(nodus, "translatio_x");
    md->translatio.y = attributum_i32(nodus, "translatio_y");
    md->scala        = attributum_i32(nodus, "scala");
    si (md->scala < I) { md->scala = I; }
    md->textus       = attributum_chorda(nodus, g == MANDATUM_IMAGO ? "fons" : "textus", m->piscina);
    md->provenientia = attributum_chorda(nodus, "provenientia", m->piscina);

    si (g == MANDATUM_LINEA || g == MANDATUM_POLYGONUM) { puncta_legere(nodus, md, m->piscina); }

    si (g == MANDATUM_COETUS)
    {
        liberum = stml_primus_liberum(nodus);
        dum (liberum)
        {
            si (liberum->genus == STML_NODUS_ELEMENTUM) { mandatum_legere(liberum, m); }
            liberum = stml_frater_proximus(liberum);
        }
        /* md pointer potest mutari post additiones: relegere */
        md = mandata_obtinere(m, index);
        md->magnitudo_arboris = xar_numerus(m->elementa) - index;
    }
}

Mandata*
mandata_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus res;
    Mandata*     m;
    StmlNodus*   liberum;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix) { redde NIHIL; }
    m = mandata_creare(piscina, intern);
    liberum = stml_primus_liberum(res.elementum_radix);
    dum (liberum)
    {
        si (liberum->genus == STML_NODUS_ELEMENTUM) { mandatum_legere(liberum, m); }
        liberum = stml_frater_proximus(liberum);
    }
    redde m;
}


/* ========================================================================
 * COMPARATIO
 * ======================================================================== */

b32
mandata_aequalia (
    constans Mandata* a,
    constans Mandata* b)
{
    i32       n;
    i32       i;
    i32       k;
    Mandatum* x;
    Mandatum* y;

    n = mandata_numerus(a);
    si (n != mandata_numerus(b)) { redde FALSUM; }
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(a, i);
        y = mandata_obtinere(b, i);
        si (x->genus != y->genus)                         { redde FALSUM; }
        si (memcmp(&x->fines, &y->fines, magnitudo(Fines))) { redde FALSUM; }
        si (x->color.genus != y->color.genus || x->color.valor != y->color.valor) { redde FALSUM; }
        si (x->impletum != y->impletum || x->crassitudo != y->crassitudo) { redde FALSUM; }
        si (x->fons != y->fons || x->sectio != y->sectio || x->scala != y->scala) { redde FALSUM; }
        si (x->translatio.x != y->translatio.x || x->translatio.y != y->translatio.y) { redde FALSUM; }
        si (x->magnitudo_arboris != y->magnitudo_arboris) { redde FALSUM; }
        si (x->numerus_punctorum != y->numerus_punctorum) { redde FALSUM; }
        per (k = ZEPHYRUM; k < x->numerus_punctorum; k++)
        {
            si (x->puncta[k].x != y->puncta[k].x || x->puncta[k].y != y->puncta[k].y) { redde FALSUM; }
        }
        si (!chorda_aequalis(x->textus, y->textus))             { redde FALSUM; }
        si (!chorda_aequalis(x->provenientia, y->provenientia)) { redde FALSUM; }
    }
    redde VERUM;
}
```
`STML_NODUS_ELEMENTUM` is the element genus (`include/stml.h`, `StmlNodusGenus`).

- [x] **Step 5: Run to verify it passes**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh mandatum`
Expected: PASS. If `-Wconversion` flags a cast, cast explicitly at the site — never relax the flags.

- [x] **Step 6: Worklog + commit**

`lib/mandatum.worklog.md`:
```
## 2026-09-04 — natus

Six primitives + coetus spans (magnitudo_arboris) in a flat xar;
STML round-trip; equality. Design: pictor-spec.md §3.2, brainstorm
§XV. Coetus carries clip/translate/scale — no push/pop commands
exist. No id/actio/partes on any primitive (those live on componens).
```
```bash
git add include/mandatum.h lib/mandatum.c lib/mandatum.worklog.md probationes/probatio_mandatum.c compile_tests_fontes_generata.sh
git commit -m "ludus: T2 - mandatum: sex primitiva, spatia coetuum, circuitus STML, aequalitas"
```

---

### Task 3: `componens` — the logical tree

**Files:**
- Create: `include/componens.h`, `lib/componens.c`, `lib/componens.worklog.md`
- Test: `probationes/probatio_componens.c`

**Interfaces:**
- Consumes: `Punctum`, `Fines`, `fines_continet` (Task 2).
- Produces: `Partes`, `Praedicatum`, `Componens`; `componens_creare(piscina, intern, id_cstr, partes)`, `componens_addere_liberum(parens, liberum)`, `componens_ponere_fines/actio/titulus/praedicatum/focusabilis/sectio/transformatio`, `componens_numerus_liberorum`, `componens_liberum`, `componens_invenire_per_id(radix, id) → Componens*`, `componens_scribere_stml(radix, piscina, intern, pulchrum) → chorda`, `componens_legere_stml(cstr, piscina, intern) → Componens*`, `componens_aequalis(a, b) → b32`, `partes_titulus(p) → cstr`, `partes_ex_titulo(cstr) → Partes`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_componens.c`:
```c
/* probatio_componens.c - Probationes arboris componentium */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
             Componens* radix;
             Componens* bottone;
             Componens* tabula;
             Componens* inventus;
             Componens* radix2;
                  Fines f;
                 chorda textus;

    piscina = piscina_generare_dynamicum("probatio_componens", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans aedificationem ---\n");
    radix = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    CREDO_NON_NIHIL (radix);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CDLXXX; f.altitudo = CCC;
    componens_ponere_fines(radix, f);

    bottone = componens_creare(piscina, intern, "servare", PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(bottone, f);
    componens_ponere_actio(bottone, "documentum.servare");
    componens_ponere_titulum(bottone, "Servare");
    componens_ponere_focusabilis(bottone, VERUM);
    componens_addere_liberum(radix, bottone);

    tabula = componens_creare(piscina, intern, "tabula", PARTES_TABULA);
    f.x = C; f.y = X; f.latitudo = CCC; f.altitudo = CC;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "penicillus.ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);

    CREDO_AEQUALIS_I32 (componens_numerus_liberorum(radix), II);
    CREDO_VERUM (componens_liberum(radix, I) == tabula);
    CREDO_VERUM (tabula->parens == radix);

    imprimere("\n--- Probans invenire_per_id ---\n");
    inventus = componens_invenire_per_id(radix, chorda_ex_literis("tabula", piscina));
    CREDO_VERUM (inventus == tabula);
    inventus = componens_invenire_per_id(radix, chorda_ex_literis("nemo", piscina));
    CREDO_NIHIL (inventus);

    imprimere("\n--- Probans partes tituli ---\n");
    CREDO_VERUM (partes_ex_titulo("bottone") == PARTES_BOTTONE);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(partes_titulus(PARTES_TABULA), piscina), "tabula");

    imprimere("\n--- Probans circuitum STML ---\n");
    textus = componens_scribere_stml(radix, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    radix2 = componens_legere_stml(chorda_ut_cstr(textus, piscina), piscina, intern);
    CREDO_NON_NIHIL (radix2);
    CREDO_VERUM (componens_aequalis(radix, radix2));
    inventus = componens_invenire_per_id(radix2, chorda_ex_literis("servare", piscina));
    CREDO_NON_NIHIL (inventus);
    CREDO_VERUM (inventus->partes == PARTES_BOTTONE);
    CREDO_VERUM (inventus->focusabilis);
    CREDO_CHORDA_AEQUALIS_LITERIS (inventus->actio, "documentum.servare");

    imprimere("\n--- Probans inaequalitatem ---\n");
    inventus->fines.x = inventus->fines.x + I;
    CREDO_FALSUM (componens_aequalis(radix, radix2));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh componens`
Expected: FAIL — `'componens.h' file not found`.

- [x] **Step 3: Write the header**

`include/componens.h`:
```c
/* componens.h - Arbor componentium: nodus LOGICUS (mores, non formae)
 *
 * Duae arbores numquam fusae (ludus-brainstorm.md XV par. 1): haec
 * fert id, actio, partes, fines, praedicatum, focus, liberos - NULLAM
 * formam. Pingere formas ex hac derivat; destinatio/focus/captura HANC
 * solam ambulant. Componens FINES POSSIDET; pingere AD eos pingit.
 *
 * USUS:
 *   Componens* r = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
 *   componens_ponere_fines(r, fines);
 *   componens_addere_liberum(r, liberum);
 *   chorda s = componens_scribere_stml(r, piscina, intern, VERUM);
 */

#ifndef COMPONENS_H
#define COMPONENS_H

/* <aedilis corpus="lib/componens.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "mandatum.h"   /* Punctum, Fines */


/* ========================================================================
 * TYPI
 * ======================================================================== */

nomen enumeratio {
    PARTES_NULLUM = ZEPHYRUM,
    PARTES_BOTTONE,
    PARTES_CAMPUS,
    PARTES_OPTIO,
    PARTES_INDEX,
    PARTES_ITEM,
    PARTES_DIALOGUS,
    PARTES_TITULUS,
    PARTES_IMAGO,
    PARTES_TABULA,
    PARTES_NUMERUS
} Partes;

/* Praedicatum ictus, escalans: rectangulum (fines) -> polygonum ->
 * masca -> PROPRIUS (componens punctum locale ipse resolvit). */
nomen enumeratio {
    PRAEDICATUM_RECTANGULUM = ZEPHYRUM,
    PRAEDICATUM_POLYGONUM,
    PRAEDICATUM_MASCA,
    PRAEDICATUM_PROPRIUS
} Praedicatum;

nomen structura Componens {
                  chorda  id;                 /* internata; vacua = anonymus */
                  chorda  actio;              /* nomen actionis; vacua = nulla */
                  chorda  titulus;              /* titulus accessibilis (label) */
                  Partes  partes;
                   Fines  fines;              /* in spatio PARENTIS */
             Praedicatum  praedicatum;
                Punctum*  puncta;             /* polygonum */
                     i32  numerus_punctorum;
                     b32  focusabilis;
                     b32  sectio;             /* praecidere liberos ad fines */
                 Punctum  translatio;         /* liberis applicata */
                     i32  scala;              /* >= I */
                    Xar*  liberi;             /* Xar de Componens* */
    structura Componens*  parens;
} Componens;


/* ========================================================================
 * AEDIFICATIO
 * ======================================================================== */

Componens*
componens_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                 Partes  partes);

vacuum componens_addere_liberum     (Componens* parens, Componens* liberum);
vacuum componens_ponere_fines       (Componens* c, Fines fines);
vacuum componens_ponere_actio       (Componens* c, constans character* actio);
vacuum componens_ponere_titulum       (Componens* c, constans character* titulus);
vacuum componens_ponere_praedicatum (Componens* c, Praedicatum p);
vacuum componens_ponere_focusabilis (Componens* c, b32 focusabilis);
vacuum componens_ponere_sectio      (Componens* c, b32 sectio);
vacuum componens_ponere_transformatio (Componens* c, i32 translatio_x, i32 translatio_y, i32 scala);

i32
componens_numerus_liberorum (
    constans Componens* c);

Componens*
componens_liberum (
    constans Componens* c,
                   i32 index);

Componens*
componens_invenire_per_id (
    Componens* radix,
        chorda id);


/* ========================================================================
 * SERIALIZATIO ET COMPARATIO
 * ======================================================================== */

chorda
componens_scribere_stml (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum);

Componens*
componens_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

b32
componens_aequalis (
    constans Componens* a,
    constans Componens* b);

constans character*
partes_titulus (
    Partes p);

Partes
partes_ex_titulo (
    constans character* titulus);


#endif /* COMPONENS_H */
```

- [x] **Step 4: Write the implementation**

`lib/componens.c`:
```c
/* componens.c - Arbor componentium: nodus logicus */

#include "componens.h"
#include "stml.h"

#include <string.h>


hic_manens constans character* tituli_partium[PARTES_NUMERUS] = {
    "nullum", "bottone", "campus", "optio", "index", "item",
    "dialogus", "titulus", "imago", "tabula"
};

hic_manens constans character* tituli_praedicatorum[IV] = {
    "rectangulum", "polygonum", "masca", "proprius"
};


constans character*
partes_titulus (
    Partes p)
{
    si (p < ZEPHYRUM || p >= PARTES_NUMERUS) { redde "nullum"; }
    redde tituli_partium[p];
}

Partes
partes_ex_titulo (
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < PARTES_NUMERUS; i++)
    {
        si (strcmp(titulus, tituli_partium[i]) == ZEPHYRUM) { redde (Partes)i; }
    }
    redde PARTES_NULLUM;
}


/* ========================================================================
 * AEDIFICATIO
 * ======================================================================== */

Componens*
componens_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* id,
                 Partes  partes)
{
    Componens* c;

    c = (Componens*)piscina_allocare(piscina, magnitudo(Componens));
    si (!c) { redde NIHIL; }
    memset(c, ZEPHYRUM, magnitudo(Componens));
    c->id     = *chorda_internare_ex_literis(intern, id);
    c->actio  = *chorda_internare_ex_literis(intern, "");
    c->titulus  = *chorda_internare_ex_literis(intern, "");
    c->partes = partes;
    c->scala  = I;
    c->liberi = xar_creare(piscina, (i32)magnitudo(Componens*));
    redde c;
}

vacuum
componens_addere_liberum (
    Componens* parens,
    Componens* liberum)
{
    Componens** slot;

    slot  = (Componens**)xar_addere(parens->liberi);
    *slot = liberum;
    liberum->parens = parens;
}

vacuum componens_ponere_fines       (Componens* c, Fines fines)            { c->fines = fines; }
vacuum componens_ponere_praedicatum (Componens* c, Praedicatum p)          { c->praedicatum = p; }
vacuum componens_ponere_focusabilis (Componens* c, b32 focusabilis)        { c->focusabilis = focusabilis; }
vacuum componens_ponere_sectio      (Componens* c, b32 sectio)             { c->sectio = sectio; }

vacuum
componens_ponere_transformatio (
    Componens* c,
           i32 translatio_x,
           i32 translatio_y,
           i32 scala)
{
    c->translatio.x = translatio_x;
    c->translatio.y = translatio_y;
    c->scala        = scala < I ? I : scala;
}

/* actio/titulus: chordae internatae absunt hic (intern non in nodo);
 * duplicamus ex literis in piscinam nodi non habemus - ergo
 * consumptor literas STATICAS aut vivas praebeat. Pro ludus: nomina
 * actionum sunt literae constantes registri (actio.h). */
vacuum
componens_ponere_actio (
    Componens* c,
    constans character* actio)
{
    c->actio = *chorda_internare_ex_literis(c->intern, actio);
}

vacuum
componens_ponere_titulum (
    Componens* c,
    constans character* titulus)
{
    c->titulus = *chorda_internare_ex_literis(c->intern, titulus);
}

i32
componens_numerus_liberorum (
    constans Componens* c)
{
    redde xar_numerus(c->liberi);
}

Componens*
componens_liberum (
    constans Componens* c,
                   i32 index)
{
    redde *(Componens**)xar_obtinere(c->liberi, index);
}

Componens*
componens_invenire_per_id (
    Componens* radix,
        chorda id)
{
    i32        i;
    i32        n;
    Componens* inventus;

    si (chorda_aequalis(radix->id, id)) { redde radix; }
    n = componens_numerus_liberorum(radix);
    per (i = ZEPHYRUM; i < n; i++)
    {
        inventus = componens_invenire_per_id(componens_liberum(radix, i), id);
        si (inventus) { redde inventus; }
    }
    redde NIHIL;
}


/* ========================================================================
 * SERIALIZATIO
 * ======================================================================== */

interior vacuum
attributum_numericum (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* titulus,
                     i32 n)
{
    stml_attributum_addere(nodus, piscina, intern, titulus,
                           chorda_ut_cstr(chorda_ex_s32((s32)n, piscina), piscina));
}

interior StmlNodus*
componens_ad_nodum (
              Componens* c,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlNodus* nodus;
    i32        i;
    i32        n;
    chorda     s;
    chorda     spatium;
    chorda     comma;

    nodus = stml_elementum_creare(piscina, intern, "componens");
    stml_attributum_addere(nodus, piscina, intern, "id", chorda_ut_cstr(c->id, piscina));
    stml_attributum_addere(nodus, piscina, intern, "partes", partes_titulus(c->partes));
    si (c->actio.mensura > ZEPHYRUM) { stml_attributum_addere(nodus, piscina, intern, "actio", chorda_ut_cstr(c->actio, piscina)); }
    si (c->titulus.mensura > ZEPHYRUM) { stml_attributum_addere(nodus, piscina, intern, "titulus", chorda_ut_cstr(c->titulus, piscina)); }
    attributum_numericum(nodus, piscina, intern, "x", c->fines.x);
    attributum_numericum(nodus, piscina, intern, "y", c->fines.y);
    attributum_numericum(nodus, piscina, intern, "latitudo", c->fines.latitudo);
    attributum_numericum(nodus, piscina, intern, "altitudo", c->fines.altitudo);
    stml_attributum_addere(nodus, piscina, intern, "praedicatum", tituli_praedicatorum[c->praedicatum]);
    stml_attributum_addere(nodus, piscina, intern, "focusabilis", c->focusabilis ? "verum" : "falsum");
    stml_attributum_addere(nodus, piscina, intern, "sectio", c->sectio ? "verum" : "falsum");
    attributum_numericum(nodus, piscina, intern, "translatio_x", c->translatio.x);
    attributum_numericum(nodus, piscina, intern, "translatio_y", c->translatio.y);
    attributum_numericum(nodus, piscina, intern, "scala", c->scala);

    si (c->numerus_punctorum > ZEPHYRUM)
    {
        s = chorda_ex_literis("", piscina);
        spatium = chorda_ex_literis(" ", piscina);
        comma   = chorda_ex_literis(",", piscina);
        per (i = ZEPHYRUM; i < c->numerus_punctorum; i++)
        {
            si (i > ZEPHYRUM) { s = chorda_concatenare(s, spatium, piscina); }
            s = chorda_concatenare(s, chorda_ex_s32((s32)c->puncta[i].x, piscina), piscina);
            s = chorda_concatenare(s, comma, piscina);
            s = chorda_concatenare(s, chorda_ex_s32((s32)c->puncta[i].y, piscina), piscina);
        }
        stml_attributum_addere(nodus, piscina, intern, "puncta", chorda_ut_cstr(s, piscina));
    }

    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        stml_liberum_addere(nodus, componens_ad_nodum(componens_liberum(c, i), piscina, intern));
    }
    redde nodus;
}

chorda
componens_scribere_stml (
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum)
{
    redde stml_scribere(componens_ad_nodum(radix, piscina, intern), piscina, pulchrum);
}

interior i32
attributum_i32 (StmlNodus* nodus, constans character* titulus)
{
    chorda* c; i32 v;
    c = stml_attributum_capere(nodus, titulus);
    v = ZEPHYRUM;
    si (c) { chorda_ut_i32(*c, &v); }
    redde v;
}

interior b32
attributum_b32 (StmlNodus* nodus, constans character* titulus)
{
    chorda* c;
    c = stml_attributum_capere(nodus, titulus);
    redde (c && chorda_aequalis_literis(*c, "verum"));
}

interior Componens*
nodus_ad_componens (
              StmlNodus* nodus,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    Componens*            c;
    chorda*               a;
    StmlNodus*            liberum;
    chorda_fissio_fructus paria;
    chorda_fissio_fructus xy;
    i32                   i;
    i32                   v;
    i32                   p;

    a = stml_attributum_capere(nodus, "id");
    c = componens_creare(piscina, intern, a ? chorda_ut_cstr(*a, piscina) : "", PARTES_NULLUM);
    a = stml_attributum_capere(nodus, "partes");
    si (a) { c->partes = partes_ex_titulo(chorda_ut_cstr(*a, piscina)); }
    a = stml_attributum_capere(nodus, "actio");
    si (a) { componens_ponere_actio(c, chorda_ut_cstr(*a, piscina)); }
    a = stml_attributum_capere(nodus, "titulus");
    si (a) { componens_ponere_titulum(c, chorda_ut_cstr(*a, piscina)); }
    c->fines.x        = attributum_i32(nodus, "x");
    c->fines.y        = attributum_i32(nodus, "y");
    c->fines.latitudo = attributum_i32(nodus, "latitudo");
    c->fines.altitudo = attributum_i32(nodus, "altitudo");
    a = stml_attributum_capere(nodus, "praedicatum");
    c->praedicatum = PRAEDICATUM_RECTANGULUM;
    si (a)
    {
        per (p = ZEPHYRUM; p < IV; p++)
        {
            si (chorda_aequalis_literis(*a, tituli_praedicatorum[p])) { c->praedicatum = (Praedicatum)p; }
        }
    }
    c->focusabilis  = attributum_b32(nodus, "focusabilis");
    c->sectio       = attributum_b32(nodus, "sectio");
    c->translatio.x = attributum_i32(nodus, "translatio_x");
    c->translatio.y = attributum_i32(nodus, "translatio_y");
    c->scala        = attributum_i32(nodus, "scala");
    si (c->scala < I) { c->scala = I; }

    a = stml_attributum_capere(nodus, "puncta");
    si (a && a->mensura > ZEPHYRUM)
    {
        paria = chorda_fissio(*a, ' ', piscina);
        c->numerus_punctorum = paria.numerus;
        c->puncta = (Punctum*)piscina_allocare(piscina, magnitudo(Punctum) * (memoriae_index)paria.numerus);
        per (i = ZEPHYRUM; i < paria.numerus; i++)
        {
            xy = chorda_fissio(paria.elementa[i], ',', piscina);
            v = ZEPHYRUM; si (xy.numerus > ZEPHYRUM) { chorda_ut_i32(xy.elementa[0], &v); }
            c->puncta[i].x = v;
            v = ZEPHYRUM; si (xy.numerus > I)        { chorda_ut_i32(xy.elementa[1], &v); }
            c->puncta[i].y = v;
        }
    }

    liberum = stml_primus_liberum(nodus);
    dum (liberum)
    {
        si (liberum->genus == STML_NODUS_ELEMENTUM)
        {
            componens_addere_liberum(c, nodus_ad_componens(liberum, piscina, intern));
        }
        liberum = stml_frater_proximus(liberum);
    }
    redde c;
}

Componens*
componens_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus res;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix) { redde NIHIL; }
    redde nodus_ad_componens(res.elementum_radix, piscina, intern);
}


/* ========================================================================
 * COMPARATIO
 * ======================================================================== */

b32
componens_aequalis (
    constans Componens* a,
    constans Componens* b)
{
    i32 i;
    i32 n;

    si (!chorda_aequalis(a->id, b->id))       { redde FALSUM; }
    si (!chorda_aequalis(a->actio, b->actio)) { redde FALSUM; }
    si (!chorda_aequalis(a->titulus, b->titulus)) { redde FALSUM; }
    si (a->partes != b->partes)               { redde FALSUM; }
    si (memcmp(&a->fines, &b->fines, magnitudo(Fines))) { redde FALSUM; }
    si (a->praedicatum != b->praedicatum)     { redde FALSUM; }
    si (a->focusabilis != b->focusabilis || a->sectio != b->sectio) { redde FALSUM; }
    si (a->translatio.x != b->translatio.x || a->translatio.y != b->translatio.y || a->scala != b->scala) { redde FALSUM; }
    si (a->numerus_punctorum != b->numerus_punctorum) { redde FALSUM; }
    per (i = ZEPHYRUM; i < a->numerus_punctorum; i++)
    {
        si (a->puncta[i].x != b->puncta[i].x || a->puncta[i].y != b->puncta[i].y) { redde FALSUM; }
    }
    n = componens_numerus_liberorum(a);
    si (n != componens_numerus_liberorum(b)) { redde FALSUM; }
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (!componens_aequalis(componens_liberum(a, i), componens_liberum(b, i))) { redde FALSUM; }
    }
    redde VERUM;
}
```
Note on `componens_ponere_actio`/`_nomen`: they alias the caller's C string (no copy) because action names are registry literals and labels come from interned or piscina-owned text. If a caller passes a temporary, intern it first with `chorda_internare_ex_literis` and pass `chorda_ut_cstr` of that. This is documented in the header comment; keep it.

- [x] **Step 5: Run to verify it passes**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh componens`
Expected: PASS.

- [x] **Step 6: Worklog + commit**

`lib/componens.worklog.md`:
```
## 2026-09-04 — natus

Logical node: id/actio/titulus/partes/fines/praedicatum/focusabilis +
clip/translate/scale (targeting needs them; pingere copies them onto
the coetus). STML round-trip, equality, find-by-id. Design:
pictor-spec.md §3.1. actio/titulus alias caller strings (registry
literals) — documented in the header.
```
```bash
git add include/componens.h lib/componens.c lib/componens.worklog.md probationes/probatio_componens.c compile_tests_fontes_generata.sh
git commit -m "ludus: T3 - componens: nodus logicus, arbor, circuitus STML, aequalitas, invenire_per_id"
```

---

### Task 4: `derivare` — double-click from timestamps

**Files:**
- Create: `include/derivare.h`, `lib/derivare.c`, `lib/derivare.worklog.md`
- Test: `probationes/probatio_derivare.c`

**Interfaces:**
- Consumes: `Eventus` with `tempus` (Task 1), `Punctum` (Task 2).
- Produces: `Derivator`; `derivator_initiare(d, intervallum_ms, distantia)`; `derivare(d, ev, effusio)` appending to a `Xar` of `Eventus`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_derivare.c`:
```c
/* probatio_derivare.c - derivatio eventuum: duplex ex tempore */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "fenestra.h"
#include "derivare.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
mus_depressus (s64 tempus, i32 x, i32 y)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus        = EVENTUS_MUS_DEPRESSUS;
    e.tempus       = tempus;
    e.datum.mus.x  = x;
    e.datum.mus.y  = y;
    redde e;
}

s32 principale (vacuum)
{
     Piscina* piscina;
    Derivator d;
         Xar* effusio;
      Eventus e;
     Eventus* ultimus;

    piscina = piscina_generare_dynamicum("probatio_derivare", XVI * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    effusio = xar_creare(piscina, (i32)magnitudo(Eventus));
    derivator_initiare(&d, CCC, IV);

    imprimere("\n--- Duo ictus intra CCC ms et IV px -> DUPLEX ---\n");
    e = mus_depressus(M, X, X);      derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);
    e = mus_depressus(M + CC, XII, XI); derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), III);   /* depressus + DUPLEX */
    ultimus = (Eventus*)xar_obtinere(effusio, II);
    CREDO_VERUM (ultimus->genus == EVENTUS_MUS_DUPLEX);
    CREDO_VERUM (ultimus->tempus == M + CC);

    imprimere("\n--- Tertius ictus statim NON triplex-ut-duplex ---\n");
    e = mus_depressus(M + CCL, XII, XI); derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), IV);    /* solum depressus */

    imprimere("\n--- Duo ictus longe in tempore -> nullus DUPLEX ---\n");
    xar_vacare(effusio);
    e = mus_depressus(V * M, X, X);        derivare(&d, &e, effusio);
    e = mus_depressus(V * M + DC, X, X);   derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- Duo ictus longe in spatio -> nullus DUPLEX ---\n");
    xar_vacare(effusio);
    e = mus_depressus(X * M, X, X);        derivare(&d, &e, effusio);
    e = mus_depressus(X * M + C, C, C);    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), II);

    imprimere("\n--- Eventus non-mus transit immutatus ---\n");
    xar_vacare(effusio);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_CLAVIS_DEPRESSUS; e.tempus = XX * M;
    derivare(&d, &e, effusio);
    CREDO_AEQUALIS_I32 (xar_numerus(effusio), I);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh derivare`
Expected: FAIL — `'derivare.h' file not found`.

- [x] **Step 3: Write header and implementation**

`include/derivare.h`:
```c
/* derivare.h - Derivatio eventuum PURA ex fluxu crudo
 *
 * Duplex ictus ex TEMPORE eventuum (numquam ex horologio), gradus
 * primus. Hover/focus derivantur in dispensatore post scopum (id
 * geometricum poscunt). Independens a strategia scopi.
 *
 * <purus/>
 */

#ifndef DERIVARE_H
#define DERIVARE_H

/* <aedilis corpus="lib/derivare.c"/> */

#include "latina.h"
#include "xar.h"
#include "fenestra.h"
#include "mandatum.h"   /* Punctum */

nomen structura {
        s64  tempus_ultimi;
    Punctum  ultimus;
        b32  habet_ultimum;
        s64  intervallum_ms;
        i32  distantia;
} Derivator;

vacuum
derivator_initiare (
    Derivator* d,
          s64  intervallum_ms,
          i32  distantia);

/* Appendit eventum crudum, deinde derivata (DUPLEX) ad effusionem
 * (Xar de Eventus). */
vacuum
derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio);

#endif /* DERIVARE_H */
```
`lib/derivare.c`:
```c
/* derivare.c - Derivatio eventuum pura */

#include "derivare.h"

interior i32
abs_i32 (s32 v) { redde (i32)(v < ZEPHYRUM ? -v : v); }

vacuum
derivator_initiare (
    Derivator* d,
          s64  intervallum_ms,
          i32  distantia)
{
    d->tempus_ultimi  = ZEPHYRUM;
    d->ultimus.x      = ZEPHYRUM;
    d->ultimus.y      = ZEPHYRUM;
    d->habet_ultimum  = FALSUM;
    d->intervallum_ms = intervallum_ms;
    d->distantia      = distantia;
}

vacuum
derivare (
           Derivator* d,
    constans Eventus* ev,
                 Xar* effusio)
{
    Eventus* slot;
    b32      duplex;

    slot  = (Eventus*)xar_addere(effusio);
    *slot = *ev;

    si (ev->genus != EVENTUS_MUS_DEPRESSUS) { redde; }

    duplex = d->habet_ultimum
          && (ev->tempus - d->tempus_ultimi) <= d->intervallum_ms
          && abs_i32((s32)ev->datum.mus.x - (s32)d->ultimus.x) <= d->distantia
          && abs_i32((s32)ev->datum.mus.y - (s32)d->ultimus.y) <= d->distantia;

    si (duplex)
    {
        slot  = (Eventus*)xar_addere(effusio);
        *slot = *ev;
        slot->genus = EVENTUS_MUS_DUPLEX;
        d->habet_ultimum = FALSUM;   /* ne triplex fiat duplex */
    }
    alioquin
    {
        d->tempus_ultimi = ev->tempus;
        d->ultimus.x     = ev->datum.mus.x;
        d->ultimus.y     = ev->datum.mus.y;
        d->habet_ultimum = VERUM;
    }
}
```

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh derivare`
Expected: PASS.

- [x] **Step 5: Worklog + commit**

`lib/derivare.worklog.md`:
```
## 2026-09-04 — natus

Double-click by event timestamps (interval + distance), reset after
a double so a triple isn't a second double. Replaces the wall-clock
tempus_nunc() logic in widget.c's manager (unreplayable). Hover and
focus derivation live in dispensator (need the targeting result).
```
```bash
git add include/derivare.h lib/derivare.c lib/derivare.worklog.md probationes/probatio_derivare.c compile_tests_fontes_generata.sh
git commit -m "ludus: T4 - derivare: duplex ex tempore eventuum, purum"
```

---

### Task 5: `eventus_stml` — replay logs as STML

**Files:**
- Create: `include/eventus_stml.h`, `lib/eventus_stml.c`, `lib/eventus_stml.worklog.md`
- Test: `probationes/probatio_eventus_stml.c`

**Interfaces:**
- Consumes: `Eventus` (Task 1).
- Produces: `eventus_scribere_stml(eventus_xar, piscina, intern, pulchrum) → chorda`, `eventus_legere_stml(cstr, piscina, intern) → Xar*` (of `Eventus`), `eventus_genus_titulus(genus) → cstr`, `eventus_genus_ex_titulo(cstr) → eventus_genus_t`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_eventus_stml.c`:
```c
/* probatio_eventus_stml.c - Eventus[] <-> STML (plagulae replay) */
#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "eventus_stml.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    Xar* index;
                    Xar* index2;
                Eventus* e;
                 chorda  textus;

    piscina = piscina_generare_dynamicum("probatio_eventus_stml", XVI * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans tituli generum ---\n");
    CREDO_VERUM (eventus_genus_ex_titulo("mus_depressus") == EVENTUS_MUS_DEPRESSUS);
    CREDO_VERUM (strcmp(eventus_genus_titulus(EVENTUS_CLAVIS_LIBERATUS), "clavis_liberatus") == ZEPHYRUM);

    imprimere("\n--- Probans circuitum ---\n");
    index = xar_creare(piscina, (i32)magnitudo(Eventus));
    e = (Eventus*)xar_addere(index); memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus = EVENTUS_MUS_DEPRESSUS; e->tempus = M; e->datum.mus.x = X; e->datum.mus.y = XX; e->datum.mus.modificantes = II;
    e = (Eventus*)xar_addere(index); memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus = EVENTUS_CLAVIS_DEPRESSUS; e->tempus = M + L; e->datum.clavis.typus = 'p';
    e = (Eventus*)xar_addere(index); memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus = EVENTUS_MUS_ROTULA; e->tempus = M + C; e->datum.rotula.delta_y = -1.5f;
    e = (Eventus*)xar_addere(index); memset(e, ZEPHYRUM, magnitudo(Eventus));
    e->genus = EVENTUS_MUTARE_MAGNITUDINEM; e->tempus = M + CC; e->datum.mutare_magnitudinem.latitudo = CDLXXX; e->datum.mutare_magnitudinem.altitudo = CCC;

    textus = eventus_scribere_stml(index, piscina, intern, VERUM);
    CREDO_CHORDA_NON_VACUA (textus);
    index2 = eventus_legere_stml(chorda_ut_cstr(textus, piscina), piscina, intern);
    CREDO_NON_NIHIL (index2);
    CREDO_AEQUALIS_I32 (xar_numerus(index2), IV);
    e = (Eventus*)xar_obtinere(index2, ZEPHYRUM);
    CREDO_VERUM (e->genus == EVENTUS_MUS_DEPRESSUS && e->tempus == M && e->datum.mus.x == X && e->datum.mus.modificantes == II);
    e = (Eventus*)xar_obtinere(index2, I);
    CREDO_VERUM (e->genus == EVENTUS_CLAVIS_DEPRESSUS && e->datum.clavis.typus == 'p');
    e = (Eventus*)xar_obtinere(index2, II);
    CREDO_VERUM (e->datum.rotula.delta_y < -1.4f && e->datum.rotula.delta_y > -1.6f);
    e = (Eventus*)xar_obtinere(index2, III);
    CREDO_AEQUALIS_I32 (e->datum.mutare_magnitudinem.latitudo, CDLXXX);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh eventus_stml`
Expected: FAIL — `'eventus_stml.h' file not found`.

- [x] **Step 3: Write header and implementation**

`include/eventus_stml.h`:
```c
/* eventus_stml.h - Eventus[] <-> STML: plagulae replay
 *
 * Forma:
 *   <eventus_index>
 *     <eventus genus="mus_depressus" tempus="1000" x="10" y="20" botton="0" modificantes="2"/>
 *     <eventus genus="clavis_depressus" tempus="1050" clavis="0" typus="112" modificantes="0"/>
 *     <eventus genus="mus_rotula" tempus="1100" delta_x="0.000" delta_y="-1.500"/>
 *     <eventus genus="mutare_magnitudinem" tempus="1200" latitudo="480" altitudo="300"/>
 *   </eventus_index>
 * typus scribitur ut numerus (codex characteris).
 */

#ifndef EVENTUS_STML_H
#define EVENTUS_STML_H

/* <aedilis corpus="lib/eventus_stml.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"

constans character*
eventus_genus_titulus (
    eventus_genus_t genus);

eventus_genus_t
eventus_genus_ex_titulo (
    constans character* titulus);

chorda
eventus_scribere_stml (
           constans Xar* eventus,   /* Xar de Eventus */
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum);

Xar*
eventus_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern);

#endif /* EVENTUS_STML_H */
```
`lib/eventus_stml.c`:
```c
/* eventus_stml.c - Eventus[] <-> STML */

#include "eventus_stml.h"
#include "stml.h"

#include <string.h>

/* Ordo = ordo enumerationis eventus_genus_t (fenestra.h). Extende
 * UNA cum enumeratione. */
hic_manens constans character* tituli[] = {
    "nihil", "claudere", "mutare_magnitudinem", "focus", "defocus",
    "exponere", "clavis_depressus", "clavis_liberatus", "mus_depressus",
    "mus_liberatus", "mus_motus", "mus_rotula", "mus_duplex",
    "mus_intravit", "mus_exiit", "focus_captus", "focus_amissus", "focus_petitus"
};
#define TITULI_NUMERUS ((i32)(magnitudo(tituli) / magnitudo(tituli[0])))

constans character*
eventus_genus_titulus (
    eventus_genus_t genus)
{
    si ((i32)genus >= TITULI_NUMERUS) { redde "ignotum"; }
    redde tituli[genus];
}

eventus_genus_t
eventus_genus_ex_titulo (
    constans character* titulus)
{
    i32 i;
    per (i = ZEPHYRUM; i < TITULI_NUMERUS; i++)
    {
        si (strcmp(titulus, tituli[i]) == ZEPHYRUM) { redde (eventus_genus_t)i; }
    }
    redde EVENTUS_NIHIL;
}

interior vacuum
attr_i (StmlNodus* n, Piscina* p, InternamentumChorda* in, constans character* t, s64 v)
{
    stml_attributum_addere(n, p, in, t, chorda_ut_cstr(chorda_ex_s32((s32)v, p), p));
}

interior vacuum
attr_f (StmlNodus* n, Piscina* p, InternamentumChorda* in, constans character* t, f32 v)
{
    stml_attributum_addere(n, p, in, t, chorda_ut_cstr(chorda_ex_f64((f64)v, III, p), p));
}

chorda
eventus_scribere_stml (
           constans Xar* eventus,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  pulchrum)
{
    StmlNodus* radix;
    StmlNodus* n;
    Eventus*   e;
    i32        i;
    i32        num;

    radix = stml_elementum_creare(piscina, intern, "eventus_index");
    num   = xar_numerus(eventus);
    per (i = ZEPHYRUM; i < num; i++)
    {
        e = (Eventus*)xar_obtinere(eventus, i);
        n = stml_elementum_creare(piscina, intern, "eventus");
        stml_attributum_addere(n, piscina, intern, "genus", eventus_genus_titulus(e->genus));
        attr_i(n, piscina, intern, "tempus", e->tempus);
        commutatio (e->genus)
        {
            casus EVENTUS_MUS_DEPRESSUS: casus EVENTUS_MUS_LIBERATUS:
            casus EVENTUS_MUS_MOTUS:     casus EVENTUS_MUS_DUPLEX:
                attr_i(n, piscina, intern, "x", e->datum.mus.x);
                attr_i(n, piscina, intern, "y", e->datum.mus.y);
                attr_i(n, piscina, intern, "botton", (s64)e->datum.mus.botton);
                attr_i(n, piscina, intern, "modificantes", e->datum.mus.modificantes);
                frange;
            casus EVENTUS_CLAVIS_DEPRESSUS: casus EVENTUS_CLAVIS_LIBERATUS:
                attr_i(n, piscina, intern, "clavis", (s64)e->datum.clavis.clavis);
                attr_i(n, piscina, intern, "typus", (s64)(insignatus character)e->datum.clavis.typus);
                attr_i(n, piscina, intern, "modificantes", e->datum.clavis.modificantes);
                frange;
            casus EVENTUS_MUS_ROTULA:
                attr_f(n, piscina, intern, "delta_x", e->datum.rotula.delta_x);
                attr_f(n, piscina, intern, "delta_y", e->datum.rotula.delta_y);
                frange;
            casus EVENTUS_MUTARE_MAGNITUDINEM:
                attr_i(n, piscina, intern, "latitudo", e->datum.mutare_magnitudinem.latitudo);
                attr_i(n, piscina, intern, "altitudo", e->datum.mutare_magnitudinem.altitudo);
                frange;
            ordinarius: frange;
        }
        stml_liberum_addere(radix, n);
    }
    redde stml_scribere(radix, piscina, pulchrum);
}

interior s64
capere_i (StmlNodus* n, constans character* t)
{
    chorda* c; s32 v;
    c = stml_attributum_capere(n, t);
    v = ZEPHYRUM;
    si (c) { chorda_ut_s32(*c, &v); }
    redde (s64)v;
}

interior f32
capere_f (StmlNodus* n, constans character* t)
{
    chorda* c; f64 v;
    c = stml_attributum_capere(n, t);
    v = 0.0;
    si (c) { chorda_ut_f64(*c, &v); }
    redde (f32)v;
}

Xar*
eventus_legere_stml (
     constans character* cstr,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    StmlResultus res;
    Xar*         index;
    StmlNodus*   n;
    Eventus*     e;
    chorda*      g;

    res = stml_legere_ex_literis(cstr, piscina, intern);
    si (!res.successus || !res.elementum_radix) { redde NIHIL; }
    index = xar_creare(piscina, (i32)magnitudo(Eventus));
    n = stml_primus_liberum(res.elementum_radix);
    dum (n)
    {
        si (n->genus == STML_NODUS_ELEMENTUM)
        {
            e = (Eventus*)xar_addere(index);
            memset(e, ZEPHYRUM, magnitudo(Eventus));
            g = stml_attributum_capere(n, "genus");
            e->genus  = g ? eventus_genus_ex_titulo(chorda_ut_cstr(*g, piscina)) : EVENTUS_NIHIL;
            e->tempus = capere_i(n, "tempus");
            e->datum.mus.x = (i32)capere_i(n, "x");
            e->datum.mus.y = (i32)capere_i(n, "y");
            si (e->genus == EVENTUS_CLAVIS_DEPRESSUS || e->genus == EVENTUS_CLAVIS_LIBERATUS)
            {
                e->datum.clavis.clavis       = (clavis_t)capere_i(n, "clavis");
                e->datum.clavis.typus        = (character)capere_i(n, "typus");
                e->datum.clavis.modificantes = (i32)capere_i(n, "modificantes");
            }
            alioquin si (e->genus == EVENTUS_MUS_ROTULA)
            {
                e->datum.rotula.delta_x = capere_f(n, "delta_x");
                e->datum.rotula.delta_y = capere_f(n, "delta_y");
            }
            alioquin si (e->genus == EVENTUS_MUTARE_MAGNITUDINEM)
            {
                e->datum.mutare_magnitudinem.latitudo = (i32)capere_i(n, "latitudo");
                e->datum.mutare_magnitudinem.altitudo = (i32)capere_i(n, "altitudo");
            }
            alioquin
            {
                e->datum.mus.botton       = (mus_botton_t)capere_i(n, "botton");
                e->datum.mus.modificantes = (i32)capere_i(n, "modificantes");
            }
        }
        n = stml_frater_proximus(n);
    }
    redde index;
}
```
The five trailing titles (`mus_intravit` … `focus_petitus`) name derived genera that Task 10 adds to `eventus_genus_t`; until Task 10 lands they are unused entries and `TITULI_NUMERUS` guards the lookup.

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh eventus_stml` — Expected: PASS.

- [x] **Step 5: Worklog + commit**

`lib/eventus_stml.worklog.md`: `## 2026-09-04 — natus` + "Eventus[] ⇄ STML for replay logs; title table must be extended with the enum (Task 10 adds five derived genera)."
```bash
git add include/eventus_stml.h lib/eventus_stml.c lib/eventus_stml.worklog.md probationes/probatio_eventus_stml.c compile_tests_fontes_generata.sh
git commit -m "ludus: T5 - eventus_stml: plagulae replay, circuitus Eventus[] <-> STML"
```

---

### Task 6: `insula` — three-genera repository, gates, restore, mendacium

**Files:**
- Create: `include/insula.h`, `lib/insula.c`, `lib/insula.worklog.md`
- Test: `probationes/probatio_insula.c`

**Interfaces:**
- Consumes: `stml`, `canon` (exist).
- Produces: `InsulaGenus {INSULA_DURABILIS, INSULA_EPHEMERA}`, `InsulaMutator`, `InsulaActarius`, `InsulaRepositorium`; `insula_repositorium_creare(piscina, intern, durabilis_cstr, ephemera_cstr)`, `insula_radix(repo, genus) → StmlNodus*` (READ ONLY), `insula_attributum(repo, genus, titulus) → chorda*` (root attribute; NIHIL if absent), `mutare_durabile(repo, fn, ctx) → b32`, `mutare_ephemera(repo, fn, ctx) → b32`, `insula_versio(repo, genus)`, `insula_scribere(repo, genus, piscina) → chorda`, `insula_restituere(repo) → b32` (VERUM = honest), `insula_mendacium(repo)`, `insula_ponere_canonem(repo, genus, Canon*)`, `insula_ponere_actarium(repo, fn, ctx)`, `insula_causa(repo) → chorda`.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_insula.c`:
```c
/* probatio_insula.c - tria genera, portae unicae, restitutio, mendacium */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
ponere_instrumentum (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    stml_attributum_addere(radix, p, in, "instrumentum", (constans character*)ctx);
}

interior vacuum
addere_ignotum (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    (vacuum)ctx;
    stml_liberum_addere(radix, stml_elementum_creare(p, in, "ignotum"));
}

interior vacuum
numerare (InsulaGenus genus, StmlNodus* radix, vacuum* ctx)
{
    (vacuum)genus; (vacuum)radix;
    (*(i32*)ctx)++;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                chorda*  a;
                    i32  acta;
                 chorda  causa;
                  Canon* canon;
             StmlNodus*  radix;

    piscina = piscina_generare_dynamicum("probatio_insula", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Probans creationem et lectionem ---\n");
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL (repo);
    a = insula_attributum(repo, INSULA_DURABILIS, "latitudo");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "320");
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "penicillus");
    CREDO_NIHIL (insula_attributum(repo, INSULA_EPHEMERA, "nemo"));
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_EPHEMERA), ZEPHYRUM);

    imprimere("\n--- Probans portam ephemerae ---\n");
    acta = ZEPHYRUM;
    insula_ponere_actarium(repo, numerare, &acta);
    CREDO_VERUM (mutare_ephemera(repo, ponere_instrumentum, (vacuum*)"impletio"));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "impletio");
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_EPHEMERA), I);
    CREDO_AEQUALIS_I32 (acta, I);
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_DURABILIS), ZEPHYRUM);

    imprimere("\n--- Probans restitutionem HONESTAM ---\n");
    CREDO_VERUM  (insula_restituere(repo));
    CREDO_FALSUM (insula_mendacium(repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "impletio");
    a = insula_attributum(repo, INSULA_EPHEMERA, "focus");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "tabula");

    imprimere("\n--- Probans scriptorem MENTIENTEM (lex rehydrationis) ---\n");
    radix = insula_radix(repo, INSULA_EPHEMERA);
    stml_attributum_addere(radix, piscina, intern, "instrumentum", "furtivus");  /* extra portam */
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "furtivus");
    CREDO_FALSUM (insula_restituere(repo));
    CREDO_VERUM  (insula_mendacium(repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "impletio");   /* ex textu honesto refecta */

    imprimere("\n--- Probans canonem: scriptura recusata ---\n");
    canon = canon_legere(chorda_ex_literis(
        "<canon dialectus=\"ephemera\" versio=\"1\">"
        "<elementum nomen=\"ephemera\" radix=\"verum\"/>"
        "</canon>", piscina), piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, canon);
    CREDO_VERUM  (mutare_ephemera(repo, ponere_instrumentum, (vacuum*)"linea"));
    CREDO_FALSUM (mutare_ephemera(repo, addere_ignotum, NIHIL));   /* liberum non declaratum */
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_EPHEMERA), II);
    CREDO_CHORDA_NON_VACUA (insula_causa(repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "linea");

    imprimere("\n--- Probans scribere ---\n");
    CREDO_CHORDA_CONTINET (insula_scribere(repo, INSULA_DURABILIS, piscina), chorda_ex_literis("latitudo", piscina));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
The canon assertion relies on `canon_iudicare` reporting an undeclared child element as a vitium (the `<liberum>` declarations in `aedilis.canon` are the precedent). If the engine treats undeclared children as permitted, change `addere_ignotum` to add an undeclared ATTRIBUTE and declare `instrumentum` via `<attributum nomen="instrumentum"/>` inside `<elementum>`, matching whichever rule `canon.canon` (the canon of canons, repo root) documents.

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh insula`
Expected: FAIL — `'insula.h' file not found`.

- [ ] **Step 3: Write header and implementation**

`include/insula.h`:
```c
/* insula.h - Insulae: status in tribus generibus, porta una per genus
 *
 * Veritas est ARBOR STML in piscina, per hoc repositorium sola
 * accessa (ludus-brainstorm.md XIII, DECISUS). Genera: DURABILIS
 * (quod basis datorum teneret) et EPHEMERA (quod restitutiones
 * supervivit). MOTUS non est insula - structura in memoria
 * (motus.h), in quiete huc effusa.
 *
 * PORTA UNICA: mutare_durabile / mutare_ephemera. Mutator in
 * DUPLICATO currit (scribere+legere in piscinam alteram), canon
 * iudicat, deinde permutatio - scriptura aut tota aut nulla.
 *
 * LEX REHYDRATIONIS: post omnem scripturam per portam textus
 * ultimus servatur; restituere arborem in memoria cum eo confert -
 * discrepantia = MENDACIUM (scriptor extra portam) - deinde ex textu
 * honesto reficit.
 *
 * <componens/>-lint L1: extra has portas nemo insulam mutat.
 */

#ifndef INSULA_H
#define INSULA_H

/* <aedilis corpus="lib/insula.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"

nomen enumeratio {
    INSULA_DURABILIS = ZEPHYRUM,
    INSULA_EPHEMERA,
    INSULA_GENUS_NUMERUS
} InsulaGenus;

nomen vacuum (*InsulaMutator)(
              StmlNodus* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx);

/* Vocatur post omnem scripturam per portam successam - pictor P3
 * hic acta appendit. */
nomen vacuum (*InsulaActarius)(
    InsulaGenus  genus,
     StmlNodus*  radix,
        vacuum*  ctx);

nomen structura {
                Piscina* piscina;               /* repositorii ipsius */
    InternamentumChorda* intern;
                Piscina* piscinae[INSULA_GENUS_NUMERUS][II];   /* ping-pong */
                    i32  activa[INSULA_GENUS_NUMERUS];
             StmlNodus*  radices[INSULA_GENUS_NUMERUS];
                 chorda  textus_ultimus[INSULA_GENUS_NUMERUS];
                    i32  versio[INSULA_GENUS_NUMERUS];
                  Canon* canones[INSULA_GENUS_NUMERUS];
         InsulaActarius  actarius;
                vacuum*  actarius_ctx;
                    b32  mendacium;
                 chorda  causa;
} InsulaRepositorium;

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* durabilis_stml,
     constans character* ephemera_stml);

/* LECTIO SOLA. Mutare per hunc pointer = mendacium. */
StmlNodus*
insula_radix (
    InsulaRepositorium* repo,
           InsulaGenus  genus);

chorda*
insula_attributum (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus);

b32 mutare_durabile (InsulaRepositorium* repo, InsulaMutator fn, vacuum* ctx);
b32 mutare_ephemera (InsulaRepositorium* repo, InsulaMutator fn, vacuum* ctx);

i32
insula_versio (
    constans InsulaRepositorium* repo,
                    InsulaGenus  genus);

chorda
insula_scribere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
               Piscina* piscina);

b32 insula_restituere (InsulaRepositorium* repo);
b32 insula_mendacium (constans InsulaRepositorium* repo);
chorda insula_causa (constans InsulaRepositorium* repo);

vacuum insula_ponere_canonem  (InsulaRepositorium* repo, InsulaGenus genus, Canon* canon);
vacuum insula_ponere_actarium (InsulaRepositorium* repo, InsulaActarius fn, vacuum* ctx);

#endif /* INSULA_H */
```
`lib/insula.c`:
```c
/* insula.c - Insulae: tria genera, porta una per genus */

#include "insula.h"
#include "xar.h"

#include <string.h>

interior StmlNodus*
legere_in (
    InsulaRepositorium* repo,
               Piscina* piscina,
    constans character* cstr)
{
    StmlResultus res;
    res = stml_legere_ex_literis(cstr, piscina, repo->intern);
    si (!res.successus) { redde NIHIL; }
    redde res.elementum_radix;
}

InsulaRepositorium*
insula_repositorium_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* durabilis_stml,
     constans character* ephemera_stml)
{
    InsulaRepositorium* repo;
    i32                 g;
    constans character* fontes[INSULA_GENUS_NUMERUS];

    repo = (InsulaRepositorium*)piscina_allocare(piscina, magnitudo(InsulaRepositorium));
    si (!repo) { redde NIHIL; }
    memset(repo, ZEPHYRUM, magnitudo(InsulaRepositorium));
    repo->piscina = piscina;
    repo->intern  = intern;
    repo->causa   = chorda_ex_literis("", piscina);
    fontes[INSULA_DURABILIS] = durabilis_stml;
    fontes[INSULA_EPHEMERA]  = ephemera_stml;
    per (g = ZEPHYRUM; g < INSULA_GENUS_NUMERUS; g++)
    {
        repo->piscinae[g][0] = piscina_generare_dynamicum("insula_a", XVI * M);
        repo->piscinae[g][1] = piscina_generare_dynamicum("insula_b", XVI * M);
        repo->activa[g]      = ZEPHYRUM;
        repo->radices[g]     = legere_in(repo, repo->piscinae[g][0], fontes[g]);
        si (!repo->radices[g]) { redde NIHIL; }
        repo->textus_ultimus[g] = stml_scribere(repo->radices[g], repo->piscinae[g][0], FALSUM);
    }
    redde repo;
}

StmlNodus*
insula_radix (
    InsulaRepositorium* repo,
           InsulaGenus  genus)
{
    redde repo->radices[genus];
}

chorda*
insula_attributum (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus)
{
    redde stml_attributum_capere(repo->radices[genus], titulus);
}

interior b32
mutare (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
          InsulaMutator fn,
                vacuum* ctx)
{
    i32        alia;
    Piscina*   p;
    StmlNodus* duplicatum;
    chorda     textus;
    Xar*       vitia;

    alia = I - repo->activa[genus];
    p    = repo->piscinae[genus][alia];
    piscina_vacare(p);
    textus     = stml_scribere(repo->radices[genus], p, FALSUM);
    duplicatum = legere_in(repo, p, chorda_ut_cstr(textus, p));
    si (!duplicatum) { repo->causa = chorda_ex_literis("duplicatio fracta", repo->piscina); redde FALSUM; }

    fn(duplicatum, p, repo->intern, ctx);

    si (repo->canones[genus])
    {
        vitia = canon_iudicare(repo->canones[genus], duplicatum, p);
        si (vitia && xar_numerus(vitia) > ZEPHYRUM)
        {
            repo->causa = chorda_ex_literis("canon recusat scripturam", repo->piscina);
            redde FALSUM;   /* duplicatum abicitur cum piscina alia proxime vacata */
        }
    }

    repo->radices[genus]        = duplicatum;
    repo->activa[genus]         = alia;
    repo->textus_ultimus[genus] = stml_scribere(duplicatum, p, FALSUM);
    repo->versio[genus]++;
    si (repo->actarius) { repo->actarius(genus, duplicatum, repo->actarius_ctx); }
    redde VERUM;
}

b32 mutare_durabile (InsulaRepositorium* repo, InsulaMutator fn, vacuum* ctx) { redde mutare(repo, INSULA_DURABILIS, fn, ctx); }
b32 mutare_ephemera (InsulaRepositorium* repo, InsulaMutator fn, vacuum* ctx) { redde mutare(repo, INSULA_EPHEMERA, fn, ctx); }

i32
insula_versio (
    constans InsulaRepositorium* repo,
                    InsulaGenus  genus)
{
    redde repo->versio[genus];
}

chorda
insula_scribere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
               Piscina* piscina)
{
    redde stml_scribere(repo->radices[genus], piscina, VERUM);
}

b32
insula_restituere (
    InsulaRepositorium* repo)
{
    i32        g;
    i32        alia;
    Piscina*   p;
    chorda     nunc;
    b32        honestum;

    honestum = VERUM;
    per (g = ZEPHYRUM; g < INSULA_GENUS_NUMERUS; g++)
    {
        alia = I - repo->activa[g];
        p    = repo->piscinae[g][alia];
        piscina_vacare(p);
        nunc = stml_scribere(repo->radices[g], p, FALSUM);
        si (!chorda_aequalis(nunc, repo->textus_ultimus[g])) { honestum = FALSUM; }
        /* refacere EX TEXTU HONESTO solo */
        repo->radices[g] = legere_in(repo, p, chorda_ut_cstr(repo->textus_ultimus[g], p));
        repo->textus_ultimus[g] = stml_scribere(repo->radices[g], p, FALSUM);
        repo->activa[g] = alia;
    }
    repo->mendacium = !honestum;
    si (!honestum) { repo->causa = chorda_ex_literis("mendacium: insula extra portam mutata", repo->piscina); }
    redde honestum;
}

b32    insula_mendacium (constans InsulaRepositorium* repo) { redde repo->mendacium; }
chorda insula_causa     (constans InsulaRepositorium* repo) { redde repo->causa; }

vacuum insula_ponere_canonem  (InsulaRepositorium* repo, InsulaGenus genus, Canon* canon) { repo->canones[genus] = canon; }
vacuum insula_ponere_actarium (InsulaRepositorium* repo, InsulaActarius fn, vacuum* ctx) { repo->actarius = fn; repo->actarius_ctx = ctx; }
```
`piscina_vacare(Piscina*)` is the reset-to-empty call (`piscina.h:78`). If its name differs, use the function at that line.

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh insula` — Expected: PASS (see the canon note under Step 1 if the last block fails on rule semantics).

- [ ] **Step 5: Worklog + commit**

`lib/insula.worklog.md`: `## 2026-09-04 — natus` + "Ping-pong piscinas per genus: a gated write round-trips the tree through text into the other piscina (that IS the rehydration discipline), mutates, canon-judges, swaps. Restore compares in-memory serialization with the last honest text; mismatch = mendacium, then rebuilds from honest text. `insula_attributum` pointers are valid until the next gated write of that genus."
```bash
git add include/insula.h lib/insula.c lib/insula.worklog.md probationes/probatio_insula.c compile_tests_fontes_generata.sh
git commit -m "ludus: T6 - insula: repositorium trium generum, portae unicae, restitutio, mendacium, canon"
```

---

### Task 7: `motus` — the in-memory kinetic struct and the quiet seat

**Files:**
- Create: `include/motus.h`, `lib/motus.c`, `lib/motus.worklog.md`
- Test: `probationes/probatio_motus.c`

**Interfaces:**
- Consumes: `Punctum` (Task 2), `InsulaRepositorium`, `mutare_ephemera` (Task 6).
- Produces: `Motus`, `MotusMutator`; `motus_initiare(motus, piscina)`, `mutare_motum(motus, fn, ctx, tempus)`, `motus_quies(motus, nunc, quies_ms) → b32`, `motus_effundere(motus, repo) → b32` (`<quies/>`), `motus_captura_ponere(motus, id)`, `motus_captura_tollere(motus)`.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_motus.c`:
```c
/* probatio_motus.c - structura motus, quies, effusio */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "insula.h"
#include "motus.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
pan_ponere (Motus* motus, vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = XL; motus->pan.y = -X;
}

interior vacuum
punctum_addere (Motus* motus, vacuum* ctx)
{
    Punctum* p;
    p = (Punctum*)xar_addere(motus->ictus_pendens);
    *p = *(Punctum*)ctx;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
               Motus  motus;
                Punctum  p;
                chorda*  a;

    piscina = piscina_generare_dynamicum("probatio_motus", XVI * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>", "<ephemera/>");
    motus_initiare(&motus, piscina);

    imprimere("\n--- Mutatio motus NON tangit insulam ---\n");
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    CREDO_AEQUALIS_I32 (motus.pan.x, XL);
    CREDO_VERUM (motus.sordida);
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_EPHEMERA), ZEPHYRUM);

    imprimere("\n--- Quies iudicatur ex tempore ---\n");
    CREDO_FALSUM (motus_quies(&motus, M + CC, CCC));
    CREDO_VERUM  (motus_quies(&motus, M + CCC, CCC));
    p.x = I; p.y = II;
    mutare_motum(&motus, punctum_addere, &p, M + CCL);     /* mutatio nova differt quietem */
    CREDO_FALSUM (motus_quies(&motus, M + CCC, CCC));
    CREDO_VERUM  (motus_quies(&motus, M + DL, CCC));
    CREDO_AEQUALIS_I32 (xar_numerus(motus.ictus_pendens), I);

    imprimere("\n--- Effusio in quiete scribit insulam SEMEL ---\n");
    CREDO_VERUM (motus_effundere(&motus, repo));
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_EPHEMERA), I);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "40");
    CREDO_FALSUM (motus.sordida);
    CREDO_FALSUM (motus_quies(&motus, X * M, CCC));   /* nihil sordidum */

    imprimere("\n--- Captura ---\n");
    motus_captura_ponere(&motus, chorda_ex_literis("tabula", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS (motus.captura, "tabula");
    motus_captura_tollere(&motus);
    CREDO_VERUM (chorda_vacua(motus.captura));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh motus` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/motus.h`:
```c
/* motus.h - Genus MOBILE: structura in memoria, effusa in quiete
 *
 * "Pictura per eventum, scriptura IN QUIETE" (laboratorium 0007/0015).
 * Scripturae per eventum HUC veniunt (vili); insula ephemerarum solum
 * in quiete tangitur (motus_effundere, sedes <quies/>) aut a
 * scriptura durabili in liberatione superatur. Ictus finitus insulam
 * ephemerarum NUMQUAM tangit.
 *
 * Lint L4: scripturae insulae camporum mobilium solum ex <quies/>.
 */

#ifndef KINETICA_H
#define KINETICA_H

/* <aedilis corpus="lib/motus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "mandatum.h"   /* Punctum */
#include "insula.h"

nomen structura {
      chorda  captura;         /* id componentis; vacua = nulla */
        Xar*  ictus_pendens;   /* Xar de Punctum */
     Punctum  pan;
         i32  zoom;
         s64  tempus_ultimae_mutationis;
         b32  sordida;
    Piscina*  piscina;
} Motus;

nomen vacuum (*MotusMutator)(Motus* motus, vacuum* ctx);

vacuum motus_initiare (Motus* motus, Piscina* piscina);

vacuum
mutare_motum (
           Motus* motus,
     MotusMutator fn,
             vacuum* ctx,
                 s64 tempus);

b32
motus_quies (
    constans Motus* motus,
                  s64  nunc,
                  s64  quies_ms);

/* <quies/> - scriptura UNA camporum persistendorum (pan, zoom) in
 * insulam ephemerarum. */
b32
motus_effundere (
              Motus* motus,
    InsulaRepositorium* repo);

vacuum motus_captura_ponere  (Motus* motus, chorda id);
vacuum motus_captura_tollere (Motus* motus);

#endif /* KINETICA_H */
```
`lib/motus.c`:
```c
/* motus.c - genus mobile in memoria */

#include "motus.h"

vacuum
motus_initiare (
    Motus* motus,
     Piscina* piscina)
{
    motus->captura       = chorda_ex_literis("", piscina);
    motus->ictus_pendens = xar_creare(piscina, (i32)magnitudo(Punctum));
    motus->pan.x = ZEPHYRUM; motus->pan.y = ZEPHYRUM;
    motus->zoom  = I;
    motus->tempus_ultimae_mutationis = ZEPHYRUM;
    motus->sordida = FALSUM;
    motus->piscina = piscina;
}

vacuum
mutare_motum (
           Motus* motus,
     MotusMutator fn,
             vacuum* ctx,
                 s64 tempus)
{
    fn(k, ctx);
    motus->tempus_ultimae_mutationis = tempus;
    motus->sordida = VERUM;
}

b32
motus_quies (
    constans Motus* motus,
                  s64  nunc,
                  s64  quies_ms)
{
    redde motus->sordida && (nunc - motus->tempus_ultimae_mutationis) >= quies_ms;
}

nomen structura { Motus* motus; } EffusioCtx;

interior vacuum
effusio_mutator (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    Motus* motus;
    motus = ((EffusioCtx*)ctx)->motus;
    stml_attributum_addere(radix, p, in, "pan_x", chorda_ut_cstr(chorda_ex_s32((s32)motus->pan.x, p), p));
    stml_attributum_addere(radix, p, in, "pan_y", chorda_ut_cstr(chorda_ex_s32((s32)motus->pan.y, p), p));
    stml_attributum_addere(radix, p, in, "zoom",  chorda_ut_cstr(chorda_ex_s32((s32)motus->zoom,  p), p));
}

b32
motus_effundere (
              Motus* motus,
    InsulaRepositorium* repo)
{
    EffusioCtx ctx;
    b32        ok;

    ctx.motus = motus;
    ok = mutare_ephemera(repo, effusio_mutator, &ctx);
    si (ok) { motus->sordida = FALSUM; }
    redde ok;
}

vacuum motus_captura_ponere  (Motus* motus, chorda id) { motus->captura = id; }
vacuum motus_captura_tollere (Motus* motus)            { motus->captura = chorda_ex_literis("", motus->piscina); }
```
`pan.x` is `i32` (unsigned) — the test's `-X` wraps; if `-Wsign-conversion` rejects it, use `motus->pan.y = ZEPHYRUM` in `pan_ponere` and assert only `pan_x`. Signed screen coordinates are a P3 decision (spec §10 names are unsealed; so are widths).

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh motus` — Expected: PASS.

- [ ] **Step 5: Worklog + commit**

`lib/motus.worklog.md`: `## 2026-09-04 — natus` + "In-memory kinetic tier; quiet judged from event time; effundere is the single `<quies/>` seat writing pan/zoom to the ephemera island."
```bash
git add include/motus.h lib/motus.c lib/motus.worklog.md probationes/probatio_motus.c compile_tests_fontes_generata.sh
git commit -m "ludus: T7 - motus: structura in memoria, quies ex tempore, effusio <quies/>, captura"
```

---

### Task 8: `destinatio` — targeting strategy, focus order, capture

**Files:**
- Create: `include/destinatio.h`, `lib/destinatio.c`, `lib/destinatio.worklog.md`
- Test: `probationes/probatio_destinatio.c`

**Interfaces:**
- Consumes: `Componens` tree (Task 3), `Motus.captura` (Task 7), `Eventus`.
- Produces: `Destinatio {id_captum, id_geometricum, ascensus (Xar of Componens*, target first, root last), punctum_locale}`, `DestinatioStrategia` fn type, `destinatio_geometrica(arbor, motus, focus, ev, piscina) → Destinatio` (default), `destinatio_focus_proximum(arbor, focus, piscina) → chorda`, `destinatio_focus_praecedens(...)`, `destinatio_componens(destinatio) → Componens*` (target or NIHIL).

- [ ] **Step 1: Write the failing test**

`probationes/probatio_destinatio.c`:
```c
/* probatio_destinatio.c - strategia geometrica, focus, captura */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "motus.h"
#include "destinatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Componens*
nodus (Piscina* p, InternamentumChorda* in, constans character* id, Partes partes,
       i32 x, i32 y, i32 w, i32 h, b32 focusabilis)
{
    Componens* c; Fines f;
    c = componens_creare(p, in, id, partes);
    f.x = x; f.y = y; f.latitudo = w; f.altitudo = h;
    componens_ponere_fines(c, f);
    componens_ponere_focusabilis(c, focusabilis);
    redde c;
}

interior Eventus
mus (eventus_genus_t genus, i32 x, i32 y)
{
    Eventus e; memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = genus; e.datum.mus.x = x; e.datum.mus.y = y;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
              Componens* radix;
              Componens* pannus;    /* container with translatio + sectio */
              Componens* b1;
              Componens* b2;
              Componens* tabula;
               Motus  motus;
                Eventus  e;
                 Destinatio  s;
                 chorda  focus;
                 chorda  vacua;

    piscina = piscina_generare_dynamicum("probatio_destinatio", XVI * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    motus_initiare(&motus, piscina);
    vacua = chorda_ex_literis("", piscina);

    /* radix 0,0,200,100
     *   b1     10,10,50,20   focusabilis
     *   pannus 70,0,130,100  translatio (0,50) sectio; liberi in spatio panni:
     *     b2     0,0,40,20   focusabilis     -> in schirmo 70..110, 50..70
     *     tabula 50,0,60,40  proprius focusabilis -> in schirmo 120..180, 50..90
     */
    radix  = nodus(piscina, intern, "radix", PARTES_NULLUM, ZEPHYRUM, ZEPHYRUM, CC, C, FALSUM);
    b1     = nodus(piscina, intern, "b1", PARTES_BOTTONE, X, X, L, XX, VERUM);
    pannus = nodus(piscina, intern, "pannus", PARTES_NULLUM, LXX, ZEPHYRUM, CXXX, C, FALSUM);
    componens_ponere_transformatio(pannus, ZEPHYRUM, L, I);
    componens_ponere_sectio(pannus, VERUM);
    b2     = nodus(piscina, intern, "b2", PARTES_BOTTONE, ZEPHYRUM, ZEPHYRUM, XL, XX, VERUM);
    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA, L, ZEPHYRUM, LX, XL, VERUM);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_addere_liberum(radix, b1);
    componens_addere_liberum(radix, pannus);
    componens_addere_liberum(pannus, b2);
    componens_addere_liberum(pannus, tabula);

    imprimere("\n--- Ictus in b1 ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, XV, XV);
    s = destinatio_geometrica(radix, &k, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_geometricum, "b1");
    CREDO_AEQUALIS_I32 (xar_numerus(s.ascensus), II);      /* b1, radix */
    CREDO_VERUM (destinatio_componens(&s) == b1);

    imprimere("\n--- Ictus in tabula per translationem panni; punctum locale ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, CXXV, LX);               /* schirmo -> pannus (55,60) -> tabula locale (5,10) */
    s = destinatio_geometrica(radix, &k, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_geometricum, "tabula");
    CREDO_AEQUALIS_I32 (xar_numerus(s.ascensus), III);     /* tabula, pannus, radix */
    CREDO_AEQUALIS_I32 (s.punctum_locale.x, V);
    CREDO_AEQUALIS_I32 (s.punctum_locale.y, X);

    imprimere("\n--- Sectio: punctum in b2 SUPRA pannum praecisum -> radix ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, LXXX, X);                /* in pannus fines but child b2 is at y 50.. */
    s = destinatio_geometrica(radix, &k, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_geometricum, "pannus");

    imprimere("\n--- Extra omnia -> radix ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, CXC, XC);
    s = destinatio_geometrica(radix, &k, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_geometricum, "radix");

    imprimere("\n--- Captura vincit geometriam ---\n");
    motus_captura_ponere(&motus, chorda_ex_literis("tabula", piscina));
    e = mus(EVENTUS_MUS_MOTUS, XV, XV);
    s = destinatio_geometrica(radix, &k, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_captum, "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS (s.id_geometricum, "b1");
    CREDO_VERUM (destinatio_componens(&s) == tabula);
    motus_captura_tollere(&motus);

    imprimere("\n--- Clavis it ad focum ---\n");
    memset(&e, ZEPHYRUM, magnitudo(Eventus)); e.genus = EVENTUS_CLAVIS_DEPRESSUS;
    focus = chorda_ex_literis("b2", piscina);
    s = destinatio_geometrica(radix, &k, focus, &e, piscina);
    CREDO_VERUM (destinatio_componens(&s) == b2);
    CREDO_AEQUALIS_I32 (xar_numerus(s.ascensus), III);
    CREDO_VERUM (chorda_vacua(s.id_geometricum));

    imprimere("\n--- Ordo foci ---\n");
    focus = destinatio_focus_proximum(radix, vacua, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (focus, "b1");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (focus, "b2");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (focus, "tabula");
    focus = destinatio_focus_proximum(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (focus, "b1");                 /* involvit */
    focus = destinatio_focus_praecedens(radix, focus, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS (focus, "tabula");

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh destinatio` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/destinatio.h`:
```c
/* destinatio.h - Destinatio: STRATEGIA una, permutabilis, componentibus ignota
 *
 * Ordinaria: ambulare arborem componentium ULTIMAM intime-primum,
 * summum-primum (ordo pictoris = ordo z), sectionem honorans,
 * translationem/scalam invertens. Captura (motus) vincit
 * geometriam pro eventibus muris; claves ad focum eunt, deinde
 * ascendunt. Componentia scopum NUMQUAM vocant (lint L9-gemellus:
 * strategia arborem logicam solam legit, numquam mandata).
 */

#ifndef SCOPUS_H
#define SCOPUS_H

/* <aedilis corpus="lib/destinatio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "fenestra.h"
#include "componens.h"
#include "motus.h"

nomen structura {
     chorda  id_captum;        /* vacua = nulla captura */
     chorda  id_geometricum;   /* vacua = nullus ictus / non eventus muris */
       Xar*  ascensus;         /* Xar de Componens*: destinatio primus, radix ultima */
    Punctum  punctum_locale;   /* punctum in spatio scopi geometrici */
} Destinatio;

nomen Destinatio (*DestinatioStrategia)(
             Componens* arbor,
    constans Motus* motus,
                chorda focus,
     constans Eventus* ev,
              Piscina* piscina);

Destinatio
destinatio_geometrica (
             Componens* arbor,
    constans Motus* motus,
                chorda focus,
     constans Eventus* ev,
              Piscina* piscina);

/* Componens scopi: captum si adest, alioquin primus ascensus. */
Componens*
destinatio_componens (
    constans Destinatio* s);

chorda destinatio_focus_proximum   (Componens* arbor, chorda focus, Piscina* piscina);
chorda destinatio_focus_praecedens (Componens* arbor, chorda focus, Piscina* piscina);

#endif /* SCOPUS_H */
```
`lib/destinatio.c`:
```c
/* destinatio.c - strategia geometrica ordinaria */

#include "destinatio.h"

interior b32
est_muris (eventus_genus_t g)
{
    redde g == EVENTUS_MUS_DEPRESSUS || g == EVENTUS_MUS_LIBERATUS
        || g == EVENTUS_MUS_MOTUS || g == EVENTUS_MUS_ROTULA || g == EVENTUS_MUS_DUPLEX;
}

interior b32
est_clavis (eventus_genus_t g)
{
    redde g == EVENTUS_CLAVIS_DEPRESSUS || g == EVENTUS_CLAVIS_LIBERATUS;
}

/* p in spatio PARENTIS c. Redde scopum intimum, punctum locale
 * scriptum in *locale; NIHIL si nihil tactum. */
interior Componens*
invenire (
    Componens* c,
       Punctum p,
      Punctum* locale)
{
    Punctum    q;
    i32        i;
    Componens* hit;
    b32        intra;

    intra = fines_continet(c->fines, p);
    si (c->sectio && !intra) { redde NIHIL; }

    /* in spatium liberorum: origo c, translatio, scala */
    q.x = (i32)(((s32)p.x - (s32)c->fines.x - (s32)c->translatio.x) / (s32)c->scala);
    q.y = (i32)(((s32)p.y - (s32)c->fines.y - (s32)c->translatio.y) / (s32)c->scala);

    per (i = componens_numerus_liberorum(c) - I; i >= ZEPHYRUM; i--)
    {
        hit = invenire(componens_liberum(c, i), q, locale);
        si (hit) { redde hit; }
        si (i == ZEPHYRUM) { frange; }   /* i est i32: ne infra ZEPHYRUM eat */
    }
    si (intra)
    {
        locale->x = (i32)((s32)p.x - (s32)c->fines.x);
        locale->y = (i32)((s32)p.y - (s32)c->fines.y);
        redde c;
    }
    redde NIHIL;
}

interior Xar*
ascensus_ex (
    Componens* c,
      Piscina* piscina)
{
    Xar*        a;
    Componens** slot;

    a = xar_creare(piscina, (i32)magnitudo(Componens*));
    dum (c)
    {
        slot = (Componens**)xar_addere(a);
        *slot = c;
        c = c->parens;
    }
    redde a;
}

Destinatio
destinatio_geometrica (
             Componens* arbor,
    constans Motus* motus,
                chorda focus,
     constans Eventus* ev,
              Piscina* piscina)
{
    Destinatio     s;
    Componens* geo;
    Componens* destinatum;
    Punctum    p;

    s.id_captum      = chorda_ex_literis("", piscina);
    s.id_geometricum = chorda_ex_literis("", piscina);
    s.punctum_locale.x = ZEPHYRUM; s.punctum_locale.y = ZEPHYRUM;
    destinatum = arbor;

    si (est_muris(ev->genus))
    {
        p.x = ev->datum.mus.x; p.y = ev->datum.mus.y;
        geo = invenire(arbor, p, &s.punctum_locale);
        si (geo) { s.id_geometricum = geo->id; destinatio = geo; }
        si (!chorda_vacua(motus->captura))
        {
            s.id_captum = motus->captura;
            destinatum = componens_invenire_per_id(arbor, motus->captura);
            si (!destinatio) { destinatio = geo ? geo : arbor; }
        }
    }
    alioquin si (est_clavis(ev->genus) && !chorda_vacua(focus))
    {
        destinatum = componens_invenire_per_id(arbor, focus);
        si (!destinatio) { destinatio = arbor; }
    }
    s.ascensus = ascensus_ex(destinatum, piscina);
    redde s;
}

Componens*
destinatio_componens (
    constans Destinatio* s)
{
    si (xar_numerus(s->ascensus) == ZEPHYRUM) { redde NIHIL; }
    redde *(Componens**)xar_obtinere(s->ascensus, ZEPHYRUM);
}

interior vacuum
colligere_focusabiles (Componens* c, Xar* index)
{
    Componens** slot;
    i32         i;
    i32         n;

    si (c->focusabilis) { slot = (Componens**)xar_addere(index); *slot = c; }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++) { colligere_focusabiles(componens_liberum(c, i), index); }
}

interior chorda
focus_gradu (Componens* arbor, chorda focus, s32 gradus, Piscina* piscina)
{
    Xar* index;
    i32  n;
    i32  i;
    s32  k;

    index = xar_creare(piscina, (i32)magnitudo(Componens*));
    colligere_focusabiles(arbor, index);
    n = xar_numerus(index);
    si (n == ZEPHYRUM) { redde chorda_ex_literis("", piscina); }
    k = -I;
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis((*(Componens**)xar_obtinere(index, i))->id, focus)) { k = (s32)i; frange; }
    }
    si (k < ZEPHYRUM) { k = gradus > ZEPHYRUM ? -I : ZEPHYRUM; }
    k = (k + gradus + (s32)n) % (s32)n;
    redde (*(Componens**)xar_obtinere(index, (i32)k))->id;
}

chorda destinatio_focus_proximum   (Componens* arbor, chorda focus, Piscina* piscina) { redde focus_gradu(arbor, focus,  I, piscina); }
chorda destinatio_focus_praecedens (Componens* arbor, chorda focus, Piscina* piscina) { redde focus_gradu(arbor, focus, -I, piscina); }
```

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh destinatio` — Expected: PASS. If the `per` loop with `i >= ZEPHYRUM` on unsigned `i32` warns, restructure as `i = n; dum (i > ZEPHYRUM) { i--; ... }`.

- [ ] **Step 5: Worklog + commit**

`lib/destinatio.worklog.md`: `## 2026-09-04 — natus` + "Default geometric strategy over the logical tree; children last-to-first (topmost first); sectio prunes; translate/scale inverted per level; capture overrides for pointer events; keys route to focus; tab order = DFS focusables with wrap. Polygon/mask predicates are P3 pulls (rect and proprius only here — proprius means the punctum_locale is authoritative)."
```bash
git add include/destinatio.h lib/destinatio.c lib/destinatio.worklog.md probationes/probatio_destinatio.c compile_tests_fontes_generata.sh
git commit -m "ludus: T8 - destinatio: strategia geometrica ordinaria, ascensus, captura, focus proximum/praecedens"
```

---

### Task 9: `actio` — the action registry, resolved both ways

**Files:**
- Create: `include/actio.h`, `lib/actio.c`, `lib/actio.worklog.md`
- Test: `probationes/probatio_actio.c`

**Interfaces:**
- Consumes: `InsulaRepositorium` (T6), `Motus` (T7), `Destinatio` (T8), `Componens` (T3), `Eventus`.
- Produces: `ActioFn`, `ActioRegistrum`; `actio_registrum_creare(piscina, intern)`, `actio_registrare(reg, titulus_cstr, fn, ctx) → b32`, `actio_invenire(reg, titulus_chorda, &fn, &ctx) → b32`, `actio_non_registratae(reg, arbor, piscina) → Xar*` (of `chorda`: names in the tree with no handler), `actio_non_relatae(reg, arbor, piscina) → Xar*` (registered names no componens references).

- [ ] **Step 1: Write the failing test**

`probationes/probatio_actio.c`:
```c
/* probatio_actio.c - registrum actionum; resolutio UTRIMQUE (L10) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "componens.h"
#include "actio.h"
#include "credo.h"
#include <stdio.h>

interior b32
nihil_agere (InsulaRepositorium* r, Motus* motus, constans Destinatio* s, Componens* c, constans Eventus* e, vacuum* ctx)
{
    (vacuum)r; (vacuum)motus; (vacuum)s; (vacuum)c; (vacuum)e;
    (*(i32*)ctx)++;
    redde VERUM;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
         ActioRegistrum* reg;
              Componens* radix;
              Componens* b;
                ActioFn  fn;
                vacuum*  ctx;
                    i32  numerus;
                   Xar*  desunt;
                   Xar*  otiosae;

    piscina = piscina_generare_dynamicum("probatio_actio", XVI * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    numerus = ZEPHYRUM;

    imprimere("\n--- Registrare et invenire ---\n");
    reg = actio_registrum_creare(piscina, intern);
    CREDO_VERUM (actio_registrare(reg, "numerare", nihil_agere, &numerus));
    CREDO_VERUM (actio_registrare(reg, "fugere",   nihil_agere, &numerus));
    CREDO_FALSUM (actio_registrare(reg, "numerare", nihil_agere, &numerus));   /* duplex recusatur */
    CREDO_VERUM (actio_invenire(reg, chorda_ex_literis("numerare", piscina), &fn, &ctx));
    CREDO_VERUM (fn == nihil_agere && ctx == (vacuum*)&numerus);
    CREDO_FALSUM (actio_invenire(reg, chorda_ex_literis("ignota", piscina), &fn, &ctx));

    imprimere("\n--- Resolutio utrimque ---\n");
    radix = componens_creare(piscina, intern, "radix", PARTES_NULLUM);
    b     = componens_creare(piscina, intern, "b", PARTES_BOTTONE);
    componens_ponere_actio(b, "numerare");
    componens_addere_liberum(radix, b);
    b     = componens_creare(piscina, intern, "c", PARTES_BOTTONE);
    componens_ponere_actio(b, "ignota");
    componens_addere_liberum(radix, b);

    desunt  = actio_non_registratae(reg, radix, piscina);
    otiosae = actio_non_relatae(reg, radix, piscina);
    CREDO_AEQUALIS_I32 (xar_numerus(desunt), I);
    CREDO_CHORDA_AEQUALIS_LITERIS (*(chorda*)xar_obtinere(desunt, ZEPHYRUM), "ignota");
    CREDO_AEQUALIS_I32 (xar_numerus(otiosae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS (*(chorda*)xar_obtinere(otiosae, ZEPHYRUM), "fugere");

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh actio` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/actio.h`:
```c
/* actio.h - Registrum actionum NOMINATARUM
 *
 * Actiones in marcatione NOMINANTUR, numquam computantur (laboratorium
 * 0011). Una actio per INTENTIONEM; phasis ex genere eventus
 * (DECISUS). Registrum utrimque resolvitur (lint L10): nomen in arbore
 * sine tractatore = error; tractator a nemine relatus = monitum.
 *
 * <tractator/>-lint L5: tractatores I/O non faciunt; effectus ut data.
 */

#ifndef ACTIO_H
#define ACTIO_H

/* <aedilis corpus="lib/actio.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "destinatio.h"

/* Redde VERUM si consumptus (ascensus sistit). nodus = componens cuius
 * actio congruit (potest esse maior scopi). */
nomen b32 (*ActioFn)(
    InsulaRepositorium* repo,
              Motus* motus,
       constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx);

nomen structura {
                    Xar* nomina;      /* Xar de chorda (internatae) */
                    Xar* functiones;  /* Xar de ActioFn */
                    Xar* contextus;   /* Xar de vacuum* */
                Piscina* piscina;
    InternamentumChorda* intern;
} ActioRegistrum;

ActioRegistrum*
actio_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern);

b32
actio_registrare (
        ActioRegistrum* reg,
    constans character* titulus,
               ActioFn  fn,
               vacuum*  ctx);

b32
actio_invenire (
    constans ActioRegistrum* reg,
                     chorda  titulus,
                    ActioFn* fn_ex,
                    vacuum** ctx_ex);

Xar* actio_non_registratae (constans ActioRegistrum* reg, Componens* arbor, Piscina* piscina);
Xar* actio_non_relatae     (constans ActioRegistrum* reg, Componens* arbor, Piscina* piscina);

#endif /* ACTIO_H */
```
`lib/actio.c`:
```c
/* actio.c - registrum actionum nominatarum */

#include "actio.h"

ActioRegistrum*
actio_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern)
{
    ActioRegistrum* reg;

    reg = (ActioRegistrum*)piscina_allocare(piscina, magnitudo(ActioRegistrum));
    si (!reg) { redde NIHIL; }
    reg->nomina     = xar_creare(piscina, (i32)magnitudo(chorda));
    reg->functiones = xar_creare(piscina, (i32)magnitudo(ActioFn));
    reg->contextus  = xar_creare(piscina, (i32)magnitudo(vacuum*));
    reg->piscina    = piscina;
    reg->intern     = intern;
    redde reg;
}

interior s32
index_nominis (constans ActioRegistrum* reg, chorda titulus)
{
    i32 i;
    i32 n;
    n = xar_numerus(reg->nomina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(reg->nomina, i), titulus)) { redde (s32)i; }
    }
    redde -I;
}

b32
actio_registrare (
        ActioRegistrum* reg,
    constans character* titulus,
               ActioFn  fn,
               vacuum*  ctx)
{
    chorda*  n;
    ActioFn* f;
    vacuum** c;
    chorda   t;

    t = *chorda_internare_ex_literis(reg->intern, titulus);
    si (index_nominis(reg, t) >= ZEPHYRUM) { redde FALSUM; }
    n = (chorda*)xar_addere(reg->nomina);      *n = t;
    f = (ActioFn*)xar_addere(reg->functiones); *f = fn;
    c = (vacuum**)xar_addere(reg->contextus);  *c = ctx;
    redde VERUM;
}

b32
actio_invenire (
    constans ActioRegistrum* reg,
                     chorda  titulus,
                    ActioFn* fn_ex,
                    vacuum** ctx_ex)
{
    s32 k;
    k = index_nominis(reg, titulus);
    si (k < ZEPHYRUM) { redde FALSUM; }
    *fn_ex  = *(ActioFn*)xar_obtinere(reg->functiones, (i32)k);
    *ctx_ex = *(vacuum**)xar_obtinere(reg->contextus, (i32)k);
    redde VERUM;
}

interior vacuum
colligere_actiones (Componens* c, Xar* index)
{
    chorda* slot;
    i32     i;
    i32     n;
    i32     k;
    b32     habet;

    si (!chorda_vacua(c->actio))
    {
        habet = FALSUM;
        n = xar_numerus(index);
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (chorda_aequalis(*(chorda*)xar_obtinere(index, k), c->actio)) { habet = VERUM; }
        }
        si (!habet) { slot = (chorda*)xar_addere(index); *slot = c->actio; }
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++) { colligere_actiones(componens_liberum(c, i), index); }
}

Xar*
actio_non_registratae (
    constans ActioRegistrum* reg,
                 Componens* arbor,
                   Piscina* piscina)
{
    Xar*    relatae;
    Xar*    desunt;
    chorda* slot;
    i32     i;
    i32     n;

    relatae = xar_creare(piscina, (i32)magnitudo(chorda));
    desunt  = xar_creare(piscina, (i32)magnitudo(chorda));
    colligere_actiones(arbor, relatae);
    n = xar_numerus(relatae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (index_nominis(reg, *(chorda*)xar_obtinere(relatae, i)) < ZEPHYRUM)
        {
            slot = (chorda*)xar_addere(desunt); *slot = *(chorda*)xar_obtinere(relatae, i);
        }
    }
    redde desunt;
}

Xar*
actio_non_relatae (
    constans ActioRegistrum* reg,
                 Componens* arbor,
                   Piscina* piscina)
{
    Xar*    relatae;
    Xar*    otiosae;
    chorda* slot;
    chorda  t;
    i32     i;
    i32     k;
    i32     n;
    i32     m;
    b32     habet;

    relatae = xar_creare(piscina, (i32)magnitudo(chorda));
    otiosae = xar_creare(piscina, (i32)magnitudo(chorda));
    colligere_actiones(arbor, relatae);
    n = xar_numerus(reg->nomina);
    m = xar_numerus(relatae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = *(chorda*)xar_obtinere(reg->nomina, i);
        habet = FALSUM;
        per (k = ZEPHYRUM; k < m; k++)
        {
            si (chorda_aequalis(t, *(chorda*)xar_obtinere(relatae, k))) { habet = VERUM; }
        }
        si (!habet) { slot = (chorda*)xar_addere(otiosae); *slot = t; }
    }
    redde otiosae;
}
```

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh actio` — Expected: PASS.

- [ ] **Step 5: Worklog + commit**

`lib/actio.worklog.md`: `## 2026-09-04 — natus` + "Name → (fn, ctx) registry; duplicate names refused; L10 both-ways resolution over a componens tree."
```bash
git add include/actio.h lib/actio.c lib/actio.worklog.md probationes/probatio_actio.c compile_tests_fontes_generata.sh
git commit -m "ludus: T9 - actio: registrum nominatum, resolutio utrimque (L10)"
```

---

### Task 10: `dispensator` — the loop, the staleness rule, derived focus/hover

**Files:**
- Modify: `include/fenestra.h` (the `eventus_genus_t` enum, `:29-43`) — add five derived genera
- Modify: `include/destinatio.h`, `lib/destinatio.c` — add `destinatio_ex_componente`
- Create: `include/dispensator.h`, `lib/dispensator.c`, `lib/dispensator.worklog.md`
- Create: `probationes/ludus_toy.h` (the toy app: componere + actiones, shared by T10–T12)
- Test: `probationes/probatio_dispensator.c`

**Interfaces:**
- Consumes: everything from T1–T9.
- Produces: `Componere` fn type; `Dispensator`; `dispensator_creare(piscina, intern, repo, actiones, componere, ctx, quies_ms)`, `dispensator_ponere_strategiam(d, strategia)`, `dispensator_tractare(d, ev)`, `dispensator_pulsare(d, tempus)`, `dispensator_recomponere(d)`, `dispensator_arbor(d) → Componens*`, `dispensator_focus(d) → chorda`, `dispensator_focus_ponere(d, id)`, `dispensator_focus_impellere(d, id)`, `dispensator_focus_extrahere(d)`, `dispensator_super(d) → chorda`, `dispensator_motus(d) → Motus*`, `dispensator_numerus_compositionum(d) → i32`. New event genera: `EVENTUS_MUS_INTRAVIT`, `EVENTUS_MUS_EXIIT`, `EVENTUS_FOCUS_CAPTUS`, `EVENTUS_FOCUS_AMISSUS`, `EVENTUS_FOCUS_PETITUS`. `Destinatio destinatio_ex_componente(Componens* c, Piscina* piscina)`. Toy: `toy_componere`, `toy_registrare(reg, toy)`, `ToyStatus`.

- [ ] **Step 1: Add the derived genera and `destinatio_ex_componente`**

In `include/fenestra.h` replace lines `:42-43`:
```c
    EVENTUS_MUS_DUPLEX,         /* Double-click (derivatum) */
    /* Derivata a dispensatore (ludus): numquam a fenestra emissa. */
    EVENTUS_MUS_INTRAVIT,
    EVENTUS_MUS_EXIIT,
    EVENTUS_FOCUS_CAPTUS,
    EVENTUS_FOCUS_AMISSUS,
    EVENTUS_FOCUS_PETITUS
} eventus_genus_t;
```
In `include/destinatio.h` after `destinatio_componens`:
```c
/* Destinatio artificialis: ascensus ex componente dato - pro eventibus
 * derivatis (intravit/exiit/focus) ad nodum certum missis. */
Destinatio
destinatio_ex_componente (
    Componens* c,
      Piscina* piscina);
```
In `lib/destinatio.c` after `destinatio_componens`:
```c
Destinatio
destinatio_ex_componente (
    Componens* c,
      Piscina* piscina)
{
    Destinatio s;
    s.id_captum        = chorda_ex_literis("", piscina);
    s.id_geometricum   = c->id;
    s.punctum_locale.x = ZEPHYRUM; s.punctum_locale.y = ZEPHYRUM;
    s.ascensus         = ascensus_ex(c, piscina);
    redde s;
}
```
Run `./compile_tests.sh destinatio && ./compile_tests.sh eventus_stml` — both must still PASS (the eventus_stml title table already lists the five new names in enum order).

- [ ] **Step 2: Write the toy app header**

`probationes/ludus_toy.h`:
```c
/* ludus_toy.h - applicatio ludicra pro probationibus substrati
 *
 * radix (0,0,200,100) actio "fugere"
 *   b1     (10,10,50,20)  bottone focusabilis actio "numerare"
 *   tabula (70,10,100,80) tabula PROPRIUS focusabilis actio "ictus"
 *
 * <componens/> <purus/>: toy_componere legit repo + motus, nihil scribit.
 */
#ifndef LUDUS_TOY_H
#define LUDUS_TOY_H

#include "latina.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "actio.h"
#include "destinatio.h"
#include <string.h>

nomen structura { i32 compositiones; } ToyStatus;

interior Componens*
toy_componere (InsulaRepositorium* repo, constans Motus* motus, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    Componens* radix; Componens* b1; Componens* tabula; Fines f;
    (vacuum)repo; (vacuum)motus;
    ((ToyStatus*)ctx)->compositiones++;
    radix = componens_creare(p, in, "radix", PARTES_NULLUM);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = CC; f.altitudo = C;
    componens_ponere_fines(radix, f);
    componens_ponere_actio(radix, "fugere");
    b1 = componens_creare(p, in, "b1", PARTES_BOTTONE);
    f.x = X; f.y = X; f.latitudo = L; f.altitudo = XX;
    componens_ponere_fines(b1, f);
    componens_ponere_actio(b1, "numerare");
    componens_ponere_titulum(b1, "Numerare");
    componens_ponere_focusabilis(b1, VERUM);
    componens_addere_liberum(radix, b1);
    tabula = componens_creare(p, in, "tabula", PARTES_TABULA);
    f.x = LXX; f.y = X; f.latitudo = C; f.altitudo = LXXX;
    componens_ponere_fines(tabula, f);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_actio(tabula, "ictus");
    componens_ponere_focusabilis(tabula, VERUM);
    componens_addere_liberum(radix, tabula);
    redde radix;
}

/* mutatores */
interior vacuum
toy_incrementare (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    chorda* a; i32 v;
    a = stml_attributum_capere(radix, (constans character*)ctx);
    v = ZEPHYRUM; si (a) { chorda_ut_i32(*a, &v); }
    stml_attributum_addere(radix, p, in, (constans character*)ctx,
                           chorda_ut_cstr(chorda_ex_s32((s32)(v + I), p), p));
}

nomen structura { constans character* titulus; i32 valor; } ToyPositio;

interior vacuum
toy_ponere_numerum (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    ToyPositio* tp; tp = (ToyPositio*)ctx;
    stml_attributum_addere(radix, p, in, tp->titulus, chorda_ut_cstr(chorda_ex_s32((s32)tp->valor, p), p));
}

interior vacuum
toy_ponere_verum (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    stml_attributum_addere(radix, p, in, (constans character*)ctx, "verum");
}

interior vacuum
toy_punctum_addere (Motus* motus, vacuum* ctx)
{
    Punctum* slot; slot = (Punctum*)xar_addere(motus->ictus_pendens); *slot = *(Punctum*)ctx;
}

interior vacuum
toy_puncta_vacare (Motus* motus, vacuum* ctx) { (vacuum)ctx; xar_vacare(motus->ictus_pendens); }

/* actiones - <tractator/> */
interior b32
toy_numerare (InsulaRepositorium* r, Motus* motus, constans Destinatio* s, Componens* c, constans Eventus* e, vacuum* ctx)
{
    (vacuum)motus; (vacuum)s; (vacuum)c; (vacuum)ctx;
    si (e->genus != EVENTUS_MUS_DEPRESSUS) { redde FALSUM; }
    mutare_ephemera(r, toy_incrementare, (vacuum*)"numerus");
    redde VERUM;
}

interior b32
toy_ictus (InsulaRepositorium* r, Motus* motus, constans Destinatio* s, Componens* c, constans Eventus* e, vacuum* ctx)
{
    Punctum p; ToyPositio tp;
    (vacuum)ctx;
    p = s->punctum_locale;
    commutatio (e->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            motus_captura_ponere(k, c->id);
            mutare_motum(k, toy_puncta_vacare, NIHIL, e->tempus);
            mutare_motum(k, toy_punctum_addere, &p, e->tempus);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (chorda_vacua(motus->captura)) { redde FALSUM; }
            mutare_motum(k, toy_punctum_addere, &p, e->tempus);
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura)) { redde FALSUM; }
            tp.titulus = "puncta"; tp.valor = xar_numerus(motus->ictus_pendens);
            mutare_durabile(r, toy_ponere_numerum, &tp);
            mutare_durabile(r, toy_incrementare, (vacuum*)"ictus_numerus");
            mutare_motum(k, toy_puncta_vacare, NIHIL, e->tempus);
            motus->sordida = FALSUM;   /* ictus finitus insulam ephemerarum NON tangit */
            motus_captura_tollere(motus);
            redde VERUM;
        ordinarius: redde FALSUM;
    }
}

interior b32
toy_fugere (InsulaRepositorium* r, Motus* motus, constans Destinatio* s, Componens* c, constans Eventus* e, vacuum* ctx)
{
    (vacuum)motus; (vacuum)s; (vacuum)c; (vacuum)ctx;
    si (e->genus == EVENTUS_CLAVIS_DEPRESSUS && e->datum.clavis.typus == (character)XXVII)
    {
        mutare_ephemera(r, toy_ponere_verum, (vacuum*)"fuga");
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
toy_registrare (ActioRegistrum* reg, ToyStatus* toy)
{
    actio_registrare(reg, "numerare", toy_numerare, toy);
    actio_registrare(reg, "ictus",    toy_ictus,    toy);
    actio_registrare(reg, "fugere",   toy_fugere,   toy);
}

#endif /* LUDUS_TOY_H */
```
(`interior` = static; every probatio that includes this header gets its own copy — acceptable for a test fixture. `-Wunused-function` is not in the flag set; if it ever is, mark the functions `(vacuum)` at use.)

- [ ] **Step 3: Write the failing test**

`probationes/probatio_dispensator.c`:
```c
/* probatio_dispensator.c - ansa: derivare -> destinatio -> actio -> componere */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

interior Eventus
mus (eventus_genus_t genus, s64 t, i32 x, i32 y)
{
    Eventus e; memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = genus; e.tempus = t; e.datum.mus.x = x; e.datum.mus.y = y;
    redde e;
}

interior Eventus
clavis (s64 t, character typus)
{
    Eventus e; memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_CLAVIS_DEPRESSUS; e.tempus = t; e.datum.clavis.typus = typus;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                Eventus  e;
                chorda*  a;
                    i32  n0;

    piscina = piscina_generare_dynamicum("probatio_dispensator", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>", "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    toy.compositiones = ZEPHYRUM;
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere, &toy, CCC);
    CREDO_NON_NIHIL (d);
    CREDO_NON_NIHIL (dispensator_arbor(d));               /* quadrum 0 ante initum */
    CREDO_AEQUALIS_I32 (toy.compositiones, I);

    imprimere("\n--- Ictus in b1: actio, focus, recompositio ---\n");
    n0 = toy.compositiones;
    e = mus(EVENTUS_MUS_DEPRESSUS, M, XXXV, XX); dispensator_tractare(d, &e);
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "1");
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "b1");
    CREDO_VERUM (toy.compositiones > n0);                   /* regula staleness */
    e = mus(EVENTUS_MUS_LIBERATUS, M + L, XXXV, XX); dispensator_tractare(d, &e);

    imprimere("\n--- Motus super tabulam: super derivatur ---\n");
    e = mus(EVENTUS_MUS_MOTUS, M + C, CXX, L); dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_super(d), "tabula");

    imprimere("\n--- Ictus in tabula: captura, motus, UNA scriptura durabilis ---\n");
    e = mus(EVENTUS_MUS_DEPRESSUS, M + CC, CXX, L);   dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_motus(d)->captura, "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "tabula");
    e = mus(EVENTUS_MUS_MOTUS, M + CCXX, CXXV, LV);    dispensator_tractare(d, &e);
    e = mus(EVENTUS_MUS_MOTUS, M + CCXL, CXXX, LX);    dispensator_tractare(d, &e);
    CREDO_AEQUALIS_I32 (xar_numerus(dispensator_motus(d)->ictus_pendens), III);
    CREDO_AEQUALIS_I32 (insula_versio(repo, INSULA_DURABILIS), ZEPHYRUM);   /* nondum */
    e = mus(EVENTUS_MUS_LIBERATUS, M + CCC, CXXX, LX); dispensator_tractare(d, &e);
    CREDO_VERUM (chorda_vacua(dispensator_motus(d)->captura));
    a = insula_attributum(repo, INSULA_DURABILIS, "ictus_numerus");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "3");
    CREDO_NIHIL (insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));   /* ephemera intacta ab ictu */

    imprimere("\n--- Clavis ESC ascendit ad radicem ---\n");
    e = clavis(M + CD, (character)XXVII); dispensator_tractare(d, &e);
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "verum");

    imprimere("\n--- Tab: focus proximum, involvens ---\n");
    e = clavis(M + D, '\t'); dispensator_tractare(d, &e);
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "b1");

    imprimere("\n--- Acervus foci ---\n");
    dispensator_focus_impellere(d, chorda_ex_literis("tabula", piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "tabula");
    dispensator_focus_extrahere(d);
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "b1");

    imprimere("\n--- Quies: pulsus post CCC ms effundit motus ---\n");
    dispensator_motus(d)->pan.x = VII;
    dispensator_motus(d)->sordida = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis = M + DC;
    dispensator_pulsare(d, M + DCCC);
    CREDO_NIHIL (insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    dispensator_pulsare(d, M + M);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "7");

    imprimere("\n--- Remontatio honesta post omnia ---\n");
    CREDO_VERUM (insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 4: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh dispensator` — Expected: FAIL, `dispensator.h` not found.

- [ ] **Step 5: Write header and implementation**

`include/dispensator.h`:
```c
/* dispensator.h - Ansa: derivare -> destinatio -> actio -> componere
 *
 * Possidet Motus, Derivator, arborem componentium ultimam (piscinae
 * ping-pong), focum (in insula ephemerarum: attributum 'focus' et
 * 'focus_acervus'), super (id sub mure). REGULA STALENESS: componere
 * post OMNEM eventum dispensatum currit; pingere/rasterizare semel per
 * quadrum (extra hanc bibliothecam). Quadrum 0 componitur in creatione.
 * Sedes quietis: ante eventum quemque, si motus quiescit, effundit.
 */

#ifndef DISPENSATOR_H
#define DISPENSATOR_H

/* <aedilis corpus="lib/dispensator.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "fenestra.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"
#include "derivare.h"
#include "destinatio.h"
#include "actio.h"

nomen Componens* (*Componere)(
    InsulaRepositorium* repo,
     constans Motus* motus,
              Piscina* piscina,
   InternamentumChorda* intern,
               vacuum* ctx);

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
                Piscina* piscinae_arboris[II];
                    i32  arbor_activa;
                Piscina* scratch;
     InsulaRepositorium* repo;
               Motus  motus;
              Derivator  derivator;
         ActioRegistrum* actiones;
        DestinatioStrategia  strategia;
              Componere  componere;
                vacuum*  componere_ctx;
             Componens*  arbor;
                 chorda  super;
                    s64  quies_ms;
                    Xar* effusio;
                    i32  compositiones;
} Dispensator;

Dispensator*
dispensator_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
         ActioRegistrum* actiones,
              Componere  componere,
                vacuum*  componere_ctx,
                    s64  quies_ms);

vacuum dispensator_ponere_strategiam (Dispensator* d, DestinatioStrategia strategia);
vacuum dispensator_tractare  (Dispensator* d, constans Eventus* ev);
vacuum dispensator_pulsare   (Dispensator* d, s64 tempus);   /* EVENTUS_NIHIL cum tempore */
vacuum dispensator_recomponere (Dispensator* d);

Componens* dispensator_arbor    (constans Dispensator* d);
chorda     dispensator_focus    (Dispensator* d);
vacuum     dispensator_focus_ponere    (Dispensator* d, chorda id);
vacuum     dispensator_focus_impellere (Dispensator* d, chorda id);
vacuum     dispensator_focus_extrahere (Dispensator* d);
chorda     dispensator_super    (constans Dispensator* d);
Motus*  dispensator_motus (Dispensator* d);
i32        dispensator_numerus_compositionum (constans Dispensator* d);

#endif /* DISPENSATOR_H */
```
`lib/dispensator.c`:
```c
/* dispensator.c - ansa eventuum */

#include "dispensator.h"

#include <string.h>

/* ---- focus in insula ephemerarum ---- */

nomen structura { constans character* titulus; chorda valor; } AttrCtx;

interior vacuum
attr_ponere (StmlNodus* radix, Piscina* p, InternamentumChorda* in, vacuum* ctx)
{
    AttrCtx* a; a = (AttrCtx*)ctx;
    stml_attributum_addere(radix, p, in, a->titulus, chorda_ut_cstr(a->valor, p));
}

interior chorda
attr_legere (Dispensator* d, constans character* titulus)
{
    chorda* c;
    c = insula_attributum(d->repo, INSULA_EPHEMERA, titulus);
    si (c) { redde *c; }
    redde chorda_ex_literis("", d->piscina);
}

chorda
dispensator_focus (Dispensator* d) { redde attr_legere(d, "focus"); }

vacuum
dispensator_focus_ponere (Dispensator* d, chorda id)
{
    AttrCtx a; a.titulus = "focus"; a.valor = id;
    mutare_ephemera(d->repo, attr_ponere, &a);
}

vacuum
dispensator_focus_impellere (Dispensator* d, chorda id)
{
    AttrCtx a;
    chorda  acervus;
    chorda  currens;
    chorda  comma;

    currens = dispensator_focus(d);
    acervus = attr_legere(d, "focus_acervus");
    comma   = chorda_ex_literis(",", d->piscina);
    si (!chorda_vacua(acervus)) { acervus = chorda_concatenare(acervus, comma, d->piscina); }
    acervus = chorda_concatenare(acervus, currens, d->piscina);
    a.titulus = "focus_acervus"; a.valor = acervus;
    mutare_ephemera(d->repo, attr_ponere, &a);
    dispensator_focus_ponere(d, id);
}

vacuum
dispensator_focus_extrahere (Dispensator* d)
{
    AttrCtx               a;
    chorda                acervus;
    chorda_fissio_fructus partes;
    chorda                ultimus;
    chorda                reliquum;
    chorda                comma;
    i32                   i;

    acervus = attr_legere(d, "focus_acervus");
    si (chorda_vacua(acervus)) { redde; }
    partes  = chorda_fissio(acervus, ',', d->piscina);
    ultimus = partes.elementa[partes.numerus - I];
    reliquum = chorda_ex_literis("", d->piscina);
    comma    = chorda_ex_literis(",", d->piscina);
    per (i = ZEPHYRUM; i + I < partes.numerus; i++)
    {
        si (i > ZEPHYRUM) { reliquum = chorda_concatenare(reliquum, comma, d->piscina); }
        reliquum = chorda_concatenare(reliquum, partes.elementa[i], d->piscina);
    }
    a.titulus = "focus_acervus"; a.valor = reliquum;
    mutare_ephemera(d->repo, attr_ponere, &a);
    dispensator_focus_ponere(d, ultimus);
}

/* ---- compositio (ping-pong) ---- */

vacuum
dispensator_recomponere (Dispensator* d)
{
    i32      alia;
    Piscina* p;

    alia = I - d->arbor_activa;
    p    = d->piscinae_arboris[alia];
    piscina_vacare(p);
    d->arbor        = d->componere(d->repo, &d->motus, p, d->intern, d->componere_ctx);
    d->arbor_activa = alia;
    d->compositiones++;
}

Dispensator*
dispensator_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
         ActioRegistrum* actiones,
              Componere  componere,
                vacuum*  componere_ctx,
                    s64  quies_ms)
{
    Dispensator* d;

    d = (Dispensator*)piscina_allocare(piscina, magnitudo(Dispensator));
    si (!d) { redde NIHIL; }
    memset(d, ZEPHYRUM, magnitudo(Dispensator));
    d->piscina = piscina;
    d->intern  = intern;
    d->piscinae_arboris[0] = piscina_generare_dynamicum("arbor_a", LXIV * M);
    d->piscinae_arboris[1] = piscina_generare_dynamicum("arbor_b", LXIV * M);
    d->scratch = piscina_generare_dynamicum("destinatio_scratch", XVI * M);
    d->repo      = repo;
    d->actiones  = actiones;
    d->strategia = destinatio_geometrica;
    d->componere = componere;
    d->componere_ctx = componere_ctx;
    d->super     = chorda_ex_literis("", piscina);
    d->quies_ms  = quies_ms;
    d->effusio   = xar_creare(piscina, (i32)magnitudo(Eventus));
    motus_initiare(&d->motus, piscina);
    derivator_initiare(&d->derivator, CCC, IV);
    d->arbor_activa = I;   /* recomponere permutat ad 0 */
    dispensator_recomponere(d);   /* quadrum 0 ante initum */
    redde d;
}

vacuum
dispensator_ponere_strategiam (Dispensator* d, DestinatioStrategia strategia) { d->strategia = strategia; }

/* ---- dispensatio ---- */

interior b32
mittere (Dispensator* d, constans Destinatio* s, constans Eventus* e)
{
    i32        i;
    i32        n;
    Componens* c;
    ActioFn    fn;
    vacuum*    ctx;

    n = xar_numerus(s->ascensus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        c = *(Componens**)xar_obtinere(s->ascensus, i);
        si (chorda_vacua(c->actio)) { perge; }
        si (!actio_invenire(d->actiones, c->actio, &fn, &ctx)) { perge; }
        si (fn(d->repo, &d->motus, s, c, e, ctx)) { redde VERUM; }
    }
    redde FALSUM;
}

interior vacuum
mittere_ad (Dispensator* d, Componens* c, eventus_genus_t genus, s64 tempus)
{
    Destinatio  s;
    Eventus e;

    si (!c) { redde; }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = genus; e.tempus = tempus;
    s = destinatio_ex_componente(c, d->scratch);
    mittere(d, &s, &e);
}

interior vacuum
tractare_unum (Dispensator* d, constans Eventus* e)
{
    PiscinaNotatio nota;
    Destinatio         s;
    chorda         focus;
    chorda         focus_novus;
    Componens*     destinatum;
    b32            consumptus;

    nota  = piscina_notare(d->scratch);
    focus = dispensator_focus(d);
    s     = d->strategia(d->arbor, &d->motus, focus, e, d->scratch);
    destinatum = destinatio_componens(&s);

    /* super derivatur */
    si (e->genus == EVENTUS_MUS_MOTUS && !chorda_aequalis(s.id_geometricum, d->super))
    {
        mittere_ad(d, componens_invenire_per_id(d->arbor, d->super), EVENTUS_MUS_EXIIT, e->tempus);
        d->super = s.id_geometricum;
        mittere_ad(d, componens_invenire_per_id(d->arbor, d->super), EVENTUS_MUS_INTRAVIT, e->tempus);
    }

    /* ictus dat focum */
    si (e->genus == EVENTUS_MUS_DEPRESSUS && destinatum && destinatum->focusabilis
        && !chorda_aequalis(destinatum->id, focus))
    {
        mittere_ad(d, componens_invenire_per_id(d->arbor, focus), EVENTUS_FOCUS_AMISSUS, e->tempus);
        dispensator_focus_ponere(d, destinatum->id);
        mittere_ad(d, destinatum, EVENTUS_FOCUS_CAPTUS, e->tempus);
        focus = destinatum->id;
    }

    consumptus = mittere(d, &s, e);

    /* Tab innatus */
    si (!consumptus && e->genus == EVENTUS_CLAVIS_DEPRESSUS && e->datum.clavis.typus == '\t')
    {
        focus_novus = (e->datum.clavis.modificantes & I)
                    ? destinatio_focus_praecedens(d->arbor, focus, d->scratch)
                    : destinatio_focus_proximum(d->arbor, focus, d->scratch);
        si (!chorda_vacua(focus_novus))
        {
            mittere_ad(d, componens_invenire_per_id(d->arbor, focus), EVENTUS_FOCUS_AMISSUS, e->tempus);
            dispensator_focus_ponere(d, focus_novus);
            mittere_ad(d, componens_invenire_per_id(d->arbor, focus_novus), EVENTUS_FOCUS_CAPTUS, e->tempus);
        }
    }

    /* regula staleness */
    dispensator_recomponere(d);

    /* focus ut petitio: id absens ex arbore nova */
    focus = dispensator_focus(d);
    si (!chorda_vacua(focus) && !componens_invenire_per_id(d->arbor, focus))
    {
        mittere_ad(d, d->arbor, EVENTUS_FOCUS_PETITUS, e->tempus);
        dispensator_recomponere(d);
        si (!componens_invenire_per_id(d->arbor, focus))
        {
            dispensator_focus_ponere(d, chorda_ex_literis("", d->piscina));
        }
    }
    piscina_reficere(d->scratch, nota);
}

vacuum
dispensator_tractare (Dispensator* d, constans Eventus* ev)
{
    i32 i;
    i32 n;

    si (motus_quies(&d->motus, ev->tempus, d->quies_ms))
    {
        motus_effundere(&d->motus, d->repo);   /* <quies/> */
    }
    xar_vacare(d->effusio);
    derivare(&d->derivator, ev, d->effusio);
    n = xar_numerus(d->effusio);
    per (i = ZEPHYRUM; i < n; i++)
    {
        tractare_unum(d, (constans Eventus*)xar_obtinere(d->effusio, i));
    }
}

vacuum
dispensator_pulsare (Dispensator* d, s64 tempus)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_NIHIL; e.tempus = tempus;
    dispensator_tractare(d, &e);
}

Componens* dispensator_arbor    (constans Dispensator* d) { redde d->arbor; }
chorda     dispensator_super    (constans Dispensator* d) { redde d->super; }
Motus*  dispensator_motus (Dispensator* d)          { redde &d->motus; }
i32        dispensator_numerus_compositionum (constans Dispensator* d) { redde d->compositiones; }
```
Note `tractare_unum` passes `d->scratch` events through `xar_obtinere` on `d->effusio` while `derivare` may append during iteration — it does not (derivation happens before the loop), so the pointer is stable.

- [ ] **Step 6: Run to verify it passes**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh dispensator` — Expected: PASS. Then `./compile_tests.sh destinatio && ./compile_tests.sh eventus_stml` still PASS.

- [ ] **Step 7: Worklog + commit**

`lib/dispensator.worklog.md`: `## 2026-09-04 — natus` + "The loop. Focus and its stack live in the ephemera island (`focus`, `focus_acervus`) so restore restores them. Hover, focus-captus/amissus, and focus-petitus are derived here (need the targeting result). Staleness: recompose after every dispatched event via ping-pong piscinas. Quiet seat runs before each event. Focus-petitus P2: delivered to root; the owner-adjusts refinement is P3."
```bash
git add include/fenestra.h include/destinatio.h lib/destinatio.c include/dispensator.h lib/dispensator.c lib/dispensator.worklog.md probationes/ludus_toy.h probationes/probatio_dispensator.c compile_tests_fontes_generata.sh
git commit -m "ludus: T10 - dispensator: ansa, regula staleness, focus in insula, super/focus derivata, sedes quietis; genera derivata in fenestra.h"
```

---

### Task 11: `manus_ludus` — the in-process test hand

**Files:**
- Create: `include/manus_ludus.h`, `lib/manus_ludus.c`, `lib/manus_ludus.worklog.md`
- Test: `probationes/probatio_manus_ludus.c`

**Interfaces:**
- Consumes: `Dispensator` (T10), `ludus_toy.h`.
- Produces: `ManusLudus`; `manus_ludus_creare(piscina, d)`, `manus_ludus_invenire(m, selector) → Componens*`, `manus_ludus_ad_schirmum(m, c, locale) → Punctum`, `manus_ludus_premere(m, selector) → b32`, `manus_ludus_premere_ad(m, x, y)`, `manus_ludus_movere(m, x, y)`, `manus_ludus_trahere(m, selector, puncta, n)`, `manus_ludus_clavem(m, typus, modificantes)`, `manus_ludus_exspectare(m, ms)`, `manus_ludus_existit(m, selector)`, `manus_ludus_focus(m) → chorda`, `manus_ludus_causa(m)`; macros `CREDO_MANUS_LUDUS_EXISTIT/ABEST/FOCUS`. Selectors: `#id`, `[partes=…]`, `[actio=…]`, `[titulus=…]`.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_manus_ludus.c`:
```c
/* probatio_manus_ludus.c - manus in-processu super applicationem ludicram */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
             ManusLudus* m;
                chorda*  a;
                Punctum  via[III];
                Punctum  p;

    piscina = piscina_generare_dynamicum("probatio_manus_ludus", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>", "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    toy.compositiones = ZEPHYRUM;
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere, &toy, CCC);
    m = manus_ludus_creare(piscina, d);

    imprimere("\n--- Selectores ---\n");
    CREDO_MANUS_LUDUS_EXISTIT (m, "#b1");
    CREDO_MANUS_LUDUS_EXISTIT (m, "[partes=tabula]");
    CREDO_MANUS_LUDUS_EXISTIT (m, "[actio=numerare]");
    CREDO_MANUS_LUDUS_EXISTIT (m, "[titulus=Numerare]");
    CREDO_MANUS_LUDUS_ABEST   (m, "#nemo");
    CREDO_MANUS_LUDUS_ABEST   (m, "[partes=dialogus]");

    imprimere("\n--- ad_schirmum ---\n");
    p.x = ZEPHYRUM; p.y = ZEPHYRUM;
    p = manus_ludus_ad_schirmum(m, manus_ludus_invenire(m, "#tabula"), p);
    CREDO_AEQUALIS_I32 (p.x, LXX);
    CREDO_AEQUALIS_I32 (p.y, X);

    imprimere("\n--- premere ---\n");
    CREDO_VERUM (manus_ludus_premere(m, "[titulus=Numerare]"));
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "1");
    CREDO_MANUS_LUDUS_FOCUS (m, "b1");

    imprimere("\n--- trahere in tabula ---\n");
    via[0].x = V;  via[0].y = V;
    via[1].x = X;  via[1].y = X;
    via[2].x = XV; via[2].y = XV;
    CREDO_VERUM (manus_ludus_trahere(m, "#tabula", via, III));
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");
    CREDO_NON_NIHIL (a);
    CREDO_CHORDA_AEQUALIS_LITERIS (*a, "3");
    CREDO_MANUS_LUDUS_FOCUS (m, "tabula");

    imprimere("\n--- clavem ---\n");
    CREDO_VERUM (manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");
    CREDO_NON_NIHIL (a);
    CREDO_VERUM (manus_ludus_clavem(m, '\t', ZEPHYRUM));
    CREDO_MANUS_LUDUS_FOCUS (m, "b1");

    imprimere("\n--- exspectare movet tempus (quies) ---\n");
    dispensator_motus(d)->pan.x = III;
    dispensator_motus(d)->sordida = VERUM;
    dispensator_motus(d)->tempus_ultimae_mutationis = m->tempus;
    manus_ludus_exspectare(m, D);
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL (a);

    imprimere("\n--- selector malus nominat causam ---\n");
    CREDO_FALSUM (manus_ludus_premere(m, "#nemo"));
    CREDO_CHORDA_NON_VACUA (manus_ludus_causa(m));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh manus_ludus` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/manus_ludus.h`:
```c
/* manus_ludus.h - manus IN PROCESSU super dispensatorem (transportus
 * nativus). Eadem verba ac manus.h (premere, clavem, existit, focus),
 * SYNCHRONA: nulla asynchronia, ergo nulla mora. Selectores super
 * arborem LOGICAM: '#id', '[partes=x]', '[actio=x]', '[titulus=x]'.
 * Tempus manus per exspectare solum procedit - horologium nullum.
 * Unificatio sub manus.h ut transportus alter: dilatio nominata. */

#ifndef MANUS_LUDUS_H
#define MANUS_LUDUS_H

/* <aedilis corpus="lib/manus_ludus.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "componens.h"
#include "dispensator.h"

nomen structura {
    Dispensator* d;
             s64 tempus;
             s64 gradus_ms;     /* inter depressus et liberatus */
        Piscina* piscina;
          chorda causa;
} ManusLudus;

ManusLudus* manus_ludus_creare (Piscina* piscina, Dispensator* d);

Componens* manus_ludus_invenire    (ManusLudus* m, constans character* selector);
Punctum    manus_ludus_ad_schirmum (ManusLudus* m, Componens* c, Punctum locale);

b32 manus_ludus_premere    (ManusLudus* m, constans character* selector);
b32 manus_ludus_premere_ad (ManusLudus* m, i32 x, i32 y);
b32 manus_ludus_movere     (ManusLudus* m, i32 x, i32 y);
b32 manus_ludus_trahere    (ManusLudus* m, constans character* selector, constans Punctum* puncta, i32 n);
b32 manus_ludus_clavem     (ManusLudus* m, character typus, i32 modificantes);
vacuum manus_ludus_exspectare (ManusLudus* m, s64 ms);

b32    manus_ludus_existit (ManusLudus* m, constans character* selector);
chorda manus_ludus_focus   (ManusLudus* m);
chorda manus_ludus_causa   (constans ManusLudus* m);

#define CREDO_MANUS_LUDUS_EXISTIT(m, sel) CREDO_VERUM (manus_ludus_existit((m), (sel)))
#define CREDO_MANUS_LUDUS_ABEST(m, sel)   CREDO_FALSUM (manus_ludus_existit((m), (sel)))
#define CREDO_MANUS_LUDUS_FOCUS(m, id)    CREDO_CHORDA_AEQUALIS_LITERIS (manus_ludus_focus(m), (id))

#endif /* MANUS_LUDUS_H */
```
`lib/manus_ludus.c`:
```c
/* manus_ludus.c - manus in processu */

#include "manus_ludus.h"

#include <string.h>

ManusLudus*
manus_ludus_creare (Piscina* piscina, Dispensator* d)
{
    ManusLudus* m;
    m = (ManusLudus*)piscina_allocare(piscina, magnitudo(ManusLudus));
    si (!m) { redde NIHIL; }
    m->d = d; m->tempus = M; m->gradus_ms = L; m->piscina = piscina;
    m->causa = chorda_ex_literis("", piscina);
    redde m;
}

/* ---- selectores ---- */

nomen enumeratio { SEL_ID, SEL_PARTES, SEL_ACTIO, SEL_TITULUS, SEL_MALUS } SelGenus;

interior SelGenus
selector_legere (constans character* sel, chorda* valor, Piscina* p)
{
    constans character* aeq;
    constans character* fin;
    memoriae_index      n;

    si (sel[0] == '#') { *valor = chorda_ex_literis(sel + I, p); redde SEL_ID; }
    si (sel[0] != '[') { redde SEL_MALUS; }
    aeq = strchr(sel, '=');
    fin = strchr(sel, ']');
    si (!aeq || !fin || fin < aeq) { redde SEL_MALUS; }
    n = (memoriae_index)(fin - aeq - I);
    valor->mensura = (i32)n;
    valor->datum   = (i8*)piscina_allocare(p, n + I);
    memcpy(valor->datum, aeq + I, n);
    si (strncmp(sel + I, "partes=", VII) == ZEPHYRUM) { redde SEL_PARTES; }
    si (strncmp(sel + I, "actio=",  VI)  == ZEPHYRUM) { redde SEL_ACTIO; }
    si (strncmp(sel + I, "titulus=", VIII)  == ZEPHYRUM) { redde SEL_TITULUS; }
    redde SEL_MALUS;
}

interior Componens*
quaerere (Componens* c, SelGenus g, chorda v, Piscina* p)
{
    i32        i;
    i32        n;
    Componens* r;
    b32        congruit;

    commutatio (g)
    {
        casus SEL_ID:     congruit = chorda_aequalis(c->id, v); frange;
        casus SEL_PARTES: congruit = chorda_aequalis_literis(v, partes_titulus(c->partes)); frange;
        casus SEL_ACTIO:  congruit = chorda_aequalis(c->actio, v); frange;
        casus SEL_TITULUS:  congruit = chorda_aequalis(c->titulus, v); frange;
        ordinarius:       congruit = FALSUM; frange;
    }
    si (congruit) { redde c; }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        r = quaerere(componens_liberum(c, i), g, v, p);
        si (r) { redde r; }
    }
    redde NIHIL;
}

Componens*
manus_ludus_invenire (ManusLudus* m, constans character* selector)
{
    SelGenus g;
    chorda   v;
    g = selector_legere(selector, &v, m->piscina);
    si (g == SEL_MALUS) { m->causa = chorda_ex_literis("selector malus", m->piscina); redde NIHIL; }
    redde quaerere(dispensator_arbor(m->d), g, v, m->piscina);
}

Punctum
manus_ludus_ad_schirmum (ManusLudus* m, Componens* c, Punctum locale)
{
    Punctum    p;
    Componens* a;
    (vacuum)m;
    p.x = (i32)((s32)c->fines.x + (s32)locale.x);
    p.y = (i32)((s32)c->fines.y + (s32)locale.y);
    per (a = c->parens; a; a = a->parens)
    {
        p.x = (i32)((s32)p.x * (s32)a->scala + (s32)a->fines.x + (s32)a->translatio.x);
        p.y = (i32)((s32)p.y * (s32)a->scala + (s32)a->fines.y + (s32)a->translatio.y);
    }
    redde p;
}

/* ---- eventus ---- */

interior vacuum
mus (ManusLudus* m, eventus_genus_t genus, i32 x, i32 y)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = genus; e.tempus = m->tempus; e.datum.mus.x = x; e.datum.mus.y = y;
    dispensator_tractare(m->d, &e);
    m->tempus += m->gradus_ms;
}

b32
manus_ludus_premere_ad (ManusLudus* m, i32 x, i32 y)
{
    mus(m, EVENTUS_MUS_DEPRESSUS, x, y);
    mus(m, EVENTUS_MUS_LIBERATUS, x, y);
    redde VERUM;
}

b32
manus_ludus_premere (ManusLudus* m, constans character* selector)
{
    Componens* c;
    Punctum    centrum;
    c = manus_ludus_invenire(m, selector);
    si (!c) { m->causa = chorda_ex_literis("selector nihil invenit", m->piscina); redde FALSUM; }
    centrum.x = c->fines.latitudo / II; centrum.y = c->fines.altitudo / II;
    centrum = manus_ludus_ad_schirmum(m, c, centrum);
    redde manus_ludus_premere_ad(m, centrum.x, centrum.y);
}

b32
manus_ludus_movere (ManusLudus* m, i32 x, i32 y)
{
    mus(m, EVENTUS_MUS_MOTUS, x, y);
    redde VERUM;
}

b32
manus_ludus_trahere (ManusLudus* m, constans character* selector, constans Punctum* puncta, i32 n)
{
    Componens* c;
    Punctum    p;
    i32        i;
    c = manus_ludus_invenire(m, selector);
    si (!c || n < I) { m->causa = chorda_ex_literis("trahere: selector aut puncta", m->piscina); redde FALSUM; }
    p = manus_ludus_ad_schirmum(m, c, puncta[0]);
    mus(m, EVENTUS_MUS_DEPRESSUS, p.x, p.y);
    per (i = I; i < n; i++) { p = manus_ludus_ad_schirmum(m, c, puncta[i]); mus(m, EVENTUS_MUS_MOTUS, p.x, p.y); }
    mus(m, EVENTUS_MUS_LIBERATUS, p.x, p.y);
    redde VERUM;
}

b32
manus_ludus_clavem (ManusLudus* m, character typus, i32 modificantes)
{
    Eventus e;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_CLAVIS_DEPRESSUS; e.tempus = m->tempus;
    e.datum.clavis.typus = typus; e.datum.clavis.modificantes = modificantes;
    dispensator_tractare(m->d, &e);
    m->tempus += m->gradus_ms;
    e.genus = EVENTUS_CLAVIS_LIBERATUS; e.tempus = m->tempus;
    dispensator_tractare(m->d, &e);
    m->tempus += m->gradus_ms;
    redde VERUM;
}

vacuum
manus_ludus_exspectare (ManusLudus* m, s64 ms)
{
    m->tempus += ms;
    dispensator_pulsare(m->d, m->tempus);
}

b32    manus_ludus_existit (ManusLudus* m, constans character* selector) { redde manus_ludus_invenire(m, selector) != NIHIL; }
chorda manus_ludus_focus   (ManusLudus* m)                               { redde dispensator_focus(m->d); }
chorda manus_ludus_causa   (constans ManusLudus* m)                      { redde m->causa; }
```

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh manus_ludus` — Expected: PASS.

- [ ] **Step 5: Worklog + commit**

`lib/manus_ludus.worklog.md`: `## 2026-09-04 — natus` + "In-process hand over a dispensator: same verbs as manus.h, synchronous, selectors over the logical tree, time advanced only by exspectare. Unifying under manus.h as a second transport is a named deferral (manus.h is vitrea/imperium-shaped, 4,070 lines)."
```bash
git add include/manus_ludus.h lib/manus_ludus.c lib/manus_ludus.worklog.md probationes/probatio_manus_ludus.c compile_tests_fontes_generata.sh
git commit -m "ludus: T11 - manus_ludus: manus in processu, selectores super arborem logicam, CREDO_MANUS_LUDUS_*"
```

---

### Task 12: the replay harness — tiers 3/4/5 on the toy, exemplar discipline

**Files:**
- Create: `probationes/pictor_commune.h`, `probationes/pictor/toy.eventus.stml`, `probationes/probatio_pictor_toy.c`
- Created by first run, promoted by hand: `probationes/pictor/toy.arbor.stml`, `probationes/pictor/toy.insulae.stml`

**Interfaces:**
- Consumes: `eventus_legere_stml` (T5), `Dispensator` (T10), `componens_scribere_stml` (T3), `insula_scribere`/`insula_restituere` (T6), `filum.h` (exists: read/write a file into/from a chorda — verify the two signatures at `include/filum.h` and adjust the two call sites in `pictor_commune.h` if they differ from `filum_legere(via, piscina) → chorda` and `filum_scribere(via, contentum) → b32`).
- Produces: `exemplar_conferre(piscina, via_exemplar, candidatus) → b32` (the exemplar / candidatus / differentia convention on text).

- [ ] **Step 1: Write the exemplar helper and the event log**

`probationes/pictor_commune.h`:
```c
/* pictor_commune.h - exemplar / candidatus / differentia in TEXTU
 *
 * Exemplar absens: candidatus ut <via>.novum scribitur, probatio
 * CADIT cum nuntio 'promove' - promotio est mv manu (disciplina
 * specimen.h). Discrepantia: .novum scriptus, probatio cadit. */
#ifndef PICTOR_COMMUNE_H
#define PICTOR_COMMUNE_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "filum.h"
#include <stdio.h>

interior b32
exemplar_conferre (Piscina* piscina, constans character* via_exemplar, chorda candidatus)
{
    chorda exemplar;
    chorda via_novum;

    via_novum = chorda_concatenare(chorda_ex_literis(via_exemplar, piscina),
                                   chorda_ex_literis(".novum", piscina), piscina);
    /* '.stml.novum' -> praefer '<basis>.novum.stml' si vis; hic simplex */
    exemplar = filum_legere(via_exemplar, piscina);
    si (exemplar.mensura == ZEPHYRUM)
    {
        filum_scribere(chorda_ut_cstr(via_novum, piscina), candidatus);
        imprimere("EXEMPLAR DEEST: %s - candidatus scriptus ad %s; inspice et promove (mv)\n",
                  via_exemplar, chorda_ut_cstr(via_novum, piscina));
        redde FALSUM;
    }
    si (chorda_aequalis(exemplar, candidatus)) { redde VERUM; }
    filum_scribere(chorda_ut_cstr(via_novum, piscina), candidatus);
    imprimere("DIFFERENTIA: %s vs %s (diff -u ea)\n", via_exemplar, chorda_ut_cstr(via_novum, piscina));
    redde FALSUM;
}

#endif /* PICTOR_COMMUNE_H */
```
`probationes/pictor/toy.eventus.stml` (authored — the recorded scenario: click b1, hover to tabula, three-point stroke, Escape, tick past quiet):
```xml
<eventus_index>
  <eventus genus="mus_depressus" tempus="1000" x="35" y="20" botton="0" modificantes="0"/>
  <eventus genus="mus_liberatus" tempus="1050" x="35" y="20" botton="0" modificantes="0"/>
  <eventus genus="mus_motus"     tempus="1100" x="120" y="50" botton="0" modificantes="0"/>
  <eventus genus="mus_depressus" tempus="1200" x="120" y="50" botton="0" modificantes="0"/>
  <eventus genus="mus_motus"     tempus="1220" x="125" y="55" botton="0" modificantes="0"/>
  <eventus genus="mus_motus"     tempus="1240" x="130" y="60" botton="0" modificantes="0"/>
  <eventus genus="mus_liberatus" tempus="1300" x="130" y="60" botton="0" modificantes="0"/>
  <eventus genus="clavis_depressus" tempus="1400" clavis="0" typus="27" modificantes="0"/>
  <eventus genus="clavis_liberatus" tempus="1420" clavis="0" typus="27" modificantes="0"/>
  <eventus genus="nihil" tempus="2000"/>
</eventus_index>
```

- [ ] **Step 2: Write the failing probatio**

`probationes/probatio_pictor_toy.c`:
```c
/* probatio_pictor_toy.c - gradus III/IV/V super applicationem ludicram:
 * plagula eventuum -> insulae + arbor; exemplar; restitutio (L8). */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "filum.h"
#include "eventus_stml.h"
#include "dispensator.h"
#include "ludus_toy.h"
#include "pictor_commune.h"
#include "credo.h"
#include <stdio.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
                 chorda  fons;
                   Xar*  eventus;
                    i32  i;
                    i32  n;
                 chorda  arbor_textus;
                 chorda  insulae_textus;
              Componens* arbor_ante;
                chorda*  a;

    piscina = piscina_generare_dynamicum("probatio_pictor_toy", LXIV * M);
    si (!piscina) { imprimere("FRACTA: piscina\n"); redde I; }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern, "<documentum/>", "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    toy.compositiones = ZEPHYRUM;
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere, &toy, CCC);

    imprimere("\n--- Gradus V: plagula eventuum -> status ---\n");
    fons = filum_legere("probationes/pictor/toy.eventus.stml", piscina);
    CREDO_CHORDA_NON_VACUA (fons);
    eventus = eventus_legere_stml(chorda_ut_cstr(fons, piscina), piscina, intern);
    CREDO_NON_NIHIL (eventus);
    n = xar_numerus(eventus);
    CREDO_AEQUALIS_I32 (n, X);
    per (i = ZEPHYRUM; i < n; i++) { dispensator_tractare(d, (constans Eventus*)xar_obtinere(eventus, i)); }

    /* asserta semantica - valent etiam sine exemplaribus */
    a = insula_attributum(repo, INSULA_EPHEMERA, "numerus");       CREDO_NON_NIHIL (a); CREDO_CHORDA_AEQUALIS_LITERIS (*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "ictus_numerus"); CREDO_NON_NIHIL (a); CREDO_CHORDA_AEQUALIS_LITERIS (*a, "1");
    a = insula_attributum(repo, INSULA_DURABILIS, "puncta");        CREDO_NON_NIHIL (a); CREDO_CHORDA_AEQUALIS_LITERIS (*a, "3");
    a = insula_attributum(repo, INSULA_EPHEMERA, "fuga");          CREDO_NON_NIHIL (a); CREDO_CHORDA_AEQUALIS_LITERIS (*a, "verum");
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "tabula");
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_super(d), "tabula");

    imprimere("\n--- Gradus III: imago arboris logicae ---\n");
    arbor_textus = componens_scribere_stml(dispensator_arbor(d), piscina, intern, VERUM);
    CREDO_VERUM (exemplar_conferre(piscina, "probationes/pictor/toy.arbor.stml", arbor_textus));

    imprimere("\n--- Imago insularum ---\n");
    insulae_textus = chorda_concatenare(insula_scribere(repo, INSULA_DURABILIS, piscina),
                                        insula_scribere(repo, INSULA_EPHEMERA, piscina), piscina);
    CREDO_VERUM (exemplar_conferre(piscina, "probationes/pictor/toy.insulae.stml", insulae_textus));

    imprimere("\n--- Gradus IV: restitutio (L8) ---\n");
    arbor_ante = componens_legere_stml(chorda_ut_cstr(arbor_textus, piscina), piscina, intern);
    CREDO_VERUM (insula_restituere(repo));
    dispensator_recomponere(d);
    CREDO_VERUM (componens_aequalis(arbor_ante, dispensator_arbor(d)));
    CREDO_CHORDA_AEQUALIS_LITERIS (dispensator_focus(d), "tabula");   /* focus restitutionem supervivit */

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 3: First run — exemplars are born**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh pictor_toy`
Expected: the semantic assertions PASS; the two `exemplar_conferre` calls FAIL with `EXEMPLAR DEEST`, writing `probationes/pictor/toy.arbor.stml.novum` and `toy.insulae.stml.novum`. Open both, confirm they read as expected (three componentia, focus=tabula, numerus=1, ictus_numerus=1, puncta=3, fuga=verum, pan/zoom flushed by the final tick), then promote:
```bash
mv probationes/pictor/toy.arbor.stml.novum   probationes/pictor/toy.arbor.stml
mv probationes/pictor/toy.insulae.stml.novum probationes/pictor/toy.insulae.stml
```

- [ ] **Step 4: Second run — green**

Run: `./compile_tests.sh pictor_toy` — Expected: PASS, all tiers.
Then the full substrate: `./compile_tests.sh mandatum && ./compile_tests.sh componens && ./compile_tests.sh derivare && ./compile_tests.sh eventus_stml && ./compile_tests.sh insula && ./compile_tests.sh motus && ./compile_tests.sh destinatio && ./compile_tests.sh actio && ./compile_tests.sh dispensator && ./compile_tests.sh manus_ludus` — all PASS.

- [ ] **Step 5: Commit**

```bash
git add probationes/pictor_commune.h probationes/pictor/toy.eventus.stml probationes/pictor/toy.arbor.stml probationes/pictor/toy.insulae.stml probationes/probatio_pictor_toy.c compile_tests_fontes_generata.sh
git commit -m "ludus: T12 - replay: plagula eventuum -> imago arboris + insularum; exemplar/candidatus/differentia in textu; restitutio (L8)"
```

---

## Self-Review

**1. Spec coverage (pictor-spec.md, P0–P2 scope):**

| spec | task |
|---|---|
| §2.1 two trees | T2 (mandata, no id/actio), T3 (componens owns bounds) |
| §2.2 three genera, portae, restore, canon | T6 (durabilis/ephemera), T7 (motus struct + quiet flush) |
| §2.3 `Eventus.tempus`, derivare | T1, T4; hover/focus derivation T10 |
| §2.4 destinatio strategy, focus, capture, staleness | T8, T10 |
| §2.5 command list, STML serialization | T2 |
| §3.1 componens fields | T3 |
| §3.2 six primitives | T2 |
| §3.3 figurae registry | **P3** (not in this plan — no drawing yet; `mandata` is the contract it will emit into) |
| §6.1 tiers 1–5 | T4 (1), T10 (2, via toy actiones), T12 (3, 4, 5) |
| §6.2 files + promotion | T12 |
| §6.3 manus natively | T11 (`manus_ludus`, in-process transport) |
| §6.4 codices | **separate plan**; annotations `<componens/> <purus/> <quies/> <tractator/>` written in T4, T6, T7, T9, T10 headers and the toy |
| §7 modules P0–P2 | all present except `pictor_*`, `figura`, `delineare_mandata` (P3) |
| §8 P0 | T1, T2, T3, T4, T5 (eventus_stml folded into P0 as the log format), T12's convention |
| §8 P1 | T6, T7; canon hook proven with a toy canon (pictor's real schemas are P3) |
| §8 P2 | T8, T9, T10, T11 |

**2. Deviations from the spec, recorded:**
- `manus` → `manus_ludus`: the existing `manus.h` is the vitrea/imperium async harness (4,070 lines); the native hand is synchronous and separate. Unifying as a second transport is a named deferral.
- Five derived event genera added to `fenestra.h`'s enum (`EVENTUS_MUS_INTRAVIT/EXIIT`, `EVENTUS_FOCUS_CAPTUS/AMISSUS/PETITUS`) — the spec named the derivations, not where the enumerators live.
- Focus-as-request (spec §2.4) in P2 delivers `FOCUS_PETITUS` to the root; the "owning componens adjusts" refinement is P3.
- Hit predicates: rect + `proprius` implemented; polygon and mask are P3 pulls (spec §2.4 "escalate").
- Canon semantics assumption in T6 (undeclared child = vitium) — stated with the fallback in Step 1.
- `filum_legere`/`filum_scribere` signatures assumed in T12 — verify at `include/filum.h` before Step 1.

**3. Type consistency check:** `ActioFn(InsulaRepositorium*, Motus*, constans Destinatio*, Componens*, constans Eventus*, vacuum*)` used identically in T9 header, T10 toy, T10 dispensator `mittere`. `Componere(InsulaRepositorium*, constans Motus*, Piscina*, InternamentumChorda*, vacuum*)` matches `toy_componere`. `Destinatio` fields (`id_captum`, `id_geometricum`, `ascensus`, `punctum_locale`) consistent across T8/T10/T11. `Motus.captura` set/cleared only via `motus_captura_ponere/tollere` except the toy's direct `sordida = FALSUM` (documented: a finished stroke must not flush). `insula_attributum` returns `chorda*` everywhere. `Fines`/`Punctum` defined once in `mandatum.h`.

**4. Placeholder scan:** none of TBD/TODO/"similar to". Two "verify signature" notes remain (canon rule, filum) — each accompanied by the concrete code to use and the fallback.
