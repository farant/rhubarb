# Ludus / pictor P3 — tabula, documentum, first pixels, first binary (Plan 2)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Put the first real componens on screen — a canvas you can paint on with a brush, backed by a replayable stroke log — as a standalone baked binary, with drawing, rasterizing, the document store, and the first four discipline codices landing on the way.

**Architecture:** Plan 1 ends at the logical tree. This plan adds the drawing half of the pipeline as two pure stages (`pingere`: logical tree → `Mandata`; `delineare_mandata`: `Mandata` → pixel table), the document as an acta log on `volumen` with checkpoints by sigillum, pictor's first `componere` and its first action (`penicillus.ictus`), the window glue that attaches the dispensator to `fenestra`, and the bake. Coordinates go signed first so nothing built here inherits the unsigned wart. Every task is test-first with a planted fault; tiers 6, 7, 8 join tiers 1–5.

**Tech Stack:** C89 (clang, house flags), rhubarb `lib/` — everything from Plan 1 plus `delineare`, `color`, `thema`, `fenestra` (pixel table), `imago_typus`/`imago_png`/`imago_collatio`/`specimen`, `volumen`, `sigillum`, `aedilis` + `amalgama_ligare.sh` for the bake. No new dependencies.

**Spec:** `project-specs/pictor-spec.md` (§2.1, §3.3, §4, §5.1–5.3, §6.1 tiers 6–8, §6.4 codices 1, §7, §8 P3, §10). Rationale: `project-specs/ludus-brainstorm.md` §XIV–XVI (round 4 decisions: flows, ownership, buffering, roles-as-data). Predecessor: `project-specs/pictor-plan-1-substratum.md` (its STATUS block lists the twelve landed modules and their deviations).

> **STATUS 2026-09-05 — T1 signed coordinates DONE** (eight suites green, exemplar unchanged; serializers gained `attributum_s32`/`attributum_signatum` twins; the negative guard in `invenire` is gone — see `lib/mandatum.worklog.md`). T2 `figura` + `pingere` DONE (38 assertions; probatio's pannus translates on both axes). T3 `delineare_mandata` DONE (13 assertions; the draft's frame-pop loop decremented only the pushed frame — fixed before the first build; `mandata_prima.png` promoted after inspection; the translation plant was mute, T9's pan covers that path). T4 `pictor_documentum` DONE (46 assertions; volumen interleaves its own acta so live acta are `ictus` only, checkpoints cadence on live strokes and are found by enumeration; 200 strokes in 26 ms, undo 1 ms). T5 dispensator boundary DONE (derived events deliver after the recompose; the toy replay's exact composition count became a lower bound). T6 real canons + owners DONE (apps/pictor/canones/*; attribute diff at the gate refuses a non-owner; scriptor set by the dispensator and the flush). T7 `pictor_componentia` DONE (40 assertions; `pictor.arbor.stml` exemplar shows the stroke in flight). T8 `pictor_actiones` DONE (35 assertions through the real loop; stroke counts via `numerus_vivorum`). Next: T9 pictor figurae. Plan 1 sealed at `f84e06b3`; brainstorm §XVI at `1d9b726b`. Decisions taken for this plan (Fran, 2026-09-05, "those all make sense"): signed coordinates go FIRST (T1); `pingere` reads the tree only — `componere` copies the pending stroke into the tabula componens (T7/T9); codices batch 1 is the LAST task and runs in the MAIN tree (`../rhubarb`), rebased onto this branch (T12); the flow idiom is designed in the canon task but built at P5 (T6); wheel position is a NAMED P4 PULL — `fenestra_macos.m`'s `scrollWheel:` is empty today (T10 records it, does not build it).

## Global Constraints

Copied from Plan 1 and the spec; every task's requirements include these.

- **C89 in Latin.** House flags from `tools/vexilla.sh` (`-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -O2 -g`). Declarations first in every block; no `//`; no C99.
- **latina.h owns words.** Never declare an identifier that is a latina.h macro (`nomen`, `duplex`, `magnitudo`, `registrum`, `exire`, `structura`, `casus`, `brevis`, the single capitals I V X L C D M …). `./tools/latina_custos.sh <files>` before every build. Numerals are sparse: `DL`, `DCL`, `DCCX` do not exist — write sums (`M + D + L`). Label fields are `titulus`, never `nomen`.
- **String literals are const.** A `vacuum*` context that carries text is a `character[]` array, never a literal (`-Wwrite-strings` + `-Wcast-qual`).
- **Formator law.** Section banners are `/* ` + exactly 50 `=` (`vexillum-quinquaginta`); lines ≤ 72 (`longitudo-lxxii`). For every new or edited C file, before commit: `./silva/formator.sh <f> -scribere` then `-vitia` must print `DIVERGENTIAE 0` (`CONFORMIS`). `include/fenestra.h` carries a pre-existing baseline of 6; keep its delta 0.
- **The pythonica chain (Fran's method).** NEW C file → `./silva/scribe.sh <via> -tacitus <<'EOF' … EOF` (examen judges; REICE on a test whose header does not exist yet is expected and clears when the header lands). EXISTING C → `silva.Editio(via)` / `silva.Refactio()` (token anchors; `substituere(nomen)` by name; `inserere_post(nomen, …, definitio=False)` for a prototype in a header; a `novus` with newlines is written verbatim STARTING AT THE ANCHOR'S FIRST TOKEN — never lead it with indentation; `tolerans=False` for comment-bearing anchors). Markdown → `silva.Prosa` (needs `./md/compile_probationes.sh registrum` once per fresh worktree) or `silva.Textus`. The red → `silva.planta(via, vetus, novus, 'radix', '<filtrum>')` (multi-line anchor needs a multi-line novus; `tolerans=False` when the anchor holds a comment). Run one test → `silva.probatio_currere('probatio_x')` or `./compile_tests.sh <filtrum>`. Commit → `silva.commissio(msg, viae, portae=[('radix', '<filtrum>'), …])` — it refuses unless every named gate ran green and adds the trailer. `lib/fenestra_macos.m` is the one exception: silva cannot parse Objective-C; edit it with an exact-anchor Python replace and count-assert.
- **Worktree.** Work in `/Users/francisarant/Documents/projects/rhubarb-pictor` (branch `rhubarb-pictor`). Never bare `git stash`. `git add` only named files (commissio does). After any new `lib/*.c` or `probationes/*.c`: `./tools/compile_tests_fontes_generare.sh`. The root suite is known-red here on `probatio_git` and the stml goldens; gate on filters.
- **Sealed names (2026-09-04):** ludus, pictor, componens/componere, pingere, mandatum/mandata, figura, coetus, insula, actio, partes, destinatio, mobilis/Motus, restituere, dispensator, derivare, delineare_mandata. Still working: `manus_ludus`. NEW in this plan, proposed: `pictor_documentum`, `figura_tabulae`, `prospectus`, `ludus_fenestra` (the window glue), `tabula_pixelorum_creare_nuda` (window-free pixel table). Seal or rename at T11.
- **Discipline the code must exhibit** (the codices of T12 will lint it; until then, by hand): L1 islands change only through `mutare_durabile`/`mutare_ephemera`/`mutare_motum`; L2 `componere` and `pingere` and every figura are pure (`/* <purus/> */` anchor, no writes, no I/O); L5 handlers do no I/O (`/* <tractator/> */`); L7 no file-scope mutable statics in ludus libs; L9 the strategy reads the logical tree only, never mandata.
- **Round-4 decisions (brainstorm §XVI) that bind here:** reads global, writes OWNED — the real canons (T6) declare an owner per attribute and `insula` refuses a non-owner's write by attribute diff; derived and addressed events deliver at the NEXT event boundary (T5 makes `mittere_ad` an enqueue); roles are data, never types — no per-componens C struct with a function table; frame double-buffering is a named option, its trigger the rasterizer's share of the frame (T10 measures it).
- **Exemplars.** Text exemplars: `probationes/pictor/*.stml` via `pictor_commune.h` (`.novum` candidates gitignored, promoted by `mv` after inspection). Pixel exemplars: `probationes/pictor/specimina/*.png` via `specimen_iudicare` (a missing exemplar is a FRACTURA that names the candidate it wrote; promote by `mv`; the scrinium is NOT created by the library — `mkdir -p` it once). Never regenerate an exemplar without reading the diff.
- **Worklogs.** Every module gets `lib/<x>.worklog.md` with `## 2026-09-DD — natus (ludus P3 T#)`: what it is, deviations from this plan and WHY, the gate, the planted fault, the verdicts. Tick this plan's checkboxes per task and keep the STATUS block above current.

---

## File Structure

| File | Responsibility | Task |
|---|---|---|
| `include/mandatum.h`, `lib/mandatum.c` | `Punctum`/`Fines` become `s32`; `fines_continet` signed | T1 |
| `include/componens.h`, `lib/componens.c`, `lib/destinatio.c`, `lib/manus_ludus.c`, five probationes | ripple of T1; negative-space tests | T1 |
| `include/figura.h`, `lib/figura.c` | figura registry `(partes, thema) → fn`; `pingere` | T2 |
| `include/delineare_mandata.h`, `lib/delineare_mandata.c` | `Mandata` → `TabulaPixelorum`; window-free pixel table; `imago_ex_tabula` | T3 |
| `include/pictor_documentum.h`, `lib/pictor_documentum.c` | acta on `volumen`; projection; sigillum; checkpoints; undo/redo | T4 |
| `include/dispensator.h`, `lib/dispensator.c`, `probationes/ludus_toy.h` | derived events enqueue to the next boundary (§XVI §4); the toy observes deliveries | T5 |
| `include/insula.h`, `lib/insula.c`, `probationes/pictor/*.canon` | owner per attribute; refusal by diff; the real canons | T6 |
| `include/pictor_componentia.h`, `lib/pictor_componentia.c` | pictor's `componere`: prospectus, tabula, status_linea | T7 |
| `include/pictor_actiones.h`, `lib/pictor_actiones.c` | `penicillus.ictus`, `instrumentum.eligere` | T8 |
| `lib/pictor_figurae.c` (+ header) | `figura_tabulae` (imago + pending stroke + cursor), `figura_status` | T9 |
| `include/ludus_fenestra.h`, `lib/ludus_fenestra.c` | window glue; the frame; timing (the double-buffer trigger) | T10 |
| `apps/pictor/pictor.c`, `apps/pictor/pictor.sh`, `apps/pictor/fumus.sh` | the binary; the bake; the numbers | T11 |
| `silva/…` (main tree) | annotation vocabulary + codices L1, L2, L7, L9 | T12 |
| `probationes/probatio_*.c`, `probationes/pictor/…` | one probatio per module; replay logs; exemplars; specimina | all |

---

### Task 1: Signed coordinates — `Punctum` and `Fines` become `s32`

**Files:**
- Modify: `include/mandatum.h` (the two structs, `fines_continet`), `lib/mandatum.c` (`fines_continet`, the x/y/translatio serializers), `include/componens.h` + `lib/componens.c` (`componens_ponere_transformatio` signature; x/y/translatio serializers), `lib/destinatio.c` (`invenire`: the negative guard goes), `lib/manus_ludus.c` (`manus_ludus_ad_schirmum`: casts go), `lib/eventus_stml.c` (unchanged — mouse stays `i32` in `Eventus`)
- Modify tests: `probationes/probatio_mandatum.c`, `probatio_componens.c`, `probatio_motus.c`, `probatio_destinatio.c`, `probatio_manus_ludus.c` (assertion macros; new negative cases)

**Interfaces:**
- Consumes: Plan 1 as landed.
- Produces: `nomen structura { s32 x; s32 y; } Punctum;` `nomen structura { s32 x; s32 y; s32 latitudo; s32 altitudo; } Fines;` `b32 fines_continet(Fines, Punctum)` signed; `vacuum componens_ponere_transformatio(Componens*, s32 tx, s32 ty, i32 scala)`; `i32 mandata_coetus_incipere(Mandata*, Fines, b32 sectio, s32 tx, s32 ty, i32 scala, chorda provenientia)`. `Eventus.datum.mus.x/y` stay `i32` (fenestra's); the strategy casts at its boundary. `scala`, `crassitudo`, counts stay `i32`.

- [x] **Step 1: Write the failing tests (negative space)**

Append to `probationes/probatio_motus.c` before the `--- Captura ---` section (use `silva.Editio.replace` on the `imprimere("\n--- Captura ---\n");` anchor, novus with newlines):
```c
    imprimere("\n--- Pan negativum: coordinatae signatae ---\n");
    mutare_motum(&motus, pan_negare, NIHIL, IV * M);
    CREDO_AEQUALIS_S32(motus.pan.x, -XL);
    CREDO_VERUM(motus_quies(&motus, V * M, CCC));
    CREDO_VERUM(motus_effundere(&motus, repo));
    a = insula_attributum(repo, INSULA_EPHEMERA, "pan_x");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "-40");

    imprimere("\n--- Captura ---\n");
```
and the mutator, inserted after `pan_movere` (`e.inserere_post('pan_movere', …)`):
```c
/* Mutator: pan negativum (spatium signatum) */
interior vacuum
pan_negare (
    Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -XL;
    motus->pan.y = -X;
}
```
Append to `probationes/probatio_destinatio.c` before `--- Extra omnia -> radix ---`:
```c
    imprimere("\n--- Liberum cum finibus NEGATIVIS (signatum) ---\n");
    /* umbra (-20,-20,40,40) in panno: schirmo 50..90, 30..70 -
     * ante translationem panni (0,50) et originem (70,0) */
    umbra = nodus(piscina, intern, "umbra", PARTES_NULLUM,
                  -XX, -XX, XL, XL, FALSUM);
    componens_addere_liberum(pannus, umbra);
    e = mus(EVENTUS_MUS_DEPRESSUS, LXXV, XXXV);
    d = destinatio_geometrica(radix, &motus, vacua, &e, piscina);
    CREDO_CHORDA_AEQUALIS_LITERIS(d.id_geometricum, "umbra");
    CREDO_AEQUALIS_S32(d.punctum_locale.x, XXV);
    CREDO_AEQUALIS_S32(d.punctum_locale.y, V);
```
(declare `Componens* umbra;` in the block; the `nodus` helper's `x, y, w, h` parameters become `s32`). Check the arithmetic: screen (75,35) → pannus space (75-70-0, 35-0-50) = (5,-15) → umbra fines (-20,-20,40,40) contains it → locale (5+20, -15+20) = (25, 5). With unsigned coordinates this point was unreachable (the negative guard skipped every child).

In every one of the five probationes, every `CREDO_AEQUALIS_I32` whose first argument is a `.x`, `.y`, `.latitudo`, or `.altitudo` of a `Punctum`/`Fines` becomes `CREDO_AEQUALIS_S32`. Do it by script, count-asserted:
```python
import sys, re; sys.path.insert(0, 'pythonica'); import silva
r = silva.Refactio()
for via in ('probationes/probatio_mandatum.c', 'probationes/probatio_componens.c',
            'probationes/probatio_motus.c', 'probationes/probatio_destinatio.c',
            'probationes/probatio_manus_ludus.c'):
    e = r.editio(via)
    text = open(via).read()
    sites = re.findall(r'CREDO_AEQUALIS_I32\(([^,]*\.(?:x|y|latitudo|altitudo)),', text)
    for expr in sorted(set(sites)):
        n = text.count('CREDO_AEQUALIS_I32(%s,' % expr)
        e.replace('CREDO_AEQUALIS_I32(%s,' % expr, 'CREDO_AEQUALIS_S32(%s,' % expr, numerus=n)
    print(via, len(sites), 'sites')
fr = r.applicare(); assert fr.sana
```

- [x] **Step 2: Run to verify it fails**

Run: `./compile_tests.sh motus` — Expected: FAIL to compile (`-XL` into `i32`: "implicit conversion changes signedness" under `-Wsign-conversion -Werror`). `./compile_tests.sh destinatio` — Expected: FAIL the same way on `-XX`.

- [x] **Step 3: The type change, by name**

```python
import sys; sys.path.insert(0, 'pythonica'); import silva
r = silva.Refactio()
h = r.editio('include/mandatum.h')
h.substituere('Punctum', 'nomen structura { s32 x; s32 y; } Punctum;', genus='typus')
h.substituere('Fines',
    'nomen structura { s32 x; s32 y; s32 latitudo; s32 altitudo; } Fines;', genus='typus')
c = r.editio('lib/mandatum.c')
c.substituere('fines_continet', '''b32
fines_continet (
      Fines f,
    Punctum p)
{
    redde p.x >= f.x && p.x < f.x + f.latitudo
        && p.y >= f.y && p.y < f.y + f.altitudo;
}''')
fr = r.applicare(); assert fr.sana
```
Then the ripple, compile-error driven under the house flags — each error names a site; the fixes are mechanical and of exactly these kinds:
- `chorda_ex_i32(...)` / `chorda_ut_i32(...)` on an `x`, `y`, `latitudo`, `altitudo`, `translatio.*` field → the `chorda_ex_s32` / `chorda_ut_s32` twin (`lib/mandatum.c`, `lib/componens.c` serializers; find them with `grep -n 'chorda_ex_i32\|chorda_ut_i32' lib/mandatum.c lib/componens.c` and change only the coordinate sites).
- `componens_ponere_transformatio(Componens* c, i32 translatio_x, i32 translatio_y, i32 scala)` → `s32 translatio_x, s32 translatio_y` (substitute the definition by name, then `r.prototypum_synchronizare('lib/componens.c', 'include/componens.h', 'componens_ponere_transformatio')`).
- `mandata_coetus_incipere(..., i32 tx, i32 ty, ...)` → `s32 tx, s32 ty` and `mandata_textus(Mandata*, i32 x, i32 y, …)` → `s32 x, s32 y`, the same way in `lib/mandatum.c` / `include/mandatum.h`.
- `lib/destinatio.c` `invenire` — substitute by name; the negative guard is GONE (a child may now have negative fines), the arithmetic loses its casts:
```c
interior Componens*
invenire (
    Componens* c,
       Punctum p,
      Punctum* locale)
{
    Componens* tactum;
       Punctum q;
           s32 scala;
           i32 i;
           b32 intra;

    intra = fines_continet(c->fines, p);
    si (c->sectio && !intra)
    {
        redde NIHIL;
    }
    scala = (s32)c->scala;
    si (scala <= ZEPHYRUM)
    {
        scala = I;
    }
    /* in spatium liberorum: origo c, translatio, scala */
    q.x = (p.x - c->fines.x - c->translatio.x) / scala;
    q.y = (p.y - c->fines.y - c->translatio.y) / scala;
    i = componens_numerus_liberorum(c);
    dum (i > ZEPHYRUM)
    {
        i--;
        tactum = invenire(componens_liberum(c, i), q, locale);
        si (tactum)
        {
            redde tactum;
        }
    }
    si (intra)
    {
        locale->x = p.x - c->fines.x;
        locale->y = p.y - c->fines.y;
        redde c;
    }
    redde NIHIL;
}
```
  and in `destinatio_geometrica` the boundary cast: `p.x = (s32)ev->datum.mus.x; p.y = (s32)ev->datum.mus.y;`.
- `lib/manus_ludus.c` `manus_ludus_ad_schirmum` — substitute by name:
```c
Punctum
manus_ludus_ad_schirmum (
    ManusLudus* m,
     Componens* c,
        Punctum locale)
{
       Punctum p;
    Componens* a;

    (vacuum)m;
    p = locale;
    si (!c)
    {
        redde p;
    }
    p.x = c->fines.x + locale.x;
    p.y = c->fines.y + locale.y;
    per (a = c->parens; a; a = a->parens)
    {
        p.x = p.x * (s32)a->scala + a->fines.x + a->translatio.x;
        p.y = p.y * (s32)a->scala + a->fines.y + a->translatio.y;
    }
    redde p;
}
```
  and `mus(m, genus, p.x, p.y)` callers cast `(i32)p.x, (i32)p.y` at the `Eventus` boundary (the hand's `mus` helper takes `s32 x, s32 y` and casts inside, one place).
- `lib/motus.c` `numerus_ut_cstr(i32 valor, …)` → `s32 valor` with `chorda_ex_s32(valor, …)`; `zoom` stays `i32` and is cast at the call.
- `probationes/ludus_toy.h`: `Fines f` literals need no change; `toy_punctum_addere` unchanged.
- The `nodus` helper in `probatio_destinatio.c` takes `s32 x, y, w, h`; the `mus` helper there keeps `i32` (it fills `Eventus`).

Run `./tools/latina_custos.sh` on every touched file, then `./silva/formator.sh <f> -scribere` + `-vitia` on each.

- [x] **Step 4: Run to verify it passes**

Run, in this order (each must PASS): `./compile_tests.sh mandatum && ./compile_tests.sh componens && ./compile_tests.sh motus && ./compile_tests.sh destinatio && ./compile_tests.sh manus_ludus && ./compile_tests.sh dispensator && ./compile_tests.sh pictor_toy && ./compile_tests.sh eventus_stml`. The `pictor_toy` exemplar `toy.arbor.stml` must NOT change (coordinates were non-negative; the serializer prints the same digits) — if it does, read the diff before promoting anything.

- [x] **Step 5: Plant, worklog, commit**

Plant (must go RED on `motus`): in `lib/mandatum.c` `fines_continet`, `p.x >= f.x` → `p.x > f.x` — no; that is an off-by-one that the toy's corner cases may not catch. Plant instead in `lib/destinatio.c` `invenire`: `q.x = (p.x - c->fines.x - c->translatio.x) / scala;` → `q.x = (p.x - c->fines.x) / scala;` (translation dropped) — red at the `umbra` assertion and the tabula-through-pannus assertion. `silva.planta('lib/destinatio.c', vetus, novus, 'radix', 'destinatio')`.

Worklog: this is a cross-cutting change, so it goes in `lib/mandatum.worklog.md` (the owner of the types) as `## 2026-09-DD — coordinatae signatae (ludus P3 T1)`: why now (before any consumer of negative pan exists), what rippled, that the negative-guard in `invenire` was a workaround for unsigned space and is gone, that `Eventus` stays `i32` at the platform boundary.

Commit: `silva.commissio("ludus P3: T1 - coordinatae signatae: Punctum/Fines s32 …", viae, portae=[('radix', f) for f in ('mandatum','componens','motus','destinatio','manus_ludus','dispensator','pictor_toy','eventus_stml')])`.

---

### Task 2: `figura` — the registry and `pingere`

**Files:**
- Create: `include/figura.h`, `lib/figura.c`, `lib/figura.worklog.md`
- Test: `probationes/probatio_figura.c`

**Interfaces:**
- Consumes: `Componens` (T3 of Plan 1), `Mandata` (`mandata_creare`, `mandata_coetus_incipere/finire`, `mandata_rectangulum`, `mandata_linea`, `mandata_textus`, `mandata_numerus`, `mandata_obtinere`, `mandata_scribere_stml`), `Partes`.
- Produces: `FiguraFn` = `vacuum (*)(constans Componens* c, Mandata* m, i32 thema, vacuum* ctx)` — emits primitives in the componens's OWN space (origin = its fines' corner; `pingere` opens the coetus); `FiguraRegistrum`; `figura_registrum_creare(piscina)`; `figura_registrare(reg, Partes, i32 thema, FiguraFn, ctx) → b32` (duplicate (partes, thema) refused); `figura_invenire(reg, Partes, i32 thema, FiguraFn*, vacuum**) → b32`; `pingere(constans Componens* radix, constans FiguraRegistrum* reg, i32 thema, Mandata* m)` — DFS, one `coetus` per componens (fines, sectio, translatio, scala, provenientia = id), figura (if any) BEFORE children, `<purus/>`; `figura_finium(c, m, thema, ctx)` — the trivial built-in figura (outline of the fines in `COLOR_BORDER`) used by the probatio and as the fallback nobody registers.
- Thema is an `i32` id chosen by the caller (v1: `ZEPHYRUM` only); the house `thema.h` palette is consulted by the RASTERIZER, not here — a figura names colors as `ColorMandati` (`COLOR_MANDATI_THEMA` + `ColorThema` value, or `INDEX`, or `RGBA`).

- [x] **Step 1: Write the failing test**

`probationes/probatio_figura.c` (via `./silva/scribe.sh`):
```c
/* probatio_figura.c - registrum figurarum, pingere (gradus VI) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "thema.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "credo.h"
#include <stdio.h>

/* Figura probationis: rectangulum plenum coloris thematis + titulus */
interior vacuum
figura_bottonis_probationis (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx)
{
    Fines        f;
    ColorMandati color;

    (vacuum)thema;
    (*(i32*)ctx)++;
    f.x        = ZEPHYRUM;
    f.y        = ZEPHYRUM;
    f.latitudo = c->fines.latitudo;
    f.altitudo = c->fines.altitudo;
    color.genus = COLOR_MANDATI_THEMA;
    color.valor = (i32)COLOR_ACCENT_PRIMARY;
    mandata_rectangulum(m, f, color, VERUM);
    color.valor = (i32)COLOR_TEXT;
    mandata_textus(m, ZEPHYRUM, ZEPHYRUM, c->titulus, ZEPHYRUM, color);
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
    Componens* c;
         Fines f;

    c = componens_creare(p, in, id, partes);
    f.x        = x;
    f.y        = y;
    f.latitudo = w;
    f.altitudo = h;
    componens_ponere_fines(c, f);
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
        FiguraRegistrum* reg;
              Componens* radix;
              Componens* pannus;
              Componens* b1;
                Mandata* m;
                Mandatum* x;
                FiguraFn fn;
                vacuum*  ctx;
                    i32  vocationes;

    piscina = piscina_generare_dynamicum("probatio_figura", XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern     = internamentum_creare(piscina);
    vocationes = ZEPHYRUM;

    imprimere("\n--- Registrum ---\n");
    reg = figura_registrum_creare(piscina);
    CREDO_NON_NIHIL(reg);
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                  figura_bottonis_probationis, &vocationes));
    /* duplex (partes, thema) recusatur */
    CREDO_FALSUM(figura_registrare(reg, PARTES_BOTTONE, ZEPHYRUM,
                                   figura_bottonis_probationis, &vocationes));
    /* idem partes, thema aliud: licet */
    CREDO_VERUM(figura_registrare(reg, PARTES_BOTTONE, I,
                                  figura_finium, NIHIL));
    CREDO_FALSUM(figura_registrare(reg, PARTES_CAMPUS, ZEPHYRUM, NIHIL, NIHIL));
    fn = NIHIL;
    CREDO_VERUM(figura_invenire(reg, PARTES_BOTTONE, ZEPHYRUM, &fn, &ctx));
    CREDO_VERUM(fn == figura_bottonis_probationis);
    CREDO_FALSUM(figura_invenire(reg, PARTES_TABULA, ZEPHYRUM, &fn, &ctx));

    /* radix (0,0,200,100)
     *   pannus (70,0,130,100) translatio (0,50) scala II sectio
     *     b1 (5,5,40,20) bottone "Ok" */
    radix  = nodus(piscina, intern, "radix", PARTES_NULLUM,
                   ZEPHYRUM, ZEPHYRUM, CC, C);
    pannus = nodus(piscina, intern, "pannus", PARTES_NULLUM,
                   LXX, ZEPHYRUM, CXXX, C);
    componens_ponere_transformatio(pannus, ZEPHYRUM, L, II);
    componens_ponere_sectio(pannus, VERUM);
    b1 = nodus(piscina, intern, "b1", PARTES_BOTTONE, V, V, XL, XX);
    componens_ponere_titulum(b1, "Ok");
    componens_addere_liberum(radix, pannus);
    componens_addere_liberum(pannus, b1);

    imprimere("\n--- Pingere: coetus per componens, figura ante liberos ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    CREDO_AEQUALIS_I32(vocationes, I);
    /* coetus radix, coetus pannus, coetus b1, rect, textus = V */
    CREDO_AEQUALIS_I32(mandata_numerus(m), V);
    x = mandata_obtinere(m, ZEPHYRUM);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "radix");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, V);
    x = mandata_obtinere(m, I);
    CREDO_VERUM(x->genus == MANDATUM_COETUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "pannus");
    CREDO_AEQUALIS_S32(x->fines.x, LXX);
    CREDO_AEQUALIS_S32(x->translatio.y, L);
    CREDO_AEQUALIS_I32(x->scala, II);
    CREDO_VERUM(x->sectio);
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, IV);
    x = mandata_obtinere(m, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->provenientia, "b1");
    CREDO_AEQUALIS_I32(x->magnitudo_arboris, III);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_VERUM(x->impletum);
    /* in spatio PROPRIO: origo (0,0), non (5,5) */
    CREDO_AEQUALIS_S32(x->fines.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(x->fines.latitudo, XL);
    CREDO_VERUM(x->color.genus == COLOR_MANDATI_THEMA);
    x = mandata_obtinere(m, IV);
    CREDO_VERUM(x->genus == MANDATUM_TEXTUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->textus, "Ok");

    imprimere("\n--- Thema alterum: figura alia ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, I, m);
    CREDO_AEQUALIS_I32(vocationes, I);              /* non vocata */
    /* figura_finium: rectangulum vacuum unum */
    CREDO_AEQUALIS_I32(mandata_numerus(m), IV);
    x = mandata_obtinere(m, III);
    CREDO_VERUM(x->genus == MANDATUM_RECTANGULUM);
    CREDO_FALSUM(x->impletum);

    imprimere("\n--- Partes sine figura: coetus solus ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, II, m);
    CREDO_AEQUALIS_I32(mandata_numerus(m), III);

    imprimere("\n--- Puritas: bis idem ---\n");
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    {
        Mandata* m2;
        m2 = mandata_creare(piscina, intern);
        pingere(radix, reg, ZEPHYRUM, m2);
        CREDO_VERUM(mandata_aequalia(m, m2));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
The exact `mandata_*` emitter signatures are in `include/mandatum.h` (landed in Plan 1 T2); if `mandata_textus` takes its arguments in another order, follow the header — the plan's intent is (fines, textus, fons, color).

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh figura` — Expected: FAIL, `'figura.h' file not found`.

- [x] **Step 3: Write header and implementation**

`include/figura.h`:
```c
/* figura.h - Figurae: registrum (partes, thema) -> deponere, et
 * PINGERE, arbor logica -> mandata
 *
 * Duae arbores numquam confusae (pictor-spec §2.1): componens dicit
 * QUID (partes, fines, titulus), figura dicit QUOMODO pingatur.
 * Registrum per (partes, thema) resolvit - nulla functio in
 * componente, nullus typus per partes (brainstorm XVI §5: partes
 * sunt DATA). Figura in spatio PROPRIO componentis emittit (origo =
 * angulus finium); pingere coetum aperit (fines, sectio, translatio,
 * scala, provenientia = id) et liberos post figuram ambulat.
 *
 * <purus/>: pingere et omnis figura nihil scribunt praeter mandata.
 * Lint L2. Probantur semel per thema (gradus VI), numquam per
 * widget.
 */

#ifndef FIGURA_H
#define FIGURA_H

/* <aedilis corpus="lib/figura.c"/> */

#include "latina.h"
#include "piscina.h"
#include "xar.h"
#include "componens.h"
#include "mandatum.h"

/* ==================================================
 * Typi
 * ================================================== */

nomen vacuum (*FiguraFn)(
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx);

nomen structura {
      Partes  partes;
         i32  thema;
    FiguraFn  fn;
     vacuum*  ctx;
} FiguraIntroitus;

nomen structura {
        Xar* introitus;     /* Xar de FiguraIntroitus */
    Piscina* piscina;
} FiguraRegistrum;

/* ==================================================
 * Registrum
 * ================================================== */

FiguraRegistrum*
figura_registrum_creare (
    Piscina* piscina);

/* FALSUM si fn NIHIL aut (partes, thema) iam registratum */
b32
figura_registrare (
    FiguraRegistrum* reg,
             Partes  partes,
                i32  thema,
           FiguraFn  fn,
            vacuum*  ctx);

b32
figura_invenire (
    constans FiguraRegistrum* reg,
                      Partes  partes,
                         i32  thema,
                    FiguraFn* fn_ex,
                    vacuum**  ctx_ex);

/* ==================================================
 * Pingere
 * ================================================== */

/* <purus/> arbor logica -> mandata. Coetus per componens; figura
 * (si registrata pro (partes, thema)) ante liberos. */
vacuum
pingere (
          constans Componens* radix,
    constans FiguraRegistrum* reg,
                         i32  thema,
                    Mandata*  m);

/* Figura minima: fines vacui colore COLOR_BORDER. Pro probationibus
 * et pro partibus quibus nemo figuram dedit. */
vacuum
figura_finium (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx);

#endif /* FIGURA_H */
```
`lib/figura.c`:
```c
/* figura.c - registrum figurarum et pingere */

#include "figura.h"
#include "thema.h"

/* ==================================================
 * Registrum
 * ================================================== */

interior FiguraIntroitus*
introitus_invenire (
    constans FiguraRegistrum* reg,
                      Partes  partes,
                         i32  thema)
{
    FiguraIntroitus* f;
                i32  i;
                i32  n;

    n = xar_numerus(reg->introitus);
    per (i = ZEPHYRUM; i < n; i++)
    {
        f = (FiguraIntroitus*)xar_obtinere(reg->introitus, i);
        si (f->partes == partes && f->thema == thema)
        {
            redde f;
        }
    }
    redde NIHIL;
}

FiguraRegistrum*
figura_registrum_creare (
    Piscina* piscina)
{
    FiguraRegistrum* reg;

    si (!piscina)
    {
        redde NIHIL;
    }
    reg = (FiguraRegistrum*)piscina_allocare(piscina,
                                             magnitudo(FiguraRegistrum));
    si (!reg)
    {
        redde NIHIL;
    }
    reg->introitus = xar_creare(piscina, (i32)magnitudo(FiguraIntroitus));
    reg->piscina   = piscina;
    redde reg;
}

b32
figura_registrare (
    FiguraRegistrum* reg,
             Partes  partes,
                i32  thema,
           FiguraFn  fn,
            vacuum*  ctx)
{
    FiguraIntroitus* f;

    si (!reg || !fn)
    {
        redde FALSUM;
    }
    si (introitus_invenire(reg, partes, thema))
    {
        redde FALSUM;
    }
    f = (FiguraIntroitus*)xar_addere(reg->introitus);
    f->partes = partes;
    f->thema  = thema;
    f->fn     = fn;
    f->ctx    = ctx;
    redde VERUM;
}

b32
figura_invenire (
    constans FiguraRegistrum* reg,
                      Partes  partes,
                         i32  thema,
                    FiguraFn* fn_ex,
                    vacuum**  ctx_ex)
{
    FiguraIntroitus* f;

    si (!reg || !fn_ex || !ctx_ex)
    {
        redde FALSUM;
    }
    f = introitus_invenire(reg, partes, thema);
    si (!f)
    {
        redde FALSUM;
    }
    *fn_ex  = f->fn;
    *ctx_ex = f->ctx;
    redde VERUM;
}

/* ==================================================
 * Pingere
 * ================================================== */

interior vacuum
pingere_nodum (
          constans Componens* c,
    constans FiguraRegistrum* reg,
                         i32  thema,
                    Mandata*  m)
{
    FiguraFn fn;
     vacuum* ctx;
         i32 coetus;
         i32 i;
         i32 n;

    coetus = mandata_coetus_incipere(m, c->fines, c->sectio,
                                     c->translatio.x, c->translatio.y,
                                     c->scala, c->id);
    si (figura_invenire(reg, c->partes, thema, &fn, &ctx))
    {
        fn(c, m, thema, ctx);
    }
    n = componens_numerus_liberorum(c);
    per (i = ZEPHYRUM; i < n; i++)
    {
        pingere_nodum(componens_liberum(c, i), reg, thema, m);
    }
    mandata_coetus_finire(m, coetus);
}

/* <purus/> */
vacuum
pingere (
          constans Componens* radix,
    constans FiguraRegistrum* reg,
                         i32  thema,
                    Mandata*  m)
{
    si (!radix || !reg || !m)
    {
        redde;
    }
    pingere_nodum(radix, reg, thema, m);
}

/* <purus/> */
vacuum
figura_finium (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx)
{
    Fines        f;
    ColorMandati color;

    (vacuum)thema;
    (vacuum)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    color.genus = COLOR_MANDATI_THEMA;
    color.valor = (i32)COLOR_BORDER;
    mandata_rectangulum(m, f, color, FALSUM);
}
```
`mandata_coetus_incipere` / `mandata_coetus_finire` are Plan 1 T2's; if `componens_liberum` takes a `constans Componens*` (it does) the recursion compiles as written. `pingere_nodum` recursing on children AFTER the figura is the z-order contract: a parent paints under its children.

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh figura` — Expected: PASS.

- [x] **Step 5: Plant, worklog, commit**

Plant (RED on `figura`): in `pingere_nodum`, move the figura call after the children loop (children painted under the parent) — `silva.planta` with the multi-line anchor from `si (figura_invenire(` through the `per` loop, `novus` with the two blocks swapped. Red at the `mandata_obtinere(m, III)` genus assertion (index III becomes the coetus of b1's sibling order… the rectangle moves). Green on revert.

Worklog `lib/figura.worklog.md`: the two-registry rule (componens kinds AND figurae; a figura is keyed by role, never by node); figura emits in its own space and `pingere` owns the coetus; thema is an id here and a palette in the rasterizer; `figura_finium` as the fallback nobody registers.

Commit: `silva.commissio(msg, [include/figura.h, lib/figura.c, lib/figura.worklog.md, probationes/probatio_figura.c, compile_tests_fontes_generata.sh, plan], portae=[('radix','figura')])`.

---

### Task 3: `delineare_mandata` — `Mandata` → pixels, and the first specimen

**Files:**
- Create: `include/delineare_mandata.h`, `lib/delineare_mandata.c`, `lib/delineare_mandata.worklog.md`, `probationes/pictor/specimina/` (dir; first golden `mandata_prima.png` born by the first run)
- Test: `probationes/probatio_delineare_mandata.c`

**Interfaces:**
- Consumes: `Mandata`/`Mandatum` (T2 of Plan 1; `s32` after T1), `delineare.h` (`delineare_creare_contextum`, `delineare_ponere_praecisionem`/`_tollere_praecisionem`, `delineare_lineam`, `delineare_rectangulum`, `delineare_rectangulum_plenum`, `delineare_polygonum`, `delineare_polygonum_plenum`), `fenestra.h` (`TabulaPixelorum`, `tabula_pixelorum_vacare`, `tabula_pixelorum_obtinere_pixelum`, `tabula_pixelorum_pingere_chordam`), `color.h` (`color_ad_pixelum`, `color_ex_pixelum`), `thema.h` (`thema_initiare`, `thema_color`, `thema_color_ex_indice_colorationis`), `imago_typus.h`, `specimen.h` (`specimen_iudicare`, `specimen_regula_solita`).
- Produces: `TabulaPixelorum* tabula_pixelorum_creare_nuda(Piscina*, i32 latitudo, i32 altitudo)` (no window; `scala` 1; `fenestra_*` = dims); `Imago imago_ex_tabula(constans TabulaPixelorum*)` (same memory, no copy — the packed `i32` is R,G,B,A in memory on little-endian, exactly `Imago`'s layout); `Color color_ex_mandato(ColorMandati)`; `ImagoFons` = `constans Imago* (*)(chorda provenientia, vacuum* ctx)`; `vacuum delineare_mandata(constans Mandata*, TabulaPixelorum*, ImagoFons fons, vacuum* ctx)`.
- Semantics: a stack of frames; a `coetus` pushes (origin += fines.xy + translatio, scala *= coetus.scala, clip ∩= fines if sectio) and pops after `magnitudo_arboris - 1` following elements; primitives are transformed `screen = origin + local * scala`; `RECTANGULUM` (outline or plenum), `LINEA` (crassitudo I in v1 — thicker lines are a P4 pull), `POLYGONUM`, `TEXTUS` (house 6×8 font; not clipped in v1), `IMAGO` (resolved by `fons` from the mandatum's `textus` field — `mandata_imago(m, fons_chorda, fines)` stores its source there; blitted with integer `scala` by nearest pixel into `fines`; alpha 0 skipped — that is the marquee/overlay convention; a NIHIL resolver or unknown provenientia paints nothing).

- [x] **Step 1: Write the failing test**

`mkdir -p probationes/pictor/specimina` first. `probationes/probatio_delineare_mandata.c`:
```c
/* probatio_delineare_mandata.c - mandata -> pixela (gradus VII):
 * asserta pixelorum sine exemplari, deinde specimen */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "fenestra.h"
#include "mandatum.h"
#include "delineare_mandata.h"
#include "imago_typus.h"
#include "specimen.h"
#include "credo.h"
#include <stdio.h>

/* Fons imaginum probationis: "quadrum" = 4x4, ruber cum angulo
 * transparente */
interior constans Imago*
fons_probationis (
    chorda provenientia,
    vacuum* ctx)
{
    si (chorda_aequalis_literis(provenientia, "quadrum"))
    {
        redde (constans Imago*)ctx;
    }
    redde NIHIL;
}

interior ColorMandati
rgba (
    i8 r,
    i8 g,
    i8 b)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_RGBA;
    c.valor = color_ad_pixelum(color_ex_rgba(r, g, b, (i8)CCLV));
    redde c;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
        TabulaPixelorum* t;
                Mandata* m;
                  Fines  f;
                Punctum  a;
                Punctum  b;
                  Imago  quadrum;
                  Imago  captura;
                    i32  coetus;
                    i32  i;
         SpecimenFructus sf;
                 chorda  provenientia;

    piscina = piscina_generare_dynamicum("probatio_delineare_mandata",
        XVI * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();

    imprimere("\n--- Tabula nuda ---\n");
    t = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    CREDO_NON_NIHIL(t);
    CREDO_AEQUALIS_I32(t->latitudo, LXIV);
    CREDO_AEQUALIS_I32(t->altitudo, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(color_ex_rgb(
        (i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));

    /* quadrum 4x4 ruber, pixelum (0,0) transparens */
    quadrum.latitudo = IV;
    quadrum.altitudo = IV;
    quadrum.pixela   = (i8*)piscina_allocare(piscina, LXIV);
    per (i = ZEPHYRUM; i < XVI; i++)
    {
        quadrum.pixela[i * IV]       = (i8)CCLV;
        quadrum.pixela[i * IV + I]   = ZEPHYRUM;
        quadrum.pixela[i * IV + II]  = ZEPHYRUM;
        quadrum.pixela[i * IV + III] = (i8)CCLV;
    }
    quadrum.pixela[III] = ZEPHYRUM;   /* alpha (0,0) = 0 */

    imprimere("\n--- Mandata: rectangulum, linea, imago scalata, textus"
              " in coetu translato ---\n");
    m = mandata_creare(piscina, intern);
    f.x = II; f.y = II; f.latitudo = X; f.altitudo = VI;
    mandata_rectangulum(m, f, rgba((i8)ZEPHYRUM, (i8)CCLV, (i8)ZEPHYRUM),
                        VERUM);
    a.x = ZEPHYRUM; a.y = XX; b.x = XX; b.y = XX;
    mandata_linea(m, a, b, I, rgba((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)CCLV));
    /* coetus: origo (30,10), scala II, sectio ad (30,10,20,20) */
    f.x = XXX; f.y = X; f.latitudo = XX; f.altitudo = XX;
    provenientia = chorda_ex_literis("coetus_a", piscina);
    coetus = mandata_coetus_incipere(m, f, VERUM, ZEPHYRUM, ZEPHYRUM, II,
                                     provenientia);
    f.x = ZEPHYRUM; f.y = ZEPHYRUM; f.latitudo = IV; f.altitudo = IV;
    mandata_imago(m, chorda_ex_literis("quadrum", piscina), f);
    /* rectangulum extra sectionem: (15,15,10,10) locale -> schirmo
     * (60,40)..(80,60), praecisum ad 50,30 */
    f.x = XV; f.y = XV; f.latitudo = X; f.altitudo = X;
    mandata_rectangulum(m, f, rgba((i8)CCLV, (i8)CCLV, (i8)ZEPHYRUM),
                        VERUM);
    mandata_coetus_finire(m, coetus);
    mandata_textus(m, II, XXX, chorda_ex_literis("Ok", piscina), ZEPHYRUM,
                   rgba((i8)CCLV, (i8)CCLV, (i8)CCLV));

    delineare_mandata(m, t, fons_probationis, &quadrum);

    imprimere("\n--- Asserta pixelorum (sine exemplari) ---\n");
    /* rectangulum plenum viride: intra (5,5), extra (13,5) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, V, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)CCLV, (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XIII, V),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
    /* linea caerulea per (10,20) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, X, XX),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)CCLV)));
    /* imago scalata II: pixelum (1,1) imaginis -> schirmo (32..33, 12..13)
     * ruber; pixelum (0,0) transparens -> schirmo (30,10) manet niger */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXXIII, XIII),
        color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XXX, X),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
    /* sectio: flavum intra (49,29)... schirmo (60..69) praecisum ad 50 */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XLV, XXV),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, LV, XXXV),
        color_ad_pixelum(color_ex_rgb((i8)ZEPHYRUM, (i8)ZEPHYRUM, (i8)ZEPHYRUM)));
    /* textus: aliquid album in linea 30..37 */
    {
        b32 album;
        i32 x;
        album = FALSUM;
        per (x = II; x < XIV; x++)
        {
            i32 y;
            per (y = XXX; y < XXXVIII; y++)
            {
                si (tabula_pixelorum_obtinere_pixelum(t, x, y)
                    == color_ad_pixelum(color_ex_rgb((i8)CCLV, (i8)CCLV,
                                                     (i8)CCLV)))
                {
                    album = VERUM;
                }
            }
        }
        CREDO_VERUM(album);
    }

    imprimere("\n--- Specimen (gradus VII) ---\n");
    captura = imago_ex_tabula(t);
    CREDO_AEQUALIS_I32(captura.latitudo, LXIV);
    sf = specimen_iudicare(&captura, "mandata_prima",
        specimen_regula_solita("probationes/pictor/specimina"), piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
The yellow-rectangle clip arithmetic: local (15,15,10,10) at origin (30,10) with scala II → screen (60,40)–(80,60); the clip is the coetus fines (30,10,20,20) → screen (30,10)–(50,30); the two do not intersect, so nothing yellow is painted anywhere: the two black assertions at (45,25) and (55,35) pin that. (Change the numbers if you want a partial clip; keep at least one pixel that would be yellow WITHOUT sectio.)

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh delineare_mandata` — Expected: FAIL, `'delineare_mandata.h' file not found`.

- [x] **Step 3: Write header and implementation**

`include/delineare_mandata.h`:
```c
/* delineare_mandata.h - Mandata -> TabulaPixelorum (rasterizator CPU)
 *
 * Nomen sigillatum: nullum novum substantivum - delineare.h extensum.
 * Cursus unus super elementa: coetus impellit scaenam (origo +=
 * fines + translatio; scala *= scala; sectio ∩= fines si sectio) et
 * post magnitudo_arboris - I elementa cadit. Primitiva: schirmum =
 * origo + locale * scala. Colores per thema.h (COLOR_MANDATI_THEMA
 * -> thema_color; INDEX -> palette; RGBA -> pixelum ipsum).
 *
 * IMAGO per fontem resolvitur (campus textus = fons imaginis ->
 * Imago*): mandata pixela numquam ferunt; identitas imaginis est
 * nomen (pictor: sigillum proiectionis). Alpha 0 = transparens (overlay marquee /
 * ictus pendentis).
 *
 * V1: linea crassitudine I; textus non praecisus; scala integra.
 */

#ifndef DELINEARE_MANDATA_H
#define DELINEARE_MANDATA_H

/* <aedilis corpus="lib/delineare_mandata.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "color.h"
#include "fenestra.h"
#include "imago_typus.h"
#include "mandatum.h"

/* ==================================================
 * Tabula nuda et imago
 * ================================================== */

/* Tabula pixelorum SINE fenestra (probationes, bake, specimina):
 * scala I, dimensiones fenestrae = dimensiones. */
TabulaPixelorum*
tabula_pixelorum_creare_nuda (
    Piscina* piscina,
         i32 latitudo,
         i32 altitudo);

/* Eadem memoria, nulla copia: pixelum i32 (ABGR in verbo) iacet
 * R,G,B,A in memoria - forma Imaginis. */
Imago
imago_ex_tabula (
    constans TabulaPixelorum* tabula);

Color
color_ex_mandato (
    ColorMandati color);

/* ==================================================
 * Rasterizatio
 * ================================================== */

nomen constans Imago* (*ImagoFons)(
    chorda provenientia,
    vacuum* ctx);

vacuum
delineare_mandata (
    constans Mandata* m,
     TabulaPixelorum* tabula,
            ImagoFons fons,
              vacuum* ctx);

#endif /* DELINEARE_MANDATA_H */
```
`lib/delineare_mandata.c`:
```c
/* delineare_mandata.c - rasterizator mandatorum */

#include "delineare_mandata.h"
#include "delineare.h"
#include "thema.h"
#include "xar.h"

#include <string.h>

#define SCAENAE_MAXIMAE LXIV

/* ==================================================
 * Tabula nuda et imago
 * ================================================== */

TabulaPixelorum*
tabula_pixelorum_creare_nuda (
    Piscina* piscina,
         i32 latitudo,
         i32 altitudo)
{
    TabulaPixelorum* t;

    si (!piscina || latitudo <= ZEPHYRUM || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    t = (TabulaPixelorum*)piscina_allocare(piscina,
                                           magnitudo(TabulaPixelorum));
    si (!t)
    {
        redde NIHIL;
    }
    t->latitudo          = latitudo;
    t->altitudo          = altitudo;
    t->fenestra_latitudo = latitudo;
    t->fenestra_altitudo = altitudo;
    t->scala             = 1.0f;
    t->pixela = (i32*)piscina_allocare(piscina,
        (memoriae_index)latitudo * (memoriae_index)altitudo
            * magnitudo(i32));
    si (!t->pixela)
    {
        redde NIHIL;
    }
    memset(t->pixela, ZEPHYRUM,
           (memoriae_index)latitudo * (memoriae_index)altitudo
               * magnitudo(i32));
    redde t;
}

Imago
imago_ex_tabula (
    constans TabulaPixelorum* tabula)
{
    Imago imago;

    imago.pixela   = (i8*)tabula->pixela;
    imago.latitudo = tabula->latitudo;
    imago.altitudo = tabula->altitudo;
    redde imago;
}

Color
color_ex_mandato (
    ColorMandati color)
{
    commutatio (color.genus)
    {
        casus COLOR_MANDATI_THEMA:
            redde thema_color((ColorThema)color.valor);
        casus COLOR_MANDATI_INDEX:
            redde thema_color_ex_indice_colorationis((i8)color.valor);
        ordinarius:
            redde color_ex_pixelum(color.valor);
    }
}

/* ==================================================
 * Scaenae
 * ================================================== */

nomen structura {
    s32 origo_x;
    s32 origo_y;
    s32 scala;
    b32 praecisa;
    s32 sectio_x;
    s32 sectio_y;
    s32 sectio_latitudo;
    s32 sectio_altitudo;
    i32 reliqua;          /* elementa reliqua sub hoc coetu */
} Scaena;

interior vacuum
sectionem_applicare (
    ContextusDelineandi* ctx,
      constans Scaena* s)
{
    si (s->praecisa)
    {
        delineare_ponere_praecisionem(ctx, s->sectio_x, s->sectio_y,
            (i32)(s->sectio_latitudo < ZEPHYRUM ? ZEPHYRUM
                                                 : s->sectio_latitudo),
            (i32)(s->sectio_altitudo < ZEPHYRUM ? ZEPHYRUM
                                                 : s->sectio_altitudo));
    }
    alioquin
    {
        delineare_tollere_praecisionem(ctx);
    }
}

/* scaena nova ex coetu: origo, scala, sectio ∩ */
interior Scaena
scaenam_impellere (
    constans Scaena* parens,
    constans Mandatum* coetus)
{
    Scaena s;
       s32 x0;
       s32 y0;
       s32 x1;
       s32 y1;

    s = *parens;
    s.origo_x = parens->origo_x + coetus->fines.x * parens->scala
              + coetus->translatio.x * parens->scala;
    s.origo_y = parens->origo_y + coetus->fines.y * parens->scala
              + coetus->translatio.y * parens->scala;
    s.scala   = parens->scala * (coetus->scala > ZEPHYRUM
                                 ? (s32)coetus->scala : I);
    s.reliqua = coetus->magnitudo_arboris - I;
    si (coetus->sectio)
    {
        x0 = parens->origo_x + coetus->fines.x * parens->scala;
        y0 = parens->origo_y + coetus->fines.y * parens->scala;
        x1 = x0 + coetus->fines.latitudo * parens->scala;
        y1 = y0 + coetus->fines.altitudo * parens->scala;
        si (s.praecisa)
        {
            si (x0 < s.sectio_x) { x0 = s.sectio_x; }
            si (y0 < s.sectio_y) { y0 = s.sectio_y; }
            si (x1 > s.sectio_x + s.sectio_latitudo)
            { x1 = s.sectio_x + s.sectio_latitudo;
            }
            si (y1 > s.sectio_y + s.sectio_altitudo)
            { y1 = s.sectio_y + s.sectio_altitudo;
            }
        }
        s.praecisa        = VERUM;
        s.sectio_x        = x0;
        s.sectio_y        = y0;
        s.sectio_latitudo = x1 - x0;
        s.sectio_altitudo = y1 - y0;
    }
    redde s;
}

/* ==================================================
 * Primitiva
 * ================================================== */

interior vacuum
imaginem_pingere (
    TabulaPixelorum* t,
     constans Scaena* s,
   constans Mandatum* x,
     constans Imago* imago)
{
    s32 sx;
    s32 sy;
    s32 dx;
    s32 dy;
    s32 px;
    s32 py;
    s32 lat;
    s32 alt;
    s32 k;
    constans i8* p;

    lat = imago->latitudo < x->fines.latitudo ? imago->latitudo
                                              : x->fines.latitudo;
    alt = imago->altitudo < x->fines.altitudo ? imago->altitudo
                                              : x->fines.altitudo;
    per (sy = ZEPHYRUM; sy < alt; sy++)
    {
        per (sx = ZEPHYRUM; sx < lat; sx++)
        {
            p = imago->pixela + (sy * imago->latitudo + sx) * IV;
            si (p[III] == ZEPHYRUM)
            {
                perge;   /* transparens */
            }
            per (dy = ZEPHYRUM; dy < s->scala; dy++)
            {
                per (dx = ZEPHYRUM; dx < s->scala; dx++)
                {
                    px = s->origo_x + (x->fines.x + sx) * s->scala + dx;
                    py = s->origo_y + (x->fines.y + sy) * s->scala + dy;
                    si (px < ZEPHYRUM || py < ZEPHYRUM
                        || px >= (s32)t->latitudo || py >= (s32)t->altitudo)
                    {
                        perge;
                    }
                    si (s->praecisa
                        && (px < s->sectio_x || py < s->sectio_y
                            || px >= s->sectio_x + s->sectio_latitudo
                            || py >= s->sectio_y + s->sectio_altitudo))
                    {
                        perge;
                    }
                    k = py * (s32)t->latitudo + px;
                    t->pixela[k] = color_ad_pixelum(
                        color_ex_rgba(p[ZEPHYRUM], p[I], p[II], p[III]));
                }
            }
        }
    }
}

interior vacuum
primitivum_pingere (
    ContextusDelineandi* ctx,
        TabulaPixelorum* t,
        constans Scaena* s,
      constans Mandatum* x,
              ImagoFons fons,
                vacuum* fons_ctx)
{
    Color color;
      s32 sx;
      s32 sy;
      s32 lat;
      s32 alt;
      i32 i;
      i32* puncta;
    constans Imago* imago;

    color = color_ex_mandato(x->color);
    sx  = s->origo_x + x->fines.x * s->scala;
    sy  = s->origo_y + x->fines.y * s->scala;
    lat = x->fines.latitudo * s->scala;
    alt = x->fines.altitudo * s->scala;
    commutatio (x->genus)
    {
        casus MANDATUM_RECTANGULUM:
            si (x->impletum)
            {
                delineare_rectangulum_plenum(ctx, (i32)sx, (i32)sy,
                                             (i32)lat, (i32)alt, color);
            }
            alioquin
            {
                delineare_rectangulum(ctx, (i32)sx, (i32)sy,
                                      (i32)lat, (i32)alt, color);
            }
            frange;
        casus MANDATUM_LINEA:
            si (x->numerus_punctorum >= II)
            {
                delineare_lineam(ctx,
                    (i32)(s->origo_x + x->puncta[ZEPHYRUM].x * s->scala),
                    (i32)(s->origo_y + x->puncta[ZEPHYRUM].y * s->scala),
                    (i32)(s->origo_x + x->puncta[I].x * s->scala),
                    (i32)(s->origo_y + x->puncta[I].y * s->scala),
                    color);
            }
            frange;
        casus MANDATUM_POLYGONUM:
            si (x->numerus_punctorum >= III)
            {
                puncta = (i32*)piscina_allocare(ctx->piscina,
                    (memoriae_index)x->numerus_punctorum * II
                        * magnitudo(i32));
                per (i = ZEPHYRUM; i < x->numerus_punctorum; i++)
                {
                    puncta[i * II] = (i32)(s->origo_x
                                           + x->puncta[i].x * s->scala);
                    puncta[i * II + I] = (i32)(s->origo_y
                                               + x->puncta[i].y * s->scala);
                }
                si (x->impletum)
                {
                    delineare_polygonum_plenum(ctx, puncta,
                                               x->numerus_punctorum, color);
                }
                alioquin
                {
                    delineare_polygonum(ctx, puncta, x->numerus_punctorum,
                                        color);
                }
            }
            frange;
        casus MANDATUM_TEXTUS:
            tabula_pixelorum_pingere_chordam(t, (i32)sx, (i32)sy,
                x->textus, color_ad_pixelum(color));
            frange;
        casus MANDATUM_IMAGO:
            imago = fons ? fons(x->textus, fons_ctx) : NIHIL;
            si (imago)
            {
                imaginem_pingere(t, s, x, imago);
            }
            frange;
        ordinarius:
            frange;
    }
}

/* ==================================================
 * Cursus
 * ================================================== */

vacuum
delineare_mandata (
    constans Mandata* m,
     TabulaPixelorum* tabula,
            ImagoFons fons,
              vacuum* ctx)
{
    ContextusDelineandi* dctx;
              Scaena     scaenae[SCAENAE_MAXIMAE];
                 i32     altitudo;
                 i32     i;
                 i32     n;
    constans Mandatum*   x;

    si (!m || !tabula)
    {
        redde;
    }
    dctx = delineare_creare_contextum(m->piscina, tabula);
    si (!dctx)
    {
        redde;
    }
    memset(&scaenae[ZEPHYRUM], ZEPHYRUM, magnitudo(Scaena));
    scaenae[ZEPHYRUM].scala   = I;
    scaenae[ZEPHYRUM].reliqua = -I;   /* radix numquam cadit */
    altitudo = ZEPHYRUM;
    n = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(m, i);
        si (x->genus == MANDATUM_COETUS)
        {
            si (altitudo + I < SCAENAE_MAXIMAE)
            {
                scaenae[altitudo + I] = scaenam_impellere(&scaenae[altitudo],
                                                          x);
                altitudo++;
                sectionem_applicare(dctx, &scaenae[altitudo]);
            }
        }
        alioquin
        {
            primitivum_pingere(dctx, tabula, &scaenae[altitudo], x,
                               fons, ctx);
        }
        /* elementa consumpta: scaenae exhaustae cadunt */
        dum (altitudo > ZEPHYRUM)
        {
            scaenae[altitudo].reliqua--;
            si (scaenae[altitudo].reliqua > ZEPHYRUM)
            {
                frange;
            }
            altitudo--;
            sectionem_applicare(dctx, &scaenae[altitudo]);
        }
    }
    delineare_restituere_contextum(dctx);
}
```
Read this against `include/mandatum.h` once more before typing it: `magnitudo_arboris` counts the coetus INCLUDING itself (Plan 1 T2 — "coetus: mandata in subarbore se incluso"), so a coetus with N descendants has `magnitudo_arboris = N + 1` and its scaena must survive exactly N following elements: `reliqua = magnitudo_arboris - 1`, decremented once per element consumed at that depth, popped when it reaches zero; a coetus that is itself an element consumed under its parent decrements the parent's count when IT is pushed — the `dum` loop after the push handles that because the freshly pushed scaena has `reliqua > 0` (or equals 0 for an empty coetus, which pops immediately). Nested empties chain-pop. If `magnitudo_arboris` turns out to EXCLUDE self, use `reliqua = magnitudo_arboris` — the probatio's index assertions decide. `ContextusDelineandi` exposes `piscina` (delineare.h:61) — used for the polygon scratch.

- [x] **Step 4: First run — the specimen is born, then promote**

Run: `./compile_tests.sh delineare_mandata` — Expected: the pixel assertions PASS; the specimen FAILS with `SPECIMEN_EXEMPLAR_ABEST` and `sf.causa` names the candidate it wrote under `probationes/pictor/specimina/`. Open the PNG (or `python3 -c` a dump of a few pixels). Confirm: green box, blue line, a red 8×8 block with a black 2×2 hole at its top-left, "Ok" in white at the bottom, NO yellow. Promote by `mv` to `probationes/pictor/specimina/mandata_prima.png` (the exact name `specimen_iudicare` looks for — read `lib/specimen.c`'s naming if the candidate name differs from `<titulus>.png`). Second run: PASS.

- [x] **Step 5: Plant, worklog, commit**

Plant (RED): in `scaenam_impellere`, drop the translation term from `s.origo_x` — the red image shifts and both the pixel assertion at (33,13) and the specimen go red. Second plant: `s.praecisa = VERUM;` → `FALSUM` — the yellow appears at (55,35): red. Both green on revert.

Worklog `lib/delineare_mandata.worklog.md`: the pixel formats (the packed i32 is R,G,B,A in memory — `imago_ex_tabula` is free); the frame stack and the `magnitudo_arboris` contract; v1 limits (line thickness I, text unclipped, integer scale); the provenientia resolver as the only path to pixels; specimen candidate naming as observed.

Commit: `silva.commissio(msg, [include/delineare_mandata.h, lib/delineare_mandata.c, lib/delineare_mandata.worklog.md, probationes/probatio_delineare_mandata.c, probationes/pictor/specimina/mandata_prima.png, compile_tests_fontes_generata.sh, plan], portae=[('radix','delineare_mandata')])`.

---

### Task 4: `pictor_documentum` — acta on `volumen`, projection, checkpoints, undo/redo

**Files:**
- Create: `include/pictor_documentum.h`, `lib/pictor_documentum.c`, `lib/pictor_documentum.worklog.md`
- Test: `probationes/probatio_pictor_documentum.c`

**Interfaces:**
- Consumes: `volumen.h` (`volumen_temporarium`, `volumen_actum_appendere(vol, genus_cstr, chorda datum) → s64 seq`, `volumen_acta_legere(vol, post_seq, piscina) → Xar* of VolumenActum {seq, momentum, genus, datum}`, `volumen_massam_condere(vol, chorda, character hex[65]) → b32`, `volumen_massam_promere(vol, chorda hex, piscina, b32* inventum) → chorda`, `volumen_plagulam_condere/promere`, `volumen_summa_actorum`), `sigillum.h`, `stml.h`, `delineare.h`, `thema.h`, `imago_typus.h`, `delineare_mandata.h` (`tabula_pixelorum_creare_nuda`, `imago_ex_tabula`).
- Produces: `PictorDocumentum {volumen, piscina, intern, latitudo, altitudo, tabula (TabulaPixelorum*), proiectio (Imago, same memory), cursor (s64), finis (s64), intervallum (i32), sigillum (Sigillum)}`; `pictor_documentum_creare(piscina, intern, Volumen*, i32 lat, i32 alt, i32 intervallum)`; `pictor_documentum_aperire(piscina, intern, Volumen*)` (dimensions from the `documentum` plagula; projection rebuilt); `pictor_documentum_actum(doc, chorda actum_stml) → s64` (appends — after a `<ramus>` if the cursor is behind the end — applies, checkpoints every `intervallum`); `pictor_documentum_revocare(doc) → b32` / `_reficere(doc) → b32` (cursor moves; projection rebuilt from the nearest checkpoint at or below the target); `pictor_documentum_proiectio(doc) → constans Imago*`; `pictor_documentum_sigillum_hex(doc, piscina) → chorda`; `pictor_documentum_verificare(doc) → b32` (tier 8: reproject from zero, compare sigilla); `pictor_documentum_cursor/finis(doc) → s64`.
- **The acta vocabulary v1** (spec §4): `<ictus instrumentum="penicillus" color="N" magnitudo="M"><punctum x="" y=""/>…</ictus>` — the brush paints a filled square of side M centred on every point and a line of thickness I between consecutive points; `color` is a palette index (0–15, `thema_color_ex_indice_colorationis`); `<ramus ab="SEQ"/>` — the acta after SEQ up to this ramus are dead (undo followed by a new stroke); `<impletio>`, `<figura>`, `<insertio>`, `<stratum>`, `<paletta>`, `<selectio_commissa>` are P5 pulls and are IGNORED by v1 with a note in the worklog when first seen. Checkpoints: massa = the raw RGBA bytes of the projection (binary-safe: the probatio pins a zero byte inside), plagula `checkpoint/<seq>` = the massa's sigillum hex; plagula `documentum` = `<documentum latitudo="" altitudo="" intervallum=""/>`.

- [x] **Step 1: Write the failing test**

`probationes/probatio_pictor_documentum.c`:
```c
/* probatio_pictor_documentum.c - acta -> proiectio -> sigillum
 * (gradus VIII); checkpoints; revocare/reficere; ramus */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "sigillum.h"
#include "fenestra.h"
#include "pictor_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ictus unus: linea horizontalis (x0..x1, y) magnitudine I */
interior chorda
ictus (
    Piscina* p,
        s32  x0,
        s32  x1,
        s32  y,
        i32  color)
{
    chorda s;

    s = chorda_ex_literis("<ictus instrumentum=\"penicillus\" color=\"",
                          p);
    s = chorda_concatenare(s, chorda_ex_s32((s32)color, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"1\">"
                                                "<punctum x=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(x0, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\"/><punctum x=\"", p),
                           p);
    s = chorda_concatenare(s, chorda_ex_s32(x1, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
    s = chorda_concatenare(s, chorda_ex_s32(y, p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\"/></ictus>", p), p);
    redde s;
}

interior i32
pixelum (
    constans Imago* im,
               s32  x,
               s32  y)
{
    constans i8* p;

    p = im->pixela + (y * im->latitudo + x) * IV;
    redde color_ad_pixelum(color_ex_rgba(p[ZEPHYRUM], p[I], p[II], p[III]));
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
       PictorDocumentum* doc2;
                 chorda  s1;
                 chorda  s2;
                 chorda  s3;
                    i32  albus;
                    i32  niger;
                    i32  i;

    piscina = piscina_generare_dynamicum("probatio_pictor_documentum",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    albus = color_ad_pixelum(thema_color_ex_indice_colorationis(
        (i8)PALETTE_WHITE));
    niger = color_ad_pixelum(thema_color_ex_indice_colorationis(
        (i8)PALETTE_BLACK));

    imprimere("\n--- Creatio: proiectio alba, cursor 0 ---\n");
    vol = volumen_temporarium(piscina, "probatio_pictor_documentum");
    CREDO_NON_NIHIL(vol);
    doc = pictor_documentum_creare(piscina, intern, vol, XXXII, XVI, II);
    CREDO_NON_NIHIL(doc);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), ZEPHYRUM);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), V, V),
                       albus);
    s1 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_AEQUALIS_I32(s1.mensura, LXIV);

    imprimere("\n--- Tres ictus: proiectio, sigillum mutatur, checkpoint"
              " ad II ---\n");
    CREDO_VERUM(pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, II, PALETTE_BLACK)) > ZEPHYRUM);
    CREDO_VERUM(pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, V, PALETTE_BLACK)) > ZEPHYRUM);
    CREDO_VERUM(pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, VIII, PALETTE_BLACK)) > ZEPHYRUM);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), III);
    CREDO_AEQUALIS_S64(pictor_documentum_finis(doc), III);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, II),
                       niger);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, VIII),
                       niger);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, III),
                       albus);
    s2 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_FALSUM(chorda_aequalis(s1, s2));
    /* checkpoint post II acta (intervallum II): plagula + massa */
    {
        b32    inventum;
        chorda hex;
        hex = volumen_plagulam_promere(vol,
            chorda_ex_literis("checkpoint/2", piscina), piscina, &inventum);
        CREDO_VERUM(inventum);
        CREDO_AEQUALIS_I32(hex.mensura, LXIV);
        CREDO_AEQUALIS_S64(volumen_summa_massarum(vol), I);
    }

    imprimere("\n--- Gradus VIII: documentum se ipsum verificat ---\n");
    CREDO_VERUM(pictor_documentum_verificare(doc));

    imprimere("\n--- Revocare: cursor II, proiectio ex checkpoint ---\n");
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), II);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, VIII),
                       albus);
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, V),
                       niger);
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), ZEPHYRUM);
    CREDO_FALSUM(pictor_documentum_revocare(doc));       /* nihil ultra */
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, II),
                       albus);

    imprimere("\n--- Reficere: cursor III, sigillum idem ac ante ---\n");
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_VERUM(pictor_documentum_reficere(doc));
    CREDO_FALSUM(pictor_documentum_reficere(doc));
    s3 = pictor_documentum_sigillum_hex(doc, piscina);
    CREDO_VERUM(chorda_aequalis(s2, s3));

    imprimere("\n--- Ramus: revocare I, ictus novus -> reficere nihil ---\n");
    CREDO_VERUM(pictor_documentum_revocare(doc));
    CREDO_VERUM(pictor_documentum_actum(doc,
        ictus(piscina, ZEPHYRUM, XXXI, XII, PALETTE_BLACK)) > ZEPHYRUM);
    CREDO_FALSUM(pictor_documentum_reficere(doc));
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, VIII),
                       albus);                               /* mortuus */
    CREDO_AEQUALIS_I32(pixelum(pictor_documentum_proiectio(doc), X, XII),
                       niger);
    /* acta in volumine: III ictus + ramus + ictus = V */
    CREDO_AEQUALIS_S64(volumen_summa_actorum(vol) >= V ? I : ZEPHYRUM, I);
    CREDO_VERUM(pictor_documentum_verificare(doc));

    imprimere("\n--- Aperire idem volumen: proiectio eadem ---\n");
    doc2 = pictor_documentum_aperire(piscina, intern, vol);
    CREDO_NON_NIHIL(doc2);
    CREDO_AEQUALIS_I32(doc2->latitudo, XXXII);
    CREDO_VERUM(chorda_aequalis(pictor_documentum_sigillum_hex(doc, piscina),
                                pictor_documentum_sigillum_hex(doc2,
                                                               piscina)));

    imprimere("\n--- Mensura: CC ictus, tempus per actum (worklog) ---\n");
    {
        s64 t0;
        s64 t1;
        t0 = fenestra_tempus_ms();
        per (i = ZEPHYRUM; i < CC; i++)
        {
            pictor_documentum_actum(doc,
                ictus(piscina, ZEPHYRUM, XXXI, (s32)(i % XVI), PALETTE_BLACK));
        }
        t1 = fenestra_tempus_ms();
        imprimere("  CC acta: %ld ms totales\n", (long)(t1 - t0));
        t0 = fenestra_tempus_ms();
        pictor_documentum_revocare(doc);
        t1 = fenestra_tempus_ms();
        imprimere("  revocare unum: %ld ms\n", (long)(t1 - t0));
        CREDO_VERUM(t1 >= t0);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
`fenestra_tempus_ms` is Plan 1 T1's; it links without a window (it is in `fenestra_macos.m`, which the root suite already links with its three frameworks — the same as `probatio_fenestra_tempus`). `CREDO_AEQUALIS_S64` exists (credo.h). If `volumen_temporarium` needs a suffix convention, follow `lib/volumen.c`'s comment.

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh pictor_documentum` — Expected: FAIL, header not found.

- [x] **Step 3: Write header and implementation**

`include/pictor_documentum.h`:
```c
/* pictor_documentum.h - Documentum pictoris = cauda ictuum
 *
 * Veritas est ACTA (volumen: solum-appende); proiectio (bitmap) est
 * derivata: checkpoint proximus + acta post eum. Undo/redo = cursor
 * in memoria; ictus novus post revocationem RAMUM appendit
 * (<ramus ab="seq"/>): acta inter ab et ramum mortua sunt - cauda
 * numquam truncatur, historia numquam mentitur. Checkpoint omni
 * intervallo: massa (pixela RGBA) sigillo addressata + plagula
 * 'checkpoint/<seq>' -> sigillum hex. Gradus VIII: verificare =
 * reproicere ex nihilo et sigilla conferre.
 *
 * Acta v1: <ictus instrumentum color magnitudo><punctum x y/>...
 * </ictus>, <ramus ab/>. Cetera (§4) ignorantur cum nota.
 */

#ifndef PICTOR_DOCUMENTUM_H
#define PICTOR_DOCUMENTUM_H

/* <aedilis corpus="lib/pictor_documentum.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "sigillum.h"
#include "fenestra.h"
#include "imago_typus.h"

nomen structura {
                Volumen* volumen;
                Piscina* piscina;
    InternamentumChorda* intern;
                    i32  latitudo;
                    i32  altitudo;
                    i32  intervallum;    /* acta per checkpoint */
        TabulaPixelorum* tabula;         /* proiectio (memoria) */
                  Imago  proiectio;      /* eadem memoria */
                    s64  cursor;         /* acta applicata (seq) */
                    s64  finis;          /* seq ultimum vivum */
               Sigillum  sigillum;       /* proiectionis currentis */
} PictorDocumentum;

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum);

/* ex volumine exsistente: dimensiones ex plagula 'documentum',
 * proiectio ex checkpoint proximo + actis */
PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen);

/* appendit (ramus prius si cursor < finis), applicat, checkpoint si
 * debetur. Redde seq (> 0) aut 0 si recusatum. */
s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml);

b32
pictor_documentum_revocare (
    PictorDocumentum* doc);

b32
pictor_documentum_reficere (
    PictorDocumentum* doc);

constans Imago*
pictor_documentum_proiectio (
    constans PictorDocumentum* doc);

chorda
pictor_documentum_sigillum_hex (
    constans PictorDocumentum* doc,
                     Piscina* piscina);

/* reproicere ex nihilo (nullo checkpoint) et sigilla conferre */
b32
pictor_documentum_verificare (
    PictorDocumentum* doc);

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc);

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc);

#endif /* PICTOR_DOCUMENTUM_H */
```
`lib/pictor_documentum.c`:
```c
/* pictor_documentum.c - acta -> proiectio */

#include "pictor_documentum.h"
#include "delineare_mandata.h"
#include "delineare.h"
#include "thema.h"
#include "color.h"
#include "stml.h"
#include "xar.h"

#include <string.h>

/* ==================================================
 * Auxilia
 * ================================================== */

interior memoriae_index
mensura_pixelorum (
    constans PictorDocumentum* doc)
{
    redde (memoriae_index)doc->latitudo * (memoriae_index)doc->altitudo
         * magnitudo(i32);
}

/* nullum chorda_ex_s64 in domo: per f64 sine decimalibus (exactum
 * infra 2^53), ut eventus_stml tempus scribit */
interior chorda
seq_chorda (
        s64  seq,
    Piscina* piscina)
{
    redde chorda_ex_f64((f64)seq, ZEPHYRUM, piscina);
}

interior vacuum
sigillum_renovare (
    PictorDocumentum* doc)
{
    doc->sigillum = sigillum_computare(doc->tabula->pixela,
                                       mensura_pixelorum(doc));
}

interior s32
attributum_s32 (
    StmlNodus* n,
    constans character* titulus,
    s32 praestitutum)
{
    chorda* a;
       s32  v;

    a = stml_attributum_capere(n, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior vacuum
vacare_albam (
    PictorDocumentum* doc)
{
    tabula_pixelorum_vacare(doc->tabula, color_ad_pixelum(
        thema_color_ex_indice_colorationis((i8)PALETTE_WHITE)));
}

/* ==================================================
 * Applicatio actorum
 * ================================================== */

/* <ictus instrumentum color magnitudo><punctum x y/>...</ictus> */
interior vacuum
ictum_applicare (
    PictorDocumentum* doc,
           StmlNodus* ictus)
{
    ContextusDelineandi* ctx;
                  Color  color;
                    s32  magnitudo_penicilli;
                    s32  x;
                    s32  y;
                    s32  x_ante;
                    s32  y_ante;
                    i32  i;
                    i32  n;
              StmlNodus* punctum;

    ctx = delineare_creare_contextum(doc->piscina, doc->tabula);
    si (!ctx)
    {
        redde;
    }
    color = thema_color_ex_indice_colorationis(
        (i8)attributum_s32(ictus, "color", (s32)PALETTE_BLACK));
    magnitudo_penicilli = attributum_s32(ictus, "magnitudo", I);
    si (magnitudo_penicilli < I)
    {
        magnitudo_penicilli = I;
    }
    n      = stml_numerus_liberorum(ictus);
    x_ante = ZEPHYRUM;
    y_ante = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        punctum = stml_liberum_ad_indicem(ictus, i);
        si (punctum->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        x = attributum_s32(punctum, "x", ZEPHYRUM);
        y = attributum_s32(punctum, "y", ZEPHYRUM);
        si (i > ZEPHYRUM)
        {
            delineare_lineam(ctx, (i32)x_ante, (i32)y_ante, (i32)x, (i32)y,
                             color);
        }
        delineare_rectangulum_plenum(ctx,
            (i32)(x - magnitudo_penicilli / II),
            (i32)(y - magnitudo_penicilli / II),
            (i32)magnitudo_penicilli, (i32)magnitudo_penicilli, color);
        x_ante = x;
        y_ante = y;
    }
    delineare_restituere_contextum(ctx);
}

interior vacuum
actum_applicare (
    PictorDocumentum* doc,
              chorda  datum)
{
    StmlResultus res;

    res = stml_legere_ex_literis(chorda_ut_cstr(datum, doc->piscina),
                                 doc->piscina, doc->intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde;
    }
    si (chorda_aequalis_literis(*res.elementum_radix->titulus, "ictus"))
    {
        ictum_applicare(doc, res.elementum_radix);
    }
    /* ramus: nihil pingit; cetera v1 ignorata (worklog) */
}

/* acta viva usque ad 'ad' (inclusive), rami honorati: reddit Xar de
 * VolumenActum ordine seq, mortuis exclusis */
interior Xar*
acta_viva (
    PictorDocumentum* doc,
                 s64  post,
                 s64  ad)
{
    Xar*          omnia;
    Xar*          viva;
    VolumenActum* a;
    VolumenActum* sedes;
    i32           i;
    i32           n;
    i32           k;
    s64           ab;
    StmlResultus  res;

    omnia = volumen_acta_legere(doc->volumen, post, doc->piscina);
    viva  = xar_creare(doc->piscina, (i32)magnitudo(VolumenActum));
    n = xar_numerus(omnia);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(omnia, i);
        si (a->seq > ad)
        {
            frange;
        }
        si (chorda_aequalis_literis(a->genus, "ramus"))
        {
            res = stml_legere_ex_literis(chorda_ut_cstr(a->datum,
                doc->piscina), doc->piscina, doc->intern);
            ab = res.successus
               ? (s64)attributum_s32(res.elementum_radix, "ab", ZEPHYRUM)
               : ZEPHYRUM;
            /* tollere viva cum seq > ab */
            k = xar_numerus(viva);
            dum (k > ZEPHYRUM
                  && ((VolumenActum*)xar_obtinere(viva, k - I))->seq > ab)
            {
                k--;
            }
            xar_truncare(viva, k);
            perge;
        }
        si (chorda_aequalis_literis(a->genus, "volumen-creatum"))
        {
            perge;
        }
        sedes  = (VolumenActum*)xar_addere(viva);
        *sedes = *a;
    }
    redde viva;
}

/* proiectio ad seq 'ad': checkpoint proximus <= ad, deinde acta viva
 * post eum; sine checkpoint ex alba */
interior vacuum
proicere_ad (
    PictorDocumentum* doc,
                 s64  ad,
                 b32  sine_checkpoint)
{
    Xar*          viva;
    VolumenActum* a;
    i32           i;
    i32           n;
    s64           basis;
    s64           s;
    chorda        hex;
    chorda        massa;
    b32           inventum;
    chorda        clavis;

    basis = ZEPHYRUM;
    si (!sine_checkpoint)
    {
        per (s = ad - (ad % doc->intervallum); s > ZEPHYRUM;
             s -= doc->intervallum)
        {
            clavis = chorda_concatenare(
                chorda_ex_literis("checkpoint/", doc->piscina),
                seq_chorda(s, doc->piscina), doc->piscina);
            hex = volumen_plagulam_promere(doc->volumen, clavis,
                                           doc->piscina, &inventum);
            si (!inventum)
            {
                perge;
            }
            massa = volumen_massam_promere(doc->volumen, hex, doc->piscina,
                                           &inventum);
            si (inventum && massa.mensura == (i32)mensura_pixelorum(doc))
            {
                memcpy(doc->tabula->pixela, massa.datum,
                       mensura_pixelorum(doc));
                basis = s;
                frange;
            }
        }
    }
    si (basis == ZEPHYRUM)
    {
        vacare_albam(doc);
    }
    viva = acta_viva(doc, basis, ad);
    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        actum_applicare(doc, a->datum);
    }
    sigillum_renovare(doc);
}

interior vacuum
checkpoint_condere (
    PictorDocumentum* doc,
                 s64  seq)
{
    character hex[SIGILLUM_HEX_MENSURA];
       chorda contentum;
       chorda clavis;

    contentum.datum   = (i8*)doc->tabula->pixela;
    contentum.mensura = (i32)mensura_pixelorum(doc);
    si (!volumen_massam_condere(doc->volumen, contentum, hex))
    {
        redde;
    }
    clavis = chorda_concatenare(chorda_ex_literis("checkpoint/", doc->piscina),
                                seq_chorda(seq, doc->piscina),
                                doc->piscina);
    volumen_plagulam_condere(doc->volumen, clavis,
                             chorda_ex_literis(hex, doc->piscina),
                             "pictor:checkpoint");
}

/* ==================================================
 * Vita
 * ================================================== */

interior PictorDocumentum*
documentum_struere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;

    doc = (PictorDocumentum*)piscina_allocare(piscina,
                                              magnitudo(PictorDocumentum));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PictorDocumentum));
    doc->volumen     = volumen;
    doc->piscina     = piscina;
    doc->intern      = intern;
    doc->latitudo    = latitudo;
    doc->altitudo    = altitudo;
    doc->intervallum = intervallum > ZEPHYRUM ? intervallum : LXIV;
    doc->tabula = tabula_pixelorum_creare_nuda(piscina, latitudo, altitudo);
    si (!doc->tabula)
    {
        redde NIHIL;
    }
    doc->proiectio = imago_ex_tabula(doc->tabula);
    redde doc;
}

PictorDocumentum*
pictor_documentum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
                    i32  intervallum)
{
    PictorDocumentum* doc;
              chorda  manifestum;

    si (!piscina || !intern || !volumen || latitudo <= ZEPHYRUM
        || altitudo <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    doc = documentum_struere(piscina, intern, volumen, latitudo, altitudo,
                             intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    manifestum = chorda_ex_literis("<documentum latitudo=\"", piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)latitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)altitudo, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\" intervallum=\"", piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_s32((s32)doc->intervallum, piscina), piscina);
    manifestum = chorda_concatenare(manifestum,
        chorda_ex_literis("\"/>", piscina), piscina);
    volumen_plagulam_condere(volumen, chorda_ex_literis("documentum", piscina),
                             manifestum, "pictor:documentum");
    vacare_albam(doc);
    sigillum_renovare(doc);
    redde doc;
}

PictorDocumentum*
pictor_documentum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    PictorDocumentum* doc;
              chorda  manifestum;
                 b32  inventum;
        StmlResultus  res;
                 s32  latitudo;
                 s32  altitudo;
                 s32  intervallum;
                 Xar* viva;
                 i32  n;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    manifestum = volumen_plagulam_promere(volumen,
        chorda_ex_literis("documentum", piscina), piscina, &inventum);
    si (!inventum)
    {
        redde NIHIL;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(manifestum, piscina),
                                 piscina, intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }
    latitudo    = attributum_s32(res.elementum_radix, "latitudo", ZEPHYRUM);
    altitudo    = attributum_s32(res.elementum_radix, "altitudo", ZEPHYRUM);
    intervallum = attributum_s32(res.elementum_radix, "intervallum", LXIV);
    doc = documentum_struere(piscina, intern, volumen, (i32)latitudo,
                             (i32)altitudo, (i32)intervallum);
    si (!doc)
    {
        redde NIHIL;
    }
    /* finis = seq ultimum vivum; cursor = finis */
    viva = acta_viva(doc, ZEPHYRUM, volumen_summa_actorum(volumen));
    n = xar_numerus(viva);
    doc->finis  = n > ZEPHYRUM
                ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
                : ZEPHYRUM;
    doc->cursor = doc->finis;
    proicere_ad(doc, doc->cursor, FALSUM);
    redde doc;
}

/* ==================================================
 * Acta, revocare, reficere
 * ================================================== */

s64
pictor_documentum_actum (
    PictorDocumentum* doc,
              chorda  actum_stml)
{
    s64    seq;
    chorda ramus;

    si (!doc || chorda_vacua(actum_stml))
    {
        redde ZEPHYRUM;
    }
    si (doc->cursor < doc->finis)
    {
        ramus = chorda_concatenare(chorda_ex_literis("<ramus ab=\"",
                                                     doc->piscina),
                                   seq_chorda(doc->cursor, doc->piscina),
                                   doc->piscina);
        ramus = chorda_concatenare(ramus, chorda_ex_literis("\"/>",
                                                            doc->piscina),
                                   doc->piscina);
        volumen_actum_appendere(doc->volumen, "ramus", ramus);
    }
    seq = volumen_actum_appendere(doc->volumen, "ictus", actum_stml);
    si (seq <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    actum_applicare(doc, actum_stml);
    doc->cursor = seq;
    doc->finis  = seq;
    sigillum_renovare(doc);
    si (seq % doc->intervallum == ZEPHYRUM)
    {
        checkpoint_condere(doc, seq);
    }
    redde seq;
}

b32
pictor_documentum_revocare (
    PictorDocumentum* doc)
{
    Xar* viva;
    i32  n;
    s64  ad;

    si (!doc || doc->cursor <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* actum vivum proximum infra cursor */
    viva = acta_viva(doc, ZEPHYRUM, doc->cursor - I);
    n  = xar_numerus(viva);
    ad = n > ZEPHYRUM ? ((VolumenActum*)xar_obtinere(viva, n - I))->seq
                      : ZEPHYRUM;
    doc->cursor = ad;
    proicere_ad(doc, ad, FALSUM);
    redde VERUM;
}

b32
pictor_documentum_reficere (
    PictorDocumentum* doc)
{
    Xar*          viva;
    VolumenActum* a;
    i32           i;
    i32           n;

    si (!doc || doc->cursor >= doc->finis)
    {
        redde FALSUM;
    }
    viva = acta_viva(doc, doc->cursor, doc->finis);
    n = xar_numerus(viva);
    per (i = ZEPHYRUM; i < n; i++)
    {
        a = (VolumenActum*)xar_obtinere(viva, i);
        si (a->seq > doc->cursor)
        {
            actum_applicare(doc, a->datum);
            doc->cursor = a->seq;
            sigillum_renovare(doc);
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Lectio et verificatio
 * ================================================== */

constans Imago*
pictor_documentum_proiectio (
    constans PictorDocumentum* doc)
{
    redde doc ? &doc->proiectio : NIHIL;
}

chorda
pictor_documentum_sigillum_hex (
    constans PictorDocumentum* doc,
                     Piscina* piscina)
{
    character hex[SIGILLUM_HEX_MENSURA];
       chorda vacua;

    si (!doc)
    {
        vacua.mensura = ZEPHYRUM;
        vacua.datum   = NIHIL;
        redde vacua;
    }
    sigillum_hex(&doc->sigillum, hex);
    redde chorda_ex_literis(hex, piscina);
}

b32
pictor_documentum_verificare (
    PictorDocumentum* doc)
{
    Sigillum ante;

    si (!doc)
    {
        redde FALSUM;
    }
    ante = doc->sigillum;
    proicere_ad(doc, doc->cursor, VERUM);
    redde sigillum_aequale(&ante, &doc->sigillum);
}

s64
pictor_documentum_cursor (
    constans PictorDocumentum* doc)
{
    redde doc ? doc->cursor : ZEPHYRUM;
}

s64
pictor_documentum_finis (
    constans PictorDocumentum* doc)
{
    redde doc ? doc->finis : ZEPHYRUM;
}
```
Verified 2026-09-05: there is no `chorda_ex_s64` — `seq_chorda` above goes through `chorda_ex_f64(v, ZEPHYRUM, p)` exactly as `eventus_stml` writes `tempus`; `xar_truncare(Xar*, i32 numerus_novus)` exists (xar.h:316); `volumen_actum_appendere` returns the new seq (volumen.h says `s64`); `stml_liberum_ad_indicem` (canon.c uses it). The `proicere_ad` checkpoint search walks DOWN from the largest multiple of `intervallum` ≤ `ad` — a checkpoint whose seq was later made dead by a ramus is still a correct image of the acta at that seq (the ramus only kills acta AFTER `ab`), but a checkpoint at a seq GREATER than the ramus's `ab` and ≤ `ad` can only exist if it was written before the ramus — and `ad` is a live seq, so any checkpoint ≤ ad at a multiple of intervallum that is dead would give a wrong base. Guard: after choosing `basis`, confirm `basis` is live (in `acta_viva(doc, 0, ad)` there is an actum with `seq == basis`, or `basis` is 0); otherwise continue the walk. Add that check in Step 3 as written above becomes: replace `basis = s; frange;` with a live-check helper `seq_vivum(doc, s, ad)` — the probatio's ramus section (undo to 2, new stroke at seq 5 after ramus at 4; checkpoint/2 is live, fine; a checkpoint at 4 would be the ramus itself — never written because ramus appends do not checkpoint) pins the simple case; write a second ramus case in the worklog as a known hole if the check is deferred.

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh pictor_documentum` — Expected: PASS. Record the two printed measurements.

- [x] **Step 5: Plant, worklog, commit**

Plants (RED): (a) in `acta_viva`, skip the ramus handling (`perge` before the truncation) — red at the `mortuus` assertion; (b) in `pictor_documentum_actum`, drop `checkpoint_condere` — red at `checkpoint/2`. Green on revert.

Worklog `lib/pictor_documentum.worklog.md`: the append-only truth and the ramus idiom; checkpoint = massa + plagula; the measured ms for 200 acta and one undo at intervallum 2 and 64 (spec §10 asks for both numbers — run the measurement block twice by changing the `II` in `creare` to `LXIV` for the second reading and write both in the worklog and in spec §10); ignored acta genera; the dead-checkpoint hole if deferred.

Commit: `silva.commissio(msg, [...], portae=[('radix','pictor_documentum')])`.

---

### Task 5: `dispensator` — derived events deliver at the next event boundary

**Files:**
- Modify: `include/dispensator.h`, `lib/dispensator.c`, `lib/dispensator.worklog.md`
- Modify test: `probationes/probatio_dispensator.c`, `probationes/ludus_toy.h`

**Interfaces:**
- Consumes: Plan 1 T10 as landed.
- Produces: `vacuum dispensator_addressare(Dispensator* d, chorda id, eventus_genus_t genus, s64 tempus)` — the public `put` seat (brainstorm §XVI §1): enqueue a derived event addressed to a componens id; delivered after the CURRENT event's dispatch and recompose, against the NEW tree; events enqueued during delivery wait for the next boundary. `mittere_ad` becomes a private wrapper over it. `focus_petitus` follows the same path; the "still absent → clear focus" check runs after the boundary drain.

- [x] **Step 1: Write the failing test**

Add to `probationes/ludus_toy.h` a counting handler for the derived genera, registered under a new root-level action so the toy can observe deliveries. Insert after `toy_fugere` (`e.inserere_post('toy_fugere', …)`):
```c
/* Observator eventuum derivatorum (probatio T5): numerat quot
 * focus_captus/amissus/intravit/exiit ei tradantur, et quot
 * compositiones dispensator tunc numeraverit (ctx = ToyStatus*). */
interior b32
toy_observare (
    InsulaRepositorium* r,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* c,
     constans Eventus* e,
               vacuum* ctx)
{
    ToyStatus* toy;

    (vacuum)r;
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)c;
    toy = (ToyStatus*)ctx;
    commutatio (e->genus)
    {
        casus EVENTUS_FOCUS_CAPTUS:
        casus EVENTUS_FOCUS_AMISSUS:
        casus EVENTUS_MUS_INTRAVIT:
        casus EVENTUS_MUS_EXIIT:
            toy->derivata++;
            toy->compositiones_in_traditione = toy->compositiones;
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}
```
and extend `ToyStatus` to `{ i32 compositiones; i32 derivata; i32 compositiones_in_traditione; }` (`Editio.substituere('ToyStatus', …, genus='typus')`), register it in `toy_registrare` as `actio_registrare(reg, "observare", toy_observare, toy);`, and give `b1` the action... no — `b1` keeps `numerare`; give the ROOT a second reachable handler by changing the root's action to `"observare"` ONLY in this probatio: `probatio_dispensator.c` cannot change the toy's tree, so instead register `toy_observare` under `"fugere"` too? A name maps to one handler. Simplest honest change: the toy's root action stays `fugere`, and `toy_fugere` itself gains the counting arms (it already switches on genus; add the four `casus` lines before its ESC check, counting into the ToyStatus it receives as ctx). Do that instead of a new handler; `toy_observare` is not needed. Then in `probatio_dispensator.c`, after the `--- Ictus in b1` section's first `dispensator_tractare`:
```c
    /* focus_captus ad b1 TRADITUS post recompositionem, contra arborem
     * novam: numerus compositionum in traditione > numerus ante */
    CREDO_AEQUALIS_I32(toy.derivata, I);                /* captus b1 */
    CREDO_VERUM(toy.compositiones_in_traditione > n0);
```
and in the `--- Motus super tabulam` section after the first move: `CREDO_AEQUALIS_I32(toy.derivata, II);` (intravit tabula; no exiit because super was empty) and after the second move: `CREDO_AEQUALIS_I32(toy.derivata, IV);` (exiit tabula, intravit radix). Initialise `toy.derivata = ZEPHYRUM; toy.compositiones_in_traditione = ZEPHYRUM;` where `toy.compositiones` is. `n0` is captured before the click already.

- [x] **Step 2: Run to verify it fails**

Run: `./compile_tests.sh dispensator` — Expected: FAIL at `toy.compositiones_in_traditione > n0` (today the derived event is delivered BEFORE the recompose, so the count equals `n0`).

- [x] **Step 3: The change**

In `include/dispensator.h`, add to the struct (`Editio.membrum_addere('Dispensator', '                    Xar* differenda;             /* Xar de Differendum */', post='Xar* effusio;')`) and the prototype (`inserere_post('dispensator_recomponere', …, definitio=False)`):
```c
/* Sedes 'put' (brainstorm XVI §1): eventus derivatus ad id
 * directus, traditus post eventum currentem et recompositionem,
 * contra arborem NOVAM. In traditione directus: limen proximum. */
vacuum
dispensator_addressare (
    Dispensator* d,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus);
```
In `lib/dispensator.c`: a private type after the includes:
```c
nomen structura {
             chorda  id;
    eventus_genus_t  genus;
                s64  tempus;
} Differendum;
```
`dispensator_creare` allocates `d->differenda = xar_creare(piscina, (i32)magnitudo(Differendum));`. Substitute `mittere_ad` by name:
```c
/* Eventus derivatus: numquam statim - differtur ad limen */
interior vacuum
mittere_ad (
        Dispensator* d,
         Componens* c,
    eventus_genus_t genus,
                s64 tempus)
{
    si (!c)
    {
        redde;
    }
    dispensator_addressare(d, c->id, genus, tempus);
}
```
Add, after `mittere`:
```c
vacuum
dispensator_addressare (
    Dispensator* d,
          chorda  id,
 eventus_genus_t  genus,
             s64  tempus)
{
    Differendum* x;

    si (!d || chorda_vacua(id))
    {
        redde;
    }
    x = (Differendum*)xar_addere(d->differenda);
    x->id     = id;
    x->genus  = genus;
    x->tempus = tempus;
}

/* Limen: differenda TUNC praesentia traduntur contra arborem novam;
 * quae in traditione nascuntur, ad limen proximum manent. */
interior vacuum
limen_transire (
    Dispensator* d)
{
    Differendum* x;
     Destinatio  des;
        Eventus  e;
     Componens*  c;
            i32  i;
            i32  n;
            Xar* praesentia;

    n = xar_numerus(d->differenda);
    si (n == ZEPHYRUM)
    {
        redde;
    }
    praesentia = xar_creare(d->scratch, (i32)magnitudo(Differendum));
    per (i = ZEPHYRUM; i < n; i++)
    {
        x  = (Differendum*)xar_addere(praesentia);
        *x = *(Differendum*)xar_obtinere(d->differenda, i);
    }
    xar_vacare(d->differenda);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = (Differendum*)xar_obtinere(praesentia, i);
        c = invenire_id(d, x->id);
        si (!c)
        {
            perge;
        }
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus  = x->genus;
        e.tempus = x->tempus;
        des = destinatio_ex_componente(c, d->scratch);
        mittere(d, &des, &e);
    }
    dispensator_recomponere(d);
}
```
and in `tractare_unum`, replace the tail from `/* regula staleness */` to the `piscina_reficere` with:
```c
    /* regula staleness */
    dispensator_recomponere(d);

    /* limen: derivata contra arborem novam */
    limen_transire(d);

    /* focus ut petitio: id ex arbore nova absens -> radici (ad limen
     * proximum); si post id arbor id non reddit, focus tollitur */
    focus = dispensator_focus(d);
    si (!chorda_vacua(focus) && !invenire_id(d, focus))
    {
        mittere_ad(d, d->arbor, EVENTUS_FOCUS_PETITUS, e->tempus);
        limen_transire(d);
        si (!invenire_id(d, focus))
        {
            dispensator_focus_ponere(d, chorda_nulla());
        }
    }
    piscina_reficere(d->scratch, nota);
```
(`praesentia` lives in `d->scratch`, which `tractare_unum` resets at its end — the copy is what makes re-entrant `addressare` during delivery safe.)

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh dispensator && ./compile_tests.sh manus_ludus && ./compile_tests.sh pictor_toy` — Expected: PASS ×3 (the toy exemplar is unchanged: derived events are never in the log).

- [x] **Step 5: Plant, worklog, commit**

Plant (RED): in `mittere_ad`, deliver synchronously again (`des = destinatio_ex_componente(c, d->scratch); … mittere(d, &des, &e);` instead of `dispensator_addressare`) — red at `compositiones_in_traditione > n0`. Green on revert.

Worklog entry `## 2026-09-DD — limen (ludus P3 T5)` in `lib/dispensator.worklog.md`: why the boundary (§XVI §3–4), the copy-then-drain rule, the focus-petitus two-step.

Commit with `portae=[('radix','dispensator'), ('radix','manus_ludus'), ('radix','pictor_toy')]`.

---

### Task 6: The real island canons, owners per attribute, refusal by diff

**Files:**
- Create: `apps/pictor/canones/durabilis.canon`, `apps/pictor/canones/ephemera.canon`, `apps/pictor/canones/domini.stml`, `probationes/probatio_pictor_canones.c`
- Modify: `include/insula.h`, `lib/insula.c`, `lib/insula.worklog.md`, `probationes/probatio_insula.c`, `lib/dispensator.c` (sets the scriptor), `lib/motus.c` (`motus_effundere` sets scriptor "motus")

**Interfaces:**
- Consumes: `canon.h` (`canon_legere`, `canon_iudicare`), `stml.h`, `filum.h`.
- Produces: `insula_scriptorem_ponere(repo, chorda scriptor)` (the current writer's name; empty = anonymous); `insula_dominum_ponere(repo, genus, constans character* attributum, constans character* dominus) → b32`; `insula_dominos_legere(repo, genus, StmlNodus* domini) → i32` (count loaded from `<domini genus="ephemera"><dominus attributum="focus" scriptor="dispensator"/>…</domini>`); the gate refuses when a ROOT attribute that has an owner was added, changed, or removed by a scriptor other than that owner: `causa` = `dominus: <attributum> possidetur a <dominus>; scriptor '<scriptor>'`. Attributes with no owner are anyone's. Children (the `strata` list) are unowned in v1 (worklog names it).
- The dispensator sets the scriptor to `c->actio` around each handler call in `mittere`, to `"dispensator"` around its own focus writes, and clears it after; `motus_effundere` sets `"motus"` around its `mutare_ephemera` and restores the previous.

The canons (spec §4; the FLOW idiom from §XVI §1 is RESERVED here as `cursus`/`gradus`, owner `cursus`, unused until P5):

`apps/pictor/canones/ephemera.canon`:
```xml
<canon dialectus="pictor-ephemera" versio="1">
  <elementum nomen="ephemera" radix="verum">
    <attributum nomen="instrumentum"    genus="nomen"/>
    <attributum nomen="color_primus"    genus="numerus"/>
    <attributum nomen="color_secundus"  genus="numerus"/>
    <attributum nomen="magnitudo"       genus="numerus"/>
    <attributum nomen="stratum_activum" genus="numerus"/>
    <attributum nomen="zoom"            genus="numerus"/>
    <attributum nomen="pan_x"           genus="numerus"/>
    <attributum nomen="pan_y"           genus="numerus"/>
    <attributum nomen="focus"           genus="textus"/>
    <attributum nomen="focus_acervus"   genus="textus"/>
    <attributum nomen="selectio_x"      genus="numerus"/>
    <attributum nomen="selectio_y"      genus="numerus"/>
    <attributum nomen="selectio_latitudo" genus="numerus"/>
    <attributum nomen="selectio_altitudo" genus="numerus"/>
    <attributum nomen="cursus"          genus="nomen"
      nota="fluxus (brainstorm XVI §1) - P5"/>
    <attributum nomen="gradus"          genus="nomen"
      nota="gradus fluxus - P5"/>
    <attributum nomen="fuga"            genus="textus"
      nota="probationes solae"/>
    <attributum nomen="numerus"         genus="numerus"
      nota="probationes solae"/>
  </elementum>
</canon>
```
`apps/pictor/canones/durabilis.canon`:
```xml
<canon dialectus="pictor-durabilis" versio="1">
  <elementum nomen="documentum" radix="verum">
    <attributum nomen="latitudo" genus="numerus" necessarium="verum"/>
    <attributum nomen="altitudo" genus="numerus" necessarium="verum"/>
    <attributum nomen="paletta"  genus="textus"
      nota="XVI indices coloris, commate separati"/>
    <attributum nomen="sigillum" genus="textus"
      nota="checkpoint currens (hex)"/>
    <attributum nomen="titulus"  genus="textus"/>
    <attributum nomen="puncta"        genus="numerus" nota="probationes"/>
    <attributum nomen="ictus_numerus" genus="numerus" nota="probationes"/>
    <liberum nomen="stratum"/>
  </elementum>
  <elementum nomen="stratum">
    <attributum nomen="titulus"   genus="textus"  necessarium="verum"/>
    <attributum nomen="visibilis" genus="veritas"/>
    <attributum nomen="ordo"      genus="numerus"/>
  </elementum>
</canon>
```
(the `<liberum>` form is `aedilis.canon`'s — check `canon.canon` for `maximum`; the `nota=` attribute on `<attributum>` is declared in `canon.canon` line 122.)

`apps/pictor/canones/domini.stml`:
```xml
<domini>
  <dominus genus="ephemera" attributum="instrumentum"    scriptor="instrumentum.eligere"/>
  <dominus genus="ephemera" attributum="color_primus"    scriptor="color_primus.ponere"/>
  <dominus genus="ephemera" attributum="color_secundus"  scriptor="color_secundus.ponere"/>
  <dominus genus="ephemera" attributum="magnitudo"       scriptor="magnitudo.ponere"/>
  <dominus genus="ephemera" attributum="zoom"            scriptor="motus"/>
  <dominus genus="ephemera" attributum="pan_x"           scriptor="motus"/>
  <dominus genus="ephemera" attributum="pan_y"           scriptor="motus"/>
  <dominus genus="ephemera" attributum="focus"           scriptor="dispensator"/>
  <dominus genus="ephemera" attributum="focus_acervus"   scriptor="dispensator"/>
  <dominus genus="ephemera" attributum="cursus"          scriptor="cursus"/>
  <dominus genus="ephemera" attributum="gradus"          scriptor="cursus"/>
  <dominus genus="durabilis" attributum="sigillum"       scriptor="documentum"/>
</domini>
```

- [x] **Step 1: Write the failing tests**

Append to `probationes/probatio_insula.c` before `--- Probans scribere ---`:
```c
    imprimere("\n--- Probans dominos: scriptor alienus recusatur ---\n");
    CREDO_VERUM(insula_dominum_ponere(repo, INSULA_EPHEMERA, "focus",
                                      "dispensator"));
    CREDO_FALSUM(insula_dominum_ponere(repo, INSULA_EPHEMERA, "", "x"));
    /* anonymus (scriptor vacuus) attributum possessum tangit: recusatur */
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_focum, focus_b1));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("dominus", piscina));
    /* dominus ipse: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("dispensator", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_focum, focus_b1));
    a = insula_attributum(repo, INSULA_EPHEMERA, "focus");
    CREDO_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(*a, "b1");
    /* scriptor alienus attributum NON possessum tangit: licet */
    insula_scriptorem_ponere(repo, chorda_ex_literis("quisquis", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_instrumentum, linea));
    /* scriptor alienus attributum possessum TOLLIT: recusatur */
    CREDO_FALSUM(mutare_ephemera(repo, tollere_focum, NIHIL));
    /* versio immota a recusationibus: III scripturae acceptae */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), IV);
    insula_scriptorem_ponere(repo, chorda_ex_literis("", piscina));
```
with two mutators at the top of the file (after `addere_ignotum`): `ponere_focum` (`insula_attributum_ponere(radix, p, in, "focus", (constans character*)ctx)`) and `tollere_focum` (removes the attribute through `insula_attributum_tollere(nodus, "focus")`, added to `insula.h` in this task — stml has no removal primitive; `xar_removere_cum_ultimo(nodus->attributa, i)` (xar.h:360, swap-remove) does it in O(1); attribute order is not semantic) and `character focus_b1[] = "b1";` in `principale`. Note the versio arithmetic: before this section the test has done III accepted writes (impletio, linea via canon section, durabile) — count the accepted `mutare_ephemera` calls above this point in the file as it stands and set the expected number accordingly (the canon section's `linea` write and the `impletio` write = II on ephemera; this section adds II more = IV). The canon set earlier in the test declares `instrumentum` and `focus` only — the ownership test writes those two, so the canon still accepts.

`probationes/probatio_pictor_canones.c` (new):
```c
/* probatio_pictor_canones.c - canones veri insularum pictoris et
 * tabula dominorum: leguntur, iudicant, refusant */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "insula.h"
#include "credo.h"
#include <stdio.h>

interior Canon*
canonem_legere (
    Piscina* p,
    InternamentumChorda* in,
    constans character* via)
{
    chorda fons;
    chorda causa;
    Canon* c;

    fons = filum_legere_totum(via, p);
    si (fons.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        imprimere("canon %s: %.*s\n", via, (int)causa.mensura, causa.datum);
    }
    redde c;
}

interior vacuum
ponere_ignotum (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "ignotissimum", "x");
}

interior vacuum
ponere_zoom (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "zoom", "2");
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                  Canon* durabilis;
                  Canon* ephemera;
     InsulaRepositorium* repo;
           StmlResultus  res;
                 chorda  domini;

    piscina = piscina_generare_dynamicum("probatio_pictor_canones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- Canones leguntur ---\n");
    durabilis = canonem_legere(piscina, intern,
                               "apps/pictor/canones/durabilis.canon");
    ephemera  = canonem_legere(piscina, intern,
                               "apps/pictor/canones/ephemera.canon");
    CREDO_NON_NIHIL(durabilis);
    CREDO_NON_NIHIL(ephemera);

    imprimere("\n--- Insulae pictoris iudicantur ---\n");
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\" paletta=\"\">"
        "<stratum titulus=\"fundus\" visibilis=\"verum\" ordo=\"0\"/>"
        "</documentum>",
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo=\"1\" stratum_activum=\"0\""
        " zoom=\"1\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    insula_ponere_canonem(repo, INSULA_DURABILIS, durabilis);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, ephemera);
    /* attributum non declaratum: recusatur */
    CREDO_FALSUM(mutare_ephemera(repo, ponere_ignotum, NIHIL));
    CREDO_CHORDA_CONTINET(insula_causa(repo),
                          chorda_ex_literis("canon", piscina));

    imprimere("\n--- Domini leguntur; motus solus zoom scribit ---\n");
    domini = filum_legere_totum("apps/pictor/canones/domini.stml", piscina);
    CREDO_CHORDA_NON_VACUA(domini);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina), piscina,
                                 intern);
    CREDO_VERUM(res.successus);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_EPHEMERA,
                                             res.elementum_radix), XI);
    CREDO_AEQUALIS_I32(insula_dominos_legere(repo, INSULA_DURABILIS,
                                             res.elementum_radix), I);
    insula_scriptorem_ponere(repo, chorda_ex_literis("zoom.ponere", piscina));
    CREDO_FALSUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    insula_scriptorem_ponere(repo, chorda_ex_literis("motus", piscina));
    CREDO_VERUM(mutare_ephemera(repo, ponere_zoom, NIHIL));
    CREDO_VERUM(insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
(`zoom.ponere` is in the spec's action list but the domini table gives `zoom` to `motus` — the quiet seat writes it; `zoom.ponere` will go THROUGH motus in P4. The probatio pins the table as written; change both together if the decision changes.)

- [x] **Step 2: Run to verify they fail**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh insula` — Expected: FAIL to compile (`insula_dominum_ponere` undeclared). `./compile_tests.sh pictor_canones` — Expected: FAIL (canon files absent → `CREDO_NON_NIHIL(durabilis)` red, and the same undeclared symbols).

- [x] **Step 3: Write the canons, the owner table, the refusal**

Write the three files under `apps/pictor/canones/` exactly as above (`mkdir -p apps/pictor/canones`). Validate them standalone first: `bin/canon_examen -canon apps/pictor/canones/ephemera.canon <(printf '<ephemera zoom="1"/>')` if the tool supports stdin, else a temp file — exit 0.

`include/insula.h` additions (`inserere_post('insula_ponere_actarium', …, definitio=False)` and `membrum_addere`):
```c
/* ==================================================
 * Domini (brainstorm XVI §2: lectio globalis, scriptura possessa)
 * ================================================== */

/* Scriptor currens (nomen actionis, 'dispensator', 'motus', ...);
 * vacuus = anonymus. Dispensator eum circa tractatorem ponit. */
vacuum
insula_scriptorem_ponere (
    InsulaRepositorium* repo,
                chorda  scriptor);

/* Attributum radicis generis dato uni scriptori addicere. FALSUM si
 * attributum vacuum. */
b32
insula_dominum_ponere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* attributum,
    constans character* dominus);

/* <domini><dominus genus attributum scriptor/>...</domini>: lineae
 * generis dati leguntur; redde numerum lectarum. */
i32
insula_dominos_legere (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
            StmlNodus*  domini);

/* Attributum radicis tollere (par insula_attributum_ponere). */
b32
insula_attributum_tollere (
              StmlNodus* nodus,
     constans character* titulus);
```
struct members: `Xar* domini[INSULA_GENUS_NUMERUS]; /* Xar de InsulaDominus */` and `chorda scriptor;`, with `nomen structura { chorda attributum; chorda dominus; } InsulaDominus;` before the repository struct.

`lib/insula.c`: in `insula_repositorium_creare`, `repo->domini[g] = xar_creare(piscina, (i32)magnitudo(InsulaDominus));` per genus; the four functions; and in `mutare`, between the mutator call and the canon judgment, the diff:
```c
    /* domini: attributa radicis mutata contra tabulam dominorum */
    causa_dominorum = dominos_iudicare(repo, genus, repo->radices[genus],
                                       duplicatum);
    si (!chorda_vacua(causa_dominorum))
    {
        repo->causa = causa_dominorum;
        redde FALSUM;
    }
```
with the helper (before `mutare`):
```c
interior constans InsulaDominus*
dominum_invenire (
    constans InsulaRepositorium* repo,
                    InsulaGenus  genus,
                         chorda  attributum)
{
    InsulaDominus* d;
              i32  i;
              i32  n;

    n = xar_numerus(repo->domini[genus]);
    per (i = ZEPHYRUM; i < n; i++)
    {
        d = (InsulaDominus*)xar_obtinere(repo->domini[genus], i);
        si (chorda_aequalis(d->attributum, attributum))
        {
            redde d;
        }
    }
    redde NIHIL;
}

/* valor attributi tituli dati aut NIHIL */
interior chorda*
valor_attributi (
    StmlNodus* n,
        chorda titulus)
{
    StmlAttributum* a;
               i32  i;
               i32  k;

    si (!n->attributa)
    {
        redde NIHIL;
    }
    k = xar_numerus(n->attributa);
    per (i = ZEPHYRUM; i < k; i++)
    {
        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        si (a->titulus && chorda_aequalis(*a->titulus, titulus))
        {
            redde a->valor;
        }
    }
    redde NIHIL;
}

/* attributum mutatum? (additum, mutatum, sublatum) */
interior b32
attributum_mutatum (
    StmlNodus* ante,
    StmlNodus* post,
        chorda titulus)
{
    chorda* a;
    chorda* b;

    a = valor_attributi(ante, titulus);
    b = valor_attributi(post, titulus);
    si (!a && !b)
    {
        redde FALSUM;
    }
    si (!a || !b)
    {
        redde VERUM;
    }
    redde !chorda_aequalis(*a, *b);
}

/* causa vacua = licet */
interior chorda
dominos_iudicare (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
            StmlNodus*  ante,
            StmlNodus*  post)
{
    InsulaDominus* d;
              i32  i;
              i32  n;
           chorda  causa;

    n = xar_numerus(repo->domini[genus]);
    per (i = ZEPHYRUM; i < n; i++)
    {
        d = (InsulaDominus*)xar_obtinere(repo->domini[genus], i);
        si (!attributum_mutatum(ante, post, d->attributum))
        {
            perge;
        }
        si (chorda_aequalis(d->dominus, repo->scriptor))
        {
            perge;
        }
        causa = chorda_ex_literis("dominus: ", repo->piscina);
        causa = chorda_concatenare(causa, d->attributum, repo->piscina);
        causa = chorda_concatenare(causa,
            chorda_ex_literis(" possidetur a ", repo->piscina), repo->piscina);
        causa = chorda_concatenare(causa, d->dominus, repo->piscina);
        causa = chorda_concatenare(causa,
            chorda_ex_literis("; scriptor '", repo->piscina), repo->piscina);
        causa = chorda_concatenare(causa, repo->scriptor, repo->piscina);
        causa = chorda_concatenare(causa, chorda_ex_literis("'", repo->piscina),
                                   repo->piscina);
        redde causa;
    }
    redde chorda_nulla_insulae();
}
```
(`chorda_nulla_insulae` = the `{0, NIHIL}` helper insula.c already has in spirit — add it as a static if absent.) `insula_dominos_legere` walks `domini`'s element children, keeps those whose `genus` attribute names the given genus (`"durabilis"`/`"ephemera"`), and calls `insula_dominum_ponere` with `attributum`/`scriptor`. `insula_attributum_tollere` finds the attribute by title and calls `xar_removere_cum_ultimo(nodus->attributa, i)`; returns FALSUM if absent.

`lib/dispensator.c` `mittere`: around `fn(...)`: `insula_scriptorem_ponere(d->repo, c->actio); consumptus = fn(...); insula_scriptorem_ponere(d->repo, chorda_nulla());` and in `attr_scribere` (the focus writes): set `"dispensator"` before `mutare_ephemera`, restore empty after. `lib/motus.c` `motus_effundere`: set `"motus"` before, empty after (it has no memory of the previous scriptor; the dispensator calls it outside any handler, so empty is right — the worklog says so).

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh insula && ./compile_tests.sh pictor_canones && ./compile_tests.sh dispensator && ./compile_tests.sh motus && ./compile_tests.sh pictor_toy` — all PASS. (The toy declares no owners, so nothing changes for it.)

- [x] **Step 5: Plant, worklog, commit**

Plant (RED): in `dominos_iudicare`, `si (chorda_aequalis(d->dominus, repo->scriptor)) { perge; }` → compare against `d->attributum` instead — the owner is refused too: red at `CREDO_VERUM(mutare_ephemera(repo, ponere_focum, focus_b1))`. Green on revert.

Worklog `lib/insula.worklog.md` `## 2026-09-DD — domini (ludus P3 T6)`: reads global, writes owned; the diff at the gate costs one attribute walk per write; children unowned in v1; the scriptor is a runtime property of the dispatch context (handlers keep their signature; L5 untouched); the flow attributes reserved.

Commit with `portae=[('radix','insula'), ('radix','pictor_canones'), ('radix','dispensator'), ('radix','motus'), ('radix','pictor_toy')]`; files include the three `apps/pictor/canones/*` and the new probatio.

---

### Task 7: `pictor_componentia` — pictor's `componere`: prospectus, tabula, status

**Files:**
- Create: `include/pictor_componentia.h`, `lib/pictor_componentia.c`, `lib/pictor_componentia.worklog.md`
- Test: `probationes/probatio_pictor_componentia.c`; exemplar `probationes/pictor/pictor.arbor.stml` (born on first run)

**Interfaces:**
- Consumes: `Componens` (with `s32` fines after T1; `puncta`/`numerus_punctorum` are serialized and compared — Plan 1 T3), `InsulaRepositorium` (reads only), `Motus` (reads only), `Componere` (T10 of Plan 1), `pictor_commune.h`.
- Produces: `PictorCompositio { i32 fenestra_latitudo; i32 fenestra_altitudo; i32 status_altitudo; }` (the ctx); `Componens* pictor_componere(InsulaRepositorium*, constans Motus*, Piscina*, InternamentumChorda*, vacuum* ctx)` — `Componere`-shaped, `<componens/> <purus/>`; `constans character* pictor_actio_instrumenti(chorda instrumentum)` (`"penicillus"` → `"penicillus.ictus"`, anything else → `""`).
- **The tree (spec §5.1, v1 subset; roles are DATA — brainstorm §XVI §5):**

```
radix       (0,0,W,H)            partes nullum   actio "instrumentum.eligere" (hotkeys bubble here)
  prospectus (0,0,W,H-status)    partes nullum   sectio; translatio = motus.pan; scala = ephemera.zoom (>= I)
    tabula   (0,0,docW,docH)     partes tabula   PROPRIUS; focusabilis; actio = pictor_actio_instrumenti(ephemera.instrumentum);
                                                 puncta = motus.ictus_pendens (copied; the tier-3 snapshot shows the stroke in flight)
  status     (0,H-status,W,status) partes titulus  titulus = ephemera.instrumentum (or "nihil")
```
`docW`/`docH` come from the durabilis island's `latitudo`/`altitudo` (the canon requires them); the viewport from the ctx. `pingere` reads the TREE ONLY (decision 2026-09-05), which is why the pending stroke is copied into `tabula.puncta` here rather than read from `Motus` in the figura.

- [x] **Step 1: Write the failing test**

`probationes/probatio_pictor_componentia.c`:
```c
/* probatio_pictor_componentia.c - componere pictoris: arbor ex
 * insulis + motu, pura; exemplar arboris (gradus III) */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "insula.h"
#include "motus.h"
#include "componens.h"
#include "pictor_componentia.h"
#include "pictor_commune.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes  = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes = *(Punctum*)ctx;
}

interior vacuum
pan_ponere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -V;
    motus->pan.y = III;
}

interior vacuum
instrumentum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "instrumentum",
                             (constans character*)ctx);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
                  Motus  motus;
       PictorCompositio  ctx;
              Componens* arbor;
              Componens* arbor2;
              Componens* c;
                Punctum  p;
              character  ignotum[] = "ignotum";

    piscina = piscina_generare_dynamicum("probatio_pictor_componentia",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" zoom=\"2\" focus=\"tabula\"/>");
    CREDO_NON_NIHIL(repo);
    motus_initiare(&motus, piscina);
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    p.x = X;
    p.y = XX;
    mutare_motum(&motus, punctum_addere, &p, M + X);
    p.x = XII;
    p.y = XXII;
    mutare_motum(&motus, punctum_addere, &p, M + XX);
    ctx.fenestra_latitudo = CCCXX;
    ctx.fenestra_altitudo = CCXL;
    ctx.status_altitudo   = XII;

    imprimere("\n--- Actio ex instrumento ---\n");
    CREDO_VERUM(chorda_aequalis_literis(
        chorda_ex_literis(pictor_actio_instrumenti(
            chorda_ex_literis("penicillus", piscina)), piscina),
        "penicillus.ictus"));
    CREDO_AEQUALIS_I32((i32)pictor_actio_instrumenti(
        chorda_ex_literis("ignotum", piscina))[ZEPHYRUM], ZEPHYRUM);

    imprimere("\n--- Arbor ---\n");
    arbor = pictor_componere(repo, &motus, piscina, intern, &ctx);
    CREDO_NON_NIHIL(arbor);
    CREDO_CHORDA_AEQUALIS_LITERIS(arbor->id, "radix");
    CREDO_CHORDA_AEQUALIS_LITERIS(arbor->actio, "instrumentum.eligere");
    CREDO_AEQUALIS_S32(arbor->fines.latitudo, CCCXX);
    CREDO_AEQUALIS_I32(componens_numerus_liberorum(arbor), II);
    c = componens_invenire_per_id(arbor, chorda_ex_literis("prospectus",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->sectio);
    CREDO_AEQUALIS_S32(c->fines.altitudo, CCXL - XII);
    CREDO_AEQUALIS_S32(c->translatio.x, -V);
    CREDO_AEQUALIS_S32(c->translatio.y, III);
    CREDO_AEQUALIS_I32(c->scala, II);
    CREDO_FALSUM(c->focusabilis);
    c = componens_invenire_per_id(arbor, chorda_ex_literis("tabula",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->partes == PARTES_TABULA);
    CREDO_VERUM(c->praedicatum == PRAEDICATUM_PROPRIUS);
    CREDO_VERUM(c->focusabilis);
    CREDO_AEQUALIS_S32(c->fines.latitudo, CCCXX);
    CREDO_AEQUALIS_S32(c->fines.altitudo, CC);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->actio, "penicillus.ictus");
    CREDO_AEQUALIS_I32(c->numerus_punctorum, II);
    CREDO_AEQUALIS_S32(c->puncta[I].x, XII);
    CREDO_VERUM(c->parens != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->parens->id, "prospectus");
    c = componens_invenire_per_id(arbor, chorda_ex_literis("status",
                                                           piscina));
    CREDO_NON_NIHIL(c);
    CREDO_VERUM(c->partes == PARTES_TITULUS);
    CREDO_AEQUALIS_S32(c->fines.y, CCXL - XII);
    CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "penicillus");
    CREDO_FALSUM(c->focusabilis);

    imprimere("\n--- Instrumentum ignotum: tabula sine actione; zoom"
              " absens: scala I ---\n");
    CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere, ignotum));
    arbor2 = pictor_componere(repo, &motus, piscina, intern, &ctx);
    c = componens_invenire_per_id(arbor2, chorda_ex_literis("tabula",
                                                            piscina));
    CREDO_VERUM(chorda_vacua(c->actio));
    c = componens_invenire_per_id(arbor2, chorda_ex_literis("status",
                                                            piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "ignotum");
    {
        InsulaRepositorium* repo2;
        repo2 = insula_repositorium_creare(piscina, intern,
            "<documentum latitudo=\"16\" altitudo=\"8\"/>", "<ephemera/>");
        arbor2 = pictor_componere(repo2, &motus, piscina, intern, &ctx);
        c = componens_invenire_per_id(arbor2,
                chorda_ex_literis("prospectus", piscina));
        CREDO_AEQUALIS_I32(c->scala, I);
        c = componens_invenire_per_id(arbor2,
                chorda_ex_literis("status", piscina));
        CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "nihil");
    }

    imprimere("\n--- Puritas et exemplar (gradus III) ---\n");
    arbor2 = pictor_componere(repo, &motus, piscina, intern, &ctx);
    CREDO_VERUM(componens_aequalis(arbor, arbor2) == FALSUM);  /* instr. mutatum */
    CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere,
        (vacuum*)"penicillus") == FALSUM || VERUM);   /* vide notam */
    CREDO_VERUM(exemplar_conferre(piscina, "probationes/pictor/pictor.arbor.stml",
        componens_scribere_stml(arbor, piscina, intern, VERUM)));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
Two lines above need care when typing: the `(vacuum*)"penicillus"` cast is exactly what `-Wcast-qual` forbids — use a `character penicillus[] = "penicillus";` array and drop the tautological `== FALSUM || VERUM` (write `CREDO_VERUM(mutare_ephemera(repo, instrumentum_ponere, penicillus));` then `arbor2 = pictor_componere(...); CREDO_VERUM(componens_aequalis(arbor, arbor2));` — that is the purity assertion: same islands + same motus ⇒ equal tree). The `(i32)…[ZEPHYRUM]` assertion checks the empty string; write it as `CREDO_VERUM(pictor_actio_instrumenti(chorda_ex_literis("ignotum", piscina))[ZEPHYRUM] == ZEPHYRUM);`.

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh pictor_componentia` — Expected: FAIL, header not found.

- [x] **Step 3: Write header and implementation**

`include/pictor_componentia.h`:
```c
/* pictor_componentia.h - componere pictoris (P3: prospectus, tabula,
 * status)
 *
 * <componens/> <purus/>: arbor logica ex insulis (lectio) et motu
 * (lectio) in piscinam datam; nihil scribit. Partes sunt DATA
 * (brainstorm XVI §5): nullus typus per componens, actio nominata
 * ex statu (tabula: ex ephemera.instrumentum - nomen functio pura
 * status, tractator numquam de instrumento ramificat, spec §5.1).
 * Ictus pendens in tabula.puncta COPIATUR ut pingere arborem solam
 * legat (decisum 2026-09-05).
 */

#ifndef PICTOR_COMPONENTIA_H
#define PICTOR_COMPONENTIA_H

/* <aedilis corpus="lib/pictor_componentia.c"/> */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"

nomen structura {
    i32 fenestra_latitudo;
    i32 fenestra_altitudo;
    i32 status_altitudo;
} PictorCompositio;

/* "penicillus" -> "penicillus.ictus"; ignotum -> "" */
constans character*
pictor_actio_instrumenti (
    chorda instrumentum);

/* Componere-formata (dispensator.h): ctx = PictorCompositio* */
Componens*
pictor_componere (
     InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* piscina,
    InternamentumChorda* intern,
                vacuum* ctx);

#endif /* PICTOR_COMPONENTIA_H */
```
`lib/pictor_componentia.c`:
```c
/* pictor_componentia.c - componere pictoris */

#include "pictor_componentia.h"
#include "xar.h"

#include <string.h>

/* ==================================================
 * Auxilia (lectio insularum)
 * ================================================== */

interior s32
attributum_s32 (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_attributum(repo, genus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
attributum_chorda (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_attributum(repo, genus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;
    redde vacua;
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
    Componens* c;
         Fines f;

    c = componens_creare(p, in, id, partes);
    f.x        = x;
    f.y        = y;
    f.latitudo = w;
    f.altitudo = h;
    componens_ponere_fines(c, f);
    redde c;
}

constans character*
pictor_actio_instrumenti (
    chorda instrumentum)
{
    si (chorda_aequalis_literis(instrumentum, "penicillus"))
    {
        redde "penicillus.ictus";
    }
    redde "";
}

/* ==================================================
 * Componere
 * ================================================== */

/* <componens/> <purus/> */
Componens*
pictor_componere (
     InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* piscina,
    InternamentumChorda* intern,
                vacuum* ctx)
{
    PictorCompositio* cfg;
          Componens* radix;
          Componens* prospectus;
          Componens* tabula;
          Componens* status;
              chorda instrumentum;
                 s32 doc_latitudo;
                 s32 doc_altitudo;
                 s32 zoom;
                 s32 status_altitudo;
                 i32 n;
                 i32 i;

    si (!repo || !piscina || !intern || !ctx)
    {
        redde NIHIL;
    }
    cfg             = (PictorCompositio*)ctx;
    status_altitudo = (s32)cfg->status_altitudo;
    instrumentum    = attributum_chorda(repo, INSULA_EPHEMERA, "instrumentum");
    doc_latitudo    = attributum_s32(repo, INSULA_DURABILIS, "latitudo", I);
    doc_altitudo    = attributum_s32(repo, INSULA_DURABILIS, "altitudo", I);
    zoom            = attributum_s32(repo, INSULA_EPHEMERA, "zoom", I);
    si (zoom < I)
    {
        zoom = I;
    }

    radix = nodus(piscina, intern, "radix", PARTES_NULLUM, ZEPHYRUM,
                  ZEPHYRUM, (s32)cfg->fenestra_latitudo,
                  (s32)cfg->fenestra_altitudo);
    componens_ponere_actio(radix, "instrumentum.eligere");

    prospectus = nodus(piscina, intern, "prospectus", PARTES_NULLUM,
                       ZEPHYRUM, ZEPHYRUM, (s32)cfg->fenestra_latitudo,
                       (s32)cfg->fenestra_altitudo - status_altitudo);
    componens_ponere_sectio(prospectus, VERUM);
    componens_ponere_transformatio(prospectus,
        motus ? motus->pan.x : ZEPHYRUM, motus ? motus->pan.y : ZEPHYRUM,
        (i32)zoom);

    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA, ZEPHYRUM,
                   ZEPHYRUM, doc_latitudo, doc_altitudo);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_focusabilis(tabula, VERUM);
    componens_ponere_actio(tabula, pictor_actio_instrumenti(instrumentum));
    n = motus && motus->ictus_pendens ? xar_numerus(motus->ictus_pendens)
                                      : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        tabula->puncta = (Punctum*)piscina_allocare(piscina,
            (memoriae_index)n * magnitudo(Punctum));
        per (i = ZEPHYRUM; i < n; i++)
        {
            tabula->puncta[i] = *(Punctum*)xar_obtinere(motus->ictus_pendens,
                                                        i);
        }
        tabula->numerus_punctorum = n;
    }

    status = nodus(piscina, intern, "status", PARTES_TITULUS, ZEPHYRUM,
                   (s32)cfg->fenestra_altitudo - status_altitudo,
                   (s32)cfg->fenestra_latitudo, status_altitudo);
    componens_ponere_titulum(status,
        chorda_vacua(instrumentum) ? "nihil"
                                   : chorda_ut_cstr(instrumentum, piscina));

    componens_addere_liberum(prospectus, tabula);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    redde radix;
}
```
`componens_ponere_actio`/`_titulum` take `constans character*` and intern (Plan 1 T3); `chorda_ut_cstr` allocates in the compose piscina, which is what we want (the tree's arena). `tabula->puncta` is assigned directly: `Componens` is a plain record and has no puncta setter (Plan 1 T3 set it only through STML); the worklog names the dual use of `puncta` (polygon predicate / pending stroke on a PROPRIUS tabula).

- [x] **Step 4: First run — the arbor exemplar is born**

Run: `./compile_tests.sh pictor_componentia` — Expected: all assertions PASS except `exemplar_conferre`, which writes `probationes/pictor/pictor.arbor.stml.novum`. Read it: four componentia, `translatio_x="-5"`, `scala="2"`, `puncta="10,20;12,22"` (whatever separator Plan 1's serializer uses), `titulus="penicillus"`. Promote with `mv`. Second run: PASS.

- [x] **Step 5: Plant, worklog, commit**

Plant (RED): drop the `puncta` copy (`si (n > ZEPHYRUM)` → `si (FALSUM && n > ZEPHYRUM)`) — red at `numerus_punctorum == II` and at the exemplar. Green on revert.

Worklog `lib/pictor_componentia.worklog.md`: roles as data (four nodes, one record type); the action chosen at compose time; why the stroke is copied; `zoom` and dimensions read from the islands, the viewport from ctx; what P4 adds (instrumentarium, paletta, magnitudo_selector) and where they hang (children of radix, before status).

Commit with `portae=[('radix','pictor_componentia')]`.

---

### Task 8: `pictor_actiones` — `penicillus.ictus` and `instrumentum.eligere`

**Files:**
- Create: `include/pictor_actiones.h`, `lib/pictor_actiones.c`, `lib/pictor_actiones.worklog.md`
- Test: `probationes/probatio_pictor_actiones.c` (tier 2 exactly as spec §6.1 words it, driven through the real loop)

**Interfaces:**
- Consumes: `ActioRegistrum`/`ActioFn` (T9 of Plan 1), `Motus`, `InsulaRepositorium`, `PictorDocumentum` (T4), `Destinatio`, `Eventus`.
- Produces: `PictorActiones { PictorDocumentum* doc; }` (ctx); `vacuum pictor_actiones_registrare(ActioRegistrum* reg, PictorActiones* ctx)` registering `"penicillus.ictus"` → `pictor_penicillus_ictus` and `"instrumentum.eligere"` → `pictor_instrumentum_eligere`; both `<tractator/>` (no I/O; the document write goes through `pictor_documentum_actum`, which is the document's own gate — the worklog says so).
- **`penicillus.ictus`** (spec §5.2 "The stroke", one handler, phase by genus): `MUS_DEPRESSUS` → capture on the tabula, clear the pending points, add `punctum_locale`, consumed; `MUS_MOTUS` → if captured add the point, consumed (else FALSUM); `MUS_LIBERATUS` → if captured build ONE `<ictus instrumentum="penicillus" color="C" magnitudo="M"><punctum x y/>…</ictus>` from `motus->ictus_pendens` with `C` = `ephemera.color_primus` (default `0`) and `M` = `ephemera.magnitudo` (default `1`), append it via `pictor_documentum_actum`, clear the points, `sordida = FALSUM` (a finished stroke never touches the ephemera island), release capture, consumed; `CLAVIS_DEPRESSUS` with `typus == 27` while captured → clear, release, consumed, NO actum (Escape mid-stroke); anything else → FALSUM.
- **`instrumentum.eligere`**: `CLAVIS_DEPRESSUS` with `typus == 'p'` → `instrumentum="penicillus"` through `mutare_ephemera`, consumed; else FALSUM. (The owner table gives `instrumentum` to `instrumentum.eligere`; the dispensator sets the scriptor to the action's name around the call — T6.)

- [x] **Step 1: Write the failing test**

`probationes/probatio_pictor_actiones.c`:
```c
/* probatio_pictor_actiones.c - gradus II: ictus scriptus UNUM <ictus>
 * cum n punctis parit, insula ephemera intacta (praeter focum);
 * Escape medio ictu nihil parit; 'p' instrumentum eligit */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "motus.h"
#include "actio.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "credo.h"
#include <stdio.h>

interior i32
numerare_puncta (
    chorda actum,
    Piscina* p,
    InternamentumChorda* in)
{
    StmlResultus res;

    res = stml_legere_ex_literis(chorda_ut_cstr(actum, p), p, in);
    si (!res.successus || !res.elementum_radix)
    {
        redde -I;
    }
    redde stml_numerus_liberorum(res.elementum_radix);
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
         PictorActiones  actiones;
       PictorCompositio  cfg;
            Dispensator* d;
             ManusLudus* m;
                Punctum  via[III];
                   Xar*  acta;
           VolumenActum* a;
                    i32  versio_ante;
                 chorda  causa;
                 chorda  fons;
                  Canon* ephemera;

    piscina = piscina_generare_dynamicum("probatio_pictor_actiones",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_actiones");
    doc = pictor_documentum_creare(piscina, intern, vol, CCCXX, CC, LXIV);
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"320\" altitudo=\"200\"/>",
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " magnitudo=\"1\" zoom=\"1\"/>");
    fons = filum_legere_totum("apps/pictor/canones/ephemera.canon", piscina);
    ephemera = canon_legere(fons, piscina, intern, &causa);
    CREDO_NON_NIHIL(ephemera);
    insula_ponere_canonem(repo, INSULA_EPHEMERA, ephemera);
    {
        chorda domini;
        StmlResultus res;
        domini = filum_legere_totum("apps/pictor/canones/domini.stml",
                                    piscina);
        res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina),
                                     piscina, intern);
        CREDO_VERUM(res.successus);
        insula_dominos_legere(repo, INSULA_EPHEMERA, res.elementum_radix);
    }
    reg = actio_registrum_creare(piscina, intern);
    actiones.doc = doc;
    pictor_actiones_registrare(reg, &actiones);
    cfg.fenestra_latitudo = CCCXX;
    cfg.fenestra_altitudo = CCXII;
    cfg.status_altitudo   = XII;
    d = dispensator_creare(piscina, intern, repo, reg, pictor_componere,
                           &cfg, CCC);
    CREDO_NON_NIHIL(d);
    m = manus_ludus_creare(piscina, d);
    CREDO_MANUS_LUDUS_EXISTIT(m, "#tabula");
    CREDO_MANUS_LUDUS_EXISTIT(m, "[actio=penicillus.ictus]");
    CREDO_AEQUALIS_I32(xar_numerus(actio_non_registratae(reg,
        dispensator_arbor(d), piscina)), ZEPHYRUM);          /* L10 */

    imprimere("\n--- Ictus scriptus: UNUM <ictus> cum III punctis ---\n");
    versio_ante = insula_versio(repo, INSULA_EPHEMERA);
    via[ZEPHYRUM].x = X;
    via[ZEPHYRUM].y = XX;
    via[I].x        = XV;
    via[I].y        = XXV;
    via[II].x       = XX;
    via[II].y       = XXX;
    CREDO_VERUM(manus_ludus_trahere(m, "#tabula", via, III));
    acta = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    {
        i32 i;
        i32 n;
        i32 ictus;
        ictus = ZEPHYRUM;
        n = xar_numerus(acta);
        per (i = ZEPHYRUM; i < n; i++)
        {
            a = (VolumenActum*)xar_obtinere(acta, i);
            si (chorda_aequalis_literis(a->genus, "ictus"))
            {
                ictus++;
                CREDO_AEQUALIS_I32(numerare_puncta(a->datum, piscina, intern),
                                   III);
                CREDO_CHORDA_CONTINET(a->datum,
                    chorda_ex_literis("instrumentum=\"penicillus\"", piscina));
            }
        }
        CREDO_AEQUALIS_I32(ictus, I);
    }
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), I);
    /* motus purgatus, captura soluta, nihil sordidum */
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       ZEPHYRUM);
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_FALSUM(dispensator_motus(d)->sordida);
    /* ephemera: focus solus scriptus (versio + I), pan absens */
    CREDO_AEQUALIS_I32(insula_versio(repo, INSULA_EPHEMERA), versio_ante + I);
    CREDO_MANUS_LUDUS_FOCUS(m, "tabula");
    CREDO_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "pan_x"));
    CREDO_VERUM(insula_restituere(repo));
    /* proiectio: pixelum sub (15,25) nigrum */
    {
        constans Imago* im;
        constans i8*    px;
        im = pictor_documentum_proiectio(doc);
        px = im->pixela + (XXV * im->latitudo + XV) * IV;
        CREDO_AEQUALIS_I32((i32)px[ZEPHYRUM], ZEPHYRUM);
        CREDO_AEQUALIS_I32((i32)px[III], CCLV);
    }

    imprimere("\n--- Escape medio ictu: nihil paritur ---\n");
    manus_ludus_premere_ad(m, XL, XL);          /* nihil: premere = ictus */
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), II);
    {
        Eventus e;
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUS_DEPRESSUS;
        e.tempus = m->tempus;
        e.datum.mus.x = L;
        e.datum.mus.y = L;
        dispensator_tractare(d, &e);
        CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_motus(d)->captura, "tabula");
        e.genus = EVENTUS_MUS_MOTUS;
        e.datum.mus.x = LV;
        dispensator_tractare(d, &e);
        CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                           II);
    }
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(chorda_vacua(dispensator_motus(d)->captura));
    CREDO_AEQUALIS_I32(xar_numerus(dispensator_motus(d)->ictus_pendens),
                       ZEPHYRUM);
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(doc), II);   /* immotus */

    imprimere("\n--- 'p' instrumentum eligit (dominus = actio) ---\n");
    {
        character ignotum[] = "ignotum";
        insula_scriptorem_ponere(repo, chorda_ex_literis("instrumentum.eligere",
                                                         piscina));
        /* per manum: instrumentum aliud, deinde 'p' */
        (vacuum)ignotum;
    }
    CREDO_VERUM(manus_ludus_clavem(m, 'p', ZEPHYRUM));
    {
        chorda* a2;
        a2 = insula_attributum(repo, INSULA_EPHEMERA, "instrumentum");
        CREDO_NON_NIHIL(a2);
        CREDO_CHORDA_AEQUALIS_LITERIS(*a2, "penicillus");
    }
    CREDO_VERUM(insula_restituere(repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
Tidy when typing: the `'p'` section's scratch block (`ignotum`, the manual `insula_scriptorem_ponere`) is unnecessary once T6's dispensator sets the scriptor — delete it and keep only the `clavem('p')` + the attribute read; to make the assertion non-vacuous, first set `instrumentum` to something else through a mutator under scriptor `instrumentum.eligere` (or start the ephemera island with `instrumentum="nihil"` and assert the tabula has NO action before the keypress and `penicillus.ictus` after a recompose). `#include <string.h>` for `memset`. `manus_ludus_premere_ad(m, XL, XL)` on the tabula is a one-point stroke: cursor goes to II — that is intended (a click paints a dot, as MacPaint).

- [x] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh pictor_actiones` — Expected: FAIL, header not found.

- [x] **Step 3: Write header and implementation**

`include/pictor_actiones.h`:
```c
/* pictor_actiones.h - tractatores pictoris (P3: penicillus.ictus,
 * instrumentum.eligere)
 *
 * <tractator/>: nullum I/O; scripturae per portas solas (insulae:
 * mutare_*, documentum: pictor_documentum_actum - porta documenti).
 * Una actio per INTENTIONEM, phasis ex genere eventus (spec §5.2).
 */

#ifndef PICTOR_ACTIONES_H
#define PICTOR_ACTIONES_H

/* <aedilis corpus="lib/pictor_actiones.c"/> */

#include "latina.h"
#include "actio.h"
#include "pictor_documentum.h"

nomen structura {
    PictorDocumentum* doc;
} PictorActiones;

vacuum
pictor_actiones_registrare (
    ActioRegistrum* reg,
    PictorActiones* ctx);

/* <tractator/> */
b32
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* nodus,
     constans Eventus* ev,
               vacuum* ctx);

/* <tractator/> */
b32
pictor_instrumentum_eligere (
    InsulaRepositorium* repo,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* nodus,
     constans Eventus* ev,
               vacuum* ctx);

#endif /* PICTOR_ACTIONES_H */
```
`lib/pictor_actiones.c`:
```c
/* pictor_actiones.c - tractatores pictoris */

#include "pictor_actiones.h"
#include "xar.h"

/* ==================================================
 * Mutatores motus
 * ================================================== */

interior vacuum
puncta_vacare (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    xar_vacare(motus->ictus_pendens);
}

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes  = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes = *(Punctum*)ctx;
}

/* ==================================================
 * Mutator insulae
 * ================================================== */

hic_manens character litterae_penicillus[] = "penicillus";

interior vacuum
instrumentum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "instrumentum",
                             (constans character*)ctx);
}

/* ==================================================
 * Actum ictus
 * ================================================== */

interior s32
attributum_s32 (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_attributum(repo, INSULA_EPHEMERA, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
ictum_scribere (
    InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* p)
{
    chorda   s;
    Punctum* q;
    i32      i;
    i32      n;

    s = chorda_ex_literis("<ictus instrumentum=\"penicillus\" color=\"", p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(repo, "color_primus", ZEPHYRUM), p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\" magnitudo=\"", p), p);
    s = chorda_concatenare(s,
        chorda_ex_s32(attributum_s32(repo, "magnitudo", I), p), p);
    s = chorda_concatenare(s, chorda_ex_literis("\">", p), p);
    n = xar_numerus(motus->ictus_pendens);
    per (i = ZEPHYRUM; i < n; i++)
    {
        q = (Punctum*)xar_obtinere(motus->ictus_pendens, i);
        s = chorda_concatenare(s, chorda_ex_literis("<punctum x=\"", p), p);
        s = chorda_concatenare(s, chorda_ex_s32(q->x, p), p);
        s = chorda_concatenare(s, chorda_ex_literis("\" y=\"", p), p);
        s = chorda_concatenare(s, chorda_ex_s32(q->y, p), p);
        s = chorda_concatenare(s, chorda_ex_literis("\"/>", p), p);
    }
    s = chorda_concatenare(s, chorda_ex_literis("</ictus>", p), p);
    redde s;
}

/* ==================================================
 * Tractatores
 * ================================================== */

/* <tractator/> */
b32
pictor_penicillus_ictus (
    InsulaRepositorium* repo,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* nodus,
     constans Eventus* ev,
               vacuum* ctx)
{
    PictorActiones* pa;
           Punctum  p;

    pa = (PictorActiones*)ctx;
    si (!repo || !motus || !destinatio || !nodus || !ev || !pa)
    {
        redde FALSUM;
    }
    p = destinatio->punctum_locale;
    commutatio (ev->genus)
    {
        casus EVENTUS_MUS_DEPRESSUS:
            motus_captura_ponere(motus, nodus->id);
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_MOTUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            mutare_motum(motus, punctum_addere, &p, ev->tempus);
            redde VERUM;
        casus EVENTUS_MUS_LIBERATUS:
            si (chorda_vacua(motus->captura))
            {
                redde FALSUM;
            }
            pictor_documentum_actum(pa->doc,
                ictum_scribere(repo, motus, pa->doc->piscina));
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            motus->sordida = FALSUM;   /* ictus finitus ephemera non tangit */
            motus_captura_tollere(motus);
            redde VERUM;
        casus EVENTUS_CLAVIS_DEPRESSUS:
            si (chorda_vacua(motus->captura)
                || ev->datum.clavis.typus != (character)XXVII)
            {
                redde FALSUM;
            }
            mutare_motum(motus, puncta_vacare, NIHIL, ev->tempus);
            motus->sordida = FALSUM;
            motus_captura_tollere(motus);
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* <tractator/> */
b32
pictor_instrumentum_eligere (
    InsulaRepositorium* repo,
                Motus* motus,
   constans Destinatio* destinatio,
            Componens* nodus,
     constans Eventus* ev,
               vacuum* ctx)
{
    (vacuum)motus;
    (vacuum)destinatio;
    (vacuum)nodus;
    (vacuum)ctx;
    si (!repo || !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.typus == 'p')
    {
        redde mutare_ephemera(repo, instrumentum_ponere, litterae_penicillus);
    }
    redde FALSUM;
}

vacuum
pictor_actiones_registrare (
    ActioRegistrum* reg,
    PictorActiones* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    actio_registrare(reg, "penicillus.ictus", pictor_penicillus_ictus, ctx);
    actio_registrare(reg, "instrumentum.eligere",
                     pictor_instrumentum_eligere, ctx);
}
```
`hic_manens character litterae_penicillus[]` is a file-scope static that is WRITTEN never (only read as a `constans character*` through the ctx cast) — L7 forbids file-scope MUTABLE statics; make it `hic_manens constans character litterae_penicillus[]` and cast it as `(vacuum*)(uintptr)`? No — `-Wcast-qual` refuses that. Keep it non-const and never write it; the worklog names this as the accepted pattern (same as `ludus_toy.h`), and T12's L7 codex treats a `character[]` initialised from a literal and never assigned as immutable-by-use. `pa->doc->piscina` is the document's arena, where the actum text belongs.

- [x] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh pictor_actiones` — Expected: PASS.

- [x] **Step 5: Plant, worklog, commit**

Plants (RED): (a) in `ictum_scribere`, `per (i = ZEPHYRUM; i < n; i++)` → `i < ZEPHYRUM` — red at `numerare_puncta == III`; (b) in the `LIBERATUS` arm, drop `motus_captura_tollere` — red at `chorda_vacua(captura)`. Green on revert.

Worklog `lib/pictor_actiones.worklog.md`: the stroke as one handler; the document gate vs the island gates; why `sordida` is cleared on release; Escape's contract (no acta); what P4/P5 add (impletio.punctum, color.capere, selectio.*, revocare/reficere, dialogus.*).

Commit with `portae=[('radix','pictor_actiones'), ('radix','pictor_documentum')]`.

---

### Task 9: `pictor_figurae` — `figura_tabulae` and `figura_tituli`, the first pictor pixels

**Files:**
- Create: `include/pictor_figurae.h`, `lib/pictor_figurae.c`, `lib/pictor_figurae.worklog.md`
- Test: `probationes/probatio_pictor_figurae.c`; specimen `probationes/pictor/specimina/pictor_prima.png` (born on first run)

**Interfaces:**
- Consumes: `figura.h` (T2), `delineare_mandata.h` (T3), `pictor_documentum.h` (T4), `pictor_componentia.h` (T7), `thema.h`.
- Produces: `PictorFigurae { PictorDocumentum* doc; }` (ctx for the registry AND for the image source); `vacuum pictor_figurae_registrare(FiguraRegistrum* reg, i32 thema, PictorFigurae* ctx)` registering `PARTES_TABULA` → `figura_tabulae` and `PARTES_TITULUS` → `figura_tituli`; `constans Imago* pictor_imago_fons(chorda provenientia, vacuum* ctx)` — the `ImagoFons` for the rasterizer: provenientia == the document's current sigillum hex → `pictor_documentum_proiectio`, else NIHIL.
- **`figura_tabulae`** (`<purus/>`, spec §5.3): `mandata_imago(m, sigillum_hex, (0,0,w,h))` — the canvas is one image whose IDENTITY is the projection's sigillum (content-addressed: the same acta always name the same picture); the pending stroke as `LINEA` between consecutive `c->puncta` in `COLOR_ACCENT_PRIMARY` (the preview colour; the committed stroke's colour comes from the document — the figura cannot read the islands, and that is the point); a 3×3 `RECTANGULUM` cursor at the last point. The marquee overlay is P5.
- **`figura_tituli`**: `mandata_rectangulum` plenum `COLOR_BACKGROUND` over the fines, then `mandata_textus(m, II, II, c->titulus, ZEPHYRUM, COLOR_TEXT)`.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_pictor_figurae.c`:
```c
/* probatio_pictor_figurae.c - gradus VI (figurae pictoris -> mandata)
 * et VII (specimen pictor_prima): arbor vera, documentum verum */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda.h"
#include "color.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "insula.h"
#include "motus.h"
#include "componens.h"
#include "mandatum.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "specimen.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_figurae.h"
#include "credo.h"
#include <stdio.h>

interior vacuum
punctum_addere (
     Motus* motus,
    vacuum* ctx)
{
    Punctum* sedes;

    sedes  = (Punctum*)xar_addere(motus->ictus_pendens);
    *sedes = *(Punctum*)ctx;
}

interior vacuum
pan_ponere (
     Motus* motus,
    vacuum* ctx)
{
    (vacuum)ctx;
    motus->pan.x = -V;
    motus->pan.y = III;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
                  Motus  motus;
       PictorCompositio  cfg;
          PictorFigurae  pf;
        FiguraRegistrum* reg;
              Componens* arbor;
                Mandata* m;
               Mandatum* x;
        TabulaPixelorum* t;
                  Imago  captura;
        SpecimenFructus  sf;
                Punctum  p;
                    i32  i;
                    i32  n;
                    i32  imagines;
                    i32  lineae;
                    i32  textus;
                 chorda  sigillum;

    piscina = piscina_generare_dynamicum("probatio_pictor_figurae",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_pictor_figurae");
    doc = pictor_documentum_creare(piscina, intern, vol, XL, XXX, LXIV);
    /* actum: linea nigra horizontalis y=2, x 0..39 */
    pictor_documentum_actum(doc, chorda_ex_literis(
        "<ictus instrumentum=\"penicillus\" color=\"0\" magnitudo=\"1\">"
        "<punctum x=\"0\" y=\"2\"/><punctum x=\"39\" y=\"2\"/></ictus>",
        piscina));
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"40\" altitudo=\"30\"/>",
        "<ephemera instrumentum=\"penicillus\" zoom=\"1\"/>");
    motus_initiare(&motus, piscina);
    mutare_motum(&motus, pan_ponere, NIHIL, M);
    p.x = X;
    p.y = X;
    mutare_motum(&motus, punctum_addere, &p, M);
    p.x = XX;
    p.y = XV;
    mutare_motum(&motus, punctum_addere, &p, M);
    cfg.fenestra_latitudo = LXIV;
    cfg.fenestra_altitudo = XLVIII;
    cfg.status_altitudo   = XII;
    arbor = pictor_componere(repo, &motus, piscina, intern, &cfg);
    CREDO_NON_NIHIL(arbor);

    imprimere("\n--- Gradus VI: figurae -> mandata ---\n");
    pf.doc = doc;
    reg = figura_registrum_creare(piscina);
    pictor_figurae_registrare(reg, ZEPHYRUM, &pf);
    m = mandata_creare(piscina, intern);
    pingere(arbor, reg, ZEPHYRUM, m);
    sigillum = pictor_documentum_sigillum_hex(doc, piscina);
    imagines = ZEPHYRUM;
    lineae   = ZEPHYRUM;
    textus   = ZEPHYRUM;
    n = mandata_numerus(m);
    per (i = ZEPHYRUM; i < n; i++)
    {
        x = mandata_obtinere(m, i);
        si (x->genus == MANDATUM_IMAGO)
        {
            imagines++;
            CREDO_VERUM(chorda_aequalis(x->textus, sigillum));
            CREDO_AEQUALIS_S32(x->fines.latitudo, XL);
        }
        si (x->genus == MANDATUM_LINEA)
        {
            lineae++;
        }
        si (x->genus == MANDATUM_TEXTUS)
        {
            textus++;
            CREDO_CHORDA_AEQUALIS_LITERIS(x->textus, "penicillus");
        }
    }
    CREDO_AEQUALIS_I32(imagines, I);
    CREDO_AEQUALIS_I32(lineae, I);          /* II puncta -> I linea */
    CREDO_AEQUALIS_I32(textus, I);

    imprimere("\n--- Fons imaginum: sigillum -> proiectio ---\n");
    CREDO_VERUM(pictor_imago_fons(sigillum, &pf)
                == pictor_documentum_proiectio(doc));
    CREDO_NIHIL(pictor_imago_fons(chorda_ex_literis("alienum", piscina), &pf));

    imprimere("\n--- Gradus VII: pixela, deinde specimen ---\n");
    t = tabula_pixelorum_creare_nuda(piscina, LXIV, XLVIII);
    tabula_pixelorum_vacare(t, color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(m, t, pictor_imago_fons, &pf);
    /* linea documenti y=2 sub pan (-5,3): schirmo y=5, x 0..34 nigra;
     * x=35..: fundus (documentum album ibi absens? non: documentum
     * 40 latum, pan -5 -> schirmo -5..34) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XX, V),
        color_ad_pixelum(thema_color_ex_indice_colorationis((i8)PALETTE_BLACK)));
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XX, VI),
        color_ad_pixelum(thema_color_ex_indice_colorationis((i8)PALETTE_WHITE)));
    /* extra documentum (x=40): fundus thematis */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, XL, X),
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    captura = imago_ex_tabula(t);
    sf = specimen_iudicare(&captura, "pictor_prima",
        specimen_regula_solita("probationes/pictor/specimina"), piscina);
    si (sf.sententia != SPECIMEN_CONGRUIT)
    {
        imprimere("SPECIMEN %s: %.*s\n",
                  specimen_sententia_nomen(sf.sententia),
                  (int)sf.causa.mensura, sf.causa.datum);
    }
    CREDO_VERUM(sf.sententia == SPECIMEN_CONGRUIT);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
Arithmetic to hold in mind: the document is 40×30 at prospectus origin (0,0) with pan (-5,3) and scala 1, so document pixel (x,y) lands on screen (x-5, y+3); the black line at y=2 is at screen y=5 for x from -5 to 34, so (20,5) is black and (20,6) white; screen x=40 is beyond the document (34) → the theme background. The prospectus clips at (0,0,64,36).

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh pictor_figurae` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/pictor_figurae.h`:
```c
/* pictor_figurae.h - figurae pictoris (P3: tabula, titulus)
 *
 * <purus/>: figura arborem solam legit. Tabula = imago UNA cuius
 * identitas est sigillum proiectionis (contentu addressata) +
 * ictus pendens (lineae inter puncta componentis) + cursor. Fons
 * imaginum pro rasterizatore: sigillum -> proiectio documenti.
 */

#ifndef PICTOR_FIGURAE_H
#define PICTOR_FIGURAE_H

/* <aedilis corpus="lib/pictor_figurae.c"/> */

#include "latina.h"
#include "chorda.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "pictor_documentum.h"

nomen structura {
    PictorDocumentum* doc;
} PictorFigurae;

vacuum
pictor_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      PictorFigurae* ctx);

/* ImagoFons: provenientia == sigillum hex documenti -> proiectio */
constans Imago*
pictor_imago_fons (
    chorda provenientia,
    vacuum* ctx);

/* <purus/> */
vacuum
figura_tabulae (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx);

/* <purus/> */
vacuum
figura_tituli (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx);

#endif /* PICTOR_FIGURAE_H */
```
`lib/pictor_figurae.c`:
```c
/* pictor_figurae.c - figurae pictoris */

#include "pictor_figurae.h"
#include "thema.h"

interior ColorMandati
color_thematis (
    ColorThema c)
{
    ColorMandati cm;

    cm.genus = COLOR_MANDATI_THEMA;
    cm.valor = (i32)c;
    redde cm;
}

/* <purus/> */
vacuum
figura_tabulae (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx)
{
    PictorFigurae* pf;
            Fines  f;
              i32  i;

    (vacuum)thema;
    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde;
    }
    f.x        = ZEPHYRUM;
    f.y        = ZEPHYRUM;
    f.latitudo = c->fines.latitudo;
    f.altitudo = c->fines.altitudo;
    mandata_imago(m, pictor_documentum_sigillum_hex(pf->doc, m->piscina), f);
    per (i = I; i < c->numerus_punctorum; i++)
    {
        mandata_linea(m, c->puncta[i - I], c->puncta[i], I,
                      color_thematis(COLOR_ACCENT_PRIMARY));
    }
    si (c->numerus_punctorum > ZEPHYRUM)
    {
        f.x        = c->puncta[c->numerus_punctorum - I].x - I;
        f.y        = c->puncta[c->numerus_punctorum - I].y - I;
        f.latitudo = III;
        f.altitudo = III;
        mandata_rectangulum(m, f, color_thematis(COLOR_CURSOR), FALSUM);
    }
}

/* <purus/> */
vacuum
figura_tituli (
    constans Componens* c,
              Mandata* m,
                  i32  thema,
              vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    f.x        = ZEPHYRUM;
    f.y        = ZEPHYRUM;
    f.latitudo = c->fines.latitudo;
    f.altitudo = c->fines.altitudo;
    mandata_rectangulum(m, f, color_thematis(COLOR_BACKGROUND), VERUM);
    mandata_textus(m, II, II, c->titulus, ZEPHYRUM,
                   color_thematis(COLOR_TEXT));
}

constans Imago*
pictor_imago_fons (
    chorda provenientia,
    vacuum* ctx)
{
    PictorFigurae* pf;

    pf = (PictorFigurae*)ctx;
    si (!pf || !pf->doc)
    {
        redde NIHIL;
    }
    si (chorda_aequalis(provenientia,
            pictor_documentum_sigillum_hex(pf->doc, pf->doc->piscina)))
    {
        redde pictor_documentum_proiectio(pf->doc);
    }
    redde NIHIL;
}

vacuum
pictor_figurae_registrare (
    FiguraRegistrum* reg,
                i32  thema,
      PictorFigurae* ctx)
{
    si (!reg || !ctx)
    {
        redde;
    }
    figura_registrare(reg, PARTES_TABULA, thema, figura_tabulae, ctx);
    figura_registrare(reg, PARTES_TITULUS, thema, figura_tituli, ctx);
}
```
`Mandata` exposes `piscina` (Plan 1 T2 struct) — the sigillum chorda is allocated in the mandata's arena, which lives as long as the frame. `pictor_imago_fons` formats the hex on every call: 64 bytes per image mandatum per frame — fine; the worklog notes a cached hex on the document as the obvious later saving.

- [ ] **Step 4: First run — the specimen is born**

Run: `./compile_tests.sh pictor_figurae` — Expected: tier 6 and pixel assertions PASS; specimen `EXEMPLAR_ABEST` names `probationes/pictor/specimina/pictor_prima.novum.png`. Look at it: a white 40×30 sheet shifted (-5,+3) on the theme background with a black line near its top, an accent-coloured diagonal from (5,13) to (15,18) with a small cursor box, "penicillus" in the status strip. Promote by `mv` to `pictor_prima.png`. Second run: PASS.

- [ ] **Step 5: Plant, worklog, commit**

Plants (RED): (a) `figura_tabulae` emits no image (comment out `mandata_imago`) — red at `imagines == I` and at the black-pixel assertion; (b) `pictor_imago_fons` compares against `"x"` — red at the fons assertion and the pixel. Green on revert.

Worklog `lib/pictor_figurae.worklog.md`: content-addressed canvas identity; the preview colour compromise (figura cannot read islands) and its P4 fix (a `color` field on the tabula componens filled by `componere`); cursor and marquee status; `PARTES_TITULUS` for the status line (a role, not a type).

Commit with `portae=[('radix','pictor_figurae'), ('radix','pictor_componentia'), ('radix','figura'), ('radix','delineare_mandata')]`; files include the promoted `pictor_prima.png`.

---

### Task 10: `ludus_fenestra` — the window glue, the frame, the numbers

**Files:**
- Create: `include/ludus_fenestra.h`, `lib/ludus_fenestra.c`, `lib/ludus_fenestra.worklog.md`
- Test: `probationes/probatio_ludus_fenestra.c` (headless: the frame function against a bare pixel table; the window loop itself is exercised by T11's smoke)

**Interfaces:**
- Consumes: `fenestra.h` (`fenestra_creare`, `fenestra_debet_claudere`, `fenestra_perscrutari_eventus`, `fenestra_obtinere_eventus`, `fenestra_creare_tabulam_pixelorum`, `fenestra_praesentare_pixela`, `fenestra_tempus_ms`, `fenestra_destruere`), `Dispensator` (T5), `figura.h` + `pingere` (T2), `delineare_mandata.h` (T3), `thema.h`.
- Produces: `LudusFenestra { Dispensator* d; FiguraRegistrum* figurae; i32 thema; ImagoFons fons; vacuum* fons_ctx; TabulaPixelorum* tabula; Piscina* piscina_quadri; Mandata* mandata; LudusMensurae mensurae; }`, `LudusMensurae { i32 quadra; s64 ms_compositionis; s64 ms_pingendi; s64 ms_delineandi; s64 ms_quadri_maximum; }`; `ludus_fenestra_creare(piscina, d, figurae, thema, fons, fons_ctx, TabulaPixelorum*)`; `vacuum ludus_quadrum(LudusFenestra*, s64 nunc)` — ONE frame: `dispensator_pulsare(d, nunc)` (quiet seat + recompose), `pingere` the last tree into fresh `Mandata` in the per-frame piscina, `delineare_mandata` into the table; timings accumulated (the double-buffer trigger from brainstorm §XVI §3 is decided from `ms_delineandi` vs the rest); `s32 ludus_fenestra_currere(LudusFenestra*, Fenestra*, i32 quadra_maxima)` — the loop: poll, drain events into `dispensator_tractare` (the platform stamps `tempus`; a `ZEPHYRUM` tempus is stamped here from `fenestra_tempus_ms` as a guard), `ludus_quadrum`, present; returns after `EVENTUS_CLAUDERE` or after `quadra_maxima` frames (`-fumus`); prints the `LudusMensurae` summary on exit.
- **Not in this task (named pull):** wheel events. `lib/fenestra_macos.m`'s `scrollWheel:` is EMPTY today (verified 2026-09-05) — no `EVENTUS_MUS_ROTULA` is ever emitted, and `Eventus.datum.rotula` carries no position. Zoom-at-cursor (P4, `zoom.ponere`) pulls: `x`/`y` members on `datum.rotula` (Editio on `fenestra.h`), emission in the `.m` with the location, and `destinatio` treating a positioned wheel geometrically. Record the pull in the worklog; do not start it here.
- Window pixel table: `fenestra_creare_tabulam_pixelorum(piscina, fenestra, altitudo_fixa)` with `altitudo_fixa` = the window's content height, so `scala` is 1 and mouse coordinates and table coordinates coincide (a scaled table would need the mouse divided by `scala` before dispatch — noted in the worklog as the retina question).

- [ ] **Step 1: Write the failing test**

`probationes/probatio_ludus_fenestra.c`:
```c
/* probatio_ludus_fenestra.c - quadrum unum sine fenestra: pulsus ->
 * pingere -> delineare; mensurae */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "color.h"
#include "thema.h"
#include "fenestra.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
              ToyStatus  toy;
            Dispensator* d;
        FiguraRegistrum* figurae;
        TabulaPixelorum* t;
          LudusFenestra* lf;
                Eventus  e;
                    i32  n0;

    piscina = piscina_generare_dynamicum("probatio_ludus_fenestra",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg  = actio_registrum_creare(piscina, intern);
    memset(&toy, ZEPHYRUM, magnitudo(ToyStatus));
    toy_registrare(reg, &toy);
    d = dispensator_creare(piscina, intern, repo, reg, toy_componere,
                           &toy, CCC);
    figurae = figura_registrum_creare(piscina);
    CREDO_VERUM(figura_registrare(figurae, PARTES_BOTTONE, ZEPHYRUM,
                                  figura_finium, NIHIL));
    t  = tabula_pixelorum_creare_nuda(piscina, CC, C);
    lf = ludus_fenestra_creare(piscina, d, figurae, ZEPHYRUM, NIHIL, NIHIL,
                               t);
    CREDO_NON_NIHIL(lf);

    imprimere("\n--- Quadrum: pulsus, pingere, delineare ---\n");
    n0 = toy.compositiones;
    ludus_quadrum(lf, M);
    CREDO_AEQUALIS_I32(lf->mensurae.quadra, I);
    CREDO_VERUM(toy.compositiones > n0);              /* pulsus recomposuit */
    CREDO_VERUM(mandata_numerus(lf->mandata) >= III); /* coetus x3 + rect */
    /* b1 (10,10,50,20) fines vacui colore COLOR_BORDER: angulus (10,10) */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, X, X),
        color_ad_pixelum(thema_color(COLOR_BORDER)));
    CREDO_VERUM(lf->mensurae.ms_quadri_maximum >= ZEPHYRUM);

    imprimere("\n--- Eventus sine tempore stampatur; quadrum alterum ---\n");
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus       = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x = XXXV;
    e.datum.mus.y = XX;
    ludus_fenestra_tractare(lf, &e, M + L);
    CREDO_NON_NIHIL(insula_attributum(repo, INSULA_EPHEMERA, "numerus"));
    ludus_quadrum(lf, M + C);
    CREDO_AEQUALIS_I32(lf->mensurae.quadra, II);
    /* piscina quadri vacatur: mandata nova, non accumulata */
    CREDO_VERUM(mandata_numerus(lf->mandata) < X);

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```
`ludus_fenestra_tractare(lf, ev, nunc)` is the event seat: stamps `tempus` if zero, then `dispensator_tractare`.

- [ ] **Step 2: Run to verify it fails**

Run: `./tools/compile_tests_fontes_generare.sh && ./compile_tests.sh ludus_fenestra` — Expected: FAIL, header not found.

- [ ] **Step 3: Write header and implementation**

`include/ludus_fenestra.h`:
```c
/* ludus_fenestra.h - glutinum fenestrae: eventus -> dispensator,
 * quadrum -> pingere -> delineare -> praesentare
 *
 * Unicum locus ubi ludus horologium tangit (fenestra_tempus_ms) et
 * unicus qui fenestram videt. Quadrum = pulsus (sedes quietis +
 * recompositio), pingere arboris ultimae in piscinam quadri,
 * rasterizatio. Mensurae per quadrum: compositio (in pulsu),
 * pingere, delineare - causa optionis status duplicati (brainstorm
 * XVI §3): si delineare quadrum dominatur, rasterizare quadrum N dum
 * quadrum N+1 dispensatur.
 *
 * Rotula: fenestra_macos.m scrollWheel VACUUS est (2026-09-05) -
 * tractio P4 (zoom ad cursorem).
 */

#ifndef LUDUS_FENESTRA_H
#define LUDUS_FENESTRA_H

/* <aedilis corpus="lib/ludus_fenestra.c"/> */

#include "latina.h"
#include "piscina.h"
#include "fenestra.h"
#include "dispensator.h"
#include "figura.h"
#include "mandatum.h"
#include "delineare_mandata.h"

nomen structura {
    i32 quadra;
    s64 ms_compositionis;     /* intra pulsum */
    s64 ms_pingendi;
    s64 ms_delineandi;
    s64 ms_quadri_maximum;
} LudusMensurae;

nomen structura {
        Dispensator* d;
    FiguraRegistrum* figurae;
                i32  thema;
          ImagoFons  fons;
            vacuum*  fons_ctx;
    TabulaPixelorum* tabula;
           Piscina*  piscina_quadri;
           Mandata*  mandata;        /* quadri ultimi */
      LudusMensurae  mensurae;
} LudusFenestra;

LudusFenestra*
ludus_fenestra_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
            vacuum*  fons_ctx,
    TabulaPixelorum* tabula);

/* eventus in dispensatorem; tempus ZEPHYRUM stampatur 'nunc' */
vacuum
ludus_fenestra_tractare (
     LudusFenestra* lf,
    constans Eventus* ev,
                s64  nunc);

/* quadrum unum: pulsus, pingere, delineare (non praesentat) */
vacuum
ludus_quadrum (
    LudusFenestra* lf,
              s64  nunc);

/* ansa vera: perscrutari, tractare, quadrum, praesentare; finis in
 * EVENTUS_CLAUDERE aut post quadra_maxima (> 0). Mensurae ad stdout. */
s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima);

#endif /* LUDUS_FENESTRA_H */
```
`lib/ludus_fenestra.c`:
```c
/* ludus_fenestra.c - glutinum fenestrae */

#include "ludus_fenestra.h"
#include "thema.h"
#include "color.h"

#include <stdio.h>
#include <string.h>

LudusFenestra*
ludus_fenestra_creare (
            Piscina* piscina,
        Dispensator* d,
    FiguraRegistrum* figurae,
                i32  thema,
          ImagoFons  fons,
            vacuum*  fons_ctx,
    TabulaPixelorum* tabula)
{
    LudusFenestra* lf;

    si (!piscina || !d || !figurae || !tabula)
    {
        redde NIHIL;
    }
    lf = (LudusFenestra*)piscina_allocare(piscina, magnitudo(LudusFenestra));
    si (!lf)
    {
        redde NIHIL;
    }
    memset(lf, ZEPHYRUM, magnitudo(LudusFenestra));
    lf->d              = d;
    lf->figurae        = figurae;
    lf->thema          = thema;
    lf->fons           = fons;
    lf->fons_ctx       = fons_ctx;
    lf->tabula         = tabula;
    lf->piscina_quadri = piscina_generare_dynamicum("ludus_quadrum",
                                                    LXIV * M);
    si (!lf->piscina_quadri)
    {
        redde NIHIL;
    }
    redde lf;
}

vacuum
ludus_fenestra_tractare (
     LudusFenestra* lf,
    constans Eventus* ev,
                s64  nunc)
{
    Eventus e;

    si (!lf || !ev)
    {
        redde;
    }
    e = *ev;
    si (e.tempus == ZEPHYRUM)
    {
        e.tempus = nunc;
    }
    dispensator_tractare(lf->d, &e);
}

vacuum
ludus_quadrum (
    LudusFenestra* lf,
              s64  nunc)
{
    s64 t0;
    s64 t1;
    s64 t2;
    s64 t3;

    si (!lf)
    {
        redde;
    }
    t0 = fenestra_tempus_ms();
    dispensator_pulsare(lf->d, nunc);
    t1 = fenestra_tempus_ms();
    piscina_vacare(lf->piscina_quadri);
    lf->mandata = mandata_creare(lf->piscina_quadri, lf->d->intern);
    pingere(dispensator_arbor(lf->d), lf->figurae, lf->thema, lf->mandata);
    t2 = fenestra_tempus_ms();
    tabula_pixelorum_vacare(lf->tabula,
        color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
    delineare_mandata(lf->mandata, lf->tabula, lf->fons, lf->fons_ctx);
    t3 = fenestra_tempus_ms();
    lf->mensurae.quadra++;
    lf->mensurae.ms_compositionis += t1 - t0;
    lf->mensurae.ms_pingendi      += t2 - t1;
    lf->mensurae.ms_delineandi    += t3 - t2;
    si (t3 - t0 > lf->mensurae.ms_quadri_maximum)
    {
        lf->mensurae.ms_quadri_maximum = t3 - t0;
    }
}

s32
ludus_fenestra_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima)
{
    Eventus e;
        s64 nunc;
        b32 claudendum;

    si (!lf || !fenestra)
    {
        redde I;
    }
    claudendum = FALSUM;
    dum (!claudendum && !fenestra_debet_claudere(fenestra))
    {
        fenestra_perscrutari_eventus(fenestra);
        nunc = fenestra_tempus_ms();
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
        }
        ludus_quadrum(lf, nunc);
        fenestra_praesentare_pixela(fenestra, lf->tabula);
        si (quadra_maxima > ZEPHYRUM && lf->mensurae.quadra >= quadra_maxima)
        {
            claudendum = VERUM;
        }
    }
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum);
    redde ZEPHYRUM;
}
```
`Dispensator` exposes `intern` (Plan 1 T10 struct). The per-frame piscina is vacated at the top of every frame: mandata never accumulate. `imprimere` = `printf` in the house; the numbers line is what T11's smoke greps.

- [ ] **Step 4: Run to verify it passes**

Run: `./compile_tests.sh ludus_fenestra` — Expected: PASS.

- [ ] **Step 5: Plant, worklog, commit**

Plant (RED): in `ludus_quadrum`, skip `piscina_vacare` and reuse `lf->mandata` without recreating — the second frame's count doubles: red at `< X`. Green on revert.

Worklog `lib/ludus_fenestra.worklog.md`: the frame; the measurement fields and what decides the double-buffer option; the wheel pull with the empty `scrollWheel:` fact; the scale-1 table decision and the retina question.

Commit with `portae=[('radix','ludus_fenestra'), ('radix','dispensator')]`.

---

### Task 11: `apps/pictor` — the binary, the smoke, the first bake

**Files:**
- Create: `apps/pictor/pictor.c`, `apps/pictor/pictor.sh`, `apps/pictor/fumus.sh`, `apps/pictor/pictor.worklog.md`
- Modify: `project-specs/pictor-spec.md` §10 (the first-bake numbers; the names sealed or renamed)

**Interfaces:**
- Consumes: everything above; `volumen.h` (`volumen_aperire_aut_creare`, `volumen_temporarium`), `filum.h` (`filum_legere_totum` for the canons + domini — from disk in P3; capsula is the P4 pull when icons arrive), `argumenta.h` if the house has one (check `include/argumenta.h`; else a two-flag hand parse: `-fumus`, `-volumen <via>`).
- Produces: `bin/pictor`; `./apps/pictor/pictor.sh [-fumus] [-volumen <via>]` (the app convention: `bin/aedilis apps/pictor/pictor.c` derives the closure and emits `build/aedilis/pictor/struere.sh`, which is run, then the binary is copied with `rm -f` first — the macOS SIGKILL rule); `-fumus` runs 30 frames in a real window with a temporary volumen and exits 0 printing the `ludus:` numbers line; `./apps/pictor/fumus.sh` asserts the line and the exit code.
- **The first bake's numbers** (spec §8 P3, §10): closure size = `bin/aedilis apps/pictor/pictor.c --partes | wc -l` (translation units) and `wc -c` of the concatenated closure; build time = `time bash build/aedilis/pictor/struere.sh` cold; binary size = `wc -c bin/pictor`; and the single-file amalgam via `./tools/amalgama_ligare.sh pictor apps/pictor/pictor.c` → `build/amalgamata/pictor/pictor.{h,c}` sizes and whether it compiles standalone under the house flags (its own gate does that). All five go into spec §10 (via `silva.Prosa` on the `## 10.` section) and the worklog.

- [ ] **Step 1: Write the smoke (the failing test)**

`apps/pictor/fumus.sh`:
```bash
#!/bin/bash
# apps/pictor/fumus.sh - fumus pictoris: fenestra vera, XXX quadra,
# volumen temporarium, exitus 0, linea mensurarum. Nullus cursor hoc
# verrit (apps/ extra probationes/); vocatur manu et a T11.
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
echo "=== FUMUS PICTORIS ==="
EXITUS=$(./apps/pictor/pictor.sh -fumus 2>&1)
CODEX=$?
echo "$EXITUS"
[ $CODEX -eq 0 ] || { echo "FUMUS FRACTUS (codex $CODEX)"; exit 1; }
echo "$EXITUS" | grep -q "ludus: quadra=30" || { echo "FRACTUM: quadra"; exit 1; }
echo "FUMUS PLENUS"
```
`apps/pictor/pictor.sh`:
```bash
#!/bin/bash
# apps/pictor/pictor.sh - involucrum tenue: aedilis + struere + exec
# (conventio apps/: constructio tota per aedilem derivata; capsula
# nulla in P3 - fons in codice, canones e disco)
#
# Usus: ./apps/pictor/pictor.sh [-fumus] [-volumen <via>]
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 1
[ -x bin/aedilis ] || ./tools/aedilis_struere.sh >&2 || exit 1
./bin/aedilis apps/pictor/pictor.c >&2 || exit 1
bash build/aedilis/pictor/struere.sh >&2 || exit 1
# rm ante cp: macOS binarium exsistens superscriptum SIGKILL dat
mkdir -p bin && rm -f bin/pictor && cp build/aedilis/pictor/pictor bin/pictor
exec bin/pictor "$@"
```
Run `./apps/pictor/fumus.sh` — Expected: FAIL (`pictor.c` absent; aedilis refuses).

- [ ] **Step 2: Write the binary**

`apps/pictor/pictor.c`:
```c
/* pictor.c - pictor: editor rasterum (P3: tabula + penicillus)
 *
 * Catena: volumen -> documentum; canones + domini e disco; insulae;
 * registra (actiones, figurae); dispensator (componere pictoris);
 * fenestra; ludus_fenestra_currere. -fumus: volumen temporarium,
 * XXX quadra, exitus.
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "volumen.h"
#include "fenestra.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"
#include "pictor_documentum.h"
#include "pictor_componentia.h"
#include "pictor_actiones.h"
#include "pictor_figurae.h"
#include <stdio.h>
#include <string.h>

#define PICTOR_LATITUDO   DCXL
#define PICTOR_ALTITUDO   CDLXXX
#define STATUS_ALTITUDO   XII
#define DOC_LATITUDO      CCCXX
#define DOC_ALTITUDO      CC
#define QUADRA_FUMI       XXX

interior Canon*
canonem_legere (
    Piscina* p,
    InternamentumChorda* in,
    constans character* via)
{
    chorda fons;
    chorda causa;
    Canon* c;

    fons = filum_legere_totum(via, p);
    si (fons.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "pictor: canon abest: %s\n", via);
        redde NIHIL;
    }
    c = canon_legere(fons, p, in, &causa);
    si (!c)
    {
        fprintf(stderr, "pictor: canon malus %s: %.*s\n", via,
                (int)causa.mensura, causa.datum);
    }
    redde c;
}

s32 principale (
    s32 argc,
    character** argv)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* vol;
       PictorDocumentum* doc;
     InsulaRepositorium* repo;
         ActioRegistrum* actiones;
         PictorActiones  actiones_ctx;
        FiguraRegistrum* figurae;
          PictorFigurae  figurae_ctx;
       PictorCompositio  compositio;
            Dispensator* d;
               Fenestra* fenestra;
        TabulaPixelorum* tabula;
          LudusFenestra* lf;
   FenestraConfiguratio  cfg;
                 chorda  domini;
           StmlResultus  res;
                    b32  fumus;
     constans character* via_voluminis;
                    s32  i;
                    s32  exitus;
                 chorda  ephemera_initialis;
                 chorda  durabilis_initialis;

    fumus         = FALSUM;
    via_voluminis = NIHIL;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-fumus") == ZEPHYRUM)
        {
            fumus = VERUM;
        }
        alioquin si (strcmp(argv[i], "-volumen") == ZEPHYRUM && i + I < argc)
        {
            i++;
            via_voluminis = argv[i];
        }
    }

    piscina = piscina_generare_dynamicum("pictor", IV * M * M);
    si (!piscina)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    thema_initiare();

    /* documentum */
    si (fumus || !via_voluminis)
    {
        vol = fumus ? volumen_temporarium(piscina, "pictor_fumus")
                    : volumen_aperire_aut_creare(piscina,
                                                 "pictor.volumen");
    }
    alioquin
    {
        vol = volumen_aperire_aut_creare(piscina, via_voluminis);
    }
    si (!vol)
    {
        fprintf(stderr, "pictor: volumen aperiri non potuit\n");
        redde I;
    }
    doc = pictor_documentum_aperire(piscina, intern, vol);
    si (!doc)
    {
        doc = pictor_documentum_creare(piscina, intern, vol, DOC_LATITUDO,
                                       DOC_ALTITUDO, LXIV);
    }
    si (!doc)
    {
        fprintf(stderr, "pictor: documentum\n");
        redde I;
    }

    /* insulae + canones + domini */
    durabilis_initialis = chorda_ex_literis("<documentum latitudo=\"", piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)doc->latitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\" altitudo=\"", piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_s32((s32)doc->altitudo, piscina), piscina);
    durabilis_initialis = chorda_concatenare(durabilis_initialis,
        chorda_ex_literis("\"/>", piscina), piscina);
    ephemera_initialis = chorda_ex_literis(
        "<ephemera instrumentum=\"penicillus\" color_primus=\"0\""
        " color_secundus=\"5\" magnitudo=\"1\" zoom=\"1\""
        " focus=\"tabula\"/>", piscina);
    repo = insula_repositorium_creare(piscina, intern,
        chorda_ut_cstr(durabilis_initialis, piscina),
        chorda_ut_cstr(ephemera_initialis, piscina));
    si (!repo)
    {
        redde I;
    }
    insula_ponere_canonem(repo, INSULA_DURABILIS,
        canonem_legere(piscina, intern, "apps/pictor/canones/durabilis.canon"));
    insula_ponere_canonem(repo, INSULA_EPHEMERA,
        canonem_legere(piscina, intern, "apps/pictor/canones/ephemera.canon"));
    domini = filum_legere_totum("apps/pictor/canones/domini.stml", piscina);
    res = stml_legere_ex_literis(chorda_ut_cstr(domini, piscina), piscina,
                                 intern);
    si (res.successus)
    {
        insula_dominos_legere(repo, INSULA_EPHEMERA, res.elementum_radix);
        insula_dominos_legere(repo, INSULA_DURABILIS, res.elementum_radix);
    }

    /* registra + dispensator */
    actiones         = actio_registrum_creare(piscina, intern);
    actiones_ctx.doc = doc;
    pictor_actiones_registrare(actiones, &actiones_ctx);
    figurae          = figura_registrum_creare(piscina);
    figurae_ctx.doc  = doc;
    pictor_figurae_registrare(figurae, ZEPHYRUM, &figurae_ctx);
    compositio.fenestra_latitudo = PICTOR_LATITUDO;
    compositio.fenestra_altitudo = PICTOR_ALTITUDO;
    compositio.status_altitudo   = STATUS_ALTITUDO;
    d = dispensator_creare(piscina, intern, repo, actiones, pictor_componere,
                           &compositio, CCC);
    si (!d)
    {
        redde I;
    }

    /* fenestra */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus  = "pictor";
    cfg.x        = C;
    cfg.y        = C;
    cfg.latitudo = PICTOR_LATITUDO;
    cfg.altitudo = PICTOR_ALTITUDO;
    cfg.vexilla  = FENESTRA_ORDINARIA;
    fenestra = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "pictor: fenestra\n");
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               PICTOR_ALTITUDO);
    lf = ludus_fenestra_creare(piscina, d, figurae, ZEPHYRUM,
                               pictor_imago_fons, &figurae_ctx, tabula);
    si (!tabula || !lf)
    {
        redde I;
    }
    exitus = ludus_fenestra_currere(lf, fenestra,
                                    fumus ? QUADRA_FUMI : ZEPHYRUM);
    fenestra_destruere(fenestra);
    volumen_claudere(vol);
    piscina_destruere(piscina);
    redde exitus;
}
```
The default volumen path `pictor.volumen` in the cwd is a P3 placeholder; P5's `documentum.aperire/servare` own paths. `FenestraConfiguratio.titulus` is `constans character*` (fenestra.h:232). `principale` is `#define principale main` (latina.h:43); copy the argument form from `apps/forum/forum.c:2011`.

- [ ] **Step 3: Build, smoke, bake, record**

Run `./apps/pictor/fumus.sh` — Expected: a window opens for 30 frames and closes; output contains `ludus: quadra=30 …`; `FUMUS PLENUS`. Then the bake numbers:
```bash
./bin/aedilis apps/pictor/pictor.c --partes | tee build/pictor_clausura.txt | wc -l
cut -f2 build/pictor_clausura.txt | xargs cat | wc -c
time bash build/aedilis/pictor/struere.sh
wc -c bin/pictor
./tools/amalgama_ligare.sh pictor apps/pictor/pictor.c && wc -c build/amalgamata/pictor/pictor.h build/amalgamata/pictor/pictor.c
```
(`--partes` prints one `O<tab>lib/x.c` line per translation unit — verified on forum: 116 lines — so the two commands above are right). Write the five numbers into spec §10 under a new bullet `**First-bake numbers (2026-09-DD, P3 T11):** …` with `silva.Prosa(...).sectio(incipit='10.')` + `paragraphum_addere`, and into `apps/pictor/pictor.worklog.md`. The `aedilis.stml` rule that adds `-framework Cocoa` for `fenestra_macos.m` (line ~42) is what makes the link work; confirm it appears in `build/aedilis/pictor/struere.sh`.

- [ ] **Step 4: Seal or rename**

Names introduced by this plan: `pictor_documentum`, `pictor_componentia`, `pictor_actiones`, `pictor_figurae`, `figura_tabulae`, `figura_tituli`, `prospectus`, `ludus_fenestra`, `tabula_pixelorum_creare_nuda`, `ludus_quadrum`, `ramus` (the undo branch actum), `domini`/`dominus`/`scriptor` (ownership), `limen` (the delivery boundary). Put the list to Fran; record the verdict in spec §10's sealed-names bullet and brainstorm §VIII.

- [ ] **Step 5: Commit**

`silva.commissio(msg, ['apps/pictor/pictor.c', 'apps/pictor/pictor.sh', 'apps/pictor/fumus.sh', 'apps/pictor/pictor.worklog.md', 'project-specs/pictor-spec.md', plan], portae=[('radix','ludus_fenestra'), ('radix','pictor_figurae'), ('radix','pictor_actiones')])` — the smoke is not a `portae` gate (it opens a window; `apps/` is outside every runner); run it by hand before the commit and say so in the message.

---

### Task 12: Codices batch 1 — the annotation vocabulary and L1, L2, L7, L9 (in the MAIN tree)

**Where:** `../rhubarb` (main), on `main` or a short branch there — silva/examen is shared with the other agent; coordinate before starting, commit there, then `git -C ../rhubarb-pictor merge main` and rerun the ludus gates on this branch. This is the one task of the plan that does not run in the worktree.

**Files:**
- Modify (main): `silva/fontes/silva_c89_semantica.h` (`ExamenCodex`: four lines before `EXAMEN_CODEX_NUMERUS`, after 90), `silva/fontes/silva_c89_semantica.c` (four `_codices[]` rows, judgment seats), the annotation vocabulary registration where `annotationes-stml-spec.md` chunks A–C put the existing anchors (read that spec's chunk C for the registration site; `bin/natura quaere <tag>` must answer LIBERUM or UMBRATUM-for-ludus for each new tag before it is taken; `bin/natura` is absent in a fresh worktree — `./tools/natura_struere.sh` builds it).
- Create (main): fixtures under `silva/probationes/fixa/examinis/` (the codex fixture directory; `annotationes/` beside it holds the anchor-vocabulary fixtures — put `ludus_purus.c` there): `l1_mendax.c`, `l2_impurus.c`, `l7_staticum.c`, `l9_mandata_in_destinatione.c`, each a minimal TU that fires exactly its codex once; and a `ludus_purus.c` that carries every anchor and fires nothing.
- Modify (this branch, after the merge): `probationes/probatio_insula.c` (the intentional lying write gets the house tolera anchor for codex 91), `lib/destinatio.c` (`<destinatio/>` anchors on `destinatio_geometrica` and `invenire`), every ludus `lib/*.c` (`/* <ludus/> */` as the first comment after the includes — the L7 scope anchor).

**Interfaces:**
- Produces codices 91–94, all `EXAMEN_VIOLATIO` (REICE): **91 L1 porta unica** — a value defined by `insula_radix(…)` reaching, in the same function, argument 1 of `stml_attributum_addere`, `stml_liberum_addere`, `insula_attributum_ponere`, or `insula_attributum_tollere` (def-use through `fluxus_datorum`, the codex 71 machinery: def = the call, use = the argument; anchor at the writing call; socius = the `insula_radix` call). **92 L2 purus** — inside a function whose leading comment carries `<purus/>` or `<componens/>`: any call to `mutare_durabile`, `mutare_ephemera`, `mutare_motum`, `insula_attributum_ponere`, `insula_attributum_tollere`, `stml_attributum_addere`, `stml_liberum_addere`, `pictor_documentum_actum`, or any identifier beginning `volumen_`, `filum_`, `fenestra_`, or `imprimere`/`printf`/`fprintf` (anchor at the call). **93 L7 statica** — in a TU carrying `<ludus/>`: a file-scope `hic_manens` object without `constans` that is the target of an assignment or `++`/`--` anywhere in the TU (a `character x[]` initialised from a literal and never assigned is allowed — the ctx-literal pattern of `ludus_toy.h` and `pictor_actiones.c`; anchor at the assignment, socius the declaration). **94 L9 destinatio caeca** — inside a function marked `<destinatio/>`: any reference to the identifiers `Mandata`, `Mandatum`, or a call beginning `mandata_`; and inside a function marked `<componens/>`: any call beginning `destinatio_` (anchor at the reference). `<quies/>` and `<tractator/>` are registered in the vocabulary now and judged by L4/L5 in a later batch (spec §6.4 items 2–3).
- The tags: `componens`, `purus`, `quies`, `tractator`, `destinatio`, `ludus`, and the house tolera anchor, which names a codex by its ENUM NAME: `/* <tolera codex="PORTA_UNICA"/> */` (semantica.c:1075 shows `<tolera codex="CONVERSIO_SIGNI_SEVERA"`), so the four new enum members need speakable names: `EXAMEN_CODEX_PORTA_UNICA`, `_PURITAS`, `_STATICUM_LUDI`, `_DESTINATIO_CAECA`. Six anchors, not four — `destinatio` and `ludus` are scoping anchors this batch needs; say so in the vocabulary spec's registry.

- [ ] **Step 1: Write the four fixtures (they are the failing tests)**

`l1_mendax.c`:
```c
/* l1_mendax.c - codex XCI: scriptura extra portam (fixum plantatum) */
#include "latina.h"
#include "insula.h"

vacuum mentiri (InsulaRepositorium* repo);

vacuum
mentiri (
    InsulaRepositorium* repo)
{
    StmlNodus* radix;

    radix = insula_radix(repo, INSULA_EPHEMERA);
    insula_attributum_ponere(radix, repo->piscina, repo->intern,
                             "focus", "furtivus");
}
```
`l2_impurus.c`:
```c
/* l2_impurus.c - codex XCII: componere quod scribit (fixum plantatum) */
#include "latina.h"
#include "componens.h"
#include "insula.h"
#include "motus.h"

Componens* componere_impurum (InsulaRepositorium* repo,
    constans Motus* motus, Piscina* p, InternamentumChorda* in,
    vacuum* ctx);

interior vacuum
tangere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    (vacuum)ctx;
    insula_attributum_ponere(radix, p, in, "numerus", "1");
}

/* <componens/> <purus/> */
Componens*
componere_impurum (
     InsulaRepositorium* repo,
        constans Motus* motus,
               Piscina* p,
    InternamentumChorda* in,
                vacuum* ctx)
{
    (vacuum)motus;
    (vacuum)ctx;
    mutare_ephemera(repo, tangere, NIHIL);
    redde componens_creare(p, in, "radix", PARTES_NULLUM);
}
```
`l7_staticum.c`:
```c
/* l7_staticum.c - codex XCIII: staticum mutabile in bibliotheca ludi
 * (fixum plantatum) */
#include "latina.h"

/* <ludus/> */

hic_manens i32 numerus_quadrorum = ZEPHYRUM;
hic_manens character titulus_ctx[] = "penicillus";   /* licet: numquam scriptum */

vacuum quadrum (vacuum);

vacuum
quadrum (vacuum)
{
    numerus_quadrorum++;
    (vacuum)titulus_ctx;
}
```
`l9_mandata_in_destinatione.c`:
```c
/* l9_mandata_in_destinatione.c - codex XCIV: strategia quae mandata
 * legit (fixum plantatum) */
#include "latina.h"
#include "mandatum.h"
#include "componens.h"

Componens* strategia_caeca (Componens* arbor, constans Mandata* m);

/* <destinatio/> */
Componens*
strategia_caeca (
          Componens* arbor,
    constans Mandata* m)
{
    si (mandata_numerus(m) > ZEPHYRUM)
    {
        redde arbor;
    }
    redde NIHIL;
}
```
`ludus_purus.c`: one file with `<ludus/>`, a `<purus/>` function that only reads, a `<destinatio/>` function that only walks `Componens`, a `<componens/>` function that returns a fresh tree, a `<quies/>` and a `<tractator/>` function — nothing fires.

Run: `./silva/examen.sh <fixture>` for each — Expected today: ACCIPE on all five (the codices do not exist), which is the red.

- [ ] **Step 2: Register**

Per `silva-fluxus-1-spec-v2.md` §6 "Registration (mechanical)": four enum lines after codex 90 in `ExamenCodex` (semantica.h, before `EXAMEN_CODEX_NUMERUS`); four `_codices[]` rows in semantica.c (severity `EXAMEN_VIOLATIO`, nuntius in Latin naming the lint: `porta unica insulae violata (L1)`, `functio pura scribit (L2)`, `staticum mutabile in ludo (L7)`, `destinatio mandata legit / componens destinationem vocat (L9)`), the static assert updated; the vocabulary tags registered where chunk C of `annotationes-stml-spec.md` registers the existing anchors, after `bin/natura quaere` for each. `examen_vectis.sh` needs zero wiring (ordinals from enum order).

- [ ] **Step 3: The seats**

91 in the `_fluxum_examinare` consumer (codex 71's def-use shape: def = call to `insula_radix`, use = argument 1 of one of the four writers; fire at the writer with socius at the def). 92, 93, 94 in the walk that already sees leading comments and file-scope declarations (codex 64's shape for comment-scoped rules; the `<purus/>`/`<componens/>`/`<destinatio/>` anchor is the function's leading comment — `Editio.commentum` semantics, the same text the formator keeps with the function). 93's "assigned anywhere" is a TU-wide use scan over the identifier (the same table `_tolera_absorbere` consults for identifiers).

- [ ] **Step 4: Verify**

`./silva/examen.sh` on each fixture → REICE naming exactly its codex (91/92/93/94) once; on `ludus_purus.c` → ACCIPE. Then, from the merged branch: `./silva/examen.sh lib/<every ludus lib>.c probationes/probatio_insula.c` → ACCIPE (the lying write in `probatio_insula.c` carries the tolera anchor; nothing else fires — if something does, that is a real finding: fix the code, not the codex). Add the five fixtures to the codex probatio suite the way 89/90's are added; `./silva/examen.sh -census` shows four new rows. `./silva/compile_probationes.sh exemplaria_lint` green.

- [ ] **Step 5: Commit (main), merge, gates, commit (branch)**

Main: `silva.commissio(msg, [semantica.h, semantica.c, fixtures, vocabulary spec], portae=[('silva','examen'), ('silva','exemplaria_lint')])` — from `../rhubarb`. Branch: `git merge main` (resolve nothing: the branch touches no silva file), add the anchors and the tolera, then `silva.commissio(msg, [the anchored files, plan], portae=[('radix', f) for every ludus filter])`.

---

## Self-Review

**Spec coverage (§8 P3 and what it pulls):**

| Spec item | Task |
|---|---|
| `tabula` + `penicillus.ictus` against tier-2 and tier-3 expectations | T7 (tree, tier 3 exemplar), T8 (tier 2 through the real loop) |
| `pictor_documentum` acta → projection with checkpoints (tier 8) | T4 (creare/aperire/actum/revocare/reficere/verificare; measurements) |
| `figura_tabulae` | T9 (+ `figura_tituli`; T2 provides the registry and `pingere`, tier 6) |
| `delineare_mandata` for `imago` + `rectangulum` | T3 (+ linea, polygonum, textus; tier 7 specimen) |
| First bake: closure, build time, binary size into §10 | T11 (+ the amalgam) |
| §6.4 item 1: vocabulary + L1/L2/L7/L9 | T12 (deferred from P2; in main) |
| §4 canons for the islands (P1 owed) | T6 (+ owners, from brainstorm §XVI) |
| Brainstorm §XVI §4 delivery boundary | T5 |
| Signed coordinates (Plan 1 deferral) | T1 |
| Window loop (implicit in "first bake" — a binary must run) | T10 |
| NOT in this plan (named): wheel position (P4), palettes/optio (P4), modals + flows (P5), marquee/selection (P5), capsula for icons (P4), thicker lines (P4), clipped text (P4) | — |

**Placeholder scan:** every task has its test, its code, its plant, its worklog line, its commit gates. The two places that delegate to a house convention rather than restate it are deliberate and named: the specimen candidate filename (`<titulus>.novum.png`, read from `lib/specimen.c:21`) and T12's registration sites (the fluxus spec §6 recipe, quoted). T12's judgment code is specified by rule and anchor, not written out: it lives in silva's walker, whose shape this plan cannot restate without the other agent's context — the fixtures are the contract.

**Type consistency across tasks:** after T1, `Punctum`/`Fines` are `s32` everywhere below; T2's figura code and T3's rasterizer are written with `s32` fields and `i32` counts/scales; `mandata_textus(m, x, y, textus, fons, color)` (x, y become `s32` in T1) is the form used in T2, T3, T9; `mandata_imago(m, fons_chorda, fines)` stores its source in `Mandatum.textus`, which T3's rasterizer and T9's assertion both read; `Componere` is `(repo, motus, piscina, intern, ctx)` in T7, T10, T11 as in Plan 1 T10; `ImagoFons` is `(chorda, vacuum*)` in T3, T9, T10, T11; `PictorFigurae` is both the registry ctx and the fons ctx (T9, T11); `LudusFenestra.mensurae` is what T11's smoke greps; `insula_scriptorem_ponere` is called by the dispensator (T6) with `c->actio`, which is why `instrumentum.eligere` (T8) owns `instrumentum` without knowing about owners.

**AUDIENDA — not verified while writing this plan (check at the task, adjust the code, note in the worklog):**
- Whether `volumen_massam_condere` stores a chorda with embedded zero bytes intact (T4's checkpoint bytes are raw RGBA; the probatio's white sheet has 255s but the alpha/colour zeros are there — the `checkpoint/2` round-trip assertion via `proicere_ad` proves it).
- `canon.canon`'s `<liberum>` attributes (`maximum`, `minimum`) — T6's `durabilis.canon` uses the bare form.
- Whether `chorda_ut_s32` parses a leading `-` (T1's `pan_y="-10"` round trip and T7's `translatio_x="-5"` exemplar depend on it; `lib/chorda.c`'s body did not show a sign branch on a first grep — if it lacks one, add it in T1 with its own assertion in `probatio_chorda`).
- Whether `stml_scribere` escapes nothing in attribute values that T7's exemplar relies on (`puncta="10,20;12,22"` — whatever Plan 1 T3's separator is).
- The `Eventus` mouse coordinate space versus a scaled pixel table (T10 sidesteps with scale 1).
- `-Wunused-function` on the `interior` functions in `ludus_toy.h` when a probatio uses only one entry point (T5 adds a handler arm rather than a function for this reason).
- Whether `aedilis` accepts a `.c` under `apps/pictor/` without a `<aedilis>` directive of its own (forum/villa have `apps/<x>/<x>.c` the same way — expected yes).
