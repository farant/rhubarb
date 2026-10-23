# Android build tier Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking. **House execution:** inline, one task per turn, Fran approves each task before the next.

**Goal:** House C libraries build for Android and their credo suites run green on the Fire tablet through one manual command, `./tools/android/probatio.sh`.

**Architecture:** aedilis learns a variant preference list (`--varians android,posix`) so a closure prefers `X_android.c` and falls back to `X_posix.c`; `volumen` honours `$TMPDIR`; the NDK is pinned in a sourced shell file with a verifying installer; a thin runner cross-compiles each suite's closure with the NDK clang, pushes it to one root on the device, runs it there, and always removes that root.

**Tech Stack:** C89 (house flags, `latina.h`), bash 3.2 (macOS `/bin/bash` - no `mapfile`, no associative arrays), Android NDK r27d (clang 18), `adb`, credo v2.

**Spec:** `project-specs/android-gradus-spec.md` (quarta fd389f13, approved by Fran 2026-10-23).

## Global Constraints

- C89 with the exact house flags (`tools/vexilla.sh` `VEXILLA_C89`); Latin identifiers, comments and messages; latina.h macros are forbidden as identifiers (`nomen`, `per`, Roman numerals 0-3999 ...).
- `chorda` is NOT NUL-terminated; `i32`/`i64` are unsigned, `s32`/`s64` signed.
- A `.c` using POSIX calls (`setenv`, `unsetenv`) includes `postulata_posix.h` FIRST.
- NDK pin: version `27.3.13750724` (r27d), size `839006233`, SHA-1 `2970926d705988f79baa9b04c51b4f7914dd8c56`, SHA-256 `e69092f9d2bfa5d1199039980a14eb91c03cc971ab5c6968fc08a8e6b84e7bb7`, minimum API `30`, location `~/Library/Android/sdk/ndk/27.3.13750724`.
- Device root: `/data/local/tmp/rhubarb/` - the runner never writes or removes anything outside it.
- Runner exit codes: 0 all passed, 1 any failed (compile failure and `.m` refusal count, named), 2 apparatus.
- Commits through `silva.commissio` (never raw `git commit`); never install to `~/.bin` from the worktree; never bare `git stash`.
- Touched a tool's source (`tools/aedilis.c`) -> rebuild the tree-local tool before commissio.

## Review Focus

1. **Several devices / unauthorized device** - a second tablet or an emulator attached, or the "Allow USB debugging?" prompt not accepted: the runner must refuse by name (exit 2), never pick one silently. -> Task 4, Step 6 (case c).
2. **Run killed mid-suite** (Ctrl-C while a suite runs on the device): the trap still removes the root; the next run starts clean. -> Task 4, Step 7.
3. **TMPDIR with trailing slashes** (macOS sets `/var/folders/.../T/`): no `//` in volume paths. -> Task 2, Step 1 (case II).
4. **Malformed variant list** (`android,,posix`, `,`, empty): named refusal, never a silent single-variant fallback. -> Task 1, Step 1.
5. **Stale or foreign NDK directory** at the pinned path (other version unpacked there by hand): installer refuses to overwrite and names it; runner refuses to compile with it. -> Task 3, Step 5 (case d) and Task 4, Step 6 (case e).

---

## Rulings made while writing the plan (each reaches Fran in the handoff)

- **R1 - NDK pin is a sourced shell file `tools/android/ndk_fixum.sh`, not `ndk.stml`** (spec II.3). Both readers are shell scripts and the house has no shell-side STML query (`bin/stml` offers `formare | expandere | vertere`, checked 2026-10-23); parsing attributes with sed would be the regex-on-structure the house forbids. Cost if wrong: one file rename plus an `stml` query verb later.
- **R2 - on macOS, temporary volumes MOVE from `/tmp` to `$TMPDIR`** (`/var/folders/.../T/`). The spec's "nothing changes in practice" (II.2) is inaccurate. Checked: no tool or test depends on the `/tmp` location of `*.volumen` (grep over tools/, pythonica/, probationes/, lib/). The memory note "Tests must close temporary volumes ... look in /tmp" gets updated after Task 2. Cost if wrong: a leak hunt looks in the wrong directory.
- **R3 - the runner creates `<root>/build/` as well as `<root>/tmp/`**: `probatio_volumen.c` writes `build/probatio_volumen*.db` relative to the working directory (lines 18, 95-104, 625, 854). Answers half of the spec's second AUDIENDA item: of the five target suites none READS a repository file; only volumen WRITES under `build/`.
- **R4 - an `.m` source in a closure counts as a failure (exit 1), named** - the spec says "named refusal" without an exit code; it is a code-level fact about the suite, not apparatus.
- **R5 - binaries are compiled for the pin's minimum API (`aarch64-linux-android30`), not the device's API** - one binary for every device at or above 30.

---

### Task 1: aedilis variant preference list

**Files:**
- Create: `probationes/fixa/aedilis/lib/beta_android.c`
- Modify: `lib/aedilis.c` (new `_variantes_scindere` after `_verba_scindere` at :128-157; branch at :879-885)
- Modify: `include/aedilis.h:175-177` (doc comment of `aedilis_derivare`)
- Modify: `tools/aedilis.c:8, :691-692, :774` (usage text)
- Test: `probationes/probatio_aedilis.c` (extractor entry; section `derivationem`, `_sectio_septima`, after the posix case at :765-775)

**Interfaces:**
- Consumes: nothing new.
- Produces: `aedilis_derivare(piscina, configuratio, scopus, "android,posix", extractor, datum, &causa)` resolves units in list order; `fructus->varians` = FIRST element (`"android"`); malformed list -> `NIHIL` + non-empty `causa`. CLI: `bin/aedilis F --varians android,posix --partes` (Task 4 calls exactly this).

- [ ] **Step 1: Write the failing tests**

Fixture `probationes/fixa/aedilis/lib/beta_android.c` (one line, like its siblings):

```c
/* fixtura aedilis: stipes fontis (varians android) */
```

In `_extractor_fixturarum`, next to the `lib/beta_posix.c` branch:

```c
    alioquin si (strstr(via, "lib/beta_android.c") != NIHIL)
    {
        _fix_addere(*directivae_out, "beta.h", piscina);
    }
```

In `_sectio_septima`, after the existing `"posix"` block (ends at :775, before the closing brace of the inner block):

```c
        /* index variantium (android-gradus II.1): android prior ->
         * beta_android, nec posix nec macos */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", "android,posix", _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_CHORDA_AEQUALIS_LITERIS(fructus->varians, "android");
        CREDO_NON_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_android.c"));
        CREDO_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_posix.c"));
        CREDO_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_macos.m"));

        /* prima absens (linux) -> sequens (posix) */
        fructus = aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", "linux,posix", _extractor_fixturarum,
            NIHIL, &causa);
        CREDO_NON_NIHIL(fructus);
        CREDO_NON_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_posix.c"));
        CREDO_NIHIL(_obiectum_invenire(fructus,
            "lib/beta_android.c"));

        /* index malus = recusatio nominata, numquam varians una */
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_NIHIL(aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", "android,,posix", _extractor_fixturarum,
            NIHIL, &causa));
        CREDO_CHORDA_NON_VACUA(causa);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_NIHIL(aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", ",", _extractor_fixturarum, NIHIL,
            &causa));
        CREDO_CHORDA_NON_VACUA(causa);
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        CREDO_NIHIL(aedilis_derivare(piscina, configuratio,
            "fons/scopus.c", "", _extractor_fixturarum, NIHIL,
            &causa));
        CREDO_CHORDA_NON_VACUA(causa);
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `CREDO_SECTIO=derivationem ./compile_tests.sh aedilis`
Expected: FAIL in `derivationem` - `fructus->varians` is `"android,posix"` (the whole string is taken as one variant), `lib/beta_android.c` not found, and the malformed lists return non-NIHIL.

- [ ] **Step 3: Write the implementation**

`lib/aedilis.c`, after `_verba_scindere` (:157):

```c
/* "android,posix" -> verba ordine praelationis. Elementum vacuum
 * ("a,,b", ",", "") = FALSUM: index malus nominatur, numquam tacite
 * ad variantem unam cadit (android-gradus II.1) */
interior b32
_variantes_scindere (
    chorda  s,
       Xar* fructus)
{
    i32 i;
    i32 initium;

    si (s.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= s.mensura; i++)
    {
        si (i == s.mensura || s.datum[i] == (i8)',')
        {
            chorda verbum;

            si (i == initium)
            {
                redde FALSUM;
            }
            verbum.datum    = s.datum + initium;
            verbum.mensura  = i - initium;
            _chordam_addere(fructus, verbum);
            initium = i + I;
        }
    }
    redde VERUM;
}
```

Replace the branch at :879-885:

```c
    /* variantes effectivae: -varians dato = index eius ordine
     * ("android,posix"; nomen unum = illud solum); aliter praelatio
     * configurationis ordine */
    machina.variantes_effectivae = _xar_chordarum(piscina);
    si (varians != NIHIL)
    {
        si (!_variantes_scindere(chorda_ex_literis(varians, piscina),
                machina.variantes_effectivae))
        {
            si (causa_out != NIHIL)
            {
                *causa_out = chorda_ex_literis("aedilis: --varians"
                    " index malus (elementum vacuum) - forma:"
                    " android,posix", piscina);
            }
            redde NIHIL;
        }
    }
```

`include/aedilis.h:175-177` doc comment becomes:

```c
/* Clausuram scopi derivare (punctum fixum trans plagulas).
 * varians: nomen unum ("posix") aut index ordine praelationis
 * ("android,posix": X_android, aliter X_posix); NIHIL = praelatio
 * configurationis. Index malus (elementum vacuum) = recusatio.
 * NIHIL + causa in recusatione - nullus fructus partialis. */
```

`tools/aedilis.c`: line 8 usage comment and line 774 usage string `--varians <verbum>` -> `--varians <verbum[,verbum...]>`; line 691-692 help text -> `"Varians aut index ordine praelationis (android,posix); ordinarie praelatio configurationis"`.

- [ ] **Step 4: Run tests to verify they pass**

Run: `./compile_tests.sh aedilis`
Expected: both aedilis suites pass, `Fracti: 0`.

- [ ] **Step 5: Plant (the fallback is load-bearing)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.planta('lib/aedilis.c',
    "            initium = i + I;\n        }\n    }\n    redde VERUM;",
    "            initium = i + I;\n            frange;\n        }\n    }\n    redde VERUM;",
    'aedilis'))
EOF
```
Expected: red (only the first element is kept -> `linux,posix` finds no beta object; `android,posix` still green), then green after the automatic revert.

- [ ] **Step 6: Rebuild the tree-local tool, check the CLI**

Run: `bin/fabrica sanare bin/aedilis && bin/aedilis probationes/probatio_xar.c --varians android,posix --partes | head -3 && bin/aedilis probationes/probatio_xar.c --varians 'android,,posix' --partes; echo rc=$?`
Expected: closure lines (`O\t...`) for the list; the malformed list prints the named refusal and a non-zero rc.

- [ ] **Step 7: Commit**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio('''aedilis: --varians index ordine praelationis (android,posix) - X_android, aliter X_posix; index malus recusatio nominata (android-gradus T1)

Co-Authored-By: Claude Opus 5.5 (1M context) <noreply@anthropic.com>''',
    ['lib/aedilis.c', 'include/aedilis.h', 'tools/aedilis.c',
     'probationes/probatio_aedilis.c',
     'probationes/fixa/aedilis/lib/beta_android.c'])
EOF
```
A committed `.c` under `probationes/fixa/` is swept by examen-corpus at merge tier: run `./silva/examen_vectis.sh -corpus` before the commit (memory `fixa-c-examen-corpus`).

---

### Task 2: volumen honours TMPDIR

**Files:**
- Modify: `lib/volumen.c` (new `_basis_temporaria` before `volumen_temporarium`; the loop at :379-401; the message at :403-405; add `#include <stdlib.h>`)
- Modify: `include/volumen.h:106-111` (doc: `/tmp` -> `$TMPDIR`, else `/tmp`)
- Test: `probationes/probatio_volumen.c` (includes; new section after `volumen_temporarium` in the table at :1110)

**Interfaces:**
- Consumes: nothing new.
- Produces: `volumen_temporarium(piscina, praefixum)` creates `<$TMPDIR or /tmp>/<praefixum>-<n>.volumen` (trailing `/` of TMPDIR trimmed). Task 4's runner relies on this to keep volumes under the device root.

- [ ] **Step 1: Write the failing test**

`probationes/probatio_volumen.c`: insert `#include "postulata_posix.h"` as the FIRST line before `#include "latina.h"`, and `#include <stdlib.h>` beside `<string.h>`. New section before the table:

```c
/* ========================================================
 * PROBARE: volumen temporarium sub $TMPDIR (android-gradus II.2)
 *
 * Android /tmp caret; cursor tabulae TMPDIR sub radice sua
 * ponit. Basis: $TMPDIR non vacuum (virgulae finales abscissae),
 * aliter /tmp - regula credo (lib/credo.c).
 * ======================================================== */

interior b32
_incipit (
    constans character* via,
    constans character* praefixum)
{
    redde strncmp(via, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* casus unus: TMPDIR = valor (NIHIL = absens) -> volumen sub
 * 'exspectatum'; exspectatum NIHIL = basis /tmp, quae in Android
 * deest - ibi volumen nasci NON debet (nec alibi) */
interior vacuum
_temporarium_sub (
           CredoContextus* c,
    constans character* valor,
    constans character* exspectatum)
{
     Volumen* vol;
    character via[DXII];

    si (valor != NIHIL)
    {
        (vacuum)setenv("TMPDIR", valor, I);
    }
    alioquin
    {
        (vacuum)unsetenv("TMPDIR");
    }
    vol = volumen_temporarium(c->piscina, "tmpdir_probatio");
    si (exspectatum == NIHIL && !filum_directorium_existit("/tmp"))
    {
        CREDO_NIHIL (vol);
        redde;
    }
    CREDO_NON_NIHIL (vol);
    si (vol == NIHIL)
    {
        redde;
    }
    strcpy(via, volumen_via(vol));
    CREDO_VERUM (_incipit(via, exspectatum != NIHIL ? exspectatum
        : "/tmp/tmpdir_probatio-"));
    CREDO_VERUM (filum_existit(via));
    volumen_claudere(vol);
    CREDO_FALSUM (filum_existit(via));
}

interior vacuum
_sectio_temporarii_tmpdir (
    CredoContextus* c)
{
    constans character* vetus;
             character  servatum[DXII];
                   b32  habuit;

    vetus   = getenv("TMPDIR");
    habuit  = vetus != NIHIL && strlen(vetus) < DXII;
    si (habuit)
    {
        strcpy(servatum, vetus);
    }
    CREDO_VERUM (filum_directorium_creare_cum_parentibus(
        "build/probatio_volumen_tmpdir"));

    /* I: directorium datum */
    _temporarium_sub(c, "build/probatio_volumen_tmpdir",
        "build/probatio_volumen_tmpdir/tmpdir_probatio-");
    /* II: virgulae finales abscinduntur (macOS TMPDIR '/' terminat) */
    _temporarium_sub(c, "build/probatio_volumen_tmpdir//",
        "build/probatio_volumen_tmpdir/tmpdir_probatio-");
    /* III-IV: vacuum aut absens -> /tmp */
    _temporarium_sub(c, "", NIHIL);
    _temporarium_sub(c, NIHIL, NIHIL);

    si (habuit)
    {
        (vacuum)setenv("TMPDIR", servatum, I);
    }
    alioquin
    {
        (vacuum)unsetenv("TMPDIR");
    }
}
```

Table entry after `{ "volumen_temporarium", _sectio_decima, ... }`:

```c
    { "volumen_temporarium sub TMPDIR", _sectio_temporarii_tmpdir,
        NIHIL, NIHIL, NIHIL, NIHIL },
```

- [ ] **Step 2: Run test to verify it fails**

Run: `CREDO_SECTIO="volumen_temporarium sub TMPDIR" ./compile_tests.sh volumen`
Expected: FAIL in cases I and II (`_incipit` false - the path starts with `/tmp/`).

- [ ] **Step 3: Write the implementation**

`lib/volumen.c`: add `#include <stdlib.h>` beside `<string.h>`. Before `volumen_temporarium`:

```c
/* Basis temporariorum: $TMPDIR non vacuum, virgulis finalibus
 * abscissis (regula credo, lib/credo.c), aliter /tmp. Android /tmp
 * caret - ibi TMPDIR=/data/local/tmp (android-gradus II.2).
 * Copia in 'basis' (DXII), ut chorda constans non castetur. */
interior vacuum
_basis_temporaria (
    character* basis)
{
    constans character* tmp;
         memoriae_index  n;

    tmp = getenv("TMPDIR");
    si (tmp == NIHIL || tmp[0] == '\0')
    {
        tmp = "/tmp";
    }
    n = strlen(tmp);
    dum (n > I && tmp[n - I] == '/')
    {
        n--;
    }
    si (n > DXI)
    {
        n = DXI;
    }
    memcpy(basis, tmp, n);
    basis[n] = '\0';
}
```

In `volumen_temporarium`: declare `character basis[DXII];`, call `_basis_temporaria(basis);` before the loop, and replace `chorda_aedificator_appendere_literis(aed, "/tmp/");` with:

```c
        chorda_aedificator_appendere_literis(aed, basis);
        chorda_aedificator_appendere_literis(aed, "/");
```

The message at :403-405 becomes:

```c
        fprintf(stderr, "volumen_temporarium: C nomina occupata"
            " (praefixum '%s') - reliquiae in %s?\n", praefixum,
            basis);
```

The comment above the loop (`qui in /tmp inspicit`) -> `qui in basi temporaria inspicit`. `include/volumen.h:106-111`: every `/tmp` in that comment -> `$TMPDIR (aliter /tmp)`. Also run `grep -n '/tmp' lib/volumen*.stml include/volumen.h` and update any prose that names `/tmp` as THE location.

- [ ] **Step 4: Run tests to verify they pass**

Run: `./compile_tests.sh volumen`
Expected: every volumen suite passes (`probatio_volumen`, `probatio_volumen_rivi`, ...), `Fracti: 0`. Then `ls "$TMPDIR" | grep -c volumen` -> 0 (nothing left behind).

- [ ] **Step 5: Plant**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.planta('lib/volumen.c',
    '        chorda_aedificator_appendere_literis(aed, basis);\n        chorda_aedificator_appendere_literis(aed, "/");',
    '        chorda_aedificator_appendere_literis(aed, "/tmp/");',
    'volumen'))
EOF
```
Expected: red (TMPDIR cases), then green after revert. If `basis` becomes unused under the plant and `-Werror` breaks the build, planta refuses ("planta non compilat") - then plant `tmp = "/tmp";` over `tmp = getenv("TMPDIR");` instead.

- [ ] **Step 6: Commit**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio('''volumen: volumen_temporarium sub $TMPDIR (aliter /tmp), virgulis finalibus abscissis - Android /tmp caret (android-gradus T2)

macOS: volumina temporaria nunc in $TMPDIR (/var/folders/.../T), non /tmp (plan R2; nullum instrumentum locum /tmp exspectat).

Co-Authored-By: Claude Opus 5.5 (1M context) <noreply@anthropic.com>''',
    ['lib/volumen.c', 'include/volumen.h', 'probationes/probatio_volumen.c'])
EOF
```
`lib/volumen.c` is linked by house binaries: if commissio names stale installed artifacts, follow memory `heal-installed-before-commit` (tree-local heal; never `~/.bin` from the worktree).

---

### Task 3: NDK pin and verifying installer

**Files:**
- Create: `tools/android/ndk_fixum.sh` (sourced data, ruling R1)
- Create: `tools/android/ndk_instituere.sh`

**Interfaces:**
- Consumes: nothing.
- Produces: after `. tools/android/ndk_fixum.sh`: `NDK_VERSIO NDK_NOMEN NDK_URL NDK_MAGNITUDO NDK_SHA1 NDK_SHA256 NDK_API_MINIMA NDK_SEDES`. Task 4 sources it. Installed check = line `Pkg.Revision = $NDK_VERSIO` in `$NDK_SEDES/source.properties`.

- [ ] **Step 1: Write the checks first (they fail: the scripts do not exist)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
Z=/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/ndk_dl/android-ndk-r27d-darwin.zip
./tools/android/ndk_instituere.sh -verificare "$Z"; echo "rc=$?"
```
Expected now: `No such file or directory`, rc=127. (If the 2026-10-22 download is gone from the scratchpad, fetch it once: `curl -fL -o "$Z" https://dl.google.com/android/repository/android-ndk-r27d-darwin.zip`.)

- [ ] **Step 2: Write `tools/android/ndk_fixum.sh`**

```bash
# tools/android/ndk_fixum.sh - NDK FIXUM (android-gradus II.3, plan R1):
# fons UNUS, fontatur ab ndk_instituere.sh et probatio.sh; nihil agit.
# Versio nova = commissio consulta: repository2-3.xml Google legere
# (https://dl.google.com/android/repository/repository2-3.xml,
# remotePackage 'ndk;<versio>', archivum macosx) - magnitudo et sha1
# inde; sha256 ipsi computamus super plagulam verificatam.
NDK_VERSIO="27.3.13750724"
NDK_NOMEN="r27d"
NDK_URL="https://dl.google.com/android/repository/android-ndk-r27d-darwin.zip"
NDK_MAGNITUDO="839006233"
NDK_SHA1="2970926d705988f79baa9b04c51b4f7914dd8c56"
NDK_SHA256="e69092f9d2bfa5d1199039980a14eb91c03cc971ab5c6968fc08a8e6b84e7bb7"
NDK_API_MINIMA="30"
NDK_SEDES="$HOME/Library/Android/sdk/ndk/$NDK_VERSIO"
```

- [ ] **Step 3: Write `tools/android/ndk_instituere.sh`** (`chmod +x`)

```bash
#!/bin/bash
# tools/android/ndk_instituere.sh - NDK fixum depromere, verificare,
# instituere (android-gradus II.3). Manu, numquam porta.
#
# Usus: ./tools/android/ndk_instituere.sh                  # depromit + instituit
#       ./tools/android/ndk_instituere.sh -verificare ZIP  # solum confert
#
# Recusat (exitus 2, causa nominata): magnitudo, sha1 (manifestum
# Google), sha256 (fixum nostrum) discrepantes; directorium alienum in
# sede fixa. Institutum rectum nuntiatur et relinquitur (exitus 0).
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2
. tools/android/ndk_fixum.sh

verificare () {
    local m s1 s256
    m=$(stat -f %z "$1" 2>/dev/null) || { echo "ndk: $1 legi nequit" >&2; return 2; }
    [ "$m" = "$NDK_MAGNITUDO" ] || { echo "ndk: magnitudo $m != $NDK_MAGNITUDO (fixum)" >&2; return 2; }
    s1=$(shasum -a 1 "$1" | cut -d' ' -f1)
    [ "$s1" = "$NDK_SHA1" ] || { echo "ndk: sha1 $s1 != $NDK_SHA1 (manifestum Google)" >&2; return 2; }
    s256=$(shasum -a 256 "$1" | cut -d' ' -f1)
    [ "$s256" = "$NDK_SHA256" ] || { echo "ndk: sha256 $s256 != $NDK_SHA256 (fixum nostrum)" >&2; return 2; }
    return 0
}

institutum () {
    grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null
}

if [ "${1:-}" = "-verificare" ]; then
    [ $# -eq 2 ] || { echo "usus: $0 -verificare ZIP" >&2; exit 2; }
    verificare "$2" || exit 2
    echo "ndk: $2 congruit (NDK $NDK_NOMEN $NDK_VERSIO)"
    exit 0
fi
[ $# -eq 0 ] || { echo "usus: $0 [-verificare ZIP]" >&2; exit 2; }

if institutum; then
    echo "ndk: $NDK_NOMEN ($NDK_VERSIO) iam institutum: $NDK_SEDES"
    exit 0
fi
if [ -e "$NDK_SEDES" ]; then
    echo "ndk: $NDK_SEDES exstat sed Pkg.Revision non est $NDK_VERSIO - manu inspice et remove" >&2
    exit 2
fi
T=$(mktemp -d "${TMPDIR:-/tmp}/ndk_instituere.XXXXXX") || exit 2
trap 'rm -rf "$T"' EXIT
echo "ndk: depromo $NDK_URL ($NDK_MAGNITUDO octeti)"
curl -fL --progress-bar -o "$T/ndk.zip" "$NDK_URL" || { echo "ndk: depromptio fracta" >&2; exit 2; }
verificare "$T/ndk.zip" || exit 2
unzip -q "$T/ndk.zip" -d "$T/x" || { echo "ndk: unzip fractum" >&2; exit 2; }
mkdir -p "$(dirname "$NDK_SEDES")" || exit 2
mv "$T/x/android-ndk-$NDK_NOMEN" "$NDK_SEDES" || { echo "ndk: translatio fracta" >&2; exit 2; }
institutum || { echo "ndk: post institutionem Pkg.Revision non congruit" >&2; exit 2; }
echo "ndk: $NDK_NOMEN ($NDK_VERSIO) institutum: $NDK_SEDES"
```

- [ ] **Step 4: Run the checks (green and the named refusals)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
S=/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad
Z=$S/ndk_dl/android-ndk-r27d-darwin.zip
./tools/android/ndk_instituere.sh -verificare "$Z"; echo "a rc=$?"
head -c 1000 "$Z" > $S/ndk_brevis.zip
./tools/android/ndk_instituere.sh -verificare $S/ndk_brevis.zip; echo "b rc=$?"
cp "$Z" $S/ndk_mutatus.zip && printf 'X' | dd of=$S/ndk_mutatus.zip bs=1 seek=4096 conv=notrunc 2>/dev/null
./tools/android/ndk_instituere.sh -verificare $S/ndk_mutatus.zip; echo "c rc=$?"
rm -f $S/ndk_brevis.zip $S/ndk_mutatus.zip
./tools/android/ndk_instituere.sh; echo "inst rc=$?"
```
Expected: a `congruit` rc=0; b `magnitudo 1000 != 839006233` rc=2; c `sha1 ... != 2970926d...` rc=2; inst `iam institutum` rc=0.

- [ ] **Step 5: Foreign directory at the pinned path (Review Focus 5)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
NDKD=~/Library/Android/sdk/ndk/27.3.13750724
cp -p "$NDKD/source.properties" /private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/sp.orig
sed -i '' 's/^Pkg.Revision = .*/Pkg.Revision = 26.0.0/' "$NDKD/source.properties"
./tools/android/ndk_instituere.sh; echo "d rc=$?"
cp /private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/sp.orig "$NDKD/source.properties"
./tools/android/ndk_instituere.sh; echo "restitutum rc=$?"
```
Expected: d `exstat sed Pkg.Revision non est 27.3.13750724 - manu inspice et remove` rc=2 (nothing downloaded or removed); restitutum `iam institutum` rc=0.

- [ ] **Step 6: Commit**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio('''tools/android: NDK r27d fixum (ndk_fixum.sh, fons unus) + ndk_instituere.sh (depromit, magnitudinem/sha1/sha256 confert, instituit; -verificare ZIP) - android-gradus T3

Probatum: plagula vera congruit; brevis -> magnitudo nominata; octetus mutatus -> sha1 nominata; sedes aliena -> recusatio, nihil tactum.

Co-Authored-By: Claude Opus 5.5 (1M context) <noreply@anthropic.com>''',
    ['tools/android/ndk_fixum.sh', 'tools/android/ndk_instituere.sh'])
EOF
```
If commissio asks for a unit declaration of the new scripts (`unitates.stml` iudex / manual-tool registry), follow its named instruction and ledger a ruling.

---

### Task 4: the device runner and its cleanup lifecycle

**Files:**
- Create: `tools/android/probatio.sh`
- Modify: `project-specs/android-gradus-spec.md` (status line -> IMPLETUM with commits; AUDIENDA answers)

**Interfaces:**
- Consumes: Task 1 `bin/aedilis P --varians android,posix --partes` (`O\t<via>` objects, `V\t<via>` vendored); Task 2 volumes under `$TMPDIR`; Task 3 `tools/android/ndk_fixum.sh` variables.
- Produces: `./tools/android/probatio.sh [-servare | -purgare] <suites...>`; exit 0/1/2; log `build/test_logs/android.log`; host artifacts `build/android/<abi>/<suite>/`.

- [ ] **Step 1: Write the check first (fails: the runner does not exist)**

Run: `./tools/android/probatio.sh piscina; echo rc=$?`
Expected: `No such file or directory`, rc=127.

- [ ] **Step 2: Write `tools/android/probatio.sh`** (`chmod +x`)

```bash
#!/bin/bash

# Effusio cursus ultimi semper in build/test_logs/android.log (ut
# tools/linux/probatio.sh); ante 'cd' infra, ne "$0" relativum pereat.
if [ -z "${ANDROID_PROBATIO_EFFUSIO:-}" ]; then
    export ANDROID_PROBATIO_EFFUSIO=1
    mkdir -p "$(dirname "$0")/../../build/test_logs"
    "$0" "$@" 2>&1 | tee "$(dirname "$0")/../../build/test_logs/android.log"
    exit "${PIPESTATUS[0]}"
fi

# tools/android/probatio.sh - probationes credo in tabula Android
# (android-gradus II.4-II.5)
#
# Usus: ./tools/android/probatio.sh [-servare | -purgare] <nomina...>
#       e.g. ./tools/android/probatio.sh piscina chorda xar json volumen
#
# Clausura per 'bin/aedilis P --varians android,posix --partes'; NDK
# clang (tools/android/ndk_fixum.sh) cum VEXILLA_C89 (tools/vexilla.sh);
# binarium nudatum in tabulam, ibi curritur.
#
# TABULA: radix UNA /data/local/tmp/rhubarb - extra eam nihil scribitur
# aut deletur. Initio deletur et renascitur (tmp/, build/); suitae cum
# TMPDIR=<radix>/tmp et cwd=<radix> currunt; FINE (etiam Ctrl-C) mensura
# nuntiatur et radix deletur. -servare: radix manet (via nuntiatur).
# -purgare: radicem delet, nihil currit.
#
# Exitus: 0 omnes praeteritae | 1 fractae (compilatio et fons .m
# numerantur) | 2 apparatus (NDK, adb, tabula, aedilis)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/../.." || exit 2

RADIX_TABULAE=/data/local/tmp/rhubarb
ADB="${ADB:-$HOME/Library/Android/sdk/platform-tools/adb}"
SERVARE=0
PURGARE=0
while [ $# -gt 0 ]; do
    case "$1" in
        -servare) SERVARE=1; shift ;;
        -purgare) PURGARE=1; shift ;;
        *) break ;;
    esac
done

apparatus () {
    echo "android/probatio.sh: $*" >&2
    exit 2
}

# --- tabula: exacte una, auctorizata -------------------------------
[ -x "$ADB" ] || apparatus "adb deest ($ADB)"
ADB_TABULAE=$("$ADB" devices | awk 'NR > 1 && NF >= 2 {print $1 " " $2}')
if [ -n "${ANDROID_SERIAL:-}" ]; then
    STATUS=$(printf '%s\n' "$ADB_TABULAE" | awk -v s="$ANDROID_SERIAL" '$1 == s {print $2}')
    [ -n "$STATUS" ] || apparatus "tabula $ANDROID_SERIAL non connexa"
else
    NUMERUS=$(printf '%s\n' "$ADB_TABULAE" | grep -c .)
    [ "$NUMERUS" -ge 1 ] || apparatus "nulla tabula connexa (adb devices vacuum)"
    [ "$NUMERUS" -eq 1 ] || apparatus "plures tabulae ($NUMERUS) - ANDROID_SERIAL elige"
    ANDROID_SERIAL=$(printf '%s\n' "$ADB_TABULAE" | awk '{print $1}')
    STATUS=$(printf '%s\n' "$ADB_TABULAE" | awk '{print $2}')
fi
[ "$STATUS" = "device" ] || apparatus "tabula $ANDROID_SERIAL status '$STATUS' - 'Allow USB debugging?' in tabula accipe"
export ANDROID_SERIAL

A () { "$ADB" "$@"; }

purgare () {
    local kib
    kib=$(A shell "du -sk $RADIX_TABULAE 2>/dev/null" | tr -d '\r' | awk '{print $1}')
    A shell "rm -rf $RADIX_TABULAE" > /dev/null 2>&1
    echo "vestigium in tabula: ${kib:-0} KiB, purgatum"
}

if [ "$PURGARE" = 1 ]; then
    purgare
    exit 0
fi
[ $# -ge 1 ] || apparatus "usus: $0 [-servare | -purgare] <nomina probationum...>"

# --- NDK fixum -------------------------------------------------------
. tools/android/ndk_fixum.sh
grep -qx "Pkg.Revision = $NDK_VERSIO" "$NDK_SEDES/source.properties" 2>/dev/null \
    || apparatus "NDK $NDK_VERSIO deest aut alienum ($NDK_SEDES) - ./tools/android/ndk_instituere.sh"
NDK_BIN="$NDK_SEDES/toolchains/llvm/prebuilt/darwin-x86_64/bin"

ABI=$(A shell getprop ro.product.cpu.abi | tr -d '\r')
API=$(A shell getprop ro.build.version.sdk | tr -d '\r')
case "$ABI" in
    arm64-v8a) TRIPLEX="aarch64-linux-android$NDK_API_MINIMA" ;;
    armeabi-v7a) TRIPLEX="armv7a-linux-androideabi$NDK_API_MINIMA" ;;
    *) apparatus "ABI ignota: '$ABI'" ;;
esac
[ "$API" -ge "$NDK_API_MINIMA" ] 2>/dev/null \
    || apparatus "API tabulae '$API' < $NDK_API_MINIMA (fixum)"

. tools/vexilla.sh
vexilla_radices ""
CC=("$NDK_BIN/clang" "--target=$TRIPLEX")
VEXILLA=("${VEXILLA_C89[@]}" "${RADICES_C89[@]}")
VFLAGS=(-O2 -DBR_ENABLE_INTRINSICS=1 -DSQLITE_ENABLE_FTS5 -DSQLITE_THREADSAFE=0
    -DSQLITE_DQS=0 -DSQLITE_DEFAULT_MEMSTATUS=0 -DSQLITE_OMIT_LOAD_EXTENSION
    -DSQLITE_OMIT_DEPRECATED -DSQLITE_DEFAULT_WAL_SYNCHRONOUS=1)

# --- radix tabulae: initio munda, fine semper deleta -------------------
finis () {
    if [ "$SERVARE" = 1 ]; then
        echo "radix servata: $RADIX_TABULAE (./tools/android/probatio.sh -purgare)"
    else
        purgare
    fi
}
A shell "rm -rf $RADIX_TABULAE && mkdir -p $RADIX_TABULAE/tmp $RADIX_TABULAE/build" \
    || apparatus "radix $RADIX_TABULAE creari nequit"
trap finis EXIT
trap 'exit 130' INT TERM

echo "tabula $ANDROID_SERIAL: $ABI, API $API; NDK $NDK_NOMEN, $TRIPLEX"
FRACTAE=""
for titulus in "$@"; do
    PROBATIO="probationes/probatio_${titulus}.c"
    [ -f "$PROBATIO" ] || apparatus "$PROBATIO deest"
    PARTES=$(bin/aedilis "$PROBATIO" --varians android,posix --partes 2>&1) \
        || apparatus "aedilis fracta ($titulus): $PARTES"
    OBIECTA=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "O" {print $2}')
    VENDITI=$(printf '%s\n' "$PARTES" | awk -F'\t' '$1 == "V" {print $2}')
    echo "== $titulus =="
    if printf '%s\n' "$OBIECTA" | grep -q '\.m$'; then
        echo "FRACTA: fons .m in clausura (codex Mac solum): $(printf '%s\n' "$OBIECTA" | grep '\.m$' | tr '\n' ' ')"
        FRACTAE="$FRACTAE $titulus"
        continue
    fi
    D="build/android/$ABI/$titulus"
    rm -rf "$D" && mkdir -p "$D"
    VOBJ=()
    for v in $VENDITI; do
        o="$D/v_$(basename "$v" .c).o"
        "${CC[@]}" "${VFLAGS[@]}" -c "$v" -o "$o" > "$D/venditum.log" 2>&1 \
            || { echo "FRACTA: venditum $v"; tail -5 "$D/venditum.log"; FRACTAE="$FRACTAE $titulus"; continue 2; }
        VOBJ+=("$o")
    done
    # shellcheck disable=SC2086 - OBIECTA: viae sine spatiis (aedilis)
    "${CC[@]}" "${VEXILLA[@]}" "$PROBATIO" $OBIECTA ${VOBJ[@]+"${VOBJ[@]}"} -lm \
        -o "$D/probatio_$titulus" > "$D/compilatio.log" 2>&1 \
        || { echo "FRACTA: COMPILATIO"; tail -15 "$D/compilatio.log"; FRACTAE="$FRACTAE $titulus"; continue; }
    "$NDK_BIN/llvm-strip" -o "$D/probatio_$titulus.nudum" "$D/probatio_$titulus" \
        || apparatus "llvm-strip fractum ($titulus)"
    A push "$D/probatio_$titulus.nudum" "$RADIX_TABULAE/probatio_$titulus" > /dev/null \
        || apparatus "adb push fractum ($titulus)"
    A shell "cd $RADIX_TABULAE && chmod 755 probatio_$titulus && TMPDIR=$RADIX_TABULAE/tmp ./probatio_$titulus; echo EXITUS=\$?" \
        > "$D/effusio.txt" 2>&1
    tr -d '\r' < "$D/effusio.txt" | grep -v '^EXITUS=' | tail -6
    EXITUS=$(tr -d '\r' < "$D/effusio.txt" | awk -F= '/^EXITUS=/ {print $2}' | tail -1)
    [ "$EXITUS" = "0" ] || FRACTAE="$FRACTAE $titulus"
done

if [ -z "$FRACTAE" ]; then
    echo "PROBATIONES ANDROID ($ABI, API $API): PRAETERITAE ($#)"
    exit 0
fi
echo "PROBATIONES ANDROID ($ABI, API $API): FRACTAE:$FRACTAE" >&2
exit 1
```

- [ ] **Step 3: First green run on the Fire (the spec's AUDIENDA items surface here)**

Run (tablet plugged in, unlocked not required): `./tools/android/probatio.sh piscina chorda xar json volumen; echo rc=$?`
Expected: each suite's tail shows `Fracti: 0`; last lines `PROBATIONES ANDROID (arm64-v8a, API 30): PRAETERITAE (5)` and `vestigium in tabula: <N> KiB, purgatum`; rc=0. Any bionic incompatibility shows as `FRACTA: COMPILATIO` with the compiler's lines - fix in the library (a `_android` variant only if a real platform difference, else portable C), each fix with its own failing test where one can be written, and ledger it as a ruling.

- [ ] **Step 4: Cleanup proof (normal run)**

Run: `~/Library/Android/sdk/platform-tools/adb shell ls /data/local/tmp`
Expected: no `rhubarb` entry.

- [ ] **Step 5: Red - a planted assertion fails the run (exit 1, suite named)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import subprocess, sys; sys.path.insert(0, 'pythonica'); import silva
def porta_android(via):
    r = subprocess.run(['./tools/android/probatio.sh', 'xar'],
                       capture_output=True, text=True)
    acta = r.stdout + r.stderr
    ultima = acta.strip().splitlines()[-1] if acta.strip() else ''
    return silva.Porta('android-xar', True, r.returncode == 0, ultima,
                       r.returncode, acta, [])
with open('probationes/probatio_xar.c') as f:
    textus = f.read()
# ancora: assertio prima CREDO_AEQUALIS_I32 in plagula (exacte ut scripta)
i = textus.index('CREDO_AEQUALIS_I32 (')
j = textus.index(';', i) + 1
vetus = textus[i:j]
print('ancora:', vetus)
print(silva.planta('probationes/probatio_xar.c', vetus,
                   'CREDO_VERUM (FALSUM);', porta_android))
EOF
```
Expected: red Porta (rc 1, last line `PROBATIONES ANDROID (...): FRACTAE: xar`), then green after the automatic revert. If the first `CREDO_AEQUALIS_I32 (` in `probatio_xar.c` is written without the space, use the spelling the file actually has (read it first).

- [ ] **Step 6: Apparatus cases (exit 2, cause named)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
# a) unplugged: ask Fran to unplug the tablet, then:
./tools/android/probatio.sh piscina; echo "a rc=$?"
# b) unknown device serial:
ANDROID_SERIAL=nusquam ./tools/android/probatio.sh piscina; echo "b rc=$?"
# c) several devices / unauthorized (Review Focus 1): adb devices output is
#    read through the same awk; simulate with a fake adb on PATH:
F=/private/tmp/claude-501/-Users-francisarant-Documents-projects-rhubarb/444a1e4f-bf1e-419f-9392-4000c2ec3cb4/scratchpad/adb_falsum
mkdir -p $F
printf '#!/bin/sh\necho "List of devices attached"\necho "AAA\tdevice"\necho "BBB\tdevice"\n' > $F/plures; chmod +x $F/plures
printf '#!/bin/sh\necho "List of devices attached"\necho "AAA\tunauthorized"\n' > $F/ignota; chmod +x $F/ignota
ADB=$F/plures ./tools/android/probatio.sh piscina; echo "c1 rc=$?"
ADB=$F/ignota ./tools/android/probatio.sh piscina; echo "c2 rc=$?"
# d) unknown suite:
./tools/android/probatio.sh nusquam; echo "d rc=$?"
# e) foreign NDK at the pinned path (Review Focus 5):
NDKD=~/Library/Android/sdk/ndk/27.3.13750724; cp -p "$NDKD/source.properties" $F/sp.orig
sed -i '' 's/^Pkg.Revision = .*/Pkg.Revision = 26.0.0/' "$NDKD/source.properties"
./tools/android/probatio.sh piscina; echo "e rc=$?"
cp $F/sp.orig "$NDKD/source.properties"
```
Expected: a `nulla tabula connexa` rc=2; b `tabula nusquam non connexa` rc=2; c1 `plures tabulae (2) - ANDROID_SERIAL elige` rc=2; c2 `status 'unauthorized' - 'Allow USB debugging?' in tabula accipe` rc=2; d `probationes/probatio_nusquam.c deest` rc=2; e `NDK 27.3.13750724 deest aut alienum ... ndk_instituere.sh` rc=2. In every case no `rhubarb/` remains on the device (`adb shell ls /data/local/tmp`).

- [ ] **Step 7: Ctrl-C mid-run, -servare, -purgare (Review Focus 2)**

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta
A=~/Library/Android/sdk/platform-tools/adb
./tools/android/probatio.sh volumen & P=$!
until $A shell ls /data/local/tmp/rhubarb/probatio_volumen > /dev/null 2>&1; do sleep 1; done
# the trap lives in the INNER run (ANDROID_PROBATIO_EFFUSIO=1, under the tee
# wrapper); background jobs of a non-interactive shell ignore SIGINT, so TERM
INTERIOR=$(pgrep -f 'tools/android/probatio.sh volumen' | grep -v "^$P\$" | head -1)
kill -TERM "$INTERIOR"; wait $P; echo "term rc=$?"
$A shell ls /data/local/tmp; echo ---
./tools/android/probatio.sh -servare piscina; $A shell ls /data/local/tmp/rhubarb
./tools/android/probatio.sh -purgare; $A shell ls /data/local/tmp
```
Expected: term rc=130 (the inner trap's `exit 130`, passed out through `PIPESTATUS`), the log shows `vestigium in tabula: ... purgatum`, listing shows no `rhubarb`; `-servare` prints `radix servata: /data/local/tmp/rhubarb` and the listing shows `build probatio_piscina tmp`; `-purgare` prints the footprint and the final listing shows no `rhubarb`. A real Ctrl-C in Fran's terminal sends INT to the whole foreground group, which the same trap handles.

- [ ] **Step 8: Spec status + AUDIENDA answers, then commit**

In `project-specs/android-gradus-spec.md`: status line `APPROBANDUM` -> `IMPLETUM 2026-10-2x` with the four commit hashes; under AUDIENDA, answer each item from Step 3's evidence (compiled unchanged or what was fixed; volumen's `build/` writes per R3; `llvm-strip`/`du -sk` worked).

```bash
cd /Users/francisarant/Documents/projects/rhubarb-quarta && python3 - <<'EOF'
import sys; sys.path.insert(0, 'pythonica'); import silva
silva.commissio('''tools/android/probatio.sh: suitae credo in tabula Fire (NDK clang, aedilis android,posix, radix una /data/local/tmp/rhubarb initio et fine deleta, etiam Ctrl-C) - android-gradus T4

Viva in Fire 7 (KFQUWI, API 30, arm64): piscina chorda xar json volumen PRAETERITAE; planta xar -> exitus 1 nominata; apparatus (nulla/plures/non auctorizata tabula, suita absens, NDK alienum) -> exitus 2 nominatus; Ctrl-C -> radix deleta; -servare / -purgare.

Co-Authored-By: Claude Opus 5.5 (1M context) <noreply@anthropic.com>''',
    ['tools/android/probatio.sh', 'project-specs/android-gradus-spec.md'])
EOF
```

---

## Self-review (done while writing)

- **Spec coverage:** II.1 -> T1; II.2 -> T2 (+R2); II.3 -> T3 (+R1); II.4 -> T4 Step 2; II.5 -> T4 Steps 2, 4, 7; II.6 -> deliberately not built (spec IV); III aedilis/volumen/runner/installer tests -> T1 S1-5, T2 S1-5, T4 S3-7, T3 S4-5.
- **Names across tasks:** `--varians android,posix` (T1 produces, T4 consumes); `NDK_*` variables (T3 produces, T4 consumes); `_basis_temporaria` only inside volumen.c; `TMPDIR=<root>/tmp` (T4) relies on T2.
- **Placeholders:** none; the one "spelling the file actually has" note in T4 Step 5 names the exact check.
