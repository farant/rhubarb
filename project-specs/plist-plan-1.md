# plist plan 1 — the library (P1–P7 of plist-spec.md)

> **For agentic workers:** REQUIRED SUB-SKILL: Use
> superpowers:subagent-driven-development (recommended) or
> superpowers:executing-plans to implement this plan task-by-task.
> Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** `include/plist.h` + `lib/plist.c` — read an XML property list
into a typed value, inspect and edit it, write it back, compare two —
with the five gaps it exposed fixed in their own libraries
(`chorda`, `xml`, `fasti`), every gate born red, and `plutil` as the
foreign oracle.

**Architecture:** plist owns the plist GRAMMAR and nothing else. The
XML layer below it already parses and serializes with escaping in both
directions, so plist builds an `XmlNodus` tree to write and walks one
to read; the prolog and DOCTYPE are two literal lines it prepends. A
value is a tagged union in a `Piscina`, with a dict as an ORDERED list
of pairs so that read → write is byte-stable.

**Tech Stack:** C89 under the house flags; `xml`, `chorda`, `base64`,
`fasti`, `xar`, `piscina`, `credo`; bash runner; `/usr/bin/plutil` as
an external oracle; pythonica (`silva.commissio`, `silva.planta`) for
gates and commits.

**Spec:** `project-specs/plist-spec.md` v1 — this plan implements §3
(value model), §4 (reading), §5 (writing), §6 (the five home-library
fixes), §7 (gates G1–G7) and §8 (phases P1–P7). Two amendments to the
spec are made by this plan and recorded in Task 7: (a) `plist_scribere`
takes a `PlistStatus*` out-param, since §6.7's control-character
refusal needs a cause; (b) `XML_ERROR_SUBSETUM_INTERNUM` is appended to
`XmlStatus`, which §6.1 implied without naming.

## Global Constraints

- **Compile flags, verbatim** (`tools/vexilla.sh`, never a local copy):
  `-std=c89 -pedantic -Wall -Wextra -Werror -Wconversion
  -Wsign-conversion -Wcast-qual -Wstrict-prototypes
  -Wmissing-prototypes -Wwrite-strings -Wno-long-long
  -Wno-overlength-strings -fbracket-depth=512 -O2 -g`
- **Latin identifiers, comments and messages.** English only in `.md`.
- **`latina.h` macros are forbidden as identifiers** — never `nomen`,
  `casus`, `magnitudo`, `structura`, `brevis`, `registrum`, `exire`;
  single capitals `I V X L C D M` are Roman numerals, not names.
- **C89 shape:** every declaration at the head of its block; no `//`;
  no `_Bool`; no variadic macros; no flexible array members.
- **`i32`/`i64` are UNSIGNED, `s32`/`s64` signed.** Any bound derived
  by subtraction needs a guard (`mensura - I` when `mensura` may be 0).
- **`chorda` is NOT NUL-terminated** and may legally contain NUL.
- **`s64` is `longus longus`** (`include/latina.h:412`), compiling only
  because of `-Wno-long-long`. `-std=c89` defines `__STRICT_ANSI__`, so
  Apple's `<stdlib.h>` hides `strtoll` — s64 parsing is hand-rolled
  (Task 1), never a library call.
- **Every gate is born red.** Use `silva.planta(via, vetus, novus,
  <porta>, <filtrum>)`: plant, gate must go RED (not a build error),
  revert, gate must go GREEN. A gate never seen red is unproven.
- **A refusal test asserts the SPECIFIC status**, never merely that
  something was refused.
- **Format before commit:** `./silva/formator.sh <viae> -scribere` then
  `-vitia`; lines ≤ 72 columns.
- **New identifier words need a glossary line:** `./oratio/vocabula.sh
  -nova` names them; add the entry to `oratio/glossarium.stml`.
- **Commit from Python, never chained in bash:** `silva.commissio(msg,
  viae, portae=[...])`. `set -e` and `pipestatus` have both failed here.
- **Exit 2 from any runner means NOTHING RAN** — never read it as green.
- **New C file** → `./silva/scribe.sh <via> <<'EOF'` (write + examen in
  one call). **Existing C file** → the Edit tool or `silva.Editio`.

---

## File structure

| File | Responsibility |
|---|---|
| `include/chorda.h` (modify) | declare `chorda_ut_s64`, `chorda_ex_s64`, `chorda_ex_f64_exacta` |
| `lib/chorda.c` (modify) | the three new functions; NUL + range refusals inside `chorda_ut_s32/i32/f64` |
| `probationes/probatio_chorda.c` (modify) | Task 1 gates |
| `include/xml.h` (modify) | append `XML_ERROR_SUBSETUM_INTERNUM` to `XmlStatus` |
| `lib/xml.c` (modify, `:1526`) | skip `<!DOCTYPE …>` in the prolog; refuse an internal subset |
| `probationes/probatio_xml.c` (modify) | Task 2 gates |
| `include/fasti.h` (modify) | declare `fasti_ad_iso`, `fasti_ex_iso` |
| `lib/fasti.c` (modify) | the ISO-8601 datetime pair |
| `probationes/probatio_fasti.c` (modify) | Task 3 gates |
| `include/plist.h` (create) | the whole public surface: genera, value, statuses, read, write, compare |
| `lib/plist.c` (create) | value constructors, dict/list access, writer, reader |
| `probationes/probatio_plist.c` (create) | G1–G5, G7 |
| `probationes/fixa/plist/apple_info.plist` (create) | Apple-written fixture, copied from a `.dSYM` |
| `probationes/probatio_plist_plutil.sh` (create) | G6, the foreign oracle, control first |
| `compile_tests.sh` (modify, before `return 0` at `:652`) | register G6 beside the qr gate |
| `oratio/glossarium.stml` (modify) | one `ignotum-permissum` line for `plist` |
| `lib/plist.worklog.md` (create), `lib/chorda.worklog.md` (modify) | findings at find-time |

---

### Task 1: `chorda` — the s64 pair, the exact real, the family refusals

Implements spec §6.2, §6.3, §6.4. Nothing downstream compiles without
it, and each piece is useful on its own.

**Files:**
- Modify: `include/chorda.h` (after `chorda_ex_f64`, `:268`)
- Modify: `lib/chorda.c` (new functions; guards inside `chorda_ut_s32`
  `:998-1032`, `chorda_ut_i32` `:1034-1070`, `chorda_ut_f64`
  `:1613-1647`)
- Modify: `probationes/probatio_chorda.c` (insert before
  `credo_imprimere_compendium()` at `:1438`)
- Modify: `lib/chorda.worklog.md`

**Interfaces:**
- Consumes: nothing from other tasks.
- Produces: `b32 chorda_ut_s64(chorda, s64*)`,
  `chorda chorda_ex_s64(s64, Piscina*)`,
  `chorda chorda_ex_f64_exacta(f64, Piscina*)`; and the now-strict
  behaviour of `chorda_ut_s32` / `chorda_ut_i32` / `chorda_ut_f64`
  (embedded NUL and out-of-range both FALSUM).

- [ ] **Step 1: Write the failing tests**

Insert before `credo_imprimere_compendium();` in
`probationes/probatio_chorda.c`:

```c
    /* ---- s64: ambae directiones, limites exacti, recusationes ---- */
    {
        chorda      s;
        s64         v;
        i8          cum_nullo[III];
        f64         r;
        i32         i;
        constans character* bona[V];
        s64         exspectata[V];

        imprimere("\n--- Probans chorda_ut_s64 / ex_s64 ---\n");

        bona[0]       = "0";
        exspectata[0] = 0;
        bona[1]       = "-1";
        exspectata[1] = -1;
        bona[2]       = "9223372036854775807";
        exspectata[2] = 9223372036854775807LL;
        bona[3]       = "-9223372036854775808";
        exspectata[3] = -9223372036854775807LL - 1LL;
        bona[4]       = "0000000000000000000012";
        exspectata[4] = 12;

        per (i = ZEPHYRUM; i < V; i++)
        {
            CREDO_VERUM(chorda_ut_s64(
                chorda_ex_literis(bona[i], piscina), &v));
            CREDO_AEQUALIS_S64(v, exspectata[i]);
        }

        /* iter reditus: ex_s64 -> ut_s64 idem */
        per (i = ZEPHYRUM; i < V; i++)
        {
            CREDO_VERUM(chorda_ut_s64(
                chorda_ex_s64(exspectata[i], piscina), &v));
            CREDO_AEQUALIS_S64(v, exspectata[i]);
        }
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_s64(0, piscina), "0");
        CREDO_CHORDA_AEQUALIS_LITERIS(chorda_ex_s64(-1, piscina), "-1");
        CREDO_CHORDA_AEQUALIS_LITERIS(
            chorda_ex_s64(-9223372036854775807LL - 1LL, piscina),
            "-9223372036854775808");

        /* superfluitas, sordes, vacuum */
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("9223372036854775808", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("99999999999999999999", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("12x", piscina), &v));
        CREDO_FALSUM(chorda_ut_s64(
            chorda_ex_literis("-", piscina), &v));

        /* NULLUM insertum: familia tota recusat */
        cum_nullo[0]  = (i8)'1';
        cum_nullo[1]  = (i8)ZEPHYRUM;
        cum_nullo[2]  = (i8)'2';
        s.datum       = cum_nullo;
        s.mensura     = III;
        CREDO_FALSUM(chorda_ut_s64(s, &v));
        {
            s32 a;
            i32 b;

            CREDO_FALSUM(chorda_ut_s32(s, &a));
            CREDO_FALSUM(chorda_ut_i32(s, &b));
            CREDO_FALSUM(chorda_ut_f64(s, &r));
        }
    }

    /* ---- superfluitas s32 / i32: truncatio tacita recusata ---- */
    {
        s32 a;
        i32 b;

        imprimere("\n--- Probans limites s32 / i32 ---\n");

        CREDO_VERUM(chorda_ut_s32(
            chorda_ex_literis("2147483647", piscina), &a));
        CREDO_AEQUALIS_S32(a, 2147483647L);
        CREDO_VERUM(chorda_ut_s32(
            chorda_ex_literis("-2147483648", piscina), &a));
        CREDO_AEQUALIS_S32(a, -2147483647L - 1L);
        CREDO_FALSUM(chorda_ut_s32(
            chorda_ex_literis("2147483648", piscina), &a));
        CREDO_FALSUM(chorda_ut_s32(
            chorda_ex_literis("99999999999", piscina), &a));

        CREDO_VERUM(chorda_ut_i32(
            chorda_ex_literis("4294967295", piscina), &b));
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis("4294967296", piscina), &b));
        /* strtoul '-1' circumvolvit - nunc recusatur */
        CREDO_FALSUM(chorda_ut_i32(
            chorda_ex_literis("-1", piscina), &b));
    }

    /* ---- f64 exacta: iter reditus duplicis ---- */
    {
        f64 valores[III];
        f64 reditus;
        i32 i;

        imprimere("\n--- Probans chorda_ex_f64_exacta ---\n");

        valores[0] = 1e300;
        valores[1] = 1e-20;
        valores[2] = 0.1;

        per (i = ZEPHYRUM; i < III; i++)
        {
            CREDO_VERUM(chorda_ut_f64(
                chorda_ex_f64_exacta(valores[i], piscina), &reditus));
            CREDO_VERUM(reditus == valores[i]);
        }
        /* forma vetus NON fert: 1e300 sub '%.*f' perit */
        CREDO_FALSUM(chorda_ut_f64(
            chorda_ex_f64(1e300, VI, piscina), &reditus)
            && reditus == 1e300);
    }
```

- [ ] **Step 2: Run the tests and watch them fail**

```bash
./compile_tests.sh chorda > /tmp/t1.log 2>&1; echo "rc=$?"
grep -E 'error:|Fracti' /tmp/t1.log | head
```

Expected: a COMPILE failure naming `chorda_ut_s64`,
`chorda_ex_s64`, `chorda_ex_f64_exacta` as undeclared. A compile
failure is the honest first red here — the functions do not exist.

- [ ] **Step 3: Declare the three new functions**

In `include/chorda.h`, after `chorda_ex_f64` (`:268`):

```c
/* chorda_ut_s64 - Convertere chordam ad integrum LXIV bitorum
 *
 * Omnes cifrae, signum optionale, nihil aliud; zephyra ducentia
 * sine pondere. RECUSAT: NULLUM insertum (chorda terminatorem non
 * fert, ergo cstr eam truncaret), sordes post numerum, superfluitas
 * (limes lexice confertur - strtoll sub -std=c89 latet).
 *
 * Redde: VERUM si conversa
 */
b32
chorda_ut_s64 (
    chorda  s,
       s64* fructus);

/* chorda_ex_s64 - Convertere integrum LXIV bitorum ad chordam
 * Redde: Nova chorda (allocata ex piscina)
 */
chorda
chorda_ex_s64 (
         s64  numerus,
    Piscina* piscina);

/* chorda_ex_f64_exacta - Forma quae ITER REDITUS fert ('%.17g')
 *
 * chorda_ex_f64 praecisionem decimalem fixam scribit, ergo 1e300 et
 * 1e-20 per eam pereunt. Haec forma legi potest et eundem duplicem
 * reddere.
 */
chorda
chorda_ex_f64_exacta (
         f64  numerus,
    Piscina* piscina);
```

- [ ] **Step 4: Implement — the NUL guard and the three functions**

In `lib/chorda.c`, before `chorda_ut_s32` (`:998`):

```c
/* NULLUM insertum? Conversiones numericae per cstr transeunt, ubi
 * NULLUM prefixum tacite acciperet ('1\0garbage' -> 1). Custodia
 * UNA in limine, non in singulis vocationibus. */
interior b32
_chorda_nullum_habet (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        si (s.datum[i] == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}
```

Then append to `lib/chorda.c`:

```c
b32
chorda_ut_s64 (
    chorda  s,
       s64* fructus)
{
    constans character* limes;
                   s64  valor   = ZEPHYRUM;
                   i32  initium = ZEPHYRUM;
                   i32  cifrae;
                   i32  k;
                   b32  negans  = FALSUM;

    si (!s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }
    si (s.datum[ZEPHYRUM] == '+' || s.datum[ZEPHYRUM] == '-')
    {
        negans  = (s.datum[ZEPHYRUM] == '-') ? VERUM : FALSUM;
        initium = I;
    }
    si (initium >= s.mensura)
    {
        redde FALSUM;
    }
    per (k = initium; k < s.mensura; k++)
    {
        si (s.datum[k] < '0' || s.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    /* zephyra ducentia sine pondere (unum servatur) */
    dum (initium < s.mensura - I && s.datum[initium] == '0')
    {
        initium++;
    }
    cifrae = s.mensura - initium;
    si (cifrae > XIX)
    {
        redde FALSUM;
    }
    /* limes LEXICE confertur: arithmetica ipsa circumvolveret */
    limes = negans ? "9223372036854775808" : "9223372036854775807";
    si (cifrae == XIX)
    {
        per (k = ZEPHYRUM; k < XIX; k++)
        {
            si (s.datum[initium + k] > (i8)limes[k])
            {
                redde FALSUM;
            }
            si (s.datum[initium + k] < (i8)limes[k])
            {
                frange;
            }
        }
    }
    /* negativa NEGATIVE accumulantur: -9223372036854775808 positivum
     * esse non potest */
    per (k = initium; k < s.mensura; k++)
    {
        s64 cifra = (s64)(s.datum[k] - '0');

        valor = negans ? valor * X - cifra : valor * X + cifra;
    }
    *fructus = valor;
    redde VERUM;
}

chorda
chorda_ex_s64 (
         s64  numerus,
    Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XXIV];
    character  inversa[XXIV];
          i32  mensura = ZEPHYRUM;
          i32  k;
          i64  magnitudo;
          b32  negans;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    si (numerus < ZEPHYRUM)
    {
        negans     = VERUM;
        /* -(numerus + I) + I: extremum negativum negari non potest */
        magnitudo  = (i64)(-(numerus + I)) + I;
    }
    alioquin
    {
        negans     = FALSUM;
        magnitudo  = (i64)numerus;
    }
    fac
    {
        buffer[mensura] = (character)('0' + (integer)(magnitudo % X));
        mensura++;
        magnitudo = magnitudo / X;
    }
    dum (magnitudo != ZEPHYRUM);
    si (negans)
    {
        buffer[mensura] = '-';
        mensura++;
    }
    per (k = ZEPHYRUM; k < mensura; k++)
    {
        inversa[k] = buffer[mensura - I - k];
    }
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, inversa, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}

chorda
chorda_ex_f64_exacta (
         f64  numerus,
    Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XLVIII];
          s32  mensura_signed;
          i32  mensura;
           i8* allocatus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina)
    {
        redde fructus;
    }
    mensura_signed = snprintf(buffer, magnitudo(buffer), "%.17g",
                              numerus);
    si (mensura_signed < ZEPHYRUM
        || mensura_signed >= (s32)magnitudo(buffer))
    {
        redde fructus;
    }
    mensura   = (i32)mensura_signed;
    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, buffer, (memoriae_index)mensura);
    fructus.datum    = allocatus;
    fructus.mensura  = mensura;
    redde fructus;
}
```

- [ ] **Step 5: Implement — the guards inside the three old functions**

In `chorda_ut_s32` (`lib/chorda.c:998`), extend the first guard and add
a range check. Anchor the edit at the `si (!s.datum || !fructus ||
s.mensura == ZEPHYRUM)` line and at the acceptance branch:

```c
    si (!s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }
```

```c
    si (terminus != cstr_temporalis && *terminus == '\0'
        && longus_valor <= 2147483647L
        && longus_valor >= -2147483647L - 1L)
    {
        *fructus = (s32)longus_valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }
```

In `chorda_ut_i32` (`:1034`) the same first-guard change, plus: reject
a leading `-` (because `strtoul` wraps it silently) and bound the
value.

```c
    si (!s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s) || s.datum[ZEPHYRUM] == '-')
    {
        redde FALSUM;
    }
```

```c
    si (terminus != cstr_temporalis && *terminus == '\0'
        && valor <= 4294967295UL)
    {
        *fructus = (i32)valor;
        liberare(cstr_temporalis);
        redde VERUM;
    }
```

In `chorda_ut_f64` (`:1613`) only the first-guard change (a double has
no integral range to police):

```c
    si (!s.datum || !fructus || s.mensura == ZEPHYRUM
        || _chorda_nullum_habet(s))
    {
        redde FALSUM;
    }
```

- [ ] **Step 6: Run the tests and watch them pass**

```bash
./compile_tests.sh chorda > /tmp/t1.log 2>&1; echo "rc=$?"
grep -E 'Totalis|Praeteriti|Fracti|Conditio' /tmp/t1.log
```

Expected: `rc=0`, `Fracti: 0`, and the assertion total risen by the
number of new asserts.

- [ ] **Step 7: Prove the gate can go red (planted fault)**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
p = silva.planta('lib/chorda.c',
                 '        || _chorda_nullum_habet(s))',
                 '        || FALSUM)',
                 'radix', 'chorda')
print(p)
PY
```

Expected: RED with the planted fault (the NUL cases fail), GREEN after
the automatic revert. `porta muta` means the tests cannot see the
guard — fix the test, not the plant.

- [ ] **Step 8: Format, note the finding, commit**

```bash
./silva/formator.sh include/chorda.h lib/chorda.c \
    probationes/probatio_chorda.c -scribere
./silva/formator.sh include/chorda.h lib/chorda.c \
    probationes/probatio_chorda.c -vitia
```

Append to `lib/chorda.worklog.md` a dated note: the whole `chorda_ut_*`
family accepted NUL-truncated input (`1\0garbage` → 1) because
`*terminus == '\0'` is true AT an embedded NUL; `ut_s32` truncated via
an unchecked `long` cast and `ut_i32` accepted `-1` as 4294967295 via
`strtoul`; found while designing plist, fixed as a family; `s64`
parsing is hand-rolled because `-std=c89` hides `strtoll`.

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'chorda: par s64, forma f64 exacta, recusationes familiae ut_* '
    '(NULLUM insertum et superfluitas) - lacunae a consilio plist '
    'inventae (plist-spec par. VI.2-VI.4)',
    ['include/chorda.h', 'lib/chorda.c', 'probationes/probatio_chorda.c',
     'lib/chorda.worklog.md'],
    portae=[('radix', 'chorda')]))
PY
```

---

### Task 2: `xml` — skip `<!DOCTYPE …>`, refuse an internal subset

Implements spec §6.1. Without it, G5 (reading an Apple-written plist)
cannot run at all.

**Files:**
- Modify: `include/xml.h` (`XmlStatus` enum, `:41`)
- Modify: `lib/xml.c` (insert before `/* Elementum radix */` at `:1526`)
- Modify: `probationes/probatio_xml.c` (before `:634`)

**Interfaces:**
- Consumes: nothing.
- Produces: `xml_legere` accepts a document whose prolog carries a
  doctype declaration; `XML_ERROR_SUBSETUM_INTERNUM` for one with an
  internal subset.

**Where it goes:** a doctype is legal ONLY before the root element, so
only the prolog loop in `xml_legere` needs it — the second declaration
skip (`lib/xml.c:1172`, inside element parsing) is left alone
deliberately.

- [ ] **Step 1: Write the failing tests**

Insert before `credo_imprimere_compendium();` in
`probationes/probatio_xml.c`:

```c
    /* ---- DOCTYPE: plists eam semper ferunt ---- */
    {
        XmlResultus r;
        constans character* cum_doctype =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict/></plist>";
        constans character* cum_subseto =
            "<!DOCTYPE x [ <!ENTITY a \"b\"> ]>\n<x/>";

        imprimere("\n--- Probans DOCTYPE in prologo ---\n");

        r = xml_legere_ex_literis(cum_doctype, piscina, intern);
        CREDO_VERUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)XML_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);
        CREDO_CHORDA_AEQUALIS_LITERIS(*r.radix->titulus, "plist");

        /* subsetum internum: ad '>' primum saltare documentum
         * frangeret, ergo RECUSATUR nominatim */
        r = xml_legere_ex_literis(cum_subseto, piscina, intern);
        CREDO_FALSUM(r.successus);
        CREDO_AEQUALIS_I32((i32)r.status,
                           (i32)XML_ERROR_SUBSETUM_INTERNUM);
    }
```

If `probatio_xml.c` names its interner differently, use that name; the
file already creates one for `xml_legere_ex_literis`.

- [ ] **Step 2: Run and watch it fail**

```bash
./compile_tests.sh xml > /tmp/t2.log 2>&1; echo "rc=$?"
grep -E 'error:|FRACTA|Fracti' /tmp/t2.log | head
```

Expected: compile failure on `XML_ERROR_SUBSETUM_INTERNUM`.

- [ ] **Step 3: Append the status (never reorder an enum)**

In `include/xml.h`, as the LAST member of `XmlStatus`:

```c
    XML_ERROR_SUBSETUM_INTERNUM  /* <!DOCTYPE … [ … ]> - non tractatum */
```

- [ ] **Step 4: Implement the skip**

In `lib/xml.c`, insert between the comment branch (ends `:1524`) and
`/* Elementum radix */ frange;` (`:1526`):

```c
            /* Verificare si declaratio generis (<!DOCTYPE …>).
             * Plists eam semper ferunt; subsetum internum ('[')
             * RECUSATUR, quia ad '>' primum saltare corpus eius
             * documenti frangeret. */
            si (   ctx.positus + VIII < ctx.input.mensura
                && ctx.input.datum[ctx.positus + I]    == '!'
                && ctx.input.datum[ctx.positus + II]   == 'D'
                && ctx.input.datum[ctx.positus + III]  == 'O'
                && ctx.input.datum[ctx.positus + IV]   == 'C'
                && ctx.input.datum[ctx.positus + V]    == 'T'
                && ctx.input.datum[ctx.positus + VI]   == 'Y'
                && ctx.input.datum[ctx.positus + VII]  == 'P'
                && ctx.input.datum[ctx.positus + VIII] == 'E')
            {
                ctx.positus += IX;
                ctx.columna += IX;

                dum (ctx.positus < ctx.input.mensura)
                {
                    character d;

                    d = (character)ctx.input.datum[ctx.positus];
                    si (d == '[')
                    {
                        resultus.status           =
                            XML_ERROR_SUBSETUM_INTERNUM;
                        resultus.linea_erroris    = ctx.linea;
                        resultus.columna_erroris  = ctx.columna;
                        redde resultus;
                    }
                    si (d == '>')
                    {
                        ctx.positus++;
                        ctx.columna++;
                        frange;
                    }
                    si (d == '\n')
                    {
                        ctx.linea++;
                        ctx.columna = I;
                    }
                    alioquin
                    {
                        ctx.columna++;
                    }
                    ctx.positus++;
                }

                perge;
            }
```

- [ ] **Step 5: Run and watch it pass**

```bash
./compile_tests.sh xml > /tmp/t2.log 2>&1; echo "rc=$?"
grep -E 'Totalis|Praeteriti|Fracti|Conditio' /tmp/t2.log
```

- [ ] **Step 6: Prove the gate can go red**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.planta('lib/xml.c',
                   "                    si (d == '[')",
                   "                    si (FALSUM)",
                   'radix', 'xml'))
PY
```

Expected RED on the internal-subset assertion, GREEN after revert.

- [ ] **Step 7: Format and commit**

```bash
./silva/formator.sh include/xml.h lib/xml.c \
    probationes/probatio_xml.c -scribere
./silva/formator.sh include/xml.h lib/xml.c \
    probationes/probatio_xml.c -vitia
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'xml: DOCTYPE in prologo saltatur, subsetum internum RECUSATUR '
    '(XML_ERROR_SUBSETUM_INTERNUM appensum) - sine hoc Info.plist '
    'verum parsari non potest (plist-spec par. VI.1)',
    ['include/xml.h', 'lib/xml.c', 'probationes/probatio_xml.c'],
    portae=[('radix', 'xml')]))
PY
```

---

### Task 3: `fasti` — the ISO-8601 datetime pair

Implements spec §6.5. `<date>` types on it.

**Files:**
- Modify: `include/fasti.h` (after `fasti_parsare_diem`, `:423`)
- Modify: `lib/fasti.c` (beside `fasti_parsare_iso`, `:963-1037`)
- Modify: `probationes/probatio_fasti.c` (before `:764`)

**Interfaces:**
- Consumes: nothing.
- Produces: `chorda fasti_ad_iso(DiesHora, Piscina*)` →
  `YYYY-MM-DDTHH:MM:SSZ` (20 bytes); `b32 fasti_ex_iso(chorda,
  DiesHora*)`, which accepts `Z` or a `±HH:MM` offset and normalizes to
  UTC. `DiesHora` is `{ Dies dies; Hora hora; }`
  (`include/fasti.h:109`); `Dies` is `{annus, mensis, dies}`, `Hora` is
  `{hora, minutum, secundum}`.

- [ ] **Step 1: Write the failing tests**

Insert before `credo_imprimere_compendium();` in
`probationes/probatio_fasti.c`:

```c
    /* ---- ISO-8601 plenum: dies + hora, UTC ---- */
    {
        DiesHora dh;
        DiesHora reditus;
        chorda   s;

        imprimere("\n--- Probans fasti_ad_iso / fasti_ex_iso ---\n");

        dh.dies  = fasti_dies(2026, 9, 11);
        dh.hora  = fasti_hora(2, 41, 32);
        s        = fasti_ad_iso(dh, piscina);
        CREDO_CHORDA_AEQUALIS_LITERIS(s, "2026-09-11T02:41:32Z");

        CREDO_VERUM(fasti_ex_iso(s, &reditus));
        CREDO_AEQUALIS_S32(reditus.dies.annus, 2026);
        CREDO_AEQUALIS_S32(reditus.dies.mensis, 9);
        CREDO_AEQUALIS_S32(reditus.dies.dies, 11);
        CREDO_AEQUALIS_S32(reditus.hora.hora, 2);
        CREDO_AEQUALIS_S32(reditus.hora.minutum, 41);
        CREDO_AEQUALIS_S32(reditus.hora.secundum, 32);

        /* offsetum ad UTC vertitur */
        CREDO_VERUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11T04:41:32+02:00", piscina),
            &reditus));
        CREDO_AEQUALIS_S32(reditus.hora.hora, 2);
        CREDO_AEQUALIS_S32(reditus.dies.dies, 11);

        /* offsetum trans medium noctis diem mutat */
        CREDO_VERUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11T00:30:00+02:00", piscina),
            &reditus));
        CREDO_AEQUALIS_S32(reditus.dies.dies, 10);
        CREDO_AEQUALIS_S32(reditus.hora.hora, 22);

        /* recusationes nominatae */
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11", piscina), &reditus));
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11 02:41:32Z", piscina),
            &reditus));
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11T02:41:32", piscina),
            &reditus));
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-13-11T02:41:32Z", piscina),
            &reditus));
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11T25:41:32Z", piscina),
            &reditus));
        /* fractiones secundorum: v1 RECUSAT nominatim */
        CREDO_FALSUM(fasti_ex_iso(
            chorda_ex_literis("2026-09-11T02:41:32.5Z", piscina),
            &reditus));
    }
```

- [ ] **Step 2: Run and watch it fail**

```bash
./compile_tests.sh fasti > /tmp/t3.log 2>&1; echo "rc=$?"
grep -E 'error:|Fracti' /tmp/t3.log | head
```

- [ ] **Step 3: Declare the pair**

In `include/fasti.h`, after `fasti_parsare_diem` (`:423`):

```c
/* Formare instans ISO-8601 UTC: "YYYY-MM-DDTHH:MM:SSZ" (XX octeti) */
chorda
fasti_ad_iso (
    DiesHora  dh,
    Piscina* piscina);

/* Parsare instans ISO-8601: 'Z' aut offsetum '±HH:MM' (ad UTC
 * versum). Fractiones secundorum RECUSANTUR (v1).
 * Redde: VERUM si parsatum et validum
 */
b32
fasti_ex_iso (
      chorda  s,
    DiesHora* fructus);
```

- [ ] **Step 4: Implement**

Append to `lib/fasti.c` (`_parsare_duo_digiti` already exists in this
file and is reused):

```c
chorda
fasti_ad_iso (
    DiesHora  dh,
    Piscina* piscina)
{
       chorda  fructus;
    character  buffer[XXI];
           i8* allocatus;
          s32  annus;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    si (!piscina || !fasti_dies_valida(dh.dies)
        || !fasti_hora_valida(dh.hora) || dh.dies.annus < ZEPHYRUM
        || dh.dies.annus > 9999)
    {
        redde fructus;
    }
    annus = dh.dies.annus;
    buffer[0]   = (character)('0' + annus / 1000);
    buffer[1]   = (character)('0' + (annus / 100) % X);
    buffer[2]   = (character)('0' + (annus / X) % X);
    buffer[3]   = (character)('0' + annus % X);
    buffer[4]   = '-';
    buffer[5]   = (character)('0' + dh.dies.mensis / X);
    buffer[6]   = (character)('0' + dh.dies.mensis % X);
    buffer[7]   = '-';
    buffer[8]   = (character)('0' + dh.dies.dies / X);
    buffer[9]   = (character)('0' + dh.dies.dies % X);
    buffer[10]  = 'T';
    buffer[11]  = (character)('0' + dh.hora.hora / X);
    buffer[12]  = (character)('0' + dh.hora.hora % X);
    buffer[13]  = ':';
    buffer[14]  = (character)('0' + dh.hora.minutum / X);
    buffer[15]  = (character)('0' + dh.hora.minutum % X);
    buffer[16]  = ':';
    buffer[17]  = (character)('0' + dh.hora.secundum / X);
    buffer[18]  = (character)('0' + dh.hora.secundum % X);
    buffer[19]  = 'Z';

    allocatus = (i8*)piscina_allocare(piscina, (memoriae_index)XX);
    si (!allocatus)
    {
        redde fructus;
    }
    memcpy(allocatus, buffer, (memoriae_index)XX);
    fructus.datum    = allocatus;
    fructus.mensura  = XX;
    redde fructus;
}

b32
fasti_ex_iso (
      chorda  s,
    DiesHora* fructus)
{
        Dies  dies;
        s32   hora;
        s32   minutum;
        s32   secundum;
        s32   offsetum = ZEPHYRUM;
        b32   offsetum_negans;
        s32   off_horae;
        s32   off_minuta;
    MomentumSec  instans;

    si (!s.datum || !fructus || s.mensura < XX)
    {
        redde FALSUM;
    }
    /* dies: pars prima, parsatore exsistente */
    si (!fasti_parsare_iso(chorda_sectio(s, ZEPHYRUM, X), &dies))
    {
        redde FALSUM;
    }
    si (s.datum[X] != 'T')
    {
        redde FALSUM;
    }
    si (!_parsare_duo_digiti(s, XI, &hora) || s.datum[XIII] != ':'
        || !_parsare_duo_digiti(s, XIV, &minutum)
        || s.datum[XVI] != ':'
        || !_parsare_duo_digiti(s, XVII, &secundum))
    {
        redde FALSUM;
    }
    /* zona: 'Z' aut '±HH:MM'; nihil aliud (fractiones recusatae) */
    si (s.datum[XIX] == 'Z')
    {
        si (s.mensura != XX)
        {
            redde FALSUM;
        }
    }
    alioquin si (s.datum[XIX] == '+' || s.datum[XIX] == '-')
    {
        si (s.mensura != XXV || s.datum[XXII] != ':')
        {
            redde FALSUM;
        }
        offsetum_negans = (s.datum[XIX] == '-') ? VERUM : FALSUM;
        si (!_parsare_duo_digiti(s, XX, &off_horae)
            || !_parsare_duo_digiti(s, XXIII, &off_minuta))
        {
            redde FALSUM;
        }
        si (off_horae > XIV || off_minuta > LIX)
        {
            redde FALSUM;
        }
        offsetum = off_horae * 3600 + off_minuta * 60;
        si (!offsetum_negans)
        {
            offsetum = -offsetum;
        }
    }
    alioquin
    {
        redde FALSUM;
    }

    fructus->dies  = dies;
    fructus->hora  = fasti_hora(hora, minutum, secundum);
    si (!fasti_dies_valida(fructus->dies)
        || !fasti_hora_valida(fructus->hora))
    {
        redde FALSUM;
    }
    si (offsetum != ZEPHYRUM)
    {
        instans  = fasti_ad_unix(*fructus) + (MomentumSec)offsetum;
        *fructus = fasti_ex_unix(instans);
    }
    redde VERUM;
}
```

- [ ] **Step 5: Run and watch it pass**

```bash
./compile_tests.sh fasti > /tmp/t3.log 2>&1; echo "rc=$?"
grep -E 'Totalis|Praeteriti|Fracti|Conditio' /tmp/t3.log
```

- [ ] **Step 6: Prove the gate can go red**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.planta('lib/fasti.c',
                   '        si (!offsetum_negans)',
                   '        si (FALSUM)',
                   'radix', 'fasti'))
PY
```

Expected RED on the two offset assertions (the sign inverts), GREEN
after revert.

- [ ] **Step 7: Format and commit**

```bash
./silva/formator.sh include/fasti.h lib/fasti.c \
    probationes/probatio_fasti.c -scribere
./silva/formator.sh include/fasti.h lib/fasti.c \
    probationes/probatio_fasti.c -vitia
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'fasti: par ISO-8601 plenum (fasti_ad_iso / fasti_ex_iso, '
    'offsetum ad UTC versum, fractiones recusatae) iuxta '
    'fasti_ad_unix / fasti_ex_unix - plist <date> ei innititur '
    '(plist-spec par. VI.5)',
    ['include/fasti.h', 'lib/fasti.c', 'probationes/probatio_fasti.c'],
    portae=[('radix', 'fasti')]))
PY
```

---

### Task 4: `plist` — the value model and the writer

Implements spec §3, §5; gates G1 and G3. Writing first, because it has
no unknowns and gives the reader something real to read back.

**Files:**
- Create: `include/plist.h`, `lib/plist.c`,
  `probationes/probatio_plist.c`
- Modify: `oratio/glossarium.stml` (one line for the word `plist`)
- Run: `./tools/compile_tests_fontes_generare.sh`

**Interfaces:**
- Consumes: Task 1's `chorda_ex_s64` and `chorda_ex_f64_exacta`; Task
  3's `fasti_ad_iso`.
- Produces: `PlistGenus`, `PlistStatus`, `PlistValor`, `PlistPar`, the
  eight constructors, `plist_dictio_ponere`, `plist_lista_addere`,
  `plist_dictio_capere`, `plist_numerus`, `plist_ad_indicem`,
  `plist_aequalis`, `chorda plist_scribere(PlistValor*, PlistStatus*,
  Piscina*)`, `b32 plist_scribere_plagulam(PlistValor*, constans
  character*, PlistStatus*, Piscina*)`.

**Spec amendment made here:** `plist_scribere` gains a `PlistStatus*`
out-param — §6.7's control-character refusal needs to name its cause,
and an empty `chorda` cannot.

- [ ] **Step 1: Write `include/plist.h`**

```bash
./silva/scribe.sh include/plist.h <<'EOF'
/* plist.h - Tabulae proprietatum (XML property lists): legere,
 * scribere, conferre.
 *
 * Grammatica plist SOLA hic vivit; stratum XML infra iam escapat et
 * entitates nominatas decodificat (lib/xml.c:564-615, :898-950), ergo
 * haec bibliotheca arborem XmlNodus struit ut scribat et eam ambulat
 * ut legat. Prologus et DOCTYPE duae lineae literales sunt.
 *
 * Consumptores: fasciculum (Info.plist), postea iura et LaunchAgentes.
 * Specificatio: project-specs/plist-spec.md v1.
 *
 * NON tractat: plists binarias (bplist00), validationem DTD,
 * versionem praeter 1.0 - singula RECUSANTUR nominatim.
 */

#ifndef PLIST_H
#define PLIST_H

#include "latina.h"
#include "chorda.h"
#include "piscina.h"
#include "internamentum.h"
#include "fasti.h"
#include "xar.h"

nomen enumeratio {
    PLIST_DICTIO,
    PLIST_LISTA,
    PLIST_CHORDA,
    PLIST_VERITAS,
    PLIST_INTEGER,
    PLIST_REALIS,
    PLIST_DIES,
    PLIST_DATA
} PlistGenus;

nomen enumeratio {
    PLIST_SUCCESSUS = ZEPHYRUM,
    PLIST_ERROR_XML,
    PLIST_ERROR_NON_PLIST,
    PLIST_ERROR_VERSIO,
    PLIST_ERROR_BINARIUM,
    PLIST_ERROR_STRUCTURA,
    PLIST_ERROR_BASE64,
    PLIST_ERROR_NUMERUS,
    PLIST_ERROR_DIES,
    PLIST_ERROR_SIGNUM,
    PLIST_ERROR_MEMORIA
} PlistStatus;

nomen structura PlistValor PlistValor;

/* Par clavis-valor dictionis; ordo SERVATUR (vide par. III specs) */
nomen structura {
       chorda  clavis;
    PlistValor* valor;
} PlistPar;

structura PlistValor {
    PlistGenus  genus;
        chorda  textus;    /* CHORDA: textus; DATA: octeti decodificati */
      DiesHora  dies;      /* DIES */
           s64  integer;   /* INTEGER */
           f64  realis;    /* REALIS */
           b32  veritas;   /* VERITAS */
          Xar*  liberi;    /* DICTIO: PlistPar; LISTA: PlistValor* */
};

/* ---- structores, unus per genus ---- */

PlistValor*
plist_dictio_creare (
    Piscina* piscina);

PlistValor*
plist_lista_creare (
    Piscina* piscina);

PlistValor*
plist_chordam_creare (
      chorda  textus,
    Piscina* piscina);

PlistValor*
plist_veritatem_creare (
         b32  valor,
    Piscina* piscina);

PlistValor*
plist_integrum_creare (
         s64  numerus,
    Piscina* piscina);

PlistValor*
plist_realem_creare (
         f64  numerus,
    Piscina* piscina);

PlistValor*
plist_diem_creare (
    DiesHora  dh,
    Piscina* piscina);

PlistValor*
plist_data_creare (
      chorda  octeti,
    Piscina* piscina);

/* ---- aditus ---- */

/* Ponere par: clavem exsistentem SUBSTITUIT in loco, aliter
 * APPENDIT - ergo editio plist ordinem numquam permutat. */
b32
plist_dictio_ponere (
    PlistValor*  dictio,
         chorda  clavis,
    PlistValor*  valor,
       Piscina*  piscina);

b32
plist_lista_addere (
    PlistValor*  lista,
    PlistValor*  valor,
       Piscina*  piscina);

/* Clavis ut literae C: lectio fere semper literalis est in sede
 * vocationis. Geminus _chorda in PULL manet, non ante. */
PlistValor*
plist_dictio_capere (
          PlistValor*  dictio,
    constans character*  clavis);

/* Numerus parium (dictio) aut elementorum (lista); ZEPHYRUM pro
 * genere scalari. */
i32
plist_numerus (
    PlistValor* valor);

PlistValor*
plist_ad_indicem (
    PlistValor*  lista,
            i32  index);

/* Aequalitas VALORUM (non octetorum): genus, numerus, ordo, et
 * contenta. Fasciculum ea utitur ut plist iam rectam non rescribat. */
b32
plist_aequalis (
    PlistValor* a,
    PlistValor* b);

/* ---- scribere ---- */

/* Purum: nihil in disco. Redde chordam vacuam si status != SUCCESSUS.
 * Emittit prologum, DOCTYPE Apple, <plist version="1.0">, arborem.
 * RECUSAT signum C0 in <string> (PLIST_ERROR_SIGNUM): stratum
 * escapandi nostrum id exprimere non potest, et crudum XML invalidum
 * pareret (par. VI.7 specs). */
chorda
plist_scribere (
    PlistValor*  radix,
    PlistStatus* status,
       Piscina*  piscina);

b32
plist_scribere_plagulam (
          PlistValor*  radix,
    constans character*  via,
         PlistStatus*  status,
            Piscina*  piscina);

#endif /* PLIST_H */
EOF
```

- [ ] **Step 2: Write the failing tests (G1 + G3)**

```bash
./silva/scribe.sh probationes/probatio_plist.c <<'EOF'
/* probatio_plist.c - modellum valoris, scriptor, lector, iter reditus
 *
 * Portae G1-G5 et G7 specificationis (project-specs/plist-spec.md
 * par. VII). Oraculum alienum (plutil) in
 * probationes/probatio_plist_plutil.sh vivit - G6.
 */

#include "latina.h"
#include "plist.h"
#include "credo.h"
#include "piscina.h"
#include "internamentum.h"

#include <stdio.h>
#include <string.h>

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_plist", 1048576);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    intern = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* ---- G1: ordo dictionis servatus, ponere substituit ---- */
    {
        PlistValor* d;
        PlistPar*   par;

        imprimere("\n--- G1: modellum valoris ---\n");

        d = plist_dictio_creare(piscina);
        CREDO_NON_NIHIL(d);
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("alpha", piscina),
            plist_integrum_creare(1, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("beta", piscina),
            plist_integrum_creare(2, piscina), piscina));
        CREDO_AEQUALIS_I32(plist_numerus(d), II);

        /* clavis exsistens SUBSTITUITUR in loco: numerus idem,
         * ordo idem, valor novus */
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("alpha", piscina),
            plist_integrum_creare(3, piscina), piscina));
        CREDO_AEQUALIS_I32(plist_numerus(d), II);
        par = (PlistPar*)xar_obtinere(d->liberi, ZEPHYRUM);
        CREDO_NON_NIHIL(par);
        CREDO_CHORDA_AEQUALIS_LITERIS(par->clavis, "alpha");
        CREDO_AEQUALIS_S64(par->valor->integer, 3);

        CREDO_NON_NIHIL(plist_dictio_capere(d, "beta"));
        CREDO_NIHIL(plist_dictio_capere(d, "gamma"));
        CREDO_AEQUALIS_I32(
            plist_numerus(plist_chordam_creare(
                chorda_ex_literis("x", piscina), piscina)),
            ZEPHYRUM);
    }

    /* ---- G3: scriptor - octeti, determinismus, genera omnia ---- */
    {
        PlistValor* d;
        PlistValor* lista;
        PlistStatus status;
        chorda      prima;
        chorda      secunda;
        DiesHora    dh;
        i8          octeti[III];

        imprimere("\n--- G3: scriptor ---\n");

        dh.dies       = fasti_dies(2026, 9, 11);
        dh.hora       = fasti_hora(2, 41, 32);
        octeti[0]     = (i8)0x00;
        octeti[1]     = (i8)0x01;
        octeti[2]     = (i8)0xFF;

        lista = plist_lista_creare(piscina);
        CREDO_VERUM(plist_lista_addere(lista,
            plist_veritatem_creare(VERUM, piscina), piscina));
        CREDO_VERUM(plist_lista_addere(lista,
            plist_veritatem_creare(FALSUM, piscina), piscina));

        d = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Titulus", piscina),
            plist_chordam_creare(
                chorda_ex_literis("a < b & c", piscina), piscina),
            piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Numerus", piscina),
            plist_integrum_creare(-9223372036854775807LL - 1LL,
                                  piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Realis", piscina),
            plist_realem_creare(1e300, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Dies", piscina),
            plist_diem_creare(dh, piscina), piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Data", piscina),
            plist_data_creare(chorda_ex_buffer(octeti, III), piscina),
            piscina));
        CREDO_VERUM(plist_dictio_ponere(d,
            chorda_ex_literis("Lista", piscina), lista, piscina));

        prima = plist_scribere(d, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        CREDO_CHORDA_NON_VACUA(prima);

        /* prologus et DOCTYPE ASSERUNTUR: plutil utrumque ignoscit */
        CREDO_CHORDA_INCIPIT(prima, chorda_ex_literis(
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n",
            piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<plist version=\"1.0\">", piscina));
        /* escapatio a strato XML */
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("a &lt; b &amp; c", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<integer>-9223372036854775808</integer>",
                              piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<date>2026-09-11T02:41:32Z</date>",
                              piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<data>AAH/</data>", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<true/>", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<false/>", piscina));
        /* realis: forma exacta, non decimalis fixa */
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("<real>1", piscina));
        CREDO_CHORDA_CONTINET(prima,
            chorda_ex_literis("e+300</real>", piscina));

        /* DETERMINISMUS: idem valor, idem octeti */
        secunda = plist_scribere(d, &status, piscina);
        CREDO_CHORDA_AEQUALIS(prima, secunda);

        /* G7 pars: signum C0 in <string> RECUSATUR */
        {
            PlistValor* malus;
            i8          cum_signo[II];
            chorda      nihil_scriptum;

            cum_signo[0]  = (i8)'a';
            cum_signo[1]  = (i8)0x01;
            malus = plist_dictio_creare(piscina);
            CREDO_VERUM(plist_dictio_ponere(malus,
                chorda_ex_literis("x", piscina),
                plist_chordam_creare(
                    chorda_ex_buffer(cum_signo, II), piscina),
                piscina));
            nihil_scriptum = plist_scribere(malus, &status, piscina);
            CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_ERROR_SIGNUM);
            CREDO_CHORDA_VACUA(nihil_scriptum);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
EOF
```

- [ ] **Step 3: Wire the suite and watch the tests fail**

```bash
./tools/compile_tests_fontes_generare.sh
./compile_tests.sh plist > /tmp/t4.log 2>&1; echo "rc=$?"
grep -E 'error:|Undefined|Fracti' /tmp/t4.log | head
```

Expected: link or compile failure — `lib/plist.c` does not exist yet.
`rc=2` here would mean the filter matched nothing: check the generator
ran and `probationes/probatio_plist.c` is in `SOURCE_FILES`.

- [ ] **Step 4: Implement `lib/plist.c` — constructors and access**

```bash
./silva/scribe.sh lib/plist.c <<'EOF'
/* plist.c - tabulae proprietatum (contractus in capite).
 *
 * Scriptor arborem XmlNodus struit et xml_scribere vocat, ergo
 * escapatio uno loco vivit (strato XML). Lector eandem arborem
 * ambulat. Prologus et DOCTYPE hic praefiguntur.
 */

#include "plist.h"
#include "xml.h"
#include "base64.h"
#include "chorda_aedificator.h"
#include <string.h>

hic_manens constans character PLIST_PROLOGUS[] =
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
    "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n";

interior PlistValor*
_valorem_creare (
    PlistGenus  genus,
      Piscina* piscina)
{
    PlistValor* v;

    si (!piscina)
    {
        redde NIHIL;
    }
    v = (PlistValor*)piscina_allocare(piscina, magnitudo(PlistValor));
    si (!v)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(PlistValor));
    v->genus = genus;
    redde v;
}

PlistValor*
plist_dictio_creare (
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DICTIO, piscina);

    si (v)
    {
        v->liberi = xar_creare(piscina, (i32)magnitudo(PlistPar));
    }
    redde v;
}

PlistValor*
plist_lista_creare (
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_LISTA, piscina);

    si (v)
    {
        v->liberi = xar_creare(piscina, (i32)magnitudo(PlistValor*));
    }
    redde v;
}

PlistValor*
plist_chordam_creare (
      chorda  textus,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_CHORDA, piscina);

    si (v)
    {
        v->textus = textus;
    }
    redde v;
}

PlistValor*
plist_veritatem_creare (
         b32  valor,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_VERITAS, piscina);

    si (v)
    {
        v->veritas = valor;
    }
    redde v;
}

PlistValor*
plist_integrum_creare (
         s64  numerus,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_INTEGER, piscina);

    si (v)
    {
        v->integer = numerus;
    }
    redde v;
}

PlistValor*
plist_realem_creare (
         f64  numerus,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_REALIS, piscina);

    si (v)
    {
        v->realis = numerus;
    }
    redde v;
}

PlistValor*
plist_diem_creare (
    DiesHora  dh,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DIES, piscina);

    si (v)
    {
        v->dies = dh;
    }
    redde v;
}

PlistValor*
plist_data_creare (
      chorda  octeti,
    Piscina* piscina)
{
    PlistValor* v = _valorem_creare(PLIST_DATA, piscina);

    si (v)
    {
        v->textus = octeti;
    }
    redde v;
}

b32
plist_dictio_ponere (
    PlistValor*  dictio,
         chorda  clavis,
    PlistValor*  valor,
       Piscina*  piscina)
{
    PlistPar* par;
         i32  i;

    (vacuum)piscina;
    si (!dictio || dictio->genus != PLIST_DICTIO || !dictio->liberi
        || !valor)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(dictio->liberi); i++)
    {
        par = (PlistPar*)xar_obtinere(dictio->liberi, i);
        si (par && chorda_aequalis(par->clavis, clavis))
        {
            par->valor = valor;   /* IN LOCO: ordo servatur */
            redde VERUM;
        }
    }
    par = (PlistPar*)xar_addere(dictio->liberi);
    si (!par)
    {
        redde FALSUM;
    }
    par->clavis  = clavis;
    par->valor   = valor;
    redde VERUM;
}

b32
plist_lista_addere (
    PlistValor*  lista,
    PlistValor*  valor,
       Piscina*  piscina)
{
    PlistValor** sedes;

    (vacuum)piscina;
    si (!lista || lista->genus != PLIST_LISTA || !lista->liberi
        || !valor)
    {
        redde FALSUM;
    }
    sedes = (PlistValor**)xar_addere(lista->liberi);
    si (!sedes)
    {
        redde FALSUM;
    }
    *sedes = valor;
    redde VERUM;
}

PlistValor*
plist_dictio_capere (
          PlistValor*  dictio,
    constans character*  clavis)
{
    PlistPar* par;
         i32  i;

    si (!dictio || dictio->genus != PLIST_DICTIO || !dictio->liberi
        || !clavis)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(dictio->liberi); i++)
    {
        par = (PlistPar*)xar_obtinere(dictio->liberi, i);
        si (par && chorda_aequalis_literis(par->clavis, clavis))
        {
            redde par->valor;
        }
    }
    redde NIHIL;
}

i32
plist_numerus (
    PlistValor* valor)
{
    si (!valor || !valor->liberi
        || (valor->genus != PLIST_DICTIO && valor->genus != PLIST_LISTA))
    {
        redde ZEPHYRUM;
    }
    redde xar_numerus(valor->liberi);
}

PlistValor*
plist_ad_indicem (
    PlistValor*  lista,
            i32  index)
{
    PlistValor** sedes;

    si (!lista || lista->genus != PLIST_LISTA || !lista->liberi)
    {
        redde NIHIL;
    }
    sedes = (PlistValor**)xar_obtinere(lista->liberi, index);
    redde sedes ? *sedes : NIHIL;
}

b32
plist_aequalis (
    PlistValor* a,
    PlistValor* b)
{
    i32 i;

    si (!a || !b || a->genus != b->genus)
    {
        redde FALSUM;
    }
    commutatio (a->genus)
    {
        casus PLIST_CHORDA:
        casus PLIST_DATA:
            redde chorda_aequalis(a->textus, b->textus);
        casus PLIST_VERITAS:
            redde (a->veritas ? VERUM : FALSUM)
                == (b->veritas ? VERUM : FALSUM);
        casus PLIST_INTEGER:
            redde a->integer == b->integer ? VERUM : FALSUM;
        casus PLIST_REALIS:
            redde a->realis == b->realis ? VERUM : FALSUM;
        casus PLIST_DIES:
            redde (fasti_aequalis(a->dies.dies, b->dies.dies)
                   && a->dies.hora.hora == b->dies.hora.hora
                   && a->dies.hora.minutum == b->dies.hora.minutum
                   && a->dies.hora.secundum == b->dies.hora.secundum)
                ? VERUM : FALSUM;
        casus PLIST_LISTA:
            si (plist_numerus(a) != plist_numerus(b))
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < plist_numerus(a); i++)
            {
                si (!plist_aequalis(plist_ad_indicem(a, i),
                                    plist_ad_indicem(b, i)))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        casus PLIST_DICTIO:
            si (plist_numerus(a) != plist_numerus(b))
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < plist_numerus(a); i++)
            {
                PlistPar* pa;
                PlistPar* pb;

                pa = (PlistPar*)xar_obtinere(a->liberi, i);
                pb = (PlistPar*)xar_obtinere(b->liberi, i);
                si (!pa || !pb
                    || !chorda_aequalis(pa->clavis, pb->clavis)
                    || !plist_aequalis(pa->valor, pb->valor))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}
EOF
```

- [ ] **Step 5: Implement the writer (same file, appended)**

Append to `lib/plist.c` with `silva.Editio('lib/plist.c')` or the Edit
tool. `_signum_vetitum` is the §6.7 refusal: XML 1.0 permits only tab,
newline and carriage return below 0x20.

```c
interior b32
_signum_vetitum (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (c < (i8)0x20 && c != (i8)'\t' && c != (i8)'\n'
            && c != (i8)'\r')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* valorem in nodum XML vertere; status ponitur si recusatur */
interior XmlNodus*
_nodum_ex_valore (
             PlistValor*  v,
                Piscina*  piscina,
    InternamentumChorda*  intern,
            PlistStatus*  status)
{
    XmlNodus* nodus;
         i32  i;

    si (!v)
    {
        *status = PLIST_ERROR_STRUCTURA;
        redde NIHIL;
    }
    commutatio (v->genus)
    {
        casus PLIST_CHORDA:
            si (_signum_vetitum(v->textus))
            {
                *status = PLIST_ERROR_SIGNUM;
                redde NIHIL;
            }
            nodus = xml_elementum_creare(piscina, intern, "string");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(v->textus, piscina));
            }
            redde nodus;

        casus PLIST_VERITAS:
            redde xml_elementum_creare(piscina, intern,
                v->veritas ? "true" : "false");

        casus PLIST_INTEGER:
            nodus = xml_elementum_creare(piscina, intern, "integer");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        chorda_ex_s64(v->integer, piscina), piscina));
            }
            redde nodus;

        casus PLIST_REALIS:
            nodus = xml_elementum_creare(piscina, intern, "real");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        chorda_ex_f64_exacta(v->realis, piscina),
                        piscina));
            }
            redde nodus;

        casus PLIST_DIES:
        {
            chorda textus = fasti_ad_iso(v->dies, piscina);

            si (chorda_vacua(textus))
            {
                *status = PLIST_ERROR_DIES;
                redde NIHIL;
            }
            nodus = xml_elementum_creare(piscina, intern, "date");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(textus, piscina));
            }
            redde nodus;
        }

        casus PLIST_DATA:
            nodus = xml_elementum_creare(piscina, intern, "data");
            si (nodus)
            {
                xml_textum_addere(nodus, piscina, intern,
                    chorda_ut_cstr(
                        base64_codificare(v->textus.datum,
                                          v->textus.mensura, piscina),
                        piscina));
            }
            redde nodus;

        casus PLIST_LISTA:
            nodus = xml_elementum_creare(piscina, intern, "array");
            si (!nodus)
            {
                *status = PLIST_ERROR_MEMORIA;
                redde NIHIL;
            }
            per (i = ZEPHYRUM; i < plist_numerus(v); i++)
            {
                XmlNodus* liberum = _nodum_ex_valore(
                    plist_ad_indicem(v, i), piscina, intern, status);

                si (!liberum)
                {
                    redde NIHIL;
                }
                xml_liberum_addere(nodus, liberum);
            }
            redde nodus;

        casus PLIST_DICTIO:
            nodus = xml_elementum_creare(piscina, intern, "dict");
            si (!nodus)
            {
                *status = PLIST_ERROR_MEMORIA;
                redde NIHIL;
            }
            per (i = ZEPHYRUM; i < plist_numerus(v); i++)
            {
                PlistPar* par;
                XmlNodus* clavis;
                XmlNodus* liberum;

                par = (PlistPar*)xar_obtinere(v->liberi, i);
                si (!par)
                {
                    *status = PLIST_ERROR_STRUCTURA;
                    redde NIHIL;
                }
                si (_signum_vetitum(par->clavis))
                {
                    *status = PLIST_ERROR_SIGNUM;
                    redde NIHIL;
                }
                clavis = xml_elementum_creare(piscina, intern, "key");
                si (!clavis)
                {
                    *status = PLIST_ERROR_MEMORIA;
                    redde NIHIL;
                }
                xml_textum_addere(clavis, piscina, intern,
                    chorda_ut_cstr(par->clavis, piscina));
                xml_liberum_addere(nodus, clavis);

                liberum = _nodum_ex_valore(par->valor, piscina, intern,
                                           status);
                si (!liberum)
                {
                    redde NIHIL;
                }
                xml_liberum_addere(nodus, liberum);
            }
            redde nodus;

        ordinarius:
            *status = PLIST_ERROR_STRUCTURA;
            redde NIHIL;
    }
}

chorda
plist_scribere (
    PlistValor*  radix,
    PlistStatus* status,
       Piscina*  piscina)
{
                 chorda  vacua;
                 chorda  corpus;
              XmlNodus*  nodus_plist;
              XmlNodus*  nodus_radix;
    InternamentumChorda*  intern;
      ChordaAedificator*  aed;
            PlistStatus  interior_status = PLIST_SUCCESSUS;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status)
    {
        redde vacua;
    }
    *status = PLIST_SUCCESSUS;
    si (!radix || !piscina)
    {
        *status = PLIST_ERROR_STRUCTURA;
        redde vacua;
    }
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    nodus_plist = xml_elementum_creare(piscina, intern, "plist");
    si (!nodus_plist
        || !xml_attributum_addere(nodus_plist, piscina, intern,
                                  "version", "1.0"))
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    nodus_radix = _nodum_ex_valore(radix, piscina, intern,
                                   &interior_status);
    si (!nodus_radix)
    {
        *status = interior_status == PLIST_SUCCESSUS
                ? PLIST_ERROR_STRUCTURA : interior_status;
        redde vacua;
    }
    xml_liberum_addere(nodus_plist, nodus_radix);

    corpus = xml_scribere(nodus_plist, piscina, VERUM);
    si (chorda_vacua(corpus))
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    aed = chorda_aedificator_creare(piscina, corpus.mensura + CCLVI);
    si (!aed)
    {
        *status = PLIST_ERROR_MEMORIA;
        redde vacua;
    }
    chorda_aedificator_appendere_literis(aed, PLIST_PROLOGUS);
    chorda_aedificator_appendere_chorda(aed, corpus);
    chorda_aedificator_appendere_literis(aed, "\n");
    redde chorda_aedificator_finire(aed);
}

b32
plist_scribere_plagulam (
          PlistValor*  radix,
    constans character*  via,
         PlistStatus*  status,
            Piscina*  piscina)
{
    chorda textus = plist_scribere(radix, status, piscina);

    si (chorda_vacua(textus))
    {
        redde FALSUM;
    }
    redde filum_scribere(via, textus);
}
```

`plist_scribere_plagulam` needs `#include "filum.h"` at the top of
`lib/plist.c`. The builder calls are confirmed:
`chorda_aedificator_appendere_chorda(ChordaAedificator*, chorda)`
(`include/chorda_aedificator.h:75`), `_appendere_literis` (`:70`),
`_creare` (`:46`), `_finire` (`:232`).

- [ ] **Step 6: Run and watch the tests pass**

```bash
./compile_tests.sh plist > /tmp/t4.log 2>&1; echo "rc=$?"
grep -E 'Totalis|Praeteriti|Fracti|Conditio' /tmp/t4.log
```

- [ ] **Step 7: Prove G1 and G3 can go red (two plants)**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
# G1: ponere appends instead of replacing -> order and count break
print(silva.planta('lib/plist.c',
                   '            par->valor = valor;   /* IN LOCO: ordo servatur */\n            redde VERUM;',
                   '            (vacuum)par;',
                   'radix', 'plist'))
# G3: the control-character refusal stops refusing
print(silva.planta('lib/plist.c',
                   '            si (_signum_vetitum(v->textus))',
                   '            si (FALSUM)',
                   'radix', 'plist'))
PY
```

Both must go RED, then GREEN after revert.

- [ ] **Step 8: Glossary, format, commit**

```bash
./oratio/vocabula.sh -nova
```

Expected: the word `plist` reported NEW. Add to
`oratio/glossarium.stml`, in the TERMINI TECHNICI PERMISSI section,
beside the `nanoid` line:

```xml
  <vocabulum lemma="plist" lingua="anglica" classis="ignotum-permissum" contextus="latinus" nota="nomen formae Apple (tabula proprietatum), 2026-09-11"/>
```

```bash
./silva/formator.sh include/plist.h lib/plist.c \
    probationes/probatio_plist.c -scribere
./silva/formator.sh include/plist.h lib/plist.c \
    probationes/probatio_plist.c -vitia
./oratio/vocabula.sh -nova   # exspecta NOVA 0
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'plist: modellum valoris (octo genera, dictio ORDINATA) et '
    'SCRIPTOR purus (arbor XmlNodus -> xml_scribere, ergo escapatio '
    'uno loco; prologus + DOCTYPE praefixi; signum C0 RECUSATUM). '
    'Portae G1 et G3 cum culpis plantatis duabus. Glossarium: plist '
    '(plist-spec par. III, V)',
    ['include/plist.h', 'lib/plist.c', 'probationes/probatio_plist.c',
     'oratio/glossarium.stml', 'compile_tests_fontes.sh'],
    portae=[('radix', 'plist'), ('oratio', 'vocabula')]))
PY
```

If `./tools/compile_tests_fontes_generare.sh` wrote a different
generated file than `compile_tests_fontes.sh`, commit the path it
actually changed — `git status --short` names it.

---

### Task 5: `plist` — the reader, its refusals, and the round trip

Implements spec §4; gates G2, G4 and the reader half of G7.

**Files:**
- Modify: `include/plist.h` (reading surface), `lib/plist.c`,
  `probationes/probatio_plist.c`

**Interfaces:**
- Consumes: Task 1's `chorda_ut_s64` / `ut_f64`; Task 2's DOCTYPE skip;
  Task 3's `fasti_ex_iso`; Task 4's constructors and `plist_aequalis`.
- Produces: `PlistResultus { PlistStatus status; PlistValor* radix;
  chorda semita; chorda causa; }`, `plist_legere(chorda, Piscina*,
  InternamentumChorda*)`, `plist_legere_plagulam(constans character*,
  Piscina*, InternamentumChorda*)`.

- [ ] **Step 1: Write the failing tests**

Insert before `imprimere("\n");` near the end of
`probationes/probatio_plist.c`:

```c
    /* ---- G2: lector, unum genus per casum ---- */
    {
        PlistResultus r;
        PlistValor*   v;
        constans character* textus =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" "
            "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\"><dict>\n"
            "  <key>S</key><string>a &lt; b</string>\n"
            "  <key>I</key><integer>-42</integer>\n"
            "  <key>R</key><real>0.5</real>\n"
            "  <key>V</key><true/>\n"
            "  <key>D</key><date>2026-09-11T02:41:32Z</date>\n"
            "  <key>B</key><data>AAH/</data>\n"
            "  <key>L</key><array><integer>1</integer>"
            "<integer>2</integer></array>\n"
            "</dict></plist>\n";

        imprimere("\n--- G2: lector ---\n");

        r = plist_legere(chorda_ex_literis(textus, piscina), piscina,
                         intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(r.radix);
        CREDO_AEQUALIS_I32(plist_numerus(r.radix), VII);

        v = plist_dictio_capere(r.radix, "S");
        CREDO_NON_NIHIL(v);
        CREDO_AEQUALIS_I32((i32)v->genus, (i32)PLIST_CHORDA);
        CREDO_CHORDA_AEQUALIS_LITERIS(v->textus, "a < b");

        v = plist_dictio_capere(r.radix, "I");
        CREDO_AEQUALIS_S64(v->integer, -42);
        v = plist_dictio_capere(r.radix, "R");
        CREDO_VERUM(v->realis == 0.5);
        v = plist_dictio_capere(r.radix, "V");
        CREDO_VERUM(v->veritas);
        v = plist_dictio_capere(r.radix, "D");
        CREDO_AEQUALIS_S32(v->dies.dies.annus, 2026);
        CREDO_AEQUALIS_S32(v->dies.hora.minutum, 41);
        v = plist_dictio_capere(r.radix, "B");
        CREDO_AEQUALIS_I32(v->textus.mensura, III);
        CREDO_AEQUALIS_I32((i32)(i8)v->textus.datum[II], 255);
        v = plist_dictio_capere(r.radix, "L");
        CREDO_AEQUALIS_I32(plist_numerus(v), II);
        CREDO_AEQUALIS_S64(plist_ad_indicem(v, I)->integer, 2);
    }

    /* ---- G2: recusationes, singulae per STATUM SUUM ---- */
    {
        PlistResultus r;
        i32           i;
        constans character* mala[VIII];
        PlistStatus   exspectati[VIII];

        imprimere("\n--- G2: recusationes nominatae ---\n");

        mala[0]       = "<dict/>";
        exspectati[0] = PLIST_ERROR_NON_PLIST;
        mala[1]       = "<plist version=\"2.0\"><dict/></plist>";
        exspectati[1] = PLIST_ERROR_VERSIO;
        mala[2]       = "bplist00\001\002";
        exspectati[2] = PLIST_ERROR_BINARIUM;
        mala[3]       = "<plist version=\"1.0\"><dict>"
                        "<key>a</key></dict></plist>";
        exspectati[3] = PLIST_ERROR_STRUCTURA;
        mala[4]       = "<plist version=\"1.0\"><dict>"
                        "<string>sine clave</string></dict></plist>";
        exspectati[4] = PLIST_ERROR_STRUCTURA;
        mala[5]       = "<plist version=\"1.0\"><ignotum/></plist>";
        exspectati[5] = PLIST_ERROR_STRUCTURA;
        mala[6]       = "<plist version=\"1.0\">"
                        "<data>!!!not base64!!!</data></plist>";
        exspectati[6] = PLIST_ERROR_BASE64;
        mala[7]       = "<plist version=\"1.0\">"
                        "<integer>99999999999999999999</integer></plist>";
        exspectati[7] = PLIST_ERROR_NUMERUS;

        per (i = ZEPHYRUM; i < VIII; i++)
        {
            r = plist_legere(chorda_ex_literis(mala[i], piscina),
                             piscina, intern);
            CREDO_AEQUALIS_I32((i32)r.status, (i32)exspectati[i]);
            CREDO_NIHIL(r.radix);
            CREDO_CHORDA_NON_VACUA(r.semita);
        }

        /* dies mala et plures radices */
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><date>heri</date></plist>",
            piscina), piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_DIES);
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><dict/><dict/></plist>", piscina),
            piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_STRUCTURA);
        /* textus non albus intra continentem */
        r = plist_legere(chorda_ex_literis(
            "<plist version=\"1.0\"><dict>sordes<key>a</key>"
            "<string>b</string></dict></plist>", piscina), piscina,
            intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_STRUCTURA);
    }

    /* ---- G4: iter reditus nostrum (valor -> octeti -> valor) ---- */
    {
        PlistValor*   d;
        PlistValor*   lista;
        PlistStatus   status;
        PlistResultus r;
        chorda        octeti;
        DiesHora      dh;
        i8            data[II];

        imprimere("\n--- G4: iter reditus ---\n");

        dh.dies   = fasti_dies(1978, 2, 3);
        dh.hora   = fasti_hora(23, 59, 58);
        data[0]   = (i8)0x10;
        data[1]   = (i8)0x20;

        lista = plist_lista_creare(piscina);
        plist_lista_addere(lista,
            plist_realem_creare(1e-20, piscina), piscina);
        plist_lista_addere(lista,
            plist_data_creare(chorda_ex_buffer(data, II), piscina),
            piscina);

        d = plist_dictio_creare(piscina);
        plist_dictio_ponere(d, chorda_ex_literis("a", piscina),
            plist_chordam_creare(
                chorda_ex_literis("&<>\"'", piscina), piscina),
            piscina);
        plist_dictio_ponere(d, chorda_ex_literis("b", piscina),
            plist_integrum_creare(9223372036854775807LL, piscina),
            piscina);
        plist_dictio_ponere(d, chorda_ex_literis("c", piscina),
            plist_diem_creare(dh, piscina), piscina);
        plist_dictio_ponere(d, chorda_ex_literis("d", piscina),
            lista, piscina);

        octeti = plist_scribere(d, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        r = plist_legere(octeti, piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        CREDO_VERUM(plist_aequalis(d, r.radix));

        /* aequalitas ORDINEM curat: eaedem claves, ordo inversus */
        {
            PlistValor* e = plist_dictio_creare(piscina);

            plist_dictio_ponere(e, chorda_ex_literis("y", piscina),
                plist_integrum_creare(2, piscina), piscina);
            plist_dictio_ponere(e, chorda_ex_literis("x", piscina),
                plist_integrum_creare(1, piscina), piscina);
            {
                PlistValor* f = plist_dictio_creare(piscina);

                plist_dictio_ponere(f, chorda_ex_literis("x", piscina),
                    plist_integrum_creare(1, piscina), piscina);
                plist_dictio_ponere(f, chorda_ex_literis("y", piscina),
                    plist_integrum_creare(2, piscina), piscina);
                CREDO_FALSUM(plist_aequalis(e, f));
            }
        }
    }
```

- [ ] **Step 2: Run and watch it fail**

```bash
./compile_tests.sh plist > /tmp/t5.log 2>&1; echo "rc=$?"
grep -E 'error:|Fracti' /tmp/t5.log | head
```

Expected: compile failure on `PlistResultus` / `plist_legere`.

- [ ] **Step 3: Declare the reading surface**

Add to `include/plist.h` before `#endif`:

```c
/* ---- legere ---- */

/* Recusationes SEDEM nominant, non lineam: XmlNodus positionem non
 * fert (include/xml.h:68). semita = "plist/dict/key[3]". */
nomen structura {
    PlistStatus  status;
    PlistValor*  radix;    /* NIHIL nisi PLIST_SUCCESSUS */
         chorda  semita;
         chorda  causa;
} PlistResultus;

PlistResultus
plist_legere (
                 chorda  textus,
               Piscina*  piscina,
    InternamentumChorda*  intern);

PlistResultus
plist_legere_plagulam (
    constans character*  via,
              Piscina*  piscina,
    InternamentumChorda*  intern);
```

- [ ] **Step 4: Implement the reader**

Append to `lib/plist.c`. The walk carries a `semita` builder so every
refusal names its place; whitespace-only text nodes are skipped and
any other text inside a container is `PLIST_ERROR_STRUCTURA`.

```c
interior b32
_textus_albus (
    chorda s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s.mensura; i++)
    {
        i8 c = s.datum[i];

        si (c != (i8)' ' && c != (i8)'\t' && c != (i8)'\n'
            && c != (i8)'\r')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* liberum proximum quod ELEMENTUM est; textum album praeterit,
 * textum aliud RECUSAT */
interior XmlNodus*
_elementum_proximum (
    XmlNodus*  parens,
          i32* index,
          b32* vitium)
{
    *vitium = FALSUM;
    dum (*index < xml_numerus_liberorum(parens))
    {
        XmlNodus* liberum = xml_liberum_ad_indicem(parens, *index);

        (*index)++;
        si (!liberum)
        {
            perge;
        }
        si (liberum->genus == XML_NODUS_ELEMENTUM)
        {
            redde liberum;
        }
        si (liberum->genus == XML_NODUS_TEXTUS && liberum->valor
            && !_textus_albus(*liberum->valor))
        {
            *vitium = VERUM;
            redde NIHIL;
        }
    }
    redde NIHIL;
}
```

Then `_valorem_ex_nodo(XmlNodus*, chorda semita, Piscina*,
InternamentumChorda*, PlistResultus*)`, which switches on the element
name: `string` → `plist_chordam_creare(xml_textus_internus(...))`;
`integer` → `chorda_ut_s64` else `PLIST_ERROR_NUMERUS`; `real` →
`chorda_ut_f64` else `PLIST_ERROR_NUMERUS`; `true`/`false` →
`plist_veritatem_creare`; `date` → `fasti_ex_iso` else
`PLIST_ERROR_DIES`; `data` → `base64_decodificare`, `datum == NIHIL` →
`PLIST_ERROR_BASE64`; `array` → loop `_elementum_proximum`, appending;
`dict` → loop pairs, requiring `key` then a value element (either
missing → `PLIST_ERROR_STRUCTURA`); any other name →
`PLIST_ERROR_STRUCTURA` with the name in `causa`.

And `plist_legere` itself:

```c
PlistResultus
plist_legere (
                 chorda  textus,
               Piscina*  piscina,
    InternamentumChorda*  intern)
{
    PlistResultus  r;
      XmlResultus  xr;
        XmlNodus*  radix_xml;
           chorda* versio;
              i32  index = ZEPHYRUM;
              b32  vitium;

    r.status          = PLIST_SUCCESSUS;
    r.radix           = NIHIL;
    r.semita          = chorda_ex_literis("plist", piscina);
    r.causa.datum     = NIHIL;
    r.causa.mensura   = ZEPHYRUM;

    si (!piscina || !intern || chorda_vacua(textus))
    {
        r.status  = PLIST_ERROR_STRUCTURA;
        r.causa   = chorda_ex_literis("input vacuum", piscina);
        redde r;
    }
    /* plist binaria: forma ALIA, recusatur ante omnia */
    si (textus.mensura >= VIII
        && memcmp(textus.datum, "bplist0", VII) == ZEPHYRUM)
    {
        r.status  = PLIST_ERROR_BINARIUM;
        r.causa   = chorda_ex_literis(
            "bplist00 - forma binaria non tractatur", piscina);
        redde r;
    }
    xr = xml_legere(textus, piscina, intern);
    si (!xr.successus || !xr.radix)
    {
        r.status  = PLIST_ERROR_XML;
        r.causa   = chorda_ex_s32((s32)xr.status, piscina);
        redde r;
    }
    radix_xml = xr.radix;
    si (!radix_xml->titulus
        || !chorda_aequalis_literis(*radix_xml->titulus, "plist"))
    {
        r.status  = PLIST_ERROR_NON_PLIST;
        r.causa   = chorda_ex_literis("radix non est plist", piscina);
        redde r;
    }
    versio = xml_attributum_capere(radix_xml, "version");
    si (versio && !chorda_aequalis_literis(*versio, "1.0"))
    {
        r.status  = PLIST_ERROR_VERSIO;
        r.causa   = *versio;
        redde r;
    }
    {
        XmlNodus* unicum = _elementum_proximum(radix_xml, &index,
                                               &vitium);
        XmlNodus* superfluum;

        si (vitium || !unicum)
        {
            r.status  = PLIST_ERROR_STRUCTURA;
            r.causa   = chorda_ex_literis(
                "plist unum liberum poscit", piscina);
            redde r;
        }
        superfluum = _elementum_proximum(radix_xml, &index, &vitium);
        si (superfluum || vitium)
        {
            r.status  = PLIST_ERROR_STRUCTURA;
            r.causa   = chorda_ex_literis(
                "plist plus uno libero fert", piscina);
            redde r;
        }
        r.radix = _valorem_ex_nodo(unicum, r.semita, piscina, intern,
                                   &r);
    }
    si (r.status != PLIST_SUCCESSUS)
    {
        r.radix = NIHIL;
    }
    redde r;
}

PlistResultus
plist_legere_plagulam (
    constans character*  via,
              Piscina*  piscina,
    InternamentumChorda*  intern)
{
    redde plist_legere(filum_legere_totum(via, piscina), piscina,
                       intern);
}
```

- [ ] **Step 5: Run and watch it pass**

```bash
./compile_tests.sh plist > /tmp/t5.log 2>&1; echo "rc=$?"
grep -E 'Totalis|Praeteriti|Fracti|Conditio' /tmp/t5.log
```

- [ ] **Step 6: Prove the reader's gates can go red (two plants)**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
# the single-child rule stops firing
print(silva.planta('lib/plist.c',
                   '        si (superfluum || vitium)',
                   '        si (FALSUM)',
                   'radix', 'plist'))
# non-whitespace text inside a container stops being refused
print(silva.planta('lib/plist.c',
                   '            *vitium = VERUM;',
                   '            *vitium = FALSUM;',
                   'radix', 'plist'))
PY
```

- [ ] **Step 7: Format and commit**

```bash
./silva/formator.sh include/plist.h lib/plist.c \
    probationes/probatio_plist.c -scribere
./silva/formator.sh include/plist.h lib/plist.c \
    probationes/probatio_plist.c -vitia
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'plist: LECTOR (semita pro linea, decem status recusationis '
    'singuli probati, textus albus praeteritus et alius recusatus, '
    'unum liberum radicis) + iter reditus nostrum per plist_aequalis. '
    'Portae G2 et G4, culpae plantatae duae (plist-spec par. IV)',
    ['include/plist.h', 'lib/plist.c', 'probationes/probatio_plist.c'],
    portae=[('radix', 'plist')]))
PY
```

---

### Task 6: The foreign oracle — Apple's fixture and `plutil`

Implements gates G5 and G6. Last, because it judges finished output.

**Files:**
- Create: `probationes/fixa/plist/apple_info.plist` (copied, not
  hand-written)
- Create: `probationes/probatio_plist_plutil.sh`
- Modify: `probationes/probatio_plist.c` (G5), `compile_tests.sh`
  (register G6 before `return 0`, `:652`)

**Interfaces:**
- Consumes: Task 5's `plist_legere_plagulam`, Task 4's
  `plist_scribere_plagulam`.
- Produces: no C surface; two gates.

- [ ] **Step 1: Copy Apple's own plist in as a fixture**

```bash
mkdir -p probationes/fixa/plist
cp knotapel/demo_85_indecomposability/demo_85.dSYM/Contents/Info.plist \
   probationes/fixa/plist/apple_info.plist
wc -c probationes/fixa/plist/apple_info.plist   # exspecta 636
grep -c DOCTYPE probationes/fixa/plist/apple_info.plist  # exspecta 1
```

The fixture is copied rather than referenced so the test never reaches
into `knotapel/`, and it is Apple-written so it carries a real DOCTYPE
— the reason Task 2 exists.

- [ ] **Step 2: Write G5 (their round trip) as a failing test**

Insert before `imprimere("\n");` in `probationes/probatio_plist.c`:

```c
    /* ---- G5: iter reditus ALIENUM (Info.plist ab Apple scriptum) --- */
    {
        PlistResultus prima;
        PlistResultus secunda;
        PlistStatus   status;
        chorda        nostra;
        PlistValor*   v;

        imprimere("\n--- G5: iter reditus alienum ---\n");

        prima = plist_legere_plagulam(
            "probationes/fixa/plist/apple_info.plist", piscina, intern);
        CREDO_AEQUALIS_I32((i32)prima.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(prima.radix);
        CREDO_AEQUALIS_I32((i32)prima.radix->genus, (i32)PLIST_DICTIO);

        /* claves quas dSYM semper fert */
        v = plist_dictio_capere(prima.radix, "CFBundleIdentifier");
        CREDO_NON_NIHIL(v);
        CREDO_AEQUALIS_I32((i32)v->genus, (i32)PLIST_CHORDA);
        CREDO_CHORDA_NON_VACUA(v->textus);

        /* nostra scriptio, deinde lectio: valores IDEM */
        nostra = plist_scribere(prima.radix, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        secunda = plist_legere(nostra, piscina, intern);
        CREDO_AEQUALIS_I32((i32)secunda.status, (i32)PLIST_SUCCESSUS);
        CREDO_VERUM(plist_aequalis(prima.radix, secunda.radix));
    }
```

- [ ] **Step 3: Run it; fix what the real file exposes**

```bash
./compile_tests.sh plist > /tmp/t6.log 2>&1; echo "rc=$?"
grep -E 'FRACTA|Fracti|Conditio' /tmp/t6.log
```

A real Apple file is the first foreign input the reader meets. If it
refuses, read `r.status` and `r.causa` before changing anything: the
likely causes are a key the walk mishandles or whitespace the skip
misses, and the fix belongs in the reader, not in the fixture. Never
edit the fixture to make the test pass.

- [ ] **Step 4: Write the `plutil` gate, control first**

```bash
cat > probationes/probatio_plist_plutil.sh <<'EOF'
#!/bin/bash
# probatio_plist_plutil.sh - ORACULUM ALIENUM: plutil Apple plists
# nostras iudicat (exemplar: probationes/probatio_qr_gyrus.sh).
#
# CONTROLLUM PRIMUM: plist ab APPLE scripta linteatur, ut oraculum
# ipsum probetur ante nostra. Si controllum cadit, instrumentum
# fractum est, non codex noster.
#
# CALIBRATIO NOMINATA (plist-spec par. VII): plutil plist SINE
# DOCTYPE accipit et indentationem ignorat - ergo silentium eius
# neutrum probat; ea probatio_plist.c asserit (G3).
#
# Exitus: 0 factum; 1 defectio; 2 NIHIL CURSUM.

set -u
RADIX="$(cd "$(dirname "$0")/.." && pwd)"
cd "$RADIX" || exit 2

FIXUM=probationes/fixa/plist/apple_info.plist
AREA="$(mktemp -d /tmp/probatio_plist_plutil.XXXXXX)" || exit 2
purgare () { rm -rf "$AREA"; }

[ -x /usr/bin/plutil ] || { echo "plutil abest" >&2; purgare; exit 2; }
[ -f "$FIXUM" ] || { echo "fixum abest: $FIXUM" >&2; purgare; exit 2; }
[ -x build/probationes/probatio_plist ] || {
    echo "probatio_plist non aedificata - ./compile_tests.sh plist prius" >&2
    purgare; exit 2; }

# I. CONTROLLUM: oraculum plist veram accipit
if ! /usr/bin/plutil -lint "$FIXUM" > "$AREA/controllum.log" 2>&1; then
    echo "CONTROLLUM FRACTUM: plutil plist Apple recusavit" >&2
    cat "$AREA/controllum.log" >&2
    purgare; exit 1
fi

# II. nostra scriptio: probatio_plist eam in $AREA ponit
if ! PLIST_AREA="$AREA" ./build/probationes/probatio_plist \
        > "$AREA/probatio.log" 2>&1; then
    echo "probatio_plist fracta" >&2
    tail -20 "$AREA/probatio.log" >&2
    purgare; exit 1
fi
if [ ! -f "$AREA/nostra.plist" ]; then
    echo "NIHIL CURSUM: probatio_plist nihil in PLIST_AREA scripsit" >&2
    purgare; exit 2
fi

# III. plutil nostram linteat
if ! /usr/bin/plutil -lint "$AREA/nostra.plist" \
        > "$AREA/nostra.log" 2>&1; then
    echo "plutil plist NOSTRAM recusavit:" >&2
    cat "$AREA/nostra.log" >&2
    purgare; exit 1
fi

# IV. utraque ad formam canonicam versa, deinde conferre
/usr/bin/plutil -convert xml1 -o "$AREA/fixum.xml1" "$FIXUM" || {
    echo "conversio fixi fracta" >&2; purgare; exit 1; }
/usr/bin/plutil -convert xml1 -o "$AREA/nostra.xml1" \
    "$AREA/nostra.plist" || {
    echo "conversio nostrae fracta" >&2; purgare; exit 1; }
if ! diff -u "$AREA/fixum.xml1" "$AREA/nostra.xml1" \
        > "$AREA/differentia" 2>&1; then
    echo "plutil formae canonicae DISCREPANT:" >&2
    head -40 "$AREA/differentia" >&2
    purgare; exit 1
fi

echo "plist plutil: controllum + nostra linteata, formae canonicae congruunt"
purgare
exit 0
EOF
chmod +x probationes/probatio_plist_plutil.sh
```

- [ ] **Step 5: Make `probatio_plist` write the file the gate reads**

Add to `probationes/probatio_plist.c`, inside the G5 block after
`nostra` is built — the gate needs our bytes for the same Apple input:

```c
        /* pro porta plutil (probatio_plist_plutil.sh): si PLIST_AREA
         * in ambitu est, scriptionem nostram illuc pone */
        {
            constans character* area = getenv("PLIST_AREA");

            si (area != NIHIL)
            {
                character via[CCLVI];

                sprintf(via, "%.200s/nostra.plist", area);
                CREDO_VERUM(plist_scribere_plagulam(prima.radix, via,
                                                    &status, piscina));
                CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
            }
        }
```

`getenv` needs `#include <stdlib.h>` in the test file.

- [ ] **Step 6: Register the gate in the runner**

In `compile_tests.sh`, insert before `return 0` (`:652`), modeled on
the qr block above it:

```bash
    # Oraculum plist: plutil (instrumentum Apple) plists nostras
    # iudicat. probatio_plist.c VALORES probat; hoc solum probat quod
    # Apple ipsa octetos nostros accipit.
    if [ -x "probationes/probatio_plist_plutil.sh" ]; then
        if ! ./probationes/probatio_plist_plutil.sh > /dev/null 2>&1; then
            echo -e "${RED}✗ plist plutil fractus (curre: ./probationes/probatio_plist_plutil.sh)${RESET}"
            return 1
        fi
        echo -e "${BLUE}plist plutil: octeti ab Apple accepti${RESET}"
    fi
```

- [ ] **Step 7: Run both gates**

```bash
./probationes/probatio_plist_plutil.sh; echo "rc=$?"
./compile_tests.sh plist > /tmp/t6.log 2>&1; echo "rc=$?"
grep -E 'plutil|Fracti|Conditio' /tmp/t6.log
```

Expected: the shell gate prints its success line and `rc=0`; the suite
stays green.

- [ ] **Step 8: Prove G6 can go red, and that its control works**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
# emit a broken prolog: plutil must refuse OUR file while the control passes
print(silva.planta('lib/plist.c',
                   '    "<?xml version=\\"1.0\\" encoding=\\"UTF-8\\"?>\\n"',
                   '    "<?xml version=\\"1.0\\" encoding=\\"UTF-8\\"?>\\n<junk>\\n"',
                   'radix', 'plist'))
PY
```

Then prove the CONTROL is load-bearing by hand: temporarily point
`FIXUM` at a non-plist file (`README.md`) and check the gate exits 1
with `CONTROLLUM FRACTUM`, then restore it. A control that cannot fail
is decoration.

- [ ] **Step 9: Commit**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'plist: ORACULUM ALIENUM - Info.plist ab Apple scripta in fixa '
    '(DOCTYPE verum, 636 octeti), iter reditus alienum (G5) et porta '
    'plutil cum CONTROLLO primo (G6, in compile_tests.sh iuxta qr). '
    'Calibratio nominata: plutil DOCTYPE absentem et indentationem '
    'ignoscit, ergo G3 utrumque asserit (plist-spec par. VII)',
    ['probationes/fixa/plist/apple_info.plist',
     'probationes/probatio_plist_plutil.sh',
     'probationes/probatio_plist.c', 'compile_tests.sh'],
    portae=[('radix', 'plist')]))
PY
```

---

### Task 7: Seal — worklog, spec as-built, whole suite

**Files:**
- Create: `lib/plist.worklog.md`
- Modify: `project-specs/plist-spec.md` (as-built notes, the two
  amendments, AUDIENDA closed)

- [ ] **Step 1: Write `lib/plist.worklog.md`**

Record, in English: what the five home-library fixes actually were and
which were found by reading rather than by failing; the writer's reuse
of `xml_scribere` for escaping (one place, not two); why the dict is
ordered; that `plist_scribere` grew a `PlistStatus*` the spec did not
have; anything the Apple fixture exposed in Task 6 Step 3 (the most
valuable paragraph in the file — a real foreign input meeting a fresh
reader); and the `plutil` calibration (what it forgives).

- [ ] **Step 2: Update the spec's as-built notes**

Amend `project-specs/plist-spec.md`:
- §5: `plist_scribere` takes `PlistStatus*` (amendment (a)).
- §6.1: name `XML_ERROR_SUBSETUM_INTERNUM` (amendment (b)).
- §10: close the `chorda_ut_i32` AUDIENDUM — it shares the NUL flaw and
  additionally accepted `-1` as 4294967295 through `strtoul`; both
  fixed in Task 1. Close or keep the remaining AUDIENDA honestly, based
  on what the work actually touched.

- [ ] **Step 3: Run the WHOLE root suite, not a filter**

```bash
./compile_tests.sh > /tmp/omnia.log 2>&1; echo "rc=$?"
grep -E 'Tests (Total|Passed|Failed)|plutil|PROBATIONES' /tmp/omnia.log
```

`rc=2` means nothing ran. Anything but `Failed: 0` stops the seal.

- [ ] **Step 4: Lint and format everything touched**

```bash
./oratio/vocabula.sh -nova                     # exspecta NOVA 0
./silva/formator.sh include/plist.h lib/plist.c \
    probationes/probatio_plist.c include/chorda.h lib/chorda.c \
    include/fasti.h lib/fasti.c include/xml.h lib/xml.c -vitia
```

- [ ] **Step 5: Commit the seal**

```bash
python3 - <<'PY'
import sys; sys.path.insert(0, 'pythonica'); import silva
print(silva.commissio(
    'plist: SIGILLUM plani I - worklog, spec ut-aedificata (duae '
    'emendationes: plist_scribere statum reddit, '
    'XML_ERROR_SUBSETUM_INTERNUM nominatum), AUDIENDUM '
    'chorda_ut_i32 clausum. Suita radicis tota viridis.',
    ['lib/plist.worklog.md', 'project-specs/plist-spec.md'],
    portae=[('radix', None)]))
PY
```

---

## Self-review (done while writing; the executor re-runs it at the end)

**Spec coverage.** §3 → Task 4 Steps 1, 4. §4 → Task 5. §5 → Task 4
Steps 5. §6.1 → Task 2. §6.2, §6.3, §6.4 → Task 1. §6.5 → Task 3.
§6.6 needs no work by definition. §6.7 → Task 4 (`_signum_vetitum`,
the write half) and spec §9 (the read half stays PARCATUM). §7 G1, G3
→ Task 4; G2, G4 → Task 5; G5, G6 → Task 6; G7 → Task 1 (chorda half)
and Task 4 (control-character half). §8 P1–P7 → Tasks 1–7 in order.
§9 deferrals: nothing in this plan implements them, as intended.

**Placeholders.** None: every code step carries the code, every run
step carries its command and expected result. The one deliberately
prose-shaped step is Task 5 Step 4's per-element switch, where the
mapping table is spelled out element by element rather than written
twice; if that proves too loose in execution, write the switch from the
list in that step and the G2 table in Task 5 Step 1, which together
pin every branch.

**Type consistency.** `PlistStatus` is the status type everywhere
(`plist_scribere`, `PlistResultus.status`, both `_ex_nodo` helpers).
`plist_scribere` takes `(PlistValor*, PlistStatus*, Piscina*)` in the
header, in the tests, and in Task 6's file-writing block.
`plist_numerus` is used for both dict and list counts throughout.
`chorda_ex_s64` / `chorda_ut_s64` / `chorda_ex_f64_exacta` are spelled
identically in Task 1's header, implementation and tests, and in Task
4's writer.

**Both former assumptions were checked before this plan was
committed**, so the executor inherits facts, not risks:
1. `chorda_aedificator_appendere_chorda(ChordaAedificator*, chorda)`
   exists — `include/chorda_aedificator.h:75`.
2. `XmlNodusGenus` is `{ XML_NODUS_ELEMENTUM = I, XML_NODUS_TEXTUS =
   II, XML_NODUS_COMMENTUM = III }` — `include/xml.h:30-34`. Task 5
   Step 4's `_elementum_proximum` uses the first two exactly as
   written.

What stays genuinely open is **Task 6 Step 3**: a real Apple-written
plist meeting a fresh reader for the first time. That is the step most
likely to find something, and the plan's instruction there is the
important one — read `r.status` and `r.causa`, fix the READER, never
the fixture.
