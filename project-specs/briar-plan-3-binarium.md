# briar — plan 3: binarium (the `briar` binary, the shared corpus block, the fumus gate, the first shebang run)

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:executing-plans to implement this plan task-by-task, inline, in the worktree `../rhubarb-briar` on branch `rhubarb-briar`. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `./x.thistle a b` runs: briar parses the file, keys it, builds a project under `~/.rhubarb/briar/` once, and becomes the program. `-probatio` runs the file's probatio region. The corpus is embedded so a thistle file works outside any repository.

**Architecture:** The CLI logic (flag rules, the reserved first argument, project-directory naming, the two stamp strategies) lives in a gated module `briar_imperium`; `tools/briar.c` is a thin main that resolves the corpus exactly as silex does (`-f` > ascent > embedded), drives fabrica (plan 2), runs the generated build script through `processus_exsequi`, and `processus_transformare`s into the built binary. The corpus-embedding block of `silex_struere.sh` is extracted into `tools/corpus_infixum.sh` and sourced by both build scripts, so one generated object serves both binaries. A freshness gate runs the installed binary from a directory outside the repository with a fake `HOME`.

**Tech Stack:** C89 in house Latin; plan 2's `briar_fabrica`/`briar_silva`/`briar_nexus`; silex corpus (`silex_fons_corporis`, `silex_fons_disci`, `silex_fabricam_invenire`); `processus` (exsequi, transformare); `sigillum`; bash for the two scripts; pythonica gate table.

**Spec:** `project-specs/briar-spec.md` v1.2 — §4.1 (build home and cache), §4.5 (run), §5 (binary, flags, shebang form, struere, fumus), §8 P4. This plan amends §4.1 (disk-corpus stamp), §5 (hand-parsed flags, why not `argumenta`), §8 (P4 done) — Task 5 writes them.

## Executed 2026-09-05 — deviations from the text below

All five tasks done on branch `rhubarb-briar` (9cb3bb50 corpus block,
39a03a8a binary = T2+T3 in one commit, T4 fumus, T5 seal). Where the
code differs from the plan, the code is right:

- Tasks 2 and 3 landed as one commit: `tools/briar.c` was written once,
  complete, because splitting it meant writing the file twice.
- The `-arbor` projection lives in a new unit `briar_proiectio`
  (`briar_proiectionem_scribere`): `tools/briar.c` holds silva AND
  would have needed `materia_arbor.h` (stml) — the plan-2 lesson again.
- `-versio` no longer returns from the flag loop early; `-f` after it
  is honored (`briar -versio -f /nonexistent` now refuses, exit 1).
- The silex warnings go through a variadic `_monere` helper guarded by
  the switch, not three `si` wrappers.
- `silex_struere.sh` also gained a filter: `build/*.o` minus
  `probatio_*.o` — fourteen leftover test objects (pre-d55b9096) broke
  the link on main too (`capsula_speculi_hospes`).
- The fumus runs `briar -versio` from the area to prove the embedded
  corpus; the plant fixture is `adversa/probatio_rubra.thistle`.
- `-agere` (window + `bin/manus`) was NOT run in this session (GUI,
  by Fran's hand); stages 0–VI are the gate.

## Global Constraints

- **Worktree:** all work in `/Users/francisarant/Documents/projects/rhubarb-briar`. `./silva/scribe.sh` for new C files. Commit via `silva.commissio_umbra(msg, viae, portae)`; goldens/fixtures listed as individual files. Formator `-scribere` then `-vitia` = CONFORMIS before each commit.
- **`~/.bin` is shared by every tree.** `tools/briar_struere.sh` installs `~/.bin/briar` — that is intended (briar exists only on this branch). **Never run `./tools/silex_struere.sh` from the worktree with the real `HOME`** (it would overwrite `~/.bin/silex` with a branch build); Task 1 verifies it with `HOME` redirected to a scratch directory.
- **zsh does not word-split `$VAR`** in the Bash tool: pass file lists explicitly.
- **The runner links sqlite + the silva amalgam** (plan 2); a cold `./briar/compile_probationes.sh` is ~21 s once.
- **Forbidden identifiers:** every latina.h word; single capitals are numerals. No `//`, no declaration after statement. POSIX `.c` files put `postulata_posix.h` first.
- **Every gate is born red by a planted fault**, reverted before commit.
- **GUI:** `tools/briar_fumus.sh -agere` opens a real window and drives it with `bin/manus`; it is a by-hand step, never part of the default gate (memory: ask before focus-stealing sweeps).
- **Exit 2 = NOTHING RAN** for every runner and for the fumus script.

## Decisions taken in this plan (Fran may override before execution)

1. **Flags are hand-parsed, not `argumenta`.** briar's shape is `[-flag] [-f <radix>] file [args…]` with pass-through: everything after the file (except a reserved first argument) belongs to the program, and a program's own `-foo` must not be refused. `argumenta` refuses undeclared flags and has no stop-at-positional. The declared-options spirit is kept: exactly six words are recognized before the file, anything else starting with `-` there is a refusal.
2. **Disk-corpus stamp = hash of the closure contents.** The house `filum` has no mtime accessor, and a walk of `include/ lib/ vendor/` would cost a POSIX `stat` per file. Instead, when the corpus is a disk tree, the fabrica runs first and the stamp is `clausura:<SHA-256 of every closure file's bytes, in closure order>` — the key moves exactly when a file that goes into the project changes. Cost per run: one parse + closure read (~tens of ms). With the EMBEDDED corpus the stamp is `corpus.versio` and the key is computed from bytes before any parse (the fast path the spec describes).
3. **`-probatio` execs the generated `probare.sh` when the probatio binary is absent** (that script compiles and `exec`s it, so the exit code propagates) and execs `bin/probatio_<t>` directly when present.
4. **silex gains a quiet switch** (`silex_monitiones_tacere`): its closure collector prints "monitio - nulla implementatio pro latina.h" to stderr for every header-only file, and a script tool cannot print that on every cache miss. A one-flag library touch; `probatio_silex` unchanged.

## File structure

| path | role |
|---|---|
| `tools/corpus_infixum.sh` | CREATE: `corpus_infixum_regenerare` (the block extracted from `silex_struere.sh`) |
| `tools/silex_struere.sh` | MODIFY: sources the block |
| `briar/fontes/briar_imperium.h/.c` | CREATE: flag rules, reserved first argument, project dir, stamps |
| `briar/probationes/probatio_briar_imperium.c` | CREATE: gate |
| `tools/briar.c` | CREATE: the binary |
| `tools/briar_struere.sh` | CREATE: build + install `~/.bin/briar` |
| `include/silex.h`, `lib/silex.c` | MODIFY: `silex_monitiones_tacere` |
| `tools/briar_fumus.sh` | CREATE: freshness gate from outside the repo |
| `pythonica/silva.py` | MODIFY: `'briar-fumus'` row in `PORTAE` |
| docs: `briar/CLAUDE.md`, worklog, spec v1.3, memory, ledger | MODIFY |

---

### Task 1: `tools/corpus_infixum.sh` — one corpus object for two binaries

**Files:**
- Create: `tools/corpus_infixum.sh`
- Modify: `tools/silex_struere.sh` (lines 56–103: the block from `CORPUS_C=build/capsula_corpus_silicis.c` through the `mv … build/ || exit 1` + `fi`)

**Interfaces:**
- Produces: a bash function `corpus_infixum_regenerare` that, run from the repo root, leaves `build/capsula_corpus_silicis.c` and `.h` fresh (regenerating only when a corpus source is newer), writes `corpus.versio` and `corpus_silicis.toml` exactly as today.

- [ ] **Step 1: Extract**

Create `tools/corpus_infixum.sh`:

```bash
#!/bin/bash
# tools/corpus_infixum.sh - corpus bibliothecarum INFIXUM: capsula
# build/capsula_corpus_silicis.{c,h} e lib/ include/ vendor/ canonibus
# (regenerata SOLUM cum fons recentior; stampa = commit eius temporis,
# SORDIDUM si arbor sordida in contentis corporis).
#
# Fons COMMUNIS duorum binariorum: tools/silex_struere.sh et
# tools/briar_struere.sh eum 'source' faciunt et
# corpus_infixum_regenerare vocant e radice arboris. Obiectum unum,
# stampa una - 'silex -versio' et 'briar -versio' eandem dicunt.
#
# Usus (e radice): source tools/corpus_infixum.sh; corpus_infixum_regenerare

corpus_infixum_regenerare () {
    local CORPUS_C=build/capsula_corpus_silicis.c
    local regen=0
    if [ ! -f "$CORPUS_C" ]; then
        regen=1
    elif [ -n "$(find lib include vendor tools/capsula_generare.c \
            natura/cocta canones.registrum natura/natura.canon \
            aedilis.canon canon.canon silva/grammatica/grammatica.canon \
            silva/quaestiones.canon \
            -newer "$CORPUS_C" -print -quit 2>/dev/null)" ]; then
        regen=1
    fi
    if [ "$regen" = 1 ]; then
        echo "  [corpus] stampa + capsula (tardum semel)"
        local STAMPA="commit=$(git rev-parse --short HEAD 2>/dev/null \
            || echo ignotum)"
        # sorditia SCOPATA ad contenta corporis - plagulae aliae (FAQ,
        # gesta) semper mutatae sunt nec in capsulam eunt
        if [ -n "$(git status --porcelain -- lib include vendor \
                tools/capsula_generare.c natura/cocta canones.registrum \
                natura/natura.canon aedilis.canon canon.canon \
                silva/grammatica/grammatica.canon \
                silva/quaestiones.canon 2>/dev/null)" ]; then
            STAMPA="$STAMPA SORDIDUM"
        fi
        STAMPA="$STAMPA dies=$(date -u +%Y-%m-%dT%H:%M:%SZ)"
        printf '%s\n' "$STAMPA" > corpus.versio
        cat > corpus_silicis.toml <<'TOML'
# GENERATUM a tools/corpus_infixum.sh - NE MANU EDITES (gitignoratum)
corpus_silicis_files = ["lib/*.c", "lib/*.m", "include/*.h", "vendor/*", "tools/capsula_generare.c", "corpus.versio", "natura/cocta/*.canon", "natura/cocta/semina.census", "canones.registrum", "natura/natura.canon", "aedilis.canon", "canon.canon", "silva/grammatica/*.canon", "silva/quaestiones.canon"]
corpus_silicis_compress = true
TOML
        if [ ! -x bin/capsula_generare ]; then
            ./compile_tools.sh capsula_generare >/dev/null || return 1
        fi
        ./bin/capsula_generare corpus_silicis.toml || return 1
        mv capsula_corpus_silicis.h capsula_corpus_silicis.c build/ \
            || return 1
    fi
    return 0
}
```

Copy the `find`/`git status` file lists VERBATIM from the current `silex_struere.sh` (compare with `diff <(sed -n '56,103p' tools/silex_struere.sh) …` by eye) — the toml line's comment changes from "a silex_struere.sh" to "a tools/corpus_infixum.sh", nothing else.

In `tools/silex_struere.sh` replace lines 53–103 (from the comment `# corpus bibliothecarum: capsula infixa…` through the closing `fi`) with:

```bash
# corpus bibliothecarum: capsula infixa - bloccus COMMUNIS cum
# tools/briar_struere.sh (tools/corpus_infixum.sh)
source "$SCRIPT_DIR/corpus_infixum.sh"
corpus_infixum_regenerare || exit 1
```

- [ ] **Step 2: Verify without touching `~/.bin`**

```bash
bash -n tools/corpus_infixum.sh tools/silex_struere.sh
rm -f build/capsula_corpus_silicis.c build/capsula_corpus_silicis.h
DOMUS="$SCRATCH/domus_silex"; mkdir -p "$DOMUS"
HOME="$DOMUS" ./tools/silex_struere.sh 2>&1 | tail -4
ls -la build/capsula_corpus_silicis.c "$DOMUS/.bin/silex" | awk '{print $5, $9}'
cat corpus.versio
"$DOMUS/.bin/silex" -versio
```
Expected: `[corpus] stampa + capsula (tardum semel)`, `aedificatum: bin/silex`, `institutum: …/domus_silex/.bin/silex`; the real `~/.bin/silex` untouched (`ls -la ~/.bin/silex` mtime unchanged); `-versio` prints the stamp from `corpus.versio`. (`build/*.o` must exist — run `./compile_tests.sh registrum` first if `build/` is empty in the worktree.)

- [ ] **Step 3: Commit**

`commissio_umbra` portae `['briar']` (no C changed; the silex gate does not exercise the script), viae `tools/corpus_infixum.sh`, `tools/silex_struere.sh`. Message: extraction, verified with redirected HOME, byte-identical corpus behavior.

---

### Task 2: `briar_imperium` — the CLI rules, gated; `tools/briar.c` with `-versio`, `-arbor`, `-partes`; `briar_struere.sh`

**Files:**
- Create: `briar/fontes/briar_imperium.h`, `briar/fontes/briar_imperium.c`
- Test: `briar/probationes/probatio_briar_imperium.c`
- Create: `tools/briar.c`, `tools/briar_struere.sh`

**Interfaces:**
```c
nomen enumeratio {
    BRIAR_ACTIO_CURRERE = 0,   /* (nihil) */
    BRIAR_ACTIO_PROBATIO,      /* -probatio */
    BRIAR_ACTIO_STRUERE,       /* -struere [-iterum] */
    BRIAR_ACTIO_ARBOR,         /* -arbor */
    BRIAR_ACTIO_PARTES,        /* -partes */
    BRIAR_ACTIO_VERSIO,        /* -versio (sine plagula) */
    BRIAR_ACTIO_AUXILIUM       /* -auxilium / -h (sine plagula) */
} BriarActio;

nomen structura {
                    BriarActio  actio;
                           b32  iterum;        /* -iterum (struere) */
            constans character* via;           /* plagula .thistle aut NIHIL */
            constans character* fabrica;       /* -f <radix> aut NIHIL */
    constans character* constans* reliqua;     /* argumenta programmatis (NIHIL-terminata) */
                           i32  numerus_reliquorum;
                        chorda  causa;         /* recusatio (mensura > 0) */
} BriarImperium;

/* argv[1..] legere. Ante plagulam: -probatio -struere -iterum -arbor
 * -partes -versio -auxilium -h, -f <radix>; aliud '-…' = recusatio.
 * Post plagulam: argumentum PRIMUM solum inspicitur - si unum e
 * quinque vexillis (-probatio -struere -arbor -partes -versio) actio
 * fit; '--' primum = finis vexillorum (abscisum); cetera reliqua. */
b32 briar_imperium_legere (Piscina*, i32 argc, constans character* constans* argv, BriarImperium* out);

/* $HOME/.rhubarb/briar/<titulus>-<sigillum>; NIHIL si HOME abest */
constans character* briar_domus_proiecti (Piscina*, constans character* titulus, constans character* sigillum_xvi);

/* stampa fontis disci: 'clausura:' + SHA-256 XVI hex super contenta
 * omnium plagularum clausurae, ordine clausurae */
constans character* briar_stampa_clausurae (Piscina*, constans Xar* clausura);

/* XVI hex de SHA-256 chordae vexillorum (pro -versio) */
vacuum briar_vexilla_sigillum (constans character* vexilla, character* hex_xvii);
```

- [ ] **Step 1: The failing gate**

`briar/probationes/probatio_briar_imperium.c` (scribe):

```c
/* probatio_briar_imperium.c - Regulae imperii: vexilla ante plagulam,
 * argumentum primum reservatum post eam, '--', -f, recusationes,
 * domus proiecti, stampa clausurae, sigillum vexillorum.
 */

#include "latina.h"
#include "credo.h"
#include "briar_imperium.h"
#include "piscina.h"
#include "silex.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior b32
_legere (
          Piscina* piscina,
    BriarImperium* imp,
              i32  n,
    constans character* a1,
    constans character* a2,
    constans character* a3,
    constans character* a4,
    constans character* a5)
{
    constans character* argv[7];

    argv[0] = "briar";
    argv[1] = a1; argv[2] = a2; argv[3] = a3; argv[4] = a4; argv[5] = a5;
    argv[6] = NIHIL;
    redde briar_imperium_legere(piscina, n + I, argv, imp);
}

s32
principale (vacuum)
{
              b32  praeteritus;
          Piscina* piscina;
    BriarImperium  imp;

    piscina = piscina_generare_dynamicum("probatio_briar_imperium",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans formae imperii ---\n");
    /* briar x.thistle a b : currere, reliqua II */
    CREDO_VERUM (_legere(piscina, &imp, 3, "x.thistle", "a", "b", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_VERUM (strcmp(imp.via, "x.thistle") == ZEPHYRUM);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, II);
    CREDO_VERUM (strcmp(imp.reliqua[0], "a") == ZEPHYRUM);
    CREDO_VERUM (strcmp(imp.reliqua[1], "b") == ZEPHYRUM);
    CREDO_VERUM (imp.reliqua[2] == NIHIL);
    CREDO_VERUM (imp.fabrica == NIHIL);
    CREDO_FALSUM (imp.iterum);

    /* briar -probatio x.thistle : probatio */
    CREDO_VERUM (_legere(piscina, &imp, 2, "-probatio", "x.thistle", NIHIL, NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_PROBATIO);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* forma shebang: briar ./x.thistle -probatio : argumentum primum reservatum */
    CREDO_VERUM (_legere(piscina, &imp, 2, "./x.thistle", "-probatio", NIHIL, NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_PROBATIO);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, ZEPHYRUM);

    /* briar x.thistle -- -probatio : '--' abscisum, -probatio programmatis */
    CREDO_VERUM (_legere(piscina, &imp, 3, "x.thistle", "--", "-probatio", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, I);
    CREDO_VERUM (strcmp(imp.reliqua[0], "-probatio") == ZEPHYRUM);

    /* briar x.thistle -foo : vexillum alienum post plagulam = programmatis */
    CREDO_VERUM (_legere(piscina, &imp, 2, "x.thistle", "-foo", NIHIL, NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_CURRERE);
    CREDO_AEQUALIS_I32 (imp.numerus_reliquorum, I);

    /* briar x.thistle -probatio a : reservatum + reliqua */
    CREDO_VERUM (_legere(piscina, &imp, 3, "x.thistle", "-struere", "-iterum", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_STRUERE);
    CREDO_VERUM (imp.iterum);

    /* -struere -iterum ante plagulam */
    CREDO_VERUM (_legere(piscina, &imp, 3, "-struere", "-iterum", "x.thistle", NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_STRUERE);
    CREDO_VERUM (imp.iterum);

    /* -f radix */
    CREDO_VERUM (_legere(piscina, &imp, 3, "-f", "/tmp/rhubarb", "x.thistle", NIHIL, NIHIL));
    CREDO_VERUM (strcmp(imp.fabrica, "/tmp/rhubarb") == ZEPHYRUM);

    /* -versio / -auxilium sine plagula */
    CREDO_VERUM (_legere(piscina, &imp, 1, "-versio", NIHIL, NIHIL, NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_VERSIO);
    CREDO_VERUM (_legere(piscina, &imp, 1, "-h", NIHIL, NIHIL, NIHIL, NIHIL));
    CREDO_AEQUALIS_S32 ((s32)imp.actio, (s32)BRIAR_ACTIO_AUXILIUM);

    imprimere("\n--- Probans recusationes ---\n");
    CREDO_FALSUM (_legere(piscina, &imp, 2, "-ignotum", "x.thistle", NIHIL, NIHIL, NIHIL));
    CREDO_VERUM (imp.causa.mensura > ZEPHYRUM);
    CREDO_FALSUM (_legere(piscina, &imp, 0, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, 1, "-probatio", NIHIL, NIHIL, NIHIL, NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, 2, "-iterum", "x.thistle", NIHIL, NIHIL, NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, 2, "-f", "x.thistle", NIHIL, NIHIL, NIHIL));
    CREDO_FALSUM (_legere(piscina, &imp, 3, "-probatio", "-struere", "x.thistle", NIHIL, NIHIL));

    imprimere("\n--- Probans domus, stampa, sigillum ---\n");
    {
        constans character* d;
        character hex[17];

        setenv("HOME", "/tmp/domus_ficta", 1);
        d = briar_domus_proiecti(piscina, "salve", "0123456789abcdef");
        CREDO_NON_NIHIL (d);
        CREDO_VERUM (strcmp(d,
            "/tmp/domus_ficta/.rhubarb/briar/salve-0123456789abcdef")
            == ZEPHYRUM);
        unsetenv("HOME");
        CREDO_VERUM (briar_domus_proiecti(piscina, "salve", "0") == NIHIL);

        briar_vexilla_sigillum("-Wall", hex);
        CREDO_AEQUALIS_I32 ((i32)strlen(hex), (i32)16);
        {
            Xar* clausura = xar_creare(piscina, (i32)magnitudo(SilexRes));
            SilexRes* r = (SilexRes*)xar_addere(clausura);
            constans character* s1;
            constans character* s2;

            r->via       = chorda_ex_literis("include/a.h", piscina);
            r->contentum = chorda_ex_literis("int a;", piscina);
            r->origo     = "probatio";
            s1 = briar_stampa_clausurae(piscina, clausura);
            CREDO_VERUM (strncmp(s1, "clausura:", (size_t)9) == ZEPHYRUM);
            CREDO_AEQUALIS_I32 ((i32)strlen(s1), (i32)25);
            r->contentum = chorda_ex_literis("int b;", piscina);
            s2 = briar_stampa_clausurae(piscina, clausura);
            CREDO_VERUM (strcmp(s1, s2) != ZEPHYRUM);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
```
(`setenv`/`unsetenv` are POSIX: add `#include "postulata_posix.h"` as the FIRST include of the test.)

Run: `./briar/compile_probationes.sh imperium` → FRACTA (compilatio).

- [ ] **Step 2: Module**

`briar/fontes/briar_imperium.h` (scribe) — the interfaces above with a head comment stating the two rules (before the file; first argument after it) and the reason `argumenta` is not used. `briar/fontes/briar_imperium.c`:

```c
/* briar_imperium.c - Vide briar_imperium.h. */

#include "briar_imperium.h"
#include "chorda_aedificator.h"
#include "sigillum.h"
#include <stdlib.h>
#include <string.h>

interior b32
_est (
    constans character* a,
    constans character* b)
{
    redde (b32)(a != NIHIL && strcmp(a, b) == ZEPHYRUM);
}

/* vexillum actionis: actio aut -I */
interior s32
_actio_vexilli (
    constans character* a)
{
    si (_est(a, "-probatio")) redde (s32)BRIAR_ACTIO_PROBATIO;
    si (_est(a, "-struere"))  redde (s32)BRIAR_ACTIO_STRUERE;
    si (_est(a, "-arbor"))    redde (s32)BRIAR_ACTIO_ARBOR;
    si (_est(a, "-partes"))   redde (s32)BRIAR_ACTIO_PARTES;
    si (_est(a, "-versio"))   redde (s32)BRIAR_ACTIO_VERSIO;
    redde -I;
}

interior vacuum
_recusare (
    BriarImperium* imp,
         Piscina* piscina,
    constans character* causa)
{
    imp->causa = chorda_ex_literis(causa, piscina);
}

b32
briar_imperium_legere (
                         Piscina* piscina,
                             i32  argc,
    constans character* constans* argv,
                   BriarImperium* imp)
{
    i32 i;
    b32 actio_data = FALSUM;

    memset(imp, 0, magnitudo(*imp));
    imp->actio = BRIAR_ACTIO_CURRERE;
    /* I. ante plagulam */
    per (i = I; i < argc && imp->via == NIHIL; i++)
    {
        constans character* a = argv[i];
        s32 actio = _actio_vexilli(a);

        si (_est(a, "-h") || _est(a, "-auxilium"))
        {
            imp->actio = BRIAR_ACTIO_AUXILIUM;
            redde VERUM;
        }
        alioquin si (actio >= ZEPHYRUM)
        {
            si (actio_data)
            {
                _recusare(imp, piscina, "vexilla duo actionis: unum elige");
                redde FALSUM;
            }
            imp->actio = (BriarActio)actio;
            actio_data = VERUM;
            si (imp->actio == BRIAR_ACTIO_VERSIO)
            {
                redde VERUM;
            }
        }
        alioquin si (_est(a, "-iterum"))
        {
            imp->iterum = VERUM;
        }
        alioquin si (_est(a, "-f"))
        {
            si (i + I >= argc || argv[i + I][0] == '-'
                || strstr(argv[i + I], ".thistle") != NIHIL)
            {
                _recusare(imp, piscina, "-f sine radice");
                redde FALSUM;
            }
            imp->fabrica = argv[i + I];
            i = i + I;
        }
        alioquin si (a[0] == '-' && a[1] != '\0')
        {
            ChordaAedificator* aed = chorda_aedificator_creare(piscina,
                (memoriae_index)128);

            chorda_aedificator_appendere_literis(aed, "vexillum ignotum: ");
            chorda_aedificator_appendere_literis(aed, a);
            chorda_aedificator_appendere_literis(aed,
                " (nota: -probatio -struere [-iterum] -arbor -partes -versio"
                " -f <radix>)");
            imp->causa = chorda_aedificator_finire(aed);
            redde FALSUM;
        }
        alioquin
        {
            imp->via = a;
        }
    }
    si (imp->via == NIHIL)
    {
        _recusare(imp, piscina,
            "plagula .thistle deest (usus: briar [-vexillum] <x.thistle> [argumenta])");
        redde FALSUM;
    }
    si (imp->iterum && imp->actio != BRIAR_ACTIO_STRUERE)
    {
        _recusare(imp, piscina, "-iterum solum cum -struere");
        redde FALSUM;
    }
    /* II. argumentum primum post plagulam: reservatum aut '--' */
    si (i < argc)
    {
        s32 actio = _actio_vexilli(argv[i]);

        si (_est(argv[i], "--"))
        {
            i = i + I;
        }
        alioquin si (actio >= ZEPHYRUM)
        {
            si (actio_data && (BriarActio)actio != imp->actio)
            {
                _recusare(imp, piscina, "vexilla duo actionis: unum elige");
                redde FALSUM;
            }
            imp->actio = (BriarActio)actio;
            i = i + I;
            si (imp->actio == BRIAR_ACTIO_STRUERE && i < argc
                && _est(argv[i], "-iterum"))
            {
                imp->iterum = VERUM;
                i = i + I;
            }
        }
    }
    /* III. reliqua (NIHIL-terminata) */
    imp->numerus_reliquorum = argc - i;
    {
        constans character** r = (constans character**)piscina_allocare(
            piscina, (memoriae_index)((imp->numerus_reliquorum + I)
                * (i32)magnitudo(constans character*)));
        i32 k;

        per (k = ZEPHYRUM; k < imp->numerus_reliquorum; k++)
        {
            r[k] = argv[i + k];
        }
        r[imp->numerus_reliquorum] = NIHIL;
        imp->reliqua = (constans character* constans*)r;
    }
    redde VERUM;
}

constans character*
briar_domus_proiecti (
               Piscina* piscina,
    constans character* titulus,
    constans character* sigillum_xvi)
{
    constans character* domus = getenv("HOME");
     ChordaAedificator* aed;

    si (domus == NIHIL || domus[0] == '\0')
    {
        redde NIHIL;
    }
    aed = chorda_aedificator_creare(piscina, (memoriae_index)256);
    chorda_aedificator_appendere_literis(aed, domus);
    chorda_aedificator_appendere_literis(aed, "/.rhubarb/briar/");
    chorda_aedificator_appendere_literis(aed, titulus);
    chorda_aedificator_appendere_literis(aed, "-");
    chorda_aedificator_appendere_literis(aed, sigillum_xvi);
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

constans character*
briar_stampa_clausurae (
         Piscina* piscina,
    constans Xar* clausura)
{
    SigillumContextus ctx;
             Sigillum s;
            character hex[SIGILLUM_HEX_MENSURA];
            character* r;
                  i32 i;

    sigillum_incipere(&ctx);
    per (i = ZEPHYRUM; i < xar_numerus(clausura); i++)
    {
        constans SilexRes* e = (constans SilexRes*)xar_obtinere(
            (Xar*)clausura, i);

        sigillum_addere(&ctx, e->via.datum, (memoriae_index)e->via.mensura);
        sigillum_addere(&ctx, "\n", (memoriae_index)I);
        sigillum_addere(&ctx, e->contentum.datum,
            (memoriae_index)e->contentum.mensura);
        sigillum_addere(&ctx, "\n", (memoriae_index)I);
    }
    s = sigillum_finire(&ctx);
    sigillum_hex(&s, hex);
    r = (character*)piscina_allocare(piscina, (memoriae_index)26);
    memcpy(r, "clausura:", (size_t)9);
    memcpy(r + 9, hex, (size_t)16);
    r[25] = '\0';
    redde r;
}

vacuum
briar_vexilla_sigillum (
    constans character* vexilla,
             character* hex_xvii)
{
    Sigillum  s = sigillum_computare(vexilla, (memoriae_index)strlen(vexilla));
    character hex[SIGILLUM_HEX_MENSURA];

    sigillum_hex(&s, hex);
    memcpy(hex_xvii, hex, (size_t)16);
    hex_xvii[16] = '\0';
}
```
(`xar_obtinere` takes a non-const `Xar*`; the cast is deliberate and commented; if examen objects, take `Xar*` in the signature.)

Run: `./briar/compile_probationes.sh imperium` → green. Plant: in rule II skip the `_actio_vexilli` check (treat the first argument as a program argument) → the shebang-form assertion goes red. Revert.

- [ ] **Step 3: `tools/briar.c` — `-versio`, `-auxilium`, `-arbor`, `-partes`, corpus resolution**

```c
/* briar.c (instrumentum) - plagulas .thistle currere: programmata C89
 * litterata (prosa markdown, tags STML, regiones <c!>) cum '#!'.
 *
 *   briar [-vexillum] [-f <radix>] <x.thistle> [argumenta...]
 *   ./x.thistle [argumenta...]           (forma shebang: '#!/usr/bin/env briar')
 *
 * Vexilla (non verba - plagulae thistle scripta sunt): (nihil) = currere
 * (aedificare si abest, deinde programma FIERI); -probatio = probationem
 * regionis munus="probatio" currere (exitus = iudicium); -struere
 * [-iterum] = aedificare solum, directorium proiecti imprimere; -arbor =
 * proiectio STML; -partes = clausura; -versio = stampa corporis + sigillum
 * vexillorum. Forma shebang vexilla ut argumentum PRIMUM post plagulam
 * agnoscit ('./x.thistle -probatio'); '--' ea programmati relinquit.
 *
 * Corpus: -f > ascensus e cwd (intra arborem rhubarb: discus) > corpus
 * INFIXUM (usus ordinarius scripti). Aedificatio: tools/briar_struere.sh.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "capsula.h"
#include "filum.h"
#include "internamentum.h"
#include "processus.h"
#include "silex.h"
#include "xar.h"
#include "briar_arbor.h"
#include "briar_fabrica.h"
#include "briar_imperium.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "briar_stml.h"
#include "materia_arbor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* corpus bibliothecarum a struere genitum - IDEM obiectum quod silex
 * (tools/corpus_infixum.sh); externus directus */
/* <aedilis obiectum="build/capsula_corpus_silicis.c"/> */
externus constans CapsulaEmbed capsula_corpus_silicis;

#define BRIAR_VERSIO "v0"
#define BRIAR_MORA_AEDIFICANDI_MS 600000

interior character*
_plagulam_legere (Piscina* piscina, constans character* via, i32* mensura)
{ /* idem ac briar/instrumenta/fabrica.c */ }

interior vacuum
_auxilium (vacuum)
{
    imprimere(
        "briar %s - plagulas .thistle currere\n"
        "usus: briar [-vexillum] [-f <radix>] <x.thistle> [argumenta...]\n"
        "      ./x.thistle [-vexillum] [argumenta...]\n"
        "  (nihil)     aedificare si abest, deinde programma fieri\n"
        "  -probatio   probationem regionis munus=\"probatio\" currere\n"
        "  -struere    aedificare solum; -iterum = clavem neglegere\n"
        "  -arbor      proiectionem STML imprimere\n"
        "  -partes     clausuram imprimere (ADEST/ABEST)\n"
        "  -versio     stampam corporis et sigillum vexillorum\n"
        "  -f <radix>  arbor rhubarb (alioquin ascensus, alioquin corpus infixum)\n"
        "  --          post plagulam: vexilla programmati relinquere\n",
        BRIAR_VERSIO);
}

/* fons: -f > ascensus > infixum; *e_disco VERUM si discus */
interior SilexFons*
_fontem_aperire (Piscina* piscina, constans character* fabrica_opt, b32* e_disco)
{
    SilexFons* fons = NIHIL;
    constans character* fabrica = fabrica_opt;

    *e_disco = FALSUM;
    si (fabrica == NIHIL)
    {
        fabrica = silex_fabricam_invenire(piscina, ".");
    }
    si (fabrica != NIHIL)
    {
        fons = silex_fons_disci(piscina, fabrica);
        si (fons == NIHIL && fabrica_opt != NIHIL)
        {
            fprintf(stderr, "briar: fabrica invalida (include/ deest): %s\n",
                fabrica);
            redde NIHIL;
        }
        *e_disco = (b32)(fons != NIHIL);
    }
    si (fons == NIHIL)
    {
        fons = silex_fons_corporis(piscina, &capsula_corpus_silicis);
    }
    si (fons == NIHIL)
    {
        fprintf(stderr, "briar: nec fabrica nec corpus - binarium sine"
            " corpore aedificatum?\n");
    }
    redde fons;
}

s32
principale (integer argc, character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("briar", 33554432);
    BriarImperium imp;
    SilexFons* fons;
    b32 e_disco;

    si (!briar_imperium_legere(piscina, (i32)argc,
        (constans character* constans*)argv, &imp))
    {
        fprintf(stderr, "briar: %.*s\n", (integer)imp.causa.mensura,
            (constans character*)imp.causa.datum);
        redde II;
    }
    si (imp.actio == BRIAR_ACTIO_AUXILIUM)
    {
        _auxilium();
        redde ZEPHYRUM;
    }
    silex_monitiones_tacere(VERUM);   /* Task 3 */
    fons = _fontem_aperire(piscina, imp.fabrica, &e_disco);
    si (fons == NIHIL)
    {
        redde I;
    }
    si (imp.actio == BRIAR_ACTIO_VERSIO)
    {
        character hp[17];
        character hv[17];

        briar_vexilla_sigillum(briar_fabrica_vexilla(BRIAR_FORMA_PLANA), hp);
        briar_vexilla_sigillum(briar_fabrica_vexilla(BRIAR_FORMA_VITREA), hv);
        imprimere("briar %s\ncorpus: %s%s\nvexilla: plana %s vitrea %s\n",
            BRIAR_VERSIO, fons->titulus, e_disco ? " (discus)" : "",
            hp, hv);
        redde ZEPHYRUM;
    }
    /* … Task 3: -arbor, -partes, currere, -probatio, -struere … */
    redde ZEPHYRUM;
}
```
In this task implement `-arbor` (parse + `materia_arbor_scribere_nodum` with `briar_stml_consilium`, as `briar/instrumenta/arbor.c` does — copy its body) and `-partes` (fabricate, then print each closure `via` + `origo` one per line, `ADEST` when `silex_fons_existit`). Leave `currere/probatio/struere` as `fprintf(stderr, "briar: nondum (plan 3 T3)\n"); redde II;` for now.

- [ ] **Step 4: `tools/briar_struere.sh`**

```bash
#!/bin/bash
# tools/briar_struere.sh - bin/briar aedificare et in ~/.bin instituere
#
# Obiecta: briar/build/*.o (cursor briar/compile_probationes.sh ea
# struit: clausura silicis + sqlite + amalgama silvae + materia + md +
# briar) + corpus infixum build/capsula_corpus_silicis.c (bloccus
# COMMUNIS cum silex_struere.sh: tools/corpus_infixum.sh). ORDO: suite
# briar prius (obiecta), deinde hoc.
#
# Usus: ./tools/briar_struere.sh
set -u
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR/.."
source "$SCRIPT_DIR/vexilla.sh"
declare -a GCC_FLAGS=("${VEXILLA_C89[@]}")

if [ -z "$(ls -A briar/build/briar_*.o 2>/dev/null)" ]; then
    echo "briar_struere: briar/build/ vacuum - ./briar/compile_probationes.sh registrum prius" >&2
    exit 1
fi
if [ -z "$(ls -A build/*.o 2>/dev/null)" ]; then
    echo "briar_struere: build/ vacuum - ./compile_tests.sh registrum prius (capsula_generare)" >&2
    exit 1
fi

source "$SCRIPT_DIR/corpus_infixum.sh"
corpus_infixum_regenerare || exit 1

OBJ=""
for o in briar/build/*.o; do
    case "$(basename "$o")" in probatio_*) continue ;; esac
    OBJ="$OBJ $o"
done

mkdir -p bin
echo "  [briar] tools/briar.c + corpus"
clang "${GCC_FLAGS[@]}" -Iinclude -Imateria/fontes -Imd/fontes \
    -Ibriar/fontes -Isilva/amalgama \
    tools/briar.c build/capsula_corpus_silicis.c $OBJ \
    -o bin/briar || exit 1
echo "aedificatum: bin/briar"

mkdir -p "$HOME/.bin"
rm -f "$HOME/.bin/briar"
cp bin/briar "$HOME/.bin/briar"
echo "institutum: ~/.bin/briar"
```
(`$OBJ` is built by a loop, so it is a real space-separated string and `bash` splits it — this is bash, not zsh.) If linking complains about missing symbols (e.g. an `.m` object), add the named object from `build/` — briar's closure has no GUI, so no frameworks are expected.

Run: `./briar/compile_probationes.sh imperium && ./tools/briar_struere.sh && bin/briar -versio && bin/briar -versio -f /nonexistent; echo "exit=$?"; (cd /tmp && ~/.bin/briar -versio)`.
Expected: the version with `corpus: <stampa> (discus)` inside the tree, `corpus: commit=… dies=…` from `/tmp` (embedded), the invalid `-f` refused with exit 1, `bin/briar -arbor briar/probationes/fixa/thistle/salve.thistle | head -3` prints the STML projection, `-partes` lists `include/chorda.h ADEST` etc. Record `ls -la bin/briar` size (spec §10: two corpus-bearing binaries).

- [ ] **Step 5: Commit**

Formator on the three new C files. `commissio_umbra` portae `['briar']`; viae the module, test, `tools/briar.c`, `tools/briar_struere.sh`. (`tools/briar.c` is compiled by no suite — the fumus of Task 4 covers it; say so in the message.)

---

### Task 3: run, `-probatio`, `-struere`; the silex quiet switch

**Files:**
- Modify: `include/silex.h`, `lib/silex.c` (`silex_monitiones_tacere`)
- Modify: `tools/briar.c`

- [ ] **Step 1: silex quiet switch**

In `lib/silex.c` find the `fprintf(stderr, "silex: monitio - nulla implementatio pro %s …` site(s) (`grep -n 'monitio' lib/silex.c`). Add near the top of the file:

```c
/* monitiones (nulla implementatio pro caput solum...) in stderr:
 * instrumentum scripti (briar) eas tacere vult */
hic_manens b32 _monitiones_tacitae = FALSUM;

vacuum
silex_monitiones_tacere (
    b32 tacere)
{
    _monitiones_tacitae = tacere;
}
```
and guard each monitio print with `si (!_monitiones_tacitae)`. Prototype in `silex.h` after `silex_fabricam_invenire`:
```c
/* monitiones stderr (caput sine implementatione...) tacere; ordinarium
 * FALSUM (silex loquax, ut hodie) */
vacuum
silex_monitiones_tacere (
    b32 tacere);
```
Run `./compile_tests.sh silex` → green (default unchanged). Also call `silex_monitiones_tacere(VERUM)` at the top of `briar/instrumenta/fabrica.c` and in the fabrica/silva gates' `principale` (the suite logs lose the noise).

- [ ] **Step 2: The run path in `tools/briar.c`**

After the `-versio` branch:

```c
    /* plagula legere, arbor, nexus, silva */
    {
        character* textus;
        i32 mensura = ZEPHYRUM;
        MateriaNodus* doc;
        InternamentumChorda* intern = internamentum_creare(piscina);
        Xar* nexus;
        BriarFabricaOptiones optiones;
        BriarFabricaFructus fructus;
        chorda octeti;
        constans character* stampa;
        constans character* dir;
        character clavis[17];

        textus = _plagulam_legere(piscina, imp.via, &mensura);
        si (textus == NIHIL)
        {
            fprintf(stderr, "briar: plagula non lecta: %s\n", imp.via);
            redde I;
        }
        doc = briar_arbor_parsare(piscina, textus, mensura);
        si (imp.actio == BRIAR_ACTIO_ARBOR)
        {
            /* Task 2 */
        }
        nexus = briar_nexus_texere(piscina, doc, intern);
        si (nexus == NIHIL || briar_silvam_texere(piscina, nexus, fons) < ZEPHYRUM)
        {
            fprintf(stderr, "briar: nexus fractus\n");
            redde I;
        }
        octeti.datum = (i8*)textus;
        octeti.mensura = mensura;
        optiones.via_thistle  = via_absoluta_cstr(piscina, imp.via);   /* via.h: via_absoluta(chorda, piscina) */
        optiones.fons_titulus = fons->titulus;
        optiones.stampa       = fons->titulus;   /* infixum: corpus.versio; discus: post fabricam */
        fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones, octeti);
        si (!fructus.successus)
        {
            fprintf(stderr, "%s:%d: %.*s\n", imp.via, (integer)fructus.linea_causae,
                (integer)fructus.causa.mensura, (constans character*)fructus.causa.datum);
            redde I;
        }
        si (imp.actio == BRIAR_ACTIO_PARTES) { /* Task 2 */ }
        /* clavis: infixum = stampa corporis (iam in fructus.sigillum);
         * discus = stampa clausurae (contenta) - recomputata */
        stampa = e_disco ? briar_stampa_clausurae(piscina, fructus.clausura)
                         : fons->titulus;
        briar_fabrica_clavem_computare(stampa, briar_fabrica_vexilla(fructus.forma),
            octeti, clavis);
        dir = briar_domus_proiecti(piscina, fructus.titulus, clavis);
        si (dir == NIHIL)
        {
            fprintf(stderr, "briar: HOME ignotum\n");
            redde I;
        }
        /* aedificare si abest (aut -iterum) */
        {
            constans character* binarium = _texere(piscina, dir, "/bin/", fructus.titulus);
            b32 adest = filum_existit(binarium);

            si (!adest || imp.iterum)
            {
                chorda causa;
                constans character* ordo[3];
                ProcessusResultus res;

                si (!briar_fabricam_scribere(piscina, &fructus, dir, &causa))
                {
                    fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                        (constans character*)causa.datum);
                    redde I;
                }
                ordo[0] = "/bin/sh";
                ordo[1] = _texere(piscina, dir, "/aedificare.sh", NIHIL);
                ordo[2] = NIHIL;
                res = processus_exsequi(ordo, BRIAR_MORA_AEDIFICANDI_MS, piscina);
                si (!res.successus || res.codex_exitus != ZEPHYRUM)
                {
                    fwrite(res.effusio.datum, I, (size_t)res.effusio.mensura, stderr);
                    fwrite(res.erratum.datum, I, (size_t)res.erratum.mensura, stderr);
                    fprintf(stderr, "briar: aedificatio defecit (%s, exitus %d): %s\n",
                        res.successus ? "cucurrit" : processus_error_nomen(res.error),
                        (integer)res.codex_exitus, dir);
                    redde I;
                }
                si (imp.actio == BRIAR_ACTIO_STRUERE)
                {
                    fwrite(res.effusio.datum, I, (size_t)res.effusio.mensura, stdout);
                }
            }
            si (imp.actio == BRIAR_ACTIO_STRUERE)
            {
                imprimere("%s\n", dir);
                redde ZEPHYRUM;
            }
            si (imp.actio == BRIAR_ACTIO_PROBATIO)
            {
                constans character* probatio = _texere(piscina, dir, "/bin/probatio_", fructus.titulus);
                constans character* ordo[3];

                si (!fructus.probatio_adest)
                {
                    fprintf(stderr, "briar: %s: regio munus=\"probatio\" deest\n", imp.via);
                    redde II;
                }
                si (filum_existit(probatio))
                {
                    ordo[0] = probatio;
                    ordo[1] = NIHIL;
                }
                alioquin
                {
                    ordo[0] = "/bin/sh";
                    ordo[1] = _texere(piscina, dir, "/probare.sh", NIHIL);
                    ordo[2] = NIHIL;
                }
                fflush(stdout);
                (vacuum)processus_transformare(ordo);
                fprintf(stderr, "briar: exec defecit: %s\n", ordo[0]);
                redde I;
            }
            /* currere: programma FIERI */
            {
                constans character** ordo = (constans character**)piscina_allocare(piscina,
                    (memoriae_index)((imp.numerus_reliquorum + II) * (i32)magnitudo(constans character*)));
                i32 k;

                ordo[0] = binarium;
                per (k = ZEPHYRUM; k < imp.numerus_reliquorum; k++)
                {
                    ordo[k + I] = imp.reliqua[k];
                }
                ordo[imp.numerus_reliquorum + I] = NIHIL;
                fflush(stdout);
                (vacuum)processus_transformare(ordo);
                fprintf(stderr, "briar: exec defecit: %s\n", binarium);
                redde I;
            }
        }
    }
```
Notes: `via_absoluta` is `chorda via_absoluta(chorda, Piscina*)` — wrap it (`_texere` is the fabrica tool's helper, copy it). The generated main's `-vivum`/`-portus` flags pass through `reliqua` untouched. With the embedded corpus a cache hit still parses the file (needed for `titulus` and the forma's flag string) — parse cost is ~1 ms; the silva pass is skipped ONLY if you restructure: acceptable for v1 to run the nexus too (~20 ms); record the hit latency in Task 5.

- [ ] **Step 3: Run it by hand, inside the tree and outside**

```bash
./tools/briar_struere.sh
time bin/briar briar/probationes/fixa/thistle/salve.thistle        # cold: builds, prints salve, munde
time bin/briar briar/probationes/fixa/thistle/salve.thistle        # hit
bin/briar -probatio briar/probationes/fixa/thistle/salve.thistle; echo "exitus=$?"
bin/briar -struere -iterum briar/probationes/fixa/thistle/punctum.thistle
ls ~/.rhubarb/briar/
# extra arborem, corpus infixum:
mkdir -p "$SCRATCH/extra" && cp briar/probationes/fixa/thistle/salve.thistle "$SCRATCH/extra/" && chmod +x "$SCRATCH/extra/salve.thistle"
(cd "$SCRATCH/extra" && ./salve.thistle && ./salve.thistle -probatio; echo "exitus=$?")
```
Expected: `salve, munde` both times; the probatio's `OMNIA PRAETERIERUNT` with exit 0; the struere line prints the project dir; the shebang run works from outside the repo. Record cold/hit times.

- [ ] **Step 4: Commit**

Formator on `tools/briar.c lib/silex.c include/silex.h`. `commissio_umbra` portae `[('radix', 'silex'), 'briar']`.

---

### Task 4: `tools/briar_fumus.sh` — the freshness gate from outside the repository

**Files:**
- Create: `tools/briar_fumus.sh`
- Modify: `pythonica/silva.py` (`PORTAE['briar-fumus'] = (['./tools/briar_fumus.sh'], r'fumus briar: (sanum|FRACTUM)')`)
- Create: `briar/probationes/fixa/thistle/adversa/probatio_rubra.thistle` (the plant: `CREDO_FALSUM(VERUM)` in its probatio)

- [ ] **Step 1: The script**

Model: `tools/silex_semen_fumus.sh` (its `deficere`/`purgare`, the fake `HOME` exported, exit 2 for missing prerequisites). Steps:

```
0. prerequisites: ~/.bin/briar executable (else exit 2); bin/manus when -agere
   HOME=<area>/domus exported; print `briar -versio`
I.   plain: cp fixa/thistle/salve.thistle → area; chmod +x; (cd area && ./salve.thistle) → stdout must contain "salve, munde";
     corpus must be EMBEDDED: `briar -versio` from the area must NOT say "(discus)"
II.  plain probatio: ./salve.thistle -probatio → exit 0 and "OMNIA PRAETERIERUNT"
III. punctum: ./punctum.thistle → "summa 3"; -probatio green
IV.  vitrea: ./salve_vitreum.thistle -struere → prints a dir; <dir>/bin/salve_vitreum exists; ./salve_vitreum.thistle -probatio → green
V.   the plant: ./probatio_rubra.thistle -probatio → exit MUST be non-zero (a gate that cannot see red is dead)
VI.  refusal: ./duo_principalia.thistle → exit 1 and stderr names "duo principalia"
VII. (-agere) bin/manus incipere "<dir>/bin/salve_vitreum -vivum"; affordantiae -machina must list >= 1 (the button);
     premere the button; -exspecta textus body must contain "salve, munde"; finire
Last line: "fumus briar: sanum" / "fumus briar: FRACTUM" (regex for pythonica).
```
The area is `mktemp -d /tmp/briar_fumus.XXXXXX`; `-servare` keeps it. Everything runs with the fake HOME so `~/.rhubarb/briar/` of the real user is untouched.

- [ ] **Step 2: Run it**

`./tools/briar_fumus.sh` → `fumus briar: sanum`. Then break it on purpose: temporarily edit the plant fixture's probatio to pass (`CREDO_VERUM(VERUM)`) → step V must turn the gate `FRACTUM`; revert. Then `./tools/briar_fumus.sh -agere` by hand (opens a window) — record the affordance count and the body text in the worklog.

- [ ] **Step 3: Register + commit**

Add the `PORTAE` row; `python3 -c "import sys; sys.path.insert(0,'pythonica'); import silva; print(silva.porta('briar-fumus'))"` → sana. Commit via `commissio_umbra` portae `['briar', 'briar-fumus', 'pythonica']` (the pythonica gate needs `oratio/build` — run `./oratio/compile_probationes.sh` once if the tree is fresh). Note in the message: the fumus runs the INSTALLED binary — a stale `~/.bin/briar` lies green; `briar_struere.sh` first.

---

### Task 5: Seal — first-run numbers, docs, spec v1.3, memory, ledger

- [ ] **Step 1: Numbers** (into the worklog and spec §8 P4): cold run of salve (parse+build+exec), cache-hit latency, `bin/briar` size, the vitrea cold build from a shebang run, the fumus duration.
- [ ] **Step 2: Docs.** `briar/CLAUDE.md`: the binary, flags, the shebang form, `briar_struere.sh` ORDER (suite → struere), `briar_fumus.sh` (+ `-agere` by hand), `~/.rhubarb/briar/<t>-<key>/`, the two stamps, the quiet switch. Worklog entry. `FONTES.md` (probatio_rubra). Spec v1.3: §4.1 stamp decision, §4.5 as built, §5 hand-parsed flags + `-auxilium`, §8 P4 DONE, §10 two binaries' sizes; plan 3 "Executed" header with deviations.
- [ ] **Step 3: Memory + ledger.** `briar-project.md`: plan 3 sealed; NEXT = merge to main (the finishing menu), then the named deferrals (§9). MEMORY.md line. Ledger nota on 01M1QR7TXQ from the main session.
- [ ] **Step 4: Commit** (portae `['briar']`), then present the finishing-a-development-branch menu: the branch is complete when plan 3 seals.

## Self-review

- Spec coverage: §4.1 key/dir → Task 2 (`briar_domus_proiecti`) + Task 3; §4.5 run/exec → Task 3; §5 flags/shebang/`-versio`/struere/fumus → Tasks 2–4; §8 P4 numbers → Task 5. Deferrals (§9) untouched.
- Types: `briar_fabricare` / `briar_fabricam_scribere` / `briar_fabrica_clavem_computare` / `briar_fabrica_vexilla` as sealed in plan 2 (`include/<t>_regiones.h`, `res->silva`).
- Placeholders: the `-arbor`/`-partes` bodies say "copy from `briar/instrumenta/arbor.c`" — that file exists and is 100 lines; the executor copies it verbatim. `_plagulam_legere` likewise.
- Order: Task 1 before Task 2 (struere sources the block); Task 2 before 3; 3 before 4.
