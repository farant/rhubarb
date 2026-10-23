# Testa B1 Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans (Fran chose inline, one task per turn, Fran approves each). Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** pictor (the vicus pictor - spraycan, layers) draws under a
finger on the Fire 7 from its home-screen icon, built and installed by
one command; on the way pictor, scriba and vicus move onto one app
contract on macOS with byte-identical output.

**Architecture:** `include/testa.h` = "I am an app": including it
links a shell that owns `principale` (macOS, `lib/testa.c`) or the
NativeActivity entry (Android, `lib/testa_android.c`) by the aedilis
variant rule; the app defines only `applicatio_initium`. Shared shell
logic (`testa_cursus`) and the Android pure pieces
(`testa_android_initus`) are portable C with host suites.

**Tech Stack:** C89 in Latin (latina.h), credo v2, aedilis, NDK r27d
clang 18, aapt2/zipalign/apksigner (build-tools 34.0.0), adb.

**Spec:** `project-specs/testa-spec.md` (quarta 48709793, approved
2026-10-23).

## Global Constraints

- C89, house flags (`tools/vexilla.sh` `VEXILLA_C89`), Latin
  identifiers; latina.h macros (incl. every Roman numeral, `nomen`,
  `brevis`, `per`, `casus`) are FORBIDDEN as identifiers.
- No second thread, ever (D1).
- Android: API 30 minimum, `targetSdkVersion 34`, arm64-v8a only,
  package `earth.golden.<app>` (D5), landscape, back = quit, clipboard
  stubbed (D6).
- Every Android-only `.c` starts `#include "postulata_posix.h"` then
  `#include "postulata_android.h"` (`#define inline static __inline__`).
- Device work: one tablet at a time through the shared host lock;
  nothing left installed after a smoke unless `-servare` (Fran's
  cleanup rule).
- Never install into `~/.bin` from this worktree; GUI windows
  (`-fumus` runs of pictor/scriba/vicus) open only with Fran's OK.
- Commit with `silva.commissio`; chorda is NOT null-terminated.

## Review Focus

1. Touch that starts in the letterbox margin and drags onto the canvas
   - expected: coordinates outside `[0, latitudo)` while in the margin
   (A3c), a normal stroke once inside; never a crash or a clamp.
   Pinned in T2 (`_sectio_margo`).
2. The tablet locking mid-stroke (pause while a finger is down) -
   expected: `CANCEL` or no further events, then `MUS_LIBERATUS`
   delivered once; no stuck button after resume. Pinned in T2
   (`_sectio_abruptio`) and checked by Fran in T3.
3. Two fingers - expected: the second finger is ignored, the first
   keeps drawing; after all fingers lift, a new touch works. Pinned in
   T2 (`_sectio_digiti_duo`).
4. A pending delayed frame when a touch arrives - expected: the touch
   is drawn at once, not after the 300 ms idle wait. Pinned in T2
   (`_sectio_quadrum`, row "pendens post, debitum -> statim").
5. vicus's two exits (Cmd+Q via EVENTUS_CLAUDERE, closing the window)
   still close its volumen exactly once. Pinned in T1
   (`_sectio_dimittere_semel`) + parity run.

## Rulings (made while planning; spec binds, these refine)

- **R1 shell file names follow aedilis's variant rule**:
  `lib/testa.c` (base; macOS and any OS with a pollable `fenestra`),
  `lib/testa_android.c` (variant `android`). The spec's
  `testa_fenestra.c` would never be found by `_obiectum_probare`
  (`lib/aedilis.c:556-613`: `lib/<basis>_<varians>.{m,c}`, then
  `lib/<basis>.c`). Cost if wrong: a rename.
- **R2 two headers**: `include/testa_cursus.h` (types + shared shell
  logic, `lib/testa_cursus.c`) and `include/testa.h` (includes it,
  declares `applicatio_initium`). Including `testa.h` links a shell
  with `principale`, so suites include `testa_cursus.h` only. Cost if
  wrong: one header merge.
- **R3 escaping locals move into the piscina**: today `principale`
  passes addresses of locals into the app (`&app.montatio.figurae_ctx`
  as `fons_ctx`, vicus `&pulsus`, `&porta_quaestionis`); after the
  move they would dangle when `applicatio_initium` returns. Each app
  allocates its host struct from `a->piscina`.
- **R4 Android frame cadence mirrors the macOS loop**: after a frame
  the next is requested after `ludus_fenestra_mora` (the dispensator's
  `quies_ms`, 300 in pictor), capped at XVI ms with a pulse, STATIM
  when `mora == 0` - so dispensator timers behave identically. Two
  callbacks (`statim`, `post`) because Choreographer callbacks cannot
  be cancelled.
- **R5 the shell queries the screen only when not `-fumus`** (vicus
  did exactly that, `vicus.c:158`); pictor/scriba never queried it, so
  their non-fumus start gains two NSScreen reads before
  `fenestra_creare` - no visible change expected, checked by Fran.
- **R6 `ludus_fenestra.h` keeps `#include "fenestra.h"`** in B1: on
  Android it resolves to `lib/fenestra_textus.c` only (portable); a
  call to an unimplemented `fenestra_*` fails the link
  (`-Wl,--no-undefined`). Cost if wrong: one include swap.
- **R7 the "ludus: quadra=..." line moves into
  `testa_cursum_finire`**, printed by both shells (logcat on Android);
  `apps/pictor/fumus.sh` still greps it.

## File map

| File | Task | Role |
|---|---|---|
| `include/testa_cursus.h`, `lib/testa_cursus.c` | T1 | contract types, options, forma check, cursus (incipere / fenestrare / finire / dimittere) |
| `include/testa.h` | T1 (T4 docs) | app marker; `applicatio_initium`; authoring rules |
| `lib/testa.c` | T1 | macOS shell: `principale`, loop (ex `ludus_fenestra_currere`) |
| `apps/pictor/pictor.c`, `apps/scriba/scriba.c`, `apps/vicus/vicus.c` | T1 | `principale` -> `applicatio_initium` |
| `include/ludus_fenestra.h`, `lib/ludus_fenestra.c` | T1 | `ludus_fenestra_currere` removed |
| `lib/ludus_fenestra.interrogationes.stml`, `lib/testa_cursus.interrogationes.stml` | T1 | questions follow the move |
| `probationes/probatio_testa.c` | T1 | contract suite |
| `include/postulata_android.h` | T2 | NDK `inline` |
| `include/testa_android_initus.h`, `lib/testa_android_initus.c` | T2 | geometry, touch -> Eventus, frame decision, blit |
| `probationes/probatio_testa_android_initus.c` | T2 | pure pieces suite (host + Fire) |
| `lib/testa_android.c` | T3 | Android shell |
| `tools/android/communia.sh` | T3 | adb preflight + host lock, sourced by `probatio.sh` and `apk.sh` |
| `tools/android/apk.sh`, `tools/android/ndk_fixum.sh` | T3 | APK build/install/launch; SDK pins |
| `tools/android/apk.sh -fumus` | T4 | device smoke |
| `tools/android_struere_porta.sh`, `pythonica/silva.py` | T4 | gate `android-struere` |

---
### Task 1: the contract and the macOS shell; pictor, scriba, vicus moved

**Files:**
- Create: `include/testa_cursus.h`, `lib/testa_cursus.c`, `include/testa.h`, `lib/testa.c`, `probationes/probatio_testa.c`, `lib/testa_cursus.interrogationes.stml`
- Modify: `apps/pictor/pictor.c:1-125`, `apps/scriba/scriba.c:1-108`, `apps/vicus/vicus.c:1-257`, `include/ludus_fenestra.h:96-105`, `lib/ludus_fenestra.c:11-12,213-277`, `lib/ludus_fenestra.interrogationes.stml:3-16,80-92`

**Interfaces:**
- Consumes: `ludus_fenestra_creare/_tractare/_pingendum/_mora/_pulsum_ponere`, `ludus_quadrum`, `ludus_fenestra_imaginem_scribere` (`include/ludus_fenestra.h`); `dispensator_finire`; `fenestra_*` (`include/fenestra.h`); `clipboard_capere_imaginem`.
- Produces (T2-T4 rely on these exact names):
  - `nomen ImagoFructus (*TestaImagoCapere)(vacuum* ctx, Piscina* piscina);`
  - `TestaAmbitus { Piscina* piscina; s32 argc; character** argv; b32 fumus; i32 schirmi_latitudo, schirmi_altitudo; i32 utilis_x, utilis_y, utilis_latitudo, utilis_altitudo; constans character* sedes_datorum; i32 densitas; TestaImagoCapere imago_glutinata; vacuum* imago_ctx; }` (`densitas` 0 on macOS; Android fills it in T3)
  - `TestaForma { constans character* titulus; i32 latitudo, altitudo, scala; b32 plena_visio; Dispensator* d; FiguraRegistrum* figurae; i32 thema; ImagoFons fons; vacuum* fons_ctx; LudusPulsator pulsator; vacuum* pulsator_ctx; vacuum (*finis)(vacuum* ctx); vacuum* finis_ctx; }`
  - `nomen b32 (*TestaInitium)(constans TestaAmbitus* a, TestaForma* forma);`
  - `TestaOptiones { constans character* via_imaginis; b32 fumus; }`, `vacuum testa_optiones_legere(s32 argc, character** argv, TestaOptiones* o)`
  - `constans character* testa_titulus_processus(constans character* programma)`
  - `b32 testa_formam_probare(constans TestaForma* f, constans character** causa)`
  - `TestaCursus { TestaForma forma; LudusFenestra* lf; b32 initiatus; b32 finitus; }`
  - `b32 testa_cursum_incipere(TestaCursus* c, constans TestaAmbitus* a, TestaInitium initium, constans character** causa)`
  - `b32 testa_cursum_ludum_struere(TestaCursus* c, Piscina* piscina, TabulaPixelorum* tabula)`
  - `vacuum testa_cursum_finire(TestaCursus* c)` - `dispensator_finire` + the "ludus: quadra=..." line (R7)
  - `vacuum testa_cursum_dimittere(TestaCursus* c)` - `forma.finis` at most once, only if initium returned VERUM
  - `b32 applicatio_initium(constans TestaAmbitus* a, TestaForma* forma)` (declared in `testa.h`, defined by each app)

- [ ] **Step 1: Capture parity PNGs from TODAY's binaries (ask Fran first - three windows open for ~10 s each)**

```bash
mkdir -p build/testa/paritas
for app in pictor scriba vicus; do
  for k in a b; do
    ./apps/$app/$app.sh -fumus -imago build/testa/paritas/$app.vetus.$k.png \
      > build/testa/paritas/$app.vetus.$k.log 2>&1; echo "$app $k rc=$?"
  done
  cmp build/testa/paritas/$app.vetus.a.png build/testa/paritas/$app.vetus.b.png \
    && echo "$app DETERMINATUM" || echo "$app NON DETERMINATUM"
done
```

Expected: rc=0 six times; pictor and scriba DETERMINATUM. If vicus is
NON DETERMINATUM (clock in the tab line, battery, Claude-session
notices), ledger `Task 1: Ruling: vicus parity = same exit, same
"ludus: quadra=30" line, Fran's eye on the PNG` and continue.

- [ ] **Step 2: Write the failing contract suite `probationes/probatio_testa.c`**

```c
/* probatio_testa.c - contractus applicationis (testa-spec III.2):
 * optiones, titulus processus, forma, cursus incipere -> ludum struere ->
 * finire -> dimittere (finis semel, numquam sine initio). Testa vera
 * (principale) non includitur: testa_cursus.h solum. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "insula.h"
#include "actio.h"
#include "figura.h"
#include "dispensator.h"
#include "delineare_mandata.h"
#include "testa_cursus.h"
#include "ludus_toy.h"
#include "credo.h"
#include <string.h>

/* applicatio probanda: status totus in piscina ambitus (R3) */
nomen structura {
    ToyStatus  toy;
          i32  fines;      /* quoties finis vocatus */
          i32  pulsus;
} AppProbanda;

interior vacuum
_finire_probandam (
    vacuum* ctx)
{
    ((AppProbanda*)ctx)->fines += I;
}

interior b32
_pulsare_probandam (
    vacuum* ctx)
{
    ((AppProbanda*)ctx)->pulsus += I;
    redde FALSUM;
}

/* initium verum: toy dispensator, figurae, forma plena */
interior b32
_initium_probandum (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    InternamentumChorda* intern;
     InsulaRepositorium* repo;
         ActioRegistrum* reg;
            AppProbanda* app;

    app = (AppProbanda*)piscina_allocare(a->piscina,
        magnitudo(AppProbanda));
    si (!app)
    {
        redde FALSUM;
    }
    memset(app, ZEPHYRUM, magnitudo(AppProbanda));
    intern  = internamentum_creare(a->piscina);
    thema_initiare();
    repo    = insula_repositorium_creare(a->piscina, intern, "<insula/>");
    reg     = actio_registrum_creare(a->piscina, intern);
    toy_registrare(reg, &app->toy);
    forma->titulus       = "probanda";
    forma->latitudo      = CC;
    forma->altitudo      = C;
    forma->scala         = II;
    forma->d             = dispensator_creare(a->piscina, intern, repo,
                               reg, toy_componere, &app->toy, CCC);
    forma->figurae       = figura_registrum_creare(a->piscina);
    si (   !forma->figurae
        || !figura_registrare(forma->figurae, PARTES_BOTTONE, ZEPHYRUM,
               figura_finium, NIHIL))
    {
        redde FALSUM;
    }
    forma->pulsator      = _pulsare_probandam;
    forma->pulsator_ctx  = app;
    forma->finis         = _finire_probandam;
    forma->finis_ctx     = app;
    redde VERUM;
}

/* initium deficiens (volumen non apertum, e.g.) */
interior b32
_initium_deficiens (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    (vacuum)a;
    forma->finis = _finire_probandam;
    redde FALSUM;
}

/* initium verum sed forma prava (scala ZEPHYRUM) - finis tamen
 * vocandus: applicatio statum (volumen) iam aperuit */
interior b32
_initium_scala_nulla (
    constans TestaAmbitus* a,
               TestaForma* forma)
{
    redde _initium_probandum(a, forma)
        && (forma->scala = ZEPHYRUM, VERUM);
}

interior vacuum
_ambitum_ponere (
     TestaAmbitus* a,
          Piscina* piscina)
{
    memset(a, ZEPHYRUM, magnitudo(TestaAmbitus));
    a->piscina        = piscina;
    a->sedes_datorum  = ".";
}

interior vacuum
_sectio_optiones (
    CredoContextus* c)
{
    character*     omnia[V];
    character*     sola[II];
    TestaOptiones  o;

    (vacuum)c;
    omnia[ZEPHYRUM]  = (character*)"bin/pictor";
    omnia[I]         = (character*)"-volumen";
    omnia[II]        = (character*)"x.volumen";
    omnia[III]       = (character*)"-fumus";
    omnia[IV]        = (character*)"-imago";
    testa_optiones_legere(V, omnia, &o);
    CREDO_GRADUS("-imago ultimum sine via: NIHIL");
    CREDO_VERUM(o.fumus);
    CREDO_NIHIL(o.via_imaginis);

    sola[ZEPHYRUM]  = (character*)"bin/pictor";
    sola[I]         = (character*)"-imago";
    testa_optiones_legere(II, sola, &o);
    CREDO_FALSUM(o.fumus);
    CREDO_NIHIL(o.via_imaginis);

    omnia[IV - I]    = (character*)"-imago";
    omnia[IV]        = (character*)"y.png";
    testa_optiones_legere(V, omnia, &o);
    CREDO_GRADUS("-imago y.png: via; -fumus absens");
    CREDO_FALSUM(o.fumus);
    CREDO_NON_NIHIL(o.via_imaginis);
    CREDO_VERUM(strcmp(o.via_imaginis, "y.png") == ZEPHYRUM);
}

interior vacuum
_sectio_titulus (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(strcmp(testa_titulus_processus("bin/pictor"),
        "pictor") == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus("vicus"), "vicus")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus(NIHIL), "applicatio")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus(""), "applicatio")
        == ZEPHYRUM);
    CREDO_VERUM(strcmp(testa_titulus_processus("bin/"), "applicatio")
        == ZEPHYRUM);
}

interior vacuum
_sectio_cursus (
    CredoContextus* c)
{
         TestaAmbitus  a;
          TestaCursus  cursus;
    constans character* causa;
     TabulaPixelorum* t;
          AppProbanda* app;
              Eventus  e;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_VERUM(testa_cursum_incipere(&cursus, &a, _initium_probandum,
        &causa));
    CREDO_NIHIL(causa);
    app = (AppProbanda*)cursus.forma.finis_ctx;

    t = tabula_pixelorum_creare_nuda(c->piscina, CC, C);
    CREDO_VERUM(testa_cursum_ludum_struere(&cursus, c->piscina, t));
    CREDO_NON_NIHIL(cursus.lf);
    CREDO_GRADUS("pulsus formae in ludo positus");
    CREDO_VERUM(cursus.lf->pulsator == _pulsare_probandam);
    CREDO_GRADUS("ludum struere iterum: recusatur (ludus unus)");
    CREDO_FALSUM(testa_cursum_ludum_struere(&cursus, c->piscina, t));

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = EVENTUS_SUSPENSIO;
    ludus_fenestra_tractare(cursus.lf, &e, (s64)M);
    ludus_quadrum(cursus.lf, (s64)M);
    CREDO_AEQUALIS_I32(cursus.lf->mensurae.quadra, I);

    testa_cursum_finire(&cursus);
    CREDO_AEQUALIS_I32(app->fines, ZEPHYRUM);
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(app->fines, I);
}

/* Review Focus 5: finis semel, quotquot exitus */
interior vacuum
_sectio_dimittere_semel (
    CredoContextus* c)
{
         TestaAmbitus  a;
          TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    CREDO_VERUM(testa_cursum_incipere(&cursus, &a, _initium_probandum,
        &causa));
    testa_cursum_dimittere(&cursus);
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(((AppProbanda*)cursus.forma.finis_ctx)->fines,
        I);
}

interior vacuum
_sectio_initium_deficiens (
    CredoContextus* c)
{
         TestaAmbitus  a;
          TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_FALSUM(testa_cursum_incipere(&cursus, &a, _initium_deficiens,
        &causa));
    CREDO_NON_NIHIL(causa);
    CREDO_GRADUS("initium FALSUM: finis numquam (status nullus)");
    testa_cursum_dimittere(&cursus);
    CREDO_FALSUM(cursus.finitus);
}

interior vacuum
_sectio_forma_prava (
    CredoContextus* c)
{
         TestaAmbitus  a;
          TestaCursus  cursus;
    constans character* causa;

    _ambitum_ponere(&a, c->piscina);
    memset(&cursus, ZEPHYRUM, magnitudo(TestaCursus));
    causa = NIHIL;
    CREDO_FALSUM(testa_cursum_incipere(&cursus, &a, _initium_scala_nulla,
        &causa));
    CREDO_NON_NIHIL(causa);
    CREDO_VERUM(strcmp(causa, "forma: scala < I") == ZEPHYRUM);
    CREDO_GRADUS("forma prava post initium verum: finis tamen semel");
    testa_cursum_dimittere(&cursus);
    CREDO_AEQUALIS_I32(((AppProbanda*)cursus.forma.finis_ctx)->fines,
        I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "optiones testae", _sectio_optiones, NIHIL, NIHIL, NIHIL, NIHIL },
    { "titulus processus", _sectio_titulus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cursus: incipere, ludum struere, quadrum, finire, dimittere",
        _sectio_cursus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "dimittere semel", _sectio_dimittere_semel, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "initium deficiens", _sectio_initium_deficiens, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "forma prava", _sectio_forma_prava, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("testa", SECTIONES);
}
```

(`CredoSectio` = six fields, as `probationes/probatio_ludus_fenestra.c:222-226`;
macros checked against `include/credo.h` 2026-10-23.)

- [ ] **Step 3: Run it - expect a build failure**

Run: `./compile_tests.sh probatio_testa`
Expected: FAIL - `testa_cursus.h` not found (exit 1, or 2 if nothing
compiled - read which).

- [ ] **Step 4: Write `include/testa_cursus.h`** (new file via `./silva/scribe.sh include/testa_cursus.h <<'EOF'` - in a worktree, not `~/.bin/scribe`)

```c
#ifndef TESTA_CURSUS_H
#define TESTA_CURSUS_H

/* <aedilis corpus="lib/testa_cursus.c"/> */
/* testa_cursus.h - contractus applicationis (testa-spec II.1) et
 * cursus communis testarum: initium -> fenestra -> eventus/quadra ->
 * finis. Testae (lib/testa.c, lib/testa_android.c) id vocant;
 * probationes quoque, quia principale NON affert (testa.h affert). */

#include "latina.h"
#include "piscina.h"
#include "imago.h"
#include "eventus.h"
#include "dispensator.h"
#include "figura.h"
#include "delineare_mandata.h"
#include "ludus_fenestra.h"

/* clipboard: idem genus ac actiones_ctx.imago_capere pictoris */
nomen ImagoFructus (*TestaImagoCapere)(vacuum* ctx, Piscina* piscina);

/* quod testa applicationi praebet */
nomen structura {
               Piscina* piscina;            /* testa possidet et destruit */
                   s32  argc;               /* argv integer: app suas */
            character** argv;               /* optiones legit */
                   b32  fumus;              /* -fumus */
                   i32  schirmi_latitudo;   /* puncta (macOS) / pixela */
                   i32  schirmi_altitudo;   /* (Android); 0 = ignotus */
                   i32  utilis_x;           /* area utilis (sine menu, */
                   i32  utilis_y;           /* navali), origo fenestrae */
                   i32  utilis_latitudo;    /* 0 = ignota */
                   i32  utilis_altitudo;
    constans character* sedes_datorum;      /* macOS ".", Android
                                             * internalDataPath (= cwd) */
                   i32  densitas;           /* Android dpi (Fire 7: 160);
                                             * 0 = ignota (macOS: puncta
                                             * iam scalam systematis ferunt) */
      TestaImagoCapere  imago_glutinata;    /* Android B1: semper vacua */
                vacuum* imago_ctx;
} TestaAmbitus;

/* quod applicatio testae reddit (testa eam ZEPHYRUM implet ante) */
nomen structura {
    constans character* titulus;
                   i32  latitudo;           /* pixela NOSTRA */
                   i32  altitudo;
                   i32  scala;              /* puncta per pixelum */
                   b32  plena_visio;
           Dispensator* d;
       FiguraRegistrum* figurae;
                   i32  thema;
             ImagoFons  fons;
                vacuum* fons_ctx;
         LudusPulsator  pulsator;           /* NIHIL = nullus */
                vacuum* pulsator_ctx;
                vacuum  (*finis)(vacuum* ctx);   /* NIHIL licet */
                vacuum* finis_ctx;
} TestaForma;

nomen b32 (*TestaInitium)(constans TestaAmbitus* a, TestaForma* forma);

/* optiones TESTAE ex argv (applicatio argv totum quoque videt) */
nomen structura {
    constans character* via_imaginis;       /* -imago <via>; NIHIL */
                   b32  fumus;              /* -fumus */
} TestaOptiones;

vacuum
testa_optiones_legere (
              s32  argc,
       character** argv,
    TestaOptiones* o);

/* basis programmatis ("bin/pictor" -> "pictor"): titulus piscinae, ut olim
 * per applicationem; "applicatio" si NIHIL aut vacua */
constans character*
testa_titulus_processus (
    constans character* programma);

/* forma valida? FALSUM + causa (litterae fixae) nominata */
b32
testa_formam_probare (
     constans TestaForma* f,
    constans character** causa);

nomen structura {
       TestaForma  forma;
    LudusFenestra* lf;
              b32  initiatus;    /* initium VERUM reddidit */
              b32  finitus;      /* finis vocatus */
} TestaCursus;

/* initium vocat (forma ZEPHYRUM ante), formam probat. FALSUM: causa
 * nominata; si initium VERUM erat, dimittere finem tamen vocat */
b32
testa_cursum_incipere (
            TestaCursus* c,
    constans TestaAmbitus* a,
           TestaInitium  initium,
    constans character** causa);

/* ludum ex forma struere (semel; iterum FALSUM) */
b32
testa_cursum_ludum_struere (
        TestaCursus* c,
            Piscina* piscina,
    TabulaPixelorum* tabula);

/* finis ansae: dispensator_finire + linea mensurarum "ludus: ..." */
vacuum
testa_cursum_finire (
    TestaCursus* c);

/* forma.finis semel, solum post initium verum */
vacuum
testa_cursum_dimittere (
    TestaCursus* c);

#endif /* TESTA_CURSUS_H */
```

- [ ] **Step 5: Write `lib/testa_cursus.c`**

```c
/* testa_cursus.c - cursus communis testarum (testa-spec II.1, II.3) */

#include "testa_cursus.h"

#include <stdio.h>
#include <string.h>

vacuum
testa_optiones_legere (
              s32  argc,
       character** argv,
    TestaOptiones* o)
{
    s32 k;

    o->via_imaginis  = NIHIL;
    o->fumus         = FALSUM;
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-fumus") == ZEPHYRUM)
        {
            o->fumus = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-imago") == ZEPHYRUM
                     && k + I < argc)
        {
            o->via_imaginis = argv[k + I];
        }
    }
}

constans character*
testa_titulus_processus (
    constans character* programma)
{
    constans character* basis;
    constans character* p;

    si (!programma)
    {
        redde "applicatio";
    }
    basis = programma;
    per (p = programma; *p != '\0'; p++)
    {
        si (*p == '/')
        {
            basis = p + I;
        }
    }
    redde *basis != '\0' ? basis : "applicatio";
}

b32
testa_formam_probare (
     constans TestaForma* f,
    constans character** causa)
{
    si (!f->titulus)
    {
        *causa = "forma: titulus NIHIL";
        redde FALSUM;
    }
    si (f->scala < I)
    {
        *causa = "forma: scala < I";
        redde FALSUM;
    }
    si (f->latitudo < I || f->altitudo < I)
    {
        *causa = "forma: magnitudo non positiva";
        redde FALSUM;
    }
    si (!f->d)
    {
        *causa = "forma: dispensator NIHIL";
        redde FALSUM;
    }
    si (!f->figurae)
    {
        *causa = "forma: figurae NIHIL";
        redde FALSUM;
    }
    redde VERUM;
}

b32
testa_cursum_incipere (
            TestaCursus* c,
    constans TestaAmbitus* a,
           TestaInitium  initium,
    constans character** causa)
{
    memset(c, ZEPHYRUM, magnitudo(TestaCursus));
    si (!initium(a, &c->forma))
    {
        *causa = "applicatio_initium FALSUM reddidit";
        redde FALSUM;
    }
    c->initiatus = VERUM;
    redde testa_formam_probare(&c->forma, causa);
}

b32
testa_cursum_ludum_struere (
        TestaCursus* c,
            Piscina* piscina,
    TabulaPixelorum* tabula)
{
    si (!c->initiatus || c->lf)
    {
        redde FALSUM;
    }
    c->lf = ludus_fenestra_creare(piscina, c->forma.d, c->forma.figurae,
        c->forma.thema, c->forma.fons, c->forma.fons_ctx, tabula);
    si (!c->lf)
    {
        redde FALSUM;
    }
    ludus_fenestra_pulsum_ponere(c->lf, c->forma.pulsator,
        c->forma.pulsator_ctx);
    redde VERUM;
}

vacuum
testa_cursum_finire (
    TestaCursus* c)
{
    LudusFenestra* lf;

    lf = c->lf;
    si (!lf)
    {
        redde;
    }
    /* S1a: pendentia (gestus, pan/zoom) ante exitum effunduntur */
    dispensator_finire(lf->d);
    imprimere("ludus: quadra=%d compositio=%ldms pingere=%ldms"
              " delineare=%ldms maximum=%ldms\n",
              (int)lf->mensurae.quadra,
              (long)lf->mensurae.ms_compositionis,
              (long)lf->mensurae.ms_pingendi,
              (long)lf->mensurae.ms_delineandi,
              (long)lf->mensurae.ms_quadri_maximum);
}

vacuum
testa_cursum_dimittere (
    TestaCursus* c)
{
    si (!c->initiatus || c->finitus)
    {
        redde;
    }
    c->finitus = VERUM;
    si (c->forma.finis)
    {
        c->forma.finis(c->forma.finis_ctx);
    }
}
```

- [ ] **Step 6: Run the suite - expect green**

Run: `./compile_tests.sh probatio_testa`
Expected: all six sections PRAETERITAE, exit 0. Then plant via
`silva.planta()`: `testa_cursum_dimittere` without the `c->finitus`
check -> `dimittere semel` red (fines 2); restore.

- [ ] **Step 7: Write `include/testa.h` and the macOS shell `lib/testa.c`**

`include/testa.h`:

```c
#ifndef TESTA_H
#define TESTA_H

/* testa.h - "applicatio sum: da mihi testam" (testa-spec II.1).
 * Includere = principale a testa accipere: lib/testa.c (macOS, et
 * quaevis fenestra perscrutabilis) aut lib/testa_android.c (--varians
 * android,posix) - regula variantium aedilis, nullus casus in
 * scriptis. Applicatio SOLUM applicatio_initium definit. Probationes
 * testa_cursus.h includunt, numquam hoc (principale duplex). */

#include "testa_cursus.h"

/* Status applicationis in a->piscina aedificare et formam implere.
 * Semel vocatur, schirmo noto; FALSUM = exitus I (nuntium ipsa
 * scribit). Monstratores in formam dati vivere debent usque ad finem:
 * nulla variabilis localis (R3). */
b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma);

#endif /* TESTA_H */
```

`lib/testa.c`:

```c
/* testa.c - testa fenestrae (macOS; quaevis fenestra perscrutabilis):
 * principale possidet, applicatio_initium vocat, ansam currit
 * (testa-spec II.3). Ansa = ludus_fenestra_currere olim (sedes
 * mutata, verbatim; mensurae in testa_cursum_finire). */

#include "testa.h"
#include "fenestra.h"
#include "clipboard_platform.h"

#include <stdio.h>
#include <string.h>

#define QUADRA_FUMI  XXX
/* S1c: exspectatio maxima cum applicatione viva (ut terminale) */
#define MORA_PULSUS  XVI

/* L5: porta imaginis - clipboard systematis (Cmd+V in pictore) */
interior ImagoFructus
_imaginem_glutinatam (
     vacuum* ctx,
    Piscina* piscina)
{
    (vacuum)ctx;
    redde clipboard_capere_imaginem(piscina);
}

/* ansa vera: exspectare (ludus_fenestra_mora), tractare, quadrum,
 * praesentare; finis in EVENTUS_CLAUDERE aut post quadra_maxima */
interior vacuum
_currere (
    LudusFenestra* lf,
         Fenestra* fenestra,
              i32  quadra_maxima)
{
    Eventus e;
        s64 nunc;
        b32 claudendum;

    claudendum = FALSUM;
    dum (!claudendum && !fenestra_debet_claudere(fenestra))
    {
        Mora mora;
         b32 eventa;

        /* S1c: applicatio viva (pulsus) - XVI ms ad summum */
        mora = ludus_fenestra_mora(lf);
        si (lf->pulsator && (mora == ZEPHYRUM || mora > MORA_PULSUS))
        {
            mora = MORA_PULSUS;
        }
        fenestra_expectare_eventus(fenestra, mora);
        nunc    = fenestra_tempus_ms();
        eventa  = FALSUM;
        dum (fenestra_obtinere_eventus(fenestra, &e))
        {
            si (e.genus == EVENTUS_CLAUDERE)
            {
                claudendum = VERUM;
                frange;
            }
            ludus_fenestra_tractare(lf, &e, nunc);
            eventa = VERUM;
        }
        /* fumus (quadra_maxima): omne quadrum pingitur - aliter
         * applicatio viva otiosa post quadrum primum numquam exit */
        si (   quadra_maxima == ZEPHYRUM
            && !ludus_fenestra_pingendum(lf, eventa, nunc))
        {
            perge;
        }
        ludus_quadrum(lf, nunc);
        fenestra_praesentare_pixela(fenestra, lf->tabula);
        si (   quadra_maxima > ZEPHYRUM
            && lf->mensurae.quadra >= quadra_maxima)
        {
            claudendum = VERUM;
        }
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
          TestaAmbitus  a;
         TestaOptiones  o;
           TestaCursus* cursus;
     constans TestaForma* f;
    FenestraConfiguratio  cfg;
              Fenestra* fenestra;
       TabulaPixelorum* tabula;
    constans character* causa;
                   i32  latitudo_fenestrae;
                   i32  altitudo_fenestrae;
                   s32  exitus;

    piscina = piscina_generare_dynamicum(
        testa_titulus_processus(argc > ZEPHYRUM ? argv[ZEPHYRUM] : NIHIL),
        VIII * M * M);
    si (!piscina)
    {
        redde I;
    }
    testa_optiones_legere((s32)argc, argv, &o);
    memset(&a, ZEPHYRUM, magnitudo(TestaAmbitus));
    a.piscina          = piscina;
    a.argc             = (s32)argc;
    a.argv             = argv;
    a.fumus            = o.fumus;
    a.sedes_datorum    = ".";
    a.imago_glutinata  = _imaginem_glutinatam;
    /* R5: schirmus solum extra fumum (ut vicus olim): fumus magnitudine
     * fixa, imagines certae */
    si (   !o.fumus
        && !fenestra_spatium_schirmi(&a.schirmi_latitudo,
               &a.schirmi_altitudo))
    {
        a.schirmi_latitudo  = ZEPHYRUM;
        a.schirmi_altitudo  = ZEPHYRUM;
    }
    si (   !o.fumus
        && !fenestra_spatium_utile(&a.utilis_x, &a.utilis_y,
               &a.utilis_latitudo, &a.utilis_altitudo))
    {
        a.utilis_latitudo  = ZEPHYRUM;
        a.utilis_altitudo  = ZEPHYRUM;
    }

    cursus = (TestaCursus*)piscina_allocare(piscina,
        magnitudo(TestaCursus));
    si (!cursus)
    {
        redde I;
    }
    si (!testa_cursum_incipere(cursus, &a, applicatio_initium, &causa))
    {
        fprintf(stderr, "%s: %s\n",
            testa_titulus_processus(argc > ZEPHYRUM ? argv[ZEPHYRUM]
                                                    : NIHIL), causa);
        testa_cursum_dimittere(cursus);
        redde I;
    }
    f = &cursus->forma;

    /* S2c (vicus): plena visio -> fenestra in area utili, altitudine
     * pixelis nostris integra (scala tabulae exacte f->scala: 1147/573
     * = 2.0017 schirmum 599 non 600 daret), plena visio POST tabulam
     * (scala in creatione tabulae ex altitudine CURRENTE figitur) */
    memset(&cfg, ZEPHYRUM, magnitudo(FenestraConfiguratio));
    cfg.titulus         = f->titulus;
    cfg.x               = C;
    cfg.y               = C;
    cfg.vexilla         = FENESTRA_ORDINARIA;
    latitudo_fenestrae  = f->latitudo;
    altitudo_fenestrae  = f->altitudo;
    si (   f->plena_visio && a.utilis_latitudo > ZEPHYRUM
        && a.utilis_altitudo > ZEPHYRUM)
    {
        cfg.x               = a.utilis_x;
        cfg.y               = a.utilis_y;
        latitudo_fenestrae  = a.utilis_latitudo / f->scala;
        altitudo_fenestrae  = a.utilis_altitudo / f->scala;
    }
    cfg.latitudo  = latitudo_fenestrae * f->scala;
    cfg.altitudo  = altitudo_fenestrae * f->scala;
    fenestra      = fenestra_creare(piscina, &cfg);
    si (!fenestra)
    {
        fprintf(stderr, "%s: fenestra\n", f->titulus);
        redde I;
    }
    tabula = fenestra_creare_tabulam_pixelorum(piscina, fenestra,
                                               altitudo_fenestrae);
    si (tabula && f->plena_visio)
    {
        fenestra_commutare_plenam_visionem(fenestra);
    }
    si (!tabula || !testa_cursum_ludum_struere(cursus, piscina, tabula))
    {
        redde I;
    }

    _currere(cursus->lf, fenestra, o.fumus ? QUADRA_FUMI : ZEPHYRUM);
    testa_cursum_finire(cursus);
    exitus = ZEPHYRUM;
    si (   o.via_imaginis
        && !ludus_fenestra_imaginem_scribere(cursus->lf, o.via_imaginis))
    {
        fprintf(stderr, "%s: imago non scripta: %s\n", f->titulus,
            o.via_imaginis);
        exitus = I;
    }
    fenestra_destruere(fenestra);
    testa_cursum_dimittere(cursus);
    piscina_destruere(piscina);
    redde exitus;
}
```

Order check against today (`apps/pictor/pictor.c:112-124`): loop ->
mensurae line -> `-imago` -> `fenestra_destruere` -> `volumen_claudere`
(now `finis`) -> `piscina_destruere`. Same.

- [ ] **Step 8: Move pictor - replace `apps/pictor/pictor.c` lines 1-125 with**

```c
/* pictor.c - pictor: editor rasterum (P3: tabula + penicillus)
 *
 * Compositio communis (pictor_applicatio) + forma pro testa (testa.h:
 * principale a testa - macOS fenestra, Android NativeActivity);
 * gemellus pictor_terminalis.c (A4 moduli 013). -fumus: volumen
 * temporarium, XXX quadra (testa), exitus; -imago <via> (testa).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "testa.h"
#include "pictor_applicatio.h"
#include <stdio.h>

#define PICTOR_LATITUDO   (DC + XL)
#define PICTOR_ALTITUDO   CDLXXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II

interior vacuum
_pictorem_finire (
    vacuum* ctx)
{
    volumen_claudere((Volumen*)ctx);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
    InternamentumChorda* intern;
                Volumen* vol;
       PictorApplicatio* app;
                    b32  fumus;

    intern = internamentum_creare(a->piscina);
    thema_initiare();
    vol = pictor_volumen_aperire(a->piscina, a->argc, a->argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "pictor: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    /* R3: app in piscina - fons_ctx (&montatio.figurae_ctx) ultra
     * initium vivit */
    app = (PictorApplicatio*)piscina_allocare(a->piscina,
        magnitudo(PictorApplicatio));
    si (   !app
        || !pictor_applicatio_aedificare(app, a->piscina, intern, vol,
               PICTOR_LATITUDO, PICTOR_ALTITUDO))
    {
        redde FALSUM;
    }
    /* L5: Cmd+V imaginem e clipboard (servitium testae) in stratum
     * novum glutinat */
    app->montatio.actiones_ctx.imago_capere  = a->imago_glutinata;
    app->montatio.actiones_ctx.imago_ctx     = a->imago_ctx;

    forma->titulus    = "pictor";
    forma->latitudo   = PICTOR_LATITUDO;
    forma->altitudo   = PICTOR_ALTITUDO;
    forma->scala      = SCALA;
    forma->d          = app->d;
    forma->figurae    = app->figurae;
    forma->thema      = ZEPHYRUM;
    forma->fons       = pictor_imago_fons;
    forma->fons_ctx   = &app->montatio.figurae_ctx;
    forma->finis      = _pictorem_finire;
    forma->finis_ctx  = vol;
    redde VERUM;
}
```

Run: `./apps/pictor/pictor.sh -fumus -imago build/testa/paritas/pictor.nova.png; cmp build/testa/paritas/pictor.vetus.a.png build/testa/paritas/pictor.nova.png && ./apps/pictor/fumus.sh`
Expected: `cmp` silent, `FUMUS PLENUS`. (Fran's OK for the window.)

- [ ] **Step 9: Move scriba - replace `apps/scriba/scriba.c` lines 1-108 with**

```c
/* scriba.c - scriba: pagina vim (folium LXVIII x LVI)
 *
 * Compositio communis (scriba_applicatio) + forma pro testa (testa.h);
 * gemellus scriba_terminalis.c (scriba-plan S3). -fumus: volumen
 * temporarium, XXX quadra (testa), exitus; -volumen <via>
 * (ordinarie scriba.volumen); -imago <via> (testa).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "testa.h"
#include "scriba_applicatio.h"
#include <stdio.h>

/* LXXX x LX cellulae: folium cum margine et linea status */
#define SCRIBA_LATITUDO   CDLXXX
#define SCRIBA_ALTITUDO   CDLXXX
/* pixelum nostrum = II puncta fenestrae (ut terminale) */
#define SCALA             II

interior vacuum
_scribam_finire (
    vacuum* ctx)
{
    volumen_claudere((Volumen*)ctx);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
    InternamentumChorda* intern;
                Volumen* vol;
       ScribaApplicatio* app;
                    b32  fumus;

    intern = internamentum_creare(a->piscina);
    thema_initiare();
    vol = scriba_volumen_aperire(a->piscina, a->argc, a->argv, &fumus);
    si (!vol)
    {
        fprintf(stderr, "scriba: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    app = (ScribaApplicatio*)piscina_allocare(a->piscina,
        magnitudo(ScribaApplicatio));
    si (   !app
        || !scriba_applicatio_aedificare(app, a->piscina, intern, vol,
               SCRIBA_LATITUDO, SCRIBA_ALTITUDO))
    {
        redde FALSUM;
    }
    forma->titulus    = "scriba";
    forma->latitudo   = SCRIBA_LATITUDO;
    forma->altitudo   = SCRIBA_ALTITUDO;
    forma->scala      = SCALA;
    forma->d          = app->d;
    forma->figurae    = app->figurae;
    forma->thema      = ZEPHYRUM;
    forma->finis      = _scribam_finire;
    forma->finis_ctx  = vol;
    redde VERUM;
}
```

Run: `./apps/scriba/scriba.sh -fumus -imago build/testa/paritas/scriba.nova.png; cmp build/testa/paritas/scriba.vetus.a.png build/testa/paritas/scriba.nova.png`
Expected: `cmp` silent, exit 0, `ludus: quadra=30` in the output.

- [ ] **Step 10: Move vicus - replace `apps/vicus/vicus.c` `principale` (lines 89-257) and the clipboard wrapper (lines 60-68) with a host struct + `applicatio_initium`**

Keep lines 1-58 (header comment: replace "in FENESTRA" with "pro
testa (testa.h)"; drop `#include "fenestra.h"`, `"delineare_mandata.h"`,
`"ludus_fenestra.h"`, `"clipboard_platform.h"`, `<stdlib.h>`,
`<string.h>` only if unused after the edit - compile tells), keep
`PulsusVici`/`vicum_pulsare`, keep `pilam_systematis` (energia is a
variant library, spec II.2), add `#include "testa.h"`; delete
`QUADRA_FUMI`. Then:

```c
/* R3: status hospitis totus in piscina testae - monstratores eius
 * (pulsus, portae) in formam et vicum exeunt */
nomen structura {
          VicusApplicatio  app;
    VicusPortaQuaestionis  porta_quaestionis;
           VicusPortaSoni  porta_soni;
               PulsusVici  pulsus;
                  Volumen* vol;
} HospesVici;

interior vacuum
_vicum_finire (
    vacuum* ctx)
{
    volumen_claudere(((HospesVici*)ctx)->vol);
}

b32
applicatio_initium (
    constans TestaAmbitus* a,
              TestaForma* forma)
{
             HospesVici* h;
    InternamentumChorda* intern;
                Reactor* reactor;
                    b32  fumus;
                    b32  per_adiutorem;
                    b32  plena;
                    s32  k;
                    i32  latitudo;
                    i32  altitudo;

    h = (HospesVici*)piscina_allocare(a->piscina, magnitudo(HospesVici));
    si (!h)
    {
        redde FALSUM;
    }
    memset(h, ZEPHYRUM, magnitudo(HospesVici));
    reactor = reactor_creare(a->piscina);
    intern  = internamentum_creare(a->piscina);
    thema_initiare();

    h->vol = vicus_volumen_aperire(a->piscina, a->argc, a->argv, &fumus);
    si (!h->vol)
    {
        fprintf(stderr, "vicus: volumen aperiri non potuit\n");
        redde FALSUM;
    }
    /* S2c: vicus in PLENA VISIONE aperitur (Franus 2026-10-08: sic
     * laborat) - compositio et documenta magnitudine schirmi totius;
     * fenestram in area utili testa ponit (testa.c). fumus: magnitudo
     * fixa, sine plena visione (imagines certae; testa schirmum sub
     * fumo non quaerit). Olim 'plena' sine initio sub fumo legebatur
     * (parcum ...J49JH): nunc semper definitum. */
    plena = !fumus && a->schirmi_latitudo > ZEPHYRUM
                   && a->schirmi_altitudo > ZEPHYRUM;
    latitudo = VICUS_LATITUDO;
    altitudo = VICUS_ALTITUDO;
    si (plena)
    {
        latitudo = a->schirmi_latitudo / SCALA;
        altitudo = a->schirmi_altitudo / SCALA;
    }
    si (!vicus_applicatio_aedificare(&h->app, a->piscina, intern, h->vol,
            latitudo, altitudo))
    {
        redde FALSUM;
    }
    /* S4: horologium locale in linea tabularum */
    vicus_horologium_ponere(h->app.vicus, vicus_horologium_locale, NIHIL);
    /* S5: pila (IOKit; machina mensae: nulla) */
    vicus_pilam_ponere(h->app.vicus, pilam_systematis, NIHIL);
    /* T4a: admonitiones ex libro praeconii (colloquia Claude Code) */
    (vacuum)vicus_applicatio_admonitiones_aperire(&h->app,
        praeconium_via(a->piscina));
    /* sonus S4: admonitiones sonantes; -fumus tacet */
    si (   !fumus && vicus_sonus_porta(&h->porta_soni, a->piscina,
            (i64)time(NIHIL)))
    {
        vicus_applicatio_soni_portam_ponere(&h->app, &h->porta_soni);
    }
    /* L5: Cmd+V in pictore: imago e clipboard (servitium testae) */
    vicus_applicatio_imaginis_portam_ponere(&h->app, a->imago_glutinata,
        a->imago_ctx);
    /* $question: in processu (via B) nisi -quaestio adiutor (via A) */
    per_adiutorem = FALSUM;
    per (k = I; k + I < a->argc; k++)
    {
        si (   strcmp(a->argv[k], "-quaestio")   == ZEPHYRUM
            && strcmp(a->argv[k + I], "adiutor") == ZEPHYRUM)
        {
            per_adiutorem = VERUM;
        }
    }
    h->porta_quaestionis = per_adiutorem
        ? vicus_quaestio_porta(vicus_quaestio_adiutoris_via(
              a->argc > ZEPHYRUM ? a->argv[ZEPHYRUM] : NIHIL, a->piscina),
              ZEPHYRUM, a->piscina)
        : vicus_quaestio_interna(reactor, NIHIL, a->piscina);
    vicus_applicatio_quaestionis_portam_ponere(&h->app,
        &h->porta_quaestionis);

    h->pulsus.vicus    = h->app.vicus;
    h->pulsus.reactor  = reactor;
    forma->titulus       = "vicus";
    forma->latitudo      = latitudo;
    forma->altitudo      = altitudo;
    forma->scala         = SCALA;
    forma->plena_visio   = plena;
    forma->d             = h->app.d;
    forma->figurae       = vicus_figurae(h->app.vicus);
    forma->thema         = ZEPHYRUM;
    forma->fons          = vicus_imago_fons;
    forma->fons_ctx      = h->app.vicus;
    forma->pulsator      = vicum_pulsare;
    forma->pulsator_ctx  = &h->pulsus;
    forma->finis         = _vicum_finire;
    forma->finis_ctx     = h;
    redde VERUM;
}
```

Run: `./apps/vicus/vicus.sh -fumus -imago build/testa/paritas/vicus.nova.png; cmp build/testa/paritas/vicus.vetus.a.png build/testa/paritas/vicus.nova.png`
Expected: `cmp` silent if Step 1 said DETERMINATUM; otherwise the
ledgered Step-1 ruling (exit 0, `ludus: quadra=30`, PNG read and
compared by eye with `vicus.vetus.a.png`). Then ask Fran to open vicus
normally: fullscreen, tabs, pictor pane, Cmd+Q (Review Focus 5).

- [ ] **Step 11: Retire `ludus_fenestra_currere` from the public API**

- `include/ludus_fenestra.h`: delete the declaration and its comment
  (lines 96-105, "ansa vera: ..."); in the header comment add one line:
  "Ansa ipsa testae est (lib/testa.c; testa-spec II.3)."
- `lib/ludus_fenestra.c`: delete `MORA_PULSUS` (lines 11-12) and
  `ludus_fenestra_currere` (lines 213-277); `#include <stdio.h>` stays
  only if still used (compile tells).
- Check no caller is left:

Run: `./silva/nexus.sh ludus_fenestra_currere`
Expected: no callers (worklogs may still mention it - history, keep).

- `lib/ludus_fenestra.interrogationes.stml`: the "how do I run an app's
  dispatcher in a real window?" entry (lines 3-16) moves to a new
  `lib/testa_cursus.interrogationes.stml` (unitas `testa_cursus`),
  rewritten as:

```xml
<interrogationes unitas="testa_cursus">

  <interrogatio genus="quomodo">
    <quaestio! (>how do I run an app's dispatcher in a real window?
    <responsum symbolum="applicatio_initium testa_cursum_incipere testa_cursum_ludum_struere" via="apps/pictor/pictor.c">Include
      testa.h and define applicatio_initium(a, forma): build your
      state in a->piscina (never in locals - their addresses outlive
      the call), then fill the forma - titulus, latitudo, altitudo,
      scala, d, figurae, optional fons/fons_ctx, pulsator and finis.
      The shell (lib/testa.c on macOS, lib/testa_android.c on
      Android) owns principale, the window, the loop, -fumus (XXX
      frames) and -imago, and calls finis once at the end. A forma
      failing testa_formam_probare stops the run with its causa named;
      finis still runs.</responsum>
  </interrogatio>

</interrogationes>
```

  and the "can I use quadra_maxima for a smoke run ..." entry (lines
  80-92) keeps its facts with `symbolum="ludus_fenestra_pingendum
  ludus_fenestra_mora"` and "the shell's loop (lib/testa.c)" in place
  of "ludus_fenestra_currere".

Run: `tools/interrogationes_fumus.sh`
Expected: no dead anchors.

- [ ] **Step 12: Whole-task checks, then commit**

```bash
./compile_tests.sh probatio_testa
./compile_tests.sh probatio_ludus_fenestra
./compile_tests.sh probatio_ludus_imago
./tools/portae_debitae.sh
```

Expected: the three suites exit 0; `portae_debitae` lists the gates
commissio will run (generata regenerates `compile_tests_fontes_
generata.sh` and `docs/faq.html` for the new `lib/` files - commissio
heals by plan). Fran's eye on the three apps (Steps 8-10). Then:

```python
silva.commissio('testa T1: contractus applicationis (testa.h + testa_cursus) et testa macOS (lib/testa.c possidet principale et ansam olim ludus_fenestra_currere); pictor, scriba, vicus applicatio_initium definiunt (status in piscina testae, R3) - imagines -fumus octetim eaedem; vicus plena sine initio (...J49JH) clausum', [
  'include/testa_cursus.h', 'lib/testa_cursus.c', 'include/testa.h',
  'lib/testa.c', 'probationes/probatio_testa.c',
  'apps/pictor/pictor.c', 'apps/scriba/scriba.c', 'apps/vicus/vicus.c',
  'include/ludus_fenestra.h', 'lib/ludus_fenestra.c',
  'lib/ludus_fenestra.interrogationes.stml',
  'lib/testa_cursus.interrogationes.stml'])
```

(`git add -N` the new files first. Close park ...J49JH in the ledger.)

---

### Task 2: `postulata_android.h` and the Android shell's pure pieces

**Files:**
- Create: `include/postulata_android.h`, `include/testa_android_initus.h`, `lib/testa_android_initus.c`, `probationes/probatio_testa_android_initus.c`

**Interfaces:**
- Consumes: `Eventus`, `EventusExemplum`, `EVENTUS_*`, `MUS_SINISTER`, `EVENTUS_INDICATOR_*`, `EVENTUS_PRESSIO_IGNOTA`, `EVENTUS_EXEMPLA_MAXIMA` (`include/eventus.h`); `TabulaPixelorum` (`include/tabula_pixelorum.h`); `Mora` (`include/fasti.h`); T1 nothing.
- Produces (T3 uses exactly these):
  - constants `TESTA_TACTUS_{DEPRESSIO,LIBERATIO,MOTUS,ABRUPTIO,INDICATOR_DEPRESSIO,INDICATOR_LIBERATIO}` = 0,1,2,3,5,6; `TESTA_INSTRUMENTUM_{DIGITUS,STILUS,MUS,GUMMI}` = 1,2,3,4; `TESTA_INDICATORES_MAXIMI` = X
  - `TestaGeometria { i32 fenestra_latitudo, fenestra_altitudo, scala, latitudo, altitudo, margo_x, margo_y; }`
  - `b32 testa_geometriam_computare(i32 fenestra_latitudo, i32 fenestra_altitudo, i32 scala, TestaGeometria* g)`
  - `vacuum testa_punctum_mappare(constans TestaGeometria* g, f32 x, f32 y, s32* nx, s32* ny)`
  - `TestaTactusCrudus { s32 actio; i32 index_actionis; i32 numerus; s32 ids[X]; s32 instrumenta[X]; f32 x[X]; f32 y[X]; s64 tempus; i32 numerus_historiae; f32 historia_x[X][EVENTUS_EXEMPLA_MAXIMA]; f32 historia_y[X][EVENTUS_EXEMPLA_MAXIMA]; s64 historia_tempus[EVENTUS_EXEMPLA_MAXIMA]; }`
  - `TestaTactus { b32 activus; b32 exspectat; s32 indicator; EventusExemplum exempla[EVENTUS_EXEMPLA_MAXIMA]; }`
  - `b32 testa_tactum_vertere(TestaTactus* s, constans TestaTactusCrudus* c, constans TestaGeometria* g, Eventus* ev)`
  - `vacuum testa_facultates_tactus(Eventus* ev, s64 tempus)`
  - `TestaQuadriGenus { TESTA_QUADRUM_NULLUM, TESTA_QUADRUM_STATIM, TESTA_QUADRUM_POST }`, `TestaQuadriPetitio { TestaQuadriGenus genus; Mora post_ms; }`, `TestaQuadriStatus { b32 fenestra, vivus, pendens_statim, pendens_post, debitum; Mora mora; b32 pulsus; i32 fumus_reliqua; }`, `TestaQuadriPetitio testa_quadrum_petere(constans TestaQuadriStatus* s)`
  - `vacuum testa_pixela_transferre(constans TabulaPixelorum* t, constans TestaGeometria* g, i32* destinatio, i32 gradus, i32 color_marginis)`

- [ ] **Step 1: Write `include/postulata_android.h`**

```c
#ifndef POSTULATA_ANDROID_H
#define POSTULATA_ANDROID_H

/* postulata_android.h - in omni .c Android solo SECUNDUM (post
 * postulata_posix.h), ante capita NDK. Capita NDK 'inline' C99 nudum
 * ferunt; sub -std=c89 (regulae GNU89) 'inline' nudum definitionem
 * EXTERNAM emittit: ANativeWindow_clearFrameRate in .so effugit et
 * symbolum API XXXI traxit - dlopen in API XXX fractum (spica
 * 2026-10-22, llvm-nm -D). 'static __inline__' functiones capitum
 * intra unitatem tenet. */

#define inline static __inline__

#endif /* POSTULATA_ANDROID_H */
```

- [ ] **Step 2: Write `include/testa_android_initus.h`**

```c
#ifndef TESTA_ANDROID_INITUS_H
#define TESTA_ANDROID_INITUS_H

/* testa_android_initus.h - partes PURAE testae Android (testa-spec
 * II.5-II.6): geometria (scala, margo), tactus -> Eventus, decisio
 * quadri, transfusio pixelorum. Valores plani intrant et exeunt -
 * nullum caput NDK: in hospite et in tabula probantur. Constantae
 * NDK hic reddita (android/input.h, NDK r27d); lib/testa_android.c
 * congruentiam in compilatione probat. */

#include "latina.h"
#include "eventus.h"
#include "fasti.h"
#include "tabula_pixelorum.h"

/* AMOTION_EVENT_ACTION_* (actio & AMOTION_EVENT_ACTION_MASK) */
#define TESTA_TACTUS_DEPRESSIO              ZEPHYRUM  /* _DOWN */
#define TESTA_TACTUS_LIBERATIO              I         /* _UP */
#define TESTA_TACTUS_MOTUS                  II        /* _MOVE */
#define TESTA_TACTUS_ABRUPTIO               III       /* _CANCEL */
#define TESTA_TACTUS_INDICATOR_DEPRESSIO    V         /* _POINTER_DOWN */
#define TESTA_TACTUS_INDICATOR_LIBERATIO    VI        /* _POINTER_UP */

/* AMOTION_EVENT_TOOL_TYPE_* */
#define TESTA_INSTRUMENTUM_DIGITUS  I     /* _FINGER */
#define TESTA_INSTRUMENTUM_STILUS   II    /* _STYLUS */
#define TESTA_INSTRUMENTUM_MUS      III   /* _MOUSE */
#define TESTA_INSTRUMENTUM_GUMMI    IV    /* _ERASER (stili cauda) */

#define TESTA_INDICATORES_MAXIMI    X     /* plures praetermittuntur */


/* ==================================================
 * Geometria: fenestra (pixela tabulae Android) -> pixela nostra
 * ================================================== */

/* latitudo = fenestra / scala (pavimentum) - IDEM ac
 * tabula_pixelorum_ad_fenestram; residuum (< scala) margo est,
 * centratus (pars minor sinistra/summa) */
nomen structura {
    i32 fenestra_latitudo;
    i32 fenestra_altitudo;
    i32 scala;
    i32 latitudo;      /* pixela nostra */
    i32 altitudo;
    i32 margo_x;       /* pixela fenestrae */
    i32 margo_y;
} TestaGeometria;

/* FALSUM si scala < I aut fenestra minor scala */
b32
testa_geometriam_computare (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               i32  scala,
    TestaGeometria* g);

/* punctum fenestrae -> pixela nostra, PAVIMENTO (negativa recte):
 * in margine extra [0, latitudo) cadit, ut mus extra fenestram (A3c) */
vacuum
testa_punctum_mappare (
    constans TestaGeometria* g,
                        f32  x,
                        f32  y,
                        s32* nx,
                        s32* ny);


/* ==================================================
 * Tactus -> Eventus (digitus primus solus ducit, B1)
 * ================================================== */

/* AMotionEvent in valores planos (involucrum in testa_android.c) */
nomen structura {
    s32  actio;                         /* TESTA_TACTUS_* */
    i32  index_actionis;                /* indicator quem actio tangit */
    i32  numerus;                       /* indicatores (<= MAXIMI) */
    s32  ids[TESTA_INDICATORES_MAXIMI];
    s32  instrumenta[TESTA_INDICATORES_MAXIMI];
    f32  x[TESTA_INDICATORES_MAXIMI];
    f32  y[TESTA_INDICATORES_MAXIMI];
    s64  tempus;                        /* ms (CLOCK_MONOTONIC) */
    i32  numerus_historiae;             /* <= EVENTUS_EXEMPLA_MAXIMA */
    f32  historia_x[TESTA_INDICATORES_MAXIMI][EVENTUS_EXEMPLA_MAXIMA];
    f32  historia_y[TESTA_INDICATORES_MAXIMI][EVENTUS_EXEMPLA_MAXIMA];
    s64  historia_tempus[EVENTUS_EXEMPLA_MAXIMA];
} TestaTactusCrudus;

/* status inter eventus; exempla = repositorium VISUS eventus ultimi
 * (valet donec vertere iterum vocatur) */
nomen structura {
                b32  activus;     /* ducens depressus */
                b32  exspectat;   /* ducens liberatus, alii adhuc */
                s32  indicator;   /* id ducentis */
    EventusExemplum  exempla[EVENTUS_EXEMPLA_MAXIMA];
} TestaTactus;

/* VERUM + ev impletus, aut FALSUM (nihil tradendum) */
b32
testa_tactum_vertere (
                 TestaTactus* s,
    constans TestaTactusCrudus* c,
       constans TestaGeometria* g,
                     Eventus* ev);

/* EVENTUS_FACULTATES tactus: super (hover) FALSUM, pressio FALSUM
 * (pressio digiti Fire 0.10 synthetica, spica) */
vacuum
testa_facultates_tactus (
    Eventus* ev,
        s64  tempus);


/* ==================================================
 * Decisio quadri (R4: ansam macOS imitatur)
 * ================================================== */

nomen enumeratio {
    TESTA_QUADRUM_NULLUM = ZEPHYRUM,
    TESTA_QUADRUM_STATIM,
    TESTA_QUADRUM_POST
} TestaQuadriGenus;

nomen structura {
    TestaQuadriGenus  genus;
                Mora  post_ms;    /* POST solum */
} TestaQuadriPetitio;

nomen structura {
     b32  fenestra;           /* fenestra adest */
     b32  vivus;              /* resumptus, non in pausa */
     b32  pendens_statim;     /* vocatio statim iam petita */
     b32  pendens_post;       /* vocatio dilata iam petita */
     b32  debitum;            /* eventa post quadrum ultimum */
    Mora  mora;               /* ludus_fenestra_mora */
     b32  pulsus;             /* forma pulsum habet */
     i32  fumus_reliqua;      /* quadra fumi restantia; 0 = nullus */
} TestaQuadriStatus;

TestaQuadriPetitio
testa_quadrum_petere (
    constans TestaQuadriStatus* s);


/* ==================================================
 * Transfusio: tabula (pixela nostra) -> alveus fenestrae
 * ================================================== */

/* proximus vicinus, scala integra, margo colore; gradus = pixela per
 * lineam alvei (>= fenestra_latitudo). RGBA_8888: octeti R,G,B,A in
 * memoria - forma tabulae ipsa */
vacuum
testa_pixela_transferre (
    constans TabulaPixelorum* t,
     constans TestaGeometria* g,
                        i32* destinatio,
                         i32  gradus,
                         i32  color_marginis);

#endif /* TESTA_ANDROID_INITUS_H */
```

- [ ] **Step 3: Write the failing suite `probationes/probatio_testa_android_initus.c`**

```c
/* probatio_testa_android_initus.c - partes purae testae Android
 * (testa-spec II.5-II.6, III.3): geometria, margo, tactus, digiti
 * duo, abruptio, facultates, decisio quadri, transfusio. In hospite et
 * in tabula (tools/android/probatio.sh testa_android_initus). */
#include "latina.h"
#include "piscina.h"
#include "eventus.h"
#include "delineare_mandata.h"
#include "testa_android_initus.h"
#include "credo.h"
#include <string.h>

interior vacuum
_crudum (
    TestaTactusCrudus* c,
                  s32  actio,
                  i32  index_actionis,
                  s64  tempus)
{
    memset(c, ZEPHYRUM, magnitudo(TestaTactusCrudus));
    c->actio           = actio;
    c->index_actionis  = index_actionis;
    c->tempus          = tempus;
}

/* indicatorem i in crudo ponere */
interior vacuum
_indicator (
    TestaTactusCrudus* c,
                  i32  i,
                  s32  id,
                  s32  instrumentum,
                  f32  x,
                  f32  y)
{
    c->ids[i]          = id;
    c->instrumenta[i]  = instrumentum;
    c->x[i]            = x;
    c->y[i]            = y;
    si (c->numerus < i + I)
    {
        c->numerus = i + I;
    }
}

interior vacuum
_sectio_geometria (
    CredoContextus* c)
{
     TestaGeometria  g;
    TabulaPixelorum* t;

    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    CREDO_AEQUALIS_I32(g.latitudo, DXII);
    CREDO_AEQUALIS_I32(g.altitudo, CCC);
    CREDO_AEQUALIS_I32(g.margo_x, ZEPHYRUM);
    CREDO_AEQUALIS_I32(g.margo_y, ZEPHYRUM);

    CREDO_GRADUS("congruit cum tabula_pixelorum_ad_fenestram");
    t = tabula_pixelorum_creare_nuda(c->piscina, X, X);
    t->scala = 2.0f;
    CREDO_VERUM(tabula_pixelorum_ad_fenestram(t, c->piscina, MXXIV, DC));
    CREDO_AEQUALIS_I32(t->latitudo, g.latitudo);
    CREDO_AEQUALIS_I32(t->altitudo, g.altitudo);

    CREDO_GRADUS("residuum II sub scala III: margo I centratus");
    CREDO_VERUM(testa_geometriam_computare(MXXV, DCII, III, &g));
    CREDO_AEQUALIS_I32(g.latitudo, CCCXLI);
    CREDO_AEQUALIS_I32(g.altitudo, CC);
    CREDO_AEQUALIS_I32(g.margo_x, I);
    CREDO_AEQUALIS_I32(g.margo_y, I);

    CREDO_GRADUS("pravae: scala ZEPHYRUM, fenestra minor scala");
    CREDO_FALSUM(testa_geometriam_computare(MXXIV, DC, ZEPHYRUM, &g));
    CREDO_FALSUM(testa_geometriam_computare(I, I, II, &g));
}

/* Review Focus 1: tactus in margine extra [0, latitudo) */
interior vacuum
_sectio_margo (
    CredoContextus* c)
{
    TestaGeometria  g;
               s32  x;
               s32  y;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXV, DCII, III, &g));
    testa_punctum_mappare(&g, 0.0f, 0.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, -I);
    CREDO_AEQUALIS_S32(y, -I);
    testa_punctum_mappare(&g, 1.0f, 1.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(y, ZEPHYRUM);
    testa_punctum_mappare(&g, 1023.9f, 600.5f, &x, &y);
    CREDO_AEQUALIS_S32(x, CCCXL);
    CREDO_AEQUALIS_S32(y, CXCIX);
    CREDO_GRADUS("margo dexter: x == latitudo (extra)");
    testa_punctum_mappare(&g, 1024.0f, 601.0f, &x, &y);
    CREDO_AEQUALIS_S32(x, CCCXLI);
    CREDO_AEQUALIS_S32(y, CC);
    CREDO_GRADUS("negativum pavimento, non truncatione");
    testa_punctum_mappare(&g, -2.5f, -0.5f, &x, &y);
    CREDO_AEQUALIS_S32(x, -II);
    CREDO_AEQUALIS_S32(y, -I);
}

interior vacuum
_sectio_tactus (
    CredoContextus* c)
{
       TestaGeometria  g;
          TestaTactus  s;
    TestaTactusCrudus  r;
              Eventus  e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));

    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 100.0f,
        50.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_DEPRESSUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, L);
    CREDO_AEQUALIS_S32(e.datum.mus.y, XXV);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.botton, (i32)MUS_SINISTER);
    CREDO_AEQUALIS_S32(e.datum.mus.indicator, VII);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_TACTUS);
    CREDO_AEQUALIS_S32(e.datum.mus.pressio, EVENTUS_PRESSIO_IGNOTA);
    CREDO_AEQUALIS_S64(e.tempus, (s64)M);

    CREDO_GRADUS("motus cum historia: exempla II, botton tentus");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)(M + XX));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 120.0f,
        60.0f);
    r.numerus_historiae          = II;
    r.historia_x[ZEPHYRUM][ZEPHYRUM] = 104.0f;
    r.historia_y[ZEPHYRUM][ZEPHYRUM] = 52.0f;
    r.historia_tempus[ZEPHYRUM]  = (s64)(M + V);
    r.historia_x[ZEPHYRUM][I]    = 110.0f;
    r.historia_y[ZEPHYRUM][I]    = 56.0f;
    r.historia_tempus[I]         = (s64)(M + XII);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_MOTUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, LX);
    CREDO_AEQUALIS_I32((i32)e.datum.mus.botton, (i32)MUS_SINISTER);
    CREDO_AEQUALIS_I32(e.datum.mus.numerus_exemplorum, II);
    CREDO_NON_NIHIL(e.datum.mus.exempla);
    CREDO_AEQUALIS_S32(e.datum.mus.exempla[ZEPHYRUM].x, LII);
    CREDO_AEQUALIS_S32(e.datum.mus.exempla[I].y, XXVIII);
    CREDO_AEQUALIS_S64(e.datum.mus.exempla[I].tempus, (s64)(M + XII));

    CREDO_GRADUS("liberatio; motus post eam nihil");
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)(M + XL));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 120.0f,
        60.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)(M + L));
    _indicator(&r, ZEPHYRUM, VII, TESTA_INSTRUMENTUM_DIGITUS, 130.0f,
        60.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("instrumenta: stilus -> STILUS, mus -> MUS");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_STILUS, 2.0f, 2.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_STILUS);
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_STILUS, 2.0f, 2.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)MM);
    _indicator(&r, ZEPHYRUM, II, TESTA_INSTRUMENTUM_MUS, 2.0f, 2.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.datum.mus.indicator_genus,
        (i32)EVENTUS_INDICATOR_MUS);
}

/* Review Focus 3: digitus secundus praetermittitur */
interior vacuum
_sectio_digiti_duo (
    CredoContextus* c)
{
       TestaGeometria  g;
          TestaTactus  s;
    TestaTactusCrudus  r;
              Eventus  e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("indicator secundus deprimitur: nihil");
    _crudum(&r, TESTA_TACTUS_INDICATOR_DEPRESSIO, I, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    _indicator(&r, I, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f, 300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("motus amborum: positio PRIMI (ordine permutato)");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f,
        300.0f);
    _indicator(&r, I, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS, 20.0f,
        30.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_S32(e.datum.mus.x, X);
    CREDO_AEQUALIS_S32(e.datum.mus.y, XV);

    CREDO_GRADUS("secundus liberatur: nihil");
    _crudum(&r, TESTA_TACTUS_INDICATOR_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f,
        300.0f);
    _indicator(&r, I, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS, 20.0f,
        30.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("primus liberatur dum alius manet: LIBERATUS");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        10.0f, 10.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_INDICATOR_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, ZEPHYRUM, TESTA_INSTRUMENTUM_DIGITUS,
        12.0f, 10.0f);
    _indicator(&r, I, I, TESTA_INSTRUMENTUM_DIGITUS, 500.0f, 300.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    CREDO_AEQUALIS_S32(e.datum.mus.x, VI);

    CREDO_GRADUS("reliquus movetur, deinde liberatur: nihil");
    _crudum(&r, TESTA_TACTUS_MOTUS, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 510.0f,
        300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));
    _crudum(&r, TESTA_TACTUS_LIBERATIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, I, TESTA_INSTRUMENTUM_DIGITUS, 510.0f,
        300.0f);
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));

    CREDO_GRADUS("omnibus sublatis tactus novus ducit");
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, II, TESTA_INSTRUMENTUM_DIGITUS, 40.0f,
        40.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_S32(e.datum.mus.indicator, II);
}

/* Review Focus 2: abruptio (tabula clausa media linea) */
interior vacuum
_sectio_abruptio (
    CredoContextus* c)
{
       TestaGeometria  g;
          TestaTactus  s;
    TestaTactusCrudus  r;
              Eventus  e;

    (vacuum)c;
    CREDO_VERUM(testa_geometriam_computare(MXXIV, DC, II, &g));
    memset(&s, ZEPHYRUM, magnitudo(TestaTactus));
    _crudum(&r, TESTA_TACTUS_DEPRESSIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, III, TESTA_INSTRUMENTUM_DIGITUS, 10.0f,
        10.0f);
    (vacuum)testa_tactum_vertere(&s, &r, &g, &e);
    _crudum(&r, TESTA_TACTUS_ABRUPTIO, ZEPHYRUM, (s64)M);
    _indicator(&r, ZEPHYRUM, III, TESTA_INSTRUMENTUM_DIGITUS, 14.0f,
        10.0f);
    CREDO_VERUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_MUS_LIBERATUS);
    CREDO_GRADUS("abruptio iterata: nihil (botton non bis liberatur)");
    CREDO_FALSUM(testa_tactum_vertere(&s, &r, &g, &e));
    CREDO_FALSUM(s.activus);
}

interior vacuum
_sectio_facultates (
    CredoContextus* c)
{
    Eventus e;

    (vacuum)c;
    testa_facultates_tactus(&e, (s64)M);
    CREDO_AEQUALIS_I32((i32)e.genus, (i32)EVENTUS_FACULTATES);
    CREDO_FALSUM(e.datum.facultates.super);
    CREDO_FALSUM(e.datum.facultates.pressio);
    CREDO_AEQUALIS_S64(e.tempus, (s64)M);
}

interior TestaQuadriPetitio
_petere (
     b32  fenestra,
     b32  vivus,
     b32  pendens_statim,
     b32  pendens_post,
     b32  debitum,
    Mora  mora,
     b32  pulsus,
     i32  fumus)
{
    TestaQuadriStatus s;

    s.fenestra        = fenestra;
    s.vivus           = vivus;
    s.pendens_statim  = pendens_statim;
    s.pendens_post    = pendens_post;
    s.debitum         = debitum;
    s.mora            = mora;
    s.pulsus          = pulsus;
    s.fumus_reliqua   = fumus;
    redde testa_quadrum_petere(&s);
}

interior vacuum
_sectio_quadrum (
    CredoContextus* c)
{
    TestaQuadriPetitio p;

    (vacuum)c;
    CREDO_GRADUS("sine fenestra aut in pausa: nullum");
    p = _petere(FALSUM, VERUM, FALSUM, FALSUM, VERUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    p = _petere(VERUM, FALSUM, FALSUM, FALSUM, VERUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("otiosa: post moram (quies 300)");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_POST);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)CCC);
    CREDO_GRADUS("ante quadrum primum (mora 0): statim");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, ZEPHYRUM, FALSUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
    CREDO_GRADUS("pendens post, debitum -> statim (Review Focus 4)");
    p = _petere(VERUM, VERUM, FALSUM, VERUM, VERUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
    CREDO_GRADUS("pendens statim: nihil iterum");
    p = _petere(VERUM, VERUM, VERUM, FALSUM, VERUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("pendens post, otiosa: nihil iterum");
    p = _petere(VERUM, VERUM, FALSUM, VERUM, FALSUM, CCC, FALSUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_NULLUM);
    CREDO_GRADUS("pulsus: XVI ad summum (mora 300 et 0)");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, VERUM, ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_POST);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)XVI);
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, ZEPHYRUM, VERUM,
        ZEPHYRUM);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)XVI);
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, X, VERUM, ZEPHYRUM);
    CREDO_AEQUALIS_S64(p.post_ms, (s64)X);
    CREDO_GRADUS("fumus: statim donec quadra restant");
    p = _petere(VERUM, VERUM, FALSUM, FALSUM, FALSUM, CCC, FALSUM, III);
    CREDO_AEQUALIS_I32((i32)p.genus, (i32)TESTA_QUADRUM_STATIM);
}

interior vacuum
_sectio_transferre (
    CredoContextus* c)
{
     TestaGeometria  g;
    TabulaPixelorum* t;
                i32  alveus[IX * VIII];
                i32  speratum[VIII];
                i32  y;
                i32  x;

    t = tabula_pixelorum_creare_nuda(c->piscina, II, II);
    t->pixela[ZEPHYRUM]  = I;
    t->pixela[I]         = II;
    t->pixela[II]        = III;
    t->pixela[III]       = IV;
    CREDO_VERUM(testa_geometriam_computare(VIII, VIII, III, &g));
    per (x = ZEPHYRUM; x < IX * VIII; x++)
    {
        alveus[x] = 0xABu;
    }
    testa_pixela_transferre(t, &g, alveus, IX, 0x77u);
    CREDO_GRADUS("lineae 0 et VII margo; columna VIII (gradus) intacta");
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        CREDO_AEQUALIS_I32(alveus[y * IX + VIII], 0xABu);
        si (y == ZEPHYRUM || y == VII)
        {
            per (x = ZEPHYRUM; x < VIII; x++)
            {
                CREDO_AEQUALIS_I32(alveus[y * IX + x], 0x77u);
            }
            perge;
        }
        speratum[ZEPHYRUM]  = 0x77u;
        speratum[VII]       = 0x77u;
        per (x = I; x < VII; x++)
        {
            speratum[x] = (y < IV ? I : III) + (x < IV ? ZEPHYRUM : I);
        }
        per (x = ZEPHYRUM; x < VIII; x++)
        {
            CREDO_AEQUALIS_I32(alveus[y * IX + x], speratum[x]);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "geometria", _sectio_geometria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "margo", _sectio_margo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tactus", _sectio_tactus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "digiti duo", _sectio_digiti_duo, NIHIL, NIHIL, NIHIL, NIHIL },
    { "abruptio", _sectio_abruptio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "facultates", _sectio_facultates, NIHIL, NIHIL, NIHIL, NIHIL },
    { "decisio quadri", _sectio_quadrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "transfusio", _sectio_transferre, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("testa_android_initus", SECTIONES);
}
```

Before running, cast every enum compared with `CREDO_AEQUALIS_I32` to
`(i32)` (`e.genus`, `e.datum.mus.botton`, `e.datum.mus.indicator_genus`
and their constants) - `-Wsign-conversion`.

- [ ] **Step 4: Run it - expect a link failure**

Run: `./compile_tests.sh probatio_testa_android_initus`
Expected: FAIL - undefined `testa_geometriam_computare` etc. (header
exists, `lib/testa_android_initus.c` does not).

- [ ] **Step 5: Write `lib/testa_android_initus.c`**

```c
/* testa_android_initus.c - partes purae testae Android (testa-spec
 * II.5-II.6; testa_android_initus.h) */

#include "testa_android_initus.h"

#include <string.h>

#define MORA_PULSUS  XVI   /* ut lib/testa.c (R4) */

/* pavimentum sine libm: (s32)v truncat versus ZEPHYRUM */
interior s32
_pavimentum (
    f32 v)
{
    s32 t;

    t = (s32)v;
    si (v < (f32)t)
    {
        t -= I;
    }
    redde t;
}

/* divisio pavimento (a negativum: -1/3 = -1, non 0) */
interior s32
_divisio_pavimenti (
    s32 a,
    s32 b)
{
    s32 q;

    q = a / b;
    si ((a % b) != ZEPHYRUM && a < ZEPHYRUM)
    {
        q -= I;
    }
    redde q;
}

b32
testa_geometriam_computare (
               i32  fenestra_latitudo,
               i32  fenestra_altitudo,
               i32  scala,
    TestaGeometria* g)
{
    si (   !g || scala < I || fenestra_latitudo < scala
        || fenestra_altitudo < scala)
    {
        redde FALSUM;
    }
    g->fenestra_latitudo  = fenestra_latitudo;
    g->fenestra_altitudo  = fenestra_altitudo;
    g->scala              = scala;
    g->latitudo           = fenestra_latitudo / scala;
    g->altitudo           = fenestra_altitudo / scala;
    g->margo_x = (fenestra_latitudo - g->latitudo * scala) / II;
    g->margo_y = (fenestra_altitudo - g->altitudo * scala) / II;
    redde VERUM;
}

vacuum
testa_punctum_mappare (
    constans TestaGeometria* g,
                        f32  x,
                        f32  y,
                        s32* nx,
                        s32* ny)
{
    *nx = _divisio_pavimenti(_pavimentum(x) - (s32)g->margo_x,
        (s32)g->scala);
    *ny = _divisio_pavimenti(_pavimentum(y) - (s32)g->margo_y,
        (s32)g->scala);
}

interior s32
_indicem_invenire (
    constans TestaTactusCrudus* c,
                          s32  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < c->numerus && i < TESTA_INDICATORES_MAXIMI;
         i++)
    {
        si (c->ids[i] == id)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior EventusIndicatorGenus
_genus_instrumenti (
    s32 instrumentum)
{
    si (   instrumentum == TESTA_INSTRUMENTUM_STILUS
        || instrumentum == TESTA_INSTRUMENTUM_GUMMI)
    {
        redde EVENTUS_INDICATOR_STILUS;
    }
    si (instrumentum == TESTA_INSTRUMENTUM_MUS)
    {
        redde EVENTUS_INDICATOR_MUS;
    }
    redde EVENTUS_INDICATOR_TACTUS;
}

/* eventum muris ex indicatore i crudi implere (botton sinister:
 * tactus = sinistra pressa, etiam in motu - spec D1) */
interior vacuum
_murem_implere (
                     Eventus* ev,
              eventus_genus_t  genus,
    constans TestaTactusCrudus* c,
                          s32  i,
                          s32  indicator,
       constans TestaGeometria* g)
{
    s32 x;
    s32 y;

    memset(ev, ZEPHYRUM, magnitudo(Eventus));
    testa_punctum_mappare(g, c->x[i], c->y[i], &x, &y);
    ev->genus                       = genus;
    ev->tempus                      = c->tempus;
    ev->datum.mus.x                 = x;
    ev->datum.mus.y                 = y;
    ev->datum.mus.botton            = MUS_SINISTER;
    ev->datum.mus.indicator         = indicator;
    ev->datum.mus.indicator_genus   = _genus_instrumenti(c->instrumenta[i]);
    ev->datum.mus.pressio           = EVENTUS_PRESSIO_IGNOTA;
    ev->datum.mus.exempla           = NIHIL;
    ev->datum.mus.numerus_exemplorum = ZEPHYRUM;
}

b32
testa_tactum_vertere (
                 TestaTactus* s,
    constans TestaTactusCrudus* c,
       constans TestaGeometria* g,
                     Eventus* ev)
{
    s32 i;
    i32 h;
    i32 n;
    b32 erat;

    si (c->numerus < I)
    {
        redde FALSUM;
    }
    commutatio (c->actio)
    {
        casus TESTA_TACTUS_DEPRESSIO:
            si ((i32)c->index_actionis >= c->numerus)
            {
                redde FALSUM;
            }
            i             = (s32)c->index_actionis;
            s->activus    = VERUM;
            s->exspectat  = FALSUM;
            s->indicator  = c->ids[i];
            _murem_implere(ev, EVENTUS_MUS_DEPRESSUS, c, i, s->indicator,
                g);
            redde VERUM;

        casus TESTA_TACTUS_MOTUS:
            si (!s->activus)
            {
                redde FALSUM;
            }
            i = _indicem_invenire(c, s->indicator);
            si (i < ZEPHYRUM)
            {
                redde FALSUM;
            }
            _murem_implere(ev, EVENTUS_MUS_MOTUS, c, i, s->indicator, g);
            n = c->numerus_historiae;
            si (n > EVENTUS_EXEMPLA_MAXIMA)
            {
                n = EVENTUS_EXEMPLA_MAXIMA;
            }
            per (h = ZEPHYRUM; h < n; h++)
            {
                testa_punctum_mappare(g, c->historia_x[i][h],
                    c->historia_y[i][h], &s->exempla[h].x,
                    &s->exempla[h].y);
                s->exempla[h].tempus = c->historia_tempus[h];
            }
            si (n > ZEPHYRUM)
            {
                ev->datum.mus.exempla             = s->exempla;
                ev->datum.mus.numerus_exemplorum  = n;
            }
            redde VERUM;

        casus TESTA_TACTUS_INDICATOR_LIBERATIO:
            si (   !s->activus || (i32)c->index_actionis >= c->numerus
                || c->ids[c->index_actionis] != s->indicator)
            {
                redde FALSUM;
            }
            s->activus    = FALSUM;
            s->exspectat  = VERUM;
            _murem_implere(ev, EVENTUS_MUS_LIBERATUS, c,
                (s32)c->index_actionis, s->indicator, g);
            redde VERUM;

        casus TESTA_TACTUS_LIBERATIO:
        casus TESTA_TACTUS_ABRUPTIO:
            erat          = s->activus;
            s->activus    = FALSUM;
            s->exspectat  = FALSUM;
            si (!erat)
            {
                redde FALSUM;
            }
            i = _indicem_invenire(c, s->indicator);
            _murem_implere(ev, EVENTUS_MUS_LIBERATUS, c,
                i >= ZEPHYRUM ? i : ZEPHYRUM, s->indicator, g);
            redde VERUM;

        ordinarius:
            /* POINTER_DOWN (digitus secundus), HOVER, OUTSIDE: B1 nihil */
            redde FALSUM;
    }
}

vacuum
testa_facultates_tactus (
    Eventus* ev,
        s64  tempus)
{
    memset(ev, ZEPHYRUM, magnitudo(Eventus));
    ev->genus                         = EVENTUS_FACULTATES;
    ev->tempus                        = tempus;
    ev->datum.facultates.liberationes = VERUM;
    ev->datum.facultates.super        = FALSUM;
    ev->datum.facultates.pressio      = FALSUM;
}

TestaQuadriPetitio
testa_quadrum_petere (
    constans TestaQuadriStatus* s)
{
    TestaQuadriPetitio p;
                  b32  statim;

    p.genus    = TESTA_QUADRUM_NULLUM;
    p.post_ms  = ZEPHYRUM;
    si (!s->fenestra || !s->vivus)
    {
        redde p;
    }
    statim = s->fumus_reliqua > ZEPHYRUM || s->debitum
          || (!s->pulsus && s->mora == ZEPHYRUM);
    si (statim)
    {
        si (!s->pendens_statim)
        {
            p.genus = TESTA_QUADRUM_STATIM;
        }
        redde p;
    }
    si (s->pendens_statim || s->pendens_post)
    {
        redde p;
    }
    p.genus    = TESTA_QUADRUM_POST;
    p.post_ms  = s->mora;
    si (s->pulsus && (s->mora == ZEPHYRUM || s->mora > MORA_PULSUS))
    {
        p.post_ms = MORA_PULSUS;
    }
    redde p;
}

vacuum
testa_pixela_transferre (
    constans TabulaPixelorum* t,
     constans TestaGeometria* g,
                        i32* destinatio,
                         i32  gradus,
                         i32  color_marginis)
{
             i32  latitudo;
             i32  altitudo;
             i32  y;
             i32  x;
             i32  k;
             i32  r;
             s32  ry;
            i32* linea;
    constans i32* fons;

    latitudo = t->latitudo < g->latitudo ? t->latitudo : g->latitudo;
    altitudo = t->altitudo < g->altitudo ? t->altitudo : g->altitudo;
    per (y = ZEPHYRUM; y < g->fenestra_altitudo; y++)
    {
        linea  = destinatio + y * gradus;
        ry     = (s32)y - (s32)g->margo_y;
        si (ry < ZEPHYRUM || ry >= (s32)(altitudo * g->scala))
        {
            per (x = ZEPHYRUM; x < g->fenestra_latitudo; x++)
            {
                linea[x] = color_marginis;
            }
            perge;
        }
        fons = t->pixela + ((i32)ry / g->scala) * t->latitudo;
        per (k = ZEPHYRUM; k < g->margo_x; k++)
        {
            linea[k] = color_marginis;
        }
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            per (r = ZEPHYRUM; r < g->scala; r++)
            {
                linea[k] = fons[x];
                k++;
            }
        }
        per (; k < g->fenestra_latitudo; k++)
        {
            linea[k] = color_marginis;
        }
    }
}
```

- [ ] **Step 6: Green on the host, then on the Fire**

Run: `./compile_tests.sh probatio_testa_android_initus`
Expected: 8 sections PRAETERITAE, exit 0. Two plants (`silva.planta`),
each restored after its red:
- `_divisio_pavimenti` body = `redde a / b;` -> section `margo` red
  (x = 0 maps to 0, not -1).
- in `testa_quadrum_petere`, drop `|| s->debitum` from `statim` ->
  section `decisio quadri` red at "pendens post, debitum -> statim"
  (Review Focus 4).

Run (tablet plugged in, Fran's OK): `./tools/android/probatio.sh testa_android_initus`
Expected: `PROBATIONES ANDROID (arm64-v8a, API 30): PRAETERITAE (1)`,
tablet root purged.

- [ ] **Step 7: Commit**

```python
silva.commissio('testa T2: postulata_android.h (inline NDK -> static __inline__) et partes purae testae Android - geometria (scala, margo pavimento), tactus -> Eventus (digitus primus ducit, abruptio, historia in exempla), decisio quadri (ansam macOS imitatur, R4), transfusio proximi vicini; suita in hospite et in Fire viridis', [
  'include/postulata_android.h', 'include/testa_android_initus.h',
  'lib/testa_android_initus.c',
  'probationes/probatio_testa_android_initus.c'])
```

---

### Task 3: the Android shell and `apk.sh` - pictor on the Fire by hand

**Files:**
- Create: `lib/testa_android.c`, `tools/android/communia.sh`, `tools/android/apk.sh`, `tools/android/AndroidManifest.xml.forma`
- Modify: `tools/android/probatio.sh` (preflight + lock -> `communia.sh`), `tools/android/ndk_fixum.sh` (SDK pins)

**Interfaces:**
- Consumes: T1 `testa.h` (`TestaAmbitus`, `TestaCursus`, `testa_cursum_*`, `applicatio_initium`); T2 `testa_android_initus.h` (all of it); `filum_directorium_creare_cum_parentibus`, `tabula_pixelorum_creare_nuda`, `ludus_fenestra_*`, `fenestra_tempus_ms`, `thema_color`, `color_ad_pixelum`, `chorda_ex_literis`.
- Produces: `bin`-less artifacts `build/android/arm64-v8a/apk/<X>/<X>.apk`; `tools/android/apk.sh <apps/X/X.c> [-instituere] [-currere] [-fumus] [-servare]` (exit 0 / 1 build failed / 2 apparatus); `communia.sh` functions `android_tabulam_eligere` (sets `ANDROID_SERIAL`), `android_seram_capere` / `android_seram_liberare`, `A` (adb wrapper); system properties `debug.rhubarb.fumus` (frames, 0 = off) and `debug.rhubarb.scala` (override, 0 = the forma's).

- [ ] **Step 1: Write `lib/testa_android.c`**

```c
/* testa_android.c - testa Android (testa-spec II.4): NativeActivity in
 * filo principali SOLO (nullum filum alterum, D1). Vocationes
 * systematis -> cursus communis (testa_cursus) et partes purae
 * (testa_android_initus); applicatio_initium ad fenestram primam
 * (magnitudo vera). Effusio (stdout/stderr) per fistulam in logcat.
 * Proprietates: debug.rhubarb.fumus = N quadra (deinde fumus.png,
 * EXITUS=, finis), debug.rhubarb.scala = scala imposita (mensura T3). */

#include "postulata_posix.h"
#include "postulata_android.h"

#include "testa.h"
#include "testa_android_initus.h"
#include "filum.h"
#include "thema.h"
#include "color.h"
#include "chorda.h"

#include <android/native_activity.h>
#include <android/input.h>
#include <android/looper.h>
#include <android/choreographer.h>
#include <android/native_window.h>
#include <android/log.h>
#include <android/configuration.h>
#include <sys/system_properties.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifndef TESTA_TITULUS
#define TESTA_TITULUS "applicatio"   /* apk.sh: -DTESTA_TITULUS="X" */
#endif

/* constantae testa_android_initus.h == NDK (compilatio frangitur si
 * differunt: magnitudo negativa) */
externus character testa_congruentia_ndk[
    (   AMOTION_EVENT_ACTION_DOWN         == TESTA_TACTUS_DEPRESSIO
     && AMOTION_EVENT_ACTION_UP           == TESTA_TACTUS_LIBERATIO
     && AMOTION_EVENT_ACTION_MOVE         == TESTA_TACTUS_MOTUS
     && AMOTION_EVENT_ACTION_CANCEL       == TESTA_TACTUS_ABRUPTIO
     && AMOTION_EVENT_ACTION_POINTER_DOWN == TESTA_TACTUS_INDICATOR_DEPRESSIO
     && AMOTION_EVENT_ACTION_POINTER_UP   == TESTA_TACTUS_INDICATOR_LIBERATIO
     && AMOTION_EVENT_TOOL_TYPE_FINGER    == TESTA_INSTRUMENTUM_DIGITUS
     && AMOTION_EVENT_TOOL_TYPE_STYLUS    == TESTA_INSTRUMENTUM_STILUS
     && AMOTION_EVENT_TOOL_TYPE_MOUSE     == TESTA_INSTRUMENTUM_MUS
     && AMOTION_EVENT_TOOL_TYPE_ERASER    == TESTA_INSTRUMENTUM_GUMMI)
    ? I : -I];

nomen structura {
       ANativeActivity* activitas;
               Piscina* piscina;
          TestaAmbitus  ambitus;
           TestaCursus  cursus;
             character* argv[III];
                   b32  incepta;          /* initium vocatum */
                   b32  fracta;           /* initium/forma defecit */
                   b32  finita;           /* testa_cursum_finire factum */
         ANativeWindow* fenestra;
       TabulaPixelorum* tabula;
        TestaGeometria  geometria;
           AInputQueue* cauda;
               ALooper* ansa;
        AChoreographer* chorographus;
           TestaTactus  tactus;
     TestaTactusCrudus  crudus;           /* ~5 KiB: hic, non in acervo */
                   b32  vivus;
                   b32  pendens_statim;
                   b32  pendens_post;
                   b32  debitum;
                   b32  facultates_missae;
                   i32  fumus_quadra;     /* 0 = nullus */
} TestaAndroid;

/* instantia viva: vocationes chorographi pendentes post onDestroy eam
 * comparant (nulla revocatio in API) - mortua = nihil agunt */
hic_manens TestaAndroid* _testa_viva = NIHIL;

/* --- effusio: stdout/stderr -> fistula -> logcat (per processum) --- */

hic_manens integer   _fistula_lectio = -I;
hic_manens character _linea[MXXIV];
hic_manens i32       _linea_mensura = ZEPHYRUM;

interior integer
_effusionem_legere (
    integer fd,
    integer eventa,
    vacuum* data)
{
    character  frustum[CCLVI];
      ssize_t  lecta;
      ssize_t  k;

    (vacuum)eventa;
    (vacuum)data;
    dum ((lecta = read(fd, frustum, magnitudo(frustum))) > ZEPHYRUM)
    {
        per (k = ZEPHYRUM; k < lecta; k++)
        {
            si (   frustum[k] == '\n'
                || _linea_mensura + I >= (i32)magnitudo(_linea))
            {
                _linea[_linea_mensura] = '\0';
                (vacuum)__android_log_write(ANDROID_LOG_INFO,
                    TESTA_TITULUS, _linea);
                _linea_mensura = ZEPHYRUM;
                si (frustum[k] == '\n')
                {
                    perge;
                }
            }
            _linea[_linea_mensura] = frustum[k];
            _linea_mensura += I;
        }
    }
    redde I;   /* vocatio manet */
}

interior vacuum
_effusionem_ducere (
    ALooper* ansa)
{
    integer f[II];

    si (_fistula_lectio >= ZEPHYRUM || pipe(f) != ZEPHYRUM)
    {
        redde;
    }
    (vacuum)dup2(f[I], STDOUT_FILENO);
    (vacuum)dup2(f[I], STDERR_FILENO);
    (vacuum)close(f[I]);
    (vacuum)setvbuf(stdout, NIHIL, _IOLBF, BUFSIZ);
    (vacuum)setvbuf(stderr, NIHIL, _IONBF, ZEPHYRUM);
    (vacuum)fcntl(f[ZEPHYRUM], F_SETFL, O_NONBLOCK);
    _fistula_lectio = f[ZEPHYRUM];
    (vacuum)ALooper_addFd(ansa, _fistula_lectio, ALOOPER_POLL_CALLBACK,
        ALOOPER_EVENT_INPUT, _effusionem_legere, NIHIL);
}

/* ante finem: nihil in fistula perditur (AUDIENDA spec) */
interior vacuum
_effusionem_exhaurire (
    vacuum)
{
    (vacuum)fflush(stdout);
    (vacuum)fflush(stderr);
    si (_fistula_lectio >= ZEPHYRUM)
    {
        (vacuum)_effusionem_legere(_fistula_lectio, ZEPHYRUM, NIHIL);
    }
}

/* --- servitia et sedes --- */

interior ImagoFructus
_imaginem_nullam (
     vacuum* ctx,
    Piscina* piscina)
{
    ImagoFructus f;

    (vacuum)ctx;
    memset(&f, ZEPHYRUM, magnitudo(ImagoFructus));
    f.successus  = FALSUM;
    f.error      = chorda_ex_literis("clipboard Android nondum (B1)",
                                     piscina);
    redde f;
}

/* dpi schirmi (AConfiguration; Fire 7 = 160 = ACONFIGURATION_DENSITY_MEDIUM) */
interior i32
_densitas (
    ANativeActivity* activitas)
{
    AConfiguration* configuratio;
            int32_t densitas;

    configuratio = AConfiguration_new();
    si (!configuratio)
    {
        redde ZEPHYRUM;
    }
    AConfiguration_fromAssetManager(configuratio, activitas->assetManager);
    densitas = AConfiguration_getDensity(configuratio);
    AConfiguration_delete(configuratio);
    redde densitas > ZEPHYRUM ? (i32)densitas : ZEPHYRUM;
}

interior i32
_proprietas_numerus (
    constans character* titulus)
{
    character valor[PROP_VALUE_MAX];
      integer n;

    si (__system_property_get(titulus, valor) <= ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    n = atoi(valor);
    redde n > ZEPHYRUM ? (i32)n : ZEPHYRUM;
}

/* cwd, HOME = internalDataPath; TMPDIR = <sedes>/tmp (volumen
 * temporarium fumi, identitas machinae - android-gradus) */
interior b32
_sedem_ponere (
    constans character* sedes)
{
    character tmp[DXII];

    si (!sedes || strlen(sedes) + V >= magnitudo(tmp))
    {
        redde FALSUM;
    }
    sprintf(tmp, "%s/tmp", sedes);
    si (   !filum_directorium_creare_cum_parentibus(tmp)
        || chdir(sedes) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde setenv("HOME", sedes, I) == ZEPHYRUM
        && setenv("TMPDIR", tmp, I) == ZEPHYRUM;
}

```c
/* --- quadra --- */

interior vacuum _quadrum_petere (TestaAndroid* t);

interior vacuum
_fumum_finire (
    TestaAndroid* t)
{
    character via[DXII];
          s32 exitus;

    testa_cursum_finire(&t->cursus);
    t->finita  = VERUM;
    exitus     = I;
    si (strlen(t->activitas->internalDataPath) + XII < magnitudo(via))
    {
        sprintf(via, "%s/fumus.png", t->activitas->internalDataPath);
        si (ludus_fenestra_imaginem_scribere(t->cursus.lf, via))
        {
            exitus = ZEPHYRUM;
        }
    }
    imprimere("EXITUS=%d\n", (int)exitus);
    _effusionem_exhaurire();
    t->fumus_quadra = ZEPHYRUM;
    ANativeActivity_finish(t->activitas);
}

interior vacuum
_quadrum (
    TestaAndroid* t)
{
          LudusFenestra* lf;
    ANativeWindow_Buffer  alveus;
                    s64  nunc;
                    b32  pingendum;

    lf = t->cursus.lf;
    si (!t->fenestra || !lf || !t->vivus || t->fracta)
    {
        redde;
    }
    nunc       = fenestra_tempus_ms();
    pingendum  = t->fumus_quadra > ZEPHYRUM
              || ludus_fenestra_pingendum(lf, t->debitum, nunc);
    t->debitum = FALSUM;
    si (pingendum)
    {
        ludus_quadrum(lf, nunc);
        si (ANativeWindow_lock(t->fenestra, &alveus, NIHIL) == ZEPHYRUM)
        {
            si (   alveus.width  >= (integer)t->geometria.fenestra_latitudo
                && alveus.height >= (integer)t->geometria.fenestra_altitudo)
            {
                testa_pixela_transferre(lf->tabula, &t->geometria,
                    (i32*)alveus.bits, (i32)alveus.stride,
                    color_ad_pixelum(thema_color(COLOR_BACKGROUND)));
            }
            (vacuum)ANativeWindow_unlockAndPost(t->fenestra);
        }
    }
    si (   t->fumus_quadra > ZEPHYRUM
        && lf->mensurae.quadra >= t->fumus_quadra)
    {
        _fumum_finire(t);
        redde;
    }
    _quadrum_petere(t);
}

interior vacuum
_quadrum_statim (
    int64_t  tempus_ns,
     vacuum* data)
{
    TestaAndroid* t;

    (vacuum)tempus_ns;
    t = (TestaAndroid*)data;
    si (t != _testa_viva)
    {
        redde;
    }
    t->pendens_statim = FALSUM;
    _quadrum(t);
}

interior vacuum
_quadrum_post (
    int64_t  tempus_ns,
     vacuum* data)
{
    TestaAndroid* t;

    (vacuum)tempus_ns;
    t = (TestaAndroid*)data;
    si (t != _testa_viva)
    {
        redde;
    }
    t->pendens_post = FALSUM;
    _quadrum(t);
}

interior vacuum
_quadrum_petere (
    TestaAndroid* t)
{
     TestaQuadriStatus  s;
    TestaQuadriPetitio  p;

    s.fenestra        = t->fenestra != NIHIL && t->cursus.lf != NIHIL
                     && !t->fracta;
    s.vivus           = t->vivus;
    s.pendens_statim  = t->pendens_statim;
    s.pendens_post    = t->pendens_post;
    s.debitum         = t->debitum;
    s.mora            = t->cursus.lf ? ludus_fenestra_mora(t->cursus.lf)
                                     : ZEPHYRUM;
    s.pulsus          = t->cursus.forma.pulsator != NIHIL;
    s.fumus_reliqua   = (   t->fumus_quadra > ZEPHYRUM && t->cursus.lf
                         && t->cursus.lf->mensurae.quadra < t->fumus_quadra)
                      ? t->fumus_quadra - t->cursus.lf->mensurae.quadra
                      : ZEPHYRUM;
    p = testa_quadrum_petere(&s);
    si (p.genus == TESTA_QUADRUM_STATIM)
    {
        t->pendens_statim = VERUM;
        AChoreographer_postFrameCallback64(t->chorographus,
            _quadrum_statim, t);
    }
    alioquin si (p.genus == TESTA_QUADRUM_POST)
    {
        t->pendens_post = VERUM;
        AChoreographer_postFrameCallbackDelayed64(t->chorographus,
            _quadrum_post, t, (uint32_t)p.post_ms);
    }
}

/* --- initus --- */

interior vacuum
_tradere (
              TestaAndroid* t,
    constans Eventus* e)
{
    si (!t->cursus.lf || t->fracta)
    {
        redde;
    }
    ludus_fenestra_tractare(t->cursus.lf, e, fenestra_tempus_ms());
    t->debitum = VERUM;
}

interior vacuum
_crudum_implere (
    TestaTactusCrudus* c,
    constans AInputEvent* ev)
{
    int32_t  actio;
     size_t  numerus;
     size_t  historia;
     size_t  i;
     size_t  h;

    actio     = AMotionEvent_getAction(ev);
    c->actio  = (s32)(actio & AMOTION_EVENT_ACTION_MASK);
    c->index_actionis = (i32)((actio
        & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
        >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT);
    numerus = AMotionEvent_getPointerCount(ev);
    si (numerus > TESTA_INDICATORES_MAXIMI)
    {
        numerus = TESTA_INDICATORES_MAXIMI;
    }
    historia = AMotionEvent_getHistorySize(ev);
    si (historia > EVENTUS_EXEMPLA_MAXIMA)
    {
        historia = EVENTUS_EXEMPLA_MAXIMA;
    }
    c->numerus            = (i32)numerus;
    c->numerus_historiae  = (i32)historia;
    c->tempus = (s64)(AMotionEvent_getEventTime(ev) / 1000000);
    per (h = ZEPHYRUM; h < historia; h++)
    {
        c->historia_tempus[h] =
            (s64)(AMotionEvent_getHistoricalEventTime(ev, h) / 1000000);
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        c->ids[i]          = (s32)AMotionEvent_getPointerId(ev, i);
        c->instrumenta[i]  = (s32)AMotionEvent_getToolType(ev, i);
        c->x[i]            = AMotionEvent_getX(ev, i);
        c->y[i]            = AMotionEvent_getY(ev, i);
        per (h = ZEPHYRUM; h < historia; h++)
        {
            c->historia_x[i][h] = AMotionEvent_getHistoricalX(ev, i, h);
            c->historia_y[i][h] = AMotionEvent_getHistoricalY(ev, i, h);
        }
    }
}

/* VERUM = tractatum (systema non tangit) */
interior b32
_eventum_tractare (
    TestaAndroid* t,
    AInputEvent* ev)
{
    Eventus e;

    si (AInputEvent_getType(ev) == AINPUT_EVENT_TYPE_KEY)
    {
        si (AKeyEvent_getKeyCode(ev) != AKEYCODE_BACK)
        {
            redde FALSUM;   /* volumen etc.: systematis */
        }
        si (AKeyEvent_getAction(ev) == AKEY_EVENT_ACTION_UP)
        {
            ANativeActivity_finish(t->activitas);   /* D6: back = exire */
        }
        redde VERUM;
    }
    si (AInputEvent_getType(ev) != AINPUT_EVENT_TYPE_MOTION)
    {
        redde FALSUM;
    }
    si (!t->cursus.lf)
    {
        redde VERUM;    /* ante fenestram: devoratur */
    }
    si (!t->facultates_missae)
    {
        t->facultates_missae = VERUM;
        testa_facultates_tactus(&e, fenestra_tempus_ms());
        _tradere(t, &e);
    }
    _crudum_implere(&t->crudus, ev);
    si (testa_tactum_vertere(&t->tactus, &t->crudus, &t->geometria, &e))
    {
        _tradere(t, &e);
        _quadrum_petere(t);
    }
    redde VERUM;
}

interior integer
_caudam_legere (
    integer fd,
    integer eventa,
    vacuum* data)
{
    TestaAndroid* t;
     AInputEvent* ev;

    (vacuum)fd;
    (vacuum)eventa;
    t = (TestaAndroid*)data;
    dum (t->cauda && AInputQueue_getEvent(t->cauda, &ev) >= ZEPHYRUM)
    {
        si (AInputQueue_preDispatchEvent(t->cauda, ev))
        {
            perge;
        }
        AInputQueue_finishEvent(t->cauda, ev,
            _eventum_tractare(t, ev) ? I : ZEPHYRUM);
    }
    redde I;
}

```c
/* --- vocationes activitatis --- */

interior TestaAndroid*
_testa (
    ANativeActivity* activitas)
{
    redde (TestaAndroid*)activitas->instance;
}

interior vacuum
_finire_fracte (
    TestaAndroid* t,
    constans character* causa)
{
    fprintf(stderr, "%s: %s\n", TESTA_TITULUS, causa);
    t->fracta = VERUM;
    _effusionem_exhaurire();
    ANativeActivity_finish(t->activitas);
}

interior vacuum
_fenestra_creata (
    ANativeActivity* activitas,
      ANativeWindow* fenestra)
{
          TestaAndroid* t;
    constans character* causa;
               integer  latitudo;
               integer  altitudo;
                   i32  scala;
               Eventus  e;

    t            = _testa(activitas);
    t->fenestra  = fenestra;
    latitudo     = ANativeWindow_getWidth(fenestra);
    altitudo     = ANativeWindow_getHeight(fenestra);
    si (!t->incepta)
    {
        /* initium HIC (non in onCreate): applicationes magnitudinem
         * veram poscunt (testa-spec II.4) */
        t->incepta                      = VERUM;
        t->argv[ZEPHYRUM]               = (character*)TESTA_TITULUS;
        t->argv[I]                      = (character*)"-fumus";
        t->argv[II]                     = NIHIL;
        t->ambitus.piscina              = t->piscina;
        t->ambitus.fumus                = t->fumus_quadra > ZEPHYRUM;
        t->ambitus.argc                 = t->ambitus.fumus ? II : I;
        t->ambitus.argv                 = t->argv;
        t->ambitus.schirmi_latitudo     = (i32)latitudo;
        t->ambitus.schirmi_altitudo     = (i32)altitudo;
        t->ambitus.utilis_latitudo      = (i32)latitudo;
        t->ambitus.utilis_altitudo      = (i32)altitudo;
        t->ambitus.sedes_datorum        = activitas->internalDataPath;
        t->ambitus.densitas             = _densitas(activitas);
        t->ambitus.imago_glutinata      = _imaginem_nullam;
        si (!testa_cursum_incipere(&t->cursus, &t->ambitus,
                applicatio_initium, &causa))
        {
            testa_cursum_dimittere(&t->cursus);
            _finire_fracte(t, causa);
            redde;
        }
    }
    si (t->fracta)
    {
        redde;
    }
    scala = _proprietas_numerus("debug.rhubarb.scala");
    si (scala == ZEPHYRUM)
    {
        scala = t->cursus.forma.scala;
    }
    si (!testa_geometriam_computare((i32)latitudo, (i32)altitudo, scala,
            &t->geometria))
    {
        _finire_fracte(t, "fenestra minor scala");
        redde;
    }
    (vacuum)ANativeWindow_setBuffersGeometry(fenestra, latitudo, altitudo,
        WINDOW_FORMAT_RGBA_8888);
    si (!t->tabula)
    {
        t->tabula = tabula_pixelorum_creare_nuda(t->piscina,
            t->geometria.latitudo, t->geometria.altitudo);
        si (!t->tabula)
        {
            _finire_fracte(t, "tabula");
            redde;
        }
        t->tabula->scala              = (f32)scala;
        t->tabula->fenestra_latitudo  = (i32)latitudo;
        t->tabula->fenestra_altitudo  = (i32)altitudo;
        si (!testa_cursum_ludum_struere(&t->cursus, t->piscina, t->tabula))
        {
            _finire_fracte(t, "ludus");
            redde;
        }
    }
    alioquin si (   (i32)latitudo != t->tabula->fenestra_latitudo
                 || (i32)altitudo != t->tabula->fenestra_altitudo)
    {
        /* puncta fenestrae: ludus_fenestra tabulam aptat (013 B3b) */
        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus = EVENTUS_MUTARE_MAGNITUDINEM;
        e.datum.mutare_magnitudinem.latitudo  = (i32)latitudo;
        e.datum.mutare_magnitudinem.altitudo  = (i32)altitudo;
        _tradere(t, &e);
        t->tabula->fenestra_latitudo  = (i32)latitudo;
        t->tabula->fenestra_altitudo  = (i32)altitudo;
    }
    t->debitum = VERUM;
    _quadrum_petere(t);
}

interior vacuum
_fenestra_deleta (
    ANativeActivity* activitas,
      ANativeWindow* fenestra)
{
    (vacuum)fenestra;
    _testa(activitas)->fenestra = NIHIL;
}

interior vacuum
_vitam_mutare (
    ANativeActivity* activitas,
                b32  vivus)
{
    TestaAndroid* t;
         Eventus  e;

    t         = _testa(activitas);
    t->vivus  = vivus;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = vivus ? EVENTUS_RESUMPTIO : EVENTUS_SUSPENSIO;
    _tradere(t, &e);
    _quadrum_petere(t);
}

interior vacuum
_resumpta (
    ANativeActivity* activitas)
{
    _vitam_mutare(activitas, VERUM);
}

interior vacuum
_pausata (
    ANativeActivity* activitas)
{
    _vitam_mutare(activitas, FALSUM);
}

interior vacuum
_cauda_creata (
    ANativeActivity* activitas,
        AInputQueue* cauda)
{
    TestaAndroid* t;

    t         = _testa(activitas);
    t->cauda  = cauda;
    AInputQueue_attachLooper(cauda, t->ansa, ALOOPER_POLL_CALLBACK,
        _caudam_legere, t);
}

interior vacuum
_cauda_deleta (
    ANativeActivity* activitas,
        AInputQueue* cauda)
{
    AInputQueue_detachLooper(cauda);
    _testa(activitas)->cauda = NIHIL;
}

interior vacuum
_deleta (
    ANativeActivity* activitas)
{
    TestaAndroid* t;

    t = _testa(activitas);
    _testa_viva = NIHIL;
    si (t->cursus.lf && !t->finita)
    {
        testa_cursum_finire(&t->cursus);
    }
    testa_cursum_dimittere(&t->cursus);
    _effusionem_exhaurire();
    activitas->instance = NIHIL;
    piscina_destruere(t->piscina);
}

vacuum
ANativeActivity_onCreate (
    ANativeActivity* activitas,
             vacuum* status_servatus,
              size_t magnitudo_status)
{
         Piscina* piscina;
    TestaAndroid* t;

    (vacuum)status_servatus;     /* B2 */
    (vacuum)magnitudo_status;
    _effusionem_ducere(ALooper_forThread());
    piscina = piscina_generare_dynamicum(TESTA_TITULUS, VIII * M * M);
    t = piscina ? (TestaAndroid*)piscina_allocare(piscina,
                      magnitudo(TestaAndroid))
                : NIHIL;
    si (!t || !_sedem_ponere(activitas->internalDataPath))
    {
        fprintf(stderr, "%s: piscina aut sedes deficit\n", TESTA_TITULUS);
        _effusionem_exhaurire();
        ANativeActivity_finish(activitas);
        redde;
    }
    memset(t, ZEPHYRUM, magnitudo(TestaAndroid));
    t->activitas     = activitas;
    t->piscina       = piscina;
    t->ansa          = ALooper_forThread();
    t->chorographus  = AChoreographer_getInstance();
    t->fumus_quadra  = _proprietas_numerus("debug.rhubarb.fumus");
    activitas->instance = t;
    _testa_viva         = t;

    activitas->callbacks->onNativeWindowCreated   = _fenestra_creata;
    activitas->callbacks->onNativeWindowDestroyed = _fenestra_deleta;
    activitas->callbacks->onResume                = _resumpta;
    activitas->callbacks->onPause                 = _pausata;
    activitas->callbacks->onInputQueueCreated     = _cauda_creata;
    activitas->callbacks->onInputQueueDestroyed   = _cauda_deleta;
    activitas->callbacks->onDestroy               = _deleta;
}
```

Notes for typing: if `<sys/system_properties.h>` (not used by the
spike) trips `-Werror` under the `inline` define (`static static`
duplicate specifier), include it BEFORE `postulata_android.h` with a
comment naming the header and ledger the ruling. `int64_t` / `size_t` /
`ssize_t` appear only where NDK signatures demand them.

- [ ] **Step 2: Build the shell for Android alone (compile check before any APK)**

Run:
```bash
. tools/android/ndk_fixum.sh; . tools/vexilla.sh; vexilla_radices ""
"$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin/clang" \
  --target=aarch64-linux-android30 "${VEXILLA_C89[@]}" "${RADICES_C89[@]}" \
  -fPIC -DTESTA_TITULUS='"pictor"' -c lib/testa_android.c -o /dev/null
```
Expected: no output, exit 0. (A `static static` error names the
header to move - see the note above.)

- [ ] **Step 3: Shared device helpers - create `tools/android/communia.sh`, then make `probatio.sh` source it**

```bash
# tools/android/communia.sh - communia scriptorum Android (source, non
# exec): adb, tabula UNA auctorizata, sera hospitis per tabulam
# (~/.rhubarb/android/<serial>.sera, omnibus arboribus communis).
# Vocans 'apparatus' (exitus 2 cum nuntio) ANTE 'source' definit.
# Ex tools/android/probatio.sh extractum (testa T3) - mores idem.

ADB="${ADB:-$HOME/Library/Android/sdk/platform-tools/adb}"

A () { "$ADB" "$@"; }

# ANDROID_SERIAL ponit et exportat; aliter apparatus
android_tabulam_eligere () {
    local tabulae status numerus
    [ -x "$ADB" ] || apparatus "adb deest ($ADB)"
    tabulae=$("$ADB" devices | awk 'NR > 1 && NF >= 2 {print $1 " " $2}')
    if [ -n "${ANDROID_SERIAL:-}" ]; then
        status=$(printf '%s\n' "$tabulae" | awk -v s="$ANDROID_SERIAL" '$1 == s {print $2}')
        [ -n "$status" ] || apparatus "tabula $ANDROID_SERIAL non connexa"
    else
        numerus=$(printf '%s\n' "$tabulae" | grep -c .)
        [ "$numerus" -ge 1 ] || apparatus "nulla tabula connexa (adb devices vacuum)"
        [ "$numerus" -eq 1 ] || apparatus "plures tabulae ($numerus) - ANDROID_SERIAL elige"
        ANDROID_SERIAL=$(printf '%s\n' "$tabulae" | awk '{print $1}')
        status=$(printf '%s\n' "$tabulae" | awk '{print $2}')
    fi
    [ "$status" = "device" ] || apparatus "tabula $ANDROID_SERIAL status '$status' - 'Allow USB debugging?' in tabula accipe"
    export ANDROID_SERIAL
}

# sera (recensio 2026-10-23): cursus plures eandem tabulam non tangunt;
# mkdir atomicum + pid; possessor mortuus = sera stala, capitur
ANDROID_SERA_NOSTRA=0
android_seram_capere () {
    local alius
    ANDROID_SERA_DIR="$HOME/.rhubarb/android"
    ANDROID_SERA="$ANDROID_SERA_DIR/$ANDROID_SERIAL.sera"
    mkdir -p "$ANDROID_SERA_DIR" || apparatus "directorium serae $ANDROID_SERA_DIR creari nequit"
    if ! mkdir "$ANDROID_SERA" 2>/dev/null; then
        alius=$(cat "$ANDROID_SERA/pid" 2>/dev/null)
        if [ -z "$alius" ] || kill -0 "$alius" 2>/dev/null; then
            apparatus "tabula $ANDROID_SERIAL occupata (cursus pid ${alius:-ignotus}; sera $ANDROID_SERA) - exspecta; si nullus cursus vivit: rm -rf $ANDROID_SERA"
        fi
        { rm -rf "$ANDROID_SERA" && mkdir "$ANDROID_SERA" 2>/dev/null; } \
            || apparatus "sera $ANDROID_SERA capi nequit"
    fi
    echo $$ > "$ANDROID_SERA/pid"
    ANDROID_SERA_NOSTRA=1
}

android_seram_liberare () {
    if [ "$ANDROID_SERA_NOSTRA" = 1 ]; then
        rm -rf "$ANDROID_SERA"
        ANDROID_SERA_NOSTRA=0
    fi
}
```

In `tools/android/probatio.sh`: delete `ADB=...`, the "tabula: exacte
una" block, `A () {...}`, the `SERA_*` variables and `sera_capere`;
add after the `apparatus` function:

```bash
. tools/android/communia.sh
android_tabulam_eligere
```

replace `sera_capere` with `android_seram_capere`, and in `finis`
replace the `SERA_NOSTRA` block with `android_seram_liberare`.

Run: `./tools/android/probatio.sh piscina && ./tools/android/probatio.sh -purgare`
Expected: `PRAETERITAE (1)`, then `purgatum`; `ls ~/.rhubarb/android/`
shows no `.sera` left. Second terminal while one runs: "occupata".

- [ ] **Step 4: SDK pins - append to `tools/android/ndk_fixum.sh`**

```bash
# SDK (testa T3): instrumenta APK et platforma - Android Studio
# instituit, non nostrum instrumentum (integritas non sigillata:
# AUDIENDA testa-spec)
SDK_SEDES="$HOME/Library/Android/sdk"
INSTRUMENTA_VERSIO="34.0.0"
PLATFORMA="android-34"
INSTRUMENTA_SEDES="$SDK_SEDES/build-tools/$INSTRUMENTA_VERSIO"
PLATFORMA_JAR="$SDK_SEDES/platforms/$PLATFORMA/android.jar"
```

- [ ] **Step 5: Manifest template `tools/android/AndroidManifest.forma.xml`**

```xml
<?xml version="1.0" encoding="utf-8"?>
<!-- forma manifesti testae Android (testa-spec II.7): apk.sh @TITULUS@
     substituit; NativeActivity sine codice Java -->
<manifest xmlns:android="http://schemas.android.com/apk/res/android"
    package="earth.golden.@TITULUS@"
    android:versionCode="1" android:versionName="0.1">
  <uses-sdk android:minSdkVersion="30" android:targetSdkVersion="34"/>
  <application android:label="@TITULUS@" android:hasCode="false"
      android:debuggable="true">
    <activity android:name="android.app.NativeActivity"
        android:exported="true"
        android:screenOrientation="sensorLandscape"
        android:configChanges="orientation|screenSize|screenLayout|keyboard|keyboardHidden">
      <meta-data android:name="android.app.lib_name"
          android:value="@TITULUS@"/>
      <intent-filter>
        <action android:name="android.intent.action.MAIN"/>
        <category android:name="android.intent.category.LAUNCHER"/>
      </intent-filter>
    </activity>
  </application>
</manifest>
```

- [ ] **Step 6: Write `tools/android/apk.sh`** (T4 adds `-fumus`; here the flag is parsed and refused)

```bash
#!/bin/bash
# tools/android/apk.sh - APK applicationis testae Android (testa-spec
# II.7): clausura aedilis (--varians android,posix: testa_android.c per
# testa.h) -> NDK clang -> lib<X>.so -> aapt2 + zip + zipalign +
# apksigner -> build/android/arm64-v8a/apk/<X>/<X>.apk
#
# Usus: ./tools/android/apk.sh <apps/X/X.c> [-instituere] [-currere]
#                              [-fumus] [-servare]
#   -instituere  adb install -r     -currere  am start (implicat -instituere)
#   -fumus       testa-spec II.8 (T4)  -servare  post fumum non deinstallat
#
# Clavis: ~/.rhubarb/android/debug.jks (semel per keytool; JKS quia
# Java 8 systematis PKCS12 novum legere nequit - spica). SOLUM probatio:
# signum 'android' notum, numquam emissio (B6).
#
# Exitus: 0 | 1 constructio fracta (compilatio, nexus, .m) | 2 apparatus
set -u
export LC_ALL=C
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

apparatus () { echo "android/apk.sh: $*" >&2; exit 2; }
fractum () { echo "android/apk.sh: FRACTUM: $*" >&2; exit 1; }

INSTITUERE=0; CURRERE=0; FUMUS=0; SERVARE=0; FONS=""
while [ $# -gt 0 ]; do
    case "$1" in
        -instituere) INSTITUERE=1 ;;
        -currere) CURRERE=1; INSTITUERE=1 ;;
        -fumus) FUMUS=1; INSTITUERE=1 ;;
        -servare) SERVARE=1 ;;
        -*) apparatus "optio ignota: $1" ;;
        *) [ -z "$FONS" ] || apparatus "fons unus: $FONS et $1"; FONS="$1" ;;
    esac
    shift
done
[ -n "$FONS" ] || apparatus "usus: $0 <apps/X/X.c> [-instituere] [-currere] [-fumus] [-servare]"
[ -f "$FONS" ] || apparatus "$FONS deest"
TITULUS=$(basename "$FONS" .c)
case "$TITULUS" in
    ""|*[!a-z]*) apparatus "titulus '$TITULUS': litterae minusculae solae (involucrum earth.golden.$TITULUS)" ;;
esac
INVOLUCRUM="earth.golden.$TITULUS"

# --- instrumenta -------------------------------------------------------
. tools/android/ndk_fixum.sh
grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null \
    || apparatus "NDK absens: $NDK_SEDES ($NDK_VERSIO) - ./tools/android/ndk_instituere.sh"
NDK_BIN="$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin"
for instr in aapt2 zipalign apksigner; do
    [ -x "$INSTRUMENTA_SEDES/$instr" ] || apparatus "instrumentum absens: $INSTRUMENTA_SEDES/$instr"
done
[ -f "$PLATFORMA_JAR" ] || apparatus "platforma absens: $PLATFORMA_JAR"
command -v keytool > /dev/null || apparatus "keytool deest (Java)"
command -v zip > /dev/null || apparatus "zip deest"
[ -x bin/aedilis ] || apparatus "bin/aedilis deest - ./tools/aedilis_struere.sh"

. tools/vexilla.sh
vexilla_radices ""
CC=("$NDK_BIN/clang" "--target=aarch64-linux-android$NDK_API_MINIMA")
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}" -fPIC "-DTESTA_TITULUS=\"$TITULUS\"")
VFLAGS=(-O2 -fPIC -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- constructio -------------------------------------------------------
D="build/android/arm64-v8a/apk/$TITULUS"
rm -rf "$D" && mkdir -p "$D/obiecta" "$D/radix/lib/arm64-v8a" || apparatus "$D creari nequit"
PARTES=$(bin/aedilis "$FONS" --varians android,posix --partes 2>&1) \
    || apparatus "aedilis fracta: $PARTES"
OBIECTA=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "O" {print $2}')
VENDITI=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "V" {print $2}')
if printf '%s\n' "$OBIECTA" | grep -q '\.m$'; then
    fractum "fons .m in clausura (codex Mac solum): $(printf '%s\n' "$OBIECTA" | grep '\.m$' | tr '\n' ' ')"
fi
OBJ=()
# OBIECTA: viae sine spatiis (aedilis), ergo verba nuda
for f in "$FONS" $OBIECTA; do
    o="$D/obiecta/$(printf '%s' "$f" | tr '/' '_').o"
    "${CC[@]}" "${VEXILLA[@]}" -c "$f" -o "$o" >> "$D/compilatio.log" 2>&1 \
        || { tail -15 "$D/compilatio.log" >&2; fractum "compilatio $f"; }
    OBJ+=("$o")
done
for v in $VENDITI; do
    o="$D/obiecta/v_$(basename "$v" .c).o"
    "${CC[@]}" "${VFLAGS[@]}" -c "$v" -o "$o" >> "$D/compilatio.log" 2>&1 \
        || { tail -15 "$D/compilatio.log" >&2; fractum "venditum $v"; }
    OBJ+=("$o")
done
SO="$D/radix/lib/arm64-v8a/lib$TITULUS.so"
"${CC[@]}" -shared -o "$SO" "${OBJ[@]}" -landroid -llog -lm \
    -Wl,--no-undefined "-Wl,-soname,lib$TITULUS.so" > "$D/nexus.log" 2>&1 \
    || { tail -15 "$D/nexus.log" >&2; fractum "nexus (symbolum absens = pars platformae deest)"; }
"$NDK_BIN/llvm-strip" "$SO" || apparatus "llvm-strip fractum"

sed "s/@TITULUS@/$TITULUS/g" tools/android/AndroidManifest.forma.xml > "$D/AndroidManifest.xml"
"$INSTRUMENTA_SEDES/aapt2" link -o "$D/basis.apk" --manifest "$D/AndroidManifest.xml" \
    -I "$PLATFORMA_JAR" > "$D/aapt2.log" 2>&1 \
    || { cat "$D/aapt2.log" >&2; fractum "aapt2 link"; }
(cd "$D/radix" && zip -q -r ../basis.apk lib) || fractum "zip"
"$INSTRUMENTA_SEDES/zipalign" -p -f 4 "$D/basis.apk" "$D/alignatum.apk" || fractum "zipalign"
CLAVIS="$HOME/.rhubarb/android/debug.jks"
if [ ! -f "$CLAVIS" ]; then
    mkdir -p "$(dirname "$CLAVIS")" || apparatus "$(dirname "$CLAVIS") creari nequit"
    keytool -genkeypair -keystore "$CLAVIS" -storetype JKS -storepass android \
        -keypass android -alias rhubarb -keyalg RSA -keysize 2048 -validity 10000 \
        -dname "CN=rhubarb debug" > /dev/null 2>&1 || apparatus "keytool fractum"
    chmod 600 "$CLAVIS"
fi
"$INSTRUMENTA_SEDES/apksigner" sign --ks "$CLAVIS" --ks-pass pass:android \
    --key-pass pass:android --ks-key-alias rhubarb --out "$D/$TITULUS.apk" \
    "$D/alignatum.apk" > "$D/apksigner.log" 2>&1 \
    || { cat "$D/apksigner.log" >&2; fractum "apksigner"; }
echo "APK: $D/$TITULUS.apk ($(du -k "$D/$TITULUS.apk" | awk '{print $1}') KiB, ${#OBJ[@]} obiecta)"
[ "$INSTITUERE" = 1 ] || exit 0

# --- tabula --------------------------------------------------------------
. tools/android/communia.sh
android_tabulam_eligere
android_seram_capere
trap android_seram_liberare EXIT
trap 'exit 130' INT TERM
A install -r "$D/$TITULUS.apk" > "$D/instituere.log" 2>&1 \
    || { cat "$D/instituere.log" >&2; apparatus "adb install fractum"; }
echo "institutum: $INVOLUCRUM in $ANDROID_SERIAL"
[ "$FUMUS" = 0 ] || apparatus "-fumus: testa T4"
if [ "$CURRERE" = 1 ]; then
    A shell am start -n "$INVOLUCRUM/android.app.NativeActivity" > /dev/null \
        || apparatus "am start fractum"
    echo "currit: $INVOLUCRUM (logcat: adb logcat -s $TITULUS)"
fi
exit 0
```

`chmod +x tools/android/apk.sh`.

- [ ] **Step 7: Build the pictor APK (no device)**

Run: `./tools/android/apk.sh apps/pictor/pictor.c`
Expected: `APK: build/android/arm64-v8a/apk/pictor/pictor.apk (N KiB, ~69 obiecta)`,
exit 0. Verify the GNU89 `inline` leak cannot recur:
`"$NDK_BIN/llvm-nm" -D --defined-only build/android/arm64-v8a/apk/pictor/radix/lib/arm64-v8a/libpictor.so | grep -c ANativeWindow_`
Expected: `0` (only `ANativeActivity_onCreate` plus our own symbols
exported). A link failure names the missing symbol - a platform piece
the closure still needs (`fenestra_*` would mean R6 was wrong: swap
`ludus_fenestra.h`'s include to `fenestra_tempus.h` + `eventus.h` and
ledger it).

- [ ] **Step 8: Install and launch on the Fire (tablet plugged in and unlocked by Fran)**

Run: `./tools/android/apk.sh apps/pictor/pictor.c -currere`, then
`adb logcat -d -s pictor | tail -20` and
`adb exec-out screencap -p > build/android/arm64-v8a/apk/pictor/schirmus.png`
(open it with Read).
Expected: logcat clean (no `causa`), pictor's canvas and status line
visible in the screenshot. A black screenshot = screen asleep
(`adb shell dumpsys power | grep mWakefulness`), not a bug.

- [ ] **Step 9: The scale measurement (spec II.5) - decide and record**

Look at the Step 8 screenshot (scale II -> 512x300 of our pixels).
Then: `adb shell setprop debug.rhubarb.scala 1`, relaunch with
`-currere`, screenshot again (1024x600 at scale I), then
`adb shell setprop debug.rhubarb.scala 0`. Show Fran both PNGs.
Decision rule: II stays if the canvas, status line and palette lay
out without overlap or clipping at 512x300; otherwise scale I. Record
in the ledger (`Task 3: Ruling: pictor scale on the Fire = <II|I> -
<what the two screenshots showed>`). If I wins, pictor chooses it by
density (mdpi screens: one of our pixels per device pixel), in
`apps/pictor/pictor.c`:

```c
    /* mdpi (Fire 7, 160 dpi): II = 512x300, nimis angustum (T3 mensura) */
    forma->scala      = (a->densitas > ZEPHYRUM && a->densitas < CCXL)
                      ? I : SCALA;
```

and logcat confirms the shell no longer needs the property. The spec's AUDIENDA line on 512x300 is answered in
the spec's VI section at the end of B1.

- [ ] **Step 10: Fran by hand (spec III.6)**

From the home-screen icon: draw with a finger; change tool on screen;
spraycan; lock and unlock the tablet mid-stroke (Review Focus 2: no
stuck stroke after resume); back = quit; reopen - the drawing is still
there (volumen in `internalDataPath`). Note anything wrong in the
ledger as a park (not fixed in this task unless it breaks the contract).

- [ ] **Step 11: Commit**

```python
silva.commissio('testa T3: testa Android (lib/testa_android.c - NativeActivity in filo principali solo, initium ad fenestram primam, chorographus per decisionem puram, effusio in logcat) et tools/android/apk.sh (clausura aedilis -> lib<X>.so -> aapt2/zipalign/apksigner, clavis ~/.rhubarb/android/debug.jks); communia.sh ex probatio.sh; pictor in Fire per manum Frani', [
  'lib/testa_android.c', 'tools/android/communia.sh', 'tools/android/apk.sh',
  'tools/android/AndroidManifest.forma.xml', 'tools/android/probatio.sh',
  'tools/android/ndk_fixum.sh'])
```

(+ `apps/pictor/pictor.c` if Step 9 changed it.)

---

### Task 4: device smoke, the `android-struere` gate, authoring rules

**Files:**
- Modify: `tools/android/apk.sh` (the `-fumus` branch), `include/testa.h` (rules), `pythonica/silva.py` (PORTAE), `project-specs/testa-spec.md` (status + VI)
- Create: `tools/android_struere_porta.sh`

**Interfaces:**
- Consumes: T3 `apk.sh`, `communia.sh`, property `debug.rhubarb.fumus`, shell's `fumus.png` + `EXITUS=` logcat line (T3 `_fumum_finire`).
- Produces: `./tools/android/apk.sh <fons> -fumus [-servare]` (exit 0 = `EXITUS=0` + PNG pulled + uninstalled); gate `android-struere` (signal `PORTA ANDROID-STRUERE: (viridis|FRACTA)`).

- [ ] **Step 1: Write the smoke branch - in `tools/android/apk.sh` replace `[ "$FUMUS" = 0 ] || apparatus "-fumus: testa T4"` and the `trap android_seram_liberare EXIT` line with**

```bash
# fumus (testa-spec II.8): proprietas -> lanceatio -> ictus per 'input
# swipe' -> EXITUS= in logcat -> fumus.png per run-as -> deinstallatio
# (regula Frani: nihil in tabula manet nisi -servare)
QUADRA_FUMI=600   # ~10 s: spatium ictui inter lanceationem et finem
fumum_finire () {
    A shell setprop debug.rhubarb.fumus 0 > /dev/null 2>&1
    if [ "$SERVARE" = 1 ]; then
        echo "servatum: $INVOLUCRUM (adb uninstall $INVOLUCRUM)"
        return 0
    fi
    A uninstall "$INVOLUCRUM" > /dev/null 2>&1
    if A shell pm list packages 2>/dev/null | tr -d '\r' | grep -qx "package:$INVOLUCRUM"; then
        echo "DEINSTALLATIO FRACTA: $INVOLUCRUM manet in tabula" >&2
        return 1
    fi
    echo "deinstallatum: $INVOLUCRUM"
}
finis () {
    local status=$?
    if [ "$FUMUS" = 1 ] && ! fumum_finire && [ "$status" -lt 2 ]; then
        status=2
    fi
    android_seram_liberare
    exit "$status"
}
trap finis EXIT
```

and after the `echo "institutum: ..."` line:

```bash
if [ "$FUMUS" = 1 ]; then
    A shell setprop debug.rhubarb.fumus "$QUADRA_FUMI" || apparatus "setprop fractum"
    A logcat -c
    A shell am start -W -n "$INVOLUCRUM/android.app.NativeActivity" > /dev/null \
        || apparatus "am start fractum"
    sleep 1
    # ictus trans tabulam (puncta fenestrae, landscape 1024x600)
    A shell input swipe 300 250 700 350 400 || apparatus "input swipe fractum"
    EXITUS=""
    for _ in $(seq 1 40); do
        EXITUS=$(A logcat -d -s "$TITULUS:I" 2>/dev/null | tr -d '\r' \
            | awk -F'EXITUS=' 'NF > 1 {print $2}' | tail -1)
        [ -n "$EXITUS" ] && break
        sleep 1
    done
    A logcat -d -s "$TITULUS:I" 2>/dev/null | tr -d '\r' > "$D/logcat.txt"
    [ -n "$EXITUS" ] || apparatus "nulla linea EXITUS intra XL s (logcat: $D/logcat.txt)"
    A exec-out run-as "$INVOLUCRUM" cat files/fumus.png > "$D/fumus.png" 2>/dev/null
    grep -q 'ludus: quadra=' "$D/logcat.txt" || fractum "linea mensurarum deest ($D/logcat.txt)"
    [ "$EXITUS" = 0 ] || fractum "EXITUS=$EXITUS ($D/logcat.txt)"
    [ "$(head -c 4 "$D/fumus.png" | od -An -c | tr -d ' ')" = "211PNG" ] \
        || fractum "fumus.png non PNG ($D/fumus.png)"
    echo "FUMUS ANDROID: EXITUS=0, imago $D/fumus.png"
    exit 0
fi
```

- [ ] **Step 2: Run the smoke (Fran's tablet, unlocked)**

Run: `./tools/android/apk.sh apps/pictor/pictor.c -fumus`
Expected: `FUMUS ANDROID: EXITUS=0, imago build/android/arm64-v8a/apk/pictor/fumus.png`
then `deinstallatum: earth.golden.pictor`, exit 0. Open the PNG with
Read: a stroke crosses the canvas (spec III.4). `adb shell pm list
packages | grep golden` prints nothing. Run once more with `-servare`:
the app stays (then `adb uninstall earth.golden.pictor`).

- [ ] **Step 3: The gate - create `tools/android_struere_porta.sh` (`chmod +x`, `git add -N`)**

```bash
#!/bin/bash
# tools/android_struere_porta.sh - PORTA android-struere (testa-spec
# II.10): APK pictoris SINE tabula struitur. Bibliotheca communis non
# portabilis (vocatio platformae, fons .m) nexum frangit, symbolo
# nominato (-Wl,--no-undefined). NDK aut SDK absens = RECUSATIO
# (exitus 2, signum nullum - porta 'non cucurrit'), numquam saltus
# tacitus.
# <porta titulus="android-struere" gradus="fusio" instrumentum="bin/aedilis" tegit="include/* lib/*.c apps/pictor/* tools/android/* tools/vexilla.sh"/>
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 2
./tools/android/apk.sh apps/pictor/pictor.c
rc=$?
case $rc in
    0) echo "PORTA ANDROID-STRUERE: viridis (pictor)" ;;
    1) echo "PORTA ANDROID-STRUERE: FRACTA (pictor)" ;;
    *) echo "PORTA ANDROID-STRUERE: RECUSATA (apparatus, exitus $rc) - NDK/SDK: tools/android/ndk_fixum.sh" >&2 ;;
esac
exit $rc
```

In `pythonica/silva.py` `PORTAE` (beside `'aedilis'`, line ~1649):

```python
    'android-struere': (['./tools/android_struere_porta.sh'],
                        r'PORTA ANDROID-STRUERE: (viridis|FRACTA)'),
```

(An unregistered gate script = commits without it - lesson 0d069357.)

- [ ] **Step 4: Gate born red (spec III.5)**

Plant 1 (`silva.planta`, compiles on the host): at the end of
`lib/pictor_actiones.c`'s includes add `#include "clipboard_platform.h"`,
and at file scope after the includes

```c
ImagoFructus (*pictor_planta_platformae)(Piscina*) = clipboard_capere_imaginem;
```

(a non-static global: no unused warning, and the symbol must link).
Run: `./tools/android_struere_porta.sh`
Expected: `PORTA ANDROID-STRUERE: FRACTA`, the nexus tail naming
`clipboard_capere_imaginem` (or the `.m` refusal naming
`lib/clipboard_platform_macos.m` - either names it). Restore (plain
`cp`, then `touch`).
Plant 2 (by hand): `NDK_VERSIO="27.3.0"` in `tools/android/ndk_fixum.sh`.
Expected: `RECUSATA (apparatus, exitus 2)`, `NDK absens: ...`. Restore.
Then green: `PORTA ANDROID-STRUERE: viridis (pictor)`.

- [ ] **Step 5: Authoring rules into `include/testa.h`** (insert after the first comment block)

```c
/* REGULAE (testa-spec II.9; Franus 2026-10-23: "shared material ...
 * continuity / same aesthetics", non applicatio una ubique).
 *
 * MATERIA COMMUNIS (documenta, actiones, figurae, insula, volumen,
 * thema) - leges, porta android-struere eas probat:
 *  1. nulla vocatio platformae: servitia ut portae ab hospite veniunt
 *     (exemplar: actiones_ctx.imago_capere pictoris)
 *  2. tempus ex 'nunc' (dispensatoris), numquam "N ms post cursum
 *     meum"; nihil obstruit, nulla ansa propria - applicatio in pausa
 *     (Android) quadra nulla accipit
 *  3. status qui valet in insula / volumine, numquam in memoria C sola
 *     (Android processus in fundo necat - B2)
 *
 * COMPOSITIO (pictor_componentia et similia, per superficiem) suam
 * superficiem supponere potest: super, dextra, claves in mensa; tactus
 * et situs erectus in tabula. Viae ab hospite (a->sedes_datorum),
 * numquam ex cwd binarii supposito. Documentum aspectui originis suae
 * pertinet; clientes alii fortasse solum legunt. */
```

and a one-line pointer in `include/insula.h`'s header comment:
"Applicationes portabiles: regulae in testa.h (materia communis vs
compositio)."

- [ ] **Step 6: Close the spec, then commit**

`project-specs/testa-spec.md`: status line -> `**Status: IMPLETUM
<date>**` with the four commits; add `## VI. Found while building`
(every ledger `Ruling:` of this plan, the scale decision of T3 Step 9
with both screenshots described, the AUDIENDA answers: `setprop`,
`run-as`, `input swipe`, fps observed, logcat completeness). Ledger:
note under ...577GAS ("B1 IMPLETUM"), close park ...J49JH if T1 did
not. Memory: update `android-toolchain.md` (apk.sh, properties,
earth.golden.*).

```python
silva.commissio('testa T4: fumus Android (apk.sh -fumus: proprietas, ictus per input swipe, EXITUS in logcat, fumus.png per run-as, deinstallatio), porta android-struere (gradus fusio; NDK absens = recusatio), regulae auctorum in testa.h (materia communis vs compositio); testa-spec IMPLETUM', [
  'tools/android/apk.sh', 'tools/android_struere_porta.sh',
  'pythonica/silva.py', 'include/testa.h', 'include/insula.h',
  'project-specs/testa-spec.md', 'project-specs/testa-plan.md'])
```

