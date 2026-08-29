# Plan A — Substratum: assertiones processus + tres reparationes parsatoris

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make `stml_legere` survive malformed input — no crashes, no hangs — and build the test machinery that proves it.

**Architecture:** One prerequisite (fork-based process assertions in `credo`, shipped with a planted fault so the detector is itself gated), then three independent one-site fixes in `lib/stml.c`. Each fix gets a test that would have caught it, written using the new assertions. Nothing here changes what a *valid* document parses to, so the 196-test baseline must stay green throughout.

**Tech Stack:** C89 (`-std=c89 -pedantic -Werror`), Latin identifiers via `include/latina.h`, `credo` test library, POSIX `fork`/`waitpid` via `postulata_posix.h`.

**Spec:** `project-specs/stml-instrumentum-spec.md` — §7.5.0 through §7.5.3.

## Global Constraints

- **Latin only** for identifiers, comments, and messages. `include/latina.h` maps Latin onto C keywords: `si`=if, `alioquin`=else, `redde`=return, `nomen`=typedef, `interior`=static, `per`=for, `dum`=while, `fac`=do, `frange`=break, `vacuum`=void, `VERUM`/`FALSUM`=1/0, `ZEPHYRUM`=0, `NIHIL`=NULL, `magnitudo`=sizeof, `structura`=struct.
- **Every latina.h word is a FORBIDDEN identifier.** Never name a variable `nomen`, `registrum`, `magnitudo`, `casus`, `brevis`, `structura`, `exire`. Roman numerals for small constants: `I`, `II`, `III`, `V`, `VIII`, `IX`, `X`, `XI`.
- **Compile flags are the ones in `compile_tests.sh`**, not the shorter list in root `CLAUDE.md`. They include `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes -Wwrite-strings -Wno-long-long -Wno-overlength-strings -fbracket-depth=512`.
- **C89 declarations go at the top of a block.** No mid-block declarations, no `//` comments.
- **`chorda` is NOT null-terminated** — fields are `i32 mensura; i8* datum;`. Never pass `.datum` to a `str*` function.
- **`i32`/`i64` are UNSIGNED**; `s32`/`s64` are signed. Any subtraction that can go negative needs a signed type.
- **A POSIX-using `.c` includes `postulata_posix.h` FIRST**, before any other header.
- **Exit contract for every runner: `0` pass, `1` failures, `2` NOTHING RAN.** Never gate on exit 0 alone — always confirm the test count.

## Baseline (measured 2026-08-29, before any change)

| suite | command | result |
|---|---|---|
| root | `./compile_tests.sh` | **141/141**, 0 failed, 172s |
| silva | `./silva/compile_probationes.sh` | **50/50** |
| materia | `./materia/compile_probationes.sh` | **5/5** |

These three are the blast radius of `include/stml.h` (61 files in reverse closure). Every task ends by confirming they still pass with the same counts.

**Uncovered by any runner** (check manually only if a task changes emitted bytes — none in Plan A do): `tools/silex.c`, `tools/census_recensere.c`, `tools/gutenberg_index.c`, `tools/librarium_collector.c`, `tools/librarium_merge.c`, `apps/forum/forum.c`.

---

## File Structure

| file | responsibility |
|---|---|
| `include/credo.h` (modify) | Three new assertion macros + the two helper prototypes they expand to. Stays POSIX-free. |
| `lib/credo.c` (modify) | Fork/waitpid implementation. Already includes `postulata_posix.h`. |
| `probationes/probatio_credo_processus.c` (create) | The planted fault — proves the detector detects. |
| `lib/stml.c` (modify, 3 sites) | The three fixes. One site each; no shared code. |
| `probationes/probatio_stml_incolumitas.c` (create) | Malformed-input tests for all three fixes. |

New `probationes/probatio_*.c` files are discovered by glob, so no list regeneration is needed. (`tools/compile_tests_fontes_generare.sh` regenerates `compile_tests_fontes_generata.sh`, which lists **lib** sources — untouched here.)

---

## Task 1: Process assertions in credo, with a planted fault

**Files:**
- Modify: `include/credo.h` (append after the last macro)
- Modify: `lib/credo.c` (append near the other `_credo_notare` helpers)
- Create: `probationes/probatio_credo_processus.c`

**Interfaces:**
- Consumes: `_credo_notare(genus, expressio, valor_primus, valor_secundus, filum, versus, praeteritus)` — the existing reporting primitive, `lib/credo.c:228`.
- Produces: `CREDO_NON_RUIT(expressio)`, `CREDO_NON_PENDET(expressio, mora_ms)`, `CREDO_RUIT_CUM(expressio, signum)`; the struct `CredoProcessusFructus`; the functions `credo_processus_incipere(vacuum)`, `credo_processus_filium_finire(vacuum)`, `credo_processus_iudicare(CredoProcessusFructus*, constans character*, constans character*, i32, s32, constans character*, s32)`.

**Design note — why a statement macro, not a function call.** `CREDO_NON_RUIT(stml_legere(...))` must NOT pass the expression as an argument: arguments evaluate in the *caller*, so the crash would happen in the parent and take the suite down. The macro must fork FIRST and evaluate the expression only in the child. Hence `fac { ... } dum (FALSUM)` with the expression textually inside the child branch.

- [ ] **Step 1: Write the failing test (the planted fault)**

Create `probationes/probatio_credo_processus.c`:

```c
/* probatio_credo_processus.c - PORTA NATIVA assertionum processus.
 *
 * Detector ruinarum qui ruinas non deprehendit viridem in aeternum
 * refert - peior quam nullus. Ergo culpa INSERTA: haec probatio
 * consulto ruit et affirmat apparatum id VIDERE.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>
#include <signal.h>

/* Opaca consulto: si scriptor optimizans scriptionem ad NIHIL
 * plicare posset, culpa inserta evanesceret et porta taceret. */
vacuum probatio_credo_processus_scribere_ad_nihil (integer* p);

vacuum
probatio_credo_processus_scribere_ad_nihil (
    integer* p)
{
    *p = XI;
}

interior vacuum
_pendere_semper (
    vacuum)
{
    dum (VERUM)
    {
        /* gyrus verus - nulla exitus via */
    }
}

s32
principale (vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_credo_processus",
                                         65536);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("--- Probans apparatum ipsum (culpa inserta) ---\n");

    /* I. Expressio innocua NON ruit. */
    CREDO_NON_RUIT (imprimere(""));

    /* II. CULPA INSERTA: scriptio ad NIHIL RUERE debet, et
     *     apparatus id videre debet. Si haec probatio FRACTA est,
     *     detector caecus est - omnia alia mendacia sunt. */
    CREDO_RUIT_CUM (
        probatio_credo_processus_scribere_ad_nihil((integer*)NIHIL),
        SIGSEGV);

    /* III. Expressio innocua intra terminum perficitur. */
    CREDO_NON_PENDET (imprimere(""), 2000);

    /* IV. CULPA INSERTA SECUNDA: gyrus infinitus terminum EXCEDERE
     *     debet, ergo SIGKILL a nobis - non ruina propria. */
    CREDO_RUIT_CUM (_pendere_semper(), SIGKILL);

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run it to verify it fails**

```bash
./compile_tests.sh credo_processus; echo "EXITUS=$?"
```

Expected: compile FAILS — `CREDO_NON_RUIT` undefined. (If it exits 2, the filter matched nothing; check the filename.)

- [ ] **Step 3: Add the header surface**

Append to `include/credo.h`, before the final `#endif`:

```c
/* ============================================
 * Assertiones processus - "hoc NON ruit"
 * ============================================
 *
 * Expressio in filio furcato curritur, ergo ruina aut gyrus
 * probationem NON occidit. Ratio plena: spec stml-instrumentum
 * par. 7.5.0.
 *
 * CUR MACRO SENTENTIAE, NON VOCATIO FUNCTIONIS: argumenta apud
 * VOCANTEM aestimantur. Si expressio argumentum esset, ruina in
 * PARENTE fieret et suitem totam secum traheret. Ergo furca PRIMA,
 * aestimatio in filio SOLO.
 *
 * NUMQUAM SIGSEGV intra processum capere (manubrium + longjmp):
 * mos ille INDEFINITUS est - a manubrio exceptionis computatricis
 * redire non licet, et post culpam veram status piscinae ignotus
 * est. Praeterea gyros omnino non videt.
 */

nomen structura {
    b32 in_filio;   /* VERUM in filio furcato SOLO */
    s32 pid;        /* in parente: pid filii; -I si furca defecit */
} CredoProcessusFructus;

/* fflush(NIHIL) deinde fork(). In filio redit {VERUM, 0}. */
CredoProcessusFructus
credo_processus_incipere (vacuum);

/* _exit(0) - NUMQUAM exit(): filius buffra parentis hereditavit,
 * et exit() ea eiceret, ergo omnis linea prior bis emitteretur. */
vacuum
credo_processus_filium_finire (vacuum);

/* In parente: filium ad terminum exspectat, deinde iudicat.
 * signum_exspectatum == ZEPHYRUM => praeterit si NORMALITER exiit.
 * signum_exspectatum != ZEPHYRUM => praeterit si EO signo periit. */
vacuum
credo_processus_iudicare (
    CredoProcessusFructus* fructus,
       constans character* genus,
       constans character* expressio,
                      i32  mora_ms,
                      s32  signum_exspectatum,
       constans character* filum,
                      s32  versus);

#define CREDO_MORA_ORDINARIA_MS 5000

#define CREDO_NON_RUIT(expressio) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_non_ruit", #expressio, \
            CREDO_MORA_ORDINARIA_MS, ZEPHYRUM, __FILE__, __LINE__); \
    } dum (FALSUM)

#define CREDO_NON_PENDET(expressio, mora_ms) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_non_pendet", #expressio, \
            (mora_ms), ZEPHYRUM, __FILE__, __LINE__); \
    } dum (FALSUM)

#define CREDO_RUIT_CUM(expressio, signum) \
    fac { \
        CredoProcessusFructus _cpf = credo_processus_incipere(); \
        si (_cpf.in_filio) \
        { \
            (expressio); \
            credo_processus_filium_finire(); \
        } \
        credo_processus_iudicare(&_cpf, "credo_ruit_cum", #expressio, \
            CREDO_MORA_ORDINARIA_MS, (signum), __FILE__, __LINE__); \
    } dum (FALSUM)
```

- [ ] **Step 4: Implement in `lib/credo.c`**

Append near the other helpers. `postulata_posix.h` is already included at line 2; add `<sys/wait.h>`, `<unistd.h>`, `<signal.h>`, `<time.h>` if not present.

```c
/* ============================================
 * Assertiones processus (spec stml-instrumentum par. 7.5.0)
 * ============================================ */

interior constans character*
_credo_signi_nomen (
    s32 signum)
{
    commutatio (signum)
    {
        casus SIGSEGV: redde "SEGV";
        casus SIGBUS:  redde "BUS";
        casus SIGABRT: redde "ABRT";
        casus SIGFPE:  redde "FPE";
        casus SIGILL:  redde "ILL";
        casus SIGKILL: redde "KILL (mora excessa)";
        ordinarius:    redde "signum ignotum";
    }
}

CredoProcessusFructus
credo_processus_incipere (
    vacuum)
{
    CredoProcessusFructus fructus;
    pid_t                 pid;

    fructus.in_filio = FALSUM;
    fructus.pid      = -I;

    /* Buffra ANTE furcam eicienda: aliter quidquid pendet in filium
     * transcribitur et bis emittitur. */
    fflush(NIHIL);

    pid = fork();
    si (pid == ZEPHYRUM)
    {
        fructus.in_filio = VERUM;
        fructus.pid      = ZEPHYRUM;
        redde fructus;
    }

    fructus.pid = (s32)pid;   /* -I si furca defecit */
    redde fructus;
}

vacuum
credo_processus_filium_finire (
    vacuum)
{
    /* _exit, NON exit: buffra parentis hereditata sunt. */
    _exit(ZEPHYRUM);
}

vacuum
credo_processus_iudicare (
    CredoProcessusFructus* fructus,
       constans character* genus,
       constans character* expressio,
                      i32  mora_ms,
                      s32  signum_exspectatum,
       constans character* filum,
                      s32  versus)
{
    integer         status;
    pid_t           peractus;
    i32             elapsum_ms;
    b32             praeteritus;
    s32             signum_acceptum;
    struct timespec pausa;

    si (fructus == NIHIL || fructus->in_filio)
    {
        redde;   /* filius numquam iudicat */
    }

    si (fructus->pid < ZEPHYRUM)
    {
        _credo_notare(genus, expressio, "furca defecit", "",
                      filum, versus, FALSUM);
        redde;
    }

    /* Terminus per POLLATIONEM, non per alarm(): SIGALRM cum omni
     * usu signorum in codice probato colliditur. */
    status          = ZEPHYRUM;
    elapsum_ms      = ZEPHYRUM;
    signum_acceptum = ZEPHYRUM;
    pausa.tv_sec    = ZEPHYRUM;
    pausa.tv_nsec   = 1000000L;   /* I ms */

    dum (VERUM)
    {
        peractus = waitpid((pid_t)fructus->pid, &status, WNOHANG);
        si (peractus == (pid_t)fructus->pid)
        {
            frange;
        }
        si (elapsum_ms >= mora_ms)
        {
            /* SIGKILL, non SIGTERM: processus impeditus manubrium
             * numquam curret. Praeterea SIGKILL in relatione
             * inambiguus est - NOS eum occidimus. */
            kill((pid_t)fructus->pid, SIGKILL);
            waitpid((pid_t)fructus->pid, &status, ZEPHYRUM);
            frange;
        }
        nanosleep(&pausa, NIHIL);
        elapsum_ms = elapsum_ms + I;
    }

    si (WIFSIGNALED(status))
    {
        signum_acceptum = (s32)WTERMSIG(status);
    }

    si (signum_exspectatum == ZEPHYRUM)
    {
        /* Praeterit SOLUM si normaliter exiit. Codex exitus ruinam
         * NUMQUAM narrare potest - processus mortuus nullum habet;
         * ergo WIFSIGNALED est mechanismus, non WEXITSTATUS. */
        praeteritus = (b32)(WIFEXITED(status) != ZEPHYRUM);
    }
    alioquin
    {
        praeteritus = (b32)(signum_acceptum == signum_exspectatum);
    }

    si (praeteritus)
    {
        _credo_notare(genus, expressio, "ut exspectatum", "",
                      filum, versus, VERUM);
    }
    alioquin si (signum_acceptum != ZEPHYRUM)
    {
        _credo_notare(genus, expressio,
                      _credo_signi_nomen(signum_acceptum), "",
                      filum, versus, FALSUM);
    }
    alioquin
    {
        _credo_notare(genus, expressio, "exitus normalis", "",
                      filum, versus, FALSUM);
    }
}
```

- [ ] **Step 5: Run the planted-fault test and verify it PASSES**

```bash
./compile_tests.sh credo_processus; echo "EXITUS=$?"
```

Expected: `EXITUS=0`, four assertions, all passing. **In particular the two `CREDO_RUIT_CUM` cases must PASS** — that is the evidence the detector detects. If they FAIL, the harness is blind and nothing built on it can be trusted.

- [ ] **Step 6: Prove the gate is not silently green**

Temporarily change `SIGSEGV` to `SIGABRT` in the planted-fault assertion, rerun, and confirm it **FAILS** and the message names `SEGV`. Then change it back and confirm it passes again. Do not commit the temporary change.

```bash
./compile_tests.sh credo_processus; echo "EXITUS=$?"   # expect EXITUS=1 while flipped
```

- [ ] **Step 7: Confirm the full baseline is untouched**

```bash
./compile_tests.sh              ; echo "RADIX=$?"
./silva/compile_probationes.sh  ; echo "SILVA=$?"
./materia/compile_probationes.sh; echo "MATERIA=$?"
```

Expected: all `=0`, and counts of **142/142** (141 + the new file), **50/50**, **5/5**. A count that did not go up by one means the new file was not discovered.

- [ ] **Step 8: Commit**

```bash
git add include/credo.h lib/credo.c probationes/probatio_credo_processus.c
git commit -m "credo: assertiones processus (NON_RUIT / NON_PENDET / RUIT_CUM)

Furca + waitpid cum termino; ruina et gyrus probationem non occidunt.
Porta nativa: culpa inserta (scriptio ad NIHIL + gyrus infinitus)
affirmat apparatum ruinas VIDERE - detector caecus viridem in
aeternum referret.

Spec: project-specs/stml-instrumentum-spec.md par. 7.5.0"
```

---

## Task 2: Fix the infinite loop — custos progressus

**Files:**
- Modify: `lib/stml.c` — the parser's child loop, `_liberos_legere` (~line 2733)
- Create: `probationes/probatio_stml_incolumitas.c`

**Interfaces:**
- Consumes: `CREDO_NON_PENDET` from Task 1.
- Produces: nothing new; behavior change only.

**Ticket:** `01M16YP7W1`. **Spec:** §7.5.1.

**Mechanism:** `_tok_proximus` checks raw mode (`lib/stml.c:1484`) *before* the EOF check (`1516`). Once `in_crudus` is set, `STML_TOKEN_FINIS` is unreachable: at EOF, `_tok_legere_contentus_crudus` returns a zero-length token without advancing `positus`. The child loop waits for FINIS forever. Two doors: genuinely unterminated raw content, and `<!` fallthrough mis-lexed as a raw tag with an *empty* raw title.

- [ ] **Step 1: Write the failing test**

Create `probationes/probatio_stml_incolumitas.c`:

```c
/* probatio_stml_incolumitas.c - Inputa male formata parsatorem
 * NEC occidere NEC impedire debent.
 *
 * Omnia haec ante reparationes 01M16YP7W1 / 01M16Z03YE /
 * 01M171YAEP pendebant aut ruebant. Assertiones per furcam currunt
 * (credo par. 7.5.0), ergo regressio suitem non trahit.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "credo.h"
#include "stml.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>

interior vacuum
_parsare (
    constans character* fons)
{
             Piscina* p;
 InternamentumChorda* it;

    p = piscina_generare_dynamicum("incolumitas", 1 << 20);
    si (p == NIHIL)
    {
        redde;
    }
    it = internamentum_creare(p);
    (vacuum)stml_legere_ex_literis(fons, p, it);
    piscina_destruere(p);
}

s32
principale (vacuum)
{
    Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_stml_incolumitas",
                                         65536);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("--- Gyri: contentum crudum non clausum (porta I) ---\n");
    CREDO_NON_PENDET (_parsare("<x!>a"),          3000);
    CREDO_NON_PENDET (_parsare("<x!>a</y>"),      3000);
    CREDO_NON_PENDET (_parsare("<x!>a</>"),       3000);
    CREDO_NON_PENDET (_parsare("<x!>a</#>"),      3000);

    imprimere("--- Gyri: lapsus '<!' (porta II) ---\n");
    CREDO_NON_PENDET (_parsare("<!x>"),           3000);
    CREDO_NON_PENDET (_parsare("<!!>"),           3000);
    CREDO_NON_PENDET (_parsare("<!doctype html>"), 3000);
    CREDO_NON_PENDET (_parsare("<![CDATA[x]]>"),  3000);
    CREDO_NON_PENDET (_parsare("<!ENTITY a \"b\">"), 3000);

    imprimere("--- Custodes: haec parsare DEBENT ---\n");
    CREDO_NON_PENDET (_parsare("<!DOCTYPE html>"), 3000);
    CREDO_NON_PENDET (_parsare("<!-- x -->"),      3000);
    CREDO_NON_PENDET (_parsare("<x!>a</x>"),       3000);

    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
```

- [ ] **Step 2: Run to verify it fails**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=1`. Nine `credo_non_pendet` assertions FAIL, each reporting `KILL (mora excessa)`. The three guards pass. **The suite must still terminate** — that is Task 1 doing its job; if the runner wedges here, Task 1 is broken.

- [ ] **Step 3: Implement the guard**

In `lib/stml.c`, `_liberos_legere` (~2721). **Note the field:** this function
takes `StmlParserContext`, which does NOT have a `positus` of its own — it
embeds `StmlTokenContext tok_ctx` (line 1778), so the tokenizer position is
`ctx->tok_ctx.positus`.

The loop already declares `StmlNodus* liberum;` at the top of its body, so
adding a second declaration there is C89-legal. It has **no `perge` paths**
and exactly one early `frange` (the anonymous-close-closes-fragment case),
which fires *before* any work and is a legitimate exit — the guard goes at
the END of the body and that `frange` correctly bypasses it.

```c
    dum (   ctx->current.genus != terminator
         && ctx->current.genus != STML_TOKEN_FINIS)
    {
        StmlNodus* liberum;
               i32  positus_ante;

        positus_ante = ctx->tok_ctx.positus;

        /* ... corpus ansae exsistens, immutatum, usque ad finem ... */

        /* CUSTOS PROGRESSUS (01M16YP7W1): lexema quod positum non
         * promovet gyrum infinitum facit. Idem custos in
         * stml_lexemata_colligere (lib/stml.c:1759) iam vivit -
         * hic TRANSLATUS est, non inventus. DIFFERENTIA NOTANDA:
         * lexator frangit (fluxus truncatus vocanti apparet),
         * parsator ERRARE debet - aliter gyrum pro truncatione
         * TACITA commutaremus, quod peius est. */
        si (ctx->tok_ctx.positus <= positus_ante)
        {
            _errorem_ponere(ctx, STML_ERROR_TAG_NON_CLAUSUM,
                            ctx->current.linea, ctx->current.columna);
            frange;
        }
    }
```

**If the guard trips on a valid document** (the 143-test baseline in Step 5
is what detects this), the alternative signal is `ctx->finis_ultimus` — "end
of the last CONSUMED token", line 1793. It is arguably the more precise
notion of "did we consume anything", and the zero-width token that causes
this bug leaves it unchanged too. Try `tok_ctx.positus` first; it is the
direct analogue of the guard being transplanted.

- [ ] **Step 4: Run to verify it passes**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=0`, 12/12.

- [ ] **Step 5: Confirm the baseline**

```bash
./compile_tests.sh              ; echo "RADIX=$?"
./silva/compile_probationes.sh  ; echo "SILVA=$?"
./materia/compile_probationes.sh; echo "MATERIA=$?"
```

Expected: `143/143`, `50/50`, `5/5`, all exit 0. **If any silva test regresses, stop** — `silva_arbor.c` is the largest STML consumer and a false guard trip would show up there first.

- [ ] **Step 6: Commit**

```bash
git add lib/stml.c probationes/probatio_stml_incolumitas.c
git commit -m "stml: custos progressus in ansa parsatoris (01M16YP7W1)

Ramus modi crudi in _tok_proximus ante probationem EOF sedet, ergo
STML_TOKEN_FINIS post 'in_crudus' inattingibilis fit et ansa in
aeternum gyrat. Custos idem quem stml_lexemata_colligere iam fert
(1759) hic translatus - sed ERRAT loco frangendi.

Claudit VII inputa per portas ambas: contentum crudum non clausum,
et lapsum '<!' (qui ut tagus crudus tituli VACUI male lexatur).

Spec: par. 7.5.1"
```

---

## Task 3: Fix the segfault in comment / processio

**Files:**
- Modify: `lib/stml.c` — `_tok_legere_commentum` (~428-475), `_tok_legere_processio` (~477-521)
- Modify: `probationes/probatio_stml_incolumitas.c`

**Interfaces:**
- Consumes: `CREDO_NON_RUIT` from Task 1.
- Produces: nothing new.

**Ticket:** `01M16Z03YE`. **Spec:** §7.5.2.

**Mechanism:** Both functions assign `token.valor` ONLY inside the branch that finds the terminator. At EOF they return a stack local never written — arbitrary `datum` and `mensura` — which `chorda_internare` then dereferences. **`_tok_legere_doctype` does NOT have the bug**: it assigns unconditionally outside the loop (~563). Copy its shape.

**Scope, measured:** only a genuinely unterminated comment/PI crashes. `<!-- x --->`, `<!-- x ---->`, `<!-- a -- b -->`, `<!---->` all parse cleanly — the terminator search finds `-->` wherever it sits and extra dashes are content.

- [ ] **Step 1: Write the failing test**

Append to `principale` in `probationes/probatio_stml_incolumitas.c`, before `credo_imprimere_compendium()`:

```c
    imprimere("--- Ruinae: commentum / processio non clausa ---\n");
    CREDO_NON_RUIT (_parsare("<!--x"));
    CREDO_NON_RUIT (_parsare("<?x"));
    CREDO_NON_RUIT (_parsare("<r><!-- x</r>"));
    CREDO_NON_RUIT (_parsare("<r><?php echo 1; </r>"));

    imprimere("--- Custodes: haec clausa sunt, parsare DEBENT ---\n");
    CREDO_NON_RUIT (_parsare("<!-- x -->"));
    CREDO_NON_RUIT (_parsare("<!-- x --->"));
    CREDO_NON_RUIT (_parsare("<!-- x ---->"));
    CREDO_NON_RUIT (_parsare("<!-- a -- b -->"));
    CREDO_NON_RUIT (_parsare("<!---->"));
    CREDO_NON_RUIT (_parsare("<r><?x?></r>"));
    CREDO_NON_RUIT (_parsare("<!DOCTYPE x"));
```

- [ ] **Step 2: Run to verify it fails**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=1`. The first four FAIL reporting `SEGV`; the seven guards pass. Note `<!DOCTYPE x` (unterminated) passes even now — that is the sibling function that got it right.

- [ ] **Step 3: Implement**

In `_tok_legere_commentum`, initialize `token.valor` before the search loop:

```c
    /* Valor ANTE ansam initializandus (01M16Z03YE): olim intra
     * ramum terminatoris SOLUM assignabatur, ergo ad EOF locale
     * stacki NON INITIALIZATUM reddebatur, quod chorda_internare
     * deinde dereferenciabat. _tok_legere_doctype hoc iam recte
     * facit (assignat extra ansam) - forma eius hic sequitur. */
    token.valor.datum   = ctx->input.datum + contentus_initium;
    token.valor.mensura = ZEPHYRUM;
```

Place it immediately after `contentus_initium` is set and before `dum (ctx->positus < ctx->input.mensura)`. Apply the identical change in `_tok_legere_processio`.

If a cleaner refusal is preferred, raise `STML_ERROR_TAG_NON_CLAUSUM` on the EOF path instead — but the initialization is required regardless, because the token is returned either way.

- [ ] **Step 4: Run to verify it passes**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=0`, 23/23.

- [ ] **Step 5: Confirm the baseline**

```bash
./compile_tests.sh              ; echo "RADIX=$?"
./silva/compile_probationes.sh  ; echo "SILVA=$?"
./materia/compile_probationes.sh; echo "MATERIA=$?"
```

Expected: `143/143`, `50/50`, `5/5`, all exit 0.

- [ ] **Step 6: Commit**

```bash
git add lib/stml.c probationes/probatio_stml_incolumitas.c
git commit -m "stml: token.valor initializandum in commento/processione (01M16Z03YE)

Ambae functiones valorem intra ramum terminatoris SOLUM assignabant;
ad EOF locale stacki non initializatum reddebatur et
chorda_internare id dereferenciabat. '<!--x' - V octeti - SEGV.

Gravius quam ruina sola: lectio memoriae non initializatae
INDETERMINATA est, ergo sub alia dispositione stacki memoriam
adiacentem in arborem internare ET successus=1 reddere posset.

_tok_legere_doctype formam rectam iam habebat (assignatio extra
ansam); ea hic sequitur.

Spec: par. 7.5.2"
```

---

## Task 4: Fix the segfault on a nameless tag

**Files:**
- Modify: `lib/stml.c` — `_parser_legere_elementum` (~1963-1999)
- Modify: `probationes/probatio_stml_incolumitas.c`

**Interfaces:**
- Consumes: `CREDO_NON_RUIT` from Task 1.
- Produces: nothing new.

**Ticket:** `01M171YAEP`. **Spec:** §7.5.3.

**Mechanism:** `<` followed by a space makes `_tok_legere_nomen` (~190) return an empty chorda; `chorda_internare` of an empty chorda returns NIHIL; so `nodus->titulus` is NIHIL; `titulus_ptr` (1969) is NIHIL; and line 1997 dereferences it with no guard. Two conditions must coincide — an empty name AND a closing tag to compare against — which is why `< >` alone and `<p>a <b</p>` are both safe today.

**This is not a parsing problem.** STML, like XML, does not disambiguate prose from markup: a bare `<` in text is an ERROR and `&lt;` is the escape. That policy is already implemented. The parser already concludes "malformed"; it crashes on the way to saying so.

**CAUTIO — do NOT reject empty names at LEX time.** That breaks `</>`. The dispatch table in `_tok_proximus` routes `<<`, `<!--`, `<?`, `<!D`, `</#`, `</%`, `<#`, `<%`, `<(`, `<>` to dedicated readers, but **`</>` falls through to `_tok_legere_tag`**, and its empty name is exactly the signal read at 1988 to set `clausura_anonyma`. `</>` has 9 uses in `probatio_stml_macros.c` alone. The fix is parser-side and opening-tag-only.

- [ ] **Step 1: Write the failing test**

Append to `principale` in `probationes/probatio_stml_incolumitas.c`, before `credo_imprimere_compendium()`:

```c
    imprimere("--- Ruinae: tagum apertum sine nomine ---\n");
    CREDO_NON_RUIT (_parsare("<p>a < b</p>"));
    CREDO_NON_RUIT (_parsare("<p>x < 10</p>"));
    CREDO_NON_RUIT (_parsare("<p>a < </p>"));

    imprimere("--- Custodes: formae anonymae LEGITIMAE ---\n");
    CREDO_NON_RUIT (_parsare("<r><a>x</></r>"));
    CREDO_NON_RUIT (_parsare("<r><>a</></r>"));
    CREDO_NON_RUIT (_parsare("<r><(> <a/></r>"));
    CREDO_NON_RUIT (_parsare("<r><tag><>a</><>b</></tag></r>"));
    CREDO_NON_RUIT (_parsare("<p>a <b</p>"));
    CREDO_NON_RUIT (_parsare("<p>5 &lt; 3</p>"));
```

- [ ] **Step 2: Run to verify it fails**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=1`. The first three FAIL reporting `SEGV`; all six guards pass. **The `</>`, `<>`, `<(>` guards passing before the fix is the point** — they must still pass after.

- [ ] **Step 3: Implement**

In `_parser_legere_elementum` (`lib/stml.c`), immediately after `titulus_ptr` is taken:

```c
    _titulum_ex_tokeno_ponere(ctx, nodus, ctx->current.valor);
    titulus_ptr        = nodus->titulus;

    /* Tagum apertum sine nomine (01M171YAEP): '<' spatio sequente
     * nomen VACUUM dat, chorda_internare vacuae NIHIL reddit, et
     * comparatio clausurae infra (*titulus_ptr) sine custode
     * dereferenciabat. Hic recusatur ut diagnosticum in '<' cadat,
     * non in tagum claudens quod postea sequitur.
     *
     * NON ad tempus lexationis: '</>' per _tok_legere_tag cadit et
     * nomen VACUUM eius est ipsum signum clausurae anonymae -
     * recusatio ibi eam frangeret. Hic tagum APERTUM solum
     * tangitur; clausura infra in ctx->current iudicatur. */
    si (titulus_ptr == NIHIL)
    {
        _errorem_ponere(ctx, STML_ERROR_SYNTAXIS,
                        ctx->current.linea, ctx->current.columna);
        redde NIHIL;
    }
```

Then add the belt-and-braces guard at the comparison (~1997):

```c
        alioquin si (   titulus_ptr == NIHIL
                     || !chorda_aequalis(ctx->current.valor,
                                         *titulus_ptr))
```

- [ ] **Step 4: Run to verify it passes**

```bash
./compile_tests.sh stml_incolumitas; echo "EXITUS=$?"
```

Expected: `EXITUS=0`, 32/32.

- [ ] **Step 5: Confirm the baseline — with extra care on macros**

```bash
./compile_tests.sh stml_macros   ; echo "MACROS=$?"
./compile_tests.sh               ; echo "RADIX=$?"
./silva/compile_probationes.sh   ; echo "SILVA=$?"
./materia/compile_probationes.sh ; echo "MATERIA=$?"
```

Expected: all `=0`, counts `143/143`, `50/50`, `5/5`. **`probatio_stml_macros` is run first and separately** because it holds the 9 `</>` uses — it is the test most likely to catch a fix that reached too far.

- [ ] **Step 6: Commit**

```bash
git add lib/stml.c probationes/probatio_stml_incolumitas.c
git commit -m "stml: tagum apertum sine nomine recusandum (01M171YAEP)

'<' spatio sequente ('<p>x < 10</p>' - prosa vulgaris) nomen vacuum
dat; chorda_internare vacuae NIHIL reddit; comparatio clausurae
*titulus_ptr sine custode dereferenciabat. SEGV.

Non est problema parsandi: STML, ut XML, prosam a marcatione non
discernit - '<' nudum ERRATUM est, '&lt;' effugium, et ea politia
iam implementata erat. Parsator iam concludebat 'male formatum' et
in ipso actu id DICENDI ruebat.

Recusatio in tago APERTO ponitur (non ad tempus lexationis): '</>'
per eandem viam lexatur et nomen vacuum eius est signum clausurae
anonymae - IX usus in probatio_stml_macros.c solo.

Spec: par. 7.5.3"
```

---

## Task 5: Close the tickets and record the outcome

**Files:** none in the repo — tabularium only.

- [ ] **Step 1: Verify all three suites one final time from a clean state**

```bash
./compile_tests.sh               ; echo "RADIX=$?"
./silva/compile_probationes.sh   ; echo "SILVA=$?"
./materia/compile_probationes.sh ; echo "MATERIA=$?"
```

Expected `143/143`, `50/50`, `5/5`, all exit 0. Confirm the counts, not just the exit codes — exit 2 means nothing ran.

- [ ] **Step 2: Re-run the original reproductions by hand, outside the suite**

The in-suite tests run behind a fork, so they prove the *harness* copes. This
step proves the parser itself does, with no harness in the way. Write
`probe_incolumitas.c` anywhere outside the repo tree:

```c
#include "latina.h"
#include "stml.h"
#include <stdio.h>

integer
principale (
    integer numerus,
    character** argumenta)
{
             Piscina* p;
 InternamentumChorda* it;
        StmlResultus  r;

    /* Sine hoc, gyrus omnem effusionem priorem devorat et quis
     * casus impediverit dinosci non potest. */
    setvbuf(stdout, NIHIL, _IONBF, 0);
    si (numerus < II) { redde II; }

    p  = piscina_generare_dynamicum("probe", 1 << 20);
    it = internamentum_creare(p);
    imprimere("in = %-24s | parsans...", argumenta[I]);
    r  = stml_legere_ex_literis(argumenta[I], p, it);
    imprimere(" REDIIT successus=%d status=%d\n",
        (int)r.successus, (int)r.status);
    redde ZEPHYRUM;
}
```

Build and drive it (macOS has no `timeout(1)`; `perl -e 'alarm N'` is the
portable watchdog here):

```bash
clang -std=c89 -pedantic -Wall -Wno-long-long -Iinclude probe_incolumitas.c \
  build/*.o -framework Cocoa -framework Security -framework WebKit -o /tmp/probe

for c in '<x!>a' '<x!>a</y>' '<x!>a</>' '<x!>a</#>' '<!x>' '<!!>' \
         '<!doctype html>' '<![CDATA[x]]>' '<!--x' '<?x' '<r><!-- x</r>' \
         '<p>a < b</p>' '<p>x < 10</p>' '<p>a < </p>'; do
  perl -e 'alarm 5; exec @ARGV or exit 127' -- /tmp/probe "$c"
  rc=$?
  [ $rc -eq 139 ] && echo "  *** ADHUC SEGV ***"
  { [ $rc -eq 142 ] || [ $rc -eq 14 ]; } && echo "  *** ADHUC PENDET ***"
done
```

Expected: **every line prints `REDIIT`** with a non-zero `status`. No `SEGV`,
no `PENDET`. Any input that still crashes or hangs means its fix is
incomplete — go back to the relevant task rather than closing the ticket.

- [ ] **Step 3: Close the tickets**

Set each to `clausum` with a note naming the commit and the test that now guards it:

- `01M16YP7W1` — the two loop doors
- `01M16Z03YE` — the comment/PI uninitialized read
- `01M171YAEP` — the nameless tag

- [ ] **Step 4: Add a worklog entry**

Append to `lib/stml.worklog.md`: the three mechanisms, why no existing gate caught them (the goldens were produced by the writer itself, so the oracle forgave exactly what the writer consistently did), and that `probatio_stml_incolumitas.c` is now the standing guard for the whole malformed-input class.

---

## Out of scope for Plan A

- **§7.5.4** (delete the `"` case from `_scribere_evasus`) — it changes *emitted bytes* for 47,353 corpus sites, so it ships beside `formare` in Plan B with the diff reviewed on its own.
- **The `stml` tool itself** (`formare`, `vertere`, `tools/stml_struere.sh`) — Plan B.
- **The fidelity writer's 22 divergence classes** (`01M16XGV45`) — explicitly out of scope per Fran, 2026-08-29. Filed, not fixed; becomes real only when `mutatio` needs minimal diffs.
- **Single-quoted attribute rejection** (§7.3) — decreed but sequenced with Plan B, since it needs the ~15-site corpus migration verified by a parse-based check first.
